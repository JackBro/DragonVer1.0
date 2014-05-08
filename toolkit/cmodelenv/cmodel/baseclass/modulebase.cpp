

#include "modulebase.h"
#include "../../../toolfile/dbugtools.h"

//==================== class CModuleBase ===================

CModuleBase::CModuleBase(TModulePara para)
{
	this->m_pclk = NULL;
	this->m_pchip = NULL;
	this->m_pin = NULL;
	this->m_preg = NULL;
	this->ChipInit(para);

	this->m_state = 0;
}

CModuleBase::~CModuleBase(void)
{
	if(this->m_pin)
		delete [](this->m_pin);
}

void CModuleBase::ChipInit(TModulePara para)
{
	TModuleMap map;

	this->m_id = para.id;
	this->m_base = para.base;
	this->m_modules.SetSize(para.module);

	map.pmodule = this;
	this->m_modules.Write(&map, 0);

	this->m_inputpin.SetSize(para.inputpin);
	this->m_outputpin.SetSize(para.outputpin);

	this->m_start = 0;
	this->m_target = 0;
}

void CModuleBase::InitConnectMap(void)
{
	int i;

	if(this->m_pin)
		delete [](this->m_pin);
	this->m_pin = NULL;
	this->m_pin = new CPinBase[gV568MapCount];
	if(this->m_pin == NULL)
		return;

	for(i = 0; i < gV568MapCount; i++)
	{
		this->ConnectModule( (gV568Map+i), this->m_pin+i );
		this->m_pin[i].SetDelayCount(gV568Map[i].delay);

		this->m_pin[i].SetPinId(i);
	}
}

void CModuleBase::InitProcess(void)
{
	this->InitModuleIndex();
}

void CModuleBase::SetClkModule(CModuleBase *pmodule)
{
	this->m_pclk = pmodule;
}

void CModuleBase::SetMainChip(CModuleBase *pmodule)
{
	this->m_pchip = pmodule;
}

void CModuleBase::SetReg(int adr, int val)
{
	adr &= REG_GROUP_COUNT-1;
	adr |= this->m_base;
	this->m_preg->Write(adr, val, CHIP_MODIFY_REG);
}

int CModuleBase::GetReg(int adr)
{
	adr &= REG_GROUP_COUNT-1;
	adr |= this->m_base;
	return this->m_preg->Read(adr);
}

void CModuleBase::UpdateReg(int adr)
{
	adr |= this->m_base;
	return this->m_preg->Update(adr);
}

void CModuleBase::SetRegPointer(CRegister *preg)
{
	int i, size = this->m_modules.GetSize();
	TModuleMap map;

	for(i = 0; i < size; i++)
	{
		if(this->m_modules.Read(&map, i) == 0)
			continue;
		if(map.pmodule == NULL)
			continue;
		if(map.pmodule == this)
			this->m_preg = preg;
		else
			map.pmodule->SetRegPointer(preg);
	}
}

void CModuleBase::InitRegProp1(void)
{
	int i, size = this->m_modules.GetSize();
	TModuleMap map;

	for(i = 0; i < size; i++)
	{
		if(this->m_modules.Read(&map, i) == 0)
			continue;
		if(map.pmodule == NULL)
			continue;
		if(map.pmodule == this)
			this->InitRegProp();
		else
			map.pmodule->InitRegProp1();
	}
}

void CModuleBase::InitSimReg(void)
{
//	this->m_reg.Write(REG_LOOPTEST | BASE_V568ADDR, 1, MODIFY_REG);
	this->m_reg.Write(REG_SAVEREGLIST | BASE_V568ADDR, 1, MODIFY_REG);
}

void CModuleBase::InitRegister(void)
{
	this->m_reg.SetSize(TOTALREGISTER);
	this->SetRegPointer(&(this->m_reg));
	this->InitRegProp1();
	this->InitSimReg();
}

