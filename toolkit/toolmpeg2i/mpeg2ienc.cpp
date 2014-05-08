

#include "internal.h"

CMpeg2IEncoder::CMpeg2IEncoder(int size) : CBitFifo(size, BIGBIT_ORDER)
{
}

void CMpeg2IEncoder::Open(TMpeg2Option *option)
{
	CMpeg2IBase::Open();
	if(option == NULL)
		this->Init();
	else
		//memcpy(&(this->m_option), option, sizeof(TMpeg2Option));
		this->m_option = *option;
	this->m_pos = 0;
	this->m_frmcount = 0;
	this->m_vwc = this->getVwc();
	this->m_prevmquant = DEFAULT_MPEG2INIT_QUANT;
	this->m_slice = 0;
	this->m_actuallen = 0;
	this->m_targetlen = 0;
	CBitFifo::ClearPoint();
}

void CMpeg2IEncoder::SetImageSize(int width, int height)
{
	this->m_option.srcwidth = width;
	this->m_option.srcheight = height;
}

void CMpeg2IEncoder::SetYuvFmt(int fmt)
{
	const int fmtb[][2] = 
	{
		{ YUV420BLK, CHROMT_420 },
		{ YUV422BLK, CHROMT_422 },
		{ YUV444BLK, CHROMT_444 }
	};
	int i;

	this->m_option.chroma = CHROMT_FMTRES;
	for(i = 0; i < (sizeof(fmtb) / sizeof(int)) >> 1; i++)
	{
		if(fmtb[i][0] == fmt)
			this->m_option.chroma = (char)fmtb[i][1];
	}
}

void CMpeg2IEncoder::Init(void)
{
	memset(&(this->m_table), 0, sizeof(TMpeg2Table));
	this->m_table.QTable = (UINT8 *)gDefaultIntraQtable;
	this->m_option.chroma = CHROMT_420;
	this->m_option.frmrate = FRMRATE_25;
	this->m_option.level = LEVEL_LOW;
	this->m_option.profile = PROFILE_MAIN;
	this->m_option.dc_precision = MPEGDC_8BITS;
	this->m_option.pictype = PIC_INTRA;
	this->m_option.vidfmt = VID_PAL;
	this->m_option.vbvsize = 20;
	this->m_option.bitrate = 115200;
	this->m_option.srcwidth = 176;
	this->m_option.srcheight = 144;
	this->m_option.frmfield = PICSTRUCT_FRAMEPIC;
}

void CMpeg2IEncoder::putSeqHdr(void)
{
	int val;

	CBitFifo::wAlignBits();
	CBitFifo::Write(MPEG_SEQHEADER_CODE | MPEG_STARTCODE, 32);
	CBitFifo::Write(this->m_option.srcwidth, 12);
	CBitFifo::Write(this->m_option.srcheight, 12);
	CBitFifo::Write(DEFAULT_ASPECT_RATIO, 4);
	CBitFifo::Write(this->m_option.frmrate, 4);
	val = (this->m_option.bitrate + 200) / 400;
	CBitFifo::Write(val, 18);
	CBitFifo::Write(1, 1);			//marker bit
	CBitFifo::Write(this->m_option.vbvsize, 10);
	CBitFifo::Write(0, 1);			//always 0 for mpeg2
	CBitFifo::Write(0, 1);			//load custom intra quantizer table
	CBitFifo::Write(0, 1);			//load custom non-intra quantizer table
}

void CMpeg2IEncoder::putSeqExt(void)
{
	int val;

	CBitFifo::wAlignBits();
	CBitFifo::Write(MPEG_EXTSTART_CODE | MPEG_STARTCODE, 32);
	CBitFifo::Write(MPEG_SEQ_EXTID, 4);
	CBitFifo::Write( (this->m_option.profile << 4) | this->m_option.level , 8);
	CBitFifo::Write(1, 1);	//progressive sequence
	CBitFifo::Write(this->m_option.chroma, 2);
	CBitFifo::Write(this->m_option.srcwidth >> 12, 2);
	CBitFifo::Write(this->m_option.srcheight >> 12, 2);
	val = (this->m_option.bitrate + 200) / 400;
	CBitFifo::Write(val >> 18, 12);
	CBitFifo::Write(1, 1);		//marker bit
	CBitFifo::Write(this->m_option.vbvsize >> 10, 8);
	CBitFifo::Write(0, 1);		//low delay
	CBitFifo::Write(0, 2);		//frame rate externsion n
	CBitFifo::Write(0, 5);		//frame rate externsion d
}
	
void CMpeg2IEncoder::putSeqDispExt(void)
{
	CBitFifo::wAlignBits();
	CBitFifo::Write(MPEG_EXTSTART_CODE | MPEG_STARTCODE, 32);
	CBitFifo::Write(MPEG_SEQDIS_EXTID, 4);
	CBitFifo::Write(this->m_option.vidfmt, 3);
	CBitFifo::Write(1, 1);		//colour description enable
	CBitFifo::Write(ITU_R_BT_470_BG, 8);	//colour primary
	CBitFifo::Write(ITU_R_BT_470_BG, 8);	//color character
	CBitFifo::Write(this->m_option.srcwidth, 14);	//display width
	CBitFifo::Write(1, 1);
	CBitFifo::Write(this->m_option.srcheight, 14);	//display height
}

void CMpeg2IEncoder::putUserData(char *buf, int len)
{
	CBitFifo::wAlignBits();
	CBitFifo::Write(buf, len << 3);
}

