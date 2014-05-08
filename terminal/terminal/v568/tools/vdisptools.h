#ifndef _VDISPTOOLS_H_
#define _VDISPTOOLS_H_

#ifdef __cplusplus
extern "C" {
#endif

	typedef struct tag_TBmpInfo {
		TSize	size;
		UINT8	fmt;
		UINT16	palsize;
		UINT8	*ppal;
		UINT8	*pbuf;
	} TBmpInfo, *PTBmpInfo;

	UINT32	GetBmemSize(UINT8 fmt, TSize size);
	UINT8	ParseBmpData(UINT8 *pbuf, PTBmpInfo info);
	UINT16	GetGeSpan(UINT8 fmt, UINT16 width);
	UINT8	GetBitbltDir(TPoint srcp, TPoint dstp, TSize size);
	UINT8	GetDeInterVal(UINT16 wcy);
	UINT16	Convert24To555(UINT32 color);
	UINT16	Convert24To565(UINT32 color);
	UINT32	Convert555To24(UINT16 color);
	UINT32	Convert565To24(UINT16 color);

#ifdef __cplusplus
}
#endif

#endif



