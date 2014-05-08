#ifndef __SUMMON_MP3_H__
#define __SUMMON_MP3_H__

#define SUMMON_MP3_MAXCHAR 8
#define SUMMON_MP3_MAXBITRATE 14
typedef struct _tag_TMp3BitRate
{
	int  Mp3BitRate[SUMMON_MP3_MAXBITRATE];
	char Mp3BitRateStr[SUMMON_MP3_MAXBITRATE][SUMMON_MP3_MAXCHAR];
}TMp3BitRate,*PMp3BitRate;

//Mp3
extern const TMp3BitRate gMPEG1BitRate, gMPEG2BitRate;
#endif