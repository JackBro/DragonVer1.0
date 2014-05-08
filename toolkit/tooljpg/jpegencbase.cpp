

#include "internal.h"

CJpegEncBase::CJpegEncBase(int size) : CBitFifo(size, BIGBIT_ORDER)
{
	this->Init();
}

void CJpegEncBase::Init(void)
{
	CBitTools::SetChk(0xff);
	CBitTools::SetFill(0);
	this->m_yuv = YUV422BLK;
	this->m_ImgSize.cx = 640;
	this->m_ImgSize.cy = 480;
	memset(&(this->m_Table), 0, sizeof(TJpegTable));
	CJpegBase::createHuffmanTable( (UINT8 *)gDefaultHuffmanTbl+1, 
		(UINT8 *)gDefaultHuffmanTbl+17, this->m_Table.Ldc );
	CJpegBase::createHuffmanTable( (UINT8 *)gDefaultHuffmanTbl+30, 
		(UINT8 *)gDefaultHuffmanTbl+46, this->m_Table.Cdc );
	CJpegBase::createHuffmanTable( (UINT8 *)gDefaultHuffmanTbl+59, 
		(UINT8 *)gDefaultHuffmanTbl+75, this->m_Table.Lac );
	CJpegBase::createHuffmanTable( (UINT8 *)gDefaultHuffmanTbl+238, 
		(UINT8 *)gDefaultHuffmanTbl+254, this->m_Table.Cac );
}

void CJpegEncBase::writeJpgHeader(void)
{
	int val = 0;
	const UINT8 jpghd[] = {0xff, 0xd8, 0xff, 0xfe, 0x0, 0x24, 0x0, 0x0, 0x0, 0x0};
	const char blk422[] = {0x32, 0x12, 0xb, 0x51, 0x4, 0x51, 0x4, 0, 0};
	const char blk420[] = {0x32, 0x22, 0x13, 0x51, 0x4, 0x51, 0x4, 0, 0};
	const char blk411[] = {0x32, 0x14, 0x13, 0x51, 0x4, 0x51, 0x4, 0, 0};
	const char blk444[] = {0x32, 0x11, 0x07, 0x51, 0x4, 0x51, 0x4, 0, 0};
	const char blk400[] = {0x11, 0x11, 0x07, 0x0, 0x0, 0x0, 0x0, 0, 0};

	CBitFifo::Write((char *)jpghd, sizeof(jpghd) << 3);
	CBitFifo::Write((char *)gJpegDefaultImgpara, gJpegImgparaSize << 3);
//	for(i = 0; i < 15; i++)
//		CBitFifo::Write((char *)&val, 1 << 3);
	CBitFifo::Write((char *)&(this->m_ImgSize.cy), 2 << 3);
	CBitFifo::Write((char *)&(this->m_ImgSize.cx), 2 << 3);
	CBitFifo::Write((char *)&(this->m_qf), 1 << 3);
	CBitFifo::Write((char *)&val, 1 << 3);

	switch(this->m_yuv)
	{
	case YUV422BLK:
		CBitFifo::Write((char *)blk422, sizeof(blk422) << 3);
		break;
	case YUV420BLK:
		CBitFifo::Write((char *)blk420, sizeof(blk420) << 3);
		break;
	case YUV411BLK:
		CBitFifo::Write((char *)blk411, sizeof(blk411) << 3);
		break;
	case YUV400BLK:
		CBitFifo::Write((char *)blk400, sizeof(blk400) << 3);
		break;
	case YUV444BLK:
		CBitFifo::Write((char *)blk444, sizeof(blk444) << 3);
		break;
	default:
		return;
	}
}

