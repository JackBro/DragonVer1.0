
#ifndef _TOOLS_JMMATRIX_H_
#define _TOOLS_JMMATRIX_H_


#ifndef TOOLJPG_DLL
	#define TOOLJPG_DLL  __declspec(dllimport)
#endif



//===================================================
//
//		CJmMatrix class
//
//===================================================
class TOOLJPG_DLL CJmMatrix
{
protected:
	void	fpdct8x8(int *inblk, int *oublk);
	void	zigzag(int *inblk, int *oublk);
	void	zigzag(double *inblk, int *oublk);
	void	quant(int *inblk, int *oublk, int *brcTable, int sft);
	void	dctquant(char *block, double *qt, double *oublk);

	//virtual void	real_fdct8x8(int *inblk, int *oublk);
	void	dct8x8(int *inblk);	
	void	fquant(int *inblk, int *oublk, int *QTable);

protected:
	void	izigzag(int *inblk, int *oublk);
	void	izigzag(int *inblk, double *oublk);
	void	izigzag(int *inblk, int *oublk, int alter);
	void	dequant(int *inblk, int *oublk, int *brcTable, int sft);
	void	fpidct8x8(int *inblk, int *oublk);
	void	idctdequant(double *inblk, double *qt, char *block);

	void	fiquant(int *inblk, int *oublk, int *QTable);
	void	idct8x8(int *inblk);
	void	idct8x8(double *inblk);

	int		ValAdjust(int val, int index);

protected:
	int	shiftFix(int input, int shift);
	int	roundDouble(double input);
	int	roundfc(int input);
	int	rounddb(int input);
	int	roundeb(int input);
	int	roundgb(int input);
	int	round12(int input);
	int	round14(int input);

protected:
	void	mpeg2iquant(int *inblk, int *oublk, int *QTalbe, int mbtype = 0, char dcprecision = 0);
};


#endif

