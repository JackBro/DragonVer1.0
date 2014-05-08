 
#include "driver938.h"

#ifndef	V938CMD_DISABLE


TAudioVolInfo gAudioVolInfo = 
{
    0x0,   //Left volume of MIDI
	0x0,   //Right volume of MIDI
	0x0,   //Left volume of wave
	0x0,   //Right volume of wave
	0x0,   //Left channel PANT out gain of DAC
	0x0,   //Right channel PANT out gain of DAC
	0x0   //Mixer output gain 
};

TAudioSFifoInfo gAudioSFifoInfo[4] = 
{
    /* wm      base addr    end addr */
	{  0x0,     0x0,        0x03af},
	{  0x0,     0x3b0,      0x03af},
	{  0x0,     0x760,      0x03af},
	{  0x0,     0xb100,     0x03af}
};

void V9_VwsOpen(void)
{
	UINT8 i;

	/* Set audio volume */
	V9B_AudioSetVolMidL(gAudioVolInfo.volMidL);
	V9B_AudioSetVolMidR(gAudioVolInfo.volMidR);
	V9B_AudioSetVolWavL(gAudioVolInfo.volWavL);
	V9B_AudioSetVolWavR(gAudioVolInfo.volWavR);
	V9B_AudioSetHpnVolL(gAudioVolInfo.hpnVolL);
	V9B_AudioSetHpnVolR(gAudioVolInfo.hpnVolR);
	V9B_AudioSetDacPgg(gAudioVolInfo.dacPgg);

	/* Set initial load address */
	V9B_AudioSetLoadAddr(0x0);

	/* Set SFIFI water mark and write base/end address */
	for(i = 0; i < 4; i++)
	{
		V9B_AudioSetSFifoWm(i, gAudioSFifoInfo[i].wm);
		V9B_AudioSetSFifoAWB(i,gAudioSFifoInfo[i].wBaseAddr);
		V9B_AudioSetSFifoAWE(i,gAudioSFifoInfo[i].wEndAddr);
	}

	/* Start ws */
	V9B_CpmResuWavSyn();
}

void     V9_VwsClose(void)
{

}

#endif //V938CMD_DISABLE


