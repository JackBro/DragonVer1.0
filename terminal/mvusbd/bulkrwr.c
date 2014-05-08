
#include "bulkusb.h"
#include "bulkrwr.h"
#include "bulkpnp.h"
#include "bulkdev.h"

static NTSTATUS BulkUsb_BulkInitialize(IN PDEVICE_OBJECT DeviceObject,
									   IN OUT PTUsbdRwContext* pRwContext)
{
    NTSTATUS			ntStatus;
    PTDeviceExtension	deviceExtension;
	PTUsbdRwContext		rwContext;
    ULONG               urbFlags;
	UCHAR				AllocationFlags;

	ntStatus = STATUS_SUCCESS;
	BulkUsb_DbgPrint(1, ("file bulkrwr: BulkUsb_BulkInitialize process"));
    deviceExtension = (PTDeviceExtension) DeviceObject->DeviceExtension;
    *pRwContext = (PTUsbdRwContext)ExAllocatePool(NonPagedPool, sizeof(TUsbdRwContext));
	rwContext = *pRwContext;
    if(rwContext == NULL) 
	{
        BulkUsb_DbgPrint(1, ("Failed to alloc mem for rwContext\n"));
        return STATUS_INSUFFICIENT_RESOURCES;
    }

	rwContext->PacketBuffer = ExAllocatePool(NonPagedPool, BULKUSB_MAX_TRANSFER_SIZE);
	if(rwContext->PacketBuffer == NULL)
	{
		ExFreePool(rwContext);
		rwContext = NULL;
		return STATUS_INSUFFICIENT_RESOURCES; 
	}

	rwContext->Irp = IoAllocateIrp(deviceExtension->TopOfStackDeviceObject->StackSize, FALSE);
    if (rwContext->Irp == NULL) 
	{
		ExFreePool(rwContext->PacketBuffer);
		rwContext->PacketBuffer = NULL;
		ExFreePool(rwContext);
		rwContext = NULL;
		return STATUS_INSUFFICIENT_RESOURCES; 
    }
    rwContext->Urb = ExAllocatePool(NonPagedPool, sizeof(struct _URB_BULK_OR_INTERRUPT_TRANSFER));
    if(rwContext->Urb == NULL) 
	{
		ExFreePool(rwContext->PacketBuffer);
		rwContext->PacketBuffer = NULL;
        IoFreeIrp(rwContext->Irp);
		rwContext->Irp = NULL;
		ExFreePool(rwContext);
		rwContext = NULL;
		return STATUS_INSUFFICIENT_RESOURCES; 
    }
	return STATUS_SUCCESS;
}

static NTSTATUS BulkUsb_BulkUninitialize(IN PDEVICE_OBJECT DeviceObject,
								  IN OUT PTUsbdRwContext* pRwContext)
{
	PTUsbdRwContext	   rwContext;

	BulkUsb_DbgPrint(1, ("file bulkrwr: BulkUsb_BulkUninitialize process"));
	rwContext = *pRwContext;
	if(rwContext == NULL)
	{
		return STATUS_INVALID_HANDLE;
	}
	if(rwContext->PacketBuffer != NULL)
	{
		ExFreePool(rwContext->PacketBuffer);
		rwContext->PacketBuffer = NULL;
	}
	if(rwContext->Urb != NULL)
	{
		ExFreePool(rwContext->Urb);
		rwContext->Urb = NULL;
	}
	if(rwContext->Irp != NULL)
	{
		IoFreeIrp(rwContext->Irp);
		rwContext->Irp = NULL;
	}

	ExFreePool(rwContext);
	rwContext = NULL;

	return STATUS_SUCCESS;
}

static NTSTATUS BulkUsb_TransferPacketCompletion(IN PDEVICE_OBJECT DeviceObject,
												 IN PIRP           Irp,
												 IN PVOID          Context)
{
    PTUsbdRwContext rwContext;
	
	BulkUsb_DbgPrint(1, ("file bulkrwr: ReadWriteCompletion\n"));
    rwContext = (PTUsbdRwContext) Context;
	if(rwContext) 
	{
		KeSetEvent(&rwContext->BulkEvent, 1, FALSE);
	}
	
    return STATUS_MORE_PROCESSING_REQUIRED;
}

