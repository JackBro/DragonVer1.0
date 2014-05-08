#include "v9initenv.h"
#ifndef	V938CMD_DISABLE


const char* gCpmIntType[] =
{
	"INT_STMR 	         \n",/*** INT section 1 ***/
    "INT_EFIFOEMPTY      \n",
    "INT_TMR             \n",
    "INT_EFIFOERROR      \n",
    "INT_SFIFO0EMPTY     \n",
    "INT_SFIFO1EMPTY     \n",
    "INT_SFIFO2EMPTY     \n",
    "INT_SFIFO3EMPTY     \n",

	"INT_MFIFOEMPTY      \n",/*** INT section 2 ***/
    "INT_SWI1            \n",
    "INT_SWI2            \n",
    "INT_SWI3            \n",
    "INT_SWI4            \n",
    "INT_SWI5            \n",
    "INT_SWI6            \n",
    "INT_SWI7            \n",

	"INT_STM0_END        \n",/*** INT section 3 ***/
    "INT_STM1_END        \n",
    "INT_STM2_END        \n",
    "INT_STM3_END        \n",
    "INT_MP3_END         \n",
};

const char gIntNum938 = sizeof(gCpmIntType) / sizeof(char*);

#endif //V578CMD_DISABLE


