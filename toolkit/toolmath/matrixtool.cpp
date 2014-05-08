
#include "internal.h"

// class CBaseMat
int	CBaseMat::RevOrder(int *ary, int n)
{
	int i, j, val = 1;

	if(n <= 1)
		return 1;
	for(i = 0; i < n-1; i++)
	{
		for(j = i; j < n; j++)
		{
			if(ary[i] > ary[j])
				val = 0 - val;
		}
	}
	return val;
}

void CBaseMat::PrintMatrix(int *blk, int height, int width)
{
	int i, j;
	CFileBin fb;

	fb.Open(MATRIX_FNAME, "at");
	fb.Printf("\n==========\ntable size: %d X %d\n", height, width);
	for(i = 0; i < height; i++)
	{
		for(j = 0; j < width; j++)
			fb.Printf("%d, ", (blk[i * width + j]));
		fb.Printf("\n");
	}
}

void CBaseMat::PrintMatrix(double *blk, int height, int width)
{
	int i, j;
	CFileBin fb;

	fb.Open(MATRIX_FNAME, "at");
	fb.Printf("\n==========\ntable size: %d X %d\n", height, width);
	for(i = 0; i < height; i++)
	{
		for(j = 0; j < width; j++)
			fb.Printf("%lf, ", (blk[i * width + j]));
		fb.Printf("\n");
	}
}

