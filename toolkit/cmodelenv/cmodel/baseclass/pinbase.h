

#ifndef _CMODEL_PINBASE_H_
#define _CMODEL_PINBASE_H_

#include "../../../tools/global.h"
#include "../../../toolbuf/channelbuf.h"
#include "../../../toolfile/fileinput.h"
#include "../../../toolfile/dbugtools.h"

class CModuleBase;

//
//	use "COM" conception later to modify cmodel
//


//=============== class CPinBase =================

class CPinBase : public CChannelBuf
{
public:
	CPinBase(int delay = 1);
	virtual ~CPinBase();

public:
	virtual int		Write(TStream *stream);
	virtual int		Read(TStream *stream);
	virtual void	SetDelayCount(int delay);

	virtual int		SelfDriver(TStream *stream);
	virtual void	Reset(void);

protected:
	virtual void	Clear(void);
	virtual int		ReadInputFile(TStream *stream);
	virtual void	WriteDbugFile(TStream *stream);

public:	//debug
	virtual void	SetPinId(int id);
	int		m_id;

public:
	//CModuleBase	*m_src;
	//CModuleBase	*m_dst;

public:
	CFileInput	m_input;
	CDbugTools	m_dbug;

private:
	int		m_fileend;
};


#endif


