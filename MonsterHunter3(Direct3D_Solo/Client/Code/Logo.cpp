#include "stdafx.h"
#include "Logo.h"
#include "Stage.h"

#include "SoundMgr.h"
#include "LogoIru.h"
#include "Export_Fucntion.h"


CLogo::CLogo(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CScene(pGraphicDev)
	, m_pLoading(nullptr)
{

}

CLogo::~CLogo(void)
{

}

HRESULT CLogo::Ready_Scene(void)
{
	FAILED_CHECK_RETURN(Ready_Resources(m_pGraphicDev, RESOURCE_END), E_FAIL);

	FAILED_CHECK_RETURN(Ready_Environment_Layer(L"Environment"), E_FAIL);

	m_pLoading = CLoading::Create(m_pGraphicDev, LOADING_STAGE);
	NULL_CHECK_RETURN(m_pLoading, E_FAIL);

	CSoundMgr::Get_Instance()->PlayBGM(L"Mura.mp3");
	return S_OK;
}

Engine::_int CLogo::Update_Scene(const _float& fTimeDelta)
{


	D3DXMatrixLookAtLH(&m_matView, &_vec3(0.f, 0.f, -5.f), &_vec3(0.f, 0.f, 1.f), &_vec3(0.f, 1.f, 0.f));
	D3DXMatrixPerspectiveFovLH(&m_matProj, D3DXToRadian(60.f), _float(WINCX) / WINCY, 0.1f, 1000.f);

	_int iExit = Engine::CScene::Update_Scene(fTimeDelta);

	if (true == m_pLoading->Get_Finish())
	{
		CManagement*		pManagement = CManagement::GetInstance();

		if (nullptr == pManagement)
			return 0;

		CLogoIru*	pLogoIru = dynamic_cast<CLogoIru*>(pManagement->Get_GameObject(L"Environment", L"LogoIru"));

		pLogoIru->Set_Loading(true);

		if (GetAsyncKeyState(VK_RETURN) & 0x8000)
		{
			Engine::CScene*		pScene = nullptr;

			pScene = CStage::Create(m_pGraphicDev);
			NULL_CHECK_RETURN(pScene, E_FAIL);

			FAILED_CHECK_RETURN(Engine::Set_Scene(pScene), E_FAIL);

			return iExit;
		}	
	}

	return iExit;
}

void CLogo::Render_Scene(void)
{
	// DEBUG 용
	//Engine::Render_Font(L"Font_Jinji", m_pLoading->Get_String(), &_vec2(200, 530), D3DXCOLOR(0.f, 0.f, 0.f, 1.f));	//영상엔 이위치로

	Engine::Render_Font(L"Font_Jinji", m_pLoading->Get_String(), &_vec2(50, 230), D3DXCOLOR(0.f, 0.f, 0.f, 1.f));
}

void CLogo::Free(void)
{
	Safe_Release(m_pLoading);

	Engine::CScene::Free();
}


CLogo* CLogo::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CLogo*		pInstance = new CLogo(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
		Safe_Release(pInstance);

	return pInstance;
}


HRESULT CLogo::Ready_Resources(LPDIRECT3DDEVICE9& pGraphicDev, RESOURCEID eMax)
{
	Engine::Reserve_ContainerSize(eMax);

	FAILED_CHECK_RETURN(Engine::Ready_Buffer(pGraphicDev, RESOURCE_STATIC, L"Buffer_TriCol", Engine::BUFFER_TRICOL), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Buffer(pGraphicDev, RESOURCE_STATIC, L"Buffer_RcTex", Engine::BUFFER_RCTEX), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Texture(pGraphicDev, RESOURCE_STATIC, L"Texture_Logo", Engine::TEX_NORMAL, L"../../Resource/Texture/Logo/Logo1.png", 1), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(pGraphicDev, RESOURCE_STATIC, L"Texture_Logoback", Engine::TEX_NORMAL, L"../../Resource/Texture/Loading/LoadingBack.png", 1), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(pGraphicDev, RESOURCE_STATIC, L"Texture_Player", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Ma.jpg", 1), E_FAIL);



	Engine::CComponent*	pComponent = nullptr;
	
	// Transform
	pComponent = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_Transform", pComponent), E_FAIL);

	// Calculator
	pComponent = Engine::CCalculator::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_Calculator", pComponent), E_FAIL);

	// Optimization
	pComponent = Engine::COptimization::Create(m_pGraphicDev, true, 129, 129);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_Optimization", pComponent), E_FAIL);


	return S_OK;
}

HRESULT CLogo::Ready_Environment_Layer(const _tchar* pLayerTag)
{
	Engine::CLayer*			pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	// 오브젝트 추가
	Engine::CGameObject*		pGameObject = nullptr;
	
	// LogoBack
	pGameObject = CLogoBack::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"LogoBack", pGameObject), E_FAIL);

	//LogoIru
	pGameObject = CLogoIru::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"LogoIru", pGameObject), E_FAIL);

	//// TestPlayer
	//pGameObject = CTestPlayer::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"TestPlayer", pGameObject), E_FAIL);

	//// TestMonster
	//pGameObject = CTestMonster::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"TestMonster", pGameObject), E_FAIL);
	
	m_mapLayer.emplace(pLayerTag, pLayer);

	return S_OK;
}
