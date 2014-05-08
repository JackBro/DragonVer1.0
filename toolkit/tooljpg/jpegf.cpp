

#include "internal.h"

int CJpegF::CheckJpegList(char *list)
{
	FILE *fp = NULL;
	char str[0x100];

	fp = fopen(list, "rb");
	if(fp == NULL)
		return JPEGF_FAILED;
	fscanf(fp, "%s\n", str);
	this->m_dbug.PrintMsg(str);
	fscanf(fp, "%s\n", str);
	this->m_dbug.PrintMsg(str);
	fclose(fp);

	return JPEGF_SUCCEED;
}

void CJpegF::SplitJpeg(char *srcf, char *list, char *dstf)
{
	CFileBin fb, fblist;
	CBufferBase pool;
	TJpegIndex info;
	char *pbuf;
	char str[0x100];
	int size, pos = 0;
	CStringTools tools;
	CJpegDecoder tjpg;

	fb.Open(srcf);
	size = fb.GetFileLength();
	pool.SetSize(size << 1);
	if(pool.GetSize() == 0)
		return;
	pbuf = pool.m_buf;
	fblist.Open(list, "wb");
	sprintf(str, "file size is 0x%x\nstart pos \t end pos \t sop\n", size);
	sprintf(str, "%s==============================================\n", str);
	fblist.Write(str, (int)strlen(str));
	fb.Read(pool.m_buf, size);
	do {
		tjpg.JpegParse((unsigned char*)pbuf, size, &info);
		if(info.YUVType != JPEG_UNKNOWN)
		{
			sprintf(str, "0x%06x \t 0x%06x \t 0x%06x\n", pos + info.sop, pos + info.eop, info.sop);
			fblist.Write(str, (int)strlen(str));
			if(dstf != NULL)
			{
				fb.Open(dstf, "wb");
				fb.Write(pbuf + info.sop, info.eop - info.sop);
				tools.AutoIndex2Str(dstf, FILE_TAIL_JPG, dstf);
			}
		}
		pos += info.eop;
		pbuf += info.eop;
		size -= info.eop;
	} while(size > 0);
}

void CJpegF::JpegParse(char *filename, PTJpegIndex pinfo)
{
	CFileBin fn;
	CBufferBase pool;
	int len;
	CJpegDecoder tools;

	fn.Open(filename);
	len = fn.GetFileLength();
	pool.SetSize(len);
	fn.Read(pool.m_buf, len);
	tools.JpegParse((unsigned char*)pool.m_buf, len, pinfo);
}

void CJpegF::Jpeg2Blk(char *src, char *dest)
{
	CFileBin fsrc, fdst;
	CBufferBase srcbuf, dstbuf;
	int size;
	TJpegIndex info;

	fsrc.Open(src);
	size = fsrc.GetFileLength();
	srcbuf.SetSize(size);
	this->JpegParse(src, &info);
	dstbuf.SetSize(info.vwc << 2);
	if( (srcbuf.GetTotalSize() == 0) && (dstbuf.GetTotalSize() == 0) )
	{
		this->m_dbug.PrintMsg("Can not allocate so big buffer\n");
		return;
	}
	fsrc.Read(srcbuf.m_buf, size);
	size = CJpeg::Jpeg2Blk(srcbuf.m_buf, size, dstbuf.m_buf);
	fdst.Open(dest, "wb");
	fdst.Write(dstbuf.m_buf, size);
}

