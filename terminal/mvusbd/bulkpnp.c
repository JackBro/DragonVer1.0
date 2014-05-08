
#include "bulkusb.h"
#include "bulkpnp.h"
#include "bulkdev.h"
#include "bulkpwr.h"
#include "bulkwmi.h"

NTSTATUS CallUSBD(IN PDEVICE_OBJECT DeviceObject, IN PURB Urb)
{
    PIRP               irp;
    KEVENT             event;
    NTSTATUS           ntStatus;
    IO_STATUS_BLOCK    ioStatus;
    PIO_STACK_LOCATION nextStack;
    PTDeviceExtension  deviceExtension;

    irp = NULL;
    deviceExtension = DeviceObject->DeviceExtension;
    KeInitializeEvent(&event, NotificationEvent, FALSE);	//NotificationEvent is defined in wdm.h
    irp = IoBuildDeviceIoControlRequest(IOCTL_INTERNAL_USB_SUBMIT_URB, 
                                        deviceExtension->TopOfStackDeviceObject,
                                        NULL, 
                                        0, 
                                        NULL, 
                                        0, 
                                        TRUE, 
                                        &event, 
                                        &ioStatus);

    if(!irp) {
        BulkUsb_DbgPrint(1, ("file bulkpnp: IoBuildDeviceIoControlRequest failed\n"));
        return STATUS_INSUFFICIENT_RESOURCES;
    }

    nextStack = IoGetNextIrpStackLocation(irp);
    ASSERT(nextStack != NULL);
    nextStack->Parameters.Others.Argument1 = Urb;
    BulkUsb_DbgPrint(3, ("file bulkpnp: CallUSBD::"));
    BulkUsb_IoIncrement(deviceExtension);
    ntStatus = IoCallDriver(deviceExtension->TopOfStackDeviceObject, irp);
    if(ntStatus == STATUS_PENDING) 
	{
        KeWaitForSingleObject(&event, 
                              Executive,	//it is defined int wdm.h
                              KernelMode,	//it is defined int wdm.h
                              FALSE, 
                              NULL);
        ntStatus = ioStatus.Status;
    }
    BulkUsb_DbgPrint(3, ("file bulkpnp: CallUSBD::"));
    BulkUsb_IoDecrement(deviceExtension);
    return ntStatus;
}

static NTSTATUS SelectInterfaces(IN PDEVICE_OBJECT DeviceObject,
								 IN PUSB_CONFIGURATION_DESCRIPTOR ConfigurationDescriptor)
{
    LONG                        numberOfInterfaces, 
                                interfaceNumber, 
                                interfaceindex;
    ULONG                       i;
    PURB                        urb;
    PUCHAR                      pInf;
    NTSTATUS                    ntStatus;
    PTDeviceExtension           deviceExtension;
    PUSB_INTERFACE_DESCRIPTOR   interfaceDescriptor;
    PUSBD_INTERFACE_LIST_ENTRY  interfaceList, 
                                tmp;
    PUSBD_INTERFACE_INFORMATION Interface;

    urb = NULL;
    Interface = NULL;
    interfaceDescriptor = NULL;
    deviceExtension = DeviceObject->DeviceExtension;
    numberOfInterfaces = ConfigurationDescriptor->bNumInterfaces;
    interfaceindex = interfaceNumber = 0;
    tmp = interfaceList = ExAllocatePool(NonPagedPool, 
				sizeof(USBD_INTERFACE_LIST_ENTRY) * (numberOfInterfaces + 1));
    if(!tmp) 
	{
        BulkUsb_DbgPrint(1, ("file bulkpnp: Failed to allocate mem for interfaceList\n"));
        return STATUS_INSUFFICIENT_RESOURCES;
    }

	while(interfaceNumber < numberOfInterfaces) 
	{
        interfaceDescriptor = USBD_ParseConfigurationDescriptorEx(
                                            ConfigurationDescriptor, 
                                            ConfigurationDescriptor,
                                            interfaceindex,
                                            0, -1, -1, -1);
        if(interfaceDescriptor) 
		{
            interfaceList->InterfaceDescriptor = interfaceDescriptor;
            interfaceList->Interface = NULL;
            interfaceList++;
            interfaceNumber++;
        }
        interfaceindex++;
    }

    interfaceList->InterfaceDescriptor = NULL;
    interfaceList->Interface = NULL;
    urb = USBD_CreateConfigurationRequestEx(ConfigurationDescriptor, tmp);
    if(urb) 
	{
        Interface = &urb->UrbSelectConfiguration.Interface;
        for(i=0; i<Interface->NumberOfPipes; i++) 
            Interface->Pipes[i].MaximumTransferSize = USBD_DEFAULT_MAXIMUM_TRANSFER_SIZE;
        ntStatus = CallUSBD(DeviceObject, urb);
        if(NT_SUCCESS(ntStatus)) 
		{
            deviceExtension->UsbInterface = ExAllocatePool(NonPagedPool, Interface->Length);
            if(deviceExtension->UsbInterface) 
			{
                RtlCopyMemory(deviceExtension->UsbInterface, Interface, Interface->Length);
            }
            else 
			{
                ntStatus = STATUS_INSUFFICIENT_RESOURCES;
                BulkUsb_DbgPrint(1, ("file bulkpnp: memory alloc for UsbInterface failed\n"));
            }
            Interface = &urb->UrbSelectConfiguration.Interface;
            BulkUsb_DbgPrint(3, ("file bulkpnp: ---------\n"));
            BulkUsb_DbgPrint(3, ("file bulkpnp: NumberOfPipes 0x%x\n", Interface->NumberOfPipes));
            BulkUsb_DbgPrint(3, ("file bulkpnp: Length 0x%x\n", Interface->Length));
            BulkUsb_DbgPrint(3, ("file bulkpnp: Alt Setting 0x%x\n", Interface->AlternateSetting));
            BulkUsb_DbgPrint(3, ("file bulkpnp: Interface Number 0x%x\n", Interface->InterfaceNumber));
            BulkUsb_DbgPrint(3, ("file bulkpnp: Class, subclass, protocol 0x%x 0x%x 0x%x\n",
                                 Interface->Class, Interface->SubClass, Interface->Protocol));

            if(Interface->NumberOfPipes) 
			{
                deviceExtension->PipeContext = ExAllocatePool(NonPagedPool,
                                   Interface->NumberOfPipes * sizeof(TPipeContext));
                if(deviceExtension->PipeContext) 
				{
                    for(i=0; i<Interface->NumberOfPipes; i++)
                        deviceExtension->PipeContext[i].PipeOpen = FALSE;
                }
                else 
				{
                    ntStatus = STATUS_INSUFFICIENT_RESOURCES;
                    BulkUsb_DbgPrint(1, ("memory alloc for UsbInterface failed\n"));
                }
            }

            for(i=0; i<Interface->NumberOfPipes; i++) 
			{
                BulkUsb_DbgPrint(3, ("file bulkpnp: ---------\n"));
                BulkUsb_DbgPrint(3, ("file bulkpnp: PipeType 0x%x\n", Interface->Pipes[i].PipeType));
                BulkUsb_DbgPrint(3, ("file bulkpnp: EndpointAddress 0x%x\n", 
									Interface->Pipes[i].EndpointAddress));
                BulkUsb_DbgPrint(3, ("file bulkpnp: MaxPacketSize 0x%x\n", 
                                    Interface->Pipes[i].MaximumPacketSize));
                BulkUsb_DbgPrint(3, ("file bulkpnp: Interval 0x%x\n", Interface->Pipes[i].Interval));
                BulkUsb_DbgPrint(3, ("file bulkpnp: Handle 0x%x\n", Interface->Pipes[i].PipeHandle));
                BulkUsb_DbgPrint(3, ("file bulkpnp: MaximumTransferSize 0x%x\n", 
                                    Interface->Pipes[i].MaximumTransferSize));
            }

            BulkUsb_DbgPrint(3, ("file bulkpnp: ---------\n"));
        }
        else 
		{
            BulkUsb_DbgPrint(1, ("file bulkpnp: Failed to select an interface\n"));
        }
    }
    else 
	{
        BulkUsb_DbgPrint(1, ("file bulkpnp: USBD_CreateConfigurationRequestEx failed\n"));
        ntStatus = STATUS_INSUFFICIENT_RESOURCES;
    }

    if(tmp) 
	{
        ExFreePool(tmp);
    }

    if(urb) 
	{
        ExFreePool(urb);
    }
    return ntStatus;
}

