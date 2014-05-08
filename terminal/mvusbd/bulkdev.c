
#include "bulkusb.h"
#include "bulkpnp.h"
#include "bulkdev.h"
#include "bulkpwr.h"
#include "bulkrwr.h"
#include "bulkusr.h"

static VOID PoIrpAsyncCompletionFunc(IN PDEVICE_OBJECT DeviceObject, IN UCHAR MinorFunction,
    IN POWER_STATE PowerState, IN PVOID Context, IN PIO_STATUS_BLOCK IoStatus)
{
    PTDeviceExtension DeviceExtension;
    
    DeviceExtension = (PTDeviceExtension) Context;
	if(DeviceExtension)
	{
		BulkUsb_DbgPrint(3, ("file bulkdev: PoIrpAsyncCompletionFunc::"));
		BulkUsb_IoDecrement(DeviceExtension);
	}
}

static VOID PoIrpCompletionFunc(IN PDEVICE_OBJECT DeviceObject, IN UCHAR MinorFunction,
    IN POWER_STATE PowerState, IN PVOID Context, IN PIO_STATUS_BLOCK IoStatus)
{
	PTIrpCompletionContext irpContext;

	//irpContext = NULL;
	//if(Context) 
		irpContext = (PTIrpCompletionContext) Context;
	if(irpContext) 
	{
		KeSetEvent(irpContext->Event, 0, FALSE);
		BulkUsb_DbgPrint(3, ("file bulkdev: PoIrpCompletionFunc::"));
		BulkUsb_IoDecrement(irpContext->DeviceExtension);
		ExFreePool(irpContext);
	}
}

VOID WWIrpCompletionFunc(IN PDEVICE_OBJECT DeviceObject, IN UCHAR MinorFunction,
    IN POWER_STATE PowerState, IN PVOID Context, IN PIO_STATUS_BLOCK IoStatus)
{
	PTDeviceExtension DeviceExtension;

	DeviceExtension = (PTDeviceExtension) Context;
	if(DeviceExtension)
	{
		BulkUsb_DbgPrint(3, ("file bulkdev: WWIrpCompletionFunc::"));
		BulkUsb_IoDecrement(DeviceExtension);
	}
}