static NTSTATUS BulkUsb_TransferPacket(IN PDEVICE_OBJECT DeviceObject,
									   IN PTUsbdRwContext rwContext,
									   IN PUSBD_PIPE_INFORMATION pipeInformation,
									   IN ULONG TransferLength,
									   IN BOOLEAN read)
{
    NTSTATUS			ntStatus;
    PTDeviceExtension	deviceExtension;
    ULONG				urbFlags;
    PIO_STACK_LOCATION  nextStack;
	LARGE_INTEGER timeout;
	
    BulkUsb_DbgPrint(3, ("file bulkrwr: BulkUsb_TransferPacket  \n"));
    deviceExtension = (PTDeviceExtension) DeviceObject->DeviceExtension;
	urbFlags = USBD_SHORT_TRANSFER_OK;
    if(read) 
	{
        urbFlags |= USBD_TRANSFER_DIRECTION_IN;
        BulkUsb_DbgPrint(3, ("file bulkrwr: Read operation\n"));
    }
    else 
	{
        urbFlags |= USBD_TRANSFER_DIRECTION_OUT;
        BulkUsb_DbgPrint(3, ("file bulkrwr: Write operation\n"));
    }

    UsbBuildInterruptOrBulkTransferRequest(
                            rwContext->Urb,
                            sizeof(struct _URB_BULK_OR_INTERRUPT_TRANSFER),
                            pipeInformation->PipeHandle,
                            rwContext->PacketBuffer,
                            NULL,
                            TransferLength,
                            urbFlags,
                            NULL);
	
	nextStack = IoGetNextIrpStackLocation(rwContext->Irp);
	nextStack->MajorFunction = IRP_MJ_INTERNAL_DEVICE_CONTROL;
	nextStack->Parameters.Others.Argument1 = rwContext->Urb;
	nextStack->Parameters.DeviceIoControl.IoControlCode = IOCTL_INTERNAL_USB_SUBMIT_URB;
	IoSetCompletionRoutine(rwContext->Irp,
			BulkUsb_TransferPacketCompletion,
			rwContext,
			TRUE,
			TRUE,
			TRUE);

	timeout.QuadPart = -10000 * 100; //100 ms
    KeInitializeEvent(&rwContext->BulkEvent, SynchronizationEvent, FALSE);
    if (KeGetCurrentIrql() < DISPATCH_LEVEL) 
	{
		ntStatus = IoCallDriver(deviceExtension->TopOfStackDeviceObject, rwContext->Irp);
	}
	else
	{
		rwContext->Irp->IoStatus.Information = 0;
		rwContext->Irp->IoStatus.Status = STATUS_SUCCESS;
		return STATUS_SUCCESS;
	}
	if(ntStatus == STATUS_PENDING)
	{
        ntStatus = KeWaitForSingleObject(&rwContext->BulkEvent, Executive, KernelMode, FALSE, &timeout);
        if (ntStatus == STATUS_TIMEOUT) 
		{
            ntStatus = STATUS_IO_TIMEOUT;
            IoCancelIrp(rwContext->Irp);
			KeWaitForSingleObject(&rwContext->BulkEvent, Executive, KernelMode, FALSE,NULL);
			rwContext->Irp->IoStatus.Information = 0;
			ntStatus = STATUS_CANCELLED;
        }
        else 
		{
			rwContext->Irp->IoStatus.Information = rwContext->Urb->UrbBulkOrInterruptTransfer.TransferBufferLength;
			rwContext->Irp->IoStatus.Status = rwContext->Urb->UrbBulkOrInterruptTransfer.Hdr.Status;
            ntStatus = STATUS_SUCCESS;
        }
	}
	else
	{
		if(ntStatus == STATUS_SUCCESS)
		{
			rwContext->Irp->IoStatus.Information = rwContext->Urb->UrbBulkOrInterruptTransfer.TransferBufferLength;
			rwContext->Irp->IoStatus.Status = rwContext->Urb->UrbBulkOrInterruptTransfer.Hdr.Status;
		}
	}
	
	return ntStatus;
}

