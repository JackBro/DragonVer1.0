#include "mobidriver.h"


PMOBIUSB_PIPE_CONTEXT
MobiUsb_PipeWithName(
    IN PDEVICE_OBJECT  DeviceObject,
    IN PUNICODE_STRING FileName
    )
/*++
 
Routine Description:

    This routine will pass the string pipe name and
    fetch the pipe number.

Arguments:

    DeviceObject - pointer to DeviceObject
    FileName - string pipe name

Return Value:

    The device extension maintains a pipe context for 
    the pipes on 82930 board.
    This routine returns the pointer to this context in
    the device extension for the "FileName" pipe.

--*/
{
    LONG                  ix;
    ULONG                 uval; 
    ULONG                 nameLength;
    ULONG                 umultiplier;
    PDEVICE_EXTENSION     devExtension;
    PMOBIUSB_PIPE_CONTEXT pipeContext;

    //
    // initialize variables
    //
    pipeContext = NULL;
    //
    // typedef WCHAR *PWSTR;
    //
    nameLength = (FileName->Length / sizeof(WCHAR));
    devExtension = (PDEVICE_EXTENSION) DeviceObject->DeviceExtension;

    MobiUsb_DbgPrint(3, ("MobiUsb_PipeWithName: begins\n"));

    if(nameLength != 0) {
    
        MobiUsb_DbgPrint(3, ("MobiUsb_PipeWithName: Filename = %ws nameLength = %d\n", FileName->Buffer, nameLength));

        //
        // Parse the pipe#
        //
        ix = nameLength - 1;

        // if last char isn't digit, decrement it.
        while((ix > -1) &&
              ((FileName->Buffer[ix] < (WCHAR) '0')  ||
               (FileName->Buffer[ix] > (WCHAR) '9')))             {

            ix--;
        }

        if(ix > -1) {

            uval = 0;
            umultiplier = 1;

            // traversing least to most significant digits.

            while((ix > -1) &&
                  (FileName->Buffer[ix] >= (WCHAR) '0') &&
                  (FileName->Buffer[ix] <= (WCHAR) '9'))          {
        
                uval += (umultiplier *
                         (ULONG) (FileName->Buffer[ix] - (WCHAR) '0'));

                ix--;
                umultiplier *= 10;
            }

            if(uval < 6 && devExtension->PipeContext) {
        
                pipeContext = &devExtension->PipeContext[uval];
            }
        }
    }

    MobiUsb_DbgPrint(3, ("MobiUsb_PipeWithName: ends\n"));

    return pipeContext;
}

NTSTATUS
MobiBulk_DispatchReadWrite(
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP           Irp
    )
