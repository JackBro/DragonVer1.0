#ifndef _V5USERSETTING_H_
#define _V5USERSETTING_H_

#ifdef __cplusplus
extern "C" {
#endif


	#ifdef WIN32
	#define V5MF_HANDLE FILE* 
	#else
	#define V5MF_HANDLE	int
	#endif

	char *V5M_LoadImageFile(char *str, UINT32 *size);
	void V5M_SaveImageFile(char *str, char *buf, UINT32 len);

#ifdef __cplusplus
}
#endif

#endif
