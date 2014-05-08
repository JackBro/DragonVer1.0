
#include "internal.h"

int CTypeProp::getVwc(int width, int height, int format, int unit)
{
	format &= (1 << CLASSSHIFT) - 1;
	return (width * height * unit * gImageProp[format]) >> 3;
}

char *CTypeProp::GetFormatDesc(int fmt)
{
	int type, index;

	type = (fmt & CLASSMASK) >> CLASSSHIFT;
	index = fmt & FORMATMASK;
	switch(type)
	{
	case VIDEOCLASS:
		return (char *)gImageStreamType[index];
	case AUDIOCLASS:
		return (char *)gAudStreamType[index];
	case SYSTEMCLASS:
		return (char *)gSystemStreamType[index];
	case CTRLCLASS:
		return (char *)gCtrlStreamType[index];
	default:
		break;
	}
	return (char *)gSystemStreamType[UNKNOWNSTREAM];
}

int CTypeProp::getLinewidth(int width, int format, int unit)
{
	int len;

	format &= (1 << CLASSSHIFT) - 1;
	len = this->getVwc(width, 1, format, unit);
	switch(format & FORMATMASK)
	{
	case YUV422BLK:
	case YUV411BLK:
	case YUV400BLK:
	case YUV444BLK:
	case YUV422LBUF:
	case YUV411LBUF:
	case YUV400LBUF:
	case YUV444LBUF:
	case JPEG422:
	case JPEG411:
	case JPEG400:
	case JPEG444:
		len <<= 3;
		break;

	case YUV422M2BLK:
	case YUV444M2BLK:
	case YUV420BLK:
	case YUV420LBUF:
	case MPEG1I_FRM:
	case MPEG2IFRM420:
	case MPEG2IFRM422:
	case MPEG2IFRM444:
	case JPEG420:
	case MPEG4_FRM:
		len <<= 4;
		break;

	default:
		break;
	}
	return len;
}