/*++
 
Routine Description:

    Dispatch routine for read and write.
    This routine creates a MOBIUSB_RW_CONTEXT for a read/write.
    This read/write is performed in stages of MOBIUSB_MAX_TRANSFER_SIZE.
    once a stage of transfer is complete, then the irp is circulated again, 
    until the requested length of tranfer is performed.

Arguments:

    DeviceObject - pointer to device object
    Irp - I/O request packet

Return Value:

    NT status value

--*/
{
    NTSTATUS               ntStatus;
    PFILE_OBJECT           fileObject;
    PDEVICE_EXTENSION      devExtension;
    PUSBD_PIPE_INFORMATION pipeInformation;
	PIO_STACK_LOCATION     irpStack;
	BOOLEAN				   Read;

    //
    // initialize variables
    //
    irpStack = IoGetCurrentIrpStackLocation(Irp);
    fileObject = irpStack->FileObject;
    Read = (irpStack->MajorFunction == IRP_MJ_READ) ? TRUE : FALSE;
    devExtension = (PDEVICE_EXTENSION) DeviceObject->DeviceExtension;

	MobiUsb_DbgPrint(4, ("MobiBulk_DispatchReadWrite: begins\n"));
    MobiUsb_IoIncrement(devExtension);

    if(devExtension->DeviceState != Working) {

        MobiUsb_DbgPrint(3, ("MobiBulk_DispatchReadWrite: Invalid device state\n"));

        ntStatus = STATUS_INVALID_DEVICE_STATE;
        goto MobiUsb_DispatchReadWrite_Exit;
    }
    MobiUsb_DbgPrint(4, ("MobiBulk_DispatchReadWrite: Waiting on the IdleReqPendEvent\n"));
    if(devExtension->SSEnable) {

        KeWaitForSingleObject(&devExtension->NoIdleReqPendEvent, 
                              Executive, 
                              KernelMode, 
                              FALSE, 
                              NULL);
    }

    if(fileObject && fileObject->FsContext) {

        pipeInformation = fileObject->FsContext;

        if((UsbdPipeTypeBulk != pipeInformation->PipeType) &&
           (UsbdPipeTypeInterrupt != pipeInformation->PipeType)) {
            
            MobiUsb_DbgPrint(3, ("MobiBulk_DispatchReadWrite: Usbd pipe type is not bulk or interrupt\n"));

            ntStatus = STATUS_INVALID_HANDLE;
            goto MobiUsb_DispatchReadWrite_Exit;
        }
    }
    else {

        MobiUsb_DbgPrint(3, ("MobiBulk_DispatchReadWrite: Invalid handle\n"));

        ntStatus = STATUS_INVALID_HANDLE;
        goto MobiUsb_DispatchReadWrite_Exit;
    }


	ntStatus = MobiBulkReadWrite(DeviceObject, Irp, pipeInformation, Read);
	if(ntStatus == STATUS_PENDING)
		return ntStatus;

MobiUsb_DispatchReadWrite_Exit:

    Irp->IoStatus.Status = ntStatus;
    Irp->IoStatus.Information = 0;

    IoCompleteRequest(Irp, IO_NO_INCREMENT);
    MobiUsb_IoDecrement(devExtension);

    MobiUsb_DbgPrint(3, ("MobiBulk_DispatchReadWrite: ends\n"));

    return ntStatus;
}


NTSTATUS
MobiBulkReadWrite(
	IN PDEVICE_OBJECT			DeviceObject,
	IN PIRP						Irp,
	IN PUSBD_PIPE_INFORMATION	PipeInformation,
 	IN BOOLEAN					Read
   )
{
	NTSTATUS				ntStatus;
	PBULKRW_CONTEXT			rwContext;		
	PDEVICE_EXTENSION		devExtension;
	KIRQL					oldIrql;
	ULONG					i;


	MobiUsb_DbgPrint(3, ("MobiBulkReadWrite: beg\n"));

    devExtension = (PDEVICE_EXTENSION) DeviceObject->DeviceExtension;

	ntStatus = MobiBulkInitialize(DeviceObject, PipeInformation, Irp, Read, &rwContext);
	if(ntStatus != STATUS_SUCCESS)
		return ntStatus;

    IoSetCancelRoutine(Irp, MobiBulkCancelReadWrite);
    if(Irp->Cancel) {

        ntStatus = STATUS_CANCELLED;
		if (IoSetCancelRoutine(Irp, NULL))
			--rwContext->Lock;
		else
			ntStatus = STATUS_SUCCESS;
	}

	IoSetCompletionRoutine(Irp, (PIO_COMPLETION_ROUTINE) MobiBulkReadWriteComplete,
		(PVOID) rwContext, TRUE, TRUE, TRUE);
	IoMarkIrpPending(Irp);
	IoSetNextIrpStackLocation(Irp);

	if (!NT_SUCCESS(ntStatus))
	{
#ifdef MOBI_DIRECT_IO
		if(rwContext->SubMdl)
			IoFreeMdl(rwContext->SubMdl);
#endif
		Irp->IoStatus.Status = ntStatus;
		IoCompleteRequest(Irp, IO_NO_INCREMENT);
		return STATUS_PENDING;
	}

	ntStatus = IoCallDriver(devExtension->TopOfStackDeviceObject, rwContext->SubIrp);
	if(!NT_SUCCESS(ntStatus))
		MobiUsb_DbgPrint(1, ("IoCallDriver: status =============== %x\n", ntStatus));



	return STATUS_PENDING;
}

