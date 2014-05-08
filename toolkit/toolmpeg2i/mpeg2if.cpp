

#include "internal.h"

void CMpeg2IF::Mpeg2Parse(char *src, PTMpeg2Option pinfo)
{
	CFileBin fn;
	CBufferBase pool;
	int len;
	CMpeg2Decoder tools;

	fn.Open(src);
	len = fn.GetFileLength();
	if(len > 0x100)
		len = 0x100;
	fn.Read(pool.m_buf, len);
	tools.Open();
	tools.Write(pool.m_buf, len);
	tools.Mpeg2Parse(pinfo);
}

void CMpeg2IF::Mpeg2Blk(char *src, char *dest)
{
	CFileBin fsrc, fdst;
	CBufferBase srcbuf, dstbuf;
	int size;
	TMpeg2Option info;

	fsrc.Open(src);
	size = fsrc.GetFileLength();
	srcbuf.SetSize(size);
	this->Mpeg2Parse(src, &info);

	dstbuf.SetSize(info.vwc);
	if( (srcbuf.GetTotalSize() == 0) && (dstbuf.GetTotalSize() == 0) )
	{
		this->m_dbug.PrintMsg("Can not allocate so big buffer\n");
		return;
	}
	fsrc.Read(srcbuf.m_buf, size);
	size = CMpeg2I::Mpeg2Blk(srcbuf.m_buf, size, dstbuf.m_buf, info.vwc);
	fdst.Open(dest, "wb");
	fdst.Write(dstbuf.m_buf, size);
}

