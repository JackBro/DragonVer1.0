

#include "internal.h"

#define MAX(A, B)		( ((A) > (B)) ? (A) : (B) )
#define MIN(A, B)		( ((A) < (B)) ? (A) : (B) )
#define ABS(X)			( (X) > 0 ? (X) : -(X) )
#define DIV_DIV(A, B)   ( (A) > 0 ? ((A) + ((B) >> 1)) / (B) : ((A) - ((B) >> 1)) / (B) )
#define RESCALE(pred, curr, coeff)	( ((coeff) != 0) ? DIV_DIV((coeff) * (pred), (curr)) : 0 )  

CMpeg4DecBase::CMpeg4DecBase()
{
}

CMpeg4DecBase::~CMpeg4DecBase()
{
}

void CMpeg4DecBase::Mpeg4Parse(PTMpeg4Option pinfo)
{
	int val, flg = 1;

	memset(pinfo, 0, sizeof(PTMpeg4Option));
	while (flg)
	{
		if (this->NextStartCode() == 0)
			return;
		val = CBitFifo::Read(8);
		if ((val >= VIDEO_OBJECT_MIN_START_CODE) && (val <= VIDEO_OBJECT_MAX_START_CODE))
		{			
			continue;
		}
		if ((val >= VIDEO_OBJECT_LAYER_MIN_CODE) && (val <= VIDEO_OBJECT_LAYER_MAX_CODE))
		{
			this->ParseVidObjLayer(&(this->m_option));
			this->CheckOption();
			*pinfo = this->m_option;
			return;
		}
		switch(val)
		{
		case VISUAL_OBJECT_SEQ_START_CODE:
			this->ParseVisObjSeqHdr(&(this->m_option));
			break;
		case USER_DTAT_START_CODE:
			this->ParseUserData();
			break;
		case VISUAL_OBJECT_START_CODE:
			this->ParseVisObjHdr(&(this->m_option));
			break;
		case GROUP_OF_VOP_START_CODE:			
			break;
		case VOP_START_CODE:
			break;
		case VISULA_OBJECT_SEQ_END_CODE:
			return;
		default:
			break;
		}
	}
}


void CMpeg4DecBase::SetmbsMV(int num)
{
	if (this->m_mbs != NULL)
	{
		delete [](this->m_mbs);
		this->m_mbs = NULL;
	}
	this->m_mbs = new TMacroBlock[num];
	memset(this->m_mbs, 0, sizeof(TMacroBlock) * num);
}

void CMpeg4DecBase::SetBufSize(int width, int height)
{
	int offset;

	offset = width * 32 + 32;
	
	this->m_buf[0].YBuf.SetFrameSize(width, height);
	this->m_buf[0].YBuf.ChangeBufPoint(offset);
	this->m_buf[1].YBuf.SetFrameSize(width, height);
	this->m_buf[1].YBuf.ChangeBufPoint(offset);
	offset >>= 1;
	this->m_buf[0].UBuf.SetFrameSize(width >> 1, height >> 1);
	this->m_buf[0].UBuf.ChangeBufPoint(offset);
	this->m_buf[0].VBuf.SetFrameSize(width >> 1, height >> 1);
	this->m_buf[0].VBuf.ChangeBufPoint(offset);
	
	this->m_buf[1].UBuf.SetFrameSize(width >> 1, height >> 1);
	this->m_buf[1].UBuf.ChangeBufPoint(offset);
	this->m_buf[1].VBuf.SetFrameSize(width >> 1, height >> 1);
	this->m_buf[1].VBuf.ChangeBufPoint(offset);
}

int CMpeg4DecBase::NextStartCode(void)
{
	int val, len;

	CBitFifo::rAlignBits();

	val = 0;
	do {
		val = CBitFifo::Read(val, 8);
		val &= 0xffffff;
		len = CBitFifo::GetDataSize();
	} while( (val != 0x1) && (len >= 8) );

	if(val == 1)
		return 1;
	return 0;
}
/*
int CMpeg4DecBase::NextStartCode(void)
{
	
	CBitFifo::rAlignBits();
	while (CBitFifo::Query(24) != 1)
	{
		CBitFifo::Read(8);
		if (CBitFifo::GetDataSize() < 24)
			return 0;
	}
	CBitFifo::Read(24);
	return 1;
}
*/
void CMpeg4DecBase::ParseUserData(void)
{
	//int i = 0;
	//char userdata[100];
	while (CBitFifo::Query(24) != 1)
		//userdata[i++] = (char)CBitFifo::Read(8);
		CBitFifo::Read(8);
}

