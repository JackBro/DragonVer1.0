

#include "internal.h"

CMpeg2DecBase::CMpeg2DecBase()
{

}

CMpeg2DecBase::~CMpeg2DecBase()
{

}

void CMpeg2DecBase::Mpeg2Parse(PTMpeg2Option pinfo)
{
	int val, flag = 1;

	memset(pinfo, 0, sizeof(TMpeg2Option));
	while(flag)
	{
		if(this->NextStartCode() == 0)
			return;
		val = CBitFifo::Read(8);
		switch(val)
		{
		case MPEGPIC_START_CODE:
			break;

		case MPEG_USERDATA_CODE:
			break;

		case MPEG_SEQHEADER_CODE:
			this->ParseSeqHdr(&(this->m_option));
			flag |= 2;
			break;

		case MPEG_SEQERROR_CODE:
			break;

		case MPEG_EXTSTART_CODE:
			this->ParseExtHdr(&(this->m_option));
			flag |= 4;
			break;

		case MPEG_SEQEND_CODE:
			return;

		case MPEG_GROUPSTART_CODE:
			break;

		default:
			break;
		}
		if( (flag & 2) && (flag & 4) )
		{
			this->CheckOption();
			//memcpy(pinfo, &(this->m_option), sizeof(TMpeg2Option));
			*pinfo = this->m_option;
			return;
		}
	}
}


void CMpeg2DecBase::ParseUserData(PTMpeg2Option pinfo)
{		
	//int i = 0;
	//char userdata[100];
	pinfo = pinfo;
	while (CBitFifo::Query(24) != 1)
		//userdata[i++] = (char)CBitFifo::Read(8);
		CBitFifo::Read(8);
}



void CMpeg2DecBase::ParseMBmodehdr(PTMpeg2Option pinfo)
{
	pinfo->mbtype = (char)this->GetHuffIndex((THuffValue *)gMbtypeTable[pinfo->pictype - 1], 9, 32);		//macroblock type

	if ((pinfo->mbtype & MACROBLOCK_MOTION_FORWARD) || (pinfo->mbtype & MACROBLOCK_MOTION_BACKWARD))
	{
		if (pinfo->frmfield == PICSTRUCT_FRAMEPIC)
		{
			pinfo->motiontype = (char)(pinfo->fpfdct ? MOTION_FRAME : CBitFifo::Read(2));
			
			pinfo->mvcnt = (pinfo->motiontype == MOTION_FIELD) ? 2 : 1;
			pinfo->mvfmt = (char)((pinfo->motiontype == MOTION_FRAME) ? MVFORMAT_FRAME : MVFORMAT_FIELD);
			
		}
		else
		{
			pinfo->motiontype = (char)CBitFifo::Read(2);									//field_motion_type

			pinfo->mvcnt = (pinfo->motiontype == MOTION_16X8) ? 2 : 1;
			pinfo->mvfmt = MVFORMAT_FIELD;			
		}
		pinfo->dmv = (pinfo->motiontype == MOTION_DMV);
	}

	if ((pinfo->frmfield == PICSTRUCT_FRAMEPIC) && (pinfo->fpfdct == 0) && 
		(pinfo->mbtype & (MACROBLOCK_INTRA | MACROBLOCK_PATTERN)))

		pinfo->dcttype = (char)CBitFifo::Read(1);
	else 
		pinfo->dcttype = 0;
	
}


int	CMpeg2DecBase::GetHuffIndex(PTHuffValue table, int bitlen, int tsize)
{
	int i = 0, j = 0, val = 0;

	while(i < bitlen)
	{
		val = CBitFifo::Read(val, 1);
		i++;
		j = 0;
		while(j < tsize)
		{
			if( (table[j].value == val) && (table[j].length == i) )
				return j;
			j++;
		}
	}
	return j;
}

