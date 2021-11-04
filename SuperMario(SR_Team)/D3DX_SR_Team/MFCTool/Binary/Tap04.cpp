// Tap04.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFCTool.h"
#include "Tap04.h"
#include "afxdialogex.h"
#include "CubeColor.h"
#include "CubeLine.h"
#include "SliderColor.h"
#include "BoxColor.h"


// CTap04 대화 상자입니다.
IMPLEMENT_DYNAMIC(CTap04, CDialog)

#pragma region 생성자.
CTap04::CTap04(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_Tap04, pParent)
	
	, m_iCountED(0)
	, m_ivXED(0)
	, m_ivYED(0)
	, m_ivZED(0)

	, m_strNameED(_T(""))
	, m_fScaleXED(0)
	, m_fScaleYED(0)
	, m_fScaleZED(0)
	, m_fPosXXED(0)
	, m_fPosYYED(0)
	, m_fPosZZED(0)
{

}

CTap04::~CTap04()
{
}

void CTap04::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);


	DDX_Text(pDX, IDC_EDCOUNT, m_iCountED);
	DDX_Text(pDX, IDC_EDVX, m_ivXED);
	DDX_Text(pDX, IDC_EDVY, m_ivYED);
	DDX_Text(pDX, IDC_EDVZ, m_ivZED);

	DDX_Control(pDX, IDC_CHECK_SNAP, m_ctlSnapCHECK);

	DDX_Control(pDX, IDC_CHECK_CHAIN, m_ctlChainCHECK);
	DDX_Control(pDX, IDC_LIST_OBJECT, m_ctlObjectLIST);
	DDX_Control(pDX, IDC_LIST_TEXTURE, m_ctlTextureLIST);
	DDX_Control(pDX, IDC_CHECK_AllSAVE, m_ctlAllSaveCB);
	//DDX_Control(pDX, IDC_CHECK_SELECTSAVE, m_ctlSelectSaveCB);
	DDX_Text(pDX, IDC_EDNAME, m_strNameED);
	DDX_Text(pDX, IDC_EDSCALEX, m_fScaleXED);
	DDX_Text(pDX, IDC_EDSCALEY, m_fScaleYED);
	DDX_Text(pDX, IDC_EDSCALEZ, m_fScaleZED);
	DDX_Control(pDX, IDC_hanCHECK_Red, m_ctlRedCB);
	DDX_Control(pDX, IDC_hanCHECK_Green, m_ctlGreenCB);
	DDX_Control(pDX, IDC_hanCHECK_Blue, m_ctlBlueCB);
	DDX_Control(pDX, IDC_hanCHECK_CubeColor, m_ctlCubeColorCB);
	DDX_Control(pDX, IDC_hanCHECK_CubeTexture, m_ctlCubeTextureCB);

	DDX_Control(pDX, IDC_hanCHECK_SliderColor, m_ctlSliderColorCB);
	DDX_Control(pDX, IDC_hanCHECK_SliderTexture, m_ctlSliderTextureCB);
	DDX_Control(pDX, IDC_hanCHECK_Etc, m_ctlEtcCB);
	DDX_Text(pDX, IDC_EDPOSX, m_fPosXXED);
	DDX_Text(pDX, IDC_EDPOSY, m_fPosYYED);
	DDX_Text(pDX, IDC_EDPOSZ, m_fPosZZED);
	DDX_Control(pDX, IDC_hanCHECK_SET_CHECKER, m_ctlSetCheckerCB);
	DDX_Control(pDX, IDC_StaticLoad, m_ctlStaticLoadCB);
	DDX_Control(pDX, IDC_Erase_Han, m_ctlEraseCB);
}
#pragma endregion

#pragma region 다이럴로그.
BEGIN_MESSAGE_MAP(CTap04, CDialog)


	ON_BN_CLICKED(IDC_BT_SET, &CTap04::OnBnClicked_Set)
	ON_BN_CLICKED(IDC_BT_DELETE, &CTap04::OnBnClicked_Delete)
	ON_BN_CLICKED(IDC_BT_CLEAR, &CTap04::OnBnClicked_AllClear)
	ON_BN_CLICKED(IDC_BT_SAVE, &CTap04::OnBnClicked_Save)
	ON_BN_CLICKED(IDC_BT_LOAD, &CTap04::OnBnClicked_Load)
	ON_LBN_SELCHANGE(IDC_LIST_OBJECT, &CTap04::OnLIST_Object)
	ON_LBN_SELCHANGE(IDC_LIST_TEXTURE, &CTap04::OnLIST_Texture)
	ON_BN_CLICKED(IDC_BT_ROTX, &CTap04::OnBnClicked_RotX)
	ON_BN_CLICKED(IDC_BT_ROTY, &CTap04::OnBnClicked_RotY)
	ON_BN_CLICKED(IDC_BT_ROTZ, &CTap04::OnBnClicked_RotZ)
	ON_BN_CLICKED(IDC_BT_INVERSE, &CTap04::OnBnClicked_MatrixInverse)
	ON_BN_CLICKED(IDC_CHECK_AllSAVE, &CTap04::CheckBox_AllSave)
	//ON_BN_CLICKED(IDC_CHECK_SELECTSAVE, &CTap04::CheckBox_SelectSave)
	ON_BN_CLICKED(IDC_hanCHECK_CubeColor, &CTap04::CheckBox_CubeColor)
	ON_BN_CLICKED(IDC_hanCHECK_CubeTexture, &CTap04::CheckBox_CubeTexture)
	ON_BN_CLICKED(IDC_hanCHECK_SliderColor, &CTap04::CheckBox_SliderColor)
	ON_BN_CLICKED(IDC_hanCHECK_SliderTexture, &CTap04::CheckBox_SliderTexture)
	ON_BN_CLICKED(IDC_hanCHECK_Etc, &CTap04::CheckBox_Etc)
	ON_BN_CLICKED(IDC_hanBT_DownXSet, &CTap04::OnBnClicked_DownXSet)
	ON_BN_CLICKED(IDC_hanBT_UpXSet, &CTap04::OnBnClicked_UpXSet)
	ON_BN_CLICKED(IDC_hanBT_UpZSet, &CTap04::OnBnClicked_UpZSet)
	ON_BN_CLICKED(IDC_hanBT_DownZSet, &CTap04::OnBnClicked_DownZSet)
	ON_BN_CLICKED(IDC_hanBT_UpYSet, &CTap04::OnBnClicked_UpYSet)
	ON_BN_CLICKED(IDC_hanBT_DownYSet, &CTap04::OnBnClicked_DownYSet)
	ON_BN_CLICKED(IDC_BT_Fix, &CTap04::OnBnClicked_Fix)
END_MESSAGE_MAP()
#pragma endregion


