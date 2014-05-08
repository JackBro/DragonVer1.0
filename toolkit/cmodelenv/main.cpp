
#include "main.h"
#include "info.h"
#include "shell.h"

static volatile int gCmodelRun = CMODEL_CLKHIGH;

CV568Chip *gpchip = NULL;

unsigned int OpenCmodel(char *cfgfile)
{
	char str[2][100], *argv[2];

	argv[0] = str[0];
	argv[1] = str[1];
	sprintf(argv[0], "main");
	sprintf(argv[1], cfgfile);

	return main(2, argv);
}

void CloseCmodel(void)
{
	StopCmodel();
}

void StopCmodel(void)
{
	gCmodelRun = CMODEL_CLKSTOP;
}

void StartCmodel(void)
{
	gCmodelRun = CMODEL_CLKHIGH;
}

void PauseCmodel(void)
{
	gCmodelRun = CMODEL_CLKPAUSE;
}

static void InitEnv(void)
{
	shellInit();
}

int __cdecl main(int argc, char * argv[])
{
	int ret, flag = 1;

	gpchip = new CV568Chip;
	if(gpchip == NULL)
		return 0;
//	StartRecode();
	InitEnv();
	if(argc > 1)
		gpchip->SetConfigFile(argv[1]);
	else 
		gpchip->SetConfigFile();
	StartCmodel();
	while(flag)
	{
		ret = gpchip->Process(gCmodelRun);
		if(ret & CMODEL_ALLOVER)
			flag = 0;
	}
	if(gpchip)
		delete gpchip;
	gpchip = NULL;
//	PrintRunTime();
	return 0;
}

