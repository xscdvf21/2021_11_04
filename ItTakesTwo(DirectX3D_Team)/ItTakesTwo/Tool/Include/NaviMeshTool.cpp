// CameraTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "NaviMeshTool.h"
#include "afxdialogex.h"

#include "Base.h"
#include "Engine_Define.h"

#include "FileInfo.h"
#include "ToolDoc.h"
#include "MainFrm.h"
#include "ToolView.h"
#include "MyFormView.h"
#include "NaviMesh.h"
#include "Cell.h"

// CCameraTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CNaviMeshTool, CDialog)

CNaviMeshTool::CNaviMeshTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_CAMERATOOL, pParent)
	, m_FirstX(_T(""))
	, m_FirstY(_T(""))
	, m_FirstZ(_T(""))
	, m_SecondX(_T(""))
	, m_SecondY(_T(""))
	, m_SecondZ(_T(""))
	, m_ThirdX(_T(""))
	, m_ThirdY(_T(""))
	, m_ThirdZ(_T(""))
	, m_iPickCount(0)
	, m_fPosY(0)
{

}

CNaviMeshTool::~CNaviMeshTool()
{
}

void CNaviMeshTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RADIO20, m_MouseState[0]);
	DDX_Control(pDX, IDC_RADIO21, m_MouseState[1]);
	DDX_Control(pDX, IDC_RADIO22, m_MouseState[2]);
	DDX_Control(pDX, IDC_RADIO23, m_MouseState[3]);
	DDX_Control(pDX, IDC_RADIO26, m_MouseState[4]);
	DDX_Control(pDX, IDC_RADIO24, m_CellState[0]);
	DDX_Control(pDX, IDC_RADIO25, m_CellState[1]);
	DDX_Control(pDX, IDC_LIST1, m_NaviList);
	DDX_Text(pDX, IDC_EDIT20, m_FirstX);
	DDX_Text(pDX, IDC_EDIT21, m_FirstY);
	DDX_Text(pDX, IDC_EDIT22, m_FirstZ);
	DDX_Text(pDX, IDC_EDIT23, m_SecondX);
	DDX_Text(pDX, IDC_EDIT24, m_SecondY);
	DDX_Text(pDX, IDC_EDIT25, m_SecondZ);
	DDX_Text(pDX, IDC_EDIT26, m_ThirdX);
	DDX_Text(pDX, IDC_EDIT27, m_ThirdY);
	DDX_Text(pDX, IDC_EDIT28, m_ThirdZ);
	DDX_Control(pDX, IDC_LIST2, m_GroupList);
	DDX_Control(pDX, IDC_CHECK1, m_Lock);
	DDX_Text(pDX, IDC_EDIT2, m_fPosY);
}


BEGIN_MESSAGE_MAP(CNaviMeshTool, CDialog)
	ON_LBN_SELCHANGE(IDC_LIST1, &CNaviMeshTool::OnLbnSelchangeNaviMeshList)
	ON_BN_CLICKED(IDC_BUTTON2, &CNaviMeshTool::OnBnClickedNaviMeshSave)
	ON_BN_CLICKED(IDC_BUTTON12, &CNaviMeshTool::OnBnClickedNaviMeshLoad)
	ON_LBN_SELCHANGE(IDC_LIST2, &CNaviMeshTool::OnLbnSelchangeGroupList)
	ON_BN_CLICKED(IDC_BUTTON5, &CNaviMeshTool::OnBnClickedCreateGroup)
	ON_BN_CLICKED(IDC_BUTTON13, &CNaviMeshTool::OnBnClickedDeleteGroup)
	ON_BN_CLICKED(IDC_BUTTON3, &CNaviMeshTool::OnBnClickedDeleteCell)
	ON_BN_CLICKED(IDC_BUTTON7, &CNaviMeshTool::OnBnClickedCellStateChange)
	ON_BN_CLICKED(IDC_BUTTON9, &CNaviMeshTool::OnBnClickedButton9)
END_MESSAGE_MAP()


// CCameraTool 메시지 처리기입니다.


BOOL CNaviMeshTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_MouseState[0].SetCheck(TRUE);
	m_CellState[0].SetCheck(TRUE);
	m_Lock.SetCheck(TRUE);

	return TRUE;
}

void CNaviMeshTool::Update_NaviMeshTool()
{
	if (m_MouseState[0].GetCheck() == TRUE)
		m_iMouseState = 0;
	else if (m_MouseState[1].GetCheck() == TRUE)
		m_iMouseState = 1;
	else if (m_MouseState[2].GetCheck() == TRUE)
		m_iMouseState = 2;
	else if (m_MouseState[3].GetCheck() == TRUE)
		m_iMouseState = 3;
	else if (m_MouseState[4].GetCheck() == TRUE)
		m_iMouseState = 4;

	if (m_CellState[0].GetCheck() == TRUE)
		m_eCellState = CCell::STATE_GROUND;
	else if (m_CellState[1].GetCheck() == TRUE)
		m_eCellState = CCell::STATE_FALL;

	if (m_Lock.GetCheck() == TRUE)
		m_bLock = true;
	else
		m_bLock = false;
}

