


#ifndef _RDK_VBRIDGE_H_
#define _RDK_VBRIDGE_H_

#ifdef __cplusplus
extern "C" {
#endif


	typedef struct tag_TV5Context {
		//PTCtrlContext	ctrl;
		//PTDispContext	disp;
		//PTVispContext	isp;
		//PTJpegContext	jpeg;
		//PTMarbContext	marb;
		//PTPhotoContext	photo;
		//PTCameraContext camera;
		UINT8	                   mode;
	} TV5Context, *PTV5Context;

	//typedef struct tag_TVBridgeInfo {
	//	PTCtrlInfo		ctrl;
	//	PTPanelInfo		disp;
	//	PTSnrInfo	isp;
	//} TVBridgeInfo, *PTVBridgeInfo;

	void	V5_InitContext(void);
	void	V5_Open(void);
	void	V5_SetMode(UINT8 mode);
	UINT8	V5_GetMode(void);
	void	V5_Close(void);

	extern TV5Context gV5Context;

#ifdef __cplusplus
}
#endif

#endif 


