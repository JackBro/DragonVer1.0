
#include "internal.h"



CBatchParse::CBatchParse(char *filename, int size) : CBufferBase(size)//, m_dbug(NULL, 0, 1)
{
	this->m_keyreg = BATCH_DEFAULT_KEYREG;
	this->m_keystr = BATCH_DEFAULT_KEYSTR;
	this->m_fp = NULL;
	this->OpenBatchFile(filename);
}

CBatchParse::~CBatchParse()
{
	this->CloseBatch();
}

void CBatchParse::CloseBatch(void)
{
	if(this->m_fp)
		fclose(this->m_fp);
	this->m_len = 0;
	this->m_pos = 0;
	this->m_filepos = 0;
	this->m_line = 0;
}

void CBatchParse::OpenBatchFile(char *filename)
{
	this->CloseBatch();
	if(filename == NULL)
		return;
	if(filename[0] == '\0')
		return;
	this->m_fp = fopen(filename, "rb");
}

void CBatchParse::SetKey2Type(char keyreg, char keystr)
{
	this->m_keyreg = keyreg;
	this->m_keystr = keystr;
}

int CBatchParse::GetContent(void)
{
	if(!this->m_fp)
		return 0;

	fseek(this->m_fp, this->m_filepos, SEEK_SET);
	this->m_len = (int)fread(this->m_buf, 1, this->m_totalsize, this->m_fp);
	if(this->m_len == 0)
	{
		fclose(this->m_fp);
		this->m_fp = NULL;
		return 0;
	}
	if(this->m_len < this->m_totalsize)
	{
		fclose(this->m_fp);
		this->m_fp = NULL;
	}
	while( (this->m_buf[this->m_len-1] != '\n') && (this->m_fp) )
	{
		if(this->m_len)
			this->m_len--;
		else
		{
			this->m_dbug.PrintMsg("Batch pools too small\n");
			if(this->m_fp)
			{
				fclose(this->m_fp);
				this->m_fp = NULL;
			}
			return 0;
		}
	}
	this->m_filepos += this->m_len;
	this->m_pos = 0;
	return 1;
}

int	CBatchParse::EncountComment(void)
{
	while( (this->m_buf[this->m_pos] != '\n') && (this->m_pos < this->m_len) )
		this->m_pos++;
	return 0;
}

int	CBatchParse::GetLineItems(char **ptr, int total)
{
	int index = 0;
	int initem = 0;
	int flag = 1;

	if(this->m_pos >= this->m_len)
		return 0;

	while(flag)
	{
		switch(this->m_buf[this->m_pos])
		{
		case ' ':
		case '\t':
			if( !initem )
				break;

			this->m_buf[this->m_pos] = 0;
			if(index < total)
			{
				index++;
				initem = 0;
				break;
			}
			flag = this->EncountComment();
			break;

		case '/':		
			if( (this->m_pos+1 >= this->m_len) || (this->m_buf[this->m_pos+1] != '/') )
				break;
		case ';':		//comments
		case '\r':
			this->m_buf[this->m_pos] = 0;
			if(initem)
			{
				initem = 0;
				index++;
			}
			flag = this->EncountComment();
			break;

		default:
			if( !initem )
			{
				ptr[index] = this->m_buf + this->m_pos;
				initem = 1;
			}
			break;
		}
		this->m_pos++;
		if(this->m_pos >= this->m_len)
		{
			this->m_buf[this->m_pos] = 0;
			if(initem)
			{
				initem = 0;
				if(index < total)
					index++;
			}
			flag = 0;
		}
	}
	this->m_line++;
	return index;
}

int	CBatchParse::GetItemType(char **ptr, int total)
{
	int len, i, j = 0;

	while(j < total)
	{
		len = (int)strlen(ptr[j]);
		i = 0;
		while(i < len)
		{
			if(ptr[j][i] == this->m_keyreg)
				return BATCH_REGTYPE;
			if(ptr[j][i] == this->m_keystr)
				return BATCH_STRINGTYPE;
			i++;
		}
		j++;
	}
	return BATCH_UNKNOWTYPE;
}

