#ifndef _MAIN_H_
#define _MAIN_H_
#include "ex_OldCmodel.h"


#ifdef WIN32
int main1(int argc, char * argv[]);
#else
int main(int argc, char * argv[]);
#endif

void test(void);

void convert(void);

void yuv2bmp(void);



#endif