void CMpeg4DecBase::ParseVisObjSeqHdr(PTMpeg4Option pinfo)
{
	pinfo->profile = CBitFifo::Read(8);
}

void CMpeg4DecBase::ParseVisObjHdr(PTMpeg4Option pinfo)
{
	if (CBitFifo::Read(1))									//is_visual_object_identifier
	{
		pinfo->vo_verid = (char)CBitFifo::Read(4);			//Visual_object_verid
		pinfo->vo_prio = (char)CBitFifo::Read(3);			//Visual_object_priority
	}
	else
		pinfo->vo_verid = 1;

	pinfo->vo_type = (char)CBitFifo::Read(4);				//visual_object_type
	if ((pinfo->vo_type == VIDEO_ID) || (pinfo->vo_type == STILL_TEXTURE_ID))
	{
		if (CBitFifo::Read(1))								//video_signal_type
		{
			pinfo->videofmt = (char)CBitFifo::Read(3);
			pinfo->videorange = (char)CBitFifo::Read(1);
			
			if(CBitFifo::Read(1))							//colour description
			{
				CBitFifo::Read(8);							//colour_primaries
				CBitFifo::Read(8);							//transfer_characteristics
				CBitFifo::Read(8);							//matrix_coefficients
			}
		}
	}
}

void CMpeg4DecBase::ParseVidObjLayer(PTMpeg4Option pinfo)
{
	pinfo->shortvidhdr = 0;		
	CBitFifo::Read(1);									//random_accessible_vol
	pinfo->vidobjtype = (char)CBitFifo::Read(8);
	if (CBitFifo::Read(1))
	{
		pinfo->vol_verid = (char)CBitFifo::Read(4);		//video_object_layer_verid
		CBitFifo::Read(3);								//video_object_layer_priority
	}
	else
		pinfo->vol_verid = 1;

	if (CBitFifo::Read(4) == 0x0f)						//aspect_ratio_info
	{
		CBitFifo::Read(8);								//par_width
		CBitFifo::Read(8);								//par_height
	}
	if (CBitFifo::Read(1))								//vol_control_parameters
	{
		pinfo->chroma = (char)CBitFifo::Read(2);
		if (pinfo->chroma != CHROMA_420)
			return;
		pinfo->lowdelay = (char)CBitFifo::Read(1);
		if ((char)CBitFifo::Read(1))					//vbv_parameters
		{
			CBitFifo::Read(15);							//first_half_bit_rate
			CBitFifo::Read(1);
			CBitFifo::Read(15);							//latter_half_bit_rate
			CBitFifo::Read(1);
			CBitFifo::Read(15);							//first_half_vbv_buffer_size
			CBitFifo::Read(1);
			CBitFifo::Read(3);							//latter_half_vbv_buffer_size
			CBitFifo::Read(11);							//first_half_vbv_occupancy
			CBitFifo::Read(1);
			CBitFifo::Read(15);							//latter_half_vbv_occupancy
			CBitFifo::Read(1);				
		}
	}
	pinfo->vidshape = (char)CBitFifo::Read(2);
	if ((pinfo->vidshape == SHAPE_GRAYSCALE) && (pinfo->vol_verid != 1))
		CBitFifo::Read(4);								//video_object_layer_shape_extension

	CBitFifo::Read(1);
	pinfo->timeinc = CBitFifo::Read(16);				//vop_time_increment_resolution
	if (pinfo->timeinc > 0)
		pinfo->timeinc_bits = this->log2bin(pinfo->timeinc - 1);
	else
		pinfo->timeinc_bits = 1;

	CBitFifo::Read(1);
	if (CBitFifo::Read(1))								//fixed_vop_rate
		CBitFifo::Read(pinfo->timeinc_bits);			//fixed_vop_time_increment

	if (pinfo->vidshape != SHAPE_BINARY_ONLY)
	{
		if (pinfo->vidshape == SHAPE_RECT)
		{
			CBitFifo::Read(1);
			pinfo->srcwidth = CBitFifo::Read(13);		//video_object_layer_width
			CBitFifo::Read(1);
			pinfo->srcheight = CBitFifo::Read(13);		//video_object_layer_height
			CBitFifo::Read(1);
		}
		pinfo->interlaced = (char)CBitFifo::Read(1);
		if (!CBitFifo::Read(1))							//obmc_disable, this bit must be set 1
			return;

		if ( CBitFifo::Read((pinfo->vol_verid == 1) ? 1 : 2) )		//sprite_enable
			return;													//we don't support sprite decode

		if ( (pinfo->vol_verid != 1) && (pinfo->vidshape != SHAPE_RECT) )
			CBitFifo::Read(1);										//sadct_disable
		if (CBitFifo::Read(1))										//not_8_bit
		{
			pinfo->quant_bits = (char)CBitFifo::Read(4);			//quant_precision
			CBitFifo::Read(4);										//bits_per_pixel
		}
		else
			pinfo->quant_bits = 5;

		if (pinfo->vidshape == SHAPE_GRAYSCALE)
		{
			CBitFifo::Read(1);										//no_gray_quant_update
			CBitFifo::Read(1);										//composition_method 
			CBitFifo::Read(1);										//linear_composition
		}
		pinfo->quant_type = (char)CBitFifo::Read(1);
		if (pinfo->quant_type)
		{
			if (CBitFifo::Read(1))									//load_intra_quant_mat
				this->GetQtable(pinfo, 1);
			if (CBitFifo::Read(1))									//load_nonintra_quant_mat
				this->GetQtable(pinfo, 0);

			if (pinfo->vidshape == SHAPE_GRAYSCALE)
				return;
		}
		if (pinfo->vol_verid != 1)
			pinfo->quanterpel = (char)CBitFifo::Read(1);
		else
			pinfo->quanterpel = 0;
		if (!CBitFifo::Read(1))										//complexity_estimation_disable
			return;
		CBitFifo::Read(1);											//resync_marker_disable
		if (CBitFifo::Read(1))										//data_partitioned
			CBitFifo::Read(1);										//reversible_vlc
		if (pinfo->vol_verid != 1)
		{
			if (CBitFifo::Read(1))									//newpred_enable
			{
				CBitFifo::Read(2);									//requested_upstream_message_type
				CBitFifo::Read(1);									//newpred_segment_type
			}
			if (CBitFifo::Read(1))									//reduced_resolution_vop_enable
				return ;
		}
		pinfo->scalability = (char)CBitFifo::Read(1);
		if (pinfo->scalability)										//scalability
			return;
	}
	else
	{
		if (pinfo->vol_verid != 1)
		{
			if (CBitFifo::Read(1))									//scalability
				return;
		}
		CBitFifo::Read(1);											//resync_marker_disable
	}
	pinfo->chroma = CHROMA_420;
}


