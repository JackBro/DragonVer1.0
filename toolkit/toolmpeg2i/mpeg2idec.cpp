

#include "internal.h"

#define SWAP(A, B)	{ int tmp = A; A = B; B = tmp; }

CMpeg2Decoder::CMpeg2Decoder(int gap)
{
	this->m_gapsize = gap << 3;
	this->m_cur = 0;
	this->m_ref = 1;
}

void CMpeg2Decoder::Open(void)
{
	this->ClearPoint();
	CMpeg2IBase::Open();
	memset(&(this->m_option), 0, sizeof(TMpeg2Option));
	this->m_horwidth = 0;
	this->m_Ydc = this->m_Udc = this->m_Vdc = 0;
	this->m_quant = 0xff;
}
/*
int CMpeg2Decoder::Write(char *buf, int len)
{
	return CJmBitFifo::Write(buf, len);
}
*/

int	CMpeg2Decoder::Read(char *buf, int size, int mode)
{
	int len = 0, val;	

	while( (size - len) >= this->m_horwidth)
	{
		if(mode == MPEG_DECNORMAL)
		{
			if(this->GetDataSize() < this->m_gapsize)
				return len;
		}
		if(this->NextStartCode() == 0)
			return len;
		val = CBitFifo::Read(8);		//get header marker
		if( (val >= MPEG_MINSLICE_CODE) && (val <= MPEG_MAXSLICE_CODE) )
		{
			
			if( (this->m_option.pictype == PIC_BI_PREDICT)  ||
				(this->m_option.frmfield != PICSTRUCT_FRAMEPIC) )
				continue;
			this->Slice(buf+len, val);
			len += this->m_horwidth;
			if (this->m_option.pictype == PIC_INTRA)
				len = 0;
			continue;		
			
		}
		switch(val)
		{
		case MPEGPIC_START_CODE:
			this->ParsePicHdr(&(this->m_option));			
			SWAP(this->m_cur, this->m_ref);				//exchange cur and ref
			break;
		case MPEG_USERDATA_CODE:
			this->ParseUserData(&(this->m_option));
			break;
		case MPEG_SEQHEADER_CODE:
			this->ParseSeqHdr(&(this->m_option));
			//this->CheckOption();
			break;
		case MPEG_SEQERROR_CODE:
			break;
		case MPEG_EXTSTART_CODE:
			this->ParseExtHdr(&(this->m_option));
			//this->CheckOption();
			break;
		case MPEG_SEQEND_CODE:
			return len;
		case MPEG_GROUPSTART_CODE:
			this->ParseGopHdr(&(this->m_option));
		default:
			break;
		}
	}	
	return len;
}


int	CMpeg2Decoder::GetHuffDcVal(PTHuffValue table, int bitlen, int tsize)
{
	int index, val, halfrange;

	index = this->GetHuffIndex(table, bitlen, tsize);
	if (index == 0)
		return 0;
	val = CBitFifo::Read(index);
	halfrange = 1 << (index - 1);
	if (val >= halfrange)
		return val;
	else
	{
		val = (val + 1) - (halfrange << 1);
		return val;
	}	
}

/*
int	CMpeg2IDecoder::GetIntraRunLevel(int val, int len, int *pos, int *blk)
{
	int i, j;

	for(i = 0; i < 2; i++)
	{
		for(j = 0; j < 40; j++)
		{
			if( (val == gIntraActbl1a[i][j].value) && (len == gIntraActbl1a[i][j].length) )
			{
				val = (i < (63 - (*pos))) ? i : (63 - (*pos));
				while(val--)
					blk[(*pos)++] = 0;
				len = j+1;
				val = CBitFifo::Read(1);
				if(val)
					len = -len;
				blk[(*pos)++] = len;
				return 1;
			}
		}
	}

	for(i = 0; i < 30; i++)
	{
		for(j = 0; j < 5; j++)
		{
			if( (val == gIntraActbl2a[i][j].value) && (len == gIntraActbl2a[i][j].length) )
			{
				val = ((i + 2) < (63 - (*pos))) ? (i + 2) : (63 - (*pos));
				while(val--)
					blk[(*pos)++] = 0;
				len = j+1;
				val = CBitFifo::Read(1);
				if(val)
					len = -len;
				blk[(*pos)++] = len;
				return 1;
			}
		}
	}
	return 0;
}

void CMpeg2IDecoder::GetHuffAcVal(int *blk)
{
	int val, i, pos = 1, flag;

	for(i = 1; i < 64; i++)
		blk[i] = 0;
	while(pos < 64)
	{
		val = 0;
		flag = 1;
		i = 0;
		while(flag)
		{
			val = CBitFifo::Read(val, 1);
			i++;
			if( (val == 6) && (i == 4) )	//end of block
				return;
			if( (val == 1) && (i == 6) )	//escape
			{
				val = CBitFifo::Read(6);
				if ( val > (63 - pos))
					return;
				while(val--)
					blk[pos++] = 0;
				val = CBitFifo::Read(12);
				if(val & (1 << 11))
					blk[pos++] = val;
				else
					blk[pos++] = val | 0xfffff000;
			}
			else if(this->GetIntraRunLevel(val, i, &pos, blk))
				flag = 0;
			else if (i >= 16)
			{
				while (CBitFifo::Query(4) != 6)
					CBitFifo::Read(1);
				return;
			}
		}
	}	
}
*/

