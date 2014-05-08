
#include "internal938.h"
#ifndef	V938CMD_DISABLE

#include "../driver/basefunc/basefunc938.h"


static void isrHandle(UINT8 type);

static void __isrINT_STMR		    (void){ isrHandle(ISR_INT_STMR);}
static void __isrINT_EFIFOEMPTY		(void){ isrHandle(ISR_INT_EFIFOEMPTY);}
static void __isrINT_TMR		    (void){ isrHandle(ISR_INT_TMR);}
static void __isrINT_EFIFOERROR		(void){ isrHandle(ISR_INT_EFIFOERROR);}
static void __isrINT_SFIFO0EMPTY	(void){ isrHandle(ISR_INT_SFIFO0EMPTY);}
static void __isrINT_SFIFO1EMPTY	(void){ isrHandle(ISR_INT_SFIFO1EMPTY);}
static void __isrINT_SFIFO2EMPTY	(void){ isrHandle(ISR_INT_SFIFO2EMPTY);}
static void __isrINT_SFIFO3EMPTY	(void){ isrHandle(ISR_INT_SFIFO3EMPTY);}

static void __isrINT_MFIFOEMPTY		(void){ isrHandle(ISR_INT_MFIFOEMPTY);}
static void __isrINT_SWI1		    (void){ isrHandle(ISR_INT_SWI1);}
static void __isrINT_SWI2		    (void){ isrHandle(ISR_INT_SWI2);}
static void __isrINT_SWI3		    (void){ isrHandle(ISR_INT_SWI3);}
static void __isrINT_SWI4			(void){ isrHandle(ISR_INT_SWI4);}
static void __isrINT_SWI5		    (void){ isrHandle(ISR_INT_SWI5);}
static void __isrINT_SWI6		    (void){ isrHandle(ISR_INT_SWI6);}
static void __isrINT_SWI7		    (void){ isrHandle(ISR_INT_SWI7);}

static void __isrINT_STM0_END		(void){ isrHandle(ISR_INT_STM0_END);}
static void __isrINT_STM1_END		(void){ isrHandle(ISR_INT_STM1_END);}
static void __isrINT_STM2_END		(void){ isrHandle(ISR_INT_STM2_END);}
static void __isrINT_STM3_END		(void){ isrHandle(ISR_INT_STM3_END);}
static void __isrINT_MP3_END		(void){ isrHandle(ISR_INT_MP3_END);}


