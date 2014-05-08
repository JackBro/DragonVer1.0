#ifndef _HIGHAPI_INHEADERS_H_
#define _HIGHAPI_INHEADERS_H_

#include <stdio.h>
#include <string.h>
#include "../tools/type.h"
#include "../driver/v5api.h"
#include "../driver/v5type.h"
#include "../../s3c44box/hostapi.h"
#include "../v568env/v5initenv.h"
#include "../driver/basefunc/basefunc.h"
#include "../userapi/userapi.h"
#include "v5herror.h"
#include "v5hcmd.h"
#include "v5camera.h"
#include "v5display.h"
#include "v5hjpeg.h"
#include "v5usersetting.h"

//#include "../highapi/display.h"

extern THapiContext gHapiContext;
extern const THapiStdInfo gHapiStdInfo;
extern V5MMalloc *gpmalloc;
extern V5MFree *gpfree;
extern const TFileOperations gfileop ; 

#endif 

