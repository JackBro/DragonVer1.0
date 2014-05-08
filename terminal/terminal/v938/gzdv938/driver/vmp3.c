/*------------------------------------------------------------------------

				COPYRIGHT (C) 2004, Vimicro Corporation
                        	ALL RIGHTS RESERVED

	This source code has been made available to you by VIMICRO on an 
	AS-IS basis. Anyone receiving this source code is licensed under
	VIMICRO copyrights to use it in any way he or she deems fit, including 
	copying it,  modifying it, compiling it, and redistributing it either
	with or without  modifications. Any person who transfers this source 
	code or any derivative  work must include the VIMICRO copyright notice 
	and	this paragraph in the transferred software.

	Version		Description        	Date         	Author
	1.0         Add MP3 decoder function              2004.8.16        Andy,liu
	1.1         Add Mp3 seek function                 2004.11.2        Andy,liu
				Add Mp3 Loop A to B                   2004.11.2        Andy,liu
				Change MP3 seek CBR                   2004.11.2        Andy,liu
	            Change MP3 VBR check head             2004.11.3        Andy,liu
	1.2         Change Mp3 coding style               2004.11.4        Andy,liu

	1.3												  2004.12.09       gzd
    		    Remove the obsolescent functions VMp3_GetVoice()
    		    and VMp3_GetEmStream().
	1.4         Add const for g_Gnere,g_Bitrate and g_Frequency        
													  2004.12.13       Andy,liu
				Change File stream seek bug           2004.12.13       Andy,liu    
	1.5			Change MP3 file stream bug            2004.12.28       Andy,liu
	1.6         Support NAND_FLASH                    2005.01.25       Andy,liu
	1.7         Support MP3_FASH_PLAY pause and resume2005.02.01       Andy,liu
	1.8			Change MP3 frame error bug            2005.02.17       Andy,liu
	1.9			Change Mp3 title bug                  2005.02.28	   Andy,liu
	2.0         Change Mp3 MP3_FASH_PLAY repeat bug   2005.04.11       Andy,liu

Note 1: Because reading file from NAND FLASH is very slow, so---
	  If you want play fast when using NAND FLASH,please set MP3_FASH_PLAY = 1 in vmp3.h,but total time 
		is not right sometimes.
      If you use NAND FLASH and your file system has no cache,please set MP3_NAND_FLASH_SUPPORT=1 in vmp3.h.  
      The MP3_NAND_BUFFER_SIZE has to less than VDEV_MAX_STREAM_FIFO_LEN when using NAND FLASH in vmp3.h.
Note 2: About ID3V2 flag---
	  If you want getting more information from ID3V2, please change HandleMP3ID3v2Frame function. 
	  If you want reading ID3V2 document,please get from http://www.id3.org/id3guide.htm.

	2.0			reset mp3 global variables in filestream mode. 2005.03.21	gzd
	
-------------------------------------------------------------------------*/

//#define DEBUG_PRINT_MP3 1
#ifdef DEBUG_PRINT_MP3
#include "stdio.h"
#endif
#include "vmd.h"
#include "Vmp3.h"
//Support NAND_FLASH
#if MP3_NAND_FLASH_SUPPORT
#include "vdev.h"
#endif
extern VOID ReadBuffer(UINT8* buffer, UINT32 buffersize, UINT32* realsize, UINT32 dOffset);

//Global variable
MP3_HEADER _FAR g_Mp3Header;
MP3_ID3TAG _FAR g_Mp3Id3V1TAG;
MP3_INFO _FAR g_Mp3Info;
MP3_ID3V2HEADER _FAR g_MP3ID3V2Header;
MP3_ID3V2FRAMEHEADER _FAR g_MP3ID3V2FrameHeader;
MP3_VBRINFO _FAR g_MP3VbrInfo;
MP3_LOOP _FAR g_Mp3LoopInfo;

#if MP3_NAND_FLASH_SUPPORT
MP3_NAND g_Mp3Nand;
#endif

UINT32 _FAR g_Bitrate[2][3][16] = {
   { {0,32,64,96,128,160,192,224,256,288,320,352,384,416,448,},
     {0,32,48,56, 64, 80, 96,112,128,160,192,224,256,320,384,},
     {0,32,40,48, 56, 64, 80, 96,112,128,160,192,224,256,320,} },

   { {0,32,48,56,64,80,96,112,128,144,160,176,192,224,256,},
     {0,8,16,24,32,40,48,56,64,80,96,112,128,144,160,},
     {0,8,16,24,32,40,48,56,64,80,96,112,128,144,160,} }
};



UINT32 _FAR g_Frequency[9] = {44100,48000,32000,22050,24000,16000,11025,12000,8000 };

UINT8 _FAR g_Genre[148][23] = {
    "Blues",
    "Classic Rock",
    "Country",
    "Dance",
    "Disco",
    "Funk",
    "Grunge",
    "Hip-Hop",
    "Jazz",
    "Metal",
    "New Age",
    "Oldies",
    "Other",
    "Pop",
    "R&B",
    "Rap",
    "Reggae",
    "Rock",
    "Techno",
    "Industrial",
    "Alternative",
    "Ska",
    "Death Metal",
    "Pranks",
    "Soundtrack",
    "Euro-Techno",
    "Ambient",
    "Trip-Hop",
    "Vocal",
    "Jazz+Funk",
    "Fusion",
    "Trance",
    "Classical",
    "Instrumental",
    "Acid",
    "House",
    "Game",
    "Sound Clip",
    "Gospel",
    "Noise",
    "Alternative Rock",
    "Bass",
    "Soul",
    "Punk",
    "Space",
    "Meditative",
    "Instrumental Pop",
    "Instrumental Rock",
    "Ethnic",
    "Gothic",
    "Darkwave",
    "Techno-Industrial",
    "Electronic",
    "Pop-Folk",
    "Eurodance",
    "Dream",
    "Southern Rock",
    "Comedy",
    "Cult",
    "Gangsta",
    "Top 40",
    "Christian Rap",
    "Pop/Funk",
    "Jungle",
    "Native American",
    "Cabaret",
    "New Wave",
    "Psychadelic",
    "Rave",
    "Showtunes",
    "Trailer",
    "Lo-Fi",
    "Tribal",
    "Acid Punk",
    "Acid Jazz",
    "Polka",
    "Retro",
    "Musical",
    "Rock & Roll",
    "Hard Rock",
    "Folk",
    "Folk-Rock",
    "National Folk",
    "Swing",
    "Fast Fusion",
    "Bebob",
    "Latin",
    "Revival",
    "Celtic",
    "Bluegrass",
    "Avantgarde",
    "Gothic Rock",
    "Progressive Rock",
    "Psychedelic Rock",
    "Symphonic Rock",
    "Slow Rock",
    "Big Band",
    "Chorus",
    "Easy Listening",
    "Acoustic",
    "Humour",
    "Speech",
    "Chanson",
    "Opera",
    "Chamber Music",
    "Sonata",
    "Symphony",
    "Booty Bass",
    "Primus",
    "Porn Groove",
    "Satire",
    "Slow Jam",
    "Club",
    "Tango",
    "Samba",
    "Folklore",
    "Ballad",
    "Power Ballad",
    "Rhythmic Soul",
    "Freestyle",
    "Duet",
    "Punk Rock",
    "Drum Solo",
    "Acapella",
    "Euro-House",
    "Dance Hall",
    "Goa", 
    "Drum & Bass", 
    "Club-House", 
    "Hardcore", 
    "Terror", 
    "Indie", 
    "BritPop", 
    "Negerpunk", 
    "Polsk Punk", 
    "Beat", 
    "Christian Gangsta Rap", 
    "Heavy Metal", 
    "Black Metal", 
    "Crossover", 
    "Contemporary Christian", 
    "Christian Rock",
    "Merengue", 
    "Salsa", 
    "Thrash Metal", 
    "Anime", 
    "Jpop", 
    "Synthpop" 
};

UINT8 _FAR g_GenreUnkonwn[8] = "unknown";
/***************************************************************
Description:
		Get four Bytes From Buffer.
		
Parameters:
		pbBuffer:	pointer to Buffer.
		
Returns:
		Got Value,prevent the big endian and little endian.
****************************************************************/
UINT32 Get4bytesFromBuffer(HUGE UINT8* pbBuffer)
{
	UINT32 dFourByte;
	dFourByte = ((UINT32)(*pbBuffer)<<24) | ((UINT32)*(pbBuffer+1)<<16) |
          ((UINT32)*(pbBuffer+2)<<8) | (UINT32)*(pbBuffer+3);
	return dFourByte;
}

/***************************************************************
Description:
		Get four Bytes From Buffer.
		
Parameters:
		pbBuffer:	pointer to Buffer.
		dLen:		the bytes length.
Returns:
		Got Value,prevent the big endian and little endian.
****************************************************************/
UINT32 GetLenBytesFromBuffer(HUGE UINT8* pbBuffer, UINT32 dLen)
{
	UINT32 dIndex, dTemp, dLenBytes= 0,dPos = 0;

	if(pbBuffer)
	{
		for(dIndex=0; dIndex<dLen ;dIndex++)
		{ 
			dTemp = dLen-1-dIndex  ;                                                            
			dLenBytes = dLenBytes | (UINT32)(pbBuffer[dPos +dIndex] & 0xff ) << ( 8*dTemp );
		}
		dPos += dLen ;
		return dLenBytes;
	}
	else
	{
		return 0;
	}
}

/***************************************************************
Description:
		Get two Bytes From Buffer.
		
Parameters:
		pbBuffer:	pointer to Buffer.
		
Returns:
		Got Value,prevent the big endian and little endian.		
****************************************************************/
UINT16 Get2bytesFromBuffer(HUGE UINT8* pbBuffer)
{
    	UINT16 wTemp;
    	wTemp = ((UINT16)(*pbBuffer)<<8) | ((UINT16)*(pbBuffer+1));
    	return wTemp;
}

/***************************************************************
Description:
		upper the char.
		
Parameters:
		bChar:	the upper or lower char.
		
Returns:
		the upper char.		
****************************************************************/
UINT8 Vmd_MemUpper(const UINT8 bChar)
{
	if((bChar<=122) && (bChar>=97))
	{
		return (bChar-32);
	}
	return bChar;
}

#if MP3_NAND_FLASH_SUPPORT
/***************************************************************
Description:
		Read buffer from NAND flash

Parameters:
		bBuffer	   : buffer address
		dBufferSize : number bytes to be read
		pdRealSize  : number bytes read in fact
		dOffset	   : file offset

Returns:
		  NULL
****************************************************************/
extern UINT8	g_bStmBuf[VDEV_MAX_STREAM_FIFO_LEN];
VOID ReadNANDBuffer(UINT8* buffer, UINT32 buffersize, UINT32* realsize, UINT32 dOffset)
{
	if(g_Mp3Nand.dEnd<(dOffset+buffersize))
	{
		if((g_Mp3Info.dTotalSize-dOffset)<MP3_NAND_BUFFER_SIZE)
		{
			ReadBuffer(g_bStmBuf,buffersize,realsize,dOffset);	
		}
		else
			ReadBuffer(g_bStmBuf,MP3_NAND_BUFFER_SIZE,realsize,dOffset);
		g_Mp3Nand.dStart = dOffset;
		g_Mp3Nand.dEnd = dOffset + (*realsize);
	}
	if(buffersize<=(g_Mp3Nand.dEnd-dOffset))
	{
		Vmd_MemCpy(buffer,g_bStmBuf+(dOffset-g_Mp3Nand.dStart),buffersize);
		*realsize = buffersize;
	}
	else
	{
		Vmd_MemCpy(buffer,g_bStmBuf+(dOffset-g_Mp3Nand.dStart),(g_Mp3Nand.dEnd-dOffset));
		*realsize = (g_Mp3Nand.dEnd-dOffset);		
	}

}
#endif

