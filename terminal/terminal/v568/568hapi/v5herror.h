#ifndef _V5ERROR_H_
#define _V5ERROR_H_

#ifdef __cplusplus
extern "C" {
#endif


enum V5MERRORenum{
	CAMERA_ISNOT_OPEN,
	DIGITALZOOM_OUTOFRANGE,
	BRIGHT_CONTRAST_OUTOFRANGE,
	CANNOT_ALLOC_MEMORY,
	V5M_USERJBUF_ISFULL,
	V5M_BUFINVALID,
	V5M_TCR_IS_TOOLARGE

};



#ifdef __cplusplus
}
#endif

#endif