#pragma region 다이얼로그 초기 셋팅.
BOOL CTap04::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_pMainTool = g_pView->m_pMainTool;
	
	UpdateData(TRUE);

	m_fScaleXED = 1.f;
	m_fScaleYED = 1.f;
	m_fScaleZED = 1.f;

	m_fPosXXED;
	m_fPosYYED;
	m_fPosZZED;

	m_ctlCubeColorCB.SetCheck(FALSE);
	m_ctlCubeTextureCB.SetCheck(TRUE);
	m_ctlSliderColorCB.SetCheck(FALSE);
	m_ctlSliderTextureCB.SetCheck(FALSE);
	m_ctlEtcCB.SetCheck(FALSE);


	//texture 리스트
	int i = 0;
	/*CString strTemp, strName = L"Terrain";

	for (int i = 0; i < m_pMainTool->m_iTerrainTextureCount; ++i)
	{
		strTemp.Format(_T("%d"), i);
		
	}
	*/
	//텍스쳐 이름.
	m_ctlTextureLIST.AddString(L"0. 퍼플");
	m_ctlTextureLIST.AddString(L"1. 스카이 블루");
	m_ctlTextureLIST.AddString(L"2. 다크 브라운");
	m_ctlTextureLIST.AddString(L"3. 브라이트 그린");
	m_ctlTextureLIST.AddString(L"4. 파스텔 블루");
	m_ctlTextureLIST.AddString(L"5. 브라이트 그레이");
	m_ctlTextureLIST.AddString(L"6. 브라이트 그린");
	m_ctlTextureLIST.AddString(L"7. 마리오 블럭.");
	m_ctlTextureLIST.AddString(L"8. 브라이트 퍼플");
	m_ctlTextureLIST.AddString(L"9. 오렌지");
	m_ctlTextureLIST.AddString(L"10. 파스텔 핑크");
	m_ctlTextureLIST.AddString(L"11. 딥 퍼플");
	m_ctlTextureLIST.AddString(L"12. 레드");
	m_ctlTextureLIST.AddString(L"13. 화이트");
	m_ctlTextureLIST.AddString(L"14. 옐로");


	m_ctlTextureLIST.SetCurSel(0);

	UpdateData(FALSE);

	m_ctlAllSaveCB.SetCheck(true);

	return TRUE;
}
#pragma endregion


void CTap04::OnBnClicked_Set()
{
#pragma region UpdateData
	UpdateData(TRUE);
	//위치 업뎃.


	m_fScaleXED;
	m_fScaleYED;
	m_fScaleZED;

	m_strNameED;

	m_iCountED;

	m_ctlCubeColorCB;
	m_ctlCubeTextureCB;
	m_ctlSliderColorCB;
	m_ctlSliderTextureCB;
	m_ctlEtcCB;

	UpdateData(FALSE);
#pragma endregion

#pragma region 예외처리.

	if (nullptr == m_pLineObj)
	{
		m_pLineObj = m_pMainTool->Clone_CubeLine();
	}

	if (false == m_ctlCubeColorCB.GetCheck()
		&& false == m_ctlCubeTextureCB.GetCheck()
		&& false == m_ctlSliderColorCB.GetCheck()
		&& false == m_ctlSliderTextureCB.GetCheck()
		&& false == m_ctlEtcCB.GetCheck())
	{
		return;
	}

	if (0 == m_strNameED.GetLength())//이름이 공백이면 return
	{
		return;
	}

	if (0.f == m_fScaleXED && 0.f == m_fScaleYED && 0.f == m_fScaleZED)
	{
		return;
	}

	_bool bCheck = false;
	if (0 == m_ivXED && 0 == m_ivYED && 0 == m_ivZED)//Chain 활성 조건.
	{
		bCheck = true; // Chain을 활성 시키지 못하게 함.
	}

#pragma endregion

#pragma region 사전 처리

	UpdateData(TRUE);
	int iSwitch = 0;
	if (TRUE == m_ctlCubeColorCB.GetCheck())
	{
		iSwitch = 0;
	}
	else if (TRUE == m_ctlCubeTextureCB.GetCheck())
	{
		iSwitch = 1;
	}
	else if (TRUE == m_ctlSliderColorCB.GetCheck())
	{
		iSwitch = 2;
	}

	else if (TRUE == m_ctlSliderTextureCB.GetCheck())
	{
		iSwitch = 3;
	}

	else if (TRUE == m_ctlEtcCB.GetCheck())
	{
		iSwitch = 4;
	}
	UpdateData(FALSE);
#pragma endregion 

	//오브젝트 추가 관련 코드.
	if (0 < m_iCountED && true == m_ctlChainCHECK.GetCheck() && false == bCheck)
	{
		for (int i = 0; i < m_iCountED; ++i)
		{
#pragma region 리스트 링크
			//https://m.blog.naver.com/PostView.nhn?blogId=lwj798&logNo=220628724244&proxyReferer=https:%2F%2Fwww.google.com%2F
#pragma endregion
			CString strTemp;
			strTemp.Format(_T("%d"), i);

			m_ctlObjectLIST.AddString(m_strNameED + L"_" + strTemp);


			_float4x4 matWorld;
			CGameObj* pObj = nullptr;

			switch (iSwitch)
			{
			case 0:
				pObj = m_pMainTool->Clone_BoxColor();

				 matWorld = dynamic_cast<CBoxColor*>(pObj)
					->m_pTransformCom->Get_WorldMatrix();
				 m_listObj.push_back(pObj);
				break;
			case 1:
				pObj = m_pMainTool->Clone_CubeTexture(m_ctlTextureLIST.GetCurSel());

				 matWorld = dynamic_cast<CCubeTexture*>(pObj)
					->m_pTransformCom->Get_WorldMatrix();
				 m_listObj.push_back(pObj);
				break;
			case 2:
				pObj = m_pMainTool->Clone_SliderColor();

				 matWorld = dynamic_cast<CSliderColor*>(pObj)
					->m_pTransformCom->Get_WorldMatrix();
				 m_listObj.push_back(pObj);
				break;
			case 3:
				pObj = m_pMainTool->Clone_CubeColor();

				 matWorld = dynamic_cast<CCubeColor*>(pObj)
					->m_pTransformCom->Get_WorldMatrix();
				 m_listObj.push_back(pObj);
				break;
			case 4:
				pObj = m_pMainTool->Clone_CubeColor();

				 matWorld = dynamic_cast<CCubeColor*>(pObj)
					->m_pTransformCom->Get_WorldMatrix();
				 m_listObj.push_back(pObj);
				break;

			default:
				PRINT_LOG(L"오류", L"set 확인 바람.");
				return;
				break;
			}

			_float3 vX, vY, vZ, vPos;

			memcpy(&vX, &matWorld.m[0][0], sizeof(_float3));
			memcpy(&vY, &matWorld.m[1][0], sizeof(_float3));
			memcpy(&vZ, &matWorld.m[2][0], sizeof(_float3));
			vPos = _float3(m_fPosXXED, m_fPosYYED, m_fPosZZED);

			vX *= (_float)m_ivXED * (_float)i;
			vY *= (_float)m_ivYED * (_float)i;
			vZ *= (_float)m_ivZED * (_float)i;

			vPos += vX + vY + vZ;

			switch (iSwitch)
			{
			case 0:
				dynamic_cast<CBoxColor*>(pObj)->m_pTransformCom->Set_Position(vPos);
				dynamic_cast<CBoxColor*> (pObj)->m_pTransformCom->Set_Scale(_float3(m_fScaleXED, m_fScaleYED, m_fScaleZED));
				break;
			case 1:
				dynamic_cast<CCubeTexture*>(pObj)->m_pTransformCom->Set_Position(vPos);
				dynamic_cast<CCubeTexture*> (pObj)->m_pTransformCom->Set_Scale(_float3(m_fScaleXED, m_fScaleYED, m_fScaleZED));
				break;
			case 2:
				dynamic_cast<CSliderColor*>(pObj)->m_pTransformCom->Set_Position(vPos);
				dynamic_cast<CSliderColor*> (pObj)->m_pTransformCom->Set_Scale(_float3(m_fScaleXED, m_fScaleYED, m_fScaleZED));
				break;
			case 3:
				dynamic_cast<CBoxColor*>(pObj)->m_pTransformCom->Set_Position(vPos);
				dynamic_cast<CBoxColor*> (pObj)->m_pTransformCom->Set_Scale(_float3(m_fScaleXED, m_fScaleYED, m_fScaleZED));
				break;
			case 4:
				dynamic_cast<CBoxColor*>(pObj)->m_pTransformCom->Set_Position(vPos);
				dynamic_cast<CBoxColor*> (pObj)->m_pTransformCom->Set_Scale(_float3(m_fScaleXED, m_fScaleYED, m_fScaleZED));
				break;

			default:
				PRINT_LOG(L"오류", L"set 확인 바람.");
				return;
				break;
			}

		}
	}
	else
	{
		m_ctlObjectLIST.AddString(m_strNameED);

		CGameObj* pObj = nullptr;

		switch (iSwitch)
		{

		case 1:
			UpdateData(TRUE);
			m_ctlTextureLIST;
			UpdateData(FALSE);
	
			pObj = m_pMainTool->Clone_CubeTexture(m_ctlTextureLIST.GetCurSel());

			dynamic_cast<CCubeTexture*>(pObj)->m_pTransformCom
				->Set_Position(_float3(m_fPosXXED, m_fPosYYED, m_fPosZZED));
			dynamic_cast<CCubeTexture*> (pObj)->m_pTransformCom
				->Set_Scale(_float3(m_fScaleXED, m_fScaleYED, m_fScaleZED));
			m_listObj.push_back(pObj);
			break;

		case 4://etc
			UpdateData(TRUE);
			m_ctlTextureLIST;
			UpdateData(FALSE);
			pObj = m_pMainTool->Clone_CubeTexture(7);
			dynamic_cast<CCubeTexture*>(pObj)->m_pTransformCom
				->Set_Position(_float3(m_fPosXXED, m_fPosYYED, m_fPosZZED));
			dynamic_cast<CCubeTexture*> (pObj)->m_pTransformCom
				->Set_Scale(_float3(m_fScaleXED, m_fScaleYED, m_fScaleZED));
			m_listObj.push_back(pObj);
			break;

		default:
			PRINT_LOG(L"오류", L"확인 바람.");
			return;
			break;
		}
		
	}


}

