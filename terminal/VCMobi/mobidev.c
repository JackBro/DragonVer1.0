#include "mobidriver.h"

NTSTATUS
MobiUsb_DispatchCreate(
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP           Irp
    )
/*++
 
Routine Description:

    Dispatch routine for create.

Arguments:

    DeviceObject - pointer to device object
    Irp - I/O request packet.

Return Value:

    NT status value

--*/
{
    ULONG                       i;
    NTSTATUS                    ntStatus;
    PFILE_OBJECT                fileObject;
    PDEVICE_EXTENSION           deviceExtension;
    PIO_STACK_LOCATION          irpStack;
    PMOBIUSB_PIPE_CONTEXT       pipeContext;
    PUSBD_INTERFACE_INFORMATION interface;

    PAGED_CODE();

	MobiUsb_DbgPrint(3, ("file mobidev: MobiUsb_DispatchCreate - begins\n"));

    //
    // initialize variables
    //
    irpStack = IoGetCurrentIrpStackLocation(Irp);
    fileObject = irpStack->FileObject;
    deviceExtension = (PDEVICE_EXTENSION) DeviceObject->DeviceExtension;

    if(deviceExtension->DeviceState != Working) {

        ntStatus = STATUS_INVALID_DEVICE_STATE;
        goto MobiUsb_DispatchCreate_Exit;
    }

    if(deviceExtension->UsbInterface) {

        interface = deviceExtension->UsbInterface;
    }
    else {

        MobiUsb_DbgPrint(1, ("file mobidev: UsbInterface not found\n"));

        ntStatus = STATUS_INVALID_DEVICE_STATE;
        goto MobiUsb_DispatchCreate_Exit;
    }

    //
    // FsContext is Null for the device
    //
    if(fileObject) {
        
        fileObject->FsContext = NULL; 
    }
    else {

        ntStatus = STATUS_INVALID_PARAMETER;
        goto MobiUsb_DispatchCreate_Exit;
    }

    if(0 == fileObject->FileName.Length) {

        //
        // opening a device as opposed to pipe.
        //
        ntStatus = STATUS_SUCCESS;

        InterlockedIncrement(&deviceExtension->OpenHandleCount);

        //
        // the device is idle if it has no open handles or pending PnP Irps
        // since we just received an open handle request, cancel idle req.
        //
        if(deviceExtension->SSEnable) {
        
            CancelSelectSuspend(deviceExtension);
        }

        goto MobiUsb_DispatchCreate_Exit;
    }
    
    pipeContext = MobiUsb_PipeWithName(DeviceObject, &fileObject->FileName);

    if(pipeContext == NULL) {

        ntStatus = STATUS_INVALID_PARAMETER;
        goto MobiUsb_DispatchCreate_Exit;
    }

    ntStatus = STATUS_INVALID_PARAMETER;

    for(i=0; i<interface->NumberOfPipes; i++) {

        if(pipeContext == &deviceExtension->PipeContext[i]) {

            //
            // found a match
            //
            MobiUsb_DbgPrint(3, ("file mobidev: open pipe %d\n", i));

            fileObject->FsContext = &interface->Pipes[i];
            
            ASSERT(fileObject->FsContext);

            pipeContext->PipeOpen = TRUE;

            ntStatus = STATUS_SUCCESS;

            //
            // increment OpenHandleCounts
            //
            InterlockedIncrement(&deviceExtension->OpenHandleCount);

            //
            // the device is idle if it has no open handles or pending PnP Irps
            // since we just received an open handle request, cancel idle req.
            //
            if(deviceExtension->SSEnable) {

                CancelSelectSuspend(deviceExtension);
            }
        }
    }

MobiUsb_DispatchCreate_Exit:

    Irp->IoStatus.Status = ntStatus;
    Irp->IoStatus.Information = 0;

    IoCompleteRequest(Irp, IO_NO_INCREMENT);

    MobiUsb_DbgPrint(3, ("file mobidev: MobiUsb_DispatchCreate - ends\n"));
    
    return ntStatus;
}

NTSTATUS
MobiUsb_DispatchClose(
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP           Irp
    )
/*++
 
Routine Description:

    Dispatch routine for close.

Arguments:

    DeviceObject - pointer to device object
    Irp - I/O request packet

Return Value:

    NT status value

--*/
{
    NTSTATUS               ntStatus;
    PFILE_OBJECT           fileObject;
    PDEVICE_EXTENSION      deviceExtension;
    PIO_STACK_LOCATION     irpStack;
    PMOBIUSB_PIPE_CONTEXT  pipeContext;
    PUSBD_PIPE_INFORMATION pipeInformation;
    
    PAGED_CODE();

    //
    // initialize variables
    //
    irpStack = IoGetCurrentIrpStackLocation(Irp);
    fileObject = irpStack->FileObject;
    pipeContext = NULL;
    pipeInformation = NULL;
    deviceExtension = (PDEVICE_EXTENSION) DeviceObject->DeviceExtension;

    MobiUsb_DbgPrint(3, ("file mobidev: MobiUsb_DispatchClose - begins\n"));

    if(fileObject && fileObject->FsContext) {

        pipeInformation = fileObject->FsContext;

        if(0 != fileObject->FileName.Length) {

            pipeContext = MobiUsb_PipeWithName(DeviceObject, 
                                               &fileObject->FileName);
        }

        if(pipeContext && pipeContext->PipeOpen) {
            
            pipeContext->PipeOpen = FALSE;
        }
    }

    //
    // set ntStatus to STATUS_SUCCESS 
    //
    ntStatus = STATUS_SUCCESS;

    Irp->IoStatus.Status = ntStatus;
    Irp->IoStatus.Information = 0;

    IoCompleteRequest(Irp, IO_NO_INCREMENT);

    InterlockedDecrement(&deviceExtension->OpenHandleCount);

    MobiUsb_DbgPrint(3, ("file mobidev: MobiUsb_DispatchClose - ends\n"));

    return ntStatus;
}

NTSTATUS
MobiUsb_DispatchDevCtrl(
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP Irp
    )
