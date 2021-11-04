// MapTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "MapTool.h"
#include "afxdialogex.h"
#include "FileInfo.h"
#include "MainFrm.h"
#include "ToolView.h"
#include "Terrain.h"
// MapTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CMapTool, CDialog)

CMapTool::CMapTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_MAPTOOL, pParent)
	, m_iDrawID(0)
{

}

CMapTool::~CMapTool()
{
}

void CMapTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListBox);
	DDX_Control(pDX, IDC_PICTURE, m_Picture);
}


BEGIN_MESSAGE_MAP(CMapTool, CDialog)
	ON_WM_DROPFILES()
	ON_LBN_SELCHANGE(IDC_LIST1, &CMapTool::OnLbnSelchangePicture)
	ON_BN_CLICKED(IDC_BUTTON1, &CMapTool::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CMapTool::OnBnClickedButton2)
END_MESSAGE_MAP()


// MapTool 메시지 처리기입니다.


void CMapTool::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	int i = 0; 
	// 0000 0001
	// 1000 0001
	// 1000 0010 == 0 ? 
	// 1111 1110 // 1의보수 
	// 1111 1111
	UpdateData(TRUE); 
	TCHAR szFilePath[MAX_PATH] = L""; 
	//드래그해서 던진 파일의 정보를 얻어오는 함수. 
	// 만약 2번째 인자에 -1 혹은 0xffffffff을 넣어주게 되면 드래그해서 던진 파일의 총 갯수를 반환을 해준다. 
	int iIndex = DragQueryFile(hDropInfo, 0xffffffff, nullptr, 0); 
	TCHAR szFileName[MAX_PATH] = L""; 
	for (int i = 0 ; i < iIndex ; ++i)
	{
		DragQueryFile(hDropInfo, i, szFilePath, MAX_PATH); 
		CString strRelativePath = CFileInfo::ConvertRelativePath(szFilePath);
		CString strFileNameAndExtant = PathFindFileName(szFilePath);
		lstrcpy(szFileName, strFileNameAndExtant.GetString()); 
		PathRemoveExtension(szFileName); 
		m_ListBox.AddString(szFileName);
	}
	HorizontalScroll(); 
	UpdateData(FALSE);
	CDialog::OnDropFiles(hDropInfo);
}

void CMapTool::HorizontalScroll()
{
	int iCX = 0; 
	CString strFilePath = L""; 
	CSize tSize{}; 

	CDC* pDC = m_ListBox.GetDC(); 
	for (int i = 0 ; i < m_ListBox.GetCount(); ++i)
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


void CMapTool::OnLbnSelchangePicture()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE); 
	int iIndex = m_ListBox.GetCurSel(); 
	if (LB_ERR == iIndex)
		return; 
	CString strFileName;
	m_ListBox.GetText(iIndex, strFileName); 

	int iLength = strFileName.GetLength(); 
	int i = 0; 
	for (; i < iLength; ++i)
	{
		// isdigit - 문자가 0~9인지까지 검사하는 함수. 
		if (isdigit(strFileName[i]))
			break; 
	}
	// 원하는 인덱스 번째 부터 원하는 인덱스 번째까지 문자 잘라주는 함수. 
	strFileName.Delete(0, i); 

	//눈치가 아주 빨라졌어. 
	m_iDrawID = _ttoi(strFileName.GetString()); 
	CGraphic_Device::Get_Instance()->Render_Begin(); 
	const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"Terrain", L"Tile", m_iDrawID);
	if (nullptr == pTexInfo)
		return; 
	float fCenterX = pTexInfo->tImageInfo.Width >> 1; 
	float fCenterY = pTexInfo->tImageInfo.Height / 2; 
	float fSizeX = float(WINCX) / TILECX ;
	float fSizeY = static_cast<float>(WINCY) / TILECY;
	D3DXMATRIX matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, fSizeX, fSizeY, 0.f);
	D3DXMatrixTranslation(&matTrans, WINCX >> 1, WINCY >> 1, 0.f);

	matWorld = matScale * matTrans; 
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld); 
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255)); 


	CGraphic_Device::Get_Instance()->Render_End(m_Picture.m_hWnd);
	UpdateData(FALSE); 
}


void CMapTool::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog Dlg(FALSE, L"dat", L"*.dat", OFN_OVERWRITEPROMPT, L"Data File(*.dat)|*.dat||", this);

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
		// 자 문근용아 여기에선 솔직히 딱히 설명할게 없었었어.. 이거 유닛툴에서 복사해온거야... 
		// 여기서부터 달라 오키 도키 ? 
		CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
		CToolView* pView = dynamic_cast<CToolView*>(pMain->m_MainSplitter.GetPane(0, 1)); 
		vector<TILE*> vecTile = pView->m_pTerrain->Get_Tile(); 
		// 자 근용아 여기에서는 굳이 따로 저장하고 불러올 필요가 없을거 같아.. 그치? 
		//과한 주석을 
		//원했으니
		//내가 지금 확실하게 
		//주석다는거야. 
		// 주석은 달면 달수록 
		//좋고 
		//코드는 짜면 짤수록
		//좋고 
		///뭐유? 없는데유 ? 
		for (auto& pTile : vecTile)
			WriteFile(hFile, pTile, sizeof(TILE), &dwByte, nullptr);
		// 자 이제 이렇게 되면 타일이 호로록하고 저장이 된단다 한번 해볼가? 자 컨트롤 f5를 눌러서 실행해보자. 
		CloseHandle(hFile);
	}
}


void CMapTool::OnBnClickedButton2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog Dlg(TRUE, L"dat", L"*.dat", OFN_OVERWRITEPROMPT, L"Data File(*.dat)|*.dat||", this);

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
		HANDLE hFile = CreateFile(strFilePath.GetString(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (INVALID_HANDLE_VALUE == hFile)
		{
			ERR_MSG(L"file open Failed");
			return;
		}

		DWORD dwByte = 0;
		// 자 문근용아 여기에선 솔직히 딱히 설명할게 없었었어.. 이거 유닛툴에서 복사해온거야... 
		// 여기서부터 달라 오키 도키 ? 
		CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
		CToolView* pView = dynamic_cast<CToolView*>(pMain->m_MainSplitter.GetPane(0, 1));
		pView->m_pTerrain->Release_Terrain(); 

		// 자 근용아 여기에서는 굳이 따로 저장하고 불러올 필요가 없을거 같아.. 그치? 
		//과한 주석을 
		//원했으니
		//내가 지금 확실하게 
		//주석다는거야. 
		// 주석은 달면 달수록 
		//좋고 
		//코드는 짜면 짤수록
		//좋고 
		///뭐유? 없는데유 ? 
		TILE* pTile = nullptr; 
		vector<TILE*> vecTile; 
		while (true)
		{
			pTile = new TILE; 
			ReadFile(hFile, pTile, sizeof(TILE), &dwByte, nullptr);
			if (0 == dwByte)
			{
				Safe_Delete(pTile); 
				break; 
			}
			vecTile.emplace_back(pTile); 
		}
		pView->m_pTerrain->Set_Tile(vecTile);
		// 자 이제 이렇게 되면 타일이 호로록하고 저장이 된단다 한번 해볼가? 자 컨트롤 f5를 눌러서 실행해보자. 
		pView->Invalidate(FALSE); 
		CloseHandle(hFile);
	}
}
