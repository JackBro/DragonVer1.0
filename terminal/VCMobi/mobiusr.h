#ifndef _MOBIUSB_USER_H
#define _MOBIUSB_USER_H

#include <initguid.h>

DEFINE_GUID(GUID_CLASS_VCMOBI_MOBI, 
0xcfcbe01a, 0xed6d, 0x4be6, 0x9e, 0xd9, 0x7d, 0xf0, 0x40, 0x1e, 0xb6, 0x0);

#define MOBIUSB_IOCTL_INDEX             0x0000


#define IOCTL_MOBIUSB_GET_CONFIG_DESCRIPTOR CTL_CODE(FILE_DEVICE_UNKNOWN,     \
                                                     MOBIUSB_IOCTL_INDEX,     \
                                                     METHOD_BUFFERED,         \
                                                     FILE_ANY_ACCESS)
                                                   
#define IOCTL_MOBIUSB_RESET_DEVICE          CTL_CODE(FILE_DEVICE_UNKNOWN,     \
                                                     MOBIUSB_IOCTL_INDEX + 1, \
                                                     METHOD_BUFFERED,         \
                                                     FILE_ANY_ACCESS)

#define IOCTL_MOBIUSB_RESET_PIPE            CTL_CODE(FILE_DEVICE_UNKNOWN,     \
                                                     MOBIUSB_IOCTL_INDEX + 2, \
                                                     METHOD_BUFFERED,         \
                                                     FILE_ANY_ACCESS)

#define IOCTL_MOBIUSB_VENDOR				CTL_CODE(FILE_DEVICE_UNKNOWN,     \
                                                     MOBIUSB_IOCTL_INDEX + 3, \
                                                     METHOD_BUFFERED,         \
                                                     FILE_ANY_ACCESS)

#endif

