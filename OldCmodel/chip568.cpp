

#include "dbugPrint.h"
#include "chip568.h"


DbugPrintMap gDbugMap[] =
{
	{	"SifInputFile",			{'\0'}},	//SIF_INPUT_FILE
	{	"IspInputFile",			{'\0'}},	//ISP_INPUT_FILE
	{	"IppInputFile",			{'\0'}},	//IPP_INPUT_FILE
	{	"LbufInputFile",		{'\0'}},	//LBUF_INPUT_FILE
	{	"JpegInputFile",		{'\0'}},	//JPEG_INPUT_FILE
	{	"LayerBInputFile",		{'\0'}},	//LAYERB_INPUT_FILE
	{	"LayerAInputFile",		{'\0'}},	//LAYERA_INPUT_FILE
	{	"JbufInputFile",		{'\0'}},	//JBUF_INPUT_FILE

	{	"SifPrintInput",		{'\0'}},	//SIF_PRINT_INPUT
	{	"IspPrintInput",		{'\0'}},	//ISP_PRINT_INPUT
	{	"IppPrintInput",		{'\0'}},	//IPP_PRINT_INPUT
	{	"LbufPrintInput",		{'\0'}},	//LBUF_PRINT_INPUT
	{	"JpegPrintInput",		{'\0'}},	//JPEG_PRINT_INPUT
	{	"LayerBPrintInput",		{'\0'}},	//LAYERB_PRINT_INPUT
	{	"LayerAPrintInput",		{'\0'}},	//LAYERA_PRINT_INPUT
	{	"JbufPrintInput",		{'\0'}},	//JBUF_PRINTF_INPUT

	{	"IspBlackPrintInput",	{'\0'}},	//ISP_BLACKLEVEL_PRINT_INPUT
	{	"IspTestbarPrintInput",	{'\0'}},	//ISP_TESTBAR_PRINT_INPUT
	{	"IspLensPrintInput",	{'\0'}},	//ISP_LENS_PRINT_INPUT
	{	"IspDpdPrintInput",		{'\0'}},	//ISP_DPD_PRINT_INPUT
	{	"IspGainPrintInput",	{'\0'}},	//ISP_GAIN_PRINT_INPUT
	{	"IspInterpolatePrintInput",	{'\0'}},//ISP_INTERPOLATE_PRINT_INPUT
	{	"IspColorCorrectPrintInput",{'\0'}},//ISP_COLORCORRECT_PRINT_INPUT
	{	"IspGammaCorrectPrintInput",{'\0'}},//ISP_GAMMACORRECT_PRINT_INPUT
	{	"IspColorConvertPrintInput",{'\0'}},//ISP_COLORCONVERT_PRINT_INPUT
	{	"IspEdgeEnhancePrintInput",{'\0'}},	//ISP_EDGEENHANCE_PRINT_INPUT
	{	"IspEdgePrintInput",	{'\0'}},	//ISP_EDGE_PRINT_INPUT

	{	"IppEdgePrintInput",	{'\0'}},	//IPP_EDGE_PRINT_INPUT
	{	"IppZoomPrintInput",	{'\0'}},	//IPP_ZOOM_PRINT_INPUT
	{	"IppSpecialPrintInput",	{'\0'}},	//IPP_YUV2RGB_PRINT_INPUT
	{	"IppPrefilterPrintInput",{'\0'}},	//IPP_PREFILTER_PRINT_INPUT
	{	"IppDisplayPrintInput",{'\0'}},		//IPP_DISPSIZER_PRINT_INPUT
	{	"IppCapturePrintInput",{'\0'}},		//IPP_CAPSIZER_PRINT_INPUT

	{	"LbufZoomPrintInput",{'\0'}},		//LBUF_ZOOM_PRINT_INPUT
	{	"Lbuf2BlockPrintInput",{'\0'}},		//LBUF_2BLOCKPRINT_INPUT

	{	"JpegEncodePrintInput",{'\0'}}		//JPEG_ENCODE_PRINT_INPUT
};


CCmodelSim::CCmodelSim(void)
{
	int i;
	int len = sizeof(gDbugMap) / sizeof(DbugPrintMap);

	this->gInputLine = 1;
	this->gSelect = 0;
	this->gpModule = NULL;

	for(i = 0; i < len; i++)
		gDbugMap[i].filename[0] = '\0';

	this->SimRegInit();
}