NTSTATUS
MobiBulkInitialize(
	IN PDEVICE_OBJECT			DeviceObject,
	IN PUSBD_PIPE_INFORMATION	pipeInformation,
	IN PIRP						Irp,
	IN BOOLEAN					Read,
	IN OUT PBULKRW_CONTEXT*		pRwContext
    )
{
    PIO_STACK_LOCATION		irpStack;
    PIO_STACK_LOCATION		nextStack;
    ULONG					totalLength;
    ULONG					stageSize;
	PUCHAR					virtualAddress;
	PBULKRW_CONTEXT			rwContext;
	PDEVICE_EXTENSION		devExtension;

	devExtension = (PDEVICE_EXTENSION) DeviceObject->DeviceExtension;
#ifdef MOBI_DIRECT_IO
	if(Irp->MdlAddress) 
		totalLength = MmGetMdlByteCount(Irp->MdlAddress);
	else
		totalLength = 0;
#else
	irpStack = IoGetCurrentIrpStackLocation(Irp);
	if(Read)
	{
		totalLength = irpStack->Parameters.Read.Length;
	}
	else
	{
		totalLength = irpStack->Parameters.Write.Length;
	}
#endif
	if(totalLength == 0)
	{
		MobiUsb_DbgPrint(3, ("MobiBulkReadWrite : totalLength size is zero\n"));
        return STATUS_INVALID_PARAMETER;
	}
#ifdef MOBI_DIRECT_IO
    virtualAddress = (PUCHAR) MmGetMdlVirtualAddress(Irp->MdlAddress);
#else
	virtualAddress = (PUCHAR) Irp->AssociatedIrp.SystemBuffer;
#endif
	if(totalLength > MOBIUSB_TEST_BOARD_TRANSFER_BUFFER_SIZE)
		totalLength = MOBIUSB_TEST_BOARD_TRANSFER_BUFFER_SIZE;
	stageSize = MOBIUSB_MAX_TRANSFER_SIZE;
	if(stageSize > totalLength)
		stageSize = totalLength;

	*pRwContext = (PBULKRW_CONTEXT)ExAllocatePool(NonPagedPool, sizeof(BULKRW_CONTEXT));
	rwContext = *pRwContext;
	if(rwContext == NULL) {
		MobiUsb_DbgPrint(3, ("MobiUsb_MobiInitialize : ExAllocatePool (rwContext) failed\n"));
        return STATUS_INSUFFICIENT_RESOURCES;
	}
	RtlZeroMemory(rwContext, sizeof(BULKRW_CONTEXT));
	rwContext->MainIrp = Irp;
    rwContext->Lock = 2;
	rwContext->DeviceExtension = devExtension;
	rwContext->pipeInformation = pipeInformation;
    Irp->Tail.Overlay.DriverContext[0] = (PVOID) rwContext;

    rwContext->SubIrp = IoAllocateIrp(devExtension->TopOfStackDeviceObject->StackSize+1, FALSE);
    if(rwContext->SubIrp == NULL) {
		ExFreePool(rwContext);
		return STATUS_INSUFFICIENT_RESOURCES;
	}

#ifdef MOBI_DIRECT_IO
	rwContext->SubMdl = IoAllocateMdl((PVOID) (PAGE_SIZE - 1), PAGE_SIZE, FALSE, FALSE, NULL);
//	rwContext->SubMdl = IoAllocateMdl((PVOID) virtualAddress, totalLength, FALSE, FALSE, NULL);
    if(rwContext->SubMdl == NULL) {
		IoFreeIrp(rwContext->SubIrp);
 		ExFreePool(rwContext);
		return STATUS_INSUFFICIENT_RESOURCES;
	}
    IoBuildPartialMdl(Irp->MdlAddress, rwContext->SubMdl, (PVOID) virtualAddress, stageSize);
#endif
 //   rwContext->virtualAddress = virtualAddress + stageSize;
	//rwContext->Length = totalLength - stageSize;
    rwContext->virtualAddress = virtualAddress;
	rwContext->Length = totalLength;

	rwContext->urbFlag = USBD_SHORT_TRANSFER_OK | ( Read ? USBD_TRANSFER_DIRECTION_IN : USBD_TRANSFER_DIRECTION_OUT );

#ifdef MOBI_DIRECT_IO
	UsbBuildInterruptOrBulkTransferRequest( &rwContext->SubUrb, sizeof(struct _URB_BULK_OR_INTERRUPT_TRANSFER),
		pipeInformation->PipeHandle, NULL, rwContext->SubMdl, stageSize, rwContext->urbFlag, NULL);
#else
	if(!Read)
	{
		RtlCopyMemory(rwContext->PacketBuffer, (PVOID)virtualAddress, stageSize);
	}
	UsbBuildInterruptOrBulkTransferRequest( &rwContext->SubUrb, sizeof(struct _URB_BULK_OR_INTERRUPT_TRANSFER),
		pipeInformation->PipeHandle, rwContext->PacketBuffer, NULL, stageSize, rwContext->urbFlag, NULL);
#endif
    IoSetNextIrpStackLocation(rwContext->SubIrp);
    nextStack = IoGetCurrentIrpStackLocation(rwContext->SubIrp);

    nextStack->DeviceObject = DeviceObject;

    nextStack = IoGetNextIrpStackLocation(rwContext->SubIrp);
    nextStack->MajorFunction = IRP_MJ_INTERNAL_DEVICE_CONTROL;
    nextStack->Parameters.Others.Argument1 = (PVOID) (&rwContext->SubUrb);
    nextStack->Parameters.DeviceIoControl.IoControlCode = IOCTL_INTERNAL_USB_SUBMIT_URB;

    IoSetCompletionRoutine(rwContext->SubIrp, (PIO_COMPLETION_ROUTINE) MobiBulkStageComplete,
		(PVOID) rwContext, TRUE, TRUE, TRUE);

	return STATUS_SUCCESS;
}

