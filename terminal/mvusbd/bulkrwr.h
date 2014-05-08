
#ifndef _BULKUSB_RWR_H
#define _BULKUSB_RWR_H

typedef struct tag_TUsbdRwContext {
	PIRP			  Irp;
    PURB              Urb;
 	KEVENT			  BulkEvent;
    PVOID			  PacketBuffer;
} TUsbdRwContext, *PTUsbdRwContext;

NTSTATUS BulkUsb_DispatchReadWrite(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp);
NTSTATUS BulkUsb_ResetPipe(IN PDEVICE_OBJECT DeviceObject, IN PUSBD_PIPE_INFORMATION PipeInfo);
PTPipeContext BulkUsb_PipeWithName(IN PDEVICE_OBJECT  DeviceObject, IN PUNICODE_STRING FileName);

#endif