int CMpeg2Decoder::GetRunLevel(int val, int len, int *pos, int *blk, int Intra)
{
	int i;
	int run, level;
	int *TblLen;
	THuffEvent *AcTbl;

	TblLen = Intra ? (int *)gIntraAcTblLen : (int *)gInterAcTblLen;
	AcTbl = Intra ? (THuffEvent *)gIntraActbl[len] : (THuffEvent *)gInterActbl[len];
	
	for (i = 0; i < TblLen[len]; i++)
	{
		if( val == AcTbl[i].value)
		{
			run = AcTbl[i].run;
			level = AcTbl[i].level;
			
			run = (run < (63 - (*pos))) ? run : (63 - (*pos));
			while (run--)
				blk[(*pos)++] = 0;
			if (CBitFifo::Read(1))
				level = -level;
			blk[(*pos)++] = level;
			return 1;
		}
	}
	return 0;
}

void CMpeg2Decoder::GetIntraAcVal(int *blk, int intravlc)
{
	int val, i, flag, pos = 1;

	for (i = 1; i < 64; i++)
		blk[i] = 0;

	while (pos < 64)
	{
		val = 0;
		flag = 1;
		i = 1;
		val = CBitFifo::Read(val, 1);
		while (flag)
		{
			val = CBitFifo::Read(val, 1);
			i++;
			if (this->GetRunLevel(val, i, &pos, blk, intravlc))
				flag = 0;
			else if ( (intravlc && (i == 4) && (val == 6)) || (!intravlc && (i == 2) && (val == 2)) )			//end of block
				return;
			else if	( (i == 6) && (val == 1) )	//escape
			{
				val = CBitFifo::Read(6);
				if ( val > (63 - pos))
					return;
				while(val--)
					blk[pos++] = 0;
				val = CBitFifo::Read(12);
				if(val & (1 << 11))
					blk[pos++] = val | 0xfffff000;
				else
					blk[pos++] = val;

				flag = 0;
			}
			else if (i >= 16)
			{
				if(intravlc)
				{
					while (CBitFifo::Query(4) != 6)
						CBitFifo::Read(1);
					CBitFifo::Read(4);					
				}
				else
				{
					while (CBitFifo::Query(2) != 2)
						CBitFifo::Read(1);
					CBitFifo::Read(2);
				}
				return ;
			}
		}
	}
}

void CMpeg2Decoder::GetInterAcVal(int *blk)
{
	int val, i, flag, pos = 0;

	for(i = 0; i < 64; i++)
		blk[i] = 0;

	if (CBitFifo::Query(1))
	{
		CBitFifo::Read(1);
		val = CBitFifo::Read(1) ? -1 : 1;
		blk[pos++] = val;
	}

	while(pos < 64)
	{
		val = 0;
		flag = 1;
		i = 1;
		val = CBitFifo::Read(val, 1);
		while(flag)
		{
			val = CBitFifo::Read(val, 1);
			i++;
			if (this->GetRunLevel(val, i, &pos, blk, 0))
				flag = 0;
			else if ( (i == 2) && (val == 2) )			//end of block
				return;
			else if	( (i == 6) && (val == 1) )			//escape
			{
				val = CBitFifo::Read(6);
				if ( val > (63 - pos))
					return;
				while(val--)
					blk[pos++] = 0;
				val = CBitFifo::Read(12);
				if(val & (1 << 11))
					blk[pos++] = val | 0xfffff000;
				else
					blk[pos++] = val;

				flag = 0;
			}
			else if (i >= 16)
			{				
				while (CBitFifo::Query(2) != 2)
					CBitFifo::Read(1);
				CBitFifo::Read(2);				
				return ;
			}
		}
	}
}