CCmodelSim::~CCmodelSim(void)
{
}

void CCmodelSim::SimRegInit(void)
{
	CChipBase1::RegInit();

	this->SimSetReg(REG_SIM_MODE, SIM_SIF2ISP);
	this->SimSetReg(REG_SIM_SAVEREGLIST, SAVEREGLIST);
	this->SimSetReg(REG_LOOP_TEST,	0x1);
	this->SimSetReg(REG_SIM_STREAMWIDTH_L,	SIM_STREAMWIDTH & 0xff);
	this->SimSetReg(REG_SIM_STREAMWIDTH_H,	(SIM_STREAMWIDTH >> 8) & 0xff);
	this->SimSetReg(REG_SIM_STREAMHEIGHT_L,	SIM_STREAMHEIGHT & 0xff);
	this->SimSetReg(REG_SIM_STREAMHEIGHT_H,	(SIM_STREAMHEIGHT >> 8) & 0xff);
	this->SimSetReg(REG_SIM_FORMAT, RAW_BG);
}

void CCmodelSim::SimSetReg(int adr, int val)
{
	CChipBase1::SetReg(adr, val);
}

int CCmodelSim::SimGetReg(int adr)
{
	return CChipBase1::GetReg(adr);
}

void CCmodelSim::RegInit(void)
{
	this->gSif.RegInit();
	this->gIsp.RegInit();
	this->gIpp.RegInit();
	this->gJbuf.RegInit();
	this->gJpeg.RegInit();
	this->gLbuf.RegInit();
	this->gLcdIF.RegInit();
	this->gChipCtrl.RegInit();
	this->gGE.RegInit();
	this->gBusIF.RegInit();
}

void CCmodelSim::SetReg(int adr, int val)
{
	this->gModify = 1;
	if(this->SimGetReg(REG_SIM_SAVEREGLIST) == SAVEREGLIST)
		this->SaveRegList(adr, val);

	switch(adr & 0xff00)
	{
	case BASE_JPEGADDR:
		this->gJpeg.SetReg(adr & 0xff, val & 0xff);
		break;

	case BASE_LCDIFADDR:
		this->gLcdIF.SetReg(adr & 0xff, val & 0xff);
		break;

	case BASE_SENSORIFADDR:
		this->gSif.SetReg(adr & 0xff, val & 0xff);
		break;

	case BASE_LBUFADDR:
		this->gLbuf.SetReg(adr & 0xff, val & 0xff);
		break;

	case BASE_GEADDR:
		this->gGE.SetReg(adr & 0xff, val & 0xff);
		break;

	case BASE_CHIPCTRLADDR:
		this->gChipCtrl.SetReg(adr & 0xff, val & 0xff);
		break;

	case BASE_BUSIFADDR:
		this->gBusIF.SetReg(adr & 0xff, val & 0xff);
		break;

	case BASE_IPPADDR:
		this->gIpp.SetReg(adr & 0xff, val & 0xff);
		break;

	case BASE_ISPADDR:
		this->gIsp.SetReg(adr & 0xff, val & 0xff);
		break;

	case BASE_JBUFADDR:
		this->gJbuf.SetReg(adr & 0xff, val & 0xff);
		break;

	case BASE_SIMADDR:
		this->SimSetReg(adr & 0xff, val & 0xff);
		break;

	default:
		break;
	}
}

int CCmodelSim::GetReg(int adr)
{
	int val = 0;

	switch(adr & 0xff00)
	{
	case BASE_JPEGADDR:
		val = this->gJpeg.GetReg(adr);
		break;

	case BASE_LCDIFADDR:
		val = this->gLcdIF.GetReg(adr);
		break;

	case BASE_SENSORIFADDR:
		val = this->gSif.GetReg(adr);
		break;

	case BASE_LBUFADDR:
		val = this->gLbuf.GetReg(adr);
		break;

	case BASE_GEADDR:
		val = this->gGE.GetReg(adr);
		break;

	case BASE_CHIPCTRLADDR:
		val = this->gChipCtrl.GetReg(adr);
		break;

	case BASE_BUSIFADDR:
		val = this->gBusIF.GetReg(adr & 0xff);
		break;

	case BASE_IPPADDR:
		val = this->gIpp.GetReg(adr);
		break;

	case BASE_ISPADDR:
		val = this->gIsp.GetReg(adr);
		break;

	case BASE_JBUFADDR:
		val = this->gJbuf.GetReg(adr);
		break;

	case BASE_SIMADDR:
		val = this->SimGetReg(adr & 0xff);
		break;

	default:
		break;
	}
	return val;
}