/***************************************************************
Description:
		Init MP3 decode struct and variable.
		
Parameters:
		void.
Returns:
		VRES_SUCCESS OR VRES_ERROR.
****************************************************************/
VRESULT	VMp3_Init(VOID)
{
	//Initial Mp3 header
	g_Mp3Header.bSync = 0;
	g_Mp3Header.bID = 0;
	g_Mp3Header.bOption = 0;
	g_Mp3Header.bProtection = 0;
	g_Mp3Header.bPrivateBit = 0;
	g_Mp3Header.bBitrateIndex = 0;
	g_Mp3Header.bChannelMode = 0;
	g_Mp3Header.bCopyright = 0;
	g_Mp3Header.bModeExtension = 0;
	g_Mp3Header.bOriginal = 0;
	g_Mp3Header.bEmphasis = 0;
	g_Mp3Header.bPad = 0;
	g_Mp3Header.bSampleRateIndex  = 0;
	g_Mp3Header.bMpeg_25 = 0;

	//Init Mp3 info 
	g_Mp3Info.dHeader = 0;

	g_Mp3Info.dBitrate = 0;
	g_Mp3Info.dStartPos = 0;
	g_Mp3Info.dCurrentTime = 0;
	g_Mp3Info.dSampleRate = 0;
	g_Mp3Info.dTotalSize = 0;
	g_Mp3Info.dTotalTime = 0;
	g_Mp3Info.dFrameSize = 0;
	g_Mp3Info.bStereo = 0;
	g_Mp3Info.dReadOffset = 0;
	g_Mp3Info.pMp3Data = NULL;
	g_Mp3Info.bXingFlag = 0xFF;
	g_Mp3Info.bVbrFlag = 0;
	g_Mp3Info.bMillionSec = 0;
	g_Mp3Info.bID3V1Tag = 0;
	g_Mp3Info.bID3V1Length = 0;
	g_Mp3Info.bFinish = 0;
	g_Mp3Info.dBufferOffset = 0;
	g_Mp3Info.dFileOffset = 0;
	g_Mp3Info.bStreamFlag = 0;
	g_Mp3Info.dID3V2Length = 0;
	g_Mp3Info.dSeekTime = 0;
	g_Mp3Info.dFrameNum = 0;

	//MP3 ID3V1 info
	g_Mp3Id3V1TAG.bVersion = 0;
	g_Mp3Id3V1TAG.bMp3Genre = NULL;
	
	//MP3 ID3V2 header info
	g_MP3ID3V2Header.dSize = 0;
	g_MP3ID3V2Header.bFlag = 0;
	g_MP3ID3V2Header.bRevisionVersion = 0;
	g_MP3ID3V2Header.bMajorVerison = 0;

	//Mp3 VBR information
	g_MP3VbrInfo.dVbr_StreamBytes = 0;
	g_MP3VbrInfo.dVbr_StreamFrames = 0;
	g_MP3VbrInfo.wVbr_Delay = 0;
	g_MP3VbrInfo.wVbr_EntryBytes = 0;
	g_MP3VbrInfo.wVbr_EntryFrames = 0;
	g_MP3VbrInfo.wVbr_Quality = 0;
	g_MP3VbrInfo.wVbr_TableLength = 0;
	g_MP3VbrInfo.wVbr_TableScale = 0;
	g_MP3VbrInfo.wVbr_TableSize = 0;
	g_MP3VbrInfo.wVbr_Version = 0;
	g_MP3VbrInfo.bTOCTag = 0;

	//Mp3 Loop Information
	g_Mp3LoopInfo.bLoopFlag = 0;
	g_Mp3LoopInfo.dLoopPositionA = 0;
	g_Mp3LoopInfo.dLoopPositionB = 0;
	g_Mp3LoopInfo.dLoopTimeA = 0;
	g_Mp3LoopInfo.dLoopTimeB = 0;
	g_Mp3LoopInfo.bHasSeek = 0;
	
	//Init tag
	Vmd_MemSet(g_Mp3Id3V1TAG.ID3.V1_0.bTitle,0,30);
	Vmd_MemSet(g_Mp3Id3V1TAG.ID3.V1_0.bArtist,0,30);
	Vmd_MemSet(g_Mp3Id3V1TAG.ID3.V1_0.bAlbum,0,30);
	Vmd_MemSet(g_Mp3Id3V1TAG.ID3.V1_0.bYear,0,4);
	Vmd_MemSet(g_Mp3Id3V1TAG.ID3.V1_0.bComment,0,30);

	//MP3 NAND FLASH Cache File
#if MP3_NAND_FLASH_SUPPORT
	g_Mp3Nand.dStart = 0;
	g_Mp3Nand.dEnd = 0;

#endif

	return VRES_SUCCESS;
}


/***************************************************************
Description:
		Check the mp3 header.
		
Parameters:
		pbFile:    the pointer of buffer.
Returns:
		VHEADER type.
****************************************************************/
VHEADER CheckHeadTYPE(HUGE UINT8* pbFile)
{
	UINT8 bSync = 0xFF;
	UINT8 bID[4] = "ID3";
	if(!Vmd_MemCmp((HUGE UINT8*)pbFile,(HUGE UINT8*)bID,3))
	{
		return VHEADER_ID3V2;
	}
	else if (!Vmd_MemCmp((HUGE UINT8*)pbFile,(HUGE UINT8*)(&bSync),1)) 
	{
		return VHEADER_HEADER;
	}
	return VHEADER_ERROR;
}

/***************************************************************
Description:
		Check the mp3 sync byte.
		
Parameters:
		dHead:	the four bytes of header.
Returns:
		VRES_SUCCESS OR VRES_ERROR.
****************************************************************/
VRESULT Sync_check(UINT32 dHead)
{
   	if((dHead & 0xffe00000) != 0xffe00000)
		return VRES_ERROR;
	if(((dHead>>19)&3)==0x01)
		return VRES_ERROR;
    if(((dHead>>17)&3)!=0x01)
		return VRES_ERROR;
	if( ((dHead>>12)&0xf) == 0xf)
		return VRES_ERROR;
	if( ((dHead>>10)&0x3) == 0x3 )
		return VRES_ERROR;
	return VRES_SUCCESS;
}

/***************************************************************
Description:
		Seek MP3 song next frame.
		
Parameters:
		pbFile:     the pointer of buffer.
		pbOffset:   the last Frame size.
		dMaxOffset: the maximum header offset.
		dLen:       the file length.
Returns:
		VRES_SUCCESS OR VRES_ERROR.
****************************************************************/
UINT32 GetFirstSync(HUGE UINT8* pbFile,UINT32 *pbOffset,UINT32 dMaxOffset,SINT32 sdLen)
{
	UINT32 dHeader=0,dPos = 0;
	UINT8  bTemp;
	UINT32 dReadLen=3,dRealLen;
	HUGE UINT8* pbFilePtr= pbFile;
	if(g_Mp3Info.bStreamFlag)
	{
		pbFilePtr  = (HUGE UINT8*)(g_Mp3Info.bBuffer);
#if MP3_NAND_FLASH_SUPPORT
		ReadNANDBuffer(g_Mp3Info.bBuffer,dReadLen,&dRealLen,g_Mp3Info.dFileOffset+dPos);
#else
		ReadBuffer(g_Mp3Info.bBuffer,dReadLen,&dRealLen,g_Mp3Info.dFileOffset+dPos);
#endif
		if(dRealLen< dReadLen)
			return 0;
	}
//	if(dMaxOffset>=g_Mp3Info.dTotalSize)
//		dMaxOffset = g_Mp3Info.dTotalSize-g_Mp3Info.bID3V1Length-g_Mp3Info.dID3V2Length;
	if(pbFilePtr == NULL)
		return 0;

	//If have garbage bytes,then rewind header
	dHeader = GetLenBytesFromBuffer(pbFilePtr,3);

	dPos = dPos +3;

	do {
		sdLen--;
		if(sdLen<=0)
			return 0;
		dHeader <<= 8;
		
		if(g_Mp3Info.bStreamFlag)
		{
			dReadLen = 1;
#if MP3_NAND_FLASH_SUPPORT
			ReadNANDBuffer(g_Mp3Info.bBuffer,dReadLen,&dRealLen,g_Mp3Info.dFileOffset + dPos);
#else
			ReadBuffer(g_Mp3Info.bBuffer,dReadLen,&dRealLen,g_Mp3Info.dFileOffset + dPos);
#endif
			if(dRealLen<dReadLen)
				return 0;
			bTemp= g_Mp3Info.bBuffer[0];
		}
		else
		{
			if((pbFilePtr+dPos) == NULL)
				return 0;
			bTemp= *(pbFilePtr+dPos);
		}
		
		
		dHeader |= bTemp;
		
		dPos ++;
		if(dMaxOffset > 0 && dPos > dMaxOffset)
			return 0;
	} while((Sync_check(dHeader)==VRES_ERROR));
	
	//return the offset
	*pbOffset = dPos - 4 ;
	
	return dHeader;
}

