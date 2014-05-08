
#ifndef	_MYDEMO_H_
#define _MYDEMO_H_

#include "../../../graphics/gui/core/GUI.h"
#include "../../../graphics/gui/widget/button.h"
#include "../../tools/type.h"


/**************************************************************
					    Macro Definition
***************************************************************/

#define	KEY_ENTER			0x0d
#define KEY_END				0x18
#define	KEY_BACKSPACE		0x20


//	Color for rainbow
#define	COLOR_1				0x8B008B
#define	COLOR_2				0xFF0000
#define	COLOR_3				0xCEDD82
#define	COLOR_4				0x00FF00
#define	COLOR_5				0x5EFCFC
#define	COLOR_6				0x1DACFD
#define	COLOR_7				0x1A2AF3


typedef	void (*PDemoFunc)(void);


/**************************************************************
					    Structure
***************************************************************/

typedef	struct	tag_TGuiDemoContext {
	TSize			LcdSize;
	GUI_COLOR		bgcolor, fgcolor;	//default color
	const GUI_FONT	*font;				//default font
	int				textmode;
	UINT8			pensize, penshape, linestyle;
	BUTTON_Handle	cur_btn;
} TGuiDemoContext, *PTGuiDemoContext;

typedef struct tag_TMDParam {
	GUI_AUTODEV_INFO AutoDevInfo;
	float			angle;
} TMDParam, *PTMDParam;

enum MEMDEVenum {
	MEM_DEV_ON,
	MEM_DEV_OFF
};

/**************************************************************
					    Declaration
***************************************************************/
extern const GUI_BITMAP logo_vimicro;
extern const GUI_BITMAP full_bmp;
extern const GUI_BITMAP wm1_bmp;
extern const GUI_BITMAP wm2_bmp;


void DemoMain(void);





#endif