#ifndef _GLINKMAP_H_
#define _GLINKMAP_H_


#ifndef SILLGUI_DLL
	#define SILLGUI_DLL  _declspec(dllimport)
#endif

#include "LinkMap.h"

class SILLGUI_DLL CGLinkMap: public CLinkMap
{
public:
	CGLinkMap();
	virtual ~CGLinkMap();
public:
	void DrawMap(char *nodename, Graphics *graphics, Rect rect,int rowheight=80);
};

#endif
