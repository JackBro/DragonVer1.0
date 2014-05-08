

#include "internal.h"

int	CStringTools::Str2Hex(char *str, int *x)
{
	int i, len, val;

	*x = 0;
	len = (int)strlen(str);
	for(i = 0; i < len; i++)
	{
		if(this->Char2Hex(str[i], &val))
			return 1;
		*x <<= 4;
		*x += val;
	}
	return 0;
}

int	CStringTools::Str2Dec(char *str, int *x)
{
	int i, len, val;

	*x = 0;
	len = (int)strlen(str);
	for(i = 0; i < len; i++)
	{
		if(this->Char2Dec(str[i], &val))
			return 1;
		*x *= 10;
		*x += val;
	}
	return 0;
}

int	CStringTools::Str2Oct(char *str, int *x)
{
	int i, len, val;

	*x = 0;
	len = (int)strlen(str);
	for(i = 0; i < len; i++)
	{
		if(this->Char2Dec(str[i], &val))
			return 1;
		if(val > 7)
			return 1;
		*x <<= 3;
		*x += val;
	}
	return 0;
}

void CStringTools::StrUpCase(char *str, char *dst)
{
	int i, len;

	len = (int)strlen(str);
	for(i = 0; i < len; i++)
		dst[i] = this->CharUpCase(str[i]);
	dst[len] = 0;
}

void CStringTools::StrCopyNull(char *src, char *dst)
{
	char str[10];

	if(strlen(src) > 4)
	{
		strcpy(dst, src);
		return;
	}
	this->StrUpCase(src, str);
	if(strcmp(str, "NULL") == 0)
		dst[0] = 0;
	else
		strcpy(dst, src);
}

void CStringTools::DerivePathTail(char *src, char *tail, char *dest)
{
	int pos, len;

	len = (int)strlen(src);
	if((len == 0) && (tail != NULL))
	{
		sprintf(dest, "%s", tail);
		return;
	}
	pos = this->FindLastChar(src, '\\');
	sprintf(dest, "%s", src);
	dest[pos] = 0;
	if(tail != NULL)
		sprintf(dest, "%s\\%s", dest, tail);
}

void CStringTools::AddIndex(char *src, char *dest)
{
	int len, pos, start, end;

	len = (int)strlen(src);
	pos = this->FindLastNum(src, &start, &end);
	if(end != len-1)
	{
		sprintf(dest, "%s0", src);
		return;
	}
	this->Str2Dec(src+start, &len);
	len++;
	memcpy(dest, src, start);
	dest[start] = 0;
	sprintf(dest, "%s%d", dest, len);
}

void CStringTools::AutoIndex2Str(char *src, char *tail, char *dest)
{
	int pos1, pos2, pos3;

	pos1 = (int)strlen(src);
	if(pos1 == 0)
		return;
	pos1 = this->FindLastChar(src, '.');
	pos2 = this->FindLastChar(src, '\\');
	pos3 = this->FindLastChar(src, '/');
	pos2 = pos2 > pos3 ? pos2 : pos3;
	strcpy(dest, src);
	if(pos1 > pos2)
		dest[pos1] = 0;
	this->AddIndex(dest, dest);
	if(tail)
		sprintf(dest, "%s.%s", dest, tail);
}

void CStringTools::AutoIndex2Str1(char *src, char *tail, char *dest)
{
	int i, len, pos[2], flag = 0;
	char str[100];

	len = (int)strlen(src);
	if( (len == 0) || (src == NULL) )
	{
		dest[0] = 0;
		return;
	}
	pos[0] = len;
	pos[1] = len;
	for(i = len - 1; i >= 0; i--)
	{
		if(src[i] == '.')
		{
			pos[1] = i;
			pos[0] = len;
			flag = BIT0;
			continue;
		}
		else if(src[i] == '\\')
			break;

	//	if( (src[i] >= '0') && (src[i] <= '9') )
		if(this->IsDigital(src[i]))
		{
			if( (flag & BIT2) == 0 )
			{
				pos[0] = i;
				flag |= BIT1;
			}
		}
		else if(flag & BIT0)
			break;
		else if(flag & BIT1)
			flag |= BIT2;

	}

	sprintf(str, src);
	str[pos[1]] = '\0';
	if( (flag & BIT1) == 0 )
	{
		sprintf(dest, "%s1.%s", str, tail);
		return;
	}
	this->Str2Dec(str+pos[0], &len);
	len += 1;
	str[pos[0]] = 0;
	sprintf(dest, "%s%d.%s", str, len, tail);
}

int	CStringTools::FindFirstChar(char *str, char ch, int len)
{
	int i;

	if(len == 0)
		len = (int)strlen(str);
	for(i = 0; i < len; i++)
	{
		if(str[i] == ch)
			break;
	}
	return i;
}

int	CStringTools::FindLastChar(char *str, char ch, int len)
{
	int i;

	if(len == 0)
		len = (int)strlen(str);
	for(i = len-1; i >= 0; i--)
	{
		if(str[i] == ch)
			break;
	}
	return i;
}

int CStringTools::FindFirstCR(char *str, int len)
{
	int i;

	if(len == 0)
		len = (int)strlen(str);
	for(i = 0; i < len; i++)
	{
		if( (str[i] == '\n') || (str[i] == '\r') )
			break;
	}
	return i;
}

int CStringTools::FindLastCR(char *str, int len)
{
	int i;

	if(len == 0)
		len = (int)strlen(str);
	for(i = len-1; i >= 0; i--)
	{
		if( (str[i] == '\n') || (str[i] == '\r') )
			break;
	}
	return i;
}

int CStringTools::FindFirstNum(char *str, int *start, int *end, int len)
{
	int i, flag = STRING_FAILED;

	*start = *end = 0;
	if(len == 0)
		len = (int)strlen(str);
	for(i = 0; i < len; i++)
	{
		if(this->IsDigital(str[i]))
	//	if( (str[i] >= '0') && (str[i] <= '9') )
		{
			if(flag == STRING_FAILED)
			{
				flag = STRING_SUCCEED;
				*start = i;
			}
		}
		else
		{
			if(flag == STRING_SUCCEED)
			{
				*end = i-1;
				break;
			}
		}
	}
	return flag;
}

int	CStringTools::FindLastNum(char *str, int *start, int *end, int len)
{
	int i, flag = STRING_FAILED;

	*start = *end = 0;
	if(len == 0)
		len = (int)strlen(str);
	for(i = len-1; i >= 0; i--)
	{
		if(this->IsDigital(str[i]))
	//	if( (str[i] >= '0') && (str[i] <= '9') )
		{
			if(flag == STRING_FAILED)
			{
				flag = STRING_SUCCEED;
				*end = i;
			}
		}
		else
		{
			if(flag == STRING_SUCCEED)
			{
				*start = i+1;
				break;
			}
		}
	}
	return flag;
}

void CStringTools::Val2Binery(int val, char *str, int unit)
{
	int i;

	str[0] = 0;
	for(i = (unit << 3) - 1; i >= 0; i--)
	{
		sprintf(str, "%s%d", str, ((val & (1 << i)) != 0));
	}
}

int	CStringTools::FindLocate(char *src, char *dst, int len)
{
	int i, j, flag = 0;

	for(i = 0; i < len; i++)
	{
		for(flag = 1, j = i; j < len; j++)
		{
			if(src[j] != dst[j-i])
			{
				flag = 0;
				break;
			}
		}
		if(flag)
			return i;
	}
	return i;
}