void CJpegEncBase::writeDqtHeader(void)
{
	int i, val = 0;
	const UINT8 dqthd[] = {0xff, 0xdb, 0, 0x84, 0};
	const UINT8 blk400[] = {0xFF, 0xFE, 0x0, 0x3, 0x0, 0xff, 0xc0, 0x0, 0xb, 0x8};
	const UINT8 nblk400[] = {0xff, 0xfe, 0x0, 0x5, 0x0, 0x0, 0x0, 0xff, 0xc0, 0x0, 0x11, 0x8};
	const UINT8 sof400[] = {0x1, 0x1, 0x11, 0x0};

	CBitFifo::Write((char *)dqthd, sizeof(dqthd) << 3);
	for (i = 0; i < 64; i++) 
	{ 
		val = (int)gJpegDefaultLumTbl[i] & 0xff;
		val *= this->m_qf;
		val += 32;
		val >>= 6;
		val = (val == 0) ? 1 : (val > 255 ? 255 : val);
		CBitFifo::Write((char *)&val, 1 << 3);
	}
	val = 1;
	CBitFifo::Write((char *)&val, 1 << 3);
	for (i = 0; i < 64; i++) 
	{
		val = (int)gJpegDefaultChrTbl[i] & 0xff;
		val *= this->m_qf;
		val += 32;
		val >>= 6;
		val = (val == 0) ? 1 : (val > 255 ? 255 : val);
		CBitFifo::Write((char *)&val, 1 << 3);
	}
	if(this->m_yuv == YUV400BLK)	//gray image
		CBitFifo::Write((char *)blk400, sizeof(blk400) << 3);
	else
		CBitFifo::Write((char *)nblk400, sizeof(nblk400) << 3);
	CBitFifo::Write(((char *)&(this->m_ImgSize.cy))+1, 1 << 3);
	CBitFifo::Write((char *)&(this->m_ImgSize.cy), 1 << 3);
	CBitFifo::Write(((char *)&(this->m_ImgSize.cx))+1, 1 << 3);
	CBitFifo::Write((char *)&(this->m_ImgSize.cx), 1 << 3);

	if(this->m_yuv == YUV400BLK)	//gray image
		CBitFifo::Write((char *)sof400, sizeof(sof400) << 3);
	else 
	{
		val = 3;
		CBitFifo::Write((char *)&val, 1 << 3);
		for(i = 1; i < 4; i++)	
		{
			CBitFifo::Write((char *)&i, 1 << 3);
			if(i == 1)	
			{
				switch(this->m_yuv)
				{
				case YUV411BLK:
					val = 0x41;
					break;
				case YUV422BLK:
					val = 0x21;
					break;
				case YUV420BLK:
					val = 0x22;
					break;
				case YUV444BLK:
					val = 0x11;
					break;
				default:
					break;
				}
				/* horizontal/vertical sampling factor */
				CBitFifo::Write((char *)&val, 1 << 3);
				val = 0;
				CBitFifo::Write((char *)&val, 1 << 3);
			}
			else 
			{
				val = 0x111;
				CBitFifo::Write((char *)&val, 2 << 3);
			}
		}
	}
}

void CJpegEncBase::writeDhtHeader(void)
{
	int i, val;
	const UINT8 dhthd[] = {0xff, 0xc4, 0x1, 0xa2};
	const UINT8 blk400[] = {0xff, 0xfe, 0x0, 0x4, 0x0, 0x0, 0xff, 0xda, 0x0, 0x8, 0x1, 0x1, 0x0};
	const UINT8 nblk400[] = {0xff, 0xfe, 0x0, 0x4, 0x0, 0x0, 0xff, 0xda, 0x0, 0xc, 0x3};
	const UINT8 tail[] = {0x0, 0x3f, 0x0};

	CBitFifo::Write((char *)dhthd, sizeof(dhthd) << 3);
	CBitFifo::Write((char *)gDefaultHuffmanTbl, gDefHuffTblSize << 3);

	if(this->m_yuv == YUV400BLK)
		CBitFifo::Write((char *)blk400, sizeof(blk400) << 3);
	else
	{
		CBitFifo::Write((char *)nblk400, sizeof(nblk400) << 3);
		for (i = 1; i < 4; i++) 
		{
			CBitFifo::Write((char *)&i, 1 << 3);
			val = 0x11;
			if(i == 1)
				val = 0;
			CBitFifo::Write((char *)&val, 1 << 3);
		}
	}
	CBitFifo::Write((char *)tail, sizeof(tail) << 3);
}

void CJpegEncBase::generateJpegHeader(void)
{
	this->writeJpgHeader();
	this->writeDqtHeader();
	this->writeDhtHeader();
}

