
#ifndef _JPEG_H_
#define _JPEG_H_
#include "ex_OldCmodel.h"



#include "chipbase.h"


#define REG_JPEG_MODE                       0x00
#define REG_JPEG_CTRL                       0x01
#define REG_JPEG_STATUS                     0x02
#define REG_JPEG_BRC                        0x03
#define REG_JPEG_TCR                        0x04
#define REG_JPEG_QF                         0x05

#define REG_JPEG_TARGET_WC0                 0x08
#define REG_JPEG_TARGET_WC1                 0x09
#define REG_JPEG_TARGET_WC2                 0x0A
#define REG_JPEG_TARGET_WC3                 0x0B

#define REG_JPEG_VIDEO_WC0                  0x0C
#define REG_JPEG_VIDEO_WC1                  0x0D
#define REG_JPEG_VIDEO_WC2                  0x0E
#define REG_JPEG_VIDEO_WC3                  0x0F

#define REG_JPEG_HUFF_WC0                   0x10
#define REG_JPEG_HUFF_WC1                   0x11
#define REG_JPEG_HUFF_WC2                   0x12
#define REG_JPEG_HUFF_WC3                   0x13

#define REG_JPEG_WIDTH_H                    0x14
#define REG_JPEG_WIDTH_L                    0x15
#define REG_JPEG_HEIGHT_H                   0x16
#define REG_JPEG_HEIGHT_L                   0x17

#define REG_JPEG_B_H                        0x18
#define REG_JPEG_B_L                        0x19
#define REG_JPEG_FRAME_CNT0                 0x1A
#define REG_JPEG_FRAME_CNT1                 0x1B

#define REG_JPEG_DCT_DATA                   0x1C
#define REG_JPEG_QUAN_T                     0x1D
#define REG_JPEG_HUFF_T                     0x1E
#define REG_JPEG_NUM_COMPQUANT              0x1F

#define REG_JPEG_COMP1_P1                   0x20
#define REG_JPEG_COMP1_P2                   0x21
#define REG_JPEG_COMP2_P1                   0x22
#define REG_JPEG_COMP2_P2                   0x23

#define REG_JPEG_COMP3_P1                   0x24
#define REG_JPEG_COMP3_P2                   0x25
#define REG_JPEG_COMP4_P1                   0x26
#define REG_JPEG_COMP4_P2                   0x27

#define REG_JPEG_BLK_WIDTH                  0x28

#define REG_JPEG_IMGPARA0                   0x30
#define REG_JPEG_IMGPARA1                   0x31
#define REG_JPEG_IMGPARA2                   0x32
#define REG_JPEG_IMGPARA3                   0x33
#define REG_JPEG_IMGPARA4                   0x34
#define REG_JPEG_IMGPARA5                   0x35
#define REG_JPEG_IMGPARA6                   0x36
#define REG_JPEG_IMGPARA7                   0x37
#define REG_JPEG_IMGPARA8                   0x38
#define REG_JPEG_IMGPARA9                   0x39
#define REG_JPEG_IMGPARA10                  0x3A
#define REG_JPEG_IMGPARA11                  0x3B
#define REG_JPEG_IMGPARA12                  0x3C
#define REG_JPEG_IMGPARA13                  0x3D
#define REG_JPEG_IMGPARA14                  0x3E


typedef struct tag_HuffValue
{
	int		value;
	int		length;
} HuffValue, *PHuffValue;

typedef struct tag_JpegTable 
{
	SINT32	sfblk[64];			/* DCT scale factor table */
	SINT32	invTable[256];		/* inv ROM table */
	SINT32	LQTable[64];		/* Lum quantization tables */
	SINT32	CQTable[64];		/* Chrom quantization tables */

	int		sftL[128];			/* Lum shift tables (compression or decompression) */
	int		sftC[128];			/* Chrom shift tables (compression or decompression) */
	SINT32	brcL[64];			/* Lum BRC tables (compression or decompression) */
	SINT32	brcC[64];			/* Chrom BRC tables (compression or decompression) */

	HuffValue Ldc[260];			/* Huffman encode table for Lum DC */	//12
	HuffValue Lac[260];			/* Huffman encode table for Lum AC */	//162
	HuffValue Cdc[260];			/* Huffman encode table for Chrom DC */	//12
	HuffValue Cac[260];			/* Huffman encode table for Chrom AC */	//162

} JpegTable, *PJpegTable;

