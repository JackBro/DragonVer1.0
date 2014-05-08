
#ifndef _TOOLS_MPEG4_H_
#define _TOOLS_MPEG4_H_


#ifndef TOOLMPEG4_DLL
	#define TOOLMPEG4_DLL  __declspec(dllimport)
#endif



class TOOLMPEG4_DLL CMpeg4
{
public:
	int Mpeg4Blk(char *src, int len, char *dest, int dstlen);

private:

};

#endif
