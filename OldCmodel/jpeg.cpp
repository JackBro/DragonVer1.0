
#include "dbugPrint.h"
#include "jpeg.h"


//================ const table =============

const UINT8 bJPEGDefaultLumTbl[64] =
{
	16,  11,  12,  14,  12,  10,  16,  14,
	13,  14,  18,  17,  16,  19,  24,  40, 
	26,  24,  22,  22,  24,  49,  35,  37,
	29,  40,  58,  51,  61,  60,  57,  51,
	56,  55,  64,  72,  92,  78,  64,  68, 
	87,  69,  55,  56,  80, 109,  81,  87,
	95,  98, 103, 104, 103,  62,  77, 113,
   121, 112, 100, 120,  92, 101, 103,  99
};

const UINT8 bDefaultLumTbl[64] =
{
	16,  11,  10,  16,  24,  40,  51,  61,
	12,  12,  14,  19,  26,  58,  60,  55, 
	14,  13,  16,  24,  40,  57,  69,  56,
	14,  17,  22,  29,  51,  87,  80,  62,
	18,  22,  37,  56,  68, 109, 103,  77, 
	24,  35,  55,  64,  81, 104, 113,  92,
	49,  64,  78,  87, 103, 121, 120, 101,
    72,  92,  95,  98, 112, 100, 103,  99
};

/* Zig Zag order of JPEG default chrominace quantization table (for JPEG header) */
const UINT8 bJPEGDefaultChrTbl[64] =
{
	17, 18, 18, 24, 21, 24, 47, 26,
	26, 47, 99, 66, 56, 66, 99, 99,
	99, 99, 99, 99, 99, 99, 99, 99,
	99, 99, 99, 99, 99, 99, 99, 99,
	99, 99, 99, 99, 99, 99, 99, 99,
	99, 99, 99, 99, 99, 99, 99, 99,
	99, 99, 99, 99, 99, 99, 99, 99,
	99, 99, 99, 99, 99, 99, 99, 99
};

const UINT8 bDefaultChrTbl[64] =
{
	17, 18, 24, 47, 99, 99, 99, 99,
	18, 21, 26, 66, 99, 99, 99, 99,
	24, 26, 56, 99, 99, 99, 99, 99,
	47, 66, 99, 99, 99, 99, 99, 99,
	99, 99, 99, 99, 99, 99, 99, 99,
	99, 99, 99, 99, 99, 99, 99, 99,
	99, 99, 99, 99, 99, 99, 99, 99,
	99, 99, 99, 99, 99, 99, 99, 99
};

const UINT8 bZigZag[64] =
{
	 0,  1,  5,  6, 14, 15, 27, 28,
	 2,  4,  7, 13, 16, 26, 29, 42,
	 3,  8, 12, 17, 25, 30, 41, 43,
	 9, 11, 18, 24, 31, 40, 44, 53,
	10, 19, 23, 32, 39, 45, 52, 54,
	20, 22, 33, 38, 46, 51, 55, 60,
	21, 34, 37, 47, 50, 56, 59, 61,
	35, 36, 48, 49, 57, 58, 62, 63
};

const UINT8 bDefaultHuffmanTbl[416] =
{
    0x00, 0x00, 0x01, 0x05, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
    0x07, 0x08, 0x09, 0x0a, 0x0b, 0x01, 0x00, 0x03,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 
    0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
    0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,
    0x0a, 0x0b, 0x10, 0x00, 0x02, 0x01, 0x03, 0x03,
    0x02, 0x04, 0x03, 0x05, 0x05, 0x04, 0x04, 0x00,
    0x00, 0x01, 0x7d, 0x01, 0x02, 0x03, 0x00, 0x04,
    0x11, 0x05, 0x12, 0x21, 0x31, 0x41, 0x06, 0x13,
    0x51, 0x61, 0x07, 0x22, 0x71, 0x14, 0x32, 0x81,
    0x91, 0xa1, 0x08, 0x23, 0x42, 0xb1, 0xc1, 0x15,
    0x52, 0xd1, 0xf0, 0x24, 0x33, 0x62, 0x72, 0x82,
    0x09, 0x0a, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x25,
    0x26, 0x27, 0x28, 0x29, 0x2a, 0x34, 0x35, 0x36,
    0x37, 0x38, 0x39, 0x3a, 0x43, 0x44, 0x45, 0x46,
    0x47, 0x48, 0x49, 0x4a, 0x53, 0x54, 0x55, 0x56,
    0x57, 0x58, 0x59, 0x5a, 0x63, 0x64, 0x65, 0x66,
    0x67, 0x68, 0x69, 0x6a, 0x73, 0x74, 0x75, 0x76,
    0x77, 0x78, 0x79, 0x7a, 0x83, 0x84, 0x85, 0x86,
    0x87, 0x88, 0x89, 0x8a, 0x92, 0x93, 0x94, 0x95,
    0x96, 0x97, 0x98, 0x99, 0x9a, 0xa2, 0xa3, 0xa4,
    0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xb2, 0xb3,
    0xb4, 0xb5, 0xb6, 0xb7, 0xb8, 0xb9, 0xba, 0xc2,
    0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xcA,
    0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7, 0xd8, 0xd9,
    0xda, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7,
    0xe8, 0xe9, 0xea, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5,
    0xf6, 0xf7, 0xf8, 0xf9, 0xfa, 0x11, 0x00, 0x02,
    0x01, 0x02, 0x04, 0x04, 0x03, 0x04, 0x07, 0x05,
    0x04, 0x04, 0x00, 0x01, 0x02, 0x77, 0x00, 0x01,
    0x02, 0x03, 0x11, 0x04, 0x05, 0x21, 0x31, 0x06,
    0x12, 0x41, 0x51, 0x07, 0x61, 0x71, 0x13, 0x22,
    0x32, 0x81, 0x08, 0x14, 0x42, 0x91, 0xa1, 0xb1,
    0xc1, 0x09, 0x23, 0x33, 0x52, 0xf0, 0x15, 0x62,
    0x72, 0xd1, 0x0a, 0x16, 0x24, 0x34, 0xe1, 0x25,
    0xf1, 0x17, 0x18, 0x19, 0x1a, 0x26, 0x27, 0x28,
    0x29, 0x2a, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a,
    0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a,
    0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a,
    0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6a,
    0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7a,
    0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89,
    0x8a, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98,
    0x99, 0x9a, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7,
    0xa8, 0xa9, 0xaa, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6,
    0xb7, 0xb8, 0xb9, 0xba, 0xc2, 0xc3, 0xc4, 0xc5,
    0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xd2, 0xd3, 0xd4,
    0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda, 0xe2, 0xe3,
    0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9, 0xea, 0xf2,
    0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8, 0xf9, 0xfa
};

