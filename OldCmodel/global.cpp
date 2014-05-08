

#include "global.h"
#include "dbugPrint.h"


//==================== class ImageBase ===================

CImageBase::CImageBase(int size)
{
	this->ClearInfo();
	if(this->InitStream(size))
	{
		this->gState = 1;
		this->gSize = size;
	}
}

CImageBase::~CImageBase(void)
{
	this->ReleaseStream();
}

void CImageBase::Clear(void)
{
	if(this->gState == 0)
		return;
	this->width = 0;
	this->height = 0;
	this->vs = 0;
	this->format = 0;
	memset(this->buf8, 0, this->gSize);
}

int CImageBase::IsValid(void)
{
	return this->gState;
}

int CImageBase::InitStream(int size)
{
	this->ReleaseStream();
	
	this->buf8 = new UINT8[size];
	if(this->buf8 == NULL)
		return FALSE;
	return TRUE;
}

void CImageBase::ClearInfo(void)
{
	this->buf8 = NULL;
	this->width = 0;
	this->height = 0;
	this->vs = 0;
	this->format = 0;
	this->gSize = 0;
	this->gState = FALSE;
}

void CImageBase::ReleaseStream(void)
{
	if(this->buf8)
		delete []this->buf8;
	this->ClearInfo();
}


//==================== class ImageStream ===================

ImageStream::ImageStream(int size) : CImageBase(size * 2)
{
}


//==================== class ImageBlock ===================

ImageBlock::ImageBlock(int size) : CImageBase(size * 16)
{
}



//=============== class CImageTools =================

void CImageTools::yuv422Torgb(pYuv422Type yuv, pRawRgbType rgb)
{
	int y0, y1, u, v, r, g, b;

	y0 = (int)(yuv[0].y) & 0xff;
	y1 = (int)(yuv[1].y) & 0xff;
	u = (int)(yuv[0].uv) & 0xff;
	v = (int)(yuv[1].uv) & 0xff;

	r = (int)(y0 + 1.4021 * v + 0.5);
	r = r > 255 ? 255 : (r > 0 ? r : 0);
	g = (int)(y0 - 0.3441 * u - 0.7142 * v + 0.5 );
	g = g > 255 ? 255 : (g > 0 ? g : 0);
	b = (int)(y0 + 1.7718 * u + 0.5 );
	b = b > 255 ? 255 : (b > 0 ? b : 0);

	rgb[0].r = (SINT16)r;
	rgb[0].g = (SINT16)g;
	rgb[0].b = (SINT16)b;

	r = (int)(y1 + 1.4021 * v + 0.5);
	r = r > 255 ? 255 : (r > 0 ? r : 0);
	g = (int)(y1 - 0.3441 * u - 0.7142 * v + 0.5 );
	g = g > 255 ? 255 : (g > 0 ? g : 0);
	b = (int)(y1 + 1.7718 * u + 0.5 );
	b = b > 255 ? 255 : (b > 0 ? b : 0);

	rgb[1].r = (SINT16)r;
	rgb[1].g = (SINT16)g;
	rgb[1].b = (SINT16)b;
}

void CImageTools::rgbToyuv422(pRawRgbType rgb, pYuv422Type yuv)
{
	int r, g, b, y, u, v;

	r = (int)(rgb[0].r);
	g = (int)(rgb[0].g);
	b = (int)(rgb[0].b);
	y = (77 * r + 150 * g + 29 * b) >> 8;
	u = ((128 * b - 43 * r - 85 * g) >> 8) + 128;
//	v = ((128 * r - 107 * g - 21 * b) >> 8) + 128;
//	yuv[0].y = (UINT8)y;
//	yuv[0].uv = (UINT8)u;
	yuv[0].y = (UINT8)(y < 0 ? 0 : ((y > 255) ? 255 : y));
	yuv[0].uv = (UINT8)(u < 0 ? 0 : ((u > 255) ? 255 : u));

	r = (int)(rgb[1].r);
	g = (int)(rgb[1].g);
	b = (int)(rgb[1].b);
	y = (77 * r + 150 * g + 29 * b) >> 8;
//	u = ((128 * b - 43 * r - 85 * g) >> 8) + 128;
	v = ((128 * r - 107 * g - 21 * b) >> 8) + 128;
//	yuv[1].y = (UINT8)y;
//	yuv[1].uv = (UINT8)v;
	yuv[1].y = (UINT8)(y < 0 ? 0 : ((y > 255) ? 255 : y));
	yuv[1].uv = (UINT8)(v < 0 ? 0 : ((v > 255) ? 255 : v));
};

