// Tap01.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFCTool.h"
#include "Tap01.h"
#include "afxdialogex.h"

#include "FileInfo.h"
#include "MFCToolView.h"
extern CMFCToolView* g_pView;

#include "CubeColor.h"


// CTap01 대화 상자입니다.
#pragma region Creater

IMPLEMENT_DYNAMIC(CTap01, CDialog)

CTap01::CTap01(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_TAP01, pParent)
	, m_pManagementTool(CManagementTool::Get_Instance())///////////
	, iColoringStart(0), iColoringEnd(0), iColoringMoveNum(0)
	, iBoxMin(0), iBoxMax(0)
	, iBoxX(0), iBoxY(0), iBoxZ(0)
	, m_bCollision(FALSE)
	, iBoxSizeX(0), iBoxSizeY(0), iBoxSizeZ(0)
	, m_stCubeName(_T(""))
	, m_bCreateCube(false)
//	, m_stTextureList(_T(""))
{
	Safe_AddReference(m_pManagementTool);///////////
}

CTap01::~CTap01()
{
}
#pragma endregion 

#pragma region MFC_Dialog1
void CTap01::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_RADIO1, m_iColoringTexture[0]);
	DDX_Control(pDX, IDC_RADIO2, m_iColoringTexture[1]);
	DDX_Control(pDX, IDC_PICTURE, m_Picture);
	DDX_Control(pDX, IDC_LIST_TEXTUREFILE, m_ListBox);


	DDX_Control(pDX, IDC_SLIDER_ALPHA_, m_sldAlpha);
	DDX_Control(pDX, IDC_SLIDER_RED_, m_sldRed);
	DDX_Control(pDX, IDC_SLIDER_GREEN_, m_sldGreen);
	DDX_Control(pDX, IDC_SLIDER_BLUE_, m_sldBlue);

	DDX_Control(pDX, IDC_EDIT_ALPHA, m_eAlpha);
	DDX_Control(pDX, IDC_EDIT_RED, m_eRed);
	DDX_Control(pDX, IDC_EDIT_GREEN, m_eGreen);
	DDX_Control(pDX, IDC_EDIT_BLUE, m_eBlue);


	DDX_Control(pDX, IDC_EDIT_BoxXYZ, m_eBoxTotal);
	DDX_Check(pDX, IDC_CHECK1, m_bCollision);

	DDX_Text(pDX, IDC_EDIT_WRITENAME, m_stCubeName);
	DDX_Control(pDX, IDC_EDIT_WRITENAME, m_eCubeName);
}


BEGIN_MESSAGE_MAP(CTap01, CDialog)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_ALPHA_, &CTap01::Control_AlphaValue)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_RED_, &CTap01::Control_RedValue)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_GREEN_, &CTap01::Control_GreenValue)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_BLUE_, &CTap01::Control_BlueValue)

	ON_WM_DROPFILES()

	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_BoxX, &CTap01::OnDeltaposSpinBoxx)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_BoxY, &CTap01::OnDeltaposSpinBoxy)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_BoxZ, &CTap01::OnDeltaposSpinBoxz)

	ON_BN_CLICKED(IDC_BUTTON_LOAD_TEXTURE, &CTap01::OnBnClickedButtonLoadTexture)
	ON_BN_CLICKED(IDC_RADIO1, &CTap01::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CTap01::OnBnClickedRadio2)
	ON_LBN_SELCHANGE(IDC_LIST_TEXTUREFILE, &CTap01::OnLbnSelchangeListTexturefile)

	ON_EN_CHANGE(IDC_EDIT_WRITENAME, &CTap01::OnEnChangeEditWritename)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CTap01::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_LOAD, &CTap01::OnBnClickedButtonLoad)
	ON_BN_CLICKED(IDC_BUTTON_REMOVE, &CTap01::OnBnClickedButtonRemove)
	ON_BN_CLICKED(IDC_BUTTON_REMOVE2, &CTap01::OnBnClickedButtonRemove2)
END_MESSAGE_MAP()
#pragma endregion