/*++
 
Routine Description:

    Dispatch routine for IRP_MJ_DEVICE_CONTROL

Arguments:

    DeviceObject - pointer to device object
    Irp - I/O request packet

Return Value:

    NT status value

--*/
{
    ULONG              code;
    PVOID              ioBuffer;
    ULONG              inputBufferLength;
    ULONG              outputBufferLength;
    ULONG              info;
    NTSTATUS           ntStatus;
    PDEVICE_EXTENSION  deviceExtension;
    PIO_STACK_LOCATION irpStack;

    //
    // initialize variables
    //
    info = 0;
    irpStack = IoGetCurrentIrpStackLocation(Irp);
    code = irpStack->Parameters.DeviceIoControl.IoControlCode;
    deviceExtension = (PDEVICE_EXTENSION) DeviceObject->DeviceExtension;

    ioBuffer           = Irp->AssociatedIrp.SystemBuffer;
    inputBufferLength  = irpStack->Parameters.DeviceIoControl.InputBufferLength;
    outputBufferLength = irpStack->Parameters.DeviceIoControl.OutputBufferLength;

    if(deviceExtension->DeviceState != Working) {

        MobiUsb_DbgPrint(1, ("file mobidev: Invalid device state\n"));

        Irp->IoStatus.Status = ntStatus = STATUS_INVALID_DEVICE_STATE;
        Irp->IoStatus.Information = info;

        IoCompleteRequest(Irp, IO_NO_INCREMENT);
        return ntStatus;
    }

    MobiUsb_DbgPrint(3, ("file mobidev: MobiUsb_DispatchDevCtrl::"));
    MobiUsb_IoIncrement(deviceExtension);

    //
    // It is true that the client driver cancelled the selective suspend
    // request in the dispatch routine for create.
    // But there is no guarantee that it has indeed been completed.
    // so wait on the NoIdleReqPendEvent and proceed only if this event
    // is signalled.
    //
    MobiUsb_DbgPrint(3, ("file mobidev: Waiting on the IdleReqPendEvent\n"));
    
    //
    // make sure that the selective suspend request has been completed.
    //

    if(deviceExtension->SSEnable) {

        KeWaitForSingleObject(&deviceExtension->NoIdleReqPendEvent, 
                              Executive, 
                              KernelMode, 
                              FALSE, 
                              NULL);
    }

    switch(code) {

    case IOCTL_MOBIUSB_RESET_PIPE:
    {
        PFILE_OBJECT           fileObject;
        PUSBD_PIPE_INFORMATION pipe;

        pipe = NULL;
        fileObject = NULL;

        //
        // FileObject is the address of the kernel file object to
        // which the IRP is directed. Drivers use the FileObject
        // to correlate IRPs in a queue.
        //
        fileObject = irpStack->FileObject;

        if(fileObject == NULL) {

            ntStatus = STATUS_INVALID_PARAMETER;

            break;
        }

        pipe = (PUSBD_PIPE_INFORMATION) fileObject->FsContext;

        if(pipe == NULL) {

            ntStatus = STATUS_INVALID_PARAMETER;
        }
        else {
            
            ntStatus = MobiUsb_ResetPipe(DeviceObject, pipe);
        }

        break;
    }

    case IOCTL_MOBIUSB_GET_CONFIG_DESCRIPTOR:
    {
        ULONG length;

        if(deviceExtension->UsbConfigurationDescriptor) {

            length = deviceExtension->UsbConfigurationDescriptor->wTotalLength;

            if(outputBufferLength >= length) {

                RtlCopyMemory(ioBuffer,
                              deviceExtension->UsbConfigurationDescriptor,
                              length);

                info = length;

                ntStatus = STATUS_SUCCESS;
            }
            else {
                
                ntStatus = STATUS_BUFFER_TOO_SMALL;
            }
        }
        else {
            
            ntStatus = STATUS_UNSUCCESSFUL;
        }

        break;
    }

    case IOCTL_MOBIUSB_RESET_DEVICE:
        
        ntStatus = MobiUsb_ResetDevice(DeviceObject);

        break;

	case IOCTL_MOBIUSB_VENDOR:
		ntStatus = USBCAMD_ControlVendorCommand(deviceExtension,0,0,0,&info,NULL,0,NULL,NULL);
		break;
    default :

        ntStatus = STATUS_INVALID_DEVICE_REQUEST;

        break;
    }

    Irp->IoStatus.Status = ntStatus;
    Irp->IoStatus.Information = info;

    IoCompleteRequest(Irp, IO_NO_INCREMENT);

    MobiUsb_DbgPrint(3, ("file mobidev: MobiUsb_DispatchDevCtrl::"));
    MobiUsb_IoDecrement(deviceExtension);

    return ntStatus;
}

NTSTATUS
MobiUsb_ResetPipe(
    IN PDEVICE_OBJECT         DeviceObject,
    IN PUSBD_PIPE_INFORMATION PipeInfo
    )
/*++
 
Routine Description:

    This routine synchronously submits a URB_FUNCTION_RESET_PIPE
    request down the stack.

Arguments:

    DeviceObject - pointer to device object
    PipeInfo - pointer to PipeInformation structure
               to retrieve the pipe handle

Return Value:

    NT status value

--*/
{
    PURB              urb;
    NTSTATUS          ntStatus;
    PDEVICE_EXTENSION deviceExtension;

    //
    // initialize variables
    //

    urb = NULL;
    deviceExtension = (PDEVICE_EXTENSION) DeviceObject->DeviceExtension;


    urb = ExAllocatePool(NonPagedPool, 
                         sizeof(struct _URB_PIPE_REQUEST));

    if(urb) {

        urb->UrbHeader.Length = (USHORT) sizeof(struct _URB_PIPE_REQUEST);
        urb->UrbHeader.Function = URB_FUNCTION_RESET_PIPE;
        urb->UrbPipeRequest.PipeHandle = PipeInfo->PipeHandle;

        ntStatus = CallUSBD(DeviceObject, urb);

        ExFreePool(urb);
    }
    else {

        ntStatus = STATUS_INSUFFICIENT_RESOURCES;
    }

    if(NT_SUCCESS(ntStatus)) {
    
        MobiUsb_DbgPrint(3, ("file mobidev: MobiUsb_ResetPipe - success\n"));
        ntStatus = STATUS_SUCCESS;
    }
    else {

        MobiUsb_DbgPrint(1, ("file mobidev: MobiUsb_ResetPipe - failed\n"));
    }

    return ntStatus;
}

NTSTATUS
MobiUsb_ResetDevice(
    IN PDEVICE_OBJECT DeviceObject
    )