const UINT8 bJPEGDefaultimgpara[15] = 
{
    00,  00,  00,  00,  00,  00,  
    00,  00,  00,  00,  00,  00, 00, 00, 00
};



#if 0
static void printArray(SINT32 *inblk)
{
	int i;

	for(i = 0; i < 64; i++)
	{
		Gdbug.DbugInterData("%ld\t", inblk[i]);
		if( (i+1) % 8 == 0 )
			Gdbug.DbugInterData("\n");
	}
}


static void huff2str(char *tmsg, int val, int len)
{
	int i;

	tmsg[0] = 0;
	for(i = len-1; i >= 0; i--)
	{
		if( val & (1 << i) )
			sprintf(tmsg, "%s1", tmsg);
		else
			sprintf(tmsg, "%s0", tmsg);
	}
	sprintf(tmsg, "%s ",tmsg);
}

static void printJpegTable(JpegTable *table)
{
	int i;
	char str[30];
	FILE *fp;

	fp = fopen("mytable.txt", "wt");
	if(fp == NULL)
	{
		printf("error open table.txt");
		return;
	}
	fprintf(fp, "scale factor:\n");
	for(i = 0; i < 64; i++)
	{
		fprintf(fp, "%d\t", table->sfblk[i]);
		if( ((i+1) % 8) == 0 )
			fprintf(fp, "\n");
	}
	fprintf(fp, "\n\ninvtable:\n");
	for(i = 0; i < 256; i++)
	{
		fprintf(fp, "%d\t", table->invTable[i]);
		if( ((i+1) % 8) == 0 )
			fprintf(fp, "\n");
	}
	fprintf(fp, "\n\nLum shift table:\n");
	for(i = 0; i < 128; i++)
	{
		fprintf(fp, "%d\t", table->sftL[i]);
		if( ((i+1) % 8) == 0 )
			fprintf(fp, "\n");
	}
	fprintf(fp, "\n\nChorm shift table:\n");
	for(i = 0; i < 128; i++)
	{
		fprintf(fp, "%d\t", table->sftC[i]);
		if( ((i+1) % 8) == 0 )
			fprintf(fp, "\n");
	}
	fprintf(fp, "\n\nLum brc table:\n");
	for(i = 0; i < 64; i++)
	{
		fprintf(fp, "%d\t", table->brcL[i]);
		if( ((i+1) % 8) == 0 )
			fprintf(fp, "\n");
	}
	fprintf(fp, "\n\nChorm brc table:\n");
	for(i = 0; i < 64; i++)
	{
		fprintf(fp, "%d\t", table->brcC[i]);
		if( ((i+1) % 8) == 0 )
			fprintf(fp, "\n");
	}
	fprintf(fp, "\n\nLum huffman DC table:\n");
	for(i = 0; i < 257; i++)
	{
		huff2str(str, table->Ldc[i].value, table->Ldc[i].length);
		fprintf(fp, "0x%x: %s\t", i, str);
		if( ((i+1) % 8) == 0 )
			fprintf(fp, "\n");
	}
	fprintf(fp, "\n\nLum huffman AC table:\n");
	for(i = 0; i < 257; i++)
	{
		huff2str(str, table->Lac[i].value, table->Lac[i].length);
		fprintf(fp, "0x%x: %s\t", i, str);
		if( ((i+1) % 8) == 0 )
			fprintf(fp, "\n");
	}
	fprintf(fp, "\n\nChrom huffman DC table:\n");
	for(i = 0; i < 257; i++)
	{
		huff2str(str, table->Cdc[i].value, table->Cdc[i].length);
		fprintf(fp, "0x%x: %s\t", i, str);
		if( ((i+1) % 8) == 0 )
			fprintf(fp, "\n");
	}
	fprintf(fp, "\n\nChrom huffman AC table:\n");
	for(i = 0; i < 257; i++)
	{
		huff2str(str, table->Cac[i].value, table->Cac[i].length);
		fprintf(fp, "0x%x: %s\t", i, str);
		if( ((i+1) % 8) == 0 )
			fprintf(fp, "\n");
	}
	fprintf(fp, "\n\nLQtable:\n");
	for(i = 0; i < 64; i++)
	{
		fprintf(fp, "%d\t", table->LQTable[i]);
		if( ((i+1) % 8) == 0 )
			fprintf(fp, "\n");
	}
	fprintf(fp, "\n\nCQtable:\n");
	for(i = 0; i < 64; i++)
	{
		fprintf(fp, "%d\t", table->CQTable[i]);
		if( ((i+1) % 8) == 0 )
			fprintf(fp, "\n");
	}

	fclose(fp);
}

static void printQtable(SINT32 *LQTable, SINT32 *CQTable, int Qfactor)
{
	FILE *fp;
	int i;

	fp = fopen("myqtable.txt", "at");
	fprintf(fp, "\n\nQfactor = 0x%x", Qfactor);
	fprintf(fp, "\n\nLQtable:\n");
	for(i = 0; i < 64; i++)
	{
		fprintf(fp, "0x%x\t", LQTable[i]);
		if( ((i+1) % 8) == 0 )
			fprintf(fp, "\n");
	}
	fprintf(fp, "\n\nCQtable:\n");
	for(i = 0; i < 64; i++)
	{
		fprintf(fp, "0x%x\t", CQTable[i]);
		if( ((i+1) % 8) == 0 )
			fprintf(fp, "\n");
	}
	fclose(fp);
}
#endif

//================ class CJpegEncode =========

CJpegEncode::CJpegEncode(void)
{
	this->InitJpegTable(&this->gTable);
}

int CJpegEncode::VsyncInit(ImageBlock *pIn)
{
	if(CChipBase1::VsyncInit(pIn) == FALSE)
		return FALSE;

	this->gqf = this->GetInterReg(REG_JPEG_QF);
	this->updateBRCTable(&(this->gTable), this->gqf);
	this->glastUdc = 0;
	this->glastYdc = 0;
	this->glastVdc = 0;
	this->gLastChar = 0;
	this->gStartPos = 0;
	this->gLength = 0;
	this->gVwc = 0;
	return TRUE;
}

