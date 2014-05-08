#ifndef __TB_DEF_H
#define __TB_DEF_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef	unsigned int UINT ;

//definition for DLY unit
#define		DLY_ERR	0
#define		DLY_MS	1
#define		DLY_US	2
#define		DLY_NS	3
#define		DLY_CLK	4

//definition for CFG command
#define		BANKCON1_ADDR		0x01C80008		//Bank 1 control register
#define		BANKCON2_ADDR		0x01C8000C		//Bank 2 control register
#define		BANKCON3_ADDR		0x01C80010		//Bank 3 control register
#define		BANKCON4_ADDR		0x01C80014		//Bank 4 control register
#define		BANKCON5_ADDR		0x01C80018		//Bank 5 control register
#define		EXTINT_ADDR			0x01d20050		//External Interrupt Control
#define		EXTINPND_ADDR		0x01d20054		//External Interrupt Pending
#define		INTCON_ADDR			0x01E00000		//Interrupt Control
#define		INTPND_ADDR			0x01e00004		//Interrupt Request Status
#define		INTMOD_ADDR			0x01e00008		//Interrupt Mode Control
#define		INTMSK_ADDR			0x01e0000c		//Interrupt Mask Control
#define		I_ISPR_ADDR			0x01e00020		//IRQ Interrupt Pending Status
#define		I_ISPC_ADDR			0x01e00024		//IRQ Interrupt Pending Clear
#define		F_ISPR_ADDR			0x01e00038		//FIQ Interrupt Pending
#define		F_ISPC_ADDR			0x01e0003c		//FIQ Interrupt Pending Clear
#define		I_INUM_ADDR			0x01e00100		//IRQ Interrupt Number
#define		I_IEDG_ADDR			0x01e00104		//IRQ Interrupt edge select
#define		I_IAST_ADDR			0x01e00108		//IRQ Interrupt active state
#define		I_PSLV_ADDR			0x01e00010		//IRQ Interrupt Previous Slave
#define		I_PMST_ADDR			0x01e00014		//IRQ Interrupt Priority Master
#define		I_CSLV_ADDR			0x01e00018		//IRQ Interrupt Current Slave
#define		I_CMST_ADDR			0x01e0001c		//IRQ Interrupt Current Master

#define		debug_ADDR			1
#define		BO_ADDR				2
#define		TEST_ADDR			3
#define		CONF_ADDR			4
#define		GPIO_ADDR			5
#define		Tas_ADDR			6
#define		Tah_ADDR			7
#define		Tcs_we_ADDR			8
#define		Twe_cs_ADDR			9
#define		Twds_ADDR			10
#define		Twdh_ADDR			11
#define		Tww_ADDR			12
#define		Tcs_rd_ADDR			13
#define		Trd_cs_ADDR			14
#define		Tacc_ADDR			15
#define		Trw_ADDR			16
#define		Trdh_ADDR			17


//definition for CALL command
#define		DEF_RESET			1	/*cmd type*/

#define		DEF_STOP			2	/*cmd type*/

#define		DEF_FINISH			3	/*cmd type*/

#define		DEF_DUMP_ON			4	/*cmd type*/
#define		DEF_U_TOP			1	/*DUMP_ON type*/
#define		DEF_U_BIU			2	/*DUMP_ON type*/
#define		DEF_U_CPM			3	/*DUMP_ON type*/
#define		DEF_U_SIF			4	/*DUMP_ON type*/
#define		DEF_U_ISP			5	/*DUMP_ON type*/
#define		DEF_U_IPP			6	/*DUMP_ON type*/
#define		DEF_U_LCDC			7	/*DUMP_ON type*/
#define		DEF_U_LCDIF			8	/*DUMP_ON type*/
#define		DEF_U_MARB			9	/*DUMP_ON type*/
#define		DEF_U_LBUF			10	/*DUMP_ON type*/
#define		DEF_U_JPEG			11	/*DUMP_ON type*/
#define		DEF_U_GE			12	/*DUMP_ON type*/
#define		DEF_U_SRAMTOP_6T	13	/*DUMP_ON type*/
#define		DEF_U_SRAMTOP_1T	14	/*DUMP_ON type*/
#define		DEF_U_PAD_CTL		15	/*DUMP_ON type*/
#define		DEF_U_TEST_CTL		16	/*DUMP_ON type*/
#define		DEF_U_UMONITOR		17	/*DUMP_ON type*/

#define		DEF_DUMP_OFF		5	/*cmd type*/

#define		DEF_CHIP			6	/*cmd type*/
#define		DEF_VC0568			1	/*CHIP type*/
#define		DEF_VC0558			2	/*CHIP type*/
#define		DEF_VC0938			3	/*CHIP type*/

#define		DEF_DEVICE			7	/*cmd type*/
#define		DEF_BIU				1	/*DEVICE type*/
#define		DEF_USB				2	/*DEVICE type*/
#define		DEF_LCD				3	/*DEVICE type*/
#define		DEF_SENSOR			4	/*DEVICE type*/
#define		DEF_SDCARD			5	/*DEVICE type*/

#define		DEF_SET_CLK			8	/*cmd type*/


//definition for reg_bytes
#define		DEF_REG_BYTE		1	/*access 1 byte  address*/
#define		DEF_REG_WORD		2	/*access 2 bytes address*/
#define		DEF_REG_DWORD		4	/*access 4 bytes address*/


#endif
