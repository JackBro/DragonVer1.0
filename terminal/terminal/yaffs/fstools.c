
#include "internal.h"


void File2pc(char *src, char *buf)
{
	int len;

#ifdef WIN32
	len = buf[0];
#endif
	len = LoadImageFile(src, gTransferBuf, 0x80000);
	tmWriteBuf((char *)&len, sizeof(len));
	tmWriteBuf(gTransferBuf, len);
}

#ifdef WIN32

char gCurDir[100] = { 0 };

void CopyCurPath(char *str)
{
	strcpy(gCurDir, str);
}

void SaveImageFile(char *str, char *buf, int len)
{
	FILE *fp = NULL;
	char str1[100];

	sprintf(str1, "%s%s", gCurDir, str);
	fp = fopen(str1, "wb");
	if(fp == NULL)
		return;
	fwrite(buf, 1, len, fp);
	fclose(fp);
}

int LoadImageFile(char *str, char *buf, int size)
{
	FILE *fp = NULL;
	char str1[100];

	sprintf(str1, "%s%s", gCurDir, str);
	fp = fopen(str1, "rb");
	if(fp == NULL)
		return 0;
	size = (int)fread(buf, 1, size, fp);
	fclose(fp);
	return size;
}

void Gbuf2File(char *str, int len, int adr)
{
	char str1[100];

	sprintf(str1, "Gbuf2File /flash/%s %x %x\n", str, len, adr);
	SendCmd(str1);
}

void File2Gbuf(char *str, int adr)
{
	char str1[100];

	sprintf(str1, "File2Gbuf /flash/%s %x\n", str, adr);
	SendCmd(str1);
}

#else

#ifndef NCSC
void SaveImageFile(char *str, char *buf, int len)
{
	int h;

	yaffs_unlink(str);
	h = yaffs_open(str,  O_CREAT | O_RDWR | O_TRUNC, S_IREAD | S_IWRITE);
	yaffs_write(h, buf, len);
	yaffs_close(h);
}

int LoadImageFile(char *str, char *buf, int size)
{
	int h;
	int len;
	
	h = yaffs_open(str, O_RDONLY, S_IREAD | S_IWRITE);
	if(h == -1)
		return 0;
	len = (int)yaffs_lseek(h, 0, SEEK_END);
	if(len > size)
		len = size;
	yaffs_lseek(h, 0, SEEK_SET);
	yaffs_read(h, buf, len);
	yaffs_close(h);

	return len;
}

#endif

void Gbuf2File(char *str, int len, int adr)
{
	SaveImageFile(str, gTransferBuf + adr, len);
}

void File2Gbuf(char *str, int adr)
{
	if(adr > 0x80000)
		adr = 0;
	LoadImageFile(str, gTransferBuf + adr, 0x80000 - adr);
}

#endif

