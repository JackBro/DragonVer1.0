
#ifndef _TESTFS_H_
#define _TESTFS_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef WIN32
	void CopyCurPath(char *str);

	extern char gCurDir[100];

#endif

	void SaveImageFile(char *str, char *buf, int len);
	int LoadImageFile(char *str, char *buf, int size);

	void Gbuf2File(char *str, int len, int adr);
	void File2Gbuf(char *str, int adr);
	void File2pc(char *src, char *buf);

#ifdef __cplusplus
}
#endif

#endif

