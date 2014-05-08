#include "GuiSmpDemo.h"


static TGuiDemoContext	gDemoContext;

/**************************************************************

					    General Function

***************************************************************/

static void DemoWaitKey(int key)
{
	while(key != GUI_WaitKey());
}

static void DemoWaitBtn(char *str)
{
	BUTTON_Handle hbtn;
	TRect btnprop;

	btnprop.size.cx = 60;
	btnprop.size.cy = 20;
	btnprop.ltp.x = gDemoContext.LcdSize.cx - btnprop.size.cx - 1;
	btnprop.ltp.y = gDemoContext.LcdSize.cy - btnprop.size.cy - 1;
	hbtn = BUTTON_Create(btnprop.ltp.x, btnprop.ltp.y, btnprop.size.cx, btnprop.size.cy, GUI_ID_OK, WM_CF_SHOW);
	gDemoContext.cur_btn = hbtn;

	BUTTON_SetFont(hbtn, &GUI_Font8_ASCII);
	BUTTON_SetText(hbtn, str);

	BUTTON_SetState(hbtn, BUTTON_STATE_FOCUS);
	DemoWaitKey(KEY_ENTER);
	BUTTON_SetState(hbtn, BUTTON_STATE_PRESSED);
	GUI_Delay(100);

	BUTTON_Delete(hbtn);
}

static void DemoDispTitle(char *str)
{
	GUI_SetFont(&GUI_Font24B_ASCII);
	GUI_DispString(str);
	GUI_SetFont(gDemoContext.font);
}


/**************************************************************

						Demo Function

***************************************************************/

static void DemoInit(void)
{
	GUI_DrawBitMap(0, 0, &full_bmp);

	gDemoContext.LcdSize.cx = LCD_GetXSize();
	gDemoContext.LcdSize.cy = LCD_GetYSize();
	gDemoContext.fgcolor	= GUI_WHITE;
	gDemoContext.bgcolor	= GUI_MAGENTA;
	gDemoContext.font		= &GUI_Font8x12_ASCII;
	gDemoContext.textmode	= GUI_TEXTMODE_NORMAL;
	gDemoContext.pensize	= 1;
	gDemoContext.penshape	= GUI_PS_ROUND;
	gDemoContext.linestyle	= GUI_LS_SOLID;

	GUI_SetColor(gDemoContext.fgcolor);
	GUI_SetBkColor(gDemoContext.bgcolor);
	GUI_SetFont(gDemoContext.font);
	GUI_SetTextMode(gDemoContext.textmode);
	GUI_SetPenSize(gDemoContext.pensize);
	GUI_SetPenShape(gDemoContext.penshape);
	GUI_SetLineStyle(gDemoContext.linestyle);
}


/*******************************************************************************************/

static void DemoTextDisp_CS(void)	//	Display char, string
{
	GUI_DispStringAt("GUI_DispChar()",	0,	40);
	GUI_GotoXY(160, 40);
	GUI_DispChar('A');

	GUI_DispStringAt("GUI_DispChars()",	0,	60);
	GUI_GotoXY(160, 60);
	GUI_DispChars('*', 10);

	GUI_DispStringAt("GUI_DispString()",0,	80);
	GUI_GotoXY(160, 80);
	GUI_DispString("Good Morning !");
}

static void DemoTextDisp_TM(void)	//	Display text mode
{
	UINT8 i = 0;
	const char *TM_index1[] = {
			"TextMode:Normal",
			"TextMode:Reverse",
			"TextMode:Trans",
			"TextMode:Xor"
	};
	const int  TM_mode[] = {
			GUI_TEXTMODE_NORMAL,
			GUI_TEXTMODE_REV,
			GUI_TEXTMODE_TRANS,
			GUI_TEXTMODE_XOR
	};
	const char *TM_index3[] = {
			"GUI_TEXTMODE_NORMAL",
			"GUI_TEXTMODE_REV",
			"GUI_TEXTMODE_TRANS",
			"GUI_TEXTMODE_XOR"
	};
	const UINT8 num = sizeof(TM_mode) / sizeof(int);

	for(i = 0; i < num; i++)
		GUI_DispStringAt(TM_index1[i],	0, 120+20*i);

	GUI_SetBkColor(GUI_YELLOW);
	GUI_SetColor(GUI_BLUE);
	for(i = 0; i < num; i++)
	{
		GUI_SetTextMode(TM_mode[i]);
		GUI_DispStringAt(TM_index3[i], 160, 120+20*i);
	}
}

