

#include "internal.h"

CMpeg4Decoder::CMpeg4Decoder(int gap)
{
	this->m_gapsize = gap << 3;
}

void CMpeg4Decoder::Open(void)
{
	int i;

	this->ClearPoint();
	CMpeg4Base::Open();
	for (i = 0; i < 64; i++)
	{
		this->m_option.intraqmat[i] = (unsigned char)gDefaultIntraMat[i];
		this->m_option.interqmat[i] = (unsigned char)gDefaultInterMat[i];
	}
}
/*
int CMpeg4Decoder::Write(char *buf, int len)
{
	return CJmBitFifo::Write(buf, len);
}
*/
int CMpeg4Decoder::Read(char *buf, int size, int mode)
{
	int len = 0, val, flag = 0;

	buf = buf;
	while ((size - len) >= this->m_horwidth)
	{
		if(mode == MPEG4_DECNORMAL)
		{
			if(this->GetDataSize() < this->m_gapsize)
				return len;
		}
		if(this->NextStartCode() == 0)
			return len;
		val = CBitFifo::Read(8);		//get header marker

		if ((val >= VIDEO_OBJECT_MIN_START_CODE) && (val <= VIDEO_OBJECT_MAX_START_CODE))
		{			
			continue;
		}
		if ((val >= VIDEO_OBJECT_LAYER_MIN_CODE) && (val <= VIDEO_OBJECT_LAYER_MAX_CODE))
		{
			this->ParseVidObjLayer(&(this->m_option));
			if (flag == 0)
			{
				this->CheckOption();
				this->SetBufSize(this->m_option.edgedwidth, this->m_option.edgedheight);
				this->SetmbsMV(this->m_option.mb_width * this->m_option.mb_height);
				flag = 1;
			}
			continue;
		}
		switch(val) 
		{
		case VISUAL_OBJECT_SEQ_START_CODE:
			this->ParseVisObjSeqHdr(&(this->m_option));
			break;
		case VISULA_OBJECT_SEQ_END_CODE:
			return len;
			//break;
		case USER_DTAT_START_CODE:
			this->ParseUserData();
			break;
		case VISUAL_OBJECT_START_CODE:
			this->ParseVisObjHdr(&(this->m_option));			
			break;
		case GROUP_OF_VOP_START_CODE:
			this->ParseGrpVopHdr(&(this->m_option));			
			break;
		case VOP_START_CODE:
			this->ParseVopHdr(&(this->m_option));
			this->decframe(buf);
			break;
		default:
			break;
		}
	}

	return len;
}


void CMpeg4Decoder::decframe(char *buf)
{
	int xnum, ynum, mbaddr = 0;

	do {
		xnum = mbaddr % this->m_option.mb_width;
		ynum = mbaddr / this->m_option.mb_height;

		if (this->GetResyncMarker())
			this->ParseVidPackHdr(&(this->m_option));

		this->ParseMBHdr(&(this->m_option), mbaddr);

		if  ( (!this->m_option.notcoded) || (this->m_option.voptype == I_VOP) )
			this->MacroBlock(buf, mbaddr);
		else
			this->SkipMacroBlock(buf, mbaddr);

		this->MotionCompensation();

		this->SaveMB();

		mbaddr++;
		buf += 384;
	}while (CBitFifo::Query(23) != 0);

}


void CMpeg4Decoder::MacroBlock(char *block, int mbaddr)
{
	int i;
	char *data = block;

	if ((this->m_option.mbmode == MODE_INTRA) && (this->m_option.mbmode == MODE_INTRA_Q))
	{
		for(i = 0; i < 6; i++)
		{
			this->decIntraBlock(data, mbaddr, i);
			data += 64;
		}
	}
	else
	{
		for(i = 0; i < 6; i++)
		{
			this->decInterBlock(data);
			data += 64;
		}
	}
}

void CMpeg4Decoder::decIntraBlock(char *block, int mbaddr, int blknum)
{
	block = block; 
	int i, direct, dc_scaler;
	int predictor[8];
	int inblk[64], oublk[64];
	int coeffaddr;
	oublk[0] = 0;
	
	for(i = 0; i < 64; i++)
		inblk[i] = 0;
	dc_scaler = this->GetDcScaler(blknum < 4);
	this->CoeffPredict(predictor, mbaddr, blknum, dc_scaler);
	if (!this->m_option.acpred)
		this->m_mbs[mbaddr].pred_direct[blknum] = 0;
	direct = this->m_mbs[mbaddr].pred_direct[blknum];

	if (this->m_option.quant < this->m_option.intradcthr)
	{
		inblk[0] = this->GetIntraDCval(blknum < 4);
		coeffaddr = 1;
	}
	else
		coeffaddr = 0;
	if ( this->m_option.cbp & (1 << (5 - blknum)) )
	{
		this->GetIntraACval(inblk, direct, coeffaddr);
	}	

	this->CoeffAdd(inblk, predictor, mbaddr, blknum, dc_scaler);



}

