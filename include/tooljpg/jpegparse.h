#ifndef _JPEGPARSE_H_
#define _JPEGPARSE_H_


#ifndef TOOLJPG_DLL
	#define TOOLJPG_DLL  __declspec(dllimport)
#endif

#include "tools/global.h"

class TOOLJPG_DLL CJpegParse
{
public:
	void	JpegParse(unsigned char *ptr, int length, PTJpegIndex pinfo);

protected:
	void			GetJpegSize(unsigned char fmt, TSize size, TSize *size1);
	unsigned long	GetImageVwc(unsigned char fmt, TSize size);
	unsigned long	GetYuvSize(unsigned char yuvfmt, TSize size);
	unsigned short	GetJpegBlockNum(unsigned char fmt, TSize size);
	unsigned char	GetComponent(unsigned char fmt, unsigned short *comp);
	

private:
	unsigned short	GetSOF0(PTJpegIndex pinfo, unsigned char* ptr);
	unsigned short	GetSOF_Unsupport(PTJpegIndex pinfo, unsigned char* ptr);	
	unsigned short	GetDQT(PTJpegIndex pinfo, unsigned char* ptr);
	unsigned short	GetDHT(PTJpegIndex pinfo, unsigned char* ptr);
	unsigned short	GetSOS(PTJpegIndex pinfo, unsigned char* ptr);
	unsigned short	GetCompFrmSOF(unsigned char *buf);
	unsigned short	GetComment(PTJpegIndex pinfo, unsigned char *ptr);
	unsigned short	ByPassMarker(unsigned char* ptr);
	
	void			ParseExif(unsigned char* ptr, PTThumbNail pthumbinfo);	
	int				GetIFD0(unsigned char* ptr, int point, PTThumbNail pthumbinfo);
	int				GetIFD1(unsigned char* ptr, int point, PTThumbNail pthumbinfo);	
	int				ExifGetDword(unsigned char* ptr, unsigned char order);
	unsigned short	ExifGetWord(unsigned char* ptr, unsigned char order);
	unsigned short	GetExifInfo(unsigned char* ptr, int point, PTThumbNail pthumbinfo);

	unsigned short	GetJfifInfo(unsigned char* ptr, int point, PTThumbNail pthumbinfo);

};




#endif