int	CBatchParse::ParseRegUnitItem(char **ptr, int total, char *punit)
{
	int count, flag = 0;
	TRegUnit *unit = (TRegUnit *)punit;
	CStringTools tools;

	unit->regtype = V558_REGTYPE;
	unit->delay = 0;
	count = this->DeleteMark(ptr, &total, '#');
	if(count == 1)
	{
		unit->regtype = SENSOR_REGTYPE;
	}

	else
	{
	    if(this->DeleteMark(ptr, &total, BATCH_REGTYPE_BY16_REG) == 1)
		{
			unit->regtype = LCD_BY_SETREG;
		}
	}

	if(this->DeleteMark(ptr, &total, this->m_keyreg) != 1)
		return BATCH_FAILED;
	if(this->DeleteMark(ptr, &total, '~') == 1)	//delay flag
		flag = 1;

	if(total < 2)
		return BATCH_FAILED;
	if(flag)
		if(total < 3)
			return BATCH_FAILED;

	if( tools.Str2Hex(ptr[0], &(unit->regval)) )
		return BATCH_FAILED;
	if( tools.Str2Hex(ptr[1], &(unit->regaddr)) )
		return BATCH_FAILED;

	if(flag)
		if( tools.Str2Hex(ptr[2], &(unit->delay)) )
			return BATCH_FAILED;

	return BATCH_WRITE_REG;
}

int CBatchParse::DeleteMark(char **ptr, int *total, char ch, int maxv)
{
	int i, j, k, len;
	int flag = 0;

	if(*total == maxv)
		return flag;
	for(k = 0; k < *total; k++)
	{
		len = (int)strlen(ptr[k]);
		i = 0;
		while(i < len)
		{
			if(ptr[k][i] == ch)
			{
				flag++;
				ptr[k][i] = '\0';
				if(i == 0)
				{
					if(len == 1)
					{
						for(j = k; j < *total; j++)
							ptr[j] = ptr[j+1];
						*total -= 1;
						k--;
					}
					else
					{
						ptr[k] += 1;
						i = -1;
						len = (int)strlen(ptr[k]);
					}
				}
				else if(i != len-1)
				{
					for(j = *total; j > k+1; j--)
						ptr[j] = ptr[j-1];
					ptr[k+1] = ptr[k]+i+1;
					*total += 1;
					if(*total == maxv)
						return flag;
				}
			}
			i++;
		}
	}
	return flag;
}

//int	CBatchParse::ParseStringItem(char **ptr, int total, TKeyStringMap *unit)
//{
//	int i = 0;
//	CStringTools tools;
//
//	this->DeleteMark(ptr, &total, this->m_keystr);
//	for(i = 0; i < gKeyCount; i++)
//	{
//		if(strcmp(ptr[0], gKeyMap[i].keychar) == 0)
//		{
//			if(total == 1)
//				unit->value[0] = 0;
//			else
//				tools.StrCopyNull(ptr[1], unit->value);
//			unit->index = i;
//			return BATCH_STRING;
//		}
//	}
//	return BATCH_FAILED;
//}

int CBatchParse::GetNextUnit(char *unit)
{
	int count, type;
	char *ptr[BATCH_MAXLINEITEM];

	if( (this->m_pos == this->m_len) && (!this->m_fp) )
		return BATCH_FAILED;
	if(unit == NULL)
		return BATCH_FAILED;

	do {
		while(this->m_pos < this->m_len)
		{
			count = this->GetLineItems(ptr, BATCH_MAXLINEITEM);
			if(count)
			{
				type = this->GetItemType(ptr, count);
				switch(type)
				{
				case BATCH_REGTYPE:
					count = this->ParseRegUnitItem(ptr, count, unit);
					break;
				case BATCH_STRINGTYPE:
				//	count = this->ParseStringItem(ptr, count, (TKeyStringMap *)unit);
				//	break;
				default:
					count = BATCH_FAILED;
					break;
				}
				if(count == BATCH_FAILED)
					this->m_dbug.PrintMsg("Line %d has error!\n", this->m_line);
				return count;
			}
		}
	} while(this->GetContent());

	return BATCH_OVER;
}




