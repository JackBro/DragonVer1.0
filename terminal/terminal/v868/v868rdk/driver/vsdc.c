 
#include "internal.h"
#ifndef	V868CMD_DISABLE


static TSdcContext gSdcContext;
static TSdcInfo gSdcInfo = { 0, 0xc, 200, 0, 0x200, 0, 0x001f0000 };

static const UINT8 cmdPre[ ] = {0, 0, 0, 0, 0, 0, 0, 0, 
                                0, 0, 0, 0, 0, 0, 0, 0,
								1, 1, 0, 0, 0, 0, 0, 1,
								1, 0, 1, 0, 0, 1, 0, 0,
								0, 0, 0, 0, 0, 0, 0, 0,
								0, 1, 0, 0, 0, 0, 0, 0,
								0, 0, 0, 0, 0, 0, 0, 1,
                                0, 0, 0, 0, 0, 0, 0 ,0 };

static const UINT8 AcmdPre[ ] = {0, 0, 0, 0, 0, 0, 0, 0, 
                                 0, 0, 0, 0, 1, 0, 0, 0,
								 0, 0, 0, 0, 0, 1, 0, 0,
								 0, 0, 0, 0, 0, 0, 0, 0,
								 0, 0, 0, 0, 0, 0, 0, 0,
								 0, 0, 0, 0, 0, 0, 0, 0,
								 0, 0, 1, 0, 0, 0, 0, 0,
                                 0, 0, 0, 0, 0, 0, 0 ,0 };

static const UINT8 cmdDir[ ] = {0, 0, 0, 0, 0, 0, 0, 0, 
                                0, 0, 0, 0, 0, 0, 0, 0,
								1, 1, 0, 0, 0, 0, 0, 0,
								0, 0, 0, 0, 0, 1, 0, 0,
								0, 0, 0, 0, 0, 0, 0, 0,
								0, 0, 0, 0, 0, 0, 0, 0,
								0, 0, 0, 0, 0, 0, 0, 0,
                                0, 0, 0, 0, 0, 0, 0 ,0 };

static const UINT8 AcmdDir[ ] = {0, 0, 0, 0, 0, 0, 0, 0, 
                                 0, 0, 0, 0, 1, 0, 0, 0,
								 0, 0, 0, 0, 0, 1, 0, 0,
								 0, 0, 0, 0, 0, 0, 0, 0,
								 0, 0, 0, 0, 0, 0, 0, 0,
								 0, 0, 0, 0, 0, 0, 0, 0,
								 0, 0, 1, 0, 0, 0, 0, 0,
                                 0, 0, 0, 0, 0, 0, 0 ,0 };

static const UINT8 cmdRsp[ ] = {0, 1, 2, 0, 0, 0, 3, 0, 
                                1, 1, 0, 3, 2, 0, 0, 2,
								2, 2, 0, 0, 0, 0, 0, 2,
								2, 0, 2, 3, 3, 2, 0, 2,
								2, 2, 2, 2, 2, 3, 0, 0,
								0, 2, 0, 0, 0, 0, 0, 0,
								0, 0, 0, 0, 0, 0, 2, 2,
                                0, 0, 0, 0, 0, 0, 0 ,0 };

static const UINT8 AcmdRsp[ ] = {2, 0, 0, 0, 0, 2, 0, 0, 
                                 0, 0, 0, 0, 2, 0, 0, 0,
								 0, 2, 0, 0, 0, 2, 2, 0,
								 2, 2, 0, 0, 0, 0, 0, 0,
								 0, 0, 0, 0, 0, 3, 0, 0,
								 2, 2, 2, 2, 2, 2, 2, 2,
								 3, 0, 2, 0, 0, 0, 0, 0,
                                 0, 0, 0, 0, 0, 0, 0 ,0 };


void V8_SdcSetInfo(TSdcInfo *pInfo)
{
	gSdcContext.pSdcInfo = pInfo;
	gSdcContext.state = SDC_INITED;
}

