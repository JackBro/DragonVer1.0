#include "vfatapi.h"


static TVFatVolume	gVFatVol;
static char gVFatLfnEnt[VFAT_LFN_ENT_MAX];
static char gVFatLfnStr[VFAT_LFN_STR_MAX];

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------------------------
Function Name	:	VFatGetStartPoint
Input			:	clus	- The current cluster index.
					start	- The 1st sector index of the next cluster.
Output			:	SUCCEED - The process succeed.
					FAILED  - An error occured in the process.
Description		:	Get the 1st sector index of the next cluster.
-----------------------------------------------------------------------------------------------*/
static UINT8 VFatGetStartPoint(char *parent, UINT32 *pclus, UINT32 *pstart)
{
	PTVFatDirEntry   par   = NULL;
	PTVFat32DirEntry par32 = NULL;

	if(parent == NULL)
	{
		*pstart = gVFatVol.rootfst;
		if(gVFatVol.type == TP_VFAT32)
			*pclus = gVFatVol.rootclus;
	}
	else
	{
		if(parent[11] == VFAT_ATTR_VOLID)
			return FAILED;
		par   = (PTVFatDirEntry)parent;
		par32 = (PTVFat32DirEntry)parent;
		if(gVFatVol.type == TP_VFAT32)
			*pclus = (par32->fstclushi << 16) + par32->fstcluslo;
		else
			*pclus = par->fstclus;
		*pstart = VFatGetClusFstSec(*pclus, gVFatVol.secpclus, gVFatVol.fstdatsec);
	}
	return SUCCEED;
}

/*----------------------------------------------------------------------------------------------
Function Name	:	VFatGetNextClusFstSec
Input			:	clus	- The current cluster index.
					start	- The 1st sector index of the next cluster.
Output			:	SUCCEED - The process succeed.
					FAILED  - An error occured in the process.
Description		:	Get the 1st sector index of the next cluster.
-----------------------------------------------------------------------------------------------*/
static UINT8 VFatGetNextClusFstSec(UINT32 clus, UINT32 *start)
{
	if((clus < 2) || (start == NULL))
		return FAILED;
	clus = VFatGetNextClus(gVFatVol.fat, clus, gVFatVol.type);
	if( ((gVFatVol.type == TP_VFAT12) && (clus == VFAT12_EOF))
		|| ((gVFatVol.type == TP_VFAT16) && (clus == VFAT16_EOF))
		|| ((gVFatVol.type == TP_VFAT32) && (clus == VFAT32_EOF)) )
		return FAILED;
	*start = VFatGetClusFstSec(clus, gVFatVol.secpclus, gVFatVol.fstdatsec);
	return SUCCEED;
}

