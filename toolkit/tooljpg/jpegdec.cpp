

#include "internal.h"
//===================================================
//
//		CJpegDecoder class
//
//===================================================

CJpegDecoder::CJpegDecoder(int size) : CJpegDecBase(size)
{
	this->m_JpgIndex.YUVType = JPEG_UNKNOWN;
}

void CJpegDecoder::Open(PTJpegIndex pinfo)
{
	int i;

	CJpegDecBase::Open(pinfo);
	for(i = 0; i < 64; i++)
	{
		this->m_Table.LQTable[i] = gDctMatrix[i];
		this->m_Table.CQTable[i] = gDctMatrix[i];
		if(pinfo->QT[0][gZigZag[i]])
			this->m_Table.LQTable[i] *= (int)pinfo->QT[0][gZigZag[i]] & 0xff;
		if(pinfo->QT[1][gZigZag[i]])
			this->m_Table.CQTable[i] *= (int)pinfo->QT[1][gZigZag[i]] & 0xff;
	}
	this->m_Table.sftL[0] = this->getShiftValue(this->m_Table.LQTable);
	this->m_Table.sftC[0] = this->getShiftValue(this->m_Table.CQTable);
	this->generateBrc(this->m_Table.LQTable, this->m_Table.sftL[0], this->m_Table.brcL);
	this->generateBrc(this->m_Table.CQTable, this->m_Table.sftC[0], this->m_Table.brcC);
}

void CJpegDecoder::generateBrc(int *QTable, int sftC, int *output)
{
	int  i;

	for(i = 0; i < 64; i++) 
		output[i] = QTable[i] >> (sftC + 2);
}

int CJpegDecoder::getShiftValue(int *QTable)
{
	int  i;
	int max = 0;

	for(i = 0; i < 64; i++)
		max = (QTable[i] > max) ? QTable[i] : max;

	for (i = 24; i >= 16 ; i--)
		if (max & (1L << i))
			break;

	return i - 17;
}

void CJpegDecoder::blockdec(int *curdc, int mode)
{
	int i, inblk[64], oublk[64];
	char block[64];

	if(mode == JPEG_YBLOCK)
		inblk[0] = this->GetHuffDcVal(this->m_Table.Ldc, 10, 12);
	else
		inblk[0] = this->GetHuffDcVal(this->m_Table.Cdc, 10, 12);
	inblk[0] += *curdc;
	*curdc = inblk[0];
	if(mode == JPEG_YBLOCK)
		this->GetHuffAcVal(this->m_Table.Lac, inblk);
	else
		this->GetHuffAcVal(this->m_Table.Cac, inblk);
	this->izigzag(inblk, oublk);
	if(mode == JPEG_YBLOCK)
		this->dequant(oublk, inblk, this->m_Table.brcL, this->m_Table.sftL[0]);
	else
		this->dequant(oublk, inblk, this->m_Table.brcC, this->m_Table.sftC[0]);
	this->fpidct8x8(inblk, oublk);
	for(i = 0; i < 64; i++)
	{
		oublk[i] = oublk[i] + 0x80;
		oublk[i] = (oublk[i] < 0) ? 0 : ((oublk[i] > 255) ? -1 : oublk[i]);
		block[i] = (char)oublk[i];
	}
	CFifoBuf::Write(block, 64);
}

//===================================================
//
//		CJpegDecoder1 class
//
//===================================================


CJpegDecoder1::CJpegDecoder1(int size) : CJpegDecBase(size)
{
	this->m_JpgIndex.YUVType = JPEG_UNKNOWN;
}

void CJpegDecoder1::Open(PTJpegIndex pinfo)
{
	int i;

	CJpegDecBase::Open(pinfo);

	for(i = 0; i < 64; i++)
	{
		this->m_Lqt[i] = (double)((int)pinfo->QT[0][gZigZag[i]] & 0xff);
		this->m_Cqt[i] = (double)((int)pinfo->QT[1][gZigZag[i]] & 0xff);
	}
}

void CJpegDecoder1::blockdec(int *curdc, int mode)
{
	int inblk[64];
	double oublk[64];
	char block[64];

	if(mode == JPEG_YBLOCK)
		inblk[0] = this->GetHuffDcVal(this->m_Table.Ldc, 10, 12);
	else
		inblk[0] = this->GetHuffDcVal(this->m_Table.Cdc, 10, 12);
	inblk[0] += *curdc;
	*curdc = inblk[0];
	if(mode == JPEG_YBLOCK)
		this->GetHuffAcVal(this->m_Table.Lac, inblk);
	else
		this->GetHuffAcVal(this->m_Table.Cac, inblk);
	this->izigzag(inblk, oublk);
	if(mode == JPEG_YBLOCK)
		this->idctdequant(oublk, this->m_Lqt, block);
	else
		this->idctdequant(oublk, this->m_Cqt, block);
	CFifoBuf::Write(block, 64);
}

//===================================================
//
//		CJpegDecoder2 class
//
//===================================================

CJpegDecoder2::CJpegDecoder2(int size) : CJpegDecBase(size)
{
	this->m_JpgIndex.YUVType = JPEG_UNKNOWN;
}

void CJpegDecoder2::Open(PTJpegIndex pinfo)
{
	int i;

	CJpegDecBase::Open(pinfo);

	for(i = 0; i < 64; i++)
	{
		this->m_LQtbl[i] = ((int)pinfo->QT[0][gZigZag[i]] & 0xff);
		this->m_CQtbl[i] = ((int)pinfo->QT[1][gZigZag[i]] & 0xff);
	}
}


void CJpegDecoder2::blockdec(int *curdc, int mode)
{
	int i, inblk[64], oublk[64];
	char block[64];

	if(mode == JPEG_YBLOCK)
		inblk[0] = this->GetHuffDcVal(this->m_Table.Ldc, 10, 12);
	else
		inblk[0] = this->GetHuffDcVal(this->m_Table.Cdc, 10, 12);
	inblk[0] += *curdc;
	*curdc = inblk[0];

	if(mode == JPEG_YBLOCK)
		this->GetHuffAcVal(this->m_Table.Lac, inblk);
	else
		this->GetHuffAcVal(this->m_Table.Cac, inblk);

	this->izigzag(inblk, oublk);

	if(mode == JPEG_YBLOCK)
		this->fiquant(oublk, inblk, this->m_LQtbl);
	else
		this->fiquant(oublk, inblk, this->m_CQtbl);

	this->idct8x8(inblk);
	for(i = 0; i < 64; i++)
	{
		oublk[i] = inblk[i] + 0x80;
		oublk[i] = (oublk[i] < 0) ? 0 : ((oublk[i] > 255) ? -1 : oublk[i]);
		block[i] = (char)oublk[i];
	}

	CFifoBuf::Write(block, 64);
}