int	CJpegEncBase::getVwc(void)
{
	int vwc = 0, cx, cy;

	switch(this->m_yuv)
	{
	case YUV422BLK:
		cx = ((this->m_ImgSize.cx + 15) >> 4) << 4;
		cy = ((this->m_ImgSize.cy + 7) >> 3) << 3;
		vwc = (cx * cy) << 1;
		break;
	case YUV420BLK:
		cx = ((this->m_ImgSize.cx + 15) >> 4) << 4;
		cy = ((this->m_ImgSize.cy + 15) >> 4) << 4;
		vwc = (cx * cy * 3) >> 1;
		break;
	case YUV411BLK:
		cx = ((this->m_ImgSize.cx + 31) >> 5) << 5;
		cy = ((this->m_ImgSize.cy + 7) >> 3) << 3;
		vwc = (cx * cy * 3) >> 1;
		break;
	case YUV400BLK:
		cx = ((this->m_ImgSize.cx + 7) >> 3) << 3;
		cy = ((this->m_ImgSize.cy + 7) >> 3) << 3;
		vwc = cx * cy;
		break;
	case YUV444BLK:
		cx = ((this->m_ImgSize.cx + 7) >> 3) << 3;
		cy = ((this->m_ImgSize.cy + 7) >> 3) << 3;
		vwc = cx * cy * 3;
		break;
	default:
		break;
	}
	return vwc;
}

void CJpegEncBase::eofFlush(void)
{
	int pos, last, rest, val = 0xd9ffffff;

	CBitTools::SetChkEn(0);
	pos = this->m_wpoint;
	last = pos >> 3;
	rest = pos % 8;

	CBitFifo::Write(val, 8 - rest);
	switch(last % 4)
	{
	case 0:
		if(rest == 0)
			CBitFifo::Write((char *)&val, 4 << 3);
		else
			CBitFifo::Write((char *)&val + 1, 3 << 3);
		break;
	case 1:
		if(rest == 0)
			CBitFifo::Write((char *)&val + 1, 3 << 3);
		else
			CBitFifo::Write((char *)&val + 2, 2 << 3);
		break;
	case 2:
		if(rest == 0)
			CBitFifo::Write((char *)&val + 2, 2 << 3);
		else
		{
			CBitFifo::Write((char *)&val, 1 << 3);
			CBitFifo::Write((char *)&val, 4 << 3);
		}
		break;
	case 3:
		if(rest == 0)
			CBitFifo::Write((char *)&val, 1 << 3);
		CBitFifo::Write((char *)&val, 4 << 3);
		break;
	default:
		break;
	}
}

void CJpegEncBase::encodeDC(int DCvalue, int *lastDC, THuffValue *pEncTable)
{
	int diff, absdiff;
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
		this->m_dbug.PrintMsg("error in huffman encode!\n");
		return;
	}
	CBitFifo::Write(pEncTable[i].value, pEncTable[i].length);
	diff -= (diff < 0) ? 1 : 0;
	CBitFifo::Write(diff, i);
}

void CJpegEncBase::encodeAC(int *zzblk, THuffValue *pEncTable)
{
	int i, j, run = 0;
	int val;

	for(i = 1; i < 64; i++)
	{
		if(zzblk[i] == 0)
		{
			run ++;
			continue;
		}
		while(run > 15)
		{
			CBitFifo::Write(pEncTable[240].value, pEncTable[240].length);
			run -= 16;
		}
		val = abs(zzblk[i]);
		for(j = 1; j < 12; j++)
		{
			if( val < (1 << j) )
				break;
		}
		CBitFifo::Write(pEncTable[j + (run<<4)].value, pEncTable[j + (run<<4)].length);
		run = 0;
		val = (UINT16)(zzblk[i] - ((zzblk[i] < 0) ? 1 : 0));
		CBitFifo::Write(val, j);
	}
	if(zzblk[63] == 0)
		CBitFifo::Write(pEncTable[0].value, pEncTable[0].length);
}

//=============== public ===================

void CJpegEncBase::SetJpegSize(TSize size)
{
	this->m_ImgSize = size;
}