void V8_SdcInitContext(void)
{
	memset(&gSdcContext, 0, sizeof(TSdcContext));
	gSdcContext.state = SDC_NOTINIT;
}

TSdcContext *V8_SdcGetContext(void)
{
	return &gSdcContext;
}

void V8_SdcInitTransSta( UINT8 stoOrMMC )
{
	UINT8 cmdRsp[16] ={ 0 };
	UINT8 busyEable = 0x0;
	UINT8 delayCnt = 0x4;

	/*send command 0*/
    V8_SdcSendCmd(0, 0);

	/*send ack command until the highest bit of response register is 1*/
	while( !busyEable && delayCnt )
	{
		if( stoOrMMC )
		{
		    V8_SdcSendACmd( 41, gSdcContext.pSdcInfo->ocr );
		}
		else
		{
			V8_SdcSendCmd( 1, gSdcContext.pSdcInfo->ocr );
		}

        V8B_SdifGetResp( cmdRsp );
		busyEable = ( cmdRsp[15] & 0x80 ) >> 7;	

		delayCnt--;
	}

	if( !delayCnt )
	{
        V8B_SdifSoftReset(SD_REG_RST | SD_CTRL_RST | SD_BUF_CLR);
	}

	/*send command 2*/
    V8_SdcSendCmd(2, 0);

	/*send command 3 and get cra from response*/
    V8_SdcSendCmd(3, 0);
	V8B_SdifGetResp( cmdRsp );
	gSdcContext.pSdcInfo->rca = ( ( (UINT16)cmdRsp[15] ) << 8 ) | ( (UINT16)cmdRsp[14] );

	/*change clock divder as 10*/
	gSdcContext.pSdcInfo->clkDivCnt = 10;
	V8B_SdifSetClkCtrl(gSdcContext.pSdcInfo->clkDivCnt);

 	/*send command 7*/
    V8_SdcSendCmd( 7, ( (UINT32)gSdcContext.pSdcInfo->rca ) << 16 );

}

void V8_SdcOpen( UINT8 stoOrMMC )
{
	V8B_SdifSoftReset(SD_REG_RST | SD_CTRL_RST | SD_BUF_CLR);

	V8_SdcInitContext( );

	V8_SdcSetInfo(&gSdcInfo);

	/*set clock divider is 200*/
	V8B_SdifSetClkCtrl(gSdcContext.pSdcInfo->clkDivCnt);

	/*set trans mod*/
	V8B_SdifSetStoMode(gSdcContext.pSdcInfo->stoMode);

	/*set user defined enable command option*/
	V8B_SdifSetCmdCtrl1(gSdcContext.pSdcInfo->option);

	/*set block size*/
	V8B_SdifSetBlkSize(gSdcContext.pSdcInfo->blkSize - 1);

	V8_SdcInitTransSta( stoOrMMC );

	gSdcContext.state = SDC_OPENED;
}

void V8_SdcClose(void)
{
	gSdcContext.state = SDC_CLOSE;
}

//////////////////////////////////////////////////////////


UINT8 V8_SdcWaitIdle(void)
{
	UINT16 delayCnt = VSDC_WAIT_IDLE_DELAY_COUNT;

	while( ( ( V8B_SdifGetDmaCtrl() & 0x3c ) >> 0x2 ) != SD_CTRL_IDEL && 
		   ( delayCnt-- ) );

	if( delayCnt <= 0 )
	{
		V8B_SdifSoftReset( SD_REG_RST | SD_CTRL_RST );

		return VSDC_WAIT_IDLE_TIMEOUT;
	}

	return VSDC_NORMAL_COMPLETE;
}

UINT8 V8_SdcWaitResp(void)
{
	UINT16 delayCnt = VSDC_WAIT_RESPONSE_DELAY_COUNT;

	while( ( V8B_SdifGetNorIntStatus() & SD_CMDCOMPLETE ) == 0x0 && 
		   ( delayCnt-- ) );

	if( delayCnt <= 0 )
	{
		return VSDC_WAIT_RESPONSE_TIMEOUT;
	}

	return VSDC_NORMAL_COMPLETE;
}

