#include "internal.h"

UINT32 GetAmemSize(UINT8 fmt, TSize size)
{
	UINT32 len;

	len = (UINT32)size.cx & 0xffff;
	switch(fmt)
	{
	case ALAYER_LINEBUF:
		len <<= 5;
		break;
	case ALAYER_2FRAMEBUF:
		len *= (UINT32)size.cy & 0xffff;
		len <<= 2;
		break;
	case ALAYER_1FRAMEBUF:
		len *= (UINT32)size.cy & 0xffff;
		len += 3;
		len >>= 2;
		len <<= 3;
		break;
	default:
		break;
	}
	return len;
}

UINT32 GetDecAmemSize(UINT8 yuvfmt, TSize size)
{
//	UINT32 len;

	switch(yuvfmt)
	{
	case V5B_LBUF_YUV_422:
	case V5B_LBUF_YUV_411:
	case V5B_LBUF_YUV_444:
	case V5B_LBUF_YUV_400:
		size.cy = 8;
		break;
	case V5B_LBUF_YUV_420:
		size.cy = 16;
		break;
	default:
		break;
	}
	size.cy += 5;
	return GetYuvSize(V5B_LBUF_YUV_422, size);
//	return len;
}

UINT32 GetYuvSize(UINT8 yuvfmt, TSize size)
{
	UINT32 len;

	len = (UINT32)size.cx & 0xffff;
	len *= (UINT32)size.cy & 0xffff;
	switch(yuvfmt)
	{
	case V5B_LBUF_YUV_422:
		len <<= 1;
		break;
	case V5B_LBUF_YUV_420:
	case V5B_LBUF_YUV_411:
		len *= 3;
		len >>= 1;
		break;
	case V5B_LBUF_YUV_444:
		len *= 3;
		break;
	case V5B_LBUF_YUV_400:
	default:
		break;
	}
	return len;
}

UINT32 GetRgbSize(TSize size)
{
	UINT32 len;

	len = (UINT32)size.cx & 0xffff;
	len *= (UINT32)size.cy & 0xffff;
	len *= 3;

	return len;
}

UINT32 GetCapLbuf0Size(UINT8 fmt, UINT16 width)
{
	TSize size, size1;

	size.cx = width;
	size.cy = 1; //Not zero to calcute the lbuf hight.

	GetJpegSize(fmt, size, &size1);

	if(fmt == V5B_LBUF_YUV_444)
		fmt = V5B_LBUF_YUV_422;

	return GetYuvSize(fmt, size1);
}

UINT32 GetDecLbuf0Size(UINT8 fmt, UINT16 width)
{
	TSize size;

	size.cy = 0;
	switch(fmt)
	{
	case V5B_LBUF_YUV_422:
		size.cx = ((width + 15) >> 4) << 4;
		size.cy = 8;
		break;
	case V5B_LBUF_YUV_411:
		size.cx = ((width + 31) >> 5) << 5;
		size.cy = 8;
		break;
	case V5B_LBUF_YUV_444:
	case V5B_LBUF_YUV_400:
		size.cx = ((width + 7) >> 3) << 3;
		size.cy = 8;
		break;
	case V5B_LBUF_YUV_420:
		size.cx = ((width + 15) >> 4) << 4;
		size.cy = 16;
		break;
	default:
		break;
	}
	if(fmt == V5B_LBUF_YUV_444)
		fmt = V5B_LBUF_YUV_422;
	return GetYuvSize(fmt, size);
}

UINT32 GetDecLbuf1Size(UINT8 fmt, UINT16 width)
{
	TSize size;

	size.cx = width;
	size.cy = 0;
	switch(fmt)
	{
	case V5B_LBUF_YUV_422:
	case V5B_LBUF_YUV_411:
	case V5B_LBUF_YUV_444:
	case V5B_LBUF_YUV_400:
		size.cy = 8;
		break;
	case V5B_LBUF_YUV_420:
		size.cy = 16;
		break;
	default:
		break;
	}
	size.cy += 5;
	return GetYuvSize(V5B_LBUF_YUV_422, size);
}

UINT8 YuvmodeToYuvfmt(UINT8 mode)
{
	switch(mode)
	{
	case V5B_LBUF_YUV_422:
		return YUV422_UYVY;
	case V5B_LBUF_YUV_420:
		return YUV420_UYVY;
	case V5B_LBUF_YUV_411:
		return YUV411_YYYYUV;
	case V5B_LBUF_YUV_400:
		return YUV400;
	default:
		return mode;
	}
}

UINT8 YuvmodeToYuvLbuf(UINT8 mode)
{
	switch(mode)
	{
	case V5B_LBUF_YUV_422:
		return YUV422LBUF;
	case V5B_LBUF_YUV_420:
		return YUV420LBUF;
	case V5B_LBUF_YUV_411:
		return YUV411LBUF;
	case V5B_LBUF_YUV_400:
		return YUV400LBUF;
	default:
		return mode;
	}
}

UINT8 YuvmodeToYuvBlk(UINT8 mode)
{
	switch(mode)
	{
	case V5B_LBUF_YUV_422:
		return YUV422BLK;
	case V5B_LBUF_YUV_420:
		return YUV420BLK;
	case V5B_LBUF_YUV_411:
		return YUV411BLK;
	case V5B_LBUF_YUV_400:
		return YUV400BLK;
	default:
		return mode;
	}
}

UINT32 CalCapFrmSize(UINT8 fmt, TSize size, UINT8 tcr)
{
	UINT32 len;

	len = GetYuvSize(fmt, size);

	if(tcr < 5)
		tcr = 5;

	len <<= 2;
	len /= (tcr-1);


	return len;
}