int CMpeg4Decoder::GetIntraDCval(int lum)
{
	int dc_size, dc_diff;
	int msb, val;

	dc_size = lum ? this->GetlumDcSize() : this->GetChromDcSize();
	if (dc_size != 0)
	{
		val = this->CBitFifo::Read(dc_size);
		msb = val >> (dc_size - 1);
		if (msb == 0)
			dc_diff = -1 * (val ^ ((1 << dc_size) - 1));
		else 
			dc_diff = val;
	}
	else
		dc_diff = 0;

	if (dc_size > 8)
		CBitFifo::Read(1);

	return dc_diff;	
}

int CMpeg4Decoder::GetlumDcSize(void)
{
	int val, i;

	val = CBitFifo::Query(11);
	for(i = 11; i > 3; i--)
	{
		if (val == 1)
		{
			CBitFifo::Read(i);
			return (i+1);
		}
		val >>= 1;
	}
	CBitFifo::Read(gDClumTab[val].len);
	return gDClumTab[val].val;
}

int CMpeg4Decoder::GetChromDcSize(void)
{
	int val, i;

	val = CBitFifo::Query(12);
	for(i = 12; i > 2; i--)
	{
		if (val == 1)
		{
			CBitFifo::Read(i);
			return i;
		}
		val >>= 1;
	}
	return (3 - CBitFifo::Read(2));
}

void CMpeg4Decoder::GetIntraACval(int *blk, int direct, int addr)
{
	int i;
	const unsigned char *zigzag;
	TMpeg4Event event;
	
    i = addr;
	zigzag = gScanTable[direct];
	do {
		event = this->GetIntraEvent();
		if (event.run == -1)
			break;
		i += event.run;
		blk[zigzag[i]] = event.level;
		i++;
	}while (!event.last);

}

TMpeg4Event CMpeg4Decoder::GetIntraEvent(void)
{
	int val;
	TMpeg4Event event;

	val = this->GetIntraVLC();
	if (val == 0)
	{
		event.last = 1;
		event.run = event.level = -1;
		return event;
	}
	if (val != ESCAPEVAL)
	{
		event.level = val & 63;
		event.run = (val >> 6) & 63;
		event.last = (val >> 12) & 1;
		event.level = CBitFifo::Read(1) ? -event.level : event.level;
	}
	else
	{
		if (this->m_option.shortvidhdr == 1)
		{
			event.last = CBitFifo::Read(1);
			event.run = CBitFifo::Read(6);
			event.level = CBitFifo::Read(8);
			if ((event.level == 0) || (event.level == 128))
			{
				event.last = 1;
				event.run = event.level = -1;
			}
			event.level = (event.level << 24) >> 24;
		}
		else
		{
			switch(CBitFifo::Query(2)) 
			{
			case 0:
			case 1:
				CBitFifo::Read(1);
				val = this->GetIntraVLC();
				if (val == 0)
				{
					event.last = 1;
					event.run = event.level = -1;
					return event;
				}
				event.level = val & 63;
				event.run = (val >> 6) & 63;
				event.last = (val >> 12) & 1;
				event.level += gEscLevelMax[1][event.last][event.run];
				event.level = CBitFifo::Read(1) ? -event.level : event.level;
				break;

			case 2:
				CBitFifo::Read(2);
				val = this->GetIntraVLC();
				if (val == 0)
				{
					event.last = 1;
					event.run = event.level = -1;
					return event;
				}
				event.level = val & 63;
				event.run = (val >> 6) & 63;
				event.last = (val >> 12) & 1;
				event.run += gEscRunMax[1][event.last][event.level] + 1;
				event.level = CBitFifo::Read(1) ? -event.level : event.level;
				break;

			case 3:
				CBitFifo::Read(2);
				event.last = CBitFifo::Read(1);
				event.run = CBitFifo::Read(6);
				CBitFifo::Read(1);
				event.level = CBitFifo::Read(12);
				CBitFifo::Read(1);
				event.level = (event.level << 20) >> 20;
				break;

			default:
				event.last = 1;
				event.run = event.level = -1;
				break;
			}
		}	
	}
	return event;
}


int CMpeg4Decoder::GetIntraVLC(void)
{
	int val;
	TVLCtab tab;

	val = CBitFifo::Query(12);
	if (val >= 512)
		tab = ( (TVLCtab)(gIntraACTab0[(val >> 5) - 16]) );
	else if (val >= 128)
		tab = ( (TVLCtab)(gIntraACTab1[(val >> 2) - 32]) );
	else if (val >= 8)
		tab = ( (TVLCtab)(gIntraACTab2[val - 8]) );
	else
		return 0;

	CBitFifo::Read(tab.len);
	return (tab.val);
}

void CMpeg4Decoder::decInterBlock(char *block)
{
	block = block;
}

void CMpeg4Decoder::SkipMacroBlock(char *block, int mbaddr)
{
	int i;

	this->m_mbs[mbaddr].mv[0].x = this->m_mbs[mbaddr].mv[1].x = 
		this->m_mbs[mbaddr].mv[2].x = this->m_mbs[mbaddr].mv[3].x = 0;
	this->m_mbs[mbaddr].mv[0].y = this->m_mbs[mbaddr].mv[1].y = 
		this->m_mbs[mbaddr].mv[2].y = this->m_mbs[mbaddr].mv[3].y = 0;

	for (i = 0; i < 384; i++)
		block[i] = 0;
}

void CMpeg4Decoder::SaveMB(void)
{
}

void CMpeg4Decoder::MotionCompensation(void)
{
}