void CTap04::OnBnClicked_Fix()
{
#pragma region UpdateData
	UpdateData(TRUE);
	//위치 업뎃.


	m_fScaleXED;
	m_fScaleYED;
	m_fScaleZED;

	m_strNameED;

	m_iCountED;

	m_ctlCubeColorCB;
	m_ctlCubeTextureCB;
	m_ctlSliderColorCB;
	m_ctlSliderTextureCB;
	m_ctlEtcCB;

	UpdateData(FALSE);
#pragma endregion

#pragma region 예외처리.

	if (nullptr == m_pLineObj)
	{
		m_pLineObj = m_pMainTool->Clone_CubeLine();
	}

	if (false == m_ctlCubeColorCB.GetCheck()
		&& false == m_ctlCubeTextureCB.GetCheck()
		&& false == m_ctlSliderColorCB.GetCheck()
		&& false == m_ctlSliderTextureCB.GetCheck()
		&& false == m_ctlEtcCB.GetCheck())
	{
		return;
	}


	if (0.f == m_fScaleXED && 0.f == m_fScaleYED && 0.f == m_fScaleZED)
	{
		return;
	}

	_bool bCheck = false;


#pragma endregion

#pragma region 사전 처리

	UpdateData(TRUE);
	int iSwitch = 0;
	if (TRUE == m_ctlCubeColorCB.GetCheck())
	{
		iSwitch = 0;
	}
	else if (TRUE == m_ctlCubeTextureCB.GetCheck())
	{
		iSwitch = 1;
	}
	else if (TRUE == m_ctlSliderColorCB.GetCheck())
	{
		iSwitch = 2;
	}

	else if (TRUE == m_ctlSliderTextureCB.GetCheck())
	{
		iSwitch = 3;
	}

	else if (TRUE == m_ctlEtcCB.GetCheck())
	{
		iSwitch = 4;
	}
	UpdateData(FALSE);
#pragma endregion 
	UpdateData(TRUE);
	int iIndex = m_ctlObjectLIST.GetCurSel();

	auto& iter = m_listObj.begin();

	for (int i = 0;  i < iIndex; ++i)
	{
		++iter;
	}



	CGameObj* pObj = (*iter);

	switch (iSwitch)
	{
	case 0:
		dynamic_cast<CBoxColor*>(pObj)->m_pTransformCom
			->Set_Position(_float3(m_fPosXXED, m_fPosYYED, m_fPosZZED));
		dynamic_cast<CBoxColor*> (pObj)->m_pTransformCom
			->Set_Scale(_float3(m_fScaleXED, m_fScaleYED, m_fScaleZED));

	case 1:
		dynamic_cast<CCubeTexture*>(pObj)->m_iTextuer = m_ctlTextureLIST.GetCurSel();
		dynamic_cast<CCubeTexture*>(pObj)->m_pTransformCom
			->Set_Position(_float3(m_fPosXXED, m_fPosYYED, m_fPosZZED));
		dynamic_cast<CCubeTexture*> (pObj)->m_pTransformCom
			->Set_Scale(_float3(m_fScaleXED, m_fScaleYED, m_fScaleZED));
		break;

	case 2:
		dynamic_cast<CSliderColor*>(pObj)->m_pTransformCom
			->Set_Position(_float3(m_fPosXXED, m_fPosYYED, m_fPosZZED));
		dynamic_cast<CSliderColor*> (pObj)->m_pTransformCom
			->Set_Scale(_float3(m_fScaleXED, m_fScaleYED, m_fScaleZED));
		break;

	case 3:
		dynamic_cast<CBoxColor*>(pObj)->m_pTransformCom
			->Set_Position(_float3(m_fPosXXED, m_fPosYYED, m_fPosZZED));
		dynamic_cast<CBoxColor*> (pObj)->m_pTransformCom
			->Set_Scale(_float3(m_fScaleXED, m_fScaleYED, m_fScaleZED));
		break;

	case 4:
		dynamic_cast<CBoxColor*>(pObj)->m_pTransformCom
			->Set_Position(_float3(m_fPosXXED, m_fPosYYED, m_fPosZZED));
		dynamic_cast<CBoxColor*> (pObj)->m_pTransformCom
			->Set_Scale(_float3(m_fScaleXED, m_fScaleYED, m_fScaleZED));
		break;

	default:
		PRINT_LOG(L"오류", L"확인 바람.");
		return;
		break;
	}
	UpdateData(FALSE);

}



