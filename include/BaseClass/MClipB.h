#ifndef _MCLIPB_H_
#define _MCLIPB_H_



#ifndef BASECLASS_DLL
	#define BASECLASS_DLL  __declspec(dllimport)
#endif


class BASECLASS_DLL CMclipB
{
public:
	CMclipB();
	int Read(char *buf, int len);
	int Write(char *buf, int len);
	int GetLength(void);

public:
	void Init(HWND hwnd);

protected:

private:
	HWND	m_hwnd;
};

#endif