char CMpeg2DecBase::GetMotionCode(void)
{
	int code;
	char ret;

	if (CBitFifo::Read(1))
		return 0;

	code = CBitFifo::Query(9);
	if (code >= 64)
	{
		code >>= 6;
		CBitFifo::Read(gMVtable0[code].len);
		ret = (char)(CBitFifo::Read(1) ? -gMVtable0[code].val : gMVtable0[code].val);
		return ret;
	}
	if (code >= 24)
	{
		code >>= 3;
		CBitFifo::Read(gMVtable1[code].len);
		ret = (char)(CBitFifo::Read(1) ? -gMVtable1[code].val : gMVtable1[code].val);
		return ret;
	}
	code -= 12;
	if (code < 0)
		return -1;						//error

	CBitFifo::Read(gMVtable2[code].len);
	ret = (char)(CBitFifo::Read(1) ? -gMVtable2[code].val : gMVtable2[code].val);
	return ret;
}

void CMpeg2DecBase::decMV(PTMpeg2Option pinfo, int num, int s, int t)
{
	int r_size, limit;
	int vector;

	r_size = pinfo->f_code[s][t] - 1;
	limit = 16 << r_size;
	vector = pinfo->pmv[num][s][t];
	if (pinfo->mncode[num][s][t] > 0)
	{
		vector += ((pinfo->mncode[num][s][t] - 1) << r_size) + pinfo->mnresid[num][s][t] + 1;
		if (vector >= limit)
			vector -= limit << 1;
	}
	else if (pinfo->mncode[num][s][t] < 0)
	{
		vector -= ((-pinfo->mncode[num][s][t] - 1) << r_size) + pinfo->mnresid[num][s][t] + 1;
		if (vector < -limit)
			vector += limit << 1;
	}
	pinfo->pmv[num][s][t] = vector;
}


void CMpeg2DecBase::MotionVector(PTMpeg2Option pinfo, int num, int s)
{
	pinfo->mncode[num][s][0] = this->GetMotionCode();

	if ((pinfo->f_code[s][0] != 1) && (pinfo->mncode[num][s][0] != 0))
		pinfo->mnresid[num][s][0] = (char)CBitFifo::Read(pinfo->f_code[s][0] - 1);
	else
		pinfo->mnresid[num][s][0] = 0;

	this->decMV(pinfo, num, s, 0);

	if (pinfo->dmv == 1)
	{
		if (CBitFifo::Read(1))
			pinfo->dmvector[0] = (char)(CBitFifo::Read(1) ? -1 : 1);
		else
			pinfo->dmvector[0] = 0;
	}
	
	pinfo->mncode[num][s][1] = this->GetMotionCode();
	if ((pinfo->f_code[s][1] != 1) && (pinfo->mncode[num][s][1] != 0))
		pinfo->mnresid[num][s][1] = (char)CBitFifo::Read(pinfo->f_code[s][1] - 1);
	else
		pinfo->mnresid[num][s][1] = 0;

	if ((pinfo->mvfmt == MVFORMAT_FIELD) && (pinfo->frmfield == PICSTRUCT_FRAMEPIC))
		pinfo->pmv[num][s][1] >>= 1;

	this->decMV(pinfo, num, s, 1);
	
	if ((pinfo->mvfmt == MVFORMAT_FIELD) && (pinfo->frmfield == PICSTRUCT_FRAMEPIC))
		pinfo->pmv[num][s][1] <<= 1;

	if (pinfo->dmv == 1)
	{
		if (CBitFifo::Read(1))
			pinfo->dmvector[1] = (char)(CBitFifo::Read(1) ? -1 : 1);
		else
			pinfo->dmvector[1] = 0;
	}		
}