void CMpeg4DecBase::GetQtable(PTMpeg4Option pinfo, int intra)
{
	int i = 0;
	unsigned char last, val = 0;

	if (intra)
	{
		do {
			last = val;
			val = (unsigned char)CBitFifo::Read(8);
			pinfo->intraqmat[gScanTable[0][i++]] = val;
		} 
		while ((val != 0) && (i < 64));
		i--;

		while(i < 64)
			pinfo->intraqmat[gScanTable[0][i++]] = last;
	}
	else
	{
		do {
			last = val;
			val = (unsigned char)CBitFifo::Read(8);
			pinfo->interqmat[gScanTable[0][i++]] = val;
		} 
		while ((val != 0) && (i < 64));
		i--;

		while(i < 64)
			pinfo->interqmat[gScanTable[0][i++]] = last;
	}
}

int CMpeg4DecBase::log2bin(int val)
{
	int n = 0;

	while (val)
	{
		val >>= 1;
		n++;
	}
	return n;
}

void CMpeg4DecBase::ParseGrpVopHdr(PTMpeg4Option pinfo)
{
	pinfo = pinfo;
	CBitFifo::Read(5);							//hours
	CBitFifo::Read(6);							//minutes
	CBitFifo::Read(1);
	CBitFifo::Read(6);							//seconds
	CBitFifo::Read(1);							//closed_gov
	CBitFifo::Read(1);							//broken_link
}