void CJpegEncode::generateBRC(SINT32 *QTable, int sftC, SINT32 *output)
{
	int  i;

	for(i = 0; i < 64; i++) 
	{
		output[i] = QTable[i] >> (16 - sftC);
		if(sftC > 2)
			output[i] = (QTable[i] >> 14) << (sftC-2);
	}
}

void CJpegEncode::updateBRCTable(JpegTable *table, int qf)
{
	/* Recalculate BRC output in the beginning of every field */
	this->getQTable(table->sfblk, table->LQTable, table->CQTable, table->invTable, qf);
	this->generateBRC(table->LQTable, table->sftL[qf], table->brcL);
	this->generateBRC(table->CQTable, table->sftC[qf], table->brcC);

//	printJpegTable(table);
}

void CJpegEncode::DCTscaleFactor(SINT32 *sfblk)
{
	int	i, j;
	double pi = 3.1415926535;
	double w[8];

	w[0] = cos(pi/4)*2;
	w[1] = cos(pi/16);
	w[2] = cos(pi/8);
	w[3] = w[1];
	w[4] = w[0];
	w[5] = w[1];
	w[6] = w[2];
	w[7] = w[1];

	for (i = 0; i < 8; i++)
		for (j = 0; j < 8; j++)
			sfblk[i*8+j] = this->roundDouble(w[i] * w[j] * 65536.0);
}

void CJpegEncode::getInvTable(SINT32 *invTable)
{
	int  i;

	invTable[0] = 1 << 15;

	for (i = 1; i < 256; i++)
//		invTable[i] = (long)((1.0/((double)i)) * pow(2.0, 15.0) + 0.5);
		invTable[i] = (SINT32)( ( 1.0 / ((double)i) ) * 32768.0 + 0.5 );
}

void CJpegEncode::getQTable(SINT32 *sfblk, SINT32 *LQTable, SINT32 *CQTable, SINT32 *invTable, int Qfactor)
{
	int  i;
	SINT32 tmp, sf;

	for(i = 0; i < 64; i++) 
	{
		sf = sfblk[i];
		tmp = ( (SINT32)((SINT8)bDefaultLumTbl[i]) * Qfactor + 32 ) >> 6;
		tmp = (tmp == 0) ? 1 : tmp;
		LQTable[i] = sf * (invTable[tmp] >> 1) + ((invTable[tmp] % 2) ? (sf >> 1) : 0);
		if( (tmp != 1) && (sf != 0x20000) )
			LQTable[i] += 16384;

		tmp = ( (SINT32)((SINT8)bDefaultChrTbl[i]) * Qfactor + 32 ) >> 6;
		tmp = (tmp == 0) ? 1 : tmp;
		CQTable[i] = sf * (invTable[tmp] >> 1) + ((invTable[tmp] % 2) ? (sf >> 1) : 0);
		if( (tmp != 1) && (sf != 0x20000) )
			CQTable[i] += 16384;
	}
}

int CJpegEncode::getShiftValue(SINT32 *QTable)
{
	int  i;
	SINT32 max = 0;

	for(i = 0; i < 64; i++)
		max = (QTable[i] > max) ? QTable[i] : max;

	for (i = 31; i >= 23 ; i--)
		if (max & (1L << i))
			break;

	return 31-i;
}

void CJpegEncode::createHuffmanTable(UINT8 *bits, UINT8 *values, HuffValue *table)
{
	int  i, j, k;
	int  size, code;
	int  sizeValues;
	int  huffSize[257];
	int  huffCode[257];

	/*	Get length of values */
	for (sizeValues = 0, i = 0; i < 16; i++)
		sizeValues += bits[i];

	/*	Calculate huffLen from bits */
	/*	If bits is { 0,1,5,1,1,1,1,1,1,0,0,0,0,0,0,0, }, then
		the huffLen is { 2,3,3,3,3,3,4,5,6,7,8,9 } */
	for (k = 0, i = 0; i < 16 ; i++)
		for (j = 1 ; j <= bits[i]; j++)
			huffSize[k++] = i+1;
	huffSize[k] = 0;

	k		= 0;
	code	= 0;
	size	= huffSize[0];

	i = 1;
	while(i) 
	{
		do {
			huffCode[k++] = code++;
		} while ((huffSize[k] == size) && (k < 257));

		if (huffSize[k] == 0)	/* All finished */
			break;

		do {
			code <<= 1;
			size++;
		} while (huffSize[k] != size);
	}

	for (i = 0; i < 257; i++)
		table[i].length = 0;

	for (i = 0 ; i < sizeValues ; i++) 
	{
		table[values[i]].value = huffCode[i];
		table[values[i]].length = huffSize[i];
	}
}

void CJpegEncode::InitJpegTable(JpegTable *table)
{
	int i;

	memset(table, 0, sizeof(JpegTable));

	this->DCTscaleFactor(table->sfblk);
	this->getInvTable(table->invTable);
	for(i = 0; i < 128; i++)
	{
		this->getQTable(this->gTable.sfblk, this->gTable.LQTable, this->gTable.CQTable, this->gTable.invTable, i);
//		printQtable(this->gTable.LQTable, this->gTable.CQTable, i);
		this->gTable.sftL[i] = this->getShiftValue(this->gTable.LQTable);
		this->gTable.sftC[i] = this->getShiftValue(this->gTable.CQTable);
	}
//	Gdbug.DbugData("Huffman table of Ldc:\n");
	this->createHuffmanTable( (UINT8 *)bDefaultHuffmanTbl+1, (UINT8 *)bDefaultHuffmanTbl+17, this->gTable.Ldc );
//	Gdbug.DbugData("Huffman table of Cdc:\n");
	this->createHuffmanTable( (UINT8 *)bDefaultHuffmanTbl+30, (UINT8 *)bDefaultHuffmanTbl+46, this->gTable.Cdc );
//	Gdbug.DbugData("Huffman table of Lac:\n");
	this->createHuffmanTable( (UINT8 *)bDefaultHuffmanTbl+59, (UINT8 *)bDefaultHuffmanTbl+75, this->gTable.Lac );
//	Gdbug.DbugData("Huffman table of Cac:\n");
	this->createHuffmanTable( (UINT8 *)bDefaultHuffmanTbl+238, (UINT8 *)bDefaultHuffmanTbl+254, this->gTable.Cac );
//	printJpegTable(table);
}

