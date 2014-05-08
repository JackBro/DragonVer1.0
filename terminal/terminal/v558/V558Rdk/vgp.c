#include "v558api.h"
#include "vjpeg.h"
#include "yuv2bmp.h"
#include "vgp.h"
#include "string.h"
/**
****************************************** low level function ******************************************

	There are the  level 1 function as below.

*******************************************************************************************************/
/*************************************************************************************************
Description:
	Do horizontall mirror.
Parameters:
	pOut   : Output the databuf after the horizontal mirror.
	pIn    : Input the databuf before the horizontal mirror.
	pinsize   : The image size of databuf.
Returns:
	NULL
Remarks:
	NULL
************************************************************************************************/
void V558_GpDoHMirror(UINT8* pOut,UINT8* pIn,PV558_SIZE pinsize)
{
	UINT16  cxlength;
	UINT32  temp;
	UINT32  i,k;
	UINT32  srcadd,dstadd;
	
	temp       =   0;
	cxlength   =   (pinsize->cx<<1);
	for( k=0; k<pinsize->cy ; k++)
	{
		temp +=  cxlength;
		for( i=0; i<(pinsize->cx>>1); i++)
		{				
			srcadd  = temp + (i<<2);
			dstadd  = temp + (cxlength - 4 - (i<<2));
			*(pOut + dstadd )     = *(pIn + srcadd+1);
			*(pOut + dstadd+1 )   = *(pIn + srcadd  );
			*(pOut + dstadd+2 )   = *(pIn + srcadd+2);
			*(pOut + dstadd+3 )   = *(pIn + srcadd+3);
		}
		if( (pinsize->cx>>1)%2)
		{
			srcadd  =  temp + (pinsize->cx-2);
			*(pOut + srcadd  )    = *(pIn + srcadd +1);      
			*(pOut + srcadd+1  )  = *(pIn + srcadd );      
		}
	}
}
/*************************************************************************************************
Description:
	Do vertical mirror
Parameters:
	pOut   : Output the databuf after the vertical mirror.
	pIn    : Input the databuf before the vertical mirror.
	pinsize   : The image size of databuf.
Returns:
	NULL
Remarks:
	NULL
*************************************************************************************************/
void V558_GpDoVMirror(UINT8* pOut , UINT8* pIn , PV558_SIZE pinsize)
{
	UINT16  cylength;
	UINT32  step;
	UINT32  i,k;
	UINT32  srcadd,dstadd;
	
	srcadd    =   0;
	dstadd    =   (((pinsize->cy-1)*pinsize->cx)<<1);
	cylength  =  (pinsize->cy>>1);
	step      =  (pinsize->cx<<1);
	for( k=0; k<cylength; k++)
	{
		srcadd  +=  step;
		dstadd  -=  step;
		for( i=0 ; i<step ; i++)
		{
			*(pOut + dstadd + i)  =  *(pIn + srcadd + i );
			*(pOut + srcadd + i)  =  *(pIn + dstadd + i);
		}		
	}
}
/**********************************************************************************
Description:
	Adjust the brightness of the image.
Paremeters:
	pOut: Output the databuf after adjusting the brightness.
	pIn : Input the databuf before adjusting the brightness.
	pinsize   : The image size of databuf.
	step:    add lum value with the value of step.
Returns:
	NULL
Remarks:
	y  =  ((y + step) > 0xff) ? 0xff :  (y+ step).
***********************************************************************************/
void V558_GpAdjustLum( UINT8* pOut , UINT8* pIn , PV558_SIZE pinsize , SINT8 step)
{
	UINT32 i,k,cxlength;
	UINT32  srcadd,tempsrcadd;
	UINT16  temp;
	cxlength  =   (pinsize->cx<<1);
	srcadd    =    0;
	if( step>0 )
	{
		for( k=0 ; k<pinsize->cy ; k++)
		{
			srcadd  += cxlength;
			for( i =0 ; i <(pinsize->cx>>1) ; i++)
			{
				tempsrcadd  =  (srcadd + (i<<2));
				
				temp =  *(pIn  + tempsrcadd);
				temp  += step;
				if(temp >=0xff)
					*(pOut +tempsrcadd  )   = 0xff;
				else
					*(pOut + tempsrcadd )   = (UINT8)temp;
				temp =  *(pIn  + tempsrcadd + 1);
				temp += step;
				if(temp >=0xff)
					*(pOut + tempsrcadd +1)   = 0xff;
				else
					*(pOut + tempsrcadd+1)   = (UINT8)temp;
				*(pOut + tempsrcadd+2)  =  *(pIn  + tempsrcadd+2);
				*(pOut + tempsrcadd+3)  =  *(pIn  + tempsrcadd+3);
			}
		}
	}
	else
	{
		step = ((~step)+1);
		for( k=0 ; k<pinsize->cy ; k++)
		{
			srcadd  += cxlength;
			for( i =0 ; i <(pinsize->cx>>1) ; i++)
			{
				tempsrcadd  =  (srcadd + (i<<2));
				temp =  *(pIn  + tempsrcadd );
				if( temp < step)
					*(pOut + tempsrcadd  )   = 0x0;
				else
					*(pOut + tempsrcadd  )   = (UINT8)(temp-step);
				temp =  *(pIn  + tempsrcadd +1);
				if(temp < step)
					*(pOut + tempsrcadd+1)   = 0x0;
				else
					*(pOut + tempsrcadd+1)   = (UINT8)(temp-step);
				*(pOut + tempsrcadd+2)  =  *(pIn  + tempsrcadd+2);
				*(pOut + tempsrcadd+3)  =  *(pIn  + tempsrcadd+3);
			}
		}
	}
}
/************************************************************************************
Description:
	Change the image from color image to gray image.
Parameters:
	pOut: Output the databuf after setting gray.
	pIn : Input the databuf before setting gray.
	pinsize   : The image size of databuf.
Returns:
	NULL
Remarks:	
	u = v = 0x80
*************************************************************************************/
void V558_GpSetGray(UINT8* pOut , UINT8* pIn , PV558_SIZE pinsize)
{
	UINT32 i,k,cxlength,temp;
	UINT32  srcadd;
	cxlength    =   (pinsize->cx<<1);
	srcadd      =    0;
	for( k=0 ; k<pinsize->cy ; k++)
	{
		srcadd  += cxlength;
		for( i =0 ; i <(pinsize->cx>>1) ; i++)
		{
			temp    =  srcadd + (i<<2);
			*(pOut +  temp    )     =  *(pIn +  temp    );
			*(pOut +  temp + 1)     =  *(pIn +  temp + 1);
			*(pOut +  temp + 2)     =  0x80;
			*(pOut +  temp + 3)     =  0x80;
		}
	}
}
/**************************************************************************************
Description:
	Do negative.
Parameters:	
	pOut: Output the databuf after doing negative.
	pIn : Input the databuf before doing negative.
	pinsize   : The image size of databuf.
Returns:
	NULL
Remarks:
	y = 0xff -y
************************************************************************************/
void V558_GpDoNegative(UINT8* pOut , UINT8* pIn , PV558_SIZE pinsize)
{
	UINT32  i,k,cxlength,temp;
	UINT32  srcadd;
	cxlength    =    (pinsize->cx<<1);
	srcadd      =     0;
	for( k=0 ; k<pinsize->cy ; k++)
	{
		srcadd  += cxlength;
		for( i =0 ; i <(pinsize->cx>>1) ; i++)
		{
			temp   =   srcadd + (i<<2);
			*(pOut + temp  ) = 0xff - *(pIn + temp   );
			*(pOut + temp+1) = 0xff - *(pIn + temp +1);
			*(pOut + temp+2) = *(pIn + temp +2);
			*(pOut + temp+3) = *(pIn + temp +3);
		}
	}
}
/***************************************************************************
Description:
	Zoom out or zoom in the image.
Parameters:
	pOut: Output the databuf after doing zoom.
	pIn : Input the databuf before doing zoom.
	poutsize: the output size after doing zoom.
	pInsize : the input size before doing zoom.
Returns:
	If success, return 1,else return 0
Remarks:
	NULL	
***************************************************************************/
UINT8 V558_GpDoZoom(UINT8* pOut , UINT8* pIn , PV558_SIZE poutsize , PV558_SIZE pInSize)
{
	UINT16 i,k,m,n;
	UINT32 a0,b0,a1,b1,c;
	UINT32 srcadd00,srcadd01,srcadd10,srcadd11,dstadd;
	c      =   (poutsize->cx * poutsize->cy);
	for( k=0 ; k<poutsize->cy ; k++)
	{
		for( i=0 ; i<poutsize->cx ; i++)
		{
			n      =   ((pInSize->cx * i) / poutsize->cx);
			m      =   ((pInSize->cy * k) / poutsize->cy);

			a0     =   ((pInSize->cx * i) - ( poutsize->cx * n));
			b0     =   ((pInSize->cy * k) - ( poutsize->cy * m));
			a1     =   poutsize->cx  - a0;
			b1     =   poutsize->cy  - b0;
			//the (i,k) point 
			dstadd   =     (((k* poutsize->cx)<<1)           + ((i/2)<<2));
			srcadd00 =     (((m* pInSize->cx)<<1)            + ((n/2)<<2));
			srcadd01 =     (((m* pInSize->cx)<<1)            + (((n+1)/2)<<2));
			srcadd10 =     ((((m+1)* pInSize->cx)<<1)        + ((n/2)<<2));
			srcadd11 =     ((((m+1)* pInSize->cx)<<1)        + (((n+1)/2)<<2));
			//'y'
			*(pOut + dstadd + (i%2))  =  ((((*(pIn + srcadd00 + (n%2))     * a1)/poutsize->cx)*b1)/poutsize->cy +
						      (((*(pIn + srcadd10 + (n%2))     * a1)/poutsize->cx)*b0)/poutsize->cy +
						      (((*(pIn + srcadd01 + ((n+1)%2)) * a0)/poutsize->cx)*b1)/poutsize->cy +
						      (((*(pIn + srcadd11 + ((n+1)%2)) * a0)/poutsize->cx)*b0)/poutsize->cy );
			//'u'
			*(pOut + dstadd + 2)     =   ((((*(pIn + srcadd00 + 2)         * a1)/poutsize->cx)*b1)/poutsize->cy +
						      (((*(pIn + srcadd10 + 2)         * a1)/poutsize->cx)*b0)/poutsize->cy +
						      (((*(pIn + srcadd01 + 2)         * a0)/poutsize->cx)*b1)/poutsize->cy +
						      (((*(pIn + srcadd11 + 2)         * a0)/poutsize->cx)*b0)/poutsize->cy );
						     
			*(pOut + dstadd + 3)     =   ((((*(pIn + srcadd00 + 3)         * a1)/poutsize->cx)*b1)/poutsize->cy +
						      (((*(pIn + srcadd10 + 3)         * a1)/poutsize->cx)*b0)/poutsize->cy +
						      (((*(pIn + srcadd01 + 3)         * a0)/poutsize->cx)*b1)/poutsize->cy +
						      (((*(pIn + srcadd11 + 3)         * a0)/poutsize->cx)*b0)/poutsize->cy );
		}
	}
	return 1;
}
/*****************************************************************************************
Description:
	Do rotation.
Parameters:
	pOut: Output the databuf after doing rotation.
	pIn : Input the databuf before doing rotation.
	poutsize: The image size of output data.
	pinsize:  The image size of input data.
	degree:   The degree when we rotate. (0:90 degree ; 1:180 degree ; 2:270 degree)
Returns:
	If success, return 1,else return 0.
Remarks:
	NULL
*/
UINT8 V558_GpDoRotation(UINT8* pOut, UINT8* pIn , PV558_SIZE poutsize, PV558_SIZE pinsize , UINT8 degree)
{
	UINT16  i,k;
	UINT32  srcadd,dstadd;
	switch(degree)
	{
	case V558_ROTATION_90:	{
		poutsize->cx   =   pinsize->cy; 
		poutsize->cy   =   pinsize->cx; 
		for( k=0 ; k<(pinsize->cy>>1) ; k++)
		{
			for( i=0 ; i<(pinsize->cx>>1) ; i++)
			{
				dstadd   =   ((i*pinsize->cy)<<2)+((((pinsize->cy>>1) - k -1) )<<2);
				srcadd   =   (((k*pinsize->cx)<<2)+(i<<2)); 
				
				*(pOut + dstadd +1 )                   =  *(pIn + srcadd);
				*(pOut + dstadd + (poutsize->cx<<1)+1) =  *(pIn + srcadd+1);
				*(pOut + dstadd    )                   =  *(pIn + srcadd + (pinsize->cx<<1));
				*(pOut + dstadd + (poutsize->cx<<1)  ) =  *(pIn + srcadd + (pinsize->cx<<1)+1);

				*(pOut + dstadd + (poutsize->cx<<1) + 2)   =   *(pIn + srcadd +2); //'u00'
				*(pOut + dstadd + (poutsize->cx<<1) + 3)   =   *(pIn + srcadd +3); //'v01'
				*(pOut + dstadd +  2)   =   *(pIn + srcadd + (pinsize->cx<<1)+2); //'u10'
				*(pOut + dstadd +  3)   =   *(pIn + srcadd + (pinsize->cx<<1)+3); //'v11'
			}
		}
	}break;
	case V558_ROTATION_180:	{
		poutsize->cx   =   pinsize->cx; 
		poutsize->cy   =   pinsize->cy; 
		for( k=0 ; k<pinsize->cy ; k++)
		{
			for( i=0 ; i<(pinsize->cx>>1) ; i++)
			{
				dstadd   =   (((pinsize->cy - k -1)*pinsize->cx)<<1)+(((pinsize->cx - (i<<1) -1) /2)<<2);
				srcadd   =   ((k*pinsize->cx)<<1)+(i<<2); 
				*(pOut + dstadd    )     =    *(pIn + srcadd + 1);
				*(pOut + dstadd + 1)     =    *(pIn + srcadd    );
				*(pOut + dstadd + 2)     =    *(pIn + srcadd + 2);
				*(pOut + dstadd + 3)     =    *(pIn + srcadd + 3);
			}
		}
	}break;
	case V558_ROTATION_270:	{
		poutsize->cx   =   pinsize->cy; 
		poutsize->cy   =   pinsize->cx; 
		for( k=0 ; k<(pinsize->cy>>1) ; k++)
		{
			for( i=0 ; i<(pinsize->cx>>1) ; i++)
			{
				dstadd   =   (((((pinsize->cx>>1)-i-1)*pinsize->cy)<<2)+(k<<2));
				srcadd   =   (((k*pinsize->cx)<<2)+(i<<2)); 
				
				*(pOut + dstadd + (poutsize->cx<<1)   )   =  *(pIn + srcadd);
				*(pOut + dstadd                       )   =  *(pIn + srcadd+1);
				*(pOut + dstadd + (poutsize->cx<<1)+1 )   =  *(pIn + srcadd + (pinsize->cx<<1));
				*(pOut + dstadd + 1                   )   =  *(pIn + srcadd + (pinsize->cx<<1)+1);

				*(pOut + dstadd + (poutsize->cx<<1) + 2)  =   *(pIn + srcadd +2); //'u00'
				*(pOut + dstadd + (poutsize->cx<<1) + 3)  =   *(pIn + srcadd +3); //'v01'
				*(pOut + dstadd +  2)                     =   *(pIn + srcadd + (pinsize->cx<<1)+2); //'u10'
				*(pOut + dstadd +  3)                     =   *(pIn + srcadd + (pinsize->cx<<1)+3); //'v11'
			}
		}
	}break;
	}
	return 1;
}

