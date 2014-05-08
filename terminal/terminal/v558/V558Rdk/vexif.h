/*************************************************************************/
/*									 */
/* The last modification date:  02-23-2004                               */
/* REMARKS:  Created initial version 1.0                                 */
/*                                                                       */
/*                Copyright (C) 2004 Vimicro CO.,LTD     		 */
/*************************************************************************/


//******************* Some util. MACRO Definitions.  ******************************
#define     TIFF_VERSION        42
#define     EXIF_BIGENDIAN      0x4d4d
#define     EXIF_LITTLEENDIAN   0x4949

//******************* Data Type Definitions.  *************************************
#define		EXIF_NOTYPE		 	0		// placeholder
#define		EXIF_BYTE			1		// 8-bit unsigned integer
#define		EXIF_ASCII			2		// 8-bit  bytes w/ last byte null
#define		EXIF_SHORT			3		// 16-bit unsigned integer
#define		EXIF_LONG			4		// 32-bit unsigned integer
#define		EXIF_RATIONAL		        5		// 64-bit unsigned fraction
#define		EXIF_SBYTE			6		// 8-bit  signed integer
#define		EXIF_UNDEFINED	 		7		// 8-bit  untyped data
#define		EXIF_SSHORT			8		// 16-bit signed integer
#define		EXIF_SLONG			9		// 32-bit signed integer
#define		EXIF_SRATIONAL			10		// 64-bit signed fraction
#define		EXIF_FLOAT			11		// 32-bit IEEE floating point
#define		EXIF_DOUBLE			12		// 64-bit IEEE floating point

//******************* TIFF6.0 Tag Definitions.*************************************

#define	EXIFTAG_IMAGEWIDTH			256			// image width in pixels
#define	EXIFTAG_IMAGELENGTH			257			// image height in pixels
#define	EXIFTAG_BITSPERSAMPLE			258			// bits per channel (sample)
#define	EXIFTAG_COMPRESSION			259			// data compression technique
#define	EXIF_COMPRESSION_NONE			1		    	//      No Comp.
#define	EXIF_COMPRESSION_JPEG			6			//      Only Thumbnail Image is
														//      JPEG compressed 
#define	EXIFTAG_PHOTOMETRIC			262			// photometric interpretation
#define	PHOTOMETRIC_RGB				2			// RGB color model
#define	PHOTOMETRIC_YCBCR			6			// CCIR 601

#define	EXIFTAG_THRESHHOLDING			263			// thresholding used on data
#define	THRESHHOLD_BILEVEL			1					// b&w art scan
#define	THRESHHOLD_HALFTONE			2					// or dithered scan
#define	THRESHHOLD_ERRORDIFFUSE			3					// usually floyd-steinberg
#define	EXIFTAG_CELLWIDTH			264			// dithering matrix width
#define	EXIFTAG_CELLLENGTH			265			// dithering matrix height
#define	EXIFTAG_FILLORDER			266			// data order within a byte
#define	    FILLORDER_MSB2LSB			1					// most significant -> least
#define	    FILLORDER_LSB2MSB			2					// least significant -> most
#define	EXIFTAG_DOCUMENTNAME			269			// name of doc. image is from

#define	EXIFTAG_IMAGEDESCRIPTION		270			// info about image
#define	EXIFTAG_MAKE				271			// scanner manufacturer name
#define	EXIFTAG_MODEL				272			// scanner model name/number

#define	EXIFTAG_STRIPOFFSETS			273			// offsets to data strips
#define	EXIFTAG_ORIENTATION			274			// image orientation
#define	    ORIENTATION_TOPLEFT		1					// row 0 top, col 0 lhs
#define	    ORIENTATION_TOPRIGHT	2					// row 0 top, col 0 rhs
#define	    ORIENTATION_BOTRIGHT	3					// row 0 bottom, col 0 rhs
#define	    ORIENTATION_BOTLEFT		4					// row 0 bottom, col 0 lhs
#define	    ORIENTATION_LEFTTOP		5					// row 0 lhs, col 0 top
#define	    ORIENTATION_RIGHTTOP	6					// row 0 rhs, col 0 top
#define	    ORIENTATION_RIGHTBOT	7					// row 0 rhs, col 0 bottom
#define	    ORIENTATION_LEFTBOT		8					// row 0 lhs, col 0 bottom
#define	EXIFTAG_SAMPLESPERPIXEL			277			// samples per pixel
#define	EXIFTAG_ROWSPERSTRIP			278			// rows per strip of data
#define	EXIFTAG_STRIPBYTECOUNTS			279			// bytes counts for strips
#define	EXIFTAG_XRESOLUTION			282			// pixels/resolution in x
#define	EXIFTAG_YRESOLUTION			283			// pixels/resolution in y
#define	EXIFTAG_PLANARCONFIG			284			// storage organization
#define	    PLANARCONFIG_CONTIG			1					// Each pixel are stored as chanky
#define	    PLANARCONFIG_SEPARATE		2					// separate planes of data
#define	EXIFTAG_RESOLUTIONUNIT			296			// units of resolutions
#define	    RESUNIT_INCH	    		2					// english
#define	    RESUNIT_CENTIMETER	    		3					// metric
#define	EXIFTAG_PAGENUMBER			297			// page numbers of multi-page
#define	EXIFTAG_TRANSFERFUNCTION		301			// colorimetry info
#define	EXIFTAG_SOFTWARE			305			// name & release
#define	EXIFTAG_DATETIME			306			// creation date and time
#define	EXIFTAG_ARTIST				315			// creator of image
#define	EXIFTAG_WHITEPOINT			318			// image white point
#define	EXIFTAG_PRIMARYCHROMATICITIES		319			// primary chromaticities
#define	EXIFTAG_JPEGIFOFFSET			513			// pointer to SOI marker
#define	EXIFTAG_JPEGIFBYTECOUNT			514			// JFIF stream length
#define	EXIFTAG_YCBCRCOEFFICIENTS		529			// RGB -> YCbCr transform
#define	EXIFTAG_YCBCRSUBSAMPLING		530			// YCbCr subsampling factors
#define	EXIFTAG_YCBCRPOSITIONING		531			// subsample positioning
#define	    YCBCRPOSITION_CENTERED		1				// as in PostScript Level 2
#define	    YCBCRPOSITION_COSITED	2					// as in CCIR 601-1
#define	EXIFTAG_REFERENCEBLACKWHITE			532			// colorimetry info

