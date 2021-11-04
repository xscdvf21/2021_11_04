// Tap03.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFCTool.h"
#include "Tap03.h"
#include "afxdialogex.h"

#include "MFCToolView.h"
extern CMFCToolView* g_pView;

#include "GameObj.h"
#include "RandomMgr.h"

// CTap03 대화 상자입니다.
#pragma region 생성자

IMPLEMENT_DYNAMIC(CTap03, CDialog)

CTap03::CTap03(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_TAP03, pParent)
	, m_strObjNameEC(_T(""))
	, m_iObjNumED(0)
	, m_iRedED(0), m_iGreenED(0), m_iBlueED(0)
	, m_fm11ED(0), m_fm12ED(0), m_fm13ED(0)
	, m_fm21ED(0), m_fm22ED(0), m_fm23ED(0)
	, m_fm31ED(0), m_fm32ED(0), m_fm33ED(0)
	, m_fm41ED(0), m_fm42ED(0), m_fm43ED(0)
	, m_fRotMinED(0), m_fRotMaxED(0)

	, m_fGravityMinED(0), m_fGravityMaxED(0)
	, m_fLifeTimeMinED(0), m_fLifeTimeMaxED(0), m_fLifeTimeDelayED(0)

	, m_fAccelXMinED(0), m_fAccelYMinED(0), m_fAccelZMinED(0)
	, m_fAccelXMaxED(0), m_fAccelYMaxED(0), m_fAccelZMaxED(0)
	, m_fScaleXStartED(0), m_fScaleYStartED(0), m_fScaleZStartED(0)
	, m_fScaleXEndED(0), m_fScaleYEndED(0), m_fScaleZEndED(0)
{
	D3DXMatrixIdentity(&m_matWorld);
	ZeroMemory(&m_vShotting, sizeof(D3DXVECTOR3));
}

CTap03::~CTap03()
{
}
#pragma endregion 

#pragma region MFC 다이얼로그 
void CTap03::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_OBJECT_ADD, m_btctlAdd_Obj);
	DDX_Control(pDX, IDC_BTN_OBJECT_DELETE, m_btctl_Del_Obj);
	DDX_Text(pDX, IDC_ED_OBJECTNAME, m_strObjNameEC);
	DDV_MaxChars(pDX, m_strObjNameEC, 256);
	DDX_Control(pDX, IDC_LIST_OBJECTPLATE, m_lbcObjPlate);
	DDX_Text(pDX, IDC_ED_OBJECTNUM, m_iObjNumED);

	DDX_Control(pDX, IDC_CHECK_COLORBEGIN, m_cbBeginCol);//컬러 시작 체크
	DDX_Control(pDX, IDC_CHECK_COLOREND, m_cbEndCol);//컬러 끝 체크
	DDX_Control(pDX, IDC_SLIDER_RED, m_ctlRedSLIDER);
	DDX_Control(pDX, IDC_SLIDER_GREEN, m_ctlGreenSLIDER);
	DDX_Control(pDX, IDC_SLIDER_BLUE, m_ctlBlueSLIDER);
	DDX_Text(pDX, IDC_ED_REDVALUE, m_iRedED);
	DDX_Text(pDX, IDC_ED_GREENVALUE, m_iGreenED);
	DDX_Text(pDX, IDC_ED_BLUEVALUE, m_iBlueED);
	DDX_Text(pDX, IDC_ED_m11, m_fm11ED);
	DDX_Text(pDX, IDC_ED_m12, m_fm12ED);
	DDX_Text(pDX, IDC_ED_m13, m_fm13ED);
	DDX_Text(pDX, IDC_ED_m21, m_fm21ED);
	DDX_Text(pDX, IDC_ED_m22, m_fm22ED);
	DDX_Text(pDX, IDC_ED_m23, m_fm23ED);
	DDX_Text(pDX, IDC_ED_m31, m_fm31ED);
	DDX_Text(pDX, IDC_ED_m32, m_fm32ED);
	DDX_Text(pDX, IDC_ED_m33, m_fm33ED);
	DDX_Text(pDX, IDC_ED_m41, m_fm41ED);
	DDX_Text(pDX, IDC_ED_m42, m_fm42ED);
	DDX_Text(pDX, IDC_ED_m43, m_fm43ED);

	DDX_Control(pDX, IDC_CHECK3, m_cbRotX); // 로테이션 체크
	DDX_Control(pDX, IDC_CHECK4, m_cbRotY);
	DDX_Control(pDX, IDC_CHECK5, m_cbRotZ);

	DDX_Text(pDX, IDC_ED_RADIUSMIN, m_fRotMinED);
	DDX_Text(pDX, IDC_ED_RADIUSMAX, m_fRotMaxED);

	DDX_Text(pDX, IDC_ED_GRAVITYMIN, m_fGravityMinED);
	DDX_Text(pDX, IDC_ED_GRAVITYMAX, m_fGravityMaxED);
	DDX_Control(pDX, IDC_CHEK_APPLY_GRAVITY, m_cbGravityCheck);
	DDX_Text(pDX, IDC_ED_LIFETIMEMIN, m_fLifeTimeMinED);
	DDX_Text(pDX, IDC_ED_LIFETIMEMAX, m_fLifeTimeMaxED);
	DDX_Text(pDX, IDC_ED_LIFETIMEDELAY, m_fLifeTimeDelayED);

	DDX_Text(pDX, IDC_ED_MinX, m_fAccelXMinED);
	DDX_Text(pDX, IDC_ED_MaxX, m_fAccelXMaxED);
	DDX_Text(pDX, IDC_EDe_, m_fScaleXStartED);
	DDX_Text(pDX, IDC_EDe_16, m_fScaleYStartED);
	DDX_Text(pDX, IDC_EDe_17, m_fScaleZStartED);
	DDX_Text(pDX, IDC_EDe_18, m_fScaleXEndED);
	DDX_Text(pDX, IDC_EDe_19, m_fScaleYEndED);
	DDX_Text(pDX, IDC_EDe_20, m_fScaleZEndED);
	DDX_Control(pDX, IDC_CHECK_MULTY, m_cbMultiColorCheck);
	
	DDX_Control(pDX, IDC_RADIO_SINGLE, m_iTotalNumRD[0]);
	DDX_Control(pDX, IDC_RADIO_LESS, m_iTotalNumRD[1]);
	DDX_Control(pDX, IDC_RADIO_MANY, m_iTotalNumRD[2]);

	//m_iConstOrRand
	DDX_Control(pDX, IDC_RADIO_CONST, m_iConstOrRand[0]);
	DDX_Control(pDX, IDC_RADIO_RANDOM, m_iConstOrRand[1]);

	//m_iType
	DDX_Control(pDX, IDC_RADIO_TYPE1, m_iType[0]);
	DDX_Control(pDX, IDC_RADIO_TYPE2, m_iType[1]);
	DDX_Control(pDX, IDC_RADIO_TYPE3, m_iType[2]);

}