/************************************************************************************************
Description:
	Adjust the contrast of image.
Parameters:
	pOut: Output the databuf after adjusting contrast.
	pIn : Input the databuf before adjusting contrast.
	pinsize:  The image size of input data.	
	grad:   The degree when we adjust contrast. (128>grad >-127)
Returns:
	If success, return 1, else return 0.
Remarks:
	NULL
************************************************************************************************/
UINT8 V558_GpAdjustContrast(UINT8* pOut , UINT8* pIn , PV558_SIZE pinsize , SINT8 grad)
{
	UINT16 i,k,cxlength;
	UINT16 temp,value;
	UINT32 srcadd,tempsrcadd;
	
	cxlength     =   (pinsize->cx<<1);
	srcadd       =    0;
	if(grad>0)
	{
		for( k=0 ; k<pinsize->cy ; k++)
		{
			srcadd    +=  cxlength;
			for( i=0 ; i<(pinsize->cx>>1) ; i++)
			{
				tempsrcadd   = srcadd + (i<<2);
				*(pOut + tempsrcadd + 2)  =  *(pIn + tempsrcadd + 2);
				*(pOut + tempsrcadd + 3)  =  *(pIn + tempsrcadd + 3);
				temp  =  *(pIn + tempsrcadd);
				if(temp<grad)
					value = 0;
				else
					value   =  (((temp-grad)<<7) / (128 - grad));
				if(value > 0xff)
					value = 0xff;
				*(pOut + tempsrcadd)  =  (UINT8)value;
				
				temp  =  *(pIn + tempsrcadd +1 );
				if(temp<grad)
					value = 0;
				else
					value   =  (((temp-grad)<<7) / (128 - grad));
				if(value > 0xff)
					value = 0xff;
				*(pOut + tempsrcadd +1 )  =  (UINT8)value;
			}
		}
	}
	else
	{
		grad  = ((~grad)+1);
		for( k=0 ; k<pinsize->cy ; k++)
		{
			srcadd    +=  cxlength;
			for( i=0 ; i<(pinsize->cx>>1) ; i++)
			{
				tempsrcadd   = srcadd + (i<<2);
				*(pOut + tempsrcadd + 2)  =  *(pIn + tempsrcadd + 2);
				*(pOut + tempsrcadd + 3)  =  *(pIn + tempsrcadd + 3);
				temp  =  *(pIn + tempsrcadd);
				if(temp<grad)
					value = 0;
				else
					value   =  (((temp+grad)<<7) / (128 + grad));
				if(value > 0xff)
					value = 0xff;
				*(pOut + tempsrcadd)  =  (UINT8)value;
				
				temp  =  *(pIn + tempsrcadd +1 );
				if(temp<grad)
					value = 0;
				else
					value   =  (((temp+grad)<<7) / (128 + grad));
				if(value > 0xff)
					value = 0xff;
				*(pOut + tempsrcadd +1 )  =  (UINT8)value;
			}
		}
	}
	return 1;
}

