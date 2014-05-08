


#ifndef _RDK_PHOTO_H_
#define _RDK_PHOTO_H_

#ifdef __cplusplus
extern "C" {
#endif

	enum PHOTOCTRLenum {
		CAPTUREFRAME	=	BIT0,
		FLASHEN			=	BIT1
	};

	typedef struct tag_TPhotoContext {
		UINT8	state;
		UINT8	frame;
	} TPhotoContext, *PTPhotoContext;

	void V5_PhotoOpen(void);
	TPhotoContext * V5_PhotoGetContext(void);

	void	V5_PhotoEnableThumb(void);

	void	V5_PhotoSetMinThumbImage(TSize size);

#ifdef __cplusplus
}
#endif

#endif 