void CTap04::OnBnClicked_Delete()
{

	UpdateData(true);
	auto& iter = m_listObj.begin();

	for (int i = 0; i < m_listObj.size(); ++i)
	{
		if ( true == TerrainChecker_Select(m_pLineObj, (*iter)))
		{
			(*iter)->m_bDead = true;
			m_listObj.erase(iter);
			m_ctlObjectLIST.DeleteString(i);

			UpdateData(false);

			return;
		}
		++iter;
	}

	UpdateData(false);

}


void CTap04::OnBnClicked_AllClear()
{
	int iIndex = 0;
	m_ctlObjectLIST.ResetContent();

	for (auto& iter : m_listObj)
	{
		iter->m_bDead = true;
	}
	m_listObj.clear();
}


void CTap04::OnLIST_Object()//눈갱 주의
{
#pragma region 사전 처리
	UpdateData(TRUE);

	int iSwitch = 0;
	if (TRUE == m_ctlCubeColorCB.GetCheck())
	{
		iSwitch = 0;
	}
	else if (TRUE == m_ctlCubeTextureCB.GetCheck())
	{
		iSwitch = 1;
	}
	else if (TRUE == m_ctlSliderColorCB.GetCheck())
	{
		iSwitch = 2;
	}

	else if (TRUE == m_ctlSliderTextureCB.GetCheck())
	{
		iSwitch = 3;
	}

	else if (TRUE == m_ctlEtcCB.GetCheck())
	{
		iSwitch = 4;
	}

	UpdateData(FALSE);
#pragma endregion 

#pragma region 예외 처리


	if (nullptr == m_pLineObj)
	{
		m_pLineObj = m_pMainTool->Clone_CubeLine();
	}
	

#pragma endregion 

	int iIndex = 0;
	iIndex = m_ctlObjectLIST.GetCurSel();

	auto& iter = m_listObj.begin();
	for (int i = 0; i < iIndex; ++i)
	{
		++iter;
	}

	if (nullptr == (*iter))
	{
		return;
	}


	_float3 vPos, vScale;


	switch (iSwitch)
	{
	case 0:
		vPos = dynamic_cast<CBoxColor*>((*iter))->m_pTransformCom->Get_Position();
		vScale = dynamic_cast<CBoxColor*>((*iter))->m_pTransformCom->Get_Scale();
		break;
	case 1:
		vPos = dynamic_cast<CCubeTexture*>((*iter))->m_pTransformCom->Get_Position();
		vScale = dynamic_cast<CCubeTexture*>((*iter))->m_pTransformCom->Get_Scale();
		break;
	case 2:
		vPos = dynamic_cast<CSliderColor*>((*iter))->m_pTransformCom->Get_Position();
		vScale = dynamic_cast<CSliderColor*>((*iter))->m_pTransformCom->Get_Scale();
		break;
	case 3:
		vPos = dynamic_cast<CBoxColor*>((*iter))->m_pTransformCom->Get_Position();
		vScale = dynamic_cast<CBoxColor*>((*iter))->m_pTransformCom->Get_Scale();
		break;
	case 4:
		vPos = dynamic_cast<CBoxColor*>((*iter))->m_pTransformCom->Get_Position();
		vScale = dynamic_cast<CBoxColor*>((*iter))->m_pTransformCom->Get_Scale();
		break;

	default:
		PRINT_LOG(L"오류", L"Obj list 확인 바람.");
		return;
		break;
	}



	UpdateData(TRUE);
	m_fPosXXED = vPos.x;
	m_fPosYYED = vPos.y;
	m_fPosZZED = vPos.z;

	m_fScaleXED = vScale.x;
	m_fScaleYED = vScale.y;
	m_fScaleZED = vScale.z;
	UpdateData(FALSE);


	dynamic_cast<CCubeLine*>(m_pLineObj)->m_pTransformCom->Set_Position(vPos);
	dynamic_cast<CCubeLine*>(m_pLineObj)->m_pTransformCom->Set_Scale(_float3(m_fScaleXED, m_fScaleYED, m_fScaleZED));

}


void CTap04::OnLIST_Texture()
{

}


void CTap04::OnBnClicked_RotX()
{
	if (0 == m_listObj.size())
		return;

	int iIndex = 0;
	iIndex = m_ctlObjectLIST.GetCurSel();

	auto& iter = m_listObj.begin();
	for (int i = 0; i < iIndex; ++i)
	{
		++iter;
	}

	dynamic_cast<CCubeColor*>((*iter))->m_pTransformCom->Rotate_Quick(EAxis::Y, D3DXToRadian(90.f));
}


void CTap04::OnBnClicked_RotY()
{
	if (0 == m_listObj.size())
		return;

	int iIndex = 0;
	iIndex = m_ctlObjectLIST.GetCurSel();

	auto& iter = m_listObj.begin();
	for (int i = 0; i < iIndex; ++i)
	{
		++iter;
	}

	dynamic_cast<CCubeColor*>((*iter))->m_pTransformCom->Rotate_Quick(EAxis::Y, D3DXToRadian(90.f));
}


void CTap04::OnBnClicked_RotZ()
{
	if (0 == m_listObj.size())
		return;

	int iIndex = 0;
	iIndex = m_ctlObjectLIST.GetCurSel();

	auto& iter = m_listObj.begin();
	for (int i = 0; i < iIndex; ++i)
	{
		++iter;
	}

	dynamic_cast<CCubeColor*>((*iter))->m_pTransformCom->Rotate_Quick(EAxis::Z, D3DXToRadian(90.f));
}


void CTap04::OnBnClicked_MatrixInverse()
{
}

//세이브
#pragma region 버림.

void CTap04::CheckBox_AllSave()
{
	//m_ctlSelectSaveCB.SetCheck(FALSE);
}


//void CTap04::CheckBox_SelectSave()
//{
//	m_ctlAllSaveCB.SetCheck(FALSE);
//}
#pragma endregion


