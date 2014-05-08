
#include <initguid.h>
#include <wdm.h>
#include <wmilib.h>
#include <wmistr.h>
#include "usbdi.h"
#include "usbdlib.h"

#ifndef _BULKUSB_H
#define _BULKUSB_H

#define BULKTAG (ULONG) 'KluB'

#undef ExAllocatePool
#define ExAllocatePool(type, size)		ExAllocatePoolWithTag(type, size, BULKTAG)

#if DBG
#define BulkUsb_DbgPrint(level, _x_)	{ if((level) <= DebugLevel) { DbgPrint _x_; } }
#else
#define BulkUsb_DbgPrint(level, _x_)
#endif

#define BULKUSB_TEST_BOARD_TRANSFER_BUFFER_SIZE (64 *1024 )
#define BULKUSB_MAX_TRANSFER_SIZE   256

typedef struct tag_TUsbdContext 
{
    UNICODE_STRING BulkUsb_RegistryPath;
} TUsbdContext, *PTUsbdContext;

#define IDLE_INTERVAL 5000

enum DEVSTATEenum 
{
    NOTSTARTED,         // not started
    STOPPED,            // device stopped
    WORKING,            // started and working
    PENDINGSTOP,        // stop pending
    PENDINGREMOVE,      // remove pending
    SURPRISEREMOVED,    // removed by surprise
    REMOVED             // removed
};

enum QUEUE_STATEenum 
{
    HOLDREQUESTS,       // device is not started yet
    ALLOWREQUESTS,      // device is ready to process
    FAILREQUESTS        // fail both existing and queued up requests
};

enum WDM_VERSIONenum 
{
    WINXPORBETTER,
    WIN2KORBETTER,
    WINMEORBETTER,
    WIN98ORBETTER
};

//
// registry path used for parameters 
// global to all instances of the driver
//

#define BULKUSB_REGISTRY_PARAMETERS_PATH  \
	L"\\REGISTRY\\Machine\\System\\CurrentControlSet\\SERVICES\\BULKUSB\\Parameters"


typedef struct tag_TPipeContext 
{
    BOOLEAN PipeOpen;
} TPipeContext, *PTPipeContext;

//
// A structure representing the instance information associated with
// this particular device.
//

typedef struct tag_TDeviceExtension 
{
    PDEVICE_OBJECT FunctionalDeviceObject;	// Functional Device Object
    PDEVICE_OBJECT TopOfStackDeviceObject;	// Device object we call when submitting Urbs
    PDEVICE_OBJECT PhysicalDeviceObject;	// The bus driver object
    UNICODE_STRING InterfaceName;   // Name buffer for our named Functional device object link
									// The name is generated based on the driver's class GUID
	int		WdmVersion;
    LONG	SSEnable;
    LONG	SSRegistryEnable;
    KTIMER	Timer;
    KDPC	DeferredProcCall;
    KEVENT	NoDpcWorkItemPendingEvent;
    KEVENT	NoIdleReqPendEvent;

    LONG	OpenHandleCount;
    ULONG	OutStandingIO;

    DEVICE_POWER_STATE DevPower;

    PIRP	PendingIdleIrp;
    LONG	IdleReqPend;
    KSPIN_LOCK IdleReqStateLock;

    KSPIN_LOCK IOCountLock;
    KEVENT	RemoveEvent;
    KEVENT	StopEvent;

	KSPIN_LOCK DevStateLock;
	int		DeviceState;
	int		PrevDevState;
    int		QueueState;

    KSPIN_LOCK QueueLock;
    LIST_ENTRY NewRequestsQueue;

    PUSB_IDLE_CALLBACK_INFO IdleCallbackInfo;

    LONG	FreeIdleIrpCount;

    ULONG	PowerDownLevel;

    DEVICE_CAPABILITIES DeviceCapabilities;

    LONG	WaitWakeEnable;

    LONG	FlagWWOutstanding;

    LONG	FlagWWCancel;

    PIRP	WaitWakeIrp;

    WMILIB_CONTEXT WmiLibInfo;

    PUSBD_INTERFACE_INFORMATION UsbInterface;
    PTPipeContext PipeContext;
    PUSB_CONFIGURATION_DESCRIPTOR UsbConfigurationDescriptor;

    SYSTEM_POWER_STATE SysPower;

} TDeviceExtension, *PTDeviceExtension;

typedef struct tag_TIrpCompletionContext 
{
    PTDeviceExtension DeviceExtension;
    PKEVENT Event;
} TIrpCompletionContext, *PTIrpCompletionContext;

extern TUsbdContext gUsbdContext;
extern ULONG DebugLevel;

#endif
