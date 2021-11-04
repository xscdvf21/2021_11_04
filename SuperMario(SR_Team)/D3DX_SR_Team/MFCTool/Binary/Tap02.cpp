// Tap02.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFCTool.h"
#include "Tap02.h"
#include "afxdialogex.h"

#include "MFCToolView.h"
extern CMFCToolView* g_pView;

// CTap02 대화 상자입니다.

#pragma region Creater
IMPLEMENT_DYNAMIC(CTap02, CDialog)

CTap02::CTap02(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_TAP02, pParent)
	, m_fMovingSize(0.f)
{
	ZeroMemory(&m_vObjPoint, sizeof(D3DXVECTOR3));
	ZeroMemory(&m_vPreObjPoint, sizeof(D3DXVECTOR3));
}

CTap02::~CTap02()
{
}
#pragma endregion 

#pragma region MFC_Dialog3
void CTap02::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_EDIT_OBJECTPOINT, m_eObjectPoint);

}


BEGIN_MESSAGE_MAP(CTap02, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_CUBELOAD, &CTap02::OnBnClickedButtonCubeload)
	ON_BN_CLICKED(IDC_BUTTON_MONSTERLOAD, &CTap02::OnBnClickedButtonMonsterload)
	ON_BN_CLICKED(IDC_BUTTON_ETCLOAD, &CTap02::OnBnClickedButtonEtcload)

	ON_BN_CLICKED(IDC_BUTTON_INSTALL, &CTap02::OnBnClickedButtonInstall)
	ON_BN_CLICKED(IDC_BUTTON_SAVEALL, &CTap02::OnBnClickedButtonSaveall)
	ON_EN_CHANGE(IDC_EDIT_OBJECTPOINT, &CTap02::OnEnChangeEditObjectpoint)
END_MESSAGE_MAP()
#pragma endregion

// CTap02 메시지 처리기입니다.

#pragma region Tap3_Initialize
BOOL CTap02::OnInitDialog()
{
	CDialog::OnInitDialog();
	UpdateData(TRUE);

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_vObjPoint = { 0.f, 0.f, 0.f };
	m_vPreObjPoint = m_vObjPoint;
	m_fMovingSize = 300.f;

	CString sValue;
	sValue.Format(_T("The point of object is (%d, %d, %d). \n"), m_vObjPoint.x, m_vObjPoint.y, m_vObjPoint.z);
	m_eObjectPoint.SetWindowTextW(sValue);

	UpdateData(FALSE);

	return TRUE;
}
#pragma endregion 

#pragma region LoadButton
void CTap02::OnBnClickedButtonCubeload()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CTap02::OnBnClickedButtonMonsterload()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CTap02::OnBnClickedButtonEtcload()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
#pragma endregion 


#pragma region Others
void CTap02::OnBnClickedButtonInstall()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_vObjPoint += {};
}


void CTap02::OnBnClickedButtonSaveall()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
#pragma endregion 


void CTap02::OnEnChangeEditObjectpoint()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialog::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//CString sValue;
	//sValue.Format(_T("(%d, %d, %d) \n"), m_vObjPoint.x, m_vObjPoint.y, m_vObjPoint.z);
	//m_eObjectPoint.SetWindowTextW(sValue);
}

void CTap02::Key_State()
{
	if (GetAsyncKeyState('I') & 0x8000)		//Up +y
	{
		m_vObjPoint.y += m_fMovingSize;
	}
	if (GetAsyncKeyState('K') & 0x8000)		//Down -y
	{
		m_vObjPoint.y -= m_fMovingSize;
	}
	if (GetAsyncKeyState('L') & 0x8000)		//Right +x
	{
		m_vObjPoint.x += m_fMovingSize;
	}
	if (GetAsyncKeyState('J') & 0x8000)		//Left -x
	{
		m_vObjPoint.x -= m_fMovingSize;
	}
	if (GetAsyncKeyState('O') & 0x8000)		//Out +z
	{
		m_vObjPoint.z += m_fMovingSize;
	}
	if (GetAsyncKeyState('U') & 0x8000)		//In -z
	{
		m_vObjPoint.z -= m_fMovingSize;
	}

	if (m_vObjPoint != m_vPreObjPoint)
	{
		UpdateData(TRUE);

		CString sValue;
		sValue.Format(_T("The point of object is (%d, %d, %d). \n"), m_vObjPoint.x, m_vObjPoint.y, m_vObjPoint.z);
		m_eObjectPoint.SetWindowTextW(sValue);
	}

}