int CImageTools::MaxDivisor(int a, int b)
{
	while(a && b)
	{
		if(a > b)
			a %= b;
		else
			b %= a;
	}
	if(a)
		return a;
	return b;
}

void CImageTools::AdjustFactor(int *up, int *down)
{
	int div;

	if(*up == 0)
		*up = 1;
	if(*down == 0)
		*down = 1;
	if(*down > *up)
		*down = *up;

	div = this->MaxDivisor(*up, *down);
	*up /= div;
	*down /= div;
}

int CImageTools::RfromYUV(int y, int u, int v)
{
	int r;

//	y -= 128;
	u -= 128;
	v -= 128;
//	r =  y + ((359 * v) >> 8) - 179;
	r =  (int)( y + 1.4021 * v + 0.5 );
	r = r > 255 ? 255 : (r > 0 ? r : 0);

	return r;
}

int CImageTools::GfromYUV(int y, int u, int v)
{
	int g;

//	y -= 128;
	u -= 128;
	v -= 128;
//	g =  y + 135 - ((88 * u + 183 * v) >> 8);
	g =  (int)( y - 0.3441 * u - 0.7142 * v + 0.5 );
	g = g > 255 ? 255 : (g > 0 ? g : 0);

	return g;
}

int CImageTools::BfromYUV(int y, int u, int v)
{
	int b;

//	y -= 128;
	u -= 128;
	v -= 128;
//	b =  y + ((454 * u) >> 8) - 227;
	b =  (int)( y + 1.7718 * u + 0.5 );
	b = b > 255 ? 255 : (b > 0 ? b : 0);

	return b;
}


//===================== CmodelTools Class ========================

CmodelTools::CmodelTools(void)
{
	this->gSrc = NULL;
	this->gDest = NULL;
	this->gState = 0;
	this->InitTools();
}

CmodelTools::~CmodelTools(void)
{
	this->ReleaseTools();
}

void CmodelTools::InitTools(void)
{
	this->ReleaseTools();
	this->gDest = new short int[TOOLS_BUFSIZE];
	this->gSrc = new short int[TOOLS_BUFSIZE];

//	this->gDest = (short int *)malloc(TOOLS_BUFSIZE * sizeof(short int));
//	this->gSrc = (short int *)malloc(TOOLS_BUFSIZE * sizeof(short int));
	if(this->gSrc && this->gDest)
		this->gState = 1;
}

void CmodelTools::ReleaseTools(void)
{
	if(this->gSrc)
		delete [](this->gSrc);
//		free(this->gSrc);
	if(this->gDest)
//		free(this->gDest);
		delete [](this->gDest);
	this->gSrc = NULL;
	this->gDest = NULL;
	this->gState = 0;
}

void CmodelTools::hex2bin(char *src, char *dest)
{
	FILE *fp1, *fp2;
	int len;
	int flag = 1;

	if(this->gState == 0)
	{
		Gdbug.DbugMsg("Tools: Tools is not initialized\n");
		return;
	}

	fp1 = fopen(src, "rb");
	if(NULL == fp1)
	{
		Gdbug.DbugMsg("Tools: Cant open source file to read\n");
		return;
	}
	fp2 = fopen(dest, "wb");
	if(NULL == fp2)
	{
		Gdbug.DbugMsg("Tools: Cant open data file to write\n");
		fclose(fp1);
		return;
	}

	while(flag == 1)
	{
		len = 0;
		while(len < (TOOLS_BUFSIZE >> 1))
		{
			if(fscanf(fp1, "%x", &(this->gSrc[len])) == EOF)
			{
				flag = 0;
				break;
			}
			len++;
		}
		fwrite(this->gSrc, sizeof(short int), len, fp2);
	}
	fclose(fp1);
	fclose(fp2);
}

