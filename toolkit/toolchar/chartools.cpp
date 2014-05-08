
#include "internal.h"
//=============	class CCharTools ==============================

int	CCharTools::Char2Hex(char a, int *x)
{
	int val;

	if( (a >= '0') && (a <= '9') )
	{
		val = a - '0';
		*x = val;
		return 0;
	}
	if( (a >= 'a') && (a <= 'f') )
	{
		val = a - 'a';
		val += 10;
		*x = val;
		return 0;
	}
	if( (a >= 'A') && (a <= 'F') )
	{
		val = a - 'A';
		val += 10;
		*x = val;
		return 0;
	}
	return 1;
}

int	CCharTools::Char2Dec(char a, int *x)
{
	int val;

	if(this->IsDigital(a))
	{
		val = a - '0';
		*x = val;
		return 0;
	}
	return 1;
}

char CCharTools::CharUpCase(char a)
{
	if( (a >= 'a') && (a <= 'z') )
		return (a-0x20);
	return a;
}

int	CCharTools::IsDigital(char a)
{
	if( (a >= '0') && (a <= '9') )
		return 1;
	return 0;
}

int	CCharTools::IsAlpha(char a)
{
	if( ((a >= 'a') && (a <= 'z')) || ((a >= 'A') && (a <= 'Z')) 
		|| (a == '_') )
		return 1;
	return 0;
}

int	CCharTools::IsOpcode(char a)
{
	if( (a == '=') || (a == '+') || (a == '-') || (a == '*') || (a == '/') ||
		(a == '%') || (a == '&') || (a == '!') || (a == '<') || (a == '>') ||
		(a == '~') || (a == '|') || (a == '?') || (a == '^') || ( a == ':' ) )
		return 1;
	return 0;
}

int	CCharTools::IsBlank(char a)
{
	if( (a == ' ') || (a == '\t') )
		return 1;
	return 0;
}