/***************************************************************************************************
Description:
	Set the color range of image.
Parameters:
	pOut: Output the databuf after set color range.
	pIn : Input the databuf before set color range.
	pinsize:  The image size of input data.	
	utop   :  The max value of u.
	ubottom:  The min value of u.
	vtop   :  The max value of v.
	vbottom:  The min value of v.
Remarks:
	             u(out)       =         0x80        .     u(in) < ubottom
	             u(out)       =         u(in)       .     ubottom < u(in) < utop
	             u(out)       =         0x80        .     u(in) > utop

	             v(out)       =         0x80        .     v(in) < vbottom
	             v(out)       =         v(in)       .     vbottom < v(in) < vtop
	             v(out)       =         0x80        .     v(in) > vtop	             
***************************************************************************************************/
UINT8 V558_GpSetColorRange(UINT8* pOut , UINT8* pIn , PV558_SIZE pinsize , UINT8 utop,UINT8 ubottom,UINT8 vtop,UINT8 vbottom)
{
	UINT8  temp;
	UINT16 i,k,cxlength;
	UINT32 srcadd,tempsrcadd;
	
	cxlength   =  (pinsize->cx<<1);
	srcadd     =  0;
	for( k=0 ; k<pinsize->cy ; k++)
	{
		srcadd    +=  cxlength;
		for( i=0 ; i<(pinsize->cx>>1) ; i++)
		{
			tempsrcadd   =   srcadd +  (i<<2);
			*(pOut + tempsrcadd    )  =  *(pIn + tempsrcadd    );
			*(pOut + tempsrcadd + 1)  =  *(pIn + tempsrcadd + 1);
			temp  =  *(pIn + tempsrcadd + 2);
			if((temp > utop) || (temp< ubottom))
				 *(pOut + tempsrcadd + 2) = 0x80;
			else
				 *(pOut + tempsrcadd + 2) = temp;

			temp  =  *(pIn + tempsrcadd + 3 );
			if((temp > vtop) || (temp < vbottom))
				*(pOut + tempsrcadd + 3 ) = 0x80;
			else
				*(pOut + tempsrcadd + 3) = temp;
		}
	}
	return 1;
}