void CmodelTools::bin2Tohex(char *src, char *dest)
{
	FILE *fp1, *fp2;
	int len, i;
	int flag = 1;

	if(this->gState == 0)
	{
		Gdbug.DbugMsg("Tools: Tools is not initialized\n");
		return;
	}

	fp1 = fopen(src, "rb");
	if(NULL == fp1)
	{
		Gdbug.DbugMsg("Tools: Cant open source file to read\n");
		return;
	}
	fp2 = fopen(dest, "wb");
	if(NULL == fp2)
	{
		Gdbug.DbugMsg("Tools: Cant open data file to write\n");
		fclose(fp1);
		return;
	}

	while(flag == 1)
	{
		len = (int)fread(this->gSrc, sizeof(short int), TOOLS_BUFSIZE, fp1);
		if(len < TOOLS_BUFSIZE)
		{
			flag = 0;
		}
		for(i = 0; i < len; i++)
			fprintf(fp2, "%x\n", this->gSrc[i] & 0xffff);
	}
	fclose(fp1);
	fclose(fp2);
}

void CmodelTools::bin1Tohex(char *src, char *dest)
{
	FILE *fp1, *fp2;
	int len, i;
	char *buf;
	int flag = 1;

	if(this->gState == 0)
	{
		Gdbug.DbugMsg("Tools: Tools is not initialized\n");
		return;
	}

	fp1 = fopen(src, "rb");
	if(NULL == fp1)
	{
		Gdbug.DbugMsg("Tools: Cant open source file to read\n");
		return;
	}
	fp2 = fopen(dest, "wb");
	if(NULL == fp2)
	{
		Gdbug.DbugMsg("Tools: Cant open data file to write\n");
		fclose(fp1);
		return;
	}

	buf = (char *)(this->gSrc);
	while(flag == 1)
	{
		len = (int)fread(this->gSrc, sizeof(short int), TOOLS_BUFSIZE, fp1);
		if(len < TOOLS_BUFSIZE)
		{
			flag = 0;
		}
		for(i = 0; i < len << 1; i++)
			fprintf(fp2, "%02x\n", buf[i] & 0xff);
	}
	fclose(fp1);
	fclose(fp2);
}

int CmodelTools::bincompare(char *src, char *dest)
{
	FILE *fp1, *fp2;
	int i, len1, len2, len, flag = 1;

	if(this->gState == 0)
	{
		Gdbug.DbugMsg("Tools: Tools is not initialized\n");
		return flag;		//invalid return value
	}
	fp1 = fopen(src, "rb");
	if(NULL == fp1)
	{
		Gdbug.DbugMsg("cant open source file to read\n");
		return flag;
	}
	fp2 = fopen(dest, "rb");
	if(NULL == fp2)
	{
		Gdbug.DbugMsg("cant open destinate file to read\n");
		fclose(fp1);
		return flag;
	}

	while(flag == 1)
	{
		len1 = (int)fread(this->gSrc, sizeof(short int), TOOLS_BUFSIZE, fp1);
		len2 = (int)fread(this->gDest, sizeof(short int), TOOLS_BUFSIZE, fp2);

		len = len1;
		if(len1 < len2)
		{
			Gdbug.DbugMsg("Tools: file %s size < file %s size\n", src, dest);
			flag = 2;		//size not equal
		}
		if(len1 > len2)
		{
			Gdbug.DbugMsg("Tools: file %s size > file %s size\n", src, dest);
			flag = 2;
			len = len2;
		}
		if(len1 < TOOLS_BUFSIZE)
		{
			if(len1 == len2)
				flag = 0;		//size equal
		}

		for(i = 0; i < len; i++)
		{
			if(this->gSrc[i] != this->gDest[i])
			{
				Gdbug.DbugMsg("Tools: content of two file is not equal at 0x%x!\n", i);
				flag = 3;		//content not equal
				break;
			}
		}
	}
	fclose(fp1);
	fclose(fp2);

	return flag;
}