void CTap04::OnBnClicked_Save()
{
#pragma region 예외처리
	/*if (false == m_ctlSelectSaveCB.GetCheck()
		&& false == m_ctlAllSaveCB.GetCheck())
	{
		return;
	}
	if (true == m_ctlSelectSaveCB.GetCheck()
		&& true == m_ctlAllSaveCB.GetCheck())
	{
		return;
	}*/
#pragma endregion

#pragma region 사전 처리

	UpdateData(TRUE);

	int iSwitch = 0;
	if (TRUE == m_ctlCubeColorCB.GetCheck())
	{
		iSwitch = 0;
	}
	else if (TRUE == m_ctlCubeTextureCB.GetCheck())
	{
		iSwitch = 1;
	}
	else if (TRUE == m_ctlSliderColorCB.GetCheck())
	{
		iSwitch = 2;
	}

	else if (TRUE == m_ctlSliderTextureCB.GetCheck())
	{
		iSwitch = 3;
	}

	else if (TRUE == m_ctlEtcCB.GetCheck())
	{
		iSwitch = 4;
	}
	UpdateData(FALSE);
#pragma endregion 
	if (TRUE == m_ctlAllSaveCB.GetCheck())
	{
		//창을 띄우도록 하는 함수. 확장자도 정함.
		CFileDialog Dlg(FALSE, L"dat", L"*.dat", OFN_OVERWRITEPROMPT, L"Data File(*.dat)|*.dat||", this);

		TCHAR szBuf[MAX_PATH] = L"";
		GetCurrentDirectory(MAX_PATH, szBuf); // 현재의 작업경로를 얻음.
		//D:\박병건\112A\FrameWork
		PathRemoveFileSpec(szBuf); //패스 (경로) 와 파일 명을 얻는 함수.
		lstrcat(szBuf, L"\\Data");//문자열 뒤에 다른 문자열을 결합하는 함수.
		Dlg.m_ofn.lpstrInitialDir = szBuf; // 시작폴더를 정하는 듯.

		if (IDOK == Dlg.DoModal())// 아마 ok를 눌렀을 경우를 의미하는 것 같다.
		{
			// 파일의 경로 얻어오느 함수
			CString strFilePath = Dlg.GetPathName();
			HANDLE hFile = CreateFile(strFilePath.GetString(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

			if (INVALID_HANDLE_VALUE == hFile)
			{
				PRINT_LOG(L"저장 실패", L"OnBnClicked_Save 확인");
				return;
			}

			DWORD dwByte = 0;
			DWORD dwStrByte = 0;

			for (auto& iter : m_listObj)//저장 과정.
			{
				if (L"Static_ReadFile" != dynamic_cast<CCubeTexture*>(iter)->m_strName)
				{
					switch (iSwitch)
					{
					case 0:

					case 1:
						WriteFile(hFile, &dynamic_cast<CCubeTexture*>(iter)->m_pTransformCom->Get_Position(), sizeof(_float3), &dwByte, nullptr);
						WriteFile(hFile, &dynamic_cast<CCubeTexture*>(iter)->m_pTransformCom->Get_Rotation(), sizeof(_float3), &dwByte, nullptr);
						WriteFile(hFile, &dynamic_cast<CCubeTexture*>(iter)->m_pTransformCom->Get_Scale(), sizeof(_float3), &dwByte, nullptr);
						WriteFile(hFile, &dynamic_cast<CCubeTexture*>(iter)->m_iTextuer, sizeof(int), &dwByte, nullptr);
						break;
					case 2:
						break;
					case 3:
						break;
					case 4:
						WriteFile(hFile, &dynamic_cast<CCubeTexture*>(iter)->m_pTransformCom->Get_Position(), sizeof(_float3), &dwByte, nullptr);
						WriteFile(hFile, &dynamic_cast<CCubeTexture*>(iter)->m_pTransformCom->Get_Rotation(), sizeof(_float3), &dwByte, nullptr);
						WriteFile(hFile, &dynamic_cast<CCubeTexture*>(iter)->m_pTransformCom->Get_Scale(), sizeof(_float3), &dwByte, nullptr);
						break;
					default:
						PRINT_LOG(L"오류", L"set 확인 바람.");
						return;
						break;
					}
				}
			}
			CloseHandle(hFile);
		}

	}
}

void CTap04::OnBnClicked_Load()
{

#pragma region 사전 처리

	UpdateData(TRUE);
	int iSwitch = 0;
	if (TRUE == m_ctlCubeColorCB.GetCheck())
	{
		iSwitch = 0;
	}
	else if (TRUE == m_ctlCubeTextureCB.GetCheck())
	{
		iSwitch = 1;
	}
	else if (TRUE == m_ctlSliderColorCB.GetCheck())
	{
		iSwitch = 2;
	}

	else if (TRUE == m_ctlSliderTextureCB.GetCheck())
	{
		iSwitch = 3;
	}

	else if (TRUE == m_ctlEtcCB.GetCheck())
	{
		iSwitch = 4;
	}
	UpdateData(FALSE);

#pragma endregion 
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
			PRINT_LOG(L"파일 오픈 실패", L"파일 오픈 실패");
			return;
		}
		DWORD dwByte = 0;
		DWORD dwStrByte = 0;
		TCHAR* pTemp = nullptr;

		//int i = 0;
		CString strName;
		if (true == m_ctlStaticLoadCB.GetCheck())
		{
			strName = L"Static_ReadFile";
		}
		else if(true==m_ctlEtcCB.GetCheck())
		{
			strName = L"Object_ReadFile";
		}
		else
		{
			 strName = L"Able_ReadFile";
		}



		while (true)
		{
			_float3 vPos, vRot, vScale;
			int iTextureNumber = 0;
			ReadFile(hFile, &vPos, sizeof(_float3), &dwByte, nullptr);
			ReadFile(hFile, &vRot, sizeof(_float3), &dwByte, nullptr);
			ReadFile(hFile, &vScale, sizeof(_float3), &dwByte, nullptr);
			ReadFile(hFile, &iTextureNumber, sizeof(int), &dwByte, nullptr);



			if (0 == dwByte)
			{
				break;
			}

			CGameObj* pObj = nullptr;

			pObj = m_pMainTool->Clone_CubeTexture();
			dynamic_cast<CCubeTexture*>(pObj)->m_pTransformCom->Set_Position(vPos);
			dynamic_cast<CCubeTexture*>(pObj)->m_pTransformCom->Set_Rotation(vRot);
			dynamic_cast<CCubeTexture*>(pObj)->m_pTransformCom->Set_Scale(vScale);
			
			if (false == m_ctlEtcCB.GetCheck())
			{
				dynamic_cast<CCubeTexture*>(pObj)->m_iTextuer = iTextureNumber;

			}

			m_listObj.push_back(pObj);

			if (true == m_ctlStaticLoadCB.GetCheck() && false == m_ctlEtcCB.GetCheck())
			{
				dynamic_cast<CCubeTexture*>(pObj)->m_strName = L"Static_ReadFile";
			}

			m_ctlObjectLIST.AddString(strName);
	
		}

		CloseHandle(hFile);
		UpdateData(FALSE);
	}

}


#pragma region 오브젝트 체크박스.

void CTap04::CheckBox_CubeColor()
{
	m_ctlCubeTextureCB.SetCheck(false);
	m_ctlSliderColorCB.SetCheck(false);
	m_ctlSliderTextureCB.SetCheck(false);
	m_ctlEtcCB.SetCheck(false);
}


void CTap04::CheckBox_CubeTexture()
{
	m_ctlCubeColorCB.SetCheck(false);
	m_ctlSliderColorCB.SetCheck(false);
	m_ctlSliderTextureCB.SetCheck(false);
	m_ctlEtcCB.SetCheck(false);
}


void CTap04::CheckBox_SliderColor()
{
	m_ctlCubeColorCB.SetCheck(false);
	m_ctlCubeTextureCB.SetCheck(false);
	m_ctlSliderTextureCB.SetCheck(false);
	m_ctlEtcCB.SetCheck(false);
}


void CTap04::CheckBox_SliderTexture()
{
	m_ctlCubeColorCB.SetCheck(false);
	m_ctlCubeTextureCB.SetCheck(false);
	m_ctlSliderColorCB.SetCheck(false);
	m_ctlEtcCB.SetCheck(false);
}


