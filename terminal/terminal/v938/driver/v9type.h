#ifndef _RDK_V9TYPE_H_
#define _RDK_V9TYPE_H_


typedef void (*PV9IsrCallback)(void);

//interrupt number
#define V9_INT_NUM 21

	// Chip control information
	typedef struct tag_TCtrlInfo938 {
		UINT8	bustype; //Bus type
		UINT8	intprop; //Interrupt control: lever or edge, highor low, width
		UINT32  mclk;
	} TCtrlInfo938, *PTCtrlInfo938;

	typedef struct tag_TCtrlContext938 {
		UINT8		               state;
		UINT8		               mode;
		UINT16		               intoccurnum;
		PV9IsrCallback             intisr[V9_INT_NUM];
		UINT32	                   clkin; //Input clock
		UINT32	                   mclk; //Chip work clock
		UINT16                     eFifoLen; //  EFIFO length
		UINT16                     sFifoLen; //  SFIFO length 
		UINT16                     cFifoLen; //  CFIFO length 
		PTCtrlInfo938              pinputinfo;
	} TCtrlContext938, *PTCtrlContext938;

	// Convert information,   maybe no use
	typedef struct tag_TConvInfo{
		UINT8	buf1WMark; //water mark of Buffer 1
		UINT16	buf1Addr;  //SEQ buffer 1 start address
		UINT16	buf2Addr;  //SEQ buffer 2 start address
		UINT16	buf2CmdAddr;  //command buffer 2 start address
		UINT32	v8051Addr;  //command buffer 2 start address
	} TConvInfo, *PTConvInfo;

	// Audio volume,   maybe no use
	typedef struct tag_TAudioVolInfo{
		UINT8	volMidL;   //Left volume of MIDI
		UINT8	volMidR;   //Right volume of MIDI
		UINT8	volWavL;   //Left volume of wave
		UINT8	volWavR;   //Right volume of wave
		UINT8	hpnVolL;   //Left channel PANT out gain of DAC
		UINT8	hpnVolR;   //Right channel PANT out gain of DAC
		UINT8   dacPgg;    //Mixer output gain 
	} TAudioVolInfo, *PTAudioVolInfo;

	// Audio SFIFO water mark and write base/end address
	typedef struct tag_TAudioSFifoInfo{
		UINT8	wm;          //Water mark
		UINT16	wBaseAddr;   //Write base address
		UINT16	wEndAddr;   //Write end address
	} TAudioSFifoInfo, *PTAudioSFifoInfo;

	typedef struct tag_TV9Context {
		UINT8	                   mode;
	} TV9Context, *PTV9Context;

	typedef struct tag_TV9TransBufContext {
		UINT8	                   *pBuf;
		UINT32                     bufSize;
        UINT32                     curRdPos;
		UINT32                     realEndPos;
	} TV9TransBufContext, *PTV9TransBufContext;


#endif