#define BUFSIZE	0x10000

int CCmodelSim::ConfigInit(char *configfile)
{
	FILE *fp = NULL;
	int len, flag = 1;
	int offset = 0;
	char buf[BUFSIZE];

	fp = fopen(configfile, "rt");
	if(fp == NULL)
	{
		this->ConfigGenerator(configfile);
		return CMODEL_GENERATOR_CONFIGFILE;
	}

	do {
		len = (int)fread(buf + offset, 1, BUFSIZE - offset, fp);
		if(len < BUFSIZE - offset)
			flag = 0;
		if(this->ConfigParse(buf, len + offset, &offset) != 0)
		{
			fclose(fp);
			return CMODEL_PARSE_ERROR;
		}
	} while(flag == 1);

	fclose(fp);

	this->ClearDataFile();

	return CMODEL_SUCCEED;
}

void CCmodelSim::ConfigGenerator(char *configfile)
{
	FILE *fp = NULL;
	int i = 0;
	int len = sizeof(gDbugMap) / sizeof(DbugPrintMap);
//	char str[20] = {"NULL"};

	fp = fopen(configfile, "wt");
	if(fp == NULL)
		return;
	for(i = 0; i < len; i++)
	{
		if(gDbugMap[i].filename[0] == '\0')
			fprintf(fp, "%s\t=\tNULL\n", gDbugMap[i].keychar);
		else
			fprintf(fp, "%s\t=\t%s\n", gDbugMap[i].keychar, gDbugMap[i].filename);
	}
	fclose(fp);
	Gdbug.DbugMsg("Generator config file: %s\n\n", configfile);
}

void CCmodelSim::ClearDataFile(void)
{
	FILE *fp = NULL;
	int i = SIF_PRINT_INPUT;
	int len = sizeof(gDbugMap) / sizeof(DbugPrintMap);

	while(i < len)
	{
		if(gDbugMap[i].filename[0] != '\0')
		{
			fp = fopen(gDbugMap[i].filename, "wb");
			fclose(fp);
		}
		i++;
	}
}

#define CONFIGREG	0
#define PRINTDATA	1

int CCmodelSim::FillConfig(int count, char *item[])
{
	int len, i, flag = 10;
	int adr, val;
	char *p = NULL;

	if(count == 0)
		return CMODEL_SUCCEED;

	switch(count)
	{
	case 1:
		len = (int)strlen(item[0]);
		for(i = 0; i < len; i++)
		{
			if(item[0][i] == '@')
			{
				item[0][i] = '\0';
				p = &(item[0][i+1]);
				flag = CONFIGREG;			//register mode
				i = len + 10;
			}
			else if(item[0][i] == '=')
			{
				item[0][i] = '=';
				p = &(item[0][i+1]);
				flag = PRINTDATA;			//debug file mode
				i = len + 10;
			}
		}
		if(i == len)
		{
			Gdbug.DbugMsg("Error in line %d\n", this->gInputLine);
			return CMODEL_PARSE_ERROR;
		}
		break;

	case 2:
		len = (int)strlen(item[0]);
		if(item[0][len-1] == '=')
		{
			item[0][len-1] = '\0';
			p = item[1];
			flag = PRINTDATA;
		}
		else if(item[0][len-1] == '@')
		{
			item[0][len-1] = '\0';
			p = item[1];
			flag = CONFIGREG;
		}
		else if(item[1][0] == '=')
		{
			p = &(item[1][1]);
			flag = PRINTDATA;
		}
		else if(item[1][0] == '@')
		{
			p = &(item[1][1]);
			flag = CONFIGREG;
		}
		else
		{
			Gdbug.DbugMsg("Error in line %d\n", this->gInputLine);
			return CMODEL_PARSE_ERROR;
		}
		break;

	case 3:
	default:
		p = item[2];
		if(item[1][0] == '=')
		{
			flag = PRINTDATA;
		}
		else if(item[1][0] == '@')
		{
			flag = CONFIGREG;
		}
		else
		{
			Gdbug.DbugMsg("Error in line %d\n", this->gInputLine);
			return CMODEL_PARSE_ERROR;
		}
		break;
	}

	switch(flag)
	{
	case CONFIGREG:			//register mode
		sscanf(item[0], "%x", &val);
		sscanf(p, "%x", &adr);
		this->SetReg(adr, val);
		break;

	case PRINTDATA:			//debug file mode
		len = sizeof(gDbugMap) / sizeof(DbugPrintMap);
		for(i = 0; i < len; i++)
		{
			if(strcmp(item[0], gDbugMap[i].keychar) == 0)
			{
				if(strcmp(p, "NULL") == 0)
					gDbugMap[i].filename[0] = '\0';
				else if(strcmp(p, "null") == 0)
					gDbugMap[i].filename[0] = '\0';
				else
					sprintf(gDbugMap[i].filename, "%s", p);
				i = len+10;
			}
		}
		if(i == len)
		{
			Gdbug.DbugMsg("Error in line %d\n", this->gInputLine);
			return CMODEL_PARSE_ERROR;
		}

		break;
	default:
		break;
	}

	return CMODEL_SUCCEED;
}