// These tags are actually IFD's within IFD's
#define EXIFTAG_EXIFIFDPOINTER				34665
#define EXIFTAG_CAMERAINFOIFD				33424
#define EXIFTAG_SPECIALEFFECTSIFD			50030
#define EXIFTAG_BORDERSIFD					50031

//******************* EXIF2.0 PRIVARTE Tag Definitions.*************************************
#define	EXIFTAG_COPYRIGHT					33432
#define	EXIFTAG_GPSINFO						34853

#define EXIFTAG_EXPOSURETIME 				33434
#define EXIFTAG_FNUMBER 					33437
#define EXIFTAG_EXPOSUREPROGRAM 			34850
#define EXIFTAG_SPECTRALSENSITIVITY 		34852
#define EXIFTAG_ISOSPEEDRATINGS 			34855
#define EXIFTAG_OECF 						34856

#define EXIFTAG_EXIFVERSION 				36864
#define EXIFTAG_DATETIMEORIGINAL 			36867
#define EXIFTAG_DALETIMEDIGITIZED 			36868

#define EXIFTAG_COMPONENTSCONFIGURATION 	37121
#define EXIFTAG_COMPRESSEDBITSPERPIXEL 		37122
#define EXIFTAG_SHUTTERSPEEDVALUE 			37377
#define EXIFTAG_APERTUREVALUE 				37378
#define EXIFTAG_BRIGHTNESSVALUE 			37379
#define EXIFTAG_EXPOSUREBIASVALUE 			37380
#define EXIFTAG_MAXAPERTUREVALUE 			37381
#define EXIFTAG_SUBJECTDISTANCE 			37382
#define EXIFTAG_METERINGMODE 				37383
#define EXIFTAG_LIGHTSOURCE 				37384
#define EXIFTAG_FLASH 						37385
#define EXIFTAG_FOCALLENGTH 				37386

#define EXIFTAG_MAKERNOTE 					37500
#define EXIFTAG_USERCOMMENT 				37510
#define EXIFTAG_SUBSECTIME 					37520
#define EXIFTAG_SUBSECTIMEORIGINAL 			37521
#define EXIFTAG_SUBSECTIMEDIGITIZED 		37522

#define EXIFTAG_FLASHPIXVERSION 			40960
#define EXIFTAG_COLORSPACE 					40961
#define EXIFTAG_PIXELXDIMENSION 			40962
#define EXIFTAG_PIXELYDIMENSION 			40963
#define EXIFTAG_RELATEDSOUNDFILE			40964

#define EXIFTAG_FLASHENERGY 				41483
#define EXIFTAG_SPATIALFREQUENCYRESPONSE 	41484
#define EXIFTAG_FOCALPLANEXRESOLUTION 		41486
#define EXIFTAG_FOCALPLANEYRESOLUTION 		41487
#define EXIFTAG_FOCALPLANERESOLUTIONUNIT 	41488
#define EXIFTAG_SUBJECTLOCATION 			41492
#define EXIFTAG_EXPOSUREINDEX 				41493
#define EXIFTAG_SENSINGMETHODE 				41495

#define EXIFTAG_FILESOURCE 					41728
#define EXIFTAG_SCENETYPE 					41729
#define EXIFTAG_CFAPATTERN 					41730

//******************* INTEROPERABILITY Tag Definitions.*************************************
#define EXIFTAG_INTEROPERABILITYIFDPTR	    40965
#define EXIFTAG_INTEROPERABILITYINDEX	        1
#define EXIFTAG_INTEROPERABILITYVERSION 	    2