void CJpegEncBase::SetJpegSize(int width, int height)
{
	this->m_ImgSize.cx = (UINT16)width;
	this->m_ImgSize.cy = (UINT16)height;
}

void CJpegEncBase::SetYuvFmt(int fmt)
{
	this->m_yuv = fmt;
}

void CJpegEncBase::Open(void)
{
	CBitTools::SetChkEn(0);
	this->m_pos = 0;
	this->m_lastUdc = 0;
	this->m_lastVdc = 0;
	this->m_lastYdc = 0;
	this->m_vwc = this->getVwc();
	CBitFifo::ClearPoint();
}

int CJpegEncBase::Write(char *buf, int len)
{
	int i;

	if(this->m_pos > this->m_vwc)
		return 0;

	if(this->m_pos == 0)
		this->generateJpegHeader();
	CBitTools::SetChkEn(1);
	switch(this->m_yuv)
	{
	case YUV422BLK:
		for(i = 0; (i < len) && (this->m_pos < this->m_vwc); i += 256, this->m_pos += 256)
		{
			this->encblock(buf+i, &this->m_lastYdc, JPEG_YBLOCK);
			this->encblock(buf+i+64, &this->m_lastYdc, JPEG_YBLOCK);
			this->encblock(buf+i+128, &this->m_lastUdc, JPEG_UVBLOCK);
			this->encblock(buf+i+192, &this->m_lastVdc, JPEG_UVBLOCK);
		}
		break;
	case YUV420BLK:
	case YUV411BLK:
		for(i = 0; (i < len) && (this->m_pos < this->m_vwc); i += 384, this->m_pos += 384)
		{
			this->encblock(buf+i, &this->m_lastYdc, JPEG_YBLOCK);
			this->encblock(buf+i+64, &this->m_lastYdc, JPEG_YBLOCK);
			this->encblock(buf+i+128, &this->m_lastYdc, JPEG_YBLOCK);
			this->encblock(buf+i+192, &this->m_lastYdc, JPEG_YBLOCK);
			this->encblock(buf+i+256, &this->m_lastUdc, JPEG_UVBLOCK);
			this->encblock(buf+i+320, &this->m_lastVdc, JPEG_UVBLOCK);
		}
		break;
	case YUV400BLK:
		for(i = 0; (i < len) && (this->m_pos < this->m_vwc); i += 64, this->m_pos += 64)
			this->encblock(buf+i, &this->m_lastYdc, JPEG_YBLOCK);
		break;
	case YUV444BLK:
		for(i = 0; (i < len) && (this->m_pos < this->m_vwc); i += 192, this->m_pos += 192)
		{
			this->encblock(buf+i, &this->m_lastYdc, JPEG_YBLOCK);
			this->encblock(buf+i+64, &this->m_lastUdc, JPEG_UVBLOCK);
			this->encblock(buf+i+128, &this->m_lastVdc, JPEG_UVBLOCK);
		}
		break;
	default:
		break;
	}
	this->m_pos += len;
	if(this->m_pos >= this->m_vwc)
		this->eofFlush();
	return len;
}

