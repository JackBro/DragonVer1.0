
#ifndef _VFAT_BASE_H_
#define _VFAT_BASE_H_

#ifdef __cplusplus
extern "C" {
#endif

/////////////////////////////////////////////////////////////////
//              Disk Size To Sector Per Cluster                //
/////////////////////////////////////////////////////////////////

typedef struct tag_TVFatDS2SPC
{
	UINT32	disksize;		// unit is sector
	UINT8	secperclus;
} TVFatDS2SPC, *PTVFatDS2SPC;


/////////////////////////////////////////////////////////////////
//                         VFat Base                           //
/////////////////////////////////////////////////////////////////

UINT32	VFatGetClusFstSec(UINT32 clus, UINT8 secpclus, UINT32 fstdatsec);
UINT32	VFatGetNextClus(char *fat, UINT32 clus, UINT8 type);
void	VFatSetNextClus(char *fat, UINT32 clus, UINT32 val, UINT8 type);
int		VFatJudgeEntry(char *entry, char *name);
UINT8	VFatLfn2Name(char *entry, char *name);
UINT8	VFatStr2Upper(char *name);
char	*VFatPathParse(char *path, char *name);













#ifdef __cplusplus
}
#endif

#endif 
