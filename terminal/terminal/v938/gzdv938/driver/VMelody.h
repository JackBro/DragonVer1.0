#ifndef __VMELODY_H__
#define __VMELODY_H__



typedef struct _MELODYFILETRACK
{
	HUGE UINT8	*pDataHead;
	HUGE UINT8 	*pCurnData;
	HUGE UINT8 	*pDataEnd;
	UINT32	dwNextTick;
	UINT8 	preStatus;
}MELODYFILETRACK;


VRESULT VIMelody_Init(void);
VRESULT VIMelody_Load(HUGE UINT8* fp, UINT32 dLen, UINT32 EmptyRes, UINT8 SrcFlag);
VRESULT VIMelody_GetEvent(PVM_UNI_MSG pUniMsg, VDEV_FIFO_TYPE FiFoType);
VRESULT VIMelody_GetCurPos(PUINT32 PosPtr, VDEV_FIFO_TYPE FiFoType);
VRESULT VIMelody_Reset(VDEV_FIFO_TYPE FiFoType);

#endif /*__VMELODY_H__*/
