#ifndef _STRINGTOOLS_H_
#define _STRINGTOOLS_H_


#ifndef TOOLCHAR_DLL
	#define TOOLCHAR_DLL  __declspec(dllimport)
#endif

#include "chartools.h"


#define FILE_TAIL_BIN	"bin"
#define FILE_TAIL_CPP	"cpp"
#define FILE_TAIL_HEX	"hex"
#define FILE_TAIL_RGB	"rgb"
#define FILE_TAIL_BMP	"bmp"
#define FILE_TAIL_TXT	"txt"
#define FILE_TAIL_DAT	"dat"
#define FILE_TAIL_JPG	"jpg"
#define FILE_TAIL_AVI	"avi"
#define FILE_TAIL_CFA	"cfa"

enum STRINGSTATEenum {
	STRING_SUCCEED,
	STRING_FAILED
};

class TOOLCHAR_DLL CStringTools : public CCharTools
{
public:
	int		Str2Hex(char *str, int *x);
	int		Str2Dec(char *str, int *x);
	int		Str2Oct(char *str, int *x);

	void	StrUpCase(char *str, char *dst);
	void	StrCopyNull(char *src, char *dst);
	void	Val2Binery(int val, char *str, int unit = 1);
	int		FindFirstChar(char *str, char ch, int len = 0);
	int		FindLastChar(char *str, char ch, int len = 0);
	int		FindFirstCR(char *str, int len = 0);
	int		FindLastCR(char *str, int len = 0);

	int		FindFirstNum(char *str, int *start, int *end, int len = 0);
	int		FindLastNum(char *str, int *start, int *end, int len = 0);

	void	AutoIndex2Str(char *src, char *tail, char *dest);
	void	AutoIndex2Str1(char *src, char *tail, char *dest);
	void	DerivePathTail(char *src, char *tail, char *dest);
	int		FindLocate(char *src, char *dst, int len);

	void	AddIndex(char *src, char *dest);
};

#endif


