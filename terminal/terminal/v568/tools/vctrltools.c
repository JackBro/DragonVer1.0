
#include "internal.h"

//========================
#ifdef WIN32
#include "ComChannel/simhost.h"
#else
#define SmPrintf(X)
#endif

/* // 568
static const UINT8 gSmicNo[][3] =
{
	//no, od0, od1
	{1, 0, 0 },
	{2, 1, 0 },
	{4, 1, 1 }
};
*/// 578

static const UINT8 gSmicNo[][4] =
{
	//no, od0, od1
	{1, 0, 0 },
	{2, 1, 0 },
	{4, 0, 1 },
	{8, 1, 1 }
};

/*Set n,m range*/
static void GetMNfield(UINT32 clkin, UINT8 *nmin, UINT8 *nmax, UINT8 *mmin, UINT8 *mmax)
{
	UINT32 x;

	if(0 == clkin)
		return;

	x = clkin/SMIC_PLL_INdivN_MAX;

	if(x < SMIC_PLL_N_MIN)
		x = SMIC_PLL_N_MIN;
	if(x > SMIC_PLL_N_MAX)
		x = SMIC_PLL_N_MAX;

	*nmin = (UINT8)x;

	x = clkin/SMIC_PLL_INdivN_MIN;

	if(x < SMIC_PLL_N_MIN)
		x = SMIC_PLL_N_MIN;
	if(x > SMIC_PLL_N_MAX)
		x = SMIC_PLL_N_MAX;

	*nmax = (UINT8)x;

	x = SMIC_PLL_OUTxNO_MAX*(*nmax);
	x /= clkin;

	if(x < SMIC_PLL_M_MIN)
		x = SMIC_PLL_M_MIN;
	if(x > SMIC_PLL_M_MAX)
		x = SMIC_PLL_M_MAX;

	*mmax = (UINT8)x;

	x = SMIC_PLL_OUTxNO_MIN*(*nmin);
	x /= clkin;

	if(x < SMIC_PLL_M_MIN)
		x = SMIC_PLL_M_MIN;
	if(x > SMIC_PLL_M_MAX)
		x = SMIC_PLL_M_MAX;

	*mmin = (UINT8)x;
}

/********************************************************************************
  Description:
	Calculate SMIC PLL clock
  Parameters:
		clkin   : input clock(KHZ)
		clkout  : request ouput clock(KHZ)
		pllparm : out pll parameter see as PSMIC_PLL_PARM
  Note:
		<TABLE>
		This function can make sure such conditions:

		clkout = (pllout/clkdiv)/2;
		pllout = clkin*(m/n)*(1/NO);

		clkin must >= 2000(KHZ) and <= 50000(KHZ);
		clkin/n must >= SMIC_PLL_INdivN_MIN and <= SMIC_PLL_INdivN_MAX;
		pllout*NO must >= 100000(KHZ) and <= 500000(KHZ);

		</TABLE>
  Returns: 1: find needed parameters, 0: not find
	void
  Remarks:
*********************************************************************************/
UINT8 SMIC_PllCalculate(UINT32 clkin, UINT32 clkout, PSMIC_PLL_PARM pllparm)
{
	UINT32 clkouttemp, clkdiff, temp;
	UINT8  nmin, nmax, mmin, mmax;
	UINT8  i, j, p, q;

	clkout <<= 1;
	clkdiff = clkout;

	if(clkin < SMIC_PLL_IN_MIN || clkin > SMIC_PLL_IN_MAX)
		return 0;

	GetMNfield(clkin, &nmin, &nmax, &mmin, &mmax);

	for(i=V5_CHIP_CLK_OUTDIV_MIN; i<V5_CHIP_CLK_OUTDIV_MAX; i++)
	{
		for(j=0; j<(sizeof(gSmicNo)/sizeof(UINT8[3])); j++)
		{
			for(p=nmin; p<nmax; p++)
			{
				for(q=mmin; q<mmax; q++)
				{
					clkouttemp = (((clkin*q)/p)/gSmicNo[j][0])/i;
					
					if(clkout == clkouttemp)
					{
						pllparm->clkout = (clkouttemp >> 1);
						pllparm->m = q;
						pllparm->n = p;
						pllparm->outdiv = i;
						pllparm->od0 = gSmicNo[j][1];
						pllparm->od1 = gSmicNo[j][2];

						return 1;
					}
					else
					{
						temp = clkouttemp > clkout ? (clkouttemp - clkout) : (clkout - clkouttemp);
						if( temp < clkdiff )
						{
							clkdiff = temp;
							pllparm->clkout = (clkouttemp >> 1);
							pllparm->m = q;
							pllparm->n = p;
							pllparm->outdiv = i;
							pllparm->od0 = gSmicNo[j][1];
							pllparm->od1 = gSmicNo[j][2];
						}
					}

				}//m

			}//n

		}//no

	}//outdiv

	return 0;
}

/********************************************************************************
  Description:
	Calculate SMIC PLL clock use user configration
  Parameters:
		clkin   : input clock(KHZ)
		pllparm : out(in) pll parameter see as PSMIC_PLL_PARM
  Note:
		<TABLE>
		This function can make sure such conditions:

		mclk = pllout/clkdiv/2;
		pllout = clkin*(m/n)*(1/NO);

		clkin must >= 2000(KHZ) and <= 50000(KHZ);
		clkin/n must >= 1000(KHZ) and <= 15000(KHZ);
		pllout*NO must >= 10000(KHZ) and <= 50000(KHZ);

		</TABLE>
  Returns:
	void
  Remarks:
*********************************************************************************/
void SMIC_PllCalCheck(UINT32 clkin, PSMIC_PLL_PARM pllparm)
{
	UINT8 nmin, nmax, mmin, mmax;
	UINT8 i, no;

	if(clkin < SMIC_PLL_IN_MIN)
		clkin = SMIC_PLL_IN_MIN;
	if(clkin > SMIC_PLL_IN_MAX)
		clkin = SMIC_PLL_IN_MAX;

	GetMNfield(clkin, &nmin, &nmax, &mmin, &mmax);

	if(pllparm->m < mmin)
		pllparm->m = mmin;
	if(pllparm->m > mmax)
		pllparm->m = mmax;

	if(pllparm->n < nmin)
		pllparm->n = nmin;
	if(pllparm->n > nmax)
		pllparm->n = nmax;

	if(pllparm->od0 > 1)
		pllparm->od0 = 1;
	
	if(pllparm->od1 > 1)
		pllparm->od1 = 1;
	
	if(pllparm->outdiv < V5_CHIP_CLK_OUTDIV_MIN)
		pllparm->outdiv = V5_CHIP_CLK_OUTDIV_MIN;
	if(pllparm->outdiv > V5_CHIP_CLK_OUTDIV_MAX)
		pllparm->outdiv = V5_CHIP_CLK_OUTDIV_MAX;

	no = 1;

	for(i=0; i< (pllparm->od0+pllparm->od1); i++)
	{
		no <<= 1;
	}

	pllparm->clkout = ((((clkin*pllparm->m)/pllparm->n)/no)/pllparm->outdiv) >> 1;
}