void CMpeg4DecBase::ParseVopHdr(PTMpeg4Option pinfo)
{
	int time_inc = 0;

	pinfo->voptype = (char)CBitFifo::Read(2);					//vop_coding_type
	while (CBitFifo::Read(1) != 0)									//modulo_time_base
		time_inc++;
	CBitFifo::Read(1);
	if (pinfo->timeinc_bits)
		CBitFifo::Read(pinfo->timeinc_bits);					//vop_time_increment
	CBitFifo::Read(1);
	if (!CBitFifo::Read(1))										//vop_coded
	{
		pinfo->voptype = N_VOP;
		return;
	}
	if ((pinfo->vidshape != SHAPE_BINARY_ONLY) && (pinfo->voptype == P_VOP))
		pinfo->roundtype = (char)CBitFifo::Read(1);
	
	if (pinfo->vidshape != SHAPE_RECT)
	{
		CBitFifo::Read(13);										//vop_width
		CBitFifo::Read(1);
		CBitFifo::Read(13);										//vop_height
		CBitFifo::Read(1);
		CBitFifo::Read(13);										//vop_horizontal_mc_spatial_ref
		CBitFifo::Read(1);
		CBitFifo::Read(13);										//vop_vetical_mc_spatial_ref
		CBitFifo::Read(1);

		CBitFifo::Read(1);										//change_conv_ratio_disable
		if (CBitFifo::Read(1))									//vop_constant_alpha
			CBitFifo::Read(8);									//vop_constant_alpha_value
	}
	if (pinfo->vidshape != SHAPE_BINARY_ONLY)
	{
		pinfo->intradcthr = gIntraDcThresholdTab[CBitFifo::Read(3)];
		if (pinfo->interlaced)
		{
			pinfo->topfield = (char)CBitFifo::Read(1);
			pinfo->alter_ver_scan = (char)CBitFifo::Read(1);
		}
	}
	if ((pinfo->quant = CBitFifo::Read(pinfo->quant_bits)) < 1)	//vop_quant
		pinfo->quant = 1;
	if (pinfo->voptype != I_VOP)
		pinfo->fcode_forward = (char)CBitFifo::Read(3);
	if (pinfo->voptype == B_VOP)
		pinfo->fcode_backward = (char)CBitFifo::Read(3);

	if (!pinfo->scalability)
	{
		if ((pinfo->vidshape != SHAPE_RECT) && (pinfo->voptype != I_VOP))
			CBitFifo::Read(1);									//vop_shape_coding_type
	}
}


int CMpeg4DecBase::GetResyncMarker(void)
{
	int nbits, val, code;
	int markerbits;

	if (this->m_option.voptype == I_VOP)
		markerbits = 17;
	else if (this->m_option.voptype == P_VOP)
		markerbits = 16 + this->m_option.fcode_forward;
	else if (this->m_option.voptype == B_VOP)
		markerbits = MAX(16 + this->m_option.fcode_forward, 18);
	else
		return 0;
	nbits = 8 - (CBitFifo::GetReadPoint() % 8);
	val = CBitFifo::Query(nbits);
	if ( val == ((1 << (nbits - 1)) - 1) )
	{
		code = (CBitFifo::Query(nbits + markerbits) << (nbits)) >> nbits;
		if (code == 1)
			return 1;
	}
	return 0;
}

void CMpeg4DecBase::ParseVidPackHdr(PTMpeg4Option pinfo)
{
	int nbits, markerbits;
	int mbnum;

	nbits = 8 - (CBitFifo::GetReadPoint() % 8);
	if (this->m_option.voptype == I_VOP)
		markerbits = 17;
	else if (this->m_option.voptype == P_VOP)
		markerbits = 16 + this->m_option.fcode_forward;
	else if (this->m_option.voptype == B_VOP)
		markerbits = MAX(16 + this->m_option.fcode_forward, 18);
	else
		return;

	CBitFifo::Read(nbits + markerbits);
	mbnum = ((pinfo->srcwidth + 15) >> 4) * ((pinfo->srcheight + 15) >> 4);
	CBitFifo::Read(this->log2bin(mbnum - 1));									//macroblock_number
	pinfo->quant = CBitFifo::Read(5);											//quant_scale

	if (CBitFifo::Read(1))														//header_extension_code
	{
		return;																	//if hec = 1, then have some codes here
	}
}