//#define DEBUG_DCT
void CMpeg2Decoder::decIntraBlock(char *block, int *qt, THuffValue *tbl, int *dc)
{
	int i, inblk[64], oublk[64];	
	
	*dc += this->GetHuffDcVal(tbl, 10, 12);
	inblk[0] = *dc;
	this->GetIntraAcVal(inblk, this->m_option.intravlcfmt);	
	CJmMatrix::izigzag(inblk, oublk, this->m_option.alterscan);
	CJmMatrix::mpeg2iquant(oublk, inblk, qt, 1, this->m_option.dc_precision);

#ifdef DEBUG_DCT
	double iiblk[64];

	for (i = 0; i < 64; i++)
		iiblk[i] = (double)inblk[i];
	CJmMatrix::idct8x8(iiblk);

	for(i = 0; i < 64; i++)
	{		
		iiblk[i] = (iiblk[i] > 0) ? (iiblk[i] + 0.5) : (iiblk[i] - 0.5);
//		iiblk[i] += 0x80;
		iiblk[i] = (iiblk[i] < 0) ? 0 : ((iiblk[i] > 255) ? -1 : iiblk[i]);
		block[i] = (char)iiblk[i];
	}
#else
	CJmMatrix::idct8x8(inblk);
	for(i = 0; i < 64; i++)
	{	
		oublk[i] = (inblk[i] < 0) ? 0 : ((inblk[i] > 255) ? -1 : inblk[i]);
		block[i] = (char)oublk[i];
	}
#endif
}

void CMpeg2Decoder::decInterBlock(char *block, int *qt)
{
	int i, inblk[64], oublk[64];

	this->GetInterAcVal(inblk);
	CJmMatrix::izigzag(inblk, oublk, this->m_option.alterscan);
	CJmMatrix::mpeg2iquant(oublk, inblk, qt);

#ifdef DEBUG_DCT
	double iiblk[64];

	for (i = 0; i < 64; i++)
		iiblk[i] = (double)inblk[i];
	CJmMatrix::idct8x8(iiblk);

	for(i = 0; i < 64; i++)
	{		
		iiblk[i] = (iiblk[i] > 0) ? (iiblk[i] + 0.5) : (iiblk[i] - 0.5);
		iiblk[i] = (iiblk[i] < -128) ? -128 : ((iiblk[i] > 127) ? 127 : iiblk[i]);
		block[i] = (char)iiblk[i];
	}
#else
	CJmMatrix::idct8x8(inblk);
	for(i = 0; i < 64; i++)
	{				
		inblk[i] = (inblk[i] < -128) ? -128 : ((inblk[i] > 127) ? 127 : inblk[i]);
		block[i] = (char)inblk[i];
	}
#endif
}

void CMpeg2Decoder::ClearBlock(char *block)
{
	int i;

	for (i = 0; i < 64; i++)	
		block[i] = 0;	
}

void CMpeg2Decoder::MacroBlock(char *block)
{
	int i, *dc;
	THuffValue *table;
	const int blkcount[3] = {6, 8, 12};

	this->ParseMBhdr(&(this->m_option));	
	if (this->m_option.mbtype & MACROBLOCK_INTRA)
	{		
		for (i = 0; i < blkcount[this->m_option.chroma - 1]; i++)
		{
			this->ClearBlock(block + (i << 6));
			if ( this->m_option.cbp & (1 << (blkcount[this->m_option.chroma - 1] - 1 - i)) )
			{
				dc = (i < 4) ? &(this->m_Ydc) : ((i & 1) ? &(this->m_Vdc) : &(this->m_Udc));
				table = (i < 4) ? (THuffValue *)gDClumtab : (THuffValue *)gDCchromtab;
				this->decIntraBlock(block + (i << 6), this->m_IntraQt, table, dc);
			}			
		}
		if (!this->m_option.concealmv)
		{
			this->m_option.pmv[0][0][0] = this->m_option.pmv[0][0][1] =this->m_option.pmv[0][1][0] = this->m_option.pmv[0][1][1] = 0;
			this->m_option.pmv[1][0][0] = this->m_option.pmv[1][0][1] =this->m_option.pmv[1][1][0] = this->m_option.pmv[1][1][1] = 0;	
		}
	}
	else
	{
		for (i = 0; i < blkcount[this->m_option.chroma - 1]; i++)
		{
			this->ClearBlock(block + (i << 6));
			if ( this->m_option.cbp & (1 << (blkcount[this->m_option.chroma - 1] - 1 - i)) )
			{
				this->decInterBlock(block + (i << 6), this->m_InterQt);
			}
		}
		this->m_Ydc = this->m_Udc = this->m_Vdc = 1 << (7 + this->m_option.dc_precision);
	}
	if ( (this->m_option.pictype == PIC_PREDICT) &&
		!(this->m_option.mbtype &(MACROBLOCK_MOTION_FORWARD | MACROBLOCK_INTRA)) )
	{
		this->m_option.pmv[0][0][0] = this->m_option.pmv[0][0][1] = 0;
		this->m_option.pmv[1][0][0] = this->m_option.pmv[1][0][1] = 0;
		if (this->m_option.frmfield == PICSTRUCT_FRAMEPIC)
			this->m_option.motiontype = MOTION_FRAME;
		else
		{
			this->m_option.motiontype = MOTION_FIELD;
			this->m_option.mvfldsel[0][0] = (this->m_option.frmfield == PICSTRUCT_BTMFIELD);
		}
		
	}
}

