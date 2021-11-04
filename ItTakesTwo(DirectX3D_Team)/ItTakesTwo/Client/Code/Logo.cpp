#include "stdafx.h"
#include "Logo.h"
#include "Menu.h"
#include "MenuSelect.h"
#include "MenuPlayerSelect.h"
#include "Export_Function.h"
#include "UI.h"
#include "SoundMgr.h"
#include "Ending.h"
#include "ChessBoss.h"
#include "ChessField.h"
#include "TestField.h"
#include "VacuumBoss.h"
#include "VacuumField.h"
#include "VacuumField2.h"
#include "VacuumField3.h"
#include "SpaceField.h"
#include "SpaceField2.h"
#include "SpaceField3.h"
#include "SpaceBoss.h"

USING(Engine)
CLogo::CLogo(LPDIRECT3DDEVICE9 pDevice)
	: CScene(pDevice), m_fBubbleTime(0.f), m_iCount(0)
{
}

CLogo::~CLogo()
{
}

HRESULT CLogo::Ready_Scene()
{
	Engine::Set_SoundScene(RESOURCE_STATIC);
	if (g_bStart)
	{

		FAILED_CHECK_RETURN(Ready_Environment_Layer(L"Environment"), E_FAIL);
		g_bStart = false;
	}
	else
	{
		FAILED_CHECK_RETURN(Ready_LoadingUI_Layer(L"Load"), E_FAIL);
	}


	m_pLoading = CLoading::Create(m_pGraphicDev, (LOADINGID)m_eNextScene);
	NULL_CHECK_RETURN(m_pLoading, E_FAIL);

	return S_OK;
}

_int CLogo::Update_Scene(const _float & fTimeDelta)
{
	D3DXMatrixLookAtLH(&m_matView, &_vec3(0.f, 0.f, -5.f), &_vec3(0.f, 0.f, 1.f), &_vec3(0.f, 1.f, 0.f));
	D3DXMatrixPerspectiveFovLH(&m_matProj, D3DXToRadian(60.f), _float(WINCX) / WINCY, 0.1f, 500.f);

	_int iExit = Engine::CScene::Update_Scene(fTimeDelta);

	if (true == m_pLoading->Get_Finish() && Engine::Key_Down(DIK_RETURN))
	{
		CTrigger::Clear_Trigger();

		Engine::CScene*		pScene = nullptr;

		switch (m_eNextScene)
		{
		case SCENE_LOGO:
			break;
		case SCENE_MENUSELECT:
			pScene = CMenuSelect::Create(m_pGraphicDev);

			break;
		case SCENE_CHAPTERSELECT:
			pScene = CMenu::Create(m_pGraphicDev);

			break;
		case SCENE_MENUPLAYERSELECT:
			pScene = CMenuPlayerSelect::Create(m_pGraphicDev);

			break;
		case SCENE_VF:
			pScene = CVacuumField::Create(m_pGraphicDev);

			break;
		case SCENE_VF2:
			pScene = CVacuumField2::Create(m_pGraphicDev);

			break;
		case SCENE_VF3:
			pScene = CVacuumField3::Create(m_pGraphicDev);

			break;
		case SCENE_VB:
			pScene = CVacuumBoss::Create(m_pGraphicDev);

			break;
		case SCENE_CF:
			pScene = CChessField::Create(m_pGraphicDev);

			break;
		case SCENE_CB:
			pScene = CChessBoss::Create(m_pGraphicDev);

			break;
		case SCENE_SF:
			pScene = CSpaceField::Create(m_pGraphicDev,m_vStartPos);

			break;
		case SCENE_SF2:
			pScene = CSpaceField2::Create(m_pGraphicDev);

			break;
		case SCENE_SF3:
			pScene = CSpaceField3::Create(m_pGraphicDev);

			break;
		case SCENE_SB:
			pScene = CSpaceBoss::Create(m_pGraphicDev);

			break;
		case SCENE_TF:
			pScene = CTestField::Create(m_pGraphicDev);

			break;
		case SCENE_ENDING:
			pScene = CEnding::Create(m_pGraphicDev);

			break;
		case SCENE_END:
			break;
		default:
			return iExit;
			break;
		}

		NULL_CHECK_RETURN(pScene, E_FAIL);
		pScene->Set_StartPos(m_vStartPos);
		FAILED_CHECK_RETURN(Engine::Set_Scene(pScene, m_pGraphicDev), E_FAIL);

		return iExit;
	}

	return iExit;
}