void CmodelTools::binsizer(char *src, char *dest, int srcwidth, 
							 int hstart, int vstart, int width, int height)
{
	FILE *fp1, *fp2;
	int i, length, h, v, len, flag = 1;

	if(this->gState == 0)
	{
		Gdbug.DbugMsg("Tools: Tools is not initialized\n");
		return;		//invalid return value
	}
	fp1 = fopen(src, "rb");
	if(NULL == fp1)
	{
		Gdbug.DbugMsg("cant open source file to read\n");
		return;
	}
	fp2 = fopen(dest, "wb");
	if(NULL == fp2)
	{
		Gdbug.DbugMsg("cant open destinate file to write\n");
		fclose(fp1);
		return;
	}

	h = 0;
	v = 0;
	while(flag == 1)
	{
		length = 0;
		len = (int)fread(this->gSrc, sizeof(short int), TOOLS_BUFSIZE, fp1);
		if(len < TOOLS_BUFSIZE)
		{
			flag = 0;
		}
		for(i = 0; i < len; i++, h++)
		{
			if(h == srcwidth)
			{
				h = 0;
				v++;
			}
			if( (h >= hstart) && (h < hstart+width) && (v >= vstart) && (v < vstart+height) )
			{
				this->gDest[length++] = this->gSrc[i];
			}
		}
		fwrite(this->gDest, sizeof(short int), length, fp2);
	}
	fclose(fp1);
	fclose(fp2);
}

void CmodelTools::bin1bytediv(char *src, char *dest, char div)
{
	FILE *fp1, *fp2;
	int i, len, flag = 1;
	int val;
	char *p = (char *)this->gDest;

	if(this->gState == 0)
	{
		Gdbug.DbugMsg("Tools: Tools is not initialized\n");
		return;		//invalid return value
	}
	if(div == 0)
	{
		Gdbug.DbugMsg("Tools: it is not allow that div equal to zero\n");
		return;
	}
	fp1 = fopen(src, "rb");
	if(NULL == fp1)
	{
		Gdbug.DbugMsg("cant open source file to read\n");
		return;
	}
	fp2 = fopen(dest, "wb");
	if(NULL == fp2)
	{
		Gdbug.DbugMsg("cant open destinate file to write\n");
		fclose(fp1);
		return;
	}

	while(flag == 1)
	{
		len = (int)fread(this->gSrc, sizeof(short int), TOOLS_BUFSIZE, fp1);
		if(len < TOOLS_BUFSIZE)
		{
			flag = 0;
		}
		for(i = 0; i < len; i++)
		{
			val = this->gSrc[i] / div;
			val = (val < 0) ? 0 : ((val > 255) ? 255 : val);
			*(p+i) = (char)val;
		}
		fwrite(p, sizeof(char), len, fp2);
	}
	fclose(fp1);
	fclose(fp2);
}

void CmodelTools::binadd(char *src, char *dest, int val)
{
	FILE *fp1, *fp2;
	int i, len, flag = 1;
	char *p = (char *)this->gDest;

	if(this->gState == 0)
	{
		Gdbug.DbugMsg("Tools: Tools is not initialized\n");
		return;		//invalid return value
	}
	fp1 = fopen(src, "rb");
	if(NULL == fp1)
	{
		Gdbug.DbugMsg("cant open source file to read\n");
		return;
	}
	fp2 = fopen(dest, "wb");
	if(NULL == fp2)
	{
		Gdbug.DbugMsg("cant open destinate file to write\n");
		fclose(fp1);
		return;
	}

	while(flag == 1)
	{
		len = (int)fread(this->gDest, sizeof(short int), TOOLS_BUFSIZE, fp1);
		if(len < TOOLS_BUFSIZE)
		{
			flag = 0;
		}
		for(i = 0; i < (len << 1); i++)
		{
			val += (int)(*(p+i));
			*(p+i) = (char)val;
		}
		fwrite(this->gDest, sizeof(short int), len, fp2);
	}
	fclose(fp1);
	fclose(fp2);
}

