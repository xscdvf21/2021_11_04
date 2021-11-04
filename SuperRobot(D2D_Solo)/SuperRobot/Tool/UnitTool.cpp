// UnitTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "UnitTool.h"
#include "afxdialogex.h"


// CUnitTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CUnitTool, CDialog)

CUnitTool::CUnitTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_UNITTOOL, pParent)
	, m_string1(_T(""))
	, m_string2(_T(""))
	, m_strName(_T(""))
	, m_iAtt(0)
	, m_iHP(0)
{

}

CUnitTool::~CUnitTool()
{
	for (auto& rPair :m_mapUnitInfo)
		Safe_Delete(rPair.second);
}

void CUnitTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_string1);
	DDX_Text(pDX, IDC_EDIT2, m_string2);
	DDX_Text(pDX, IDC_EDIT3, m_strName);
	DDX_Text(pDX, IDC_EDIT4, m_iAtt);
	DDX_Text(pDX, IDC_EDIT5, m_iHP);
	DDX_Control(pDX, IDC_LIST1, m_ListBox);
	DDX_Control(pDX, IDC_RADIO1, m_Class[0]);
	DDX_Control(pDX, IDC_RADIO2, m_Class[1]);
	DDX_Control(pDX, IDC_RADIO3, m_Class[2]);
	DDX_Control(pDX, IDC_CHECK1, m_Item[0]);
	DDX_Control(pDX, IDC_CHECK2, m_Item[1]);
	DDX_Control(pDX, IDC_CHECK3, m_Item[2]);

}


BEGIN_MESSAGE_MAP(CUnitTool, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CUnitTool::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, &CUnitTool::OnBnClickedAdd)
	ON_LBN_SELCHANGE(IDC_LIST1, &CUnitTool::OnLbnSelchangeFindName)
	ON_BN_CLICKED(IDC_BUTTON4, &CUnitTool::OnBnClickedDelete)
	ON_BN_CLICKED(IDC_BUTTON5, &CUnitTool::OnBnClickedSave)
	ON_BN_CLICKED(IDC_BUTTON6, &CUnitTool::OnBnClickedLoad)
END_MESSAGE_MAP()


// CUnitTool 메시지 처리기입니다.


void CUnitTool::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//UpdateData(TRUE);
	m_string2 = m_string1;
	//UpdateData(FALSE);
}


void CUnitTool::OnBnClickedAdd()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	auto& iter_find = m_mapUnitInfo.find(m_strName);
	if (iter_find == m_mapUnitInfo.end())
	{
		UNITINFO* pUnitData = new UNITINFO;
		pUnitData->strName = m_strName;
		pUnitData->iAtt = m_iAtt;
		pUnitData->iMaxHp = m_iHP;
		pUnitData->byItem = 0; 
		 // 직업 선택
		for (int i = 0; i < 3;++i)
		{
			if (m_Class[i].GetCheck())
			{
				pUnitData->byClass = i;
				break;
			}
		}
		// 아이템 선택 
		if (m_Item[0].GetCheck())
			pUnitData->byItem |= SWORD; 
		// 0000 0001
		if (m_Item[1].GetCheck())
			pUnitData->byItem |= DAGGER; 
		// 0000 0011
		if (m_Item[2].GetCheck())
			pUnitData->byItem |= BOW; 
		// 0000 0111


		m_mapUnitInfo.emplace(pUnitData->strName, pUnitData);

		m_ListBox.AddString(pUnitData->strName); 
	}
	UpdateData(FALSE);
}


void CUnitTool::OnLbnSelchangeFindName()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int i = 0; 
	UpdateData(TRUE); 
	int iIndex = m_ListBox.GetCurSel(); 
	if (LB_ERR == iIndex)
		return;

	CString strSelectName;
	m_ListBox.GetText(iIndex, strSelectName);

	auto& iter_Find = m_mapUnitInfo.find(strSelectName); 
	if (iter_Find == m_mapUnitInfo.end())
		return; 

	m_strName = iter_Find->second->strName;
	m_iAtt = iter_Find->second->iAtt; 
	m_iHP = iter_Find->second->iMaxHp;

	for (int i = 0; i < 3; ++i)
	{
		m_Class[i].SetCheck(FALSE);
		m_Item[i].SetCheck(FALSE); 
	}
	m_Class[iter_Find->second->byClass].SetCheck(TRUE);

	// 0000 0111 & 0000 0001 = 0000 0001
	// 0000 0110 & 0000 0001 = 0000 0000 
	if (iter_Find->second->byItem & SWORD)
		m_Item[0].SetCheck(TRUE); 
	if (iter_Find->second->byItem & DAGGER)
		m_Item[1].SetCheck(TRUE);
	if (iter_Find->second->byItem & BOW)
		m_Item[2].SetCheck(TRUE);
	UpdateData(FALSE); 
}