static void DemoTextDisp(void)
{
	DemoDispTitle("Text Display :");

	//	Display char, chars, string
	DemoTextDisp_CS();
	DemoTextDisp_TM();

	//	Restore default config
	GUI_SetBkColor(gDemoContext.bgcolor);
	GUI_SetColor(gDemoContext.fgcolor);
	GUI_SetTextMode(gDemoContext.textmode);
}


/*******************************************************************************************/

static void DemoValueDispA(void)
{
	SINT32	val = 9999;
	UINT8	len = 9, sft = 3, maxdgt = 9;

	DemoDispTitle("Value Display A:");

	//	Display Dec, DecMin, DecShift, SDec, SDecShift
	GUI_DispStringAt("GUI_DispDec()",		20,	40);
	GUI_GotoXY(200, 40);
	GUI_DispDec(val, len);

	GUI_DispStringAt("GUI_DispDecMin()",	20,	60);
	GUI_GotoXY(200, 60);
	GUI_DispDecMin(val);

	GUI_DispStringAt("GUI_DispDecShift()",	20,	80);
	GUI_GotoXY(200, 80);
	GUI_DispDecShift(val, len, sft);

	GUI_DispStringAt("GUI_DispDecSpace()",	20,	100);
	GUI_GotoXY(200, 100);
	GUI_DispDecSpace(val, maxdgt);

	GUI_DispStringAt("GUI_DispSDec()",		20,	120);
	GUI_GotoXY(200, 120);
	GUI_DispSDec(-val, maxdgt);

	GUI_DispStringAt("GUI_DispDecShift()",	20,	140);
	GUI_GotoXY(200, 140);
	GUI_DispSDecShift(-val, len, sft);

}

static void DemoValueDispB(void)
{
	float	val = (float)12.345678;
	UINT32	val1 = 200;
	SINT8	len = 9;

	GUI_Clear();
	GUI_GotoXY(0, 0);
	DemoDispTitle("Value Display B:");

	//	Display Float, FloatMin, FloatFix, SFloatMin, SFloatFix, Bin, Hex
	GUI_DispStringAt("GUI_DispFloat()",		20,	40);
	GUI_GotoXY(200, 40);
	GUI_DispFloat(val, len);

	GUI_DispStringAt("GUI_DispFloatMin()",	20,	60);
	GUI_GotoXY(200, 60);
	GUI_DispFloatMin(val, 3);

	GUI_DispStringAt("GUI_DispFloatFix()",	20,	80);
	GUI_GotoXY(200, 80);
	GUI_DispFloatFix(val, len, 3);

	GUI_DispStringAt("GUI_DispSFloatMin()",	20,	100);
	GUI_GotoXY(200, 100);
	GUI_DispFloatMin(-val, 3);

	GUI_DispStringAt("GUI_DispSFloatFix()",	20,	120);
	GUI_GotoXY(200, 120);
	GUI_DispFloatFix(-val, len, 3);

	GUI_DispStringAt("GUI_DispBin()",		20,	160);
	GUI_GotoXY(200, 160);
	GUI_DispBin(val1, len);

	GUI_DispStringAt("GUI_DispHex()",		20,	180);
	GUI_GotoXY(200, 180);
	GUI_DispHex(val1, 8);
}


/*******************************************************************************************/

static void DemoFontDisp(void)
{
	const char str[] = "Hello World !";
	const GUI_FONT *font[] = {
			&GUI_Font4x6,
			&GUI_Font6x8,
			&GUI_Font8x8,
			&GUI_Font8x10_ASCII,
			&GUI_Font8x16,
			&GUI_Font8x16x1x2,
			&GUI_FontComic18B_ASCII,
			&GUI_FontComic24B_ASCII,
			&GUI_Font32B_ASCII
	};
	const UINT8 vpos[] = {
		30, 45, 60, 75, 90, 110, 140, 165, 200
	};
	UINT8 i, num = sizeof(font) / sizeof(GUI_FONT*);

	DemoDispTitle("Font Display:");

	//	Change Font to Display
	for(i = 0; i < num; i ++)
	{
		GUI_SetFont(font[i]);
		GUI_DispStringAt(str, 20, vpos[i]);
	}
}


