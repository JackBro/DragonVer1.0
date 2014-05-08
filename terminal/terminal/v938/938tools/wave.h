/**********************************************************************************/
/*  Name:      wave.h                                                             */
/*  Function:  wave file parse                                                    */
/*  Author:    zhangjunqi                                                         */
/*  Created date:  2005/06/08                                                     */
/**********************************************************************************/


#ifndef _TOOLS938_WAVE_H_
#define _TOOLS938_WAVE_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "../../v568/tools/type.h"


/* Wave file structure */
typedef struct tag_TWaveFile
{
    UINT8     RIFFID[4];       /* "RIFF" */
	UINT32    fileLen;         /* File length, not including the ID and size */
    UINT8     WAVEID[4];       /* "WAVE" */
	UINT8     fmtID[4];        /* "fmt" */
	UINT32    resFut;          /* Reserved for future */
    UINT16    fmtTag;          /* Wave codec format, PCM or ADPCM */
	UINT16    chanNum;         /* Channel number */
	UINT16    samRate;         /* Sample rate, samples per second */
	UINT32    bytesPerSec;     /* Data number, bytes per second */
	UINT16    blkAlgn;         /* Block align */
	UINT16    bitsPerSam;      /* bits per sample */
	UINT8     dataID;          /* "data" */
	UINT32    datLen;          /* Sound data length */
	UINT8     *pDat;           /* Sound data pointer */
}TWaveFile, *PWaveFile;

/* Wave chunk structure */
typedef struct tag_TChunk
{
    UINT32     id;       /* Chunk ID */
	UINT32     len;      /* Chunk length, not including the ID and length */
    UINT8      *pDat;    /* Chunk data pointer */
}TChunk, *PChunk;


#ifdef __cplusplus
}
#endif

#endif