void CMpeg2DecBase::ParseMVhdr(PTMpeg2Option pinfo, int s)
{
	if (pinfo->mvcnt == 1)
	{
		if ((pinfo->mvfmt == MVFORMAT_FIELD) && (pinfo->dmv != 1))
			pinfo->mvfldsel[0][s] = pinfo->mvfldsel[1][s] = (char)CBitFifo::Read(1);
		this->MotionVector(pinfo, 0, s);

		pinfo->pmv[1][s][0] = pinfo->pmv[0][s][0];
		pinfo->pmv[1][s][1] = pinfo->pmv[0][s][1];
	} 
	else
	{
		pinfo->mvfldsel[0][s] = (char)CBitFifo::Read(1);
		this->MotionVector(pinfo, 0, s);

		pinfo->mvfldsel[1][s] = (char)CBitFifo::Read(1);
		this->MotionVector(pinfo, 1, s);
	}
}


void CMpeg2DecBase::getQtable(int quant)
{
	int i;

	if((quant == 0) || (quant > 31))
		return;
	this->m_quant = gQuantiserScale[this->m_option.qscaletype][quant];

	for(i = 0; i < 64; i++)
	{
		this->m_IntraQt[i] = ((gDefaultIntraQtable[i] * this->m_quant)) >> 5;
		this->m_InterQt[i] = ((gDefaultNonIntraQtable[i] * this->m_quant)) >> 5;
	}
}

int CMpeg2DecBase::GetMBaddrinc(void)
{
	int val, i = 0;
	
	val = CBitFifo::Query(11);
	while (val == 8)
	{
		CBitFifo::Read(11);
		i++;
		val = CBitFifo::Query(11);		
	}
	val = this->GetHuffIndex((THuffValue *)gAddrIncTable, 11, 34);		//macroblock address increment

	return (val + 33 * i);
}

void CMpeg2DecBase::ParseMBhdr(PTMpeg2Option pinfo)
{
	int blkcount[3] = {6, 8, 12};	
	
	this->ParseMBmodehdr(pinfo);

	if (pinfo->mbtype & MACROBLOCK_QUANT)			
		this->getQtable( CBitFifo::Read(5) );

	if ((pinfo->mbtype & MACROBLOCK_MOTION_FORWARD) || ((pinfo->mbtype & MACROBLOCK_INTRA) && pinfo->concealmv))
	{
		this->ParseMVhdr(pinfo, 0);
	}
		
	if (pinfo->mbtype & MACROBLOCK_MOTION_BACKWARD)
	{
		this->ParseMVhdr(pinfo, 1);
	}
		
	if ((pinfo->mbtype & MACROBLOCK_INTRA) && pinfo->concealmv)
		CBitFifo::Read(1);										//marker_bit
	if (pinfo->mbtype & MACROBLOCK_PATTERN)
	{
		pinfo->cbp = this->GetcbpCode();
		if (pinfo->chroma == CHROMT_422)
			pinfo->cbp = (pinfo->cbp << 2) | (char)CBitFifo::Read(2);
		if (pinfo->chroma == CHROMT_444)
			pinfo->cbp =(pinfo->cbp << 6) | (char)CBitFifo::Read(6);
	}
	else
		pinfo->cbp = (pinfo->mbtype & MACROBLOCK_INTRA) ? ((1 << blkcount[pinfo->chroma - 1]) - 1) : 0;
}


char CMpeg2DecBase::GetcbpCode(void)
{
	int code;

	code = CBitFifo::Query(9);
	if (code >= 128)
	{
		code = (code >> 4) - 8;
		CBitFifo::Read(gCBPtable0[code].len);
		return (char)gCBPtable0[code].val;
	}
	if (code >= 64)
	{
		code = (code >> 2) - 16;
		CBitFifo::Read(gCBPtable1[code].len);
		return (char)gCBPtable1[code].val;
	}
	if (code >= 8)
	{
		code = (code >> 1) - 4;
		CBitFifo::Read(gCBPtable2[code].len);
		return (char)gCBPtable2[code].val;
	}
	if (code >= 1)
	{
        CBitFifo::Read(gCBPtable3[code].len);
        return (char)gCBPtable3[code].val;
	}

	return 0;		//error!
}