UINT8 V8_SdcWaitBufWriteEn(void)
{
	UINT16 delayCnt = VSDC_WAIT_BUFWRITEEN_DELAY_COUNT;

	while( ( ( V8B_SdifGetPresentState() & SD_BUFWR_EN ) >> 0x7 ) != VSDC_BUFFER_WRITE_ENABLE && 
		   ( delayCnt-- ) );

	if( delayCnt <= 0 )
	{
		return VSDC_WAIT_BUFFERWRITEEN_TIMEOUT;
	}

	return VSDC_NORMAL_COMPLETE;
}

UINT8 V8_SdcWaitBufReadEn(void)
{
	UINT16 delayCnt = VSDC_WAIT_BUFREADEN_DELAY_COUNT;

	while( ( ( V8B_SdifGetPresentState() & SD_BUFRD_EN ) >> 0x8 ) != VSDC_BUFFER_READ_ENABLE && 
		   ( delayCnt-- ) );

	if( delayCnt <= 0 )
	{
		return VSDC_WAIT_BUFFERREADEN_TIMEOUT;
	}

	return VSDC_NORMAL_COMPLETE;
}

void V8_SdcSetCmdOptByIdx( UINT8 cmdIdx )
{
    /*set data present option*/
	if( cmdPre[cmdIdx] )
	{
		V8B_SdifEnableDataPresent( );
	}
	else
	{
		V8B_SdifDisableDataPresent( );
	}

	/*set data direction option*/
	if( cmdDir[cmdIdx] )
	{
		V8B_SdifEnableReadDirc( );
	}
	else
	{
        V8B_SdifDisableReadDirc( );
	}

	/*set response type*/
	V8B_SdifSetRespType( cmdRsp[cmdIdx] );
}

void V8_SdcSetACmdOptByIdx( UINT8 cmdIdx )
{
    /*set data present option*/
	if( AcmdPre[cmdIdx] )
	{
		V8B_SdifEnableDataPresent( );
	}
	else
	{
		V8B_SdifDisableDataPresent( );
	}

	/*set data direction option*/
	if( AcmdDir[cmdIdx] )
	{
		V8B_SdifEnableReadDirc( );
	}
	else
	{
        V8B_SdifDisableReadDirc( );
	}

	/*set response type*/
	V8B_SdifSetRespType( AcmdRsp[cmdIdx] );
}

UINT8 V8_SdcSendCmd( UINT8 cmdIdx, UINT32 cmdArgu )
{

	/*be sure the state of SDIF is idle*/
	if( V8_SdcWaitIdle( ) == VSDC_WAIT_IDLE_TIMEOUT )
	{
		V8B_SdifSoftReset( SD_REG_RST | SD_CTRL_RST );

		return VSDC_WAIT_IDLE_TIMEOUT;
	}
    
    /*set command argument*/
	V8B_SdifSetCmdArgu( cmdArgu );

	/*set command options determined by command index*/
	V8_SdcSetCmdOptByIdx( cmdIdx );

	/*send command*/
	V8B_SdifSetCmdIndex( cmdIdx );

    /*wait response*/
	if( V8_SdcWaitResp( ) == VSDC_WAIT_RESPONSE_TIMEOUT )
	{
		return VSDC_WAIT_RESPONSE_TIMEOUT;
	}

	/*check crc status*/
	if( ( V8B_SdifGetErr2IntStatus( ) & SD_HOSTRESP_STATUS ) == 0x1 )
	{
		return VSDC_CRC_ERROR;
	}

	return VSDC_NORMAL_COMPLETE;
}

