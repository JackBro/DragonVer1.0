
#ifndef _VFAT_TYPE_H_
#define _VFAT_TYPE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "../../v568/tools/type.h"


/////////////////////////////////////////////////////////////////
//                         FAT General                         //
/////////////////////////////////////////////////////////////////

enum VFATTYPEenum
{
	TP_VFAT12 = 0x01,
	TP_VFAT16,
	TP_VFAT32,
	TP_UNKNOWN
};

#define	VFAT_CLUS_MAX1		4085
#define	VFAT_CLUS_MAX2		65525

#define	VFAT12_BADCLUS		0x0FF7
#define	VFAT12_EOF			0x0FFF

#define	VFAT16_BADCLUS		0xFFF7
#define	VFAT16_EOF			0xFFFF

#define	VFAT32_BADCLUS		0x0FFFFFF7
#define	VFAT32_EOF			0x0FFFFFFF

#define	VFAT_ATTR_RO		0x01
#define	VFAT_ATTR_HIDE		0x02
#define	VFAT_ATTR_SYSTEM	0x04
#define	VFAT_ATTR_VOLID		0x08
#define	VFAT_ATTR_DIR		0x10
#define	VFAT_ATTR_ARCHIVE	0x20
#define	VFAT_ATTR_LFN		0x0F

#define	VFAT_MARK_END		0x00
#define	VFAT_MARK_DEL		0xE5


/////////////////////////////////////////////////////////////////
//                         Boot Sector                         //
/////////////////////////////////////////////////////////////////

#define	VFAT_BOOT_SIZE		0x200

#ifdef WIN32
#pragma	pack(1)
#endif
typedef struct tag_TVFatBoot
{
	UINT8	jmpboot[3];
	UINT8	oemname[8];
	UINT16	bytspersec;
    UINT8	secperclus;
	UINT16	rvdseccnt;
	UINT8	numfats;
	UINT16	rootentcnt;
	UINT16	totsec16;
	UINT8	media;
	UINT16	fatsz16;
	UINT16	secpertrk;
	UINT16	numheads;
	UINT32	hidsec;
	UINT32	totsec32;
	///////////////////////////
	UINT8	drvnum;
	UINT8	rsvd;
	UINT8	bootsig;
	UINT32	volid;
	UINT8	vollab[11];
	UINT8	fstype[8];
	UINT8	rsvd1[448];
	UINT16	endsig;
} TVFatBoot, *PTVFatBoot;
#ifdef WIN32
#pragma	pack()
#endif

#ifdef WIN32
#pragma	pack(1)
#endif
typedef struct tag_TVFat32Boot
{
	UINT8	jmpboot[3];
	UINT8	oemname[8];
	UINT16	bytspersec;
    UINT8	secperclus;
	UINT16	rvdseccnt;
	UINT8	numfats;
	UINT16	rootentcnt;
	UINT16	totsec16;
	UINT8	media;
	UINT16	fatsz16;
	UINT16	secpertrk;
	UINT16	numheads;
	UINT32	hidsec;
	UINT32	totsec32;
	///////////////////////////
	UINT32	fatsz32;
	UINT16	extflag;
	UINT16	fsver;
	UINT32	rootclus;
	UINT16	fsinfo;
	UINT16	bkbootsec;
	UINT8	rsvd[12];
	//////////////////////////
	UINT8	drvnum;
	UINT8	rsvd1;
	UINT8	bootsig;
	UINT32	volid;
	UINT8	vollab[11];
	UINT8	fstype[8];
	UINT8	rsvd2[420];
	UINT16	endsig;
} TVFat32Boot, *PTVFat32Boot;

#ifdef WIN32
#pragma	pack()
#endif

/////////////////////////////////////////////////////////////////
//                     FAT32 FSInfo Sector                     //
/////////////////////////////////////////////////////////////////

#ifdef WIN32
#pragma	pack(1)
#endif

typedef struct tag_TVFat32Info
{
	UINT32	leadsig;
	UINT8	rsvd[480];
	UINT32	structsig;
	UINT32	freecnt;
	UINT32	nxtfree;
	UINT8	rsvd1[12];
	UINT32	trailsig;
} TVFat32Info, *PTVFat32Info;

#ifdef WIN32
#pragma	pack()
#endif

/////////////////////////////////////////////////////////////////
//                         Dir Entry                           //
/////////////////////////////////////////////////////////////////

#define	VFAT_ENTRY_SIZE		0x20

#define	VFAT_NAME_MAX		0x08
#define	VFAT_EXT_MAX		0x03
#define	VFAT_FNAME1_MAX		0x0B
#define	VFAT_FNAME2_MAX		0x0C

#define	VFAT_LFN_ENT_MAX	640
#define	VFAT_LFN_STR_MAX	256
#define	VFAT_PATH_STR_MAX	260

#define	VFAT_BLANK_CHAR		' '
#define	VFAT_DOT_CHAR		'.'
#define	VFAT_NUL_CHAR		'\0'

#ifdef WIN32
#pragma	pack(1)
#endif

typedef struct tag_TVFatDirEntry
{
	char	name[11];
	UINT8	attr;
	UINT8	rsvd[10];
	UINT16	wrtime;
	UINT16	wrdate;
	UINT16	fstclus;
	UINT32	filesize;
} TVFatDirEntry, *PTVFatDirEntry;

#ifdef WIN32
#pragma	pack()
#endif

#ifdef WIN32
#pragma	pack(1)
#endif

typedef struct tag_TVFat32DirEntry
{
	char	name[11];
	UINT8	attr;
	UINT8	rsvd;
	UINT8	crttimetenth;
	UINT16	crttime;
	UINT16	crtdate;
	UINT16	lstaccdate;
	UINT16	fstclushi;
	UINT16	wrtime;
	UINT16	wrdate;
	UINT16	fstcluslo;
	UINT32	filesize;
} TVFat32DirEntry, *PTVFat32DirEntry;

#ifdef WIN32
#pragma	pack()
#endif

#ifdef WIN32
#pragma	pack(1)
#endif

typedef struct tag_TVFatLfnEntry
{
	UINT8	ord;
	UINT16	name1[5];
	UINT8	attr;
	UINT8	type;
	UINT8	chksum;
	UINT16	name2[6];
	UINT16	fstcluslo;
	UINT16	name3[2];
} TVFatLfnEntry, *PTVFatLfnEntry;

#ifdef WIN32
#pragma	pack()
#endif


/////////////////////////////////////////////////////////////////
//                           File                              //
/////////////////////////////////////////////////////////////////

typedef	unsigned char		FHANDLE;

#define	VFAT_FILE_MAX		0x40

enum VFATFILEenum
{
	MARK_RO,
	MARK_RW
};

typedef struct tag_TVFatFHdl
{
	UINT8 mode;
	char  ent[VFAT_ENTRY_SIZE];
} TVFatFHdl, *PTVFatFHdl;


#ifdef __cplusplus
}
#endif

#endif 
