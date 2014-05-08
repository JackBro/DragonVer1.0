#ifndef _BITTOOLS_H_
#define _BITTOOLS_H_


#ifndef TOOLBIT_DLL
	#define TOOLBIT_DLL  __declspec(dllimport)
#endif



class TOOLBIT_DLL CBitTools
{
public:
	CBitTools(char chk = -1, char fill = 0, int chken = 0);
	virtual ~CBitTools();

public:
	int CopyBitStream(char *src, char *dst, int bitlen, int dststart = 0, int srcstart = 0);
				//size bit unit

public:
	void ReverseBitStream(char *src, char *dst, int size, int bitwidth = 1);
				//size byte unit

public:
	int	 PutBits(char *dst, int val, int bitlen, int bitcur = 0);
	int	 GetBits(char *src, int curval, int bitlen, int bitpos = 0);
	int  PutBufBits(char *src, char *dst, int bitlen, int dststart = 0, int srcstart = 0);

public:
	int	 ValExtent(int val, int bitlen);

private:
	int  AdjustAdr(char *dst, int adr, int *cur);

public:
	void SetChk(int chk);
	void SetFill(int fill);
	void SetChkEn(int chken);

private:
	char m_chk;
	char m_fill;
	int  m_chken;
};

#endif


