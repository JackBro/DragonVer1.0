#ifdef WIN32
#include <afxpriv.h>		// required for WM_SETMESSAGESTRING
#define _BMPDEFINED_
#endif

//#include "stdio.h"
#include "stdarg.h"
#include "dbugPrint.h"


CDbugTools1 Gdbug;

CDbugTools1::CDbugTools1(void)
{
	this->gfp = fopen(DBUGDATAFILE, "wt");
}

CDbugTools1::~CDbugTools1(void)
{
	if(this->gfp)
		fclose(this->gfp);
}

void CDbugTools1::DbugData(const char *Format, ...)
{
	char tmsg[256];
	va_list va;

	if(this->gfp == NULL)
		return;

	va_start( va, Format );  
	vsprintf(tmsg, Format, va);
	va_end( va );   

	fprintf(this->gfp, "%s", tmsg);
}

void CDbugTools1::DbugInterData(const char *Format, ...)
{
	char tmsg[256];
	va_list va;
	FILE *fp;

	fp = fopen(INTERDATAFILE, "at");
	if(fp == NULL)
		return;

	va_start( va, Format );  
	vsprintf(tmsg, Format, va);
	va_end( va );   

	fprintf(fp, "%s", tmsg);
	fclose(fp);
}

void CDbugTools1::DbugBin(int val, int len)
{
	char tmsg[256];
	int i;

	if(this->gfp == NULL)
		return;

	tmsg[0] = 0;
	for(i = len-1; i >= 0; i--)
	{
		if( val & (1 << i) )
			sprintf(tmsg, "%s1", tmsg);
		else
			sprintf(tmsg, "%s0", tmsg);
	}
	sprintf(tmsg, "%s ",tmsg);

	fprintf(this->gfp, "%s", tmsg);
}

void CDbugTools1::DbugPrintData(int mode, short int *buf, int len)
{
	FILE *fp = NULL;

	if( (mode < SIF_PRINT_INPUT) || (mode >= TOTAL_PRINT) )
		return;

	if(gDbugMap[mode].filename[0] != '\0')
	{
		fp = fopen(gDbugMap[mode].filename, "ab");
		if(fp == NULL)
			return;
		fwrite(buf, sizeof(short int), len, fp);
		fclose(fp);
	}
}

void CDbugTools1::DbugPrintData(int mode, ImageStream *pIn)
{
	FILE *fp = NULL;

	if( (mode < SIF_PRINT_INPUT) || (mode >= TOTAL_PRINT) )
		return;

	if(gDbugMap[mode].filename[0] != '\0')
	{
		fp = fopen(gDbugMap[mode].filename, "ab");
		if(fp == NULL)
			return;
		switch(pIn->format)
		{
		case UYVY:
		case VYUY:
		case YUYV:
		case YVYU:

		case RAW_BG:
		case RAW_GB:
		case RAW_RG:
		case RAW_GR:

		case YUV422:
		case YUV411:
		case YUV420:
		case YUV400:
			fwrite(pIn->buf16, sizeof(short int), pIn->width, fp);
			break;

		case RAWRGB:
			fwrite(pIn->buf16, sizeof(RawRgbType), pIn->width, fp);
			break;

		case YUV444:
			fwrite(pIn->buf16, sizeof(Yuv444Type), pIn->width, fp);
			break;

		default:
			break;
		}
		fclose(fp);
	}
}

void CDbugTools1::DbugPrintData(int mode, ImageBlock *pIn)
{
	FILE *fp = NULL;

	if( (mode < SIF_PRINT_INPUT) || (mode >= TOTAL_PRINT) )
		return;

	if(gDbugMap[mode].filename[0] != '\0')
	{
		fp = fopen(gDbugMap[mode].filename, "ab");
		if(fp == NULL)
			return;
		switch(pIn->format)
		{
		case YUV422BLK:
			fwrite(pIn->buf8, sizeof(Yuv422BlockType), pIn->width, fp);
			break;
		case YUV411BLK:
		case YUV420BLK:
		case YUV400BLK:
		case YUV444BLK:
			break;
		case JPEGDATA:
			fwrite(pIn->buf8, sizeof(UINT8), pIn->width >> 3, fp);
			break;

		default:
			break;
		}
		fclose(fp);
	}
}

#ifdef WIN32
void CDbugTools1::DbugMsg(const char *Format, ...)
{
	char tmsg[256];

	va_list va;
	va_start( va, Format );  

	vsprintf(tmsg, Format, va);
	va_end( va );   

	AfxMessageBox(tmsg);
}
#else
void CDbugTools1::DbugMsg(const char *Format, ...)
{
	char tmsg[256];

	va_list va;
	va_start( va, Format );  

	vsprintf(tmsg, Format, va);
	va_end( va );   

	printf("%s", tmsg);
}

#endif