int CCmodelSim::ConfigParse(char *buf, int len, int *offset)
{
	int count = 0;
	int Initem = 0;
	char *p = buf;
	char *pend = buf+len;
	char *item[10];

	while(p < pend)
	{
		switch(*p)
		{
		case '#':
		case ';':
		case ',':
			*p++ = '\0';
			while( (*p != '\n') && (p < pend) )
				p++;
			if(p == pend)
				count = 1;
			break;
		case '\n':
			*p++ = '\0';
			if(this->FillConfig(count, item) != 0)
			{
				return CMODEL_PARSE_ERROR;
			}
			Initem = 0;
			count = 0;
			this->gInputLine++;
			break;
		case ' ':
		case '\t':
		case '\0':
			*p++ = '\0';
			if(Initem)
			{
				Initem = 0;
			}
			break;
		default:
			if(!Initem)
			{
				item[count++] = p;
				Initem = 1;
			}
			p++;
			break;
		}
	}

	if(count == 0)
	{
		*offset = 0;
	}
	else
	{
		*offset = (int)(pend - item[0]);
		memcpy(buf, item[0], *offset);
	}

	return CMODEL_SUCCEED;
}

void CCmodelSim::SaveRegList(int adr, int val)
{
	FILE *fp = NULL;
	static int flag = 0;

	if(flag == 0)
	{
		fp = fopen(REGFILE, "wt");
		if(fp == NULL)
			return;
		flag ++;
	}
	else
	{
		fp = fopen(REGFILE, "at");
		if(fp == NULL)
			return;
	}
	fprintf(fp, "%x@%x\n", val, adr);
	fclose(fp);
}

void CCmodelSim::InitStreamHeader(ImageStream *pIs)
{
	pIs->width = this->SimGetReg(REG_SIM_STREAMWIDTH_H);
	pIs->width <<= 8;
	pIs->width += this->SimGetReg(REG_SIM_STREAMWIDTH_L);

	pIs->height = this->SimGetReg(REG_SIM_STREAMHEIGHT_H);
	pIs->height <<= 8;
	pIs->height += this->SimGetReg(REG_SIM_STREAMHEIGHT_L);

	pIs->vs = 0;
	pIs->format = this->SimGetReg(REG_SIM_FORMAT);
}

int CCmodelSim::SendFirstLine(CChipBase1 *pModule, ImageStream *pIs)
{
	pIs->vs = 1;
	return this->SendLine(pModule, pIs);
}

#define MAX_OUTPUT_PIN		16

