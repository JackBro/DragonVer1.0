// VmonitorDoc.h :  CVmonitorDoc ��Ľӿ�
//

#pragma once

class CVmonitorDoc : public CDocument
{
protected: // �������л�����
	CVmonitorDoc();
	DECLARE_DYNCREATE(CVmonitorDoc)

// ����
public:

// ����
public:
	void	DisplayContent(char *buf, int size);
protected:
// ��д
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

protected:
	int		m_rwMode;
// ʵ��
public:
	virtual ~CVmonitorDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnCloseDocument();
protected:
	BOOL SaveModified();
};