void CJpegEncode::generateJPEGHeader(ImageBlock *pOut)
{
	int i, val;
	int	width, height;
	int		x0, x1;
	UINT8	*pbStart = pOut->buf8;

	x1 = this->GetInterReg(REG_JPEG_WIDTH_H);
	x0 = this->GetInterReg(REG_JPEG_WIDTH_L);
	width = (UINT16)x1;
	width <<= 8;
	width |= (UINT16)x0 & 0xff;
	x1 = this->GetInterReg(REG_JPEG_HEIGHT_H);
	x0 = this->GetInterReg(REG_JPEG_HEIGHT_L);
	height = (UINT16)x1;
	height <<= 8;
	height |= (UINT16)x0 & 0xff;

	PutWord(&pbStart, SOI);
	PutWord(&pbStart, COM);
	PutWord(&pbStart, 0x24);
	PutWord(&pbStart, 0x0);		//frame count
	PutWord(&pbStart, 0x0);		//time stamp
	for(i = 0; i < 15; i++)
		PutByte(&pbStart, 0x0);
	PutLittleWord(&pbStart, height);
	PutLittleWord(&pbStart, width);
	PutByte(&pbStart, this->gqf);
	PutByte(&pbStart, 0x0);		//jpeg mode
	switch(pOut->format)
	{
	case YUV422BLK:
		PutByte(&pbStart, 0x32);
		PutByte(&pbStart, 0x12);
		PutByte(&pbStart, 0x0b);
		PutByte(&pbStart, 0x51);
		PutByte(&pbStart, 0x04);
		PutByte(&pbStart, 0x51);
		PutByte(&pbStart, 0x04);
		PutByte(&pbStart, 0x00);
		PutByte(&pbStart, 0x00);
		break;
	case YUV420BLK:
		PutByte(&pbStart, 0x32);
		PutByte(&pbStart, 0x22);
		PutByte(&pbStart, 0x13);
		PutByte(&pbStart, 0x51);
		PutByte(&pbStart, 0x04);
		PutByte(&pbStart, 0x51);
		PutByte(&pbStart, 0x04);
		PutByte(&pbStart, 0x00);
		PutByte(&pbStart, 0x00);
		break;
	case YUV411BLK:
		PutByte(&pbStart, 0x32);
		PutByte(&pbStart, 0x14);
		PutByte(&pbStart, 0x13);
		PutByte(&pbStart, 0x51);
		PutByte(&pbStart, 0x04);
		PutByte(&pbStart, 0x51);
		PutByte(&pbStart, 0x04);
		PutByte(&pbStart, 0x00);
		PutByte(&pbStart, 0x00);
		break;
	case YUV400BLK:
		PutByte(&pbStart, 0x11);
		PutByte(&pbStart, 0x11);
		PutByte(&pbStart, 0x07);
		PutByte(&pbStart, 0x00);
		PutByte(&pbStart, 0x00);
		PutByte(&pbStart, 0x00);
		PutByte(&pbStart, 0x00);
		PutByte(&pbStart, 0x00);
		PutByte(&pbStart, 0x00);
		break;
	default:
		return;
	}
	if( (this->GetInterReg(REG_JPEG_MODE) & 4) == 0 )	//simple header disable
	{
		PutWord(&pbStart, DQT);
		PutWord(&pbStart, 132);		//length
		PutByte(&pbStart, 0x00);	//Q table precision and Id
		for (i = 0; i < 64; i++) 
		{ 
			val = (int)bJPEGDefaultLumTbl[i] & 0xff;
			val *= this->gqf;
			val += 32;
			val >>= 6;
			val = (val == 0) ? 1 : (val > 255 ? 255 : val);
			PutByte(&pbStart, (UINT8)val);
		}
		PutByte(&pbStart, 0x01);  /* Q Table precision and Id */
		for (i = 0; i < 64; i++) 
		{
			val = (int)bJPEGDefaultChrTbl[i] & 0xff;
			val *= this->gqf;
			val += 32;
			val >>= 6;
			val = (val == 0) ? 1 : (val > 255 ? 255 : val);
			PutByte(&pbStart, (UINT8)val);
		}
		if(pOut->format == YUV400BLK)	//gray image
		{
			PutWord(&pbStart, COM);
			PutWord(&pbStart, 3);
			PutByte(&pbStart, 0);
		}
		else
		{
			PutWord(&pbStart, COM);
			PutWord(&pbStart, 5);
			PutByte(&pbStart, 0);
			PutByte(&pbStart, 0);
			PutByte(&pbStart, 0);
		}
		PutWord(&pbStart, SOF);
		if(pOut->format == YUV400BLK)	//gray image
			PutWord(&pbStart, 11);	/* segment length */
		else
			PutWord(&pbStart, 17);	/* segment length */
		PutByte(&pbStart, 8);		/* sample precisio */
		PutWord(&pbStart, height);	/* image lines per frame */
		PutWord(&pbStart, width);	/* samples per line */

		if(pOut->format == YUV400BLK)	//gray image
		{
			PutByte(&pbStart, 1);	/* Y component */
			PutByte(&pbStart, 1);	/* component ID */
			PutByte(&pbStart, 0x11);/* horizontal/vertical sampling factor */
			PutByte(&pbStart, 0);	/* Q table selector */
		}
		else 
		{
			PutByte(&pbStart, 3);	/* YUV color component */
			for(i = 1; i < 4; i++)	
			{
				PutByte(&pbStart, (UINT8)i);	/* component ID */
				if(i == 1)	
				{
					/* horizontal/vertical sampling factor */
					PutByte(&pbStart, (pOut->format == YUV411BLK) ? 0x41 : (pOut->format == YUV422BLK) ? 0x21 : 0x22);
					PutByte(&pbStart, 0);	/* Q table selector */
				}
				else 
				{
					PutByte(&pbStart, 0x11);	/* horizontal/vertical sampling factor */
					PutByte(&pbStart, 0x1);		/* Q table selector */
				}
			}
		}
		PutWord(&pbStart, DHT);		//huffman table
		PutWord(&pbStart, 418);		/* length */
		PutData(&pbStart, (UINT8 *)bDefaultHuffmanTbl, sizeof(bDefaultHuffmanTbl));

		PutWord(&pbStart, COM);
		PutWord(&pbStart, 4);
		PutWord(&pbStart, 0);

		PutWord(&pbStart, SOS);
		if(pOut->format == YUV400BLK)	//gray image
		{
			PutWord(&pbStart, 8);	/* segment length */
			PutByte(&pbStart, 1);	/* Y component */
			PutByte(&pbStart, 1);
			PutByte(&pbStart, 0);
		}
		else 
		{
			PutWord(&pbStart, 12);	/* segment length */
			PutByte(&pbStart, 3);	/* YUV color component */
			for (i = 1; i < 4; i++) 
			{
				PutByte(&pbStart, (UINT8)i);
				if (i == 1)
					PutByte(&pbStart, 0);
				else
					PutByte(&pbStart, 0x11);
			}
		}
		PutByte(&pbStart, 0);		/* Spectral or predictor selection */
		PutByte(&pbStart, 0x3f);	/* Stop spectral selection */
		PutByte(&pbStart, 0);		/* appr. bit position */
	}
	pOut->width = (int)(pbStart - pOut->buf8) << 3;
}