// CTap01 메시지 처리기입니다.
#pragma region Tap1_Initialize
BOOL CTap01::OnInitDialog()
{
	CDialog::OnInitDialog();
	UpdateData(TRUE);

//	g_pView->m_pMainTool->Clone_CubeColor();

	DragAcceptFiles();
	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	m_vStartPoint = { 0.f, 0.f, 0.f };

	int iSize = 200;
	iBoxSizeX = iSize;
	iBoxSizeY = iSize;
	iBoxSizeZ = iSize;


	//행렬 셋팅.
	m_fm11ED = m_matWorld._11;
	m_fm12ED = m_matWorld._12;
	m_fm13ED = m_matWorld._13;

	m_fm21ED = m_matWorld._21;
	m_fm22ED = m_matWorld._22;
	m_fm23ED = m_matWorld._23;

	m_fm31ED = m_matWorld._31;
	m_fm32ED = m_matWorld._32;
	m_fm33ED = m_matWorld._33;

	m_fm41ED = m_matWorld._41;
	m_fm42ED = m_matWorld._42;
	m_fm43ED = m_matWorld._43;


	//Coloring
	iColoringStart = 0;
	iColoringEnd = 255;
	iColoringMoveNum = 10;

	m_sldAlpha.SetRange(iColoringStart, iColoringEnd);
	m_sldAlpha.SetPos(255);
	m_sldAlpha.SetLineSize(iColoringMoveNum);
	m_sldAlpha.SetPageSize(iColoringMoveNum);

	m_sldRed.SetRange(iColoringStart, iColoringEnd);
	m_sldRed.SetPos(255);
	m_sldRed.SetLineSize(iColoringMoveNum);
	m_sldRed.SetPageSize(iColoringMoveNum);

	m_sldGreen.SetRange(iColoringStart, iColoringEnd);
	m_sldGreen.SetPos(255);
	m_sldGreen.SetLineSize(iColoringMoveNum);
	m_sldGreen.SetPageSize(iColoringMoveNum);

	m_sldBlue.SetRange(iColoringStart, iColoringEnd);
	m_sldBlue.SetPos(255);
	m_sldBlue.SetLineSize(iColoringMoveNum);
	m_sldBlue.SetPageSize(iColoringMoveNum);

	//Texture
//	m_stTextureList = L"텍스쳐 파일을 드래그하세요.";

	//Etc
	int DefaultNum = 1;
	iBoxX = DefaultNum;
	iBoxY = DefaultNum;
	iBoxZ = DefaultNum;

	iBoxMin = 1;
	iBoxMax = 4;

	CString sValue;
	sValue.Format(_T("(%d, %d, %d) \n"), iBoxX, iBoxY, iBoxZ);
	m_eBoxTotal.SetWindowTextW(sValue);

	m_stCubeName = L"저장될 파일 이름을 입력하세요.";

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
#pragma endregion 

#pragma region ColoringPart
void CTap01::Control_AlphaValue(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	int iPos;

	if (m_iColoringTexture[0].GetCheck())		// Check Coloring
	{
		iPos = m_sldAlpha.GetPos();
	}
	else if (m_iColoringTexture[1].GetCheck())
	{
		PRINT_LOG(L"err", L"Coloring옵션이 선택되지 않았습니다.");
		return;
	}
	else
	{
		iPos = 0;
	}

	CString sPos;
	sPos.Format(_T("%d"), iPos);
	m_eAlpha.SetWindowText(sPos);

	Set_Cube();
	*pResult = 0;
}


void CTap01::Control_RedValue(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	int iPos;

	if (m_iColoringTexture[0].GetCheck())		// Check Coloring
	{
		iPos = m_sldRed.GetPos();
	}
	else if (m_iColoringTexture[1].GetCheck())
	{
		PRINT_LOG(L"err", L"Coloring옵션이 선택되지 않았습니다.");
		return;
	}
	else
	{
		iPos = 0;
	}

	CString sPos;
	sPos.Format(_T("%d"), iPos);
	m_eRed.SetWindowText(sPos);

	Set_Cube();

	*pResult = 0;
}


void CTap01::Control_GreenValue(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	int iPos;

	if (m_iColoringTexture[0].GetCheck())		// Check Coloring
	{
		iPos = m_sldGreen.GetPos();
	}
	else if (m_iColoringTexture[1].GetCheck())
	{
		PRINT_LOG(L"err", L"Coloring옵션이 선택되지 않았습니다.");
		return;
	}
	else
	{
		iPos = 0;
	}

	CString sPos;
	sPos.Format(_T("%d"), iPos);
	m_eGreen.SetWindowText(sPos);

	Set_Cube();

	*pResult = 0;
}


void CTap01::Control_BlueValue(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int iPos;

	if (m_iColoringTexture[0].GetCheck())		// Check Coloring
	{
		iPos = m_sldBlue.GetPos();
	}
	else if (m_iColoringTexture[1].GetCheck())
	{
		PRINT_LOG(L"err", L"Coloring옵션이 선택되지 않았습니다.");
		return;
	}
	else
	{
		iPos = 0;
	}
	CString sPos;
	sPos.Format(_T("%d"), iPos);
	m_eBlue.SetWindowText(sPos);

	Set_Cube();

	*pResult = 0;
}
#pragma endregion 


void CTap01::HorizontalScroll()
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


void CTap01::OnDeltaposSpinBoxx(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	int iVal = pNMUpDown->iPos + pNMUpDown->iDelta;

	if ((iBoxMin <= iVal) && (iVal <= iBoxMax))
	{
		CString sValue;
		sValue.Format(_T("(%d, %d, %d) \n"), iVal, iBoxY, iBoxZ);
		m_eBoxTotal.SetWindowTextW(sValue);

		iBoxX = iVal;
		Set_CubeSize();
	}

	*pResult = 0;
}


void CTap01::OnDeltaposSpinBoxy(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	int iVal = pNMUpDown->iPos + pNMUpDown->iDelta;

	if ((iBoxMin <= iVal) && (iVal <= iBoxMax))
	{
		CString sValue;
		sValue.Format(_T("(%d, %d, %d) \n"), iBoxX, iVal, iBoxZ);
		m_eBoxTotal.SetWindowTextW(sValue);

		iBoxY = iVal;
		Set_CubeSize();
	}

	*pResult = 0;
}


void CTap01::OnDeltaposSpinBoxz(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	int iVal = pNMUpDown->iPos + pNMUpDown->iDelta;

	if ((iBoxMin <= iVal) && (iVal <= iBoxMax))
	{
		CString sValue;
		sValue.Format(_T("(%d, %d, %d) \n"), iBoxX, iBoxY, iVal);
		m_eBoxTotal.SetWindowTextW(sValue);

		iBoxZ = iVal;
		Set_CubeSize();
	}

	*pResult = 0;
}


void CTap01::OnBnClickedButtonLoadTexture()
{
//	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//	UpdateData(TRUE);
//	//CFileDialog
//	CFileDialog Dlg(TRUE, L"dat", L"*.dat", OFN_OVERWRITEPROMPT, L"Data File(*.dat)|*.dat||", this);
//
//	TCHAR szBuf[MAX_PATH] = L"";
//	GetCurrentDirectory(MAX_PATH, szBuf);
//	//D:\박병건\112A\FrameWork
//	PathRemoveFileSpec(szBuf);
//	lstrcat(szBuf, L"\\Data");
//	Dlg.m_ofn.lpstrInitialDir = szBuf;
//	if (IDOK == Dlg.DoModal())
//	{
//		CString strFilePath = Dlg.GetPathName();
//		HANDLE hFile = CreateFile(strFilePath.GetString(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
//
//		if (INVALID_HANDLE_VALUE == hFile)
//		{
////			ERR_MSG(L"file open Failed");
//			return;
//		}
//		DWORD dwByte = 0;
//		DWORD dwStrByte = 0;
//		TCHAR* pTemp = nullptr;
//		UNITINFO* pUnitInfo = nullptr;
//
//		for (auto& rPair : m_mapUnitInfo)
//			Safe_Delete(rPair.second);
//		m_mapUnitInfo.clear();
//		m_ListBox.ResetContent();
//		while (true)
//		{
//			pUnitInfo = new UNITINFO;
//			ReadFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);
//			pTemp = new TCHAR[dwStrByte];
//			ReadFile(hFile, pTemp, dwStrByte, &dwByte, nullptr);
//			pUnitInfo->strName = pTemp;
//			if (pTemp)
//			{
//				delete pTemp;
//				pTemp = nullptr;
//			}
//			ReadFile(hFile, &pUnitInfo->iAtt, sizeof(int), &dwByte, nullptr);
//			ReadFile(hFile, &pUnitInfo->iMaxHp, sizeof(int), &dwByte, nullptr);
//			ReadFile(hFile, &pUnitInfo->byClass, sizeof(BYTE), &dwByte, nullptr);
//			ReadFile(hFile, &pUnitInfo->byItem, sizeof(BYTE), &dwByte, nullptr);
//			if (0 == dwByte)
//			{
//				Safe_Delete(pUnitInfo);
//				break;
//			}
//			m_mapUnitInfo.emplace(pUnitInfo->strName, pUnitInfo);
//			m_ListBox.AddString(pUnitInfo->strName);
//		}
//		CloseHandle(hFile);
//	}
//	UpdateData(FALSE);

}


void CTap01::Set_CubeSize()
{
	int iSize = iBoxSizeX;

	iBoxSizeX = iSize * iBoxX;
	iBoxSizeY = iSize * iBoxY;
	iBoxSizeZ = iSize * iBoxZ;

}

void CTap01::Set_CubeVertex()
{
	//버텍스 지정. 인덱스 순서는 클라이언트 순서를 지켜서 정렬.
}


void CTap01::OnBnClickedRadio1()		// Coloring선택시, Texture값 초기화
{
	m_sldAlpha.SetPos(0);
	m_sldRed.SetPos(0);
	m_sldGreen.SetPos(0);
	m_sldBlue.SetPos(0);

	if (!m_bCreateCube)
	{
		//m_pCube = g_pView->m_pMainTool->Clone_CubeColor();
		//m_pColorCube = dynamic_cast<CCubeColor*>(m_pCube);
		//m_pCube->Release();

//		m_pCube = g_pView->m_pMainTool->Clone_CubeColor();
		m_pColorCube = dynamic_cast<CCubeColor*>(g_pView->m_pMainTool->Clone_CubeColor());

		m_bCreateCube = true;
	}
	int i = 1;
}


void CTap01::OnBnClickedRadio2()		// Texture선택시, Coloring값 초기화
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_sldAlpha.SetPos(0);
	m_sldRed.SetPos(0);
	m_sldGreen.SetPos(0);
	m_sldBlue.SetPos(0);

	m_pCube = g_pView->m_pMainTool->Clone_CubeColor();
}


