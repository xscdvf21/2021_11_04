// FilePathExtraction.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "FilePathExtraction.h"
#include "afxdialogex.h"
#include "FileInfo.h"

// CFilePathExtraction 대화 상자입니다.

IMPLEMENT_DYNAMIC(CFilePathExtraction, CDialog)

CFilePathExtraction::CFilePathExtraction(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_FILEPATHEXTRACTION, pParent)
{

}

CFilePathExtraction::~CFilePathExtraction()
{
	for (auto& pPathInfo : m_listPathInfo)
		Safe_Delete(pPathInfo); 

	m_listPathInfo.clear(); 
}

void CFilePathExtraction::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListBox);
}


BEGIN_MESSAGE_MAP(CFilePathExtraction, CDialog)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDOK2, &CFilePathExtraction::OnBnClickedSaveButton)
	ON_BN_CLICKED(IDOK3, &CFilePathExtraction::OnBnClickedLoadButton)
END_MESSAGE_MAP()


// CFilePathExtraction 메시지 처리기입니다.


void CFilePathExtraction::OnDropFiles(HDROP hDropInfo)
{
	UpdateData(TRUE); 
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	int iCount = DragQueryFile(hDropInfo, -1, nullptr, 0);
	TCHAR szFilePath[MAX_PATH] = L""; 
	for (int i = 0 ; i < iCount ; ++i)
	{
		DragQueryFile(hDropInfo, i, szFilePath, MAX_PATH);
		// 여기서 슥삭하고 작업 시작. 
		CFileInfo::FilePathExtraction(szFilePath, m_listPathInfo);
		
	}
	TCHAR szCount[MAX_PATH] = L""; 

	for (auto& pPathInfo : m_listPathInfo)
	{
		_itow_s(pPathInfo->dwCount, szCount, 10); 
		wstring wstrPathCombine = pPathInfo->wstrPath + L"|" + pPathInfo->wstrObjectKey + L"|" + pPathInfo->wstrStateKey + L"|" + szCount;
		m_ListBox.AddString(wstrPathCombine.c_str()); 

	}
	HorizontalScroll();
	CDialog::OnDropFiles(hDropInfo);
	UpdateData(FALSE);
}

void CFilePathExtraction::HorizontalScroll()
{
	int iCX = 0;
	CString strFilePath = L"";
	CSize tSize{};

	CDC* pDC = m_ListBox.GetDC();
	for (int i = 0; i < m_ListBox.GetCount(); ++i)
	{
		m_ListBox.GetText(i, strFilePath);
		tSize = pDC->GetTextExtent(strFilePath);

		if (iCX < tSize.cx)
			iCX = tSize.cx;
	}

	if (iCX > m_ListBox.GetHorizontalExtent())
		m_ListBox.SetHorizontalExtent(iCX);

	ReleaseDC(pDC);
}

void CFilePathExtraction::OnBnClickedSaveButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	wofstream fout; 
	fout.open("../Data/PathInfo.txt"); 
	if (fout.fail())
	{
		ERR_MSG(L"File open failure");

		return;
	}

	for (auto& pImagePath : m_listPathInfo)
		fout << pImagePath->wstrPath << L"|" << pImagePath->wstrObjectKey << L"|" << pImagePath->wstrStateKey << L"|" << pImagePath->dwCount << endl; 

	fout.close(); 


}


void CFilePathExtraction::OnBnClickedLoadButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	wifstream fin;
	fin.open("../Data/PathInfo.txt");
	if (fin.fail())
	{
		ERR_MSG(L"File Open failed");

		return;
	}

	if (!m_listPathInfo.empty())
	{
		for (auto& pImagePath : m_listPathInfo)
			Safe_Delete(pImagePath); 
		m_listPathInfo.clear(); 
	}
	TCHAR szPath[MAX_PATH] = L""; 
	TCHAR szObjectKey[MAX_PATH] = L""; 
	TCHAR szStateKey[MAX_PATH] = L""; 
	TCHAR szCount[MAX_PATH] = L""; 
	while (true)
	{
		fin.getline(szPath, MAX_PATH, L'|'); 
		fin.getline(szObjectKey, MAX_PATH, L'|');
		fin.getline(szStateKey, MAX_PATH, L'|');
		fin.getline(szCount, MAX_PATH);
		DWORD dwCount = _ttoi(szCount); 

		if (0 == dwCount)
			break; 

		PATHINFO* pImagePath = new PATHINFO; 
		pImagePath->wstrPath = szPath; 
		pImagePath->wstrObjectKey = szObjectKey; 
		pImagePath->wstrStateKey = szStateKey; 
		pImagePath->dwCount = dwCount; 

		m_listPathInfo.emplace_back(pImagePath); 
	}
	fin.close();
	UpdateData(TRUE); 
	m_ListBox.ResetContent(); 

	TCHAR szCount2[MAX_PATH] = L"";

	for (auto& pPathInfo : m_listPathInfo)
	{
		_itow_s(pPathInfo->dwCount, szCount2, 10);
		wstring wstrPathCombine = pPathInfo->wstrPath + L"|" + pPathInfo->wstrObjectKey + L"|" + pPathInfo->wstrStateKey + L"|" + szCount2;
		m_ListBox.AddString(wstrPathCombine.c_str());

	}
	HorizontalScroll(); 
	UpdateData(FALSE);


}