void CmodelTools::bin1byte(char *src, char *dest)
{
	FILE *fp1, *fp2;
	int i, len, flag = 1;
	int val;
	char *p = (char *)this->gDest;

	if(this->gState == 0)
	{
		Gdbug.DbugMsg("Tools: Tools is not initialized\n");
		return;		//invalid return value
	}
	fp1 = fopen(src, "rb");
	if(NULL == fp1)
	{
		Gdbug.DbugMsg("cant open source file to read\n");
		return;
	}
	fp2 = fopen(dest, "wb");
	if(NULL == fp2)
	{
		Gdbug.DbugMsg("cant open destinate file to write\n");
		fclose(fp1);
		return;
	}

	while(flag == 1)
	{
		len = (int)fread(this->gSrc, sizeof(short int), TOOLS_BUFSIZE, fp1);
		if(len < TOOLS_BUFSIZE)
		{
			flag = 0;
		}
		for(i = 0; i < len; i++)
		{
			val = this->gSrc[i];
			val = (val < 0) ? 0 : ((val > 255) ? 255 : val);
			*(p+i) = (char)(val);
		}
		fwrite(p, sizeof(char), len, fp2);
	}
	fclose(fp1);
	fclose(fp2);
}

void CmodelTools::addbmpheader(char *src, char *dest, int width, int height)
{
	FILE *fp1, *fp2;
	int len, size, n, unit, flag = 1;
	char dat[3] = {0, 0, 0};
	BITMAPFILEHEADER bmp = {	0x4d42, 0x38436, 0, 0, 0x36	};
	BITMAPINFOHEADER info = {	0x28, 0x140, 0xf0, 0x1, 0x18, 0, 0x38400, 0, 0, 0, 0 };

	if(this->gState == 0)
	{
		Gdbug.DbugMsg("Tools: Tools is not initialized\n");
		return;		//invalid return value
	}
	fp1 = fopen(src, "rb");
	if(NULL == fp1)
	{
		Gdbug.DbugMsg("cant open source file to read\n");
		return;
	}
	fp2 = fopen(dest, "wb");
	if(NULL == fp2)
	{
		Gdbug.DbugMsg("cant open destinate file to write\n");
		fclose(fp1);
		return;
	}

	info.biWidth = width;
	info.biHeight = height;
	info.biSizeImage = width * height * 3;
	bmp.bfSize = info.biSizeImage + 0x36;

	fwrite(&bmp, 1, sizeof(BITMAPFILEHEADER), fp2);
	fwrite(&info, 1, sizeof(BITMAPINFOHEADER), fp2);

	unit = width * 3;
	size = (TOOLS_BUFSIZE / unit) * unit;

	while(flag == 1)
	{
		len = (int)fread(this->gSrc, 1, size, fp1);
		if(len < size)
		{
			flag = 0;
		}
		if((width % 4) == 0)
			fwrite(this->gSrc, 1, len, fp2);
		else
		{
			n = len / unit;
			while(n--)
			{
				fwrite(this->gSrc, 1, unit, fp2);
				fwrite(dat, 1, 4 - (unit%4), fp2);
			}
		}
	}
	fclose(fp1);
	fclose(fp2);
}

void CmodelTools::yuv422tobmp(char *src, char *dest)
{
	FILE *fp1, *fp2;
	int len, flag = 1;
	int length;
	char *pdest = (char *)this->gDest;
	char *psrc = (char *)this->gSrc;
	int y0, y1, u, v, i;

	if(this->gState == 0)
	{
		Gdbug.DbugMsg("Tools: Tools is not initialized\n");
		return;		//invalid return value
	}
	fp1 = fopen(src, "rb");
	if(NULL == fp1)
	{
		Gdbug.DbugMsg("cant open source file to read\n");
		return;
	}
	fp2 = fopen(dest, "wb");
	if(NULL == fp2)
	{
		Gdbug.DbugMsg("cant open destinate file to write\n");
		fclose(fp1);
		return;
	}

	while(flag == 1)
	{
		length = 0;
		len = (int)fread(this->gSrc, sizeof(short int), (TOOLS_BUFSIZE/3) << 1, fp1);
		if(len < (TOOLS_BUFSIZE/3) << 1)
		{
			flag = 0;
		}
		for(i = 0; i < len << 1; i += 4)
		{
			y0 = psrc[i+1] & 0xff;
			y1 = psrc[i+3] & 0xff;
			u = psrc[i+0] & 0xff;
			v = psrc[i+2] & 0xff;

			pdest[length++] = (char)this->BfromYUV(y0, u, v);
			pdest[length++] = (char)this->GfromYUV(y0, u, v);
			pdest[length++] = (char)this->RfromYUV(y0, u, v);
			pdest[length++] = (char)this->BfromYUV(y1, u, v);
			pdest[length++] = (char)this->GfromYUV(y1, u, v);
			pdest[length++] = (char)this->RfromYUV(y1, u, v);
		}
		fwrite(pdest, 1, length, fp2);
	}
	fclose(fp1);
	fclose(fp2);
}