void CMpeg4DecBase::ParseMBHdr(PTMpeg4Option pinfo, int mbaddr)
{
	if (pinfo->voptype != B_VOP)
	{
		if (pinfo->voptype == P_VOP)
			pinfo->notcoded = (char)CBitFifo::Read(1);

		if ((!pinfo->notcoded) || (pinfo->voptype == I_VOP))
		{
			pinfo->mcbpc = this->GetMCBPCcode();
			this->m_mbs[mbaddr].mbmode = pinfo->mbmode = pinfo->mcbpc & 7;
			pinfo->cbpc = (pinfo->mcbpc >> 4) & 3;

			if ( (!pinfo->shortvidhdr) && ((pinfo->mbmode == MODE_INTRA) || (pinfo->mbmode == MODE_INTRA_Q)) )
				pinfo->acpred = (char)CBitFifo::Read(1);

			if (pinfo->mbmode != MODE_STUFFING)
			{
				pinfo->cbpy = this->GetCBPYcode();
				pinfo->cbp = (pinfo->cbpy << 2) | pinfo->cbpc;
			}
			else
				return;

			if ((pinfo->mbmode == MODE_INTRA_Q) || (pinfo->mbmode == MODE_INTER_Q))
			{
				pinfo->dquant = (char)gDquantTab[CBitFifo::Read(2)];
				pinfo->quant += pinfo->dquant;
				pinfo->quant = (pinfo->quant > 31) ? 31 : ((pinfo->quant < 1) ? 1 : pinfo->quant);
			}
			this->m_mbs[mbaddr].quant = pinfo->quant;
			if (pinfo->interlaced)
				this->ParseInterlaceHdr(pinfo);

			if ( (pinfo->mbmode == MODE_INTER) || (pinfo->mbmode == MODE_INTER_Q) )
			{
				if (pinfo->interlaced && pinfo->field_pred)
				{
					this->GetMotionVector(mbaddr, 0);
					this->GetMotionVector(mbaddr, 1);					
				}					
				else
				{					
					this->GetMotionVector(mbaddr, 0);
					this->m_mbs[mbaddr].mv[1].x = this->m_mbs[mbaddr].mv[2].x = this->m_mbs[mbaddr].mv[3].x =
						this->m_mbs[mbaddr].mv[0].x;
					this->m_mbs[mbaddr].mv[1].y = this->m_mbs[mbaddr].mv[2].y = this->m_mbs[mbaddr].mv[3].y =
						this->m_mbs[mbaddr].mv[0].y;
				} 
			}
			else if (pinfo->mbmode == MODE_INTER4V)
			{
				this->GetMotionVector(mbaddr, 0);
				this->GetMotionVector(mbaddr, 1);
				this->GetMotionVector(mbaddr, 2);
				this->GetMotionVector(mbaddr, 3);
			}				
			else
			{
				if (pinfo->voptype == P_VOP)
				{
					this->m_mbs[mbaddr].mv[0].x = this->m_mbs[mbaddr].mv[1].x = 
						this->m_mbs[mbaddr].mv[2].x = this->m_mbs[mbaddr].mv[3].x = 0;
					this->m_mbs[mbaddr].mv[0].y = this->m_mbs[mbaddr].mv[1].y = 
						this->m_mbs[mbaddr].mv[2].y = this->m_mbs[mbaddr].mv[3].y = 0;
				}				
			}
		}
		else
		{
			//reset 
		}
	}
}

int CMpeg4DecBase::GetMCBPCcode(void)
{
	int code;

	code = CBitFifo::Query(9);
	if (this->m_option.voptype == I_VOP)
	{
		if (code == 1)
		{
			CBitFifo::Read(9);
			return 0;
		}
		else if (code < 8)
			return -1;

		code >>= 3;
		if (code >= 32)
		{
			CBitFifo::Read(1);
			return 3;
		}

		CBitFifo::Read(gMCBPCIntraTab[code].len);
		return gMCBPCIntraTab[code].val;
	}
	else
	{
		if (code == 1)
		{
			CBitFifo::Read(9);
			return 0;
		}
		else if (code == 0)
			return -1;
		
		if (code >= 256)
		{
			CBitFifo::Read(1);
			return 0;
		}
		if (code >= 192)
		{
			CBitFifo::Read(3);
			return 1;
		}
		if (code >= 128)
		{
			CBitFifo::Read(3);
			return 2;
		}
		CBitFifo::Read(gMCBPCInterTab[code].len);
		return gMCBPCInterTab[code].val;
	}
}

int CMpeg4DecBase::GetCBPYcode(void)
{
	int cbpy, code;

	code = CBitFifo::Query(6);
	if (code < 2)
		return -1;

	if (code >= 48)
	{
		CBitFifo::Read(2);
		cbpy = 15;
	}
	else
	{
		CBitFifo::Read(gCBPYTab[code].len);
		cbpy = gCBPYTab[code].val;
	}

	if ( !((this->m_option.mbmode == MODE_INTRA) || (this->m_option.mbmode == MODE_INTRA_Q)) )
		cbpy = 15 - cbpy;
	return cbpy;
}