BEGIN_MESSAGE_MAP(CTap03, CDialog)
	ON_BN_CLICKED(IDC_BTN_OBJECT_ADD, &CTap03::OnBnClicked_AddObject)
	ON_BN_CLICKED(IDC_BTN_OBJECT_DELETE, &CTap03::OnBnClicked_DeleteObject)
	ON_BN_CLICKED(IDC_BTN_CLEAR, &CTap03::OnBnClicked_Clear)

	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_RED, &CTap03::Slider_Red)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_GREEN, &CTap03::Slider_Green)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_BLUE, &CTap03::Slider_Blue)
	ON_BN_CLICKED(IDC_BTN_APPLY, &CTap03::OnBnClicked_Apply)
	ON_BN_CLICKED(IDC_CHECK_COLORBEGIN, &CTap03::CheckBox_BeginCol)
	ON_BN_CLICKED(IDC_CHECK_COLOREND, &CTap03::CheckBox_EndCol)
	ON_BN_CLICKED(IDC_BTN_Play, &CTap03::OnBnClickedBtnPlay)
	ON_LBN_SELCHANGE(IDC_LIST_OBJECTPLATE, &CTap03::OnLbnSelchangeListObjectplate)
	ON_BN_CLICKED(IDC_BTN_SAVE, &CTap03::OnBnClicked_Save)
END_MESSAGE_MAP()
#pragma endregion