static NTSTATUS ConfigureDevice(IN PDEVICE_OBJECT DeviceObject)
{
    PURB                          urb;
    ULONG                         siz;
    NTSTATUS                      ntStatus;
    PTDeviceExtension             deviceExtension;
    PUSB_CONFIGURATION_DESCRIPTOR configurationDescriptor;

    urb = NULL;
    configurationDescriptor = NULL;
    deviceExtension = DeviceObject->DeviceExtension;
	deviceExtension->UsbConfigurationDescriptor = NULL;
    urb = ExAllocatePool(NonPagedPool, sizeof(struct _URB_CONTROL_DESCRIPTOR_REQUEST));
    if(urb) 
	{
        siz = sizeof(USB_CONFIGURATION_DESCRIPTOR);
        configurationDescriptor = ExAllocatePool(NonPagedPool, siz);
        if(configurationDescriptor) 
		{
            UsbBuildGetDescriptorRequest(
                    urb, 
                    (USHORT) sizeof(struct _URB_CONTROL_DESCRIPTOR_REQUEST),
                    USB_CONFIGURATION_DESCRIPTOR_TYPE, 
                    0, 
                    0, 
                    configurationDescriptor,
                    NULL, 
                    sizeof(USB_CONFIGURATION_DESCRIPTOR), 
                    NULL);
            ntStatus = CallUSBD(DeviceObject, urb);
            if(!NT_SUCCESS(ntStatus)) 
			{
                BulkUsb_DbgPrint(1, ("file bulkpnp: UsbBuildGetDescriptorRequest failed\n"));
				ExFreePool(configurationDescriptor);
				goto ConfigureDevice_Exit;
            }
        }
        else 
		{
            BulkUsb_DbgPrint(1, ("file bulkpnp: Failed to allocate mem for config Descriptor\n"));
            ntStatus = STATUS_INSUFFICIENT_RESOURCES;
            goto ConfigureDevice_Exit;
        }
        siz = configurationDescriptor->wTotalLength;
        ExFreePool(configurationDescriptor);
        configurationDescriptor = ExAllocatePool(NonPagedPool, siz);
        if(configurationDescriptor) 
		{
            UsbBuildGetDescriptorRequest(
                    urb, 
                    (USHORT)sizeof(struct _URB_CONTROL_DESCRIPTOR_REQUEST),
                    USB_CONFIGURATION_DESCRIPTOR_TYPE,
                    0, 
                    0, 
                    configurationDescriptor, 
                    NULL, 
                    siz, 
                    NULL);
            ntStatus = CallUSBD(DeviceObject, urb);
            if(!NT_SUCCESS(ntStatus)) 
			{
                BulkUsb_DbgPrint(1,("file bulkpnp: Failed to read configuration descriptor\n"));
				ExFreePool(configurationDescriptor);
                goto ConfigureDevice_Exit;
            }
        }
        else 
		{
            BulkUsb_DbgPrint(1, ("file bulkpnp: Failed to alloc mem for config Descriptor\n"));
            ntStatus = STATUS_INSUFFICIENT_RESOURCES;
            goto ConfigureDevice_Exit;
        }
    }
    else 
	{
        BulkUsb_DbgPrint(1, ("file bulkpnp: Failed to allocate memory for urb\n"));
        ntStatus = STATUS_INSUFFICIENT_RESOURCES;
        goto ConfigureDevice_Exit;
    }

    if(configurationDescriptor) 
	{
        deviceExtension->UsbConfigurationDescriptor = configurationDescriptor;
        if(configurationDescriptor->bmAttributes & REMOTE_WAKEUP_MASK)
            deviceExtension->WaitWakeEnable = 1;
        else
            deviceExtension->WaitWakeEnable = 0;
        ntStatus = SelectInterfaces(DeviceObject, configurationDescriptor);
    }

ConfigureDevice_Exit:

    if(urb) 
	{
        ExFreePool(urb);
    }

    return ntStatus;
}

static NTSTATUS ReadandSelectDescriptors(IN PDEVICE_OBJECT DeviceObject)
{
    PURB                   urb;
    ULONG                  siz;
    NTSTATUS               ntStatus;
    PUSB_DEVICE_DESCRIPTOR deviceDescriptor;
    
    urb = NULL;
    deviceDescriptor = NULL;
    urb = ExAllocatePool(NonPagedPool, sizeof(struct _URB_CONTROL_DESCRIPTOR_REQUEST));
    if(urb) 
	{
        siz = sizeof(USB_DEVICE_DESCRIPTOR);
        deviceDescriptor = ExAllocatePool(NonPagedPool, siz);
        if(deviceDescriptor) 
		{
            UsbBuildGetDescriptorRequest(
                    urb, 
                    (USHORT) sizeof(struct _URB_CONTROL_DESCRIPTOR_REQUEST),
                    USB_DEVICE_DESCRIPTOR_TYPE, 
                    0, 
                    0, 
                    deviceDescriptor, 
                    NULL, 
                    siz, 
                    NULL);
            ntStatus = CallUSBD(DeviceObject, urb);
            if(NT_SUCCESS(ntStatus)) 
			{
                ASSERT(deviceDescriptor->bNumConfigurations);
                ntStatus = ConfigureDevice(DeviceObject);    
            }
            ExFreePool(urb);                
            ExFreePool(deviceDescriptor);
        }
        else 
		{
            BulkUsb_DbgPrint(1, ("file bulkpnp: Failed to allocate memory for deviceDescriptor\n"));
            ExFreePool(urb);
            ntStatus = STATUS_INSUFFICIENT_RESOURCES;
        }
    }
    else 
	{
        BulkUsb_DbgPrint(1, ("file bulkpnp: Failed to allocate memory for urb\n"));
        ntStatus = STATUS_INSUFFICIENT_RESOURCES;
    }

    return ntStatus;
}