int CJpegEncBase::WriteBrc(char *buf, int len)
{
	int i;
	int thd_dwc = 0, judge_pos = 0, start_rude = 0;

	if(this->m_pos > this->m_vwc)
		return 0;

	if(this->m_pos == 0)
		this->generateJpegHeader();
	CBitTools::SetChkEn(1);

	this->m_rudeapply = 0;
	if (this->m_rudeoption.rude_en)
	{
		thd_dwc = this->m_rudeoption.rude_thd;
		switch(this->m_rudeoption.rude_level) 
		{
		case RUDEBRC_ZERO:
			this->m_rudeapply = 1;
			break;
		case RUDEBRC_QUAR:
			judge_pos = this->m_vwc >> 2;			
			break;
		case RUDEBRC_HALF:
			judge_pos = this->m_vwc >> 1;	
			break;
		case RUDEBRC_3QUARS:
			judge_pos = (this->m_vwc * 3) >> 2;
			break;
		default:
			break;
		}
	}
	switch(this->m_yuv)
	{
	case YUV422BLK:
		for(i = 0; (i < len) && (this->m_pos < this->m_vwc); i += 256, this->m_pos += 256)
		{
			if ((this->m_rudeoption.rude_en) && (!start_rude) && (!this->m_rudeapply))
			{
				if (this->m_pos == judge_pos)
				{
					if( (CBitFifo::GetWritePoint() >> 5) > thd_dwc )
						this->m_rudeapply = 1;
					start_rude = 1;
				}
			}

			this->encblock(buf+i, &this->m_lastYdc, JPEG_YBLOCK);
			this->encblock(buf+i+64, &this->m_lastYdc, JPEG_YBLOCK);
			this->encblock(buf+i+128, &this->m_lastUdc, JPEG_UVBLOCK);
			this->encblock(buf+i+192, &this->m_lastVdc, JPEG_UVBLOCK);
		}
		break;
	case YUV420BLK:
	case YUV411BLK:
		for(i = 0; (i < len) && (this->m_pos < this->m_vwc); i += 384, this->m_pos += 384)
		{
			this->encblock(buf+i, &this->m_lastYdc, JPEG_YBLOCK);
			this->encblock(buf+i+64, &this->m_lastYdc, JPEG_YBLOCK);
			this->encblock(buf+i+128, &this->m_lastYdc, JPEG_YBLOCK);
			this->encblock(buf+i+192, &this->m_lastYdc, JPEG_YBLOCK);
			this->encblock(buf+i+256, &this->m_lastUdc, JPEG_UVBLOCK);
			this->encblock(buf+i+320, &this->m_lastVdc, JPEG_UVBLOCK);
		}
		break;
	case YUV400BLK:
		for(i = 0; (i < len) && (this->m_pos < this->m_vwc); i += 64, this->m_pos += 64)
			this->encblock(buf+i, &this->m_lastYdc, JPEG_YBLOCK);
		break;
	case YUV444BLK:
		for(i = 0; (i < len) && (this->m_pos < this->m_vwc); i += 192, this->m_pos += 192)
		{
			this->encblock(buf+i, &this->m_lastYdc, JPEG_YBLOCK);
			this->encblock(buf+i+64, &this->m_lastUdc, JPEG_UVBLOCK);
			this->encblock(buf+i+128, &this->m_lastVdc, JPEG_UVBLOCK);
		}
		break;
	default:
		break;
	}
	this->m_pos += len;
	if(this->m_pos >= this->m_vwc)
		this->eofFlush();
	return len;
}

void CJpegEncBase::rudeAC(int *blk, int num)
{
	int i;
	for (i = num + 1; i < 64; i++)
		blk[i] = 0;
}

int CJpegEncBase::IsApplyRude(void)
{
	return this->m_rudeapply;
}

int	CJpegEncBase::Read(char *buf, int size)
{
	size = CBitFifo::Read(buf, size << 3);
	return (size + 7) >> 3;
}

