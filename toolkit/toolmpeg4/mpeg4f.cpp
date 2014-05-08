
#include "internal.h"

void CMpeg4F::Mpeg4Parse(char *src, PTMpeg4Option pinfo)
{
	CFileBin fn;
	CBufferBase pool;
	int len;
	CMpeg4Decoder tools;

	fn.Open(src);
	len = fn.GetFileLength();
	if(len > 0x100)
		len = 0x100;
	fn.Read(pool.m_buf, len);
	tools.Open();
	tools.Write(pool.m_buf, len);
	tools.Mpeg4Parse(pinfo);
}

void CMpeg4F::Mpeg4Blk(char *src, char *dest)
{
	CFileBin fsrc, fdst;
	CBufferBase srcbuf, dstbuf;
	int size;
	TMpeg4Option info;

	fsrc.Open(src);
	size = fsrc.GetFileLength();
	srcbuf.SetSize(size);
	this->Mpeg4Parse(src, &info);

	dstbuf.SetSize(info.vwc);
	if( (srcbuf.GetTotalSize() == 0) && (dstbuf.GetTotalSize() == 0) )
	{
		this->m_dbug.PrintMsg("Can not allocate so big buffer\n");
		return;
	}
	fsrc.Read(srcbuf.m_buf, size);
	size = CMpeg4::Mpeg4Blk(srcbuf.m_buf, size, dstbuf.m_buf, size);
	fdst.Open(dest, "wb");
	fdst.Write(dstbuf.m_buf, size);
}