/****************************************************************************************************
Description:
	Set sepia.
Parameters:
	pOut: Output the databuf after set partial color(data is yuv type).
	pIn : Input the databuf before set partial color.
	pinsize:  The image size of input data.	
	colortype:  Set the type of color.(Red: 0  ;    Green: 1    ;      Blue:2 )
	offset:     The value will add to pointed colortype.
Returns:
	If success, return 1, else,return 0.
Remarks:
	NULL
***************************************************************************************************/
UINT8 V558_GpSetSepia(UINT8* pOut, UINT8* pIn, PV558_SIZE pinsize, UINT8 r, UINT8 g, UINT8 b)
{
	UINT16 i,j,cxlength;
	UINT32 len,srcadd,tempsrcadd;
	YUVType jpg0,jpg1;
	RGBType rgb0,rgb1;
	
	cxlength    =   (pinsize->cx<<1);
	srcadd      =   0;
	for(i = 0;i<pinsize->cy ;i++)
	{
		srcadd  +=  cxlength;
		for(j=0;j<(pinsize->cx>>1);j++)
		{
			tempsrcadd  = srcadd  + (j<<2);
			//get yuv data
			jpg0.y  = *(pIn+tempsrcadd  );
			jpg1.y  = *(pIn+tempsrcadd+1);
			jpg0.u  = *(pIn+tempsrcadd+2);
			jpg1.u  = jpg0.u;
			jpg0.v  = *(pIn+tempsrcadd+3);
			jpg1.v  = jpg0.v;
			//change from yuv to rgb
			YUV2RGB(&rgb0,&jpg0);
			SetPartialRgb(&rgb0  , r, g, b);
			YUV2RGB(&rgb1,&jpg1);
			SetPartialRgb(&rgb1 , r, g, b);
			RGB2YUV(&jpg0, &rgb0);
			RGB2YUV(&jpg1, &rgb1);
			*(pOut +tempsrcadd ) = jpg0.y;
			*(pOut +tempsrcadd+1) = jpg1.y;
			*(pOut +tempsrcadd+2) = jpg0.u;
			*(pOut +tempsrcadd+3) = jpg1.v;			
		}
	}
	len   =  pinsize->cx * pinsize->cy *2;

	return 1;
}
/************************************************************************************************
Description:
	Set frame to the image.
Parameters:
	pOut: Output the databuf after set frame.
	pIn : Input the databuf before set frame.
	pInFrame : The input frame buffer,the data format is 1:V558_RGB_565 or 2: V558_RGB_555
	pinsize:  The image size of input data.	
	keycolor    : If the data value of iInFrameBuf is keycolor,it will be changed by the yuvimagbuf.
Returns:
	If success, return 1, else , return 0.
Remarks:
	NULL
*************************************************************************************************/
UINT8 V558_GpSetFrame(UINT8* pOut, UINT8* pIn, UINT8* pInFrame , PV558_SIZE pinsize , UINT8 keycolor)
{
	UINT8  value;
	UINT16 i,k,cxlength;
	UINT32 srcadd,tempsrcadd;
	
	cxlength  =   (pinsize->cx<<1);
	srcadd    =   0;
	//V558_YuvToRaw(pbuf ,pIn, gGpContext.outrgbtype ,gGpContext.inyuvtype,*pinsize,1);
	for( k=0 ; k<pinsize->cy ; k++ )
	{
		srcadd  += cxlength;
		for( i=0 ; i<(pinsize->cx<<1) ; i++ )
		{
			tempsrcadd  =  srcadd + i;
			value   =   *(pInFrame + tempsrcadd);
			if(value == keycolor)
				value = *(pIn + tempsrcadd);
			*(pOut + tempsrcadd)  = value;
		}
	}
	return 1;	
}

