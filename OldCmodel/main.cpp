

#include "dbugPrint.h"
#include "chip568.h"
#include "main.h"


#define CONFIG_FILE		"oldconfig.txt"

#ifdef WIN32
int main1(int argc, char *argv[])
#else
int main(int argc, char * argv[])
#endif
{
	char configfile[100];
	CCmodelSim v568;

	if(argc == 1)
		sprintf(configfile, "%s", CONFIG_FILE);
	else
		sprintf(configfile, "%s", argv[1]);

	if(v568.ConfigInit(configfile) == 0)
		v568.Process();

	Gdbug.DbugMsg("simulation process of V568 cmodel is over!\n");

	return 0;
}


//===================== Tools ========================
void test(void)
{
	CmodelTools tools;

//	tools.hex2bin("rgb.log", "j1.dat");
//	tools.bincompare("cdyblack.bmp", "mineblack.bmp");
	tools.bin1byte("bb.dat", "bb1.dat");
//	tools.block422tobmp("jj.dat", "a.dat", 640);

//	tools.yuv422rotation("lbuf.dat", "bb.dat", 640, 480);
//	tools.yuv422tobmp("yuv.dat", "bb1.dat");
	tools.addbmpheader("bb1.dat", "bb.bmp", 640, 480);


//	tools.binadd("ippinput.dat", "ipp2.dat", -1);
//	tools.binsizer("source.dat", "ispinput.dat", 648, 4, 4, 640, 480);
//	tools.bin2Tohex("jj.dat", "indct");
//	tools.bmp2rawdata("rawdata.bmp", "a.dat", 640);
//	tools.bin1Tohex("black1.raw", "640.raw");
}

void convert(void)
{
	CmodelTools tools;

	tools.bmp2rawdata("grid.bmp", "a.dat", 640);
	tools.bin1Tohex("a.dat", "640.raw");
//	tools.bin1Tohex("lbblack.raw", "lb.cfa");
}

void yuv2bmp(void)
{
	CmodelTools tools;

	tools.yuv422tobmp("yuv.dat", "cc1.dat");
	tools.addbmpheader("cc1.dat", "cc.bmp", 640, 480);
}
