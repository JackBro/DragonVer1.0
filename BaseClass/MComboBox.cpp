//
#include "internal.h"


// CMComboBox

IMPLEMENT_DYNAMIC(CMComboBox, CComboBox)
CMComboBox::CMComboBox()
{
}

CMComboBox::~CMComboBox()
{
}


BEGIN_MESSAGE_MAP(CMComboBox, CComboBox)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()



// CMComboBox ��Ϣ�������

void CMComboBox::addString(char *str)
{
	int index;

	index = CComboBox::FindString(-1, str);
	if(index == CB_ERR)
	{
		CComboBox::InsertString(0, str);
		index = 0;
	}
	CComboBox::SetCurSel(index);
}

void CMComboBox::OnDropFiles(HDROP hDropInfo)
{
	char str[100];
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CComboBox::OnDropFiles(hDropInfo);
	DragQueryFile(hDropInfo, 0, str, 100);
	this->addString(str);
}

