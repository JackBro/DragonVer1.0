#include "internal.h"

#include "../terminal/terminal/s3c44box/hostregdef.h"



const TRegUnit gSystemReg[] =
{
	{ HOST_REGTYPE, "REG_SYSCFG",	REG_SYSCFG,		0, 0, "1'b[7]: Reserved to 0;\n1'b[6]: Reserved to 0;\n1'b[5]: DATA ABORT controls;\n1'b[4]: Enable read stall option;\n1'b[3]: Write buffer enable / disable;\n2'b[2:1]: Cache mode;\n1'b[0]: Enable stall option." }
};

const TRegUnit gCacheReg[] =
{
	{ HOST_REGTYPE, "REG_NCACHBE0", REG_NCACHBE0,	0, 0, "16'b[31:16]: End address of non-cacheable area 0;\n16'b[15:0]: Start address of non-cacheable area 0." },
	{ HOST_REGTYPE, "REG_NCACHBE1", REG_NCACHBE1,	0, 0, "16'b[31:16]: End address of non-cacheable area 1;\n16'b[15:0]: Start address of non-cacheable area 1." }
};

const TRegUnit gBusCtrlReg[] =
{
	{ HOST_REGTYPE, "REG_SBUSCON",	REG_SBUSCON,	0, 0, "1'b[31]: Round-robin/Fixed priorities;\n15'b[30:16]: Reserved;\n2'b[15:14]: LCD_DMA bus priority (read only);\n2'b[13:12]: ZDMA bus priority (read only);\n2'b[11:10]: BDMA bus priority (read only);\n2'b[9:8]: nBREQ bus priority (read only);\n2'b[7:6]: LCD_DMA bus priority;\n2'b[5:4]: ZDMA bus priority;\n2'b[3:2]: BDMA bus priority;\n2'b[1:0]: nBREQ bus priority." }
};

const TRegUnit gMemCtrlReg[] =
{
	{ HOST_REGTYPE, "REG_BWSCON",	REG_BWSCON,		0, 0, "1'b[31]: SRAM for using UB/LB for bank 7;\n1'b[30]: WAIT status for bank 7;\n2'b[29:28]: Data bus width for bank 7;\n1'b[27]: SRAM for using UB/LB for bank 6;\n1'b[26]: WAIT status for bank 6;\n2'b[25:24]: Data bus width for bank 6;\n1'b[23]: SRAM for using UB/LB for bank 5;\n1'b[22]: WAIT status for bank 5;\n2'b[21:20]: Data bus width for bank 5;\n1'b[19]: SRAM for using UB/LB for bank 4;\n1'b[18]: WAIT status for bank 4;\n2'b[17:16]: Data bus width for bank 4;\n1'b[15]: SRAM for using UB/LB for bank 3;\n1'b[14]: WAIT status for bank 3;\n2'b[13:12]: Data bus width for bank 3;\n1'b[11]: SRAM for using UB/LB for bank 2;\n1'b[10]: WAIT status for bank 2;\n2'b[9:8]: Data bus width for bank 2;\n1'b[7]: SRAM for using UB/LB for bank 1;\n1'b[6]: WAIT status for bank 1;\n2'b[5:4]: Data bus width for bank 1;\n1'b[3]: Reserved;\n2'b[2:1]: Data bus width for bank 0 (read only);\n1'b[0]: Endian mode (read only)." },
	{ HOST_REGTYPE, "REG_BANKCON0", REG_BANKCON0,	0, 0, "2'b[14:13]: Address set-up before nGCSn;\n2'b[12:11]: Chip selection set-up nOE;\n3'b[10:8]: Access cycle;\n2'b[7:6]: Chip selection hold on nOE;\n2'b[5:4]: Address holding time after nGCSn;\n2'b[3:2]: Page mode access cycle @ Page mode;\n2'b[1:0]: Page mode configuration." },
	{ HOST_REGTYPE, "REG_BANKCON1", REG_BANKCON1,	0, 0, "2'b[14:13]: Address set-up before nGCSn;\n2'b[12:11]: Chip selection set-up nOE;\n3'b[10:8]: Access cycle;\n2'b[7:6]: Chip selection hold on nOE;\n2'b[5:4]: Address holding time after nGCSn;\n2'b[3:2]: Page mode access cycle @ Page mode;\n2'b[1:0]: Page mode configuration." },
	{ HOST_REGTYPE, "REG_BANKCON2", REG_BANKCON2,	0, 0, "2'b[14:13]: Address set-up before nGCSn;\n2'b[12:11]: Chip selection set-up nOE;\n3'b[10:8]: Access cycle;\n2'b[7:6]: Chip selection hold on nOE;\n2'b[5:4]: Address holding time after nGCSn;\n2'b[3:2]: Page mode access cycle @ Page mode;\n2'b[1:0]: Page mode configuration." },
	{ HOST_REGTYPE, "REG_BANKCON3", REG_BANKCON3,	0, 0, "2'b[14:13]: Address set-up before nGCSn;\n2'b[12:11]: Chip selection set-up nOE;\n3'b[10:8]: Access cycle;\n2'b[7:6]: Chip selection hold on nOE;\n2'b[5:4]: Address holding time after nGCSn;\n2'b[3:2]: Page mode access cycle @ Page mode;\n2'b[1:0]: Page mode configuration." },
	{ HOST_REGTYPE, "REG_BANKCON4", REG_BANKCON4,	0, 0, "2'b[14:13]: Address set-up before nGCSn;\n2'b[12:11]: Chip selection set-up nOE;\n3'b[10:8]: Access cycle;\n2'b[7:6]: Chip selection hold on nOE;\n2'b[5:4]: Address holding time after nGCSn;\n2'b[3:2]: Page mode access cycle @ Page mode;\n2'b[1:0]: Page mode configuration." },
	{ HOST_REGTYPE, "REG_BANKCON5", REG_BANKCON5,	0, 0, "2'b[14:13]: Address set-up before nGCSn;\n2'b[12:11]: Chip selection set-up nOE;\n3'b[10:8]: Access cycle;\n2'b[7:6]: Chip selection hold on nOE;\n2'b[5:4]: Address holding time after nGCSn;\n2'b[3:2]: Page mode access cycle @ Page mode;\n2'b[1:0]: Page mode configuration." },
	{ HOST_REGTYPE, "REG_BANKCON6", REG_BANKCON6,	0, 0, "2'b[16:15]: Memory type for bank6 and bank7;\n\nMemory Type = ROM or SRAM\n2'b[14:13]: Address set-up before nGCS;\n2'b[12:11]: Chip selection set-up nOE;\n3'b[10:8]: Access cycle;\n2'b[7:6]: Chip selection hold on nOE;\n2'b[5:4]: Address holding time after nGCSn;\n2'b[3:2]: Page mode access cycle @ Page mode;\n2'b[1:0]: Page mode configuration.\n\nMemory Type = FP DRAM or EDO DRAM\n2'b[5:4]: RAS to CAS delay;\n1'b[3]: CAS pulse width;\n1'b[2]: CAS pre-charge;\n2'b[1:0]: Column address number.\n\nMemory Type = SDRAM\n2'b[3:2]: RAS to CAS delay;\n2'b[1:0]: Column address number." },
	{ HOST_REGTYPE, "REG_BANKCON7", REG_BANKCON7,	0, 0, "2'b[16:15]: Memory type for bank6 and bank7;\n\nMemory Type = ROM or SRAM\n2'b[14:13]: Address set-up before nGCS;\n2'b[12:11]: Chip selection set-up nOE;\n3'b[10:8]: Access cycle;\n2'b[7:6]: Chip selection hold on nOE;\n2'b[5:4]: Address holding time after nGCSn;\n2'b[3:2]: Page mode access cycle @ Page mode;\n2'b[1:0]: Page mode configuration.\n\nMemory Type = FP DRAM or EDO DRAM\n2'b[5:4]: RAS to CAS delay;\n1'b[3]: CAS pulse width;\n1'b[2]: CAS pre-charge;\n2'b[1:0]: Column address number.\n\nMemory Type = SDRAM\n2'b[3:2]: RAS to CAS delay;\n2'b[1:0]: Column address number." }
};