#pragma region 탭 초기 셋팅.
BOOL CTap03::OnInitDialog()
{
	CDialog::OnInitDialog();

	UpdateData(TRUE);

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

	//Color 셋팅
	m_ctlRedSLIDER.SetRange(0, 255);
	m_ctlRedSLIDER.SetPos(127);
	m_iRedED = m_ctlRedSLIDER.GetPos();

	m_ctlGreenSLIDER.SetRange(0, 255);
	m_ctlGreenSLIDER.SetPos(127);
	m_iGreenED = m_ctlRedSLIDER.GetPos();

	m_ctlBlueSLIDER.SetRange(0, 255);
	m_ctlBlueSLIDER.SetPos(127);
	m_iBlueED = m_ctlRedSLIDER.GetPos();


	////////forSample
	m_fLifeTimeMinED = 100.f;
	m_fLifeTimeMaxED = 200.f;
	m_fScaleXStartED = 1.f;
	m_fScaleYStartED = 1.f;
	m_fScaleZStartED = 1.f;
	m_fScaleXEndED = 0.1f;
	m_fScaleYEndED = 0.1f;
	m_fScaleZEndED = 0.1f;
	m_fGravityMinED = 7.f;
	m_fGravityMaxED = 12.f;
	m_fRotMinED = 8.f;
	m_fRotMinED = 15.f;
	m_fAccelXMinED = 2.f;
	m_fAccelXMaxED = 2.f;
	m_iTotalNumRD[0].SetCheck(BST_CHECKED);
	m_iConstOrRand[0].SetCheck(BST_CHECKED);
	m_iType[0].SetCheck(BST_CHECKED);
	//////////////////////////////////

	
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
#pragma endregion 


#pragma region Apply All Setting
void CTap03::OnBnClicked_Apply()
{
	

	SGameObject* pGameObject = new SGameObject();

	pGameObject->m_fScaleXStart = m_fScaleXStartED;
	pGameObject->m_fScaleYStart = m_fScaleYStartED;
	pGameObject->m_fScaleZStart = m_fScaleZStartED;

	pGameObject->m_vColorStart = D3DXVECTOR3((float)m_iRed_Begin, (float)m_iGreen_Begin, (float)m_iBlue_Begin);
	pGameObject->m_vColorEnd = D3DXVECTOR3((float)m_iRed_End, (float)m_iGreen_End, (float)m_iBlue_End);



}
void CTap03::Showing_Cube(D3DXVECTOR3 _vShotting, float _RandomKey)
{
	m_pColorCube = dynamic_cast<CCubeColor*>(g_pView->m_pMainTool->Clone_CubeColor());
	
	CString strtemp;
	strtemp.Format(L"%d", m_iNumber);
	++m_iNumber;
	CString strName = L"큐브";

	m_lbcObjPlate.AddString(strName + strtemp);
	m_listObj.push_back(m_pColorCube);

	//변수에 데이터 업데이트.
	UpdateData(TRUE);

	float LifeMin = m_fLifeTimeMinED;
	float LifeMax = m_fLifeTimeMaxED;

	D3DXVECTOR3	ScaleStart = { m_fScaleXStartED, m_fScaleYStartED, m_fScaleZStartED };
	D3DXVECTOR3 ScaleEnd = { m_fScaleXEndED, m_fScaleYEndED, m_fScaleZEndED };
	
	bool RotX = m_cbRotX.GetCheck();
	bool RotY = m_cbRotY.GetCheck();
	bool RotZ = m_cbRotZ.GetCheck();

	float GravMax = m_fGravityMaxED;
	float GravMin = m_fGravityMinED;

	float AccelMin = m_fAccelXMinED;
	float AccelMax = m_fAccelXMaxED;

	float RotMin = m_fRotMinED;
	float RotMax = m_fRotMaxED;

	bool bOnlyChild = m_iTotalNumRD[0].GetCheck();
	bool bNaturally = m_iConstOrRand[1].GetCheck();

	UpdateData(FALSE);


	//낱개의 큐브와 소통.
	m_pColorCube->Set_LifeTime(Random(LifeMin, LifeMax, _RandomKey, 0.5f));

	if (bNaturally)
	{
		ScaleStart.x = Random(ScaleStart.x * 0.4f, ScaleStart.x * 1.2f, _RandomKey, 0.9f);
		ScaleStart.y = Random(ScaleStart.y * 0.4f, ScaleStart.x * 1.2f, _RandomKey, 0.9f);
		ScaleStart.z = Random(ScaleStart.z * 0.4f, ScaleStart.x * 1.2f, _RandomKey, 0.9f);

		ScaleEnd.x = Random(ScaleEnd.x * 0.4f, ScaleEnd.x * 1.2f, _RandomKey, 0.9f);
		ScaleEnd.y = Random(ScaleEnd.y * 0.4f, ScaleEnd.x * 1.2f, _RandomKey, 0.9f);
		ScaleEnd.z = Random(ScaleEnd.z * 0.4f, ScaleEnd.x * 1.2f, _RandomKey, 0.9f);

		m_pColorCube->Set_Scale(ScaleStart * 0.3f, ScaleEnd * 0.3f);
	}
	else
	{
		m_pColorCube->Set_Scale(ScaleStart * 0.3f, ScaleEnd * 0.3f);
	}

	if (m_cbMultiColorCheck.GetCheck())
	{
		D3DXVECTOR3 ColorStart = { };
		ColorStart.x = Random(0.f, 255.f, _RandomKey, 0.3f);
		ColorStart.y = Random(0.f, 255.f, _RandomKey, 0.6f);
		ColorStart.z = Random(0.f, 255.f, _RandomKey, 0.9f);
		D3DXVECTOR3 ColorEnd = { ColorStart.x + 200.f, ColorStart.y + 255.f, ColorStart.z + 235.f };

		m_pColorCube->Set_Color(ColorStart, ColorEnd, true);
	}
	else
	{
		UpdateData(TRUE);
		D3DXVECTOR3 ColorStart = { (float)m_iRed_Begin, (float)m_iGreen_Begin, (float)m_iBlue_Begin };
		D3DXVECTOR3 ColorEnd = { (float)m_iRed_End, (float)m_iGreen_End, (float)m_iBlue_End };
		UpdateData(FALSE);

		//if (bNaturally)
		//{
		//	ColorStart.x = Random(ColorStart.x - 20.f, ColorStart.x + 20.f, _RandomKey, 0.5f);
		//	ColorStart.y = Random(ColorStart.y - 20.f, ColorStart.y + 20.f, _RandomKey, 0.5f);
		//	ColorStart.z = Random(ColorStart.z - 20.f, ColorStart.z + 20.f, _RandomKey, 0.5f);

		//	ColorEnd.x = Random(ColorEnd.x - 20.f, ColorEnd.x + 20.f, _RandomKey, 0.5f);
		//	ColorEnd.y = Random(ColorEnd.y - 20.f, ColorEnd.y + 20.f, _RandomKey, 0.5f);
		//	ColorEnd.z = Random(ColorEnd.z - 20.f, ColorEnd.z + 20.f, _RandomKey, 0.5f);
		//}


		m_pColorCube->Set_Color(ColorStart, ColorEnd, false);
	}

	if (m_cbGravityCheck.GetCheck())
	{	
		if (bOnlyChild)
		{
			m_fGravPower = (GravMin + GravMax) * 0.5f;
			m_pColorCube->Set_Jump(true, m_fGravPower, {0.f, 0.f, 0.f});
		}
		else
		{
			m_fGravPower = Random(GravMin, GravMax, _RandomKey, 0.8f);	//10의 값이 안정적.
			m_pColorCube->Set_Jump(true, m_fGravPower, _vShotting);
		}
	}
	else
	{
		if (bOnlyChild)
		{
			m_fGravPower = (GravMin + GravMax) * 0.5f;
			m_pColorCube->Set_Jump(false, m_fGravPower, { 0.f, 0.f, 0.f });
		}
		else
		{
			m_fGravPower = Random(GravMin, GravMax, _RandomKey, 0.8f);	//10의 값이 안정적.
			D3DXVECTOR3 vShotting = _vShotting;
			vShotting.y += _RandomKey;
			m_pColorCube->Set_Jump(false, m_fGravPower, vShotting);
		}
	}

	m_fRotation = Random(RotMin, RotMax, _RandomKey, 0.5f);
	m_pColorCube->Set_Rotation(RotX, RotY, RotZ, m_fRotation);

	UpdateData(TRUE);
	if (m_iType[1].GetCheck())
	{
		D3DXVECTOR3 Pos = {};
		Pos.x = Random(-2.f, 2.f, _RandomKey, 0.5f);
		Pos.y = 10.f;
		Pos.z = Random(0.f, 4.f, m_fRandom, 0.5f);

		m_pColorCube->Set_Position(Pos);

		m_pColorCube->Set_Jump(false, m_fGravPower, { 0.f, -1.f, 0.f });
	}
	else if (m_iType[2].GetCheck())
	{
		D3DXVECTOR3 Pos = {};
		Pos.x = Random(-2.f, 2.f, _RandomKey, 0.5f);
		Pos.y = 0.f;
		Pos.z = Random(0.f, 4.f, m_fRandom, 0.5f);

		m_pColorCube->Set_Position(Pos);

		m_pColorCube->Set_Jump(false, m_fGravPower, { 0.f, 1.f, 0.f });
	}
	UpdateData(FALSE);


	float Accel = Random(AccelMin, AccelMax, _RandomKey, 0.3f);
	m_pColorCube->Set_Accel(Accel);

	D3DXVECTOR3 LifeTime = {};

	m_pColorCube->Set_Start();			// 이제 시작.
}
#pragma endregion


// 리스트 박스에 오브젝트 추가.
void CTap03::OnBnClicked_AddObject()
{
	
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_strObjNameEC;
	UpdateData(FALSE);

	if (0 < m_iObjNumED)
	{
		for (int i = 0; i < m_iObjNumED; ++i)
		{
			CString strTemp;
			strTemp.Format(_T("%d"), i);

			m_lbcObjPlate.AddString(m_strObjNameEC + strTemp);
		}
	}
	else
	{
		m_lbcObjPlate.AddString(m_strObjNameEC);
	}

	m_ctlRedSLIDER.SetRange(0, 255);
	m_ctlRedSLIDER.SetPos(0);

	m_ctlGreenSLIDER.SetRange(0, 255);
	m_ctlGreenSLIDER.SetPos(0);

	m_ctlBlueSLIDER.SetRange(0, 255);
	m_ctlBlueSLIDER.SetPos(0);

}


void CTap03::OnBnClicked_DeleteObject()// 리스트 박스에서 선택적으로 삭제
{
}


void CTap03::OnBnClicked_Clear()// 리스트박스 청소
{
	m_lbcObjPlate.ResetContent();
}

#pragma region ColorPart

void CTap03::CheckBox_BeginCol()// 시작 체크.
{
	if (m_cbEndCol.GetCheck())
	{
		m_cbEndCol.SetCheck(FALSE);
	}


	m_ctlRedSLIDER.SetPos(m_iRed_Begin);
	m_ctlGreenSLIDER.SetPos(m_iGreen_Begin);
	m_ctlBlueSLIDER.SetPos(m_iBlue_Begin);

}


void CTap03::CheckBox_EndCol()//끝 체크
{
	if (TRUE == m_cbBeginCol.GetCheck())
	{
		m_cbBeginCol.SetCheck(FALSE);
	}
	
	m_ctlRedSLIDER.SetPos(m_iRed_End);
	m_ctlGreenSLIDER.SetPos(m_iGreen_End);
	m_ctlBlueSLIDER.SetPos(m_iBlue_End);


}

void CTap03::Slider_Red(NMHDR *pNMHDR, LRESULT *pResult)//Red 슬리이더
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

	if (TRUE == m_cbBeginCol.GetCheck())
	{
		m_iRed_Begin = m_ctlRedSLIDER.GetPos();
	}
	else if (TRUE == m_cbEndCol.GetCheck())
	{
		m_iRed_End = m_ctlRedSLIDER.GetPos();
	}

	UpdateData(TRUE);
	m_iRedED = m_ctlRedSLIDER.GetPos();
	UpdateData(FALSE);

	*pResult = 0;
}


