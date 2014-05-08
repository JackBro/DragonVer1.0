
#include "internal.h"

CYuvStat::CYuvStat(void)
{
	m_nCountY = 0;
	m_nCountU = 0;
	m_nCountV = 0;
}

CYuvStat::~CYuvStat(void)
{
}

/*
int CYuvStat::Yuv411Msg(int * des, char * src, int srcfmt, int unit)
{
	return 0;
}

int CYuvStat::Yuv420Msg(int * des, char * src, int srcfmt, int unit)
{
	return 0;
}
*/

void CYuvStat::Yuv422Msg(int * des, char * src, int srcfmt, int unit)
{
	int i, cap, yuv[4];
	char *psrc = src;

	cap = 256;
	if(unit == 2)
		cap = 1024;

	for(i = 0; i < 4; i++, psrc += unit)
	{
		yuv[YUVDetachStream[srcfmt][i]] = this->getvalue(psrc, unit);
		if( yuv[YUVDetachStream[srcfmt][i]] > cap-1)
		{
			//this->m_dbug.PrintMsg("the YUV fomat file is not correct.");
			//yuv[YUVDetachStream[srcfmt][i]] = cap-1;
		}
	}

	//得到的yuv[4]={Y,Y,U,V}，统计相应分量值到ndes中
	des[MAXY] = ( yuv[0] > yuv[1] ) ? yuv[0] : yuv[1];
	des[MINY] = ( yuv[0] < yuv[1] ) ? yuv[0] : yuv[1];
	des[AVGY] = yuv[0] + yuv[1];
	
	des[MAXU] = des[MINU] = des[AVGU] = yuv[2];
	
	des[MAXV] = des[MINV] = des[AVGV] = yuv[3];	  

}

/*
int CYuvStat::Yuv444Msg(int * des, char * src, int srcfmt, int unit)
{
	return 0;
}
*/