/*++
 
Routine Description:

    This routine invokes MobiUsb_ResetParentPort to reset the device

Arguments:

    DeviceObject - pointer to device object

Return Value:

    NT status value

--*/
{
    NTSTATUS ntStatus;
    ULONG    portStatus;

    MobiUsb_DbgPrint(3, ("file mobidev: MobiUsb_ResetDevice - begins\n"));

    ntStatus = MobiUsb_GetPortStatus(DeviceObject, &portStatus);

    if((NT_SUCCESS(ntStatus))                 &&
       (!(portStatus & USBD_PORT_ENABLED))    &&
       (portStatus & USBD_PORT_CONNECTED)) {

        ntStatus = MobiUsb_ResetParentPort(DeviceObject);
    }

    MobiUsb_DbgPrint(3, ("file mobidev: MobiUsb_ResetDevice - ends\n"));

    return ntStatus;
}

NTSTATUS
MobiUsb_GetPortStatus(
    IN PDEVICE_OBJECT DeviceObject,
    IN OUT PULONG     PortStatus
    )
/*++
 
Routine Description:

    This routine retrieves the status value

Arguments:

    DeviceObject - pointer to device object
    PortStatus - port status

Return Value:

    NT status value

--*/
{
    NTSTATUS           ntStatus;
    KEVENT             event;
    PIRP               irp;
    IO_STATUS_BLOCK    ioStatus;
    PIO_STACK_LOCATION nextStack;
    PDEVICE_EXTENSION  deviceExtension;

    //
    // initialize variables
    //
    deviceExtension = (PDEVICE_EXTENSION) DeviceObject->DeviceExtension;
    *PortStatus = 0;

    MobiUsb_DbgPrint(3, ("file mobidev: MobiUsb_GetPortStatus - begins\n"));

    KeInitializeEvent(&event, NotificationEvent, FALSE);

    irp = IoBuildDeviceIoControlRequest(
                    IOCTL_INTERNAL_USB_GET_PORT_STATUS,
                    deviceExtension->TopOfStackDeviceObject,
                    NULL,
                    0,
                    NULL,
                    0,
                    TRUE,
                    &event,
                    &ioStatus);

    if(NULL == irp) {

        MobiUsb_DbgPrint(1, ("file mobidev: memory alloc for irp failed\n"));
        return STATUS_INSUFFICIENT_RESOURCES;
    }

    nextStack = IoGetNextIrpStackLocation(irp);

    ASSERT(nextStack != NULL);

    nextStack->Parameters.Others.Argument1 = PortStatus;

    ntStatus = IoCallDriver(deviceExtension->TopOfStackDeviceObject, irp);

    if(STATUS_PENDING == ntStatus) {

        KeWaitForSingleObject(&event, Executive, KernelMode, FALSE, NULL);
    }
    else {

        ioStatus.Status = ntStatus;
    }

    ntStatus = ioStatus.Status;

    MobiUsb_DbgPrint(3, ("file mobidev: MobiUsb_GetPortStatus - ends\n"));

    return ntStatus;
}

NTSTATUS
MobiUsb_ResetParentPort(
    IN PDEVICE_OBJECT DeviceObject
    )
/*++
 
Routine Description:

    This routine sends an IOCTL_INTERNAL_USB_RESET_PORT
    synchronously down the stack.

Arguments:

Return Value:

--*/
{
    NTSTATUS           ntStatus;
    KEVENT             event;
    PIRP               irp;
    IO_STATUS_BLOCK    ioStatus;
    PIO_STACK_LOCATION nextStack;
    PDEVICE_EXTENSION  deviceExtension;

    //
    // initialize variables
    //
    deviceExtension = (PDEVICE_EXTENSION) DeviceObject->DeviceExtension;

    MobiUsb_DbgPrint(3, ("file mobidev: MobiUsb_ResetParentPort - begins\n"));

    KeInitializeEvent(&event, NotificationEvent, FALSE);

    irp = IoBuildDeviceIoControlRequest(
                    IOCTL_INTERNAL_USB_RESET_PORT,
                    deviceExtension->TopOfStackDeviceObject,
                    NULL,
                    0,
                    NULL,
                    0,
                    TRUE,
                    &event,
                    &ioStatus);

    if(NULL == irp) {

        MobiUsb_DbgPrint(1, ("file mobidev: memory alloc for irp failed\n"));
        return STATUS_INSUFFICIENT_RESOURCES;
    }

    nextStack = IoGetNextIrpStackLocation(irp);

    ASSERT(nextStack != NULL);

    ntStatus = IoCallDriver(deviceExtension->TopOfStackDeviceObject, irp);

    if(STATUS_PENDING == ntStatus) {

        KeWaitForSingleObject(&event, Executive, KernelMode, FALSE, NULL);
    }
    else {

        ioStatus.Status = ntStatus;
    }

    ntStatus = ioStatus.Status;

    MobiUsb_DbgPrint(3, ("file mobidev: MobiUsb_ResetParentPort - ends\n"));

    return ntStatus;
}


NTSTATUS
SubmitIdleRequestIrp(
    IN PDEVICE_EXTENSION DeviceExtension
    )
