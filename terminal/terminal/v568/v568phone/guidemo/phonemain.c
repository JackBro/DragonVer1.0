/*
*********************************************************************************************************
*                                                µC/GUI
*                        Universal graphic software for embedded applications
*
*                       (c) Copyright 2002, Micrium Inc., Weston, FL
*                       (c) Copyright 2000, SEGGER Microcontroller Systeme GmbH          
*
*              µC/GUI is protected by international copyright laws. Knowledge of the
*              source code may not be used to write a similar product. This file may
*              only be used in accordance with a license and should not be redistributed 
*              in any way. We appreciate your understanding and fairness.
*
* File        : Main.c
* Purpose     : Application program in windows simulator
*********************************************************************************************************
*/


#include "../../../graphics/gui/core/GUI.h"

extern const GUI_BITMAP bmMicriumLogo;
extern const GUI_BITMAP bmMicriumLogo_1bpp;


/*
  *******************************************************************
  *
  *              main()
  *
  *******************************************************************
*/

#if 0
void main(void) {
  int Cnt =0;
  int i,YPos;
  int LCDXSize = LCD_GET_XSIZE();
  int LCDYSize = LCD_GET_YSIZE();
  const GUI_BITMAP *pBitmap;
  GUI_Init();
  GUI_SetBkColor(GUI_RED); GUI_Clear();
  GUI_Delay(1000);
  GUI_SetBkColor(GUI_BLUE); GUI_Clear();
  GUI_Delay(1000);
  GUI_SetColor(GUI_WHITE);
  for (i=0; i<1000; i+=10) {
    GUI_DrawHLine(i,0,100);
    GUI_DispStringAt("Line ",0,i);
    GUI_DispDecMin(i);
  }
  GUI_Delay(1000);
  GUI_SetColor(0x0);
  GUI_SetBkColor(0xffffff);
  for (i=0; i<160; i++) {
    int len = (i<80) ? i : 160-i;
    GUI_DrawHLine(i,20,len+20);
  }
  GUI_Delay(1000);
  GUI_Clear();
  if (LCD_GET_YSIZE()>(100+bmMicriumLogo_1bpp.YSize)) {
    pBitmap=&bmMicriumLogo;
  } else {
    GUI_SetColor(GUI_BLUE);
    pBitmap=&bmMicriumLogo_1bpp;
  }
  GUI_DrawBitmap(pBitmap,(LCDXSize-pBitmap->XSize)/2,10);
  YPos=20+pBitmap->YSize;
  GUI_SetFont(&GUI_FontComic24B_1);
  GUI_DispStringHCenterAt("www.micrium.com",LCDXSize/2,YPos);
  GUI_Delay(1000);
  GUI_SetColor(GUI_RED);
  GUI_DispStringHCenterAt("?2002\n", LCDXSize/2,YPos+30);
  GUI_SetFont(&GUI_Font10S_1);
  GUI_DispStringHCenterAt("Micriµm Inc.",LCDXSize/2,YPos+60);;
  GUI_Delay(1000);
  while(1);
}

#else

#if 0

#define countof(Array) (sizeof(Array) / sizeof(Array[0]))
const GUI_POINT aPoints[] = {
	{ 0, 20},
	{ 40, 20},
	{ 20, 0}
};
GUI_POINT aEnlargedPoints[countof(aPoints)];
void Sample(void) {
	int i;
	GUI_Clear();
	GUI_SetDrawMode(GUI_DM_XOR);
	GUI_FillPolygon(aPoints, countof(aPoints), 140, 110);
	GUI_Delay(1000);
	for (i = 1; i < 10; i++) {
		GUI_EnlargePolygon(aEnlargedPoints, aPoints, countof(aPoints), i * 5);
		GUI_Delay(1000);
		GUI_FillPolygon(aEnlargedPoints, countof(aPoints), 140, 110);
		GUI_Delay(1000);
	}
}