void CMpeg2Decoder::Slice(char *block, int verpos)
{
	const int bstep[3] = { 384, 512, 768 };
	int val;	
	int mb_row, mb_column;

	this->m_Ydc = this->m_Udc = this->m_Vdc = 1 << (7 + this->m_option.dc_precision);
	this->m_option.pmv[0][0][0] = this->m_option.pmv[0][0][1] =this->m_option.pmv[0][1][0] = this->m_option.pmv[0][1][1] = 0;
	this->m_option.pmv[1][0][0] = this->m_option.pmv[1][0][1] =this->m_option.pmv[1][1][0] = this->m_option.pmv[1][1][1] = 0;

	if(this->m_option.srcheight > 2800)
		mb_row = ( CBitFifo::Read(3) << 7) + verpos - 1;
	else
		mb_row = verpos - 1;
	this->m_option.mbaddrinc = 0;
	mb_column = 0;
	
	this->getQtable(CBitFifo::Read(5));					//prequant
	
	val = CBitFifo::Query(1);
	if (val)
	{
		CBitFifo::Read(1);								//intra_slice_flag
		CBitFifo::Read(1);								//intra_slice
		CBitFifo::Read(7);								//reserved_bits
		while (CBitFifo::Query(1))
		{
			CBitFifo::Read(1);							//extra_bit_slice
			CBitFifo::Read(8);							//extra_information_slice
		}
	}
	CBitFifo::Read(1);									//extra slice bit, = 0
	
	if(this->m_option.chroma == 0)
		return;

	do {
		if (this->m_option.mbaddrinc == 0)
			this->m_option.mbaddrinc = this->GetMBaddrinc();

		if (this->m_option.mbaddrinc == 1)
			this->MacroBlock(block);
		else 
			this->SkipMacroBlock(block);

		if (!(this->m_option.mbtype & MACROBLOCK_INTRA))
		{					
			this->MontionCompensation(block, mb_row, mb_column);
		}

		this->SaveMB(block, mb_row, mb_column);

		mb_column++;
		this->m_option.mbaddrinc--;
		block += bstep[this->m_option.chroma-1];
	} while(CBitFifo::Query(23) != 0);
}

void CMpeg2Decoder::SkipMacroBlock(char *block)
{
	int i;
	int addr[3] = {384, 512, 768};

	for (i = 0; i < addr[this->m_option.chroma - 1]; i++)
		block[i] = 0;

	this->m_Ydc = this->m_Udc = this->m_Vdc = 1 << (7 + this->m_option.dc_precision);
	if (this->m_option.pictype == PIC_PREDICT)
	{
		this->m_option.pmv[0][0][0] = this->m_option.pmv[0][0][1] = 0;
		this->m_option.pmv[1][0][0] = this->m_option.pmv[1][0][1] = 0;
	}
	if (this->m_option.frmfield == PICSTRUCT_FRAMEPIC)
		this->m_option.motiontype = MOTION_FRAME;
	else
	{
        this->m_option.motiontype = MOTION_FIELD;
		this->m_option.mvfldsel[0][0] = this->m_option.mvfldsel[0][1] = (this->m_option.frmfield == PICSTRUCT_BTMFIELD);
	}
	this->m_option.mbtype &= ~MACROBLOCK_INTRA;

}


