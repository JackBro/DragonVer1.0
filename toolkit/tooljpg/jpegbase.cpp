
#include "internal.h"

void CJpegBase::createHuffmanTable(UINT8 *bits, UINT8 *values, THuffValue *table)
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
		for (j = 1; j <= bits[i]; j++)
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

	for (i = 0; i < sizeValues; i++) 
	{
		table[values[i]].value = huffCode[i];
		table[values[i]].length = huffSize[i];
	}
}

void CJpegBase::DCTscaleFactor(int *sfblk)
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
	{
		for (j = 0; j < 8; j++)
		{
			sfblk[(i << 3) + j] = this->roundDouble(w[i] * w[j] * 65536.0);
		}
	}
}

void CJpegBase::getInvTable(int *invTable)
{
	int  i;

	invTable[0] = 1 << 15;
	for (i = 1; i < 256; i++)
	{
		invTable[i] = (int)( ( 1.0 / ((double)i) ) * 32768.0 + 0.5 );
	}
}

void CJpegBase::getQTable(int *sfblk, int *LQTable, int *CQTable, int *invTable, int Qfactor)
{
	int  i, tmp;

	for(i = 0; i < 64; i++) 
	{
//		sf = sfblk[i];
		tmp = ( (int)((SINT8)gDefaultLumTbl[i]) * Qfactor + 32 ) >> 6;
		tmp = (tmp == 0) ? 1 : tmp;
		LQTable[i] = sfblk[i] * (invTable[tmp] >> 1) + ((invTable[tmp] % 2) ? (sfblk[i] >> 1) : 0);
		if( (tmp != 1) && (sfblk[i] != 0x20000) )
			LQTable[i] += 16384;

		tmp = ( (int)((SINT8)gDefaultChrTbl[i]) * Qfactor + 32 ) >> 6;
		tmp = (tmp == 0) ? 1 : tmp;
		CQTable[i] = sfblk[i] * (invTable[tmp] >> 1) + ((invTable[tmp] % 2) ? (sfblk[i] >> 1) : 0);
		if( (tmp != 1) && (sfblk[i] != 0x20000) )
			CQTable[i] += 16384;
	}
}

//=========================
//
//		print jpeg info
//
//=========================
void CJpegBase::PrintJpegIndex(PTJpegIndex pinfo, char *filename)
{
	CFileBin fb;
	int i, j;

	if(pinfo->YUVType == JPEG_UNKNOWN)
		return;
	fb.Open(filename, "wt");
	fb.Printf("Start of picture = 0x%x\n", pinfo->sop);
	fb.Printf("End of picture = 0x%x\n", pinfo->eop);
	fb.Printf("Start of data = 0x%x\n", pinfo->offset);
	fb.Printf("Jpeg type: %s\n", gJpegFmt[pinfo->YUVType]);
	fb.Printf("VWC = 0x%x\n", pinfo->vwc);
	fb.Printf("Block count = 0x%x\n", pinfo->blkcount);
	fb.Printf("Image size: %d X %d\n", pinfo->ImageSize.cx, pinfo->ImageSize.cy);
	fb.Printf("Jpeg size: %d X %d\n", pinfo->JpgSize.cx, pinfo->JpgSize.cy);
	fb.Printf("Huffuman table:\n");
	for(i = 0; i < pinfo->HTCount; i++)
	{
		fb.Printf("HT[%d] = {\n\t", i);
		for(j = 0; j < pinfo->HTLen[i]; j++)
		{
			fb.Printf("0x%x, ", pinfo->HT[i][j]);
			if( (j+1) % 8 == 0 )
				fb.Printf("\n\t");
		}
		fb.Printf("\n}\n");
	}
	fb.Printf("Quantization table:\n");
	for(i = 0; i < pinfo->QTCount; i++)
	{
		fb.Printf("QT[%d] = {\n\t", i);
		for(j = 0; j < 64; j++)
		{
			fb.Printf("0x%x, ", pinfo->QT[i][j]);
			if( (j+1) % 8 == 0 )
				fb.Printf("\n\t");
		}
		fb.Printf("\n}\n");
	}
	fb.Printf("Jpeg has %d Component = ", pinfo->CompCount);
	for(i = 0; i < pinfo->CompCount; i++)
		fb.Printf("0x%x, ", pinfo->Comp[i]);
}

void CJpegBase::PrintJpegTable(PTJpegTable pinfo, char *filename)
{
	CFileBin fb;
	int i;

	fb.Open(filename, "wt");
	fb.Printf("Luminate quantizer table = {\n\t");
	for(i = 0; i < 64; i++)
	{
		fb.Printf("0x%x, ", pinfo->LQTable[i]);
		if( ((i + 1) % 8) == 0 )
			fb.Printf("\n\t");
	}
	fb.Printf("};\n");
	fb.Printf("Chrominal quantizer table = {\n\t");
	for(i = 0; i < 64; i++)
	{
		fb.Printf("0x%x, ", pinfo->CQTable[i]);
		if( ((i + 1) % 8) == 0 )
			fb.Printf("\n\t");
	}
	fb.Printf("};\n");
	fb.Printf("Luminate huffuman DC table = {\n");
	for(i = 0; i < 255; i++)
	{
		if(pinfo->Ldc[i].length == 0)
			continue;
		fb.Printf("index = %d, length = %d, val = 0x%x\n", i, pinfo->Ldc[i].length, pinfo->Ldc[i].value);
	}
	fb.Printf("};\n");
	fb.Printf("Luminate huffuman AC table = {\n");
	for(i = 0; i < 255; i++)
	{
		if(pinfo->Lac[i].length == 0)
			continue;
		fb.Printf("index = %d, length = %d, val = 0x%x\n", i, pinfo->Lac[i].length, pinfo->Lac[i].value);
	}
	fb.Printf("};\n");
	fb.Printf("Chrominal huffuman DC table = {\n");
	for(i = 0; i < 255; i++)
	{
		if(pinfo->Cdc[i].length == 0)
			continue;
		fb.Printf("index = %d, length = %d, val = 0x%x\n", i, pinfo->Cdc[i].length, pinfo->Cdc[i].value);
	}
	fb.Printf("};\n");
	fb.Printf("Chrominal huffuman AC table = {\n");
	for(i = 0; i < 255; i++)
	{
		if(pinfo->Cac[i].length == 0)
			continue;
		fb.Printf("index = %d, length = %d, val = 0x%x\n", i, pinfo->Cac[i].length, pinfo->Cac[i].value);
	}
	fb.Printf("};\n");
}

