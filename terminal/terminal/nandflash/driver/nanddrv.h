
#ifndef _TRM_NANDDRV_H_
#define _TRM_NANDDRV_H_

#ifdef __cplusplus
extern "C" {
#endif

	#define NAND_OK		0
	#define	NAND_ERR	(-1)


	typedef struct tag_TNandInfo {
		int		id;						// Maker , Device
		int		unit;					// 1 bytes (8-bit I/O); 2 bytes (16-bit I/O);
		int		block;					// total block number
		int		page;					// page number per block
		int		dnum;					// data number per page : unit = byte/word when bus = 8-bit/16-bit
		int		tnum;					// total number per page : unit = byte/word when bus = 8-bit/16-bit
	} TNandInfo, *PTNandInfo;

	typedef struct tag_TNandContext {
		PTNandInfo	pinfo;
		char		*map;
	} TNandContext, *PTNandContext;


	void	NandInit(void);
	void	NandUpdateMap(char *buf);
	int		NandGetId(void);
	int		NandErase(int block);
	void	NandRead(int addr, char *buf, int num);
	void	NandWrite(int addr, char *buf, int num);
	int		NandReadSpare(int block, int page, int start, char *buf, int num);
	int		NandWriteSpare(int block, int page, int start, char *buf, int num);



#ifdef __cplusplus
}
#endif

#endif