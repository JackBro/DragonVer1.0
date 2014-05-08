
#ifndef _TOOLS_JPEGBASE_H_
#define _TOOLS_JPEGBASE_H_


#ifndef TOOLJPG_DLL
	#define TOOLJPG_DLL  __declspec(dllimport)
#endif

#include "tools/global.h"
#include "jmmatrix.h"
#include "jpegtable.h"

enum BLOCKDECenum
{
	JPEG_YBLOCK,
	JPEG_UVBLOCK
};

typedef struct tag_TJpegTable 
{
//	int	sfblk[64];			/* DCT scale factor table */
//	int	invTable[256];		/* inv ROM table */
	int	*sfblk;
	int	*invTable;
	int	LQTable[64];		/* Lum quantization tables */
	int	CQTable[64];		/* Chrom quantization tables */

	int	sftL[128];			/* Lum shift tables (compression or decompression) */
	int	sftC[128];			/* Chrom shift tables (compression or decompression) */
	int	brcL[64];			/* Lum BRC tables (compression or decompression) */
	int	brcC[64];			/* Chrom BRC tables (compression or decompression) */

	THuffValue Ldc[260];		/* Huffman encode table for Lum DC */	//12
	THuffValue Lac[260];		/* Huffman encode table for Lum AC */	//162
	THuffValue Cdc[260];		/* Huffman encode table for Chrom DC */	//12
	THuffValue Cac[260];		/* Huffman encode table for Chrom AC */	//162
} TJpegTable, *PTJpegTable;


#define DEFAULT_JPEGDATASIZE	0X20000

#define DEFAULT_JPEGINDEX_FILE	"jpgindex.txt"
#define DEFAULT_JPEGTABLE_FILE	"jpgtable.txt"

class TOOLJPG_DLL CJpegBase : public CJmMatrix
{
public:
	void	PrintJpegIndex(PTJpegIndex pinfo, char *filename = DEFAULT_JPEGINDEX_FILE);
	void	PrintJpegTable(PTJpegTable pinfo, char *filename = DEFAULT_JPEGTABLE_FILE);

protected:
	void	createHuffmanTable(UINT8 *bits, UINT8 *values, THuffValue *table);
	void	getInvTable(int *invTable);
	void	DCTscaleFactor(int *sfblk);
	void	getQTable(int *sfblk, int *LQTable, int *CQTable, int *invTable, int Qfactor);
};

#endif

