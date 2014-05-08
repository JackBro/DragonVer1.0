#include "internal.h"

CMclipB::CMclipB()
{
	this->m_hwnd = NULL;
}

void CMclipB::Init(HWND hwnd)
{
	this->m_hwnd = hwnd;
}

int CMclipB::Read(char *buf, int len)
{
	HGLOBAL hGlobal;		// Global memory handle
	char *ptr;
	int size;//, i;

	if(this->m_hwnd == NULL)
		return 0;
	if(!OpenClipboard(this->m_hwnd))
		return 0;
	hGlobal = GetClipboardData(CF_TEXT);
	if (hGlobal == NULL) 
		return FALSE;
	ptr = (char *)GlobalLock(hGlobal);
	size = (int)GlobalSize(hGlobal);
	if(len > size)
		len = size + 1;
	else
		size = len - 1;
	memcpy(buf, ptr, size);
	ptr[size] = 0;
//	for (i = 0; i < len; i++)
//		buf[i] = ptr[i];
	GlobalUnlock(hGlobal);
	CloseClipboard();
	return len;
}

int CMclipB::Write(char *buf, int len)
{
	HGLOBAL hGlobal;		// Global memory handle
	char *ptr;
//	int i;

	if(this->m_hwnd == NULL)
		return 0;
	if(!OpenClipboard(this->m_hwnd))
		return 0;
	EmptyClipboard();
	hGlobal = GlobalAlloc(GMEM_ZEROINIT, len+1);
	if (hGlobal == NULL) 
		return FALSE;
	ptr = (char *)GlobalLock(hGlobal);
	memcpy(ptr, buf, len);
	ptr[len] = 0;
//	for (i = 0; i < len; i++)
//		ptr[i] = buf[i];
	GlobalUnlock(hGlobal);
	SetClipboardData(CF_TEXT, hGlobal);
	CloseClipboard();
	return len;
}

int CMclipB::GetLength(void)
{
	HGLOBAL hGlobal;		// Global memory handle
	int size;

	if(this->m_hwnd == NULL)
		return 0;
	if(!OpenClipboard(this->m_hwnd))
		return 0;
	hGlobal = GetClipboardData(CF_TEXT);
	if (hGlobal == NULL) 
		return FALSE;
	size = (int)GlobalSize(hGlobal);
	GlobalUnlock(hGlobal);
	CloseClipboard();
	return size;
}


