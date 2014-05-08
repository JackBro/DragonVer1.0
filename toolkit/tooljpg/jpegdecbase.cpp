

#include "internal.h"

CJpegDecBase::CJpegDecBase(int size) : CFifoBuf(size)
{
	this->m_JpgIndex.YUVType = JPEG_UNKNOWN;
}

void CJpegDecBase::Open(PTJpegIndex pinfo)
{
//	unsigned char huff[416];
	unsigned char *phuff = (unsigned char *)gDefaultHuffmanTbl;
//	int i, j = 0;

	if(pinfo->YUVType == JPEG_UNKNOWN)
		return;
	memcpy(&this->m_JpgIndex, pinfo, sizeof(TJpegIndex));
	CJpegDecBase::SetSize(pinfo->vwc << 2);

	CJpegBase::createHuffmanTable( phuff+1, phuff+17, this->m_Table.Ldc );
	CJpegBase::createHuffmanTable( phuff+30, phuff+46, this->m_Table.Cdc );
	CJpegBase::createHuffmanTable( phuff+59, phuff+75, this->m_Table.Lac );
	CJpegBase::createHuffmanTable( phuff+238, phuff+254, this->m_Table.Cac );
/*
	if(pinfo->HTCount == 4)
	{
		phuff = huff;
		huff[j++] = 0;
		for(i = 0, j; i < pinfo->HTLen[0]; i++, j++)
			huff[j] = pinfo->HT[0][i];
		huff[j++] = 0x1;
		for(i = 0; i < pinfo->HTLen[2]; i++, j++)
			huff[j] = pinfo->HT[2][i];
		huff[j++] = 0x10;
		for(i = 0; i < pinfo->HTLen[1]; i++, j++)
			huff[j] = pinfo->HT[1][i];
		huff[j++] = 0x11;
		for(i = 0; i < pinfo->HTLen[3]; i++, j++)
			huff[j] = pinfo->HT[3][i];
		
		CJpegBase::createHuffmanTable( phuff+1, phuff+17, this->m_Table.Ldc );
		phuff += pinfo->HTLen[0] + 1;
		CJpegBase::createHuffmanTable( phuff+1, phuff+17, this->m_Table.Cdc );
		phuff += pinfo->HTLen[2] + 1;
		CJpegBase::createHuffmanTable( phuff+1, phuff+17, this->m_Table.Lac );
		phuff += pinfo->HTLen[1] + 1;
		CJpegBase::createHuffmanTable( phuff+1, phuff+17, this->m_Table.Cac );
	}	*/
/*	else	
	{
        CJpegBase::createHuffmanTable( phuff+1, phuff+17, this->m_Table.Ldc );
		CJpegBase::createHuffmanTable( phuff+30, phuff+46, this->m_Table.Cdc );
		CJpegBase::createHuffmanTable( phuff+59, phuff+75, this->m_Table.Lac );
		CJpegBase::createHuffmanTable( phuff+238, phuff+254, this->m_Table.Cac );
	}	*/
	this->ClearPoint();
}

int CJpegDecBase::Write(char *buf, int leng)
{
	CBufferBase pool;
	int len, i, Ydc, Udc, Vdc;

	if(this->m_JpgIndex.YUVType == JPEG_UNKNOWN)
		return 0;
	if(leng < (int)this->m_JpgIndex.eop)
	{
		this->m_JpgIndex.YUVType = JPEG_UNKNOWN;
		return 0;
	}
	len = this->m_JpgIndex.eop - this->m_JpgIndex.offset;
	this->m_EncBit.SetSize(len);
	pool.SetSize(len);
	this->m_EncBit.SetBitOrder(BIGBIT_ORDER);

	len = this->ff002ff(buf + this->m_JpgIndex.offset, pool.m_buf, len);
	this->m_EncBit.Write(pool.m_buf, len << 3);
	Ydc = Udc = Vdc = 0;
	len = (int)this->m_JpgIndex.vwc << 2;
	switch(this->m_JpgIndex.YUVType)
	{
	case JPEG_422:
		for(i = 0; i < len; i += 256)
		{
			this->blockdec(&Ydc, JPEG_YBLOCK);
			this->blockdec(&Ydc, JPEG_YBLOCK);
			this->blockdec(&Udc, JPEG_UVBLOCK);
			this->blockdec(&Vdc, JPEG_UVBLOCK);
		}
		break;
	case JPEG_420:
	case JPEG_411:
		for(i = 0; i < len; i += 384)
		{
			this->blockdec(&Ydc, JPEG_YBLOCK);
			this->blockdec(&Ydc, JPEG_YBLOCK);
			this->blockdec(&Ydc, JPEG_YBLOCK);
			this->blockdec(&Ydc, JPEG_YBLOCK);
			this->blockdec(&Udc, JPEG_UVBLOCK);
			this->blockdec(&Vdc, JPEG_UVBLOCK);
		}
		break;
	case JPEG_400:
		for(i = 0; i < len; i += 64)
			this->blockdec(&Ydc, JPEG_YBLOCK);
		break;
	case JPEG_444:
		for(i = 0; i < len; i += 192)
		{
			this->blockdec(&Ydc, JPEG_YBLOCK);
			this->blockdec(&Udc, JPEG_UVBLOCK);
			this->blockdec(&Vdc, JPEG_UVBLOCK);
		}
		break;
	default:
		break;
	}

	this->m_JpgIndex.YUVType = JPEG_UNKNOWN;
	return leng;
}



int CJpegDecBase::ff002ff(char *src, char *dst, int len)
{
	int i = 1, j = 1;
	unsigned char byte0, byte1;

	dst[0] = src[0];
	while(i < len) 
	{
		byte0 = src[i-1];
		byte1 = src[i];
		if( (byte0 != 0xff) || ((byte0 == 0xff) && (byte1 != 0x00)) ) 
		{
			dst[j] = byte1;
			j++;
		}
		i++;
	}
	return j;
}

int	CJpegDecBase::GetHuffIndex(PTHuffValue table, int bitlen, int tsize)
{
	int i = 0, j = 0, val = 0;

	while(i < bitlen)
	{
		val = this->m_EncBit.Read(val, 1);
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

int	CJpegDecBase::GetHuffDcVal(PTHuffValue dc, int bitlen, int tsize)
{
	int index, val;

	index = this->GetHuffIndex(dc, bitlen, tsize);
	val = this->m_EncBit.Read(index);
	return this->ValAdjust(val, index);
}

void CJpegDecBase::GetHuffAcVal(PTHuffValue ac, int *blk)
{
	int index, val, pos = 1, i;

	while(pos < 64)
	{
		index = this->GetHuffIndex(ac, 16, 251);
		if(index == 240)
		{
			i = 16;
			i = (i < (64 - pos)) ? i : (64 - pos);
			while(i--)
			{
				blk[pos] = 0;
				pos++;
			}
		}
		else if(index == 0)
		{
			while(pos < 64)
			{
				blk[pos] = 0;
				pos++;
			}
		}
		else
		{
			i = index >> 4;
			i = (i < (63 - pos)) ? i : (63 - pos);
			while(i--)
			{
				blk[pos] = 0;
				pos++;
			}
			index &= 0xf;
			if(index == 0)
			{
				this->m_dbug.PrintMsg("index = 0\n");
			}
			//val = 0;
			val = this->m_EncBit.Read(index);
			blk[pos] = this->ValAdjust(val, index);
			pos++;
		}
	}
}


