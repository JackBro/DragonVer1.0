/*========================================================================================================

		vmalloc.c

=========================================================================================================*/
#include "vmalloc.h"
#include "../s3c44box/s3c44box.h"

static void UnLink(PTVmallocChunk pChunk);
static void InsertLink(PTVmallocChunk pChunk);
PTVmallocChunk VmallocConsolidation(PTVmallocChunk p);

#ifdef WIN32
TVmallocState gMallocState;
#else
static TVmallocState gMallocState;
#endif

#ifdef _VMALLOC_DEBUG
static char gMem[TEST_MEM_LENGTH];
#endif

void* vmalloc(unsigned int size)
{
	PTVmallocChunk p, newChunk;
	unsigned int remSize;
	
	size = REQUEST2SIZE(size);
	size = ((size+7)>>3)<<3;
	for(p = (PTVmallocChunk)gMallocState.head; p; p = p->bk)
	{
		if(size <= CHUNKSIZE(p))
			break;
	}
	/* not find available chunk */
	if(!p)
		return 0;

	/* find available chunk */
	UnLink(p);
	remSize = CHUNKSIZE(p) - size;
	if(remSize > MIN_CHUNK_SIZE)
	{
		newChunk = (PTVmallocChunk)((char*)p+size);
		SET_HEAD_SIZE(p, size);
		SET_FOOT(p, size);
		SET_HEAD(newChunk, remSize);
		SET_FOOT(newChunk, remSize);
		SET_INUSE(p);
		InsertLink(newChunk);
		return CHUNK2MEM(p);
	}
	SET_INUSE(p);
	return CHUNK2MEM(p);
}

void vfree(void *p)
{
	PTVmallocChunk pChunk, pFree;

	pFree = MEM2CHUNK(p);
	CLEAR_INUSE(pFree);
	pChunk = VmallocConsolidation(pFree);
	InsertLink(pChunk);

	return;
}

void UnLink(PTVmallocChunk pChunk)
{
	if(pChunk->fd)
		pChunk->fd->bk = pChunk->bk;
	else
		gMallocState.head = pChunk->bk;
	if(pChunk->bk)
		pChunk->bk->fd = pChunk->fd;
}

void InsertLink(PTVmallocChunk pChunk)
{
	PTVmallocChunk p, pTail = gMallocState.head;
	unsigned int size = CHUNKSIZE(pChunk);

	for(p = gMallocState.head; p; pTail = p, p = p->bk)
	{
		if(size <= CHUNKSIZE(p))
			break;
	}
	if(!p)			//insert into tail
	{
		if(!pTail)
		{
			gMallocState.head = pChunk;
			pChunk->fd = 0;
		}
		else
		{
			pTail->bk = pChunk;
			pChunk->fd = pTail;
		}
		pChunk->bk = 0;
	}
	else
	{
		if(p->fd)
			p->fd->bk = pChunk;
		else
			gMallocState.head = pChunk;
		pChunk->fd = p->fd;
		pChunk->bk = p;
		p->fd = pChunk;
	}
}

PTVmallocChunk VmallocConsolidation(PTVmallocChunk pFree)
{
	PTVmallocChunk p, pChunk = pFree;

	p = NEXT_CHUNK(pChunk);
	for(; (p && !INUSE(p) && ((char*)p<(char*)gMallocState.pAddr+gMallocState.length)); p = NEXT_CHUNK(pChunk))
	{
		UnLink(p);
		SET_FOOT(pChunk, CHUNKSIZE(p)+CHUNKSIZE(pChunk));
		SET_HEAD_SIZE(pChunk, CHUNKSIZE(p)+CHUNKSIZE(pChunk));
	}
	if(pChunk->prevSize == 0)
		return pChunk;
	p = PREV_CHUNK(pChunk);
	for(; (p && !INUSE(p) && ((char*)p>=(char*)gMallocState.pAddr)); p = PREV_CHUNK(pChunk))
	{
		UnLink(p);
		SET_FOOT(p, CHUNKSIZE(p)+CHUNKSIZE(pChunk));
		SET_HEAD_SIZE(p, CHUNKSIZE(p)+CHUNKSIZE(pChunk));
		pChunk = p;
		if(pChunk->prevSize == 0)
			break;
	}

	return pChunk;
}

void VmallocInit(char *start, unsigned int length)
{
	if(length <= MIN_CHUNK_SIZE)
		return;

	gMallocState.pAddr = (void*)start;
	gMallocState.length = length;
	gMallocState.head = (PTVmallocChunk)start;
	gMallocState.head->prevSize = 0;
	SET_HEAD(gMallocState.head, (length>>1)<<1);
	gMallocState.head->fd = 0;
	gMallocState.head->bk = 0;

	return;
}

void VmallocPrint(void)
{
	PTVmallocChunk p;
	unsigned int i = 0;
	unsigned int used;
	unsigned int *addr;

	p = (PTVmallocChunk)gMallocState.pAddr; 
	for(i = 0; i < gMallocState.length; )
	{
		used = (unsigned int)INUSE(p);
		addr = (unsigned int*)p;
		tmPrintf("addr = %x, size = %ld, used = %ld\n", addr, CHUNKSIZE(p), used);
//		printf("addr %ld to %ld, size = %ld, used = %ld\n", (unsigned int*)p, (unsigned int*)((unsigned int*)p+p->size), p->size, used);
		i = i + CHUNKSIZE(p);
		p = (PTVmallocChunk)((char*)p+CHUNKSIZE(p));
	}
}

void VMallocTestCase(void)
{
#ifdef _VMALLOC_DEBUG
	//void *p1, *p2, *p3, *p4, *p5, *p6;

	VmallocInit(gMem, TEST_MEM_LENGTH);
	
	//p1 = vmalloc(100);
	//p2 = vmalloc(200);
	//p3 = vmalloc(300);
	//p4 = vmalloc(600);
	//p5 = vmalloc(500);
	//p6 = vmalloc(100);
	//vfree(p2);
	//vfree(p3);
	//p2 = vmalloc(400);
	//p3 = vmalloc(700);
	//VmallocPrint();
	//vfree(p2);
	//vfree(p3);
	//vfree(p1);

	/*int i = 100;
	void *p;

	VmallocInit(gMem, TEST_MEM_LENGTH);

	do
	{
		p = vmalloc(i);
	}while(p);*/
#else
	return;
#endif
}
