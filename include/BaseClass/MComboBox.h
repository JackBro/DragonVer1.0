#ifndef __BASECLASS_MCOMBOBOX_H__
#define __BASECLASS_MCOMBOBOX_H__



#ifndef BASECLASS_DLL
	#define BASECLASS_DLL  __declspec(dllimport)
#endif

// CMComboBox

class BASECLASS_DLL CMComboBox : public CComboBox
{
	DECLARE_DYNAMIC(CMComboBox)

public:
	CMComboBox();
	virtual ~CMComboBox();

public:
	void addString(char *str);

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDropFiles(HDROP hDropInfo);
};



#endif