_int CLogo::LateUpdate_Scene(const _float & fTimeDelta)
{
	_int iExit = Engine::CScene::LateUpdate_Scene(fTimeDelta);

	return iExit;
}

void CLogo::Render_Scene()
{
	Engine::Render_Font(L"Font_Jinji", m_pLoading->Get_String().c_str(), &_vec2(20, 20), D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
}

HRESULT CLogo::Ready_Resources(LPDIRECT3DDEVICE9 & pGraphicDev, RESOURCEID eMax)
{


	//// Component
	//lstrcpy(m_szString, L"Component Loading...............");



	return S_OK;
}

HRESULT CLogo::Ready_Environment_Layer(const _tchar * pLayerTag)
{
	Engine::CLayer*			pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	// 오브젝트 추가
	Engine::CGameObject*		pGameObject = nullptr;

	// LogoBack
	pGameObject = CLogoBack::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"LogoBack", pGameObject), E_FAIL);

	m_mapLayer.emplace(pLayerTag, pLayer);

	return S_OK;
}

HRESULT CLogo::Ready_Component_Layer(const _tchar * pLayerTag)
{
	return S_OK;
}

HRESULT CLogo::Ready_LoadingUI_Layer(const _tchar * pLayerTag)
{
	Engine::CLayer*			pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	// 오브젝트 추가
	Engine::CGameObject*		pGameObject = nullptr;

	OBJINFO tObj;
	ZeroMemory(&tObj, sizeof(OBJINFO));
	tObj.eSceneID = RESOURCE_STATIC;

	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pLoadingText = pGameObject->Is<CImageUI>();
	m_pLoadingText->Set_Image(L"LoadingText");
	m_pLoadingText->Set_Size(_vec3{ 153.f, 31.f, 0.f });
	m_pLoadingText->Set_Pos(_vec3{ 1510.f, -910.f, 0.f });
	m_pLoadingText->Set_RenderID(RENDER_MENUUI);
	m_pLoadingText->Set_PassIndex(2);
	m_pLoadingText->Set_TextureIndex(0);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"LoadingText", pGameObject), E_FAIL);
	m_pLoadingText->AddRef();

	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pLoadingImage = pGameObject->Is<CImageUI>();
	m_pLoadingImage->Set_Image(L"LoadingImage");
	m_pLoadingImage->Set_Size(_vec3{ 128.f, 128.f, 0.f });
	m_pLoadingImage->Set_Pos(_vec3{ 1670.f, -850.f, 0.f });
	m_pLoadingImage->Set_RenderID(RENDER_UI);
	m_pLoadingImage->Set_PassIndex(3);
	m_pLoadingImage->Set_TextureIndex(0);
	m_pLoadingImage->Set_AniState(true, 8, 8, 1.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"LoadingImage", pGameObject), E_FAIL);
	m_pLoadingImage->AddRef();


	m_mapLayer.emplace(pLayerTag, pLayer);

	return S_OK;
}

CLogo * CLogo::Create(LPDIRECT3DDEVICE9 pGraphicDev, SCENEID eSceneID)
{
	CLogo*		pInstance = new CLogo(pGraphicDev);

	pInstance->m_eNextScene = eSceneID;
	if (FAILED(pInstance->Ready_Scene()))
		Safe_Release(pInstance);

	return pInstance;
}

Engine::CLogo* Engine::CLogo::Create(LPDIRECT3DDEVICE9 pGraphicDev, SCENEID eSceneID, const _vec3& vPos)
{
	CLogo*		pInstance = new CLogo(pGraphicDev);
	pInstance->m_eNextScene = eSceneID;
	if (FAILED(pInstance->Ready_Scene()))
		Safe_Release(pInstance);
	else
	{

		pInstance->Set_StartPos(vPos);
	}

	return pInstance;
}

void CLogo::Free(void)
{
	Safe_Release(m_pLoading);
	Safe_Release(m_pLoadingText);
	Safe_Release(m_pLoadingImage);

	Engine::CScene::Free();
}