SINT32 CJpegEncode::shiftFix(SINT32 input, int shift)
{
	SINT32 result;

	if (shift == 0)
		return input;
	result = input >> (shift-1);

	if( (result & 3) != 0 )
		result = 1;
	else
		result = 0;

	result += (input >> (shift+1)) << 1;
	return result;
}

SINT32 CJpegEncode::roundfc(SINT32 input)
{
	SINT32  tmp;

    tmp = (input << 1) + (input >> 5);	/* sum0 */
	tmp = (input << 1) + (tmp >> 2);	/* sum1 */
	tmp = (input << 1) + (tmp >> 2);	/* sum2 */
	tmp = (input << 1) + (tmp >> 1);	/* sum3 */
	return shiftFix(tmp, 4);
}

SINT32 CJpegEncode::rounddb(SINT32 input)
{
	SINT32  tmp;

	tmp = (input >> 4) + (input << 1);	/* sum0 */
	tmp = (tmp >> 3)   - (input << 1);	/* sum1 */
	tmp = (tmp >> 2)   - (input << 1);	/* sum2 */
	tmp = (tmp >> 3)   + (input << 1);	/* sum3 */
	return shiftFix(tmp, 2);
}

SINT32 CJpegEncode::roundeb(SINT32 input)
{
	SINT32  tmp;

	tmp = (input << 1) + (input >> 5);	/* sum0 */
	tmp = (input << 1) + (tmp >> 4);	/* sum1 */
	tmp = (input << 1) + (tmp >> 3);	/* sum2 */
	return shiftFix(tmp, 3);
}

SINT32 CJpegEncode::roundgb(SINT32 input)
{
	SINT32 tmp, tm0;
	
	tm0 = (-input-1) & 7;
	tmp = (-input-1) - tm0;

	if (tm0 != 0)
		tmp = tmp + 4;

	tmp = (tmp >> 2) - input + 1;		/* sum0 */
	tmp = (tmp >> 3) - (input << 1);	/* sum1 */
	tmp = (tmp >> 2) + (input << 1);	/* sum2 */
	tmp = (tmp >> 2) - (input << 1);	/* sum3 */
	tmp = (tmp >> 2) + (input << 1);	/* sum4 */

	return shiftFix(tmp, 4);
}

SINT32 CJpegEncode::round12(SINT32 input)
{
	return shiftFix(input, 1);
}

SINT32 CJpegEncode::round14(SINT32 input)
{
	return shiftFix(input, 2);
}

SINT32 CJpegEncode::roundDouble(double input)
{
	SINT32 result;

	if(input > 0.0)
		result = (SINT32)(input+0.500000);
	else
		result = (SINT32)(input-0.500000);

	return result;
}

