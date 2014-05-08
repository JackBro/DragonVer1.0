#include "inheaders.h"
#ifndef	V578CMD_DISABLE


//void *___fopen(char *filename, char *rwmode)
//{
//	FILE *fp;
//	fp = fopen( filename, rwmode );
//	return fp;
//}
//
//UINT32 ___fclose(void *file)
//{
//	fclose((FILE *)file);
//	return 0;
//}
//
//UINT32 ___fread(UINT8 *buf, UINT32 length,void *file)
//{
//	fread(buf, length, 1, (FILE *)file);
//	return 0;
//}
//
//UINT32 ___fwrite(UINT8 *buf, UINT32 length,void *file)
//{
//	fwrite(buf, length, 1, (FILE *)file);
//	return 0;
//}


#ifdef WIN32

#include <stdio.h>
#include <malloc.h>

V5MF_HANDLE V5M_fopen(char *filename, char *rwmode)
{
	V5MF_HANDLE fp = fopen( filename, rwmode );
	return fp;
}

UINT32 V5M_fclose(V5MF_HANDLE file)
{
	fclose(file);
	return 0;
}

UINT32 V5M_fread(UINT8 *buf, UINT32 length,V5MF_HANDLE file)
{
	UINT32 count;
	count = (UINT32) fread(buf, length, 1, file);
	return count;
}

UINT32 V5M_fwrite(UINT8 *buf, UINT32 length,V5MF_HANDLE file)
{
	UINT32 count;
	count = (UINT32) fwrite(buf, length, 1, file);
	return count;
}

UINT32 V5M_flen (V5MF_HANDLE file)
{
	UINT32 count;
	fseek(file, 0, SEEK_END); 
	count = (UINT32) ftell(file);
	fseek(file, 0,SEEK_SET);
	return count;
}


void *V5M_malloc(UINT32 bufsize)
{
	return malloc(bufsize);
}

void V5M_free(void *pointer, UINT32 bufsize)
{
	free(pointer);
	bufsize = bufsize;
}

#else
#include "../../yaffs/yaffsfs.h"
#include "../../vstdio/vmalloc.h"
V5MF_HANDLE V5M_fopen(char *filename, char *rwmode)
{
	/*if (strcmp(rwmode, "wb") == 0)
		return yaffs_open(filename, O_CREAT | O_RDWR | O_TRUNC, S_IREAD | S_IWRITE);
	else */
	V5MF_HANDLE fp;

	
	if (strcmp(rwmode, "rb") == 0)
	{
		fp = yaffs_open(filename, O_RDONLY,  S_IREAD | S_IWRITE);
		if (fp == -1)
			return 0;
		return fp;
	}
	else if (strcmp(rwmode, "wb") == 0)
	{
		yaffs_unlink(filename);
		return yaffs_open(filename,  O_CREAT | O_RDWR | O_TRUNC,  S_IREAD | S_IWRITE);
	}
	else return 0;
}

UINT32 V5M_fclose(V5MF_HANDLE file)
{
	return (UINT32) yaffs_close(file);
}

UINT32 V5M_fread(UINT8 *buf, UINT32 length, V5MF_HANDLE file)
{
	return (UINT32) yaffs_read(file, (void *)buf, (unsigned int) length );
}

UINT32 V5M_fwrite(UINT8 *buf, UINT32 length, V5MF_HANDLE file)
{
	UINT32 count;
	count = (UINT32) yaffs_write(file, (void *)buf, (unsigned int) length);
	return count;
}

UINT32 V5M_flen (V5MF_HANDLE file)
{
	UINT32 count;
	count = (UINT32) yaffs_lseek(file, 0, SEEK_END);
	yaffs_lseek(file, 0, SEEK_SET);
	return count;
}


void *V5M_malloc(UINT32 bufsize)
{
	return vmalloc((unsigned int) bufsize);
}

