#include "bulkusb.h"
#include "bulkpnp.h"
#include "bulkpwr.h"
#include "bulkdev.h"

static VOID WaitWakeCallback(IN PDEVICE_OBJECT DeviceObject, IN UCHAR MinorFunction,
    IN POWER_STATE PowerState, IN PVOID Context, IN PIO_STATUS_BLOCK IoStatus)
{
    NTSTATUS               ntStatus;
    POWER_STATE            powerState;
    PTDeviceExtension      deviceExtension;

    BulkUsb_DbgPrint(3, ("file bulkpwr: WaitWakeCallback - process\n"));
    deviceExtension = (PTDeviceExtension) Context;
    InterlockedExchange(&deviceExtension->FlagWWOutstanding, 0);
    if(!NT_SUCCESS(IoStatus->Status))
        return;

    if(deviceExtension->DevPower == PowerDeviceD0) 
	{
        BulkUsb_DbgPrint(3, ("file bulkpwr: device already powered up...\n"));
        return;
    }

    BulkUsb_DbgPrint(3, ("file bulkpwr: WaitWakeCallback::\n"));
    BulkUsb_IoIncrement(deviceExtension);
    powerState.DeviceState = PowerDeviceD0;
    ntStatus = PoRequestPowerIrp(deviceExtension->PhysicalDeviceObject, 
                                 IRP_MN_SET_POWER, 
                                 powerState, 
                                 (PREQUEST_POWER_COMPLETE) WWIrpCompletionFunc,
                                 deviceExtension, 
                                 NULL);

    if(deviceExtension->WaitWakeEnable) 
	{
        IssueWaitWake(deviceExtension);
    }
    BulkUsb_DbgPrint(3, ("file bulkpwr: WaitWakeCallback - ends\n"));
    return;
}

NTSTATUS IssueWaitWake(IN PTDeviceExtension DeviceExtension)
{
    POWER_STATE poState;
    NTSTATUS    ntStatus;

    BulkUsb_DbgPrint(3, ("file bulkpwr: IssueWaitWake - process\n"));
    if(InterlockedExchange(&DeviceExtension->FlagWWOutstanding, 1)) 
        return STATUS_DEVICE_BUSY;

    InterlockedExchange(&DeviceExtension->FlagWWCancel, 0);
    poState.SystemState = DeviceExtension->DeviceCapabilities.SystemWake;
    ntStatus = PoRequestPowerIrp(DeviceExtension->PhysicalDeviceObject, 
                                 IRP_MN_WAIT_WAKE,
                                 poState, 
                                 (PREQUEST_POWER_COMPLETE) WaitWakeCallback,
                                 DeviceExtension, 
                                 &DeviceExtension->WaitWakeIrp);
    if(!NT_SUCCESS(ntStatus)) 
	{
        InterlockedExchange(&DeviceExtension->FlagWWOutstanding, 0);
    }

    return ntStatus;
}

VOID CancelWaitWake(IN PTDeviceExtension DeviceExtension)
{
    PIRP Irp;

    BulkUsb_DbgPrint(3, ("file bulkpwr: CancelWaitWake - begins\n"));
    Irp = (PIRP) InterlockedExchangePointer(&DeviceExtension->WaitWakeIrp, NULL);
    if(Irp) 
	{
        IoCancelIrp(Irp);
        if(InterlockedExchange(&DeviceExtension->FlagWWCancel, 1)) 
		{
            PoStartNextPowerIrp(Irp);
            Irp->IoStatus.Status = STATUS_CANCELLED;
            Irp->IoStatus.Information = 0;
            IoCompleteRequest(Irp, IO_NO_INCREMENT);
        }    
    }

    BulkUsb_DbgPrint(3, ("file bulkpwr: CancelWaitWake - ends\n"));
}