BOOLEAN DestroyContextStructure(PBULKRW_CONTEXT rwContext)
{
	if (InterlockedDecrement(&rwContext->Lock) > 0)
		return FALSE;

	if (rwContext->SubIrp)
			IoFreeIrp(rwContext->SubIrp);

	ExFreePool(rwContext);
	return TRUE;
}

NTSTATUS MobiBulkReadWriteComplete(
   IN PDEVICE_OBJECT DeviceObject,
   IN PIRP           Irp,
   IN PVOID          Context
)
{
    PBULKRW_CONTEXT	   rwContext;
    PDEVICE_EXTENSION  devExtension;
    ULONG              i;

	MobiUsb_DbgPrint(4, ("MobiBulkReadWriteComplete: beg\n"));

    rwContext = (PBULKRW_CONTEXT) Context;
	devExtension = rwContext->DeviceExtension;
	if (Irp->Cancel)
	{
		if(rwContext != NULL)
			Irp->IoStatus.Information = rwContext->TransferBytes;
		Irp->IoStatus.Status = STATUS_CANCELLED;
	}
	else if (NT_SUCCESS(Irp->IoStatus.Status))
		Irp->IoStatus.Information = rwContext->TransferBytes;

	if(Irp->IoStatus.Information)
		MobiUsb_DbgPrint(3, ("MobiBulkReadWriteComplete: len = %d\n", Irp->IoStatus.Information));


	if (DestroyContextStructure(rwContext))
	{
	    MobiUsb_IoDecrement(devExtension);
		return STATUS_SUCCESS;
	}
	else
		return STATUS_MORE_PROCESSING_REQUIRED;
}

VOID
MobiBulkCancelReadWrite(
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP           Irp
    )
{
    PBULKRW_CONTEXT	   rwContext;
    PDEVICE_EXTENSION  devExtension;

	IoReleaseCancelSpinLock(Irp->CancelIrql);

	MobiUsb_DbgPrint(4, ("MobiBulkCancelReadWrite: beg\n"));

	rwContext = (PBULKRW_CONTEXT)Irp->Tail.Overlay.DriverContext[0];
	devExtension = rwContext->DeviceExtension;
	IoCancelIrp(rwContext->SubIrp);
	if (DestroyContextStructure(rwContext))
	{
		Irp->Tail.Overlay.DriverContext[0] = NULL;
		Irp->IoStatus.Status = STATUS_CANCELLED;
		IoCompleteRequest(Irp, IO_NO_INCREMENT);
	    MobiUsb_IoDecrement(devExtension);
	}
}