const TRegUnit gUartReg[] =
{
	{ HOST_REGTYPE, "REG_REFRESH",	REG_REFRESH,	0, 0, "1'b[23]: DRAM/SDRAM Refresh Enable;\n1'b[22]: DRAM/SDRAM Refresh Mode;\n2'b[21:20]: DRAM/SDRAM RAS pre-charge Time;\n2'b[19:18]: SDRAM RC minimum Time;\n2'b[17:16]: CAS Hold Time(DRAM);\n5'b[15:11]: Not use;\n11'b[10:0]: DRAM/SDRAM refresh count value." },
	{ HOST_REGTYPE, "REG_BANKSIZE", REG_BANKSIZE,	0, 0, "1'b[4]: SCLK will be generated only during SDRAM access cycle;\n1'b[3]: Not use;\n3'b[2:0]: BANK6/7 memory map." },
	{ HOST_REGTYPE, "REG_MRSRB6",	REG_MRSRB6,		0, 0, "2'b[11:10]: Not use;\n1'b[9]: Write burst length;\n2'b[8:7]: Test mode;\n3'b[6:4]: CAS latency;\n1'b[3]: Burst type;\n3'b[2:0]: Burst length." },
	{ HOST_REGTYPE, "REG_MRSRB7",	REG_MRSRB7,		0, 0, "2'b[11:10]: Not use;\n1'b[9]: Write burst length;\n2'b[8:7]: Test mode;\n3'b[6:4]: CAS latency;\n1'b[3]: Burst type;\n3'b[2:0]: Burst length." },
	{ HOST_REGTYPE, "REG_ULCON0",	REG_ULCON0,		0, 0, "1'b[7]: Reserved;\n1'b[6]: Infra-Red mode;\n3'b[5:3]: Parity Mode;\n1'b[2]: Number of stop bit;\n2'b[1:0]: Word length." },
	{ HOST_REGTYPE, "REG_ULCON1",	REG_ULCON1,		0, 0, "1'b[7]: Reserved;\n1'b[6]: Infra-Red mode;\n3'b[5:3]: Parity Mode;\n1'b[2]: Number of stop bit;\n2'b[1:0]: Word length." },
	{ HOST_REGTYPE, "REG_UCON0",	REG_UCON0,		0, 0, "1'b[9]: Tx interrupt type;\n1'b[8]: Rx interrupt type;\n1'b[7]: Enable/Disable Rx time out interrupt;\n1'b[6]: Rx error status interrupt enable;\n1'b[5]: Loop-back Mode;\n1'b[4]: Send Break Signal;\n2'b[3:2]: Transmit Mode;\n2'b[1:0]: Receive Mode." },
	{ HOST_REGTYPE, "REG_UCON1",	REG_UCON1,		0, 0, "1'b[9]: Tx interrupt type;\n1'b[8]: Rx interrupt type;\n1'b[7]: Enable/Disable Rx time out interrupt;\n1'b[6]: Rx error status interrupt enable;\n1'b[5]: Loop-back Mode;\n1'b[4]: Send Break Signal;\n2'b[3:2]: Transmit Mode;\n2'b[1:0]: Receive Mode." },
	{ HOST_REGTYPE, "REG_UFCON0",	REG_UFCON0,		0, 0, "2'b[7:6]: Tx FIFO Trigger Level;\n2'b[5:4]: Rx FIFO Trigger Level;\n1'b[3]: Reserved;\n1'b[2]: Tx FIFO Reset;\n1'b[1]: Rx FIFO Reset;\n1'b[0]: FIFO Enable." },
	{ HOST_REGTYPE, "REG_UFCON1",	REG_UFCON1,		0, 0, "2'b[7:6]: Tx FIFO Trigger Level;\n2'b[5:4]: Rx FIFO Trigger Level;\n1'b[3]: Reserved;\n1'b[2]: Tx FIFO Reset;\n1'b[1]: Rx FIFO Reset;\n1'b[0]: FIFO Enable." },
	{ HOST_REGTYPE, "REG_UMCON0",	REG_UMCON0,		0, 0, "3'b[7:5]: Reserved(These bits must be 0's);\n1'b[4]: AFC(Auto Flow Control);\n3'b[3:1]: Reserved(These bits must be 0's);\n1'b[0]: Request to Send." },
	{ HOST_REGTYPE, "REG_UMCON1",	REG_UMCON1,		0, 0, "3'b[7:5]: Reserved(These bits must be 0's);\n1'b[4]: AFC(Auto Flow Control);\n3'b[3:1]: Reserved(These bits must be 0's);\n1'b[0]: Request to Send." },
	{ HOST_REGTYPE, "REG_UTRSTAT0", REG_UTRSTAT0,	0, 0, "1'b[2]: Transmit shifter empty;\n1'b[1]: Transmit buffer empty;\n1'b[0]: Receive buffer data ready." },
	{ HOST_REGTYPE, "REG_UTRSTAT1", REG_UTRSTAT1,	0, 0, "1'b[2]: Transmit shifter empty;\n1'b[1]: Transmit buffer empty;\n1'b[0]: Receive buffer data ready." },
	{ HOST_REGTYPE, "REG_UERSTAT0", REG_UERSTAT0,	0, 0, "1'b[3]: Break Detect;\n1'b[2]: Frame Error;\n1'b[1]: Parity Error;\n1'b[0]: Overrun Error." },
	{ HOST_REGTYPE, "REG_UERSTAT1", REG_UERSTAT1,	0, 0, "1'b[3]: Break Detect;\n1'b[2]: Frame Error;\n1'b[1]: Parity Error;\n1'b[0]: Overrun Error." },
	{ HOST_REGTYPE, "REG_UFSTAT0",	REG_UFSTAT0,	0, 0, "6'b[15:10]: Reserved;\n1'b[9]: Tx FIFO Full;\n1'b[8]: Rx FIFO Full;\n4'b[7:4]: Tx FIFO Count;\n4'b[3:0]: Rx FIFO Count." },
	{ HOST_REGTYPE, "REG_UFSTAT1",	REG_UFSTAT1,	0, 0, "6'b[15:10]: Reserved;\n1'b[9]: Tx FIFO Full;\n1'b[8]: Rx FIFO Full;\n4'b[7:4]: Tx FIFO Count;\n4'b[3:0]: Rx FIFO Count." },
	{ HOST_REGTYPE, "REG_UMSTAT0",	REG_UMSTAT0,	0, 0, "1'b[4]: Delta CTS;\n3'b[3:1]: Reserved;\n1'b[0]: Clear to Send." },
	{ HOST_REGTYPE, "REG_UMSTAT1",	REG_UMSTAT1,	0, 0, "1'b[4]: Delta CTS;\n3'b[3:1]: Reserved;\n1'b[0]: Clear to Send." },
	{ HOST_REGTYPE, "REG_UBRDIV0",	REG_UBRDIV0,	0, 0, "16'b[15:0]: Baud rate division value.(UBRDIV0 > 0)" },
	{ HOST_REGTYPE, "REG_UBRDIV1",	REG_UBRDIV1,	0, 0, "16'b[15:0]: Baud rate division value.(UBRDIV1 > 0)" },
	{ HOST_REGTYPE, "REG_UTXH0",	REG_UTXH0,		0, 0, "8'b[7:0]: Transmit data for UART0." },
	{ HOST_REGTYPE, "REG_UTXH1",	REG_UTXH1,		0, 0, "8'b[7:0]: Transmit data for UART1." },
	{ HOST_REGTYPE, "REG_URXH0",	REG_URXH0,		0, 0, "8'b[7:0]: Receive data for UART0." },
	{ HOST_REGTYPE, "REG_URXH1",	REG_URXH1,		0, 0, "8'b[7:0]: Receive data for UART1." }
};

const TRegUnit gSioReg[] =
{
	{ HOST_REGTYPE, "REG_SIOCON",	REG_SIOCON,		0, 0, "1'b[7]: SIO shift clock source select bit;\n1'b[6]: Data direction(LSB/MSB first);\n1'b[5]: Tx/Rx selection;\n1'b[4]: Clock edge select;\n1'b[3]: SIO start;\n1'b[2]: SIO shift operation;\n2'b[1:0]: SIO mode select." },
	{ HOST_REGTYPE, "REG_SIODAT",	REG_SIODAT,		0, 0, "8'b[7:0]: SIO DATA." },
	{ HOST_REGTYPE, "REG_SBRDR",	REG_SBRDR,		0, 0, "12'b[11:0]: The prescaler value for the baud rate." },
	{ HOST_REGTYPE, "REG_IVTCNT",	REG_IVTCNT,		0, 0, "8'b[7:0]: SIO interval counter." },
	{ HOST_REGTYPE, "REG_DCNTZ",	REG_DCNTZ,		0, 0, "1'b[1]: Enables/Disables BDMA1 service request;\n1'b[0]: Enables/Disables BDMA0 service request." }
};

