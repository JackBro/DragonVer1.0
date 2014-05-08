
#ifndef _VFAT_API_H_
#define _VFAT_API_H_


#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <string.h>
#ifdef WIN32
#include <malloc.h>
#endif

#include "../vfatbase/vfattype.h"
#include "../vfatbase/vfatbase.h"


#define	VMALLOC		malloc
#define	VCALLOC		calloc
#define	VFREE		free

/////////////////////////////////////////////////////////////////
//                      Device Interface                       //
/////////////////////////////////////////////////////////////////

typedef int (*DevAcc)(char *buf, UINT32 start, UINT32 num);			// unit is sector, including start, num & return value;

typedef struct tag_TDevInfo
{
	DevAcc	rd;
	DevAcc	wr;
} TDevInfo, *PTDevInfo;


/////////////////////////////////////////////////////////////////
//                          VFat API                           //
/////////////////////////////////////////////////////////////////

typedef struct tag_TVFatVolume
{
	UINT8			type;
	UINT16			szpsec;
	UINT8			secpclus;
	UINT16			szpclus;
	UINT16			rvdsec;
	UINT32			fatsz;
	UINT8			fatnum;
	UINT32			rootfst;
	UINT32			rootclus;
	UINT16			rootsecs;
	UINT32			fstdatsec;
	UINT32			datclus;
	char			label[11];

	char			*fat;
	char			*fdt;
	PTVFatFHdl		*fh;
	TDevInfo		dev;
} TVFatVolume, *PTVFatVolume;








void	VFatMount(PTDevInfo pdev);
void	VFatUnmount(void);
FHANDLE VFatOpen(char *path, UINT8 mode);
void	VFatClose(FHANDLE handler);









#ifdef __cplusplus
}
#endif

#endif 