void CmodelTools::block422tobmp(char *src, char *dest, int width)
{
	FILE *fp1, *fp2;
	int len, size, flag = 1;
	int adr, height, pos, adr1;
	char *pdest = (char *)this->gDest;
	char *psrc = (char *)this->gSrc;
	int y0, y1, u, v, i, j, k, m;

	if(this->gState == 0)
	{
		Gdbug.DbugMsg("Tools: Tools is not initialized\n");
		return;		//invalid return value
	}
	fp1 = fopen(src, "rb");
	if(NULL == fp1)
	{
		Gdbug.DbugMsg("cant open source file to read\n");
		return;
	}
	fp2 = fopen(dest, "wb");
	if(NULL == fp2)
	{
		Gdbug.DbugMsg("cant open destinate file to write\n");
		fclose(fp1);
		return;
	}

	pos = width << 4;
	size = TOOLS_BUFSIZE / pos;
	size *= pos;
	while(flag == 1)
	{
		len = (int)fread(psrc, sizeof(char), size, fp1);
		if(len < size)
		{
			flag = 0;
		}
		height = (len / width) >> 1;
		adr = 0;
		for(i = 0; i < height; i += 8)
		{
			for(j = 0; j < width; j += 16, adr += 256)
			{
				for(pos = adr, k = 0; k < 8; k++, pos += 8)
				{
					for(m = 0; m < 4; m++)
					{
						y0 = (int)psrc[pos + (m<<1)] & 0xff;
						y1 = (int)psrc[pos + (m<<1)+1] & 0xff;
						u = (int)psrc[pos + 128+m] & 0xff;
						v = (int)psrc[pos + 192+m] & 0xff;
						adr1 = (i+k) * width*3 + (j + m*2)*3;

						pdest[adr1 + 0] = (char)this->BfromYUV(y0, u, v);
						pdest[adr1 + 1] = (char)this->GfromYUV(y0, u, v);
						pdest[adr1 + 2] = (char)this->RfromYUV(y0, u, v);

						pdest[adr1 + 3] = (char)this->BfromYUV(y1, u, v);
						pdest[adr1 + 4] = (char)this->GfromYUV(y1, u, v);
						pdest[adr1 + 5] = (char)this->RfromYUV(y1, u, v);

						y0 = (int)psrc[pos + (m<<1)+64] & 0xff;
						y1 = (int)psrc[pos + (m<<1)+64+1] & 0xff;
						u = (int)psrc[pos + 128+4+m] & 0xff;
						v = (int)psrc[pos + 192+4+m] & 0xff;
						adr1 += 24;
						
						pdest[adr1 + 0] = (char)this->BfromYUV(y0, u, v);
						pdest[adr1 + 1] = (char)this->GfromYUV(y0, u, v);
						pdest[adr1 + 2] = (char)this->RfromYUV(y0, u, v);

						pdest[adr1 + 3] = (char)this->BfromYUV(y1, u, v);
						pdest[adr1 + 4] = (char)this->GfromYUV(y1, u, v);
						pdest[adr1 + 5] = (char)this->RfromYUV(y1, u, v);
					}
				}
			}
		}

		fwrite(pdest, 1, (len * 3) >> 1, fp2);
	}
	fclose(fp1);
	fclose(fp2);
}