//================================
static VOID DevPoCompletionRoutine(IN PDEVICE_OBJECT DeviceObject, IN UCHAR MinorFunction,
								   IN POWER_STATE PowerState, IN PVOID Context,
								   IN PIO_STATUS_BLOCK IoStatus)
{
    PIRP                      sIrp;
    PTDeviceExtension         deviceExtension;
    PTPowerCompleteContext powerContext;
    
    powerContext = (PTPowerCompleteContext) Context;
    sIrp = powerContext->SIrp;
    deviceExtension = powerContext->DeviceObject->DeviceExtension;
    BulkUsb_DbgPrint(3, ("file bulkpwr: DevPoCompletionRoutine - begins\n"));
    sIrp->IoStatus.Status = IoStatus->Status;
    PoStartNextPowerIrp(sIrp);
    sIrp->IoStatus.Information = 0;
    IoCompleteRequest(sIrp, IO_NO_INCREMENT);
    BulkUsb_DbgPrint(3, ("file bulkpwr: DevPoCompletionRoutine::"));
    BulkUsb_IoDecrement(deviceExtension);
    ExFreePool(powerContext);
    BulkUsb_DbgPrint(3, ("file bulkpwr: DevPoCompletionRoutine - ends\n"));
}

static VOID SendDeviceIrp(IN PDEVICE_OBJECT DeviceObject, IN PIRP SIrp)
{
    NTSTATUS				ntStatus;
    POWER_STATE				powState;
    PTDeviceExtension		deviceExtension;
    PIO_STACK_LOCATION		irpStack;
    SYSTEM_POWER_STATE		systemState;
    DEVICE_POWER_STATE		devState;
    PTPowerCompleteContext	powerContext;
    
    irpStack = IoGetCurrentIrpStackLocation(SIrp);
    systemState = irpStack->Parameters.Power.State.SystemState;
    deviceExtension = (PTDeviceExtension) DeviceObject->DeviceExtension;
    BulkUsb_DbgPrint(3, ("file bulkpwr: SendDeviceIrp - begins\n"));
    devState = deviceExtension->DeviceCapabilities.DeviceState[systemState];
    powState.DeviceState = devState;
    powerContext = (PTPowerCompleteContext)ExAllocatePool(NonPagedPool, sizeof(TPowerCompleteContext));
    if(!powerContext) 
	{
        BulkUsb_DbgPrint(1, ("file bulkpwr: Failed to alloc memory for powerContext\n"));
        ntStatus = STATUS_INSUFFICIENT_RESOURCES;
    }
    else 
	{
        powerContext->DeviceObject = DeviceObject;
        powerContext->SIrp = SIrp;
        ntStatus = PoRequestPowerIrp(
                            deviceExtension->PhysicalDeviceObject, 
                            irpStack->MinorFunction,
                            powState,
                            (PREQUEST_POWER_COMPLETE)DevPoCompletionRoutine,
                            powerContext, 
                            NULL);
    }

    if(!NT_SUCCESS(ntStatus)) 
	{
        if(powerContext) 
		{
            ExFreePool(powerContext);
        }

        PoStartNextPowerIrp(SIrp);
        SIrp->IoStatus.Status = ntStatus;
        SIrp->IoStatus.Information = 0;
        IoCompleteRequest(SIrp, IO_NO_INCREMENT);
        BulkUsb_DbgPrint(3, ("file bulkpwr: SendDeviceIrp::"));
        BulkUsb_IoDecrement(deviceExtension);
    }
    BulkUsb_DbgPrint(3, ("file bulkpwr: SendDeviceIrp - ends\n"));
}

static NTSTATUS SysPoCompletionRoutine(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp,
									   IN PTDeviceExtension DeviceExtension)
{
    NTSTATUS           ntStatus;
 	PIO_STACK_LOCATION irpStack;

    ntStatus = Irp->IoStatus.Status;
    irpStack = IoGetCurrentIrpStackLocation(Irp);
    BulkUsb_DbgPrint(3, ("file bulkpwr: SysPoCompletionRoutine - begins\n"));
    if(!NT_SUCCESS(ntStatus)) 
	{
        PoStartNextPowerIrp(Irp);
        BulkUsb_DbgPrint(3, ("file bulkpwr: SysPoCompletionRoutine::"));
        BulkUsb_IoDecrement(DeviceExtension);
        return STATUS_SUCCESS;
    }

    if(irpStack->MinorFunction == IRP_MN_SET_POWER) 
        DeviceExtension->SysPower = irpStack->Parameters.Power.State.SystemState;
    SendDeviceIrp(DeviceObject, Irp);
    BulkUsb_DbgPrint(3, ("file bulkpwr: SysPoCompletionRoutine - ends\n"));

    return STATUS_MORE_PROCESSING_REQUIRED;
}