/*----------------------------------------------------------------------------------------------
Function Name	:	VFatSearchEntry
Input			:	parent	- The parent entry.
					entry	- The sub entry.
					name	- Name string.
Output			:	SUCCEED - The entry has been found.
					FAILED  - The entry has not been found.
Description		:	Search the sub entry of the parent entry, which is matched with the name.
-----------------------------------------------------------------------------------------------*/
static int VFatSearchEntry(char *parent, char *entry, char *name)
{
	char   *pos = NULL;
	int	   flag = FAILED, loop = 0x01;
	UINT16 i = 0, count = 0, offset = 0, total = 0, size = 0;
	UINT32 start = 0, clus = 0;

	count = gVFatVol.szpclus / VFAT_ENTRY_SIZE;
	if(FAILED == VFatGetStartPoint(parent, &clus, &start))
		return FAILED;

	while(loop)
	{
		gVFatVol.dev.rd(gVFatVol.fdt, start, gVFatVol.secpclus);
		pos = gVFatVol.fdt;
		for(i = 0; i < count; i++, offset += VFAT_ENTRY_SIZE)
		{
			if(pos[11] == VFAT_ATTR_LFN)											// LFN
			{
				total = pos[0] & 0x1F;
				memset(gVFatLfnEnt, 0, VFAT_LFN_ENT_MAX);
				memset(gVFatLfnStr, 0, VFAT_LFN_STR_MAX);
				if(i + total >= count)
				{
					size = (count - i - 1) * VFAT_ENTRY_SIZE;
					memcpy(gVFatLfnEnt, pos, size);
					if(FAILED == VFatGetNextClusFstSec(clus, &start))
						return FAILED;
					gVFatVol.dev.rd(gVFatVol.fdt, start, gVFatVol.secpclus);
					pos = gVFatVol.fdt;
					memcpy((gVFatLfnEnt + size), pos, (total + i + 1 - count));
					i = total + i - count;
					offset = i * VFAT_ENTRY_SIZE;
				}
				else
				{
					size = total * VFAT_ENTRY_SIZE;
					memcpy(gVFatLfnEnt, pos, size);
					i = i + total;
					offset = offset + total * VFAT_ENTRY_SIZE;
				}
				if(FAILED == VFatLfn2Name(gVFatLfnEnt, gVFatLfnStr))
					return FAILED;
				i ++;
				offset += VFAT_ENTRY_SIZE;
				if(0 == strcmp(gVFatLfnStr, name))
				{
					if(i == count)
					{
						if(FAILED == VFatGetNextClusFstSec(clus, &start))
							return FAILED;
						gVFatVol.dev.rd(gVFatVol.fdt, start, gVFatVol.secpclus);
						memcpy(entry, gVFatVol.fdt, VFAT_ENTRY_SIZE);
					}
					else
						memcpy(entry, (gVFatVol.fdt + offset), VFAT_ENTRY_SIZE);
					return SUCCEED;
				}
			}
			memcpy(entry, (gVFatVol.fdt + offset), VFAT_ENTRY_SIZE);				// Short Entry
			if(entry[0] == VFAT_MARK_END)
				return FAILED;
			flag = VFatJudgeEntry(entry, name);
			if(flag == SUCCEED)
				return SUCCEED;
		}
		if(FAILED == VFatGetNextClusFstSec(clus, &start))
			break;;
	}
	return FAILED;
}

/*----------------------------------------------------------------------------------------------
Function Name	:	VFatSearchEntry
Input			:	parent	- The parent entry.
					entry	- The sub entry.
					name	- Name string.
Output			:	SUCCEED - The entry has been found.
					FAILED  - The entry has not been found.
Description		:	Search the sub entry of the parent entry, which is matched with the name.
-----------------------------------------------------------------------------------------------*/
//static int VFatSearchEntry(char *parent, char *entry, char *name)
//{
//	int	   flag = FAILED, flg = 0x01;
//	UINT16 i = 0, count = 0, offset = 0;
//	UINT32 start = 0, clus = 0;
//	PTVFatDirEntry   par   = NULL;
//	PTVFat32DirEntry par32 = NULL;
//
//	if(parent == NULL)
//		start = gVFatVol.rootfst;
//	else
//	{
//		if(parent[11] == VFAT_ATTR_VOLID)
//			return FAILED;
//		par   = (PTVFatDirEntry)parent;
//		par32 = (PTVFat32DirEntry)parent;
//		if(gVFatVol.type == TP_VFAT32)
//			clus = (par32->fstclushi << 16) + par32->fstcluslo;
//		else
//			clus = par->fstclus;
//		start = VFatGetClusFstSec(clus, gVFatVol.secpclus, gVFatVol.fstdatsec);
//	}
//	count = gVFatVol.szpclus / VFAT_ENTRY_SIZE;
//
//	while(flg)
//	{
//		gVFatVol.dev.rd(gVFatVol.fdt, start, gVFatVol.secpclus);
//		for(i = 0; i < count; i++, offset += VFAT_ENTRY_SIZE)
//		{
//			memcpy(entry, (gVFatVol.fdt + offset), VFAT_ENTRY_SIZE);
//			if(entry[0] == VFAT_MARK_END)
//				return FAILED;
//			flag = VFatJudgeEntry(entry, name);
//			if(flag == SUCCEED)
//				return SUCCEED;
//		}
//		clus = VFatGetNextClus(gVFatVol.fat, clus, gVFatVol.type);
//		if( ((gVFatVol.type == TP_VFAT12) && (clus == VFAT12_EOF))
//			|| ((gVFatVol.type == TP_VFAT16) && (clus == VFAT16_EOF))
//			|| ((gVFatVol.type == TP_VFAT32) && (clus == VFAT32_EOF)) )
//			return FAILED;
//		start = VFatGetClusFstSec(clus, gVFatVol.secpclus, gVFatVol.fstdatsec);
//	}
//	return FAILED;
//}