void CTap01::OnLbnSelchangeListTexturefile()
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

	m_iDrawID = _ttoi(strFileName.GetString());

	///그래픽 디바이스 생성 후 연결.
	//CGraphic_Device::Get_Instance()->Render_Begin();
	if (nullptr == m_pManagementTool)
		return;

	m_pDevice = m_pManagementTool->Get_Device();

	//if (FAILED(m_pManagementTool->Add_Component_Prototype(
	//	L"Component_Texture_Plate",
	//	CTextureTool::Create(m_pDevice, ETextureType::Normal, L"../Resources/Plate_Texture%d.png", 1))))
	//{
	//	PRINT_LOG(L"CMainTool", L"Plate / Texture / Prototype 실패");
	//	return E_FAIL;
	//}

//	const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"Cube", L"TheCube", m_iDrawID);
//	PrototypeImageFile()
	//if (nullptr == pTexInfo)
	//	return;

	//float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	//float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	//float fSizeX = float(WINCX) / TILECX;
	//float fSizeY = static_cast<float>(WINCY) / TILECY;

	//D3DXMATRIX matScale, matTrans, matWorld;
	//D3DXMatrixScaling(&matScale, fSizeX / 5, fSizeY / 5, 0.f);
	//D3DXMatrixTranslation(&matTrans, WINCX >> 1, WINCY >> 1, 0.f);

	//matWorld = matScale * matTrans;
	//CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	//CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	//CGraphic_Device::Get_Instance()->Render_End(m_Picture.m_hWnd);
	UpdateData(FALSE);

}