/***************************************************************
Description:
		Check the mp3 header.
		
Parameters:
		dHeader:    the four bytes of header.
Returns:
		VRES_SUCCESS OR VRES_ERROR.
****************************************************************/
VRESULT CheckHead(UINT32 dHeader)
{
	UINT32 dSize = 0;
	if(Sync_check(dHeader) == VRES_ERROR)
	{
      		return VRES_ERROR;
	}

	if( dHeader & (((UINT32)1)<<20) ) 
	{
		g_Mp3Header.bID = (dHeader & (((UINT32)1)<<19)) ? 0x0 : 0x1;
		g_Mp3Header.bMpeg_25 = 0;
	}
	else 
	{
		g_Mp3Header.bID = 1;
		g_Mp3Header.bMpeg_25 = 1;
	}
    
	g_Mp3Header.bOption = (UINT8) ((dHeader>>17)&3);
	if( ((dHeader>>10)&0x3) == 0x3) 
	{
		return VRES_ERROR;
	}
	if(g_Mp3Header.bMpeg_25 == 1) 
	{
		g_Mp3Header.bSampleRateIndex = 6 + (UINT8)((dHeader>>10)&0x3);
	}
	else
	{
		g_Mp3Header.bSampleRateIndex = (UINT8) ((dHeader>>10)&0x3) + (g_Mp3Header.bID*3);
	}
	g_Mp3Header.bProtection = (UINT8)(((dHeader>>16)&0x1)^0x1);

	g_Mp3Header.bBitrateIndex = (UINT8) ((dHeader>>12)&0xf);
	g_Mp3Header.bPad          = (UINT8) ((dHeader>>9)&0x1);
	g_Mp3Header.bPrivateBit   = (UINT8) ((dHeader>>8)&0x1);
	g_Mp3Header.bChannelMode  = (UINT8) ((dHeader>>6)&0x3);
	g_Mp3Header.bModeExtension= (UINT8) ((dHeader>>4)&0x3);
	g_Mp3Header.bCopyright    = (UINT8) ((dHeader>>3)&0x1);
	g_Mp3Header.bOriginal     = (UINT8) ((dHeader>>2)&0x1);
	g_Mp3Header.bEmphasis     = (UINT8) (dHeader& 0x3);
	
	g_Mp3Info.bStereo    = (g_Mp3Header.bChannelMode == MP3_MONO) ? 1 : 2;


	//Mp3 not support this format
	if(!g_Mp3Header.bBitrateIndex) 
	{
		return VRES_ERROR;
	}

	g_Mp3Info.dSampleRate = g_Frequency[g_Mp3Header.bSampleRateIndex];
	g_Mp3Info.dBitrate = g_Bitrate[g_Mp3Header.bID][2][g_Mp3Header.bBitrateIndex];
	if(g_Mp3Header.bOption == 1) 
	{
		if(g_Mp3Header.bID)
			dSize = (g_Mp3Info.bStereo == 1) ? 9 : 17;
		else
			dSize = (g_Mp3Info.bStereo == 1) ? 17 : 32;
		if(g_Mp3Header.bProtection)
			dSize += 2;
		g_Mp3Info.dFrameSize = (UINT32) g_Bitrate[g_Mp3Header.bID][2][g_Mp3Header.bBitrateIndex] * 144000;
		g_Mp3Info.dFrameSize = g_Mp3Info.dFrameSize /(g_Frequency[g_Mp3Header.bSampleRateIndex]<<(g_Mp3Header.bID));
		g_Mp3Info.dFrameSize = g_Mp3Info.dFrameSize + g_Mp3Header.bPad;
	}
	else
	{
		//Mp3 not support layer 1 and layer 2
		return VRES_ERROR;
	}
	
	return VRES_SUCCESS;
}

/***************************************************************
Description:
		Check the mp3 header for compute time.
		
Parameters:
		dHeader:    the four bytes of header.
Returns:
		VRES_SUCCESS OR VRES_ERROR.
****************************************************************/
VRESULT CheckHeadForTime(UINT32 dHeader)
{
	if( dHeader & (((UINT32)1)<<20) ) 
	{
		g_Mp3Header.bID = (dHeader & (((UINT32)1)<<19)) ? 0x0 : 0x1;
		g_Mp3Header.bMpeg_25 = 0;
    }
	else 
	{
		g_Mp3Header.bID = 1;
		g_Mp3Header.bMpeg_25 = 1;
	}
    

	if( ((dHeader>>10)&0x3) == 0x3) 
	{
		return VRES_ERROR;
	}
	if(g_Mp3Header.bMpeg_25 == 1) 
	{
		g_Mp3Header.bSampleRateIndex = 6 + (UINT8)((dHeader>>10)&0x3);
	}
	else
	{
		g_Mp3Header.bSampleRateIndex = (UINT8) ((dHeader>>10)&0x3) + (g_Mp3Header.bID*3);
	}

	g_Mp3Header.bBitrateIndex = (UINT8) ((dHeader>>12)&0xf);
    	
	g_Mp3Header.bPad          = (UINT8) ((dHeader>>9)&0x1);

	//Mp3 not support this format
	if(!g_Mp3Header.bBitrateIndex) 
	{
		return VRES_ERROR;
	}

	g_Mp3Info.dSampleRate = g_Frequency[g_Mp3Header.bSampleRateIndex];
	g_Mp3Info.dBitrate = g_Bitrate[g_Mp3Header.bID][2][g_Mp3Header.bBitrateIndex];
	if(g_Mp3Header.bOption == 1) 
	{
		g_Mp3Info.dFrameSize = (UINT32) g_Bitrate[g_Mp3Header.bID][2][g_Mp3Header.bBitrateIndex] * 144000;
		g_Mp3Info.dFrameSize = g_Mp3Info.dFrameSize /(g_Frequency[g_Mp3Header.bSampleRateIndex]<<(g_Mp3Header.bID));
		g_Mp3Info.dFrameSize = g_Mp3Info.dFrameSize + g_Mp3Header.bPad;
	}
	else
	{
		//Mp3 not support layer 1 and layer 2
		return VRES_ERROR;
	}
	
	return VRES_SUCCESS;
}

/***************************************************************
Description:
		Check the mp3 ID3V1 TAG.
		
Parameters:
		pbFile:    the pointer of buffer.
		dLen:      the buffer length.
Returns:
		VRES_SUCCESS OR VRES_ERROR.
****************************************************************/
VOID CheckID3V1Tag(HUGE UINT8* pbFile, UINT32 dLen)
{
	//g_Id3V1TAG
	HUGE UINT8* pbFilePtr= NULL;
	UINT8 bTag[4] = "TAG";
	UINT32 dReadLen;
	
	if(g_Mp3Info.bStreamFlag)
	{
#if MP3_NAND_FLASH_SUPPORT
		ReadNANDBuffer(g_Mp3Info.bBuffer,128,&dReadLen,dLen-128);
		g_Mp3Nand.dEnd = 0;
		g_Mp3Nand.dStart = 0;
#else
		ReadBuffer(g_Mp3Info.bBuffer,128,&dReadLen,dLen-128);
#endif
		if(dReadLen<128)
			return;
		pbFilePtr = g_Mp3Info.bBuffer;
	}
	else
	{
		pbFilePtr = pbFile+dLen-128;
	}
	
	if( !Vmd_MemCmp((HUGE UINT8*)pbFilePtr,(HUGE UINT8*)bTag,3))
	{
		g_Mp3Info.bID3V1Tag = 1;
		g_Mp3Info.bID3V1Length = 128;
		Vmd_MemCpy((HUGE UINT8*)(g_Mp3Id3V1TAG.ID3.V1_0.bTitle),(HUGE UINT8* )pbFilePtr+3,30);
		Vmd_MemCpy((HUGE UINT8* )(g_Mp3Id3V1TAG.ID3.V1_0.bArtist),(HUGE UINT8* )pbFilePtr+33,30);
		Vmd_MemCpy((HUGE UINT8* )(g_Mp3Id3V1TAG.ID3.V1_0.bAlbum),(HUGE UINT8* )pbFilePtr+63,30);
		Vmd_MemCpy((HUGE UINT8* )(g_Mp3Id3V1TAG.ID3.V1_0.bYear),(HUGE UINT8* )pbFilePtr+93,4);
		g_Mp3Id3V1TAG.ID3.V1_1.sZero = *(pbFilePtr+125);
		if(g_Mp3Id3V1TAG.ID3.V1_1.sZero)
		{
			g_Mp3Id3V1TAG.bVersion = 1;
			Vmd_MemCpy((HUGE UINT8* )(g_Mp3Id3V1TAG.ID3.V1_0.bComment),(HUGE UINT8* )pbFilePtr+97,30);
		}
		else
		{
			g_Mp3Id3V1TAG.bVersion = 2;
			Vmd_MemCpy((HUGE UINT8* )(g_Mp3Id3V1TAG.ID3.V1_1.bComment),(HUGE UINT8* )pbFilePtr+97,28);
			g_Mp3Id3V1TAG.ID3.V1_1.sTrack = *(pbFilePtr+126);
		}
		
		g_Mp3Id3V1TAG.ID3.V1_1.bGenre = *(pbFilePtr+127);
		if(g_Mp3Id3V1TAG.ID3.V1_1.bGenre > MP3_GENRE_NUM )
		{
			g_Mp3Id3V1TAG.bMp3Genre = g_GenreUnkonwn;
		}
		else
		{
			g_Mp3Id3V1TAG.bMp3Genre = (UINT8 *)g_Genre[g_Mp3Id3V1TAG.ID3.V1_1.bGenre];
		}
	}
}

/***************************************************************
Description:
		Handle the mp3 ID3V2 frame.
		
Parameters:
		pbFile:    the pointer of buffer.
		dLen:      the buffer length.
Returns:
		void.
****************************************************************/
VOID HandleMP3ID3v2Frame(HUGE UINT8* pbFile,UINT32 dLen)
{
	HUGE UINT8* pbFilePtr= pbFile + MP3_ID3V2_FRAMESIZE;
	UINT8 bTt2[4] = "TIT";
	UINT8 bTal[4] = "TAL";
	UINT8 bTpe[4] = "TPE";
	UINT8 bTye[4] = "TYE";
	UINT8 bCom[4] = "COM";

	//If MP3ID3V1TAG has exist, then not get it
	if((g_Mp3Id3V1TAG.bVersion==1) || (g_Mp3Id3V1TAG.bVersion==2))
	{
		return;
	}

	//Title
	if (Vmd_MemCmp((HUGE UINT8*)pbFile , (HUGE UINT8*)bTt2,3) == 0)
	{
		if(dLen> 30)
			dLen= 30;
		Vmd_MemCpy((HUGE UINT8* )(g_Mp3Id3V1TAG.ID3.V2.bTitle),(HUGE UINT8* )(pbFilePtr+4),dLen);		
	}

	//Album
	else if (Vmd_MemCmp((HUGE UINT8*)pbFile , (HUGE UINT8*)bTal,3) == 0)
	{
		if(dLen> 30)
			dLen= 30;
		Vmd_MemCpy((HUGE UINT8* )(g_Mp3Id3V1TAG.ID3.V2.bAlbum),(HUGE UINT8* )(pbFilePtr+4),dLen);
	}

	//Artist
   	else if (Vmd_MemCmp((HUGE UINT8*)pbFile , (HUGE UINT8*)bTpe,3) == 0)
	{
		if(dLen> 30)
			dLen= 30;
		Vmd_MemCpy((HUGE UINT8* )(g_Mp3Id3V1TAG.ID3.V2.bArtist),(HUGE UINT8* )(pbFilePtr+4),dLen);
	}


	//Year
	else if (Vmd_MemCmp((HUGE UINT8*)pbFile, (HUGE UINT8*)bTye,3) == 0)
	{
		if(dLen> 4)
			dLen= 4;
		Vmd_MemCpy((HUGE UINT8* )(g_Mp3Id3V1TAG.ID3.V2.bYear),(HUGE UINT8* )(pbFilePtr+4),dLen);
	}

	//Comment
	else if (Vmd_MemCmp((HUGE UINT8*)pbFile, (HUGE UINT8*)bCom,3) == 0)
	{
		if(dLen> 28)
			dLen= 28;
		Vmd_MemCpy((HUGE UINT8* )(g_Mp3Id3V1TAG.ID3.V2.bComment),(HUGE UINT8* )(pbFilePtr+4),dLen);
	}

}