void CmodelTools::bmp2rawdata(char *src, char *dest, int width)
{
	FILE *fp1, *fp2;
	int len, size, flag = 1;
	int gfirst, bgline, height;
	int i, j, pos, adr;
	int offset, gpixel;
	char *pdest = (char *)this->gDest;
	char *psrc = (char *)this->gSrc;
	BITMAPINFOHEADER hdr;

	if(this->gState == 0)
	{
		Gdbug.DbugMsg("Tools: Tools is not initialized\n");
		return;		//invalid return value
	}
	fp1 = fopen(src, "rb");
	if(NULL == fp1)
	{
		Gdbug.DbugMsg("cant open source file to read\n");
		return;
	}
	fp2 = fopen(dest, "wb");
	if(NULL == fp2)
	{
		Gdbug.DbugMsg("cant open destinate file to write\n");
		fclose(fp1);
		return;
	}

	fseek(fp1, sizeof(BITMAPFILEHEADER), SEEK_SET);
	fread(&hdr, 1, sizeof(BITMAPINFOHEADER), fp1);
	switch(hdr.biBitCount)
	{
	case 0x18:
		size = TOOLS_BUFSIZE / (width * 3);
		size *= width * 3;
		gfirst = 0;
		bgline = 0;
		while(flag == 1)
		{
			len = (int)fread(psrc, sizeof(char), size, fp1);
			if(len < size)
			{
				flag = 0;
			}
			height = len / (width * 3);
			pos = 0;
			for(i = 0, adr = 0; i < height; i++, adr += width * 3)
			{
				gfirst = gfirst ? 0 : 1;
				bgline = bgline ? 0 : 1;
				gpixel = gfirst ? 1 : 0;
				for(j = 0; j < width * 3; j += 3, gpixel++)
				{
					offset = 1;			//G pixel
					if(bgline)
					{
						if( (gfirst + gpixel) % 2  == 0)
							offset = 0;		//B pixel
					}
					else
					{
						if( (gfirst + gpixel) % 2 == 0)
							offset = 2;		//R pixel
					}
					pdest[pos++] = psrc[adr+j+offset];
				}
			}

			fwrite(pdest, 1, pos, fp2);
		}
		break;
	case 0x8:
		//jump to real data offset
		while(flag == 1)
		{
			len = (int)fread(psrc, sizeof(int), TOOLS_BUFSIZE, fp1);
			if(len < TOOLS_BUFSIZE)
			{
				flag = 0;
			}
			fwrite(pdest, 1, len, fp2);
		}
		break;
	default:
		break;
	}
	fclose(fp1);
	fclose(fp2);
}

void CmodelTools::yuv422rotation(char *src, char *dest, int width, int height)
{
	FILE *fp;
	char *psrc = NULL, *pdst = NULL;
	int size = width * height * 2;

	psrc = new char[size];
	pdst = new char[size];
	if( (psrc == NULL) || (pdst == NULL) )
	{
		Gdbug.DbugMsg("cant alloc memory");
		if(pdst)
			delete []pdst;
		if(psrc)
			delete []psrc;
		return;
	}
	fp = fopen(src, "rb");
	if(NULL == fp)
	{
		Gdbug.DbugMsg("cant open file %s\n", src);
		if(pdst)
			delete []pdst;
		if(psrc)
			delete []psrc;
		return;
	}
	fread(psrc, size, sizeof(char), fp);
	fclose(fp);

	fp = fopen(dest, "wb");
	if(NULL == fp)
	{
		Gdbug.DbugMsg("cant open file %s\n", dest);
		if(pdst)
			delete []pdst;
		if(psrc)
			delete []psrc;
		return;
	}
	this->rotate90( (Yuv422Type *)psrc, (Yuv422Type *)pdst, width, height );
	fwrite(pdst, size, sizeof(char), fp);
	fclose(fp);

	if(pdst)
		delete []pdst;
	if(psrc)
		delete []psrc;
}

void CmodelTools::rotate90(Yuv422Type *src, Yuv422Type *dest, int width, int height)
{
	int i, j, srcadr = 0, dstadr;

	for(i = 0; i < height; i++)
	{
		for(j = 0, dstadr = (width-1) * height + i; j < width; j++, srcadr++, dstadr -= height)
		{
			//dstadr = (width-j) * height + i;
			dest[dstadr].y = src[srcadr].y;
			if(i % 2) //pdst.uv = v
			{
				dest[dstadr].uv = src[srcadr+1].uv;
				if(j % 2)
					dest[dstadr].uv = src[srcadr].uv;
			}
			else	//pdst.uv = u
			{
				dest[dstadr].uv = src[srcadr].uv;
				if(j % 2)
					dest[dstadr].uv = src[srcadr-1].uv;
			}
		}
	}
}