void ShowColorBar(void) {
	int x0 = 60, y0 = 40, yStep = 15, i;
	int NumColors = LCD_GetDevCap(LCD_DEVCAP_NUMCOLORS);
	int xsize = LCD_GetDevCap(LCD_DEVCAP_XSIZE) - x0;
	GUI_SetFont(&GUI_Font13HB_1);
	GUI_DispStringHCenterAt("¦ÌC/GUI-sample: Show color bars", 160, 0);
	GUI_SetFont(&GUI_Font8x8);
	GUI_SetColor(GUI_WHITE);
	GUI_SetBkColor(GUI_BLACK);
#if (LCD_FIXEDPALETTE)
	GUI_DispString("Fixed palette: ");
	GUI_DispDecMin(LCD_FIXEDPALETTE);
#endif
	GUI_DispStringAt("Red", 0, y0 + yStep);
	GUI_DispStringAt("Green", 0, y0 + 3 * yStep);
	GUI_DispStringAt("Blue", 0, y0 + 5 * yStep);
	GUI_DispStringAt("Grey", 0, y0 + 6 * yStep);
	GUI_DispStringAt("Yellow", 0, y0 + 8 * yStep);
	GUI_DispStringAt("Cyan", 0, y0 + 10 * yStep);
	GUI_DispStringAt("Magenta", 0, y0 + 12 * yStep);
	for (i = 0; i < xsize; i++) {
		U16 cs = (255 * (U32)i) / xsize;
		U16 x = x0 + i;;
		/* Red */
		GUI_SetColor(cs);
		GUI_DrawVLine(x, y0 , y0 + yStep - 1);
		GUI_SetColor(0xff + (255 - cs) * 0x10100L);
		GUI_DrawVLine(x, y0 + yStep, y0 + 2 * yStep - 1);
		/* Green */
		GUI_SetColor(cs<<8);
		GUI_DrawVLine(x, y0 + 2 * yStep, y0 + 3 * yStep - 1);
		GUI_SetColor(0xff00 + (255 - cs) * 0x10001L);
		GUI_DrawVLine(x, y0 + 3 * yStep, y0 + 4 * yStep - 1);
		/* Blue */
		GUI_SetColor(cs * 0x10000L);
		GUI_DrawVLine(x, y0 + 4 * yStep, y0 + 5 * yStep - 1);
		GUI_SetColor(0xff0000 + (255 - cs) * 0x101L);
		GUI_DrawVLine(x, y0 + 5 * yStep, y0 + 6 * yStep - 1);
		/* Gray */
		GUI_SetColor((U32)cs * 0x10101L);
		GUI_DrawVLine(x, y0 + 6 * yStep, y0 + 7 * yStep - 1);
		/* Yellow */
		GUI_SetColor(cs * 0x101);
		GUI_DrawVLine(x, y0 + 7 * yStep, y0 + 8 * yStep - 1);
		GUI_SetColor(0xffff + (255 - cs) * 0x10000L);
		GUI_DrawVLine(x, y0 + 8 * yStep, y0 + 9 * yStep - 1);
		/* Cyan */
		GUI_SetColor(cs * 0x10100L);
		GUI_DrawVLine(x, y0 + 9 * yStep, y0 + 10 * yStep - 1);
		GUI_SetColor(0xffff00 + (255 - cs) * 0x1L);
		GUI_DrawVLine(x, y0 + 10 * yStep, y0 + 11 * yStep - 1);
		/* Magenta */
		GUI_SetColor(cs * 0x10001);
		GUI_DrawVLine(x, y0 + 11 * yStep, y0 + 12 * yStep - 1);
		GUI_SetColor(0xff00ff + (255 - cs) * 0x100L);
		GUI_DrawVLine(x, y0 + 12 * yStep, y0 + 13 * yStep - 1);
	}
}
#endif