UINT8 V8_SdcSendACmd( UINT8 cmdIdx, UINT32 cmdArgu )
{
	UINT8 rtnVal = 0;

	/*send command 55 firstly*/
	rtnVal = V8_SdcSendCmd( 55, 0 );
	if( rtnVal != VSDC_NORMAL_COMPLETE )
	{
		return rtnVal;
	}

	/*be sure the state of SDIF is idle*/
	if( V8_SdcWaitIdle( ) == VSDC_WAIT_IDLE_TIMEOUT )
	{
		V8B_SdifSoftReset( SD_REG_RST | SD_CTRL_RST );

		return VSDC_WAIT_IDLE_TIMEOUT;
	}
    
    /*set command argument*/
	V8B_SdifSetCmdArgu( cmdArgu );

	/*set command options determined by command index*/
	V8_SdcSetACmdOptByIdx( cmdIdx );

	/*send command*/
	V8B_SdifSetCmdIndex( cmdIdx );

    /*wait response*/
	if( V8_SdcWaitResp( ) == VSDC_WAIT_RESPONSE_TIMEOUT )
	{
		return VSDC_WAIT_RESPONSE_TIMEOUT;
	}

	/*check crc status*/
	if( ( V8B_SdifGetErr2IntStatus( ) & SD_HOSTRESP_STATUS ) == 0x1 )
	{
		return VSDC_CRC_ERROR;
	}

	return VSDC_NORMAL_COMPLETE;
}

UINT8 V8_SdcHostWriteOneWord( UINT32 wrdata )
{
    /*be sure buffer write enable*/
	if( V8_SdcWaitBufWriteEn( ) == VSDC_WAIT_BUFFERWRITEEN_TIMEOUT )
	{
	    return VSDC_WAIT_BUFFERWRITEEN_TIMEOUT;
	}

    /*write 32bit data to buffer*/
	V8B_SdifWrPort( wrdata );

	return VSDC_NORMAL_COMPLETE;
}

UINT8 V8_SdcHostReadOneWord( UINT32 *pwrdata )
{
	UINT32 *locpwrdata = pwrdata;
	UINT16  delayCnt = VSDC_WAIT_HOSTBUFFERREADCLEAR_DELAY_COUNT; 

    /*be sure buffer read enable*/
	if( V8_SdcWaitBufReadEn( ) == VSDC_WAIT_BUFFERREADEN_TIMEOUT )
	{
	    return VSDC_WAIT_BUFFERREADEN_TIMEOUT;
	}

	/*set the bit host_buf_rd of soft reset register*/
	V8B_SdifSetHostBufRd( );

	/*wait hardware clear host_buf_rd bit*/
	while( V8B_SdifGetHostBufRd( ) && delayCnt-- );

    /*read 32bit data from buffer*/
	*locpwrdata = V8B_SdifRdPort( );

	return VSDC_NORMAL_COMPLETE;
}

UINT8 V8_SdcHostWriteOneBlk( UINT16 blkSize, UINT32 *pwrdata )
{
	UINT32  *plocwrdata = pwrdata;
    UINT16  locblkSize = blkSize;
	UINT16  i = 0;

	/*be sure the size of one data block is 4 bytes align*/
	i = locblkSize & 0x3;
    locblkSize = locblkSize >> 0x2;
	locblkSize = locblkSize << 0x2;
	if(i)
	{
        locblkSize += 4;
	}

	while( locblkSize )
	{
        if( ( V8_SdcHostWriteOneWord( *plocwrdata ) ) == VSDC_WAIT_BUFFERWRITEEN_TIMEOUT )
		{
            return VSDC_WAIT_BUFFERWRITEEN_TIMEOUT;
		}

        plocwrdata++;
		locblkSize -= 4;
	}

    return VSDC_NORMAL_COMPLETE;
}

UINT8 V8_SdcHostReadOneBlk( UINT16 blkSize, UINT32 *prddata )
{
    UINT32  *plocrddata = prddata;
    UINT16  locblkSize = blkSize;
	UINT16  i = 0;

	/*be sure the size of one data block is 4 bytes align*/
	i = locblkSize & 0x3;
    locblkSize = locblkSize >> 0x2;
	locblkSize = locblkSize << 0x2;
	if(i)
	{
        locblkSize += 4;
	}

	while( locblkSize )
	{
        if( ( V8_SdcHostReadOneWord( plocrddata ) ) == VSDC_WAIT_BUFFERREADEN_TIMEOUT )
		{
            return VSDC_WAIT_BUFFERREADEN_TIMEOUT;
		}

        plocrddata++;
		locblkSize -= 4;
	}

    return VSDC_NORMAL_COMPLETE;
}

