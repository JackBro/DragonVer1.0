#include "internal.h"

UINT8	GetBitbltDir(TPoint srcp, TPoint dstp, TSize size)
{
	TPoint srcep, dstep;

	srcep.x = srcp.x + size.cx;
	srcep.y = srcp.y + size.cy;
	dstep.x = dstp.x + size.cx;
	dstep.y = dstp.y + size.cy;

	if( (dstp.x >= srcp.x) && (dstp.x <= srcep.x) &&
		(dstp.y >= srcp.y) && (dstp.y <= srcep.y) )
		return BTMRIGHT;

	if( (dstp.x >= srcp.x) && (dstp.x <= srcep.x) &&
		(dstep.y >= srcp.y) && (dstep.y <= srcep.y) )
		return UPRIGHT;

	if( (dstep.x >= srcp.x) && (dstep.x <= srcep.x) &&
		(dstp.y >= srcp.y) && (dstp.y <= srcep.y) )
		return BTMLEFT;
	return UPLEFT;
}


UINT16 GetGeSpan(UINT8 fmt, UINT16 width)
{
	switch(fmt & 0xf)
	{
	case BLAYER_RGB1:
	case BLAYER_RGB2:
	case BLAYER_RGB4:
	case BLAYER_RGB8:
	case BLAYER_RGB444:
	case BLAYER_RGB666:
	case BLAYER_RGB32:
		width += 7;
		width >>= 3;
		width += 3;
		width >>= 2;
		width <<= 2;
		break;
	case BLAYER_RGB555:
	case BLAYER_RGB565:
		width <<= 1;
		break;
	case BLAYER_RGB24:
		width *= 3;
		break;
	default:
		break;
	}
	return width;
}

UINT32 GetBmemSize(UINT8 fmt, TSize size)
{
	UINT32 len;

	len = (UINT32)size.cx & 0xffff;
	len *= (UINT32)size.cy & 0xffff;
	switch(fmt & 0xf)
	{
	case BLAYER_RGB1:
		len += 7;
		len >>= 3;
		break;
	case BLAYER_RGB2:
		len += 3;
		len >>= 2;
		break;
	case BLAYER_RGB4:
		len ++;
		len >>= 1;
		break;
	case BLAYER_RGB8:
		break;
	case BLAYER_RGB444:
		len *= 3;
		len += 1;
		len >>= 1;
		break;
	case BLAYER_RGB555:
	case BLAYER_RGB565:
		len <<= 1;
		break;
	case BLAYER_RGB666:
		len *= 18;
		len += 7;
		len >>= 3;
		break;
	case BLAYER_RGB24:
		len *= 3;
		break;
	case BLAYER_RGB32:
		len <<= 2;
		break;
	default:
		break;
	}
	return ((len + 3) >> 2) << 2;
}

static const UINT8 gFmtTable[] = { 1, 2, 4, 8, 12, 16, 16, 18, 24, 32 };

UINT8 ParseBmpData(UINT8 *pData, PTBmpInfo info)
{
	UINT8 x, y;
	PTBitmapInfoHeader pHead;
	PTBitmapFileHeader pf;
	UINT8 i;

	pHead = (PTBitmapInfoHeader)(pData + sizeof(TBitmapFileHeader));
	pf = (PTBitmapFileHeader)pData;
	x = pHead->biWidth[0];
	y = pHead->biWidth[1];
	info->size.cx = ((UINT16)y << 8) | ((UINT16)x & 0xff);
	x = pHead->biHeight[0];
	y = pHead->biHeight[1];
	info->size.cy = ((UINT16)y << 8) | ((UINT16)x & 0xff);
	x = pf->dat[10];
	y = pf->dat[11];
	info->palsize = ((UINT16)y << 8) | ((UINT16)x & 0xff);

	x = sizeof(gFmtTable) / sizeof(UINT8);
	for(i = 0; i < x; i++)
	{
		if(gFmtTable[i] == pHead->biBitCount[0])
		{
			info->fmt = i;
			if(i == BLAYER_RGB555)
			{
				if(pHead->biDat1[0] != 0)
					info->fmt++;
			}
			info->ppal = pData + 0x36;
			info->pbuf = pData + info->palsize;
			info->palsize -= 0x36;
			info->palsize >>= 2;
			break;
		}
	}
	return 0;
}

UINT8 GetDeInterVal(UINT16 wcy)
{
	UINT8 len;

	len = (UINT8)(wcy & 0x1f);
	wcy >>= 5;
	len = (UINT8)(len +(wcy & 0x1f) + 1);
	wcy >>= 5;
	len = (UINT8)(len + (wcy & 0x1f));
	return len;
}

UINT16	Convert24To555(UINT32 color)
{
	UINT16 final_color = 0x01;

	final_color <<= 5;
	final_color |= (UINT16)((color & 0x00f80000) >> 19);
	final_color <<= 5;
	final_color |= (UINT16)((color & 0x0000f800) >> 11);
	final_color <<= 5;
	final_color |= (UINT16)((color & 0x000000f8) >> 3);

	return final_color;
}

UINT16	Convert24To565(UINT32 color)
{
	UINT16 final_color = 0;

	final_color |= (UINT16)((color & 0x00f80000) >> 19);
	final_color <<= 6;
	final_color |= (UINT16)((color & 0x0000fc00) >> 10);
	final_color <<= 5;
	final_color |= (UINT16)((color & 0x000000f8) >> 3);

	return final_color;
}

UINT32	Convert555To24(UINT16 color)
{
	UINT32 final_color = 0;

	final_color = (color & 0x7c00) >> 10;
	final_color <<= 8;
	final_color |= (color & 0x03e0) >> 5;
	final_color <<= 8;
	final_color |= color & 0x001f;
	final_color <<= 3;

	return final_color;
}

UINT32	Convert565To24(UINT16 color)
{
	UINT32 final_color = 0;

	final_color = (color & 0xf800) >> 11;
	final_color <<= 9;
	final_color |= (color & 0x07e0) >> 5;
	final_color <<= 7;
	final_color |= color & 0x001f;
	final_color <<= 3;

	return final_color;
}

//void	ComputerSizer(TSize src, UINT16 width, TSize *dst);
//void ComputerSizer(TSize src, UINT16 width, TSize *dst)
//{
//	UINT16	div, flag = 1;
//	UINT16 x, y;
//
//	while(flag)
//	{
//		div = MaxDivisor(src.cx, width);
//		x = src.cx / div;
//		y = width / div;
//
//		while( (x > 0x3f) || (y > 0x3f) )
//		{
//			x += 1;
//			x >>= 1;
//			y >>= 1;
//			y += 1;
//		}
//		dst->cx = (src.cx * y) / x;
//		dst->cy = (src.cy * y) / x;
//		width = dst->cx;
//		if(width % 2)
//			width++;
//		else
//			flag = 0;
//	}
//}
//
//
