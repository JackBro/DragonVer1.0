/*
*********************************************************************************************************
*                                                uC/GUI
*                        Universal graphic software for embedded applications
*
*                       (c) Copyright 2002, Micrium Inc., Weston, FL
*                       (c) Copyright 2002, SEGGER Microcontroller Systeme GmbH
*
*              µC/GUI is protected by international copyright laws. Knowledge of the
*              source code may not be used to write a similar product. This file may
*              only be used in accordance with a license and should not be redistributed
*              in any way. We appreciate your understanding and fairness.
*
----------------------------------------------------------------------
File        : SIM_X.c
Purpose     : Windows Simulator externals
              The init routine in this file can be used to set up the
              simulator
---------------------------END-OF-HEADER------------------------------
*/

#include "SIM.h"
#include "../terminal/terminal/v568/v568phone/v568phone.h"

void SIM_X_Init() {
  //SIM_SetLCDPos(80,80);     // Define the position of the LCD in the bitmap
}

#if	0
void main(void)
{
	PhoneMain();
}
#endif


void main(void)
{
	UINT8 flag;

	GUI_Init();

	flag = 1;
	while(flag)
	{
		DemoMain();
	}
}