const TRegUnit gIisReg[] =
{
	{ HOST_REGTYPE, "REG_IISCON",	REG_IISCON,		0, 0, "1'b[8]: Left/Right channel index (read only);\n1'b[7]: Transmit FIFO ready flag (read only);\n1'b[6]: Receive FIFO ready flag (read only);\n1'b[5]: Transmit DMA service request enable;\n1'b[4]: Receive DMA service request enable;\n1'b[3]: Transmit channel idle command;\n1'b[2]: Receive channel idle command;\n1'b[1]: IIS prescaler enable;\n1'b[0]: IIS interface enable(start)." },
	{ HOST_REGTYPE, "REG_IISMOD",	REG_IISMOD,		0, 0, "1'b[8]: Master/slave mode select;\n2'b[7:6]: Transmit/receive mode select;\n1'b[5]: Active level of left/right channel;\n1'b[4]: Serial interface format;\n1'b[3]: Serial data bit per channel;\n1'b[2]: Master clock(CODECLK)frequency select;\n2'b[1:0]: Serial bit clock frequency select." },
	{ HOST_REGTYPE, "REG_IISPSR",	REG_IISPSR,		0, 0, "4'b[7:4]: Prescaler value A;\n4'b[3:0]: Prescaler value B." },
	{ HOST_REGTYPE, "REG_IISFCON",	REG_IISFCON,	0, 0, "1'b[11]: Transmit FIFO access mode select;\n1'b[10]: Receive FIFO access mode select;\n1'b[9]: Transmit FFO enable;\n1'b[8]: Receive FIFO enable;\n4'b[7:4]: Transmit FIFO data count (read only);\n4'b[3:0]: Receive FIFO data count (read only)." },
	{ HOST_REGTYPE, "REG_IISFIF",	REG_IISFIF,		0, 0, "16'b[15:0]: Transmit/Receive data for IIS." }
};

const TRegUnit gIoportReg[] =
{
	{ HOST_REGTYPE, "REG_PCONA",	REG_PCONA,		0, 0, "Configures the pins of port A\n1'b[9]: PA9;\n1'b[8]: PA8;\n1'b[7]: PA7;\n1'b[6]: PA6;\n1'b[5]: PA5;\n1'b[4]: PA4;\n1'b[3]: PA3;\n1'b[2]: PA2;\n1'b[1]: PA1;\n1'b[0]: PA0." },
	{ HOST_REGTYPE, "REG_PDATA",	REG_PDATA,		0, 0, "10'b[9:0]: The data of port A." },
	{ HOST_REGTYPE, "REG_PCONB",	REG_PCONB,		0, 0, "Configures the pins of port B\n1'b[10]: PB10;1'b[9]: PB9;\n1'b[8]: PB8;\n1'b[7]: PB7;\n1'b[6]: PB6;\n1'b[5]: PB5;\n1'b[4]: PB4;\n1'b[3]: PB3;\n1'b[2]: PB2;\n1'b[1]: PB1;\n1'b[0]: PB0." },
	{ HOST_REGTYPE, "REG_PDATB",	REG_PDATB,		0, 0, "11'b[10:0]: The data of port B." },
	{ HOST_REGTYPE, "REG_PCONC",	REG_PCONC,		0, 0, "Configures the pins of port C\n2'b[31:30]: PC15;\n2'b[29:28]: PC14;\n2'b[27:26]: PC13;\n2'b[25:24]: PC12;\n2'b[23:22]: PC11;\n2'b[21:20]: PC10;\n2'b[19:18]: PC9;\n2'b[17:16]: PC8;\n2'b[15:14]: PC7;\n2'b[13:12]: PC6;\n2'b[11:10]: PC5;\n2'b[9:8]: PC4;\n2'b[7:6]: PC3;\n2'b[5:4]: PC2;\n2'b[3:2]: PC1;\n2'b[1:0]: PC0." },
	{ HOST_REGTYPE, "REG_PDATC",	REG_PDATC,		0, 0, "16'[15:0]: The data of port C." },
	{ HOST_REGTYPE, "REG_PUPC",		REG_PUPC,		0, 0, "16'b[15:0]:\n0: the pull up resistor attached to the corresponding port pin is enabled.\n1: the pull up resistor is disabled." },
	{ HOST_REGTYPE, "REG_PCOND",	REG_PCOND,		0, 0, "Configures the pins of port D\n2'b[15:14]: PD7;\n2'b[13:12]: PD6;\n2'b[11:10]: PD5;\n2'b[9:8]: PD4;\n2'b[7:6]: PD3;\n2'b[5:4]: PD2;\n2'b[3:2]: PD1;\n2'b[1:0]: PD0." },
	{ HOST_REGTYPE, "REG_PDATD",	REG_PDATD,		0, 0, "8'b[7:0]: The data of port D." },
	{ HOST_REGTYPE, "REG_PUPD",		REG_PUPD,		0, 0, "8'b[7:0]:\n0: the pull up resistor attached to the corresponding port pin is enabled.\n1: the pull up resistor is disabled." },
	{ HOST_REGTYPE, "REG_PCONE",	REG_PCONE,		0, 0, "Configures the pins of port E\n2'b[17:16]: PE8;\n2'b[15:14]: PE7;\n2'b[13:12]: PE6;\n2'b[11:10]: PE5;\n2'b[9:8]: PE4;\n2'b[7:6]: PE3;\n2'b[5:4]: PE2;\n2'b[3:2]: PE1;\n2'b[1:0]: PE0." },
	{ HOST_REGTYPE, "REG_PDATE",	REG_PDATE,		0, 0, "9'b[8:0]: The data of port E." },
	{ HOST_REGTYPE, "REG_PUPE",		REG_PUPE,		0, 0, "8'b[7:0]:\n0: the pull up resistor attached to the corresponding port pin is enabled.\n1: the pull up resistor is disabled.\nPE8 do not have programmable pull-up resistor." },
	{ HOST_REGTYPE, "REG_PCONF",	REG_PCONF,		0, 0, "Configures the pins of port F\n3'b[21:19]: PF8;\n3'b[18:16]: PF7;\n3'b[15:13]: PF6;\n3'b[12:10]: PF5;\n2'b[9:8]: PF4;\n2'b[7:6]: PF3;\n2'b[5:4]: PF2;\n2'b[3:2]: PF1;\n2'b[1:0]: PF0." },
	{ HOST_REGTYPE, "REG_PDATF",	REG_PDATF,		0, 0, "9'b[8:0]: The data of port F." },
	{ HOST_REGTYPE, "REG_PUPF",		REG_PUPF,		0, 0, "9'b[8:0]:\n0: the pull up resistor attached to the corresponding port pin is enabled.\n1: the pull up resistor is disabled." },
	{ HOST_REGTYPE, "REG_PCONG",	REG_PCONG,		0, 0, "Configures the pins of port G\n2'b[15:14]: PG7;\n2'b[13:12]: PG6;\n2'b[11:10]: PG5;\n2'b[9:8]: PG4;\n2'b[7:6]: PG3;\n2'b[5:4]: PG2;\n2'b[3:2]: PG1;\n2'b[1:0]: PG0." },
	{ HOST_REGTYPE, "REG_PDATG",	REG_PDATG,		0, 0, "8'b[7:0]: The data of port G." },
	{ HOST_REGTYPE, "REG_PUPG",		REG_PUPG,		0, 0, "8'b[7:0]:\n0: the pull up resistor attached to the corresponding port pin is enabled.\n1: the pull up resistor is disabled." },
	{ HOST_REGTYPE, "REG_SPUCR",	REG_SPUCR,		0, 0, "1'b[2]: HZ@STOP;\n1'b[1]: DATA[15:8] port pull-up resistor is enabled/disabled;\n1'b[0]: DATA[7:0] port pull-up resistor is enabled/disabled." },
	{ HOST_REGTYPE, "REG_EXTINT",	REG_EXTINT,		0, 0, "3'b[30:28]: Setting the signaling method of the EINT7;\n3'b[26:24]: Setting the signaling method of the EINT6;\n3'b[22:20]: Setting the signaling method of the EINT5;\n3'b[18:16]: Setting the signaling method of the EINT4;\n3'b[14:12]: Setting the signaling method of the EINT3;\n3'b[10:8]: Setting the signaling method of the EINT2;\n3'b[6:4]: Setting the signaling method of the EINT1;\n3'b[2:0]: Setting the signaling method of the EINT0." },
	{ HOST_REGTYPE, "REG_EXTINTPND",REG_EXTINTPND,	0, 0, "1'b[3]: If EINT7 is activated, EXINTPND3 bit is set to 1, and also INTPND[21] is set to 1;\n1'b[2]: If EINT6 is activated, EXINTPND2 bit is set to 1, and also INTPND[21] is set to 1;\n1'b[1]: If EINT5 is activated, EXINTPND1 bit is set to 1, and also INTPND[21] is set to 1;\n1'b[0]: If EINT4 is activated, EXINTPND0 bit is set to 1, and also INTPND[21] is set to 1." }
};

