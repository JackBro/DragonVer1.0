// VmonitorDoc.h :  CVmonitorDoc 类的接口
//

#pragma once

class CVmonitorDoc : public CDocument
{
protected: // 仅从序列化创建
	CVmonitorDoc();
	DECLARE_DYNCREATE(CVmonitorDoc)

// 属性
public:

// 操作
public:
	void	DisplayContent(char *buf, int size);
protected:
// 重写
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

protected:
	int		m_rwMode;
// 实现
public:
	virtual ~CVmonitorDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnCloseDocument();
protected:
	BOOL SaveModified();
};