void CJpegEncode::fpdct8x8(SINT32 *inblk, SINT32 *oublk)
{
	int  i;
	SINT32  sum07, sum16, sum25, sum34, dif07, dif16, dif25, dif34;

	for(i = 0; i < 8; i++)
	{
		sum07 = this->shiftFix((inblk[(i<<3)+0]<<14) + (inblk[(i<<3)+7]<<14), 1);
		sum16 = this->shiftFix((inblk[(i<<3)+1]<<14) + (inblk[(i<<3)+6]<<14), 1);
		sum25 = this->shiftFix((inblk[(i<<3)+2]<<14) + (inblk[(i<<3)+5]<<14), 1);
		sum34 = this->shiftFix((inblk[(i<<3)+3]<<14) + (inblk[(i<<3)+4]<<14), 1);
															   
		dif07 = this->shiftFix((inblk[(i<<3)+0]<<14) - (inblk[(i<<3)+7]<<14), 1);
		dif16 = this->shiftFix((inblk[(i<<3)+1]<<14) - (inblk[(i<<3)+6]<<14), 1);
		dif25 = this->shiftFix((inblk[(i<<3)+2]<<14) - (inblk[(i<<3)+5]<<14), 1);
		dif34 = this->shiftFix((inblk[(i<<3)+3]<<14) - (inblk[(i<<3)+4]<<14), 1);

		oublk[(i<<3)+0]	= this->shiftFix(this->round14(sum07) + this->round14(sum16) +
										 this->round14(sum25) + this->round14(sum34), 2);
		oublk[(i<<3)+2] = this->shiftFix(this->round12(sum07) + this->roundfc(sum16) -
										 this->roundfc(sum25) - this->round12(sum34), 2);
		oublk[(i<<3)+4] = this->shiftFix(this->round14(sum07) - this->round14(sum16) -
										 this->round14(sum25) + this->round14(sum34), 2);
		oublk[(i<<3)+6] = this->shiftFix(this->roundfc(sum07) - this->round12(sum16) +
										 this->round12(sum25) - this->roundfc(sum34), 2);
			   
		oublk[(i<<3)+1] = this->shiftFix(this->round12(dif07) + this->rounddb(dif16) +
										 this->roundeb(dif25) + this->roundgb(dif34), 2);
		oublk[(i<<3)+3] = this->shiftFix(this->rounddb(dif07) - this->roundgb(dif16) -
										 this->round12(dif25) - this->roundeb(dif34), 2);
		oublk[(i<<3)+5] = this->shiftFix(this->roundeb(dif07) - this->round12(dif16) +
										 this->roundgb(dif25) + this->rounddb(dif34), 2);
		oublk[(i<<3)+7] = this->shiftFix(this->roundgb(dif07) - this->roundeb(dif16) +
										 this->rounddb(dif25) - this->round12(dif34), 2);
	}
	for(i = 0; i < 8; i++)
	{
		sum07 = this->shiftFix(oublk[i+(0<<3)] + oublk[i+(7<<3)], 1);
		sum16 = this->shiftFix(oublk[i+(1<<3)] + oublk[i+(6<<3)], 1);
		sum25 = this->shiftFix(oublk[i+(2<<3)] + oublk[i+(5<<3)], 1);
		sum34 = this->shiftFix(oublk[i+(3<<3)] + oublk[i+(4<<3)], 1);
												
		dif07 = this->shiftFix(oublk[i+(0<<3)] - oublk[i+(7<<3)], 1);
		dif16 = this->shiftFix(oublk[i+(1<<3)] - oublk[i+(6<<3)], 1);
		dif25 = this->shiftFix(oublk[i+(2<<3)] - oublk[i+(5<<3)], 1);
		dif34 = this->shiftFix(oublk[i+(3<<3)] - oublk[i+(4<<3)], 1);

		inblk[i+(0<<3)]	= this->shiftFix(this->round14(sum07) + this->round14(sum16) +
										 this->round14(sum25) + this->round14(sum34), 2);
		inblk[i+(2<<3)] = this->shiftFix(this->round12(sum07) + this->roundfc(sum16) -
										 this->roundfc(sum25) - this->round12(sum34), 2);
		inblk[i+(4<<3)] = this->shiftFix(this->round14(sum07) - this->round14(sum16) -
										 this->round14(sum25) + this->round14(sum34), 2);
		inblk[i+(6<<3)] = this->shiftFix(this->roundfc(sum07) - this->round12(sum16) +
										 this->round12(sum25) - this->roundfc(sum34), 2);
					
		inblk[i+(1<<3)] = this->shiftFix(this->round12(dif07) + this->rounddb(dif16) +
										 this->roundeb(dif25) + this->roundgb(dif34), 2);
		inblk[i+(3<<3)] = this->shiftFix(this->rounddb(dif07) - this->roundgb(dif16) -
										 this->round12(dif25) - this->roundeb(dif34), 2);
		inblk[i+(5<<3)] = this->shiftFix(this->roundeb(dif07) - this->round12(dif16) +
										 this->roundgb(dif25) + this->rounddb(dif34), 2);
		inblk[i+(7<<3)] = this->shiftFix(this->roundgb(dif07) - this->roundeb(dif16) +
										 this->rounddb(dif25) - this->round12(dif34), 2);
	}
	for (i = 0; i < 64; i++)
		oublk[i] = shiftFix(inblk[i] >> 2, 1);
}

void CJpegEncode::quant(SINT32 *inblk, SINT32 *oublk, SINT32 *brcTable, int sft)
{
	int  i;

	for(i = 0; i < 64; i++)
	{
		oublk[i] = (inblk[i] * brcTable[i]) >> 5;
		oublk[i] >>= 13+sft;
		oublk[i] += 1;
		oublk[i] >>= 1;
	}
}

void CJpegEncode::zigzag(SINT32 *inblk, SINT32 *oublk)
{
	int  i;

	for (i = 0 ; i < 64 ; i++) 
		oublk[bZigZag[i]] = inblk[i];
}

void CJpegEncode::encodeAC(UINT8 *buf, int *pos, SINT32 *zzblk, HuffValue *pEncTable)
{
	int i, j, run = 0;
	SINT32 val;
	HuffValue huff;

	for(i = 1; i < 64; i++)
	{
		if(zzblk[i] == 0)
		{
			run ++;
			continue;
		}
		while(run > 15)
		{
			this->putBits(buf, pos, pEncTable[240]);
			run -= 16;
		}
		val = abs(zzblk[i]);
		for(j = 1; j < 12; j++)
		{
			if( val < (1 << j) )
				break;
		}
		this->putBits(buf, pos, pEncTable[j + (run<<4)]);
		run = 0;
		huff.value = (UINT16)(zzblk[i] - ((zzblk[i] < 0) ? 1 : 0));
		huff.length = j;
		this->putBits(buf, pos, huff);
	}
	if(zzblk[63] == 0)
		this->putBits(buf, pos, pEncTable[0]);
}

void CJpegEncode::encodeDC(UINT8 *buf, int *pos, SINT32 DCvalue, SINT32 *lastDC, HuffValue *pEncTable)
{
	SINT32 diff, absdiff;
	HuffValue val;
	int i;

	diff = DCvalue - *lastDC;
	*lastDC = DCvalue;
	absdiff = diff > 0 ? diff : -diff;
	if(diff == 0)
		i = 0;
	else
	{
		for(i = 0; i < 12; i++)
			if( absdiff < (1 << i) )
				break;
	}
	if(i == 12)
	{
		Gdbug.DbugMsg("error in huffman encode!\n");
		return;
	}
	this->putBits(buf, pos, pEncTable[i]);
	val.value = (UINT16)(diff - ((diff < 0) ? 1 : 0));
	val.length = i;
	this->putBits(buf, pos, val);
}

void CJpegEncode::putBits(UINT8 *buf, int *pos, HuffValue val)
{
	int adr = *pos >> 3;
	int p = *pos % 8;
	int len, length = val.length;

	if(p + val.length > 7)
	{
		len = 8-p;
		buf[adr] |= (val.value >> (val.length - len)) & ((1 << len) - 1);
		if(buf[adr++] == 0xff)
		{
			buf[adr++] = 0;
			*pos += 8;
		}
		buf[adr] = 0;
		val.length -= len;
		p = 0;
	}
	if(p == 0)
	{
		while(val.length > 7)
		{
			val.length -= 8;
			buf[adr] = (UINT8)(val.value >> val.length);
			if(buf[adr++] == 0xff)
			{
				buf[adr++] = 0;
				*pos += 8;
			}
		}
		buf[adr] = 0;
	}
	if(val.length != 0)
	{
		buf[adr] |= ( val.value & ((1 << val.length) - 1) ) << (8-p-val.length);
	}
	*pos += length;
}

