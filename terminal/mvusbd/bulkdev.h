
#ifndef _BULKUSB_DEV_H
#define _BULKUSB_DEV_H

NTSTATUS SubmitIdleRequestIrp(IN PTDeviceExtension DeviceExtension);
VOID CancelSelectSuspend(IN PTDeviceExtension DeviceExtension);

VOID WWIrpCompletionFunc(IN PDEVICE_OBJECT DeviceObject, IN UCHAR MinorFunction,
    IN POWER_STATE PowerState, IN PVOID Context, IN PIO_STATUS_BLOCK IoStatus);

//===================
NTSTATUS BulkUsb_ResetDevice(IN PDEVICE_OBJECT DeviceObject);

//==============================
NTSTATUS BulkUsb_DispatchCreate(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp);
NTSTATUS BulkUsb_DispatchClose(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp);

//================
NTSTATUS BulkUsb_DispatchDevCtrl(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp);

#endif