/***************************************************************
Description:
		Check the mp3 ID3V2 TAG.
		
Parameters:
		pbFile:    the pointer of buffer.
		dLen:      the buffer length.
Returns:
		VRES_SUCCESS OR VRES_ERROR.
****************************************************************/
UINT32 CheckID3V2Tag(HUGE UINT8* pbFile, UINT32 dLen)
{
	SINT32 sdSize = 0,sdFrameSize = 0;
	HUGE UINT8* pbFilePtr= NULL;
	UINT32 dReadLen=MP3_ID3V2_HEADERSIZE-3,dRealLen=0;
	UINT32 dPos = g_Mp3Info.dFileOffset+3;

	if(g_Mp3Info.bStreamFlag)
	{
		pbFilePtr = (HUGE UINT8*)(g_Mp3Info.bBuffer);
#if MP3_NAND_FLASH_SUPPORT
		ReadNANDBuffer(g_Mp3Info.bBuffer,dReadLen,&dRealLen,dPos );
#else
		ReadBuffer(g_Mp3Info.bBuffer,dReadLen,&dRealLen,dPos );
#endif
		if(dRealLen<dReadLen)
			return dRealLen;
	}
	else
	{
		dPos = 0;
		pbFilePtr = pbFile+ 3;
	}

	g_MP3ID3V2Header.bMajorVerison = *pbFilePtr;
	g_MP3ID3V2Header.bRevisionVersion = *(pbFilePtr+1);
	g_MP3ID3V2Header.bFlag = *(pbFilePtr+2);

	//Compute Mp3 ID3V2 TAG length
	g_MP3ID3V2Header.dSize = ((UINT32)(*(pbFilePtr+3)) & 0x7F) << 21;
	g_MP3ID3V2Header.dSize+= ((UINT32)(*(pbFilePtr+4)) & 0x7F) << 14;
	g_MP3ID3V2Header.dSize+= ((UINT32)(*(pbFilePtr+5)) & 0x7F) << 7;
	g_MP3ID3V2Header.dSize+= (UINT32)(*(pbFilePtr+6)) & 0x7F;
	g_MP3ID3V2Header.dSize+= MP3_ID3V2_HEADERSIZE;
	sdSize = g_MP3ID3V2Header.dSize;
	
	if(g_Mp3Info.bStreamFlag)
	{
		dPos = dPos + 7;
	}
	else
	{
		pbFilePtr= pbFilePtr+ 7;
	}
	
	g_Mp3Id3V1TAG.bVersion = 3;


	//Write MP3 ID3V2 flag into the Struct
	for(; sdSize > 0;)
	{	
		if(g_Mp3Info.bStreamFlag)
		{
			dReadLen = 4;
			dRealLen = 0;
#if MP3_NAND_FLASH_SUPPORT
			ReadNANDBuffer(g_Mp3Info.bBuffer,dReadLen,&dRealLen,dPos +4);
#else
			ReadBuffer(g_Mp3Info.bBuffer,dReadLen,&dRealLen,dPos +4);
#endif
			if(dRealLen < dReadLen)
				return dRealLen;
			
			g_MP3ID3V2FrameHeader.dSize = Get4bytesFromBuffer((HUGE UINT8*)(g_Mp3Info.bBuffer));
		}
		else
		{
			g_MP3ID3V2FrameHeader.dSize = Get4bytesFromBuffer(pbFilePtr+4);
		}

		sdFrameSize= MP3_ID3V2_FRAMESIZE + g_MP3ID3V2FrameHeader.dSize;
		if(sdFrameSize>sdSize  || (sdFrameSize<0))
			return g_MP3ID3V2Header.dSize;
		if(g_Mp3Info.bStreamFlag)
		{
			dReadLen = sdFrameSize;
			if(sdFrameSize > MP3_BUFFER_SIZE)
			{
				dReadLen = MP3_BUFFER_SIZE;
				g_MP3ID3V2FrameHeader.dSize = MP3_BUFFER_SIZE;
			}
			dRealLen = 0;
#if MP3_NAND_FLASH_SUPPORT
			ReadNANDBuffer(g_Mp3Info.bBuffer,dReadLen,&dRealLen,dPos );
#else
			ReadBuffer(g_Mp3Info.bBuffer,dReadLen,&dRealLen,dPos );
#endif
			if(dRealLen < dReadLen)
				return dRealLen;
		
			HandleMP3ID3v2Frame((HUGE UINT8*)(g_Mp3Info.bBuffer),g_MP3ID3V2FrameHeader.dSize);
			dPos = dPos + sdFrameSize;	
		}
		else
		{
			HandleMP3ID3v2Frame(pbFilePtr,g_MP3ID3V2FrameHeader.dSize);

			pbFilePtr= pbFilePtr+ sdFrameSize;
		}
        
		sdSize = sdSize - sdFrameSize;
		
	}

	return g_MP3ID3V2Header.dSize;
}


/***************************************************************
Description:
		Write Xing header into VBR STRUCT.
		
Parameters:
		pbFile:    the pointer of buffer.
		dLen:      the buffer length.
Returns:
		VRES_SUCCESS OR VRES_ERROR.
****************************************************************/
VRESULT WriteXingHeader(HUGE UINT8* pbFile, UINT32 dLen)
{
	UINT32 dIndex,head_flags;  
	HUGE UINT8* pbFilePtr= pbFile;

	if(pbFilePtr)
		head_flags =  Get4bytesFromBuffer(pbFilePtr);
	else
		return VRES_ERROR;
	pbFilePtr = pbFilePtr+ 4;

	//If frames exist,then get it
	if( head_flags & MP3_XING_FRAMES_FLAG ) 
	{
		g_MP3VbrInfo.dVbr_StreamFrames = Get4bytesFromBuffer(pbFilePtr);	 
		pbFilePtr = pbFilePtr + 4;
	}

	//if byte exist,then get it
	if( head_flags & MP3_XING_BYTES_FLAG )  
	{
		g_MP3VbrInfo.dVbr_StreamBytes = Get4bytesFromBuffer(pbFilePtr);	 
		pbFilePtr = pbFilePtr + 4;
	}
    
	//If TOC exist,then put it into TOC table	
	if( head_flags & MP3_XING_TOC_FLAG ) 
	{
		g_MP3VbrInfo.bTOCTag = 1;
		for(dIndex=0;dIndex<100;dIndex++) 
		{
			g_MP3VbrInfo.VBR_TOC.Xing[dIndex] = pbFilePtr[dIndex];
		}
       		pbFilePtr = pbFilePtr + 4;
    	}
    
	//If XING SCALE exist,then get it
	if( head_flags & MP3_XING_SCALE_FLAG )  
	{
		g_MP3VbrInfo.wVbr_TableScale = Get4bytesFromBuffer(pbFilePtr);	
	}
	
	return VRES_SUCCESS;
}


/***************************************************************
Description:
		Write Fhg header into VBR STRUCT.
		
Parameters:
		pbFile:    the pointer of buffer.
		dLen:      the buffer length.
Returns:
		VRES_SUCCESS OR VRES_ERROR.
****************************************************************/
VRESULT WriteFhgHeader(HUGE UINT8* pbFile, UINT32 dLen)
{
	HUGE UINT8* pbFilePtr= pbFile;
	UINT32 dTableCount = 0;
	UINT32 dIndex = 0;

	g_MP3VbrInfo.wVbr_Version     = Get2bytesFromBuffer(pbFilePtr);
	g_MP3VbrInfo.wVbr_Delay       = Get2bytesFromBuffer(pbFilePtr+2);
	g_MP3VbrInfo.wVbr_Quality     = Get2bytesFromBuffer(pbFilePtr+4);

	g_MP3VbrInfo.dVbr_StreamBytes = Get4bytesFromBuffer(pbFilePtr+6);
	g_MP3VbrInfo.dVbr_StreamFrames= Get4bytesFromBuffer(pbFilePtr+10);

	g_MP3VbrInfo.wVbr_TableSize   = Get2bytesFromBuffer(pbFilePtr+14);
	g_MP3VbrInfo.wVbr_TableScale  = Get2bytesFromBuffer(pbFilePtr+16);
	g_MP3VbrInfo.wVbr_EntryBytes  = Get2bytesFromBuffer(pbFilePtr+18);
	g_MP3VbrInfo.wVbr_EntryFrames = Get2bytesFromBuffer(pbFilePtr+20);
	
	g_MP3VbrInfo.wVbr_TableLength = g_MP3VbrInfo.wVbr_TableSize * g_MP3VbrInfo.wVbr_EntryBytes; 

	pbFilePtr = pbFilePtr + 22;

	dTableCount = g_MP3VbrInfo.wVbr_TableSize;
	if(dTableCount >1024)
	{
		dTableCount = 1024;
	}
	g_MP3VbrInfo.bTOCTag = 1;
	//Write Fhg VBR-TOC into TOC table
	for ( dIndex  = 0 ; dIndex  <= dTableCount ; dIndex ++)
	{
		g_MP3VbrInfo.VBR_TOC.Fhg[dIndex] = GetLenBytesFromBuffer(pbFilePtr,g_MP3VbrInfo.wVbr_EntryBytes)*g_MP3VbrInfo.wVbr_TableScale;
		pbFilePtr = pbFilePtr + g_MP3VbrInfo.wVbr_EntryBytes;
	}
	
	return VRES_SUCCESS;
}

/***************************************************************
Description:
		Scan the VBR header: XING,INFO,VBRI.
		
Parameters:
		pbFile:    the pointer of buffer.
		dLen:      the buffer length.
Returns:
		VRES_SUCCESS OR VRES_ERROR.
****************************************************************/
VRESULT ScanVBR_CBR_Header(HUGE UINT8* pbFile, UINT32 dLen)
{
	HUGE UINT8* pbFilePtr= pbFile;
	UINT8 bVbri[5]="VBRI";
	UINT8 bInfo[5]="INFO";
	UINT8 bXing[5]="Xing";
	pbFilePtr= pbFilePtr+ dLen;
	if(pbFilePtr==NULL)
		return VRES_ERROR;
	//check Info for VBRI flag
	if (Vmd_MemCmp((HUGE UINT8*)pbFilePtr,(HUGE UINT8*)bVbri,4) == 0)
	{
		g_Mp3Info.bVbrFlag = 1;
		g_Mp3Info.bXingFlag = 2;

		//deal with VBRI flag
		WriteFhgHeader(pbFilePtr+4,dLen);
		return VRES_SUCCESS;
	}
    
	//Check Info for LAME Info flag
	else if (Vmd_MemCmp((HUGE UINT8*)pbFilePtr,(HUGE UINT8*)bInfo,4) == 0)
	{
		g_Mp3Info.bVbrFlag = 0;
		g_Mp3Info.bXingFlag = 3;
		return VRES_SUCCESS;
	}

	//check info for XING flag
	else if(Vmd_MemCmp((HUGE UINT8*)pbFilePtr,(HUGE UINT8*)bXing,4) == 0)
	{
		g_Mp3Info.bXingFlag = 1; 
		g_Mp3Info.bVbrFlag = 1;
		
		//deal with Xing flag
		WriteXingHeader(pbFilePtr+4,dLen);
		return VRES_SUCCESS;
	}
	return VRES_ERROR;
}


