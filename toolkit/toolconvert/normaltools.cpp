

#include "internal.h"

void CNormalTools::ConvertHeader2RegList(char *src, char *dest)
{
	FILE *fpsrc = NULL, *fpdst = NULL;
	char str[256];

	if( (src == NULL) || (dest == NULL) )
	{
		this->m_dbug.PrintMsg("src or dest filename is NULL\n");
		return;
	}
	if( (src[0] == 0) || (dest[0] == 0) )
	{
		this->m_dbug.PrintMsg("src or dest filename is Zero\n");
		return;
	}
	fpsrc = fopen(src, "rt");
	if(NULL == fpsrc)
	{
		this->m_dbug.PrintMsg("Can't open source file\n");
		return;
	}
	fpdst = fopen(dest, "wt");
	if(NULL == fpdst)
	{
		this->m_dbug.PrintMsg("Can't open destination file\n");
		fclose(fpsrc);
		return;
	}
	while(fscanf(fpsrc, "%s", str) != EOF)
	{
		if(strcmp(str, "#define") == 0)
		{
			if(fscanf(fpsrc, "%s", str) != EOF)
				fprintf(fpdst, "\t{ V558_REGTYPE, \"%s\", %s, 0, 0 },\n", str, str); 
		}
	}
	fclose(fpsrc);
	fclose(fpdst);
}

void CNormalTools::Hex2Bin(char *src, char *dest, int unit)
{
	FILE *fpsrc = NULL;
	char str[0x100];
	CFileBin fdst;
	int val;

	if(unit == 0)
	{
		this->m_dbug.PrintMsg("error: unit = 0\n");
		return;
	}
	if( (src == NULL) || (dest == NULL) )
		return;
	if( (src[0] == 0) || (dest[0] == 0) )
		return;
	fpsrc = fopen(src, "rb");
	if(NULL == fpsrc)
	{
		this->m_dbug.PrintMsg("Can't open source file\n");
		return;
	}

	fdst.Open(dest, "wb");
	while(fscanf(fpsrc, "%s", str) != EOF)
	{
		this->Str2Hex(str, &val);
		fdst.Write((char *)&val, unit);
	}
	fclose(fpsrc);
}

void CNormalTools::Bin2Hex(char *src, char *dest, int unit)
{
	FILE *fpdst = NULL;
	CFileBin fsrc;
	int val = 0;

	if(unit == 0)
	{
		this->m_dbug.PrintMsg("error: unit = 0\n");
		return;
	}
	if( (src == NULL) || (dest == NULL) )
		return;
	if( (src[0] == 0) || (dest[0] == 0) )
		return;
	fpdst = fopen(dest, "wb");
	if(NULL == fpdst)
	{
		this->m_dbug.PrintMsg("Can't open destination file\n");
		return;
	}

	fsrc.Open(src, "rb");

	while(fsrc.Read((char *)&val, unit) != 0)
	{
		fprintf(fpdst, "%02x\n", val);
		val = 0;
	}
	fclose(fpdst);
}

void CNormalTools::Bin2Ascii(char *src, char *dest, int unit)
{
	FILE *fpdst = NULL;
	CFileBin fsrc;
	int val = 0, i;

	if(unit == 0)
	{
		this->m_dbug.PrintMsg("error: unit = 0\n");
		return;
	}
	if( (src == NULL) || (dest == NULL) )
		return;
	if( (src[0] == 0) || (dest[0] == 0) )
		return;
	fpdst = fopen(dest, "wb");
	if(NULL == fpdst)
	{
		this->m_dbug.PrintMsg("Can't open destination file\n");
		return;
	}

	fsrc.Open(src, "rb");

	while(fsrc.Read((char *)&val, unit) != 0)
	{
		for(i = (unit << 3) - 1; i >= 0; i--)
		{
			fprintf(fpdst, "%d", ((val & (1 << i)) != 0));
		}
		fprintf(fpdst, "\n");
		val = 0;
	}
	fclose(fpdst);
}

void CNormalTools::Compare(char *src, char *dest)
{
	CFileBin fsrc, fdst;
	int pos = 0, len1, len2;
	int flag = 1;
	char val1, val2;

	fsrc.Open(src);
	fdst.Open(dest);
	do {
		len1 = fsrc.Read(&val1, 1);
		len2 = fdst.Read(&val2, 1);
		if(len1 && len2)
		{
			if(val1 != val2)
			{
				this->m_dbug.PrintMsg("files not equal at pos: 0x%x\n", pos);
				return;
			}
			pos++;
		}
		else
		{
			flag = 0;
			if(len1 != len2)
			{
				this->m_dbug.PrintMsg("files' length are not same\n");
				return;
			}
		}
	} while(flag);
	this->m_dbug.PrintMsg("File1 is same as file2\n");
}

void CNormalTools::DropHighByte(char *src, char *dest, int opcode, int div, 
								int srcunit, int dstunit, int minval, int cap)
{
	CFileBin fsrc, fdst;
	int val = 0;

	if( (srcunit == 0) || (dstunit == 0) )
	{
		this->m_dbug.PrintMsg("error: src unit = 0 or dst unit = 0\n");
		return;
	}
	fsrc.Open(src);
	fdst.Open(dest, "wb");
	while(fsrc.Read((char *)&val, srcunit) != 0)
	{
		val = this->Calculate(val, div, opcode);
		val = (val > cap) ? cap : ((val < minval) ? minval : val);
		fdst.Write((char *)&val, dstunit);
		val = 0;
	}
}

