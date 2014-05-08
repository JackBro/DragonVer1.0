#ifndef _TRM_NANDBASE_H_
#define _TRM_NANDBASE_H_

#ifdef __cplusplus
extern "C" {
#endif


	typedef void (*PNandSet)(int val);
	typedef int (*PNandGet)(void);

	typedef struct tag_TNandAccess {
		PNandSet	pNandSet[2];
		PNandGet	pNandGet[2];
	} TNandAccess, *PTNandAccess;

	void	NandDelay(int cnt);
	void	NandSetCmdBase(int cmd, int unit);
	void	NandSetAddrBase(int addr, int unit);
	void	NandSetDataBase(int data, int unit);
	int		NandGetDataBase(int unit);

#ifdef __cplusplus
}
#endif

#endif
