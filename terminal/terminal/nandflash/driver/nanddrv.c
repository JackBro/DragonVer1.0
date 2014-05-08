#include	"internal.h"


static TNandContext	gNandContext;
static char gPageBuf[BYTES_PER_PAGE];
static char gBlkMap[BLOCK_MAP_SIZE];	// block map : 0 - valid block; 1 - invalid block
//static char gBlkMap[BLOCK_MAP_SIZE] =
//{
//	0xFF, 0xFF, 0xEF, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
//	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
//	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
//	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
//	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
//	0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
//	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
//	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
//};

/////////////////////////////////////////////////////////////////////////

static void NandSetCmd(int cmd)
{
	if(gNandContext.pinfo == NULL)
		return;
	NandSetCmdBase(cmd, gNandContext.pinfo->unit);
}

static void NandSetAddr(int addr)
{
	if(gNandContext.pinfo == NULL)
		return;
	NandSetAddrBase(addr, gNandContext.pinfo->unit);
}

static void NandSetData(int data)
{
	if(gNandContext.pinfo == NULL)
		return;
	NandSetDataBase(data, gNandContext.pinfo->unit);
}

static int NandGetData(void)
{
	if(gNandContext.pinfo == NULL)
		return NAND_ERR;
	return NandGetDataBase(gNandContext.pinfo->unit);
}


/////////////////////////////////////////////////////////////////////////

/*---------------------------------------------------------------------
Function Name	:	NandUpdateMap
Input			:	buf - map buffer if need read, else it is NULL.
Output			:	None.
Description		:	Update block map.
---------------------------------------------------------------------*/
void NandUpdateMap(char *buf)
{
	int block = 0, byteoff = 0, bitoff = 0, pos[2];
	unsigned char data[4];

	if( (gNandContext.pinfo == NULL) || (gNandContext.map == NULL) )
		return;

	memset(data, 0, 4);
	if(gNandContext.pinfo->unit == 1)
	{
		pos[0] = 0x05;
		for(block = 0; block < gNandContext.pinfo->block; block ++)
		{
 			NandReadSpare(block, 0, pos[0], (char *)data, 0x01);			// 1st page
			if(data[0] == 0xff)
				NandReadSpare(block, 0x01, pos[0], (char *)data, 0x01);		// 2nd page
			if(data[0] != 0xff)
			{
				byteoff = block >> 3;
				bitoff  = block % 8;
				gNandContext.map[byteoff] |= (0x01 << bitoff);
			}
		}
	}
	if(gNandContext.pinfo->unit == 2)
	{
		pos[0] = 0;
		pos[1] = 0x05;
		for(block = 0; block < gNandContext.pinfo->block; block ++)
		{
			NandReadSpare(block, 0, pos[0], (char *)data, 0x01);				// 1st page
			NandReadSpare(block, 0, pos[1], (char *)(data + 2), 0x01);			// 1st page
			if( (data[0] == 0xff) && (data[1] == 0xff) && (data[2] == 0xff) && (data[3] == 0xff) )
			{
				NandReadSpare(block, 0x01, pos[0], (char *)data, 0x01);			// 2nd page
				NandReadSpare(block, 0x01, pos[1], (char *)(data + 2), 0x01);	// 2nd page
			}
			if( (data[0] != 0xff) || (data[1] != 0xff) || (data[2] != 0xff) || (data[3] != 0xff) )
			{
				byteoff = block >> 3;
				bitoff  = block % 8;
				gNandContext.map[byteoff] |= (0x01 << bitoff);
			}
		}
	}
	if(buf)
		memcpy(buf, gNandContext.map, BLOCK_MAP_SIZE);
}

/*---------------------------------------------------------------------
Function Name	:	NandBlockChk
Input			:	block - block index.
Output			:	0 - valid block
					1 - invalid block
Description		:	Check the block is valid or not.
---------------------------------------------------------------------*/
static int NandBlockChk(int block)
{
	int byteoff = 0, bitoff = 0;

	if( (gNandContext.pinfo == NULL) || (gNandContext.map == NULL) )
		return NAND_ERR;
	if(block >= gNandContext.pinfo->block)
		return NAND_ERR;
	byteoff = block >> 3;
	bitoff  = block % 8;
	if(gNandContext.map[byteoff] & (0x01 << bitoff))
		return 1;
	return 0;
}

