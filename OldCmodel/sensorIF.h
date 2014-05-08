
#ifndef _SENSORIF_H_
#define _SENSORIF_H_
#include "ex_OldCmodel.h"



#include "chipbase.h"


#define REG_SIF_SNR_MODE		0x00
#define REG_SIF_SNROP_MODE		0x01
#define REG_SIF_YUV_FORMAT		0x02
#define REG_SIF_V_CONTROL		0x03
#define REG_SIF_H_CONTROL		0x04
#define REG_SIF_SNRCLK_CNT		0x05
#define REG_SIF_PIXRATE_SEL		0x06
#define REG_SIF_SICLK_FACTOR	0x07
#define REG_SIF_I2C_CLKCNTH		0x08
#define REG_SIF_I2C_CLKCNTL		0x09
#define REG_SIF_HD_STARTH		0x0C
#define REG_SIF_HD_STARTL		0x0D
#define REG_SIF_HD_STOPH		0x0E
#define REG_SIF_HD_STOPL		0x0F
#define REG_SIF_HSYNC_STARTH	0x10
#define REG_SIF_HSYNC_STARTL	0x11
#define REG_SIF_HSYNC_STOPH		0x12
#define REG_SIF_HSYNC_STOPL		0x13
#define REG_SIF_HREF_STARTH		0x14
#define REG_SIF_HREF_STARTL		0x15
#define REG_SIF_HREF_LENGTHH	0x16
#define REG_SIF_HREF_LENGTHL	0x17
#define REG_SIF_VD_STARTH		0x18
#define REG_SIF_VD_STARTL		0x19
#define REG_SIF_VD_STOPH		0x1A
#define REG_SIF_VD_STOPL		0x1B
#define REG_SIF_VSYNC_STARTH	0x1C
#define REG_SIF_VSYNC_STARTL	0x1D
#define REG_SIF_VSYNC_STOPH		0x1E
#define REG_SIF_VSYNC_STOPL		0x1F
#define REG_SIF_VREF_STARTH		0x20
#define REG_SIF_VREF_STARTL		0x21
#define REG_SIF_VREF_STOPH		0x22
#define REG_SIF_VREF_STOPL		0x23
#define REG_SIF_HDFALL_STARTH	0x24
#define REG_SIF_HDFALL_STARTL	0x25
#define REG_SIF_HDFALL_STOPH	0x26
#define REG_SIF_HDFALL_STOPL	0x27
#define REG_SIF_VDFALL_STARTH	0x28
#define REG_SIF_VDFALL_STARTL	0x29
#define REG_SIF_VDFALL_STOPH	0x2A
#define REG_SIF_VDFALL_STOPL	0x2B
#define REG_SIF_COLMAXH			0x2C
#define REG_SIF_COLMAXL			0x2D
#define REG_SIF_ROWMAXH			0x2E
#define REG_SIF_ROWMAXL			0x2F
#define REG_SIF_EXTIMEH			0x30
#define REG_SIF_EXTIMEL			0x31
#define REG_SIF_GAIN			0x32
#define REG_SIF_OBREG			0x33
#define REG_SIF_IIC_BYTE		0x34
#define REG_SIF_IIC_DEVADDR		0x35
#define REG_SIF_SNRWRDATAH		0x36
#define REG_SIF_SNRWRDATAM		0x37
#define REG_SIF_SNRWRDATAL		0x38
#define REG_SIF_SNRACSCTR		0x39
#define REG_SIF_SNRADDR			0x3A
#define REG_SIF_SNRSTATUS		0x3B
#define REG_SIF_SNRRDDATAH		0x3C
#define REG_SIF_SNRRDDATAM		0x3D
#define REG_SIF_SNRRDDATAL		0x3E
#define REG_SIF_BUS_STATE		0x3F
#define REG_SIF_AEWIN_STARTH	0x40
#define REG_SIF_AEWIN_STARTL	0x41
#define REG_SIF_AEWIN_STOPH		0x42
#define REG_SIF_AEWIN_STOPL		0x43
#define REG_SIF_AEPIN_CTRL		0x44
#define REG_SIF_AEBUS_CTRL		0x45
#define REG_SIF_B_CNTL			0x46
#define REG_SIF_B_CNTH			0x47
#define REG_SIF_AE0_DATA		0x48




class OLDCMODEL_DLL CSensorIF : public CChipBase1
{
public:
	CSensorIF(void);
	virtual ~CSensorIF(void);

	//void	SetReg(int adr, int val);
	//int	GetReg(int adr);
	void	RegInit(void);
	int		Process(ImageStream *pIn, ImageStream *pOut);
public:

protected:
	int		FillImageInfo(ImageStream *pIn, ImageStream *pOut);

private:

private:

};

#endif