static NTSTATUS IdleNotificationRequestComplete(IN PDEVICE_OBJECT DeviceObject,
												IN PIRP Irp, IN PTDeviceExtension DeviceExtension)
{
	NTSTATUS                ntStatus;
	POWER_STATE             powerState;
	KIRQL                   oldIrql;
	LARGE_INTEGER           dueTime;
	PIRP                    idleIrp;
	PUSB_IDLE_CALLBACK_INFO idleCallbackInfo;

	BulkUsb_DbgPrint(3, ("file bulkdev: IdleNotificationRequestCompete - begins\n"));
	idleIrp = NULL;
	ntStatus = Irp->IoStatus.Status;
	if(!NT_SUCCESS(ntStatus) && ntStatus != STATUS_NOT_SUPPORTED) 
	{
		BulkUsb_DbgPrint(1, ("file bulkdev: Idle irp completes with error::"));
		switch(ntStatus) 
		{
		case STATUS_INVALID_DEVICE_REQUEST:
			BulkUsb_DbgPrint(1, ("file bulkdev: STATUS_INVALID_DEVICE_REQUEST\n"));
			break;

		case STATUS_CANCELLED:
			BulkUsb_DbgPrint(1, ("file bulkdev: STATUS_CANCELLED\n"));
			break;

		case STATUS_POWER_STATE_INVALID:
			BulkUsb_DbgPrint(1, ("file bulkdev: STATUS_POWER_STATE_INVALID\n"));
			goto IdleNotificationRequestComplete_Exit;

		case STATUS_DEVICE_BUSY:
			BulkUsb_DbgPrint(1, ("file bulkdev: STATUS_DEVICE_BUSY\n"));
			break;

		default:
			BulkUsb_DbgPrint(1, ("file bulkdev: default: status = %X\n", ntStatus));
			break;
		}

		if(PowerDeviceD0 != DeviceExtension->DevPower) 
		{
			BulkUsb_DbgPrint(3, ("file bulkdev: IdleNotificationRequestComplete::"));
			BulkUsb_IoIncrement(DeviceExtension);
			powerState.DeviceState = PowerDeviceD0;
			ntStatus = PoRequestPowerIrp(
						DeviceExtension->PhysicalDeviceObject, 
						IRP_MN_SET_POWER, 
						powerState, 
						(PREQUEST_POWER_COMPLETE) PoIrpAsyncCompletionFunc, 
						DeviceExtension, 
						NULL);

			if(!NT_SUCCESS(ntStatus)) 
			{
				BulkUsb_DbgPrint(1, ("file bulkdev: PoRequestPowerIrp failed\n"));
			}
		}
	}

IdleNotificationRequestComplete_Exit:

	KeAcquireSpinLock(&DeviceExtension->IdleReqStateLock, &oldIrql);
	idleCallbackInfo = DeviceExtension->IdleCallbackInfo;
	DeviceExtension->IdleCallbackInfo = NULL;
	idleIrp = (PIRP) InterlockedExchangePointer(&DeviceExtension->PendingIdleIrp, NULL);
	InterlockedExchange(&DeviceExtension->IdleReqPend, 0);
	KeReleaseSpinLock(&DeviceExtension->IdleReqStateLock, oldIrql);
	if(idleCallbackInfo) 
	{
		ExFreePool(idleCallbackInfo);
	}
	if(idleIrp) 
	{
		BulkUsb_DbgPrint(3, ("file bulkdev: completion routine has a valid irp and frees it\n"));
		IoFreeIrp(Irp);
		KeSetEvent(&DeviceExtension->NoIdleReqPendEvent, IO_NO_INCREMENT, FALSE);
	}
	else 
	{
		if(0 == InterlockedDecrement(&DeviceExtension->FreeIdleIrpCount)) 
		{
			BulkUsb_DbgPrint(3, ("file bulkdev: completion routine frees the irp\n"));
			IoFreeIrp(Irp);
			KeSetEvent(&DeviceExtension->NoIdleReqPendEvent, IO_NO_INCREMENT, FALSE);
		}
	}
	if(DeviceExtension->SSEnable) 
	{
		BulkUsb_DbgPrint(3, ("file bulkdev: Set the timer to fire DPCs\n"));
		dueTime.QuadPart = -10000 * IDLE_INTERVAL;               // 5000 ms
		KeSetTimerEx(&DeviceExtension->Timer, 
			dueTime,
			IDLE_INTERVAL,                              // 5000 ms
			&DeviceExtension->DeferredProcCall);

		BulkUsb_DbgPrint(3, ("file bulkdev: IdleNotificationRequestCompete - ends\n"));
	}
	return STATUS_MORE_PROCESSING_REQUIRED;
}

static VOID IdleNotificationCallback(IN PTDeviceExtension DeviceExtension)
{
	NTSTATUS                ntStatus;
	POWER_STATE             powerState;
	KEVENT                  irpCompletionEvent;
	PTIrpCompletionContext	irpContext;

	BulkUsb_DbgPrint(3, ("file bulkdev: IdleNotificationCallback - process\n"));
	if(DeviceExtension->DeviceState != WORKING) 
		return;

	if(DeviceExtension->WaitWakeEnable) 
		IssueWaitWake(DeviceExtension);
	irpContext = (PTIrpCompletionContext)ExAllocatePool(NonPagedPool, sizeof(TIrpCompletionContext));
	if(!irpContext) 
	{
		BulkUsb_DbgPrint(1, ("file bulkdev: Failed to alloc memory for irpContext\n"));
		//ntStatus = STATUS_INSUFFICIENT_RESOURCES;
		return;
	}
	BulkUsb_DbgPrint(3, ("file bulkdev: IdleNotificationCallback::"));
	BulkUsb_IoIncrement(DeviceExtension);
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

	if(STATUS_PENDING == ntStatus) 
	{
		BulkUsb_DbgPrint(3, ("file bulkdev: IdleNotificationCallback::"
						"waiting for the power irp to complete\n"));
		KeWaitForSingleObject(&irpCompletionEvent,
					Executive,
					KernelMode,
					FALSE,
					NULL);
	}
	ExFreePool(irpContext);
	BulkUsb_DbgPrint(3, ("file bulkdev: IdleNotificationCallback - ends\n"));
}