/*---------------------------------------------------------------------
Function Name	:	NandNextVldBlock
Input			:	block - Current block index.
Output			:	The next valid block index.
					0 - No valid block.
Description		:	Check the block is valid or not.
---------------------------------------------------------------------*/
static int NandNextVldBlock(int block)
{
	int flag = 0x01;

	if(gNandContext.pinfo == NULL)
		return 0;

	while(flag)
	{
		block ++;
		if(block >= gNandContext.pinfo->block)
			return NAND_ERR;
		flag = NandBlockChk(block);
		if(flag == NAND_ERR)
			return NAND_ERR;
	};
	return block;
}

/*---------------------------------------------------------------------
Function Name	:	NandGetId
Input			:	None.
Output			:	ID(Maker ID & Device ID).
Description		:	Get the ID of nand flash chip.
---------------------------------------------------------------------*/
int NandGetId(void)
{
	int id = 0;

	NandSetCmd(0x90);
	NandSetAddr(0);
	id = NandGetData();
	id <<= 8;
	id += NandGetData();
	NandSetMode(INVALID_NAND);
	return id;
}

/*---------------------------------------------------------------------
Function Name	:	NandErase
Input			:	block - The block index to be erased.
Output			:	0 : Success..
Description		:	Erase one block.
---------------------------------------------------------------------*/
int NandErase(int block)
{
	int err = NAND_OK;

	if(gNandContext.pinfo == NULL)
		return NAND_ERR;
	if(NandBlockChk(block))
		return NAND_ERR;
	block <<= 5;

	NandSetCmd(0x60);
	NandSetAddr((block & 0xff));
	NandSetAddr(((block >> 8) & 0xff));
	NandDelay(2);
	NandSetCmd(0xd0);

	NandDelay(3);
	NandWaitReady();
	NandSetCmd(0x70);
	if(0x01 & NandGetData())
		err = NAND_ERR;
	NandSetMode(INVALID_NAND);
	return err;
}

/*---------------------------------------------------------------------
Function Name	:	NandReadPage
Input			:	block - block index
					page  - page  index
					buf   - data buffer point
Output			:	0 : Success..
Description		:	Read one page(only data aera).
---------------------------------------------------------------------*/
static int NandReadPage(int block, int page, char *buf)
{
	int tmp = 0, i = 0, unit = 0, offset = 0;

	if((gNandContext.pinfo == NULL) || (buf == NULL))
		return NAND_ERR;
	if(block >= gNandContext.pinfo->block)
		block = gNandContext.pinfo->block;
	if(page >= gNandContext.pinfo->page)
		page = gNandContext.pinfo->page;
	tmp = (block << 5) + page;

	NandSetCmd(0);
	NandSetAddr(0);
	NandSetAddr((tmp & 0xff));
	NandSetAddr(((tmp >> 8) & 0xff));
	NandDelay(3);
	NandWaitReady();

	for(i = 0; i < gNandContext.pinfo->tnum; i++)
	{
		tmp = NandGetData();
		unit = gNandContext.pinfo->unit;
		for(; unit > 0; unit--, offset++)
		{
			*(buf + offset) = (char)(tmp & 0xff);
			tmp >>= 8;
		}
	}
	NandSetMode(INVALID_NAND);
	return NAND_OK;
}

/*---------------------------------------------------------------------
Function Name	:	NandRead
Input			:	addr - start address in successive space
					buf  - data buffer point
					num  - data number to be read
Output			:	None.
Description		:	Read data(only data aera).
---------------------------------------------------------------------*/
void NandRead(int addr, char *buf, int num)
{
	int block = 0, page = 0, offset = 0, pos = 0, dsize = 0, size = 0;

	if((gNandContext.pinfo == NULL) || (buf == NULL))
		return;
	dsize = gNandContext.pinfo->dnum * gNandContext.pinfo->unit;
	offset = addr % dsize;
	page   = (addr / dsize) % gNandContext.pinfo->page;
	block  = addr / dsize / gNandContext.pinfo->page;
	for( ; num > 0; num -= size, pos += size, page ++)
	{
		if(page >= gNandContext.pinfo->page)
		{
			page = 0;
			block ++;
		}
		if(NandBlockChk(block))
			block = NandNextVldBlock(block);
		if(block == NAND_ERR)
			break;
		NandReadPage(block, page, gPageBuf);

		if(offset > 0)
		{
			offset = 0;
			size = dsize - offset;
			size = (size > num) ? num : size;
		}
		else if(num > dsize)
			size = dsize;
		else
			size = num;
		memcpy((buf + pos), gPageBuf, size);
	}
}