void CMpeg4DecBase::ParseInterlaceHdr(PTMpeg4Option pinfo)
{
	if ( (pinfo->cbp) || ((pinfo->mbmode == MODE_INTRA) || (pinfo->mbmode == MODE_INTRA_Q)) )
		pinfo->dcttype = (char)CBitFifo::Read(1);
	if ((pinfo->mbmode == MODE_INTER) || (pinfo->mbmode == MODE_INTER_Q))
	{
		pinfo->field_pred = (char)CBitFifo::Read(1);
		if (pinfo->field_pred)
		{
			if ( (pinfo->voptype == P_VOP) || ((pinfo->voptype == B_VOP) && (pinfo->mbmode != MODE_INTER_Q)) )
			{
				pinfo->field_fortop = (char)CBitFifo::Read(1);
				pinfo->field_forbot = (char)CBitFifo::Read(1);
			}
			if ((pinfo->voptype == B_VOP) && (pinfo->mbmode != MODE_INTER_Q))
			{
				pinfo->field_baktop = (char)CBitFifo::Read(1);
				pinfo->field_bakbot = (char)CBitFifo::Read(1);
			}						
		}
	}	
}

//this function get the real mv, equal pred mv + diff mv
void CMpeg4DecBase::GetMotionVector(int mbaddr, int blknum)
{
	TVector pmv, dmv;
	int scale_fac = 1 << (this->m_option.fcode_forward - 1);
	int limit = scale_fac << 5;

	dmv = this->decMV();
	pmv = this->GetPMV(mbaddr, blknum);

	this->m_mbs[mbaddr].mv[blknum].x = pmv.x + dmv.x;
	this->m_mbs[mbaddr].mv[blknum].y = pmv.y + dmv.y;

	if (this->m_mbs[mbaddr].mv[blknum].x < -limit)
		this->m_mbs[mbaddr].mv[blknum].x += limit << 1;
	if (this->m_mbs[mbaddr].mv[blknum].x > (limit - 1))
		this->m_mbs[mbaddr].mv[blknum].x -= limit << 1;

	if (this->m_mbs[mbaddr].mv[blknum].y < -limit)
		this->m_mbs[mbaddr].mv[blknum].y += limit << 1;
	if (this->m_mbs[mbaddr].mv[blknum].y > (limit - 1))
		this->m_mbs[mbaddr].mv[blknum].y -= limit << 1;	
}

