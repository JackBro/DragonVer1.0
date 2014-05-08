
#ifndef _TOOLS_JPEG_H_
#define _TOOLS_JPEG_H_


#ifndef TOOLJPG_DLL
	#define TOOLJPG_DLL  __declspec(dllimport)
#endif


class TOOLJPG_DLL CJpeg
{
public:
	int Jpeg2Blk(char *src, int len, char *dest);

private:
};

#endif