void CTap03::Slider_Green(NMHDR *pNMHDR, LRESULT *pResult)//Green 슬라이더
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

	if (TRUE == m_cbBeginCol.GetCheck())
	{
		m_iGreen_Begin = m_ctlGreenSLIDER.GetPos();
	}
	else if (TRUE == m_cbEndCol.GetCheck())
	{
		m_iGreen_End = m_ctlGreenSLIDER.GetPos();
	}

	UpdateData(TRUE);
	m_iGreenED = m_ctlGreenSLIDER.GetPos();
	UpdateData(FALSE);

	*pResult = 0;
}


void CTap03::Slider_Blue(NMHDR *pNMHDR, LRESULT *pResult)//Blue 슬라이더
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

	if (TRUE == m_cbBeginCol.GetCheck())
	{
		m_iBlue_Begin = m_ctlBlueSLIDER.GetPos();
	}
	else if (TRUE == m_cbEndCol.GetCheck())
	{
		m_iBlue_End = m_ctlBlueSLIDER.GetPos();
	}

	UpdateData(TRUE);
	m_iBlueED = m_ctlBlueSLIDER.GetPos();
	UpdateData(FALSE);

	*pResult = 0;
}
#pragma endregion 

void CTap03::OnBnClickedBtnPlay()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	mt19937 engine((unsigned int)time(NULL));
	uniform_real_distribution<float> distrtibution(-1, 1);
	auto generator = bind(distrtibution, engine);

	srand(GetTickCount());

	m_vShotting = { 1.f, 0.f, 0.f };
	float m_fAngle = 0.f;

	UpdateData(TRUE);
	if (m_iTotalNumRD[0].GetCheck())
	{
		UpdateData(FALSE);
		Showing_Cube({ 0.f, 0.f, 0.f }, 0.f);
	}
	else if (m_iTotalNumRD[1].GetCheck())
	{
		UpdateData(FALSE);
		for (int i = 0; i < 7; ++i)
		{
			float RandomKey = generator();
			m_fRandom = generator();//
			Showing_Cube(m_vShotting, RandomKey);
			m_vShotting.x = sin(m_fAngle);
			m_vShotting.z = cos(m_fAngle);
			m_fAngle += 21.f;
		}
	}
	else if (m_iTotalNumRD[2].GetCheck())
	{
		UpdateData(FALSE);
		for (int i = 0; i < 20; ++i)
		{
			float RandomKey = generator();
			m_fRandom = generator();//
			Showing_Cube(m_vShotting, RandomKey);

			m_vShotting.x = sin(m_fAngle);
			m_vShotting.z = cos(m_fAngle);
			m_fAngle += 27.f;
		}
	}
	else
	{
		UpdateData(FALSE);
		Showing_Cube({ 0.f, 0.f, 0.f }, 0.f);
	}


}