void CMpeg2DecBase::ParseSeqHdr(PTMpeg2Option pinfo)
{
	int i;

	pinfo->srcwidth = CBitFifo::Read(12);						//horizontal_size_value
	pinfo->srcheight = CBitFifo::Read(12);						//vertical_size_value
	pinfo->aspectratio = (char)CBitFifo::Read(4);				//aspect_ratio_information
	pinfo->framerate = (char)CBitFifo::Read(4);					//frame_rate_code
	pinfo->bitrate = CBitFifo::Read(18);						//bit_rate_value
	CBitFifo::Read(1);											//marker_bit
	pinfo->vbvsize = CBitFifo::Read(10);						//vbv_buffer_size_value
	CBitFifo::Read(1);											//constrained_parameters_flag

	if (CBitFifo::Read(1))										//load_intra_quantiser_matrix
		for (i = 0; i < 64; i++)
			pinfo->intraqmat[i] = (char)CBitFifo::Read(8);		//need zig-zag scan ?

	if (CBitFifo::Read(1))										//load_nonintra_quantiser_matrix
		for (i = 0; i < 64; i++)
			pinfo->nonintraqmat[i] = (char)CBitFifo::Read(8);	//need zig-zag scan ?
	
}


void CMpeg2DecBase::ParseSeqExtHdr(PTMpeg2Option pinfo)
{
	pinfo->profile = (char)CBitFifo::Read(4);				//profile indication (low 3bit)
	pinfo->level = (char)CBitFifo::Read(4);					//level indication
	pinfo->progseq = (char)CBitFifo::Read(1);				//progressive_sequence
	pinfo->chroma = (char)CBitFifo::Read(2);				//chroma_format
	pinfo->srcwidth |= CBitFifo::Read(2) << 12;				//horizontal_size_extension
	pinfo->srcheight |= CBitFifo::Read(2) << 12;			//vertical_size_extension
	pinfo->bitrate |= CBitFifo::Read(12) << 18;				//bit_rate_extension
	CBitFifo::Read(1);										//marker_bit
	pinfo->vbvsize |= CBitFifo::Read(8) << 10;				//vbv_buffer_size_extension
	CBitFifo::Read(1);										//low_delay
	pinfo->framerate = pinfo->framerate * (CBitFifo::Read(2) + 1) + (CBitFifo::Read(5) + 1);	//frame_rate_extension_n  ..._d
}


void CMpeg2DecBase::ParseSeqDisExtHdr(PTMpeg2Option pinfo)
{
	pinfo->vidfmt = (char)CBitFifo::Read(3);				//video_format
	pinfo->clrdescrip = (char)CBitFifo::Read(1);			//colour_description
	if (pinfo->clrdescrip)
	{
		pinfo->clrprim = (char)CBitFifo::Read(8);			//clour_primaries
		pinfo->transcharac = (char)CBitFifo::Read(8);		//transfer_characteristics
		pinfo->matcoeff = (char)CBitFifo::Read(8);			//matrix_coefficients
	}
	pinfo->dispwidth = CBitFifo::Read(14);					//display_horizontal_size
	CBitFifo::Read(1);										//marker_bit
	pinfo->dispheight = CBitFifo::Read(14);					//display_vertical_size
}

void CMpeg2DecBase::ParseQmatExtHdr(PTMpeg2Option pinfo)
{
	int i;

	if (CBitFifo::Read(1))											//load_intra_quantiser_matrix
		for (i = 0; i < 64; i++)
			pinfo->intraqmat[i] = (char)CBitFifo::Read(8);			//need zig-zag scan ?

	if (CBitFifo::Read(1))											//load_nonintra_quantiser_matrix
		for (i = 0; i < 64; i++)
			pinfo->nonintraqmat[i] = (char)CBitFifo::Read(8);		//need zig-zag scan ?

	if (CBitFifo::Read(1))
		for (i = 0; i < 64; i++)
			pinfo->chrointraqmat[i] = (char)CBitFifo::Read(8);		//need zig-zag scan ?
		
	if (CBitFifo::Read(1))
		for (i = 0; i < 64; i++)
			pinfo->chrononintraqmat[i] = (char)CBitFifo::Read(8);	//need zig-zag scan ?
}