const TRegUnit gWatchDogReg[] =
{
	{ HOST_REGTYPE, "REG_WTCON",	REG_WTCON,		0, 0, "8'b[15:8]: Prescaler value;\n2'b[7:6]: Reserved;\n1'b[5]: Enable or disable bit of watchdog timer;\n2'b[4:3]: Clock select;\n1'b[2]: Enable or disable bit of the interrupt;\n1'b[1]: Reserved;\n1'b[0]: Enable or disable bit of watchdog timer output for reset signal." },
	{ HOST_REGTYPE, "REG_WTDAT",	REG_WTDAT,		0, 0, "16'b[15:0]: Watchdog timer count value for reload." },
	{ HOST_REGTYPE, "REG_WTCNT",	REG_WTCNT,		0, 0, "16'b[15:0]: The current count value of the watchdog timer." }
};

const TRegUnit gAdcReg[] =
{
	{ HOST_REGTYPE, "REG_ADCCON",	REG_ADCCON,		0, 0, "1'b[6]: A/D converter state flag (Read Only);\n1'b[5]: System power down;\n3'b[4:2]: Clock source select;\n1'b[1]: A/D conversion start by read;\n1'b[0]: A/D conversion start by enable." },
	{ HOST_REGTYPE, "REG_ADCPSR",	REG_ADCPSR,		0, 0, "8'b[7:0]: Prescaler value (0-255)." },
	{ HOST_REGTYPE, "REG_ADCDAT",	REG_ADCDAT,		0, 0, "10'b[9:0]: A/D converter output data value." }
};

const TRegUnit gTimerReg[] =
{
	{ HOST_REGTYPE, "REG_TCFG0",	REG_TCFG0,		0, 0, "8'b[31:24]: dead zone length. The 1 unit time of the dead zone length is equal to the 1 unit time of timer 0.;\n8'b[23:16]: prescaler value for Timer 4 & 5;\n8'b[15:8]: prescaler value for Timer 3 & 2;\n8'b[7:0]: prescaler value for Timer 1 & 0." },
	{ HOST_REGTYPE, "REG_TCFG1",	REG_TCFG1,		0, 0, "4'b[27:24]: Select DMA request channel;\n4'b[23:20]: Select MUX input for PWM Timer5;\n4'b[19:16]: Select MUX input for PWM Timer4;\n4'b[15:12]: Select MUX input for PWM Timer3;\n4'b[11:8]: Select MUX input for PWM Timer2;\n4'b[7:4]: Select MUX input for PWM Timer1;\n4'b[3:0]: Select MUX input for PWM Timer0." },
	{ HOST_REGTYPE, "REG_TCON",		REG_TCON,		0, 0, "1'b[26]: Timer 5 auto reload on/off;\n1'b[25]: Timer 5 manual update;\n1'b[24]: Timer 5 start/stop;\n1'b[23]: Timer 4 auto reload on/off;\n1'b[22]: Timer 4 output inverter on/off;\n1'b[21]: Timer 4 manual update;\n1'b[20]: Timer 4 start/stop;\n1'b[19]: Timer 3 auto reload on/off;\n1'b[18]: Timer 3 output inverter on/off;\n1'b[17]: Manual update for Timer 3;\n1'b[16]: Start/stop for Timer 3;\n1'b[15]: Auto reload on/off for Timer 2;\n1'b[14]: Output inverter on/off for Timer 2;\n1'b[13]: The manual update for Timer 2;\n1'b[12]: Start/stop for Timer 2;\n1'b[11]: The auto reload on/off for Timer1;\n1'b[10]: The output inverter on/off for Timer1;\n1'b[9]: The manual update for Timer 1;\n1'b[8]: Start/stop for Timer 1;\n1'b[4]: The dead zone operation;\n1'b[3]: Auto reload on/off for Timer 0;\n1'b[2]: The output inverter on/off for Timer 0;\n1'b[1]: The manual update for Timer 0;\n1'b[0]: Start/stop for Timer 0." },
	{ HOST_REGTYPE, "REG_TCNTB0",	REG_TCNTB0,		0, 0, "16'b[15:0]: Setting count buffer value for Timer 0." },
	{ HOST_REGTYPE, "REG_TCMPB0",	REG_TCMPB0,		0, 0, "16'b[15:0]: Setting compare buffer value for Timer 0." },
	{ HOST_REGTYPE, "REG_TCNTO0",	REG_TCNTO0,		0, 0, "16'b[15:0]: Setting count observation value for Timer 0." },
	{ HOST_REGTYPE, "REG_TCNTB1",	REG_TCNTB1,		0, 0, "16'b[15:0]: Setting count buffer value for Timer 1." },
	{ HOST_REGTYPE, "REG_TCMPB1",	REG_TCMPB1,		0, 0, "16'b[15:0]: Setting compare buffer value for Timer 1." },
	{ HOST_REGTYPE, "REG_TCNTO1",	REG_TCNTO1,		0, 0, "16'b[15:0]: Setting count observation value for Timer 1." },
	{ HOST_REGTYPE, "REG_TCNTB2",	REG_TCNTB2,		0, 0, "16'b[15:0]: Setting count buffer value for Timer 2." },
	{ HOST_REGTYPE, "REG_TCMPB2",	REG_TCMPB2,		0, 0, "16'b[15:0]: Setting compare buffer value for Timer 2." },
	{ HOST_REGTYPE, "REG_TCNTO2",	REG_TCNTO2,		0, 0, "16'b[15:0]: Setting count observation value for Timer 2." },
	{ HOST_REGTYPE, "REG_TCNTB3",	REG_TCNTB3,		0, 0, "16'b[15:0]: Setting count buffer value for Timer 3." },
	{ HOST_REGTYPE, "REG_TCMPB3",	REG_TCMPB3,		0, 0, "16'b[15:0]: Setting compare buffer value for Timer 3." },
	{ HOST_REGTYPE, "REG_TCNTO3",	REG_TCNTO3,		0, 0, "16'b[15:0]: Setting count observation value for Timer 3." },
	{ HOST_REGTYPE, "REG_TCNTB4",	REG_TCNTB4,		0, 0, "16'b[15:0]: Setting count buffer value for Timer 4." },
	{ HOST_REGTYPE, "REG_TCMPB4",	REG_TCMPB4,		0, 0, "16'b[15:0]: Setting compare buffer value for Timer 4." },
	{ HOST_REGTYPE, "REG_TCNTO4",	REG_TCNTO4,		0, 0, "16'b[15:0]: Setting count observation value for Timer 4." },
	{ HOST_REGTYPE, "REG_TCNTB5",	REG_TCNTB5,		0, 0, "16'b[15:0]: Setting count buffer value for Timer 5." },
	{ HOST_REGTYPE, "REG_TCNTO5",	REG_TCNTO5,		0, 0, "16'b[15:0]: Setting count observation value for Timer 5." }
};

const TRegUnit gIicReg[] =
{
	{ HOST_REGTYPE, "REG_IICCON",	REG_IICCON,		0, 0, "1'b[7]: IIC-bus acknowledge enable bit;\n1'b[6]: Source clock of IIC-bus transmit clock prescaler selection bit;\n1'b[5]: IIC-Bus Tx/Rx interrupt enable/disable bit;\n1'b[4]: IIC-bus Tx/Rx interrupt pending flag;\n4'b[3:0]: IIC-Bus transmit clock prescaler." },
	{ HOST_REGTYPE, "REG_IICSTAT",	REG_IICSTAT,	0, 0, "2'b[7:6]: IIC-bus master/slave Tx/Rx mode select bits;\n1'b[5]: IIC-Bus busy signal status bit;\n1'b[4]: IIC-bus data output enable/disable bit;\n1'b[3]: IIC-bus arbitration procedure status flag bit;\n1'b[2]: IIC-bus address-as-slave status flag bit;\n1'b[1]: IIC-bus address zero status flag bit;\n1'b[0]: IIC-bus last-received bit status flag bit." },
	{ HOST_REGTYPE, "REG_IICADD",	REG_IICADD,		0, 0, "8'b[7:0]: 7-bit slave address, latched from the IIC-bus:\nSlave address = [7:1]\nNot mapped = [0]." },
	{ HOST_REGTYPE, "REG_IICDS",	REG_IICDS,		0, 0, "8'b[7:0]: 8-bit data shift register for IIC-bus Tx/Rx operation." }
};