NTSTATUS SubmitIdleRequestIrp(IN PTDeviceExtension DeviceExtension)
{
	PIRP                    irp;
	NTSTATUS                ntStatus;
	KIRQL                   oldIrql;
	PUSB_IDLE_CALLBACK_INFO idleCallbackInfo;
	PIO_STACK_LOCATION      nextStack;

	irp = NULL;
	idleCallbackInfo = NULL;
	BulkUsb_DbgPrint(3, ("file bulkdev: SubmitIdleRequest - process\n"));
	ASSERT(KeGetCurrentIrql() == PASSIVE_LEVEL);
	if(PowerDeviceD0 != DeviceExtension->DevPower) 
	{
//		ntStatus = STATUS_POWER_STATE_INVALID;
//		goto SubmitIdleRequestIrp_Exit;
		return STATUS_POWER_STATE_INVALID;
	}

	KeAcquireSpinLock(&DeviceExtension->IdleReqStateLock, &oldIrql);
	if(InterlockedExchange(&DeviceExtension->IdleReqPend, 1)) 
	{
		BulkUsb_DbgPrint(1, ("file bulkdev: Idle request pending..\n"));
		KeReleaseSpinLock(&DeviceExtension->IdleReqStateLock, oldIrql);
//		ntStatus = STATUS_DEVICE_BUSY;
//		goto SubmitIdleRequestIrp_Exit;
		return STATUS_DEVICE_BUSY;
	}

	KeClearEvent(&DeviceExtension->NoIdleReqPendEvent);
	idleCallbackInfo = ExAllocatePool(NonPagedPool, sizeof(struct _USB_IDLE_CALLBACK_INFO));
	if(idleCallbackInfo) 
	{
		idleCallbackInfo->IdleCallback = IdleNotificationCallback;
		idleCallbackInfo->IdleContext = (PVOID)DeviceExtension;
		ASSERT(DeviceExtension->IdleCallbackInfo == NULL);
		DeviceExtension->IdleCallbackInfo = idleCallbackInfo;
		irp = IoAllocateIrp(DeviceExtension->TopOfStackDeviceObject->StackSize, FALSE);
		if(irp == NULL) 
		{
			BulkUsb_DbgPrint(1, ("file bulkdev: cannot build idle request irp\n"));
			KeSetEvent(&DeviceExtension->NoIdleReqPendEvent, IO_NO_INCREMENT, FALSE);
			InterlockedExchange(&DeviceExtension->IdleReqPend, 0);
			KeReleaseSpinLock(&DeviceExtension->IdleReqStateLock, oldIrql);
			ExFreePool(idleCallbackInfo);
//			ntStatus = STATUS_INSUFFICIENT_RESOURCES;
//			goto SubmitIdleRequestIrp_Exit;
			return STATUS_INSUFFICIENT_RESOURCES;
		}

		nextStack = IoGetNextIrpStackLocation(irp);
		nextStack->MajorFunction = IRP_MJ_INTERNAL_DEVICE_CONTROL;
		nextStack->Parameters.DeviceIoControl.IoControlCode = 
					IOCTL_INTERNAL_USB_SUBMIT_IDLE_NOTIFICATION;
		nextStack->Parameters.DeviceIoControl.Type3InputBuffer = idleCallbackInfo;
		nextStack->Parameters.DeviceIoControl.InputBufferLength = 
					sizeof(struct _USB_IDLE_CALLBACK_INFO);
		IoSetCompletionRoutine(irp, 
					IdleNotificationRequestComplete, 
					DeviceExtension, 
					TRUE, 
					TRUE, 
					TRUE);

		DeviceExtension->PendingIdleIrp = irp;
		DeviceExtension->FreeIdleIrpCount = 2;
		KeReleaseSpinLock(&DeviceExtension->IdleReqStateLock, oldIrql);
		if(!CanDeviceSuspend(DeviceExtension) || PowerDeviceD0 != DeviceExtension->DevPower) 
		{
			BulkUsb_DbgPrint(1, ("file bulkdev: Device is not idle\n"));
			KeAcquireSpinLock(&DeviceExtension->IdleReqStateLock, &oldIrql);
			DeviceExtension->IdleCallbackInfo = NULL;
			DeviceExtension->PendingIdleIrp = NULL;
			KeSetEvent(&DeviceExtension->NoIdleReqPendEvent, IO_NO_INCREMENT, FALSE);
			InterlockedExchange(&DeviceExtension->IdleReqPend, 0);
			KeReleaseSpinLock(&DeviceExtension->IdleReqStateLock, oldIrql);
			if(idleCallbackInfo) 
			{
				ExFreePool(idleCallbackInfo);
			}
			if(irp) 
			{
				IoFreeIrp(irp);
			}
//			ntStatus = STATUS_UNSUCCESSFUL;
//			goto SubmitIdleRequestIrp_Exit;
			return STATUS_UNSUCCESSFUL;
		}

		BulkUsb_DbgPrint(3, ("file bulkdev: Cancel the timers\n"));
		KeCancelTimer(&DeviceExtension->Timer);
		ntStatus = IoCallDriver(DeviceExtension->TopOfStackDeviceObject, irp);
		if(!NT_SUCCESS(ntStatus)) 
		{
//			BulkUsb_DbgPrint(1, ("file bulkdev: IoCallDriver failed\n"));
//			goto SubmitIdleRequestIrp_Exit;
			return ntStatus;
		}
	}
	else 
	{
		BulkUsb_DbgPrint(1, ("file bulkdev: Memory allocation for idleCallbackInfo failed\n"));
		KeSetEvent(&DeviceExtension->NoIdleReqPendEvent, IO_NO_INCREMENT, FALSE);
		InterlockedExchange(&DeviceExtension->IdleReqPend, 0);
		KeReleaseSpinLock(&DeviceExtension->IdleReqStateLock, oldIrql);
		ntStatus = STATUS_INSUFFICIENT_RESOURCES;
	}

//SubmitIdleRequestIrp_Exit:
	BulkUsb_DbgPrint(3, ("file bulkdev: SubmitIdleRequest - ends\n"));
	return ntStatus;
}