UINT8 V8_SdcHostWriteMulBlk( UINT16 blkCnt, UINT16 blkSize, UINT32 *pwrdata )
{
    UINT32  *plocwrdata = pwrdata;
	UINT16  locblkSize = blkSize;
	UINT16  locblkCnt = blkCnt;
	UINT16  i = 0;

	/*be sure the size of one data block is 4 bytes align*/
	i = locblkSize & 0x3;
    locblkSize = locblkSize >> 0x2;
	locblkSize = locblkSize << 0x2;
	if(i)
	{
        locblkSize += 4;
	}

    /*write data block*/
	while( locblkCnt )
	{
        if( ( V8_SdcHostWriteOneBlk( blkSize, plocwrdata ) ) == VSDC_WAIT_BUFFERWRITEEN_TIMEOUT )
		{
            return VSDC_WAIT_BUFFERWRITEEN_TIMEOUT;
		}

		locblkCnt--;
		plocwrdata += locblkSize >> 0x2; 
	}

	return VSDC_NORMAL_COMPLETE;
}

UINT8 V8_SdcHostReadMulBlk( UINT16 blkCnt, UINT16 blkSize, UINT32 *prddata)
{
	UINT32  *plocrddata = prddata;
	UINT16  locblkSize = blkSize;
	UINT16  locblkCnt = blkCnt;
	UINT16  i = 0;

	/*be sure the size of one data block is 4 bytes align*/
	i = locblkSize & 0x3;
    locblkSize = locblkSize >> 0x2;
	locblkSize = locblkSize << 0x2;
	if(i)
	{
        locblkSize += 4;
	}

    /*read data block*/
	while( locblkCnt )
	{
        if( ( V8_SdcHostReadOneBlk( locblkSize, plocrddata ) ) == VSDC_WAIT_BUFFERREADEN_TIMEOUT )
		{
            return VSDC_WAIT_BUFFERREADEN_TIMEOUT;
		}

		locblkCnt--;   
		plocrddata += locblkSize >> 0x2; 
	}

	return VSDC_NORMAL_COMPLETE;
}

UINT8 V8_SdcCalBlkSizeAndCnt( UINT32 dataSize, UINT16 blckSize, UINT16 *pblkSizeAndCnt )
{
    UINT16  locblkSize = blckSize;
	UINT16  locblkCnt = 0;
	UINT16  i;

    
	if(locblkSize <= 0 || dataSize <= 0)
	{
		return 0;
	}

	/*be sure the size of one data block is 4 bytes align*/
	i = locblkSize & 0x3;
    locblkSize = locblkSize >> 0x2;
	locblkSize = locblkSize << 0x2;
	if(i)
	{
        locblkSize += 4;
	}

	/*calculate block count*/
	locblkCnt = (UINT16)( dataSize / locblkSize ); 
	i = (UINT16)( dataSize % locblkSize ); 
	if(i)
	{
        locblkCnt++;
	}

	pblkSizeAndCnt[0] = locblkSize;
    pblkSizeAndCnt[1] = locblkCnt;

	return 1;
}

