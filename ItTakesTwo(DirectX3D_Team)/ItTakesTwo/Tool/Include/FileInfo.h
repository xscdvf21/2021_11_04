#pragma once

#ifndef FileInfo_H__
#define FileInfo_H__

#include "Engine_Define.h"

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


#endif // !FileInfo_H__
