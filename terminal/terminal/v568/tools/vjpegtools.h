#ifndef _VJPEGTOOLS_H_
#define _VJPEGTOOLS_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct tag_ExifRational {
	UINT32 numerator; 
	UINT32 denominator;
} ExifRational;

typedef struct tag_ExifSRational {
	SINT32 numerator; 
	SINT32 denominator;
} ExifSRational;


void JpegParse(UINT8* ptr, UINT32 length, PTJpegIndex pinfo);
/*
void JfifParse(UINT8* ptr, UINT32 length, PTJfifIndex pinfo);
void ExifParse(UINT8* ptr, UINT32 length, PTExifIndex pinfo);
void CreateJfif(PTDataBuf pjpgbuf, PTDataBuf pthumbuf, PTDataBuf pdst, UINT8 thumbfmt);
UINT32 GetThumbFromJfif(unsigned char* jfif, UINT32 jfifLen, unsigned char* thumb, unsigned char* fmt, unsigned char* x, unsigned char* y);
UINT32 GetThumbFromExif(unsigned char* exif, UINT32 exifLen, unsigned char* thumb, unsigned char* fmt, unsigned char* x, unsigned char* y);
*/
UINT32 Jpeg2Jfif(unsigned char* jpg, unsigned char* thumb, unsigned char* jfif, UINT32 jpgLen, UINT16 thumbLen, UINT8 thumbX, UINT8 thumbY);
UINT32 Jpeg2Jfxx(unsigned char* jpg, unsigned char* thumb, unsigned char* jfxx, UINT32 jpgLen, UINT16 thumbLen, UINT8 thumbX, UINT8 thumbY, UINT8 fmt);

UINT32	GetImageVwc(UINT8 fmt, TSize size);
UINT8	GetComponent(UINT8 fmt, UINT16 *comp);
UINT16	GetJpegBlockNum(UINT8 fmt, TSize size);
void	GetJpegSize(UINT8 fmt, TSize size, TSize *size1);


enum JPEG_MARKERenum {			
/* JPEG marker codes */
  M_SOF0   = 0xc0,
  M_SOF1   = 0xc1,
  M_SOF2   = 0xc2,
  M_SOF3   = 0xc3,

  M_SOF5   = 0xc5,
  M_SOF6   = 0xc6,
  M_SOF7   = 0xc7,

  M_JPG    = 0xc8,
  M_SOF9   = 0xc9,
  M_SOF10  = 0xca,
  M_SOF11  = 0xcb,

  M_SOF13  = 0xcd,
  M_SOF14  = 0xce,
  M_SOF15  = 0xcf,

  M_DHT    = 0xc4,

  M_DAC    = 0xcc,

  M_RST0   = 0xd0,
  M_RST1   = 0xd1,
  M_RST2   = 0xd2,
  M_RST3   = 0xd3,
  M_RST4   = 0xd4,
  M_RST5   = 0xd5,
  M_RST6   = 0xd6,
  M_RST7   = 0xd7,

  M_SOI    = 0xd8,
  M_EOI    = 0xd9,
  M_SOS    = 0xda,
  M_DQT    = 0xdb,
  M_DNL    = 0xdc,
  M_DRI    = 0xdd,
  M_DHP    = 0xde,
  M_EXP    = 0xdf,

  M_APP0   = 0xe0,
  M_APP1   = 0xe1,
  M_APP2   = 0xe2,
  M_APP3   = 0xe3,
  M_APP4   = 0xe4,
  M_APP5   = 0xe5,
  M_APP6   = 0xe6,
  M_APP7   = 0xe7,
  M_APP8   = 0xe8,
  M_APP9   = 0xe9,
  M_APP10  = 0xea,
  M_APP11  = 0xeb,
  M_APP12  = 0xec,
  M_APP13  = 0xed,
  M_APP14  = 0xee,
  M_APP15  = 0xef,

  M_JPG0   = 0xf0,
  M_JPG13  = 0xfd,
  M_COM    = 0xfe
};

