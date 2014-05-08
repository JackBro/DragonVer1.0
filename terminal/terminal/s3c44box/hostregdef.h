#ifndef _HOSTREGDEF_H_
#define _HOSTREGDEF_H_


/* System */
#define REG_SYSCFG		0x1c00000

/* Cache */
#define REG_NCACHBE0	0x1c00004
#define REG_NCACHBE1	0x1c00008

/* Bus control */
#define REG_SBUSCON		0x1c40000
    
/* Memory control */
#define REG_BWSCON		0x1c80000
#define REG_BANKCON0	0x1c80004
#define REG_BANKCON1	0x1c80008
#define REG_BANKCON2	0x1c8000c
#define REG_BANKCON3	0x1c80010
#define REG_BANKCON4	0x1c80014
#define REG_BANKCON5	0x1c80018
#define REG_BANKCON6	0x1c8001c
#define REG_BANKCON7	0x1c80020
#define REG_REFRESH		0x1c80024
#define REG_BANKSIZE	0x1c80028
#define REG_MRSRB6		0x1c8002c
#define REG_MRSRB7		0x1c80030

/* UART */
#define REG_ULCON0		0x1d00000
#define REG_ULCON1		0x1d04000
#define REG_UCON0		0x1d00004
#define REG_UCON1		0x1d04004
#define REG_UFCON0		0x1d00008
#define REG_UFCON1		0x1d04008
#define REG_UMCON0		0x1d0000c
#define REG_UMCON1		0x1d0400c
#define REG_UTRSTAT0	0x1d00010
#define REG_UTRSTAT1	0x1d04010
#define REG_UERSTAT0	0x1d00014
#define REG_UERSTAT1	0x1d04014
#define REG_UFSTAT0		0x1d00018
#define REG_UFSTAT1		0x1d04018
#define REG_UMSTAT0		0x1d0001c
#define REG_UMSTAT1		0x1d0401c
#define REG_UBRDIV0		0x1d00028
#define REG_UBRDIV1		0x1d04028

#define REG_UTXH0		0x1d00020
#define REG_UTXH1		0x1d04020
#define REG_URXH0		0x1d00024
#define REG_URXH1		0x1d04024

/* SIO */
#define REG_SIOCON		0x1d14000
#define REG_SIODAT		0x1d14004
#define REG_SBRDR		0x1d14008
#define REG_IVTCNT		0x1d1400c
#define REG_DCNTZ		0x1d14010

/* IIS */
#define REG_IISCON		0x1d18000
#define REG_IISMOD		0x1d18004
#define REG_IISPSR		0x1d18008
#define REG_IISFCON		0x1d1800c

#define REG_IISFIF			0x1d18010

/* I/O PORT */
#define REG_PCONA		0x1d20000
#define REG_PDATA		0x1d20004

#define REG_PCONB		0x1d20008
#define REG_PDATB		0x1d2000c

#define REG_PCONC		0x1d20010
#define REG_PDATC		0x1d20014
#define REG_PUPC		0x1d20018

#define REG_PCOND		0x1d2001c
#define REG_PDATD		0x1d20020
#define REG_PUPD		0x1d20024

#define REG_PCONE		0x1d20028
#define REG_PDATE		0x1d2002c
#define REG_PUPE		0x1d20030

#define REG_PCONF		0x1d20034
#define REG_PDATF		0x1d20038
#define REG_PUPF		0x1d2003c

#define REG_PCONG		0x1d20040
#define REG_PDATG		0x1d20044
#define REG_PUPG		0x1d20048

#define REG_SPUCR		0x1d2004c
#define REG_EXTINT		0x1d20050
#define REG_EXTINTPND	0x1d20054

/* WATCHDOG */
#define REG_WTCON		0x1d30000
#define REG_WTDAT		0x1d30004
#define REG_WTCNT		0x1d30008

/* ADC */
#define REG_ADCCON		0x1d40000
#define REG_ADCPSR		0x1d40004
#define REG_ADCDAT		0x1d40008

/* Timer */
#define REG_TCFG0		0x1d50000
#define REG_TCFG1		0x1d50004
#define REG_TCON		0x1d50008

#define REG_TCNTB0		0x1d5000c
#define REG_TCMPB0		0x1d50010
#define REG_TCNTO0		0x1d50014

#define REG_TCNTB1		0x1d50018
#define REG_TCMPB1		0x1d5001c
#define REG_TCNTO1		0x1d50020

#define REG_TCNTB2		0x1d50024
#define REG_TCMPB2		0x1d50028
#define REG_TCNTO2		0x1d5002c

