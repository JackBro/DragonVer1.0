#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "flash.h"
#include "def.h"

	/*  PC7		6		5		4		3		2		1		0	*/
	/*   I		I		I		I		I		I		I		I	*/
	/*   NC		NC		NC		NC		SMCALE	SMCCLE	SMCCE	SMCRB*/
	/*   00		00		00		00		01		01		01		00	*/

extern void __RdPage528(unsigned char *);	/*these two function are writed in ASM*/
extern void __WrPage528(unsigned char *);
unsigned char Verify_Buf[528];
unsigned int DEV_FD_Check_Id(void)
{
	int i;
	unsigned int id;

	FC_CMD;
	rKM29UXDATA = 0x90;

	FC_ADDR;
	rKM29UXDATA = 0;
	FC_DATA;

	for(i=0;i<10;i++);	//wait 100ns
	id = rKM29UXDATA<<8;
	id += rKM29UXDATA;
	FC_INACTIVE;
	return id;
}

void DEV_FD_Reset()		//flash reset
{ 
	int i;
	//  rPDATC = rPDATC | WP;	//write unprotect
	FC_CMD;
	rKM29UXDATA = 0xff;	//reset command
	for (i=0;i<3000;i++);	//delay 	
}
/********************************************************/
/*���ܣ�����FLASH��1Block(��Ӧ�ļ�ϵͳΪ1Cluster)          */
/*     ÿһ����Ϊ16KB�����KM29U128����1024����            */
/*���룺unsigned int cluster/block(��Ҫ������Block Number)*/
/*���أ�OK��FAIL                                         */
/********************************************************/
unsigned char DEV_FD_Erase_Block(unsigned int block) 
{
	unsigned int blockPage = (block<<5);
	int i;

	FC_CMD;
	rKM29UXDATA = 0x60;

	FC_ADDR;	     
	rKM29UXDATA = blockPage & 0xff;  
	rKM29UXDATA = (blockPage>>8) & 0xff;

	for(i=0;i<2;i++);		//tWC  50ns
    
	FC_CMD;
	rKM29UXDATA = 0xd0;
    
	for(i=0;i<3;i++);
    
	FC_DATA;
	WAITRB;					//wait max 3ms

	FC_CMD;
	rKM29UXDATA = 0x70;
	FC_DATA;
	if (rKM29UXDATA & 0x1) //erase error
	{
		FC_INACTIVE;
		return FAIL;
	}
	else 
	{
		FC_INACTIVE;   
		return OK;
	}
}
/********************************************************/
int DEV_FD_Verify_Page(unsigned int block,unsigned int page,U8 *buffer)
{
	DEV_FD_Read_Page(block,page,Verify_Buf);
	if(strncmp((char *)Verify_Buf,(char *)buffer,528)==0)
		return 1;
	else
		return 0;
}
/*****************************************************************/
/*���ܣ���ȡFLASH��ĳ��Block�е�1page����                           */
/*     ���ļ�ϵͳ�У������¶�Ӧ��ϵ��Block=Cluster                   */
/*                               Page =Sector                    */
/*���룺unsigned int block,page,unsigned char *pPage(������ݵĵ�ַ)*/
/*****************************************************************/
void DEV_FD_Read_Page(unsigned int block,unsigned int page,unsigned char *pPage)
{
	int i;
	unsigned int blockPage = (block<<5) + page;

	FC_CMD;               
	rKM29UXDATA = 0x00;
	FC_ADDR;
	rKM29UXDATA = 0;
	rKM29UXDATA = blockPage & 0xff;
	rKM29UXDATA = (blockPage>>8) & 0xff;
	for(i=0;i<3;i++);
	FC_DATA;
	WAITRB;	//random access ,wait max. 10us 
	__RdPage528(pPage);		//which is written with assemble in flash.s
	FC_INACTIVE;
}
/*****************************************************************/
/*���ܣ���FLASH��ĳ��Block�е�1pageд������                         */
/*     ���ļ�ϵͳ�У������¶�Ӧ��ϵ��Block=Cluster                   */
/*                               Page =Sector                    */
/*���룺unsigned int block,page,unsigned char *pPage(������ݵĵ�ַ)*/
/*�����0:Fail;1:OK                                               */
/*****************************************************************/
int DEV_FD_Write_Page(unsigned int block,unsigned int page,unsigned char *pPage)  
{
	unsigned int blockPage = (block<<5) + page;
	int i;
	U8 *Source;                                      //the begin addr of data to write
	Source=pPage;
	FC_CMD;
	rKM29UXDATA = 0x80;
	FC_ADDR;
	rKM29UXDATA = 0;
	rKM29UXDATA = blockPage & 0xff;
	rKM29UXDATA = (blockPage>>8) & 0xff;
	FC_DATA;
//	for(i=0;i<528;i++)
//		{rKM29UXDATA=*pPage++;}         //data input->as the same as _WrPage528()
	__WrPage528(pPage); //which is written with assemble in flash.s

	FC_CMD;
	rKM29UXDATA = 0x10;
	for(i=0;i<10;i++);  //twb=100ns. why is it 10? spec is false?  
                        //No. It's because of LED of PE1.
	WAITRB;	//wait max 500us;
	rKM29UXDATA = 0x70;
	FC_DATA;
	for(i=0;i<3;i++);  //twhr=60ns
	if((rKM29UXDATA & 0x1))	//			ע�����ĳ�����������,�ᵼ�¶�ջ����!!!�ֺ���
	{
		FC_INACTIVE;	
		return 0;
	}
	else
	{
		FC_INACTIVE;	    
		#if (WRITEVERIFY==1)
			return DEV_FD_Verify_Page(block,page,pPage);	
		#else
			return 1;
		#endif
	}
}