void CNaviMeshTool::OnLbnSelchangeNaviMeshList()
{
	UpdateData(TRUE);

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CToolView*	pView = dynamic_cast<CToolView*>(pMain->m_MainSplitter.GetPane(0, 1));

	int iIndex = m_NaviList.GetCurSel();
	if (LB_ERR == iIndex)
		return;

	CString strFileName;
	m_NaviList.GetText(iIndex, strFileName);
	lstrcpy(m_CellText, strFileName.GetString());
	m_CellIndex = iIndex;

	auto pLayer = find_if(pView->m_mapLayer.begin(), pView->m_mapLayer.end(), CTag_Finder(L"GameObject"));
	if (pLayer == pView->m_mapLayer.end())
		return;
	CToolNavi* pNavi = (CToolNavi*)pLayer->second->Get_GameObject(L"Navi");
	vector<vector<CCell*>> pvecCell = pNavi->m_pNaviMeshCom->Get_VecCell();

	if (pvecCell[m_CellGroupIndex][m_CellIndex]->Get_StateTool() == 0)
	{
		m_CellState[0].SetCheck(TRUE);
		m_CellState[1].SetCheck(FALSE);
	}
	else if (pvecCell[m_CellGroupIndex][m_CellIndex]->Get_StateTool() == 1)
	{
		m_CellState[0].SetCheck(FALSE);
		m_CellState[1].SetCheck(TRUE);
	}

	if (pvecCell[m_CellGroupIndex][m_CellIndex]->Get_Lock())
		m_Lock.SetCheck(TRUE);
	else if (!pvecCell[m_CellGroupIndex][m_CellIndex]->Get_Lock())
		m_Lock.SetCheck(FALSE);

	UpdateData(FALSE);
}

void CNaviMeshTool::HorizontalScroll()
{
	int iCX = 0;

	CString strFilePath = L"";
	CSize tSize{};

	CDC* pDC = m_NaviList.GetDC();
	for (int i = 0; i < m_NaviList.GetCount(); ++i)
	{
		m_NaviList.GetText(i, strFilePath);
		tSize = pDC->GetTextExtent(strFilePath);

		if (iCX < tSize.cx)
			iCX = tSize.cx;
	}

	if (iCX > m_NaviList.GetHorizontalExtent())
		m_NaviList.SetHorizontalExtent(iCX);

	int iCX2 = 0;

	CString strFilePath2 = L"";
	CSize tSize2{};

	CDC* pDC2 = m_GroupList.GetDC();
	for (int i = 0; i < m_GroupList.GetCount(); ++i)
	{
		m_GroupList.GetText(i, strFilePath2);
		tSize2 = pDC2->GetTextExtent(strFilePath2);

		if (iCX2 < tSize2.cx)
			iCX2 = tSize2.cx;
	}

	if (iCX > m_GroupList.GetHorizontalExtent())
		m_GroupList.SetHorizontalExtent(iCX2);

	ReleaseDC(pDC);
}

void CNaviMeshTool::OnLbnSelchangeGroupList()
{
	UpdateData(TRUE);

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CToolView*	pView = dynamic_cast<CToolView*>(pMain->m_MainSplitter.GetPane(0, 1));

	int iIndex = m_GroupList.GetCurSel();
	if (LB_ERR == iIndex)
		return;

	CString strFileName;
	m_GroupList.GetText(iIndex, strFileName);
	lstrcpy(m_CellGroupText, strFileName.GetString());
	m_CellGroupIndex = iIndex;

	auto pLayer = find_if(pView->m_mapLayer.begin(), pView->m_mapLayer.end(), CTag_Finder(L"GameObject"));
	if (pLayer == pView->m_mapLayer.end())
		return;
	CToolNavi* pNavi = (CToolNavi*)pLayer->second->Get_GameObject(L"Navi");
	vector<vector<CCell*>> pvecCell = pNavi->m_pNaviMeshCom->Get_VecCell();

	m_NaviList.ResetContent();
	for (_uint i = 0; i < pvecCell[m_CellGroupIndex].size(); ++i)
		m_NaviList.AddString(std::to_wstring(i).c_str());

	UpdateData(FALSE);
}


