
#include "bulkusb.h"
#include "bulkwmi.h"
#include "bulkpnp.h"


#define MOFRESOURCENAME L"MofResourceName"

#define WMI_BULKUSB_DRIVER_INFORMATION 0

DEFINE_GUID (BULKUSB_WMI_STD_DATA_GUID, 
0xBBA21300, 0x6DD3, 0x11d2, 0xB8, 0x44, 0x00, 0xC0, 0x4F, 0xAD, 0x51, 0x71);

static const WMIGUIDREGINFO BulkWmiGuidList[] = 
{ 
	{
        &BULKUSB_WMI_STD_DATA_GUID, 1, 0 // driver information
    }
};

static NTSTATUS BulkUsb_QueryWmiRegInfo(IN  PDEVICE_OBJECT  DeviceObject, OUT ULONG *RegFlags,
										OUT PUNICODE_STRING InstanceName, 
										OUT PUNICODE_STRING *RegistryPath,
										OUT PUNICODE_STRING MofResourceName, OUT PDEVICE_OBJECT *Pdo)
{
    PTDeviceExtension deviceExtension;

    PAGED_CODE();
    BulkUsb_DbgPrint(3, ("file bulkwmi: BulkUsb_QueryWmiRegInfo - begins\n"));
    deviceExtension = DeviceObject->DeviceExtension;
    *RegFlags     = WMIREG_FLAG_INSTANCE_PDO;
    *RegistryPath = &gUsbdContext.BulkUsb_RegistryPath;
    *Pdo          = deviceExtension->PhysicalDeviceObject;
    RtlInitUnicodeString(MofResourceName, MOFRESOURCENAME);
    BulkUsb_DbgPrint(3, ("file bulkwmi: BulkUsb_QueryWmiRegInfo - ends\n"));
    return STATUS_SUCCESS;
}

NTSTATUS BulkUsb_QueryWmiDataBlock(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp, IN ULONG GuidIndex,
								   IN ULONG InstanceIndex, IN ULONG InstanceCount, 
								   IN OUT PULONG InstanceLengthArray, IN ULONG OutBufferSize,
								   OUT PUCHAR Buffer)
{
    PTDeviceExtension deviceExtension;
    NTSTATUS          ntStatus;
    ULONG             size;
    WCHAR             modelName[] = L"Aishverya\0\0";
    USHORT            modelNameLen;

    PAGED_CODE();
    BulkUsb_DbgPrint(3, ("file bulkwmi: BulkUsb_QueryWmiDataBlock - begins\n"));
    size = 0;
    modelNameLen = (wcslen(modelName) + 1) * sizeof(WCHAR);

    ASSERT((InstanceIndex == 0) && (InstanceCount == 1));
    
    deviceExtension = (PTDeviceExtension) DeviceObject->DeviceExtension;
    switch (GuidIndex) 
	{
    case WMI_BULKUSB_DRIVER_INFORMATION:
        size = sizeof(ULONG) + modelNameLen + sizeof(USHORT);
        if (OutBufferSize < size ) 
		{
            BulkUsb_DbgPrint(3, ("file bulkwmi: OutBuffer too small\n"));
            ntStatus = STATUS_BUFFER_TOO_SMALL;
            break;
        }
        * (PULONG) Buffer = DebugLevel;
        Buffer += sizeof(ULONG);
        *((PUSHORT)Buffer) = modelNameLen;
        Buffer = (PUCHAR)Buffer + sizeof(USHORT);
        RtlCopyBytes((PVOID)Buffer, (PVOID)modelName, modelNameLen);
        *InstanceLengthArray = size ;
        ntStatus = STATUS_SUCCESS;
        break;

    default:
        ntStatus = STATUS_WMI_GUID_NOT_FOUND;
		break;
    }
    ntStatus = WmiCompleteRequest(DeviceObject, Irp, ntStatus, size, IO_NO_INCREMENT);
    BulkUsb_DbgPrint(3, ("file bulkwmi: BulkUsb_QueryWmiDataBlock - ends\n"));
    return ntStatus;
}

NTSTATUS BulkUsb_SetWmiDataBlock(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp, IN ULONG GuidIndex,
								 IN ULONG InstanceIndex, IN ULONG BufferSize, IN PUCHAR Buffer)
{
    PTDeviceExtension deviceExtension;
    NTSTATUS          ntStatus;
    ULONG             info;

    PAGED_CODE();
    deviceExtension = (PTDeviceExtension) DeviceObject->DeviceExtension;
    info = 0;
    BulkUsb_DbgPrint(3, ("file bulkwmi: BulkUsb_SetWmiDataBlock - begins\n"));
    switch(GuidIndex) 
	{
    case WMI_BULKUSB_DRIVER_INFORMATION:
        if(BufferSize == sizeof(ULONG)) 
		{
            DebugLevel = *(PULONG) Buffer;
            ntStatus = STATUS_SUCCESS;
            info = sizeof(ULONG);
        }
        else 
            ntStatus = STATUS_INFO_LENGTH_MISMATCH;
        break;
    default:
        ntStatus = STATUS_WMI_GUID_NOT_FOUND;
		break;
    }

    ntStatus = WmiCompleteRequest(DeviceObject,
                                Irp,
                                ntStatus,
                                info,
                                IO_NO_INCREMENT);
    BulkUsb_DbgPrint(3, ("file bulkwmi: BulkUsb_SetWmiDataBlock - ends\n"));
    return ntStatus;
}

