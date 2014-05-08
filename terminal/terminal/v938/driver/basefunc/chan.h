/**********************************************************************************/
/*  Name:      chan.h                                                             */
/*  Function:  channel setting                                                    */
/*  Author:    zhangjunqi                                                         */
/*  Created date:  2005/06/13                                                     */
/**********************************************************************************/


#ifndef _RDK_CHAN_H_
#define _RDK_CHAN_H_

#ifdef __cplusplus
extern "C" {
#endif
 
    //Play mode 
	enum CHANOPMODEVenum
	{
		OPMODE_HOST_WAVE_MIDI  	    		    = 0x00,
		OPMODE_HOST_WAVE_SD_MIDI	    		= 0x20,
		OPMODE_SD_WAVE_SD_MIDI  	    		= 0x30,
		OPMODE_HOST_MP3_MIDI 	    		    = 0x40,
		OPMODE_HOST_MP3_SD_MIDI 	    		= 0x60,
		OPMODE_SD_MP3_SD_MIDI 	    		    = 0x70,
		OPMODE_SD_PLAYER 	    		        = 0xf0
	};


void V9B_ChanSetChnStart(UINT8 *pVal);
void V9B_ChanGetChnStart(UINT8 *pVal);
void V9B_ChanSetChnStop(UINT8 *pVal);
void V9B_ChanGetChnStop(UINT8 *pVal);

void V9B_ChanGetChnDly(UINT8 *pVal);

void V9B_ChanSetDlyEnd(UINT8 *pVal);

void V9B_ChanSetOpMod(UINT8 val);
UINT8 V9B_ChanGetOpMod(void);


#ifdef __cplusplus
}
#endif

#endif