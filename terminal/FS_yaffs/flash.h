
/*KM29U128 IO 定义*/
	/*  PC7		6		5		4		3		2		1		0	*/
	/*   I		I		I		I		I		I		I		I	*/
	/*   NC		NC		NC		NC		SMCALE	SMCCLE	SMCCE	SMCRB*/
	/*   00		00		00		00		01		01		01		00	*/

#define RB	(0x01)
#define CE	(0x02)
#define CLE	(0x04)
#define ALE	(0x08)	
#define rKM29UXDATA	   (*(volatile unsigned char *)0x02000000)	//bank1              
/*KM29U128操作宏定义*/
#define FC_CMD		{rPDATC |= CLE;	rPDATC &= ~(ALE|CE);}
#define FC_ADDR		{rPDATC |= ALE;	rPDATC&=~(CLE|CE);}
#define FC_DATA        {rPDATC &= ~(ALE|CLE|CE);               }
#define FC_INACTIVE	{rPDATA |= CE;	rPDATC&=~(ALE|CLE);}       
#define WAITRB		while(!(rPDATC & RB))  //wait tWB and check R/B pin.   
#define KM29U128_ID (0xec73)
#define WRITEVERIFY  (1)  //Verifing is enable when writing flash
/*KM29U128函数声明*/
void DEV_FD_Reset(void);//flash reset
unsigned int DEV_FD_Check_Id(void);
unsigned char DEV_FD_Erase_Block(unsigned int block);
void DEV_FD_Read_Page(unsigned int block,unsigned int page,unsigned char *pPage);
int DEV_FD_Write_Page(unsigned int block,unsigned int page,unsigned char *pPage);
int DEV_FD_Verify_Page(unsigned int block,unsigned int page,unsigned char *buffer);