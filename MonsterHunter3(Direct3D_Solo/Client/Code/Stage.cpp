#include "stdafx.h"
#include "Stage.h"
#include "NaviMgr.h"
#include "SoundMgr.h"
#include "Boss.h"
#include "Aura.h"
#include "Export_Fucntion.h"
#include "BulletBust.h"
#include "Inventory.h"
CStage::CStage(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CScene(pGraphicDev)
{

}

CStage::~CStage(void)
{

}

HRESULT CStage::Ready_Scene(void)
{
	
	Load_Navi();

	FAILED_CHECK_RETURN(Ready_LightInfo(), E_FAIL);

	FAILED_CHECK_RETURN(Ready_GameLogic_Layer(L"GameLogic"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Environment_Layer(L"Environment"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_UI_Layer(L"UI"), E_FAIL);

	m_pGraphicDev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pGraphicDev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	CSoundMgr::Get_Instance()->StopAll();
	CSoundMgr::Get_Instance()->PlayBGM(L"Map2.mp3");

	return S_OK;
}

Engine::_int CStage::Update_Scene(const _float& fTimeDelta)
{
	CManagement*			pManagement = CManagement::GetInstance();

	if (false == m_bBossSoundOne)
	{
		CBoss*			pBoss = dynamic_cast<CBoss*>(pManagement->Get_GameObject(L"GameLogic", L"Boss"));
		m_bBossSound = pBoss->Get_PlayerCheck();
		if (m_bBossSound)
		{
			m_bBossSoundOne = true;
		}
	}

	if (true == m_bBossSound)
	{
		CSoundMgr::Get_Instance()->StopAll();
		CSoundMgr::Get_Instance()->PlayBGM(L"Boss.mp3");

		m_bBossSound = false;
	}



	_int iExit = Engine::CScene::Update_Scene(fTimeDelta);

	return iExit;
}

void CStage::Render_Scene(void)
{
	
}

void CStage::Free(void)
{
	Engine::CScene::Free();
}


CStage* CStage::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStage*		pInstance = new CStage(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
		Safe_Release(pInstance);

	return pInstance;
}


HRESULT CStage::Ready_Environment_Layer(const _tchar* pLayerTag)
{
	CManagement*		pManagement = CManagement::GetInstance();
	CTransform*			pPlayerTransform = dynamic_cast<CTransform*>(pManagement->Get_Component(L"GameLogic", L"Player", L"Com_Transform", COMPONENTID::ID_DYNAMIC));

	Engine::CLayer*			pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	// 오브젝트 추가
	Engine::CGameObject*		pGameObject = nullptr;

	// SkyBox
	pGameObject = CSkyBox::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SkyBox", pGameObject), E_FAIL);

	// dynamicCamera
	pGameObject = CDynamicCamera::Create(m_pGraphicDev, &_vec3(0.f, 2.f, -5.f), &_vec3(0.f, 0.f, 1.f), &_vec3(0.f, 1.f, 0.f), D3DXToRadian(60.f), _float(WINCX) / WINCY, 0.1f, 1000.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DynamicCamera", pGameObject), E_FAIL);

	m_mapLayer.emplace(pLayerTag, pLayer);

	return S_OK;
}

HRESULT CStage::Ready_GameLogic_Layer(const _tchar* pLayerTag)
{
	Engine::CLayer*			pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	// 오브젝트 추가
	Engine::CGameObject*		pGameObject = nullptr;


	pGameObject = CHP_Potion::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"HP_Potion", pGameObject), E_FAIL);

	pGameObject = CSP_Potion::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SP_Potion", pGameObject), E_FAIL);


	pGameObject = CIru::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Iru", pGameObject), E_FAIL);

	//// Player
	pGameObject = CPlayer::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Player", pGameObject), E_FAIL);

	////PlayerWeapon
	pGameObject = CWeapon::Create(m_pGraphicDev, 0 );
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Player_Weapon", pGameObject), E_FAIL);

	//Boss
	pGameObject = CBoss::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Boss", pGameObject), E_FAIL);

	pGameObject = CBosstail::Create(m_pGraphicDev, 0);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Boss_tail", pGameObject), E_FAIL);	
	

	//pGameObject = CBossBullet::Create(m_pGraphicDev, _vec3(0.f , 0.f, 5.f));
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Boss_Bullet", pGameObject), E_FAIL);

	//// sword
	//pGameObject = CSword::Create(m_pGraphicDev, 0);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Sword", pGameObject), E_FAIL);

	//Stone
	pGameObject = CStone::Create(m_pGraphicDev, 1);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Stone", pGameObject), E_FAIL);

	pGameObject = CInventory::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Inventory", pGameObject), E_FAIL);


	//// Effect
	//for (_uint i = 0; i < 100; ++i)
	//{
	//	pGameObject = CEffect::Create(m_pGraphicDev);
	//	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Effect", pGameObject), E_FAIL);
	//}

	// Tree
	//pGameObject = CTree::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Tree", pGameObject), E_FAIL);


	m_mapLayer.emplace(pLayerTag, pLayer);
	return S_OK;
}

HRESULT CStage::Ready_UI_Layer(const _tchar* pLayerTag)
{
	Engine::CLayer*			pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	// 오브젝트 추가
	Engine::CGameObject*		pGameObject = nullptr;



	// UI
	//pGameObject = CUI::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"UI", pGameObject), E_FAIL);

	pGameObject = CHPUI::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CHPUI", pGameObject), E_FAIL);

	//pGameObject = CBlood::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Blood", pGameObject), E_FAIL);

	m_mapLayer.emplace(pLayerTag, pLayer);
	return S_OK;
}

HRESULT CStage::Ready_LightInfo(void)
{
	D3DLIGHT9				tLightInfo;
	ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));

	tLightInfo.Type = D3DLIGHT_DIRECTIONAL;
	tLightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Direction = _vec3(1.f, -1.f, 1.f);

	FAILED_CHECK_RETURN(Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 0), E_FAIL);

	return S_OK;
}

HRESULT CStage::Load_Navi()
{

	HANDLE hFile = CreateFile(L"../../Data/Navi/NewMapObjNavi.dat", GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	DWORD dwByte = 0;
	_uint iIdx = 0;

	vector<CCell*> vecCell[NAVI_END];
	for (auto& iter : vecCell)
	{
		iter.reserve(100);
	}

	while (true)
	{
		NAVILOAD tLOAD;
		ZeroMemory(&tLOAD, sizeof(NAVILOAD));

		ReadFile(hFile, &tLOAD, sizeof(NAVILOAD), &dwByte, nullptr);

		if (0 == dwByte)
			break;

		CCell* pCell = CCell::Create(m_pGraphicDev, vecCell[tLOAD.iType].size(), &tLOAD.vPoint[CCell::POINT_A], &tLOAD.vPoint[CCell::POINT_B], &tLOAD.vPoint[CCell::POINT_C]);
		pCell->Set_Type(tLOAD.iType);

		vecCell[tLOAD.iType].emplace_back(pCell);
	}

	for (_uint i = 0; i < NAVI_END; ++i)
	{
		CNaviMesh* pNaviMesh = CNaviMesh::Create(m_pGraphicDev, vecCell[i].size(), vecCell[i]);
		CNaviMgr::GetInstance()->Add_NaviMesh(RESOURCE_STAGE, i, pNaviMesh);

		vecCell[i].shrink_to_fit();
	}


	CloseHandle(hFile);
	return S_OK;

}
