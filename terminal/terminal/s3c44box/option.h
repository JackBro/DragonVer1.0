#ifndef __OPTION_H__
#define __OPTION_H__

// ************* OPTIONS **************
#define FCLKIN			8000		/* KHz */
#define DEFAULT_MCLK		34000    
#define MIN_MCLK		12000
#define MAX_MCLK		64000
#define MIN_I2SCLK		4000
#define I2S_DEFAULT_CLK 	12000

#define DEFAULT_BAUD		115200
#define TM_DEFAULT_CHANNEL	CHANNEL_UART0

#define WRBUFOPT (0x8)   /* write_buf_on */

#define SYSCFG_0KB (0x0|WRBUFOPT)
#define SYSCFG_4KB (0x2|WRBUFOPT)
#define SYSCFG_8KB (0x6|WRBUFOPT)

#define DRAM	    1		/* In case DRAM is used */
#define SDRAM	    2		/* In case SDRAM is used */
#define BDRAMTYPE   SDRAM	/* used in power.c,44blib.c */

//BUSWIDTH; 16,32
#define BUSWIDTH    (16)

#define CACHECFG    SYSCFG_8KB

#define _RAM_STARTADDRESS 0xc000000
#define _ISR_STARTADDRESS 0xc7fff00     /* GCS6:64M DRAM/SDRAM */

#define NONECACH_START	0x2000000

#define STACKSIZE    0x1000
#define SVCSTACK	(_ISR_STARTADDRESS-0x400)	/* task0's stack */

#endif /*__OPTION_H__*/


