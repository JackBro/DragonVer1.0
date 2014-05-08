#ifndef _V5HCMD_H_
#define _V5HCMD_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "v5usersetting.h"

enum SCENEMODEenum {
    V5M_SCENE_NORMAL ,		
    V5M_SCENE_NIGHT ,		
	V5M_SCENE_PORTRAIT ,		
    V5M_SCENE_NIGHT_PORTRAIT ,
    V5M_SCENE_LANDSCAPE ,		
	V5M_SCENE_SNOW ,			
	V5M_SCENE_SAND ,			
	V5M_SCENE_BEACH ,			
	V5M_SCENE_MOTION ,		
	V5M_SCENE_FIREWORKS		
};

enum SPECEFFECTenum{
	V5M_SEPHIA ,
	V5M_SPECIALCOLOR ,
	V5M_NEGATIV ,
	V5M_SKETCH ,
	V5M_RELIEF , 
	V5M_MONOCHROME ,
	V5M_NOEFFECT = 0xFF
};


enum ROTATIONenum{
	V5M_ROTATION_0 ,
	V5M_ROTATION_90 ,
	V5M_ROTATION_180 ,
	V5M_ROTATION_270
};

enum AWBMODEenum{
	V5M_AWB_AUTO,
	V5M_AWB_SUNLIGHT,		// 　　(晴天)
	V5M_AWB_INCANDESCENT,	//  （白炽灯光）
	V5M_AWB_FLUORESCENT,	//  （荧光灯光）
	V5M_AWB_INDOOR			// （室内，或称为阴天）
};

enum AEenum{
	V5M_AE_AUTO, 
	V5M_AE_OUTDOOR,
	V5M_AE_INDOOR_50HZ, 
	V5M_AE_INDOOR_60HZ, 
	V5M_AE_NIGHT
};

enum FLASHMODEenum{
	V5M_FLASH_AUTO,
	V5M_FLASH_ALWAYSOPEN,
	V5M_FLASH_ALWAYSCLOSE
};

enum CAPSIZEenum{
	V5M_SXGA	=1,	// (1280 x 960 )  130万像素
	V5M_XGA,	//（1024 x 768）  80 万像素
	V5M_SVGA,	//（800 x 600）  50万像素
	V5M_VGA,	//（640 x 480）   30万像素
	V5M_CIF,	// (352x288)       10万像素
	V5M_SIF,	//（320 x 240）
	V5M_QCIF,	// (176x144)     
	V5M_QSIF,	// (160x120)
	V5M_LCDPANELSIZE
};

enum DIGITALZOOMenum{
	V5M_MIN_DIGITALZOOM = 1,	//(1/20) * original
	V5M_MAX_DIGITALZOOM = 20	//original
};

enum BRIGHT_CONTRASTenum{
	V5M_MIN_BRIGHT_CONTRAST = 1,
	V5M_MAX_BRIGHT_CONTRAST = 64
};

enum DECODESIZERenum{
	V5M_MIN_DECODESIZER = 1,	//(1/20) * original
	V5M_MAX_DECODESIZER = 10	//original
};

enum V5M_CAMERASTATEenum {
	V5M_CAMERA_NOTINITED,
	V5M_CAMERA_INITED,
	V5M_CAMERA_OPEN,
	V5M_CAMERA_CLOSE
};

enum V5M_FEATUREFLAG{
	V5M_BIT_FRAME			= 1,
	V5M_BIT_THUMB			= 2,
	V5M_BIT_FRAMETHUMB		= 3,
	V5M_BIT_AF				= 4,
	V5M_BIT_CAPASWALLPIC	= 8
	//V5M_BIT_CHOOSECOMPRATE	= 0x80
};

enum V5M_COMPRESSSIZETYPE{
	V5M_COMPRESSSIZE_AUTO,
	V5M_COMPRESSSIZE_RATIO,
	V5M_COMPRESSSIZE_FILESIZE
};


enum V5M_FILEFLAG{
	FLAG_READ	= 0x01,
	FLAG_WRITE	= 0x02,
	FLAG_BINARY = 0x04
};
//typedef struct tag_TFrame{
//	UINT8 frmcount;
//	TSingleFrameInfo *singleframe;
//}TFrame, *PTFrame;
//
//typedef struct tag_TSingleFrameInfo{
//	char desc[10];
//	char filename[100];
//}TSingleFrameInfo, *PTSingleFrameInfo;