enum Marker
{
	SOF = 0xFFC0,
    COM = 0xFFFE,
	DHT = 0xFFC4,
    DNL = 0xFFDC,
	DQT = 0xFFDB,
	DRI = 0xFFDD,
	RST = 0xFFD0,
	EOI = 0xFFD9,
	SOI = 0xFFD8,
	SOS = 0xFFDA,
	DQF = 0xFFE0
};


static inline void PutByte(UINT8 ** ppbBuf, int Data)
{
    **ppbBuf = (UINT8)Data;
    *ppbBuf += 1;
}

static inline void PutWord(UINT8 ** ppbBuf, int wData)
{
	PutByte(ppbBuf, (UINT8)(wData >> 8));
	PutByte(ppbBuf, (UINT8)wData);
}

static inline void PutLittleWord(UINT8 ** ppbBuf, int wData)
{
	PutByte(ppbBuf, (UINT8)wData);
	PutByte(ppbBuf, (UINT8)(wData >> 8));
}

static inline void PutData(UINT8 ** ppbBuf, UINT8 *pData, int len)
{
	memcpy(*ppbBuf, pData, len);
	*ppbBuf += len;
}


//================ class CJpegEncode =========

class OLDCMODEL_DLL CJpegEncode : public CChipBase1
{
public:
	CJpegEncode(void);
	int		Process(ImageBlock *pIn, ImageBlock *pOut);

protected:
	int		VsyncInit(ImageBlock *pIn);
	void	OutputBlock(ImageBlock *pIn, ImageBlock *pOut);
	int		FillImageInfo(ImageBlock *pIn, ImageBlock *pOut);
	int		AddOutLine(ImageBlock *pOut);
	void	AddInline(ImageBlock *pIn);

protected:
	void	DCTscaleFactor(SINT32 *sfblk);
	void	getInvTable(SINT32 *invTable);
	void	getQTable(SINT32 *sfblk, SINT32 *LQTable, SINT32 *CQTable, SINT32 *invTable, int Qfactor);
	int		getShiftValue(SINT32 *QTable);
	void	createHuffmanTable(UINT8 *bits, UINT8 *values, HuffValue *table);
	void	updateBRCTable(JpegTable *table, int qf);
	void	fpdct8x8(SINT32 *inblk, SINT32 *oublk);
	void	quant(SINT32 *inblk, SINT32 *oublk, SINT32 *brcTable, int sft);
	void	zigzag(SINT32 *inblk, SINT32 *oublk);
	void	encodeDC(UINT8 *buf, int *pos, SINT32 DCvalue, SINT32 *lastDC, HuffValue *pEncTable);
	void	encodeAC(UINT8 *buf, int *pos, SINT32 *zzblk, HuffValue *pEncTable);
	void	eofFlush(UINT8 *buf, int *pos, int length);

	SINT32	shiftFix(SINT32 input, int shift);
	SINT32	roundfc(SINT32 input);
	SINT32	rounddb(SINT32 input);
	SINT32	roundeb(SINT32 input);
	SINT32	roundgb(SINT32 input);
	SINT32	round12(SINT32 input);
	SINT32	round14(SINT32 input);
	SINT32	roundDouble(double input);
	
	void	putBits(UINT8 *buf, int *pos, HuffValue val);

	void	generateBRC(SINT32 *QTable, int sftC, SINT32 *output);
	void	InitJpegTable(JpegTable *table);
	void	generateJPEGHeader(ImageBlock *pIn);
	void	EncYBlock(UINT8 *block, SINT32 *lastDc, ImageBlock *pOut);
	void	EncUVBlock(UINT8 *block, SINT32 *lastDc, ImageBlock *pOut, int last = 0);

private:
	JpegTable	gTable;
	int			gqf;
	SINT32		glastYdc, glastUdc, glastVdc;
	UINT8		gLastChar;
	int			gStartPos;
	int			gLength;
	int			gVwc;
};

//====================== CJpeg =====================

class OLDCMODEL_DLL CJpeg : public CChipBase1
{
public:
	CJpeg(void);
	virtual ~CJpeg(void);

	void	SetReg(int adr, int val);
	void	RegInit(void);
	int		Process(ImageBlock *pLbuf, ImageBlock *pJbuf);

private:
	CJpegEncode gEncode;
};

#endif

