
#include "internal.h"
#ifndef	V578CMD_DISABLE

#ifdef WIN32
void PrintMarbMap(void)
{
	TMarbContext *pmarb;
	FILE *fp = NULL;

	pmarb = V5_MarbGetContext();
	fp = fopen("map.txt", "at");
	if(fp == NULL)
		return;

	fprintf(fp, "\n==========================\n");

	fprintf(fp, "mode = 0x%x\n", pmarb->mode);
	fprintf(fp, "layerB0start = 0x%lx\n", pmarb->map.layerB0start);
	fprintf(fp, "layerB0end = 0x%lx\n", pmarb->map.layerB0end);
	fprintf(fp, "layerB1start = 0x%lx\n", pmarb->map.layerB1start);
	fprintf(fp, "layerB1end = 0x%lx\n", pmarb->map.layerB1end);
	fprintf(fp, "lbuf0start = 0x%lx\n", pmarb->map.lbuf0start);
	fprintf(fp, "lbuf1start = 0x%lx\n", pmarb->map.lbuf1start);
	fprintf(fp, "lbufend = 0x%lx\n", pmarb->map.lbufend);
	fprintf(fp, "thumbstart = 0x%lx\n", pmarb->map.thumbstart);
	fprintf(fp, "thumbend = 0x%lx\n", pmarb->map.thumbend);
	fprintf(fp, "indexstart = 0x%lx\n", pmarb->map.indexstart);
	fprintf(fp, "indexend = 0x%lx\n", pmarb->map.indexend);
	fprintf(fp, "jbufstart = 0x%lx\n", pmarb->map.jbufstart);
	fprintf(fp, "jbufend = 0x%lx\n", pmarb->map.jbufend);
	fprintf(fp, "layerA0start = 0x%lx\n", pmarb->map.layerA0start);
	fprintf(fp, "layerA0end = 0x%lx\n", pmarb->map.layerA0end);
	fprintf(fp, "layerA1start = 0x%lx\n", pmarb->map.layerA1start);
	fprintf(fp, "layerA1end = 0x%lx\n", pmarb->map.layerA1end);

	fprintf(fp, "\n==========================\n\n");
	fclose(fp);
}

#else

void PrintMarbMap(void)
{
}

#endif

#endif //V578CMD_DISABLE