TVector CMpeg4DecBase::GetPMV(int mbaddr, int blknum)
{
	TVector pmv[4], zeroMV = {0, 0};
	int xnum, ynum;
	int lx, ly, lz;         // left
	int tx, ty, tz;         // top
	int rx, ry, rz;         // top-right
	int lpos, tpos, rpos;
	int num_cand, last_cand;

	xnum = mbaddr % this->m_option.mb_width;
	ynum = mbaddr / this->m_option.mb_height;
	
	switch(blknum)
	{
	case 0:
		lx = xnum -1;	ly = ynum;		lz = 1;
		tx = xnum;		ty = ynum - 1;	tz = 2;
		rx = xnum + 1;	ry = ynum - 1;	rz = 2;
		break;
	case 1:
		lx = xnum;		ly = ynum;		lz = 0;
		tx = xnum;		ty = ynum - 1;	tz = 3;
		rx = xnum + 1;	ry = ynum - 1;	rz = 2;
		break;
	case 2:
		lx = xnum - 1;	ly = ynum;		lz = 3;
		tx = xnum;		ty = ynum;		tz = 0;
		rx = xnum;		ry = ynum;		rz = 1;
		break;
	default:
		lx = xnum;		ly = ynum;		lz = 2;
		tx = xnum;		ty = ynum;		tz = 0;
		rx = xnum;		ry = ynum;		rz = 1;
		break;
	}

	lpos = lx + ly * this->m_option.mb_width;
	rpos = rx + ry * this->m_option.mb_width;
	tpos = tx + ty * this->m_option.mb_width;
	last_cand = num_cand = 0;

	if ((lpos >= 0) && (lx >= 0))
	{
		num_cand++;
		last_cand = 1;
		pmv[1] = this->m_mbs[lpos].mv[lz];
	}
	else
		pmv[1] = zeroMV;
	if (tpos >= 0)
	{
		num_cand++;
		last_cand = 2;
		pmv[2] = this->m_mbs[tpos].mv[tz];
	}
	else
		pmv[2] = zeroMV;
	if ((rpos >= 0) && (rx < this->m_option.mb_width))
	{
		num_cand++;
		last_cand = 3;
		pmv[3] = this->m_mbs[rpos].mv[rz];
	}
	else
		pmv[3] = zeroMV;

	if (num_cand != 1) 
	{		// set median
		pmv[0].x =
			MIN( MAX(pmv[1].x, pmv[2].x), MIN( MAX(pmv[2].x, pmv[3].x), MAX(pmv[1].x, pmv[3].x) ) );
		pmv[0].y =
			MIN( MAX(pmv[1].y, pmv[2].y), MIN( MAX(pmv[2].y, pmv[3].y), MAX(pmv[1].y, pmv[3].y) ) );
		return pmv[0];
	}
	return pmv[last_cand]; 
}
//this function get diff mv from the bitstrings
TVector CMpeg4DecBase::decMV(void)
{
	int x_mv, y_mv, res;
	int scale_fac = this->m_option.fcode_forward - 1;
	TVector mv;

	x_mv = this->GetMVdata();
	if ((scale_fac == 1) || (x_mv == 0))
		mv.x = x_mv;
	else
	{
		res = CBitFifo::Read(this->m_option.fcode_forward - 1);
		mv.x = ((ABS(x_mv) - 1) * scale_fac) + res + 1;
		if (x_mv < 0)
			mv.x = -mv.x;
	}

	y_mv = this->GetMVdata();
	if ((scale_fac == 1) || (y_mv == 0))
		mv.y = y_mv;
	else
	{
		res = CBitFifo::Read(this->m_option.fcode_forward - 1);
		mv.y = ((ABS(y_mv) - 1) * scale_fac) + res + 1;
		if (y_mv < 0)
			mv.y = - mv.y;
	}

	return mv;
}

int CMpeg4DecBase::GetMVdata(void)
{
	int code;
	int ret;

	if (CBitFifo::Read(1))
		return 0;

	code = CBitFifo::Query(11);
	if (code >= 256)
	{
		code >>= 8;
		CBitFifo::Read(gMVTab0[code].len);
		ret = CBitFifo::Read(1) ? -gMVTab0[code].val : gMVTab0[code].val;
		return ret;
	}
	if (code >= 128)
	{
		code >>= 5;
		CBitFifo::Read(gMVTab1[code].len);
		ret = CBitFifo::Read(1) ? -gMVTab1[code].val : gMVTab1[code].val;
		return ret;
	}
	if (code >= 16)
	{
		code >>= 2;
		CBitFifo::Read(gMVTab2[code].len);
		ret = CBitFifo::Read(1) ? -gMVTab2[code].val : gMVTab2[code].val;
		return ret;
	}
	CBitFifo::Read(gMVTab3[code].len);
	ret = CBitFifo::Read(1) ? -gMVTab3[code].val : gMVTab3[code].val;
	return ret;
}


int CMpeg4DecBase::GetDcScaler(int lum)
{
	int quant = this->m_option.quant;

	if (quant < 5)
		return 8;
	if ((quant < 25) && !lum)
		return ((quant + 13) / 2);
	if (quant < 9)
		return (quant << 1);
	if (quant < 25)
		return (quant + 8);

	if (lum)
		return ((quant << 1) - 16);
	else
		return (quant - 6);
}