void CModuleBase::SetConfigFile(char *cfgfile)
{
	char unit[0x100];
	TRegUnit *punit = (TRegUnit *)unit;
	TKeyStringMap *map = (TKeyStringMap *)unit;
	int flag = 1, temp;
	CBatchParse parse;

	this->InitRegister();
	this->InitConnectMap();
	if(this->m_pclk)
		this->m_pclk->SetMainChip(this);

	parse.OpenBatchFile(cfgfile);
	while(flag)
	{
		temp = parse.GetNextUnit(unit);
		switch(temp)
		{
		case BATCH_WRITE_REG:
			this->SetRegister(punit->regaddr, punit->regval);
			break;
		case BATCH_STRING:
			this->SetDbugPara(map);
			break;
		case BATCH_OVER:
		case BATCH_FAILED:
		default:
			flag = 0;
			break;
		}
	}
	this->ResetModule();
}

void CModuleBase::Disconnect(int id, int val)
{
	int i, size = this->m_modules.GetSize(), j;
	TModuleMap map;

	for(i = 0; i < size; i++)
	{
		if(this->m_modules.Read(&map, i) == 0)
			continue;
		if(map.pmodule == NULL)
			continue;
		if(map.pmodule == this)
		{
			if(id != this->m_id)
				continue;
			if(val & MODEL_ENBIT)
				this->Disconnect();
			for(j = 0; j < 3; j++)
			{
				if( val & (1 << j) )
					this->DeleteInputPin(j);
				if(val & (8 << j))
					this->DeleteOutputPin(j);
			}
		}
		else
			map.pmodule->Disconnect(id, val);
	}
}

void CModuleBase::SetStartTarget(int id, int val)
{
	int i, size = this->m_modules.GetSize();
	TModuleMap map;

	for(i = 0; i < size; i++)
	{
		if(this->m_modules.Read(&map, i) == 0)
			continue;
		if(map.pmodule == NULL)
			continue;
		if(map.pmodule == this)
		{
			if(id != this->m_id)
				continue;
			this->m_start = val & 0xf;
			this->m_target = (val >> 4) & 0xf;
		}
		else
			map.pmodule->SetStartTarget(id, val);
	}
}

void CModuleBase::SetRegister(int adr, int val)
{
	int base;

	this->SaveRegList(adr, val);
	base = adr & REG_GROUP_MASK;
	if(base == BASE_V568ADDR)
	{
		this->m_reg.Write(adr, val, MODIFY_REG);
		adr &= REG_GROUP_COUNT - 1;
		if( (adr >= IDD_JPEG) && (adr <= IDD_AUDIOADC) )
			this->Disconnect(adr, val);
		else if( (adr >= 0x100+IDD_JPEG) && (adr <= 0x100+IDD_AUDIOADC) )
			this->SetStartTarget(adr - 0x100, val);
	}
	else
	{
		this->m_reg.Write(adr, val);
		this->SetRegister1(adr, val);
	}
}

void CModuleBase::SetRegister1(int adr, int val)
{
	int base = adr & REG_GROUP_MASK;
	int i, size = this->m_modules.GetSize();
	TModuleMap map;

	for(i = 0; i < size; i++)
	{
		if( this->m_modules.Read(&map, i) == 0)
			continue;
		if(map.pmodule == NULL)
			continue;
		if(map.pmodule == this)
		{
			if(base == this->m_base)
				this->OnWriteReg(adr & REG_GROUP_COUNT - 1, val);
		}
		else
			map.pmodule->SetRegister1(adr, val);
	}
}

void CModuleBase::SaveRegList(int adr, int val)
{
	char str[100];

	if(this->m_reg.Read(REG_SAVEREGLIST | BASE_V568ADDR))
	{
		sprintf(str, "%02x@%04x\n", val, adr);
		val = (int)strlen(str);
		this->m_reglist.Write(str, val);
	}
}

int	CModuleBase::GetRegister(int adr)
{
	return this->m_reg.Read(adr);
}

void CModuleBase::GenerateCfgFile(char *cfgfile)
{
	FILE *fp = NULL;
	int i = 0;
	CDbugTools	dbug;

	fp = fopen(cfgfile, "wt");
	if(fp == NULL)
		return;
	for(i = 0; i < gKeyCount; i++)
	{
		if(gKeyMap[i].value[0] == '\0')
			fprintf(fp, "%s\t=\tNULL\n", gKeyMap[i].keychar);
		else
			fprintf(fp, "%s\t=\t%s\n", gKeyMap[i].keychar, gKeyMap[i].value);
	}
	fclose(fp);
	dbug.PrintMsg("Generator config file: %s\n\n", cfgfile);
}