void CMpeg2IEncoder::putPictHdr(void)
{
	CBitFifo::wAlignBits();
	CBitFifo::Write(MPEGPIC_START_CODE | MPEG_STARTCODE, 32);
	CBitFifo::Write(this->m_frmcount, 10);
	CBitFifo::Write(this->m_option.pictype, 3);
	CBitFifo::Write(0xffff, 16);	//vbv_delay
	CBitFifo::Write(0, 1);
}

void CMpeg2IEncoder::putPictCodExt(void)
{
	CBitFifo::wAlignBits();
	CBitFifo::Write(MPEG_EXTSTART_CODE | MPEG_STARTCODE, 32);
	CBitFifo::Write(MPEG_PICCODE_EXTID, 4);
	CBitFifo::Write(15, 4);		//forward horizontal f
	CBitFifo::Write(15, 4);		//forward vertical f
	CBitFifo::Write(15, 4);		//backward horizontal f
	CBitFifo::Write(15, 4);		//backward vertical f
	CBitFifo::Write(this->m_option.dc_precision, 2);
	CBitFifo::Write(this->m_option.frmfield, 2);
	CBitFifo::Write(0, 1);		//top field first = 0
	CBitFifo::Write(1, 1);		//frame pred frame dct = 1
	CBitFifo::Write(0, 1);		//concealment motion vector = 0
	CBitFifo::Write(0, 1);		//q scale type = 0
	CBitFifo::Write(1, 1);		//intra vlc type = 1
	CBitFifo::Write(0, 1);		//alternate scan = 0
	CBitFifo::Write(0, 1);		//repeat first = 0
	CBitFifo::Write(1, 1);		//chroma 420 type = 1
	CBitFifo::Write(1, 1);		//progressive frame = 0
	CBitFifo::Write(0, 1);		//composite display flag = 0
}

void CMpeg2IEncoder::GenerateHeader(void)
{
	this->putSeqHdr();
	//if mpeg2
	this->putSeqExt();
	this->putSeqDispExt();
	this->putUserData(VENDER_MARKER, (int)strlen(VENDER_MARKER));
	CBitFifo::wAlignBits(32);	//4byte align
}

void CMpeg2IEncoder::encblock(char *block, int *lastDc)
{
	int inblk[64], oublk[64];
	int i;

	for(i = 0; i < 64; i++)
		inblk[i] = ((int)block[i] & 0xff) - 0x80;
	CJmMatrix::fpdct8x8(inblk, oublk);
	*lastDc = 0;
}

int	CMpeg2IEncoder::calcuMquant(void)
{
	int val;

	val = this->m_prevmquant;
	if( ((this->m_actuallen - this->m_targetlen) << 4) > this->m_actuallen )
		val += 2 ;
	if( ((this->m_targetlen - this->m_actuallen ) << 4) > this->m_actuallen )
		val -= 2 ;
	val = (val < 2) ? 2 : ((val > 62) ? 62 : val);
	this->m_prevmquant = val;
	return val;
}

void CMpeg2IEncoder::GeneratePicHdr(void)
{
	//picture data
	this->putPictHdr();
	this->putPictCodExt();
}

void CMpeg2IEncoder::putMbtype(void)
{
	int mbtype;
	THuffValue huf;

	CBitFifo::Write(1, 1);			//MBAinc = 1, in intra frame, put address inc
	this->m_quant = this->calcuMquant();
	mbtype = PIC_INTRA;
	if(this->m_quant != this->m_prevmquant)
		mbtype |= MPEG2_MBQUANT;
	huf = gMbtypeTable[PIC_INTRA-1][mbtype];
	CBitFifo::Write(huf.value, huf.length);		//macro block type
	if(mbtype & MPEG2_MBQUANT)
	{
		CBitFifo::Write(this->m_quant >> 1, 5);
		this->m_prevmquant = this->m_quant;
	}
}

void CMpeg2IEncoder::putSliceHdr(void)
{
	CBitFifo::wAlignBits();
	CBitFifo::Write( (MPEG_MINSLICE_CODE | MPEG_STARTCODE) + this->m_slice, 32);
	CBitFifo::Write(this->m_prevmquant >> 1, 5);
	CBitFifo::Write(0, 1);			//slice extra bit
}

void CMpeg2IEncoder::encSlice(char *buf)
{
	int i;

	this->putSliceHdr();
	switch(this->m_option.chroma)
	{
	case CHROMT_422:
		break;
	case CHROMT_420:
		for(i = 0; i < this->m_horwidth; i+= 384)
		{
			this->putMbtype();
			this->encblock(buf+i, &this->m_Ydc);
			this->encblock(buf+i+64, &this->m_Ydc);
			this->encblock(buf+i+128, &this->m_Ydc);
			this->encblock(buf+i+192, &this->m_Ydc);
			this->encblock(buf+i+256, &this->m_Udc);
			this->encblock(buf+i+320, &this->m_Vdc);
		}
		break;
	case CHROMT_444:
		break;
	default:
		break;
	}
	this->m_slice++;
}

int	CMpeg2IEncoder::Write(char *buf, int len)
{
	int len1 = len;

	if(this->m_option.chroma == 0)
		return 0;
	if(this->m_pos >= this->m_vwc)
		return 0;
	if(this->m_pos == 0)
	{
		this->CheckOption();
		this->GenerateHeader();
		this->GeneratePicHdr();
	}
	while(len1 >= this->m_horwidth)
	{
		this->encSlice(buf);
		this->m_pos += this->m_horwidth;
		len1 -= this->m_horwidth; 
		buf += this->m_horwidth;
	}
	return len - len1;
}

int	CMpeg2IEncoder::Read(char *buf, int size)
{
	size = CBitFifo::Read(buf, size << 3);
	return (size + 7) >> 3;
}