void CTap01::OnEnChangeEditWritename()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialog::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	m_eCubeName.GetWindowText(m_stCubeName);

	UpdateData(FALSE);

//	m_stCubeName
}


void CTap01::OnBnClickedButtonSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//CFileDialog



	//////////////////예외처리/////////////////////////////////////////////////////////
	if (m_iColoringTexture[0].GetCheck())
	{

	}
	else if (m_iColoringTexture[1].GetCheck())
	{

	}
	else
	{
		PRINT_LOG(L"err", L"컬러 혹은 텍스쳐 옵션을 선택하시오.");
		return;
	}


	if (m_stCubeName == L"저장될 파일 이름을 입력하세요.")
	{
		PRINT_LOG(L"err", L"이름을 입력하시오.");
		return;
	}
	//else			//이름값 잘 저장됬는지 확인용.
	//{
	//	MessageBox(m_stCubeName);
	//}

	/////////////////////////////////////////////////////////

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
//			ERR_MSG(L"file Failed");
			return;
		}

		DWORD dwByte = 0;
		DWORD dwStrByte = 0;

		SCubeObject* pCubeObject = new SCubeObject;

		if (m_iColoringTexture[0].GetCheck())		// Check Coloring
		{
			pCubeObject->m_iAlpha = m_sldAlpha.GetPos();
			pCubeObject->m_iRed = m_sldRed.GetPos();
			pCubeObject->m_iGreen = m_sldGreen.GetPos();
			pCubeObject->m_iBlue = m_sldBlue.GetPos();

			//		pCubeObject->m_stTextureName = nullptr;

			pCubeObject->m_stCubeName = m_stCubeName;
			pCubeObject->m_matWorld = m_matWorld;
			pCubeObject->m_vBoxTotal = { (float)iBoxX , (float)iBoxY, (float)iBoxZ }; // 필요없지않을까....
			pCubeObject->m_bCollision = m_bCollision;

			Set_CubeVertex();

		}
		else if (m_iColoringTexture[1].GetCheck())		// Check Texture
		{
			pCubeObject->m_iAlpha = 0;
			pCubeObject->m_iRed = 0;
			pCubeObject->m_iGreen = 0;
			pCubeObject->m_iBlue = 0;

			//		pBoxObject->m_stTextureName = nullptr;

			pCubeObject->m_stCubeName = m_stCubeName;
			pCubeObject->m_matWorld = m_matWorld;
			pCubeObject->m_vBoxTotal = { (float)iBoxX , (float)iBoxY, (float)iBoxZ };
			pCubeObject->m_bCollision = m_bCollision;

			Set_CubeVertex();

		}
		else
		{
			PRINT_LOG(L"err", L"컬러 혹은 텍스쳐 옵션을 선택하시오.");
			return;
		}

		for (auto& rPair : m_mapCubeInfo)
		{
			dwStrByte = sizeof(TCHAR) * (rPair.first.GetLength() + 1);
			WriteFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);
			WriteFile(hFile, rPair.first.GetString(), dwStrByte, &dwByte, nullptr);

			WriteFile(hFile, &rPair.second->m_iAlpha, sizeof(int), &dwByte, nullptr);
			WriteFile(hFile, &rPair.second->m_iRed, sizeof(int), &dwByte, nullptr);
			WriteFile(hFile, &rPair.second->m_iGreen, sizeof(int), &dwByte, nullptr);
			WriteFile(hFile, &rPair.second->m_iBlue, sizeof(int), &dwByte, nullptr);

			WriteFile(hFile, &rPair.second->m_stCubeName, sizeof(CString), &dwByte, nullptr);

			WriteFile(hFile, &rPair.second->m_matWorld, sizeof(D3DXMATRIX), &dwByte, nullptr);
			WriteFile(hFile, &rPair.second->m_vBoxTotal, sizeof(D3DXVECTOR3), &dwByte, nullptr);
			WriteFile(hFile, &rPair.second->m_bCollision, sizeof(bool), &dwByte, nullptr);
		}
		CloseHandle(hFile);
	}

}