void CMpeg2DecBase::ParseSeqSlbExtHdr(PTMpeg2Option pinfo)
{
	pinfo->slbmode = (char)CBitFifo::Read(2);						//scalable_mode
	CBitFifo::Read(4);												//layer_id
	if (pinfo->slbmode == SPA_SCALABLE)
	{
		CBitFifo::Read(14);											//lower_layer_prediction_horizontal_size
		CBitFifo::Read(1);											//marker_bit
		CBitFifo::Read(14);											//lower_layer_prediction_vertical_size
		CBitFifo::Read(20);											//horizontal/vertical_subsampling_factor_m/n
	}
	if (pinfo->slbmode == TEMP_SCALABLE)
	{
		if (CBitFifo::Read(1))										//picture_mux_enable
			CBitFifo::Read(1);										//mux_to_progressive_sequence
		CBitFifo::Read(3);											//picture_mux_order
		CBitFifo::Read(3);											//picture_mux_factor
	}
}

void CMpeg2DecBase::ParsePicDisExtHdr(PTMpeg2Option pinfo)
{
	int i, frmoffset;

	if ((pinfo->progseq == 1) || (pinfo->frmfield != PICSTRUCT_FRAMEPIC))
		frmoffset = 1;
	else
	{
		if (pinfo->repfield == 1)
			frmoffset = 3;
		else
			frmoffset = 2;
	}
	for (i = 0; i < frmoffset; i++)
	{
		CBitFifo::Read(16);									//frame_center_horizontal_offset
		CBitFifo::Read(1);									//marker_bit
		CBitFifo::Read(16);									//frame_center_vetical_offset
		CBitFifo::Read(1);									//marker_bit
	}
}

void CMpeg2DecBase::ParsePicCodExtHdr(PTMpeg2Option pinfo)
{
	pinfo->f_code[0][0] = (char)CBitFifo::Read(4);
	pinfo->f_code[0][1] = (char)CBitFifo::Read(4);
	pinfo->f_code[1][0] = (char)CBitFifo::Read(4);
	pinfo->f_code[1][1] = (char)CBitFifo::Read(4);
	pinfo->dc_precision = (char)CBitFifo::Read(2);			//intra_dc_precision
	pinfo->frmfield = (char)CBitFifo::Read(2);				//picture_structure
	CBitFifo::Read(1);										//top_field_first
	pinfo->fpfdct = (char)CBitFifo::Read(1);				//frame_pred_frame_dct
	pinfo->concealmv = (char)CBitFifo::Read(1);				//concealment_motion_vectors
	pinfo->qscaletype = (char)CBitFifo::Read(1);			//q_scale_type
	pinfo->intravlcfmt = (char)CBitFifo::Read(1);			//intra_vlc_format
	pinfo->alterscan = (char)CBitFifo::Read(1);				//alternate_scan
	pinfo->repfield = (char)CBitFifo::Read(1);				//repeat_first_field
	CBitFifo::Read(1);										//chroma_420_type
	pinfo->proframe = (char)CBitFifo::Read(1);				//progressive_frame
	if (CBitFifo::Read(1))									//composite_display_flag
	{
		CBitFifo::Read(1);									//v_axis
		CBitFifo::Read(3);									//field_sequence
		CBitFifo::Read(1);									//sub_carrier
		CBitFifo::Read(7);									//burst_amplitude
		CBitFifo::Read(8);									//sub_carrier_phase
	}
}

