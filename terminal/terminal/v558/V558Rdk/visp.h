#ifndef _VISP_H_
#define _VISP_H_

#ifdef __cplusplus
extern "C" {
#endif


#ifndef SNRFAILED
	#define SNRFAILED 0xff
#endif

#ifndef SUCCEED
	#define SUCCEED 0
#endif


#define I2CWAITTIME			0x7f00;

#define EPFREQ_50HZ			10000
#define EPFREQ_60HZ			12000
#define ET_ONE_STEP			100
#define ET_DEFAULT			200

#define MAX_YMEAN 			0xff


#define FLK_THD_LT_STEP		0x10000

#define GAINRAGNE_TIME 4

typedef enum _FLK_STATE
{
	FLK_BEGIN	= 0X0,
	FLK_JUST_OUT	,
	FLK_JUST_INDOOR	,
	FLK_END
}FLK_STATE;

typedef enum _LIGHT_TYPE
{
	LIGHT_IN 	,
	LIGHT_OUT
}LIGHT_TYPE;

#define MAX_FLK_ET 25





#ifdef __cplusplus
}
#endif

#endif