void CTap01::OnBnClickedButtonLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	//CFileDialog
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
		{
//			ERR_MSG(L"file open Failed");
			return;
		}
		DWORD dwByte = 0;
		DWORD dwStrByte = 0;
		TCHAR* pTemp = nullptr;
		SCubeObject* pCubeObject = nullptr;

		for (auto& rPair : m_mapCubeInfo)
		{
//			Safe_Delete_MFC(rPair.second);
		}
		m_mapCubeInfo.clear();
		m_ListBox.ResetContent();
		while (true)
		{
			pCubeObject = new SCubeObject;
			ReadFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);
			pTemp = new TCHAR[dwStrByte];
			ReadFile(hFile, pTemp, dwStrByte, &dwByte, nullptr);
			pCubeObject->m_stCubeName = pTemp;
			if (pTemp)
			{
//				Safe_Delete_MFC(pTemp);
			}

			ReadFile(hFile, &pCubeObject->m_iAlpha, sizeof(int), &dwByte, nullptr);
			ReadFile(hFile, &pCubeObject->m_iRed, sizeof(int), &dwByte, nullptr);
			ReadFile(hFile, &pCubeObject->m_iGreen, sizeof(int), &dwByte, nullptr);
			ReadFile(hFile, &pCubeObject->m_iBlue, sizeof(int), &dwByte, nullptr);

