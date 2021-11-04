#include "stdafx.h"
#include "FileInfo.h"


CFileInfo::CFileInfo()
{
}


CFileInfo::~CFileInfo()
{
}

CString CFileInfo::ConvertRelativePath(CString wstrFileFullPath)
{
	TCHAR szRelativePath[MAX_PATH] = L"";
	TCHAR szCurrentPath[MAX_PATH] = L"";
	GetCurrentDirectory(MAX_PATH, szCurrentPath);
	PathRelativePathTo(szRelativePath,
		szCurrentPath, FILE_ATTRIBUTE_DIRECTORY,
		wstrFileFullPath, FILE_ATTRIBUTE_DIRECTORY);

	return CString(szRelativePath);
}