static NTSTATUS IrpCompletionRoutine(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp, IN PVOID Context)
{
    PKEVENT event = Context;
    KeSetEvent(event, 0, FALSE);
    return STATUS_MORE_PROCESSING_REQUIRED;
}

static NTSTATUS HandleStartDevice(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{
    KIRQL             oldIrql;
    KEVENT            startDeviceEvent;
    NTSTATUS          ntStatus;
    PTDeviceExtension deviceExtension;
    LARGE_INTEGER     dueTime;

    BulkUsb_DbgPrint(3, ("file bulkpnp: HandleStartDevice - begins\n"));

	deviceExtension = (PTDeviceExtension) DeviceObject->DeviceExtension;
    deviceExtension->UsbConfigurationDescriptor = NULL;
    deviceExtension->UsbInterface = NULL;
    deviceExtension->PipeContext = NULL;
    KeInitializeEvent(&startDeviceEvent, NotificationEvent, FALSE);
    IoCopyCurrentIrpStackLocationToNext(Irp);
    IoSetCompletionRoutine(Irp, 
						   (PIO_COMPLETION_ROUTINE)IrpCompletionRoutine, 
                           (PVOID)&startDeviceEvent, 
                           TRUE, 
                           TRUE, 
                           TRUE);

    ntStatus = IoCallDriver(deviceExtension->TopOfStackDeviceObject, Irp);
    if(ntStatus == STATUS_PENDING) 
	{
        KeWaitForSingleObject(&startDeviceEvent, 
                              Executive,	//it is defined in wdm.h
                              KernelMode,	//it is defined in wdm.h
                              FALSE, 
                              NULL);
        ntStatus = Irp->IoStatus.Status;
    }

    if(!NT_SUCCESS(ntStatus)) 
	{
        BulkUsb_DbgPrint(1, ("file bulkpnp: Lower drivers failed this Irp\n"));
        return ntStatus;
    }
    ntStatus = ReadandSelectDescriptors(DeviceObject);
    if(!NT_SUCCESS(ntStatus)) 
	{
        BulkUsb_DbgPrint(1, ("file bulkpnp: ReadandSelectDescriptors failed\n"));
        return ntStatus;
    }
    ntStatus = IoSetDeviceInterfaceState(&deviceExtension->InterfaceName, TRUE);
    if(!NT_SUCCESS(ntStatus)) 
	{
        BulkUsb_DbgPrint(1, ("file bulkpnp: IoSetDeviceInterfaceState:enable:failed\n"));
        return ntStatus;
    }

	KeAcquireSpinLock(&deviceExtension->DevStateLock, &oldIrql);
	deviceExtension->PrevDevState =  deviceExtension->DeviceState;
	deviceExtension->DeviceState = WORKING;
    deviceExtension->QueueState = ALLOWREQUESTS;
    KeReleaseSpinLock(&deviceExtension->DevStateLock, oldIrql);

	deviceExtension->FlagWWOutstanding = 0;
    deviceExtension->FlagWWCancel = 0;
    deviceExtension->WaitWakeIrp = NULL;
    if(deviceExtension->WaitWakeEnable) 
	{
        IssueWaitWake(deviceExtension);
    }
    ProcessQueuedRequests(deviceExtension);
    if(WINXPORBETTER == deviceExtension->WdmVersion) 
	{
        deviceExtension->SSEnable = deviceExtension->SSRegistryEnable;
        if(deviceExtension->SSEnable) 
		{
            dueTime.QuadPart = -10000 * IDLE_INTERVAL;               // 5000 ms
            KeSetTimerEx(&deviceExtension->Timer, 
                         dueTime,
                         IDLE_INTERVAL,                              // 5000 ms
                         &deviceExtension->DeferredProcCall);
            deviceExtension->FreeIdleIrpCount = 0;
        }
    }

    BulkUsb_DbgPrint(3, ("file bulkpnp: HandleStartDevice - ends\n"));
    return ntStatus;
}

static NTSTATUS CanStopDevice( IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{
   UNREFERENCED_PARAMETER(DeviceObject);
   UNREFERENCED_PARAMETER(Irp);
   return STATUS_SUCCESS;
}

static NTSTATUS HandleQueryStopDevice(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{
    KIRQL             oldIrql;
    NTSTATUS          ntStatus;
    PTDeviceExtension deviceExtension;

    BulkUsb_DbgPrint(3, ("file bulkpnp: HandleQueryStopDevice - begins\n"));
    deviceExtension = (PTDeviceExtension) DeviceObject->DeviceExtension;
    KeAcquireSpinLock(&deviceExtension->DevStateLock, &oldIrql);
	deviceExtension->PrevDevState =  deviceExtension->DeviceState;
	deviceExtension->DeviceState = PENDINGSTOP;
    deviceExtension->QueueState = HOLDREQUESTS;
    KeReleaseSpinLock(&deviceExtension->DevStateLock, oldIrql);

    BulkUsb_DbgPrint(3, ("file bulkpnp: HandleQueryStopDevice::"));
    BulkUsb_IoDecrement(deviceExtension);
    KeWaitForSingleObject(&deviceExtension->StopEvent, 
                          Executive, 
                          KernelMode, 
                          FALSE, 
                          NULL);

    Irp->IoStatus.Status = STATUS_SUCCESS;
    Irp->IoStatus.Information = 0;
    IoSkipCurrentIrpStackLocation(Irp);
    ntStatus = IoCallDriver(deviceExtension->TopOfStackDeviceObject, Irp);
    BulkUsb_DbgPrint(3, ("file bulkpnp: HandleQueryStopDevice - ends\n"));
    return ntStatus;
}

static NTSTATUS HandleCancelStopDevice(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{
    KIRQL             oldIrql;    
    KEVENT            event;
    NTSTATUS          ntStatus;
    PTDeviceExtension deviceExtension;

    BulkUsb_DbgPrint(3, ("file bulkpnp: HandleCancelStopDevice - begins\n"));
    deviceExtension = (PTDeviceExtension) DeviceObject->DeviceExtension;
    if(PENDINGSTOP == deviceExtension->DeviceState) 
	{
        KeInitializeEvent(&event, NotificationEvent, FALSE);
        IoCopyCurrentIrpStackLocationToNext(Irp);
        IoSetCompletionRoutine(Irp, 
                               (PIO_COMPLETION_ROUTINE)IrpCompletionRoutine, 
                               (PVOID)&event, 
                               TRUE, 
                               TRUE, 
                               TRUE);
        ntStatus = IoCallDriver(deviceExtension->TopOfStackDeviceObject, Irp);
        if(ntStatus == STATUS_PENDING) 
		{
            KeWaitForSingleObject(&event, 
                                  Executive, 
                                  KernelMode, 
                                  FALSE, 
                                  NULL);
            ntStatus = Irp->IoStatus.Status;
        }
        if(NT_SUCCESS(ntStatus)) 
		{
            KeAcquireSpinLock(&deviceExtension->DevStateLock, &oldIrql);
	        deviceExtension->DeviceState = deviceExtension->PrevDevState;
            deviceExtension->QueueState = ALLOWREQUESTS;
            ASSERT(deviceExtension->DeviceState == WORKING);
            KeReleaseSpinLock(&deviceExtension->DevStateLock, oldIrql);
            ProcessQueuedRequests(deviceExtension);
        }
    }
    else
        ntStatus = STATUS_SUCCESS;

    BulkUsb_DbgPrint(3, ("file bulkpnp: HandleCancelStopDevice - ends\n"));
    return ntStatus;
}

static NTSTATUS DeconfigureDevice(IN PDEVICE_OBJECT DeviceObject)
{
    PURB     urb;
    ULONG    siz;
    NTSTATUS ntStatus;
    
    siz = sizeof(struct _URB_SELECT_CONFIGURATION);
    urb = ExAllocatePool(NonPagedPool, siz);
    if(urb) 
	{
        UsbBuildSelectConfigurationRequest(urb, (USHORT)siz, NULL);
        ntStatus = CallUSBD(DeviceObject, urb);
        if(!NT_SUCCESS(ntStatus)) 
		{
            BulkUsb_DbgPrint(3, ("file bulkpnp: file bulkpnp: Failed to deconfigure device\n"));
        }
        ExFreePool(urb);
    }
    else 
	{
        BulkUsb_DbgPrint(1, ("file bulkpnp: Failed to allocate urb\n"));
        ntStatus = STATUS_INSUFFICIENT_RESOURCES;
    }
    return ntStatus;
}

NTSTATUS ReleaseMemory(IN PDEVICE_OBJECT DeviceObject)
{
    PTDeviceExtension deviceExtension;

    deviceExtension = (PTDeviceExtension) DeviceObject->DeviceExtension;
    if(deviceExtension->UsbConfigurationDescriptor) 
	{
        ExFreePool(deviceExtension->UsbConfigurationDescriptor);
        deviceExtension->UsbConfigurationDescriptor = NULL;
    }

    if(deviceExtension->UsbInterface) 
	{
        ExFreePool(deviceExtension->UsbInterface);
        deviceExtension->UsbInterface = NULL;
    }

    if(deviceExtension->PipeContext) 
	{
        ExFreePool(deviceExtension->PipeContext);
        deviceExtension->PipeContext = NULL;
    }

    return STATUS_SUCCESS;
}

static NTSTATUS HandleStopDevice(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{
    KIRQL             oldIrql;
    NTSTATUS          ntStatus;
    PTDeviceExtension deviceExtension;

    BulkUsb_DbgPrint(3, ("file bulkpnp: HandleStopDevice - begins\n"));

    deviceExtension = (PTDeviceExtension) DeviceObject->DeviceExtension;
    if(WINXPORBETTER == deviceExtension->WdmVersion) 
	{
        if(deviceExtension->SSEnable) 
		{
            KeCancelTimer(&deviceExtension->Timer);
            deviceExtension->SSEnable = 0;
            KeWaitForSingleObject(&deviceExtension->NoDpcWorkItemPendingEvent, 
                                  Executive, 
                                  KernelMode, 
                                  FALSE, 
                                  NULL);
            KeWaitForSingleObject(&deviceExtension->NoIdleReqPendEvent, 
                                  Executive, 
                                  KernelMode, 
                                  FALSE, 
                                  NULL);
        }
    }
    if(deviceExtension->WaitWakeEnable) 
	{
        CancelWaitWake(deviceExtension);
    }

    KeAcquireSpinLock(&deviceExtension->DevStateLock, &oldIrql);
	deviceExtension->PrevDevState =  deviceExtension->DeviceState;
	deviceExtension->DeviceState = STOPPED;
    KeReleaseSpinLock(&deviceExtension->DevStateLock, oldIrql);
    ReleaseMemory(DeviceObject);
    ntStatus = DeconfigureDevice(DeviceObject);
    Irp->IoStatus.Status = ntStatus;
    Irp->IoStatus.Information = 0;
    IoSkipCurrentIrpStackLocation(Irp);
    ntStatus = IoCallDriver(deviceExtension->TopOfStackDeviceObject, Irp);
    BulkUsb_DbgPrint(3, ("file bulkpnp: HandleStopDevice - ends\n"));

    return ntStatus;
}

static NTSTATUS CanRemoveDevice(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{
   UNREFERENCED_PARAMETER(DeviceObject);
   UNREFERENCED_PARAMETER(Irp);
   return STATUS_SUCCESS;
}

static NTSTATUS HandleQueryRemoveDevice(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{
    KIRQL             oldIrql;
    NTSTATUS          ntStatus;
    PTDeviceExtension deviceExtension;

    BulkUsb_DbgPrint(3, ("file bulkpnp: HandleQueryRemoveDevice - begins\n"));

    deviceExtension = (PTDeviceExtension) DeviceObject->DeviceExtension;
    ntStatus = CanRemoveDevice(DeviceObject, Irp);

	KeAcquireSpinLock(&deviceExtension->DevStateLock, &oldIrql);
	deviceExtension->PrevDevState =  deviceExtension->DeviceState;
	deviceExtension->DeviceState = PENDINGREMOVE;
	deviceExtension->QueueState = HOLDREQUESTS;
    KeReleaseSpinLock(&deviceExtension->DevStateLock, oldIrql);

    BulkUsb_DbgPrint(3, ("file bulkpnp: HandleQueryRemoveDevice::"));
    BulkUsb_IoDecrement(deviceExtension);

    KeWaitForSingleObject(&deviceExtension->StopEvent, 
                          Executive,
                          KernelMode, 
                          FALSE, 
                          NULL);

    Irp->IoStatus.Status = STATUS_SUCCESS;
    Irp->IoStatus.Information = 0;
    IoSkipCurrentIrpStackLocation(Irp);
    ntStatus = IoCallDriver(deviceExtension->TopOfStackDeviceObject, Irp);
    BulkUsb_DbgPrint(3, ("file bulkpnp: HandleQueryRemoveDevice - ends\n"));

    return ntStatus;
}

static NTSTATUS HandleCancelRemoveDevice(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{
    KIRQL             oldIrql;
    KEVENT            event;
    NTSTATUS          ntStatus;
    PTDeviceExtension deviceExtension;

    BulkUsb_DbgPrint(3, ("file bulkpnp: HandleCancelRemoveDevice - begins\n"));
    deviceExtension = (PTDeviceExtension) DeviceObject->DeviceExtension;
    if(PENDINGREMOVE == deviceExtension->DeviceState) 
	{
        KeInitializeEvent(&event, NotificationEvent, FALSE);
        IoCopyCurrentIrpStackLocationToNext(Irp);
        IoSetCompletionRoutine(Irp, 
                               (PIO_COMPLETION_ROUTINE)IrpCompletionRoutine, 
                               (PVOID)&event, 
                               TRUE, 
                               TRUE, 
                               TRUE);
        ntStatus = IoCallDriver(deviceExtension->TopOfStackDeviceObject, Irp);
        if(ntStatus == STATUS_PENDING) 
		{
            KeWaitForSingleObject(&event, 
                                  Executive, 
                                  KernelMode, 
                                  FALSE, 
                                  NULL);
            ntStatus = Irp->IoStatus.Status;
        }

        if(NT_SUCCESS(ntStatus)) 
		{
            KeAcquireSpinLock(&deviceExtension->DevStateLock, &oldIrql);
            deviceExtension->QueueState = ALLOWREQUESTS;
			deviceExtension->DeviceState =  deviceExtension->PrevDevState;
            KeReleaseSpinLock(&deviceExtension->DevStateLock, oldIrql);
			ProcessQueuedRequests(deviceExtension);
        }
    }
    else 
        ntStatus = STATUS_SUCCESS;
    BulkUsb_DbgPrint(3, ("file bulkpnp: HandleCancelRemoveDevice - ends\n"));
    return ntStatus;
}

static NTSTATUS BulkUsb_AbortPipes(IN PDEVICE_OBJECT DeviceObject)
{
    PURB                        urb;
    ULONG                       i;
    NTSTATUS                    ntStatus;
    PTDeviceExtension           deviceExtension;
    PTPipeContext       pipeContext;
    PUSBD_PIPE_INFORMATION      pipeInformation;
    PUSBD_INTERFACE_INFORMATION interfaceInfo;

    deviceExtension = (PTDeviceExtension) DeviceObject->DeviceExtension;
    pipeContext = deviceExtension->PipeContext;
    interfaceInfo = deviceExtension->UsbInterface;
    BulkUsb_DbgPrint(3, ("file bulkpnp: BulkUsb_AbortPipes - begins\n"));
    if(interfaceInfo == NULL || pipeContext == NULL)
        return STATUS_SUCCESS;

    for(i = 0; i < interfaceInfo->NumberOfPipes; i++) 
	{
        if(pipeContext[i].PipeOpen) 
		{
            BulkUsb_DbgPrint(3, ("file bulkpnp: Aborting open pipe %d\n", i));
            urb = ExAllocatePool(NonPagedPool, sizeof(struct _URB_PIPE_REQUEST));
            if(urb) 
			{
                urb->UrbHeader.Length = sizeof(struct _URB_PIPE_REQUEST);
                urb->UrbHeader.Function = URB_FUNCTION_ABORT_PIPE;
                urb->UrbPipeRequest.PipeHandle = interfaceInfo->Pipes[i].PipeHandle;
                ntStatus = CallUSBD(DeviceObject, urb);
                ExFreePool(urb);
            }
            else 
			{
                BulkUsb_DbgPrint(1, ("file bulkpnp: Failed to alloc memory for urb\n"));
                return STATUS_INSUFFICIENT_RESOURCES;
            }

            if(NT_SUCCESS(ntStatus)) 
                pipeContext[i].PipeOpen = FALSE;
        }
    }

    BulkUsb_DbgPrint(3, ("file bulkpnp: BulkUsb_AbortPipes - ends\n"));
    return STATUS_SUCCESS;
}

static NTSTATUS HandleSurpriseRemoval(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{
    KIRQL             oldIrql;
    NTSTATUS          ntStatus;
    PTDeviceExtension deviceExtension;

    BulkUsb_DbgPrint(3, ("file bulkpnp: HandleSurpriseRemoval - begins\n"));
    deviceExtension = (PTDeviceExtension) DeviceObject->DeviceExtension;
    if(deviceExtension->WaitWakeEnable) 
	{
        CancelWaitWake(deviceExtension);
    }

    if(WINXPORBETTER == deviceExtension->WdmVersion) 
	{
        if(deviceExtension->SSEnable) 
		{
            KeCancelTimer(&deviceExtension->Timer);
            deviceExtension->SSEnable = 0;
            KeWaitForSingleObject(&deviceExtension->NoDpcWorkItemPendingEvent, 
                                  Executive, 
                                  KernelMode, 
                                  FALSE, 
                                  NULL);
            KeWaitForSingleObject(&deviceExtension->NoIdleReqPendEvent, 
                                  Executive, 
                                  KernelMode, 
                                  FALSE, 
                                  NULL);
        }
    }

    KeAcquireSpinLock(&deviceExtension->DevStateLock, &oldIrql);
    deviceExtension->QueueState = FAILREQUESTS;
	deviceExtension->PrevDevState =  deviceExtension->DeviceState;
	deviceExtension->DeviceState = SURPRISEREMOVED;
    KeReleaseSpinLock(&deviceExtension->DevStateLock, oldIrql);

    ProcessQueuedRequests(deviceExtension);
    ntStatus = IoSetDeviceInterfaceState(&deviceExtension->InterfaceName, FALSE);

    if(!NT_SUCCESS(ntStatus)) 
	{
        BulkUsb_DbgPrint(1, ("file bulkpnp: IoSetDeviceInterfaceState::disable:failed\n"));
    }

    RtlFreeUnicodeString(&deviceExtension->InterfaceName);
    BulkUsb_AbortPipes(DeviceObject);

	Irp->IoStatus.Status = STATUS_SUCCESS;
    Irp->IoStatus.Information = 0;

    IoSkipCurrentIrpStackLocation(Irp);
    ntStatus = IoCallDriver(deviceExtension->TopOfStackDeviceObject, Irp);

    BulkUsb_DbgPrint(3, ("file bulkpnp: HandleSurpriseRemoval - ends\n"));

    return ntStatus;
}

static NTSTATUS HandleRemoveDevice(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{
    KIRQL             oldIrql;
    KEVENT            event;
    ULONG             requestCount;
    NTSTATUS          ntStatus;
    PTDeviceExtension deviceExtension;

    BulkUsb_DbgPrint(3, ("file bulkpnp: HandleRemoveDevice - begins\n"));

    deviceExtension = (PTDeviceExtension) DeviceObject->DeviceExtension;
    if(SURPRISEREMOVED != deviceExtension->DeviceState) 
	{
        KeAcquireSpinLock(&deviceExtension->DevStateLock, &oldIrql);
        deviceExtension->QueueState = FAILREQUESTS;
        KeReleaseSpinLock(&deviceExtension->DevStateLock, oldIrql);
        if(deviceExtension->WaitWakeEnable) 
            CancelWaitWake(deviceExtension);

        if(WINXPORBETTER == deviceExtension->WdmVersion) 
		{
            if(deviceExtension->SSEnable) 
			{
                KeCancelTimer(&deviceExtension->Timer);
                deviceExtension->SSEnable = 0;
                KeWaitForSingleObject(&deviceExtension->NoDpcWorkItemPendingEvent, 
                                      Executive, 
                                      KernelMode, 
                                      FALSE, 
                                      NULL);
                KeWaitForSingleObject(&deviceExtension->NoIdleReqPendEvent, 
                                      Executive, 
                                      KernelMode, 
                                      FALSE, 
                                      NULL);
            }
        }

        ProcessQueuedRequests(deviceExtension);
        ntStatus = IoSetDeviceInterfaceState(&deviceExtension->InterfaceName, FALSE);
        if(!NT_SUCCESS(ntStatus)) 
		{
            BulkUsb_DbgPrint(1, ("file bulkpnp: IoSetDeviceInterfaceState::disable:failed\n"));
        }
        RtlFreeUnicodeString(&deviceExtension->InterfaceName);
        BulkUsb_AbortPipes(DeviceObject);
    }

    KeAcquireSpinLock(&deviceExtension->DevStateLock, &oldIrql);
	deviceExtension->PrevDevState =  deviceExtension->DeviceState;
	deviceExtension->DeviceState = REMOVED;
    KeReleaseSpinLock(&deviceExtension->DevStateLock, oldIrql);
    BulkUsb_WmiDeRegistration(deviceExtension);

	BulkUsb_DbgPrint(3, ("file bulkpnp: HandleRemoveDevice::"));
    requestCount = BulkUsb_IoDecrement(deviceExtension);
    ASSERT(requestCount > 0);
    BulkUsb_DbgPrint(3, ("file bulkpnp: HandleRemoveDevice::"));
    requestCount = BulkUsb_IoDecrement(deviceExtension);
    KeWaitForSingleObject(&deviceExtension->RemoveEvent, 
                          Executive, 
                          KernelMode, 
                          FALSE, 
                          NULL);

    ReleaseMemory(DeviceObject);
    Irp->IoStatus.Status = STATUS_SUCCESS;
    Irp->IoStatus.Information = 0;

    IoSkipCurrentIrpStackLocation(Irp);
    ntStatus = IoCallDriver(deviceExtension->TopOfStackDeviceObject, Irp);
    IoDetachDevice(deviceExtension->TopOfStackDeviceObject);
    IoDeleteDevice(DeviceObject);

    BulkUsb_DbgPrint(3, ("file bulkpnp: HandleRemoveDevice - ends\n"));

    return ntStatus;
}

static NTSTATUS HandleQueryCapabilities(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{
    ULONG                i;
    KEVENT               event;
    NTSTATUS             ntStatus;
    PTDeviceExtension    deviceExtension;
    PDEVICE_CAPABILITIES pdc;
    PIO_STACK_LOCATION   irpStack;

    BulkUsb_DbgPrint(3, ("file bulkpnp: HandleQueryCapabilities - begins\n"));

    irpStack = IoGetCurrentIrpStackLocation(Irp);
    deviceExtension = (PTDeviceExtension) DeviceObject->DeviceExtension;
    pdc = irpStack->Parameters.DeviceCapabilities.Capabilities;
    if(pdc->Version < 1 || pdc->Size < sizeof(DEVICE_CAPABILITIES)) 
	{
        BulkUsb_DbgPrint(1, ("file bulkpnp: HandleQueryCapabilities::request failed\n"));
        return STATUS_UNSUCCESSFUL;
    }

    pdc->SurpriseRemovalOK = TRUE;
    Irp->IoStatus.Status = STATUS_SUCCESS;
    KeInitializeEvent(&event, NotificationEvent, FALSE);
    IoCopyCurrentIrpStackLocationToNext(Irp);
    IoSetCompletionRoutine(Irp, 
                           (PIO_COMPLETION_ROUTINE)IrpCompletionRoutine, 
                           (PVOID)&event, 
                           TRUE, 
                           TRUE, 
                           TRUE);
    ntStatus = IoCallDriver(deviceExtension->TopOfStackDeviceObject, Irp);

    if(ntStatus == STATUS_PENDING) 
	{
        KeWaitForSingleObject(&event, 
                              Executive, 
                              KernelMode, 
                              FALSE, 
                              NULL);
        ntStatus = Irp->IoStatus.Status;
    }

    deviceExtension->PowerDownLevel = PowerDeviceUnspecified;
    if(NT_SUCCESS(ntStatus)) 
	{
        deviceExtension->DeviceCapabilities = *pdc;
        for(i = PowerSystemSleeping1; i <= PowerSystemSleeping3; i++) 
		{
            if(deviceExtension->DeviceCapabilities.DeviceState[i] < PowerDeviceD3) 
                deviceExtension->PowerDownLevel = deviceExtension->DeviceCapabilities.DeviceState[i];
        }

        pdc->SurpriseRemovalOK = 1;
    }

    if(deviceExtension->PowerDownLevel == PowerDeviceUnspecified ||
			deviceExtension->PowerDownLevel <= PowerDeviceD0) 
        deviceExtension->PowerDownLevel = PowerDeviceD2;

    BulkUsb_DbgPrint(3, ("file bulkpnp: HandleQueryCapabilities - ends\n"));
    return ntStatus;
}

NTSTATUS BulkUsb_DispatchPnP(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{
    PIO_STACK_LOCATION irpStack;
    PTDeviceExtension  deviceExtension;
    KEVENT             startDeviceEvent;
    NTSTATUS           ntStatus;

    irpStack = IoGetCurrentIrpStackLocation(Irp);
    deviceExtension = DeviceObject->DeviceExtension;
    if(REMOVED == deviceExtension->DeviceState) 
	{
        ntStatus = STATUS_DELETE_PENDING;
        Irp->IoStatus.Status = ntStatus;
        Irp->IoStatus.Information = 0;
        IoCompleteRequest(Irp, IO_NO_INCREMENT);
        return ntStatus;
    }
    BulkUsb_DbgPrint(3, ("file bulkpnp: BulkUsb_DispatchPnP process\n"));
    BulkUsb_IoIncrement(deviceExtension);
    if(irpStack->MinorFunction == IRP_MN_START_DEVICE) 
	{
        ASSERT(deviceExtension->IdleReqPend == 0);
    }
    else 
	{
        if(deviceExtension->SSEnable) 
		{
            CancelSelectSuspend(deviceExtension);
        }
    }

    //BulkUsb_DbgPrint(2, (PnPMinorFunctionString(irpStack->MinorFunction)));
    switch(irpStack->MinorFunction) 
	{
    case IRP_MN_START_DEVICE:
        ntStatus = HandleStartDevice(DeviceObject, Irp);
        break;

    case IRP_MN_QUERY_STOP_DEVICE:
        ntStatus = CanStopDevice(DeviceObject, Irp);
        if(NT_SUCCESS(ntStatus)) 
		{
            ntStatus = HandleQueryStopDevice(DeviceObject, Irp);
            return ntStatus;
        }
        break;

    case IRP_MN_CANCEL_STOP_DEVICE:
        ntStatus = HandleCancelStopDevice(DeviceObject, Irp);
        break;
     
    case IRP_MN_STOP_DEVICE:
        ntStatus = HandleStopDevice(DeviceObject, Irp);
        BulkUsb_DbgPrint(3, ("file bulkpnp: BulkUsb_DispatchPnP::IRP_MN_STOP_DEVICE::\n"));
        BulkUsb_IoDecrement(deviceExtension);
        return ntStatus;

    case IRP_MN_QUERY_REMOVE_DEVICE:
        ntStatus = HandleQueryRemoveDevice(DeviceObject, Irp);
        return ntStatus;

    case IRP_MN_CANCEL_REMOVE_DEVICE:
        ntStatus = HandleCancelRemoveDevice(DeviceObject, Irp);
        break;

    case IRP_MN_SURPRISE_REMOVAL:
        ntStatus = HandleSurpriseRemoval(DeviceObject, Irp);
        BulkUsb_DbgPrint(3, ("file bulkpnp: BulkUsb_DispatchPnP::IRP_MN_SURPRISE_REMOVAL::\n"));
        BulkUsb_IoDecrement(deviceExtension);
        return ntStatus;

    case IRP_MN_REMOVE_DEVICE:
        ntStatus = HandleRemoveDevice(DeviceObject, Irp);
        return ntStatus;

    case IRP_MN_QUERY_CAPABILITIES:
        ntStatus = HandleQueryCapabilities(DeviceObject, Irp);
        break;

    default:
        IoSkipCurrentIrpStackLocation(Irp);
        ntStatus = IoCallDriver(deviceExtension->TopOfStackDeviceObject, Irp);
        BulkUsb_DbgPrint(3, ("file bulkpnp: BulkUsb_DispatchPnP::default::\n"));
        BulkUsb_IoDecrement(deviceExtension);
        return ntStatus;
    } // switch

    Irp->IoStatus.Status = ntStatus;
    Irp->IoStatus.Information = 0;
    IoCompleteRequest(Irp, IO_NO_INCREMENT);
    BulkUsb_DbgPrint(3, ("file bulkpnp: BulkUsb_DispatchPnP ends\n"));
    BulkUsb_IoDecrement(deviceExtension);

    return ntStatus;
}

NTSTATUS BulkUsb_GetRegistryDword(IN PWCHAR RegPath, IN PWCHAR ValueName, IN OUT PULONG Value)
{
    ULONG                    defaultData;
    WCHAR                    buffer[MAXIMUM_FILENAME_LENGTH];
    NTSTATUS                 ntStatus;
    UNICODE_STRING           regPath;
    RTL_QUERY_REGISTRY_TABLE paramTable[2];

    BulkUsb_DbgPrint(3, ("file bulkpnp: BulkUsb_GetRegistryDword - process\n"));

    regPath.Length = 0;
    regPath.MaximumLength = MAXIMUM_FILENAME_LENGTH * sizeof(WCHAR);
    regPath.Buffer = buffer;

    RtlZeroMemory(regPath.Buffer, regPath.MaximumLength);
    RtlMoveMemory(regPath.Buffer, RegPath, wcslen(RegPath) * sizeof(WCHAR));
    RtlZeroMemory(paramTable, sizeof(paramTable));

    paramTable[0].Flags = RTL_QUERY_REGISTRY_DIRECT;
    paramTable[0].Name = ValueName;
    paramTable[0].EntryContext = Value;
    paramTable[0].DefaultType = REG_DWORD;
    paramTable[0].DefaultData = &defaultData;
    paramTable[0].DefaultLength = sizeof(ULONG);

    ntStatus = RtlQueryRegistryValues(RTL_REGISTRY_ABSOLUTE | RTL_REGISTRY_OPTIONAL,
                                      regPath.Buffer,
                                      paramTable,
                                      NULL,
                                      NULL);

    if(NT_SUCCESS(ntStatus))
	{
        BulkUsb_DbgPrint(3, ("file bulkpnp: success Value = %X\n", *Value));
        return STATUS_SUCCESS;
	}
	*Value = 0;
	return STATUS_UNSUCCESSFUL;
}

//=======================================================

BOOLEAN CanDeviceSuspend(IN PTDeviceExtension DeviceExtension)
{
    BulkUsb_DbgPrint(3, ("file bulkpnp: CanDeviceSuspend\n"));
    if((DeviceExtension->OpenHandleCount == 0) &&
        (DeviceExtension->OutStandingIO == 1)) 
        return TRUE;
	return FALSE;
}

static VOID IdleRequestWorkerRoutine(IN PDEVICE_OBJECT DeviceObject, IN PVOID Context)
{
    PIRP                   irp;
    NTSTATUS               ntStatus;
    PTDeviceExtension      deviceExtension;
    PIO_WORKITEM           workItem;

    BulkUsb_DbgPrint(3, ("file bulkpnp: IdleRequestWorkerRoutine - begins\n"));
    deviceExtension = (PTDeviceExtension) DeviceObject->DeviceExtension;
    workItem = (PIO_WORKITEM) Context;

    if(CanDeviceSuspend(deviceExtension)) 
	{
        BulkUsb_DbgPrint(3, ("file bulkpnp: Device is idle\n"));
        ntStatus = SubmitIdleRequestIrp(deviceExtension);
        if(!NT_SUCCESS(ntStatus)) 
		{
            BulkUsb_DbgPrint(1, ("file bulkpnp: SubmitIdleRequestIrp failed\n"));
        }
    }
    else 
	{
        BulkUsb_DbgPrint(3, ("file bulkpnp: Device is not idle\n"));
    }

    IoFreeWorkItem(workItem);

    KeSetEvent(&deviceExtension->NoDpcWorkItemPendingEvent, IO_NO_INCREMENT, FALSE);
    BulkUsb_DbgPrint(3, ("file bulkpnp: IdleRequestsWorkerRoutine - ends\n"));
}

VOID DpcRoutine(IN PKDPC Dpc, IN PVOID DeferredContext, 
				IN PVOID SystemArgument1, IN PVOID SystemArgument2)
{
    NTSTATUS          ntStatus;
    PDEVICE_OBJECT    deviceObject;
    PTDeviceExtension deviceExtension;
    PIO_WORKITEM      item;

    BulkUsb_DbgPrint(3, ("file bulkpnp: DpcRoutine - begins\n"));
    deviceObject = (PDEVICE_OBJECT)DeferredContext;
    deviceExtension = (PTDeviceExtension)deviceObject->DeviceExtension;
    KeClearEvent(&deviceExtension->NoDpcWorkItemPendingEvent);
    if(CanDeviceSuspend(deviceExtension)) 
	{
        BulkUsb_DbgPrint(3, ("file bulkpnp: Device is Idle\n"));
        item = IoAllocateWorkItem(deviceObject);
        if(item) 
		{
            IoQueueWorkItem(item, IdleRequestWorkerRoutine, DelayedWorkQueue, item);
							//DelayedWorkQueue is defined in wdm.h
            ntStatus = STATUS_PENDING;
        }
        else 
		{
            BulkUsb_DbgPrint(3, ("file bulkpnp: Cannot alloc memory for work item\n"));
            ntStatus = STATUS_INSUFFICIENT_RESOURCES;
            KeSetEvent(&deviceExtension->NoDpcWorkItemPendingEvent, IO_NO_INCREMENT, FALSE);
        }
    }
    else 
	{
        BulkUsb_DbgPrint(3, ("file bulkpnp: Idle event not signaled\n"));
        KeSetEvent(&deviceExtension->NoDpcWorkItemPendingEvent, IO_NO_INCREMENT, FALSE);
    }

    BulkUsb_DbgPrint(3, ("file bulkpnp: DpcRoutine - ends\n"));
}    
//=================================================

LONG BulkUsb_IoIncrement(IN OUT PTDeviceExtension DeviceExtension)
{
    LONG  result = 0;
    KIRQL oldIrql;

    KeAcquireSpinLock(&DeviceExtension->IOCountLock, &oldIrql);
    result = InterlockedIncrement(&DeviceExtension->OutStandingIO);
    if(result == 2) 
	{
        KeClearEvent(&DeviceExtension->StopEvent);
    }
    KeReleaseSpinLock(&DeviceExtension->IOCountLock, oldIrql);
    BulkUsb_DbgPrint(3, ("file bulkpnp: BulkUsb_IoIncrement::%d\n", result));
    return result;
}

LONG BulkUsb_IoDecrement(IN OUT PTDeviceExtension DeviceExtension)
{
    LONG  result = 0;
    KIRQL oldIrql;

    KeAcquireSpinLock(&DeviceExtension->IOCountLock, &oldIrql);
    result = InterlockedDecrement(&DeviceExtension->OutStandingIO);
    if(result == 1) 
	{
        KeSetEvent(&DeviceExtension->StopEvent, IO_NO_INCREMENT, FALSE);
    }
    if(result == 0) 
	{
        ASSERT(REMOVED == DeviceExtension->DeviceState);
        KeSetEvent(&DeviceExtension->RemoveEvent, IO_NO_INCREMENT, FALSE);
    }
    KeReleaseSpinLock(&DeviceExtension->IOCountLock, oldIrql);
    BulkUsb_DbgPrint(3, ("file bulkpnp: BulkUsb_IoDecrement::%d\n", result));
    return result;
}

VOID ProcessQueuedRequests(IN OUT PTDeviceExtension DeviceExtension)
{
    KIRQL       oldIrql;
    PIRP        nextIrp,
                cancelledIrp;
    PVOID       cancelRoutine;
    LIST_ENTRY  cancelledIrpList;
    PLIST_ENTRY listEntry;

    BulkUsb_DbgPrint(3, ("file bulkpnp: ProcessQueuedRequests - begins\n"));
    cancelRoutine = NULL;
    InitializeListHead(&cancelledIrpList);

	while(1) 
	{
        KeAcquireSpinLock(&DeviceExtension->QueueLock, &oldIrql);
        if(IsListEmpty(&DeviceExtension->NewRequestsQueue)) 
		{
            KeReleaseSpinLock(&DeviceExtension->QueueLock, oldIrql);
            break;
        }
        listEntry = RemoveHeadList(&DeviceExtension->NewRequestsQueue);
        nextIrp = CONTAINING_RECORD(listEntry, IRP, Tail.Overlay.ListEntry);
        cancelRoutine = IoSetCancelRoutine(nextIrp, NULL);
        if(nextIrp->Cancel) 
		{
            if(cancelRoutine) 
			{
                InsertTailList(&cancelledIrpList, listEntry);
            }
            else 
			{
                InitializeListHead(listEntry);
            }

            KeReleaseSpinLock(&DeviceExtension->QueueLock, oldIrql);
        }
        else 
		{
            KeReleaseSpinLock(&DeviceExtension->QueueLock, oldIrql);
            if(FAILREQUESTS == DeviceExtension->QueueState) 
			{
                nextIrp->IoStatus.Information = 0;
                nextIrp->IoStatus.Status = STATUS_DELETE_PENDING;
                IoCompleteRequest(nextIrp, IO_NO_INCREMENT);
            }
            else 
			{
                PIO_STACK_LOCATION irpStack;

                BulkUsb_DbgPrint(3, ("file bulkpnp: ProcessQueuedRequests::"));
                BulkUsb_IoIncrement(DeviceExtension);
                IoSkipCurrentIrpStackLocation(nextIrp);
                IoCallDriver(DeviceExtension->TopOfStackDeviceObject, nextIrp);
                BulkUsb_DbgPrint(3, ("file bulkpnp: ProcessQueuedRequests::"));
                BulkUsb_IoDecrement(DeviceExtension);
            }
        }
    } // while loop

    while(!IsListEmpty(&cancelledIrpList)) 
	{
        PLIST_ENTRY cancelEntry = RemoveHeadList(&cancelledIrpList);

        cancelledIrp = CONTAINING_RECORD(cancelEntry, IRP, Tail.Overlay.ListEntry);
        cancelledIrp->IoStatus.Status = STATUS_CANCELLED;
        cancelledIrp->IoStatus.Information = 0;
        IoCompleteRequest(cancelledIrp, IO_NO_INCREMENT);
    }

    BulkUsb_DbgPrint(3, ("file bulkpnp: ProcessQueuedRequests - ends\n"));
}

//=============================
NTSTATUS BulkUsb_DispatchClean(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{
    PTDeviceExtension  deviceExtension;
    KIRQL              oldIrql;
    LIST_ENTRY         cleanupList;
    PLIST_ENTRY        thisEntry, 
                       nextEntry, 
                       listHead;
    PIRP               pendingIrp;
    PIO_STACK_LOCATION pendingIrpStack, 
                       irpStack;
    NTSTATUS           ntStatus;

    deviceExtension = (PTDeviceExtension) DeviceObject->DeviceExtension;
    irpStack = IoGetCurrentIrpStackLocation(Irp);
    InitializeListHead(&cleanupList);
    BulkUsb_DbgPrint(3, ("file bulkpnp: BulkUsb_DispatchClean::\n"));
    BulkUsb_IoIncrement(deviceExtension);

    KeAcquireSpinLock(&deviceExtension->QueueLock, &oldIrql);
    listHead = &deviceExtension->NewRequestsQueue;
    for(thisEntry = listHead->Flink, nextEntry = thisEntry->Flink; thisEntry != listHead;
       thisEntry = nextEntry, nextEntry = thisEntry->Flink) 
	{
        pendingIrp = CONTAINING_RECORD(thisEntry, IRP, Tail.Overlay.ListEntry);
        pendingIrpStack = IoGetCurrentIrpStackLocation(pendingIrp);
        if(irpStack->FileObject == pendingIrpStack->FileObject) 
		{
            RemoveEntryList(thisEntry);
            if(NULL == IoSetCancelRoutine(pendingIrp, NULL)) 
			{
                InitializeListHead(thisEntry);
            }
            else 
			{
                InsertTailList(&cleanupList, thisEntry);
            }
        }
    }
    KeReleaseSpinLock(&deviceExtension->QueueLock, oldIrql);

	while(!IsListEmpty(&cleanupList)) 
	{
        thisEntry = RemoveHeadList(&cleanupList);
        pendingIrp = CONTAINING_RECORD(thisEntry, IRP, Tail.Overlay.ListEntry);
        pendingIrp->IoStatus.Information = 0;
        pendingIrp->IoStatus.Status = STATUS_CANCELLED;
        IoCompleteRequest(pendingIrp, IO_NO_INCREMENT);
    }

    Irp->IoStatus.Information = 0;
    Irp->IoStatus.Status = STATUS_SUCCESS;
    IoCompleteRequest(Irp, IO_NO_INCREMENT);
    BulkUsb_DbgPrint(3, ("file bulkpnp: BulkUsb_DispatchClean::\n"));
    BulkUsb_IoDecrement(deviceExtension);
    return STATUS_SUCCESS;
}