void CMpeg2Decoder::MontionCompensation(char *block, int mb_row, int mb_column)
{
	int x_pos, y_pos;
	int mtype;

	x_pos = mb_column << 4;
	y_pos = mb_row << 4;
	if ( (this->m_option.mbtype & MACROBLOCK_MOTION_FORWARD) || (this->m_option.pictype == PIC_PREDICT) )
	{
		if (this->m_option.frmfield == PICSTRUCT_FRAMEPIC)
		{
			mtype = (this->m_option.motiontype == MOTION_FRAME) ? 0 : 1;

			this->Y_Compensation(block, x_pos, y_pos, mtype);				//block 0
			this->Y_Compensation(block+64, x_pos+8, y_pos, mtype);			//block 1
			this->Y_Compensation(block+128, x_pos, y_pos+8, mtype);			//block 2
			this->Y_Compensation(block+192, x_pos+8, y_pos+8, mtype);		//block 3				

			if (this->m_option.chroma != CHROMT_444)
				x_pos >>= 1;
			if (this->m_option.chroma == CHROMT_420)
				y_pos >>= 1;

			this->UV_Compensation(block+256, x_pos, y_pos, mtype);				//block 4,5
			if (this->m_option.chroma != CHROMT_420)
				this->UV_Compensation(block+384, x_pos, y_pos+8, mtype);		//block 6,7
			if(this->m_option.chroma == CHROMT_444)
			{
				this->UV_Compensation(block+512, x_pos+8, y_pos, mtype);		//block 8,9
				this->UV_Compensation(block+640, x_pos+8, y_pos+8, mtype);		//block 10,11
			}
		}
	}
	return;
}

void CMpeg2Decoder::Y_Compensation(char *curblk, int x_pos, int y_pos, int type)
{	
	int i, val, ref;
	char refblk[64];
	int dx, dy, xflag, yflag;

	ref = this->m_ref;
	dx = this->m_option.pmv[0][0][0] >> 1;
	dy = this->m_option.pmv[0][0][1] >> 1;
	xflag = this->m_option.pmv[0][0][0] & 1;
	yflag = this->m_option.pmv[0][0][1] & 1;

	if (!type)
	{
        this->m_buf[ref].YBuf.Read(refblk, x_pos+dx, y_pos+dy, xflag, yflag);
	}
	else
	{	
		yflag = dy & 1;
		this->m_buf[ref].YBuf.Readfield(refblk, x_pos+dx, (y_pos >> 1)+(dy >> 1), xflag, yflag, this->m_option.mvfldsel[0][0]);
		dx = this->m_option.pmv[1][0][0] >> 1;
		dy = this->m_option.pmv[1][0][1] >> 1;
		xflag = this->m_option.pmv[1][0][0] & 1;
		yflag = dy & 1;
		this->m_buf[ref].YBuf.Readfield(refblk+8, x_pos+dx, (y_pos >> 1)+(dy >> 1), xflag, yflag, this->m_option.mvfldsel[1][0]);
	}		

	for (i = 0; i < 64; i++)
	{
		val = curblk[i] + (refblk[i] & 0xff);
		val = (val < 0) ? 0 : ((val > 255) ? 255 : val);
		curblk[i] = (char)val;		
	}
}