typedef struct tag_THapiStdInfo {

	UINT8	sensorres[10];			/* sensor resolution */
	//Tsize	LCDdisplayres;				/* display resolution */
	UINT8	scenemode;
	UINT8	featureflag;//withframe;			/* with/without frame;*/
	/*
	BIT0:	frame
	BIT1:	thumbnail
	BIT2:	AF
	BIT3:	capture as wallpicture
	BIT7:	choosecomprate
	*/
	//UINT8	frameindex;
	//UINT8	withthumb; 			/* with/without thumbnail*/
	/*zhan note: withframe, withthumb can be combined into One UINT8 unit */
	TSize	thumbres;
	UINT8	specEffect; 		/* Special Effect */
	UINT8	rotation;			/* Rotation */
	UINT8	awbmode;			/* white balance mode */
	UINT8	aemode;				/* Auto Exposure */
	UINT8	flashmode;			/* Flash */
	//UINT8	isaf;				/* Auto Focus */
	UINT8	brightness;			/* brightness  */
	UINT8	contrast;			/* Contrast */
	UINT8	digitalzoom;
	UINT8	blendratio;
	UINT8	brightstep;			/* Brightness step value */
	UINT8	contraststep;		/* Contrast step value */
	UINT8	digitalzoomstep;	/* Digital Zoom step value, between(1-9) */
	UINT8	blendratiostep;
	//UINT8 	choosecomprate;		/* if true ,choose JPEG compression rate , else choose JPEG File size */
	UINT8	compresssizetype;
	UINT8	jpegcomprate;		/* JPEG compression rate, value range(1-100),0 is invalid */
	UINT16	jpegfilesize;		/* JPEG File size ,0 is invalid*/
								

	/*-------Setup only for capture multi-shot-----*/
	UINT8	mshotcount;			/* multi-shot count */
	UINT16	mshotinterval;		/* capture interval */
	/*-------Setup/control only for capture video-----*/
	UINT8	videores;
	UINT8	initvideolength;		/* real videolength can be smaller than this value when user stops capturing manually */
	/**/
	//char frame[10][255];
	/*  */
	//TFileOperations fileop;
	//V5MMalloc *pmalloc;
}THapiStdInfo, *PTHapiStdInfo;

	typedef void *V5MMalloc(UINT32 bufsize);
	typedef void V5MFree(void *pointer, UINT32 bufsize);
	typedef struct tag_FileOperations { 
	V5MF_HANDLE (*fopen) (char *filename, char *rwmode); 
	UINT32 (*fclose) (V5MF_HANDLE file); 
	UINT32 (*fread) (UINT8 *buf, UINT32 length, V5MF_HANDLE file); 
	UINT32 (*fwrite) (UINT8 *buf, UINT32 length, V5MF_HANDLE file); 
	UINT32 (*flen) (V5MF_HANDLE file);
	} TFileOperations, *PTFileOperations;


typedef struct tag_THapiContext{
	THapiStdInfo hapiStdInfo; 
	UINT8 camera_state;
	UINT8 workmode;
	TSize originwinsize;
	//TSize curwinsize;
	//TPoint curwinpoint;
	V5MMalloc	*pmalloc;
	V5MFree		*pfree;
	TFileOperations fileop;
	//something can be add later on
}THapiContext, *PTHapiContext;

//THapiContext gHapiContext;

typedef UINT32 V5MCmdCall(void *param, UINT32 paramsize);

typedef struct tag_TCmdDef{
	//char	*cmddiscr;
	V5MCmdCall	*pcall;
} TCmdDef, *PTCmdDef;

enum V5M_MESSAGEenum{
	V568_OPEN_DEFAULT = 1,
	V568_OPEN_WITH_CONTEXT,
	V5M_GETHAPISTDINFO,
	V5M_SETBRIGHTNESS,
	V5M_SETCONTRAST,
	V5M_CAMERAZOOM,
	V5M_BRIGHTNESSUP,
	V5M_BRIGHTNESSDOWN,
	V5M_CONTRASTUP,
	V5M_CONTRASTDOWN,
	V5M_CAMERAZOOMIN,
	V5M_CAMERAZOOMOUT,
	V5M_BLENDRATIOUP,
	V5M_BLENDRATIODOWN,
	V5M_SETBLENDRATIO,
	V5M_AF_ON,
	V5M_AF_OFF,
	V5M_CAPSTILL_START,
	V5M_CAPMULTISHOT_START,
	V5M_CAPVIDEO_START,
	V5M_GETCAPTUREINFO_TOFILE,
	V5M_GETCFGSNRINFO,
	V5M_GETCFGSNRSIZEBYINDEX,
	V5M_FRAMEON,
	V5M_FRAMEOFF,
	V5M_THUMBON,
	V5M_THUMBOFF,
	V5M_SETFRAMEINTERVAL,
	V5M_SETEXPOSEMODE,
	V5M_SETSNRRESOINDEX,
	V5M_SETVIDEOFILESIZE,
	V5M_SETSPECIALEFFECT,
	V5M_SETROTATEMODE,
	V5M_SPECIALEFFECTOFF,
	V5M_PREVIEWAUTO,
	V5M_SETCOMPRATIO,
	V5M_SETJFILESIZE,
	V5M_CAPASWALLPICON,
	V5M_CAPASWALLPICOFF,
	V5M_DISPLAYSTILL,
	V5M_DISPLAYVIDEO,
	V5M_DECODEFRAMEENCODE,
	V5M_DECODEIPPENCODE

};

typedef struct tag_TgetCaptureInfoToFileParam{
	char	*filename;
	UINT32 bufsizeOnetime;
}TgetCaptureInfoToFileParam,PTgetCaptureInfoToFileParam;


