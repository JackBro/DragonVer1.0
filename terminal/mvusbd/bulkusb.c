#include "bulkusb.h"
#include "bulkpnp.h"
#include "bulkusr.h"
#include "bulkwmi.h"
#include "bulkrwr.h"
#include "bulkdev.h"
#include "bulkpwr.h"

TUsbdContext gUsbdContext;
ULONG   DebugLevel = 3;

NTSTATUS DriverEntry(IN OUT PDRIVER_OBJECT  DriverObject, IN PUNICODE_STRING UniRegistryPath);
VOID BulkUsb_DriverUnload(IN PDRIVER_OBJECT DriverObject);
NTSTATUS BulkUsb_AddDevice(IN PDRIVER_OBJECT DriverObject, IN PDEVICE_OBJECT PhysicalDeviceObject);

#ifdef PAGE_CODE
#ifdef ALLOC_PRAGMA
#pragma alloc_text(INIT, DriverEntry)
#pragma alloc_text(PAGE, BulkUsb_DriverUnload)
#endif
#endif

NTSTATUS DriverEntry(IN OUT PDRIVER_OBJECT  DriverObject, IN PUNICODE_STRING UniRegistryPath)
{
    NTSTATUS        ntStatus;
    PUNICODE_STRING registryPath;

	BulkUsb_DbgPrint(1, ("file bulkusb: driverentry -- begins\n"));
    registryPath = &gUsbdContext.BulkUsb_RegistryPath;
    registryPath->MaximumLength = UniRegistryPath->Length + sizeof(UNICODE_NULL);
    registryPath->Length        = UniRegistryPath->Length;
    registryPath->Buffer        = ExAllocatePool(PagedPool, registryPath->MaximumLength);
    if (!registryPath->Buffer) 
	{
        BulkUsb_DbgPrint(1, ("Failed to allocate memory for registryPath\n"));
        ntStatus = STATUS_INSUFFICIENT_RESOURCES;
        goto DriverEntry_Exit;
    }
    RtlZeroMemory (registryPath->Buffer, registryPath->MaximumLength);
    RtlMoveMemory (registryPath->Buffer, UniRegistryPath->Buffer, UniRegistryPath->Length);
    ntStatus = STATUS_SUCCESS;

	DriverObject->MajorFunction[IRP_MJ_PNP]		= BulkUsb_DispatchPnP;
    DriverObject->MajorFunction[IRP_MJ_CREATE]	= BulkUsb_DispatchCreate;
    DriverObject->MajorFunction[IRP_MJ_CLOSE]	= BulkUsb_DispatchClose;
    DriverObject->MajorFunction[IRP_MJ_CLEANUP]	= BulkUsb_DispatchClean;
    DriverObject->MajorFunction[IRP_MJ_SYSTEM_CONTROL] = BulkUsb_DispatchSysCtrl;

//=====
    DriverObject->MajorFunction[IRP_MJ_POWER]	= BulkUsb_DispatchPower;
	DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = BulkUsb_DispatchDevCtrl;

//====

	DriverObject->MajorFunction[IRP_MJ_READ]	=
	DriverObject->MajorFunction[IRP_MJ_WRITE]	= BulkUsb_DispatchReadWrite;

	DriverObject->DriverUnload					= BulkUsb_DriverUnload;
    DriverObject->DriverExtension->AddDevice	= (PDRIVER_ADD_DEVICE)BulkUsb_AddDevice;

DriverEntry_Exit:
	BulkUsb_DbgPrint(1, ("file bulkusb: driverentry -- ends\n"));
	return ntStatus;
}

VOID BulkUsb_DriverUnload(IN PDRIVER_OBJECT DriverObject)
{
    PUNICODE_STRING registryPath;

    BulkUsb_DbgPrint(3, ("file bulkusb: BulkUsb_DriverUnload - begins\n"));
    registryPath = &gUsbdContext.BulkUsb_RegistryPath;
    if(registryPath->Buffer) 
	{
        ExFreePool(registryPath->Buffer);
        registryPath->Buffer = NULL;
    }
    BulkUsb_DbgPrint(3, ("file bulkusb: BulkUsb_DriverUnload - ends\n"));
    return;
}