void CNaviMeshTool::OnBnClickedCreateGroup()
{
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CToolView*	pView = dynamic_cast<CToolView*>(pMain->m_MainSplitter.GetPane(0, 1));

	auto pLayer = find_if(pView->m_mapLayer.begin(), pView->m_mapLayer.end(), CTag_Finder(L"GameObject"));
	if (pLayer == pView->m_mapLayer.end())
		return;
	CToolNavi* pNavi = (CToolNavi*)pLayer->second->Get_GameObject(L"Navi");

	// Create Group
	pNavi->m_pNaviMeshCom->Add_CellGroup();
	vector<vector<CCell*>> pvecCell = pNavi->m_pNaviMeshCom->Get_VecCell();

	m_GroupList.ResetContent();
	for (_uint i = 0; i < pvecCell.size(); ++i)
		m_GroupList.AddString(std::to_wstring(i).c_str());
}


void CNaviMeshTool::OnBnClickedDeleteGroup()
{
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CToolView*	pView = dynamic_cast<CToolView*>(pMain->m_MainSplitter.GetPane(0, 1));

	auto pLayer = find_if(pView->m_mapLayer.begin(), pView->m_mapLayer.end(), CTag_Finder(L"GameObject"));
	if (pLayer == pView->m_mapLayer.end())
		return;
	CToolNavi* pNavi = (CToolNavi*)pLayer->second->Get_GameObject(L"Navi");

	pNavi->m_pNaviMeshCom->Delete_CellGroup(m_CellGroupIndex);

	m_GroupList.DeleteString(m_CellGroupIndex);

	vector<vector<CCell*>> pvecCell = pNavi->m_pNaviMeshCom->Get_VecCell();
	m_GroupList.ResetContent();
	for (_uint i = 0; i < pvecCell.size(); ++i)
		m_GroupList.AddString(std::to_wstring(i).c_str());
	m_NaviList.ResetContent();
}


void CNaviMeshTool::OnBnClickedDeleteCell()
{
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CToolView*	pView = dynamic_cast<CToolView*>(pMain->m_MainSplitter.GetPane(0, 1));

	auto pLayer = find_if(pView->m_mapLayer.begin(), pView->m_mapLayer.end(), CTag_Finder(L"GameObject"));
	if (pLayer == pView->m_mapLayer.end())
		return;
	CToolNavi* pNavi = (CToolNavi*)pLayer->second->Get_GameObject(L"Navi");

	pNavi->m_pNaviMeshCom->Delete_Cell(m_CellGroupIndex, m_CellIndex);

	vector<vector<CCell*>> pvecCell = pNavi->m_pNaviMeshCom->Get_VecCell();
	m_NaviList.DeleteString(m_CellIndex);

	m_NaviList.ResetContent();
	for (_uint i = 0; i < pvecCell[m_CellGroupIndex].size(); ++i)
		m_NaviList.AddString(std::to_wstring(i).c_str());
}