NTSTATUS BulkUsb_SetWmiDataItem(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp, IN ULONG GuidIndex,
								IN ULONG InstanceIndex, IN ULONG DataItemId, IN ULONG BufferSize,
								IN PUCHAR Buffer)
{
    PTDeviceExtension deviceExtension;
    NTSTATUS          ntStatus;
    ULONG             info;
    
    PAGED_CODE();
    BulkUsb_DbgPrint(3, ("file bulkwmi: BulkUsb_SetWmiDataItem - begins\n"));
    deviceExtension = (PTDeviceExtension) DeviceObject->DeviceExtension;
    info = 0;
    switch(GuidIndex) 
	{
    case WMI_BULKUSB_DRIVER_INFORMATION:
        if(DataItemId == 1) 
		{
            if(BufferSize == sizeof(ULONG)) 
			{
                DebugLevel = *((PULONG)Buffer);
                ntStatus = STATUS_SUCCESS;
                info = sizeof(ULONG);
            }
            else 
                ntStatus = STATUS_INFO_LENGTH_MISMATCH;
        }
        else 
            ntStatus = STATUS_WMI_READ_ONLY;
        break;

    default:
        ntStatus = STATUS_WMI_GUID_NOT_FOUND;
		break;
    }

    ntStatus = WmiCompleteRequest(DeviceObject,
                                Irp,
                                ntStatus,
                                info,
                                IO_NO_INCREMENT);

    BulkUsb_DbgPrint(3, ("file bulkwmi: BulkUsb_SetWmiDataItem - ends\n"));
    return ntStatus;
}

NTSTATUS BulkUsb_WmiRegistration(IN OUT PTDeviceExtension DeviceExtension)
{
    NTSTATUS ntStatus;
    
    PAGED_CODE();
    DeviceExtension->WmiLibInfo.GuidCount = 
          sizeof (BulkWmiGuidList) / sizeof (WMIGUIDREGINFO);
    DeviceExtension->WmiLibInfo.GuidList           = (WMIGUIDREGINFO *)BulkWmiGuidList;
    DeviceExtension->WmiLibInfo.QueryWmiRegInfo    = BulkUsb_QueryWmiRegInfo;
    DeviceExtension->WmiLibInfo.QueryWmiDataBlock  = BulkUsb_QueryWmiDataBlock;
    DeviceExtension->WmiLibInfo.SetWmiDataBlock    = BulkUsb_SetWmiDataBlock;
    DeviceExtension->WmiLibInfo.SetWmiDataItem     = BulkUsb_SetWmiDataItem;
    DeviceExtension->WmiLibInfo.ExecuteWmiMethod   = NULL;
    DeviceExtension->WmiLibInfo.WmiFunctionControl = NULL;
    ntStatus = IoWMIRegistrationControl(DeviceExtension->FunctionalDeviceObject,
                                        WMIREG_ACTION_REGISTER);
    return ntStatus;
}

NTSTATUS BulkUsb_WmiDeRegistration(IN OUT PTDeviceExtension DeviceExtension)
{
    PAGED_CODE();
    return IoWMIRegistrationControl(DeviceExtension->FunctionalDeviceObject,
                                    WMIREG_ACTION_DEREGISTER);
}

//====================

NTSTATUS BulkUsb_DispatchSysCtrl(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{
    PTDeviceExtension       deviceExtension;
    SYSCTL_IRP_DISPOSITION  disposition;
    NTSTATUS                ntStatus;
    PIO_STACK_LOCATION      irpStack;
    
    PAGED_CODE();
    irpStack = IoGetCurrentIrpStackLocation (Irp);
    deviceExtension = (PTDeviceExtension) DeviceObject->DeviceExtension;
    if(REMOVED == deviceExtension->DeviceState) 
	{
        ntStatus = STATUS_DELETE_PENDING;
        Irp->IoStatus.Status = ntStatus;
        Irp->IoStatus.Information = 0;
        IoCompleteRequest(Irp, IO_NO_INCREMENT);
        return ntStatus;
    }

	BulkUsb_DbgPrint(3, ("file bulkwmi: BulkUsb_DispatchSysCtrl::\n"));
    BulkUsb_IoIncrement(deviceExtension);
    ntStatus = WmiSystemControl(&deviceExtension->WmiLibInfo, 
                                DeviceObject, 
                                Irp,
                                &disposition);

    switch(disposition) 
	{
        case IrpProcessed: 
            break;
        
        case IrpNotCompleted:
            IoCompleteRequest(Irp, IO_NO_INCREMENT);                
            break;
      
        case IrpForward:
        case IrpNotWmi:
            IoSkipCurrentIrpStackLocation (Irp);
            ntStatus = IoCallDriver(deviceExtension->TopOfStackDeviceObject, Irp);
            break;
                                    
        default:
            IoSkipCurrentIrpStackLocation (Irp);
            ntStatus = IoCallDriver(deviceExtension->TopOfStackDeviceObject, Irp);
            break;
    }
    BulkUsb_DbgPrint(3, ("file bulkwmi: BulkUsb_DispatchSysCtrl::\n"));
    BulkUsb_IoDecrement(deviceExtension);

    return ntStatus;
}