/*******************************************************************************************/

static void DemoDrawRect(void)
{
	UINT8 i = 0;
	const GUI_RECT rect[] = {
		{  0,  30, 159, 129},
		{160,  30, 319, 129},
		{  0, 130, 159, 229},
		{160, 130, 319, 229}
	};
	const GUI_COLOR color[] = {GUI_LIGHTBLUE, GUI_WHITE, GUI_GRAY, GUI_CYAN};
	const num = sizeof(rect) / sizeof(GUI_RECT);

	for(i = 0; i < num; i++)
	{
		GUI_SetColor(color[i]);
		GUI_FillRect(rect[i].x0, rect[i].y0, rect[i].x1, rect[i].y1);
	}

	GUI_SetColor(gDemoContext.fgcolor);
}

static void DemoDrawPolygon(void)
{
	UINT8 i = 0;
	const GUI_POINT sqa[] = {
			{ 0,   0}, { 0, 60}, {26,  40}, {26, -20},
			{26, -20}, {26, 40}, {86,  40}, {86, -20},
			{ 0,   0}, {60,  0}, {86, -20}, {26, -20},
			{ 0,   0}, { 0, 60}, {60,  60}, {60,   0},
			{60,   0}, {60, 60}, {86,  40}, {86, -20}
	};
	const int x = 40, y = 60, num = 4;

	GUI_SetLineStyle(GUI_LS_DOT);
	for(i = 0; i < 2; i++)
		GUI_DrawPolygon(&(sqa[i*4]), num, x, y);

	GUI_SetLineStyle(GUI_LS_SOLID);
	for(i = 2; i < 5; i++)
		GUI_DrawPolygon(&(sqa[i*4]), num, x, y);
}

static void DemoDrawCircle(void)
{
	UINT8 i = 0;
	const GUI_COLOR color[] = {GUI_BLUE, GUI_BLACK, GUI_RED, GUI_YELLOW, GUI_GREEN};
	const int r = 20;
	const GUI_POINT point[] = { {194, 70}, {240, 70}, {286, 70}, {217, 90}, {263, 90} };
	const UINT8 num = sizeof(point) / sizeof(GUI_POINT);

	for(i = 0; i < num; i++)
	{
		GUI_SetColor(color[i]);
		GUI_DrawCircle(point[i].x, point[i].y, r);
	}

	GUI_SetColor(gDemoContext.fgcolor);
}

static void DemoDrawEllipse(void)
{
	UINT8 i = 0;
	const GUI_POINT pt = {80, 180};
	const TSize size[] = { {60, 40}, {20, 40} };
	const num = sizeof(size) / sizeof(TSize);

	GUI_SetColor(GUI_LIGHTMAGENTA);
	GUI_FillEllipse(pt.x, pt.y, size[1].cx, size[1].cy);
	GUI_SetColor(GUI_BROWN);
	for(i = 0; i < num; i++)
		GUI_DrawEllipse(pt.x, pt.y, size[i].cx, size[i].cy);

	GUI_SetColor(gDemoContext.fgcolor);
}

static void DemoDrawArc(void)
{
	UINT8 i = 0;
	const GUI_POINT pt = {240, 214};
	const int a0 = 0, a1 = 180;
	const GUI_COLOR color[] = {COLOR_1, COLOR_2, COLOR_3, COLOR_4, COLOR_5, COLOR_6, COLOR_7};

	GUI_SetPenSize(8);
	for(i = 0; i < 7; i++)
	{
		GUI_SetColor(color[i]);
		GUI_DrawArc(pt.x, pt.y, 14+8*(i+1), 0, a0, a1);
	}

	GUI_SetColor(gDemoContext.fgcolor);
	GUI_SetPenSize(gDemoContext.pensize);
}

static void DemoGraphicDisp(void)
{
	DemoDispTitle("Graphic Display:");
	GUI_SetDrawMode(GUI_DRAWMODE_NORMAL);

	DemoDrawRect();
	DemoDrawPolygon();
	DemoDrawCircle();
	DemoDrawEllipse();
	DemoDrawArc();
}


/*******************************************************************************************/