/*----------------------------------------------------------------------------------------------
Function Name	:	VFatFindEntry
Input			:	parent	- The parent entry.
					entry	- The sub entry.
					name	- Name string.
Output			:	SUCCEED - The entry has been found.
					FAILED  - The entry has not been found.
Description		:	Malloc a cluster buffer, and search the entry.
-----------------------------------------------------------------------------------------------*/
static int VFatFindEntry(char *parent, char *entry, char *name)
{
	int	flag = FAILED;

	if(entry == NULL)
		return FAILED;
	gVFatVol.fdt = VMALLOC(gVFatVol.szpclus);
	if(gVFatVol.fdt == NULL)
		return FAILED;
	flag = VFatSearchEntry(parent, entry, name);
	VFREE(gVFatVol.fdt);
	return flag;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------------------------
Function Name	:	VFatParseBoot
Input			:	buf	- Boot sector data buffer.
Output			:	NULL.
Description		:	Parse the boot sector data.
-----------------------------------------------------------------------------------------------*/
static void VFatParseBoot(char *buf)
{
	PTVFatBoot		boot = NULL;
	PTVFat32Boot	boot32 = NULL;

	if(buf == NULL)
		return;
	if(gVFatVol.type != TP_VFAT32)
	{
		boot = (PTVFatBoot)buf;
		gVFatVol.szpsec		= boot->bytspersec;
		gVFatVol.secpclus	= boot->secperclus;
		gVFatVol.szpclus	= gVFatVol.secpclus * gVFatVol.szpsec;
		gVFatVol.rvdsec		= boot->rvdseccnt;
		gVFatVol.fatsz		= boot->fatsz16;
		gVFatVol.fatnum		= boot->numfats;
		gVFatVol.rootclus	= 0;
		gVFatVol.rootfst	= gVFatVol.rvdsec + gVFatVol.fatsz * gVFatVol.fatnum;
		gVFatVol.rootsecs	= ((boot->rootentcnt << 5) + (boot->bytspersec - 1)) / boot->bytspersec;
		gVFatVol.fstdatsec	= boot->rvdseccnt + gVFatVol.fatsz * boot->numfats + gVFatVol.rootsecs;
		if(boot->totsec16)
			gVFatVol.datclus = (boot->totsec16 - gVFatVol.fstdatsec) / gVFatVol.secpclus;
		else
			gVFatVol.datclus = (boot->totsec32 - gVFatVol.fstdatsec) / gVFatVol.secpclus;
	}
	else
	{
		boot32 = (PTVFat32Boot)buf;
		gVFatVol.szpsec		= boot32->bytspersec;
		gVFatVol.secpclus	= boot32->secperclus;
		gVFatVol.szpclus	= gVFatVol.secpclus * gVFatVol.szpsec;
		gVFatVol.rvdsec		= boot32->rvdseccnt;
		gVFatVol.fatsz		= boot32->fatsz16;
		if(gVFatVol.fatsz == 0)
			gVFatVol.fatsz = boot32->fatsz32;
		gVFatVol.fatnum		= boot32->numfats;
		gVFatVol.rootclus	= boot32->rootclus;
		gVFatVol.rootsecs	= 0;
		gVFatVol.fstdatsec	= boot32->rvdseccnt + gVFatVol.fatsz * boot32->numfats;
		gVFatVol.rootfst	= VFatGetClusFstSec(gVFatVol.rootclus, gVFatVol.secpclus, gVFatVol.fstdatsec);
		gVFatVol.datclus	= (boot->totsec32 - gVFatVol.fstdatsec) / gVFatVol.secpclus;
	}
}

/*----------------------------------------------------------------------------------------------
Function Name	:	VFatReadBoot
Input			:	NULL.
Output			:	SUCCEED - Read and parse succeed.
					FAILED  - An error occured in the process.
Description		:	Read and Parse the boot sector data.
-----------------------------------------------------------------------------------------------*/
static int VFatReadBoot(void)
{
	char str[6], *buf = NULL;
	PTVFatBoot	 pb   = NULL;
	PTVFat32Boot pb32 = NULL;

	if(gVFatVol.dev.rd == NULL)
		return FAILED;
	buf = VMALLOC(VFAT_BOOT_SIZE);
	if(buf == NULL)
		return FAILED;
	gVFatVol.dev.rd(buf, 0, 1);

	str[5] = '\0';
	pb   = (PTVFatBoot)buf;
	memcpy(str, pb->fstype, 5);
	if(0 == strcmp(str, "FAT12"))
		gVFatVol.type = TP_VFAT12;
	else if(0 == strcmp(str, "FAT16"))
		gVFatVol.type = TP_VFAT16;
	else
	{
		pb32 = (PTVFat32Boot)buf;
		memcpy(str, pb32->fstype, 5);
		if(0 == strcmp(str, "FAT32"))
			gVFatVol.type = TP_VFAT32;
		else
		{
			gVFatVol.type = TP_UNKNOWN;
			return FAILED;
		}
	}
	VFatParseBoot(buf);
	VFREE(buf);
	return SUCCEED;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------------------------
Function Name	:	VFatReadFat
Input			:	NULL.
Output			:	SUCCEED - Read succeed.
					FAILED  - An error occured in the process.
Description		:	Read the FAT table.
-----------------------------------------------------------------------------------------------*/
static int VFatReadFat(void)
{
	gVFatVol.fat = VMALLOC((gVFatVol.fatsz * gVFatVol.szpsec));
	if(gVFatVol.fat == NULL)
		return FAILED;
	gVFatVol.dev.rd(gVFatVol.fat, gVFatVol.rvdsec, gVFatVol.fatsz);
	return SUCCEED;
}

/*----------------------------------------------------------------------------------------------
Function Name	:	VFatWriteFat
Input			:	NULL.
Output			:	SUCCEED - Write succeed.
					FAILED  - An error occured in the process.
Description		:	Write the FAT table.
-----------------------------------------------------------------------------------------------*/
static int VFatWriteFat(void)
{
	if(gVFatVol.dev.wr == NULL)
		return FAILED;
	gVFatVol.dev.wr(gVFatVol.fat, gVFatVol.rvdsec, gVFatVol.fatsz);
	gVFatVol.dev.wr(gVFatVol.fat, (gVFatVol.rvdsec + gVFatVol.fatsz), gVFatVol.fatsz);
	return SUCCEED;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------------------------
Function Name	:	VFatReadRoot
Input			:	NULL.
Output			:	SUCCEED - Read succeed.
					FAILED  - An error occured in the process.
Description		:	Search the root entries, and find the label.
-----------------------------------------------------------------------------------------------*/
static int VFatReadRoot(void)
{
	int flag = FAILED;
	char entry[VFAT_ENTRY_SIZE];

	flag = VFatFindEntry(NULL, entry, NULL);
	if(flag == SUCCEED)
		memcpy(gVFatVol.label, entry, 11);
	return flag;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------------------------
Function Name	:	VFatInitVol
Input			:	pdev - The device property structer pointer.
Output			:	NULL.
Description		:	Initialize the volume global context.
-----------------------------------------------------------------------------------------------*/
static void VFatInitVol(PTDevInfo pdev)
{
	memset(gVFatLfnEnt, 0, VFAT_LFN_ENT_MAX);
	memset(gVFatLfnStr, 0, VFAT_LFN_STR_MAX);
	memset(&gVFatVol,   0, sizeof(TVFatVolume));

	gVFatVol.type = TP_UNKNOWN;
	if(pdev)
	{
		gVFatVol.dev.rd = pdev->rd;
		gVFatVol.dev.wr = pdev->wr;
	}
}

/*----------------------------------------------------------------------------------------------
Function Name	:	VFatFsInit
Input			:	NULL.
Output			:	NULL.
Description		:	Initialize the file system.
-----------------------------------------------------------------------------------------------*/
static void VFatFsInit(void)
{
	if(FAILED == VFatReadBoot())
		return;
	if(FAILED == VFatReadFat())
		return;
	if(FAILED == VFatReadRoot())
		return;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------------------------
Function Name	:	VFatMount
Input			:	NULL.
Output			:	NULL.
Description		:	Mount the file system.
-----------------------------------------------------------------------------------------------*/
void VFatMount(PTDevInfo pdev)
{
	VFatInitVol(pdev);
	VFatFsInit();
}

/*----------------------------------------------------------------------------------------------
Function Name	:	VFatUnmount
Input			:	NULL.
Output			:	NULL.
Description		:	Unmount the file system.
-----------------------------------------------------------------------------------------------*/
void VFatUnmount(void)
{
	VFatWriteFat();
	if(gVFatVol.fat)
		VFREE(gVFatVol.fat);
}

/*----------------------------------------------------------------------------------------------
Function Name	:	VFatOpen
Input			:	path	- The path string.
					mode	- The file open mode.
Output			:	The pointer to the file handler.
					NULL    - Open file failed.
Description		:	Open a file.
-----------------------------------------------------------------------------------------------*/
FHANDLE VFatOpen(char *path, UINT8 mode)
{
	int flag = FAILED, loop = 1;
	UINT8 idx = 0, handler = 0;
	char  *name = NULL, *entry = NULL, *parent = NULL;

	if(path == NULL)
		return 0;

	for(idx = 0; idx < VFAT_FILE_MAX; idx++)
	{
		if(gVFatVol.fh[idx] == NULL)
			break;
		if(idx == (VFAT_FILE_MAX - 1))
			return 0;
	}
	gVFatVol.fh[idx] = (PTVFatFHdl)VMALLOC(sizeof(TVFatFHdl));
	if(gVFatVol.fh[idx] == NULL)
		return 0;

	if(FAILED == VFatStr2Upper(path))
	{
		VFREE(gVFatVol.fh[idx]);
		return 0;
	}

	name = VMALLOC(VFAT_LFN_STR_MAX);
	if(name == NULL)
	{
		VFREE(gVFatVol.fh[idx]);
		return 0;
	}

	entry = VMALLOC(VFAT_ENTRY_SIZE);
	parent = entry;
	if(entry == NULL)
	{
		VFREE(name);
		VFREE(gVFatVol.fh[idx]);
		return 0;
	}

	while(loop)
	{
		path = VFatPathParse(path, name);
		if(path == NULL)
			break;
		flag = VFatFindEntry(parent, entry, name);
		if(flag == FAILED)
			break;
	}

	if(flag == SUCCEED)
	{
		handler = idx + 1;
		gVFatVol.fh[idx]->mode = mode;
		memcpy(gVFatVol.fh[idx]->ent, entry, VFAT_ENTRY_SIZE);
	}

	VFREE(name);
	VFREE(entry);
	return handler;
}

/*----------------------------------------------------------------------------------------------
Function Name	:	VFatClose
Input			:	handler	- The pointer to the file handler.
Output			:	NULL.
Description		:	Close a file.
-----------------------------------------------------------------------------------------------*/
void VFatClose(FHANDLE handler)
{
	UINT8 i = 0;

	handler --;
	if(handler >= VFAT_FILE_MAX)
		return;
	for(i = handler; i < (VFAT_FILE_MAX - 1); i++)
	{
		if(gVFatVol.fh[i+1] == NULL)
			break;
		memcpy(gVFatVol.fh[i], gVFatVol.fh[i+1], sizeof(TVFatFHdl));
	}
	VFREE(gVFatVol.fh[i]);
}