const TRegUnit gRtcReg[] =
{
	{ HOST_REGTYPE, "REG_RTCCON",	REG_RTCCON,		0, 0, "1'b[3]: RTC clock count reset;\n1'b[2]: BCD count select;\n1'b[1]: BCD clock select;\n1'b[0]: RTC read/write enable." },
	{ HOST_REGTYPE, "REG_RTCALM",	REG_RTCALM,		0, 0, "1'b[7]: Reserved;\n1'b[6]: Alarm global enable;\n1'b[5]: Year alarm enable;\n1'b[4]: Month alarm enable;\n1'b[3]: Day alarm enable;\n1'b[2]: Hour alarm enable;\n1'b[1]: Minute alarm enable;\n1'b[0]: Second alarm enable." },
	{ HOST_REGTYPE, "REG_ALMSEC",	REG_ALMSEC,		0, 0, "1'b[7]: Reserved;\n3'b[6:4]: BCD value for alarm second from 0 to 5;\n4'b[3:0]: from 0 to 9." },
	{ HOST_REGTYPE, "REG_ALMMIN",	REG_ALMMIN,		0, 0, "1'b[7]: Reserved;\n3'b[6:4]: BCD value for alarm minute from 0 to 5;\n4'b[3:0]: from 0 to 9." },
	{ HOST_REGTYPE, "REG_ALMHOUR",	REG_ALMHOUR,	0, 0, "2'b[7:6]: Reserved;\n2'b[5:4]: BCD value for alarm hour from 0 to 2;\n4'b[3:0]: from 0 to 9." },
	{ HOST_REGTYPE, "REG_ALMDAY",	REG_ALMDAY,		0, 0, "2'b[7:6]: Reserved;\n2'b[5:4]: BCD value for alarm day from 0 to 3;\n4'b[3:0]: from 0 to 9." },
	{ HOST_REGTYPE, "REG_ALMMON",	REG_ALMMON,		0, 0, "3'b[7:5]: Reserved;\n1'b[4]: BCD value for alarm month from 0 to 1;\n4'b[3:0]: from 0 to 9." },
	{ HOST_REGTYPE, "REG_ALMYEAR",	REG_ALMYEAR,	0, 0, "8'b[7:0]: BCD value for year from 00 to 99." },
	{ HOST_REGTYPE, "REG_RTCRST",	REG_RTCRST,		0, 0, "1'b[3]: Round second reset enable;\n3'b[2:0]: Round boundary for second carry generation." },
	{ HOST_REGTYPE, "REG_BCDSEC",	REG_BCDSEC,		0, 0, "1'b[7]: Reserved;\n3'b[6:4]: BCD value for second from 0 to 5;\n4'b[3:0]: from 0 to 9." },
	{ HOST_REGTYPE, "REG_BCDMIN",	REG_BCDMIN,		0, 0, "1'b[7]: Reserved;\n3'b[6:4]: BCD value for minute from 0 to 5;\n4'b[3:0]: from 0 to 9" },
	{ HOST_REGTYPE, "REG_BCDHOUR",	REG_BCDHOUR,	0, 0, "2'b[7:6]: Reserved;\n2'b[5:4]: BCD value for hour from 0 to 2;\n4'b[3:0]: from 0 to 9." },
	{ HOST_REGTYPE, "REG_BCDDAY",	REG_BCDDAY,		0, 0, "2'b[7:6]: Reserved;\n2'b[5:4]: BCD value for day from 0 to 3;\n4'b[3:0]: from 0 to 9." },
	{ HOST_REGTYPE, "REG_BCDDATE",	REG_BCDDATE,	0, 0, "5'b[7:3]: Reserved;\n3'b[2:0]: BCD value for date from 1 to 7." },
	{ HOST_REGTYPE, "REG_BCDMON",	REG_BCDMON,		0, 0, "3'b[7:5]: Reserved;\n1'b[4]: BCD value for month from 0 to 1;\n4'b[3:0]: from 0 to 9." },
	{ HOST_REGTYPE, "REG_BCDYEAR",	REG_BCDYEAR,	0, 0, "8'b[7:0]: BCD value for year from 00 to 99." },
	{ HOST_REGTYPE, "REG_TICINT",	REG_TICINT,		0, 0, "1'b[7]: Tick time interrupt enable;\n7'b[6:0]: Tick time count value(1-127)." }
};

const TRegUnit gCpmReg[] =
{
	{ HOST_REGTYPE, "REG_PLLCON",	REG_PLLCON,		0, 0, "8'b[19:12]: Main divider control;\n6'b[9:4]: Pre-divider control;\n2'b[1:0]: Post divider control." },
	{ HOST_REGTYPE, "REG_CLKCON",	REG_CLKCON,		0, 0, "1'b[14]: Controls MCLK into IIS block;\n1'b[13]: Controls MCLK into IIC block;\n1'b[12]: Controls MCLK into ADC block;\n1'b[11]: Controls MCLK into RTC control block;\n1'b[10]: Controls MCLK into GPIO block;\n1'b[9]: Controls MCLK into UART1 block;\n1'b[8]: Controls MCLK into UART0 block;\n1'b[7]: Controls MCLK into BDMA block;\n1'b[6]: Controls MCLK into LCDC block;\n1'b[5]: Controls MCLK into SIO block;\n1'b[4]: Controls MCLK into ZDMA block;\n1'b[3]: Controls MCLK into PWMTIMER block;\n1'b[2]: Enters IDLE mode;\n1'b[1]: SL_IDLE mode option;\n1'b[0]: Enters STOP mode." },
	{ HOST_REGTYPE, "REG_CLKSLOW",	REG_CLKSLOW,	0, 0, "1'b[5]: PLL is turned on/off;\n1'b[4]: 0 : Fout = Fpllo (PLL output)\n1: Fout = Fin / (2 x SLOW_VAL), (SLOW_VAL > 0)\nFout = Fin, (SLOW_VAL = 0);\n1'b[3:0]: The divider value for the slow clock when SLOW_BIT is on." },
	{ HOST_REGTYPE, "REG_LOCKTIME", REG_LOCKTIME,	0, 0, "12'b[11:0]: PLL lock time count value." }
};