static void DemoMDFrame(void)
{
	const GUI_POINT point[] = { {0, 0}, {280, 0}, {280, 90}, {0, 90} };
	const GUI_POINT sp[] = { {20, 30}, {20, 125} };
	const int num = sizeof(point) / sizeof(GUI_POINT);

	GUI_DrawPolygon(point, num, sp[0].x, sp[0].y);
	GUI_DrawPolygon(point, num, sp[1].x, sp[1].y);
}

static void DemoDrawSingleKey(GUI_POINT pt, const char *pch, UINT8 mode)
{
	GUI_RECT rect;

	rect.x0 = pt.x;
	rect.y0 = pt.y;
	rect.x1 = pt.x + 16;
	rect.y1 = pt.y + 16;

	GUI_SetColor(GUI_LIGHTGRAY);
	GUI_FillRect(rect.x0, rect.y0, rect.x1, rect.y1);
	if(mode == MEM_DEV_OFF)
		GUI_Delay(100);

	GUI_SetColor(GUI_BLACK);
	GUI_SetTextMode(GUI_TEXTMODE_TRANS);
	GUI_DispStringInRect(pch, &rect, GUI_TA_HCENTER | GUI_TA_VCENTER);
	if(mode == MEM_DEV_OFF)
		GUI_Delay(100);

	GUI_SetColor(gDemoContext.fgcolor);
	GUI_SetTextMode(gDemoContext.textmode);
}

static void DemoDrawKeyBoard(UINT8 mode)
{
	UINT8 i = 0, j = 0, num;
	char **pch = NULL;
	GUI_POINT sp;
	const GUI_POINT point[] = { {44, 50}, {52, 70}, {60, 90} };
	const char *ch1[] = {"Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P", "[", "]"};
	const char *ch2[] = {"A", "S", "D", "F", "G", "H", "J", "K", "L", ";", "'"};
	const char *ch3[] = {"Z", "X", "C", "V", "B", "N", "M", ",", ".", "/"};
	const UINT8 num1 = sizeof(ch1) / sizeof(char*), num2 = sizeof(ch2) / sizeof(char*), num3 = sizeof(ch3) / sizeof(char*);

	for(i = 0; i < 3; i++)
	{
		sp = point[i];
		if(mode == MEM_DEV_ON)
			sp.y += 95;

		switch(i)
		{
		case 1:
			num = num2;
			pch = (char **)ch2;
			break;
		case 2:
			num = num3;
			pch = (char **)ch3;
			break;
		default :
			num = num1;
			pch = (char **)ch1;
			break;
		}

		for(j = 0; j < num; j ++)
		{
			DemoDrawSingleKey(sp, pch[j], mode);
			sp.x += 20;
		}
	}
}

static void DemoMemDevDispA(void)
{
	GUI_MEMDEV_Handle mem_dev_hdl;

	DemoDispTitle("Mem Device: Normal");
	DemoMDFrame();

	//	Not Use Memory Device
	DemoDrawKeyBoard(MEM_DEV_OFF);

	//	Use Memory Device
	mem_dev_hdl = GUI_MEMDEV_Create(20, 125, 280, 90);
	GUI_MEMDEV_Select(mem_dev_hdl);

	DemoDrawKeyBoard(MEM_DEV_ON);

	GUI_MEMDEV_CopyToLCDAt(mem_dev_hdl, 20, 125);
	GUI_MEMDEV_Select(0);
	GUI_MEMDEV_Delete(mem_dev_hdl);
}

static void DemoDrawColumn(char *str, UINT8 index, UINT8 str_num, UINT8 column)
{
	UINT8 i = 0, idx = 0;
	GUI_POINT sp;
	const UINT8 lin_max = 9;

	sp.x = 20*column;
	for(i = 0; i < lin_max; i ++)
	{
		sp.y = 30 + 20*i;
		idx = index + i;
		if(idx >= str_num)
			idx = idx - str_num;

		GUI_DispCharAt(str[idx], sp.x, sp.y);
	}
}

