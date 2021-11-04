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
//D:\박병건\5개월차\FrameWork (17)\Texture\Stage\Terrain\*.*
void CFileInfo::FilePathExtraction(const wstring & wstrFilePath, list<PATHINFO*>& rlistPathInfo)
{
	// 경로를 제어하는 객체를 제공해준다. 바로 밑에 있는 이놈. 
	CFileFind find; 
	wstring wstrFullPath = wstrFilePath + L"\\*.*"; 

	// 파일을 찾아 파일이 있으면 true, 더이상 접근할 파일이 없다면 false를 반환. 
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
			// 여기서는 이제 내가 순수하게 작업해야 하는 시점. 
			if(find.IsSystem())
				continue;
			PATHINFO* pPathInfo = new PATHINFO; 

			pPathInfo->dwCount = FilePathCount(wstrFullPath);

			// Tile0
			// 절대 경로에서 상대경로로 바꾸고 게다가 이름도 바꿔야되 왜 ? 
			// Tile%d.png
			wstring strFileName = L""; //Title?? 타이틀?? -> 이름?? ㅇㅇ 그거 
			// Tile + %d.png = Tile%d.png

			strFileName = find.GetFileTitle().GetString();
			strFileName = strFileName.substr(0,strFileName.length() - 1) + L"%d.png";
			
			// 경로랑 합체 시켜줘야지! 

			TCHAR szBuf[MAX_PATH] = L""; 
			lstrcpy(szBuf, wstrFullPath.c_str()); 
			PathRemoveFileSpec(szBuf); 
			// 붙여 어디다? szBuf에다 붙여. 
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
