// MapTool.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "MapTool.h"
#include "afxdialogex.h"
#include "FileInfo.h"
#include "MainFrm.h"
#include "ToolView.h"
#include "Terrain.h"
// MapTool ��ȭ �����Դϴ�.

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


// MapTool �޽��� ó�����Դϴ�.


void CMapTool::OnDropFiles(HDROP hDropInfo)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	int i = 0; 
	// 0000 0001
	// 1000 0001
	// 1000 0010 == 0 ? 
	// 1111 1110 // 1�Ǻ��� 
	// 1111 1111
	UpdateData(TRUE); 
	TCHAR szFilePath[MAX_PATH] = L""; 
	//�巡���ؼ� ���� ������ ������ ������ �Լ�. 
	// ���� 2��° ���ڿ� -1 Ȥ�� 0xffffffff�� �־��ְ� �Ǹ� �巡���ؼ� ���� ������ �� ������ ��ȯ�� ���ش�. 
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

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
		// isdigit - ���ڰ� 0~9�������� �˻��ϴ� �Լ�. 
		if (isdigit(strFileName[i]))
			break; 
	}
	// ���ϴ� �ε��� ��° ���� ���ϴ� �ε��� ��°���� ���� �߶��ִ� �Լ�. 
	strFileName.Delete(0, i); 

	//��ġ�� ���� ��������. 
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CFileDialog Dlg(FALSE, L"dat", L"*.dat", OFN_OVERWRITEPROMPT, L"Data File(*.dat)|*.dat||", this);

	TCHAR szBuf[MAX_PATH] = L"";
	GetCurrentDirectory(MAX_PATH, szBuf);
	//D:\�ں���\112A\FrameWork
	PathRemoveFileSpec(szBuf);
	lstrcat(szBuf, L"\\Data");
	Dlg.m_ofn.lpstrInitialDir = szBuf;
	if (IDOK == Dlg.DoModal())
	{
		// ������ ��� ������ �Լ�
		CString strFilePath = Dlg.GetPathName();
		HANDLE hFile = CreateFile(strFilePath.GetString(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (INVALID_HANDLE_VALUE == hFile)
		{
			ERR_MSG(L"file open Failed");
			return;
		}

		DWORD dwByte = 0;
		// �� ���ٿ�� ���⿡�� ������ ���� �����Ұ� ��������.. �̰� ���������� �����ؿ°ž�... 
		// ���⼭���� �޶� ��Ű ��Ű ? 
		CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
		CToolView* pView = dynamic_cast<CToolView*>(pMain->m_MainSplitter.GetPane(0, 1)); 
		vector<TILE*> vecTile = pView->m_pTerrain->Get_Tile(); 
		// �� �ٿ�� ���⿡���� ���� ���� �����ϰ� �ҷ��� �ʿ䰡 ������ ����.. ��ġ? 
		//���� �ּ��� 
		//��������
		//���� ���� Ȯ���ϰ� 
		//�ּ��ٴ°ž�. 
		// �ּ��� �޸� �޼��� 
		//���� 
		//�ڵ�� ¥�� ©����
		//���� 
		///����? ���µ��� ? 
		for (auto& pTile : vecTile)
			WriteFile(hFile, pTile, sizeof(TILE), &dwByte, nullptr);
		// �� ���� �̷��� �Ǹ� Ÿ���� ȣ�η��ϰ� ������ �ȴܴ� �ѹ� �غ���? �� ��Ʈ�� f5�� ������ �����غ���. 
		CloseHandle(hFile);
	}
}


void CMapTool::OnBnClickedButton2()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CFileDialog Dlg(TRUE, L"dat", L"*.dat", OFN_OVERWRITEPROMPT, L"Data File(*.dat)|*.dat||", this);

	TCHAR szBuf[MAX_PATH] = L"";
	GetCurrentDirectory(MAX_PATH, szBuf);
	//D:\�ں���\112A\FrameWork
	PathRemoveFileSpec(szBuf);
	lstrcat(szBuf, L"\\Data");
	Dlg.m_ofn.lpstrInitialDir = szBuf;
	if (IDOK == Dlg.DoModal())
	{
		// ������ ��� ������ �Լ�
		CString strFilePath = Dlg.GetPathName();
		HANDLE hFile = CreateFile(strFilePath.GetString(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (INVALID_HANDLE_VALUE == hFile)
		{
			ERR_MSG(L"file open Failed");
			return;
		}

		DWORD dwByte = 0;
		// �� ���ٿ�� ���⿡�� ������ ���� �����Ұ� ��������.. �̰� ���������� �����ؿ°ž�... 
		// ���⼭���� �޶� ��Ű ��Ű ? 
		CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
		CToolView* pView = dynamic_cast<CToolView*>(pMain->m_MainSplitter.GetPane(0, 1));
		pView->m_pTerrain->Release_Terrain(); 

		// �� �ٿ�� ���⿡���� ���� ���� �����ϰ� �ҷ��� �ʿ䰡 ������ ����.. ��ġ? 
		//���� �ּ��� 
		//��������
		//���� ���� Ȯ���ϰ� 
		//�ּ��ٴ°ž�. 
		// �ּ��� �޸� �޼��� 
		//���� 
		//�ڵ�� ¥�� ©����
		//���� 
		///����? ���µ��� ? 
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
		// �� ���� �̷��� �Ǹ� Ÿ���� ȣ�η��ϰ� ������ �ȴܴ� �ѹ� �غ���? �� ��Ʈ�� f5�� ������ �����غ���. 
		pView->Invalidate(FALSE); 
		CloseHandle(hFile);
	}
}