/***************************************************************
Description:
		Seek MP3 song next frame.
		
Parameters:
		pbFile:      the pointer of buffer.
		dMaxOffset:  the header offset
		dMaxOffset:  the maximum header offset
		dLastHeader: the last header
		sdLen:       the File length
Returns:
		VRES_SUCCESS OR VRES_ERROR.
****************************************************************/
UINT32 SeekNextFrame(HUGE UINT8* pbFile,UINT32 *pbOffset, UINT32 dMaxOffset,UINT32 dLastHeader,SINT32 sdLen)
{
	UINT32 dHeader=0,dPos = 0;
	UINT8 bTemp;
	HUGE UINT8* pbFilePtr= pbFile;

	UINT32 dReadLen=0,dRealLen=0;

	if(pbFilePtr == NULL)
		return 0;
	//Last Header is a template to see the same frequency, layer etc as last
	dLastHeader&= 0xfffe0c00;
    
	//If have garbage bytes,then rewind header
	dHeader = GetLenBytesFromBuffer(pbFilePtr,3);

	dPos = dPos +3;

	do {
		sdLen--;
		if(sdLen<=0)
			return 0;
        		dHeader <<= 8;
		if(g_Mp3Info.bStreamFlag)
		{
			dReadLen = 1;
#if MP3_NAND_FLASH_SUPPORT
			ReadNANDBuffer(g_Mp3Info.bBuffer,dReadLen,&dRealLen,g_Mp3Info.dFileOffset + dPos);
#else
			ReadBuffer(g_Mp3Info.bBuffer,dReadLen,&dRealLen,g_Mp3Info.dFileOffset + dPos);
#endif
			if(dRealLen<dReadLen)
				return 0;
			bTemp= g_Mp3Info.bBuffer[0];
		}
		else
		{
			if((pbFilePtr+dPos) == NULL)
				return 0;
			bTemp= *(pbFilePtr+dPos);
		}
		
		
		dHeader |= bTemp;
		dPos++;
		if(dMaxOffset> 0 && dPos > dMaxOffset)
           	return 0;
	} while((Sync_check(dHeader)==VRES_ERROR) ||
		(dLastHeader?((dHeader & 0xfffe0c00) != dLastHeader):FALSE));
	
	//return the offset
	*pbOffset= dPos - 4 ;
	
	return dHeader;
}


/***************************************************************
Description:
		Extract mp3 total time information.
		
Parameters:
		pbFile:    the pointer of buffer.
		sdLen:    the buffer length.
Returns:
		VRES_SUCCESS OR VRES_ERROR.
****************************************************************/
VRESULT ExtractMP3Info(HUGE UINT8* pbFile, SINT32 sdLen)
{
	UINT32 dSamplesPerFrames = 0;
	UINT32 dFrameSize=0,dOffset=0,dLastHeader=0,dHeader=0;
	HUGE UINT8* pbFilePtr = NULL;
	UINT32 dReadLen = g_Mp3Info.dFrameSize,dRealLen=0;
	UINT32 dPos = g_Mp3Info.dFileOffset;	

	if(g_Mp3Info.bStreamFlag)
	{
		pbFilePtr = (HUGE UINT8*)(g_Mp3Info.bBuffer);
		if(dReadLen > MP3_BUFFER_SIZE)
			dReadLen = MP3_BUFFER_SIZE;
#if MP3_NAND_FLASH_SUPPORT
		ReadNANDBuffer(g_Mp3Info.bBuffer,dReadLen,&dRealLen,dPos);
#else
		ReadBuffer(g_Mp3Info.bBuffer,dReadLen,&dRealLen,dPos);
#endif
		if(dRealLen < dReadLen)
			return VRES_ERROR;
	}
	else
	{
		dPos = 0;
		pbFilePtr = pbFile;
	}
	
	dFrameSize = g_Mp3Info.dFrameSize;
	dOffset = 0;
	dLastHeader = g_Mp3Info.dHeader;
	
	//Scan VBR header,write the VBR INFO into the VBR STRUCT
	if(ScanVBR_CBR_Header(pbFilePtr,9) ==VRES_ERROR)
	{
		if(ScanVBR_CBR_Header(pbFilePtr,17) ==VRES_ERROR)
			ScanVBR_CBR_Header(pbFilePtr,32);
	}

	//VBR for  VBRI flag
	if(g_Mp3Info.bXingFlag == 2)
	{
		if(g_Mp3Info.dSampleRate >= 32000) 
		{
			dSamplesPerFrames = 1152;
		}
		else
		{
			dSamplesPerFrames = 576;
		}
		g_Mp3Info.dTotalTime = g_MP3VbrInfo.dVbr_StreamFrames * dSamplesPerFrames / g_Mp3Info.dSampleRate * 1000;
#ifdef DEBUG_PRINT_MP3
		printf("The VBRI time is %d:%d\n",g_Mp3Info.dTotalTime/60000,(g_Mp3Info.dTotalTime/1000) % 60);
#endif
	}
	//CBR flag
	else if(g_Mp3Info.bXingFlag == 3)
	{
		g_Mp3Info.dFrameNum = g_Mp3Info.dTotalSize / g_Mp3Info.dFrameSize;
		g_Mp3Info.dTotalTime = (g_Mp3Info.dFrameSize) * 8*g_Mp3Info.dFrameNum/ (g_Mp3Info.dBitrate);
#ifdef DEBUG_PRINT_MP3
		printf("The CBR flag time is %d:%d\n",g_Mp3Info.dTotalTime/60000,(g_Mp3Info.dTotalTime/1000) % 60);
#endif
	}
	//VBR for XING FLAG
	else if((g_Mp3Info.bXingFlag == 1) && (g_MP3VbrInfo.dVbr_StreamFrames!=0))
	{
		//Calculate the xing total time
		g_Mp3Info.dTotalTime = g_MP3VbrInfo.dVbr_StreamFrames * 26;
		
#ifdef DEBUG_PRINT_MP3
		printf("The XING time is %d:%d\n",g_Mp3Info.dTotalTime/60000,(g_Mp3Info.dTotalTime/1000) % 60);
#endif
	}
	else
	{
		/*
		There are three ways of calculating the total times of a mp3:
		1) Constant BITRATE: One frame can provide the information
		needed: # of frames and duration. 
		2) Variable BITRATE: Xing tag and VBRI tag. It provides the 
		number of  frames. Each frame has the same number of samples, 
		we can use it calculate total times.
		3) The Others: Count up the frames and time of each frames
		by decoding each one. We do this if we have no other choice, 
		i.e. if it's a VBR file with no Xing and VBRI tag.
		*/
		//Scan all file to calculate it
		g_Mp3Info.dTotalTime = 0;
		g_Mp3Info.dTotalTime = g_Mp3Info.dTotalTime + (g_Mp3Info.dFrameSize) * 8 / (g_Mp3Info.dBitrate);
		sdLen = sdLen - g_Mp3Info.dID3V2Length-g_Mp3Info.bID3V1Length - dFrameSize;
#if MP3_FASH_PLAY
		g_Mp3Info.dFrameNum = MP3_MAX_SCAN_FRAME+1;
#else
		g_Mp3Info.dFrameNum = 1;
#endif
		if(g_Mp3Info.bStreamFlag)
		{
			g_Mp3Info.dFileOffset = g_Mp3Info.dFileOffset + dFrameSize-4;
			dReadLen = g_Mp3Info.dFrameSize;
			dRealLen=0;	

#if MP3_NAND_FLASH_SUPPORT				
			ReadNANDBuffer(g_Mp3Info.bBuffer,dReadLen,&dRealLen,g_Mp3Info.dFileOffset);
#else
			ReadBuffer(g_Mp3Info.bBuffer,dReadLen,&dRealLen,g_Mp3Info.dFileOffset);
#endif
			if(dRealLen < dReadLen)
				return VRES_ERROR;
		}
		else
		{
			pbFilePtr = pbFilePtr + dFrameSize-4;
		}
		while (1) 
		{		
			sdLen= sdLen- dFrameSize - dOffset;
			if(sdLen< 0)
				break;
			
			dOffset = 0;
			
			dHeader = SeekNextFrame(pbFilePtr,&dOffset,MP3_MAX_FRAME_OFFSET,dLastHeader,sdLen);
			if(dHeader == 0)
				break;
			
			if(g_Mp3Header.bBitrateIndex != ((UINT8) ((dHeader>>12)&0xf)))
			{
				g_Mp3Info.bVbrFlag = 1;
			}
			
			if(CheckHeadForTime(dHeader)==VRES_ERROR)
			{
				break;
			}
			dFrameSize = g_Mp3Info.dFrameSize;
			
			if(g_Mp3Info.dFrameNum >= MP3_MAX_SCAN_FRAME)
			{
				if((g_Mp3Info.bVbrFlag != 1))
				{
					g_Mp3Info.bXingFlag = 0;
					
					g_Mp3Info.dFrameNum = g_Mp3Info.dTotalSize / g_Mp3Info.dFrameSize;
					//g_Mp3Info.dTotalTime = g_Mp3Info.dFrameNum * 26;
					g_Mp3Info.dTotalTime = (g_Mp3Info.dFrameSize) * 8*g_Mp3Info.dFrameNum/ (g_Mp3Info.dBitrate);
#ifdef DEBUG_PRINT_MP3
					printf("The CBR time is %d:%d\n",g_Mp3Info.dTotalTime/60000,(g_Mp3Info.dTotalTime/1000) % 60);			
#endif
					return VRES_SUCCESS;
				}
			}
			if(g_Mp3Info.bStreamFlag)
			{
				g_Mp3Info.dFileOffset = g_Mp3Info.dFileOffset + dFrameSize + dOffset;
				dReadLen = 4;
#if MP3_NAND_FLASH_SUPPORT				
				ReadNANDBuffer(g_Mp3Info.bBuffer,dReadLen,&dRealLen,g_Mp3Info.dFileOffset);
#else
				ReadBuffer(g_Mp3Info.bBuffer,dReadLen,&dRealLen,g_Mp3Info.dFileOffset);
#endif
				if(dRealLen < dReadLen)
					return VRES_ERROR;
				
				pbFilePtr = g_Mp3Info.bBuffer;
			}
			else
			{
				pbFilePtr = pbFilePtr + dFrameSize + dOffset;
			}
			dLastHeader = dHeader;
			
			g_Mp3Info.dFrameNum ++;
			//g_Mp3Info.dTotalTime=g_Mp3Info.dTotalTime+g_Mp3Info.dFrameSize*8/ (g_Mp3Info.dBitrate);
		}
		g_Mp3Info.dTotalTime = g_Mp3Info.dFrameSize*8/ (g_Mp3Info.dBitrate);
		g_Mp3Info.dTotalTime = g_Mp3Info.dFrameNum * g_Mp3Info.dTotalTime;
#ifdef DEBUG_PRINT_MP3
		printf("g_Mp3Info.dFrameNum = %d\n",g_Mp3Info.dFrameNum);
		printf("The VBR time is %d:%d\n",g_Mp3Info.dTotalTime/60000,(g_Mp3Info.dTotalTime/1000) % 60);
#endif
	}
	
	return VRES_SUCCESS;
}