typedef struct tag_TgetSnrSizeByIndexParam{
	char index;
	TSize size;
}TgetSnrSizeByIndexParam;

typedef struct tag_TDecodeFrameEncodeParam{
	char *srcfile;
	char *dstfile;
}TDecodeFrameEncodeParam;

typedef struct tag_TDecodeIPPEncodeParam{
	char *srcfile;
	char *dstfile;
	UINT8 ratio;
}TDecodeIPPEncodeParam;

UINT32 _V568_Open_Default(void *param, UINT32 paramsize);
UINT32 _V568_Open_WithContext(void *param, UINT32 paramsize);
UINT32 _V5M_GetHapiStdInfo(void *param, UINT32 paramsize);
UINT32 _V5M_SetBrightness(void *param, UINT32 paramsize);
UINT32 _V5M_BrightnessUp(void *param, UINT32 paramsize);
UINT32 _V5M_BrightnessDown(void *param, UINT32 paramsize);
UINT32 _V5M_SetContrast(void *param, UINT32 paramsize);
UINT32 _V5M_ContrastUp(void *param, UINT32 paramsize);
UINT32 _V5M_ContrastDown(void *param, UINT32 paramsize);
UINT32 _V5M_CameraZoomIn(void *param, UINT32 paramsize);
UINT32 _V5M_CameraZoomOut(void *param, UINT32 paramsize);
UINT32 _V5M_CameraZoom(void *param, UINT32 paramsize);
UINT32 _V5M_BlendratioUp(void *param, UINT32 paramsize);
UINT32 _V5M_BlendratioDown(void *param, UINT32 paramsize);
UINT32 _V5M_SetBlendratio(void *param, UINT32 paramsize);
UINT32 _V5M_AF_ON(void *param, UINT32 paramsize);
UINT32 _V5M_AF_OFF(void *param, UINT32 paramsize);
UINT32 _V5M_CapStill_start(void *param, UINT32 paramsize);
UINT32 _V5M_CapMultishot_start(void *param, UINT32 paramsize);
UINT32 _V5M_CapVideo_start(void *param, UINT32 paramsize) ;
UINT32 _V5M_getCaptureInfoToFile(void *param, UINT32 paramsize);
UINT32 _V5M_getcfgSnrInfo(void *param, UINT32 paramsize);
UINT32 _V5M_getcfgSnrSizeByIndex(void *param, UINT32 paramsize);
UINT32 _V5M_FrameON(void *param, UINT32 paramsize);
UINT32 _V5M_FrameOFF(void *param, UINT32 paramsize);
UINT32 _V5M_ThumbON(void *param, UINT32 paramsize);
UINT32 _V5M_ThumbOFF(void *param, UINT32 paramsize);
UINT32 _V5M_SetFrameInterval(void *param, UINT32 paramsize);
UINT32 _V5M_SetExposeMode(void *param, UINT32 paramsize);
UINT32 _V5M_SetSnrResoIndex(void *param, UINT32 paramsize);
UINT32 _V5M_SetVideoFileSize(void *param, UINT32 paramsize);
UINT32 _V5M_SetSpecialEffect(void *param, UINT32 paramsize);
UINT32 _V5M_SetRotateMode(void *param, UINT32 paramsize);
UINT32 _V5M_SpecialEffectOFF(void *param, UINT32 paramsize);
UINT32 _V5M_PreviewAuto(void *param, UINT32 paramsize);
UINT32 _V5M_SetCompRatio(void *param, UINT32 paramsize);
UINT32 _V5M_SetCompedJFileSize(void *param, UINT32 paramsize);
UINT32 _V5M_CapAsWallPicON(void *param, UINT32 paramsize);
UINT32 _V5M_CapAsWallPicOFF(void *param, UINT32 paramsize);
UINT32 _V5M_DisplayStill(void *param, UINT32 paramsize);
UINT32 _V5M_DisplayVideo(void *param, UINT32 paramsize);
UINT32 _V5M_DecodeFrameEncode(void *param, UINT32 paramsize);
UINT32 _V5M_DecodeIPPEncode(void *param, UINT32 paramsize);



UINT32	V5M_CameraGetSizeByIndex(UINT8 index, TSize *size);
void	V5M_InitContext(void);
void	V5M_Open(void);
void	V5M_SetMode(UINT8 mode);
void	V5M_SetModeAction(UINT8 mode);
UINT8	V5M_GetMode(void);
void	V5M_Close(void);
void	V5M_SetHapiStdInfo(THapiStdInfo *pInfo);
UINT32 V5M_CapStill_start(void) ;
UINT32 V5M_CapMultishot_start(UINT8 mshotcount);
UINT32 V5M_CapVideo_start(void ) ;

UINT32 V5M_SetFileOperation(TFileOperations fileop);
void V5M_SetMalloc(V5MMalloc *pmalloc);
void V5M_SetMfree(V5MFree	*pfree);
void  V5M_PreviewAuto(void);



UINT32 V5_SendCommand(UINT32 msg, void *param, UINT32 paramsize);


#ifdef __cplusplus
}
#endif

#endif