static void PrintTable(SINT32 *table)
{
	int i, j;
	FILE *fp;

	fp = fopen("t1.txt", "at");
	for(i = 0; i < 8; i++)
	{
		for(j = 0; j < 8; j++)
		{
			fprintf(fp, "0x%x\t", table[i*8+j]);
		}
		fprintf(fp, "\n");
	}
	fclose(fp);
}

void CJpegEncode::EncYBlock(UINT8 *block, SINT32 *lastDc, ImageBlock *pOut)
{
	SINT32 inblk[64], oublk[64];
	int i;
	static flag = 1;

	for(i = 0; i < 64; i++)
		inblk[i] = (SINT32)((SINT8)(block[i]-0x80));

	if(flag)
		PrintTable(inblk);
	this->fpdct8x8(inblk, oublk);
	if(flag)
		PrintTable(oublk);
	this->quant(oublk, inblk, this->gTable.brcL, this->gTable.sftL[this->gqf]);
	if(flag)
		PrintTable(inblk);
	this->zigzag(inblk, oublk);
	if(flag)
		PrintTable(oublk);
	this->encodeDC(pOut->buf8, &pOut->width, oublk[0], lastDc, this->gTable.Ldc);
	this->encodeAC(pOut->buf8, &pOut->width, oublk, this->gTable.Lac);

	flag = 0;
}

void CJpegEncode::EncUVBlock(UINT8 *block, SINT32 *lastDc, ImageBlock *pOut, int last)
{
	SINT32 inblk[64], oublk[64];
	int i;

	for(i = 0; i < 64; i++)
		inblk[i] = (SINT32)((SINT8)(block[i]-0x80));

	this->fpdct8x8(inblk, oublk);
	this->quant(oublk, inblk, this->gTable.brcC, this->gTable.sftC[this->gqf]);
	this->zigzag(inblk, oublk);
	if(last)
		oublk[63] = 0;
	this->encodeDC(pOut->buf8, &pOut->width, oublk[0], lastDc, this->gTable.Cdc);
	this->encodeAC(pOut->buf8, &pOut->width, oublk, this->gTable.Cac);
}

void CJpegEncode::eofFlush(UINT8 *buf, int *pos, int length)
{
	int last = *pos >> 3;
	int rest = *pos % 8;

	buf[last] |= ((1 << (8-rest)) - 1);// << (8 - rest);
	switch((length >> 3) % 4)
	{
	case 0:
		if(rest == 0)
			buf[last] = 0xff;
		buf[last+1] = 0xff;
		buf[last+2] = 0xff;
		buf[last+3] = 0xd9;
		*pos += 32 - rest;
		break;
	case 1:
		if(rest == 0)
			buf[last] = 0xff;
		buf[last+1] = 0xff;
		buf[last+2] = 0xd9;
		*pos += 24 - rest;
		break;
	case 2:
		if(rest == 0)
		{
			buf[last] = 0xff;
			buf[last+1] = 0xd9;
			*pos += 16;
		}
		else
		{
			buf[last+1] = 0xff;
			buf[last+2] = 0xff;
			buf[last+3] = 0xff;
			buf[last+4] = 0xff;
			buf[last+5] = 0xd9;
			*pos += 48 - rest;
		}
		break;
	case 3:
		if(rest == 0)
			buf[last] = 0xff;
		buf[last+1] = 0xff;
		buf[last+2] = 0xff;
		buf[last+3] = 0xff;
		buf[last+4] = 0xd9;
		*pos += 40 - rest;
		break;
	default:
		break;
	}
}

void CJpegEncode::OutputBlock(ImageBlock *pIn, ImageBlock *pOut)
{
	int i;
	int pos;

	if(this->gInLine == 0)
		this->generateJPEGHeader(pOut);
	pos = pOut->width;
	switch(pIn->format)
	{
	case YUV422BLK:
		for(i = 0; i < pIn->width; i++)
		{
//			Gdbug.DbugData("i = 0x%x: \n", i);
			this->EncYBlock(pIn->yuv422B[i].y0, &this->glastYdc, pOut);
			this->EncYBlock(pIn->yuv422B[i].y1, &this->glastYdc, pOut);
			this->EncUVBlock(pIn->yuv422B[i].u, &this->glastUdc, pOut);
			if( (this->gInLine == pIn->height - 1) && (i == pIn->width - 1) )
				this->EncUVBlock(pIn->yuv422B[i].v, &this->glastVdc, pOut, 1);
			else
				this->EncUVBlock(pIn->yuv422B[i].v, &this->glastVdc, pOut);
		}
		break;
	case YUV420BLK:
		break;
	case YUV411BLK:
		break;
	case YUV400BLK:
		break;
	default:
		break;
	}
	this->gLength += pOut->width - pos;
	if(this->gOutLine == pOut->height-1)
		this->eofFlush(pOut->buf8, &(pOut->width), this->gLength);
	this->gLastChar = pOut->buf8[pOut->width >> 3];
	this->gStartPos = pOut->width % 8;
	pOut->width = (pOut->width >> 3) << 3;
	pOut->format = JPEGDATA;
}

int CJpegEncode::FillImageInfo(ImageBlock *pIn, ImageBlock *pOut)
{
	int flag;

	flag = CChipBase1::FillImageInfo(pIn, pOut);
	if( flag != VSYNC_PERIOD )
		return flag;
	pOut->width = this->gStartPos;
	pOut->buf8[0] = this->gLastChar;
	return VSYNC_PERIOD;
}

void CJpegEncode::AddInline(ImageBlock *pIn)
{
	if( (pIn->width != 0) || (this->gInLine) )	//VSYNC & back porch Vblank
		this->gInLine++;
	this->gVwc += pIn->width << 5;
}

int CJpegEncode::AddOutLine(ImageBlock *pOut)
{
	int vwc = 0, temp;

	if(pOut->width != 0)
	{
		temp = this->GetInterReg(REG_JPEG_VIDEO_WC0);
		vwc = temp << 24;
		temp = this->GetInterReg(REG_JPEG_VIDEO_WC1);
		vwc |= temp << 16;
		temp = this->GetInterReg(REG_JPEG_VIDEO_WC2);
		vwc |= temp << 8;
		temp = this->GetInterReg(REG_JPEG_VIDEO_WC3);
		vwc |= temp;
		if(this->gVwc >= vwc)
		{
			this->gAllow = FRAME_STOP;
			temp = this->GetReg(REG_JPEG_FRAME_CNT1);
			temp ++;
			this->SetReg(REG_JPEG_FRAME_CNT1, temp & 0xff);
			if(temp == 0x100)
			{
				temp = this->GetReg(REG_JPEG_FRAME_CNT0);
				temp = (temp == 0xcf) ? 0 : temp+1;
				this->SetReg(REG_JPEG_FRAME_CNT0, temp);
			}
			return FLOW_FRAME;
		}
	}

	return FLOW_NORMAL;
}