void CMpeg2Decoder::UV_Compensation(char *curblk, int x_pos, int y_pos, int type)
{
	int i, val, ref;
	char refblk[64];
	int dx, dy, xflag, yflag;

	ref = this->m_ref;
	dx = this->m_option.pmv[0][0][0] >> 1;
	dy = this->m_option.pmv[0][0][1] >> 1;
	xflag = this->m_option.pmv[0][0][0] & 1;
	yflag = this->m_option.pmv[0][0][1] & 1;
	if (this->m_option.chroma != CHROMT_444)
	{
		dx = this->m_option.pmv[0][0][0] / 2;
		xflag = dx & 1;
		dx >>= 1;
	}
	if (this->m_option.chroma == CHROMT_420)
	{
		dy = this->m_option.pmv[0][0][1] / 2;
		yflag = dy & 1;
		dy >>= 1;
	}

	if (!type)
	{
        this->m_buf[ref].UBuf.Read(refblk, x_pos+dx, y_pos+dy, xflag, yflag);
	}
	else
	{		
		yflag = dy & 1;
		if (this->m_option.chroma == CHROMT_420)
		{
			dy = (this->m_option.pmv[0][0][1] >> 1) / 2;
			yflag = dy & 1;
		}		
		this->m_buf[ref].UBuf.Readfield(refblk, x_pos+dx, (y_pos >> 1)+(dy >> 1), xflag, yflag, this->m_option.mvfldsel[0][0]);
		dx = this->m_option.pmv[1][0][0] >> 1;
		dy = this->m_option.pmv[1][0][1] >> 1;
		xflag = this->m_option.pmv[1][0][0] & 1;
		yflag = dy & 1;
		if (this->m_option.chroma != CHROMT_444)
		{
			dx = this->m_option.pmv[1][0][0] / 2;
			xflag = dx & 1;
			dx >>= 1;
		}
		if (this->m_option.chroma == CHROMT_420)
		{
			dy = (this->m_option.pmv[1][0][1] >> 1) / 2;
			yflag = dy & 1;
		}
		this->m_buf[ref].UBuf.Readfield(refblk+8, x_pos+dx, (y_pos >> 1)+(dy >> 1), xflag, yflag, this->m_option.mvfldsel[1][0]);
	}

	for (i = 0; i < 64; i++)
	{
		val = curblk[i] + (refblk[i] & 0xff);
		val = (val < 0) ? 0 : ((val > 255) ? -1 : val);
		curblk[i] = (char)val;		
	}

	if (!type)
	{
		this->m_buf[ref].VBuf.Read(refblk, x_pos+dx, y_pos+dy, xflag, yflag);
	}
	else
	{		
		yflag = dy & 1;
		if (this->m_option.chroma == CHROMT_420)
		{
			dy = (this->m_option.pmv[0][0][1] >> 1) / 2;
			yflag = dy & 1;
		}		
		this->m_buf[ref].VBuf.Readfield(refblk, x_pos+dx, (y_pos >> 1)+(dy >> 1), xflag, yflag, this->m_option.mvfldsel[0][0]);
		dx = this->m_option.pmv[1][0][0] >> 1;
		dy = this->m_option.pmv[1][0][1] >> 1;
		xflag = this->m_option.pmv[1][0][0] & 1;
		yflag = dy & 1;
		if (this->m_option.chroma != CHROMT_444)
		{
			dx = this->m_option.pmv[1][0][0] / 2;
			xflag = dx & 1;
			dx >>= 1;
		}
		if (this->m_option.chroma == CHROMT_420)
		{
			dy = (this->m_option.pmv[1][0][1] >> 1) / 2;
			yflag = dy & 1;
		}
		this->m_buf[ref].VBuf.Readfield(refblk+8, x_pos+dx, (y_pos >> 1)+(dy >> 1), xflag, yflag, this->m_option.mvfldsel[1][0]);
	}

	for (i = 0; i < 64; i++)
	{
		val = curblk[64 + i] + (refblk[i] & 0xff);
		val = (val < 0) ? 0 : ((val > 255) ? -1 : val);
		curblk[64 + i] = (char)val;		
	}
}

void CMpeg2Decoder::SaveMB(char *block, int mb_row, int mb_column)
{
	int x_pos, y_pos, cur, ref;

	x_pos = mb_column << 4;
	y_pos = mb_row << 4;
	cur = this->m_cur;
	ref = this->m_ref;

	this->m_buf[cur].YBuf.Write(block, x_pos, y_pos);				//block 0
	this->m_buf[cur].YBuf.Write(block+64, x_pos+8, y_pos);			//block 1
	this->m_buf[cur].YBuf.Write(block+128, x_pos, y_pos+8);			//block 2
	this->m_buf[cur].YBuf.Write(block+192, x_pos+8, y_pos+8);		//block 3

	if (this->m_option.chroma != CHROMT_444)
		x_pos >>= 1;
	if (this->m_option.chroma == CHROMT_420)
		y_pos >>= 1;

	this->m_buf[cur].UBuf.Write(block+256, x_pos, y_pos);			//block 4
	this->m_buf[cur].VBuf.Write(block+320, x_pos, y_pos);			//block 5

	if (this->m_option.chroma != CHROMT_420)
	{
		this->m_buf[cur].UBuf.Write(block+384, x_pos, y_pos+8);		//block 6
		this->m_buf[cur].VBuf.Write(block+448, x_pos, y_pos+8);		//block 7
	}
	if (this->m_option.chroma == CHROMT_444)
	{
		this->m_buf[cur].UBuf.Write(block+512, x_pos+8, y_pos);		//block 8
		this->m_buf[cur].VBuf.Write(block+576, x_pos+8, y_pos);		//block 9
		this->m_buf[cur].UBuf.Write(block+640, x_pos+8, y_pos+8);	//block 10
		this->m_buf[cur].VBuf.Write(block+704, x_pos+8, y_pos+8);	//block 11
	}
}