/*++
 
Routine Description:

    This routine builds an idle request irp with an associated callback routine
    and a completion routine in the driver and passes the irp down the stack.

Arguments:

    DeviceExtension - pointer to device extension

Return Value:

    NT status value

--*/
{
    PIRP                    irp;
    NTSTATUS                ntStatus;
    KIRQL                   oldIrql;
    PUSB_IDLE_CALLBACK_INFO idleCallbackInfo;
    PIO_STACK_LOCATION      nextStack;

    //
    // initialize variables
    //
    
    irp = NULL;
    idleCallbackInfo = NULL;

    MobiUsb_DbgPrint(3, ("file mobidev: SubmitIdleRequest - begins\n"));

    ASSERT(KeGetCurrentIrql() == PASSIVE_LEVEL);

    if(PowerDeviceD0 != DeviceExtension->DevPower) {

        ntStatus = STATUS_POWER_STATE_INVALID;

        goto SubmitIdleRequestIrp_Exit;
    }

    KeAcquireSpinLock(&DeviceExtension->IdleReqStateLock, &oldIrql);

    if(InterlockedExchange(&DeviceExtension->IdleReqPend, 1)) {

        MobiUsb_DbgPrint(1, ("file mobidev: Idle request pending..\n"));

        KeReleaseSpinLock(&DeviceExtension->IdleReqStateLock, oldIrql);

        ntStatus = STATUS_DEVICE_BUSY;

        goto SubmitIdleRequestIrp_Exit;
    }

    //
    // clear the NoIdleReqPendEvent because we are about 
    // to submit an idle request. Since we are so early
    // to clear this event, make sure that if we fail this
    // request we set back the event.
    //
    KeClearEvent(&DeviceExtension->NoIdleReqPendEvent);

    idleCallbackInfo = ExAllocatePool(NonPagedPool, 
                                      sizeof(struct _USB_IDLE_CALLBACK_INFO));

    if(idleCallbackInfo) {

        idleCallbackInfo->IdleCallback = IdleNotificationCallback;

        idleCallbackInfo->IdleContext = (PVOID)DeviceExtension;

        ASSERT(DeviceExtension->IdleCallbackInfo == NULL);

        DeviceExtension->IdleCallbackInfo = idleCallbackInfo;

        //
        // we use IoAllocateIrp to create an irp to selectively suspend the 
        // device. This irp lies pending with the hub driver. When appropriate
        // the hub driver will invoked callback, where we power down. The completion
        // routine is invoked when we power back.
        //
        irp = IoAllocateIrp(DeviceExtension->TopOfStackDeviceObject->StackSize,
                            FALSE);

        if(irp == NULL) {

            MobiUsb_DbgPrint(1, ("file mobidev: cannot build idle request irp\n"));

            KeSetEvent(&DeviceExtension->NoIdleReqPendEvent,
                       IO_NO_INCREMENT,
                       FALSE);

            InterlockedExchange(&DeviceExtension->IdleReqPend, 0);

            KeReleaseSpinLock(&DeviceExtension->IdleReqStateLock, oldIrql);

            ExFreePool(idleCallbackInfo);

            ntStatus = STATUS_INSUFFICIENT_RESOURCES;

            goto SubmitIdleRequestIrp_Exit;
        }

        nextStack = IoGetNextIrpStackLocation(irp);

        nextStack->MajorFunction = 
                    IRP_MJ_INTERNAL_DEVICE_CONTROL;

        nextStack->Parameters.DeviceIoControl.IoControlCode = 
                    IOCTL_INTERNAL_USB_SUBMIT_IDLE_NOTIFICATION;

        nextStack->Parameters.DeviceIoControl.Type3InputBuffer =
                    idleCallbackInfo;

        nextStack->Parameters.DeviceIoControl.InputBufferLength =
                    sizeof(struct _USB_IDLE_CALLBACK_INFO);


        IoSetCompletionRoutine(irp, 
                               IdleNotificationRequestComplete,
                               DeviceExtension, 
                               TRUE, 
                               TRUE, 
                               TRUE);

        DeviceExtension->PendingIdleIrp = irp;

        //
        // we initialize the count to 2.
        // The reason is, if the CancelSelectSuspend routine manages
        // to grab the irp from the device extension, then the last of the
        // CancelSelectSuspend routine/IdleNotificationRequestComplete routine 
        // to execute will free this irp. We need to have this schema so that
        // 1. completion routine does not attempt to touch the irp freed by 
        //    CancelSelectSuspend routine.
        // 2. CancelSelectSuspend routine doesnt wait for ever for the completion
        //    routine to complete!
        //
        DeviceExtension->FreeIdleIrpCount = 2;

        KeReleaseSpinLock(&DeviceExtension->IdleReqStateLock, oldIrql);

        //
        // check if the device is idle.
        // A check here ensures that a race condition did not 
        // completely reverse the call sequence of SubmitIdleRequestIrp
        // and CancelSelectiveSuspend
        //

        if(!CanDeviceSuspend(DeviceExtension) ||
           PowerDeviceD0 != DeviceExtension->DevPower) {

            //
            // IRPs created using IoBuildDeviceIoControlRequest should be
            // completed by calling IoCompleteRequest and not merely 
            // deallocated.
            //
     
            MobiUsb_DbgPrint(1, ("file mobidev: Device is not idle\n"));

            KeAcquireSpinLock(&DeviceExtension->IdleReqStateLock, &oldIrql);

            DeviceExtension->IdleCallbackInfo = NULL;

            DeviceExtension->PendingIdleIrp = NULL;

            KeSetEvent(&DeviceExtension->NoIdleReqPendEvent,
                       IO_NO_INCREMENT,
                       FALSE);

            InterlockedExchange(&DeviceExtension->IdleReqPend, 0);

            KeReleaseSpinLock(&DeviceExtension->IdleReqStateLock, oldIrql);

            if(idleCallbackInfo) {

                ExFreePool(idleCallbackInfo);
            }

            //
            // it is still safe to touch the local variable "irp" here.
            // the irp has not been passed down the stack, the irp has
            // no cancellation routine. The worse position is that the
            // CancelSelectSuspend has run after we released the spin 
            // lock above. It is still essential to free the irp.
            //
            if(irp) {

                IoFreeIrp(irp);
            }

            ntStatus = STATUS_UNSUCCESSFUL;

            goto SubmitIdleRequestIrp_Exit;
        }

        MobiUsb_DbgPrint(3, ("file mobidev: Cancel the timers\n"));
        //
        // Cancel the timer so that the DPCs are no longer fired.
        // Thus, we are making judicious usage of our resources.
        // we do not need DPCs because we already have an idle irp pending.
        // The timers are re-initialized in the completion routine.
        //
        KeCancelTimer(&DeviceExtension->Timer);

        ntStatus = IoCallDriver(DeviceExtension->TopOfStackDeviceObject, irp);

        if(!NT_SUCCESS(ntStatus)) {

            MobiUsb_DbgPrint(1, ("file mobidev: IoCallDriver failed\n"));

            goto SubmitIdleRequestIrp_Exit;
        }
    }
    else {

        MobiUsb_DbgPrint(1, ("file mobidev: Memory allocation for idleCallbackInfo failed\n"));

        KeSetEvent(&DeviceExtension->NoIdleReqPendEvent,
                   IO_NO_INCREMENT,
                   FALSE);

        InterlockedExchange(&DeviceExtension->IdleReqPend, 0);

        KeReleaseSpinLock(&DeviceExtension->IdleReqStateLock, oldIrql);

        ntStatus = STATUS_INSUFFICIENT_RESOURCES;
    }

SubmitIdleRequestIrp_Exit:

    MobiUsb_DbgPrint(3, ("file mobidev: SubmitIdleRequest - ends\n"));

    return ntStatus;
}


VOID
IdleNotificationCallback(
    IN PDEVICE_EXTENSION DeviceExtension
    )