void CTap04::CheckBox_Etc()
{
	m_ctlCubeColorCB.SetCheck(false);
	m_ctlCubeTextureCB.SetCheck(false);
	m_ctlSliderColorCB.SetCheck(false);
	m_ctlSliderTextureCB.SetCheck(false);

	m_ctlTextureLIST.SetCurSel(7);
}
#pragma endregion


void CTap04::OnBnClicked_UpXSet()
{
#pragma region UpdateData
	UpdateData(TRUE);
	//위치 업뎃.


	m_fScaleXED;
	m_fScaleYED;
	m_fScaleZED;

	m_strNameED;

	m_iCountED;

	m_ctlCubeColorCB;
	m_ctlCubeTextureCB;
	m_ctlSliderColorCB;
	m_ctlSliderTextureCB;
	m_ctlEtcCB;

	UpdateData(FALSE);
#pragma endregion
#pragma region 사전 처리

	UpdateData(TRUE);
	int iSwitch = 0;
	if (TRUE == m_ctlCubeColorCB.GetCheck())
	{
		iSwitch = 0;
	}
	else if (TRUE == m_ctlCubeTextureCB.GetCheck())
	{
		iSwitch = 1;
	}
	else if (TRUE == m_ctlSliderColorCB.GetCheck())
	{
		iSwitch = 2;
	}

	else if (TRUE == m_ctlSliderTextureCB.GetCheck())
	{
		iSwitch = 3;
	}

	else if (TRUE == m_ctlEtcCB.GetCheck())
	{
		iSwitch = 4;
	}
	UpdateData(FALSE);

#pragma endregion 

	if (nullptr == m_pLineObj)
	{
		return;
	}

	CGameObj* pObj = nullptr;

	UpdateData(TRUE);

	++m_fPosXXED;

	dynamic_cast<CCubeLine*>(m_pLineObj)->m_pTransformCom->Set_Position(_float3(m_fPosXXED, m_fPosYYED, m_fPosZZED));

	if (true == m_ctlSetCheckerCB.GetCheck()&& false == m_ctlEraseCB.GetCheck())
	{
		if (!TerrainChecker(m_pLineObj))
		{
			m_ctlObjectLIST.AddString(m_strNameED);

			pObj = m_pMainTool->Clone_CubeTexture(m_ctlTextureLIST.GetCurSel());
			dynamic_cast<CCubeTexture*>(pObj)->m_pTransformCom
				->Set_Position(_float3(m_fPosXXED, m_fPosYYED, m_fPosZZED));
			dynamic_cast<CCubeTexture*> (pObj)->m_pTransformCom
				->Set_Scale(_float3(m_fScaleXED, m_fScaleYED, m_fScaleZED));
			
			m_listObj.push_back(pObj);
		}
	}
	else if (true == m_ctlEraseCB.GetCheck())
	{


		auto& iter = m_listObj.begin();

		for (int i = 0; i < m_listObj.size(); ++i)
		{
			if (true == TerrainChecker_Select(m_pLineObj, (*iter)))
			{
				if (L"Static_ReadFile" == dynamic_cast<CCubeTexture*>(*iter)->m_strName)
				{
					UpdateData(false);
					return;
				}

				(*iter)->m_bDead = true;
				m_listObj.erase(iter);
				m_ctlObjectLIST.DeleteString(i);

				UpdateData(false);
				return;
			}
			++iter;
		}

	}
	
	UpdateData(FALSE);

}


void CTap04::OnBnClicked_DownXSet()
{
#pragma region UpdateData
	UpdateData(TRUE);
	//위치 업뎃.


	m_fScaleXED;
	m_fScaleYED;
	m_fScaleZED;

	m_strNameED;

	m_iCountED;

	m_ctlCubeColorCB;
	m_ctlCubeTextureCB;
	m_ctlSliderColorCB;
	m_ctlSliderTextureCB;
	m_ctlEtcCB;

	UpdateData(FALSE);
#pragma endregion
#pragma region 사전 처리

	UpdateData(TRUE);
	int iSwitch = 0;
	if (TRUE == m_ctlCubeColorCB.GetCheck())
	{
		iSwitch = 0;
	}
	else if (TRUE == m_ctlCubeTextureCB.GetCheck())
	{
		iSwitch = 1;
	}
	else if (TRUE == m_ctlSliderColorCB.GetCheck())
	{
		iSwitch = 2;
	}

	else if (TRUE == m_ctlSliderTextureCB.GetCheck())
	{
		iSwitch = 3;
	}

	else if (TRUE == m_ctlEtcCB.GetCheck())
	{
		iSwitch = 4;
	}
	UpdateData(FALSE);

#pragma endregion 

	if (nullptr == m_pLineObj)
	{
		return;
	}

	CGameObj* pObj = nullptr;

	UpdateData(TRUE);

	--m_fPosXXED;

	dynamic_cast<CCubeLine*>(m_pLineObj)->m_pTransformCom->Set_Position(_float3(m_fPosXXED, m_fPosYYED, m_fPosZZED));

	if (true == m_ctlSetCheckerCB.GetCheck()&& false == m_ctlEraseCB.GetCheck())
	{
		if (!TerrainChecker(m_pLineObj))
		{
			m_ctlObjectLIST.AddString(m_strNameED);

			pObj = m_pMainTool->Clone_CubeTexture(m_ctlTextureLIST.GetCurSel());
			dynamic_cast<CCubeTexture*>(pObj)->m_pTransformCom
				->Set_Position(_float3(m_fPosXXED, m_fPosYYED, m_fPosZZED));
			dynamic_cast<CCubeTexture*> (pObj)->m_pTransformCom
				->Set_Scale(_float3(m_fScaleXED, m_fScaleYED, m_fScaleZED));

			m_listObj.push_back(pObj);
		}
	}
	else if (true == m_ctlEraseCB.GetCheck())
	{


		auto& iter = m_listObj.begin();

		for (int i = 0; i < m_listObj.size(); ++i)
		{
			if (true == TerrainChecker_Select(m_pLineObj, (*iter)))
			{
				if (L"Static_ReadFile" == dynamic_cast<CCubeTexture*>(*iter)->m_strName)
				{
					UpdateData(false);
					return;
				}

				(*iter)->m_bDead = true;
				m_listObj.erase(iter);
				m_ctlObjectLIST.DeleteString(i);

				UpdateData(false);
				return;
			}
			++iter;
		}

	}
	UpdateData(FALSE);
}


