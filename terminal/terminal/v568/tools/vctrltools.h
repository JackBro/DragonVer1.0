#ifndef _VCTRLTOOLS_H_
#define _VCTRLTOOLS_H_

#ifdef __cplusplus
extern "C" {
#endif

	/* // 568
	#define SMIC_PLL_M_MAX 513
	#define SMIC_PLL_M_MIN 2
	#define SMIC_PLL_N_MAX 33
	#define SMIC_PLL_N_MIN 2
	#define SMIC_PLL_NO_MAX 4
	#define SMIC_PLL_NO_MIN 0
	#define V5_CHIP_CLK_OUTDIV_MAX 254
	#define V5_CHIP_CLK_OUTDIV_MIN 2

	#define SMIC_PLL_IN_MAX 50000
	#define SMIC_PLL_IN_MIN 2000

	#define SMIC_PLL_OUTxNO_MAX 500000
	#define SMIC_PLL_OUTxNO_MIN 100000

	#define SMIC_PLL_INdivN_MAX 15000
	#define SMIC_PLL_INdivN_MIN 1000
	*/// 578

	#define SMIC_PLL_M_MAX 255
	#define SMIC_PLL_M_MIN 2
	#define SMIC_PLL_N_MAX 15
	#define SMIC_PLL_N_MIN 2
	#define SMIC_PLL_NO_MAX 4
	#define SMIC_PLL_NO_MIN 0
	#define V5_CHIP_CLK_OUTDIV_MAX 254
	#define V5_CHIP_CLK_OUTDIV_MIN 2

	#define SMIC_PLL_IN_MAX 375000
	#define SMIC_PLL_IN_MIN 2000

	#define SMIC_PLL_OUTxNO_MAX 1000000
	#define SMIC_PLL_OUTxNO_MIN 200000

	#define SMIC_PLL_INdivN_MAX 25000
	#define SMIC_PLL_INdivN_MIN 1000

	typedef struct _SMIC_PLL_PARM
	{
		UINT32 clkout;
		UINT8 m;
		UINT8 n;
		UINT8 od0;
		UINT8 od1;
		UINT8 outdiv;
	}SMIC_PLL_PARM, * PSMIC_PLL_PARM;


	UINT8 SMIC_PllCalculate(UINT32 clkin, UINT32 clkout, PSMIC_PLL_PARM pllparm);
	void SMIC_PllCalCheck(UINT32 clkin, PSMIC_PLL_PARM pllparm);


#ifdef __cplusplus
}
#endif

#endif