/***************************************************************
Description:
		Load file data and Analysis MP3 header.
		
Parameters:
		pbFile:    the mp3 file pointer.
		dFileLen:        the mp3 file length.
		EmptyRes:  not use.
		SrcFlag: the mp3 file is file stream or not.
Returns:
		VRES_SUCCESS OR VRES_ERROR.
****************************************************************/
VRESULT	VMp3_Load(HUGE UINT8* pbFile, UINT32 dFileLen, UINT32 EmptyRes, UINT8 SrcFlag)
{
	UINT32 dHeader = 0,dId3v2Len = 0,dOffset = 0;
	VHEADER eHeaderType;
	HUGE UINT8* pbFilePtr = pbFile; 

	UINT32 dReadLen=4,dRealLen=0;
	
	g_Mp3Info.bStreamFlag = SrcFlag;	//Add by TangMian

	g_Mp3Info.dTotalSize = dFileLen;	
	//check ID3V1Tag
	CheckID3V1Tag(pbFile,dFileLen);
	

	g_Mp3Info.pMp3Data = pbFile;
	g_Mp3Info.bEventFlag = 1;
	
	
	if(g_Mp3Info.bStreamFlag)
	{
		g_Mp3Info.PFile = (PVOID)pbFile;
#if MP3_NAND_FLASH_SUPPORT				
		ReadNANDBuffer(g_Mp3Info.bBuffer,dReadLen,&dRealLen,0);
#else
		ReadBuffer(g_Mp3Info.bBuffer,dReadLen,&dRealLen,0);
#endif
		if(dRealLen<dReadLen)
			return VRES_ERROR;
		
		eHeaderType = CheckHeadTYPE((HUGE UINT8*)(g_Mp3Info.bBuffer));
	}
	else
	{
		eHeaderType = CheckHeadTYPE(pbFilePtr);
	}
	
	//Extract ID3V2 tag
	if(eHeaderType == VHEADER_ID3V2)
	{
		//check ID3V2Tag
		if(g_Mp3Info.bStreamFlag)
		{
			dId3v2Len = CheckID3V2Tag(pbFilePtr,dFileLen);
					
			g_Mp3Info.dFileOffset = g_Mp3Info.dFileOffset + dId3v2Len ;
			
			dReadLen = 4;
			dRealLen = 0;
#if MP3_NAND_FLASH_SUPPORT
			ReadNANDBuffer(g_Mp3Info.bBuffer,dReadLen,&dRealLen,g_Mp3Info.dFileOffset);
#else
			ReadBuffer(g_Mp3Info.bBuffer,dReadLen,&dRealLen,g_Mp3Info.dFileOffset);
#endif
			if(dRealLen<dReadLen)
				return VRES_ERROR;
			eHeaderType = CheckHeadTYPE((HUGE UINT8*)(g_Mp3Info.bBuffer));
		}
		else
		{
			dId3v2Len = CheckID3V2Tag(pbFilePtr,dFileLen);		
			pbFilePtr = pbFilePtr + dId3v2Len ;
			eHeaderType = CheckHeadTYPE(pbFilePtr);
		}		
		
		g_Mp3Info.dID3V2Length = dId3v2Len;

		dFileLen= dFileLen- dId3v2Len ;
	}
	
	//Extract VBR Information
	if(eHeaderType == VHEADER_HEADER)
	{	
		//check header
		if(g_Mp3Info.bStreamFlag)
		{

			dHeader = Get4bytesFromBuffer((HUGE UINT8*)(g_Mp3Info.bBuffer));
		}
		else
		{
			dHeader = Get4bytesFromBuffer(pbFilePtr);
		}

		dHeader = GetFirstSync(pbFilePtr,&dOffset,MP3_MAX_FRAME_OFFSET,dFileLen);
		if(dOffset>(MP3_MAX_FRAME_OFFSET-8))
			return VRES_ERROR;
		g_Mp3Info.dJumpGarbage = dOffset;
		
		if(CheckHead(dHeader) == VRES_ERROR)
		{
			return VRES_ERROR;
		}
		if(g_Mp3Info.bStreamFlag)
		{
			g_Mp3Info.dFileOffset = g_Mp3Info.dFileOffset + 4;
		}
		else
		{
			pbFilePtr = pbFilePtr + 4;
		}
		dFileLen= dFileLen-4;
		
		
		//Get Mp3 total time and total frame
		if(ExtractMP3Info(pbFilePtr,dFileLen)==VRES_ERROR)
		{
			return VRES_ERROR;
		}
	}
	else
	{
		dHeader = GetFirstSync(pbFilePtr,&dOffset,MP3_MAX_FRAME_OFFSET,dFileLen);
		g_Mp3Info.dJumpGarbage = dOffset;
		if(dOffset>(MP3_MAX_FRAME_OFFSET-8))
			return VRES_ERROR;
		if(dHeader!= 0)
		{
			g_Mp3Info.dHeader = dHeader;
			if(CheckHead(dHeader) == VRES_ERROR)
			{
				return VRES_ERROR;
			}		
			if(g_Mp3Info.bStreamFlag)
			{
				g_Mp3Info.dFileOffset = g_Mp3Info.dFileOffset + 4;
			}
			else
			{
				pbFilePtr = pbFilePtr + dOffset;
			}

			dFileLen= dFileLen- dOffset;

			//Get Mp3 total time and total frame
			if(ExtractMP3Info(pbFilePtr,dFileLen)==VRES_ERROR)
			{
				return VRES_ERROR;
			}
		}
		else
		{
			return VRES_ERROR;
		}
	}
	
	return VRES_SUCCESS;
}


/***************************************************************
Description:
		Seek MP3 CBR file position.
		
Parameters:
		dMSecond:     the time of you want seek.
Returns:
		VRES_ERROR or VRES_SUCCESS.
****************************************************************/
VRESULT VMp3_CBR_Seek(UINT32 dMSecond)
{
	//If overflow buffer,than stop
	UINT32 dFrameTime,dFrameNumber;
	if(g_Mp3LoopInfo.bHasSeek)
		return VRES_SUCCESS;
	if(g_Mp3LoopInfo.bLoopFlag)
	{
		dFrameTime = g_Mp3Info.dTotalTime / g_Mp3Info.dFrameNum;
		dFrameNumber = g_Mp3LoopInfo.dLoopTimeB / dFrameTime;
		
		g_Mp3LoopInfo.dLoopPositionB = dFrameNumber * g_Mp3Info.dFrameSize + g_Mp3Info.dJumpGarbage;
		if(g_Mp3LoopInfo.dLoopPositionB >= g_Mp3Info.dTotalSize)
		{
			g_Mp3LoopInfo.dLoopPositionB = g_Mp3Info.dTotalSize;	
		}

		dFrameNumber = g_Mp3LoopInfo.dLoopTimeA/ dFrameTime;
		g_Mp3LoopInfo.dLoopPositionA = dFrameNumber * g_Mp3Info.dFrameSize + g_Mp3Info.dJumpGarbage+ g_Mp3Info.dID3V2Length;

		if(g_Mp3LoopInfo.dLoopPositionA >= g_Mp3Info.dTotalSize)
		{
			g_Mp3LoopInfo.dLoopPositionA = g_Mp3Info.dTotalSize;
			return VRES_ERROR;
		}
		g_Mp3Info.dCurrentTime = g_Mp3LoopInfo.dLoopTimeA;
		g_Mp3LoopInfo.bHasSeek = 1;

	}
	else
	{
		dFrameTime = g_Mp3Info.dTotalTime / g_Mp3Info.dFrameNum;
		dFrameNumber = dMSecond/ dFrameTime;
		g_Mp3Info.dStartPos = dFrameNumber * g_Mp3Info.dFrameSize + g_Mp3Info.dJumpGarbage;

		if(g_Mp3Info.dStartPos >= g_Mp3Info.dTotalSize)
		{
			g_Mp3Info.dStartPos = g_Mp3Info.dTotalSize;
			g_Mp3Info.dCurrentTime = g_Mp3Info.dTotalTime;		
		}
		g_Mp3Info.dCurrentTime = dMSecond;
	}
	#ifdef DEBUG_PRINT_MP3
	printf("the BR dPos = %d,the time is %d\n", g_Mp3Info.dStartPos,g_Mp3Info.dCurrentTime);	
#endif
	return VRES_SUCCESS;
}

/***************************************************************
Description:
		Seek MP3 XING file position.
		
Parameters:
		dMSecond:     the time of you want seek.
Returns:
		VRES_ERROR or VRES_SUCCESS.
****************************************************************/
VRESULT VMp3_XING_Seek(UINT32 dMSecond)
{
	UINT32 dTocIndex;
	
	if(g_Mp3LoopInfo.bHasSeek)
		return VRES_SUCCESS;
	if(g_Mp3LoopInfo.bLoopFlag)
	{
		dTocIndex = g_Mp3LoopInfo.dLoopTimeB *100/g_Mp3Info.dTotalTime;

		g_Mp3LoopInfo.dLoopPositionB = g_MP3VbrInfo.VBR_TOC.Xing[dTocIndex]*g_Mp3Info.dTotalSize/256;

		//If overflow buffer,than stop
		if(g_Mp3LoopInfo.dLoopPositionB > g_Mp3Info.dTotalSize)
		{
			g_Mp3LoopInfo.dLoopPositionB = g_Mp3Info.dTotalSize;
		}

		dTocIndex= g_Mp3LoopInfo.dLoopTimeA * 100/g_Mp3Info.dTotalTime;

		g_Mp3LoopInfo.dLoopPositionA = g_MP3VbrInfo.VBR_TOC.Xing[dTocIndex]*g_Mp3Info.dTotalSize/256;

		//If overflow buffer,than stop
		if(g_Mp3LoopInfo.dLoopPositionA > g_Mp3Info.dTotalSize)
		{
			g_Mp3LoopInfo.dLoopPositionA = g_Mp3Info.dTotalSize;
			return VRES_ERROR;
		}
		g_Mp3Info.dCurrentTime = g_Mp3LoopInfo.dLoopTimeA;
		g_Mp3LoopInfo.bHasSeek = 1;

	}
	else
	{
		dTocIndex= dMSecond*100/g_Mp3Info.dTotalTime;

		g_Mp3Info.dStartPos = g_MP3VbrInfo.VBR_TOC.Xing[dTocIndex]*g_Mp3Info.dTotalSize/256;

		//If overflow buffer,than stop
		if(g_Mp3Info.dStartPos > g_Mp3Info.dTotalSize)
		{
			g_Mp3Info.dStartPos = g_Mp3Info.dTotalSize;
			g_Mp3Info.dCurrentTime = g_Mp3Info.dTotalTime;
			return VRES_ERROR;
		}
		g_Mp3Info.dCurrentTime = dMSecond;
	}
	
	#ifdef DEBUG_PRINT_MP3
	printf("the CBR dPos = %d,the time is %d\n", g_Mp3Info.dStartPos,g_Mp3Info.dCurrentTime);	
#endif
	return VRES_SUCCESS;
}