BOOL CUnitTool::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_Class[0].SetCheck(TRUE);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CUnitTool::OnBnClickedDelete()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	int iIndex = m_ListBox.GetCurSel(); 
	if (LB_ERR == iIndex)
		return; 

	CString strfindName; 
	m_ListBox.GetText(iIndex, strfindName); 

	auto& iter_find = m_mapUnitInfo.find(strfindName); 
	if (iter_find == m_mapUnitInfo.end())
		return; 

	Safe_Delete(iter_find->second);
	m_mapUnitInfo.erase(iter_find);
	m_ListBox.DeleteString(iIndex); 
	UpdateData(FALSE);
}


void CUnitTool::OnBnClickedSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	//CFileDialog
	CFileDialog Dlg(FALSE, L"dat", L"*.dat", OFN_OVERWRITEPROMPT,L"Data File(*.dat)|*.dat||",this); 

	TCHAR szBuf[MAX_PATH] = L""; 
	GetCurrentDirectory(MAX_PATH, szBuf); 
	//D:\박병건\112A\FrameWork
	PathRemoveFileSpec(szBuf); 
	lstrcat(szBuf, L"\\Data");
	Dlg.m_ofn.lpstrInitialDir = szBuf; 
	if (IDOK == Dlg.DoModal())
	{
		// 파일의 경로 얻어오느 함수
		CString strFilePath = Dlg.GetPathName(); 
		HANDLE hFile = CreateFile(strFilePath.GetString(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr); 

		if (INVALID_HANDLE_VALUE == hFile)
		{
			ERR_MSG(L"file open Failed"); 
			return; 
		}

		DWORD dwByte = 0; 
		DWORD dwStrByte = 0; 

		for (auto& rPair : m_mapUnitInfo)
		{
			dwStrByte = sizeof(TCHAR) * (rPair.first.GetLength() + 1); 

			WriteFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr); 
			WriteFile(hFile, rPair.first.GetString(), dwStrByte, &dwByte, nullptr); 
			
			WriteFile(hFile, &rPair.second->iAtt, sizeof(int), &dwByte, nullptr); 
			WriteFile(hFile, &rPair.second->iMaxHp, sizeof(int), &dwByte, nullptr);
			WriteFile(hFile, &rPair.second->byClass, sizeof(BYTE), &dwByte, nullptr);
			WriteFile(hFile, &rPair.second->byItem, sizeof(BYTE), &dwByte, nullptr);
		}
		CloseHandle(hFile); 
	}
}


void CUnitTool::OnBnClickedLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	//CFileDialog
	CFileDialog Dlg(TRUE, L"dat", L"*.dat", OFN_OVERWRITEPROMPT, L"Data File(*.dat)|*.dat||", this);

	TCHAR szBuf[MAX_PATH] = L"";
	GetCurrentDirectory(MAX_PATH, szBuf);
	//D:\박병건\112A\FrameWork
	PathRemoveFileSpec(szBuf);
	lstrcat(szBuf, L"\\Data");
	Dlg.m_ofn.lpstrInitialDir = szBuf;
	if (IDOK == Dlg.DoModal())
	{
		CString strFilePath = Dlg.GetPathName();
		HANDLE hFile = CreateFile(strFilePath.GetString(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (INVALID_HANDLE_VALUE == hFile)
		{
			ERR_MSG(L"file open Failed");
			return;
		}
		DWORD dwByte = 0; 
		DWORD dwStrByte = 0; 
		TCHAR* pTemp = nullptr; 
		UNITINFO* pUnitInfo = nullptr; 

		for (auto& rPair : m_mapUnitInfo)
			Safe_Delete(rPair.second); 
		m_mapUnitInfo.clear(); 
		m_ListBox.ResetContent(); 
		while (true)
		{
			pUnitInfo = new UNITINFO; 
			ReadFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr); 
			pTemp = new TCHAR[dwStrByte];
			ReadFile(hFile, pTemp, dwStrByte, &dwByte, nullptr); 
			pUnitInfo->strName = pTemp;
			if (pTemp)
			{
				delete pTemp; 
				pTemp = nullptr; 
			}
			ReadFile(hFile, &pUnitInfo->iAtt, sizeof(int), &dwByte, nullptr);
			ReadFile(hFile, &pUnitInfo->iMaxHp, sizeof(int), &dwByte, nullptr);
			ReadFile(hFile, &pUnitInfo->byClass, sizeof(BYTE), &dwByte, nullptr);
			ReadFile(hFile, &pUnitInfo->byItem, sizeof(BYTE), &dwByte, nullptr);
			if (0 == dwByte)
			{
				Safe_Delete(pUnitInfo); 
				break; 
			}
			m_mapUnitInfo.emplace(pUnitInfo->strName, pUnitInfo); 
			m_ListBox.AddString(pUnitInfo->strName); 
		}
		CloseHandle(hFile); 
	}
	UpdateData(FALSE);
}
