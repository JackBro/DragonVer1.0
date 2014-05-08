
#ifndef _LCDIF_H_
#define _LCDIF_H_
#include "ex_OldCmodel.h"



#include "chipbase.h"


#define REG_LCD_HSA                         0x00
#define REG_LCD_HEA                         0x02
#define REG_LCD_HSXA                        0x04
#define REG_LCD_HSYA                        0x06
#define REG_LCD_HEXA                        0x08
#define REG_LCD_HEYA                        0x0A
#define REG_LCD_WRI                         0x0C
#define REG_LCD_LC                          0x0E
#define REG_LCD_DCE                         0x10
#define REG_LCD_LCDRST                      0x12
#define REG_LCD_LCDTEST                     0x13
#define REG_LCD_HDP                         0x16
#define REG_LCD_VDP                         0x18
#define REG_LCD_AX                          0x1A
#define REG_LCD_AY                          0x1C
#define REG_LCD_AW                          0x1E
#define REG_LCD_AH                          0x20
#define REG_LCD_ADX0                        0x22
#define REG_LCD_ADY0                        0x24
#define REG_LCD_AMW                         0x26
#define REG_LCD_AMH                         0x28
#define REG_LCD_BX                          0x2A
#define REG_LCD_BY                          0x2C
#define REG_LCD_BW                          0x2E
#define REG_LCD_BH                          0x30
#define REG_LCD_BDX0                        0x32
#define REG_LCD_BDY0                        0x34
#define REG_LCD_BDX1                        0x36
#define REG_LCD_BDY1                        0x38
#define REG_LCD_BMW                         0x3A
#define REG_LCD_BMH                         0x3C
#define REG_LCD_ATCB                        0x3E
#define REG_LCD_ATCG                        0x3F
#define REG_LCD_ATCR                        0x40
#define REG_LCD_ATCF                        0x41
#define REG_LCD_BTCB                        0x42
#define REG_LCD_BTCG                        0x43
#define REG_LCD_BTCR                        0x44
#define REG_LCD_BTCF                        0x45
#define REG_LCD_LPM                         0x46
#define REG_LCD_BMODE                       0x48
#define REG_LCD_DMAP                        0x4A
#define REG_LCD_DPAD                        0x4C
#define REG_LCD_WCONT                       0x4E
#define REG_LCD_RCONT                       0x50
#define REG_LCD_BM                          0x52
#define REG_LCD_DRS                         0x54
#define REG_LCD_BGDR                        0x56
#define REG_LCD_BGDG                        0x57
#define REG_LCD_BGDB                        0x58
#define REG_LCD_DM                          0x5A
#define REG_LCD_MODE                        0x5C
#define REG_LCD_EXWEN                       0x5E
#define REG_LCD_EXW0                        0x60
#define REG_LCD_EXW1                        0x62
#define REG_LCD_EXR0                        0x64
#define REG_LCD_EXRS                        0x66
#define REG_LCD_IST                         0x68
#define REG_LCD_IW                          0x6A
#define REG_LCD_IA                          0x6C
#define REG_LCD_EXW1                        0x62
#define REG_LCD_EXR0                        0x64
#define REG_LCD_EXRS                        0x66
#define REG_LCD_IST                         0x68
#define REG_LCD_IW                          0x6A
#define REG_LCD_IA                          0x6C
#define REG_LCD_IDB                         0x6E
#define REG_LCD_IDG                         0x6F
#define REG_LCD_IDR                         0x70
#define REG_LCD_OM                          0x72
#define REG_LCD_UPDATE                      0x74
#define REG_LCD_INDEXSEL                    0x76
#define REG_LCD_IR                          0x78
#define REG_LCD_IRDB                        0x7A
#define REG_LCD_IRDG                        0x7B
#define REG_LCD_IRDR                        0x7C

#define REG_LCD_GLTR0                       0x8C
#define REG_LCD_GLTR1                       0x8D
#define REG_LCD_GLTR2                       0x8E
#define REG_LCD_GLTR3                       0x8F
#define REG_LCD_GLTR4                       0x90
#define REG_LCD_GLTR5                       0x91
#define REG_LCD_GLTR6                       0x92
#define REG_LCD_GLTR7                       0x93
#define REG_LCD_GLTR8                       0x94
#define REG_LCD_GLTR9                       0x95
#define REG_LCD_GLTR10                      0x96
#define REG_LCD_GLTR11                      0x97
#define REG_LCD_GLTR12                      0x98
#define REG_LCD_GLTR13                      0x99
#define REG_LCD_GLTR14                      0x9A
#define REG_LCD_GLTR15                      0x9B
#define REG_LCD_GLTR16                      0x9C

#define REG_LCD_GLTG0                       0x9E
#define REG_LCD_GLTG1                       0x9F
#define REG_LCD_GLTG2                       0xA0
#define REG_LCD_GLTG3                       0xA1
#define REG_LCD_GLTG4                       0xA2
#define REG_LCD_GLTG5                       0xA3
#define REG_LCD_GLTG6                       0xA4
#define REG_LCD_GLTG7                       0xA5
#define REG_LCD_GLTG8                       0xA6
#define REG_LCD_GLTG9                       0xA7
#define REG_LCD_GLTG10                      0xA8
#define REG_LCD_GLTG11                      0xA9
#define REG_LCD_GLTG12                      0xAA
#define REG_LCD_GLTG13                      0xAB
#define REG_LCD_GLTG14                      0xAC
#define REG_LCD_GLTG15                      0xAD
#define REG_LCD_GLTG16                      0xAE

#define REG_LCD_GLTB0                       0xB0
#define REG_LCD_GLTB1                       0xB1
#define REG_LCD_GLTB2                       0xB2
#define REG_LCD_GLTB3                       0xB3
#define REG_LCD_GLTB4                       0xB4
#define REG_LCD_GLTB5                       0xB5
#define REG_LCD_GLTB6                       0xB6
#define REG_LCD_GLTB7                       0xB7
#define REG_LCD_GLTB8                       0xB8
#define REG_LCD_GLTB9                       0xB9
#define REG_LCD_GLTB10                      0xBA
#define REG_LCD_GLTB11                      0xBB
#define REG_LCD_GLTB12                      0xBC
#define REG_LCD_GLTB13                      0xBD
#define REG_LCD_GLTB14                      0xBE
#define REG_LCD_GLTB15                      0xBF
#define REG_LCD_GLTB16                      0xC0



class OLDCMODEL_DLL CLcdIF : public CChipBase1
{
public:
	CLcdIF(void);
	virtual ~CLcdIF(void);

	void	SetReg(int adr, int val);
	void	RegInit(void);
//	int		Process(ImageStream *pIn, ImageStream *pOut);

public:

protected:

};

#endif