NTSTATUS BulkUsb_DispatchReadWrite(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{
	ULONG				totalLength;
	ULONG				stageLength;
	BOOLEAN				read;
	NTSTATUS			ntStatus;
	ULONG_PTR			virtualAddress;
	PFILE_OBJECT		fileObject;
	PTDeviceExtension	deviceExtension;
	PIO_STACK_LOCATION	irpStack;
	PIO_STACK_LOCATION	nextStack;
	PTUsbdRwContext		rwContext;
	PUSBD_PIPE_INFORMATION pipeInformation;

	rwContext = NULL;
	totalLength = 0;
	irpStack = IoGetCurrentIrpStackLocation(Irp);
	fileObject = irpStack->FileObject;
	read = (irpStack->MajorFunction == IRP_MJ_READ) ? TRUE : FALSE;
	deviceExtension = (PTDeviceExtension) DeviceObject->DeviceExtension;
	BulkUsb_DbgPrint(3, ("file bulkrwr: BulkUsb_DispatchReadWrite - begins\n"));

	if(deviceExtension->DeviceState != WORKING) 
	{
		BulkUsb_DbgPrint(1, ("file bulkrwr: Invalid device state\n"));
		ntStatus = STATUS_INVALID_DEVICE_STATE;
		goto BulkUsb_DispatchReadWrite_Exit;
	}
	BulkUsb_DbgPrint(3, ("file bulkrwr: Waiting on the IdleReqPendEvent\n"));
	if(deviceExtension->SSEnable) 
	{
		KeWaitForSingleObject(&deviceExtension->NoIdleReqPendEvent, 
			Executive, 
			KernelMode, 
			FALSE, 
			NULL);
	}

	if(fileObject && fileObject->FsContext) 
	{
		pipeInformation = fileObject->FsContext;
		if((UsbdPipeTypeBulk != pipeInformation->PipeType) &&
			(UsbdPipeTypeInterrupt != pipeInformation->PipeType)) 
		{
			BulkUsb_DbgPrint(1, ("file bulkrwr: Usbd pipe type is not bulk or interrupt\n"));
			ntStatus = STATUS_INVALID_HANDLE;
			goto BulkUsb_DispatchReadWrite_Exit;
		}
	}
	else 
	{
		BulkUsb_DbgPrint(1, ("file bulkrwr: Invalid handle\n"));
		ntStatus = STATUS_INVALID_HANDLE;
		goto BulkUsb_DispatchReadWrite_Exit;
	}

	virtualAddress = (ULONG_PTR)Irp->AssociatedIrp.SystemBuffer;
	totalLength = irpStack->Parameters.Write.Length;
	if(read)
		totalLength = irpStack->Parameters.Read.Length;

	if(totalLength > BULKUSB_TEST_BOARD_TRANSFER_BUFFER_SIZE) 
	{
		BulkUsb_DbgPrint(1, ("file bulkrwr: Transfer length > circular buffer\n"));
		ntStatus = STATUS_INVALID_PARAMETER;
		goto BulkUsb_DispatchReadWrite_Exit;
	}
	if(totalLength == 0) 
	{
		BulkUsb_DbgPrint(1, ("file bulkrwr: Transfer data length = 0\n"));
		ntStatus = STATUS_SUCCESS;
		goto BulkUsb_DispatchReadWrite_Exit;
	}

	BulkUsb_BulkInitialize(DeviceObject, &rwContext);
	BulkUsb_IoIncrement(deviceExtension);
	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information = 0;
	while(totalLength)
	{
		stageLength = totalLength;
		if(totalLength > BULKUSB_MAX_TRANSFER_SIZE) 
			stageLength = BULKUSB_MAX_TRANSFER_SIZE;
		if(!read)
		{
			RtlCopyMemory(rwContext->PacketBuffer, (PVOID)virtualAddress, stageLength);
		}
		ntStatus = BulkUsb_TransferPacket(DeviceObject, rwContext, pipeInformation, stageLength, read);
		if(STATUS_SUCCESS == ntStatus)
		{
			if(read)
			{
				RtlCopyMemory((PVOID)virtualAddress, rwContext->PacketBuffer, stageLength);
			}
			Irp->IoStatus.Information += rwContext->Irp->IoStatus.Information;
			ntStatus = STATUS_SUCCESS;
			if(rwContext->Irp->IoStatus.Information < stageLength)
				break;
		}
		else //Cancled
		{
			if(!NT_SUCCESS(ntStatus)) 
			{
				BulkUsb_DbgPrint(1, ("file bulkrwr: IoCallDriver fails with status %X\n", ntStatus));
				if((ntStatus != STATUS_CANCELLED) && (ntStatus != STATUS_DEVICE_NOT_CONNECTED)) 
				{
					ntStatus = BulkUsb_ResetPipe(DeviceObject, pipeInformation);
					if(!NT_SUCCESS(ntStatus)) 
					{
						BulkUsb_DbgPrint(1, ("file bulkrwr: BulkUsb_ResetPipe failed\n"));
						ntStatus = BulkUsb_ResetDevice(DeviceObject);
					}
				}
				else 
				{
					BulkUsb_DbgPrint(3, ("file bulkrwr: ntStatus is STATUS_CANCELLED or "
						"STATUS_DEVICE_NOT_CONNECTED\n"));
				}
			}
			BulkUsb_DbgPrint(1, ("file bulkrwr: BulkUsb_TransferPacket ------------------error\n"));
			rwContext->Irp->IoStatus.Information = 0;
			ntStatus = STATUS_SUCCESS;
			break;
		}
		virtualAddress += stageLength;
		totalLength -= stageLength;
	}
	BulkUsb_IoDecrement(deviceExtension);
	BulkUsb_BulkUninitialize(DeviceObject, &rwContext);
	BulkUsb_DbgPrint(1, ("file bulkrwr: BulkUsb_read write ------------------end\n"));
	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	return ntStatus;

BulkUsb_DispatchReadWrite_Exit:

	Irp->IoStatus.Status = ntStatus;
	Irp->IoStatus.Information = 0;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	BulkUsb_DbgPrint(3, ("file bulkrwr: BulkUsb_DispatchReadWrite - ends\n"));
	return ntStatus;
}

NTSTATUS BulkUsb_ResetPipe(IN PDEVICE_OBJECT DeviceObject, IN PUSBD_PIPE_INFORMATION PipeInfo)
{
    PURB              urb;
    NTSTATUS          ntStatus;
    PTDeviceExtension deviceExtension;

	urb = NULL;
    deviceExtension = (PTDeviceExtension) DeviceObject->DeviceExtension;
    urb = ExAllocatePool(NonPagedPool, sizeof(struct _URB_PIPE_REQUEST));
    if(urb) 
	{
        urb->UrbHeader.Length = (USHORT) sizeof(struct _URB_PIPE_REQUEST);
        urb->UrbHeader.Function = URB_FUNCTION_RESET_PIPE;
        urb->UrbPipeRequest.PipeHandle = PipeInfo->PipeHandle;
        ntStatus = CallUSBD(DeviceObject, urb);
        ExFreePool(urb);
    }
    else 
        ntStatus = STATUS_INSUFFICIENT_RESOURCES;

    if(NT_SUCCESS(ntStatus)) 
	{
        BulkUsb_DbgPrint(3, ("file bulkdev: BulkUsb_ResetPipe - success\n"));
        ntStatus = STATUS_SUCCESS;
    }
    else 
	{
        BulkUsb_DbgPrint(1, ("file bulkdev: BulkUsb_ResetPipe - failed\n"));
    }

    return ntStatus;
}

//======================================
PTPipeContext BulkUsb_PipeWithName(IN PDEVICE_OBJECT DeviceObject, IN PUNICODE_STRING FileName)
{
	LONG                  ix;
	ULONG                 uval; 
	ULONG                 nameLength;
	ULONG                 umultiplier;
	PTDeviceExtension     deviceExtension;
	PTPipeContext	pipeContext;

	pipeContext = NULL;
	nameLength = (FileName->Length / sizeof(WCHAR));
	deviceExtension = (PTDeviceExtension) DeviceObject->DeviceExtension;
	BulkUsb_DbgPrint(3, ("file bulkrwr: BulkUsb_PipeWithName - begins\n"));
	if(nameLength != 0) 
	{
		ix = nameLength - 1;
		while((ix > -1) &&
				((FileName->Buffer[ix] < (WCHAR) '0')  ||
				(FileName->Buffer[ix] > (WCHAR) '9')))             
			ix--;

		if(ix > -1) 
		{
			uval = 0;
			umultiplier = 1;
			while((ix > -1) &&
					(FileName->Buffer[ix] >= (WCHAR) '0') &&
					(FileName->Buffer[ix] <= (WCHAR) '9'))          
			{
				uval += (umultiplier * (ULONG) (FileName->Buffer[ix] - (WCHAR) '0'));
				ix--;
				umultiplier *= 10;
			}
			if(uval < 6 && deviceExtension->PipeContext) 
				pipeContext = &deviceExtension->PipeContext[uval];
		}
	}
	BulkUsb_DbgPrint(3, ("file bulkrwr: BulkUsb_PipeWithName - ends\n"));

	return pipeContext;
}