/*---------------------------------------------------------------------
Function Name	:	NandReadSpare
Input			:	block - block index
					page  - page  index
					start - start address in the spare aera of one page
					buf   - data buffer point
					num   - data number to be read
Output			:	0 : Success..
Description		:	Read spare aera data of one page.
---------------------------------------------------------------------*/
int NandReadSpare(int block, int page, int start, char *buf, int num)
{
	int tmp = 0, i = 0, unit = 0, offset = 0;

	if((gNandContext.pinfo == NULL) || (buf == NULL))
		return NAND_ERR;
	if(NandBlockChk(block))
		return NAND_ERR;
	if(page >= gNandContext.pinfo->page)
		page = gNandContext.pinfo->page;
	if(start >= (gNandContext.pinfo->tnum - gNandContext.pinfo->dnum))
	{
		start = gNandContext.pinfo->tnum - gNandContext.pinfo->dnum - 1;
		num = 1;
	}
	else
	{
		tmp = gNandContext.pinfo->tnum - gNandContext.pinfo->dnum - start;
		num = (num > tmp) ? tmp : num;
	}
	tmp = (((block << 5) + page) << 8) + start;

	NandSetCmd(0x50);
	NandSetAddr((tmp & 0xff));
	NandSetAddr(((tmp >> 8) & 0xff));
	NandSetAddr(((tmp >> 16) & 0xff));
	NandDelay(3);
	NandWaitReady();

	for(i = 0; i < num; i++)
	{
		tmp = NandGetData();
		unit = gNandContext.pinfo->unit;
		for(; unit > 0; unit--, offset++)
		{
			*(buf + offset) = (char)(tmp & 0xff);
			tmp >>= 8;
		}
	}
	NandSetMode(INVALID_NAND);
	return NAND_OK;
}

/*---------------------------------------------------------------------
Function Name	:	NandWritePage
Input			:	block - block index
					page  - page  index
					buf   - data buffer point
Output			:	0 : Success..
Description		:	Write one page(only data aera).
---------------------------------------------------------------------*/
static int NandWritePage(int block, int page, char *buf)
{
	int tmp = 0, i = 0, unit = 0, offset = 0, err = NAND_OK;

	if((gNandContext.pinfo == NULL) || (buf == NULL))
		return NAND_ERR;
	if(block >= gNandContext.pinfo->block)
		block = gNandContext.pinfo->block;
	if(page >= gNandContext.pinfo->page)
		page = gNandContext.pinfo->page;
	tmp = (block << 5) + page;

	NandSetCmd(0);
	NandSetCmd(0x80);
	NandSetAddr(0);
	NandSetAddr((tmp & 0xff));
	NandSetAddr(((tmp >> 8) & 0xff));
	NandDelay(3);
	NandWaitReady();

	for(i = 0; i < gNandContext.pinfo->tnum; i++)
	{
		tmp = 0;
		unit = gNandContext.pinfo->unit;
		for(; unit > 0; unit--, offset++)
		{
			tmp <<= 8;
			tmp += buf[offset + unit - 1];
		}
		NandSetData(tmp);
	}
	NandSetCmd(0x10);

	NandDelay(10);
	NandWaitReady();
	NandSetCmd(0x70);
	if(0x01 & NandGetData())
		err = NAND_ERR;
	NandSetMode(INVALID_NAND);
	return err;
}