// the format of the "sel"
//	BIT15						BIT7			
//			module ID			Inputtype	pin number
//			

int CModuleBase::FindPin(int sel, TPinMap *pinmap)
{
	int id;
	TModuleMap map;
	int i, size = this->m_modules.GetSize();

	for(i = 0; i < size; i++)
	{
		if( this->m_modules.Read(&map, i) == 0 )
			continue;
		if(map.pmodule == NULL)
			continue;
		if(map.pmodule == this)
		{
			id = sel >> 8;
			if( (this->m_id) == id )
			{
				if(sel & INPUT_PIN_TYPE)	//inputpin type
				{
					if(this->m_inputpin.Read(pinmap, sel & PIN_MASK) == 0)
						return 0;
				}
				else
				{
					if(this->m_outputpin.Read(pinmap, sel & PIN_MASK) == 0)
						return 0;
				}
				return 1;
			}
		}
		else if(map.pmodule->FindPin(sel, pinmap))
			return 1;
	}

	return 0;
}

void CModuleBase::SetDbugPara(TKeyStringMap *map)
{
	CDbugTools	dbug;
	TPinMap pinmap;

	strcpy(gKeyMap[map->index].value, map->value);
	if(map->index == SAVE_REGLIST)
		this->m_reglist.Open(gKeyMap[OUTPUT_DIRECTORY].value, gKeyMap[map->index].value, BUFFER_DEFAULT_SIZE);

	if( gKeyMap[map->index].select == (IDD_TOTAL << 8) )
		return;
	if( this->FindPin(gKeyMap[map->index].select, &pinmap) == 0 )
		return;
	if(pinmap.pin)
	{
		if(gKeyMap[map->index].type == INPUTFILE)
			pinmap.pin->m_input.Open(gKeyMap[INPUT_DIRECTORY].value, gKeyMap[map->index].value);
		else if(gKeyMap[map->index].type == OUTPUTFILE)
		{
			//FILE *fp;
			//fp = fopen("pincontext.dat", "at");
			//fprintf(fp, "Sel = 0x%x, Pin id: 0x%x, file: %s\n", gKeyMap[map->index].select,
			//	pinmap.pin->m_id, gKeyMap[map->index].value);
			//fclose(fp);
			pinmap.pin->m_dbug.Open(gKeyMap[OUTPUT_DIRECTORY].value, gKeyMap[map->index].value, BUFFER_DEFAULT_SIZE);
		}
	}
}

void CModuleBase::SetInputPin(CPinBase *ppin, int index)
{
	TPinMap map;

	map.pin = ppin;
//	ppin->m_src = this;
	this->m_inputpin.Write(&map, index);
}

void CModuleBase::SetOutputPin(CPinBase *ppin, int index)
{
	TPinMap map;

	map.pin = ppin;
//	ppin->m_dst = this;
	this->m_outputpin.Write(&map, index);
}

void CModuleBase::DeleteInputPin(CPinBase *ppin)
{
	this->m_inputpin.DeleteModule(ppin);
//	ppin->m_src = NULL;
}

void CModuleBase::DeleteOutputPin(CPinBase *ppin)
{
	this->m_outputpin.DeleteModule(ppin);
//	ppin->m_dst = NULL;
}

void CModuleBase::DeleteInputPin(int index)
{
	TPinMap map;

	if(this->m_inputpin.Read(&map, index) == 0)
		return;
	if(map.pin == NULL)
		return;
//	map.pin->m_src = NULL;
	this->m_inputpin.DeleteModule(index);
}

void CModuleBase::DeleteOutputPin(int index)
{
	TPinMap map;

	if(this->m_outputpin.Read(&map, index) == 0)
		return;
	if(map.pin == NULL)
		return;
//	map.pin->m_dst = NULL;
	this->m_outputpin.DeleteModule(index);
}

void CModuleBase::SetModuleIndex(CModuleBase *pmodule, int index)
{
	TModuleMap map;

	map.pmodule = pmodule;
	this->m_modules.Write(&map, index);
}