void V5M_free(void *pointer, UINT32 bufsize)
{
	vfree(pointer);
	bufsize = bufsize;
}

#endif



V5MFree		*gpfree		= V5M_free;
V5MMalloc	*gpmalloc	= V5M_malloc;

const TFileOperations gfileop = 
{
	V5M_fopen,
	V5M_fclose,
	V5M_fread,
	V5M_fwrite,
	V5M_flen
};



const THapiStdInfo gHapiStdInfo ={

	{ 2, 0, 1 },			/* sensor resolution */  //count,index.....
	//Tsize	LCDdisplayres;				/* display resolution */
	0,			/*scenemode*/
	0,
	//0,			/* with/without frame;*/
	//UINT8	frameindex;
	//0,		/* with/without thumbnail*/
	{30,20},			//TSize	thumbres;
	V5M_NOEFFECT, 		/* Special Effect */
	V5M_ROTATION_0,			/* Rotation */
	V5M_AWB_AUTO,			/* white balance mode */
	V5M_AE_AUTO,				/* Auto Exposure */
	V5M_FLASH_AUTO,			/* Flash */
	//0,				/* Auto Focus */
	32,			/* brightness  */
	32,			/* Contrast */
	10,				/*digitalzoom;*/
	127,			/*blendratio*/
	2,		/* Brightness step value */
	2,		/* Contrast step value */
	1,		/* Digital Zoom step value, between(1-19) */
	10,		/* blendratio step (1-19) */
	0,		/* "Default(NO BRC)","compress ratio", "Jpeg filesize" */
	8,		/* JPEG compression rate, value range(1-100),0 is invalid */
	32,		/* JPEG File size ,0 is invalid*/
								

	/*-------Setup only for capture multi-shot-----*/
	15,			/* multi-shot count */
	2,      			/* capture interval ,ms as unit, now it has been changed to capture rate*/
	/*-------Setup/control only for capture video-----*/
	0,			//	videores index;
	100		/* real videolength can be smaller than this value when user stops capturing manually */
	
	/*will not change */
};

#ifdef WIN32
#include <stdio.h>

char *V5M_LoadImageFile(char *str, UINT32 *size)
{
	FILE *fp = NULL;
	char *buf = NULL;

	fp = fopen(str, "rb");
	if(fp == NULL)
		return NULL;

	fseek(fp, 0, SEEK_END); 
	*size  = (UINT32) ftell(fp);
	fseek(fp, 0,SEEK_SET);

	if (*size == 0)
		return NULL;
	buf = (char *) gHapiContext.pmalloc(*size);
	*size = (int)fread(buf, 1, *size, fp);
	fclose(fp);

	return buf;
}

void V5M_SaveImageFile(char *str, char *buf, UINT32 len)
{
	FILE *fp = NULL;

	fp = fopen(str, "wb");
	if(fp == NULL)
		return;
	fwrite(buf, 1, len, fp);
	fclose(fp);
}

#else

char *V5M_LoadImageFile(char *str, UINT32 *size)
{
	int h;
	char *buf = NULL;
	
	h = yaffs_open(str, O_RDONLY, S_IREAD | S_IWRITE);
	if(h == -1)
		return NULL;

	*size = (int)yaffs_lseek(h, 0, SEEK_END);
	yaffs_lseek(h, 0, SEEK_SET);

	if (*size == 0)
		return NULL;
	buf = (char *) gHapiContext.pmalloc(*size);
	yaffs_read(h, buf, (unsigned int)(*size));
	yaffs_close(h);

	return buf;
}

void V5M_SaveImageFile(char *str, char *buf, UINT32 len)
{
	int h;

	yaffs_unlink(str);
	h = yaffs_open(str,  O_CREAT | O_RDWR | O_TRUNC, S_IREAD | S_IWRITE);
	yaffs_write(h, buf, (unsigned int)len);
	yaffs_close(h);
}

#endif


#endif //V578CMD_DISABLE