void CTap04::OnBnClicked_UpZSet()
{
#pragma region UpdateData
	UpdateData(TRUE);
	//위치 업뎃.


	m_fScaleXED;
	m_fScaleYED;
	m_fScaleZED;

	m_strNameED;

	m_iCountED;

	m_ctlCubeColorCB;
	m_ctlCubeTextureCB;
	m_ctlSliderColorCB;
	m_ctlSliderTextureCB;
	m_ctlEtcCB;

	UpdateData(FALSE);
#pragma endregion
#pragma region 사전 처리

	UpdateData(TRUE);
	int iSwitch = 0;
	if (TRUE == m_ctlCubeColorCB.GetCheck())
	{
		iSwitch = 0;
	}
	else if (TRUE == m_ctlCubeTextureCB.GetCheck())
	{
		iSwitch = 1;
	}
	else if (TRUE == m_ctlSliderColorCB.GetCheck())
	{
		iSwitch = 2;
	}

	else if (TRUE == m_ctlSliderTextureCB.GetCheck())
	{
		iSwitch = 3;
	}

	else if (TRUE == m_ctlEtcCB.GetCheck())
	{
		iSwitch = 4;
	}
	UpdateData(FALSE);

#pragma endregion 

	if (nullptr == m_pLineObj)
	{
		return;
	}

	CGameObj* pObj = nullptr;

	UpdateData(TRUE);

	++m_fPosZZED;

	dynamic_cast<CCubeLine*>(m_pLineObj)->m_pTransformCom->Set_Position(_float3(m_fPosXXED, m_fPosYYED, m_fPosZZED));

	if (true == m_ctlSetCheckerCB.GetCheck()&& false == m_ctlEraseCB.GetCheck())
	{
		if (!TerrainChecker(m_pLineObj))
		{
			m_ctlObjectLIST.AddString(m_strNameED);

			pObj = m_pMainTool->Clone_CubeTexture(m_ctlTextureLIST.GetCurSel());
			dynamic_cast<CCubeTexture*>(pObj)->m_pTransformCom
				->Set_Position(_float3(m_fPosXXED, m_fPosYYED, m_fPosZZED));
			dynamic_cast<CCubeTexture*> (pObj)->m_pTransformCom
				->Set_Scale(_float3(m_fScaleXED, m_fScaleYED, m_fScaleZED));

			m_listObj.push_back(pObj);
		}
	}
	else if (true == m_ctlEraseCB.GetCheck())
	{
		auto& iter = m_listObj.begin();

		for (int i = 0; i < m_listObj.size(); ++i)
		{
			if (true == TerrainChecker_Select(m_pLineObj, (*iter)))
			{
				if (L"Static_ReadFile" == dynamic_cast<CCubeTexture*>(*iter)->m_strName)
				{
					UpdateData(false);
					return;
				}

				(*iter)->m_bDead = true;
				m_listObj.erase(iter);
				m_ctlObjectLIST.DeleteString(i);

				UpdateData(false);
				return;
			}
			++iter;
		}
	}
	UpdateData(FALSE);
}



void CTap04::OnBnClicked_DownZSet()
{

#pragma region UpdateData
	UpdateData(TRUE);
	//위치 업뎃.


	m_fScaleXED;
	m_fScaleYED;
	m_fScaleZED;

	m_strNameED;

	m_iCountED;

	m_ctlCubeColorCB;
	m_ctlCubeTextureCB;
	m_ctlSliderColorCB;
	m_ctlSliderTextureCB;
	m_ctlEtcCB;

	UpdateData(FALSE);
#pragma endregion
#pragma region 사전 처리

	UpdateData(TRUE);
	int iSwitch = 0;
	if (TRUE == m_ctlCubeColorCB.GetCheck())
	{
		iSwitch = 0;
	}
	else if (TRUE == m_ctlCubeTextureCB.GetCheck())
	{
		iSwitch = 1;
	}
	else if (TRUE == m_ctlSliderColorCB.GetCheck())
	{
		iSwitch = 2;
	}

	else if (TRUE == m_ctlSliderTextureCB.GetCheck())
	{
		iSwitch = 3;
	}

	else if (TRUE == m_ctlEtcCB.GetCheck())
	{
		iSwitch = 4;
	}
	UpdateData(FALSE);

#pragma endregion 
	if (nullptr == m_pLineObj)
	{
		return;
	}

	CGameObj* pObj = nullptr;

	UpdateData(TRUE);

	--m_fPosZZED;

	dynamic_cast<CCubeLine*>(m_pLineObj)->m_pTransformCom->Set_Position(_float3(m_fPosXXED, m_fPosYYED, m_fPosZZED));

	if (true == m_ctlSetCheckerCB.GetCheck()&& false == m_ctlEraseCB.GetCheck())
	{
		if (!TerrainChecker(m_pLineObj))
		{
			m_ctlObjectLIST.AddString(m_strNameED);

			pObj = m_pMainTool->Clone_CubeTexture(m_ctlTextureLIST.GetCurSel());
			dynamic_cast<CCubeTexture*>(pObj)->m_pTransformCom
				->Set_Position(_float3(m_fPosXXED, m_fPosYYED, m_fPosZZED));
			dynamic_cast<CCubeTexture*> (pObj)->m_pTransformCom
				->Set_Scale(_float3(m_fScaleXED, m_fScaleYED, m_fScaleZED));

			m_listObj.push_back(pObj);
		}
	}
	else if (true == m_ctlEraseCB.GetCheck())
	{
		auto& iter = m_listObj.begin();

		for (int i = 0; i < m_listObj.size(); ++i)
		{
			if (true == TerrainChecker_Select(m_pLineObj, (*iter)))
			{
				if (L"Static_ReadFile" == dynamic_cast<CCubeTexture*>(*iter)->m_strName)
				{
					UpdateData(false);
					return;
				}

				(*iter)->m_bDead = true;
				m_listObj.erase(iter);
				m_ctlObjectLIST.DeleteString(i);

				UpdateData(false);
				return;
			}
			++iter;
		}
	}
	UpdateData(FALSE);
}


void CTap04::OnBnClicked_UpYSet()
{

#pragma region UpdateData
	UpdateData(TRUE);
	//위치 업뎃.


	m_fScaleXED;
	m_fScaleYED;
	m_fScaleZED;

	m_strNameED;

	m_iCountED;

	m_ctlCubeColorCB;
	m_ctlCubeTextureCB;
	m_ctlSliderColorCB;
	m_ctlSliderTextureCB;
	m_ctlEtcCB;

	UpdateData(FALSE);
#pragma endregion
#pragma region 사전 처리

	UpdateData(TRUE);
	int iSwitch = 0;
	if (TRUE == m_ctlCubeColorCB.GetCheck())
	{
		iSwitch = 0;
	}
	else if (TRUE == m_ctlCubeTextureCB.GetCheck())
	{
		iSwitch = 1;
	}
	else if (TRUE == m_ctlSliderColorCB.GetCheck())
	{
		iSwitch = 2;
	}

	else if (TRUE == m_ctlSliderTextureCB.GetCheck())
	{
		iSwitch = 3;
	}

	else if (TRUE == m_ctlEtcCB.GetCheck())
	{
		iSwitch = 4;
	}
	UpdateData(FALSE);

#pragma endregion 

	if (nullptr == m_pLineObj)
	{
		return;
	}

	CGameObj* pObj = nullptr;

	UpdateData(TRUE);

	++m_fPosYYED;

	dynamic_cast<CCubeLine*>(m_pLineObj)->m_pTransformCom->Set_Position(_float3(m_fPosXXED, m_fPosYYED, m_fPosZZED));

	if (true == m_ctlSetCheckerCB.GetCheck()&& false == m_ctlEraseCB.GetCheck())
	{
		if (!TerrainChecker(m_pLineObj))
		{
			m_ctlObjectLIST.AddString(m_strNameED);

			pObj = m_pMainTool->Clone_CubeTexture(m_ctlTextureLIST.GetCurSel());
			dynamic_cast<CCubeTexture*>(pObj)->m_pTransformCom
				->Set_Position(_float3(m_fPosXXED, m_fPosYYED, m_fPosZZED));
			dynamic_cast<CCubeTexture*> (pObj)->m_pTransformCom
				->Set_Scale(_float3(m_fScaleXED, m_fScaleYED, m_fScaleZED));

			m_listObj.push_back(pObj);
		}
	}
	else if (true == m_ctlEraseCB.GetCheck())
	{
		auto& iter = m_listObj.begin();

		for (int i = 0; i < m_listObj.size(); ++i)
		{
			if (true == TerrainChecker_Select(m_pLineObj, (*iter)))
			{
				if (L"Static_ReadFile" == dynamic_cast<CCubeTexture*>(*iter)->m_strName)
				{
					UpdateData(false);
					return;
				}

				(*iter)->m_bDead = true;
				m_listObj.erase(iter);
				m_ctlObjectLIST.DeleteString(i);

				UpdateData(false);
				return;
			}
			++iter;
		}
	}
	UpdateData(FALSE);
}


