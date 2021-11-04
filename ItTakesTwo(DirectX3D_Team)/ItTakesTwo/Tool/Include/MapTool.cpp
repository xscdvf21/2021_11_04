// MapTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "MapTool.h"
#include "afxdialogex.h"

#include "Base.h"
#include "Engine_Define.h"
#include "Layer.h"
#include "ToolTerrain.h"

#include "FileInfo.h"
#include "ToolDoc.h"
#include "MainFrm.h"
#include "ToolView.h"
#include "NormalTerrainTex.h"

BEGIN(Engine)

class CGraphicDevice;
class CManagement;

END


// CMapTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CMapTool, CDialog)

CMapTool::CMapTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_MAPTOOL, pParent)
	, m_TerrainX(0)
	, m_TerrainZ(0)
	, m_TerrainScale(0)
	, m_fHeight(0)
	, m_fRange(0)
{

}

CMapTool::~CMapTool()
{
}

void CMapTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_TerrainX);
	DDX_Text(pDX, IDC_EDIT2, m_TerrainZ);
	DDX_Text(pDX, IDC_EDIT3, m_TerrainScale);
	DDX_Control(pDX, IDC_EDIT4, m_Height);
	DDX_Control(pDX, IDC_EDIT5, m_Range);
	DDX_Control(pDX, IDC_RADIO1, m_MouseOption[0]);
	DDX_Control(pDX, IDC_RADIO2, m_MouseOption[1]);
	DDX_Control(pDX, IDC_RADIO3, m_MouseOption[2]);
	DDX_Control(pDX, IDC_RADIO4, m_MouseOption[3]);
	DDX_Control(pDX, IDC_RADIO5, m_MouseOption[4]);
	DDX_Control(pDX, IDC_RADIO6, m_MouseOption[5]);
	DDX_Control(pDX, IDC_RADIO7, m_MouseOption[6]);
	DDX_Control(pDX, IDC_LIST1, m_TextureList);
	DDX_Control(pDX, IDC_SLIDER1, m_fHeightCtrl);
	DDX_Control(pDX, IDC_SLIDER2, m_fRangeCtrl);

}


BEGIN_MESSAGE_MAP(CMapTool, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CMapTool::OnBnClickedTerrainCreate)
	ON_LBN_SELCHANGE(IDC_LIST1, &CMapTool::OnLbnSelchangeTextureList)
	ON_WM_DROPFILES()
	ON_EN_CHANGE(IDC_EDIT4, &CMapTool::OnEnChangeHeight)
	ON_WM_HSCROLL()
	ON_EN_CHANGE(IDC_EDIT5, &CMapTool::OnEnChangeRange)
	ON_BN_CLICKED(IDC_BUTTON6, &CMapTool::OnBnClickedTerrainSave)
	ON_BN_CLICKED(IDC_BUTTON7, &CMapTool::OnBnClickedTerrainLoad)
END_MESSAGE_MAP()


// CMapTool 메시지 처리기입니다.


void CMapTool::OnBnClickedTerrainCreate()
{
	UpdateData(TRUE);

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CToolView*	pView = dynamic_cast<CToolView*>(pMain->m_MainSplitter.GetPane(0, 1));

	auto iter = find_if(pView->m_mapLayer.begin(), pView->m_mapLayer.end(), CTag_Finder(L"Terrain"));
	if (iter != pView->m_mapLayer.end())
		iter->second->Free();

	Engine::CGameObject*		pGameObject = nullptr;

	VECVTX temp;
	temp.XZScale = _vec3(_float(m_TerrainX), m_TerrainScale, _float(m_TerrainZ));
	temp.pVtx = nullptr;
	lstrcpy(temp.tagTexture, L"Sand");
	// Terrain
	pGameObject = CToolTerrain::Create(pView->m_pGraphicDev, &temp);
	NULL_CHECK_RETURN(pGameObject, );

	pView->m_mapLayer.find(L"Terrain")->second->Add_GameObject(L"Terrain", pGameObject);

	UpdateData(FALSE);
}


void CMapTool::OnLbnSelchangeTextureList()
{
	UpdateData(TRUE);

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CToolView*	pView = dynamic_cast<CToolView*>(pMain->m_MainSplitter.GetPane(0, 1));

	int iIndex = m_TextureList.GetCurSel();
	if (LB_ERR == iIndex)
		return;

	CString strFileName;
	m_TextureList.GetText(iIndex, strFileName);
	memset(m_Texture, 0, sizeof(TCHAR[MAX_PATH]));
	lstrcpy(m_Texture, strFileName.Left(strFileName.GetLength() - 1));
	strFileName = strFileName[strFileName.GetLength() - 1];

	CToolTerrain* pTerrain = nullptr;

	auto iter = find_if(pView->m_mapLayer.begin(), pView->m_mapLayer.end(), CTag_Finder(L"Terrain"));
	if (iter != pView->m_mapLayer.end())
		pTerrain = (CToolTerrain*)iter->second->Get_GameObject(L"Terrain");

	pTerrain->Change_Texture(_ttoi(strFileName));
	m_TextureIndex = _ttoi(strFileName);


	UpdateData(FALSE);
}


