


#ifndef _VREGDEF_H_
#define _VREGDEF_H_

#ifdef __cplusplus
extern "C" {
#endif

/*SIF 0x00800*/
#define V558_REG_SIF_SNR_MODE		0x00800
#define V558_REG_SIF_SNROP_MODE		0x00801
#define V558_REG_SIF_YUV_FORMAT		0x00802
#define V558_REG_SIF_V_CONTROL		0x00803
#define V558_REG_SIF_H_CONTROL		0x00804
#define V558_REG_SIF_SNRCLK_CNT		0x00805
#define V558_REG_SIF_PIXRATE_SEL		0x00806
#define V558_REG_SIF_SICLK_FACTOR	0x00807
#define V558_REG_SIF_I2C_CLKCNTH		0x00808
#define V558_REG_SIF_I2C_CLKCNTL		0x00809
#define V558_REG_SIF_HD_STARTH		0x0080C
#define V558_REG_SIF_HD_STARTL		0x0080D
#define V558_REG_SIF_HD_STOPH		0x0080E
#define V558_REG_SIF_HD_STOPL		0x0080F
#define V558_REG_SIF_HSYNC_STARTH	0x00810
#define V558_REG_SIF_HSYNC_STARTL	0x00811
#define V558_REG_SIF_HSYNC_STOPH		0x00812
#define V558_REG_SIF_HSYNC_STOPL		0x00813
#define V558_REG_SIF_HREF_STARTH		0x00814
#define V558_REG_SIF_HREF_STARTL		0x00815
#define V558_REG_SIF_HREF_LENGTHH	0x00816
#define V558_REG_SIF_HREF_LENGTHL	0x00817
#define V558_REG_SIF_VD_STARTH		0x00818
#define V558_REG_SIF_VD_STARTL		0x00819
#define V558_REG_SIF_VD_STOPH		0x0081A
#define V558_REG_SIF_VD_STOPL		0x0081B
#define V558_REG_SIF_VSYNC_STARTH	0x0081C
#define V558_REG_SIF_VSYNC_STARTL	0x0081D
#define V558_REG_SIF_VSYNC_STOPH		0x0081E
#define V558_REG_SIF_VSYNC_STOPL		0x0081F
#define V558_REG_SIF_VREF_STARTH		0x00820
#define V558_REG_SIF_VREF_STARTL		0x00821
#define V558_REG_SIF_VREF_STOPH		0x00822
#define V558_REG_SIF_VREF_STOPL		0x00823
#define V558_REG_SIF_HDFALL_STARTH	0x00824
#define V558_REG_SIF_HDFALL_STARTL	0x00825
#define V558_REG_SIF_HDFALL_STOPH	0x00826
#define V558_REG_SIF_HDFALL_STOPL	0x00827
#define V558_REG_SIF_VDFALL_STARTH	0x00828
#define V558_REG_SIF_VDFALL_STARTL	0x00829
#define V558_REG_SIF_VDFALL_STOPH	0x0082A
#define V558_REG_SIF_VDFALL_STOPL	0x0082B
#define V558_REG_SIF_COLMAXH			0x0082C
#define V558_REG_SIF_COLMAXL			0x0082D
#define V558_REG_SIF_ROWMAXH			0x0082E
#define V558_REG_SIF_ROWMAXL			0x0082F
#define V558_REG_SIF_EXTIMEH			0x00830
#define V558_REG_SIF_EXTIMEL			0x00831
#define V558_REG_SIF_GAIN			0x00832
#define V558_REG_SIF_OBREG			0x00833
#define V558_REG_SIF_IIC_BYTE		0x00834
#define V558_REG_SIF_IIC_DEVADDR		0x00835
#define V558_REG_SIF_SNRWRDATAH		0x00836
#define V558_REG_SIF_SNRWRDATAM		0x00837

#define V558_REG_SIF_SNRWRDATAL		0x00838
#define V558_REG_SIF_SNRACSCTR		0x00839
#define V558_REG_SIF_SNRADDR			0x0083A
#define V558_REG_SIF_SNRSTATUS		0x0083B
#define V558_REG_SIF_SNRRDDATAH		0x0083C
#define V558_REG_SIF_SNRRDDATAM		0x0083D
#define V558_REG_SIF_SNRRDDATAL		0x0083E
#define V558_REG_SIF_BUS_STATE		0x0083F
#define V558_REG_SIF_AEWIN_STARTH	0x00840
#define V558_REG_SIF_AEWIN_STARTL	0x00841
#define V558_REG_SIF_AEWIN_STOPH		0x00842
#define V558_REG_SIF_AEWIN_STOPL		0x00843
#define V558_REG_SIF_AEPIN_CTRL		0x00844
#define V558_REG_SIF_AEBUS_CTRL		0x00845
#define V558_REG_SIF_B_CNTL			0x00846
#define V558_REG_SIF_B_CNTH			0x00847
#define V558_REG_SIF_AE0_DATA		0x00848

/*HCTRL 0x1400*/
#define V558_REG_HCTRL_RSTCTRL1		0x01400
#define V558_REG_HCTRL_RSTCTRL2		0x01402
#define V558_REG_HCTRL_CLKOFF		0x01404
#define V558_REG_HCTRL_CHIPCTRL		0x01406
#define V558_REG_HCTRL_CLKCTRL0		0x01408
#define V558_REG_HCTRL_CLKCTRL1		0x0140a
#define V558_REG_HCTRL_CLKCTRL2		0x0140c
#define V558_REG_HCTRL_CLKCTRL3		0x0140e
#define V558_REG_HCTRL_CPU_PC		0x01410
#define V558_REG_HCTRL_LCD_PC		0x01412
#define V558_REG_HCTRL_CS_PC			0x01414
#define V558_REG_HCTRL_STRAP			0x01416
#define V558_REG_HCTRL_GPIO_CFG		0x01418
#define V558_REG_HCTRL_GPIO_MODE		0x0141A
#define V558_REG_HCTRL_GPIO_DIR		0x0141C
#define V558_REG_HCTRL_GPIO_P0		0x0141E
#define V558_REG_HCTRL_INTCTRL		0x01420
#define V558_REG_HCTRL_INTSERV		0x01422
#define V558_REG_HCTRL_INTEN			0x01424
#define V558_REG_HCTRL_INTFLAG		0x01426
#define V558_REG_HCTRL_UMCTRL		0x0142C
#define V558_REG_HCTRL_MISC_CON		0x0142E
#define V558_REG_HCTRL_INTEN0		0x01430
#define V558_REG_HCTRL_INTFLAG0		0x01432
#define V558_REG_HCTRL_INTEN1        0x01434
#define V558_REG_HCTRL_INTEN2		0x01438
#define V558_REG_HCTRL_INTEN3        0x0143c
#define V558_REG_HCTRL_INTEN4        0x01440

/*BIU	0x1800*/
#define V558_REG_BIU_CMD			0x1802	
#define V558_REG_BIU_MEM_LOW_WORD_L		0x1844
#define V558_REG_BIU_MEM_LOW_WORD_H		0x1845
#define V558_REG_BIU_MEM_HIGH_WORD		0x1848	
#define V558_REG_BIU_INCREMENT			0x184C	
#define V558_REG_BIU_MEM_FLG			0x1850	
#define V558_REG_BIU_MUL_U2IA			0x1880
#define V558_REG_BIU_OP_MODE			0x1884
#define V558_REG_BIU_SEL_PORT	        0X1888
#define V558_REG_BIU_SEL_8_16			0x188C	
#define V558_REG_BIU_BYPASS_SEL			0x1890	
#define V558_REG_BIU_SEL_ASYN_SYN		0x18A0	
#define V558_REG_BIU_WE_DRIVE			0x18A4
#define V558_REG_BIU_MUL_CLR_AUTO		0x18A8	
#define V558_REG_BIU_V558_REG_8_FLG		0x18B0	
#define V558_REG_BIU_V558_REG_8_LOW_WORD	0x18B2	
#define V558_REG_BIU_V558_REG_8_HIGH_WORD	0x18B3	
#define V558_REG_BIU_MEM_8_FLG			0x18B4	


/*Isp register 0x02000*/
#define V558_REG_ISP_BASCTL			0x02000
#define V558_REG_ISP_WINWD			0x02002
#define V558_REG_ISP_WINHT			0x02004
#define V558_REG_ISP_IMGWD			0x02006
#define V558_REG_ISP_IMGHT			0x02008
#define V558_REG_ISP_FEM_M			0x0200A
#define V558_REG_ISP_FEM_P			0x0200B
#define V558_REG_ISP_FEM_X1			0x0200C
#define V558_REG_ISP_FEM_X2			0x0200D
#define V558_REG_ISP_FEM_MIN		0x0200E
#define V558_REG_ISP_FEM_MAX		0x0200F
#define V558_REG_ISP_LFCENX			0x02010
#define V558_REG_ISP_LFCENY			0x02012
#define V558_REG_ISP_LFFREC			0x02014
#define V558_REG_ISP_AFLNCTL		0x02016
#define V558_REG_ISP_AFTHD			0x02018
#define V558_REG_ISP_AFLNSTEP		0x0201B
#define V558_REG_ISP_RGAIN			0x02022
#define V558_REG_ISP_GGAIN			0x02023
#define V558_REG_ISP_BGAIN			0x02024
#define V558_REG_ISP_GLBGAIN		0x02026
#define V558_REG_ISP_CRADJ			0x02028
#define V558_REG_ISP_CBADJ			0x02029
#define V558_REG_ISP_CROFS			0x0202A
#define V558_REG_ISP_CBOFS			0x0202B
#define V558_REG_ISP_CMAT11			0x0202C
#define V558_REG_ISP_OFSR			0x02035
#define V558_REG_ISP_OFSG			0x02036
#define V558_REG_ISP_OFSB			0x02037
#define V558_REG_ISP_YGMST0			0x02038
#define V558_REG_ISP_YGASP0			0x02041
#define V558_REG_ISP_RGMST0			0x0204A
#define V558_REG_ISP_GGMST0			0x0205B
#define V558_REG_ISP_BGMST0			0x0206C
#define V558_REG_ISP_AFCTRL			0x02080
#define V558_REG_ISP_AFWHA			0x02082
#define V558_REG_ISP_AFWHB			0x02083
#define V558_REG_ISP_AFWHC			0x02084
#define V558_REG_ISP_AFWVA			0x02085
#define V558_REG_ISP_AFWVB			0x02086
#define V558_REG_ISP_AFWVC			0x02087
#define V558_REG_ISP_AEWF			0x02088
#define V558_REG_ISP_YBOT			0x0208A
#define V558_REG_ISP_YTOP			0x0208B
#define V558_REG_ISP_QTOP			0x0208C
#define V558_REG_ISP_ITOP			0x0208D
#define V558_REG_ISP_GBOT			0x0208E
#define V558_REG_ISP_GTOP			0x0208F
#define V558_REG_ISP_AWBCTRL		0x02090
#define V558_REG_ISP_RMEAN			0x02092
#define V558_REG_ISP_GMEAN			0x02093
#define V558_REG_ISP_BMEAN			0x02094
#define V558_REG_ISP_YMEAN			0x02095
#define V558_REG_ISP_AUTO_STATUS	0x02096
#define V558_REG_ISP_DPCTHD			0x020A0
#define V558_REG_ISP_NT1			0x020A1

/*Image Postprocessing Unit 0x01C00*/
#define	V558_REG_IPP_SPECTRL			    0x01C00
#define	V558_REG_IPP_SIZCTRL			    0x01C01

#define	V558_REG_IPP_IMGWD			    0x01C02
#define	V558_REG_IPP_IMGHT   		    0x01C04
#define V558_REG_IPP_SIZWD                       0x01C06
#define V558_REG_IPP_SIZHT                       0x01C08
#define V558_REG_IPP_SIZSTX                      0x01C0A
#define V558_REG_IPP_SIZSTY                      0x01C0C

#define V558_REG_IPP_FIRCF                       0x01C0E
#define V558_REG_IPP_PIXRATE                     0x01C13
#define V558_REG_IPP_CPUPFT                      0x01C16
#define V558_REG_IPP_CPDWFT                      0x01C17
#define V558_REG_IPP_DPUPFT                      0x01C18
#define V558_REG_IPP_DPDWFT                      0x01C19
#define V558_REG_IPP_CAPWD                       0x01C1A
#define V558_REG_IPP_CAPHT                       0x01C1C
#define V558_REG_IPP_DISWD                       0x01C1E
#define V558_REG_IPP_DISHT                       0x01C20

/*Spe effect */
#define V558_REG_YOFFSET						0x1C22
#define V558_REG_UOFFSET						0x1C24
#define V558_REG_VOFFSET						0x1C25
#define V558_REG_UUPTHRS						0x1C26
#define V558_REG_UDWTHRS						0x1C27
#define V558_REG_VUPTHRS						0x1C28
#define V558_REG_VDWTHRS						0x1C29
#define V558_REG_FRMDP							0x1C30


/*LCD I/F Unit 0x0400*/
#define V558_REG_LCD_HSA                         0x0400
#define V558_REG_LCD_HEA                         0x0402
#define V558_REG_LCD_HSXA                        0x0404
#define V558_REG_LCD_HSYA                        0x0406
#define V558_REG_LCD_HEXA                        0x0408
#define V558_REG_LCD_HEYA                        0x040A
#define V558_REG_LCD_WRI                         0x040C
#define V558_REG_LCD_LC                          0x040E
#define V558_REG_LCD_DCE                         0x0410
#define V558_REG_LCD_LCDRST                      0x0412
#define V558_REG_LCD_LCDTEST                     0x0413
#define V558_REG_LCD_HDP                         0x0416
#define V558_REG_LCD_VDP                         0x0418
#define V558_REG_LCD_AX                          0x041A
#define V558_REG_LCD_AY                          0x041C
#define V558_REG_LCD_AW                          0x041E
#define V558_REG_LCD_AH                          0x0420
#define V558_REG_LCD_ADX0                        0x0422
#define V558_REG_LCD_ADY0                        0x0424
#define V558_REG_LCD_AMW                         0x0426
#define V558_REG_LCD_AMH                         0x0428
#define V558_REG_LCD_BX                          0x042A
#define V558_REG_LCD_BY                          0x042C
#define V558_REG_LCD_BW                          0x042E
#define V558_REG_LCD_BH                          0x0430
#define V558_REG_LCD_BDX0                        0x0432
#define V558_REG_LCD_BDY0                        0x0434
#define V558_REG_LCD_BDX1                        0x0436
#define V558_REG_LCD_BDY1                        0x0438
#define V558_REG_LCD_BMW                         0x043A
#define V558_REG_LCD_BMH                         0x043C
#define V558_REG_LCD_ATCB                        0x043E
#define V558_REG_LCD_ATCG                        0x043F
#define V558_REG_LCD_ATCR                        0x0440
#define V558_REG_LCD_ATCF                        0x0441
#define V558_REG_LCD_BTCB                        0x0442
#define V558_REG_LCD_BTCG                        0x0443
#define V558_REG_LCD_BTCR                        0x0444
#define V558_REG_LCD_BTCF                        0x0445
#define V558_REG_LCD_LPM                         0x0446
#define V558_REG_LCD_BMODE                       0x0448
#define V558_REG_LCD_DMAP                        0x044A
#define V558_REG_LCD_DPAD                        0x044C
#define V558_REG_LCD_WCONT                       0x044E
#define V558_REG_LCD_RCONT                       0x0450
#define V558_REG_LCD_BM                          0x0452
#define V558_REG_LCD_DRS                         0x0454
#define V558_REG_LCD_BGDR                        0x0456
#define V558_REG_LCD_BGDG                        0x0457
#define V558_REG_LCD_BGDB                        0x0458
#define V558_REG_LCD_DM                          0x045A
#define V558_REG_LCD_MODE                        0x045C
#define V558_REG_LCD_EXWEN                       0x045E
#define V558_REG_LCD_EXW0                        0x0460
#define V558_REG_LCD_EXW1                        0x0462
#define V558_REG_LCD_EXR0                        0x0464
#define V558_REG_LCD_EXRS                        0x0466
#define V558_REG_LCD_IST                         0x0468
#define V558_REG_LCD_IW                          0x046A
#define V558_REG_LCD_IA                          0x046C
#define V558_REG_LCD_EXW1                        0x0462
#define V558_REG_LCD_EXR0                        0x0464
#define V558_REG_LCD_EXRS                        0x0466
#define V558_REG_LCD_IST                         0x0468
#define V558_REG_LCD_IW                          0x046A
#define V558_REG_LCD_IA                          0x046C
#define V558_REG_LCD_IDB                         0x046E
#define V558_REG_LCD_IDG                         0x046F
#define V558_REG_LCD_IDR                         0x0470
#define V558_REG_LCD_OM                          0x0472
#define V558_REG_LCD_UPDATE                      0x0474
#define V558_REG_LCD_INDEXSEL                    0x0476
#define V558_REG_LCD_IR                          0x0478
#define V558_REG_LCD_IRDB                        0x047A
#define V558_REG_LCD_IRDG                        0x047B
#define V558_REG_LCD_IRDR                        0x047C

#define V558_REG_LCD_GLTR0                       0x048C
#define V558_REG_LCD_GLTR1                       0x048D
#define V558_REG_LCD_GLTR2                       0x048E
#define V558_REG_LCD_GLTR3                       0x048F
#define V558_REG_LCD_GLTR4                       0x0490
#define V558_REG_LCD_GLTR5                       0x0491
#define V558_REG_LCD_GLTR6                       0x0492
#define V558_REG_LCD_GLTR7                       0x0493
#define V558_REG_LCD_GLTR8                       0x0494
#define V558_REG_LCD_GLTR9                       0x0495
#define V558_REG_LCD_GLTR10                      0x0496
#define V558_REG_LCD_GLTR11                      0x0497
#define V558_REG_LCD_GLTR12                      0x0498
#define V558_REG_LCD_GLTR13                      0x0499
#define V558_REG_LCD_GLTR14                      0x049A
#define V558_REG_LCD_GLTR15                      0x049B
#define V558_REG_LCD_GLTR16                      0x049C

#define V558_REG_LCD_GLTG0                       0x049E
#define V558_REG_LCD_GLTG1                       0x049F
#define V558_REG_LCD_GLTG2                       0x04A0
#define V558_REG_LCD_GLTG3                       0x04A1
#define V558_REG_LCD_GLTG4                       0x04A2
#define V558_REG_LCD_GLTG5                       0x04A3
#define V558_REG_LCD_GLTG6                       0x04A4
#define V558_REG_LCD_GLTG7                       0x04A5
#define V558_REG_LCD_GLTG8                       0x04A6
#define V558_REG_LCD_GLTG9                       0x04A7
#define V558_REG_LCD_GLTG10                      0x04A8
#define V558_REG_LCD_GLTG11                      0x04A9
#define V558_REG_LCD_GLTG12                      0x04AA
#define V558_REG_LCD_GLTG13                      0x04AB
#define V558_REG_LCD_GLTG14                      0x04AC
#define V558_REG_LCD_GLTG15                      0x04AD
#define V558_REG_LCD_GLTG16                      0x04AE

#define V558_REG_LCD_GLTB0                       0x04B0
#define V558_REG_LCD_GLTB1                       0x04B1
#define V558_REG_LCD_GLTB2                       0x04B2
#define V558_REG_LCD_GLTB3                       0x04B3
#define V558_REG_LCD_GLTB4                       0x04B4
#define V558_REG_LCD_GLTB5                       0x04B5
#define V558_REG_LCD_GLTB6                       0x04B6
#define V558_REG_LCD_GLTB7                       0x04B7
#define V558_REG_LCD_GLTB8                       0x04B8
#define V558_REG_LCD_GLTB9                       0x04B9
#define V558_REG_LCD_GLTB10                      0x04BA
#define V558_REG_LCD_GLTB11                      0x04BB
#define V558_REG_LCD_GLTB12                      0x04BC
#define V558_REG_LCD_GLTB13                      0x04BD
#define V558_REG_LCD_GLTB14                      0x04BE
#define V558_REG_LCD_GLTB15                      0x04BF
#define V558_REG_LCD_GLTB16                      0x04C0

/*JBUF Unit 0x2400*/
#define V558_REG_JBUF_S_ADDR                     0x2404
#define V558_REG_JBUF_T_ADDR                     0x240C
#define V558_REG_JBUF_D_ADDR                     0x2408
#define V558_REG_JBUF_JPEG_ADDR                  0x2410
#define V558_REG_JBUF_S_BUF_SIZE                 0x2414
#define V558_REG_JBUF_D_BUF_SIZE                 0x2418
#define V558_REG_JBUF_J_BUF_SIZE                 0x241C
#define V558_REG_JBUF_T_BUF_SIZE                 0x2428
#define V558_REG_JBUF_BIU_ACC_LENGTH0            0x2430
#define V558_REG_JBUF_BIU_ACC_LENGTH1            0x2431
#define V558_REG_JBUF_CLR_REG                    0x2480
#define V558_REG_JBUF_GE_CLK_COUNT               0x24C0
#define V558_REG_JBUF_CAP_VIEW_CONF              0x2488

#define V558_REG_JBUF_J_WEN_PT0                  0x24A0
#define V558_REG_JBUF_J_WEN_PT1                  0x24A1
#define V558_REG_JBUF_J_RDN_PT0                  0x24A4
#define V558_REG_JBUF_J_RDN_PT1                  0x24A5

#define V558_REG_JBUF_S_WEN_PT0                  0x24A8
#define V558_REG_JBUF_S_WEN_PT1                  0x24A9
#define V558_REG_JBUF_S_RDN_PT0                  0x24AC
#define V558_REG_JBUF_S_RDN_PT1                  0x24AD

#define V558_REG_JBUF_D_WEN_PT0                  0x24B0
#define V558_REG_JBUF_D_WEN_PT1                  0x24B1
#define V558_REG_JBUF_D_RDN_PT0                  0x24B4
#define V558_REG_JBUF_D_RDN_PT1                  0x24B5

#define V558_REG_JBUF_T_WEN_PT0                  0x24B8
#define V558_REG_JBUF_T_WEN_PT1                  0x24B9
#define V558_REG_JBUF_T_RDN_PT0                  0x24BC
#define V558_REG_JBUF_T_RDN_PT1                  0x24BD

/*JPEG Unit 0x0000*/
#define V558_REG_JPEG_MODE                       0x0000
#define V558_REG_JPEG_CTRL                       0x0001
#define V558_REG_JPEG_STATUS                     0x0002
#define V558_REG_JPEG_BRC                        0x0003
#define V558_REG_JPEG_TCR                        0x0004
#define V558_REG_JPEG_QF                         0x0005

#define V558_REG_JPEG_TARGET_WC0                 0x0008
#define V558_REG_JPEG_TARGET_WC1                 0x0009
#define V558_REG_JPEG_TARGET_WC2                 0x000A
#define V558_REG_JPEG_TARGET_WC3                 0x000B

#define V558_REG_JPEG_VIDEO_WC0                  0x000C
#define V558_REG_JPEG_VIDEO_WC1                  0x000D
#define V558_REG_JPEG_VIDEO_WC2                  0x000E
#define V558_REG_JPEG_VIDEO_WC3                  0x000F

#define V558_REG_JPEG_HUFF_WC0                   0x0010
#define V558_REG_JPEG_HUFF_WC1                   0x0011
#define V558_REG_JPEG_HUFF_WC2                   0x0012
#define V558_REG_JPEG_HUFF_WC3                   0x0013

#define V558_REG_JPEG_WIDTH_H                    0x0014
#define V558_REG_JPEG_WIDTH_L                    0x0015
#define V558_REG_JPEG_HEIGHT_H                   0x0016
#define V558_REG_JPEG_HEIGHT_L                   0x0017

#define V558_REG_JPEG_B_H                        0x0018
#define V558_REG_JPEG_B_L                        0x0019
#define V558_REG_JPEG_FRAME_CNT0                 0x001A
#define V558_REG_JPEG_FRAME_CNT1                 0x001B

#define V558_REG_JPEG_DCT_DATA                   0x001C
#define V558_REG_JPEG_QUAN_T                     0x001D
#define V558_REG_JPEG_HUFF_T                     0x001E
#define V558_REG_JPEG_NUM_COMPQUANT              0x001F

#define V558_REG_JPEG_COMP1_P1                   0x0020
#define V558_REG_JPEG_COMP1_P2                   0x0021
#define V558_REG_JPEG_COMP2_P1                   0x0022
#define V558_REG_JPEG_COMP2_P2                   0x0023

#define V558_REG_JPEG_COMP3_P1                   0x0024
#define V558_REG_JPEG_COMP3_P2                   0x0025
#define V558_REG_JPEG_COMP4_P1                   0x0026
#define V558_REG_JPEG_COMP4_P2                   0x0027

#define V558_REG_JPEG_BLK_WIDTH                  0x0028

#define V558_REG_JPEG_IMGPARA0                   0x0030
#define V558_REG_JPEG_IMGPARA1                   0x0031
#define V558_REG_JPEG_IMGPARA2                   0x0032
#define V558_REG_JPEG_IMGPARA3                   0x0033
#define V558_REG_JPEG_IMGPARA4                   0x0034
#define V558_REG_JPEG_IMGPARA5                   0x0035
#define V558_REG_JPEG_IMGPARA6                   0x0036
#define V558_REG_JPEG_IMGPARA7                   0x0037
#define V558_REG_JPEG_IMGPARA8                   0x0038
#define V558_REG_JPEG_IMGPARA9                   0x0039
#define V558_REG_JPEG_IMGPARA10                  0x003A
#define V558_REG_JPEG_IMGPARA11                  0x003B
#define V558_REG_JPEG_IMGPARA12                  0x003C
#define V558_REG_JPEG_IMGPARA13                  0x003D
#define V558_REG_JPEG_IMGPARA14                  0x003E

/*LBUF Unit 0x0C00*/
#define V558_REG_LBUF_STATUS                     0x0C00
#define V558_REG_LBUF_YUVMODE                    0x0C01
#define V558_REG_LBUF_WORKMODE                   0x0C02

#define V558_REG_LBUF_WIDTH_H                    0x0C03
#define V558_REG_LBUF_WIDTH_L                    0x0C04
#define V558_REG_LBUF_HEIGHT_H                   0x0C05
#define V558_REG_LBUF_HEIGHT_L                   0x0C06

/*Graphic Engine I/F Unit 0x1000*/
#define V558_REG_GE_STATUS						0x1000
#define V558_REG_GE_STARTUP						0x1001

#define V558_REG_GE_CTRL0						0x1006
#define V558_REG_GE_CTRL1						0x1007
#define V558_REG_GE_BITWISE						0x1008
#define V558_REG_GE_COLORDEPTH					0x1009

#define V558_REG_GE_DEST_SPAN_L					0x100A
#define V558_REG_GE_DEST_SPAN_H					0x100B
#define V558_REG_GE_SRC_SPAN_L					0x100C
#define V558_REG_GE_SRC_SPAN_H					0x100D
#define V558_REG_GE_SRC_START_L					0x100E
#define V558_REG_GE_SRC_START_H					0x100F
#define V558_REG_GE_DEST_START_L				0x1010
#define V558_REG_GE_DEST_START_H				0x1011
#define V558_REG_GE_PTN_START_L					0x1012
#define V558_REG_GE_PTN_START_H					0x1013

#define V558_REG_GE_PTN_BG_B					0x1014
#define V558_REG_GE_PTN_BG_G					0x1015
#define V558_REG_GE_PTN_BG_R					0x1016
#define V558_REG_GE_PTN_FG_B					0x1018
#define V558_REG_GE_PTN_FG_G					0x1019
#define V558_REG_GE_PTN_FG_R					0x101A

#define V558_REG_GE_SRC_BG_B					0x101C
#define V558_REG_GE_SRC_BG_G					0x101D
#define V558_REG_GE_SRC_BG_R					0x101E
#define V558_REG_GE_SRC_FG_B					0x1020
#define V558_REG_GE_SRC_FG_G					0x1021
#define V558_REG_GE_SRC_FG_R					0x1022

#define V558_REG_GE_LINE_SABS_L					0x1024
#define V558_REG_GE_LINE_SABS_H					0x1025
#define V558_REG_GE_LINE_SORD_L					0x1026
#define V558_REG_GE_LINE_SORD_H					0x1027
#define V558_REG_GE_LINE_EABS_L					0x1028
#define V558_REG_GE_LINE_EABS_H					0x1029
#define V558_REG_GE_LINE_EORD_L					0x102A
#define V558_REG_GE_LINE_EORD_H					0x102B
#define V558_REG_GE_LINE_B						0x102C
#define V558_REG_GE_LINE_G						0x102D
#define V558_REG_GE_LINE_R						0x102E

#define V558_REG_GE_DEST_WIDTH_L				0x1030
#define V558_REG_GE_DEST_WIDTH_H				0x1031
#define V558_REG_GE_DEST_HEIGHT_L				0x1032
#define V558_REG_GE_DEST_HEIGHT_H				0x1033

#define V558_REG_GE_PTNBUF_L					0x1034
#define V558_REG_GE_PTNBUF_H					0x1035
#define V558_REG_GE_PTNBUF_WR					0x1033

/*Host*//*DrvWriteHostReg*/
#define V558_REG_HOST_REGUIA                     0x0
#define V558_REG_HOST_MEMUIA                     0x1
#define V558_REG_HOST_MEMLIA                     0x2
#define V558_REG_HOST_MEMLIA0                    0x3

#define V558_REG_HOST_MEMDAT                     0x80
#define V558_REG_HOST_MEMPORT                    0x90

#ifdef __cplusplus
}
#endif

#endif //_VREGDEF_H_