VOID CancelSelectSuspend(IN PTDeviceExtension DeviceExtension)
{
    PIRP  irp;
    KIRQL oldIrql;

    irp = NULL;
    BulkUsb_DbgPrint(3, ("file bulkdev: CancelSelectSuspend - begins\n"));
    KeAcquireSpinLock(&DeviceExtension->IdleReqStateLock, &oldIrql);
    if(!CanDeviceSuspend(DeviceExtension))
    {
        BulkUsb_DbgPrint(3, ("file bulkdev: Device is not idle\n"));
        irp = (PIRP)InterlockedExchangePointer(&DeviceExtension->PendingIdleIrp, NULL);
    }

    KeReleaseSpinLock(&DeviceExtension->IdleReqStateLock, oldIrql);
    if(irp) 
	{
		IoCancelIrp(irp);
        if(0 == InterlockedDecrement(&DeviceExtension->FreeIdleIrpCount)) 
		{
            BulkUsb_DbgPrint(3, ("file bulkdev: CancelSelectSuspend frees the irp\n"));
            IoFreeIrp(irp);
            KeSetEvent(&DeviceExtension->NoIdleReqPendEvent, IO_NO_INCREMENT, FALSE);
        }
    }

    BulkUsb_DbgPrint(3, ("file bulkdev: CancelSelectSuspend - ends\n"));
    return;
}

