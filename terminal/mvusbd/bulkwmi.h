#ifndef _BULKUSB_WMI_H
#define _BULKUSB_WMI_H

NTSTATUS BulkUsb_WmiRegistration(IN OUT PTDeviceExtension DeviceExtension);
NTSTATUS BulkUsb_WmiDeRegistration(IN OUT PTDeviceExtension DeviceExtension);
NTSTATUS BulkUsb_DispatchSysCtrl(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp);

#endif