void CTap03::OnLbnSelchangeListObjectplate()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int iIndex = 0;
	iIndex = m_lbcObjPlate.GetCurSel();

	auto& iter = m_listObj.begin();
	for (int i = 0; i < iIndex; ++i)
	{
		++iter;
	}

	_float3 vPos = dynamic_cast<CCubeColor*>((*iter))->m_pTransformCom->Get_Position();

	UpdateData(TRUE);
	//m_fPosXXED = vPos.x;
	//m_fPosYYED = vPos.y;
	//m_fPosZZED = vPos.z;
	UpdateData(FALSE);

//	dynamic_cast<CCubeLine*>(m_lbcObjPlate)->m_pTransformCom->Set_Position(vPos);

}

float CTap03::Random(float _MinVal, float _MaxVal, float _Df, float _Per)
{
	CRandomMgr*		Col = new CRandomMgr;

	return Col->Get_Rand(_MinVal, _MaxVal, _Df, _Per);
}

float CTap03::Random(float _Val, float _Df)
{
	CRandomMgr*		Col = new CRandomMgr;

	return Col->Get_Rand(_Val, _Df);
}

void CTap03::OnBnClicked_Save()
{

	CFileDialog Dlg(FALSE, L"dat", L"*.dat", OFN_OVERWRITEPROMPT, L"Data File(*.dat)|*.dat||", this);

	TCHAR szBuf[MAX_PATH] = L"";
	GetCurrentDirectory(MAX_PATH, szBuf);

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
			PRINT_LOG(L"err", L"세이브");
			return;
		}

		DWORD dwByte = 0;
		DWORD dwStrByte = 0;


		for (auto& iter : m_listObj)
		{
			CCubeColor* pObj = dynamic_cast<CCubeColor*>(iter);

			WriteFile(hFile, &pObj->m_vSaveScaleStart, sizeof(D3DXVECTOR3), &dwByte, nullptr);
			WriteFile(hFile, &pObj->m_vSaveScaleEnd, sizeof(D3DXVECTOR3), &dwByte, nullptr);
			WriteFile(hFile, &pObj->m_vSaveColorStart, sizeof(D3DXVECTOR3), &dwByte, nullptr);
			WriteFile(hFile, &pObj->m_vSaveColorEnd, sizeof(D3DXVECTOR3), &dwByte, nullptr);
			WriteFile(hFile, &pObj->m_vSaveShotting, sizeof(D3DXVECTOR3), &dwByte, nullptr);

			WriteFile(hFile, &pObj->e_SaveAxis, sizeof(int), &dwByte, nullptr);

			WriteFile(hFile, &pObj->m_bSaveJump, sizeof(bool), &dwByte, nullptr);
			WriteFile(hFile, &pObj->m_bSaveMultiColor, sizeof(bool), &dwByte, nullptr);
			WriteFile(hFile, &pObj->m_fSaveRotation, sizeof(float), &dwByte, nullptr);

			WriteFile(hFile, &pObj->m_fSaveJumpPowerY, sizeof(float), &dwByte, nullptr);
			WriteFile(hFile, &pObj->m_fSaveLifeTime, sizeof(float), &dwByte, nullptr);
			WriteFile(hFile, &pObj->m_fSaveAccel, sizeof(float), &dwByte, nullptr);

		}
		CloseHandle(hFile);
	}

}