/***************************************************************
Description:
		Seek MP3 FHG file position.
		
Parameters:
		dMSecond:     the time of you want seek.
Returns:
		VRES_ERROR or VRES_SUCCESS.
****************************************************************/
VRESULT VMp3_FHG_Seek(UINT32 dMSecond)
{
	UINT8  bSeekLoopA;
	UINT32 bIndex =0,bIA = 0, bSeekPoint = 0 ,bSeekPointA = 0 , bFraction = 0;
	UINT32 bDurationPerVbriFrames=0;
	UINT32 bAccumulatedTime = 0 ,bAccumulatedTimeA = 0;
 	if(g_Mp3LoopInfo.bHasSeek)
		return VRES_SUCCESS;
	bDurationPerVbriFrames = (g_Mp3Info.dTotalTime)/ (g_MP3VbrInfo.wVbr_TableSize+1);
 
 	bSeekLoopA = 1;

	while ( bAccumulatedTime <=dMSecond)
	{
    
		bSeekPoint+= g_MP3VbrInfo.VBR_TOC.Fhg[bIndex];
		bAccumulatedTime += bDurationPerVbriFrames;
		bIndex++;
		if(g_Mp3LoopInfo.bLoopFlag && (bAccumulatedTime>=g_Mp3LoopInfo.dLoopTimeA))
		{
			if(bSeekLoopA)
			{
				bSeekPointA = bSeekPoint;
				bAccumulatedTimeA = bAccumulatedTime;
				bIA = bIndex;
				bSeekLoopA = 0;
			}
		}
    
	}
  
	// Searched too far; correct result
	bFraction = ( (int)(((( bAccumulatedTime - dMSecond) / bDurationPerVbriFrames ) 
	+ (1.0f/(2*g_MP3VbrInfo.wVbr_EntryFrames))) * g_MP3VbrInfo.wVbr_EntryFrames));

	bSeekPoint -= g_MP3VbrInfo.VBR_TOC.Fhg[bIndex-1]*bFraction /g_MP3VbrInfo.wVbr_EntryFrames;

	g_Mp3Info.dStartPos = bSeekPoint;
	
	if(g_Mp3Info.dStartPos > g_Mp3Info.dTotalSize)
	{
		g_Mp3Info.dStartPos = g_Mp3Info.dTotalSize;
		g_Mp3Info.dCurrentTime = g_Mp3Info.dTotalTime;
	}

	g_Mp3Info.dCurrentTime = dMSecond;
	if(g_Mp3LoopInfo.bLoopFlag)
	{
		g_Mp3LoopInfo.dLoopPositionB = g_Mp3Info.dStartPos;
		
		// Searched too far; correct result
		bFraction = ( (int)(((( bAccumulatedTimeA - g_Mp3LoopInfo.dLoopTimeA) / bDurationPerVbriFrames ) 
		+ (1.0f/(2*g_MP3VbrInfo.wVbr_EntryFrames))) * g_MP3VbrInfo.wVbr_EntryFrames));

		bSeekPointA -= g_MP3VbrInfo.VBR_TOC.Fhg[bIA-1]*bFraction /g_MP3VbrInfo.wVbr_EntryFrames;

		g_Mp3LoopInfo.dLoopPositionA = bSeekPointA;
	
		if(g_Mp3LoopInfo.dLoopPositionA > g_Mp3Info.dTotalSize)
		{
			g_Mp3LoopInfo.dLoopPositionA = g_Mp3Info.dTotalSize;
			g_Mp3Info.dCurrentTime = g_Mp3Info.dTotalTime;
			return VRES_ERROR;
		}
		g_Mp3LoopInfo.bHasSeek = 1;

		
	}
	#ifdef DEBUG_PRINT_MP3
	printf("the CBR dPos = %d,the time is %d\n", g_Mp3Info.dStartPos,g_Mp3Info.dCurrentTime);	
#endif
	return VRES_SUCCESS;
}

/***************************************************************
Description:
		Seek MP3 NO TAB VBR file position.
		
Parameters:
		dMSecond:     the time of you want seek.
Returns:
		VRES_ERROR or VRES_SUCCESS.
****************************************************************/
VRESULT VMp3_VBR_NO_TAG_Seek(UINT32 dMSecond)
{
	UINT32 dLen=0,dFrameSize=0,dOffset,dLastHeader,dHeader,dReadLen,dRealLen,dTotalTime;
	UINT32 dFrameNum = 0,dFrameTime = 0;

	UINT8 bSeekLoopA;
	HUGE UINT8*  pbFilePtr = g_Mp3Info.pMp3Data;
	
	if(g_Mp3LoopInfo.bHasSeek)
		return VRES_SUCCESS;
	dLen = g_Mp3Info.dTotalSize - g_Mp3Info.dID3V2Length - g_Mp3Info.bID3V1Length - g_Mp3Info.dJumpGarbage;

	dLastHeader = g_Mp3Info.dHeader;

	pbFilePtr = pbFilePtr + g_Mp3Info.dID3V2Length + g_Mp3Info.dJumpGarbage;
	if(pbFilePtr == NULL)
		return VRES_ERROR;

	dOffset = 0;
	bSeekLoopA = 1;
	dTotalTime = 0;

	dFrameTime = g_Mp3Info.dFrameSize*8/ (g_Mp3Info.dBitrate);
	g_Mp3Info.dFileOffset = 0;

	while (1) 
	{		

        		if(dLen < (dFrameSize + dOffset))
			break;
		dFrameNum ++;
		g_Mp3Info.dStartPos = g_Mp3Info.dStartPos + dFrameSize + dOffset;

		dLen = dLen - dFrameSize - dOffset;
			
		dOffset = 0;

		dHeader = SeekNextFrame(pbFilePtr,&dOffset,MP3_MAX_FRAME_OFFSET,dLastHeader,dLen);
		if(dHeader == 0)
			break;

		if(CheckHeadForTime(dHeader)==VRES_ERROR)
		{
			continue;
		}
		dFrameSize = g_Mp3Info.dFrameSize;
			       
		if(g_Mp3Info.bStreamFlag)
		{
			g_Mp3Info.dFileOffset = g_Mp3Info.dFileOffset + dFrameSize + dOffset;
			dReadLen = 4;
#if MP3_NAND_FLASH_SUPPORT
			ReadNANDBuffer(g_Mp3Info.bBuffer,dReadLen,&dRealLen,g_Mp3Info.dFileOffset);
#else
			ReadBuffer(g_Mp3Info.bBuffer,dReadLen,&dRealLen,g_Mp3Info.dFileOffset);
#endif
			if(dRealLen < dReadLen)
				return VRES_ERROR;	
			pbFilePtr = g_Mp3Info.bBuffer;		
		}
		else
		{
			pbFilePtr = pbFilePtr + dFrameSize + dOffset;
		}
		dLastHeader = dHeader;

		dTotalTime = dFrameNum * dFrameTime;
		if(g_Mp3LoopInfo.bLoopFlag)
		{
			if(g_Mp3Info.bStreamFlag)
			{
				if(dTotalTime >=g_Mp3LoopInfo.dLoopTimeB)
				{
					g_Mp3LoopInfo.dLoopPositionB = g_Mp3Info.dFileOffset;
						g_Mp3LoopInfo.bHasSeek = 1;
					break;
				}
				if((dTotalTime >=g_Mp3LoopInfo.dLoopTimeA) && bSeekLoopA)
				{
					g_Mp3LoopInfo.dLoopPositionA = g_Mp3Info.dFileOffset;
					g_Mp3Info.dCurrentTime = g_Mp3LoopInfo.dLoopTimeA;
	
					bSeekLoopA = 0;
				}
			}
			else
			{
				if(dTotalTime >=g_Mp3LoopInfo.dLoopTimeB)
				{
					g_Mp3LoopInfo.dLoopPositionB = g_Mp3Info.dTotalSize - g_Mp3Info.dID3V2Length - g_Mp3Info.bID3V1Length - dLen;
						g_Mp3LoopInfo.bHasSeek = 1;
					break;
				}
				if((dTotalTime >=g_Mp3LoopInfo.dLoopTimeA) && bSeekLoopA)
				{
					g_Mp3LoopInfo.dLoopPositionA = g_Mp3Info.dTotalSize - g_Mp3Info.dID3V2Length - g_Mp3Info.bID3V1Length - dLen;
					g_Mp3Info.dCurrentTime = g_Mp3LoopInfo.dLoopTimeA;
	
					bSeekLoopA = 0;
				}
			}

		}
		else if(dTotalTime >=dMSecond)
		{
			if(g_Mp3Info.bStreamFlag)
			{
				g_Mp3Info.dStartPos = g_Mp3Info.dFileOffset;
				g_Mp3Info.dCurrentTime = dMSecond;				
			}
			else
			{
				g_Mp3Info.dStartPos = g_Mp3Info.dTotalSize - g_Mp3Info.dID3V2Length - g_Mp3Info.bID3V1Length - dLen;
				g_Mp3Info.dCurrentTime = dMSecond;
			}
			break;	
		}
	}


	if(g_Mp3Info.dStartPos > g_Mp3Info.dTotalSize)
	{
		g_Mp3Info.dStartPos = g_Mp3Info.dTotalSize;
		g_Mp3Info.dCurrentTime = g_Mp3Info.dTotalTime;
	}

#ifdef DEBUG_PRINT_MP3
	printf("the CBR dPos = %d,the time is %d\n", g_Mp3Info.dStartPos,g_Mp3Info.dCurrentTime);
#endif
	return VRES_SUCCESS;
}

/***************************************************************
Description:
		Seek MP3 file position.
		
Parameters:
		dMSecond:     the time of you want seek.
		FiFoType:     the File type,used by user.
Returns:
		VRES_ERROR or VRES_SUCCESS.
****************************************************************/
VRESULT	VMp3_Seek(UINT32 dMSecond,VDEV_FIFO_TYPE FiFoType)
{

#if MP3_FASH_PLAY
	if (dMSecond ==0)
	{
		g_Mp3Info.dReadOffset = 0;
		g_Mp3Info.dStartPos = 0;
	}

	return VRES_SUCCESS;

#endif
	//If overflow buffer,than stop
	if(!g_Mp3LoopInfo.bLoopFlag)
	{
		if(dMSecond == 0)
			return VRES_SUCCESS;
		if(dMSecond>3*26)
			dMSecond = dMSecond -26*3;
		g_Mp3Info.dSeekTime = dMSecond;
	}

	if((!g_Mp3LoopInfo.bLoopFlag) && (dMSecond>=g_Mp3Info.dTotalTime))
	{
		g_Mp3Info.dStartPos = 0;
		g_Mp3Info.dSeekTime = 0;
		return VRES_SUCCESS;
	}
	
	if(g_Mp3Info.bVbrFlag==0)
	{
		VMp3_CBR_Seek(dMSecond);
	}
#if 0
	//Xing flag
	else if((g_Mp3Info.bXingFlag == 1) && (g_MP3VbrInfo.bTOCTag ==1))
	{
		VMp3_XING_Seek(dMSecond);
	}

	//VBRI flag
	else if(g_Mp3Info.bXingFlag == 2)
	{
		VMp3_FHG_Seek(dMSecond);
	}
#endif
	//No flag for VBR
	else
	{
		VMp3_VBR_NO_TAG_Seek(dMSecond);
	}

	return VRES_SUCCESS;
}


