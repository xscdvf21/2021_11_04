#include "stdafx.h"
#include "FileInfo.h"

USING(Engine)
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

void CFileInfo::FilePathExtraction(const wstring & wstrFilePath, list<PATHINFO*>& rlistPathInfo)
{
	CFileFind find;
	wstring wstrFullPath = wstrFilePath + L"\\*.*";

	int bContinue = find.FindFile(wstrFullPath.c_str());
	while (bContinue)
	{
		bContinue = find.FindNextFile();

		if (find.IsDots())
			continue;
		if (find.IsDirectory())
		{
			FilePathExtraction(find.GetFilePath().GetString(), rlistPathInfo);
		}
		else
		{
			if (find.IsSystem())
				continue;
			PATHINFO* pPathInfo = new PATHINFO;

			pPathInfo->dwCount = FilePathCount(wstrFullPath);

			wstring strFileName = L"";

			strFileName = find.GetFileTitle().GetString();
			strFileName = strFileName.substr(0, strFileName.length() - 1) + L"%d.png";

			TCHAR szBuf[MAX_PATH] = L"";
			lstrcpy(szBuf, wstrFullPath.c_str());
			PathRemoveFileSpec(szBuf);
			PathCombine(szBuf, szBuf, strFileName.c_str());

			pPathInfo->wstrPath = ConvertRelativePath(szBuf).GetString();

			PathRemoveFileSpec(szBuf);
			pPathInfo->wstrStateKey = PathFindFileName(szBuf);

			PathRemoveFileSpec(szBuf);
			pPathInfo->wstrObjectKey = PathFindFileName(szBuf);

			rlistPathInfo.emplace_back(pPathInfo);
			return;
		}

	}
}

DWORD CFileInfo::FilePathCount(const wstring & wstrFilePath)
{
	CFileFind find;
	DWORD dwCount = 0;
	int bContinue = find.FindFile(wstrFilePath.c_str());
	while (bContinue)
	{
		bContinue = find.FindNextFile();
		if (find.IsDots() || find.IsSystem())
			continue;
		++dwCount;
	}
	return dwCount;
}