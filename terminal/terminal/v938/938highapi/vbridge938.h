
#ifndef _RDK_VBRIDGE938_H_
#define _RDK_VBRIDGE938_H_

#ifdef __cplusplus
extern "C" {
#endif


	void	V9_InitContext(void);
	void	V9_Open(void);
	void	V9_SetMode(UINT8 mode);
	UINT8	V9_GetMode(void);
	void	V9_Close(void);

	extern TV9Context gV9Context;

#ifdef __cplusplus
}
#endif

#endif   //_RDK_VBRIDGE938_H_