int CCmodelSim::SendLine(CChipBase1 *pModule, ImageStream *pIs)
{
	ImageStream		ISbuf[MAX_OUTPUT_PIN];
	CChipBase1 * pM[MAX_OUTPUT_PIN];
	int n, i, j, flag[MAX_OUTPUT_PIN];

	if(pModule == (CChipBase1 *)&(this->gSif))
		flag[0] = this->gSif.Process(pIs, &ISbuf[0]);
	if(pModule == (CChipBase1 *)&(this->gIsp))
		flag[0] = this->gIsp.Process(pIs, &ISbuf[0], &(this->gEdge));
	if(pModule == (CChipBase1 *)&(this->gIpp))
		flag[0] = this->gIpp.Process(pIs, &(this->gEdge), &ISbuf[0], &ISbuf[1]);
	if(pModule == (CChipBase1 *)&(this->gLbuf))
	{
		if(pIs != NULL)
			flag[0] = this->gLbuf.Process(pIs, &(this->gLbufBlock));
		else
			flag[0] = this->gLbuf.Process(&(this->gLbufBlock), ISbuf);
	}
	if(pModule == (CChipBase1 *)&(this->gJpeg))
		flag[0] = this->gJpeg.Process(&(this->gLbufBlock), &(this->gJbufBlock));
	if(pModule == (CChipBase1 *)&(this->gLcdIF))
		flag[0] = this->gLcdIF.Process(pIs, &ISbuf[0]);
	if(pModule == (CChipBase1 *)&(this->gJbuf))
//		flag[0] = this->gJbuf.Process(pIs, &ISbuf[0]);
		return FLOW_FRAME;
	if(pModule == NULL)
		return FLOW_FRAME;

//	pModule = temp;
	if(flag[0] == FLOW_ERROR)
		return flag[0];

	n = this->gBusIF.NextModule(pModule, pM);
	for(i = 0; i < n; i++)
	{
		if( (pM[i] == (CChipBase1 *)&(this->gLbuf)) && (pModule == (CChipBase1 *)&(this->gJpeg)) )
			flag[i+1] = this->SendLine(pM[i], NULL);
		else if( (pM[i] == (CChipBase1 *)&(this->gIpp)) && (pModule == (CChipBase1 *)&(this->gLbuf)) )
		{
			for(j = 0; j < 16; j++)
			{
				flag[i+1] = this->SendLine(pM[i], &ISbuf[j]);
				if(flag[i+1] == FLOW_ERROR)
					return FLOW_ERROR;
			}
		}
		else
			flag[i+1] = this->SendLine(pM[i], &ISbuf[i]);

		if(flag[i+1] == FLOW_ERROR)
			return FLOW_ERROR;
	}
	for(i = 0; i < n+1; i++)
	{
		if(flag[i] == FLOW_NORMAL)
			return FLOW_NORMAL;
	}

	return FLOW_FRAME;
}

int CCmodelSim::SendLastLine(CChipBase1 *pModule, ImageStream *pIs)
{
	int flag;

	flag = this->SendLine(pModule, pIs);
	pIs->width = 0;
	while( (flag != FLOW_FRAME) && (flag != FLOW_ERROR) )
	{
		flag = this->SendLine(pModule, pIs);
	}

	return flag;
}

void CCmodelSim::ClearEdge(void)
{
	this->gEdge.Clear();
	this->gEdge.format = YUV422;
}