const TRegUnit gInterruptReg[] =
{
	{ HOST_REGTYPE, "REG_INTCON",	REG_INTCON,		0, 0, "1'b[3]: Reserved;\n1'b[2]: Disables/enables vector mode for IRQ;\n1'b[1]: Enables IRQ interrupt request line to CPU;\n1'b[0]: Enables FIQ interrupt request line to CPU." },
	{ HOST_REGTYPE, "REG_INTPND",	REG_INTPND,		0, 0, "Indicates the interrupt request status.\n1'b[25]: EINT0;\n1'b[24]: EINT1;\n1'b[23]: EINT2;\n1'b[22]: EINT3;\n1'b[21]: EINT4/5/6/7;\n1'b[20]: NT_TICK;\n1'b[19]: INT_ZDMA0;\n1'b[18]: INT_ZDMA1;\n1'b[17]: INT_BDMA0;\n1'b[16]: INT_BDMA1;\n1'b[15]: INT_WDT;\n1'b[14]: INT_UERR0/1;\n1'b[13]: INT_TIMER0;\n1'b[12]: INT_TIMER1;\n1'b[11]: INT_TIMER2;\n1'b[10]: INT_TIMER3;\n1'b[9]: INT_TIMER4;\n1'b[8]: INT_TIMER5;\n1'b[7]: INT_URXD0;\n1'b[6]: INT_URXD1;\n1'b[5]: INT_IIC;\n1'b[4]: INT_SIO;\n1'b[3]: INT_UTXD0;\n1'b[2]: INT_UTXD1;\n1'b[1]: INT_RTC;\n1'b[0]: INT_ADC." },
	{ HOST_REGTYPE, "REG_INTMOD",	REG_INTMOD,		0, 0, "Interrupt mode Register\n1'b[25]: EINT0;\n1'b[24]: EINT1;\n1'b[23]: EINT2;\n1'b[22]: EINT3;\n1'b[21]: EINT4/5/6/7;\n1'b[20]: NT_TICK;\n1'b[19]: INT_ZDMA0;\n1'b[18]: INT_ZDMA1;\n1'b[17]: INT_BDMA0;\n1'b[16]: INT_BDMA1;\n1'b[15]: INT_WDT;\n1'b[14]: INT_UERR0/1;\n1'b[13]: INT_TIMER0;\n1'b[12]: INT_TIMER1;\n1'b[11]: INT_TIMER2;\n1'b[10]: INT_TIMER3;\n1'b[9]: INT_TIMER4;\n1'b[8]: INT_TIMER5;\n1'b[7]: INT_URXD0;\n1'b[6]: INT_URXD1;\n1'b[5]: INT_IIC;\n1'b[4]: INT_SIO;\n1'b[3]: INT_UTXD0;\n1'b[2]: INT_UTXD1;\n1'b[1]: INT_RTC;\n1'b[0]: INT_ADC." },
	{ HOST_REGTYPE, "REG_INTMSK",	REG_INTMSK,		0, 0, "The masked interrupt source will not be serviced.\n1'b[27]: Reserved;\n1'b[26]: Global;\n1'b[25]: EINT0;\n1'b[24]: EINT1;\n1'b[23]: EINT2;\n1'b[22]: EINT3;\n1'b[21]: EINT4/5/6/7;\n1'b[20]: NT_TICK;\n1'b[19]: INT_ZDMA0;\n1'b[18]: INT_ZDMA1;\n1'b[17]: INT_BDMA0;\n1'b[16]: INT_BDMA1;\n1'b[15]: INT_WDT;\n1'b[14]: INT_UERR0/1;\n1'b[13]: INT_TIMER0;\n1'b[12]: INT_TIMER1;\n1'b[11]: INT_TIMER2;\n1'b[10]: INT_TIMER3;\n1'b[9]: INT_TIMER4;\n1'b[8]: INT_TIMER5;\n1'b[7]: INT_URXD0;\n1'b[6]: INT_URXD1;\n1'b[5]: INT_IIC;\n1'b[4]: INT_SIO;\n1'b[3]: INT_UTXD0;\n1'b[2]: INT_UTXD1;\n1'b[1]: INT_RTC;\n1'b[0]: INT_ADC." },
	{ HOST_REGTYPE, "REG_I_PSLV",	REG_I_PSLV,		0, 0, "IRQ priority of slave register\n8'b[31:24]: The priorities among sGA, B, C, D of mGA;\n8'b[23:16]: The priorities among sGA, B, C, D of mGB;\n8'b[15:8]: The priorities among sGA, B, C, D of mGC;\n8'b[7:0]: The priorities among sGA, B, C, D of mGD." },
	{ HOST_REGTYPE, "REG_I_PMST",	REG_I_PMST,		0, 0, "IRQ priority of master register\n3'b[15:13]: Reserved;\n1'b[12]: Master operating mode;\n4'b[11:8]: Slave operating mode;\n8'b[7:0]: The priorities among 4 slave units." },
	{ HOST_REGTYPE, "REG_I_CSLV",	REG_I_CSLV,		0, 0, "Current IRQ priority of slave register\n8'b[31:24]: The current priorities of mGA;\n8'b[23:16]: The current priorities of mGB;\n8'b[15:8]: The current priorities of mGC;\n8'b[7:0]: The current priorities of mGD." },
	{ HOST_REGTYPE, "REG_I_CMST",	REG_I_CMST,		0, 0, "Current IRQ priority of master register\n2'b[15:14]: Reserved;\n6'b[13:8]: The lower 6 bits of corresponding branch machine code;\n8'b[7:0]: Current priority of master." },
	{ HOST_REGTYPE, "REG_I_ISPR",	REG_I_ISPR,		0, 0, "IRQ interrupt service pending register\n1'b[25]: EINT0;\n1'b[24]: EINT1;\n1'b[23]: EINT2;\n1'b[22]: EINT3;\n1'b[21]: EINT4/5/6/7;\n1'b[20]: NT_TICK;\n1'b[19]: INT_ZDMA0;\n1'b[18]: INT_ZDMA1;\n1'b[17]: INT_BDMA0;\n1'b[16]: INT_BDMA1;\n1'b[15]: INT_WDT;\n1'b[14]: INT_UERR0/1;\n1'b[13]: INT_TIMER0;\n1'b[12]: INT_TIMER1;\n1'b[11]: INT_TIMER2;\n1'b[10]: INT_TIMER3;\n1'b[9]: INT_TIMER4;\n1'b[8]: INT_TIMER5;\n1'b[7]: INT_URXD0;\n1'b[6]: INT_URXD1;\n1'b[5]: INT_IIC;\n1'b[4]: INT_SIO;\n1'b[3]: INT_UTXD0;\n1'b[2]: INT_UTXD1;\n1'b[1]: INT_RTC;\n1'b[0]: INT_ADC." },
	{ HOST_REGTYPE, "REG_I_ISPC",	REG_I_ISPC,		0, 0, "IRQ interrupt service pending clear register\n1'b[25]: EINT0;\n1'b[24]: EINT1;\n1'b[23]: EINT2;\n1'b[22]: EINT3;\n1'b[21]: EINT4/5/6/7;\n1'b[20]: NT_TICK;\n1'b[19]: INT_ZDMA0;\n1'b[18]: INT_ZDMA1;\n1'b[17]: INT_BDMA0;\n1'b[16]: INT_BDMA1;\n1'b[15]: INT_WDT;\n1'b[14]: INT_UERR0/1;\n1'b[13]: INT_TIMER0;\n1'b[12]: INT_TIMER1;\n1'b[11]: INT_TIMER2;\n1'b[10]: INT_TIMER3;\n1'b[9]: INT_TIMER4;\n1'b[8]: INT_TIMER5;\n1'b[7]: INT_URXD0;\n1'b[6]: INT_URXD1;\n1'b[5]: INT_IIC;\n1'b[4]: INT_SIO;\n1'b[3]: INT_UTXD0;\n1'b[2]: INT_UTXD1;\n1'b[1]: INT_RTC;\n1'b[0]: INT_ADC." },
	{ HOST_REGTYPE, "REG_F_ISPR",	REG_F_ISPR,		0, 0, "FIRQ interrupt service pending register\n1'b[25]: EINT0;\n1'b[24]: EINT1;\n1'b[23]: EINT2;\n1'b[22]: EINT3;\n1'b[21]: EINT4/5/6/7;\n1'b[20]: NT_TICK;\n1'b[19]: INT_ZDMA0;\n1'b[18]: INT_ZDMA1;\n1'b[17]: INT_BDMA0;\n1'b[16]: INT_BDMA1;\n1'b[15]: INT_WDT;\n1'b[14]: INT_UERR0/1;\n1'b[13]: INT_TIMER0;\n1'b[12]: INT_TIMER1;\n1'b[11]: INT_TIMER2;\n1'b[10]: INT_TIMER3;\n1'b[9]: INT_TIMER4;\n1'b[8]: INT_TIMER5;\n1'b[7]: INT_URXD0;\n1'b[6]: INT_URXD1;\n1'b[5]: INT_IIC;\n1'b[4]: INT_SIO;\n1'b[3]: INT_UTXD0;\n1'b[2]: INT_UTXD1;\n1'b[1]: INT_RTC;\n1'b[0]: INT_ADC." },
	{ HOST_REGTYPE, "REG_F_ISPC",	REG_F_ISPC,		0, 0, "FIRQ interrupt service pending clear register\n1'b[25]: EINT0;\n1'b[24]: EINT1;\n1'b[23]: EINT2;\n1'b[22]: EINT3;\n1'b[21]: EINT4/5/6/7;\n1'b[20]: NT_TICK;\n1'b[19]: INT_ZDMA0;\n1'b[18]: INT_ZDMA1;\n1'b[17]: INT_BDMA0;\n1'b[16]: INT_BDMA1;\n1'b[15]: INT_WDT;\n1'b[14]: INT_UERR0/1;\n1'b[13]: INT_TIMER0;\n1'b[12]: INT_TIMER1;\n1'b[11]: INT_TIMER2;\n1'b[10]: INT_TIMER3;\n1'b[9]: INT_TIMER4;\n1'b[8]: INT_TIMER5;\n1'b[7]: INT_URXD0;\n1'b[6]: INT_URXD1;\n1'b[5]: INT_IIC;\n1'b[4]: INT_SIO;\n1'b[3]: INT_UTXD0;\n1'b[2]: INT_UTXD1;\n1'b[1]: INT_RTC;\n1'b[0]: INT_ADC." }
};

