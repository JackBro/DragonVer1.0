/*
 * YAFFS: Yet another FFS. A NAND-flash specific file system. 
 * yaffs_ramdisk.c: yaffs ram disk component
 *
 * Copyright (C) 2002 Aleph One Ltd.
 *
 * Created by Charles Manning <charles@aleph1.co.uk>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */

// This provides a rma disk under yaffs.
// NB this is not intended for NAND emulation.
// Use this with dev->useNANDECC enabled, then ECC overheads are not required.

const char *yaffs_flashif_c_version = "$Id: yaffs_flashif.c,v 1.1 2005/12/15 05:06:46 sx Exp $";


#include "yportenv.h"

#include "yaffs_flashif.h"
#include "yaffs_guts.h"
#include "devextras.h"
#include "flash.h"

#define SIZE_IN_MB 16

#define BLOCK_SIZE (32 * 528)
#define BLOCKS_PER_MEG ((1024*1024)/(32 * 512))



typedef struct 
{
	__u8 data[528]; // Data + spare
} yflash_Page;

typedef struct
{
	yflash_Page page[32]; // The pages in the block
	
} yflash_Block;



typedef struct
{
	yflash_Block **block;
	int nBlocks;
} yflash_Device;

static yflash_Device ramdisk;

static int  CheckInit(yaffs_Device *dev)
{
	DEV_FD_Reset();	
	return 1;
}

int yflash_WriteChunkToNAND(yaffs_Device *dev,int chunkInNAND,const __u8 *data, yaffs_Spare *spare)
{
	int blk;
	int pg;
	int r;
	unsigned char ndata[528];

	CheckInit(dev);
	
	blk = chunkInNAND/32;
	pg = chunkInNAND%32;
	
	//printf("AD %x %x |", data[0],data[1]);
	if(data)
	{
		memcpy(ndata,data,512);
		//printf("*%x ",ndata[0]);
	}
	
	
	if(spare)
	{
		memcpy(&ndata[512],spare,16);
	}
	r = DEV_FD_Write_Page(blk, pg, ndata);
	//printf("Writing %d %d:%d |",r, blk, pg);
	//printf("D %x %x && N %x %x |", data[0],data[1], ndata[0], ndata[1]);
	//printf("ND %x %x && S %x %x |\n", ndata[526],ndata[527], spare[14], spare[15]);
	return YAFFS_OK;	

}


int yflash_ReadChunkFromNAND(yaffs_Device *dev,int chunkInNAND, __u8 *data, yaffs_Spare *spare)
{
	int blk;
	int pg;
	int r = 0;
	unsigned char ndata[528];
	
	CheckInit(dev);
	
	blk = chunkInNAND/32;
	pg = chunkInNAND%32;
	
	DEV_FD_Read_Page(blk, pg, &(ndata[0]));
	//printf("reading %d %d:%d|",r, blk, pg);

	if(data)
	{
		memcpy(data,ndata,512);
	}
	
	
	if(spare)
	{
		memcpy(spare, &ndata[512], 16);
		if(dev->useNANDECC)
		{
			struct yaffs_NANDSpare *nsp = (struct yaffs_NANDSpare *)spare;
			nsp->eccres1 = 0;
			nsp->eccres2 = 0;
		}
	}
	//printf("D %x %x && N %x %x |", data[0], data[1], ndata[0], ndata[1]);
	//printf("ND %x %x && S %x %x |\n", ndata[526],ndata[527], spare[14], spare[15]);
	return YAFFS_OK;
}


int yflash_EraseBlockInNAND(yaffs_Device *dev, int blockNumber)
{
	
	CheckInit(dev);
	
	/*if(blockNumber < 0 || blockNumber >= ramdisk.nBlocks)
	{
		T(YAFFS_TRACE_ALWAYS,("Attempt to erase non-existant block %d\n",blockNumber));
		return YAFFS_FAIL;
	}
	else
	{*/
		
		printf("Erase %d\n",DEV_FD_Erase_Block(blockNumber));
		return YAFFS_OK;
	//}
	
}

int yflash_InitialiseNAND(yaffs_Device *dev)
{
	int i;
	dev->useNANDECC = 1; // force on useNANDECC which gets faked. 
						 // This saves us doing ECC checks.
	DEV_FD_Reset();

	return YAFFS_OK;
}