/*++
 
Routine Description:

  "A pointer to a callback function in your driver is passed down the stack with
   this IOCTL, and it is this callback function that is called by USBHUB when it
   safe for your device to power down."

  "When the callback in your driver is called, all you really need to do is to
   to first ensure that a WaitWake Irp has been submitted for your device, if 
   remote wake is possible for your device and then request a SetD2 (or DeviceWake)"

Arguments:

    DeviceExtension - pointer to device extension

Return Value:

    NT status value

--*/
{
    NTSTATUS                ntStatus;
    POWER_STATE             powerState;
    KEVENT                  irpCompletionEvent;
    PIRP_COMPLETION_CONTEXT irpContext;

    MobiUsb_DbgPrint(3, ("file mobidev: IdleNotificationCallback - begins\n"));

    //
    // Dont idle, if the device was just disconnected or being stopped
    // i.e. return for the following DeviceState(s)
    // NotStarted, Stopped, PendingStop, PendingRemove, SurpriseRemoved, Removed
    //

    if(DeviceExtension->DeviceState != Working) {

        return;
    }

    //
    // If there is not already a WW IRP pending, submit one now
    //
    if(DeviceExtension->WaitWakeEnable) {

        IssueWaitWake(DeviceExtension);
    }


    //
    // power down the device
    //

    irpContext = (PIRP_COMPLETION_CONTEXT) 
                 ExAllocatePool(NonPagedPool,
                                sizeof(IRP_COMPLETION_CONTEXT));

    if(!irpContext) {

        MobiUsb_DbgPrint(1, ("file mobidev: Failed to alloc memory for irpContext\n"));
        ntStatus = STATUS_INSUFFICIENT_RESOURCES;
    }
    else {

        //
        // increment the count. In the HoldIoRequestWorkerRoutine, the
        // count is decremented twice (one for the system Irp and the 
        // other for the device Irp. An increment here compensates for 
        // the sytem irp..The decrement corresponding to this increment 
        // is in the completion function
        //

        MobiUsb_DbgPrint(3, ("file mobidev: IdleNotificationCallback::"));
        MobiUsb_IoIncrement(DeviceExtension);

        powerState.DeviceState = DeviceExtension->PowerDownLevel;

        KeInitializeEvent(&irpCompletionEvent, NotificationEvent, FALSE);

        irpContext->DeviceExtension = DeviceExtension;
        irpContext->Event = &irpCompletionEvent;

        ntStatus = PoRequestPowerIrp(
                          DeviceExtension->PhysicalDeviceObject, 
                          IRP_MN_SET_POWER, 
                          powerState, 
                          (PREQUEST_POWER_COMPLETE) PoIrpCompletionFunc,
                          irpContext, 
                          NULL);

        if(STATUS_PENDING == ntStatus) {

            MobiUsb_DbgPrint(3, ("file mobidev: IdleNotificationCallback::"
                           "waiting for the power irp to complete\n"));

            KeWaitForSingleObject(&irpCompletionEvent,
                                  Executive,
                                  KernelMode,
                                  FALSE,
                                  NULL);
        }
    }
    
    if(!NT_SUCCESS(ntStatus)) {

        if(irpContext) {

            ExFreePool(irpContext);
        }
    }

    MobiUsb_DbgPrint(3, ("file mobidev: IdleNotificationCallback - ends\n"));
}


NTSTATUS
IdleNotificationRequestComplete(
    IN PDEVICE_OBJECT    DeviceObject,
    IN PIRP              Irp,
    IN PDEVICE_EXTENSION DeviceExtension
    )
/*++
 
Routine Description:

  Completion routine for idle notification irp

Arguments:

    DeviceObject - pointer to device object
    Irp - I/O request packet
    DeviceExtension - pointer to device extension

Return Value:

    NT status value

--*/
{
    NTSTATUS                ntStatus;
    POWER_STATE             powerState;
    KIRQL                   oldIrql;
    LARGE_INTEGER           dueTime;
    PIRP                    idleIrp;
    PUSB_IDLE_CALLBACK_INFO idleCallbackInfo;

    MobiUsb_DbgPrint(3, ("file mobidev: IdleNotificationRequestCompete - begins\n"));

    idleIrp = NULL;

    //
    // check the Irp status
    //

    ntStatus = Irp->IoStatus.Status;

    if(!NT_SUCCESS(ntStatus) && ntStatus != STATUS_NOT_SUPPORTED) {

        MobiUsb_DbgPrint(1, ("file mobidev: Idle irp completes with error::"));

        switch(ntStatus) {
            
        case STATUS_INVALID_DEVICE_REQUEST:

            MobiUsb_DbgPrint(1, ("file mobidev: STATUS_INVALID_DEVICE_REQUEST\n"));

            break;

        case STATUS_CANCELLED:

            MobiUsb_DbgPrint(1, ("file mobidev: STATUS_CANCELLED\n"));

            break;

        case STATUS_POWER_STATE_INVALID:

            MobiUsb_DbgPrint(1, ("file mobidev: STATUS_POWER_STATE_INVALID\n"));

            goto IdleNotificationRequestComplete_Exit;

        case STATUS_DEVICE_BUSY:

            MobiUsb_DbgPrint(1, ("file mobidev: STATUS_DEVICE_BUSY\n"));

            break;

        default:

            MobiUsb_DbgPrint(1, ("file mobidev: default: status = %X\n", ntStatus));

            break;
        }

        //
        // if in error, issue a SetD0 (only when not in D0)
        //

        if(PowerDeviceD0 != DeviceExtension->DevPower) {
            MobiUsb_DbgPrint(3, ("file mobidev: IdleNotificationRequestComplete::"));
            MobiUsb_IoIncrement(DeviceExtension);

            powerState.DeviceState = PowerDeviceD0;

            ntStatus = PoRequestPowerIrp(
                              DeviceExtension->PhysicalDeviceObject, 
                              IRP_MN_SET_POWER, 
                              powerState, 
                              (PREQUEST_POWER_COMPLETE) PoIrpAsyncCompletionFunc, 
                              DeviceExtension, 
                              NULL);

            if(!NT_SUCCESS(ntStatus)) {

                MobiUsb_DbgPrint(1, ("file mobidev: PoRequestPowerIrp failed\n"));
            }
        }
    }

IdleNotificationRequestComplete_Exit:

    KeAcquireSpinLock(&DeviceExtension->IdleReqStateLock, &oldIrql);

    idleCallbackInfo = DeviceExtension->IdleCallbackInfo;

    DeviceExtension->IdleCallbackInfo = NULL;

    idleIrp = (PIRP) InterlockedExchangePointer(
                                        &DeviceExtension->PendingIdleIrp,
                                        NULL);

    InterlockedExchange(&DeviceExtension->IdleReqPend, 0);

    KeReleaseSpinLock(&DeviceExtension->IdleReqStateLock, oldIrql);

    if(idleCallbackInfo) {

        ExFreePool(idleCallbackInfo);
    }

    //
    // since the irp was created using IoAllocateIrp, 
    // the Irp needs to be freed using IoFreeIrp.
    // Also return STATUS_MORE_PROCESSING_REQUIRED so that 
    // the kernel does not reference this in the near future.
    //

    if(idleIrp) {

        MobiUsb_DbgPrint(3, ("file mobidev: completion routine has a valid irp and frees it\n"));
        IoFreeIrp(Irp);
        KeSetEvent(&DeviceExtension->NoIdleReqPendEvent,
                   IO_NO_INCREMENT,
                   FALSE);
    }
    else {

        //
        // The CancelSelectiveSuspend routine has grabbed the Irp from the device 
        // extension. Now the last one to decrement the FreeIdleIrpCount should
        // free the irp.
        //
        if(0 == InterlockedDecrement(&DeviceExtension->FreeIdleIrpCount)) {

            MobiUsb_DbgPrint(3, ("file mobidev: completion routine frees the irp\n"));
            IoFreeIrp(Irp);

            KeSetEvent(&DeviceExtension->NoIdleReqPendEvent,
                       IO_NO_INCREMENT,
                       FALSE);
        }
    }

    if(DeviceExtension->SSEnable) {

        MobiUsb_DbgPrint(3, ("file mobidev: Set the timer to fire DPCs\n"));

        dueTime.QuadPart = -10000 * IDLE_INTERVAL;               // 5000 ms

        KeSetTimerEx(&DeviceExtension->Timer, 
                     dueTime,
                     IDLE_INTERVAL,                              // 5000 ms
                     &DeviceExtension->DeferredProcCall);

        MobiUsb_DbgPrint(3, ("file mobidev: IdleNotificationRequestCompete - ends\n"));
    }

    return STATUS_MORE_PROCESSING_REQUIRED;
}