const TRegUnit gLcdReg[] =
{
	{ HOST_REGTYPE, "REG_LCDCON1",	REG_LCDCON1,	0, 0, "10'b[31:22]: The status of the line counter.Down count from LINEVAL to 0(Read only);\n10'b[21:22]: The rate of VCLK;\n2'b[11:10]: The VLINE pulse's high level width;\n2'b[9:8]: The delay between VLINE and VCLK;\n1'b[7]: The toggle rate of the VM;\n2'b[6:5]: Select the display mode;\n1'b[4]: The polarity of the VCLK active edge;\n1'b[3]: The line pulse polarity;\n1'b[2]: The frame pulse polarity;\n1'b[1]: The video data(VD[7:0]) polarity;\n1'b[0]: LCD video output and the logic enable/disable." },
	{ HOST_REGTYPE, "REG_LCDCON2",	REG_LCDCON2,	0, 0, "11'b[31:21]: The blank time in one horizontal line duration time;\n11'b[20:10]: The horizontal size of the LCD panel;\n10'b[9:0]: The vertical size of LCD panel." },
	{ HOST_REGTYPE, "REG_LCDCON3",	REG_LCDCON3,	0, 0, "2'b[2:1]: reserved for test;\n1'b[0]: LCD self refresh mode enable bit." },
	{ HOST_REGTYPE, "REG_LCDSADDR1",REG_LCDSADDR1,	0, 0, "2'b[28:27]: Select the monochrome, gray, or color mode;\n6'b[26:21]: These bits indicate A[27:22] of the bank location for the video buffer in the system memory.;\n21'b[20:0]: These bits indicate A[21:1] of the start address of the upper address counter, which is for the upper frame memory of dual scan LCD or the frame memory of single scan LCD." },
	{ HOST_REGTYPE, "REG_LCDSADDR2",REG_LCDSADDR2,	0, 0, "1'b[29]: Byte swap control bit;\n8'b[28:21]: the rate at which the VM signal will toggle if the MMODE bit is set to logic '1'.;\n21'b[20:0]: These bits indicate A[21:1] of the start address of the lower address counter, which is used for the lower frame memory of dual scan LCD.\nLCDBASEL = LCDBASEU + (PAGEWIDTH + OFFSIZE) x (LINEVAL +1)." },
	{ HOST_REGTYPE, "REG_LCDSADDR3",REG_LCDSADDR3,	0, 0, "11'b[19:9]: Virtual screen offset size(the number of half words);\n9'b[8:0]: Virtual screen page width(the number of half words)." },
	{ HOST_REGTYPE, "REG_REDLUT",	REG_REDLUT,		0, 0, "32'b[31:0]: These bits define which of the 16 shades each of the 8 possible red combinations will choose.." },
	{ HOST_REGTYPE, "REG_GREENLUT", REG_GREENLUT,	0, 0, "32'b[31:0]: These bits define which of the 16 shades each of the 8 possible green combinations will choose.." },
	{ HOST_REGTYPE, "REG_BLUELUT",	REG_BLUELUT,	0, 0, "16'b[15:0]: These bits define which of the 16 shades each of the 4 possible blue combinations will choose." },
	{ HOST_REGTYPE, "REG_DP1_2",	REG_DP1_2,		0, 0, "16'b[15:0]: Dithering pattern duty 1/2 register." },
	{ HOST_REGTYPE, "REG_DP4_7",	REG_DP4_7,		0, 0, "28'b[27:0]: Dithering pattern duty 4/7 register." },
	{ HOST_REGTYPE, "REG_DP3_5",	REG_DP3_5,		0, 0, "20'b[19:0]: Dithering pattern duty 3/5 register." },
	{ HOST_REGTYPE, "REG_DP2_3",	REG_DP2_3,		0, 0, "12'b[11:0]: Dithering pattern duty 2/3 register." },
	{ HOST_REGTYPE, "REG_DP5_7",	REG_DP5_7,		0, 0, "28'b[27:0]: Dithering pattern duty 5/7 register." },
	{ HOST_REGTYPE, "REG_DP3_4",	REG_DP3_4,		0, 0, "16'b[15:0]: Dithering pattern duty 3/4 register." },
	{ HOST_REGTYPE, "REG_DP4_5",	REG_DP4_5,		0, 0, "20'b[19:0]: Dithering pattern duty 4/5 register." },
	{ HOST_REGTYPE, "REG_DP6_7",	REG_DP6_7,		0, 0, "28'b[27:0]: Dithering pattern duty 6/7 register." },
	{ HOST_REGTYPE, "REG_DITHMODE", REG_DITHMODE,	0, 0, "19'b[18:0]: Dithering Mode Register.Use one of following value for your LCD 0x12210 or 0x0." }
};

const TRegUnit gZdma0Reg[] =
{
	{ HOST_REGTYPE, "REG_ZDCON0",	REG_ZDCON0,		0, 0, "2'b[7:6]: Reserved;\n2'b[5:4]: Status of DMA channel (Read only);\n2'b[3:2]: Disable/Enable External DMA request (nXDREQ);\n2'b[1:0]: Software commands." },
	{ HOST_REGTYPE, "REG_ZDISRC0",	REG_ZDISRC0,	0, 0, "2'b[31:30]: Data size for transfer;\n2'b[29:28]: Direction of address for load;\n28'b[27:0]: Initial/current source address for ZDMA0." },
	{ HOST_REGTYPE, "REG_ZDIDES0",	REG_ZDIDES0,	0, 0, "2'b[31:30]: DMA internal options. OPT = 10 is recommended;\n2'b[29:28]: Direction of address for store;\n28'b[27:0]: Initial/current destination address for ZDMA0" },
	{ HOST_REGTYPE, "REG_ZDICNT0",	REG_ZDICNT0,	0, 0, "2'b[31:30]: DREQ(DMA request) source selection;\n2'b[29:28]: DREQ protocol;\n2'b[27:26]: Transfer mode;\n2'b[25:24]: On the fly mode;\n2'b[23:22]: Interrupt mode set;\n1'b[21]: Auto-reload and Auto-start after DMA count are 0;\n1'b[20]: DMA H/W enable/disable;\n20'b[19:0]: Initial/current transfer count for ZDMA0." },
	{ HOST_REGTYPE, "REG_ZDCSRC0",	REG_ZDCSRC0,	0, 0, "2'b[31:30]: Data size for transfer;\n2'b[29:28]: Direction of address for load;\n28'b[27:0]: Initial/current source address for ZDMA0." },
	{ HOST_REGTYPE, "REG_ZDCDES0",	REG_ZDCDES0,	0, 0, "2'b[31:30]: DMA internal options. OPT = 10 is recommended;\n2'b[29:28]: Direction of address for store;\n28'b[27:0]: Initial/current destination address for ZDMA0" },
	{ HOST_REGTYPE, "REG_ZDCCNT0",	REG_ZDCCNT0,	0, 0, "2'b[31:30]: DREQ(DMA request) source selection;\n2'b[29:28]: DREQ protocol;\n2'b[27:26]: Transfer mode;\n2'b[25:24]: On the fly mode;\n2'b[23:22]: Interrupt mode set;\n1'b[21]: Auto-reload and Auto-start after DMA count are 0;\n1'b[20]: DMA H/W enable/disable;\n20'b[19:0]: Initial/current transfer count for ZDMA0." }
};

const TRegUnit gZdma1Reg[] =
{
	{ HOST_REGTYPE, "REG_ZDCON1",	REG_ZDCON1,		0, 0, "2'b[7:6]: Reserved;\n2'b[5:4]: Status of DMA channel (Read only);\n2'b[3:2]: Disable/Enable External DMA request (nXDREQ);\n2'b[1:0]: Software commands." },
	{ HOST_REGTYPE, "REG_ZDISRC1",	REG_ZDISRC1,	0, 0, "2'b[31:30]: Data size for transfer;\n2'b[29:28]: Direction of address for load;\n28'b[27:0]: Initial/current source address for ZDMA1." },
	{ HOST_REGTYPE, "REG_ZDIDES1",	REG_ZDIDES1,	0, 0, "2'b[31:30]: DMA internal options. OPT = 10 is recommended;\n2'b[29:28]: Direction of address for store;\n28'b[27:0]: Initial/current destination address for ZDMA1." },
	{ HOST_REGTYPE, "REG_ZDICNT1",	REG_ZDICNT1,	0, 0, "2'b[31:30]: DREQ(DMA request) source selection;\n2'b[29:28]: DREQ protocol;\n2'b[27:26]: Transfer mode;\n2'b[25:24]: On the fly mode;\n2'b[23:22]: Interrupt mode set;\n1'b[21]: Auto-reload and Auto-start after DMA count are 0;\n1'b[20]: DMA H/W enable/disable;\n20'b[19:0]: Initial/current transfer count for ZDMA1." },
	{ HOST_REGTYPE, "REG_ZDCSRC1",	REG_ZDCSRC1,	0, 0, "2'b[31:30]: Data size for transfer;\n2'b[29:28]: Direction of address for load;\n28'b[27:0]: Initial/current source address for ZDMA1." },
	{ HOST_REGTYPE, "REG_ZDCDES1",	REG_ZDCDES1,	0, 0, "2'b[31:30]: DMA internal options. OPT = 10 is recommended;\n2'b[29:28]: Direction of address for store;\n28'b[27:0]: Initial/current destination address for ZDMA1" },
	{ HOST_REGTYPE, "REG_ZDCCNT1",	REG_ZDCCNT1,	0, 0, "2'b[31:30]: DREQ(DMA request) source selection;\n2'b[29:28]: DREQ protocol;\n2'b[27:26]: Transfer mode;\n2'b[25:24]: On the fly mode;\n2'b[23:22]: Interrupt mode set;\n1'b[21]: Auto-reload and Auto-start after DMA count are 0;\n1'b[20]: DMA H/W enable/disable;\n20'b[19:0]: Initial/current transfer count for ZDMA1." }
};

