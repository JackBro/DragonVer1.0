#ifndef _RDK868_VSDC_H_
#define _RDK868_VSDC_H_

#ifdef __cplusplus
extern "C" {
#endif

	enum VSDCCOMMANDOPTRSPenum {
		VSDC_NORMAL_COMPLETE                = 0x0,
		VSDC_WAIT_IDLE_TIMEOUT              = 0x1,
		VSDC_WAIT_RESPONSE_TIMEOUT          = 0x2,
		VSDC_WAIT_BUFFERWRITEEN_TIMEOUT     = 0x3,
		VSDC_WAIT_BUFFERREADEN_TIMEOUT      = 0x4,
        VSDC_CRC_ERROR                      = 0x5
	};

	enum VSDCSTATEenum {
		SDC_NOTINIT,
		SDC_INITED,
		SDC_OPENED,
		SDC_CLOSE
	};

	enum VSDCDELAYCOUNTenum {
		VSDC_WAIT_IDLE_DELAY_COUNT                    = 0x20,
		VSDC_WAIT_RESPONSE_DELAY_COUNT                = 0x20,
		VSDC_WAIT_BUFWRITEEN_DELAY_COUNT              = 0x100,
		VSDC_WAIT_BUFREADEN_DELAY_COUNT               = 0x100,
		VSDC_WAIT_HOSTBUFFERREADCLEAR_DELAY_COUNT     = 0x100
	};

    #define VSDC_BUFFER_WRITE_ENABLE         0x1
    #define VSDC_BUFFER_READ_ENABLE          0x1

	typedef struct tag_TSdcInfo {
		UINT8   stoMode;
		UINT8	option;
		UINT8	clkDivCnt;
		UINT8	reserve0;
		UINT16  blkSize;
		UINT16	rca;
		UINT32  ocr;
	} TSdcInfo, *PTSdcInfo;

	typedef struct tag_TSdcContext {
		UINT8	 option;
		UINT8	 state;
		UINT16	 reserve0;
		TSdcInfo *pSdcInfo;
	} TSdcContext, *PTSdcContext;


	//=========================
void         V8_SdcInitContext(void);
TSdcContext  *V8_SdcGetContext(void);
void         V8_SdcSetInfo(TSdcInfo *pInfo);
void         V8_SdcOpen(UINT8 stoOrMMC);
void         V8_SdcClose(void);
UINT8        V8_SdcSendCmd( UINT8 cmdIdx, UINT32 cmdArgu );
UINT8        V8_SdcSendACmd( UINT8 cmdIdx, UINT32 cmdArgu );
UINT32       V8_SdcHostWriteData( UINT32 addr, UINT8 *buf, UINT32 size );
UINT32       V8_SdcHostReadData( UINT32 addr, UINT8 *buf, UINT32 size );


#ifdef __cplusplus
}
#endif

#endif 