static void DemoDrawMatrix(void *pdata)
{
	UINT8 i = 0;
	UINT16 cnt = 0;
	const char str[] = "tDAwKxUeq76JHvuT4F21sMjBbCiuNY4dhLO9ZEn5faGIcR3gSXlV8";
	const UINT8 str_num = sizeof(str) / sizeof(char) - 1, column_max = 16;
	UINT8 index[] = {
			 0, 20, 40, 50,
			 5, 12, 37, 22,
			33,  7,  2, 43,
			15, 27, 39, 19
	};

	cnt = 200;
	while(cnt)
	{
		for(i = 0; i < column_max; i++)
		{
			DemoDrawColumn((char*)str, index[i], (UINT8)str_num, i);
			index[i] ++;
			if(index[i] >= str_num)
				index[i] = 0;
		}
		cnt --;
	}
	pdata = pdata;
}

static void DemoMemDevDispB(void)
{
	GUI_RECT rect = {0, 30, 319, 209};
	DemoDispTitle("Mem Device: Banding");

	GUI_SetColor(GUI_LIGHTGREEN);
	GUI_SetBkColor(GUI_BLACK);
	GUI_ClearRect(rect.x0, rect.y0, rect.x1, rect.y1);

	GUI_MEMDEV_Draw(&rect, DemoDrawMatrix, NULL, 5, 0);

	GUI_SetColor(gDemoContext.fgcolor);
	GUI_SetBkColor(gDemoContext.bgcolor);
}

static void DemoDrawAuto(void *pdata)
{
	PTMDParam pPara = (PTMDParam)pdata;
	const GUI_POINT srcp[] = { {0, -30}, {10, 0}, {0, 10}, {-10, 0} };
	GUI_POINT dstp[4];
	char *str[] = {"N", "S", "W", "E"};
	GUI_RECT rect[] = {
		{150,  50, 170,  70},
		{150, 180, 170, 200},
		{ 90, 110, 110, 130},
		{220, 110, 240, 130}
	};

	if(pPara->AutoDevInfo.DrawFixed)
	{
		GUI_SetBkColor(GUI_BLACK);
		GUI_ClearRect(0, 30, 319, 219);

		GUI_SetPenSize(6);
		GUI_SetColor(GUI_LIGHTBLUE);
		GUI_DrawArc(160, 120, 80, 0, 0, 360);

		GUI_SetColor(GUI_LIGHTGREEN);
		GUI_DispStringInRect(str[0], &rect[0], GUI_TA_HCENTER);
		GUI_DispStringInRect(str[1], &rect[1], GUI_TA_HCENTER);
		GUI_DispStringInRect(str[2], &rect[2], GUI_TA_VCENTER);
		GUI_DispStringInRect(str[3], &rect[3], GUI_TA_VCENTER);

		GUI_SetColor(GUI_RED);
	}

	GUI_RotatePolygon(dstp, srcp, 4, pPara->angle * 3.14f/180);
	GUI_FillPolygon(dstp, 4, 160, 120);
}

static void DemoMemDevDispC(void)
{
	UINT8 flag = 81, i = 0;
	TMDParam para;
	GUI_AUTODEV autodev;

	DemoDispTitle("Mem Device: Auto");

	GUI_MEMDEV_CreateAuto(&autodev);
	while(flag)
	{
		para.angle = (i % 8) * 45;
		GUI_MEMDEV_DrawAuto(&autodev, &para.AutoDevInfo, DemoDrawAuto, &para);
		GUI_Delay(100);
		
		flag --;
		i ++;
		if(i == 0xff)
			i = 0;
	}
	GUI_MEMDEV_DeleteAuto(&autodev);

	GUI_SetColor(gDemoContext.fgcolor);
	GUI_SetBkColor(gDemoContext.bgcolor);
	GUI_SetTextMode(gDemoContext.textmode);
}


/*******************************************************************************************/

static const char *str[] = {
		"option1", "option2", "option3", "option4",
		"option5", "option6", "option7", "option8"
};

