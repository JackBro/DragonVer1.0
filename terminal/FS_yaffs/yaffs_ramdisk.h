/*
 * YAFFS: Yet another FFS. A NAND-flash specific file system. 
 * yaffs_ramdisk.h: yaffs ram disk component
 *
 * Copyright (C) 2002 Aleph One Ltd.
 *
 * Created by Charles Manning <charles@aleph1.co.uk>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * $Id: yaffs_ramdisk.h,v 1.1 2005/12/15 05:06:51 sx Exp $
 */

// This provides a rma disk under yaffs.
// NB this is not intended for NAND emulation.
// Use this with dev->useNANDECC enabled, then ECC overheads are not required.

#ifndef __YAFFS_RAMDISK_H__
#define __YAFFS_RAMDISK_H__


#include "yaffs_guts.h"
int yramdisk_EraseBlockInNAND(yaffs_Device *dev, int blockNumber);
int yramdisk_WriteChunkToNAND(yaffs_Device *dev,int chunkInNAND,const __u8 *data, yaffs_Spare *spare);
int yramdisk_ReadChunkFromNAND(yaffs_Device *dev,int chunkInNAND, __u8 *data, yaffs_Spare *spare);
int yramdisk_EraseBlockInNAND(yaffs_Device *dev, int blockNumber);
int yramdisk_InitialiseNAND(yaffs_Device *dev);

#endif
