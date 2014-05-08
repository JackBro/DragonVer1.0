#ifndef _BULKUSB_POWER_H
#define _BULKUSB_POWER_H

NTSTATUS IssueWaitWake(IN PTDeviceExtension DeviceExtension);
VOID CancelWaitWake(IN PTDeviceExtension DeviceExtension);

//======
typedef struct tag_TPowerCompleteContext {
    PDEVICE_OBJECT DeviceObject;
    PIRP           SIrp;
} TPowerCompleteContext, *PTPowerCompleteContext;

typedef struct tag_TWorkThrdContext {
    PDEVICE_OBJECT DeviceObject;
    PIRP           Irp;
    PIO_WORKITEM   WorkItem;
} TWorkThrdContext, *PTWorkThrdContext;

NTSTATUS BulkUsb_DispatchPower(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp);

#endif
