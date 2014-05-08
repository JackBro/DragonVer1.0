/*========================================================================================================

		vmalloc.h

=========================================================================================================*/

#ifndef _VMALLOC_H_
#define _VMALLOC_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef WIN32
#define _VMALLOC_DEBUG
#else
#undef _VMALLOC_DEBUG
#endif

#ifdef _VMALLOC_DEBUG
#define TEST_MEM_LENGTH	5000
#endif

/* conversion from malloc headers to user pointers, and back */
#define CHUNK2MEM(p)   ((void*)((char*)(p) + 2*sizeof(unsigned int)))
#define MEM2CHUNK(mem) ((PTVmallocChunk)((char*)(mem) - 2*sizeof(unsigned int)))

/* The smallest possible chunk */
#define MIN_CHUNK_SIZE        (sizeof(TVmallocChunk))

/* size field being with PREV_INUSE when previous adjacent chunk in use */
#define PREV_INUSE 0x1

/* extract inuse bit of previous chunk */
#define PREV_CHUNK_INUSE(p)       ((p)->size & PREV_INUSE)

/* Get size, ignoring use bits */
#define CHUNKSIZE(p)         ((p)->size & ~(PREV_INUSE))
/* Ptr to next physical malloc_chunk. */
#define NEXT_CHUNK(p) ((PTVmallocChunk)( ((char*)(p)) + ((p)->size & ~PREV_INUSE) ))
/* Ptr to previous physical malloc_chunk */
#define PREV_CHUNK(p) ((PTVmallocChunk)( ((char*)(p)) - ((p)->prevSize) ))
/* extract p's inuse bit */
#define INUSE(p) ((((PTVmallocChunk)(((char*)(p))+((p)->size & ~PREV_INUSE)))->size) & PREV_INUSE)
/* set/clear chunk as being inuse */
#define SET_INUSE(p) ((PTVmallocChunk)(((char*)(p)) + ((p)->size & ~PREV_INUSE)))->size |= PREV_INUSE
#define CLEAR_INUSE(p) ((PTVmallocChunk)(((char*)(p)) + ((p)->size & ~PREV_INUSE)))->size &= ~(PREV_INUSE)
/* Set size at head */
#define SET_HEAD_SIZE(p, s)  ((p)->size = (((p)->size & PREV_INUSE) | (s)))
/* Set size/use field */
#define SET_HEAD(p, s)       ((p)->size = (s))
/* Set size at footer (only when chunk is not in use) */
#define SET_FOOT(p, s)       (((PTVmallocChunk)((char*)(p) + (s)))->prevSize = (s))

#define REQUEST2SIZE(req)	((req) + 2*sizeof(unsigned int))

typedef struct tag_TVmallocChunk
{
	unsigned int	prevSize;  /* Size of previous chunk (if free).  */
	unsigned int	size;       /* Size in bytes, including overhead. */

	struct tag_TVmallocChunk* fd;         /* double links -- used only if free. */
	struct tag_TVmallocChunk* bk;
} TVmallocChunk, *PTVmallocChunk;

typedef struct tag_TVmallocState
{
	PTVmallocChunk	head;		/*free chunk header*/
	void *pAddr;
	unsigned int length;
} TVmallocState, *PTVmallocState;

void*	vmalloc(unsigned int size);
void	vfree(void *p);
void	VmallocInit(char *start, unsigned int length);
void	VmallocPrint(void);

//test malloc
void VMallocTestCase(void);

#ifdef WIN32
extern TVmallocState gMallocState;
#endif

#ifdef __cplusplus
}
#endif

#endif/* _VMALLOC_H_ */