void CTap04::OnBnClicked_DownYSet()
{
#pragma region UpdateData
	UpdateData(TRUE);
	//위치 업뎃.


	m_fScaleXED;
	m_fScaleYED;
	m_fScaleZED;

	m_strNameED;

	m_iCountED;

	m_ctlCubeColorCB;
	m_ctlCubeTextureCB;
	m_ctlSliderColorCB;
	m_ctlSliderTextureCB;
	m_ctlEtcCB;

	UpdateData(FALSE);
#pragma endregion
#pragma region 사전 처리

	UpdateData(TRUE);
	int iSwitch = 0;
	if (TRUE == m_ctlCubeColorCB.GetCheck())
	{
		iSwitch = 0;
	}
	else if (TRUE == m_ctlCubeTextureCB.GetCheck())
	{
		iSwitch = 1;
	}
	else if (TRUE == m_ctlSliderColorCB.GetCheck())
	{
		iSwitch = 2;
	}

	else if (TRUE == m_ctlSliderTextureCB.GetCheck())
	{
		iSwitch = 3;
	}

	else if (TRUE == m_ctlEtcCB.GetCheck())
	{
		iSwitch = 4;
	}
	UpdateData(FALSE);

#pragma endregion 

	if (nullptr == m_pLineObj)
	{
		return;
	}

	CGameObj* pObj = nullptr;

	UpdateData(TRUE);

	--m_fPosYYED;

	dynamic_cast<CCubeLine*>(m_pLineObj)->m_pTransformCom->Set_Position(_float3(m_fPosXXED, m_fPosYYED, m_fPosZZED));

	if (true == m_ctlSetCheckerCB.GetCheck()&& false == m_ctlEraseCB.GetCheck())
	{
		if (!TerrainChecker(m_pLineObj))
		{
			m_ctlObjectLIST.AddString(m_strNameED);

			pObj = m_pMainTool->Clone_CubeTexture(m_ctlTextureLIST.GetCurSel());
			dynamic_cast<CCubeTexture*>(pObj)->m_pTransformCom
				->Set_Position(_float3(m_fPosXXED, m_fPosYYED, m_fPosZZED));
			dynamic_cast<CCubeTexture*> (pObj)->m_pTransformCom
				->Set_Scale(_float3(m_fScaleXED, m_fScaleYED, m_fScaleZED));

			m_listObj.push_back(pObj);
		}
	}
	else if (true == m_ctlEraseCB.GetCheck())
	{
		auto& iter = m_listObj.begin();

		for (int i = 0; i < m_listObj.size(); ++i)
		{
			if (true == TerrainChecker_Select(m_pLineObj, (*iter)))
			{
				if (L"Static_ReadFile" == dynamic_cast<CCubeTexture*>(*iter)->m_strName)
				{
					UpdateData(false);
					return;
				}

				(*iter)->m_bDead = true;
				m_listObj.erase(iter);
				m_ctlObjectLIST.DeleteString(i);

				UpdateData(false);
				return;
			}
			++iter;
		}
	}

	UpdateData(FALSE);
}



bool CTap04::TerrainChecker(CGameObj * Dst)
{
	if (0 == m_listObj.size())
	{
		return false;
	}

	for (auto& Src : m_listObj) // 대상의 메모를 받고.
	{
		float fX = fabs(dynamic_cast<CCubeLine*>(Dst)->Get_Transform()->Get_Position().x - dynamic_cast<CCubeTexture*>(Src)->Get_Transform()->Get_Position().x);
		float fY = fabs(dynamic_cast<CCubeLine*>(Dst)->Get_Transform()->Get_Position().y  - dynamic_cast<CCubeTexture*>(Src)->Get_Transform()->Get_Position().y);
		float fZ = fabs(dynamic_cast<CCubeLine*>(Dst)->Get_Transform()->Get_Position().z - dynamic_cast<CCubeTexture*>(Src)->Get_Transform()->Get_Position().z);

		float fCX = (float)((0.5f + dynamic_cast<CCubeTexture*>(Src)->Get_Transform()->Get_AbsScale().x)*0.5f);
		float fCY = (float)((0.5f + dynamic_cast<CCubeTexture*>(Src)->Get_Transform()->Get_AbsScale().y)*0.5f);
		float fCZ = (float)((0.5f + dynamic_cast<CCubeTexture*>(Src)->Get_Transform()->Get_AbsScale().z)*0.5f);

		if (fCX > fX && fCZ > fZ && fCY > fY)
		{
			return true;
		}
	}
	return false;
}


bool CTap04::TerrainChecker_Select(CGameObj * Dst , CGameObj* Src)
{
	if (0 == m_listObj.size())
	{
		return false;
	}

	_float3 vPos_Dst = dynamic_cast<CCubeLine*>(Dst)->Get_Transform()->Get_Position();
	_float3 vPos_Src = dynamic_cast<CCubeTexture*>(Src)->Get_Transform()->Get_Position();

	_float3 vScale_Dst = dynamic_cast<CCubeLine*>(Dst)->Get_Transform()->Get_AbsScale();
	_float3 vScale_Src = dynamic_cast<CCubeTexture*>(Src)->Get_Transform()->Get_AbsScale();

	float fX = fabs(vPos_Dst.x - vPos_Src.x);
	float fY = fabs(vPos_Dst.y - vPos_Src.y);
	float fZ = fabs(vPos_Dst.z - vPos_Src.z);


	float fCX = (float)(((vScale_Dst.x)-0.5f + vScale_Src.x) *0.5);
	float fCY = (float)(((vScale_Dst.y)-0.5f + vScale_Src.y) *0.5);
	float fCZ = (float)(((vScale_Dst.z)-0.5f + vScale_Src.z) *0.5);
	

	//float fCX = (float)((1.f + vScale_Src.x) *0.5);
	//float fCY = (float)((1.f + vScale_Src.y) *0.5);
	//float fCZ = (float)((1.f + vScale_Src.z) *0.5);

	if (fCX > fX && fCZ > fZ && fCY > fY)
	{
		return true;
	}

	return false;
}