UINT32 V8_SdcHostWriteData( UINT32 addr, UINT8 *buf, UINT32 size )
{
    UINT32  *plocwrdata = (UINT32*)buf;
	UINT16  locBlkSizeAndCnt[2] = {0};
	UINT16  locBlkSize = gSdcContext.pSdcInfo->blkSize;
	UINT8    rtnVal;


	if( 0 == V8_SdcCalBlkSizeAndCnt( size, locBlkSize, locBlkSizeAndCnt ) )
	{
		return 0;
	}

	/*clear buffer*/
	V8B_SdifBufClear( );

	/*set block count*/
	V8B_SdifSetBlkCnt( locBlkSizeAndCnt[1] );
   
	/*disable read direction*/
	V8B_SdifDisableReadDirc( );

	if( 1 == locBlkSizeAndCnt[1] )
	{
		/*write one block*/
		rtnVal = V8_SdcHostWriteOneBlk( locBlkSizeAndCnt[0], plocwrdata );
		if( VSDC_NORMAL_COMPLETE == rtnVal )
		{
            rtnVal = V8_SdcSendCmd(24, addr);
		}
	}
	else
	{
		/*write the first block*/
		rtnVal = V8_SdcHostWriteOneBlk( locBlkSizeAndCnt[0], plocwrdata );
		if( rtnVal != VSDC_NORMAL_COMPLETE )
		{
            return 0;
		}

		/*set AutoCommand12 bit of option*/
		V8B_SdifEnableAutoCmd12();

		/*send write multiplex block command*/
		rtnVal = V8_SdcSendCmd(25, addr); 

		/*write the left bolck*/
		plocwrdata += locBlkSizeAndCnt[0] >> 0x2;
		if( rtnVal == VSDC_NORMAL_COMPLETE )
		{
            rtnVal = V8_SdcHostWriteMulBlk( locBlkSizeAndCnt[1] - 1, locBlkSizeAndCnt[0], plocwrdata );
		}
	}

	/*be sure the state of SDIF is idle*/
	rtnVal = V8_SdcWaitIdle( );
	if( VSDC_WAIT_IDLE_TIMEOUT == rtnVal )
	{
		V8B_SdifSoftReset( SD_REG_RST | SD_CTRL_RST );
	}

	if( VSDC_NORMAL_COMPLETE != rtnVal)
	{
		return 0;
	}

	return locBlkSizeAndCnt[0] * locBlkSizeAndCnt[1];
}

UINT32 V8_SdcHostReadData( UINT32 addr, UINT8 *buf, UINT32 size )
{
    UINT32  *plocrddata = (UINT32*)buf;	
	UINT16  locBlkSizeAndCnt[2] = {0};
	UINT16  locBlkSize = gSdcContext.pSdcInfo->blkSize;
	UINT8    rtnVal;

    
	if( 0 == V8_SdcCalBlkSizeAndCnt( size, locBlkSize, locBlkSizeAndCnt ) )
	{
		return 0;
	}

	/*clear buffer*/
	V8B_SdifBufClear( );

	/*set block count*/
	V8B_SdifSetBlkCnt( locBlkSizeAndCnt[1] );

	if( 1 == locBlkSizeAndCnt[1] )
	{
		/*read one block*/
		rtnVal = V8_SdcSendCmd(17, addr);

		if( rtnVal == VSDC_NORMAL_COMPLETE )
		{
            rtnVal = V8_SdcHostReadOneBlk( locBlkSizeAndCnt[0], plocrddata );
		}
	}
	else
	{

		/*read multiplex blocks*/
        /*set AutoCommand12 bit of option*/
		V8B_SdifEnableAutoCmd12();

		/*send read multiplex command*/
		rtnVal = V8_SdcSendCmd(18, addr);
		if( VSDC_NORMAL_COMPLETE == rtnVal )
		{
            rtnVal = V8_SdcHostReadMulBlk( locBlkSizeAndCnt[1], locBlkSizeAndCnt[0], plocrddata);
		}
	}

	/*be sure the state of SDIF is idle*/
	rtnVal = V8_SdcWaitIdle( );
	if(  VSDC_WAIT_IDLE_TIMEOUT == rtnVal )
	{
		V8B_SdifSoftReset( SD_REG_RST | SD_CTRL_RST );
	}

	if( VSDC_NORMAL_COMPLETE != rtnVal)
	{
		return 0;
	}

	return locBlkSizeAndCnt[0] * locBlkSizeAndCnt[1];
}

#endif	//V868CMD_DISABLE
