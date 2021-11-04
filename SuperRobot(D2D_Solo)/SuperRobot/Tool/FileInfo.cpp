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
//D:\�ں���\5������\FrameWork (17)\Texture\Stage\Terrain\*.*
void CFileInfo::FilePathExtraction(const wstring & wstrFilePath, list<PATHINFO*>& rlistPathInfo)
{
	// ��θ� �����ϴ� ��ü�� �������ش�. �ٷ� �ؿ� �ִ� �̳�. 
	CFileFind find; 
	wstring wstrFullPath = wstrFilePath + L"\\*.*"; 

	// ������ ã�� ������ ������ true, ���̻� ������ ������ ���ٸ� false�� ��ȯ. 
	bool bContinue = find.FindFile(wstrFullPath.c_str());
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
			// ���⼭�� ���� ���� �����ϰ� �۾��ؾ� �ϴ� ����. 
			if(find.IsSystem())
				continue;
			PATHINFO* pPathInfo = new PATHINFO; 

			pPathInfo->dwCount = FilePathCount(wstrFullPath);

			// Tile0
			// ���� ��ο��� ����η� �ٲٰ� �Դٰ� �̸��� �ٲ�ߵ� �� ? 
			// Tile%d.png
			wstring strFileName = L""; //Title?? Ÿ��Ʋ?? -> �̸�?? ���� �װ� 
			// Tile + %d.png = Tile%d.png

			strFileName = find.GetFileTitle().GetString();
			strFileName = strFileName.substr(0,strFileName.length() - 1) + L"%d.png";
			
			// ��ζ� ��ü ���������! 

			TCHAR szBuf[MAX_PATH] = L""; 
			lstrcpy(szBuf, wstrFullPath.c_str()); 
			PathRemoveFileSpec(szBuf); 
			// �ٿ� ����? szBuf���� �ٿ�. 
			////..\Texture\Stage\Terrain\Tile + \ + Tile%d 
			PathCombine(szBuf, szBuf, strFileName.c_str());
			
			//..\Texture\Stage\Terrain\Tile\Tile%d; 
			pPathInfo->wstrPath = ConvertRelativePath(szBuf).GetString();

			PathRemoveFileSpec(szBuf);
			//..\Texture\Stage\Terrain\Tile; 
			pPathInfo->wstrStateKey = PathFindFileName(szBuf); 

			PathRemoveFileSpec(szBuf); 
			//..\Texture\Stage\Terrain
			pPathInfo->wstrObjectKey = PathFindFileName(szBuf); 

			rlistPathInfo.emplace_back(pPathInfo); 
			return;
		}

	}

}

DWORD CFileInfo::FilePathCount(const wstring & wstrFilePath)
{
	CFileFind find; 
	//wstring wstrFullPath = wstrFilePath + L"\\*.*"; 
	DWORD dwCount = 0; 
	bool bContinue = find.FindFile(wstrFilePath.c_str());
	while (bContinue)
	{
		bContinue = find.FindNextFile();
		if (find.IsDots() || find.IsSystem())
			continue;
		++dwCount; 
	}
	return dwCount;
}
