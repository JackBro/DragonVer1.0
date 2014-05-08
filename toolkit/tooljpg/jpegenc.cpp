
#include "internal.h"
//===================================================
//
//		CJpegEncoder class
//
//===================================================

CJpegEncoder::CJpegEncoder(int size) : CJpegEncBase(size)
{
	this->InitJpegTable(&(this->m_Table));
	this->SetJpegQf(0x78);
	this->m_fb.Open("dctinfo.txt", "wt");
	this->m_fb1.Open("qinfo.txt", "wt");
}

void CJpegEncoder::InitJpegTable(TJpegTable *table)
{
	int i;

//	this->DCTscaleFactor(table->sfblk);
//	this->getInvTable(table->invTable);
//	table->sfblk = (int *)gDctMatrix;
//	table->invTable = (int *)gInvTable;
	for(i = 0; i < 128; i++)
	{
		this->getQTable((int *)gDctMatrix, table->LQTable, table->CQTable, (int *)gInvTable, i);
		table->sftL[i] = this->getShiftValue(table->LQTable);
		table->sftC[i] = this->getShiftValue(table->CQTable);
	}
}

int CJpegEncoder::getShiftValue(int *QTable)
{
	int  i;
	int max = 0;

	for(i = 0; i < 64; i++)
		max = (QTable[i] > max) ? QTable[i] : max;

	for (i = 31; i >= 23 ; i--)
		if (max & (1L << i))
			break;

	return 31-i;
}

void CJpegEncoder::generateBrc(int *QTable, int sftC, int *output)
{
	int  i;

	for(i = 0; i < 64; i++) 
	{
		output[i] = QTable[i] >> (16 - sftC);
		if(sftC > 2)
			output[i] = (QTable[i] >> 14) << (sftC-2);
	}
}

void CJpegEncoder::updateBrcTable(TJpegTable *table, int qf)
{
	/* Recalculate BRC output in the beginning of every field */
	this->getQTable((int *)gDctMatrix, table->LQTable, table->CQTable, (int *)gInvTable, qf);
	this->generateBrc(table->LQTable, table->sftL[qf], table->brcL);
	this->generateBrc(table->CQTable, table->sftC[qf], table->brcC);
}

void CJpegEncoder::encblock(char *block, int *lastDc, int mode)
{
	int inblk[64], oublk[64];
	int i, j;

	for(i = 0; i < 64; i++)
		inblk[i] = ((int)block[i] & 0xff) - 0x80;
	this->fpdct8x8(inblk, oublk);
	
	for (i = 0; i < 8; i++)
		for (j = 0; j < 8; j++)
			this->m_fb.Printf("%05x\n", oublk[i + ( j << 3)] & 0x1ffff);

	if(mode == JPEG_YBLOCK)
		this->quant(oublk, inblk, this->m_Table.brcL, this->m_Table.sftL[this->m_qf]);
	else
		this->quant(oublk, inblk, this->m_Table.brcC, this->m_Table.sftC[this->m_qf]);

	for (i = 0; i < 8; i++)
		for (j = 0; j < 8; j++)
			this->m_fb1.Printf("%03x\n", inblk[i + ( j << 3)] & 0x7ff);

	this->zigzag(inblk, oublk);	

	if(mode == JPEG_YBLOCK)
	{
		if	((this->m_rudeoption.rude_en) && this->m_rudeapply && (!this->m_rudeoption.only_uv))
			this->rudeAC(oublk, this->m_rudeoption.rude_ac);

		this->encodeDC(oublk[0], lastDc, this->m_Table.Ldc);
		this->encodeAC(oublk, this->m_Table.Lac);
	}
	else
	{
		if ((this->m_rudeoption.rude_en) && this->m_rudeapply)
			this->rudeAC(oublk, this->m_rudeoption.rude_ac);

		this->encodeDC(oublk[0], lastDc, this->m_Table.Cdc);
		this->encodeAC(oublk, this->m_Table.Cac);
	}
}

//=============== public ===================

