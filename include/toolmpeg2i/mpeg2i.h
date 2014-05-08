
#ifndef _TOOLS_MPEG2I_H_
#define _TOOLS_MPEG2I_H_


#ifndef TOOLMPEG2I_DLL
	#define TOOLMPEG2I_DLL  __declspec(dllimport)
#endif



class TOOLMPEG2I_DLL CMpeg2I
{
public:
	int Mpeg2Blk(char *src, int len, char *dest, int dstlen);

private:
};

#endif

