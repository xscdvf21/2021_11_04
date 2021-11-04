#include "stdafx.h"
#include "Logo.h"
#include "Ending.h"
#include "Export_Function.h"
#include "ImageUI.h"


CEnding::CEnding(LPDIRECT3DDEVICE9 pDevice)
	:Engine::CScene(pDevice)
{
}

CEnding::~CEnding()
{
	Safe_Release(m_pEnding);
}

HRESULT CEnding::Ready_Scene()
{
	Engine::Set_SoundScene(RESOURCE_ENDING);

	FAILED_CHECK_RETURN(Ready_GameObject_Layer(L"GameObject"), E_FAIL);
		StopAll();
		PlayBGM(L"EndBG.wav");


	return S_OK;
}

_int CEnding::Update_Scene(const _float & fTimeDelta)
{
	if (m_bStart)
	{
		m_bStart = false;
	}
	_int iExit = Engine::CScene::Update_Scene(fTimeDelta);

	m_pEnding->Set_AddPos(_vec3(0.f, fTimeDelta * 200.f, 0.f));

	return iExit;
}

_int CEnding::LateUpdate_Scene(const _float & fTimeDelta)
{
	if (m_bSceneChange)
		return 0;

	_int iExit = Engine::CScene::LateUpdate_Scene(fTimeDelta);

	return iExit;
}

void CEnding::Render_Scene()
{
}

HRESULT CEnding::Ready_GameObject_Layer(const _tchar * pLayerTag)
{
	Engine::CLayer*			pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	// 오브젝트 추가
	Engine::CGameObject*		pGameObject = nullptr;

	OBJINFO tObj;
	ZeroMemory(&tObj, sizeof(OBJINFO));
	tObj.eSceneID = RESOURCE_ENDING;

	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pEnding = pGameObject->Is<CImageUI>();
	m_pEnding->Set_Image(L"EndingCredit");
	m_pEnding->Set_Size(_vec3{ 1920.f, 8234.f, 0.f });
	m_pEnding->Set_Pos(_vec3{ 0.f, 0.f, 0.f });
	m_pEnding->Set_RenderID(RENDER_FINALUI);
	m_pEnding->Set_PassIndex(2);
	m_pEnding->Set_Color(_vec4(255.f, 255.f, 255.f, 255.f));
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Ending", pGameObject), E_FAIL);
	m_pEnding->AddRef();

	m_mapLayer.emplace(pLayerTag, pLayer);

	return S_OK;
}

CEnding * CEnding::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CEnding*		pInstance = new CEnding(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
		Safe_Release(pInstance);

	return pInstance;
}

void CEnding::Free(void)
{
	Engine::CScene::Free();
}