#ifndef _VMARBTOOLS_H_
#define _VMARBTOOLS_H_

#ifdef __cplusplus
extern "C" {
#endif

	UINT32 GetAmemSize(UINT8 fmt, TSize size);
	UINT32 GetDecAmemSize(UINT8 yuvfmt, TSize size);
	UINT32 GetYuvSize(UINT8 yuvfmt, TSize size);
	UINT32 GetRgbSize(TSize size);
	UINT32 GetDecLbuf0Size(UINT8 fmt, UINT16 width);
	UINT32 GetDecLbuf1Size(UINT8 fmt, UINT16 width);
	UINT32 GetCapLbuf0Size(UINT8 fmt, UINT16 width);

	UINT8 YuvmodeToYuvfmt(UINT8 mode);
	UINT8 YuvmodeToYuvLbuf(UINT8 mode);
	UINT8 YuvmodeToYuvBlk(UINT8 mode);

	UINT32 CalCapFrmSize(UINT8 fmt, TSize size, UINT8 tcr);

#ifdef __cplusplus
}
#endif

#endif



