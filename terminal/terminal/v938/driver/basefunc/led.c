#include "basefunc938.h"

#ifndef	V938CMD_DISABLE


void  V9B_LedSetTimSlic(UINT16 val)
{
	V9_SetReg(V9_REG_PAGE5_REG_TIMESLICE_HIGH, val, BIT_16_HTOL);
}

UINT16 V9B_EqGetTimSlic(void)
{
	UINT16 val;

	val = (UINT16)V9_GetReg(V9_REG_PAGE5_REG_TIMESLICE_HIGH, BIT_16_HTOL);

	return val;
}

void  V9B_LedSetPwmRLed(UINT16 val)
{
	V9_SetReg(V9_REG_PAGE5_REG_PWM_RLED_HIGH, val, BIT_16_HTOL);
}

UINT16 V9B_EqGetPwmRLed(void)
{
	UINT16 val;

	val = (UINT16)V9_GetReg(V9_REG_PAGE5_REG_PWM_RLED_HIGH, BIT_16_HTOL);

	return val;
}

void  V9B_LedSetPwmBLed(UINT16 val)
{
	V9_SetReg(V9_REG_PAGE5_REG_PWM_BLED_HIGH, val, BIT_16_HTOL);
}

UINT16 V9B_EqGetPwmBLed(void)
{
	UINT16 val;

	val = (UINT16)V9_GetReg(V9_REG_PAGE5_REG_PWM_BLED_HIGH, BIT_16_HTOL);

	return val;
}

void  V9B_LedSetPwmGLed(UINT16 val)
{
	V9_SetReg(V9_REG_PAGE5_REG_PWM_GLED_HIGH, val, BIT_16_HTOL);
}

UINT16 V9B_EqGetPwmGLed(void)
{
	UINT16 val;

	val = (UINT16)V9_GetReg(V9_REG_PAGE5_REG_PWM_GLED_HIGH, BIT_16_HTOL);

	return val;
}

void  V9B_LedSetMotConf(UINT8 val)
{
	V9_SetReg(V9_REG_PAGE5_REG_LEDMOT_CONF, val, BIT_8);
}

UINT8 V9B_EqGetMotConf(void)
{
	UINT8 val;

	val = (UINT8)V9_GetReg(V9_REG_PAGE5_REG_LEDMOT_CONF, BIT_8);

	return val;
}

void  V9B_LedSetCirPeriod(UINT8 val)
{
	V9_SetReg(V9_REG_PAGE5_REG_CIRCLING_PERIOD, val, BIT_8);
}

UINT8 V9B_EqGetCirPeriod(void)
{
	UINT8 val;

	val = (UINT8)V9_GetReg(V9_REG_PAGE5_REG_CIRCLING_PERIOD, BIT_8);

	return val;
}

void  V9B_LedSetIdx(UINT8 val)
{
	V9_SetReg(V9_REG_PAGE5_REG_INDEX, val, BIT_8);
}

UINT8 V9B_EqGetIdx(void)
{
	UINT8 val;

	val = (UINT8)V9_GetReg(V9_REG_PAGE5_REG_INDEX, BIT_8);

	return val;
}

void  V9B_LedSetPattern0(UINT16 val)
{
	V9_SetReg(V9_REG_PAGE5_REG_PATTERN0_HIGH, val, BIT_16_HTOL);
}

UINT16  V9B_LedGetPattern0(void)
{
	UINT16 val;

	val = (UINT16)V9_GetReg(V9_REG_PAGE5_REG_PATTERN0_HIGH, BIT_16_HTOL);

	return val;
}

void  V9B_LedSetPattern1(UINT16 val)
{
	V9_SetReg(V9_REG_PAGE5_REG_PATTERN1_HIGH, val, BIT_16_HTOL);
}

UINT16  V9B_LedGetPattern1(void)
{
	UINT16 val;

	val = (UINT16)V9_GetReg(V9_REG_PAGE5_REG_PATTERN1_HIGH, BIT_16_HTOL);

	return val;
}

void  V9B_LedSetPattern2(UINT16 val)
{
	V9_SetReg(V9_REG_PAGE5_REG_PATTERN2_HIGH, val, BIT_16_HTOL);
}

UINT16  V9B_LedGetPattern2(void)
{
	UINT16 val;

	val = (UINT16)V9_GetReg(V9_REG_PAGE5_REG_PATTERN2_HIGH, BIT_16_HTOL);

	return val;
}

void  V9B_LedSetPattern3(UINT16 val)
{
	V9_SetReg(V9_REG_PAGE5_REG_PATTERN3_HIGH, val, BIT_16_HTOL);
}

UINT16  V9B_LedGetPattern3(void)
{
	UINT16 val;

	val = (UINT16)V9_GetReg(V9_REG_PAGE5_REG_PATTERN3_HIGH, BIT_16_HTOL);

	return val;
}

void  V9B_LedSetPattern4(UINT16 val)
{
	V9_SetReg(V9_REG_PAGE5_REG_PATTERN4_HIGH, val, BIT_16_HTOL);
}

UINT16  V9B_LedGetPattern4(void)
{
	UINT16 val;

	val = (UINT16)V9_GetReg(V9_REG_PAGE5_REG_PATTERN4_HIGH, BIT_16_HTOL);

	return val;
}

void  V9B_LedSetPattern5(UINT16 val)
{
	V9_SetReg(V9_REG_PAGE5_REG_PATTERN5_HIGH, val, BIT_16_HTOL);
}

UINT16  V9B_LedGetPattern5(void)
{
	UINT16 val;

	val = (UINT16)V9_GetReg(V9_REG_PAGE5_REG_PATTERN5_HIGH, BIT_16_HTOL);

	return val;
}

void  V9B_LedSetPattern6(UINT16 val)
{
	V9_SetReg(V9_REG_PAGE5_REG_PATTERN6_HIGH, val, BIT_16_HTOL);
}

UINT16  V9B_LedGetPattern6(void)
{
	UINT16 val;

	val = (UINT16)V9_GetReg(V9_REG_PAGE5_REG_PATTERN6_HIGH, BIT_16_HTOL);

	return val;
}

void  V9B_LedSetPattern7(UINT16 val)
{
	V9_SetReg(V9_REG_PAGE5_REG_PATTERN7_HIGH, val, BIT_16_HTOL);
}

UINT16  V9B_LedGetPattern7(void)
{
	UINT16 val;

	val = (UINT16)V9_GetReg(V9_REG_PAGE5_REG_PATTERN7_HIGH, BIT_16_HTOL);

	return val;
}


#endif

