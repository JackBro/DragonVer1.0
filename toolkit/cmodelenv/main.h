#ifndef _CMODEL_MAIN_H_
#define _CMODEL_MAIN_H_

#include "./cmodel/v568.h"

int __cdecl main(int argc, char * argv[]);

unsigned int OpenCmodel(char *cfgfile);
void CloseCmodel(void);

void StopCmodel(void);
void StartCmodel(void);
void PauseCmodel(void);

extern CV568Chip *gpchip;

#endif

