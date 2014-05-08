

#ifndef _BULKUSB_USER_H
#define _BULKUSB_USER_H

#include <initguid.h>

// {CFCBE01A-ED6D-4be6-9ED9-7DF0401EB600}
DEFINE_GUID(GUID_CLASS_VCMOBI_BULK, 
0xcfcbe01a, 0xed6d, 0x4be6, 0x9e, 0xd9, 0x7d, 0xf0, 0x40, 0x1e, 0xb6, 0x0);

#define BULKUSB_IOCTL_INDEX             0x0000


#define IOCTL_BULKUSB_GET_CONFIG_DESCRIPTOR CTL_CODE(FILE_DEVICE_UNKNOWN,     \
                                                     BULKUSB_IOCTL_INDEX,     \
                                                     METHOD_BUFFERED,         \
                                                     FILE_ANY_ACCESS)
                                                   
#define IOCTL_BULKUSB_RESET_DEVICE          CTL_CODE(FILE_DEVICE_UNKNOWN,     \
                                                     BULKUSB_IOCTL_INDEX + 1, \
                                                     METHOD_BUFFERED,         \
                                                     FILE_ANY_ACCESS)

#define IOCTL_BULKUSB_RESET_PIPE            CTL_CODE(FILE_DEVICE_UNKNOWN,     \
                                                     BULKUSB_IOCTL_INDEX + 2, \
                                                     METHOD_BUFFERED,         \
                                                     FILE_ANY_ACCESS)

#define IOCTL_BULKUSB_VENDOR				CTL_CODE(FILE_DEVICE_UNKNOWN,     \
                                                     BULKUSB_IOCTL_INDEX + 3, \
                                                     METHOD_BUFFERED,         \
                                                     FILE_ANY_ACCESS)

#endif