//============================================
static NTSTATUS BulkUsb_GetPortStatus(IN PDEVICE_OBJECT DeviceObject, IN OUT PULONG PortStatus)
{
    NTSTATUS           ntStatus;
    KEVENT             event;
    PIRP               irp;
    IO_STATUS_BLOCK    ioStatus;
    PIO_STACK_LOCATION nextStack;
    PTDeviceExtension  deviceExtension;

    deviceExtension = (PTDeviceExtension) DeviceObject->DeviceExtension;
    *PortStatus = 0;
    BulkUsb_DbgPrint(3, ("file bulkdev: BulkUsb_GetPortStatus - begins\n"));
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

    if(NULL == irp) 
	{
        BulkUsb_DbgPrint(1, ("file bulkdev: memory alloc for irp failed\n"));
        return STATUS_INSUFFICIENT_RESOURCES;
    }

    nextStack = IoGetNextIrpStackLocation(irp);
    ASSERT(nextStack != NULL);
    nextStack->Parameters.Others.Argument1 = PortStatus;
    ntStatus = IoCallDriver(deviceExtension->TopOfStackDeviceObject, irp);
    if(STATUS_PENDING == ntStatus) 
	{
        KeWaitForSingleObject(&event, Executive, KernelMode, FALSE, NULL);
    }
    else 
        ioStatus.Status = ntStatus;

    ntStatus = ioStatus.Status;
    BulkUsb_DbgPrint(3, ("file bulkdev: BulkUsb_GetPortStatus - ends\n"));
    return ntStatus;
}

static NTSTATUS BulkUsb_ResetParentPort(IN PDEVICE_OBJECT DeviceObject)
{
    NTSTATUS           ntStatus;
    KEVENT             event;
    PIRP               irp;
    IO_STATUS_BLOCK    ioStatus;
    PIO_STACK_LOCATION nextStack;
    PTDeviceExtension  deviceExtension;

    deviceExtension = (PTDeviceExtension) DeviceObject->DeviceExtension;
    BulkUsb_DbgPrint(3, ("file bulkdev: BulkUsb_ResetParentPort - begins\n"));
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

    if(NULL == irp) 
	{
        BulkUsb_DbgPrint(1, ("file bulkdev: memory alloc for irp failed\n"));
        return STATUS_INSUFFICIENT_RESOURCES;
    }
    nextStack = IoGetNextIrpStackLocation(irp);
    ASSERT(nextStack != NULL);
    ntStatus = IoCallDriver(deviceExtension->TopOfStackDeviceObject, irp);
    if(STATUS_PENDING == ntStatus) 
	{
        KeWaitForSingleObject(&event, Executive, KernelMode, FALSE, NULL);
    }
    else 
        ioStatus.Status = ntStatus;

    ntStatus = ioStatus.Status;
    BulkUsb_DbgPrint(3, ("file bulkdev: BulkUsb_ResetParentPort - ends\n"));
    return ntStatus;
}

NTSTATUS BulkUsb_ResetDevice(IN PDEVICE_OBJECT DeviceObject)
{
    NTSTATUS ntStatus;
    ULONG    portStatus;

    BulkUsb_DbgPrint(3, ("file bulkdev: BulkUsb_ResetDevice - begins\n"));
    ntStatus = BulkUsb_GetPortStatus(DeviceObject, &portStatus);
    if((NT_SUCCESS(ntStatus)) && 
		(!(portStatus & USBD_PORT_ENABLED)) &&
       (portStatus & USBD_PORT_CONNECTED)) 
	{
        ntStatus = BulkUsb_ResetParentPort(DeviceObject);
    }
    BulkUsb_DbgPrint(3, ("file bulkdev: BulkUsb_ResetDevice - ends\n"));

    return ntStatus;
}

//=============================================