void CCmodelSim::ProcessSim(void)
{
	FILE *fp = NULL;
	int	j;
	int		times = 1;
	int		line = 0;
	ImageStream		ISbuf;

	switch(this->gSelect)
	{
	case SIM_START_SIF:
		fp = fopen(gDbugMap[SIF_INPUT_FILE].filename, "rb");
		break;
	case SIM_START_ISP:
		fp = fopen(gDbugMap[ISP_INPUT_FILE].filename, "rb");
		break;
	case SIM_START_IPP:
		fp = fopen(gDbugMap[IPP_INPUT_FILE].filename, "rb");
		break;
//	case SIM_START_LBUF:
//		fp = fopen(gDbugMap[LBUF_INPUT_FILE].filename, "rb");
//		break;
//	case SIM_START_JPEG:
//		fp = fopen(gDbugMap[JPEG_INPUT_FILE].filename, "rb");
//		break;
	default:
		Gdbug.DbugMsg("Error test selection!!!\n");
		return;
	}

	if(NULL == fp)
	{
		Gdbug.DbugMsg("Can't open sensor input file: %s\n", gDbugMap[SIF_INPUT_FILE].filename);
		return;
	}

	times = this->SimGetReg(REG_LOOP_TEST);
	while( times-- )
	{
		line = 0;
		rewind(fp);

		do {
			this->InitStreamHeader(&ISbuf);
			for(j = 0; j < ISbuf.width; j++)
			{
				if( fscanf(fp, "%x", ISbuf.buf16+j) == EOF )
				{
					line = ISbuf.height + 10;
					j = ISbuf.width + 10;
				}
			}
			this->ClearEdge();
			if(line == 0)
			{
				line ++;
				if(this->SendFirstLine(this->gpModule, &ISbuf) == FLOW_ERROR)
					line = ISbuf.height + 10;
			}
			else if(line < ISbuf.height - 1)
			{
				line ++;
				if(this->SendLine(this->gpModule, &ISbuf) == FLOW_ERROR)
					line = ISbuf.height + 10;
			}
			else if(line == ISbuf.height - 1)
			{
				line = 0;
				if(this->SendLastLine(this->gpModule, &ISbuf) == FLOW_ERROR)
					line = ISbuf.height + 10;
			}
		} while(line < ISbuf.height);
	}

	fclose(fp);
}

void CCmodelSim::Process(void)
{
	switch(this->SimGetReg(REG_SIM_MODE))
	{
	case SIM_WHOLECHIP:
		this->gBusIF.gpSif = (CChipBase1 *)&(this->gSif);
		this->gBusIF.gpIsp = (CChipBase1 *)&(this->gIsp);
		this->gBusIF.gpIpp = (CChipBase1 *)&(this->gIpp);
		this->gBusIF.gpJbuf = (CChipBase1 *)&(this->gJbuf);
		this->gBusIF.gpJpeg = (CChipBase1 *)&(this->gJpeg);
		this->gBusIF.gpLbuf = (CChipBase1 *)&(this->gLbuf);
		this->gBusIF.gpLcdIF = (CChipBase1 *)&(this->gLcdIF);

		this->gSelect = SIM_START_SIF;
		this->gpModule = (CChipBase1 *)&(this->gSif);
		break;

	case SIM_SIF2ISP:
		this->gBusIF.gpSif = (CChipBase1 *)&(this->gSif);
		this->gBusIF.gpIsp = (CChipBase1 *)&(this->gIsp);
		this->gBusIF.gpIpp = NULL;
		this->gBusIF.gpJbuf = NULL;
		this->gBusIF.gpJpeg = NULL;
		this->gBusIF.gpLbuf = NULL;
		this->gBusIF.gpLcdIF = NULL;

		this->gSelect = SIM_START_SIF;
		this->gpModule = (CChipBase1 *)&(this->gSif);
		break;

	case SIM_ISP2IPP:
		this->gBusIF.gpSif = NULL;
		this->gBusIF.gpIsp = (CChipBase1 *)&(this->gIsp);
		this->gBusIF.gpIpp = (CChipBase1 *)&(this->gIpp);
		this->gBusIF.gpLbuf = (CChipBase1 *)&(this->gLbuf);
		this->gBusIF.gpJpeg = (CChipBase1 *)&(this->gJpeg);
		this->gBusIF.gpJbuf = NULL;
		this->gBusIF.gpLcdIF = NULL;

		this->gSelect = SIM_START_ISP;
		this->gpModule = (CChipBase1 *)&(this->gIsp);
		break;

	case SIM_IPP2LBUF:
		this->gBusIF.gpSif = NULL;
		this->gBusIF.gpIsp = NULL;
		this->gBusIF.gpIpp = (CChipBase1 *)&(this->gIpp);
		this->gBusIF.gpLbuf = (CChipBase1 *)&(this->gLbuf);
		this->gBusIF.gpJpeg = (CChipBase1 *)&(this->gJpeg);
		this->gBusIF.gpJbuf = NULL;
		this->gBusIF.gpLcdIF = NULL;

		this->gSelect = SIM_START_IPP;
		this->gpModule = (CChipBase1 *)&(this->gIpp);
		break;
	case SIM_LBUF2JPEG:
		break;
	case SIM_IPP2LCDIF:
		break;
	default:
		break;
	}

	this->ProcessSim();
}


