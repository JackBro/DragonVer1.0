// TestJpeg.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "../jpgconvert/jpgconvert.h"
#include "toolconvert/generaltools.h"

int _tmain(int argc, _TCHAR* argv[])
{
	CFileBin fsrc, fdst;
	CBufferBase srcbuf, dstbuf;
	int size;
	TJpegIndex info;
	CJpeg tools;
	CJpegF tdec;

	argc = argv[0][0];
	fsrc.Open("../output/girfl.jpg");
	size = fsrc.GetFileLength();
	srcbuf.SetSize(size);
	tdec.JpegParse("../output/girfl.jpg", &info);
	dstbuf.SetSize(info.vwc << 2);
	if( (srcbuf.GetTotalSize() == 0) && (dstbuf.GetTotalSize() == 0) )
		return 0;
	fsrc.Read(srcbuf.m_buf, size);
	size = tools.Jpeg2Blk(srcbuf.m_buf, size, dstbuf.m_buf);
	fdst.Open("../output/girfl.blk", "wb");
	fdst.Write(dstbuf.m_buf, size);
	return 0;
}