NTSTATUS BulkUsb_DispatchCreate(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{
    ULONG                       i;
    NTSTATUS                    ntStatus;
    PFILE_OBJECT                fileObject;
    PTDeviceExtension           deviceExtension;
    PIO_STACK_LOCATION          irpStack;
    PTPipeContext				pipeContext;
    PUSBD_INTERFACE_INFORMATION Interface;

    PAGED_CODE();
	BulkUsb_DbgPrint(3, ("file bulkdev: BulkUsb_DispatchCreate - begins\n"));
    irpStack = IoGetCurrentIrpStackLocation(Irp);
    fileObject = irpStack->FileObject;
    deviceExtension = (PTDeviceExtension) DeviceObject->DeviceExtension;
    if(deviceExtension->DeviceState != WORKING) 
	{
        ntStatus = STATUS_INVALID_DEVICE_STATE;
        goto BulkUsb_DispatchCreate_Exit;
    }

    if(deviceExtension->UsbInterface) 
        Interface = deviceExtension->UsbInterface;
    else 
	{
        BulkUsb_DbgPrint(1, ("file bulkdev: UsbInterface not found\n"));
        ntStatus = STATUS_INVALID_DEVICE_STATE;
        goto BulkUsb_DispatchCreate_Exit;
    }

    if(fileObject) 
        fileObject->FsContext = NULL; 
    else 
	{
        ntStatus = STATUS_INVALID_PARAMETER;
        goto BulkUsb_DispatchCreate_Exit;
    }

    if(0 == fileObject->FileName.Length) 
	{
        ntStatus = STATUS_SUCCESS;
        InterlockedIncrement(&deviceExtension->OpenHandleCount);
        if(deviceExtension->SSEnable) 
		{
            CancelSelectSuspend(deviceExtension);
        }
        goto BulkUsb_DispatchCreate_Exit;
    }
    
    pipeContext = BulkUsb_PipeWithName(DeviceObject, &fileObject->FileName);
    if(pipeContext == NULL) 
	{
        ntStatus = STATUS_INVALID_PARAMETER;
        goto BulkUsb_DispatchCreate_Exit;
    }

    ntStatus = STATUS_INVALID_PARAMETER;
    for(i = 0; i < Interface->NumberOfPipes; i++) 
	{
        if(pipeContext == &deviceExtension->PipeContext[i]) 
		{
            BulkUsb_DbgPrint(3, ("file bulkdev: open pipe %d\n", i));
            fileObject->FsContext = &Interface->Pipes[i];
            ASSERT(fileObject->FsContext);
            pipeContext->PipeOpen = TRUE;
            ntStatus = STATUS_SUCCESS;
            InterlockedIncrement(&deviceExtension->OpenHandleCount);
            if(deviceExtension->SSEnable) 
			{
                CancelSelectSuspend(deviceExtension);
            }
        }
    }

BulkUsb_DispatchCreate_Exit:

    Irp->IoStatus.Status = ntStatus;
    Irp->IoStatus.Information = 0;
    IoCompleteRequest(Irp, IO_NO_INCREMENT);
    BulkUsb_DbgPrint(3, ("file bulkdev: BulkUsb_DispatchCreate - ends\n"));
    return ntStatus;
}

NTSTATUS BulkUsb_DispatchClose(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{
    NTSTATUS				ntStatus;
    PFILE_OBJECT			fileObject;
    PTDeviceExtension		deviceExtension;
    PIO_STACK_LOCATION		irpStack;
    PTPipeContext			pipeContext;
    PUSBD_PIPE_INFORMATION pipeInformation;
    
    PAGED_CODE();

    irpStack = IoGetCurrentIrpStackLocation(Irp);
    fileObject = irpStack->FileObject;
    pipeContext = NULL;
    pipeInformation = NULL;
    deviceExtension = (PTDeviceExtension) DeviceObject->DeviceExtension;
    BulkUsb_DbgPrint(3, ("file bulkdev: BulkUsb_DispatchClose - begins\n"));
    if(fileObject && fileObject->FsContext) 
	{
        pipeInformation = fileObject->FsContext;
        if(0 != fileObject->FileName.Length) 
		{
            pipeContext = BulkUsb_PipeWithName(DeviceObject, &fileObject->FileName);
        }

        if(pipeContext && pipeContext->PipeOpen) 
            pipeContext->PipeOpen = FALSE;
    }
    ntStatus = STATUS_SUCCESS;
    Irp->IoStatus.Status = ntStatus;
    Irp->IoStatus.Information = 0;
    IoCompleteRequest(Irp, IO_NO_INCREMENT);
    InterlockedDecrement(&deviceExtension->OpenHandleCount);
    BulkUsb_DbgPrint(3, ("file bulkdev: BulkUsb_DispatchClose - ends\n"));
    return ntStatus;
}

//====================
NTSTATUS BulkUsb_DispatchDevCtrl(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{
    ULONG              code;
    PVOID              ioBuffer;
    ULONG              inputBufferLength;
    ULONG              outputBufferLength;
    ULONG              info;
    NTSTATUS           ntStatus;
    PTDeviceExtension  deviceExtension;
    PIO_STACK_LOCATION irpStack;

    info = 0;
    irpStack = IoGetCurrentIrpStackLocation(Irp);
    code = irpStack->Parameters.DeviceIoControl.IoControlCode;
    deviceExtension = (PTDeviceExtension) DeviceObject->DeviceExtension;
    ioBuffer           = Irp->AssociatedIrp.SystemBuffer;
    inputBufferLength  = irpStack->Parameters.DeviceIoControl.InputBufferLength;
    outputBufferLength = irpStack->Parameters.DeviceIoControl.OutputBufferLength;
    if(deviceExtension->DeviceState != WORKING) 
	{
        BulkUsb_DbgPrint(1, ("file bulkdev: Invalid device state\n"));
        Irp->IoStatus.Status = ntStatus = STATUS_INVALID_DEVICE_STATE;
        Irp->IoStatus.Information = info;
        IoCompleteRequest(Irp, IO_NO_INCREMENT);
        return ntStatus;
    }
    BulkUsb_DbgPrint(3, ("file bulkdev: BulkUsb_DispatchDevCtrl::"));
    BulkUsb_IoIncrement(deviceExtension);
    BulkUsb_DbgPrint(3, ("file bulkdev: Waiting on the IdleReqPendEvent\n"));
    
    if(deviceExtension->SSEnable) 
	{
        KeWaitForSingleObject(&deviceExtension->NoIdleReqPendEvent, 
                              Executive, 
                              KernelMode, 
                              FALSE, 
                              NULL);
    }

    switch(code) 
	{
    case IOCTL_BULKUSB_RESET_PIPE:
    {
        PFILE_OBJECT           fileObject;
        PUSBD_PIPE_INFORMATION pipe;

        pipe = NULL;
        fileObject = irpStack->FileObject;
        if(fileObject == NULL) 
		{
            ntStatus = STATUS_INVALID_PARAMETER;
            break;
        }

        pipe = (PUSBD_PIPE_INFORMATION) fileObject->FsContext;
        if(pipe == NULL) 
            ntStatus = STATUS_INVALID_PARAMETER;
        else 
            ntStatus = BulkUsb_ResetPipe(DeviceObject, pipe);
        break;
    }

    case IOCTL_BULKUSB_GET_CONFIG_DESCRIPTOR:
    {
        ULONG length;

        if(deviceExtension->UsbConfigurationDescriptor) 
		{
            length = deviceExtension->UsbConfigurationDescriptor->wTotalLength;
            if(outputBufferLength >= length) 
			{
                RtlCopyMemory(ioBuffer, deviceExtension->UsbConfigurationDescriptor, length);
                info = length;
                ntStatus = STATUS_SUCCESS;
            }
            else 
                ntStatus = STATUS_BUFFER_TOO_SMALL;
        }
        else 
            ntStatus = STATUS_UNSUCCESSFUL;
        break;
    }

    case IOCTL_BULKUSB_RESET_DEVICE:
        ntStatus = BulkUsb_ResetDevice(DeviceObject);
        break;

    default :
        ntStatus = STATUS_INVALID_DEVICE_REQUEST;
        break;
    }

    Irp->IoStatus.Status = ntStatus;
    Irp->IoStatus.Information = info;
    IoCompleteRequest(Irp, IO_NO_INCREMENT);
    BulkUsb_DbgPrint(3, ("file bulkdev: BulkUsb_DispatchDevCtrl::"));
    BulkUsb_IoDecrement(deviceExtension);
    return ntStatus;
}