static void DemoDlgCallback(WM_MESSAGE * pMsg)
{
	WM_HWIN h_edit0, h_edit1, h_edit2, h_edit3, h_edit4, h_lsbox0;

	switch (pMsg->MsgId)
	{
	case WM_INIT_DIALOG:
		h_edit0   = WM_GetDialogItem(pMsg->hWin, GUI_ID_EDIT0);
		h_edit1   = WM_GetDialogItem(pMsg->hWin, GUI_ID_EDIT1);
		h_edit2   = WM_GetDialogItem(pMsg->hWin, GUI_ID_EDIT2);
		h_edit3   = WM_GetDialogItem(pMsg->hWin, GUI_ID_EDIT3);
		h_edit4   = WM_GetDialogItem(pMsg->hWin, GUI_ID_EDIT4);
		h_lsbox0  = WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTBOX0);

		EDIT_SetText(h_edit0, "Hello World !");
		EDIT_SetText(h_edit1, "Hello World !");
		EDIT_SetTextAlign(h_edit1, GUI_TA_LEFT);
		EDIT_SetDecMode(h_edit2, -1234, -0xffff, 0xffff, 2, GUI_EDIT_SIGNED);
		EDIT_SetHexMode(h_edit3, 0x1234, 0, 0xffff);
		EDIT_SetBinMode(h_edit4, 0x1234, 0, 0xffff);

		LISTBOX_SetText(h_lsbox0, str);
		SCROLLBAR_CreateAttached(h_lsbox0, SCROLLBAR_CF_VERTICAL);

		CHECKBOX_Check(WM_GetDialogItem(pMsg->hWin, GUI_ID_CHECK0));
		CHECKBOX_Check(WM_GetDialogItem(pMsg->hWin, GUI_ID_CHECK1));
		WM_DisableWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_CHECK1));

		SLIDER_SetValue(WM_GetDialogItem(pMsg->hWin, GUI_ID_SLIDER0), 20);
		SLIDER_SetValue(WM_GetDialogItem(pMsg->hWin, GUI_ID_SLIDER1), 50);

		SCROLLBAR_SetValue(WM_GetDialogItem(pMsg->hWin, GUI_ID_SCROLLBAR0), 30);
		SCROLLBAR_SetValue(WM_GetDialogItem(pMsg->hWin, GUI_ID_SCROLLBAR1), 40);
		break;

	case WM_KEY:
		switch(((WM_KEY_INFO*)(pMsg->Data.p))->Key)
		{
		case KEY_END:
			GUI_EndDialog(pMsg->hWin, 1);
			break;
		default :
			break;
		}
		break;

	default:
		WM_DefaultProc(pMsg);
		break;
	}
}

static void DemoSetDialog(void)
{
	const GUI_WIDGET_CREATE_INFO pdlginfo[] = {
		{ FRAMEWIN_CreateIndirect, "Dialog",	0,					20,		30,		280,	180,	FRAMEWIN_CF_MOVEABLE,	0  },

		{ LISTBOX_CreateIndirect,  NULL,		GUI_ID_LISTBOX0,	10,		10,		100,	80 },

		{ TEXT_CreateIndirect,     "LText",		0,					110,	10,		48,		15,		TEXT_CF_RIGHT  },
		{ EDIT_CreateIndirect,     NULL,		GUI_ID_EDIT0,		160,	10,		100,	15,		0,						50 },
		{ TEXT_CreateIndirect,     "RText",		0,					110,	30,		48,		15,		TEXT_CF_RIGHT },
		{ EDIT_CreateIndirect,     NULL,		GUI_ID_EDIT1,		160,	30,		100,	15,		0,						50 },
		{ TEXT_CreateIndirect,     "Dec",		0,					110,	50,		48,		15,		TEXT_CF_RIGHT },
		{ EDIT_CreateIndirect,     NULL,		GUI_ID_EDIT2,		160,	50,		100,	15 },
		{ TEXT_CreateIndirect,     "Hex",		 0,					110,	70,		48,		15,		TEXT_CF_RIGHT },
		{ EDIT_CreateIndirect,     NULL,		GUI_ID_EDIT3,		160,	70,		100,	15,		0,						6 },
		{ TEXT_CreateIndirect,     "Bin",		0,					110,	90,		48,		15,		TEXT_CF_RIGHT },
		{ EDIT_CreateIndirect,     NULL,		GUI_ID_EDIT4,		160,	90,		100,	15 },

		{ CHECKBOX_CreateIndirect, NULL,		GUI_ID_CHECK0,		10,		95,		0,		0 },
		{ TEXT_CreateIndirect,     "CheckBox",  0,					30,		95,		48,		15,		TEXT_CF_LEFT  },
		{ CHECKBOX_CreateIndirect, NULL,		GUI_ID_CHECK1,		10,		110,	0,		0 },
		{ TEXT_CreateIndirect,     "CheckBox",  0,					30,		110,	48,		15,		TEXT_CF_LEFT  },

		{ SLIDER_CreateIndirect,   NULL,		GUI_ID_SLIDER0,		10,		130,	100,	10 },
		{ SLIDER_CreateIndirect,   NULL,		GUI_ID_SLIDER1,		180,	110,	15,		50,		SLIDER_CF_VERTICAL },

		{ SCROLLBAR_CreateIndirect,NULL,		GUI_ID_SCROLLBAR0,	10,		145,	100,	10 },
		{ SCROLLBAR_CreateIndirect,NULL,		GUI_ID_SCROLLBAR1,	220,	110,	15,		50,		SCROLLBAR_CF_VERTICAL }
	};
	UINT8 num = sizeof(pdlginfo) / sizeof(GUI_WIDGET_CREATE_INFO);

	GUI_MessageBox("System Error !", "Warning", GUI_MESSAGEBOX_CF_MOVEABLE);
	GUI_CreateDialogBox(pdlginfo, num, DemoDlgCallback, 0, 0, 0);
};