void CNormalTools::DropLowByte(char *src, char *dest, int opcode, int div, 
							   int srcunit, int dstunit, int minval, int cap)
{
	CFileBin fsrc, fdst;
	int val = 0;

	if( (srcunit == 0) || (dstunit == 0) || (srcunit <= dstunit) )
	{
		this->m_dbug.PrintMsg("error: src unit = 0 or dst unit = 0 or src unit smaller than dst unit\n");
		return;
	}
	fsrc.Open(src);
	fdst.Open(dest, "wb");
	while(fsrc.Read((char *)&val, srcunit) != 0)
	{
		val = this->Calculate(val, div, opcode);
		val = (val > cap) ? cap : ((val < minval) ? minval : val);
		fdst.Write((char *)&val+(srcunit-dstunit), dstunit);
		val = 0;
	}
}

void CNormalTools::CreateOrderHexFile(char *dest, int size)
{
	CFileBin fdest;
	int i, len;
	char str[10];

	fdest.Open(dest, "wb");
	for(i = 0; i < size; i++)
	{
		sprintf(str, "0x%x\n", i);
		len = (int)strlen(str);
		fdest.Write(str, len);
	}
}

void CNormalTools::AppendFile(char *src, char *dst)
{
	CFileBin fsrc, fdst;
	CBufferBase pool;
	int size = pool.GetTotalSize();

	if(size == 0)
		return;
	fsrc.Open(src);
	fdst.Open(dst, "ab");
	while(size)
	{
		size = fsrc.Read(pool.m_buf, size);
		fdst.Write(pool.m_buf, size);
	}
}

void CNormalTools::MergeFile(char *src1, char *src2, char *dst, int src1unit, int src2unit)
{
	CFileBin fsrc1, fsrc2, fdst;
	char val[10];
	int flag = 3, len;

	if( (src1unit == 0) || (src2unit == 0) )
	{
		this->m_dbug.PrintMsg("error: src1 unit = 0 or src2 unit = 0\n");
		return;
	}
	fsrc1.Open(src1);
	fsrc2.Open(src2);
	fdst.Open(dst, "wb");
	while(flag)
	{
		if(flag & 1)
		{
			len = fsrc1.Read(val, src1unit );
			if(len)
				fdst.Write(val, len);
			else
				flag &= 2;
		}
		if(flag & 2)
		{
			len = fsrc2.Read(val, src2unit );
			if(len)
				fdst.Write(val, len);
			else
				flag &= 1;
		}
	}
}

void CNormalTools::SpanConvert(char *src, char *dst, int len, int adr, int span, 
							   int size, int opcode, int opval, int unit)
{
	CFileBin fsrc, fdst;
	CBufferBase pool;
	int length, temp;

	if( (unit > 4) || (unit <= 0) )
		unit = 1;
	fsrc.Open(src);
	length = fsrc.GetFileLength();
	temp = ((len * span) / size) + adr;
	if(length < temp)
		return;
	if(length > pool.GetTotalSize())
		pool.SetSize(length);
	fsrc.Read(pool.m_buf, length);
	CBaseConvert::DataCalSpan(pool.m_buf, len, adr, span, size, opcode, opval, unit);
	fdst.Open(dst, "wb");
	fdst.Write(pool.m_buf, length);
}

void CNormalTools::SeqConvert(char *src, char *dst, int len, int adr, int opcode, int opval, int unit)
{
	CFileBin fsrc, fdst;
	CBufferBase pool;
	int length, temp;

	if( (unit > 4) || (unit <= 0) )
		unit = 1;
	fsrc.Open(src);
	length = fsrc.GetFileLength();
	temp = len + adr;
	if(length < temp)
		return;
	if(length > pool.GetTotalSize())
		pool.SetSize(length);
	fsrc.Read(pool.m_buf, length);
	CBaseConvert::DataCal(pool.m_buf, len, adr, opcode, opval, unit);
	fdst.Open(dst, "wb");
	fdst.Write(pool.m_buf, length);
}

void CNormalTools::CompareImg(char *src1, char *src2, char *dest)
{
	CFileBin fsrc1, fsrc2, fdst;
	int len1, len2;
	int flag = 1;
	char val1, val2;

	fsrc1.Open(src1);
	fsrc2.Open(src2);
	fdst.Open(dest, "wb");
	do {
		len1 = fsrc1.Read(&val1, 1);
		len2 = fsrc2.Read(&val2, 1);
		if(len1 && len2)
		{
			val1 = val1 - val2 ? 0xff : 0;
			fdst.Write(&val1, 1);
			if(val1)
				flag = 2;
		}
		else
		{
			if(len1 != len2)
			{
				this->m_dbug.PrintMsg("files' length are not same\n");
				return;
			}
			break;
		}
	} while(flag);
	if(flag == 2)
		this->m_dbug.PrintMsg("File1 is not same as file2\n");
	else
		this->m_dbug.PrintMsg("File1 is same as file2\n");
}

double CNormalTools::Snr(char *src1, char *src2, int unit)
{
	CFileBin fsrc1, fsrc2;
	int len1, len2, val1, val2, sum[100], diff, i, j, len;
	double total;

	memset(sum, 0, sizeof(sum));
	fsrc1.Open(src1);
	fsrc2.Open(src2);
	i = 0;
	len = 0;
	do {
		len1 = fsrc1.Read((char *)&val1, unit);
		len2 = fsrc2.Read((char *)&val2, unit);
		if( (len1 == 0) || (len2 == 0) )
			break;
		diff = (val1 - val2) * (val1 - val2);
		sum[i] += diff;
		len++;
		if(sum[i] > 0x7f000000)
		{
			if(i == 99)
				break;
			else
				i++;
		}
	} while(len1);

	total = 0;
	for(j = 0; j <= i; j++)
		total += sum[i];
	if(total == 0.0)
		return 0;
	total /= len-1;
	total = 255.0 / sqrt(total);
	total = 20 * log10(total);
	return total;
}



