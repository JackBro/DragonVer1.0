

#include "internal.h"

static void Rgb2Yuv(int *rgb, int *yuv, int up, int low)
{
	yuv[0] = (77 * rgb[0] + 150 * rgb[1] + 29 * rgb[2]) >> 8;	//y
	yuv[1] = ((-43 * rgb[0] - 85 * rgb[1] + 128 * rgb[2]) >> 8) + 128; //u
	yuv[2] = ((128 * rgb[0] - 107 * rgb[1] - 21 * rgb[2]) >> 8) + 128; //v

	yuv[0] = (yuv[0] > up) ? up : ((yuv[0] < low) ? low : yuv[0]);
	yuv[1] = (yuv[1] > up) ? up : ((yuv[1] < low) ? low : yuv[1]);
	yuv[2] = (yuv[2] > up) ? up : ((yuv[2] < low) ? low : yuv[2]);
}

static void Yuv2Rgb(int *yuv, int *rgb, int up, int low)
{
	rgb[0] = yuv[0] + ( (359 * yuv[2]) >> 8 ) - 179;		//r
	//rgb[0] = (359 * yuv[2]) >> 8;
	//rgb[0] += yuv[0] + 76;
	//if(rgb[0] > 0x1ff)
	//	rgb[0] -= 0x1ff;
	//rgb[0] -= 179;
   	rgb[1] = yuv[0] +   135 - ( (88 * yuv[1] + 183 * yuv[2]) >> 8 );	//g
   	rgb[2] = yuv[0] + ( (454 * yuv[1]) >> 8 ) - 227;	//b

	rgb[0] = (rgb[0] > up) ? up : ((rgb[0] < low) ? low : rgb[0]);
	rgb[1] = (rgb[1] > up) ? up : ((rgb[1] < low) ? low : rgb[1]);
	rgb[2] = (rgb[2] > up) ? up : ((rgb[2] < low) ? low : rgb[2]);
}


void TestColorConvert(void)
{
	int rgb[3], yuv[3], diff[3];
	int r, g, b;
	//char str[100];
	CFileBin fb;

	fb.Open("a.txt", "wb");
	for(r = 0; r < 256; r++)
	{
		for(g = 0; g < 256; g++)
		{
			for(b = 0; b < 256; b++)
			{
				rgb[0] = r;
				rgb[1] = g;
				rgb[2] = b;
				Rgb2Yuv(rgb, yuv, 500, -500);
				yuv[0] &= 0xff;
				yuv[1] &= 0xff;
				yuv[2] &= 0xff;
				Yuv2Rgb(yuv, rgb, 500, -500);

				if(rgb[0] > r)
					diff[0] = rgb[0] - r;
				else
					diff[0] = r - rgb[0];

				if(rgb[1] > g)
					diff[1] = rgb[1] - g;
				else
					diff[1] = g - rgb[1];

				if(rgb[2] > b)
					diff[2] = rgb[2] - b;
				else
					diff[2] = b - rgb[2];

				if( (diff[0] > 4) || (diff[1] > 4) || (diff[2] > 4) )
				{
					//sprintf(str, "=============================\n");
					//fb.Write(str, (int)strlen(str));
					//sprintf(str, "r = %d, g, %d, b = %d\n", r, g, b);
					//fb.Write(str, (int)strlen(str));
					//sprintf(str, "y = %d, u = %d, v = %d\n", yuv[0], yuv[1], yuv[2]);
					//fb.Write(str, (int)strlen(str));
					//sprintf(str, "r = %d, g = %d, b = %d\n", rgb[0], rgb[1], rgb[2]);
					//fb.Write(str, (int)strlen(str));
				}
			}
		}
	}
}

void ColorMat(int *in, int *out)
{
	UINT8 mat[][3] =
	{
		{ 0x60, 0xde, 0x9 },
		{ 0xf1, 0x5f, 0xf4 },
		{ 0xf1, 0xe5, 0x68 }
	};
	int tem[3];
	int i;

	for(i = 0; i < 3; i++)
	{
		tem[0] = (int)mat[i][0];
		tem[1] = (int)mat[i][1];
		tem[2] = (int)mat[i][2];

		out[i] = tem[0] * in[0] + tem[1] * in[1] + tem[2] * in[2];
		out[i] >>= 6;
	}
}

void TestColorMatrix(void)
{
}