/*---------------------------------------------------------------------
Function Name	:	NandWrite
Input			:	addr - start address in successive space
					buf  - data buffer point
					num  - data number to be read
Output			:	None.
Description		:	Write data(only data aera).
---------------------------------------------------------------------*/
void NandWrite(int addr, char *buf, int num)
{
	int block = 0, page = 0, offset = 0, pos = 0, dsize = 0, size = 0;

	if((gNandContext.pinfo == NULL) || (buf == NULL))
		return;
	dsize = gNandContext.pinfo->dnum * gNandContext.pinfo->unit;
	offset = addr % dsize;
	page   = (addr / dsize) % gNandContext.pinfo->page;
	block  = addr / dsize / gNandContext.pinfo->page;
	for( ; num > 0; num -= size, pos += size, page ++)
	{
		if(page >= gNandContext.pinfo->page)
		{
			page = 0;
			block ++;
		}
		if(NandBlockChk(block))
			block = NandNextVldBlock(block);
		if(block == NAND_ERR)
			break;
		NandReadPage(block, page, gPageBuf);

		if(offset > 0)
		{
			offset = 0;
			size = dsize - offset;
			size = (size > num) ? num : size;
		}
		else if(num > dsize)
			size = dsize;
		else
			size = num;
		memcpy(gPageBuf, (buf + pos), size);
		NandWritePage(block, page, gPageBuf);
	}
}

/*---------------------------------------------------------------------
Function Name	:	NandWriteSpare
Input			:	block - block index
					page  - page  index
					start - start address in the spare aera of one page
					buf   - data buffer point
					num   - data number to be read
Output			:	0 : Success..
Description		:	Write spare aera data of one page.
---------------------------------------------------------------------*/
int NandWriteSpare(int block, int page, int start, char *buf, int num)
{
	int tmp = 0, i = 0, unit = 0, offset = 0, err = NAND_OK;

	if((gNandContext.pinfo == NULL) || (buf == NULL))
		return NAND_ERR;
	if(NandBlockChk(block))
		return NAND_ERR;
	if(page >= gNandContext.pinfo->page)
		page = gNandContext.pinfo->page;
	if(start >= (gNandContext.pinfo->tnum - gNandContext.pinfo->dnum))
	{
		start = gNandContext.pinfo->tnum - gNandContext.pinfo->dnum - 1;
		num = 1;
	}
	else
	{
		tmp = gNandContext.pinfo->tnum - gNandContext.pinfo->dnum - start;
		num = (num > tmp) ? tmp : num;
	}
	tmp = (((block << 5) + page) << 8) + start;

	NandSetCmd(0x50);
	NandSetCmd(0x80);
	NandSetAddr((tmp & 0xff));
	NandSetAddr(((tmp >> 8) & 0xff));
	NandSetAddr(((tmp >> 16) & 0xff));
	NandDelay(3);
	NandWaitReady();

	for(i = 0; i < num; i++)
	{
		tmp = 0;
		unit = gNandContext.pinfo->unit;
		for(; unit > 0; unit--, offset++)
		{
			tmp <<= 8;
			tmp += buf[offset + unit - 1];
		}
		NandSetData(tmp);
	}
	NandSetCmd(0x10);

	NandDelay(10);
	NandWaitReady();
	NandSetCmd(0x70);
	if(0x01 & NandGetData())
		err = NAND_ERR;
	NandSetMode(INVALID_NAND);
	return err;
}

/*---------------------------------------------------------------------
Function Name	:	NandInitContext
Input			:	None.
Output			:	None.
Description		:	Initialize the context of nand module.
---------------------------------------------------------------------*/
static void NandInitContext(void)
{
	memset(&gNandContext, 0, sizeof(TNandContext));
	gNandContext.pinfo = (PTNandInfo)&gNandInfo;
	gNandContext.map   = gBlkMap;
}

/*---------------------------------------------------------------------
Function Name	:	NandInit
Input			:	None.
Output			:	None.
Description		:	Initialize nand module.
---------------------------------------------------------------------*/
void NandInit(void)
{
	memset(gPageBuf, 0, BYTES_PER_PAGE);
	memset(gBlkMap,  0, BLOCK_MAP_SIZE);

	NandInitContext();
//	NandUpdateMap(NULL);
	NandSetMode(INVALID_NAND);
}