VOID
CancelSelectSuspend(
    IN PDEVICE_EXTENSION DeviceExtension
    )
/*++
 
Routine Description:

    This routine is invoked to cancel selective suspend request.

Arguments:

    DeviceExtension - pointer to device extension

Return Value:

    None.

--*/
{
    PIRP  irp;
    KIRQL oldIrql;

    irp = NULL;

    MobiUsb_DbgPrint(3, ("file mobidev: CancelSelectSuspend - begins\n"));

    KeAcquireSpinLock(&DeviceExtension->IdleReqStateLock, &oldIrql);

    if(!CanDeviceSuspend(DeviceExtension))
    {
        MobiUsb_DbgPrint(3, ("file mobidev: Device is not idle\n"));
    
        irp = (PIRP) InterlockedExchangePointer(
                            &DeviceExtension->PendingIdleIrp, 
                            NULL);
    }

    KeReleaseSpinLock(&DeviceExtension->IdleReqStateLock, oldIrql);

    //
    // since we have a valid Irp ptr,
    // we can call IoCancelIrp on it,
    // without the fear of the irp 
    // being freed underneath us.
    //
    if(irp) {

        //
        // This routine has the irp pointer.
        // It is safe to call IoCancelIrp because we know that
        // the compleiton routine will not free this irp unless...
        // 
        //        
        if(IoCancelIrp(irp)) {

            MobiUsb_DbgPrint(3, ("file mobidev: IoCancelIrp returns TRUE\n"));
        }
        else {
            MobiUsb_DbgPrint(3, ("file mobidev: IoCancelIrp returns FALSE\n"));
        }

        //
        // ....we decrement the FreeIdleIrpCount from 2 to 1.
        // if completion routine runs ahead of us, then this routine 
        // decrements the FreeIdleIrpCount from 1 to 0 and hence shall
        // free the irp.
        //
        if(0 == InterlockedDecrement(&DeviceExtension->FreeIdleIrpCount)) {

            MobiUsb_DbgPrint(3, ("file mobidev: CancelSelectSuspend frees the irp\n"));
            IoFreeIrp(irp);

            KeSetEvent(&DeviceExtension->NoIdleReqPendEvent,
                       IO_NO_INCREMENT,
                       FALSE);
        }
    }

    MobiUsb_DbgPrint(3, ("file mobidev: CancelSelectSuspend - ends\n"));

    return;
}

VOID
PoIrpCompletionFunc(
    IN PDEVICE_OBJECT DeviceObject,
    IN UCHAR MinorFunction,
    IN POWER_STATE PowerState,
    IN PVOID Context,
    IN PIO_STATUS_BLOCK IoStatus
    )
/*++
 
Routine Description:

    Completion routine for power irp PoRequested in 
    IdleNotificationCallback.

Arguments:

    DeviceObject - pointer to device object
    MinorFunciton - minor function for the irp.
    PowerState - irp power state
    Context - context passed to the completion function
    IoStatus - status block.

Return Value:

    None

--*/
{
    PIRP_COMPLETION_CONTEXT irpContext;
    
    //
    // initialize variables
    //
    irpContext = NULL;

    if(Context) {

        irpContext = (PIRP_COMPLETION_CONTEXT) Context;
    }

    //
    // all we do is set the event and decrement the count
    //

    if(irpContext) {

        KeSetEvent(irpContext->Event, 0, FALSE);

        MobiUsb_DbgPrint(3, ("file mobidev: PoIrpCompletionFunc::"));
        MobiUsb_IoDecrement(irpContext->DeviceExtension);

        ExFreePool(irpContext);
    }

    return;
}

VOID
PoIrpAsyncCompletionFunc(
    IN PDEVICE_OBJECT DeviceObject,
    IN UCHAR MinorFunction,
    IN POWER_STATE PowerState,
    IN PVOID Context,
    IN PIO_STATUS_BLOCK IoStatus
    )