static const GUI_POINT aPoints[] = {
	{-50, 0},
	{-10, 10},
	{ 0, 50},
	{ 10, 10},
	{ 50, 0},
	{ 10,-10},
	{ 0,-50},
	{-10,-10}
};
#define SIZE_OF_ARRAY(Array) (sizeof(Array) / sizeof(Array[0]))
typedef struct {
	int XPos_Poly, YPos_Poly;
	int XPos_Text, YPos_Text;
	GUI_POINT aPointsDest[8];
} tDrawItContext;
/*******************************************************************
*
* Drawing routine
*
********************************************************************
*/
static void DrawIt(void * pData) {
	static I8 temp = 0;
	U32 color = 0;
	tDrawItContext * pDrawItContext = (tDrawItContext *)pData;
	GUI_Clear();
	GUI_SetFont(&GUI_Font8x8);
	GUI_SetTextMode(GUI_TM_TRANS);
	/* draw background */

	switch(temp)
	{
	case 0:
		color = GUI_GREEN;
		break;
	case 1:
		color = GUI_BLUE;
		break;
	case 2:
		color = GUI_RED;
		break;
	default: 
		break;
	}
	//GUI_SetColor(GUI_GREEN);
	GUI_SetColor(color);
	GUI_FillRect(pDrawItContext->XPos_Text,
		pDrawItContext->YPos_Text - 25,
		pDrawItContext->XPos_Text + 100,
		pDrawItContext->YPos_Text - 5);
	/* draw polygon */
	//GUI_SetColor(GUI_BLUE);
	GUI_SetColor(color);
	GUI_FillPolygon(pDrawItContext->aPointsDest, SIZE_OF_ARRAY(aPoints), 160, 120);
	/* draw foreground */
	//GUI_SetColor(GUI_RED);
	GUI_SetColor(color);
	GUI_FillRect(220 - pDrawItContext->XPos_Text,
		pDrawItContext->YPos_Text + 5,
		220 - pDrawItContext->XPos_Text + 100,
		pDrawItContext->YPos_Text + 25);
	GUI_WaitKey();
	//GUI_Delay(200);
	temp++;
	temp %= 3;
}
/*******************************************************************
*
* Demonstrates the banding memory device
*
********************************************************************
*/
#define USE_BANDING_MEMDEV (1) /* Set to 0 for drawing without banding memory device
*/
void DemoBandingMemdev(void) {
	int i;
	int XSize = LCD_GET_XSIZE();
	int YSize = LCD_GET_YSIZE();
	tDrawItContext DrawItContext;
	GUI_SetFont(&GUI_Font8x9);
	GUI_SetColor(GUI_WHITE);
	GUI_DispStringHCenterAt("Banding memory device\nwithout flickering",
		XSize / 2, 40);
	DrawItContext.XPos_Poly = XSize / 2;
	DrawItContext.YPos_Poly = YSize / 2;
	DrawItContext.YPos_Text = YSize / 2 - 4;
	for (i = 0; i < (XSize - 100); i++) 
	{
		float angle = i * 3.1415926 / 60;
		DrawItContext.XPos_Text = i;
//		130 CHAPTER 10 Memory Devices
			/* Rotate the polygon */
			GUI_RotatePolygon(DrawItContext.aPointsDest,
			aPoints,
			SIZE_OF_ARRAY(aPoints), angle);
#if USE_BANDING_MEMDEV
		{
			GUI_RECT Rect = {0, 70, 320,170};
			/* Use banding memory device for drawing */
			GUI_MEMDEV_Draw(&Rect, &DrawIt, &DrawItContext, 0, 0);
		}
#else
		/* Simple drawing without using memory devices */
		DrawIt((void *)&DrawItContext);
#endif
#ifdef WIN32
		GUI_Delay(20); /* Use a short delay only in the simulation */
#endif
	}
}
/*******************************************************************
*
* main
*
********************************************************************
*/
#if 0

void main(void) {
	int i = 0;

	GUI_Init();
	while(1) {
		DemoBandingMemdev();
		GUI_DispDecAt(i++%10000, 200, 200, 4);

	}
}
#endif




void PhoneMain(void)
{
	int flag = 1;

	GUI_Init();

	GUI_DispDecAt(0, 200, 200, 4);
	GUI_DrawLine(20, 20, 40, 40);
	while(flag)
	{
	}
}

#if 0
void main(void) 
{
	int i = 0;
	GUI_Init();

	ShowColorBar();
	while(1)
	GUI_Delay(100);

#if 0
	GUI_DispString("Hello world");
	Sample();
	while(1)
	{
		GUI_DispDecAt(i++, 200, 200, 4);
		if(i > 9999) 
			i = 0;
	}
#endif
}
#endif

#endif
