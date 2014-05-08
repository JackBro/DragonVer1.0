


#ifndef _TRM_PANELDBUG_H_
#define _TRM_PANELDBUG_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*PPanelDbgW)(UINT16 val);
typedef UINT16 (*PPanelDbgR)(void);

typedef struct tag_TPanelDbgContext
{
	TPoint		biaspt;
	TSize		panelsize;
	PPanelDbgW	RsLW;		//Write to panel when RS low
	PPanelDbgW	RsHW;		//Write to panel when RS high
	PPanelDbgR	RsLR;		//Read to panel when RS low
	PPanelDbgR	RsHR;		//Read to panel when RS high
} TPanelDbgContext, *PTPanelDbgContext;

void	PanelDbgInitContext(void);
void	PanelDbgSetMode(UINT8 mode);
void	PanelSetReg(UINT32 adr, UINT16 val);
UINT16  PanelGetReg(UINT32 adr);
void	PanelWriteGram(TPoint pt, UINT16 val, UINT16 len);
void	PanelWriteLine(UINT16 line, UINT16 val);
void	PanelWriteImage(UINT16 *buf, UINT32 len);


#ifdef __cplusplus
}
#endif

#endif


