
#ifndef _V9_INITENV_H_
#define _V9_INITENV_H_

#include "../../main/option.h"

#ifdef __cplusplus
extern "C" {
#endif


#define V938_DEFAULTMCLK     24000000

void V9_InitEnv(void);
void Lm4867ShutDown(int sel);


extern const char* gCpmIntType[];
extern const char gIntNum938; 

#ifdef __cplusplus
}
#endif


#endif


