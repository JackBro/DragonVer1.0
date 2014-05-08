

#include "internal.h"

int CMpeg2I::Mpeg2Blk(char *src, int len, char *dest, int dstlen)
{
	CMpeg2Decoder tools;

	tools.Open();
	tools.Write(src, len);
	return tools.Read(dest, dstlen, MPEG_DECFLUSH);
}
