
#ifndef _TERMINAL_MAIN_H_
#define _TERMINAL_MAIN_H_

#include <string.h>
#include <stdio.h>

#include "option.h"

#include "shell.h"
#include "cmdlist.h"
#include "../v568/tools/trmtype.h"

#include "../yaffs/yaffs.h"

#include "../s3c44box/s3c44box.h"
#include "../s3c44box/hostapi.h"

#include "../usb9604/usbapi/usbapi.h"
#include "../usb9604/driver/driver.h"

//#include "../v568/driver/v5api.h"
#include "../v568/driver/driver.h"
#include "../v568/highapi/highapi.h"
#include "../v568/userapi/userapi.h"
#include "../v568/v568testdemo/v568testdemo.h"
#include "../v568/v568simpledemo/v568simpledemo.h"
#include "../v568/dbugtools/vdbugtools.h"
#include "../v568/v568env/v5initenv.h"

#include "../v568/568hapi/v5hcmd.h"

#include "../v938/driver/basefunc/basefunc938.h"
#include "../v938/v938env/v9initenv.h"
#include "../v938/938testdemo/internal938.h"

#include "../v938/driver/driver938.h"

#include "../vstdio/vmalloc.h"

#include "../nandflash/nandenv/nandenv.h"
#include "../nandflash/driver/nandbase.h"
#include "../nandflash/driver/nanddrv.h"
#include "../nandflash/userapi/option.h"

#include "../v868/v868rdk/driver/internal.h"

#include "../fat/vfatapi/vfatapi.h"

#endif