BOOL CMapTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_MouseOption[0].SetCheck(TRUE);
	m_MouseOption[3].SetCheck(TRUE);
	m_MouseOption[5].SetCheck(TRUE);


	m_fHeightCtrl.SetRange(0, 100);
	m_fHeightCtrl.SetPos(0);
	m_fHeightCtrl.SetLineSize(1);

	int iHeight = m_fHeightCtrl.GetPos();
	CString sHeight;
	sHeight.Format(L"%f", iHeight);
	m_Height.SetWindowTextW(sHeight);


	m_fRangeCtrl.SetRange(0, 100);
	m_fRangeCtrl.SetPos(5);
	m_fRangeCtrl.SetLineSize(1);

	int iRange = m_fRangeCtrl.GetPos();
	CString sRange;
	sRange.Format(L"%f", iRange);
	m_Range.SetWindowTextW(sRange);

	return TRUE;
}

void CMapTool::HorizontalScroll()
{
	int iCX = 0;
	CString strFilePath = L"";
	CSize tSize{};

	CDC* pDC = m_TextureList.GetDC();
	for (int i = 0; i < m_TextureList.GetCount(); ++i)
	{
		m_TextureList.GetText(i, strFilePath);
		tSize = pDC->GetTextExtent(strFilePath);

		if (iCX < tSize.cx)
			iCX = tSize.cx;
	}

	if (iCX > m_TextureList.GetHorizontalExtent())
		m_TextureList.SetHorizontalExtent(iCX);
	ReleaseDC(pDC);
}

void CMapTool::OnDropFiles(HDROP hDropInfo)
{
	UpdateData(TRUE);

	TCHAR szFilePath[MAX_PATH] = L"";
	int iIndex = DragQueryFile(hDropInfo, -1, nullptr, 0);

	TCHAR szFileName[MAX_PATH] = L"";
	for (int i = 0; i < iIndex; ++i)
	{
		DragQueryFile(hDropInfo, i, szFilePath, MAX_PATH);
		CString strRelativePath = CFileInfo::ConvertRelativePath(szFilePath);
		CString strFileNameAndExtant = PathFindFileName(szFilePath);
		lstrcpy(szFileName, strFileNameAndExtant.GetString());
		PathRemoveExtension(szFileName);
		m_TextureList.AddString(szFileName);
	}

	HorizontalScroll();
	UpdateData(FALSE);

	CDialog::OnDropFiles(hDropInfo);
}

void CMapTool::Update_MapTool()
{
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CToolView*	pView = dynamic_cast<CToolView*>(pMain->m_MainSplitter.GetPane(0, 1));

	// WIREFRAME
	if (m_MouseOption[3].GetCheck() == TRUE)
		pView->m_bWireFrame = true;
	else if (m_MouseOption[4].GetCheck() == TRUE)
		pView->m_bWireFrame = false;

	// MOUSE_PICKING
	if (m_MouseOption[0].GetCheck() == TRUE)
		m_Mul = 1.f;
	else if (m_MouseOption[1].GetCheck() == TRUE)
		m_Mul = -1.f;


	// Round, Edge
	if (m_MouseOption[5].GetCheck() == TRUE)
		m_Round1Edge2Flat3 = 1;
	else if (m_MouseOption[6].GetCheck() == TRUE)
		m_Round1Edge2Flat3 = 2;
	else if (m_MouseOption[2].GetCheck() == TRUE)
		m_Round1Edge2Flat3 = 3;

	// Height, Range
	m_fHeight = m_fHeightCtrl.GetPos() / 1000.f;
	m_fRange = _float(m_fRangeCtrl.GetPos());
}

void CMapTool::OnEnChangeHeight()
{
	CString sHeight;
	m_Height.GetWindowTextW(sHeight);
	int Height = _ttoi(sHeight);
	m_fHeightCtrl.SetPos(Height);
}

void CMapTool::OnEnChangeRange()
{
	CString sRange;
	m_Range.GetWindowTextW(sRange);
	int Range = _ttoi(sRange);
	m_fRangeCtrl.SetPos(Range);
}



void CMapTool::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (IDC_SLIDER1 == pScrollBar->GetDlgCtrlID())
	{
		int iHeight = m_fHeightCtrl.GetPos();
		CString sHeight;
		sHeight.Format(L"%d", iHeight);
		m_Height.SetWindowTextW(sHeight);
	}

	if (IDC_SLIDER2 == pScrollBar->GetDlgCtrlID())
	{
		int iRange = m_fRangeCtrl.GetPos();
		CString sRange;
		sRange.Format(L"%d", iRange);
		m_Range.SetWindowTextW(sRange);
	}

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}




