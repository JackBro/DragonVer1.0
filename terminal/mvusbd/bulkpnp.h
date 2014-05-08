#ifndef _BULKUSB_PNP_H
#define _BULKUSB_PNP_H

#define REMOTE_WAKEUP_MASK 0x20

BOOLEAN CanDeviceSuspend(IN PTDeviceExtension DeviceExtension);
NTSTATUS BulkUsb_GetRegistryDword(IN PWCHAR RegPath, IN PWCHAR ValueName, IN OUT PULONG Value);
VOID DpcRoutine(IN PKDPC Dpc, IN PVOID DeferredContext, 
				IN PVOID SystemArgument1, IN PVOID SystemArgument2);

LONG BulkUsb_IoIncrement(IN OUT PTDeviceExtension DeviceExtension);
LONG BulkUsb_IoDecrement(IN OUT PTDeviceExtension DeviceExtension);

NTSTATUS BulkUsb_DispatchPnP(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp);
VOID ProcessQueuedRequests(IN OUT PTDeviceExtension DeviceExtension);
NTSTATUS CallUSBD(IN PDEVICE_OBJECT DeviceObject, IN PURB Urb);

//=================================
NTSTATUS BulkUsb_DispatchClean(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp);

#endif