#define REG_TCNTB3		0x1d50030
#define REG_TCMPB3		0x1d50034
#define REG_TCNTO3		0x1d50038

#define REG_TCNTB4		0x1d5003c
#define REG_TCMPB4		0x1d50040
#define REG_TCNTO4		0x1d50044

#define REG_TCNTB5		0x1d50048
#define REG_TCNTO5		0x1d5004c

/* IIC */
#define REG_IICCON     0x1d60000
#define REG_IICSTAT    0x1d60004
#define REG_IICADD     0x1d60008
#define REG_IICDS      0x1d6000c

/* RTC */
#define REG_RTCCON         0x1d70040
#define REG_RTCALM         0x1d70050
#define REG_ALMSEC         0x1d70054
#define REG_ALMMIN         0x1d70058
#define REG_ALMHOUR        0x1d7005c
#define REG_ALMDAY         0x1d70060
#define REG_ALMMON         0x1d70064
#define REG_ALMYEAR        0x1d70068
#define REG_RTCRST         0x1d7006c
#define REG_BCDSEC         0x1d70070
#define REG_BCDMIN         0x1d70074
#define REG_BCDHOUR        0x1d70078
#define REG_BCDDAY         0x1d7007c
#define REG_BCDDATE        0x1d70080
#define REG_BCDMON         0x1d70084
#define REG_BCDYEAR        0x1d70088
#define REG_TICINT         0x1d7008c

/* Clock & Power management */
#define REG_PLLCON		0x1d80000
#define REG_CLKCON		0x1d80004
#define REG_CLKSLOW		0x1d80008
#define REG_LOCKTIME	0x1d8000c

/* INTERRUPT */
#define REG_INTCON		0x1e00000
#define REG_INTPND		0x1e00004
#define REG_INTMOD		0x1e00008
#define REG_INTMSK		0x1e0000c

#define REG_I_PSLV		0x1e00010
#define REG_I_PMST		0x1e00014
#define REG_I_CSLV		0x1e00018

#define REG_I_CMST		0x1e0001c
#define REG_I_ISPR		0x1e00020
#define REG_I_ISPC		0x1e00024

#define REG_F_ISPR		0x1e00038
#define REG_F_ISPC		0x1e0003c

/* LCD */
#define REG_LCDCON1		0x1f00000
#define REG_LCDCON2		0x1f00004
#define REG_LCDCON3		0x1f00040
#define REG_LCDSADDR1	0x1f00008
#define REG_LCDSADDR2	0x1f0000c
#define REG_LCDSADDR3	0x1f00010
#define REG_REDLUT		0x1f00014
#define REG_GREENLUT	0x1f00018
#define REG_BLUELUT		0x1f0001c
#define REG_DP1_2		0x1f00020
#define REG_DP4_7		0x1f00024
#define REG_DP3_5		0x1f00028
#define REG_DP2_3		0x1f0002c
#define REG_DP5_7		0x1f00030
#define REG_DP3_4		0x1f00034
#define REG_DP4_5		0x1f00038
#define REG_DP6_7		0x1f0003c
#define REG_DITHMODE	0x1f00044

/* ZDMA0 */
#define REG_ZDCON0		0x1e80000
#define REG_ZDISRC0		0x1e80004
#define REG_ZDIDES0		0x1e80008
#define REG_ZDICNT0		0x1e8000c
#define REG_ZDCSRC0		0x1e80010
#define REG_ZDCDES0		0x1e80014
#define REG_ZDCCNT0		0x1e80018

/* ZDMA1 */
#define REG_ZDCON1		0x1e80020
#define REG_ZDISRC1		0x1e80024
#define REG_ZDIDES1		0x1e80028
#define REG_ZDICNT1		0x1e8002c
#define REG_ZDCSRC1		0x1e80030
#define REG_ZDCDES1		0x1e80034
#define REG_ZDCCNT1		0x1e80038

/* BDMA0 */
#define REG_BDCON0		0x1f80000
#define REG_BDISRC0		0x1f80004
#define REG_BDIDES0		0x1f80008
#define REG_BDICNT0		0x1f8000c
#define REG_BDCSRC0		0x1f80010
#define REG_BDCDES0		0x1f80014
#define REG_BDCCNT0		0x1f80018

/* BDMA1 */
#define REG_BDCON1		0x1f80020
#define REG_BDISRC1		0x1f80024
#define REG_BDIDES1		0x1f80028
#define REG_BDICNT1		0x1f8002c
#define REG_BDCSRC1		0x1f80030
#define REG_BDCDES1		0x1f80034
#define REG_BDCCNT1		0x1f80038

#endif

