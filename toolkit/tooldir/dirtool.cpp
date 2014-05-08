#include "internal.h"

CDirTools::CDirTools(char *root)
{
	char path[0x100];

	this->CheckPath(root, path);
	GetCurrentDirectory(0x100, this->m_root);
	sprintf(this->m_root, "%s/%s", this->m_root, path);
	this->SetRoot(this->m_root);
}

void CDirTools::CreateDir(char *path)
{
	char dir[0x100];

	if(this->CheckPath(path))
		return;
	sprintf(dir, "%s/%s", this->m_root, path);
	CreateDirectory(dir, NULL);
}

void CDirTools::DeleteDir(char *path)
{
	char dir[0x100];

	if(this->CheckPath(path))
		return;
	sprintf(dir, "%s/%s", this->m_root, path);
	this->RmDir(dir);
}

void CDirTools::GetRoot(char *root)
{
	strcpy(root, this->m_root);
}

//================================
//
//		private function
//
//============================
void CDirTools::SetRoot(char *root)
{
	CFileFind ft;
	CStringTools tools;
	int flag = 1;

	if(root != this->m_root)
		strcpy(this->m_root, root);
	do {
		if(ft.FindFile(this->m_root) == 0)
		{
			CreateDirectory(this->m_root, NULL);
			return;
		}
		ft.FindNextFile();
		if(ft.IsDirectory())
			return;
		tools.AddIndex(this->m_root, this->m_root);
	} while(flag);
}

void CDirTools::RmDir(char *path)
{
	char src[0x100];
	CFileFind ft;
	int flag = 1, count;
	CString str;

	if(ft.FindFile(path) == 0)
		return;
	ft.Close();
	sprintf(src, "%s/*.*", path);
	ft.FindFile(src);
	do {
		count = ft.FindNextFile();
		if(ft.IsDots())
			continue;
		if(ft.IsDirectory())
		{
			str = ft.GetFilePath();
			this->RmDir(str.GetBuffer());
			continue;
		}
		str = ft.GetFilePath();
		DeleteFile(str.GetBuffer());
		if(count == 0)
			break;
	} while(flag);
	ft.Close();
	RemoveDirectory(path);
}

int CDirTools::CheckPath(char *src, char *dst)
{
	if( (src == NULL) || (src[0] == '\0') )
	{
		if(dst)
			strcpy(dst, DEFINE_ROOTDIR);
		return 1;
	}
	if(dst)
		strcpy(dst, src);
	return 0;
}


