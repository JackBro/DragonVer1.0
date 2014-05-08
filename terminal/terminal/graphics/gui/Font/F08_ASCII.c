/*
*********************************************************************************************************
*                                                uC/GUI
*                        Universal graphic software for embedded applications
*
*                       (c) Copyright 2002, Micrium Inc., Weston, FL
*                       (c) Copyright 2002, SEGGER Microcontroller Systeme GmbH
*
*              �C/GUI is protected by international copyright laws. Knowledge of the
*              source code may not be used to write a similar product. This file may
*              only be used in accordance with a license and should not be redistributed
*              in any way. We appreciate your understanding and fairness.
*
----------------------------------------------------------------------
File        : F8_ASCII.C
Purpose     : Proportional 8 pixel font, ASCII character set
Height      : 8
---------------------------END-OF-HEADER------------------------------
*/

#include "../core/GUI.H"

extern const GUI_FONT GUI_Font8_ASCII;

#ifndef GUI_FLASH
  #define GUI_FLASH
#endif

GUI_FLASH const unsigned char acFont8ASCII[][8] = {
  {
   ________,
   ________,
   ________,
   ________,
   ________,
   ________,
   ________,
   ________}

/*  !  */
 ,{
   X_______,
   X_______,
   X_______,
   X_______,
   X_______,
   ________,
   X_______,
   ________}


/*  "  */
 ,{
   X_X_____,
   X_X_____,
   X_X_____,
   ________,
   ________,
   ________,
   ________,
   ________}

/* #  */
 ,{
   _X_X____,
   _X_X____,
   XXXXX___,
   _X_X____,
   XXXXX___,
   _X_X____,
   _X_X____,
   ________}

/*  $  */
 ,{
   __X_____,
   _XXXX___,
   X_X_____,
   _XXX____,
   __X_X___,
   XXXX____,
   __X_____}

/*  %  */
 ,{
   XX______,
   XX__X___,
   ___X____,
   __X_____,
   _X______,
   X__XX___,
   ___XX___,
   ________}

/*  &  */
 ,{
   _XX_____,
   X__X____,
   X_X_____,
   _X______,
   X_X_X___,
   X__X____,
   _XX_X___,
   ________}

/*  '  */
 ,{
   XX______,
   _X______,
   X_______,
   ________,
   ________,
   ________,
   ________,
   ________}

/*  (  */
 ,{
   __X_____,
   _X______,
   X_______,
   X_______,
   X_______,
   _X______,
   __X_____,
   ________}

/*  )  */
 ,{
   X_______,
   _X______,
   __X_____,
   __X_____,
   __X_____,
   _X______,
   X_______,
   ________}

/*  *  */
 ,{
   ________,
   _X_X____,
   __X_____,
   XXXXX___,
   __X_____,
   _X_X____,
   ________,
   ________}

 ,{
   ________,
   __X_____,
   __X_____,
   XXXXX___,
   __X_____,
   __X_____,
   ________,
   ________}

 ,{
   ________,
   ________,
   ________,
   ________,
   ________,
   XX______,
   _X______,
   X_______}

 ,{
   ________,
   ________,
   ________,
   XXXXX___,
   ________,
   ________,
   ________,
   ________}

 ,{
   ________,
   ________,
   ________,
   ________,
   ________,
   XX______,
   XX______,
   ________}

 ,{
   ________,
   ____X___,
   ___X____,
   __X_____,
   _X______,
   X_______,
   ________,
   ________}

/* 0 */
 ,{
   _XXX____,
   X___X___,
   X__XX___,
   X_X_X___,
   XX__X___,
   X___X___,
   _XXX____,
   ________}

/* 1 */
 ,{
   __X_____,
   _XX_____,
   __X_____,
   __X_____,
   __X_____,
   __X_____,
   _XXX____,
   ________}

/* 2 */
 ,{
   _XXX____,
   X___X___,
   ____X___,
   __XX____,
   _X______,
   X_______,
   XXXXX___,
   ________}

/* 3 */
 ,{
   _XXX____,
   X___X___,
   ____X___,
   __XX____,
   ____X___,
   X___X___,
   _XXX____,
   ________}

/* 4 */
 ,{
   ___X____,
   __XX____,
   _X_X____,
   X__X____,
   XXXXX___,
   ___X____,
   ___X____,
   ________}

/* 5 */
 ,{
   XXXXX___,
   X_______,
   XXXX____,
   ____X___,
   ____X___,
   X___X___,
   _XXX____,
   ________}

/* 6 */
 ,{
   __XX____,
   _X______,
   X_______,
   XXXX____,
   X___X___,
   X___X___,
   _XXX____,
   ________}

/* 7 */
 ,{
   XXXXX___,
   ____X___,
   ___X____,
   __X_____,
   _X______,
   _X______,
   _X______,
   ________}

/* 8 */
 ,{
   _XXX____,
   X___X___,
   X___X___,
   _XXX____,
   X___X___,
   X___X___,
   _XXX____,
   ________}

/* 9 */
 ,{
   _XXX____,
   X___X___,
   X___X___,
   _XXXX___,
   ____X___,
   ___X____,
   _XX_____,
   ________}

/* ':' 3a */
 ,{
   ________,
   XX______,
   XX______,
   ________,
   XX______,
   XX______,
   ________,
   ________}

/* ';' 3b */
 ,{
   ________,
   ________,
   XX______,
   XX______,
   ________,
   XX______,
   _X______,
   X_______}


/* '<' 3c */
 ,{
   ___X____,
   __X_____,
   _X______,
   X_______,
   _X______,
   __X_____,
   ___X____,
   ________}

/* '=' 3d */
 ,{
   ________,
   ________,
   XXXXX___,
   ________,
   XXXXX___,
   ________,
   ________,
   ________}

/* '>' */
 ,{
   X_______,
   _X______,
   __X_____,
   ___X____,
   __X_____,
   _X______,
   X_______,
   ________}

/* '?' */
 ,{
   _XXX____,
   X___X___,
   ____X___,
   ___X____,
   __X_____,
   ________,
   __X_____,
   ________}

/* @ */
 ,{
   _XXX____,
   X___X___,
   ____X___,
   _XX_X___,
   X_X_X___,
   X_X_X___,
   _XXX____,
   ________}

/* A */
 ,{
   _XXX____,
   X___X___,
   X___X___,
   XXXXX___,
   X___X___,
   X___X___,
   X___X___,
   ________}

/* B */
 ,{
   XXXX____,
   X___X___,
   X___X___,
   XXXX____,
   X___X___,
   X___X___,
   XXXX____,
   ________}

/* C */
 ,{
   _XXX____,
   X___X___,
   X_______,
   X_______,
   X_______,
   X___X___,
   _XXX____,
   ________}

/* D */
 ,{
   XXX_____,
   X__X____,
   X___X___,
   X___X___,
   X___X___,
   X__X____,
   XXX_____,
   ________}

/* E */
 ,{
   XXXXX___,
   X_______,
   X_______,
   XXXX____,
   X_______,
   X_______,
   XXXXX___,
   ________}

/* 1 */
 ,{
   XXXXX___,
   X_______,
   X_______,
   XXXX____,
   X_______,
   X_______,
   X_______,
   ________}

/* 1 */
 ,{
   _XXX____,
   X___X___,
   X_______,
   X_______,
   X__XX___,
   X___X___,
   _XXXX___,
   ________}

/* 1 */
 ,{
   X___X___,
   X___X___,
   X___X___,
   XXXXX___,
   X___X___,
   X___X___,
   X___X___,
   ________}

/* I */
 ,{
   XXX_____,
   _X______,
   _X______,
   _X______,
   _X______,
   _X______,
   XXX_____,
   ________}

/* J */
 ,{
   __XXX___,
   ___X____,
   ___X____,
   ___X____,
   ___X____,
   X__X____,
   _XX_____,
   ________}

/* K */
 ,{
   X___X___,
   X__X____,
   X_X_____,
   XX______,
   X_X_____,
   X__X____,
   X___X___,
   ________}

/* L */
 ,{
   X_______,
   X_______,
   X_______,
   X_______,
   X_______,
   X_______,
   XXXXX___,
   ________}

/* M */
 ,{
   X___X___,
   XX_XX___,
   X_X_X___,
   X_X_X___,
   X___X___,
   X___X___,
   X___X___,
   ________}

/* N */
 ,{
   X___X___,
   X___X___,
   XX__X___,
   X_X_X___,
   X__XX___,
   X___X___,
   X___X___,
   ________}

/* O */
 ,{
   _XXX____,
   X___X___,
   X___X___,
   X___X___,
   X___X___,
   X___X___,
   _XXX____,
   ________}

/* P */
 ,{
   XXXX____,
   X___X___,
   X___X___,
   XXXX____,
   X_______,
   X_______,
   X_______,
   ________}

/* Q */
 ,{
   _XXX____,
   X___X___,
   X___X___,
   X___X___,
   X_X_X___,
   X__X____,
   _XX_X___,
   ________}

/* R */
 ,{
   XXXX____,
   X___X___,
   X___X___,
   XXXX____,
   X_X_____,
   X__X____,
   X___X___,
   ________}

/* S */
 ,{
   _XXX____,
   X___X___,
   X_______,
   _XXX____,
   ____X___,
   X___X___,
   _XXX____,
   ________}

/* T */
 ,{
   XXXXX___,
   __X_____,
   __X_____,
   __X_____,
   __X_____,
   __X_____,
   __X_____,
   ________}

/* U */
 ,{
   X___X___,
   X___X___,
   X___X___,
   X___X___,
   X___X___,
   X___X___,
   _XXX____,
   ________}

/* V */
 ,{
   X___X___,
   X___X___,
   X___X___,
   X___X___,
   X___X___,
   _X_X____,
   __X_____,
   ________}

/* W */
 ,{
   X___X___,
   X___X___,
   X___X___,
   X_X_X___,
   X_X_X___,
   X_X_X___,
   _X_X____,
   ________}

/* X */
 ,{
   X___X___,
   X___X___,
   _X_X____,
   __X_____,
   _X_X____,
   X___X___,
   X___X___,
   ________}

/* Y */
 ,{
   X___X___,
   X___X___,
   _X_X____,
   __X_____,
   __X_____,
   __X_____,
   __X_____,
   ________}

/* Z */
 ,{
   XXXXX___,
   ____X___,
   ___X____,
   __X_____,
   _X______,
   X_______,
   XXXXX___,
   ________}

/* 5b */
 ,{
   XXX_____,
   X_______,
   X_______,
   X_______,
   X_______,
   X_______,
   XXX_____,
   ________}

/* 5c */
 ,{
   ________,
   X_______,
   _X______,
   __X_____,
   ___X____,
   ____X___,
   ________,
   ________}

/* 5d */
 ,{
   XXX_____,
   __X_____,
   __X_____,
   __X_____,
   __X_____,
   __X_____,
   XXX_____,
   ________}

/* 5e */
 ,{
   __X_____,
   _X_X____,
   X___X___,
   ________,
   ________,
   ________,
   ________,
   ________}

/* 5f */
 ,{
   ________,
   ________,
   ________,
   ________,
   ________,
   ________,
   ________,
   XXXXX___}

/* 60 */
 ,{
   X_______,
   _X______,
   __X_____,
   ________,
   ________,
   ________,
   ________,
   ________}

/* a */
 ,{
   ________,
   ________,
   _XXX____,
   ____X___,
   _XXXX___,
   X___X___,
   _XXXX___,
   ________}

/* b */
 ,{
   X_______,
   X_______,
   X_XX____,
   XX__X___,
   X___X___,
   X___X___,
   XXXX____,
   ________}

/* c */
 ,{
   ________,
   ________,
   _XX_____,
   X__X____,
   X_______,
   X__X____,
   _XX_____,
   ________}

/* d */
 ,{
   ____X___,
   ____X___,
   _XX_X___,
   X__XX___,
   X___X___,
   X___X___,
   _XXXX___,
   ________}

/* e */
 ,{
   ________,
   ________,
   _XXX____,
   X___X___,
   XXXXX___,
   X_______,
   _XXX____,
   ________}

/* f */
 ,{
   __X_____,
   _X_X____,
   _X______,
   XXX_____,
   _X______,
   _X______,
   _X______,
   ________}

/* g */
 ,{
   ________,
   ________,
   _XXXX___,
   X___X___,
   X___X___,
   _XXXX___,
   ____X___,
   _XXX____}

/* h */
 ,{
   X_______,
   X_______,
   X_XX____,
   XX__X___,
   X___X___,
   X___X___,
   X___X___,
   ________}

/* i */
 ,{
   X_______,
   ________,
   X_______,
   X_______,
   X_______,
   X_______,
   X_______,
   ________}

/* j */
 ,{
   __X_____,
   ________,
   _XX_____,
   __X_____,
   __X_____,
   __X_____,
   __X_____,
   XX______}

/* k */
 ,{
   X_______,
   X_______,
   X__X____,
   X_X_____,
   XX______,
   X_X_____,
   X__X____,
   ________}

/* l */
 ,{
   XX______,
   _X______,
   _X______,
   _X______,
   _X______,
   _X______,
   XXX_____,
   ________}

/* m */
 ,{
   ________,
   ________,
   XX_X____,
   X_X_X___,
   X___X___,
   X___X___,
   X___X___,
   ________}

/* n */
 ,{
   ________,
   ________,
   X_XX____,
   XX_X____,
   X__X____,
   X__X____,
   X__X____,
   ________}

/* o */
 ,{
   ________,
   ________,
   _XX_____,
   X__X____,
   X__X____,
   X__X____,
   _XX_____,
   ________}

/* p */
 ,{
   ________,
   ________,
   XXX_____,
   X__X____,
   X__X____,
   XXX_____,
   X_______,
   X_______}

/* q */
 ,{
   ________,
   ________,
   _XXX____,
   X__X____,
   X__X____,
   _XXX____,
   ___X____,
   ___X____}

/* r */
 ,{
   ________,
   ________,
   X_X_____,
   XX______,
   X_______,
   X_______,
   X_______,
   ________}

/* s */
 ,{
   ________,
   ________,
   _XXX____,
   X_______,
   _XX_____,
   ___X____,
   XXX_____,
   ________}

/* t */
 ,{
   _X______,
   _X______,
   XXX_____,
   _X______,
   _X______,
   _X______,
   _X______,
   ________}

/* u */
 ,{
   ________,
   ________,
   X__X____,
   X__X____,
   X__X____,
   X__X____,
   _XXX____,
   ________}

/* v */
 ,{
   ________,
   ________,
   X___X___,
   X___X___,
   X___X___,
   _X_X____,
   __X_____,
   ________}

/* w */
 ,{
   ________,
   ________,
   X___X___,
   X___X___,
   X_X_X___,
   X_X_X___,
   _X_X____,
   ________}

/* X */
 ,{
   ________,
   ________,
   X___X___,
   _X_X____,
   __X_____,
   _X_X____,
   X___X___,
   ________}

/* y */
 ,{
   ________,
   ________,
   X__X____,
   X__X____,
   X__X____,
   _XXX____,
   ___X____,
   _XX_____}

/* z */
 ,{
   ________,
   ________,
   XXXXX___,
   ___X____,
   __X_____,
   _X______,
   XXXXX___,
   ________}

/* 0x7b */
 ,{
   __X_____,
   _X______,
   _X______,
   X_______,
   _X______,
   _X______,
   __X_____,
   ________}

/* 0x7c */
 ,{
   _X______,
   _X______,
   _X______,
   _X______,
   _X______,
   _X______,
   _X______,
   ________}

/* 0x7d */
 ,{
   X_______,
   _X______,
   _X______,
   __X_____,
   _X______,
   _X______,
   X_______,
   ________}

/* 0x7e */
 ,{
   _XX_X___,
   X__X____,
   ________,
   ________,
   ________,
   ________,
   ________,
   ________}
};