void CMpeg2DecBase::ParsePicSpaExtHdr(PTMpeg2Option pinfo)
{
	pinfo = pinfo;
	CBitFifo::Read(10);										//lower_layer_temporal_refence
	CBitFifo::Read(1);										//marker_bit
	CBitFifo::Read(15);										//lower_layer_horizontal_offset
	CBitFifo::Read(1);										//marker_bit
	CBitFifo::Read(15);										//lower_layer_vertical_offset
	CBitFifo::Read(2);										//spatial_temporal_weight_code_table_index
	CBitFifo::Read(1);										//lower_layer_progressive_frame
	CBitFifo::Read(1);										//lower_layer_deinterlaced_field_select
}

void CMpeg2DecBase::ParsePicTemExtHdr(PTMpeg2Option pinfo)
{
	pinfo = pinfo;
	CBitFifo::Read(2);										//reference_select_code
	CBitFifo::Read(10);										//forward_temporal_reference
	CBitFifo::Read(1);										//marker_bit
	CBitFifo::Read(10);										//backward_temporal_reference
}


void CMpeg2DecBase::SetBufSize(int width, int height, int format)
{		
	this->m_buf[0].YBuf.SetFrameSize(width, height);
	this->m_buf[1].YBuf.SetFrameSize(width, height);

	if (format != CHROMT_444)
		width >>= 1;
	if (format == CHROMT_420)
		height >>= 1;
	this->m_buf[0].UBuf.SetFrameSize(width, height);
	this->m_buf[0].VBuf.SetFrameSize(width, height);
	this->m_buf[1].UBuf.SetFrameSize(width, height);
	this->m_buf[1].VBuf.SetFrameSize(width, height);	
}

void CMpeg2DecBase::ParseExtHdr(PTMpeg2Option pinfo)
{
	int val;	

	val = CBitFifo::Read(4);
	switch(val)
	{
	case MPEG_SEQ_EXTID:		//6.2.2.3
		this->ParseSeqExtHdr(pinfo);
		this->CheckOption();
		this->SetBufSize(this->m_option.mb_width << 4, this->m_option.mb_height << 4, this->m_option.chroma);
		break;	

	case MPEG_SEQDIS_EXTID:
		this->ParseSeqDisExtHdr(pinfo);		
		break;

	case MPEG_QMATRIX_EXTID:
		this->ParseQmatExtHdr(pinfo);		
		break;

	case MPEG_SEQSCALA_EXTID:
		this->ParseSeqSlbExtHdr(pinfo);		
		break;

	case MPEG_PICDISP_EXTID:
		this->ParsePicDisExtHdr(pinfo);		
		break;	

	case MPEG_PICCODE_EXTID:
		this->ParsePicCodExtHdr(pinfo);		
		break;

	case MPEG_PICSPA_EXTID:
		this->ParsePicSpaExtHdr(pinfo);		
		break;

	case MPEG_PICTEM_EXTID:
		this->ParsePicTemExtHdr(pinfo);		
		break;
	
	default:
		break;
	}
}


void CMpeg2DecBase::ParseGopHdr(PTMpeg2Option pinfo)
{
	pinfo = pinfo;
	CBitFifo::Read(25);											//group_start_code
	CBitFifo::Read(1);											//close_gop
	CBitFifo::Read(1);											//broken_link
}

void CMpeg2DecBase::ParsePicHdr(PTMpeg2Option pinfo)
{
	CBitFifo::Read(10);											//temporal_reference
	pinfo->pictype = (char)CBitFifo::Read(3);					//picture_coding_type
	pinfo->vbvdelay = CBitFifo::Read(16);						//vbv_delay
	if (pinfo->pictype == PIC_PREDICT || pinfo->pictype == PIC_BI_PREDICT)
	{
		CBitFifo::Read(1);										//full_pel_forward_vector
		CBitFifo::Read(3);										//forward_f_code
	}
	if (pinfo->pictype == PIC_BI_PREDICT)
	{
		CBitFifo::Read(1);										//full_pel_backward_vector
		CBitFifo::Read(3);										//backward_f_code
	}
	CBitFifo::Read(1);											//extra_bit_picture
}



int CMpeg2DecBase::NextStartCode(void)
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