void CMapTool::OnBnClickedTerrainSave()
{
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CToolView*	pView = dynamic_cast<CToolView*>(pMain->m_MainSplitter.GetPane(0, 1));

	CFileDialog Dlg(FALSE, L"dat", L"*.dat", OFN_OVERWRITEPROMPT, L"Data File(*.dat)|*.dat||", this);

	TCHAR szBuf[MAX_PATH] = L"";
	GetCurrentDirectory(MAX_PATH, szBuf);

	PathRemoveFileSpec(szBuf);
	PathRemoveFileSpec(szBuf);
	lstrcat(szBuf, L"\\Data");
	Dlg.m_ofn.lpstrInitialDir = szBuf;

	if (IDOK == Dlg.DoModal())
	{
		CString strFilePath = Dlg.GetPathName();
		HANDLE hFile = CreateFile(strFilePath.GetString(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		DWORD dwByte = 0;

		auto iter = find_if(pView->m_mapLayer.begin(), pView->m_mapLayer.end(), CTag_Finder(L"Terrain"));
		if (iter == pView->m_mapLayer.end())
			return;
		CToolTerrain* pTerrain = (CToolTerrain*)iter->second->Get_GameObject(L"Terrain");


		WriteFile(hFile, &m_TerrainX, sizeof(DWORD), &dwByte, nullptr);
		WriteFile(hFile, &m_TerrainZ, sizeof(DWORD), &dwByte, nullptr);
		WriteFile(hFile, &m_Texture, MAX_PATH, &dwByte, nullptr);
		WriteFile(hFile, &m_TextureIndex, sizeof(unsigned int), &dwByte, nullptr);


		VTXTEX* Temp = (VTXTEX*)pTerrain->m_pBufferCom->Get_Vertices();
		for (_ulong i = 0; i < _ulong(m_TerrainX * m_TerrainZ); ++i)
		{
			WriteFile(hFile, &Temp[i], sizeof(VTXTEX), &dwByte, nullptr);
		}

		CloseHandle(hFile);
	}
}


void CMapTool::OnBnClickedTerrainLoad()
{
	UpdateData(TRUE);

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CToolView*	pView = dynamic_cast<CToolView*>(pMain->m_MainSplitter.GetPane(0, 1));

	CFileDialog Dlg(TRUE, L"dat", L"*.dat", OFN_OVERWRITEPROMPT, L"Data File(*.dat)|*.dat||", this);

	TCHAR szBuf[MAX_PATH] = L"";
	GetCurrentDirectory(MAX_PATH, szBuf);

	PathRemoveFileSpec(szBuf);
	lstrcat(szBuf, L"\\Data");
	Dlg.m_ofn.lpstrInitialDir = szBuf;

	if (IDOK == Dlg.DoModal())
	{
		CString strFilePath = Dlg.GetPathName();
		HANDLE hFile = CreateFile(strFilePath.GetString(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		DWORD dwByte = 0;
		int iCntX = 0;
		int iCntZ = 0;
		TCHAR szBuff[MAX_PATH];
		unsigned int iIndex = 0;

		auto iter = find_if(pView->m_mapLayer.begin(), pView->m_mapLayer.end(), CTag_Finder(L"Terrain"));
		if (iter == pView->m_mapLayer.end())
			return;

		ReadFile(hFile, &iCntX, sizeof(DWORD), &dwByte, nullptr);
		ReadFile(hFile, &iCntZ, sizeof(DWORD), &dwByte, nullptr);
		ReadFile(hFile, &szBuff, MAX_PATH, &dwByte, nullptr);
		ReadFile(hFile, &iIndex, sizeof(unsigned int), &dwByte, nullptr);

		VECVTX temp;
		temp.XZScale = _vec3(_float(iCntX), 1, _float(iCntZ));
		lstrcpy(temp.tagTexture, szBuff);
		temp.pVtx = new VTXTEX[iCntZ * iCntX];
		_int i = 0;
		while (true)
		{
			ReadFile(hFile, &temp.pVtx[i], sizeof(VTXTEX), &dwByte, nullptr);
			if (0 == dwByte)
				break;
			++i;
		}
		
		iter->second->Free();
		Engine::CGameObject*		pGameObject = nullptr;

		// Terrain
		pGameObject = CToolTerrain::Create(pView->m_pGraphicDev, &temp);
		NULL_CHECK_RETURN(pGameObject, );
		pView->m_mapLayer.find(L"Terrain")->second->Add_GameObject(L"Terrain", pGameObject);

		m_TerrainX = iCntX;
		m_TerrainZ = iCntZ;
		m_TerrainScale = 1;
		lstrcpy(m_Texture, temp.tagTexture);
		dynamic_cast<CToolTerrain*>(pGameObject)->Change_Texture(iIndex);

		Safe_Delete_Array(temp.pVtx);

		CloseHandle(hFile);
	}
	UpdateData(FALSE);
}