//******************* GPS INFO. Tag Definitions.*************************************
#define EXIFTAG_GPSVERSIONID				0
#define EXIFTAG_GPSLATITUDEREF				1
#define EXIFTAG_GPSLATITUDE					2				
#define EXIFTAG_GPSLONGITUDEREF				3
#define EXIFTAG_GPSLONGITUDE				4
#define EXIFTAG_GPSALTITUDEREF				5
#define EXIFTAG_GPSALTITUDE					6
#define EXIFTAG_GPSTIMESTAMP				7
#define EXIFTAG_GPSSATELLITES				8
#define EXIFTAG_GPSSTATUS					9
#define EXIFTAG_GPSMESUREMODE				10
#define EXIFTAG_GPSDOP						11
#define EXIFTAG_GPSSPEEDREF					12
#define EXIFTAG_GPSSPEED					13
#define EXIFTAG_GPSTRACKREF					14
#define EXIFTAG_GPSTRACK					15
#define EXIFTAG_GPSIMGDIRECTIONREF			16
#define EXIFTAG_GPSIMGDIRECTION				17
#define EXIFTAG_GPSMAPDATUM					18
#define EXIFTAG_GPSDESTLATITUDEREF			19
#define EXIFTAG_GPSDESTLATITUDE				20
#define EXIFTAG_GPSDESTLONGITUDEREF			21
#define EXIFTAG_GPSDESTLONGITUDE			22
#define EXIFTAG_GPSDESTBEARINGREF			23
#define EXIFTAG_GPSDESTBEARING				24
#define EXIFTAG_GPSDISTANCEREF				25
#define EXIFTAG_GPSDISTANCE					26

//******************* PictureCD APP3 Tag Definitions.*********************************
#define EXIFTAG_FILMPRODUCTCODE				50000
#define EXIFTAG_IMAGESOURCE					50001
#define EXIFTAG_PRINTAREA					50002
#define EXIFTAG_CAMERAOWNER					50003
#define EXIFTAG_CAMERASERIALNUM				50004
#define EXIFTAG_GROUPCAPTION				50005
#define EXIFTAG_DEALERID					50006
#define EXIFTAG_ORDERID						50007
#define EXIFTAG_BAGNUM						50008
#define EXIFTAG_SCANFRAMESEQNUM				50009
#define EXIFTAG_FILMCATEGORY				50010
#define EXIFTAG_FILMGENCODE					50011
#define EXIFTAG_SCANSOFTWARE				50012
#define EXIFTAG_FILMSIZE					50013
#define EXIFTAG_SBARGBSHIFTS				50014
#define EXIFTAG_SBAINPUTCOLOR				50015
#define EXIFTAG_SBAINPUTBITDEPTH			50016
#define EXIFTAG_SBAEXPOSUREREC				50017
#define EXIFTAG_USERSBARGBSHIFTS			50018
#define EXIFTAG_IMAGEROTATIONSTATUS			50019
#define EXIFTAG_ROLLGUID					50020
#define EXIFTAG_APP3VERSION					50021

#define EXIF_HEADER_LEN 6
//   --------------------------------------------------------------------------
typedef	struct {
	unsigned short	magic;	    // magic number (defines byte order)
	unsigned short	version;	// TIFF version number
	unsigned long	diroff;	    // byte offset to first directory
} TiffHeader, * PTiffHeader;

typedef	struct {
	unsigned short  tag;	
	unsigned short  type;	
	unsigned long   count;	
	unsigned long   offset;
} IFDEntry, * PIFDEntry;

typedef	struct {
	unsigned long    offset;
	IFDEntry         entry;
    	char             *ascii;
    	UINT32 rationalH;
    	UINT32 rationalL; 
    	UINT32          srationalH; 
    	UINT32          srationalL; 
} IFDData, * PIFDData;


UINT32   createExif(UINT8 * pExifHeader, UINT8 * pThumbnail, UINT32 ThumbnailLen);
UINT32   writeExifAPP1(UINT8 * pMarkerHead, UINT8* pThumbnail,UINT32 ThumbnailLen);
void  put1Bytes(UINT8 *array, UINT32 offset, UINT8 value);
void  put2Bytes(UINT8*array, UINT32 offset, UINT16 value);
void put4Bytes(UINT8*array, UINT32 offset, UINT32 value);
UINT32  Get4Bytes(UINT8 *pt, UINT8 IsBigEndian);
UINT16  Get2Bytes(UINT8 *pt, UINT8 IsBigEndian);
void GetIFDEntry(UINT8* pt, PIFDEntry entry, UINT8 IsBigEndian);

void createIFDdata(UINT8 * pTiffHeader, PIFDData pIfdData);
void modifyJPEGIFBYTECOUNT(UINT32 jfiflen);
UINT32 GetExifApp1HeaderLen(void);
UINT32 writeExifHeader(UINT8* pBuffer);




//   --------------------------------------------------------------------------
