#ifndef _V5DISPLAY_H_
#define _V5DISPLAY_H_

#ifdef __cplusplus
extern "C" {
#endif

void V5M_LoadBmp(UINT8 *pbmp, UINT8 dst);
void V5M_DisplayB0Bmp(UINT8 *pbmp);
void V5M_DispSetPanelRegList(PTRegGroup parm, UINT8 type);
void V5M_DispSetPanelState(UINT8 state, UINT8 flag);



#ifdef __cplusplus
}
#endif

#endif
