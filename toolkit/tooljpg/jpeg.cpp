

#include "internal.h"

int CJpeg::Jpeg2Blk(char *src, int len, char *dest)
{
	CJpegDecoder tools;
	TJpegIndex info;

	tools.JpegParse((unsigned char*)src, len, &info);
	if(info.YUVType == JPEG_UNKNOWN)
		return 0;
	tools.Open(&info);
//	tools.PrintJpegIndex(&info, "jpeginfo.txt");
	tools.Write(src, len);
	return tools.Read(dest, info.vwc << 2);
}
