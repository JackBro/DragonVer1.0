#include <stdio.h>
#include <string.h>
#include "vfattype.h"
#include "vfatbase.h"

//	Disk size to sector per cluster table for VFAT16
//	To use this talbe :
//	rvdseccnt[2]  must be 1;
//	numfats		  must be 2;
//	rootentcnt[2] must be 512
const TVFatDS2SPC gVFatDskTable[] =
{	
	{ 8400		,   0	},	// disks up to 4.1 MB, the 0 value for SecPerClusVal trips an error
	{ 32680		,   2	},	// disks up to 16 MB,   1k cluster
	{ 262144	,   4	},	// disks up to 128 MB,  2k cluster
	{ 524288	,   8	},	// disks up to 256 MB,  4k cluster
	{ 1048576	,	16	},	// disks up to 512 MB,  8k cluster
							// The entries after this point are not used unless FAT16 is forced
	{ 2097152	,	32	},	// disks up to 1 GB,    16k cluster
	{ 4194304	,	64	},	// disks up to 2 GB,    32k cluster
	{ 0xFFFFFFFF,	0	}	// any disk greater than 2GB, 0 value for SecPerClusVal trips an error
};

//	Disk size to sector per cluster table for VFAT32
//	To use this talbe :
//	rvdseccnt[2]  must be 32;
//	numfats		  must be 2;
const TVFatDS2SPC gVFat32DskTable[] = 
{	
	{ 66600		,	0	},	// disks up to 32.5 MB, the 0 value for SecPerClusVal trips an error
	{ 532480	, 	1	},	// disks up to 260 MB, 		0.5k cluster
	{ 16777216	, 	8	},	// disks up to 8 GB, 		4k cluster
	{ 33554432	,	16	},	// disks up to 16 GB, 		8k cluster
	{ 67108864	, 	32	},	// disks up to 32 GB, 		16k cluster
	{ 0xFFFFFFFF,	64	}	// disks greater than 32GB, 32k cluster
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------------------------
Function Name	:	VFatGetClusFstSec
Input			:	clus		- cluster index.
					secpclus	- sector number of a cluster.
					fstdatsec	- the first sector index of data region.
Output			:	The first sector index.
Description		:	Get the first sector index of a cluster.
-----------------------------------------------------------------------------------------------*/
UINT32 VFatGetClusFstSec(UINT32 clus, UINT8 secpclus, UINT32 fstdatsec)
{
	if(clus < 2)
		return 0;
	return ((clus - 2) * secpclus + fstdatsec);
}

/*----------------------------------------------------------------------------------------------
Function Name	:	VFatGetNextClus
Input			:	fat		- FAT table pointer.
					clus	- The current cluster index.
					type	- FAT type.
Output			:	The next cluster index.
Description		:	Get the next cluster index in the cluster chain.
-----------------------------------------------------------------------------------------------*/
UINT32 VFatGetNextClus(char *fat, UINT32 clus, UINT8 type)
{
	UINT32 next = 0;

	if(type == TP_VFAT12)
	{
		next = (UINT32)(*((UINT16 *)(fat + (clus + (clus >> 1)))));
		next = (clus & 0x01) ? (next >> 4) : (next & 0x0FFF);
		return next;
	}
	if(type == TP_VFAT16)
		return (UINT32)(*((UINT16 *)(fat + (clus << 1))));
	if(type == TP_VFAT32)
		return (*((UINT32 *)(fat + (clus << 2)))) & 0x0FFFFFFF;
	return next;
}

/*----------------------------------------------------------------------------------------------
Function Name	:	VFatGetNextClus
Input			:	fat		- FAT table pointer.
					clus	- The current cluster index.
					val		- The next cluster index.
					type	- FAT type.
Output			:	NULL.
Description		:	Set the next cluster index in the cluster chain.
-----------------------------------------------------------------------------------------------*/
void VFatSetNextClus(char *fat, UINT32 clus, UINT32 val, UINT8 type)
{
	if(type == TP_VFAT12)
	{
		if(clus & 0x01)
		{
			val <<= 4;
			*((UINT16 *)(fat + (clus + (clus >> 1)))) &= 0x000F;
		}
		else
		{
			val &= 0x0FFF;
			*((UINT16 *)(fat + (clus + (clus >> 1)))) &= 0xF000;
		}
		*((UINT16 *)(fat + (clus + (clus >> 1)))) |= (UINT16)val;
	}
	if(type == TP_VFAT16)
		*((UINT16 *)(fat + (clus << 1))) = (UINT16)val;
	if(type == TP_VFAT32)
	{
		val &= 0x0FFFFFFF;
		(*((UINT32 *)(fat + (clus << 2)))) &= 0xF0000000;
		(*((UINT32 *)(fat + (clus << 2)))) |= val;
	}
}

/*----------------------------------------------------------------------------------------------
Function Name	:	VFatEnt2Name
Input			:	ent_name	- The source name in the entry.
					name		- The actual name string.
Output			:	Null.
Description		:	Get the actual name string from the entry.
-----------------------------------------------------------------------------------------------*/
static void VFatEnt2Name(char *ent_name, char *name)
{
	UINT8 i = 0, j = 0, blank = 0;

	if((ent_name == NULL) || (name == NULL))
		return;
	for(i = 0; i < VFAT_NAME_MAX; i++)
	{
		name[i] = ent_name[i];
		blank = (name[i] == VFAT_BLANK_CHAR) ? (blank + 1) : 0;
	}
	i = i - blank;
	name[i++] = VFAT_DOT_CHAR;
	for(j = 0, blank = 0; j < VFAT_EXT_MAX; j++)
	{
		i = i + j;
		name[i] = ent_name[i];
		blank = (name[i] == VFAT_BLANK_CHAR) ? (blank + 1) : 0;
	}
	i = i - blank;
	if(blank < VFAT_BLANK_CHAR)
		i += 1;
	name[i] = VFAT_NUL_CHAR;
}

/*----------------------------------------------------------------------------------------------
Function Name	:	VFatJudgeEntry
Input			:	entry	- The entry to be judged.
					name	- The directory or file name string.
Output			:	SUCCEED - The entry is matched with the name string.
					FAILED  - The entry is not matched with the name string.
Description		:	Judge the entry is found or not.
-----------------------------------------------------------------------------------------------*/
int VFatJudgeEntry(char *entry, char *name)
{
	char cur_name[VFAT_FNAME2_MAX];

	if((entry[0] == VFAT_MARK_DEL) || (entry[11] == VFAT_ATTR_LFN))
		return FAILED;
	if((name == NULL) && (entry[11] == VFAT_ATTR_VOLID))
		return SUCCEED;
	memset(cur_name, 0, VFAT_FNAME2_MAX);
	VFatEnt2Name(entry, cur_name);
	if(0 == strcmp(cur_name, name))
		return SUCCEED;
	return FAILED;
}

/*----------------------------------------------------------------------------------------------
Function Name	:	VFatCheckSum
Input			:	name - Short entry name.
Output			:	Check sum value.
Description		:	Get the check sum value of the short entry name.
-----------------------------------------------------------------------------------------------*/
static UINT8 VFatCheckSum(UINT8 *name)
{
	UINT8 i = 0, sum = 0;

	if(name == NULL)
		return 0;
	for(i = 0; i < VFAT_FNAME1_MAX; i++)
		sum = ((sum & 1) ? 0x80 : 0) + (sum >> 1) + *name++;
	return sum;
}

/*----------------------------------------------------------------------------------------------
Function Name	:	VFatLfn2Name
Input			:	entry	- The first entry of the LFN entry chain.
					name	- The actual name string.
Output			:	SUCCEED - Name convert succeed.
					FAILED  - Name convert failed.
Description		:	Get the actual name string from the LFN entries.
-----------------------------------------------------------------------------------------------*/
UINT8 VFatLfn2Name(char *entry, char *name)
{
	char   *pos = NULL;
	UINT8  cnt = 0, sum = 0, i = 1, j = 0, k = 0;
	UINT16 data = 0;
	const UINT8 offset[] = {1, 3, 5, 7, 9, 14, 16, 18, 20, 22, 24, 28, 30};

	if((entry == NULL) || (name == NULL))
		return FAILED;
	cnt = entry[0] & 0x1F;
	if(cnt > 20)
		return FAILED;
	pos = entry + cnt * VFAT_ENTRY_SIZE;
	sum = VFatCheckSum((UINT8 *)pos);

	pos -= VFAT_ENTRY_SIZE;
	while(i <= cnt)
	{
		if((entry[11] != VFAT_ATTR_LFN) || ((UINT8)(entry[13]) != sum) || ((entry[0] & 0x1F) != i))
			return FAILED;
		for(j = 0; j < 13; j++, k++)
		{
			data = *((UINT16 *)(entry + offset[j]));
			if(data == 0)
				break;
			name[k] = (char)data;
			if((i == cnt) && (j == 8))
				break;
		}
		if(data == 0)
			break;
		i ++;
		pos -= VFAT_ENTRY_SIZE;
	}
	name[k+1] = VFAT_NUL_CHAR;
	return SUCCEED;
}

/*----------------------------------------------------------------------------------------------
Function Name	:	VFatStr2Upper
Input			:	name	- Name string.
Output			:	SUCCEED - Name convert succeed.
					FAILED  - Name convert failed.
Description		:	Convert the name string to upper case.
-----------------------------------------------------------------------------------------------*/
UINT8 VFatStr2Upper(char *name)
{
	int i = 0;

	if(name == NULL)
		return FAILED;
	if(VFAT_PATH_STR_MAX <= strlen(name))
		return FAILED;
	while(name[i] != VFAT_NUL_CHAR)
	{
		if((name[i] > 0x60) && (name[i] < 0x7B))
			name[i] -= 0x20;
		i ++;
	};
	return SUCCEED;
}

/*----------------------------------------------------------------------------------------------
Function Name	:	VFatStr2Upper
Input			:	name	- Name string.
Output			:	SUCCEED - Name convert succeed.
					FAILED  - Name convert failed.
Description		:	Convert the name string to upper case.
-----------------------------------------------------------------------------------------------*/
char *VFatPathParse(char *path, char *name)
{
	int len = 0;
	char *pt = NULL;

	if((path == NULL) || (name == NULL))
		return NULL;

	path ++;						// path[0] = '/'
	pt = strchr(path, '/');
	if(pt == NULL)
		len = (int)strlen(path);
	else
		len = (int)(pt - path);
	if(len == 0)
		return NULL;
	memcpy(name, path, len);
	name[len] = VFAT_NUL_CHAR;

	return pt;
}