TIntIsr938 gIntisr938[] =
{
	{	ISR_INT_STMR       	  ,	"INT_STMR		    \n",	CTRL_INT_STMR		,	__isrINT_STMR	        },
	{	ISR_INT_EFIFOEMPTY	  ,	"INT_EFIFOEMPTY		\n",	CTRL_INT_EFIFOEMPTY	,	__isrINT_EFIFOEMPTY	    },
	{	ISR_INT_TMR	          , "INT_TMR			\n",	CTRL_INT_TMR	        ,	__isrINT_TMR	        },
	{	ISR_INT_EFIFOERROR	  ,	"INT_EFIFOERROR		\n",	CTRL_INT_EFIFOERROR	,	__isrINT_EFIFOERROR	    },
	{	ISR_INT_SFIFO0EMPTY	  ,	"INT_SFIFO0EMPTY	\n",	CTRL_INT_SFIFO0EMPTY	,	__isrINT_SFIFO0EMPTY    },
	{	ISR_INT_SFIFO1EMPTY	  ,	"INT_SFIFO1EMPTY	\n",	CTRL_INT_SFIFO1EMPTY	,	__isrINT_SFIFO1EMPTY    },
	{	ISR_INT_SFIFO2EMPTY	  ,	"INT_SFIFO2EMPTY	\n",	CTRL_INT_SFIFO2EMPTY	,	__isrINT_SFIFO2EMPTY    },
	{	ISR_INT_SFIFO3EMPTY   ,	"INT_SFIFO3EMPTY	\n",	CTRL_INT_SFIFO3EMPTY	,	__isrINT_SFIFO3EMPTY	},

	{	ISR_INT_MFIFOEMPTY    ,	"INT_MFIFOEMPTY		\n",	CTRL_INT_MFIFOEMPTY	,	__isrINT_MFIFOEMPTY	    },
	{	ISR_INT_SWI1		  ,	"INT_SWI1		    \n",	CTRL_INT_SWI1		,	__isrINT_SWI1	        },
	{	ISR_INT_SWI2	      , "INT_SWI2			\n",	CTRL_INT_SWI2	    ,	__isrINT_SWI2	        },
	{	ISR_INT_SWI3	      ,	"INT_SWI3		    \n",	CTRL_INT_SWI3	    ,	__isrINT_SWI3	        },
	{	ISR_INT_SWI4		  ,	"INT_SWI4		    \n",	CTRL_INT_SWI4		,	__isrINT_SWI4		    },
	{	ISR_INT_SWI5	      ,	"INT_SWI5		    \n",	CTRL_INT_SWI5	    ,	__isrINT_SWI5	        },
	{	ISR_INT_SWI6	      ,	"INT_SWI6	        \n",	CTRL_INT_SWI6	    ,	__isrINT_SWI6	        },
	{	ISR_INT_SWI7	      ,	"INT_SWI7	        \n",	CTRL_INT_SWI7	    ,	__isrINT_SWI7	        },

	{	ISR_INT_STM0_END      ,	"INT_STM0_END		\n",	CTRL_INT_STM0_END	,	__isrINT_STM0_END	    },
	{	ISR_INT_STM1_END	  ,	"INT_STM1_END		\n",	CTRL_INT_STM1_END	,	__isrINT_STM1_END	    },
	{	ISR_INT_STM2_END	  , "INT_STM2_END		\n",	CTRL_INT_STM2_END	,	__isrINT_STM2_END	    },
	{	ISR_INT_STM3_END	  ,	"INT_STM3_END		\n",	CTRL_INT_STM3_END	,	__isrINT_STM3_END	    },
	{	ISR_INT_MP3_END		  ,	"INT_MP3_END		\n",	CTRL_INT_MP3_END		,	__isrINT_MP3_END	    }
};


const int gIntnum938 = sizeof(gIntisr938)/sizeof(TIntIsr938);

#ifndef WIN32
extern char gTransBuf938[];
#endif

void TestCtrlInit938(void)
{
	V9_ChipCtrlOpen();
}

void V9_ChipInitContext(void)
{
	V9_CtrlInitContext();
}

void V9_ChipCtrlOpen(void)
{
	V9_CtrlInitContext();
	V9_CtrlSetInfo((PTCtrlInfo938)&gCtrlInfo938);
	V9_CtrlOpen();
}

#ifdef WIN32

void testIntInit938(void)
{
	char str[0x100];

	sprintf(str, "testIntInit938 \n ");
	SendCmd(str);
}

void testDownAudioFile(UINT8 *pDat, UINT32 datLen)
{
	WriteBuf(0, (char*)pDat, datLen);
}

#else

void testIntInit938(void)
{
	int i;

#ifndef NCSC
	EnableExInt(EXINT3);
#endif

	for(i=0; i< gIntnum938; i++)
	{
		V9_CtrlIntSetCallback(gIntisr938[i].INTtype, gIntisr938[i].pIsr);
	}
}

void testDownAudioFile(UINT8 *pDat, UINT32 datLen)
{
	UINT32 i;
	char *tmpBuf = gTransferBuf;
         char *tmpFileBuf = gTransBuf938;

    pDat = pDat;

	WriteBuf(0, gTransferBuf, (int)datLen);
	
    for(i = 0; i < datLen && tmpFileBuf != NULL && tmpBuf != NULL; i++)
		*tmpFileBuf ++ = *tmpBuf++;
}

#endif

void isrHandle(UINT8 type)
{
	static unsigned long count = 0;
	count++;
	tmPrintf("==========ocurr counter = %d +++++++++++ %20s", count, gIntisr938[type].Desc);
}

#endif //V938CMD_DISABLE
