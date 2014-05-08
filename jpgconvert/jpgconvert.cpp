//#include "../toolkit/toolgeneral/generaltools.h"
#include "toolconvert/generaltools.h"
#include "jpgconvert.h"

int JpegEncoder(char *src, char *dst, int width, int height, int qf, int format)
{
	CJpegEncoder enc;
	int len;

	enc.SetJpegQf(qf);
	enc.SetJpegSize(width, height);
	enc.SetYuvFmt(format);
	enc.Open();
	len = enc.getVwc();
	enc.Write(src, len);
	len = enc.Read(dst, len);
	return len;
}

int JpegDecoder(char *src, int len, char *dst)
{
	CJpegDecoder tools;
	TJpegIndex info;

	tools.JpegParse((unsigned char*)src, len, &info);
	if(info.YUVType == JPEG_UNKNOWN)
		return 0;
	tools.Open(&info);
	tools.Write(src, len);
	return tools.Read(dst, info.vwc << 2);
}