/************************************************************************************
Description:
	change rgb to yuv.
Parameters:
	pOut  £»    The output data buffer.
	pIn   :     The input data buffer.
	size  :     The image size.
Returns:
	The length of output data.
Remarks:
	NULL
**************************************************************************************/
UINT32 V558_GpRgbToYuv(UINT8* pOut,UINT8* pIn,V558_SIZE size) 
{
	UINT16  i,j,cxlength;
	UINT32  len,srcadd,dstadd,tempsrcadd,tempdstadd;
	YUVType jpg0,jpg1;
	RGBType rgb;
	
	cxlength   =   (size.cx<<1);
	srcadd     =    0;
	dstadd     =    0;
	for(i = 0;i<size.cy ;i++)
	{
		srcadd    += (cxlength + size.cx);
		dstadd    +=  cxlength;
		for(j=0;j<(size.cx>>1);j++)
		{
			tempsrcadd  =  srcadd + (((j<<1) +j)<<1);
			tempdstadd  =  dstadd + (j<<2);
			//change from yuv to rgb
			rgb.b      =   *(pIn + tempsrcadd   );
			rgb.g      =   *(pIn + tempsrcadd +1);
			rgb.r      =   *(pIn + tempsrcadd +2);
			RGB2YUV(&jpg0,&rgb);
			rgb.b      =   *(pIn + tempsrcadd +3);
			rgb.g      =   *(pIn + tempsrcadd +4);
			rgb.r      =   *(pIn + tempsrcadd +5);
			RGB2YUV(&jpg1,&rgb);
			//get yuv data
			*(pOut + tempdstadd   )  =  jpg0.y;
			*(pOut + tempdstadd +1)  =  jpg1.y;
			*(pOut + tempdstadd +2)  =  jpg0.u;
			*(pOut + tempdstadd +3)  =  jpg1.v;
		}
	}
	len  =  (size.cx * cxlength);
	return  len;
}

/******************************************************************************************
Description:
	Set partial color.
Parameters:
	pOut     :   The output buffer of RGB type
	colortype:   The type of color.(0:blue ; 1:green ; 2:red)
	offset   :   The value will add to pointed colortype.	
Returns:
	NULL
Remarks:
	NULL
*******************************************************************************************/
void   SetPartialRgb(RGBType* rgb , UINT8 r, UINT8 g, UINT8 b)
{
	if(r)
		rgb->r  = 0xff;
	if(g)
		rgb->g  = 0xff;
	if(b)
				rgb->b = 0xff;
	
}