int CJpegEncode::Process(ImageBlock *pIn, ImageBlock *pOut)
{
	int flag;

//	Gdbug.DbugPrintData(JPEG_ENCODE_PRINT_INPUT, pIn);
	this->VsyncInit(pIn);
	if(this->varify(pIn) != SUCCEED)
		return FLOW_ERROR;

	flag = this->FillImageInfo(pIn, pOut);
	if(flag == VSTART_PERIOD)	return FLOW_NORMAL;
	if(flag == VEND_PERIOD)		return FLOW_FRAME;
	if(flag == INVAILD_PERIOD)	return FLOW_ERROR;
	if(flag == INSERT_BLANK)	return FLOW_NORMAL;

	if(this->IsFrameOver(pOut))	return FLOW_FRAME;

	this->OutputBlock(pIn, pOut);
	Gdbug.DbugPrintData(JPEG_ENCODE_PRINT_INPUT, pOut);
	this->AddInline(pIn);
	return this->AddOutLine(pOut);
};



//================ class CJpeg =========

CJpeg::CJpeg(void)
{
	this->RegInit();
}

CJpeg::~CJpeg(void)
{
}

void CJpeg::RegInit(void)
{
	CChipBase1::RegInit();
	//this->SetReg(REG_JPEG_MODE,		0x0);
	//this->SetReg(REG_JPEG_CTRL,		0x0);
	this->SetReg(REG_JPEG_BRC,			0xf8);
	this->SetReg(REG_JPEG_TCR,			0x3d);
	this->SetReg(REG_JPEG_QF,			0x78);
	//this->SetReg(REG_JPEG_TARGET_WC0, 0x0);
	//this->SetReg(REG_JPEG_TARGET_WC1, 0x0);
	this->SetReg(REG_JPEG_TARGET_WC2,	0x28);
	//this->SetReg(REG_JPEG_TARGET_WC3, 0x0);
//	this->SetReg(REG_JPEG_VIDEO_WC0,	0x0);
	this->SetReg(REG_JPEG_VIDEO_WC1,	0x2);
	this->SetReg(REG_JPEG_VIDEO_WC2,	0x58);
//	this->SetReg(REG_JPEG_VIDEO_WC3,	0);
	this->SetReg(REG_JPEG_WIDTH_H,		0x2);
	this->SetReg(REG_JPEG_WIDTH_L,		0x80);
	this->SetReg(REG_JPEG_HEIGHT_H,		0x1);
	this->SetReg(REG_JPEG_HEIGHT_L,		0xe0);
	this->SetReg(REG_JPEG_B_H,			0x1);
	this->SetReg(REG_JPEG_B_L,			0x11);
//	this->SetReg(REG_JPEG_DCT_DATA,		0x0);
//	this->SetReg(REG_JPEG_QUAN_T,		0x0);
//	this->SetReg(REG_JPEG_HUFF_T,		0x0);
	this->SetReg(REG_JPEG_NUM_COMPQUANT,0x3);
	this->SetReg(REG_JPEG_COMP1_P1,		0xa);
	this->SetReg(REG_JPEG_COMP1_P2,		0x8);
	this->SetReg(REG_JPEG_COMP2_P1,		0x49);
	this->SetReg(REG_JPEG_COMP2_P2,		0x7);
	this->SetReg(REG_JPEG_COMP3_P1,		0x49);
	this->SetReg(REG_JPEG_COMP3_P2,		0x7);
//	this->SetReg(REG_JPEG_COMP4_P1,		0x0);
//	this->SetReg(REG_JPEG_COMP4_P2,		0x0);
	this->SetReg(REG_JPEG_BLK_WIDTH,	0xa0);

	//this->SetReg(REG_JPEG_IMGPARA0,		0x0);
	//this->SetReg(REG_JPEG_IMGPARA1,		0x0);
	//this->SetReg(REG_JPEG_IMGPARA2,		0x0);
	//this->SetReg(REG_JPEG_IMGPARA3,		0x0);
	//this->SetReg(REG_JPEG_IMGPARA4,		0x0);
	//this->SetReg(REG_JPEG_IMGPARA5,		0x0);
	//this->SetReg(REG_JPEG_IMGPARA6,		0x0);
	//this->SetReg(REG_JPEG_IMGPARA7,		0x0);
	//this->SetReg(REG_JPEG_IMGPARA8,		0x0);
	//this->SetReg(REG_JPEG_IMGPARA9,		0x0);
	//this->SetReg(REG_JPEG_IMGPARA10,	0x0);
	//this->SetReg(REG_JPEG_IMGPARA11,	0x0);
	//this->SetReg(REG_JPEG_IMGPARA12,	0x0);
	//this->SetReg(REG_JPEG_IMGPARA13,	0x0);
	//this->SetReg(REG_JPEG_IMGPARA14,	0x0);
}

void CJpeg::SetReg(int adr, int val)
{
	CChipBase1::SetReg(adr, val);
	this->gEncode.SetReg(adr, val);
	if(adr == REG_JPEG_BRC)
		this->SetReg(REG_JPEG_QF, val & 0x7f);
}

int	CJpeg::Process(ImageBlock *pLbuf, ImageBlock *pJbuf)
{
	int mode;

	mode = this->GetReg(REG_JPEG_MODE) & 3;

	if(mode == 0)		//capture mode
	{
		Gdbug.DbugPrintData(JPEG_PRINT_INPUT, pLbuf);
		return this->gEncode.Process(pLbuf, pJbuf);
	}

	if( (mode == 1)	|| (mode == 3) )	//display mode & JPEG decode mode
	{
		Gdbug.DbugPrintData(JPEG_PRINT_INPUT, pJbuf);
		pLbuf->format = YUV422BLK;
		pLbuf->vs = 0;
		pLbuf->width = 0;
	}

	if(mode == 2)		//JPEG encode mode
	{
		pJbuf->format = JPEGDATA;
		pJbuf->vs = 0;
		pJbuf->width = 0;
	}

	return FLOW_FRAME;
}