/*++
 
Routine Description:

    Completion routine for power irp PoRequested in IdleNotification
    RequestComplete routine.

Arguments:

    DeviceObject - pointer to device object
    MinorFunciton - minor function for the irp.
    PowerState - irp power state
    Context - context passed to the completion function
    IoStatus - status block.

Return Value:

    None

--*/
{
    PDEVICE_EXTENSION DeviceExtension;
    
    //
    // initialize variables
    //
    DeviceExtension = (PDEVICE_EXTENSION) Context;

    //
    // all we do is decrement the count
    //
    
    MobiUsb_DbgPrint(3, ("file mobidev: PoIrpAsyncCompletionFunc::"));
    MobiUsb_IoDecrement(DeviceExtension);

    return;
}

VOID
WWIrpCompletionFunc(
    IN PDEVICE_OBJECT DeviceObject,
    IN UCHAR MinorFunction,
    IN POWER_STATE PowerState,
    IN PVOID Context,
    IN PIO_STATUS_BLOCK IoStatus
    )
/*++
 
Routine Description:

    Completion routine for PoRequest wait wake irp

Arguments:

    DeviceObject - pointer to device object
    MinorFunciton - minor function for the irp.
    PowerState - irp power state
    Context - context passed to the completion function
    IoStatus - status block.    

Return Value:

    None

--*/
{
    PDEVICE_EXTENSION DeviceExtension;
    
    //
    // initialize variables
    //
    DeviceExtension = (PDEVICE_EXTENSION) Context;

    //
    // all we do is decrement the count
    //
    
    MobiUsb_DbgPrint(3, ("file mobidev: WWIrpCompletionFunc::"));
    MobiUsb_IoDecrement(DeviceExtension);

    return;
}


//for test l_yong 2004.8.16

//---------------------------------------------------------------------------
// USBCAMD_ControlVendorCommand
//---------------------------------------------------------------------------
NTSTATUS
USBCAMD_ControlVendorCommandWorker(
    IN PVOID DeviceContext,
    IN UCHAR Request,
    IN USHORT Value,
    IN USHORT Index,
    IN PVOID Buffer,
    IN OUT PULONG BufferLength,
    IN BOOLEAN GetData
    )
/*++

Routine Description:

    Send a vendor command to the camera to fetch data.

Arguments:

    DeviceExtension - pointer to the device extension for this instance of the USB camera
                    devcice.

    Request - Request code for setup packet.

    Value - Value for setup packet.

    Index - Index for setup packet.

    Buffer - Pointer to input buffer

    BufferLength - pointer size of input/output buffer (optional)

Return Value:

    NT status code

--*/
{
    NTSTATUS ntStatus;
    BOOLEAN allocated = FALSE;
    PUCHAR localBuffer;
    PUCHAR buffer;
    PURB urb;
    PDEVICE_EXTENSION deviceExtension;

    ULONG length = BufferLength ? *BufferLength : 0;

    USBCAMD_KdPrint (MIN_TRACE, ("'enter USBCAMD_ControlVendorCommand\n"));
	
	deviceExtension = DeviceContext;
//    deviceExtension = USBCAMD_GET_DEVICE_EXTENSION(DeviceContext);

    buffer = ExAllocatePool(NonPagedPool,
                            sizeof(struct
                            _URB_CONTROL_VENDOR_OR_CLASS_REQUEST) + length);


    if (buffer) {
        urb = (PURB) (buffer + length);

        USBCAMD_KdPrint (3, ("'enter USBCAMD_ControlVendorCommand req %x val %x index %x\n",
            Request, Value, Index));

        if (BufferLength && *BufferLength != 0) {
            localBuffer = buffer;
            if (!GetData) {
                RtlCopyMemory(localBuffer, Buffer, *BufferLength);
            }
        } else {
            localBuffer = NULL;
        }

        UsbBuildVendorRequest(urb,
                              URB_FUNCTION_VENDOR_DEVICE,
                              sizeof(struct _URB_CONTROL_VENDOR_OR_CLASS_REQUEST),
                              GetData ? USBD_TRANSFER_DIRECTION_IN :
                                  0,
                              0,
                              Request,
                              Value,
                              Index,
                              localBuffer,
                              NULL,
                              length,
                              NULL);

        USBCAMD_KdPrint (3, ("'BufferLength =  0x%x buffer = 0x%x\n",
            length, localBuffer));

        ntStatus = USBCAMD_CallUSBD(deviceExtension, urb);

        if (NT_SUCCESS(ntStatus)) {
            if (BufferLength) {
                *BufferLength =
                    urb->UrbControlVendorClassRequest.TransferBufferLength;

                USBCAMD_KdPrint (3, ("'BufferLength =  0x%x buffer = 0x%x\n",
                    *BufferLength, localBuffer));
                if (localBuffer && GetData) {
                    RtlCopyMemory(Buffer, localBuffer, *BufferLength);
                }
            }
        }
        else {
            USBCAMD_KdPrint (MIN_TRACE, ("USBCAMD_ControlVendorCommand Error 0x%x\n", ntStatus));            

            // Only expected failure.
            // TEST_TRAP();        
        }

        ExFreePool(buffer);
    } else {
        ntStatus = STATUS_INSUFFICIENT_RESOURCES;
        USBCAMD_KdPrint (MIN_TRACE, ("'USBCAMD_ControlVendorCommand Error 0x%x\n", ntStatus));
    }

    return ntStatus;

}


//---------------------------------------------------------------------------
// USBCAMD_ControlVendorCommand
//---------------------------------------------------------------------------
NTSTATUS
USBCAMD_ControlVendorCommand(
    IN PVOID DeviceContext,
    IN UCHAR Request,
    IN USHORT Value,
    IN USHORT Index,
    IN PVOID Buffer,
    IN OUT PULONG BufferLength,
    IN BOOLEAN GetData,
    IN PCOMMAND_COMPLETE_FUNCTION CommandComplete,
    IN PVOID CommandContext
    )
