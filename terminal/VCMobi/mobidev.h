#ifndef _MOBIUSB_DEV_H
#define _MOBIUSB_DEV_H

NTSTATUS
MobiUsb_DispatchCreate(
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP           Irp
    );

NTSTATUS
MobiUsb_DispatchClose(
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP           Irp
    );

NTSTATUS
MobiUsb_DispatchDevCtrl(
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP           Irp
    );

NTSTATUS
MobiUsb_ResetPipe(
    IN PDEVICE_OBJECT         DeviceObject,
    IN PUSBD_PIPE_INFORMATION PipeInfo
    );

NTSTATUS
MobiUsb_ResetDevice(
    IN PDEVICE_OBJECT DeviceObject
    );

NTSTATUS
MobiUsb_GetPortStatus(
    IN PDEVICE_OBJECT DeviceObject,
    IN PULONG PortStatus
    );

NTSTATUS
MobiUsb_ResetParentPort(
    IN IN PDEVICE_OBJECT DeviceObject
    );

NTSTATUS
SubmitIdleRequestIrp(
    IN PDEVICE_EXTENSION DeviceExtension
    );

VOID
IdleNotificationCallback(
    IN PDEVICE_EXTENSION DeviceExtension
    );

NTSTATUS
IdleNotificationRequestComplete(
    IN PDEVICE_OBJECT    DeviceObject,
    IN PIRP              Irp,
    IN PDEVICE_EXTENSION DeviceExtension
    );

VOID
CancelSelectSuspend(
    IN PDEVICE_EXTENSION DeviceExtension
    );

VOID
PoIrpCompletionFunc(
    IN PDEVICE_OBJECT   DeviceObject,
    IN UCHAR            MinorFunction,
    IN POWER_STATE      PowerState,
    IN PVOID            Context,
    IN PIO_STATUS_BLOCK IoStatus
    );

VOID
PoIrpAsyncCompletionFunc(
    IN PDEVICE_OBJECT DeviceObject,
    IN UCHAR MinorFunction,
    IN POWER_STATE PowerState,
    IN PVOID Context,
    IN PIO_STATUS_BLOCK IoStatus
    );

VOID
WWIrpCompletionFunc(
    IN PDEVICE_OBJECT   DeviceObject,
    IN UCHAR            MinorFunction,
    IN POWER_STATE      PowerState,
    IN PVOID            Context,
    IN PIO_STATUS_BLOCK IoStatus
    );

//Define for test. l_yong 2004.8.16

typedef 
NTSTATUS
(*PCOMMAND_COMPLETE_FUNCTION)(
    PVOID DeviceContext,
    PVOID CommandContext,
    NTSTATUS NtStatus
    );

typedef struct _COMMAND_WORK_ITEM {
    PVOID DeviceContext;
    WORK_QUEUE_ITEM WorkItem;
    UCHAR Request;
    USHORT Value;
    USHORT Index;
    PVOID Buffer;
    OUT PULONG BufferLength;
    BOOLEAN GetData;
    PCOMMAND_COMPLETE_FUNCTION CommandComplete;
    PVOID CommandContext;
} COMMAND_WORK_ITEM, *PCOMMAND_WORK_ITEM;



VOID
USBCAMD_CommandWorkItem(
    PVOID Context
    );

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
    );    
NTSTATUS
USBCAMD_CallUSBD(
    IN PDEVICE_EXTENSION DeviceExtension,
    IN PURB Urb
    );

#define MIN_TRACE 3
#define USBCAMD_KdPrint(a, b) MobiUsb_DbgPrint(a, b)

#endif
