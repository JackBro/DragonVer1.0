#ifndef _MOBI_BULK_H
#define _MOBI_BULK_H


//typedef struct _SUB_BULKRW {
//	PIRP	Irp;
//	PURB	Urb;
//	PMDL	Mdl;
//} SUB_BULKRW, *PSUB_BULKRW;

typedef struct _BULKRW_CONTEXT {
	URB					SubUrb;
	PIRP				SubIrp;
	PIRP				MainIrp;
	PUCHAR				virtualAddress;
#ifdef MOBI_DIRECT_IO
	PMDL				SubMdl;
#else
    PUCHAR				PacketBuffer[MOBIUSB_MAX_TRANSFER_SIZE];
#endif
	ULONG				TransferBytes;
    ULONG				Length;
    ULONG				Lock;
	NTSTATUS			ntStatus;
	ULONG				urbFlag;
	PUSBD_PIPE_INFORMATION	pipeInformation;
	PDEVICE_EXTENSION	DeviceExtension;

//  ULONG				IrpsPending;
//	SUB_BULKRW			SubContext[1];

//	KSPIN_LOCK			SpinLock;
} BULKRW_CONTEXT, * PBULKRW_CONTEXT;


PMOBIUSB_PIPE_CONTEXT
MobiUsb_PipeWithName(
    IN PDEVICE_OBJECT  DeviceObject,
    IN PUNICODE_STRING FileName
    );

NTSTATUS
MobiBulk_DispatchReadWrite(
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP           Irp
    );


NTSTATUS
MobiBulkReadWrite(
	IN PDEVICE_OBJECT			DeviceObject,
	IN PIRP						Irp,
	IN PUSBD_PIPE_INFORMATION	PipeInformation,
 	IN BOOLEAN					Read
   );


NTSTATUS
MobiBulkInitialize(
	IN PDEVICE_OBJECT			DeviceObject,
	IN PUSBD_PIPE_INFORMATION	PipeInformation,
	IN PIRP						Irp,
	IN BOOLEAN					Read,
	IN OUT PBULKRW_CONTEXT*		pRwContext
    );

BOOLEAN DestroyContextStructure(PBULKRW_CONTEXT rwContext);

NTSTATUS MobiBulkReadWriteComplete(
   IN PDEVICE_OBJECT DeviceObject,
   IN PIRP           Irp,
   IN PVOID          Context
);

VOID
MobiBulkCancelReadWrite(
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP           Irp
    );

NTSTATUS
MobiBulkStageComplete(
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP           Irp,
    IN PVOID          Context
    );


#endif