static NTSTATUS HandleSystemSetPower(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{
    NTSTATUS           ntStatus;
    PTDeviceExtension  deviceExtension;
    SYSTEM_POWER_STATE systemState;
    PIO_STACK_LOCATION irpStack;
    
    BulkUsb_DbgPrint(3, ("file bulkpwr: HandleSystemSetPower - begins\n"));
    deviceExtension = (PTDeviceExtension)DeviceObject->DeviceExtension;
    irpStack = IoGetCurrentIrpStackLocation(Irp);
    systemState = irpStack->Parameters.Power.State.SystemState;
    BulkUsb_DbgPrint(3, ("file bulkpwr: Set request for system power state S%X\n"
                         "Current system power state S%X\n",
                         systemState - 1, deviceExtension->SysPower - 1));

    IoCopyCurrentIrpStackLocationToNext(Irp);
    IoSetCompletionRoutine(
            Irp, 
            (PIO_COMPLETION_ROUTINE)SysPoCompletionRoutine,
            deviceExtension, 
            TRUE, 
            TRUE, 
            TRUE);

    ntStatus = PoCallDriver(deviceExtension->TopOfStackDeviceObject, Irp);
    BulkUsb_DbgPrint(3, ("file bulkpwr: HandleSystemSetPower - ends\n"));

    return STATUS_PENDING;
}

static NTSTATUS FinishDevPoDnIrp(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp,
								 IN PTDeviceExtension DeviceExtension)
{
    NTSTATUS           ntStatus;
    POWER_STATE        newState;
    PIO_STACK_LOCATION irpStack;

    ntStatus = Irp->IoStatus.Status;
    irpStack = IoGetCurrentIrpStackLocation(Irp);
    newState = irpStack->Parameters.Power.State;
    BulkUsb_DbgPrint(3, ("file bulkpwr: FinishDevPoDnIrp - begins\n"));
    if(NT_SUCCESS(ntStatus) && irpStack->MinorFunction == IRP_MN_SET_POWER) 
	{
        BulkUsb_DbgPrint(3, ("file bulkpwr: updating cache..\n"));
        DeviceExtension->DevPower = newState.DeviceState;
        PoSetPowerState(DeviceObject, DevicePowerState, newState);
    }
    PoStartNextPowerIrp(Irp);
    BulkUsb_DbgPrint(3, ("file bulkpwr: FinishDevPoDnIrp::"));
    BulkUsb_IoDecrement(DeviceExtension);
    BulkUsb_DbgPrint(3, ("file bulkpwr: FinishDevPoDnIrp - ends\n"));
    return STATUS_SUCCESS;
}

static NTSTATUS SetDeviceFunctional(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp,
									IN PTDeviceExtension DeviceExtension)
{
    KIRQL              oldIrql;
    NTSTATUS           ntStatus;
    POWER_STATE        newState;
    PIO_STACK_LOCATION irpStack;
    DEVICE_POWER_STATE newDevState, oldDevState;

    ntStatus = Irp->IoStatus.Status;
    irpStack = IoGetCurrentIrpStackLocation(Irp);
    newState = irpStack->Parameters.Power.State;
    newDevState = newState.DeviceState;
    oldDevState = DeviceExtension->DevPower;
    BulkUsb_DbgPrint(3, ("file bulkpwr: SetDeviceFunctional - begins\n"));
    DeviceExtension->DevPower = newDevState;
    PoSetPowerState(DeviceObject, DevicePowerState, newState);
    if(PowerDeviceD0 == newDevState) 
	{
        KeAcquireSpinLock(&DeviceExtension->DevStateLock, &oldIrql);
        DeviceExtension->QueueState = ALLOWREQUESTS;
        KeReleaseSpinLock(&DeviceExtension->DevStateLock, oldIrql);
        ProcessQueuedRequests(DeviceExtension);
    }

    PoStartNextPowerIrp(Irp);
    Irp->IoStatus.Status = STATUS_SUCCESS;
    Irp->IoStatus.Information = 0;
    IoCompleteRequest(Irp, IO_NO_INCREMENT);
    BulkUsb_DbgPrint(3, ("file bulkpwr: SetDeviceFunctional::"));
    BulkUsb_IoDecrement(DeviceExtension);
    BulkUsb_DbgPrint(3, ("file bulkpwr: SetDeviceFunctional - ends\n"));
    return STATUS_SUCCESS;
}

static NTSTATUS FinishDevPoUpIrp(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp,
								 IN PTDeviceExtension DeviceExtension)
{
    NTSTATUS           ntStatus;
                        
    ntStatus = Irp->IoStatus.Status;
    BulkUsb_DbgPrint(3, ("file bulkpwr: FinishDevPoUpIrp - begins\n"));
    if(Irp->PendingReturned) 
	{
        IoMarkIrpPending(Irp);
    }

    if(!NT_SUCCESS(ntStatus)) 
	{
        PoStartNextPowerIrp(Irp);
        BulkUsb_DbgPrint(3, ("file bulkpwr: FinishDevPoUpIrp::"));
        BulkUsb_IoDecrement(DeviceExtension);
        return STATUS_SUCCESS;
    }

    SetDeviceFunctional(DeviceObject, Irp, DeviceExtension);
    BulkUsb_DbgPrint(3, ("file bulkpwr: FinishDevPoUpIrp - ends\n"));
    return STATUS_MORE_PROCESSING_REQUIRED;
}

static VOID HoldIoRequestsWorkerRoutine(IN PDEVICE_OBJECT DeviceObject, IN PVOID Context)
{
    PIRP                   irp;
    NTSTATUS               ntStatus;
    PTDeviceExtension      deviceExtension;
    PTWorkThrdContext context;

    BulkUsb_DbgPrint(3, ("file bulkpwr: HoldIoRequestsWorkerRoutine - begins\n"));
    deviceExtension = (PTDeviceExtension) DeviceObject->DeviceExtension;
    context = (PTWorkThrdContext) Context;
    irp = (PIRP) context->Irp;
    BulkUsb_DbgPrint(3, ("file bulkpwr: HoldIoRequestsWorkerRoutine::"));
    BulkUsb_IoDecrement(deviceExtension);
    BulkUsb_DbgPrint(3, ("file bulkpwr: HoldIoRequestsWorkerRoutine::"));
    BulkUsb_IoDecrement(deviceExtension);
    KeWaitForSingleObject(&deviceExtension->StopEvent, Executive, KernelMode, FALSE, NULL);
    BulkUsb_DbgPrint(3, ("file bulkpwr: HoldIoRequestsWorkerRoutine::"));
    BulkUsb_IoIncrement(deviceExtension);
    BulkUsb_DbgPrint(3, ("file bulkpwr: HoldIoRequestsWorkerRoutine::"));
    BulkUsb_IoIncrement(deviceExtension);
    IoCopyCurrentIrpStackLocationToNext(irp);
    IoSetCompletionRoutine(irp, (PIO_COMPLETION_ROUTINE) FinishDevPoDnIrp,
                           deviceExtension, TRUE, TRUE, TRUE);
    ntStatus = PoCallDriver(deviceExtension->TopOfStackDeviceObject, irp);
    if(!NT_SUCCESS(ntStatus)) 
	{
        BulkUsb_DbgPrint(1, ("file bulkpwr: Lower driver fail a power Irp\n"));
    }

    IoFreeWorkItem(context->WorkItem);
    ExFreePool((PVOID)context);
    BulkUsb_DbgPrint(3, ("file bulkpwr: HoldIoRequestsWorkerRoutine - ends\n"));
}

static NTSTATUS HoldIoRequests(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{
    NTSTATUS               ntStatus;
    PIO_WORKITEM           item;
    PTDeviceExtension      deviceExtension;
    PTWorkThrdContext context;

    deviceExtension = (PTDeviceExtension) DeviceObject->DeviceExtension;
    BulkUsb_DbgPrint(3, ("file bulkpwr: HoldIoRequests - begins\n"));
    deviceExtension->QueueState = HOLDREQUESTS;
    context = ExAllocatePool(NonPagedPool, sizeof(TWorkThrdContext));
    if(context) 
	{
        item = IoAllocateWorkItem(DeviceObject);
        context->Irp = Irp;
        context->DeviceObject = DeviceObject;
        context->WorkItem = item;
        if(item) 
		{
            IoMarkIrpPending(Irp);
            IoQueueWorkItem(item, HoldIoRequestsWorkerRoutine, DelayedWorkQueue, context);
            ntStatus = STATUS_PENDING;
        }
        else 
		{
            BulkUsb_DbgPrint(3, ("file bulkpwr: Failed to allocate memory for workitem\n"));
            ExFreePool(context);
            ntStatus = STATUS_INSUFFICIENT_RESOURCES;
        }
    }
    else 
	{
        BulkUsb_DbgPrint(1, ("file bulkpwr: Failed to alloc memory for worker thread context\n"));
        ntStatus = STATUS_INSUFFICIENT_RESOURCES;
    }
    BulkUsb_DbgPrint(3, ("file bulkpwr: HoldIoRequests - ends\n"));
    return ntStatus;
}

static NTSTATUS HandleDeviceSetPower(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{
    KIRQL              oldIrql;
    NTSTATUS           ntStatus;
    POWER_STATE        newState;    
    PIO_STACK_LOCATION irpStack;
    PTDeviceExtension  deviceExtension;
    DEVICE_POWER_STATE newDevState, oldDevState;

    BulkUsb_DbgPrint(3, ("file bulkpwr: HandleDeviceSetPower - begins\n"));
    deviceExtension = (PTDeviceExtension)DeviceObject->DeviceExtension;
    irpStack = IoGetCurrentIrpStackLocation(Irp);
    oldDevState = deviceExtension->DevPower;
    newState = irpStack->Parameters.Power.State;
    newDevState = newState.DeviceState;
    BulkUsb_DbgPrint(3, ("file bulkpwr: Set request for device power state D%X\n"
                         "Current device power state D%X\n",
                         newDevState - 1, deviceExtension->DevPower - 1));
    if(newDevState < oldDevState) 
	{
        BulkUsb_DbgPrint(3, ("file bulkpwr: Adding power to the device\n"));
        IoCopyCurrentIrpStackLocationToNext(Irp);
        IoSetCompletionRoutine(
                Irp, 
                (PIO_COMPLETION_ROUTINE)FinishDevPoUpIrp,
                deviceExtension, 
                TRUE, 
                TRUE, 
                TRUE);
        ntStatus = PoCallDriver(deviceExtension->TopOfStackDeviceObject, Irp);
	}
    else 
	{
        if(PowerDeviceD0 == oldDevState && newDevState > oldDevState) 
		{
            BulkUsb_DbgPrint(3, ("file bulkpwr: Removing power from the device\n"));
            ntStatus = HoldIoRequests(DeviceObject, Irp);
            if(!NT_SUCCESS(ntStatus)) 
			{
                PoStartNextPowerIrp(Irp);
                Irp->IoStatus.Status = ntStatus;
                Irp->IoStatus.Information = 0;
                IoCompleteRequest(Irp, IO_NO_INCREMENT);
                BulkUsb_DbgPrint(3, ("file bulkpwr: HandleDeviceSetPower::"));
                BulkUsb_IoDecrement(deviceExtension);
                return ntStatus;
            }
            else 
                goto HandleDeviceSetPower_Exit;
        }
        else if(PowerDeviceD0 == oldDevState && PowerDeviceD0 == newDevState) 
		{
            BulkUsb_DbgPrint(3, ("file bulkpwr: A SetD0 request\n"));
            KeAcquireSpinLock(&deviceExtension->DevStateLock, &oldIrql);
            deviceExtension->QueueState = ALLOWREQUESTS;
            KeReleaseSpinLock(&deviceExtension->DevStateLock, oldIrql);
            ProcessQueuedRequests(deviceExtension);
        }   
        IoCopyCurrentIrpStackLocationToNext(Irp);
        IoSetCompletionRoutine(
                Irp, 
                (PIO_COMPLETION_ROUTINE) FinishDevPoDnIrp,
                deviceExtension, 
                TRUE, 
                TRUE, 
                TRUE);

        ntStatus = PoCallDriver(deviceExtension->TopOfStackDeviceObject, Irp);
        if(!NT_SUCCESS(ntStatus)) 
		{
            BulkUsb_DbgPrint(1, ("file bulkpwr: Lower drivers failed a power Irp\n"));
        }
    }

HandleDeviceSetPower_Exit:

    BulkUsb_DbgPrint(3, ("file bulkpwr: HandleDeviceSetPower - ends\n"));
    return STATUS_PENDING;
}

static NTSTATUS HandleSystemQueryPower(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{
    NTSTATUS           ntStatus;
    PTDeviceExtension  deviceExtension;
    SYSTEM_POWER_STATE systemState;
    PIO_STACK_LOCATION irpStack;
    
    BulkUsb_DbgPrint(3, ("file bulkpwr: HandleSystemQueryPower - begins\n"));
    deviceExtension = (PTDeviceExtension)DeviceObject->DeviceExtension;
    irpStack = IoGetCurrentIrpStackLocation(Irp);
    systemState = irpStack->Parameters.Power.State.SystemState;
    BulkUsb_DbgPrint(3, ("file bulkpwr: Query for system power state S%X\n"
                         "Current system power state S%X\n",
                         systemState - 1, deviceExtension->SysPower - 1));
    if((systemState > deviceExtension->SysPower) && (deviceExtension->WaitWakeEnable)) 
	{
        IssueWaitWake(deviceExtension);
    }
    IoCopyCurrentIrpStackLocationToNext(Irp);
    IoSetCompletionRoutine(
            Irp, 
            (PIO_COMPLETION_ROUTINE)SysPoCompletionRoutine,
            deviceExtension, 
            TRUE, 
            TRUE, 
            TRUE);

    ntStatus = PoCallDriver(deviceExtension->TopOfStackDeviceObject, Irp);
    BulkUsb_DbgPrint(3, ("file bulkpwr: HandleSystemQueryPower - ends\n"));
    return STATUS_PENDING;
}

static NTSTATUS HandleDeviceQueryPower(PDEVICE_OBJECT DeviceObject,  PIRP Irp)
{
    NTSTATUS           ntStatus;
    PTDeviceExtension  deviceExtension;
    PIO_STACK_LOCATION irpStack;
    DEVICE_POWER_STATE deviceState;

    BulkUsb_DbgPrint(3, ("file bulkpwr: HandleDeviceQueryPower - begins\n"));
    deviceExtension = (PTDeviceExtension) DeviceObject->DeviceExtension;
    irpStack = IoGetCurrentIrpStackLocation(Irp);
    deviceState = irpStack->Parameters.Power.State.DeviceState;
    BulkUsb_DbgPrint(3, ("file bulkpwr: Query for device power state D%X\n"
                         "Current device power state D%X\n",
                         deviceState - 1, deviceExtension->DevPower - 1));

    if(deviceState < deviceExtension->DevPower) 
        ntStatus = STATUS_SUCCESS;
    else 
	{
        ntStatus = HoldIoRequests(DeviceObject, Irp);
        if(STATUS_PENDING == ntStatus) 
            return ntStatus;
    }

    PoStartNextPowerIrp(Irp);
    Irp->IoStatus.Status = ntStatus;
    Irp->IoStatus.Information = 0;
    if(!NT_SUCCESS(ntStatus)) 
	{
        IoCompleteRequest(Irp, IO_NO_INCREMENT);
    }
    else 
	{
        IoSkipCurrentIrpStackLocation(Irp);
        ntStatus = PoCallDriver(deviceExtension->TopOfStackDeviceObject, Irp);
    }

    BulkUsb_DbgPrint(3, ("file bulkpwr: HandleDeviceQueryPower::"));
    BulkUsb_IoDecrement(deviceExtension);
    BulkUsb_DbgPrint(3, ("file bulkpwr: HandleDeviceQueryPower - ends\n"));
    return ntStatus;
}

static NTSTATUS WaitWakeCompletionRoutine(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp,
										  IN PTDeviceExtension DeviceExtension)
{
    BulkUsb_DbgPrint(3, ("file bulkpwr: WaitWakeCompletionRoutine - begins\n"));
    if(Irp->PendingReturned) 
	{
        IoMarkIrpPending(Irp);
    }

    if(InterlockedExchangePointer(&DeviceExtension->WaitWakeIrp, NULL)) 
	{
        PoStartNextPowerIrp(Irp);
        return STATUS_SUCCESS;
    }
    if(InterlockedExchange(&DeviceExtension->FlagWWCancel, 1)) 
	{
        PoStartNextPowerIrp(Irp);
        return STATUS_CANCELLED;
    }

    BulkUsb_DbgPrint(3, ("file bulkpwr: WaitWakeCompletionRoutine - ends\n"));
    return STATUS_MORE_PROCESSING_REQUIRED;
}

NTSTATUS BulkUsb_DispatchPower(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{
    NTSTATUS           ntStatus;
    PIO_STACK_LOCATION irpStack;
    PUNICODE_STRING    tagString;
    PTDeviceExtension  deviceExtension;
	
    irpStack = IoGetCurrentIrpStackLocation(Irp);
    deviceExtension = (PTDeviceExtension)DeviceObject->DeviceExtension;
    if(REMOVED == deviceExtension->DeviceState) 
	{
        PoStartNextPowerIrp(Irp);
        Irp->IoStatus.Status = ntStatus = STATUS_DELETE_PENDING;
        Irp->IoStatus.Information = 0;
        IoCompleteRequest(Irp, IO_NO_INCREMENT);
        return ntStatus;
    }

    if(NOTSTARTED == deviceExtension->DeviceState) 
	{
        PoStartNextPowerIrp(Irp);
        IoSkipCurrentIrpStackLocation(Irp);
        return PoCallDriver(deviceExtension->TopOfStackDeviceObject, Irp);
    }

    BulkUsb_DbgPrint(3, ("file bulkpwr: BulkUsb_DispatchPower::"));
    BulkUsb_IoIncrement(deviceExtension);
    switch(irpStack->MinorFunction) 
	{
    case IRP_MN_SET_POWER:
        IoMarkIrpPending(Irp);
        switch(irpStack->Parameters.Power.Type) 
		{
        case SystemPowerState:
            HandleSystemSetPower(DeviceObject, Irp);
            ntStatus = STATUS_PENDING;
            break;

        case DevicePowerState:
            HandleDeviceSetPower(DeviceObject, Irp);
            ntStatus = STATUS_PENDING;
            break;
        }
        break;

    case IRP_MN_QUERY_POWER:
        IoMarkIrpPending(Irp);
        switch(irpStack->Parameters.Power.Type) 
		{
        case SystemPowerState:
            HandleSystemQueryPower(DeviceObject, Irp);
            ntStatus = STATUS_PENDING;
            break;

        case DevicePowerState:
            HandleDeviceQueryPower(DeviceObject, Irp);
            ntStatus = STATUS_PENDING;
            break;
        }
        break;

    case IRP_MN_WAIT_WAKE:
        IoMarkIrpPending(Irp);
        IoCopyCurrentIrpStackLocationToNext(Irp);
        IoSetCompletionRoutine(
                        Irp,
                        (PIO_COMPLETION_ROUTINE)WaitWakeCompletionRoutine,
                        deviceExtension, 
                        TRUE, 
                        TRUE, 
                        TRUE);
        PoStartNextPowerIrp(Irp);
        ntStatus = PoCallDriver(deviceExtension->TopOfStackDeviceObject, Irp);
        if(!NT_SUCCESS(ntStatus)) 
		{
            BulkUsb_DbgPrint(1, ("file bulkpwr: Lower drivers failed the wait-wake Irp\n"));
        }
        ntStatus = STATUS_PENDING;
        BulkUsb_DbgPrint(3, ("file bulkpwr: IRP_MN_WAIT_WAKE::"));
        BulkUsb_IoDecrement(deviceExtension);
        break;

    case IRP_MN_POWER_SEQUENCE:
    default:
        PoStartNextPowerIrp(Irp);
        IoSkipCurrentIrpStackLocation(Irp);
        ntStatus = PoCallDriver(deviceExtension->TopOfStackDeviceObject, Irp);
        if(!NT_SUCCESS(ntStatus)) 
		{
            BulkUsb_DbgPrint(1, ("file bulkpwr: Lower drivers failed default power Irp\n"));
        }
        BulkUsb_DbgPrint(3, ("file bulkpwr: BulkUsb_DispatchPower::"));
        BulkUsb_IoDecrement(deviceExtension);

        break;
    }

    return ntStatus;
}