NTSTATUS BulkUsb_AddDevice(IN PDRIVER_OBJECT DriverObject, IN PDEVICE_OBJECT PhysicalDeviceObject)
{
    NTSTATUS          ntStatus;
    PDEVICE_OBJECT    deviceObject;
    PTDeviceExtension deviceExtension;
    POWER_STATE       state;

    BulkUsb_DbgPrint(3, ("file bulkusb: BulkUsb_AddDevice - process\n"));

    deviceObject = NULL;
    ntStatus = IoCreateDevice(
                    DriverObject,                   // our driver object
                    sizeof(TDeviceExtension),       // extension size for us
                    NULL,                           // name for this device
                    FILE_DEVICE_UNKNOWN,
                    FILE_AUTOGENERATED_DEVICE_NAME, // device characteristics
                    FALSE,                          // Not exclusive
                    &deviceObject);                 // Our device object

    if(!NT_SUCCESS(ntStatus)) 
	{
        BulkUsb_DbgPrint(1, ("file bulkusb: Failed to create device object\n"));
        return ntStatus;
    }
    deviceExtension = (PTDeviceExtension) deviceObject->DeviceExtension;
    deviceExtension->FunctionalDeviceObject = deviceObject;
    deviceExtension->PhysicalDeviceObject = PhysicalDeviceObject;
//    deviceObject->Flags |= DO_DIRECT_IO;
    deviceObject->Flags |= DO_BUFFERED_IO;

    KeInitializeSpinLock(&deviceExtension->DevStateLock);
	deviceExtension->DeviceState =  NOTSTARTED;
	deviceExtension->PrevDevState = NOTSTARTED;

    deviceExtension->OpenHandleCount = 0;

    KeInitializeSpinLock(&deviceExtension->IOCountLock);
	deviceExtension->OutStandingIO = 1;

    KeInitializeSpinLock(&deviceExtension->IdleReqStateLock);
    deviceExtension->IdleReqPend = 0;
    deviceExtension->PendingIdleIrp = NULL;

    InitializeListHead(&deviceExtension->NewRequestsQueue);
    KeInitializeSpinLock(&deviceExtension->QueueLock);

    KeInitializeEvent(&deviceExtension->RemoveEvent, SynchronizationEvent, FALSE);
    KeInitializeEvent(&deviceExtension->StopEvent, SynchronizationEvent, TRUE);
									//SynchronizationEvent is defined in wdm.h

    ntStatus = BulkUsb_WmiRegistration(deviceExtension);

    if(!NT_SUCCESS(ntStatus)) 
	{
        BulkUsb_DbgPrint(1, ("file bulkusb: BulkUsb_WmiRegistration failed with %X\n", ntStatus));
        IoDeleteDevice(deviceObject);
        return ntStatus;
    }

    if(PhysicalDeviceObject->Flags & DO_POWER_PAGABLE) 
        deviceObject->Flags |= DO_POWER_PAGABLE;

    deviceExtension->DevPower = PowerDeviceD0;
    deviceExtension->SysPower = PowerSystemWorking;

    state.DeviceState = PowerDeviceD0;	//it's defined in winnt.h
    PoSetPowerState(deviceObject, DevicePowerState, state);	//DevicePowerState is defined in wdm.h

    deviceExtension->TopOfStackDeviceObject = 
			IoAttachDeviceToDeviceStack(deviceObject, PhysicalDeviceObject);

    if(NULL == deviceExtension->TopOfStackDeviceObject) 
	{
        BulkUsb_WmiDeRegistration(deviceExtension);
        IoDeleteDevice(deviceObject);
        return STATUS_NO_SUCH_DEVICE;
    }

    ntStatus = IoRegisterDeviceInterface(deviceExtension->PhysicalDeviceObject, 
                                         &GUID_CLASS_VCMOBI_BULK, 
                                         NULL, 
                                         &deviceExtension->InterfaceName);

    if(!NT_SUCCESS(ntStatus)) 
	{
        BulkUsb_WmiDeRegistration(deviceExtension);
        IoDetachDevice(deviceExtension->TopOfStackDeviceObject);
        IoDeleteDevice(deviceObject);
        return ntStatus;
    }

	if(IoIsWdmVersionAvailable(1, 0x20))
        deviceExtension->WdmVersion = WINXPORBETTER;
    else if(IoIsWdmVersionAvailable(1, 0x10))
        deviceExtension->WdmVersion = WIN2KORBETTER;
    else if(IoIsWdmVersionAvailable(1, 0x5))
        deviceExtension->WdmVersion = WINMEORBETTER;
    else if(IoIsWdmVersionAvailable(1, 0x0))
        deviceExtension->WdmVersion = WIN98ORBETTER;

	deviceExtension->SSRegistryEnable = 0;
    deviceExtension->SSEnable = 0;
    if(WINXPORBETTER == deviceExtension->WdmVersion) 
	{
        BulkUsb_GetRegistryDword(BULKUSB_REGISTRY_PARAMETERS_PATH,
                                 L"VCMobiEnable",
                                 &deviceExtension->SSRegistryEnable);
        if(deviceExtension->SSRegistryEnable) 
		{
            KeInitializeDpc(&deviceExtension->DeferredProcCall, DpcRoutine, deviceObject);
            KeInitializeTimerEx(&deviceExtension->Timer, NotificationTimer);
						//SynchronizationTimer is defined in wdm.h
            KeInitializeEvent(&deviceExtension->NoDpcWorkItemPendingEvent, NotificationEvent, TRUE);
            KeInitializeEvent(&deviceExtension->NoIdleReqPendEvent, NotificationEvent, TRUE);
						//NotificationEvent is defined in wdm.h
        }
	}
    deviceObject->Flags &= ~DO_DEVICE_INITIALIZING;
    BulkUsb_DbgPrint(3, ("file bulkusb: BulkUsb_AddDevice - ends\n"));
    return ntStatus;
}