void CModuleBase::ConnectModule(const TConnectMap *bridge, CPinBase *pin)
{
	TModuleMap map;
	int i, size;
	
	if( (bridge->pin1 & INPUT_PIN_TYPE) == (bridge->pin2 & INPUT_PIN_TYPE) )	//wrong connect
		return;

	size = this->m_modules.GetSize();
	for(i = 0; i < size; i++)
	{
		if( this->m_modules.Read(&map, i) == 0 )
			continue;
		if(map.pmodule == NULL)
			continue;
		if(map.pmodule == this)
		{
			if(this->m_id == bridge->id1)
			{
				if(bridge->pin1 & INPUT_PIN_TYPE)
					map.pmodule->SetInputPin(pin, bridge->pin1 & PIN_MASK);
				else
					map.pmodule->SetOutputPin(pin, bridge->pin1 & PIN_MASK);
			}
			else if(this->m_id == bridge->id2)
			{
				if(bridge->pin2 & INPUT_PIN_TYPE)
					map.pmodule->SetInputPin(pin, bridge->pin2 & PIN_MASK);
				else
					map.pmodule->SetOutputPin(pin, bridge->pin2 & PIN_MASK);
			}

		}
		else
			map.pmodule->ConnectModule(bridge, pin);
	}
}

void CModuleBase::Disconnect(void)
{
	this->m_inputpin.Clear();
	this->m_outputpin.Clear();
}

void CModuleBase::SetRegMask(int adr, int mask)
{
	adr |= this->m_base;
	this->m_preg->SetMask(adr, mask);
}

int CModuleBase::GetProcessState(void)
{
	return this->m_state;
}

void CModuleBase::ClearModuleState(void)
{
	int i, size = this->m_modules.GetSize();
	TModuleMap map;

	for(i = 0; i < size; i++)
	{
		if( this->m_modules.Read(&map, i) == 0 )
			continue;
		if(map.pmodule == NULL)
			continue;
		if(map.pmodule == this)
		{
			this->m_state = 0;
		}
		else
			map.pmodule->ClearModuleState();
	}
}

void CModuleBase::ResetModule(void)
{
	int i, size = this->m_modules.GetSize();
	TModuleMap map;

	for(i = 0; i < size; i++)
	{
		if( this->m_modules.Read(&map, i) == 0 )
			continue;
		if(map.pmodule == NULL)
			continue;
		if(map.pmodule == this)
		{
			this->m_state = 0;
			this->m_inputpin.Reset();
			this->m_outputpin.Reset();
			this->ResetCounter();
		}
		else
			map.pmodule->ResetModule();
	}
}

void CModuleBase::PrintPinMap(void)
{
	int i, size = this->m_modules.GetSize();
	int j, size1;
	TModuleMap map;
	TPinMap pinmap;
	FILE *fp = NULL;;

	for(i = 0; i < size; i++)
	{
		if( this->m_modules.Read(&map, i) == 0)
			continue;
		if(map.pmodule == NULL)
			continue;
		if(map.pmodule == this)
		{
			size1 = this->m_inputpin.GetSize();
			for(j = 0; j < size1; j++)
			{
				if(this->m_inputpin.Read(&pinmap, j) == 0)
					continue;
				if(pinmap.pin == NULL)
					continue;
				fp = fopen("pinmap.dat", "at");
				fprintf(fp, "module id = 0x%x, input pin id = 0x%x\n", this->m_id, pinmap.pin->m_id);
				fclose(fp);
			}
			size1 = this->m_outputpin.GetSize();
			for(j = 0; j < size1; j++)
			{
				if(this->m_outputpin.Read(&pinmap, j) == 0)
					continue;
				if(pinmap.pin == NULL)
					continue;
				fp = fopen("pinmap.dat", "at");
				fprintf(fp, "module id = 0x%x, output pin id = 0x%x\n", this->m_id, pinmap.pin->m_id);
				fclose(fp);
			}
		}
		else
			map.pmodule->PrintPinMap();
	}
}