const TRegUnit gBdma0Reg[] =
{
	{ HOST_REGTYPE, "REG_BDCON0",	REG_BDCON0,		0, 0, "2'b[7:6]: Reserved;\n2'b[5:4]: Status of DMA channel (Read only);\n2'b[3:2]: Disable/Enable External/Internal DMA request (UARTn, SIO, IIS, Timer);\n2'b[1:0]: Software commands." },
	{ HOST_REGTYPE, "REG_BDISRC0",	REG_BDISRC0,	0, 0, "2'b[31:30]: Data size for transfer;\n2'b[29:28]: Direction of address for load;\n28'b[27:0]: Initial/current source address for BDMA0." },
	{ HOST_REGTYPE, "REG_BDIDES0",	REG_BDIDES0,	0, 0, "2'b[31:30]: Transfer direction mode;\n2'b[29:28]: Direction of address for store;\n28'b[27:0]: Initial/current destination address for BDMA0." },
	{ HOST_REGTYPE, "REG_BDICNT0",	REG_BDICNT0,	0, 0, "2'b[31:30]: DMA request source selection;\n2'b[29:28]: Reserved;\n2'b[27:26]: Reserved;\n2'b[25:24]: Reserved;\n2'b[23:22]: Interrupt mode set;\n1'b[21]: Auto-reload and Auto-start after DMA count are 0;\n1'b[20]: DMA H/W enable/disable;\n20'b[19:0]: Transfer count for BDMA0." },
	{ HOST_REGTYPE, "REG_BDCSRC0",	REG_BDCSRC0,	0, 0, "2'b[31:30]: Data size for transfer;\n2'b[29:28]: Direction of address for load;\n28'b[27:0]: Initial/current source address for BDMA0." },
	{ HOST_REGTYPE, "REG_BDCDES0",	REG_BDCDES0,	0, 0, "2'b[31:30]: Transfer direction mode;\n2'b[29:28]: Direction of address for store;\n28'b[27:0]: Initial/current destination address for BDMA0." },
	{ HOST_REGTYPE, "REG_BDCCNT0",	REG_BDCCNT0,	0, 0, "2'b[31:30]: DMA request source selection;\n2'b[29:28]: Reserved;\n2'b[27:26]: Reserved;\n2'b[25:24]: Reserved;\n2'b[23:22]: Interrupt mode set;\n1'b[21]: Auto-reload and Auto-start after DMA count are 0;\n1'b[20]: DMA H/W enable/disable;\n20'b[19:0]: Transfer count for BDMA0." }
};

const TRegUnit gBdma1Reg[] =
{
	{ HOST_REGTYPE, "REG_BDCON1",	REG_BDCON1,		0, 0, "2'b[7:6]: Reserved;\n2'b[5:4]: Status of DMA channel (Read only);\n2'b[3:2]: Disable/Enable External/Internal DMA request (UARTn, SIO, IIS, Timer);\n2'b[1:0]: Software commands." },
	{ HOST_REGTYPE, "REG_BDISRC1",	REG_BDISRC1,	0, 0, "2'b[31:30]: Data size for transfer;\n2'b[29:28]: Direction of address for load;\n28'b[27:0]: Initial/current source address for BDMA1." },
	{ HOST_REGTYPE, "REG_BDIDES1",	REG_BDIDES1,	0, 0, "2'b[31:30]: Transfer direction mode;\n2'b[29:28]: Direction of address for store;\n28'b[27:0]: Initial/current destination address for BDMA1." },
	{ HOST_REGTYPE, "REG_BDICNT1",	REG_BDICNT1,	0, 0, "2'b[31:30]: DMA request source selection;\n2'b[29:28]: Reserved;\n2'b[27:26]: Reserved;\n2'b[25:24]: Reserved;\n2'b[23:22]: Interrupt mode set;\n1'b[21]: Auto-reload and Auto-start after DMA count are 0;\n1'b[20]: DMA H/W enable/disable;\n20'b[19:0]: Transfer count for BDMA1." },
	{ HOST_REGTYPE, "REG_BDCSRC1",	REG_BDCSRC1,	0, 0, "2'b[31:30]: Data size for transfer;\n2'b[29:28]: Direction of address for load;\n28'b[27:0]: Initial/current source address for BDMA1." },
	{ HOST_REGTYPE, "REG_BDCDES1",	REG_BDCDES1,	0, 0, "2'b[31:30]: Transfer direction mode;\n2'b[29:28]: Direction of address for store;\n28'b[27:0]: Initial/current destination address for BDMA1." },
	{ HOST_REGTYPE, "REG_BDCCNT1",	REG_BDCCNT1,	0, 0, "2'b[31:30]: DMA request source selection;\n2'b[29:28]: Reserved;\n2'b[27:26]: Reserved;\n2'b[25:24]: Reserved;\n2'b[23:22]: Interrupt mode set;\n1'b[21]: Auto-reload and Auto-start after DMA count are 0;\n1'b[20]: DMA H/W enable/disable;\n20'b[19:0]: Transfer count for BDMA1." }
};

const TRegList gArm7Reglist[] =
{
	{ "System reg", (PTRegUnit)gSystemReg, sizeof(gSystemReg) / sizeof(TRegUnit) },
	{ "Cache reg", (PTRegUnit)gCacheReg, sizeof(gCacheReg) / sizeof(TRegUnit)	},
	{ "BusCtrl reg", (PTRegUnit)gBusCtrlReg, sizeof(gBusCtrlReg) / sizeof(TRegUnit) },
	{ "MemCtrl reg", (PTRegUnit)gMemCtrlReg, sizeof(gMemCtrlReg) / sizeof(TRegUnit) },
	{ "UART reg", (PTRegUnit)gUartReg, sizeof(gUartReg) / sizeof(TRegUnit) },
	{ "SIO reg", (PTRegUnit)gSioReg, sizeof(gSioReg) / sizeof(TRegUnit) },
	{ "IIS reg", (PTRegUnit)gIisReg, sizeof(gIisReg) / sizeof(TRegUnit) },
	{ "I/O Port reg", (PTRegUnit)gIoportReg, sizeof(gIoportReg) / sizeof(TRegUnit) },
	{ "Watchdog reg", (PTRegUnit)gWatchDogReg, sizeof(gWatchDogReg) / sizeof(TRegUnit) },
	{ "ADC reg", (PTRegUnit)gAdcReg, sizeof(gAdcReg) / sizeof(TRegUnit) },
	{ "Timer reg", (PTRegUnit)gTimerReg, sizeof(gTimerReg) / sizeof(TRegUnit) },

	{ "IIC reg", (PTRegUnit)gIicReg, sizeof(gIicReg) / sizeof(TRegUnit) },
	{ "RTC reg", (PTRegUnit)gRtcReg, sizeof(gRtcReg) / sizeof(TRegUnit)	},
	{ "CPM reg", (PTRegUnit)gCpmReg, sizeof(gCpmReg) / sizeof(TRegUnit) },
	{ "Interrupt reg", (PTRegUnit)gInterruptReg, sizeof(gInterruptReg) / sizeof(TRegUnit) },
	{ "LCD reg", (PTRegUnit)gLcdReg, sizeof(gLcdReg) / sizeof(TRegUnit) },
	{ "ZDMA0 reg", (PTRegUnit)gZdma0Reg, sizeof(gZdma0Reg) / sizeof(TRegUnit) },
	{ "ZDMA1 reg", (PTRegUnit)gZdma0Reg, sizeof(gZdma0Reg) / sizeof(TRegUnit) },
	{ "BDMA0 reg", (PTRegUnit)gBdma0Reg, sizeof(gBdma0Reg) / sizeof(TRegUnit) },
	{ "BDMA1 reg", (PTRegUnit)gBdma0Reg, sizeof(gBdma0Reg) / sizeof(TRegUnit) }
};

const int gArm7ReglistCount = sizeof(gArm7Reglist) / sizeof(TRegList);

