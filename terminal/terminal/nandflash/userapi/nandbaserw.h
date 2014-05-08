
#ifndef _TRM_NANDBASERW_H_
#define _TRM_NANDBASERW_H_

#ifdef __cplusplus
extern "C" {
#endif


	#define	NAND_IO_ADDR				0x02000000

	enum GPIOSETenum {
		ENABLE_CMD,
		ENABLE_ADR,
		ENABLE_DAT,
		INVALID_NAND
	};


#ifdef	WIN32

	void NAND_SET_IO8(int val);
	int  NAND_GET_IO8(void);
	void NAND_SET_IO16(int val);
	int  NAND_GET_IO16(void);

#else

	#define	NAND_SET_IO8(val)			*(volatile char *)NAND_IO_ADDR = val
	#define	NAND_GET_IO8()				*(volatile char *)NAND_IO_ADDR
	#define	NAND_SET_IO16(val)			*(volatile unsigned short *)NAND_IO_ADDR = val
	#define	NAND_GET_IO16()				*(volatile unsigned short *)NAND_IO_ADDR

#endif

	void	NandSetMode(int mode);
	void	NandWaitReady(void);


#ifdef __cplusplus
}
#endif

#endif