NTSTATUS
MobiBulkStageComplete(
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP           Irp,
    IN PVOID          Context
    )
{
    PBULKRW_CONTEXT	   rwContext;
    PDEVICE_EXTENSION  devExtension;
	PIO_STACK_LOCATION nextStack;
    ULONG              stageSize;
    ULONG              txNum;
	NTSTATUS		   ntStatus;

	MobiUsb_DbgPrint(4, ("MobiBulkStageComplete: beg\n"));

	rwContext = (PBULKRW_CONTEXT)Context;
	devExtension = rwContext->DeviceExtension;

	if (NT_SUCCESS(Irp->IoStatus.Status)) {
		txNum = rwContext->SubUrb.UrbBulkOrInterruptTransfer.TransferBufferLength;
#ifndef MOBI_DIRECT_IO
		if( rwContext->urbFlag & USBD_TRANSFER_DIRECTION_IN )
			RtlCopyMemory(rwContext->virtualAddress, rwContext->PacketBuffer, txNum);
#endif
		rwContext->TransferBytes += txNum;

		rwContext->virtualAddress += txNum;
		rwContext->Length -= txNum;

		if(rwContext ->Length && (txNum == MOBIUSB_MAX_TRANSFER_SIZE)) {

			stageSize = MOBIUSB_MAX_TRANSFER_SIZE;
			if(stageSize > rwContext->Length)
				stageSize = rwContext->Length;

#ifdef MOBI_DIRECT_IO
			MmPrepareMdlForReuse(rwContext->SubMdl);

			IoBuildPartialMdl(rwContext->MainIrp->MdlAddress,
								rwContext->SubMdl,
								(PVOID) rwContext->virtualAddress,
								stageSize);

			UsbBuildInterruptOrBulkTransferRequest( &rwContext->SubUrb, sizeof(struct _URB_BULK_OR_INTERRUPT_TRANSFER),
				rwContext->pipeInformation->PipeHandle, NULL, rwContext->SubMdl, stageSize, rwContext->urbFlag, NULL);
#else
			if(!(rwContext->urbFlag & USBD_TRANSFER_DIRECTION_IN))
				RtlCopyMemory(rwContext->PacketBuffer, (PVOID)rwContext->virtualAddress, stageSize);
			UsbBuildInterruptOrBulkTransferRequest( &rwContext->SubUrb, sizeof(struct _URB_BULK_OR_INTERRUPT_TRANSFER),
				rwContext->pipeInformation->PipeHandle, rwContext->PacketBuffer, NULL, stageSize, rwContext->urbFlag, NULL);
#endif

//			rwContext->SubUrb.UrbBulkOrInterruptTransfer.TransferBufferLength = stageSize;
			//rwContext->virtualAddress += stageSize;
			//rwContext->Length -= stageSize;

			nextStack = IoGetNextIrpStackLocation(Irp);
			nextStack->MajorFunction = IRP_MJ_INTERNAL_DEVICE_CONTROL;
			nextStack->Parameters.Others.Argument1 = &rwContext->SubUrb;
			nextStack->Parameters.DeviceIoControl.IoControlCode = IOCTL_INTERNAL_USB_SUBMIT_URB;

			IoSetCompletionRoutine(Irp, (PIO_COMPLETION_ROUTINE) MobiBulkStageComplete,
				(PVOID) rwContext, TRUE, TRUE, TRUE);

			ntStatus = IoCallDriver(rwContext->DeviceExtension->TopOfStackDeviceObject,  Irp);
			if(!NT_SUCCESS(ntStatus))
				MobiUsb_DbgPrint(1, ("IoCallDriver: status =============== %x\n", ntStatus));

			return STATUS_MORE_PROCESSING_REQUIRED;
		}
	}
	else {
		if(STATUS_CANCELLED == Irp->IoStatus.Status)
		{
			rwContext->TransferBytes += rwContext->SubUrb.UrbBulkOrInterruptTransfer.TransferBufferLength;
		}
		else {
			MobiUsb_DbgPrint(1, ("Irp->IoStatus.Status: status =============== %x\n", Irp->IoStatus.Status));
		}
		rwContext->ntStatus = Irp->IoStatus.Status;
	}

#ifdef MOBI_DIRECT_IO
	IoFreeMdl(rwContext->SubMdl);
#endif
	if (IoSetCancelRoutine(rwContext->MainIrp, NULL))
	{
		InterlockedDecrement(&rwContext->Lock);
	}
	rwContext->MainIrp->IoStatus.Status = rwContext->ntStatus;
	IoCompleteRequest(rwContext->MainIrp, IO_NO_INCREMENT);

	return STATUS_MORE_PROCESSING_REQUIRED;
}