/***************************************************************
Description:
		Get MP3 event.
		
Parameters:
		pUniMsg:      the event pointer.
		FiFoType:     the File type,used by user.
Returns:
		VRES_ERROR or VRES_SUCCESS .
****************************************************************/
VRESULT VMp3_GetEvent(PVM_UNI_MSG pUniMsg, VDEV_FIFO_TYPE FiFoType)
{
	UINT32 bBitPerSample = 16;
	if(pUniMsg==NULL)
	{
		return VRES_BUFFER_NULL;
	}

	if (g_Mp3Info.bEventFlag==1)
	{
		pUniMsg->bChn= 26;
		pUniMsg->sdDeltaTime = 0;
		pUniMsg->bMsgID = VMD_MSG_MP3_ON;
		pUniMsg->dPara1 = ((g_Mp3Info.dSampleRate & 0x0000FFFF) << 16) + ((g_Mp3Info.bStereo & 0x3)<<4) + ((bBitPerSample & 0xFF)<<6);
		pUniMsg->dPara2 = 0;
		pUniMsg->dPara3 = 0; 

		g_Mp3Info.bEventFlag = 2;
		
		return VRES_SUCCESS;
	}
	else if (g_Mp3Info.bEventFlag==2)
	{
/*		pUniMsg->bChn= 26;

		if(g_Mp3LoopInfo.bLoopFlag)
			pUniMsg->sdDeltaTime = g_Mp3LoopInfo.dLoopTimeB-g_Mp3LoopInfo.dLoopTimeA;
		else
			pUniMsg->sdDeltaTime = g_Mp3Info.dTotalTime-g_Mp3Info.dSeekTime;
		pUniMsg->bMsgID = VMD_MSG_MP3_OFF;
		pUniMsg->dPara1 = 0;
		pUniMsg->dPara2 = 0;
		pUniMsg->dPara3 = 0; 
*/
		g_Mp3Info.bEventFlag = 0;
		
//		return VRES_SUCCESS;
		return VRES_END_FLAG;
	}
	else
		return VRES_END_FLAG;
}

/***************************************************************
Description:
		Get MP3 file Information.
		
Parameters:
		pvFileInfo:      FILE pointer.
		FiFoType:     the File type,used by user.
Returns:
		VRES_ERROR or VRES_SUCCESS.
****************************************************************/
VRESULT	VMp3_GetFileInfo(PVOID pvFileInfo, VDEV_FIFO_TYPE FiFoType)
{
	FILE_INFO	*PFileInfo = (FILE_INFO*)pvFileInfo;	//Added By TangMian

	if(pvFileInfo== NULL)
	{
		return VRES_BUFFER_NULL;
	}
	PFileInfo->dTotalTime = g_Mp3Info.dTotalTime;		//Added By TangMian
	//*(UINT32*)pvFileInfo = g_Mp3Info.dTotalTime;
	
	/*-----------------------------------Added By TangMian  -------------------*/

	PFileInfo->mp3_title = g_Mp3Id3V1TAG.ID3.V1_0.bTitle;
	PFileInfo->mp3_artist = g_Mp3Id3V1TAG.ID3.V1_0.bArtist;
	PFileInfo->mp3_album = g_Mp3Id3V1TAG.ID3.V1_0.bAlbum;
	PFileInfo->mp3_genre = g_Mp3Id3V1TAG.bMp3Genre;
	PFileInfo->mp3_year = g_Mp3Id3V1TAG.ID3.V1_0.bYear;
	PFileInfo->mp3_comment = g_Mp3Id3V1TAG.ID3.V1_0.bComment;

	/*-----------------------------------Added By TangMian  -------------------*/

	return VRES_SUCCESS;
}

/***************************************************************
Description:
		Read the MP3 block.
		
Parameters:
		pbOutBuf:      the mp3 buffer.
		dBufLen:       the read buffer length.
		pdDataSize:  the real read buffer length.
		FiFoType:      the File type,used by user.      
Returns:
		VRES_ERROR or VRES_SUCCESS.
****************************************************************/
VRESULT	VMp3_ReadBlock(PUINT8 pbOutBuf, UINT32 dBufLen, PUINT32 pdDataSize, VDEV_FIFO_TYPE FiFoType)
{

	SINT32 dSize;
	UINT32 dRealLen=0;
	HUGE UINT8* dTemp;

	if (pbOutBuf== NULL || dBufLen == 0)
	{
		*pdDataSize = 0;
		g_Mp3Info.dStartPos = 0;
		return VRES_ERROR;
	}
	dSize=g_Mp3Info.dTotalSize;

	if(g_Mp3Info.bFinish)
	{
		*pdDataSize = 0;
		g_Mp3Info.dStartPos = 0;
		return VRES_STREAM_DATA_END;

	}
	if(g_Mp3Info.dReadOffset>=(dSize-g_Mp3Info.dID3V2Length-g_Mp3Info.dStartPos))
	{
		g_Mp3Info.dReadOffset = 0;
		*pdDataSize = 0;
		g_Mp3Info.dStartPos = 0;
		return VRES_STREAM_DATA_END;
	}

	//If Set Mp3 loop flag,then begin check loop mode
	if(g_Mp3LoopInfo.bLoopFlag)
	{
		//If Direct loop,then loop now
		dTemp=g_Mp3Info.pMp3Data + g_Mp3LoopInfo.dLoopPositionA + g_Mp3Info.dReadOffset;
		if((g_Mp3Info.dReadOffset + g_Mp3LoopInfo.dLoopPositionA) >= g_Mp3LoopInfo.dLoopPositionB)
			return VRES_STREAM_DATA_END;
	}
	else
	{
		//set buffer data
		dTemp=g_Mp3Info.pMp3Data+g_Mp3Info.dReadOffset + g_Mp3Info.dID3V2Length+g_Mp3Info.dStartPos;
	}
	
	//set length
	if (dBufLen >= (dSize - g_Mp3Info.dReadOffset-g_Mp3Info.dStartPos-g_Mp3Info.dID3V2Length))
	{
		*pdDataSize = (UINT16)(dSize - g_Mp3Info.dReadOffset-g_Mp3Info.dStartPos-g_Mp3Info.dID3V2Length);
		if(g_Mp3Info.bStreamFlag)
		{
			ReadBuffer(pbOutBuf,*pdDataSize,&dRealLen,(g_Mp3Info.dReadOffset+g_Mp3Info.dID3V2Length+g_Mp3Info.dStartPos));
			*pdDataSize = dRealLen;
		}
		else
		{
			Vmd_MemCpy((HUGE UINT8*)pbOutBuf,(HUGE UINT8*)dTemp,*pdDataSize);
		}
		
		
		g_Mp3Info.dReadOffset+=dSize;
	}
	else
	{
		*pdDataSize = dBufLen;

		if(g_Mp3Info.bStreamFlag)
		{
			ReadBuffer(pbOutBuf,*pdDataSize,&dRealLen,(g_Mp3Info.dReadOffset+g_Mp3Info.dID3V2Length+g_Mp3Info.dStartPos));
			*pdDataSize = dRealLen;

		}
		else
		{
			Vmd_MemCpy((HUGE UINT8*)pbOutBuf,(HUGE UINT8*)dTemp, dBufLen);
		}
				
		g_Mp3Info.dReadOffset+=dBufLen;
	}
	
	
	return VRES_SUCCESS;
	
	
}

/***************************************************************
Description:
		Set MP3  LoopA to B.
		
Parameters:
		bTimerA: the point A's time.
		bTimerB: the point b's time. 
Returns:
		VRES_SUCCESS OR VRES_ERROR.
****************************************************************/
VRESULT	VMp3_SetAtoB(UINT32 bTimerA,UINT32 bTimerB)
{
	if(bTimerA>26*3)
		bTimerA = bTimerA - 26*3;
	g_Mp3LoopInfo.dLoopTimeA = bTimerA;
	g_Mp3LoopInfo.dLoopTimeB = bTimerB;
	
	if(g_Mp3LoopInfo.dLoopTimeB > g_Mp3Info.dTotalTime)
	{
		g_Mp3LoopInfo.dLoopTimeB = g_Mp3Info.dTotalTime;
	}

	if(g_Mp3LoopInfo.dLoopTimeA >= g_Mp3LoopInfo.dLoopTimeB)
	{
		return VRES_ERROR;
	}
	g_Mp3LoopInfo.bLoopFlag = 1;
	g_Mp3LoopInfo.bHasSeek = 0;
	VMp3_Seek(bTimerB,(VDEV_FIFO_TYPE)1);
	g_Mp3Info.dReadOffset = 0;
	return VRES_SUCCESS;
}


/***************************************************************
Description:
		Get Mp3 Current time.
		
Parameters:
		pdCurTime:	the current time.
		FiFoType:      the File type,used by user.  
Returns:
		VRES_SUCCESS or VRES_Error.
****************************************************************/
VRESULT  VMp3_GetCurPos(PUINT32 pdCurTime, VDEV_FIFO_TYPE FiFoType)
{
	return VRES_SUCCESS;
}


/***************************************************************
Description:
		Get Mp3 Current time.
		
Parameters:
		void.
Returns:
		the  MP3 current time.
****************************************************************/
UINT32  VMp3_GetCurTime(VOID)
{
	return g_Mp3Info.dCurrentTime;
}

/***************************************************************
Description:
		Unload MP3 decoder and reset value.
		
Parameters:	FiFoType:      the File type,used by user.  
Returns:
		VRES_SUCCESS OR VRES_ERROR.
****************************************************************/
VRESULT	VMp3_Unload(VDEV_FIFO_TYPE FiFoType)
{
	g_Mp3Info.bFinish = 1;
	g_Mp3LoopInfo.bLoopFlag = 0;
	g_Mp3LoopInfo.dLoopPositionA = 0;
	g_Mp3LoopInfo.dLoopPositionB = 0;
	g_Mp3LoopInfo.dLoopTimeA = 0;
	g_Mp3LoopInfo.dLoopTimeB = 0;
	g_Mp3LoopInfo.bHasSeek = 0;
	return VRES_SUCCESS;
}

/***************************************************************
Description:
		End MP3 decoder.
		
Parameters:
		void.
Returns:
		VRES_SUCCESS OR VRES_ERROR.
****************************************************************/
VRESULT	VMp3_End(VOID)
{
	g_Mp3Info.bFinish = 1;
	return VRES_SUCCESS;
}



/***************************************************************
Description:
		Configure the Wave parser.
		
Parameters:
		dVal:		Conifg data.
		pvPtr:		Point to config data.
		bConfigID:	The parameter to be configured.
		
Returns:
		Return VRES_SUCCESS if success, else return error value.
		
****************************************************************/
VRESULT VMp3_Configure(UINT32 dVal, UINT32 dDat, UINT8 bConfigID)
{
#if 0
	if(dDat==1)
		g_Mp3Info.bStreamFlag = dDat;
	else
		g_Mp3Info.bStreamFlag = 0;
#endif
	return VRES_SUCCESS;
}

/***************************************************************
Description:
		Reset the parameters of wave parser
		
Parameters:
		FiFoType:		The FIFO which the data sent to .
		
Returns:
		Return VRES_SUCCESS if success, else return error value.
		
****************************************************************/
VRESULT VMp3_Reset(VDEV_FIFO_TYPE FiFoType)
{
	g_Mp3Info.bFinish = 0;			//Changed by GaoZhanDong  2004,10,18
	g_Mp3Info.dCurrentTime = 0;

#if MP3_FASH_PLAY
#else
	g_Mp3Info.dReadOffset = 0;
	g_Mp3Info.dStartPos = 0;
#endif
	g_Mp3Info.bEventFlag = 1;
	g_Mp3Info.dSeekTime = 0;
	return VRES_SUCCESS;
	
	
}
























