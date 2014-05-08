#ifndef __DEF_H__
#define __DEF_H__

#ifdef __cplusplus
extern "C" {
#endif

//----Data type definition 
#ifndef SINT8
#define SINT8 signed char
#endif

#ifndef UINT8
#define UINT8 unsigned char
#endif

#ifndef SINT16
#define SINT16 signed short
#endif

#ifndef UINT16
#define UINT16 unsigned short
#endif

#ifndef SINT32
#define SINT32 signed long
#endif

#ifndef UINT32
#define UINT32 unsigned long
#endif
//---end of Data type definition 
#ifndef NULL
#define  NULL	((void *)0)
#endif

#ifndef LPVOID
#define  LPVOID void *
#endif

#ifndef TRUE
#define TRUE	1
#endif

#ifndef FALSE
#define FALSE	0
#endif

#ifndef OK
#define OK	1
#endif

#ifndef FAIL
#define FAIL	0
#endif

///////////////////////////////////////////////////////////////////////////////////////////
//------define v558 bus type----------------------
#define V558_BUS_SEPERATE		1
#define V558_BUS_MULTI8		2
#define V558_BUS_MULTI16		3
//Define current bus type
#define V558_BUS_TYPE V558_BUS_MULTI8

//------define sensor type---------------------
#define SELECT_HYNIX7131GP_BAYER	0
#define SELECT_HYNIX7131GP_YUV		0
#define SELECT_FUJITSU_MBS03		1
#define SELECT_FUJITSU_MBS04_BAYER	0
#define SELECT_OV_7649_BAYER		0
#define SELECT_HYNIX7131R_BAYER		0
#define SELECT_MI360_BAYER		0
#define SELECT_MI360_YUV		0
#define SELECT_ALC4V403V_BAYER		0
#define SELECT_OV_7649_YUV		0
#define SELECT_TCM8210MD_YUV		0
#define SELECT_PIXEL2030K_BAYER		0
#define SELECT_PIXEL2030NC_BAYER    	0
#define SELECT_PIXART302BCW_BAYER   	0
#define SELECT_SAMSUNG437_BAYER		0
#define SELECT_SAMSUNG532_YUV		0
#define SELECT_STS502_BAYER		0
#define SELECT_SAMSUNG5K532_BAYER	0

//-----define LCD panel type-----------------
#define LTS180SH_H1I_X		1
#define LTS180S3_HF1		0
#define Truly_128160_16    0
#define BYD_128160         0
#define Jemiteck_J180S 	0
#define UG_12D181_M16		0
#define UG_12D181_M8		0
#define UG_120114A	0
#define LP_8937_A_M16		0
#define LP_8937_A_M8		0
#define CV90_K2817_SSD1781 0
#define WB_F1216V2_6FLWB    0

#define MGG1256DTSW_ZXRT		0
#define LMG_128FE9		0
//-----------auto flicker ----------------
#define AUTO_FLK 0 //0-disable,1-enable;
#define FREQ 0//0-50hz,1-60hz

//-------define mapping address ---------
//Seperate
#define INTER_ADDR_REGUIA		0x8000000
#define HOST_ADDR_REGUIA		0x8000300
#define HOST_ADDR_MEMUIA		0x8000301
#define HOST_ADDR_MEMLIA		0x8000302
#define HOST_ADDR_MEMLIA0		0x8000303


#define HOST_ADDR_MEMDATA       0x8000380
#define HOST_ADDR_MEMPORT		0x8000390


//Multi8,Muti16
#define MULTI8_ADDR_CTRL		0x8000000
#define MULTI8_ADDR_DATA		0x8000100

#define MULTI8_REG_PORT		0xb0
#define MULTI8_REG_WORDL		0xb2
#define MULTI8_REG_WORDH		0xb3
#define MULTI8_MEM_PORT		0xb4

#define MULTI16_ADDR_ADDR		0x8000000
#define MULTI16_ADDR_VAL		0x8000100
#define MULTI16_ADDR_VAL_L		0x8000100
#define MULTI16_ADDR_VAL_H	0x8000101


//Bypass
#define BYPASS_ADDR_LCDADDR	0x8000000
#define BYPASS_ADDR_LCDVAL	0x8000200
//--------end of V558 define---------------------


//------the list is for version control--------------------------------------------------
#if (SELECT_HYNIX7131GP_BAYER)
	#define SENSOR_TYPE 1
#endif
#if (SELECT_HYNIX7131GP_YUV)
	#define SENSOR_TYPE 2
#endif
#if (SELECT_FUJITSU_MBS03)
	#define SENSOR_TYPE 3
#endif
#if	(SELECT_OV_7649_BAYER)
	#define SENSOR_TYPE 4
#endif
#if (SELECT_HYNIX7131R_BAYER)
	#define SENSOR_TYPE 5
#endif
#if (SELECT_MI360_BAYER)
	#define SENSOR_TYPE 6
#endif
#if (SELECT_ALC4V403V_BAYER)
	#define SENSOR_TYPE 7
#endif
#if (SELECT_OV_7649_YUV)
	#define SENSOR_TYPE 8
#endif
#if (SELECT_TCM8210MD_YUV)
	#define SENSOR_TYPE 9
#endif
#if (SELECT_PIXEL2030K_BAYER)
	#define SENSOR_TYPE 10
#endif
#if (SELECT_PIXEL2030NC_BAYER)
	#define SENSOR_TYPE 11
#endif
#if(SELECT_PIXART302BCW_BAYER)
	#define SENSOR_TYPE 12
#endif

#if(SELECT_MI360_YUV)
	#define SENSOR_TYPE 13
#endif

#if(SELECT_FUJITSU_MBS04_BAYER)
	#define SENSOR_TYPE 14
#endif

#if(SELECT_SAMSUNG437_BAYER)
	#define SENSOR_TYPE 15
#endif


#if(SELECT_SAMSUNG532_YUV)
	#define SENSOR_TYPE 16
#endif

#if(SELECT_STS502_BAYER)
	#define SENSOR_TYPE 17
#endif

#if(SELECT_SAMSUNG5K532_BAYER)
	#define SENSOR_TYPE 18
#endif

//--------------------------------
#if  (LTS180SH_H1I_X)
	#define TwoPanel 0
	#define LCD_TYPE 1
#endif
#if (LTS180S3_HF1)
	#define TwoPanel 0
	#define LCD_TYPE 2
#endif

#if (Truly_128160_16)
	#define TwoPanel 1
	#define LCD_TYPE 4
#endif
#if (BYD_128160)
	#define TwoPanel 0
	#define LCD_TYPE 5
#endif
#if (Jemiteck_J180S)
	#define TwoPanel 0
	#define LCD_TYPE 6
#endif
#if (UG_12D181_M16)
	#define TwoPanel 0
	#define LCD_TYPE 7
#endif
#if (UG_12D181_M8)
	#define TwoPanel 0
	#define LCD_TYPE 8
#endif
#if (UG_120114A)
	#define TwoPanel 0
      #define LCD_TYPE 9
#endif
#if(LP_8937_A_M16)
	#define TwoPanel 0
      #define LCD_TYPE 10
#endif
#if(LP_8937_A_M8)
	#define TwoPanel 0
      #define LCD_TYPE 11
#endif
#if(CV90_K2817_SSD1781)
	#define TwoPanel 0
      #define LCD_TYPE 12
#endif
#if(WB_F1216V2_6FLWB)
	#define TwoPanel 1
      #define LCD_TYPE 13
#endif
#if(MGG1256DTSW_ZXRT)
	#define TwoPanel 1
      #define LCD_TYPE 14
#endif
#if(LMG_128FE9)
	#define TwoPanel 0
      #define LCD_TYPE 15
#endif
//--------------end of version control def----------------------

//typedef __packed struct
typedef  struct
{
	UINT8	bySignature_b1;		//Signature that helps identify this data packet as an OBW
	UINT8	bySignature_b2;		//Signature that helps identify this data packet as an OBW
	UINT8	bySignature_b3;		//Signature that helps identify this data packet as an OBW
	UINT8	bySignature_b4;		//Signature that helps identify this data packet as an OBW
	UINT8	byDataLength;			//Data Lenth.
	UINT8	byOpenrationCode;
	UINT8	byPara1_b1;
	UINT8	byPara1_b2;
	UINT8	byPara1_b3;
	UINT8	byPara1_b4;
	UINT8	byPara2_b1;
	UINT8	byPara2_b2;
	UINT8	byPara2_b3;
	UINT8	byPara2_b4;
}OBW, *pOBW;

//typedef __packed struct
typedef  struct
{
	UINT8	bySignature_b1;		//Signature that helps identify this data packet as an RSW
	UINT8	bySignature_b2;		//Signature that helps identify this data packet as an RSW
	UINT8	bySignature_b3;		//Signature that helps identify this data packet as an RSW
	UINT8	bySignature_b4;		//Signature that helps identify this data packet as an RSW
	UINT8	Status;				//Status indicates the success or failure of the operation.0 = Success1 = Failed2 = Phase Error
	UINT8	byPara1_b1;
	UINT8	byPara1_b2;
	UINT8	byPara1_b3;
	UINT8	byPara1_b4;
	UINT8	byPara2_b1;
	UINT8	byPara2_b2;
	UINT8	byPara2_b3;
	UINT8	byPara2_b4;
}RSW, *pRSW;

typedef enum
{
	BEG_STATE	= 0,
	R_STATE,
	M_STATE,
	C_STATE,
	CMDDATA_STATE,
	RECIVEDATA_STATE



}RAMCOMD_STATE;



////////////////////////////////////////////////

//OBW CMD

////////////////////////////////////////////////

typedef enum

{

	CMD_Undefined		= 0x0,



	//Chip command.

	CMD_VMVGET_REG		,

	CMD_VMVSET_REG		,

	CMD_VMVTRANS_DATA		,

	CMD_VMVWBUF2LAYERB	,

	CMD_VMVWBUF2JBUF		,

	CMD_VMVDEMO_BUTTON	,

	CMD_CAPSTILL			,

	CMD_STILLDONE			,

	CMD_CAPVIDEOSTART		,

	CMD_CAPVIDEOSTOP		,

	CMD_VMVWBUF2DEMOEXP     ,

	CMD_VMVWBUF2FLASH       ,

	CMD_VMVSRAMREAD         ,

	CMD_VMVSRAMWRITE        ,

	CMD_VMVCTRLINIT         ,

	CMD_VMVENCODE           ,

	CMD_VMVDECODE           ,

	CMD_VMVSETCAP           ,

	CMD_VMVSETDIS           ,

	CMD_VMVBYPASSSEL        ,

	CMD_VMVCAPSEND		,

	CMD_VMVGETFLK		,

	CMD_VMVPLAY



} ARMCMD;



typedef enum _TYPE_PLAY{

	TYPE_PLAY_Undef		=0x0,

	TYPE_PLAY_FLK			=0x1,

	TYPE_PLAY_LCD

}TYPE_PLAY;






/////////////////////////////////////////////////

//RSW CMD

/////////////////////////////////////////////////

typedef enum

{

	RMD_Undefined		=0x0,

	RMD_CAPDATA		=0x1,

	RMD_CAPVIDEO	=0x2,

	RMD_INT

}RSWCMD;



////////////////////////////////////////////////

//Reg type

////////////////////////////////////////////////

typedef enum _REGTYPE_VF{

	TYPE_Undef		=0x0,

	TYPE_Chip		,

	TYPE_Sensor		,

	TYPE_Byctrl		,

	TYPE_Bydata

}REGTYPE_VF;



typedef enum _VMV_MULTI_SEL

{

	VMV_MULTI_SEL8	= 0,

	VMV_MULTI_SEL16	= 1

}VMV_MULTI_SEL;





/* Sensor type */

#define V558_SNR_HY7131R		0

#define V558_SNR_TAS5130D1B	1

#define V558_SNR_PAS202B		2

#define V558_SNR_PAS302BCW		3

#define V558_SNR_MI360			4

#define V558_SNR_MI_SOC_343	5

#define V558_SNR_VF10			6

#define V558_SNR_MB86S03		7

#define V558_SNR_PO1030K		8

#define V558_SNR_OV7648		9

#define V558_SNR_LM9647		10

#define V558_SNR_LZ0P3714		11

#define V558_SNR_PIXART302BCW_BAYER 12

//Define current sensor type

//#define V558_SNR_TYPE V558_SNR_PAS202B



//#define V558_SENSOR_AE_ACESS V558_SENSOR_ACESS_IIC

/*jpeg define*/





#ifdef __cplusplus

}

#endif

#endif /*__DEF_H___*/





