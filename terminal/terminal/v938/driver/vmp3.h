/**********************************************************************************/
/*  Name:      vmp3.h                                                             */
/*  Function:  provide mp3, eq, led and motor control functions                   */
/*  Author:    zhangjunqi                                                         */
/*  Created date:  2005/06/14                                                     */
/**********************************************************************************/



#ifndef _RDK_VMP3_H_
#define _RDK_VMP3_H_

#ifdef __cplusplus
extern "C" {
#endif

enum MP3EQTYPEenum
{
	MP3_EQ_ORIGINAL,
	MP3_EQ_CLASSICAL,
	MP3_EQ_CLUB,
	MP3_EQ_POP,
	MP3_EQ_BASE,
	MP3_EQ_TREBLE,
	MP3_EQ_BASE_TREBLE,
	MP3_EQ_ROCK,
	MP3_EQ_DANCE
};


void V9_Vmp3SetMp3Eq(UINT8 mp3EqType);


#ifdef __cplusplus
}
#endif

#endif  //  _RDK_VMP3_H_ 


