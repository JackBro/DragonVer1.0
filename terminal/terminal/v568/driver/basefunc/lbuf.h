


#ifndef _RDK_LBUF_H_
#define _RDK_LBUF_H_

#ifdef __cplusplus
extern "C" {
#endif


//lbuf mode
enum V5B_LBUF_MODEenum
{
	V5B_LBUF_MODE_VIEWFINDER		= 0x0,
	V5B_LBUF_MODE_CAPTURE			= 0x1,
	V5B_LBUF_MODE_DISPLAY			= 0x2,
	V5B_LBUF_MODE_DECODER_NORM		= 0x4,
	V5B_LBUF_MODE_DECODER_IPP		= 0xc,
	V5B_LBUF_MODE_DECODER_IPP_LCD	= 0x14
};

//lbuf internal ctrl mode
enum V5B_LBUF_INTER_CTRLenum
{
	V5B_LBUF_INTERCTRL_RESET		= BIT0,
	V5B_LBUF_INTERCTRL_ENCPREF		= BIT1,
	V5B_LBUF_INTERCTRL_DECPREF		= BIT2,
	V5B_LBUF_INTERCTRL_RGBSEL		= BIT3,
	V5B_LBUF_INTERCTRL_DECBYPASS	= BIT4
};

UINT8	V5B_LbufGetStatus(void);
UINT8	V5B_Lbuf0Isok(void);
UINT8	V5B_Lbuf1Isok(void);
UINT8	V5B_LbufIsHblankDone(void);
UINT8	V5B_LbufGetLinenum(void);

void	V5B_LbufSetYuvModeCtrl(UINT8 ctrl);
UINT8	V5B_LbufGetYuvModeCtrl(void);
void	V5B_LbufSetYuvMode(UINT8 mode);
UINT8	V5B_LbufGetYuvMode(void);
UINT8	V5B_LbufSetDataFromLcd(void);
UINT8	V5B_LbufSetDataFromIpp(void);
void	V5B_LbufSetPixrate(UINT8 val);
UINT8	V5B_LbufGetPixrate(void);

void	V5B_LbufSetLbufModeCtrl(UINT8 ctrl);
UINT8	V5B_LbufGetLbufModeCtrl(void);
UINT8	V5B_LbufVsyncDelayEnable(void);
UINT8	V5B_LbufVsyncDelayDisable(void);
void	V5B_LbufSetMode(UINT8 mode);
UINT8	V5B_LbufGetMode(void);

void	V5B_LbufSetInterCtrl(UINT8 val);
UINT8	V5B_LbufGetInterCtrl(void);

UINT8	V5B_LbufCounterReset(void);
UINT8	V5B_LbufDecBypEnable(void);
UINT8	V5B_LbufDecBypDisable(void);
UINT8	V5B_LbufDec2Yuv422(void);
UINT8	V5B_LbufDec2Rgb565(void);
UINT8	V5B_LbufDecPreEnable(void);
UINT8	V5B_LbufDecPreDisable(void);
UINT8	V5B_LbufEncPreEnable(void);
UINT8	V5B_LbufEncPreDisable(void);

void	V5B_LbufSetWidth(UINT16 val);
UINT16	V5B_LbufGetWidth(void);

void	V5B_LbufSetHblank(UINT16 val);
UINT16	V5B_LbufGetHblank(void);

void	V5B_LbufSetAddrInMarb(UINT32 val);
UINT32	V5B_LbufGetAddrInMarb(void);
UINT32	V5B_LbufGetBufAddr(void);

void	V5B_LbufSetCapDlyCtrl(UINT16 val);
UINT16	V5B_LbufGetCapDlyCtrl(void);
UINT16	V5B_LbufCapDlyEnable(void);
UINT16	V5B_LbufCapDlyDisable(void);
void	V5B_LbufSetCapDly(UINT16 val);
UINT16	V5B_LbufGetCapDly(void);


#ifdef __cplusplus
}
#endif

#endif