static void DemoDialogDisp(void)
{
	DemoDispTitle("Dialog :");
	DemoSetDialog();
}


/*******************************************************************************************/

static void DemoDrawWin(WM_HWIN hdl, UINT8 idx)
{
	const GUI_COLOR color[] = {GUI_WHITE, GUI_BROWN};
	const GUI_COLOR bk_color[] = {GUI_BLUE, GUI_GREEN};
	const char *str[] = {"Window 1", "Window 2"};
	const GUI_BITMAP *bmp[] = {&wm1_bmp, &wm2_bmp};

	WM_SelectWindow(hdl);
	GUI_SetColor(color[idx]);
	GUI_SetBkColor(bk_color[idx]);
	GUI_Clear();
	GUI_DispStringAt(str[idx], 2, 4);
	GUI_DrawBitMap(2, 20, bmp[idx]);
}

static void DemoWMDisp(void)
{
	UINT8 i = 0, j = 0;
	WM_HWIN h_win1, h_win2;
	const GUI_POINT mv_step[] = {{2, 0}, {0, 2}, {-2, 0}, {0, -2}};

	DemoDispTitle("Windows Manager :");

	h_win1 = WM_CreateWindow(10, 30, 164, 142, WM_CF_SHOW, NULL, 0);
	DemoDrawWin(h_win1, 0);

	h_win2 = WM_CreateWindow(70, 70, 164, 142, WM_CF_SHOW, NULL, 0);
	DemoDrawWin(h_win2, 1);

	WM_EnableMemdev(h_win1);
	WM_SelectWindow(h_win1);
	for(i = 0; i < 4; i++)
	{
		for(j = 0; j < 30; j++)
		{
			GUI_SetBkColor(gDemoContext.bgcolor);
			GUI_Clear();
			WM_MoveWindow(h_win1, mv_step[i].x, mv_step[i].y);
			DemoDrawWin(h_win1, 0);
			GUI_Delay(2);
		}
	}
	WM_DisableMemdev(h_win1);

	WM_DeleteWindow(h_win1);
	WM_DeleteWindow(h_win2);
	GUI_SetColor(gDemoContext.fgcolor);
	GUI_SetBkColor(gDemoContext.bgcolor);
}


/**************************************************************

					Demo Main Function

***************************************************************/

void DemoMain(void)
{
	const PDemoFunc pfunc[] = {
			DemoInit,
			DemoTextDisp,
			DemoValueDispA,
			DemoValueDispB,
			DemoFontDisp,
			DemoGraphicDisp,
			DemoMemDevDispA,
			DemoMemDevDispB,
			DemoMemDevDispC,
			DemoDialogDisp,
			DemoWMDisp
	};
	const UINT8 num = sizeof(pfunc) / sizeof(PDemoFunc);
	UINT8 i;

	for(i = 0; i < num; i++)
	{
		GUI_Clear();
		GUI_GotoXY(0, 0);

		(pfunc[i])();

		if(i == 0)
			DemoWaitBtn("Start ->");
		else if(i == num - 1)
			DemoWaitBtn("-- End --");
		else
			DemoWaitBtn("Next  ->");
	}
}