void CJpegEncoder::SetJpegQf(int qf, int tcr, PTRudebrcOption pinfo)
{
	this->updateBrcTable(&(this->m_Table), qf);
	this->m_qf = qf;
	this->m_tcr = tcr;

	memset(&this->m_rudeoption, 0, sizeof(TRudebrcOption));		

	if (pinfo != NULL)
	{
		memcpy(&(this->m_rudeoption), pinfo, sizeof(TRudebrcOption));
	}
}

//===================================================
//
//		CJpegEncoder1 class
//
//===================================================

CJpegEncoder1::CJpegEncoder1(int size) : CJpegEncBase(size)
{
	this->SetJpegQf(0x78);
}

void CJpegEncoder1::encblock(char *block, int *lastDc, int mode)
{
	double inblk[64];
	int oublk[64];

	if(mode == JPEG_YBLOCK)
		this->dctquant(block, this->m_Lqt, inblk);
	else
		this->dctquant(block, this->m_Cqt, inblk);
	this->zigzag(inblk, oublk);	

	if(mode == JPEG_YBLOCK)
	{
		this->encodeDC(oublk[0], lastDc, this->m_Table.Ldc);
		this->encodeAC(oublk, this->m_Table.Lac);
	}
	else
	{
		this->encodeDC(oublk[0], lastDc, this->m_Table.Cdc);
		this->encodeAC(oublk, this->m_Table.Cac);
	}
}

//=============== public ===================

void CJpegEncoder1::SetJpegQf(int qf)
{
	int i;

	for (i = 0; i < 64; i++) 
	{ 
		this->m_Lqt[i] = (double)(gDefaultLumTbl[i] & 0xff);
		this->m_Lqt[i] *= qf;
		this->m_Lqt[i] /= 64;
		this->m_Lqt[i] = (this->m_Lqt[i] < 1) ? 1 : (this->m_Lqt[i] > 255 ? 255 : this->m_Lqt[i]);
	}
	for (i = 0; i < 64; i++) 
	{
		this->m_Cqt[i] = (double)(gDefaultChrTbl[i] & 0xff);
		this->m_Cqt[i] *= qf;
		this->m_Cqt[i] /= 64;
		this->m_Cqt[i] = (this->m_Cqt[i] < 1) ? 1 : (this->m_Cqt[i] > 255 ? 255 : this->m_Cqt[i]);
	}
	this->m_qf = qf;
}

//===================================================
//
//		CJpegEncoder2 class
//
//===================================================
CJpegEncoder2::CJpegEncoder2(int size) : CJpegEncBase(size)
{
	this->SetJpegQf(0x78);
}


void CJpegEncoder2::encblock(char *block, int *lastDc, int mode)
{
	int i, inblk[64], oublk[64];	
	
	for(i = 0; i < 64; i++)
		inblk[i] = ((int)block[i] & 0xff) - 0x80;

	this->dct8x8(inblk);

	if(mode == JPEG_YBLOCK)
		this->fquant(inblk, oublk, this->m_LQtbl);
	else
		this->fquant(inblk, oublk, this->m_CQtbl);

	this->zigzag(oublk, inblk);

	if(mode == JPEG_YBLOCK)
	{
		this->encodeDC(inblk[0], lastDc, this->m_Table.Ldc);
		this->encodeAC(inblk, this->m_Table.Lac);
	}
	else
	{
		this->encodeDC(inblk[0], lastDc, this->m_Table.Cdc);
		this->encodeAC(inblk, this->m_Table.Cac);
	}
}

//=============== public ===================

void CJpegEncoder2::SetJpegQf(int qf)
{
	int i, tmp;

	for (i = 0; i < 64; i++)
	{
		tmp = (gDefaultLumTbl[i] * qf + 32) >> 6;
		tmp = (tmp == 0) ? 1 : tmp;
		this->m_LQtbl[i] = gInvTable[tmp];
	}

	for (i = 0; i < 64; i++)
	{
		tmp = (gDefaultChrTbl[i] * qf + 32) >> 6;
		tmp = (tmp == 0) ? 1 : tmp;
		this->m_CQtbl[i] = gInvTable[tmp];
	}
	
	this->m_qf = qf;
}