int CModuleBase::Process(int clk)
{
	if(this->m_state == CMODEL_ALLOVER)
		return CMODEL_ALLOVER;

	//static int flag = 0;
	//if(flag == 0)
	//this->PrintPinMap();
	//flag = 1;

	switch(clk)
	{
	case CMODEL_CLKRESTART:
		this->ResetModule();
		return this->m_state;

	case CMODEL_CLKSTOP:
		this->SetReg(REG_LOOPTEST, 0);
		this->m_state = CMODEL_ALLOVER;
		return CMODEL_ALLOVER;

	case CMODEL_CLKPAUSE:
		return CMODEL_NORMAL;

	case CMODEL_CLKLOW:
	case CMODEL_CLKRISE:
	case CMODEL_CLKFALL:
	case CMODEL_CLKHIGH:
	default:
		break;
	}

	if(this->m_pclk == NULL)
		this->Run0(clk);
	else
		this->m_pclk->Process(clk);

	return this->m_state;
}

void CModuleBase::Run0(int clk)
{
	int val;

	if(clk != CMODEL_CLKHIGH)
		return;

	if( (this->m_state & CMODEL_ONEFRAME) && (this->m_state & CMODEL_FILEEND) )
	{
		val = this->m_reg.Read(REG_LOOPTEST | BASE_V568ADDR);
		if(val == 0)
		{
			this->m_state |= CMODEL_ALLOVER;
			return;
		}
		val--;
		this->m_reg.Write(REG_LOOPTEST | BASE_V568ADDR, val, MODIFY_REG);
		this->ResetModule();
	}
	this->ClearModuleState();
	this->Run1(clk);
	this->m_state = this->Run3(this->m_state);
}

int CModuleBase::Run3(int flag)
{
	int count, i;
	TModuleMap map;

	count = this->m_modules.GetSize();
	for(i = 0; i < count; i++)
	{
		if( this->m_modules.Read(&map, i) == 0 )
			continue;
		if(map.pmodule == NULL)
			continue;
		if(map.pmodule == this)
		{
			flag |= this->m_state;
		}
		else
		{
			flag |= map.pmodule->Run3(flag);
		}
		if( (flag & CMODEL_FILEEND) && (flag & CMODEL_ONEFRAME) )
			return flag;
	}
	return flag;
}

void CModuleBase::Run1(int clk)
{
	int count, i;
	TModuleMap map;

	if(clk != CMODEL_CLKHIGH)
		return;

	count = this->m_modules.GetSize();
	for(i = 0; i < count; i++)
	{
		if( this->m_modules.Read(&map, i) == 0 )
			continue;
		if(map.pmodule == NULL)
			continue;
		if(map.pmodule == this)
			this->Run2(clk);
		else
			map.pmodule->Run1(clk);
	}

}

void CModuleBase::Run2(int clk)
{
	int i;
	TStream input[8], output[8];

	for(i = 0; i < 8; i++)
	{
		input[i].flag = 0;
		output[i].flag = 0;
	}
	if(this->CanRun(clk) == 0)
		return;
	this->ReadInput(input);
	this->doProcess(input, output);
	this->WriteOutput(output);
}

void CModuleBase::ReadInput(TStream *stream)
{
	TPinMap map;
	int i, size = this->m_inputpin.GetSize();

	for(i = 0; i < size; i++)
	{
		if(this->m_inputpin.Read(&map, i) == 0)
			continue;
		if(map.pin == NULL)
			continue;
		if(this->m_start & (1 << i))
		{
			this->SelfDriver(stream+i, i);
			if(map.pin->SelfDriver(stream+i) == 0)
				this->m_state |= CMODEL_FILEEND;
			map.pin->Read(stream+i);
		}
		else
			map.pin->Read(stream+i);
	}
}

void CModuleBase::WriteOutput(TStream *stream)
{
	TPinMap map;
	int i, size = this->m_outputpin.GetSize();

	for(i = 0; i < size; i++)
	{
		if(this->m_outputpin.Read(&map, i) == 0)
			continue;
		if(map.pin == NULL)
			continue;
		map.pin->Write(stream+i);
		if( (stream[i].flag & VSYNC_FLAG) && (this->m_target & (1 << i)) )
			this->m_state |= CMODEL_ONEFRAME;
		else
			this->m_state &= ~CMODEL_ONEFRAME;
	}
}