void CNaviMeshTool::OnBnClickedNaviMeshSave()
{
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CToolView*	pView = dynamic_cast<CToolView*>(pMain->m_MainSplitter.GetPane(0, 1));

	CFileDialog Dlg(FALSE, L"navi", L"*.navi", OFN_OVERWRITEPROMPT, L"Navi File(*.navi)|*.navi||", this);

	TCHAR szBuf[MAX_PATH] = L"";
	GetCurrentDirectory(MAX_PATH, szBuf);

	PathRemoveFileSpec(szBuf);
	PathRemoveFileSpec(szBuf);
	lstrcat(szBuf, L"\\Data\\Navi");
	Dlg.m_ofn.lpstrInitialDir = szBuf;

	if (IDOK == Dlg.DoModal())
	{
		CString strFilePath = Dlg.GetPathName();
		HANDLE hFile = CreateFile(strFilePath.GetString(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		DWORD dwByte = 0;

		CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
		CToolView*	pView = dynamic_cast<CToolView*>(pMain->m_MainSplitter.GetPane(0, 1));

		auto pLayer = find_if(pView->m_mapLayer.begin(), pView->m_mapLayer.end(), CTag_Finder(L"GameObject"));
		if (pLayer == pView->m_mapLayer.end())
			return;
		CToolNavi* pNavi = (CToolNavi*)pLayer->second->Get_GameObject(L"Navi");
		vector<vector<CCell*>> pvecCell = pNavi->m_pNaviMeshCom->Get_VecCell();
		_uint tempsize = 0;
		_uint iState = 0;
		_bool bLock;

		for (auto& iter : pvecCell)
		{
			tempsize = _uint(iter.size());
			WriteFile(hFile, &tempsize, sizeof(_uint), &dwByte, nullptr);
			
			for (auto& vec3 : iter)
			{
				WriteFile(hFile, vec3->Get_Point(CCell::POINT_A), sizeof(_vec3), &dwByte, nullptr);
				WriteFile(hFile, vec3->Get_Point(CCell::POINT_B), sizeof(_vec3), &dwByte, nullptr);
				WriteFile(hFile, vec3->Get_Point(CCell::POINT_C), sizeof(_vec3), &dwByte, nullptr);
				iState = vec3->Get_StateTool();
				WriteFile(hFile, &iState, sizeof(_uint), &dwByte, nullptr);
				bLock = vec3->Get_Lock();
				WriteFile(hFile, &bLock, sizeof(_bool), &dwByte, nullptr);
			}
		}

		CloseHandle(hFile);
	}
}


void CNaviMeshTool::OnBnClickedNaviMeshLoad()
{
	UpdateData(TRUE);

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CToolView*	pView = dynamic_cast<CToolView*>(pMain->m_MainSplitter.GetPane(0, 1));

	CFileDialog Dlg(TRUE, L"navi", L"*.navi", OFN_OVERWRITEPROMPT, L"Navi File(*.navi)|*.navi||", this);

	TCHAR szBuf[MAX_PATH] = L"";
	GetCurrentDirectory(MAX_PATH, szBuf);

	PathRemoveFileSpec(szBuf);
	lstrcat(szBuf, L"\\Data\\Navi");
	Dlg.m_ofn.lpstrInitialDir = szBuf;

	Engine::CGameObject*		pGameObject = nullptr;
	if (IDOK == Dlg.DoModal())
	{
		CString strFilePath = Dlg.GetPathName();
		HANDLE hFile = CreateFile(strFilePath.GetString(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		DWORD dwByte = 0;

		CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
		CToolView*	pView = dynamic_cast<CToolView*>(pMain->m_MainSplitter.GetPane(0, 1));

		auto pLayer = find_if(pView->m_mapLayer.begin(), pView->m_mapLayer.end(), CTag_Finder(L"GameObject"));
		if (pLayer == pView->m_mapLayer.end())
			return;
		CToolNavi* pNavi = (CToolNavi*)pLayer->second->Get_GameObject(L"Navi");

		_uint size = 0;
		_uint index = 0;
		_vec3 vPoint1, vPoint2, vPoint3;
		_uint iCellState = 0;
		_bool bLock;

		while (true)
		{
			vector<vector<CCell*>> pvecCell = pNavi->m_pNaviMeshCom->Get_VecCell();
			ReadFile(hFile, &size, sizeof(_uint), &dwByte, nullptr);

			if (dwByte == 0)
				break;

			pNavi->m_pNaviMeshCom->Add_CellGroup();
			m_GroupList.AddString(std::to_wstring(index).c_str());

			for (_uint i = 0; i < size; ++i)
			{
				ReadFile(hFile, &vPoint1, sizeof(_vec3), &dwByte, nullptr);
				ReadFile(hFile, &vPoint2, sizeof(_vec3), &dwByte, nullptr);
				ReadFile(hFile, &vPoint3, sizeof(_vec3), &dwByte, nullptr);
				ReadFile(hFile, &iCellState, sizeof(_uint), &dwByte, nullptr);
				ReadFile(hFile, &bLock, sizeof(_bool), &dwByte, nullptr);

				pNavi->m_pNaviMeshCom->Add_Cell(vPoint1, vPoint2, vPoint3, index, iCellState, bLock);
			}
			++index;
		}

		CloseHandle(hFile);
	}
	UpdateData(FALSE);
}

void CNaviMeshTool::OnBnClickedCellStateChange()
{
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CToolView*	pView = dynamic_cast<CToolView*>(pMain->m_MainSplitter.GetPane(0, 1));

	auto pLayer = find_if(pView->m_mapLayer.begin(), pView->m_mapLayer.end(), CTag_Finder(L"GameObject"));
	if (pLayer == pView->m_mapLayer.end())
		return;
	CToolNavi* pNavi = (CToolNavi*)pLayer->second->Get_GameObject(L"Navi");
	vector<vector<CCell*>> pvecCell = pNavi->m_pNaviMeshCom->Get_VecCell();

	pvecCell[m_CellGroupIndex][m_CellIndex]->Set_CellState(m_eCellState);
	pvecCell[m_CellGroupIndex][m_CellIndex]->Set_Lock(m_bLock);
}


void CNaviMeshTool::OnBnClickedButton9()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CToolView*	pView = dynamic_cast<CToolView*>(pMain->m_MainSplitter.GetPane(0, 1));

	UpdateData(TRUE);
	m_fPosY = m_fPosY;
	UpdateData(FALSE);

	D3DXPlaneFromPoints(&pView->m_tPlane, &_vec3(0.f, m_fPosY, 0.f), &_vec3(1.f, m_fPosY, 0.f), &_vec3(0.f, m_fPosY, 1.f));

}
