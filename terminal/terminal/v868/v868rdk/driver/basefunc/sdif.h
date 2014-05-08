


#ifndef _RDK868_SDIF_H_
#define _RDK868_SDIF_H_

#ifdef __cplusplus
extern "C" {
#endif

	enum SDIFSTOMODEenum {
		SD_DMAMODE	=	BIT0,
		SD_SPIEN	=	BIT1,
		SD_TRANS4	=	BIT2
	};

	enum SDIFSOFTRSTenum {
		SD_REG_RST	   =    BIT0,
		SD_CTRL_RST	   =    BIT1,
		SD_BUF_CLR	   =    BIT3,
		SD_HOST_BUF_RD =    BIT4
	};

	enum SDIF_CMDCTRL1enum {
		SD_NORESP       =   0x0,
		SD_RESP136      =   0x1,
		SD_RESP48       =   0x2, 
		SD_RESP48BUSY   =   0x3,
		SD_CHKCRCEN		=	BIT2,
		SD_CHKINXEN		=	BIT3,
		SD_RDDIRC		=	BIT4,
		SD_DATAPRENT	=	BIT5,

		SD_AUTOCMD12EN	=	BIT7
	};

	enum SDIF_DMACTRLenum {
		SD_TRANSTOP		=	BIT0,
		SD_DMASTART		=	BIT1
	};

	enum SDIF_SDCTRLSTATEenum {
		SD_CTRL_IDEL,
		SD_CTRL_CMD,
		SD_CTRL_RESP,
		SD_CTRL_DATA,
		SD_CTRL_CMD12,
		SD_CTRL_RESP12
	};

	enum SDIF_PRESENTSTATEenum {
		SD_CMDLINE_INHIBIT	=	BIT0,
		SD_DATLINE_INHIBIT	=	BIT1,
		SD_DATLINE_ACTIVE	=	BIT2,
		SD_WRTRANS_ACTIVE	=	BIT5,
		SD_RDTRANS_ACTIVE	=	BIT6,
		SD_BUFWR_EN			=	BIT7,
		SD_BUFRD_EN			=	BIT8,
		SD_CARDDECPIN		=	BIT9,
		SD_ENABLEWR			=	BIT10,
		SD_DAT0LINE			=	BIT11,
		SD_DAT1LINE			=	BIT12,
		SD_DAT2LINE			=	BIT13,
		SD_DAT3LINE			=	BIT14,
		SD_CMDLINE			=	BIT15
	};

	enum SDIF_NORINTSTATUSenum {
		SD_CMDCOMPLETE	=	BIT0,
		SD_TRANCOMPLETE	=	BIT1,
		SD_DMAFINISH	=	BIT2,
		SD_CARDEVENT	=	BIT3,
		SD_ERR2OCCUR	=	BIT6,
		SD_ERR1OCCUR	=	BIT7
	};

	enum SDIF_ERRINT1STATUSenum {
		SD_WRCRCBUSY_TMOUT	=	BIT0,
		SD_RESPR1B_TMOUT	=	BIT1,
		SD_WRCRC_TMOUT		=	BIT2,
		SD_RDDAT_TMOUT		=	BIT3,
		SD_RESPSTART_TMOUT	=	BIT4,
		SD_HDATCRC_ERR		=	BIT5,
		SD_HRESPCRC_ERR		=	BIT6,
		SD_DATWRCRC_ERR		=	BIT7
	};

	enum SDIF_ERRINT2STATUSenum {
		SD_CMD_TMOUT		=	BIT0,
		SD_CMD12CRC_ERR		=	BIT1,
		SD_CMD12ENDB_ERR	=	BIT2,
		SD_CMD12INDEX_ERR	=	BIT3,
		SD_CMDENDB_ERR		=	BIT4,
		SD_CMDINDEX_ERR		=	BIT5,
		SD_DATENDB_ERR		=	BIT6,
		SD_HOSTRESP_STATUS	=	BIT7
	};




void	V8B_SdifSetStoMode(UINT8 x);
UINT8	V8B_SdifGetStoMode(void);
UINT8	V8B_SdifEnableDmaMode(void);
UINT8	V8B_SdifDisableDmaMode(void);
UINT8	V8B_SdifEnableSpi(void);
UINT8	V8B_SdifDisableSpi(void);
UINT8	V8B_SdifEnableTrans4(void);
UINT8	V8B_SdifDisableTrans4(void);

//===========================================
void	V8B_SdifSoftReset(UINT8 x);
void	V8B_SdifRegReset(void);
void	V8B_SdifCtrlReset(void);
void	V8B_SdifBufClear(void);
void    V8B_SdifSetHostBufRd(void);
UINT8   V8B_SdifGetHostBufRd( void );

//===========================================
void V8B_SdifSetBlkSize(UINT16 x);
UINT16 V8B_SdifGetBlkSize(void);

//===========================================
void	V8B_SdifSetBlkCnt(UINT16 x);
UINT16	V8B_SdifGetBlkCnt(void);

//===========================================
void	V8B_SdifSetCmdArgu(UINT32 x);
UINT32	V8B_SdifGetCmdArgu(void);

//===========================================
void	V8B_SdifSetCmdCtrl0(UINT8 x);
UINT8	V8B_SdifGetCmdCtrl0(void);
void	V8B_SdifSetCmdIndex(UINT8 sel);
UINT8	V8B_SdifGetCmdIndex(void);

//===========================================
void	V8B_SdifSetCmdCtrl1(UINT8 x);
UINT8	V8B_SdifGetCmdCtrl1(void);
void	V8B_SdifSetRespType(UINT8 sel);
UINT8	V8B_SdifGetRespType(void);
UINT8	V8B_SdifEnableCrcChk(void);
UINT8	V8B_SdifDisableCrcChk(void);
UINT8	V8B_SdifEnableInxChk(void);
UINT8	V8B_SdifDisableInxChk(void);
UINT8	V8B_SdifEnableReadDirc(void);
UINT8	V8B_SdifDisableReadDirc(void);
UINT8	V8B_SdifEnableDataPresent(void);
UINT8	V8B_SdifDisableDataPresent(void);
UINT8	V8B_SdifEnableAutoCmd12(void);
UINT8	V8B_SdifDisableAutoCmd12(void);
//===========================================
void	V8B_SdifGetResp(UINT8 *x);

//===========================================
void	V8B_SdifWrPort(UINT32 x);
UINT32	V8B_SdifRdPort(void);

//===========================================
void	V8B_SdifSetDmaCtrl(UINT8 x);
UINT8	V8B_SdifGetDmaCtrl(void);
UINT8	V8B_SdifGetCtrlState(void);
UINT8	V8B_SdifEnableDmaStart(void);
UINT8	V8B_SdifDisableDmaStart(void);
UINT8	V8B_SdifEnableTranStop(void);
UINT8	V8B_SdifDisableTranStop(void);

//===========================================
UINT16	V8B_SdifGetPresentState(void);

//===========================================

void	V8B_SdifSetClkCtrl(UINT8 x);
UINT8	V8B_SdifGetClkCtrl(void);

//===========================================
UINT16	V8B_SdifGetNorIntStatus(void);

//===========================================
UINT16	V8B_SdifGetErr1IntStatus(void);

//===========================================
UINT16	V8B_SdifGetErr2IntStatus(void);

//===========================================
void	V8B_SdifSetNorIntMask(UINT8 x);
UINT8	V8B_SdifGetNorIntMask(void);

//===========================================
void	V8B_SdifSetErr1IntMask(UINT8 x);
UINT8	V8B_SdifGetErr1IntMask(void);

//===========================================
void	V8B_SdifSetErr2IntMask(UINT8 x);
UINT8	V8B_SdifGetErr2IntMask(void);

//===========================================
void	V8B_SdifSetRespTmout(UINT8 x);
UINT8	V8B_SdifGetRespTmout(void);

//===========================================
void	V8B_SdifSetBusyTmout(UINT8 x);
UINT8	V8B_SdifGetBusyTmout(void);

//===========================================
void	V8B_SdifSetBusyCrcTmout(UINT8 x);
UINT8	V8B_SdifGetBusyCrcTmout(void);

//===========================================
void	V8B_SdifSetWrCrcTmout(UINT8 x);
UINT8	V8B_SdifGetWrCrcTmout(void);

//===========================================
void	V8B_SdifSetRdTmout(UINT8 x);
UINT8	V8B_SdifGetRdTmout(void);

//===========================================
UINT8	V8B_SdifGetCardAct(void);

//===========================================
void	V8B_SdifSetRdSbCmdIndex(UINT8 x);
UINT8	V8B_SdifGetRdSbCmdIndex(void);

//===========================================
void	V8B_SdifSetRdMbCmdIndex(UINT8 x);
UINT8	V8B_SdifGetRdMbCmdIndex(void);

//===========================================
void	V8B_SdifSetWrSbCmdIndex(UINT8 x);
UINT8	V8B_SdifGetWrSbCmdIndex(void);

//===========================================
void	V8B_SdifSetWrMbCmdIndex(UINT8 x);
UINT8	V8B_SdifGetWrMbCmdIndex(void);

//===========================================
void	V8B_SdifSetIdRdRespDma(UINT32 x);
UINT32	V8B_SdifGetIdRdRespDma(void);

//===========================================
void	V8B_SdifSetIdWrRespDma(UINT32 x);
UINT32	V8B_SdifGetIdWrRespDma(void);

//===========================================
void	V8B_SdifSetCrcErrThrd(UINT8 x);
UINT8	V8B_SdifGetCrcErrThrd(void);



#ifdef __cplusplus
}
#endif

#endif //_RDK_BIU_H_

