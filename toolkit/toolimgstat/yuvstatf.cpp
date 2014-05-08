
#include "internal.h"

CYuvStatF::CYuvStatF(void)
{
}

CYuvStatF::~CYuvStatF(void)
{
}

/*
void CYuvStatF::Yuv411Msg(int * des, char * src, int srcfmt, int width, int height, int unit)
{
}

void CYuvStatF::Yuv420Msg(int * des, char * src, int srcfmt, int width, int height, int unit)
{
}
*/

void CYuvStatF::updateYuv(int *des, int *ndes)
{
	des[MAXY] = ( ndes[MAXY] > des[MAXY] )? ndes[MAXY] : des[MAXY];
	des[MINY] = ( ndes[MINY] < des[MINY] )? ndes[MINY] : des[MINY];
	des[AVGY] = ndes[AVGY] + des[AVGY];
	this->m_nCountY += 2;
	if ( this->m_nCountY == 1024)
	{
		des[AVGY] >>= 10;
		this->m_nCountY = 1;
	}

	des[MAXU] = ( ndes[MAXU] > des[MAXU] )? ndes[MAXU] : des[MAXU];
	des[MINU] = ( ndes[MINU] < des[MINU] )? ndes[MINU] : des[MINU];
	des[AVGU] = ndes[AVGU] + des[AVGU];
	this->m_nCountU += 1;
	if ( this->m_nCountU == 1024)
	{
		des[AVGU] >>= 10;
		this->m_nCountU = 1;
	}

	des[MAXV] = ( ndes[MAXV] > des[MAXV] )? ndes[MAXV] : des[MAXV];
	des[MINV] = ( ndes[MINV] < des[MINV] )? ndes[MINV] : des[MINV];
	des[AVGV] = ndes[AVGV] + des[AVGV];
	this->m_nCountV += 1;
	if ( this->m_nCountV == 1024)
	{
		des[AVGV] >>= 10;
		this->m_nCountV = 1;
	}
}

void CYuvStatF::Yuv422Msg(int * des, char * src, int srcfmt, int width, int height, int unit)
{
	int i, len, size, ndes[9];
	CFileBin fsrc;
	CBufferBase srcbuf;

	if( (unit != 1) && (unit != 2) )
		unit = 1;
	if(width % 2)
	{
		this->m_dbug.PrintMsg("Image width must be even\n");
		return;
	}
	
	fsrc.Open(src);

	size = unit << 2;
	len = width * height * unit * 2;
	if( 0 == len )
	{
		this->m_dbug.PrintMsg("The size of the image can not be zero!");
		return;
	}

	for(i = 0; i < len; i += size)
	{
		fsrc.Read(srcbuf.m_buf, size);
		CYuvStat::Yuv422Msg(ndes, srcbuf.m_buf, srcfmt, unit);

		if( 0 == i )//第一次得到小块统计值ndes先初始化到存储总的统计值的des数组中
		{
			for( i=0; i<9; i++)
				des[i] = ndes[i];
			
			this->m_nCountY = 2;//Y分量的AVGY(ndes[AVGY])中存储了两Y分量之和
			this->m_nCountU = 1;
			this->m_nCountV = 1;
		}
		else//以后每次都合并ndes值到des中
		{
			updateYuv(des, ndes);
		}
	}

	//最后计算均值
	des[AVGY] /= this->m_nCountY;
	this->m_nCountY = 1;
	des[AVGU] /= this->m_nCountU;
	this->m_nCountU = 1;
	des[AVGV] /= this->m_nCountV;
	this->m_nCountV = 1;
}


/*void CYuvStatF::Yuv444Msg(int * des, char * src, int srcfmt, int width, int height, int unit)
{

}
*/

void CYuvStatF::printYuvfmtMsg(int * des, char * src,int srcfmt, int width, int height, int unit)
{
	switch(srcfmt)
	{
	case YUV422_UYVY:
	case YUV422_VYUY:
	case YUV422_YUYV:
	case YUV422_YVYU:
	case YUV422_YUVY:
	case YUV422_YVUY:
	case YUV422_UVYY:
	case YUV422_VUYY:
	case YUV422_YYVU:
		this->Yuv422Msg(des, src, srcfmt, width, height, unit);
		break;

	case RAW_BG:
	case RAW_GB:
	case RAW_RG:
	case RAW_GR:
		break;
	case RGB1:
	case RGB2:
	case RGB4:
	case RGB8:
	case RGB444:
	case RGB666:
	case RGB32:
		break;

	case RGB24:
		break;
	case RGB565:
	case RGB555:
		break;
	case YUV422_YYUV:
		break;
	case YUV411_YYYYUV:
	case YUV411_YYYYVU:
	case YUV411_YUYVYY:
	case YUV411_YVYUYY:
	case YUV411_UVYYYY:
	case YUV411_VUYYYY:
		//this->Yuv411Msg(des, src, srcfmt, width, height, unit);
		break;

	case YUV420_UYVY:
	case YUV420_VYUY:
	case YUV420_YUYV:
	case YUV420_YVYU:
	case YUV420_YUVY:
	case YUV420_YVUY:
	case YUV420_UVYY:
	case YUV420_VUYY:
	case YUV420_YYVU:
	case YUV420_YYUV:
		//this->Yuv420Msg(des, src, srcfmt, width, height, unit);
		break;

	case YUV400:
		break;
	case YUV444:
		break;
	case YUV422BLK:
	case YUV411BLK:
		break;
	case YUV420BLK:
		break;
	case YUV400BLK:
	case YUV444BLK:
	case YUV422LBUF:
	case YUV411LBUF:
		break;
	case YUV420LBUF:
		break;
	case YUV400LBUF:
	case YUV444LBUF:
		break;
	case BMP:
	case JPEG422:
	case JPEG420:
	case JPEG411:
	case JPEG400:
	case JPEG444:
		break;
	case YUV420FRM:
		break;
	default:
		break;
	}
}
