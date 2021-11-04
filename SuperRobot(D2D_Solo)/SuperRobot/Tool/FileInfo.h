#pragma once
class CFileInfo
{
public:
	CFileInfo();
	~CFileInfo();
public:
	static CString ConvertRelativePath(CString wstrFileFullPath);
	static void FilePathExtraction(const wstring& wstrFilePath, list<PATHINFO*>& rlistPathInfo); 
	static DWORD FilePathCount(const wstring& wstrFilePath);

};

