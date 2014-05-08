

#include "internal.h"

int CMpeg4::Mpeg4Blk(char *src, int len, char *dest, int dstlen)
{
	CMpeg4Decoder tools;

	tools.Open();
	tools.Write(src, len);
	return tools.Read(dest, dstlen, MPEG4_DECFLUSH);
}