//			ReadFile(hFile, &pCubeObject->m_stCubeName, sizeof(CString), &dwByte, nullptr);
			ReadFile(hFile, &pCubeObject->m_matWorld, sizeof(D3DXMATRIX), &dwByte, nullptr);
			ReadFile(hFile, &pCubeObject->m_vBoxTotal, sizeof(D3DXVECTOR3), &dwByte, nullptr);
			ReadFile(hFile, &pCubeObject->m_bCollision, sizeof(bool), &dwByte, nullptr);

			////////////////////////

			if (0 == dwByte)
			{
//				Safe_Delete_MFC(pCubeObject);
				break;
			}
			m_mapCubeInfo.emplace(pCubeObject->m_stCubeName, pCubeObject);
			m_ListBox.AddString(pCubeObject->m_stCubeName);
		}
		CloseHandle(hFile);
	}
	UpdateData(FALSE);

}


void CTap01::OnBnClickedButtonRemove()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	int iIndex = m_ListBox.GetCurSel();
	if (LB_ERR == iIndex)
		return;

	CString strfindName;
	m_ListBox.GetText(iIndex, strfindName);

	auto& iter_find = m_mapCubeInfo.find(strfindName);
	if (iter_find == m_mapCubeInfo.end())
		return;

//	Safe_Delete_MFC(iter_find->second);
	m_mapCubeInfo.erase(iter_find);
	m_ListBox.DeleteString(iIndex);

	UpdateData(FALSE);

}


void CTap01::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	if (m_iColoringTexture[0].GetCheck())		// Check Coloring
	{
		PRINT_LOG(L"err", L"Texture옵션이 선택되지 않았습니다.");
		return;
	}
	else if (m_iColoringTexture[1].GetCheck())
	{

	}
	else
	{
		PRINT_LOG(L"err", L"Texture옵션이 선택되지 않았습니다.");
		return;
	}

	UpdateData(TRUE);
	TCHAR szFilePath[MAX_PATH] = L"";
	int iIndex = DragQueryFile(hDropInfo, 0xffffffff, nullptr, 0);
	TCHAR szFileName[MAX_PATH] = L"";

	for (int i = 0; i < iIndex; ++i)
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

void CTap01::Set_Cube()
{

	if (m_iColoringTexture[0].GetCheck())
	{
		int iA = m_sldAlpha.GetPos();
		int iR = m_sldRed.GetPos();
		int iG = m_sldGreen.GetPos();
		int iB = m_sldBlue.GetPos();

		if (nullptr != m_pColorCube)
		{
			m_pColorCube->Set_ChangeColor(iA, iR, iG, iB, 0);
		}
	}
	else if (m_iColoringTexture[1].GetCheck())
	{

	}
}


void CTap01::OnBnClickedButtonRemove2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