int CJpegEncBase::calcQ(int bytesize)
{
	int i, vwc, hwc, tcr, b;
	int *p;
	char amsb, res, val = 1;
	char aptr[8], bptr[8], tptr[8], tmp[8];

	vwc = this->m_vwc >> 2;
	hwc = (bytesize >> 2) << 3;
	//tcr = (this->m_vwc << 2) / TARGET_BUFSIZE + 1;
	//tcr = this->m_rudeoption.tcr;
	tcr = this->m_tcr;
	b = ABRC_B;
	for (i = 0; i < 8; i++)	
		aptr[i] = bptr[i] = tptr[i] = tmp[i] = 0;	

	p =(int *)&aptr;							//put A in aptr[8]
	*p = hwc;
	p = (int *)&bptr;							//put B in bptr[8]
	*p = vwc;

	this->Sub(tptr, bptr, tmp, 8, 8, 8);		//P = P - B,  P is put in tptr[8]
	memcpy(tptr, tmp, 8);
	memset(tmp, 0, 8);

	for(i = 0; i <= 40; i++)
	{
		aptr[4] &= 0x07;
		aptr[5] = aptr[6] = aptr[7] = 0;
		amsb = (aptr[4] & 0x04) >> 2;					//get the 35 bit of A
		
		if (tptr[7] < 0)
		{
			this->Add(tptr, tptr, tmp, 8, 8, 8);		//P = (P << 1) + amsb
			this->Add(tmp, &amsb, tptr, 8, 1, 8);

			this->Add(aptr, aptr, tmp, 8, 8, 8);		// A = A << 1;
			memcpy(aptr, tmp, 8);

			this->Add(tptr, bptr, tmp, 8, 8, 8);		//P = P + B
			memcpy(tptr, tmp, 8);
		}
		else
		{
			this->Add(tptr, tptr, tmp, 8, 8, 8);		//P = (P << 1) + amsb
			this->Add(tmp, &amsb, tptr, 8, 1, 8);

			this->Add(aptr, aptr, tmp, 8, 8, 8);		//A = (A << 1) + 1
			this->Add(tmp, &val, aptr, 8, 1, 8);

			this->Sub(tptr, bptr, tmp, 8, 8, 8);		//P = P - B
			memcpy(tptr, tmp, 8);
		}
	}

	this->Mul(aptr, (char *)&tcr, tmp, 8, 4, 8);		//A = A * tcr
	memcpy(aptr, tmp, 8);

	this->m_qf = (this->m_qf << 6) + b;
	val = 4;
	this->Div(aptr, &val, tmp, &res, 8, 1, 8, 1);		//A = (A >> 2) * Q
	this->Mul(tmp, (char *)&(this->m_qf), aptr, 8, 4, 8);
	
	this->Mul(aptr, &val, tmp, 8, 1, 8);				//A = (A << 2) - (b << 10)
	b <<= 10;
	this->Sub(tmp, (char *)&b, aptr, 8, 4, 8);

	i = 1 << 15;
	this->Add(aptr, (char *)&i, tmp, 8, 4, 8);				// A = A + (1 << 15)
	i <<= 1;
	this->Div(tmp, (char *)&i, aptr, tptr, 8, 4, 8, 4);		//A = A >> 16

	i = (aptr[0] & 0xff) + ((aptr[1] & 0xff) << 8) + ((aptr[2] & 0xff) << 16) + ((aptr[3] & 0xff) << 24);
	i = (aptr[7] < 0) ? 0 : ((i > 127) ? 127 : i);

	return i;
}

int CJpegEncBase::RudeBrcQ(int bytesize)
{
	int qcomp;	
	int curq, qdiff0, qdiff1;

	curq = this->m_qf;	
	
	if (this->m_rudeapply)
	{		
		if (this->m_qf >= 64)
			qcomp = this->m_rudeoption.qfcomp5;
		else if (this->m_qf >= 32)
			qcomp = this->m_rudeoption.qfcomp4;
		else if (this->m_qf >= 24)
			qcomp = this->m_rudeoption.qfcomp3;
		else if (this->m_qf >= 16)
			qcomp = this->m_rudeoption.qfcomp2;
		else if (this->m_qf >= 8)
			qcomp = this->m_rudeoption.qfcomp1;
		else
			qcomp = this->m_rudeoption.qfcomp0;
		this->m_qf += qcomp;
	}

	this->m_qf = this->calcQ(bytesize);

	qdiff0 = this->m_qf - curq;
	qdiff1 = curq - this->m_qf;
	if (qdiff0 >= this->m_rudeoption.thdr2)
		this->m_qf = curq + ((qdiff0 * this->m_rudeoption.shiftr2) >> 6);
	else if (qdiff0 >= this->m_rudeoption.thdr1)
		this->m_qf = curq + ((qdiff0 * this->m_rudeoption.shiftr1) >> 6);
	else if (qdiff1 >= this->m_rudeoption.thdl2)
		this->m_qf = curq + ((qdiff1 * this->m_rudeoption.shiftl2) >> 6);
	else if (qdiff1 >= this->m_rudeoption.thdl1)
		this->m_qf = curq + ((qdiff1 * this->m_rudeoption.shiftl1) >> 6);

	this->m_qf = this->m_qf > 127 ? 127 : this->m_qf;
	return this->m_qf;
}