void CMpeg4DecBase::CoeffPredict(int predictor[8], int mbaddr, int blknum, int dc_scaler)
{
	int left_quant, top_quant, cur_quant;
	int *left, *top, *diag, *current;
	int *pLeft, *pTop, *pDiag;
	int xnum = mbaddr % this->m_option.mb_width;
	int i;

	left = top = diag = current = 0;
	pLeft = pTop = pDiag = (int *)gDefaultPredVal;
	left_quant = top_quant = cur_quant = this->m_mbs[mbaddr].quant;

	if ( xnum && 
		((this->m_mbs[mbaddr - 1].mbmode == MODE_INTRA) ||
		(this->m_mbs[mbaddr - 1].mbmode == MODE_INTRA_Q)) )
	{
		left = this->m_mbs[mbaddr - 1].pred_val[0];
		left_quant = this->m_mbs[mbaddr - 1].quant;
	}
	if ( (mbaddr >= this->m_option.mb_width) &&
		((this->m_mbs[mbaddr - this->m_option.mb_width].mbmode == MODE_INTRA) ||
		(this->m_mbs[mbaddr - this->m_option.mb_width].mbmode == MODE_INTRA_Q)) )
	{
		top = this->m_mbs[mbaddr - this->m_option.mb_width].pred_val[0];
		top_quant = this->m_mbs[mbaddr - this->m_option.mb_width].quant;
	}
	if ( xnum && (mbaddr >= this->m_option.mb_width + 1) &&
		((this->m_mbs[mbaddr - this->m_option.mb_width - 1].mbmode == MODE_INTRA) ||
		(this->m_mbs[mbaddr - this->m_option.mb_width - 1].mbmode == MODE_INTRA_Q)) )
	{
		diag = this->m_mbs[mbaddr - this->m_option.mb_width - 1].pred_val[0];
	}
	current = this->m_mbs[mbaddr].pred_val[0];

	switch(blknum) 
	{
	case 0:
		if (left)
			pLeft = left + MBPRED_SIZE;
		if (top)
			pTop = top + (MBPRED_SIZE << 1);
		if (diag)
			pDiag = diag + 3 * MBPRED_SIZE;
		break;
	case 1:
		pLeft = current;
		left_quant = cur_quant;
		if (top)
		{
			pTop = top + 3 * MBPRED_SIZE;
			pDiag = top + (MBPRED_SIZE << 1);
		}
		break;
	case 2:
		if (left)
		{
			pLeft = left + 3 * MBPRED_SIZE;
			pDiag = left + MBPRED_SIZE;
		}
		pTop = current;
		top_quant = cur_quant;
		break;
	case 3:
		pLeft = current + (MBPRED_SIZE << 1);
		left_quant = cur_quant;
		pTop = current + MBPRED_SIZE;
		top_quant = cur_quant;
		pDiag = current;
		break;
	case 4:
		if (left)
			pLeft = left + (MBPRED_SIZE << 2);
		if (top)
			pTop = top + (MBPRED_SIZE << 2);
		if (diag)
			pDiag = diag + (MBPRED_SIZE << 2);
		break;
	case 5:
		if (left)
			pLeft = left + 5 * MBPRED_SIZE;
		if (top)
			pTop = top + 5 * MBPRED_SIZE;
		if (diag)
			pDiag = diag + 5 * MBPRED_SIZE;
		break;
	default:
		break;
	}

	if (ABS(pLeft[0] - pDiag[0]) < ABS(pDiag[0] - pTop[0]))
	{
		this->m_mbs[mbaddr].pred_direct[blknum] = 1;	// vertical
		predictor[0] = DIV_DIV(pTop[0], dc_scaler);
		for (i = 1; i < 8; i++)
		{
			predictor[i] = RESCALE(top_quant, cur_quant, pTop[i]);
		}
	}
	else
	{
		this->m_mbs[mbaddr].pred_direct[blknum] = 2;	// horizontal
		predictor[0] = DIV_DIV(pLeft[0], dc_scaler);
		for (i = 1; i < 8; i++)
		{
			predictor[i] = RESCALE(left_quant, cur_quant, pLeft[i + 7]);
		}
	}
}

void CMpeg4DecBase::CoeffAdd(int *block, int predictor[8], int mbaddr, int blknum, int dc_scaler)
{
	int i, level;
	int pred_direct = this->m_mbs[mbaddr].pred_direct[blknum];
	int *pcur = this->m_mbs[mbaddr].pred_val[blknum];

	level = block[0] + predictor[0];
	block[0] = level;
	pcur[0] = block[0] * dc_scaler;

	if (pred_direct == 1)
	{
		for (i = 1; i < 8; i++)
		{
			level = block[i] + predictor[i];
			block[i] = level;
			pcur[i] = block[i];
			pcur[i + 7] = block[i * 8];
		}
	}
	else if (pred_direct == 2)
	{
		for (i = 1; i < 8; i++)
		{
			level = block[i * 8] + (char)predictor[i];
			block[i * 8] = level;
			pcur[i + 7] = block[i * 8];
			pcur[i] = block[i];
		}
	}
	else
	{
		for (i = 1; i < 8; i++)
		{
			pcur[i] = block[i];
			pcur[i + 7] = block[i * 8];
		}
	}
}












