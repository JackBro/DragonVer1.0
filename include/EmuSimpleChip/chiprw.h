
#ifndef _SIMREG_CHIPRW_H_
#define _SIMREG_CHIPRW_H_


#ifndef EMUSIMPLECHIP_DLL
	#define EMUSIMPLECHIP_DLL  __declspec(dllimport)
#endif



extern EMUSIMPLECHIP_DLL void	SimReg_SetReg(int adr, int val, int type);
extern EMUSIMPLECHIP_DLL int	SimReg_GetReg(int adr, int type);

extern EMUSIMPLECHIP_DLL void	SimUsb_SetReg(int adr, int val);
extern EMUSIMPLECHIP_DLL int	SimUsb_GetReg(int adr);

extern EMUSIMPLECHIP_DLL void	Simhost_SetReg(int adr, int val);
extern EMUSIMPLECHIP_DLL int	Simhost_GetReg(int adr);

extern EMUSIMPLECHIP_DLL void	SimRegOpen(void);

#endif //_SIMREG_CHIPRW_H_