enum EXIFTAGenum {
/* EXIF TAG codes */
	EXIF_TAG_INTEROPERABILITY_INDEX		= 0x0001,
	EXIF_TAG_INTEROPERABILITY_VERSION	= 0x0002,
	EXIF_TAG_IMAGE_WIDTH 			= 0x0100,
	EXIF_TAG_IMAGE_LENGTH 			= 0x0101,
	EXIF_TAG_BITS_PER_SAMPLE 		= 0x0102,
	EXIF_TAG_COMPRESSION 			= 0x0103,
	EXIF_TAG_PHOTOMETRIC_INTERPRETATION 	= 0x0106,
	EXIF_TAG_FILL_ORDER 			= 0x010a,
	EXIF_TAG_DOCUMENT_NAME 			= 0x010d,
	EXIF_TAG_IMAGE_DESCRIPTION 		= 0x010e,
	EXIF_TAG_MAKE 				= 0x010f,
	EXIF_TAG_MODEL 				= 0x0110,
	EXIF_TAG_STRIP_OFFSETS 			= 0x0111,
	EXIF_TAG_ORIENTATION 			= 0x0112,
	EXIF_TAG_SAMPLES_PER_PIXEL 		= 0x0115,
	EXIF_TAG_ROWS_PER_STRIP 		= 0x0116,
	EXIF_TAG_STRIP_BYTE_COUNTS		= 0x0117,
	EXIF_TAG_X_RESOLUTION 			= 0x011a,
	EXIF_TAG_Y_RESOLUTION 			= 0x011b,
	EXIF_TAG_PLANAR_CONFIGURATION 		= 0x011c,
	EXIF_TAG_RESOLUTION_UNIT 		= 0x0128,
	EXIF_TAG_TRANSFER_FUNCTION 		= 0x012d,
	EXIF_TAG_SOFTWARE 			= 0x0131,
	EXIF_TAG_DATE_TIME			= 0x0132,
	EXIF_TAG_ARTIST				= 0x013b,
	EXIF_TAG_WHITE_POINT			= 0x013e,
	EXIF_TAG_PRIMARY_CHROMATICITIES		= 0x013f,
	EXIF_TAG_TRANSFER_RANGE			= 0x0156,
	EXIF_TAG_JPEG_PROC			= 0x0200,
	EXIF_TAG_JPEG_INTERCHANGE_FORMAT	= 0x0201,
	EXIF_TAG_JPEG_INTERCHANGE_FORMAT_LENGTH	= 0x0202,
	EXIF_TAG_YCBCR_COEFFICIENTS		= 0x0211,
	EXIF_TAG_YCBCR_SUB_SAMPLING		= 0x0212,
	EXIF_TAG_YCBCR_POSITIONING		= 0x0213,
	EXIF_TAG_REFERENCE_BLACK_WHITE		= 0x0214,
	EXIF_TAG_RELATED_IMAGE_FILE_FORMAT	= 0x1000,
	EXIF_TAG_RELATED_IMAGE_WIDTH		= 0x1001,
	EXIF_TAG_RELATED_IMAGE_LENGTH		= 0x1002,
	EXIF_TAG_CFA_REPEAT_PATTERN_DIM		= 0x828d,
	EXIF_TAG_CFA_PATTERN			= 0x828e,
	EXIF_TAG_BATTERY_LEVEL			= 0x828f,
	EXIF_TAG_COPYRIGHT			= 0x8298,
	EXIF_TAG_EXPOSURE_TIME			= 0x829a,
	EXIF_TAG_FNUMBER			= 0x829d,
	EXIF_TAG_IPTC_NAA			= 0x83bb,
	EXIF_TAG_EXIF_IFD_POINTER		= 0x8769,
	EXIF_TAG_INTER_COLOR_PROFILE		= 0x8773,
	EXIF_TAG_EXPOSURE_PROGRAM		= 0x8822,
	EXIF_TAG_SPECTRAL_SENSITIVITY		= 0x8824,
	EXIF_TAG_GPS_INFO_IFD_POINTER		= 0x8825,
	EXIF_TAG_ISO_SPEED_RATINGS		= 0x8827,
	EXIF_TAG_OECF				= 0x8828,
	EXIF_TAG_EXIF_VERSION			= 0x9000,
	EXIF_TAG_DATE_TIME_ORIGINAL		= 0x9003,
	EXIF_TAG_DATE_TIME_DIGITIZED		= 0x9004,
	EXIF_TAG_COMPONENTS_CONFIGURATION	= 0x9101,
	EXIF_TAG_COMPRESSED_BITS_PER_PIXEL	= 0x9102,
	EXIF_TAG_SHUTTER_SPEED_VALUE		= 0x9201,
	EXIF_TAG_APERTURE_VALUE			= 0x9202,
	EXIF_TAG_BRIGHTNESS_VALUE		= 0x9203,
	EXIF_TAG_EXPOSURE_BIAS_VALUE		= 0x9204,
	EXIF_TAG_MAX_APERTURE_VALUE		= 0x9205,
	EXIF_TAG_SUBJECT_DISTANCE		= 0x9206,
	EXIF_TAG_METERING_MODE			= 0x9207,
	EXIF_TAG_LIGHT_SOURCE			= 0x9208,
	EXIF_TAG_FLASH				= 0x9209,
	EXIF_TAG_FOCAL_LENGTH			= 0x920a,
	EXIF_TAG_SUBJECT_AREA			= 0x9214,
	EXIF_TAG_MAKER_NOTE			= 0x927c,
	EXIF_TAG_USER_COMMENT			= 0x9286,
	EXIF_TAG_SUB_SEC_TIME			= 0x9290,
	EXIF_TAG_SUB_SEC_TIME_ORIGINAL		= 0x9291,
	EXIF_TAG_SUB_SEC_TIME_DIGITIZED		= 0x9292,
	EXIF_TAG_FLASH_PIX_VERSION		= 0xa000,
	EXIF_TAG_COLOR_SPACE			= 0xa001,
	EXIF_TAG_PIXEL_X_DIMENSION		= 0xa002,
	EXIF_TAG_PIXEL_Y_DIMENSION		= 0xa003,
	EXIF_TAG_RELATED_SOUND_FILE		= 0xa004,
	EXIF_TAG_INTEROPERABILITY_IFD_POINTER	= 0xa005,
	EXIF_TAG_FLASH_ENERGY			= 0xa20b,
	EXIF_TAG_SPATIAL_FREQUENCY_RESPONSE	= 0xa20c,
	EXIF_TAG_FOCAL_PLANE_X_RESOLUTION	= 0xa20e,
	EXIF_TAG_FOCAL_PLANE_Y_RESOLUTION	= 0xa20f,
	EXIF_TAG_FOCAL_PLANE_RESOLUTION_UNIT	= 0xa210,
	EXIF_TAG_SUBJECT_LOCATION		= 0xa214,
	EXIF_TAG_EXPOSURE_INDEX			= 0xa215,
	EXIF_TAG_SENSING_METHOD			= 0xa217,
	EXIF_TAG_FILE_SOURCE			= 0xa300,
	EXIF_TAG_SCENE_TYPE			= 0xa301,
	EXIF_TAG_NEW_CFA_PATTERN		= 0xa302,
	EXIF_TAG_CUSTOM_RENDERED		= 0xa401,
	EXIF_TAG_EXPOSURE_MODE			= 0xa402,
	EXIF_TAG_WHITE_BALANCE			= 0xa403,
	EXIF_TAG_DIGITAL_ZOOM_RATIO		= 0xa404,
	EXIF_TAG_FOCAL_LENGTH_IN_35MM_FILM	= 0xa405,
	EXIF_TAG_SCENE_CAPTURE_TYPE		= 0xa406,
	EXIF_TAG_GAIN_CONTROL			= 0xa407,
	EXIF_TAG_CONTRAST			= 0xa408,
	EXIF_TAG_SATURATION			= 0xa409,
	EXIF_TAG_SHARPNESS			= 0xa40a,
	EXIF_TAG_DEVICE_SETTING_DESCRIPTION	= 0xa40b,
	EXIF_TAG_SUBJECT_DISTANCE_RANGE		= 0xa40c,
	EXIF_TAG_IMAGE_UNIQUE_ID		= 0xa420
};

enum EXIFFORMATenum {
/* EXIF Format Type */
	EXIF_FORMAT_BYTE       =  1,
	EXIF_FORMAT_ASCII      =  2,
	EXIF_FORMAT_SHORT      =  3,
	EXIF_FORMAT_LONG       =  4,
	EXIF_FORMAT_RATIONAL   =  5,
	EXIF_FORMAT_SBYTE      =  6,
	EXIF_FORMAT_UNDEFINED  =  7,
	EXIF_FORMAT_SSHORT     =  8,
	EXIF_FORMAT_SLONG      =  9,
	EXIF_FORMAT_SRATIONAL  = 10,
	EXIF_FORMAT_FLOAT      = 11,
	EXIF_FORMAT_DOUBLE     = 12
};

enum EXIFBYTEORDER {
	EXIF_BYTE_ORDER_MOTOROLA = 0x1,
	EXIF_BYTE_ORDER_INTEL = 0x2
};


enum THUMBNAIL_FORMATenum {
	THUMB_FORMAT_JPEG		=	0,
	THUMB_FORMAT_RGB8		=	1,
	THUMB_FORMAT_RGB24		=	2,
	THUMB_FORMAT_YCBCR		=	3
};

#ifdef __cplusplus
}
#endif

#endif