/*++

Routine Description:

    Send a vendor command to the camera to fetch data.

Arguments:

    DeviceExtension - pointer to the device extension for this instance of the USB camera
                    devcice.

    Request - Request code for setup packet.

    Value - Value for setup packet.

    Index - Index for setup packet.

    Buffer - Pointer to input buffer

    BufferLength - pointer size of input/output buffer (optional)

Return Value:

    NT status code

--*/
{
    NTSTATUS ntStatus;
    PCOMMAND_WORK_ITEM workitem;

    USBCAMD_KdPrint (MIN_TRACE, ("'enter USBCAMD_ControlVendorCommand2\n"));

    if (KeGetCurrentIrql() < DISPATCH_LEVEL) {
        //
        // we are at passive level, just do the command
        //
        ntStatus = USBCAMD_ControlVendorCommandWorker(DeviceContext,
                                                Request,
                                                Value,
                                                Index,
                                                Buffer,
                                                BufferLength,
                                                GetData);

        if (CommandComplete) {
            // call the completion handler
            (*CommandComplete)(DeviceContext, CommandContext, ntStatus);
        }

    } else {
//        TEST_TRAP();
        //
        // schedule a work item
        //
        ntStatus = STATUS_PENDING;

        workitem = ExAllocatePool(NonPagedPool,
                                          sizeof(COMMAND_WORK_ITEM));
        if (workitem) {

            ExInitializeWorkItem(&workitem->WorkItem,
                                 USBCAMD_CommandWorkItem,
                                 workitem);

            workitem->DeviceContext = DeviceContext;
            workitem->Request = Request;
            workitem->Value = Value;
            workitem->Index = Index;
            workitem->Buffer = Buffer;
            workitem->BufferLength = BufferLength;
            workitem->GetData = GetData;
            workitem->CommandComplete = CommandComplete;
            workitem->CommandContext = CommandContext;

            ExQueueWorkItem(&workitem->WorkItem,
                            DelayedWorkQueue);

        } else {
            ntStatus = STATUS_INSUFFICIENT_RESOURCES;
        }

    }

    return ntStatus;
}


VOID
USBCAMD_CommandWorkItem(
    PVOID Context
    )
/*++

Routine Description:

    Call the mini driver to convert a raw packet to the proper format.

Arguments:

Return Value:

    None.

--*/
{
    NTSTATUS ntStatus;
    PCOMMAND_WORK_ITEM workItem = Context;

    ntStatus = USBCAMD_ControlVendorCommandWorker(workItem->DeviceContext,
                                            workItem->Request,
                                            workItem->Value,
                                            workItem->Index,
                                            workItem->Buffer,
                                            workItem->BufferLength,
                                            workItem->GetData);


    if (workItem->CommandComplete) {
        // call the completion handler
        (*workItem->CommandComplete)(workItem->DeviceContext,
                                   workItem->CommandContext,
                                   ntStatus);
    }

    ExFreePool(workItem);
}

NTSTATUS
USBCAMD_CallUsbdCompletion (
    IN PDEVICE_OBJECT   DeviceObject,
    IN PIRP             Irp,
    IN PVOID            Context
    )
{
    PKEVENT kevent = (PKEVENT)Context;
    KeSetEvent(kevent, IO_NO_INCREMENT,FALSE);
    return STATUS_MORE_PROCESSING_REQUIRED;
}


//---------------------------------------------------------------------------
// USBCAMD_CallUSBD
//---------------------------------------------------------------------------
NTSTATUS
USBCAMD_CallUSBD(
    IN PDEVICE_EXTENSION DeviceExtension,
    IN PURB Urb
    )
/*++

Routine Description:

    Passes a URB to the USBD class driver

Arguments:

    DeviceExtension - pointer to the device extension for this instance of an USB camera

    Urb - pointer to Urb request block

Return Value:

    STATUS_SUCCESS if successful,
    STATUS_UNSUCCESSFUL otherwise

--*/
{
    NTSTATUS ntStatus, status = STATUS_SUCCESS;
    PIRP irp;
    KEVENT TimeoutEvent;
    PIO_STACK_LOCATION nextStack;

    KeWaitForSingleObject(&DeviceExtension->CallUSBSemaphore,Executive,KernelMode,FALSE,NULL);

    // Initialize the event we'll wait on
    //
    KeInitializeEvent(&TimeoutEvent,SynchronizationEvent,FALSE);

    // Allocate the Irp
    //
    irp = IoAllocateIrp(DeviceExtension->TopOfStackDeviceObject->StackSize, FALSE);

    if (irp == NULL){
        ntStatus =  STATUS_UNSUCCESSFUL;
        goto Exit_CallUSB;
    }
    //
    // Set the Irp parameters
    //
    nextStack = IoGetNextIrpStackLocation(irp);
    ASSERT(nextStack != NULL);
    nextStack->MajorFunction = IRP_MJ_INTERNAL_DEVICE_CONTROL;
    nextStack->Parameters.DeviceIoControl.IoControlCode =  IOCTL_INTERNAL_USB_SUBMIT_URB;
    nextStack->Parameters.Others.Argument1 = Urb;
    //
    // Set the completion routine.
    //
    IoSetCompletionRoutine(irp,USBCAMD_CallUsbdCompletion,&TimeoutEvent, TRUE, TRUE,TRUE);   
    //
    // pass the irp down usb stack
    //
#if 0
    if (DeviceExtension->Initialized ) {
#endif
        ntStatus = IoCallDriver(DeviceExtension->TopOfStackDeviceObject,irp);
#if 0
    } else {
        ntStatus = STATUS_DEVICE_DATA_ERROR;
    }
#endif

    USBCAMD_KdPrint (MIN_TRACE, ("return from IoCallDriver USBD %x\n", ntStatus));

    if (ntStatus == STATUS_PENDING) {
        // Irp i spending. we have to wait till completion..
        LARGE_INTEGER timeout;

        // Specify a timeout of 5 seconds to wait for this call to complete.
        //
        timeout.QuadPart = -10000 * 5000;

        ntStatus = KeWaitForSingleObject(&TimeoutEvent, Executive,KernelMode,FALSE, &timeout);
        if (ntStatus == STATUS_TIMEOUT) {
             ntStatus = STATUS_IO_TIMEOUT;

            // Cancel the Irp we just sent.
            //
            IoCancelIrp(irp);

            // And wait until the cancel completes
            //
            KeWaitForSingleObject(&TimeoutEvent,Executive, KernelMode, FALSE,NULL);
        }
        else {
            ntStatus = irp->IoStatus.Status;
        }
    }

    // Done with the Irp, now free it.
    //
    IoFreeIrp(irp);

Exit_CallUSB:

    KeReleaseSemaphore(&DeviceExtension->CallUSBSemaphore,LOW_REALTIME_PRIORITY,1,FALSE);

    if (NT_ERROR(ntStatus)) {
        USBCAMD_KdPrint(MIN_TRACE, ("***Error*** USBCAMD_CallUSBD (%x)\n", ntStatus));
    }

    return ntStatus;
}

