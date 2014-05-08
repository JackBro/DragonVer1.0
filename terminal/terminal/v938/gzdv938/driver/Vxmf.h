
#ifndef __VXMF_H__

#define __VXMF_H__

#define MAX_MIDITRACK		32
#define TEMPO_CODE			500000		// 120 bpm

#define MAX_MIDI_CHN		16
#define MELTA_TEMPO		0x51
#define MELTA_EVENT			0xff
#define MSG_KEYON			0x90
#define MSG_KEYOFF			0x80
#define MSG_PROGCHANGE	0xc0
#define MSG_SYSEXCLUDE		0xF0
#define MSG_PITCHBEND		0xe0
#define MSG_CTRLCHANGE		0xb0
#define MSG_CHNPRESSURE	0xd0

#define CTRL_MODULATION	0x01
#define CTRL_DATAENTRYMSB	0x06
#define CTRL_DATAENTRYLSB	0x26
#define CTRL_VOLUME			0x07
#define CTRL_PANPOT			0x0a
#define CTRL_EXPRESS		0x0b	
#define CTRL_HOLD1			0x40
#define CTRL_RESETCTRLS		0x79
#define CTRL_ALLNOTEOFF	0x7b
#define CTRL_RPNLSB			0x64
#define CTRL_RPNMSB			0x65

#define MAX_ENTRY_NUM            64
#define MAX_ITEM_NUM             64
#define MAX_SMF_NUM              64

VRESULT VXmf_Init			(void);
VRESULT VXmf_Load			(HUGE UINT8* pbFile, UINT32 dLength, UINT32 dEmptyRes, UINT8 bSrcFlag);
VRESULT	VXmf_Seek		(UINT32 dPos, VDEV_FIFO_TYPE FiFoType);
VRESULT VXmf_GetEvent		(PVM_UNI_MSG pUniMsg, VDEV_FIFO_TYPE FiFoType);
VRESULT VXmf_GetFileInfo		(PVOID pFileInfo, VDEV_FIFO_TYPE FiFoType);
VRESULT VXmf_GetCurPos		(PUINT32 pdCurTime, VDEV_FIFO_TYPE FiFoType);
VRESULT VXmf_Reset 			(VDEV_FIFO_TYPE FiFoType);


typedef struct _XMFMIDIFILETRACK
{
	HUGE UINT8	*pDataHead;
	HUGE UINT8 	*pCurnData;
	HUGE UINT8 	*pDataEnd;
	UINT32	dwNextTick;
	UINT8 	preStatus;
}XMFMIDIFILETRACK;

typedef struct _TYPEENTRY
{
	UINT8	chMetaDataType; 
	UINT8   chStringFormatID;
	char    chLangCountryName[10];
}TYPEENTRY, *PTYPEENTRY;

/*
typedef struct _METADATATYPETABLE
{
	UINT32	lSize; 
	UINT8   chEntryNum;
	TYPEENTRY typeEntry[MAX_ENTRY_NUM];
}METADATATYPETABLE, *PMETADATATYPETABLE;
*/
///////////////////////////////////////////////////////////////////
typedef struct _NODECONTENT
{
	UINT8 chRefTypeID;
	HUGE  UINT8* pData;
}NODECONTENT, *PNODECONTENT;

typedef struct _CONTENTVER
{
	UINT8	chMetaDataType;
	UINT32	lLen;
	char	*strVDContent;
}CONTENTVER, *PCONTENTVER;

typedef struct _UNIVERSALCONTENT
{
	UINT8 ch;
	UINT32 fieldContent;
}UNIVERSALCONTENT, *PUNIVERSALCONTENT;

typedef struct _INTERCONTENT
{
	UINT8 chVersionNum;
	UINT32 lLen;
	CONTENTVER contentVer[MAX_ITEM_NUM];
}INTERCONTENT, *PINTERCONTENT;

typedef struct _UNPACKERENTRY
{
	UINT32 unpackerID;
	UINT32 lDecodedSize;
}UNPACKERENTRY, *PUNPACKERENTRY;
/*
typedef struct _METADATAITEM
{
	UINT32 fieldSpe;
	INTERCONTENT fieldContent;
//	FIELDCONTENT fieldContent;
}METADATAITEM, *PMETADATAITEM;

typedef struct _NODEUNPACKER
{
	UINT32	lNodeUpackerLen; 
	UNPACKERENTRY unparkerEntry[MAX_ITEM_NUM];
}NODEUNPACKER, *PNODEUNPACKER;

typedef struct _NODEMETADATA
{
	UINT32	lNodeMetaDataLen; 
	METADATAITEM metaDataItem[MAX_ITEM_NUM];
}NODEMETADATA, *PNODEMETADATA;
*/
typedef struct _NODEHEADER
{
	UINT32	lNodeLen; 
	UINT32   chNodeContainedItems;
	UINT32  lNodeHeaderLen;
//	NODEMETADATA nodeMetaData[MAX_ITEM_NUM];
//	NODEUNPACKER nodeUnpacker[MAX_ITEM_NUM];
}NODEHEADER, *PNODEHEADER;

typedef struct _NODE
{
	NODEHEADER	nodeHeader; 
	NODECONTENT       nodeContent;
	struct _NODE *pNext;
	struct _NODE *pPrev;
	struct _NODE *pParent;
	struct _NODE *pChild;
}NODE, *PNODE;

typedef struct _FILEHEADER
{
	UINT32   fileID;
	UINT32   fileVersion;
	UINT32	 lFileLen; 
	UINT32   lTreeStart;
	UINT32   lTreeEnd;
}FILEHEADER, *PFILEHEADER;

typedef struct _XMFDATA
{
	HUGE UINT8	*pXmfData; 
	UINT32  dwXmfDataLen;
	UINT8   chXmfCompTypeID;
	UINT8   chXmfEncryptType;
	HUGE UINT8	*pDlsData; 
	UINT32  dwlsDataLen;
	UINT8   chDlsCompTypeID;
	UINT8   chDlsEncryptType;
}XMFDATA, *PXMFDATA;

#endif /*__VXMF_H__*/











