GUI_FLASH const GUI_CHARINFO GUI_Font8_ASCII_CharInfo[95] = {
   { 3,3,1, (void GUI_FLASH *)&acFont8ASCII[0]}  /* char.code 32: ' ' */
  ,{ 3,3,1, (void GUI_FLASH *)&acFont8ASCII[1]}  /* char.code 33: '!' */
  ,{ 4,4,1, (void GUI_FLASH *)&acFont8ASCII[2]}  /* char.code 34: '"' */
  ,{ 6,6,1, (void GUI_FLASH *)&acFont8ASCII[3]}  /* char.code 35: '#' */
  ,{ 6,6,1, (void GUI_FLASH *)&acFont8ASCII[4]}  /* char.code 36: '$' */
  ,{ 6,6,1, (void GUI_FLASH *)&acFont8ASCII[5]}  /* char.code 37: '%' */
  ,{ 6,6,1, (void GUI_FLASH *)&acFont8ASCII[6]}  /* char.code 38: '&' */
  ,{ 3,3,1, (void GUI_FLASH *)&acFont8ASCII[7]}  /* char.code 39: ''' */
  ,{ 5,5,1, (void GUI_FLASH *)&acFont8ASCII[8]}  /* char.code 40: '(' */
  ,{ 5,5,1, (void GUI_FLASH *)&acFont8ASCII[9]}  /* char.code 41: ')' */
  ,{ 6,6,1, (void GUI_FLASH *)&acFont8ASCII[10]} /* char.code 42: '*' */
  ,{ 6,6,1, (void GUI_FLASH *)&acFont8ASCII[11]} /* char.code 43: '+' */
  ,{ 4,4,1, (void GUI_FLASH *)&acFont8ASCII[12]} /* char.code 44: ',' */
  ,{ 6,6,1, (void GUI_FLASH *)&acFont8ASCII[13]} /* char.code 45: '-' */
  ,{ 3,3,1, (void GUI_FLASH *)&acFont8ASCII[14]} /* char.code 46: '.' point */
  ,{ 6,6,1, (void GUI_FLASH *)&acFont8ASCII[15]} /* char.code 47: '/' */
  ,{ 6,6,1, (void GUI_FLASH *)&acFont8ASCII[16]} /* char.code 48: '0' */
  ,{ 6,6,1, (void GUI_FLASH *)&acFont8ASCII[17]} /* char.code 49: '1' */
  ,{ 6,6,1, (void GUI_FLASH *)&acFont8ASCII[18]} /* char.code 50: '2' */
  ,{ 6,6,1, (void GUI_FLASH *)&acFont8ASCII[19]} /* char.code 51: '3' */
  ,{ 6,6,1, (void GUI_FLASH *)&acFont8ASCII[20]} /* char.code 52: '4' */
  ,{ 6,6,1, (void GUI_FLASH *)&acFont8ASCII[21]} /* char.code 53: '5' */
  ,{ 6,6,1, (void GUI_FLASH *)&acFont8ASCII[22]} /* char.code 54: '6' */
  ,{ 6,6,1, (void GUI_FLASH *)&acFont8ASCII[23]} /* char.code 55: '7' */
  ,{ 6,6,1, (void GUI_FLASH *)&acFont8ASCII[24]} /* char.code 56: '8' */
  ,{ 6,6,1, (void GUI_FLASH *)&acFont8ASCII[25]} /* char.code 57: '9' */
  ,{ 3,3,1, (void GUI_FLASH *)&acFont8ASCII[26]} /* char.code 58: ':' */
  ,{ 3,3,1, (void GUI_FLASH *)&acFont8ASCII[27]} /* char.code 59: ';' */
  ,{ 5,5,1, (void GUI_FLASH *)&acFont8ASCII[28]} /* char.code 60: '<' */
  ,{ 6,6,1, (void GUI_FLASH *)&acFont8ASCII[29]} /* char.code 61: '=' */
  ,{ 5,5,1, (void GUI_FLASH *)&acFont8ASCII[30]} /* char.code 62: '>' */
  ,{ 6,6,1, (void GUI_FLASH *)&acFont8ASCII[31]} /* char.code 63: '?' */
  ,{ 6,6,1, (void GUI_FLASH *)&acFont8ASCII[32]} /* char.code 64: '@' */
  ,{ 6,6,1, (void GUI_FLASH *)&acFont8ASCII[33]} /* char.code 65: 'A' */
  ,{ 6,6,1, (void GUI_FLASH *)&acFont8ASCII[34]} /* char.code 66: 'B' */
  ,{ 6,6,1, (void GUI_FLASH *)&acFont8ASCII[35]} /* char.code 67: 'C' */
  ,{ 6,6,1, (void GUI_FLASH *)&acFont8ASCII[36]} /* char.code 68: 'D' */
  ,{ 6,6,1, (void GUI_FLASH *)&acFont8ASCII[37]} /* char.code 69: 'E' */
  ,{ 6,6,1, (void GUI_FLASH *)&acFont8ASCII[38]} /* char.code 70: 'F' */
  ,{ 6,6,1, (void GUI_FLASH *)&acFont8ASCII[39]} /* char.code 71: 'G' */
  ,{ 6,6,1, (void GUI_FLASH *)&acFont8ASCII[40]} /* char.code 72: 'H' */
  ,{ 4,4,1, (void GUI_FLASH *)&acFont8ASCII[41]} /* char.code 73: 'I' */
  ,{ 6,6,1, (void GUI_FLASH *)&acFont8ASCII[42]} /* char.code 74: 'J' */
  ,{ 6,6,1, (void GUI_FLASH *)&acFont8ASCII[43]} /* char.code 75: 'K' */
  ,{ 6,6,1, (void GUI_FLASH *)&acFont8ASCII[44]} /* char.code 76: 'L' */
  ,{ 6,6,1, (void GUI_FLASH *)&acFont8ASCII[45]} /* char.code 77: 'M' */
  ,{ 6,6,1, (void GUI_FLASH *)&acFont8ASCII[46]} /* char.code 78: 'N' */
  ,{ 6,6,1, (void GUI_FLASH *)&acFont8ASCII[47]} /* char.code 79: 'O' */
  ,{ 6,6,1, (void GUI_FLASH *)&acFont8ASCII[48]} /* char.code 80: 'P' */
  ,{ 6,6,1, (void GUI_FLASH *)&acFont8ASCII[49]} /* char.code 81: 'Q' */
  ,{ 6,6,1, (void GUI_FLASH *)&acFont8ASCII[50]} /* char.code 82: 'R' */
  ,{ 6,6,1, (void GUI_FLASH *)&acFont8ASCII[51]} /* char.code 83: 'S' */
  ,{ 6,6,1, (void GUI_FLASH *)&acFont8ASCII[52]} /* char.code 84: 'T' */
  ,{ 6,6,1, (void GUI_FLASH *)&acFont8ASCII[53]} /* char.code 85: 'U' */
  ,{ 6,6,1, (void GUI_FLASH *)&acFont8ASCII[54]} /* char.code 86: 'V' */
  ,{ 6,6,1, (void GUI_FLASH *)&acFont8ASCII[55]} /* char.code 87: 'W' */
  ,{ 6,6,1, (void GUI_FLASH *)&acFont8ASCII[56]} /* char.code 88: 'X' */
  ,{ 6,6,1, (void GUI_FLASH *)&acFont8ASCII[57]} /* char.code 89: 'Y' */
  ,{ 6,6,1, (void GUI_FLASH *)&acFont8ASCII[58]} /* char.code 90: 'Z' */
  ,{ 4,4,1, (void GUI_FLASH *)&acFont8ASCII[59]} /* char.code 91: '[' */
  ,{ 6,6,1, (void GUI_FLASH *)&acFont8ASCII[60]} /* char.code 92: '\' */
  ,{ 4,4,1, (void GUI_FLASH *)&acFont8ASCII[61]} /* char.code 93: ']' */
  ,{ 6,6,1, (void GUI_FLASH *)&acFont8ASCII[62]} /* char.code 94: '^' */
  ,{ 6,6,1, (void GUI_FLASH *)&acFont8ASCII[63]} /* char.code 95: '_' */
  ,{ 6,6,1, (void GUI_FLASH *)&acFont8ASCII[64]} /* char.code 96: '`' */
  ,{ 6,6,1, (void GUI_FLASH *)&acFont8ASCII[65]} /* char.code 97: 'a' */
  ,{ 6,6,1, (void GUI_FLASH *)&acFont8ASCII[66]} /* char.code 98: 'b' */
  ,{ 5,5,1, (void GUI_FLASH *)&acFont8ASCII[67]} /* char.code 99: 'c' */
  ,{ 6,6,1, (void GUI_FLASH *)&acFont8ASCII[68]} /* char.code 100: 'd' */
  ,{ 6,6,1, (void GUI_FLASH *)&acFont8ASCII[69]} /* char.code 101: 'e' */
  ,{ 5,5,1, (void GUI_FLASH *)&acFont8ASCII[70]} /* char.code 102: 'f' */
  ,{ 6,6,1, (void GUI_FLASH *)&acFont8ASCII[71]} /* char.code 103: 'g' */
  ,{ 6,6,1, (void GUI_FLASH *)&acFont8ASCII[72]} /* char.code 104: 'h' */
  ,{ 3,3,1, (void GUI_FLASH *)&acFont8ASCII[73]} /* char.code 105: 'i' */
  ,{ 4,4,1, (void GUI_FLASH *)&acFont8ASCII[74]} /* char.code 106: 'j' */
  ,{ 6,6,1, (void GUI_FLASH *)&acFont8ASCII[75]} /* char.code 107: 'k' */
  ,{ 4,4,1, (void GUI_FLASH *)&acFont8ASCII[76]} /* char.code 108: 'l' */
  ,{ 6,6,1, (void GUI_FLASH *)&acFont8ASCII[77]} /* char.code 109: 'm' */
  ,{ 5,5,1, (void GUI_FLASH *)&acFont8ASCII[78]} /* char.code 110: 'n' */
  ,{ 6,6,1, (void GUI_FLASH *)&acFont8ASCII[79]} /* char.code 111: 'o' */
  ,{ 5,5,1, (void GUI_FLASH *)&acFont8ASCII[80]} /* char.code 112: 'p' */
  ,{ 5,5,1, (void GUI_FLASH *)&acFont8ASCII[81]} /* char.code 113: 'q' */
  ,{ 4,4,1, (void GUI_FLASH *)&acFont8ASCII[82]} /* char.code 114: 'r' */
  ,{ 5,5,1, (void GUI_FLASH *)&acFont8ASCII[83]} /* char.code 115: 's' */
  ,{ 4,4,1, (void GUI_FLASH *)&acFont8ASCII[84]} /* char.code 116: 't' */
  ,{ 5,5,1, (void GUI_FLASH *)&acFont8ASCII[85]} /* char.code 117: 'u' */
  ,{ 6,6,1, (void GUI_FLASH *)&acFont8ASCII[86]} /* char.code 118: 'v' */
  ,{ 6,6,1, (void GUI_FLASH *)&acFont8ASCII[87]} /* char.code 119: 'w' */
  ,{ 6,6,1, (void GUI_FLASH *)&acFont8ASCII[88]} /* char.code 120: 'x' */
  ,{ 5,5,1, (void GUI_FLASH *)&acFont8ASCII[89]} /* char.code 121: 'y' */
  ,{ 6,6,1, (void GUI_FLASH *)&acFont8ASCII[90]} /* char.code 122: 'z' */
  ,{ 4,4,1, (void GUI_FLASH *)&acFont8ASCII[91]} /* char.code 123: '{' */
  ,{ 3,3,1, (void GUI_FLASH *)&acFont8ASCII[92]} /* char.code 124: '|' */
  ,{ 4,4,1, (void GUI_FLASH *)&acFont8ASCII[93]} /* char.code 125: '}' */
  ,{ 6,6,1, (void GUI_FLASH *)&acFont8ASCII[94]} /* char.code 126: '~' */
};


GUI_FLASH const GUI_FONT_PROP GUI_Font8ASCII_Prop = {
   32             /* first character               */
  ,126            /* last character                */
  ,&GUI_Font8_ASCII_CharInfo[0] /* address of first character    */
  ,(void GUI_FLASH *)0      /* pointer to next GUI_FONTRANGE */
};

GUI_FLASH const GUI_FONT GUI_Font8_ASCII = {
   GUI_FONTTYPE_PROP /* type of font    */
  ,8                /* height of font  */
  ,8                /* space of font y */
  ,1                /* magnification x */
  ,1                /* magnification y */
  ,(void GUI_FLASH *)&GUI_Font8ASCII_Prop
};