#include "stdafx.h"
#include "ChessBoss.h"
#include "Export_Function.h"
#include "UI.h"
#include "Cody.h"
#include "May.h"
#include "CodyCamera.h"
#include "MayCamera.h"
#include "StaticObject.h"
#include "MiniVacuumHub.h"
#include "MiniVacuumHead.h"
#include "MiniVacuumBody.h"
#include "May_Castle.h"
#include "Cody_Wizard.h"

#include "Knight.h"
#include "MorningStar.h"
#include "Sheild.h"
#include "Sheilder.h"
#include "Arbalist.h"
#include "CrossBow.h"
#include "ToySword.h"
#include "Mage.h"
#include "Staff.h"
#include "ChessKing.h"
#include "ChessQueen.h"
#include "ChessKnight.h"
#include "ChessPawn.h"
#include "ChessTile.h"
#include "CollisionMgr.h"
#include "FireSwordEffect.h"
#include "Escape_Shuttle.h"
#include "ChessBossDoor.h"
#include "Orb.h"
#include "FireSwirl.h"
#include "Torch.h"
#include "Map.h"


CChessBoss::CChessBoss(LPDIRECT3DDEVICE9 pDevice)
	: CScene(pDevice)
{
}

CChessBoss::~CChessBoss()
{
}

HRESULT CChessBoss::Ready_Scene()
{
	Engine::Set_SoundScene(RESOURCE_CF);

	Engine::End_Split();

	CChessTile::GetInstance()->Ready_ChessTile();
	Engine::Ready_Font(m_pGraphicDev, L"Font_Jinji", L"궁서", 20, 20, FW_BOLD);
	g_bMenu = false;
	g_bSlideFinal = false;

	auto* pPhysics = Engine::Get_Physics();
	auto* pDispatcher = Engine::Get_Dispatcher();
	auto* pCudaMgr = Engine::Get_CudaMgr();
	CScene::Ready_PhysX(this, pPhysics, pDispatcher, pCudaMgr);

	if (!m_pScene)
		return E_FAIL;

	//pPlane->setGlobalPose(PxTransform(0, 0.f, 0));

	auto* pPlane = PxCreatePlane(*pPhysics, PxPlane(0, 1, 0, 0), *pPhysics->createMaterial(0.5f, 0.5f, 0.f));

	m_pScene->addActor(*pPlane);
	m_pScene->setGravity(PxVec3(0.0f, -20.f, 0.0f));
	FAILED_CHECK_RETURN(Ready_Environment_Layer(L"Environment"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_GameObject_Layer(L"GameObject"), E_FAIL);
	//FAILED_CHECK_RETURN(Ready_LoadMap_Layer(L"LoadObject", L"../../Data/ITT_Test/Test.dat"), E_FAIL);
		Ready_LightInfo();

	Create_BossUI(L"CB_UI");

	Engine::StopAll();
	Engine::PlayBGM(L"ChessBoss_BGM.wav", 1.f);


	m_iBossMaxHp = CChessTile::GetInstance()->Get_BossHp();

	return S_OK;
}

_int CChessBoss::Update_Scene(const _float & fTimeDelta)
{
	if (Engine::Key_Down(DIK_Y))
	{

		CHANGE_SCENE_POS(m_pGraphicDev, SCENE_SF,_vec3(0.f, 3.f, -3.5f), OBJ_NOEVENT);

		return SCENE_CHANGE;
	}

	if (m_bStart)
	{
		StopSound(CSoundMgr::CHANNELID::Crusher);
		PlaySoundW(L"CB_Start.ogg", CSoundMgr::CHANNELID::CrusherHitPoint, 0.3f);
		m_bStart = false;
	}


	_int iExit = Engine::CScene::Update_Scene(fTimeDelta);
	if (iExit == SCENE_CHANGE)
		return iExit;
	m_pScene->simulate(fTimeDelta);

	m_pScene->fetchResults(true);

	Check_BossHp();

	return iExit;
}

_int CChessBoss::LateUpdate_Scene(const _float & fTimeDelta)
{
	_int iExit = Engine::CScene::LateUpdate_Scene(fTimeDelta);

	return iExit;
}

void CChessBoss::Render_Scene()
{
	CCollisionMgr::GetInstance()->Collision_Player_Monster();
	CCollisionMgr::GetInstance()->Collision_Enermy_Player();
	CCollisionMgr::GetInstance()->Collision_EnermyWeapon_Player();
	CCollisionMgr::GetInstance()->Collision_Bullet_Monster();
	CCollisionMgr::GetInstance()->Collision_SplashBullet_Monster();
	CCollisionMgr::GetInstance()->Clear_List();

	_uint iBossHp = CChessTile::GetInstance()->Get_BossHp();

#ifdef _DEBUG
	TCHAR szbuf[MAX_PATH] = L"";
	wsprintf(szbuf, L"King And Queen : %d", iBossHp);
	Engine::Render_Font(L"Font_Jinji", szbuf, &_vec2(20, 20), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
#endif
}

void CChessBoss::onConstraintBreak(PxConstraintInfo* constraints, PxU32 count)
{
	return;
}

void CChessBoss::onWake(PxActor** actors, PxU32 count)
{
	OutputDebugString(L"깼음");
	return;
}

void CChessBoss::onSleep(PxActor** actors, PxU32 count)
{
	OutputDebugString(L"잠듬");

	return;
}

void CChessBoss::onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs)
{
	OutputDebugString(L"충돌있음\n");
	for (PxU32 i = 0; i < nbPairs; ++i)
	{
		if (pairHeader.actors[0]->getType() == PxActorType::eRIGID_STATIC &&pairHeader.actors[1]->getType() == PxActorType::eRIGID_DYNAMIC ||
			pairHeader.actors[0]->getType() == PxActorType::eRIGID_DYNAMIC &&pairHeader.actors[1]->getType() == PxActorType::eRIGID_STATIC)
		{
		}
	}
}

void CChessBoss::onTrigger(PxTriggerPair* pairs, PxU32 count)
{
	for (PxU32 i = 0; i < count; ++i)
	{
		auto Trigger = pairs[i];
		auto* pTrigger = ToTrigger(Trigger.triggerShape->getName());

		if (Trigger.status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
			pTrigger->Set_Active(true);
		else if (Trigger.status == PxPairFlag::eNOTIFY_TOUCH_LOST)
			pTrigger->Set_Active(false);

		if (!pTrigger)
			return;
		switch (pTrigger->Get_Type())
		{
		case TRIGGER_ATTACK:
			Event_ATTACK(&Trigger);
			break;
		case TRIGGER_CHESS_SHUTTLE:
			Event_Shuttle(&Trigger);
			break;
		case TRIGGER_CHESS_SHUTTLE_DOOR:
			Event_ShuttleDoor(&Trigger);
			break;
		default:
			break;
		}
	}
}

void CChessBoss::Event_ATTACK(PxTriggerPair * pairs)
{
	//auto* pTrigger = ToTrigger(pairs->triggerShape->getName());
	if (pairs->otherShape->getSimulationFilterData().word0 & FilterGroup::eMonster)
	{
		if (pairs->status &PxPairFlag::eNOTIFY_TOUCH_FOUND)
			ToObj<CEnermy>(pairs->otherShape->getName())->Set_TickDamage(true, 2, false);
		else if(pairs->status &PxPairFlag::eNOTIFY_TOUCH_LOST)
			ToObj<CEnermy>(pairs->otherShape->getName())->Set_TickOff(false);
	}
}
void CChessBoss::Event_Shuttle(PxTriggerPair * pairs)
{
	auto* pTrigger = ToTrigger(pairs->triggerShape->getName());

	CEscape_Shuttle*		pShuttle = pTrigger->Is<CEscape_Shuttle>();

	if (pairs->otherShape->getSimulationFilterData().word0 & FilterGroup::eMay)
	{
		if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{

			pShuttle->Set_ShuttleStack(1);
			OutputDebugString(L"메이가 우주선에 탔음");

		}
		else if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_LOST)
		{
			pShuttle->Set_ShuttleStack(-1);
			OutputDebugString(L"코디가 우주선에 내림");

		}
	}
	else if (pairs->otherShape->getSimulationFilterData().word0 & FilterGroup::eCody)
	{
		if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			pShuttle->Set_ShuttleStack(1);
			OutputDebugString(L"코디가 우주선에 탔음");

		}
		else if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_LOST)
		{
			pShuttle->Set_ShuttleStack(-1);
			OutputDebugString(L"코디가 우주선에 내림");
	
		}
	}

}
void CChessBoss::Event_ShuttleDoor(PxTriggerPair * pairs)
{
	if (pairs->otherShape->getSimulationFilterData().word0 & (FilterGroup::eMay | FilterGroup::eCody))
	{
		if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			CEscape_Shuttle*	pShuttle = (CEscape_Shuttle*)Get_GameObject(L"GameObject", L"ChessEscape_Shuttle");
			pShuttle->Set_CutScene();
			OutputDebugString(L"우주선 컷신 시작");
		}
	}


}
void CChessBoss::onAdvance(const PxRigidBody*const* bodyBuffer, const PxTransform* poseBuffer, const PxU32 count)
{
	OutputDebugString(L"업데이트");
}

HRESULT CChessBoss::Ready_Environment_Layer(const _tchar * pLayerTag)
{
	Engine::CLayer*			pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	// 오브젝트 추가
	Engine::CGameObject*		pGameObject = nullptr;

	OBJINFO tInfo;
	_int iX = 0, iY = 0, iZ = 0, iScale = 0;


	m_mapLayer.emplace(pLayerTag, pLayer);
	return S_OK;
}
//#스테이지	체스 보스
HRESULT CChessBoss::Ready_GameObject_Layer(const _tchar * pLayerTag)
{
	Engine::CLayer*			pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	OBJINFO tObj;
	ZeroMemory(&tObj, sizeof(OBJINFO));
	tObj.eSceneID = RESOURCE_CB;
	// 오브젝트 추가
	Engine::CGameObject*		pGameObject = nullptr;

	// MainCamera

	pGameObject = CMainCamera::Create(m_pGraphicDev, &_vec3(0.f, 10.f, -5.f), &_vec3(0.f, 0.f, 1.f), &_vec3(0.f, 1.f, 0.f), D3DXToRadian(60.f), _float(WINCX) / WINCY, 0.1f, 1000.f, &_vec3(5.0f, 15.0f, -5.0f), 17.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MainCamera", pGameObject), E_FAIL);


	tObj.vPos = _vec3(0.f, 0.f, 0.f);


	pGameObject = CCodyWizard::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Cody", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eCody, FilterGroup::eWall | FilterGroup::eMay );


	pGameObject = CMayCastle::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"May", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMay, FilterGroup::eWall | FilterGroup::eCody) ;



	pGameObject = CToySword::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ToySword", pGameObject), E_FAIL);

	pGameObject = COrb::Create(m_pGraphicDev, "RightAttach", &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"IceOrb_Left", pGameObject), E_FAIL);

	pGameObject = COrb::Create(m_pGraphicDev, "LeftAttach", &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"IceOrb_Right", pGameObject), E_FAIL);

	pGameObject = CChessKing::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Chess_King", pGameObject, this), E_FAIL);

	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eDefaultTrigger);

	pGameObject = CChessQueen::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Chess_Queen", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eDefaultTrigger);

	pGameObject = CSkyBox::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SpaceSkybox", pGameObject, this), E_FAIL);



	tObj.vScale = { 0.011f, 0.011f, 0.011f };
	tObj.vAngle = { 0.f, 180.f, 0.f };
	tObj.vPos = { -9.1f , 0.f, 0.f };
	pGameObject = CChessBossDoor::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ChessBossDoor", pGameObject, this), E_FAIL);


	tObj.vScale = { 0.01f, 0.01f, 0.01f };
	tObj.vAngle = { 0.f, 0.f, 0.f };
	tObj.vPos = _vec3(-30.f, -1.5f, 0.f);
	//tObj.vPos = _vec3(-5.f, -1.5f, 0.f);
	pGameObject = CEscape_Shuttle::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ChessEscape_Shuttle", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eInteractItem, FilterGroup::eDefaultTrigger);


	//pGameObject = CFireSwirl::Create(m_pGraphicDev, &tObj);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"FireSwirl", pGameObject), E_FAIL);

	//tObj.vPos = _vec3(20.f, 2.f, 20.f);
	//pGameObject = CTorch::Create(m_pGraphicDev, &tObj);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Torch", pGameObject), E_FAIL);


	//pGameObject = CChessKnight::Create(m_pGraphicDev, 7);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Chess_Knight", pGameObject), E_FAIL);

	//pGameObject = CChessKnight::Create(m_pGraphicDev, 15);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Chess_Knight2", pGameObject), E_FAIL);

	//pGameObject = CChessKnight::Create(m_pGraphicDev, 23);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Chess_Knight3", pGameObject), E_FAIL);

	//pGameObject = CChessKnight::Create(m_pGraphicDev, 31);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Chess_Knight4", pGameObject), E_FAIL);

	//pGameObject = CChessKnight::Create(m_pGraphicDev, 39);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Chess_Knight5", pGameObject), E_FAIL);

	//pGameObject = CChessKnight::Create(m_pGraphicDev, 47);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Chess_Knight6", pGameObject), E_FAIL);

	//pGameObject = CChessPawn::Create(m_pGraphicDev, 58);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Chess_Pawn", pGameObject), E_FAIL);



	lstrcpy(tObj.tagMesh, L"ChessBossField");
	tObj.vScale = _vec3{ 0.007f,0.007f,0.007f };
	tObj.eMeshType = 1;
	tObj.eRenderType = RENDER_NONALPHA;
	tObj.vPos = { 0.f,0.f,0.f };
	tObj.vAngle = { 0.f, D3DXToRadian(180.f), 0.f };
	pGameObject = CMap::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Field", pGameObject, this), E_FAIL);

	m_mapLayer.emplace(pLayerTag, pLayer);


	return S_OK;
}

HRESULT CChessBoss::Ready_LoadMap_Layer(const _tchar * pLayerTag, const _tchar * pFilePath)
{
	Engine::CLayer*			pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject*	pGameObject = nullptr;

	HANDLE hFile = CreateFile(pFilePath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
		return S_OK;

	DWORD dwByte = 0;
	OBJINFO tObj{};
	int iIndex = 0;

	while (true)
	{
		ReadFile(hFile, &tObj, sizeof(OBJINFO), &dwByte, nullptr);

		if (0 == dwByte)
			break;

		wstring GameObjectName = std::to_wstring(iIndex);
		TCHAR* tagSearch = new TCHAR[MAX_PATH];
		lstrcpy(tagSearch, GameObjectName.c_str());

		pGameObject = CStaticObject::Create(m_pGraphicDev, &tObj);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagSearch, pGameObject, this), E_FAIL);
		m_vecObject.emplace_back(tagSearch);


		++iIndex;
	}
	CloseHandle(hFile);
	m_mapLayer.emplace(pLayerTag, pLayer);


	return S_OK;
}

HRESULT CChessBoss::Ready_LightInfo(void)
{
	D3DLIGHT9				tLightInfo;
	ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));

	tLightInfo.Type = D3DLIGHT_POINT;
	tLightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Ambient = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.f);


	tLightInfo.Diffuse = D3DXCOLOR(1.f , 0.8f, 0.5f, 1.f);
	tLightInfo.Position = _vec3(0.f, 6.f, 0.f);
	tLightInfo.Range = 40.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 0);

	tLightInfo.Diffuse = D3DXCOLOR(0.25f, 0.2f, 0.1f, 0.5f);
	tLightInfo.Position = _vec3(-18.45f, 3.29f, 7.47f) * 0.7f;
	tLightInfo.Range = 3.5f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 1);

	tLightInfo.Diffuse = D3DXCOLOR(0.25f, 0.2f, 0.1f, 0.5f);
	tLightInfo.Position = _vec3(-18.45f, 3.29f, -7.47f) * 0.7f;
	tLightInfo.Range = 3.5f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 2);


	tLightInfo.Diffuse = D3DXCOLOR(0.5f, 0.4f, 0.2f, 0.5f);
	tLightInfo.Position = _vec3(-18.05f, 3.29f, 19.8742f) * 0.7f;
	tLightInfo.Range = 3.5f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 3);

	tLightInfo.Diffuse = D3DXCOLOR(0.5f, 0.4f, 0.2f, 0.5f);
	tLightInfo.Position = _vec3(-18.05f, 3.29f, -19.8742f) * 0.7f;

	tLightInfo.Range = 3.5f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 4);

	tLightInfo.Diffuse = D3DXCOLOR(0.25f, 0.2f, 0.1f, 0.5f);
	tLightInfo.Position = _vec3(-10.19f, 3.29f, -20.80f) * 0.7f;
	tLightInfo.Range = 3.5f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 5);

	tLightInfo.Diffuse = D3DXCOLOR(0.25f, 0.2f, 0.1f, 0.5f);
	tLightInfo.Position = _vec3(-5.01f, 3.29f, -20.80f) * 0.7f;
	tLightInfo.Range = 3.5f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 6);

	tLightInfo.Diffuse = D3DXCOLOR(0.25f, 0.2f, 0.1f, 0.5f);
	tLightInfo.Position = _vec3(2.39f, 3.29f, -20.80f) * 0.7f;
	tLightInfo.Range = 3.5f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 7);

	tLightInfo.Diffuse = D3DXCOLOR(0.25f, 0.2f, 0.1f, 0.5f);
	tLightInfo.Position = _vec3(7.58f, 3.29f, -20.80f) * 0.7f;
	tLightInfo.Range = 3.5f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 8);

	tLightInfo.Diffuse = D3DXCOLOR(0.25f, 0.2f, 0.1f, 0.5f);
	tLightInfo.Position = _vec3(-10.19f, 3.29f, 20.80f) * 0.7f;
	tLightInfo.Range = 3.5f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 9);

	tLightInfo.Diffuse = D3DXCOLOR(0.25f, 0.2f, 0.1f, 0.5f);
	tLightInfo.Position = _vec3(-5.01f, 3.29f, 20.80f) * 0.7f;
	tLightInfo.Range = 3.5f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 10);

	tLightInfo.Diffuse = D3DXCOLOR(0.25f, 0.2f, 0.1f, 0.5f);
	tLightInfo.Position = _vec3(2.39f, 3.29f, 20.80f) * 0.7f;
	tLightInfo.Range = 3.5f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 11);

	tLightInfo.Diffuse = D3DXCOLOR(0.25f, 0.2f, 0.1f, 0.5f);
	tLightInfo.Position = _vec3(7.58f, 3.29f, 20.80f) * 0.7f;
	tLightInfo.Range = 3.5f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 12);

	tLightInfo.Diffuse = D3DXCOLOR(0.25f, 0.2f, 0.1f, 0.5f);
	tLightInfo.Position = _vec3(16.39f, 3.29f, 19.77f) * 0.7f;
	tLightInfo.Range = 3.5f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 13);

	tLightInfo.Diffuse = D3DXCOLOR(0.25f, 0.2f, 0.1f, 0.5f);
	tLightInfo.Position = _vec3(16.58f, 3.29f, 19.77f) * 0.7f;
	tLightInfo.Range = 3.5f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 14);

	tLightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Position = _vec3(-41.41f, 6.f, 0.f) * 0.7f;
	tLightInfo.Range = 30.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 15);

	return S_OK;
}

void CChessBoss::Create_BossUI(const _tchar * pLayerTag)
{
	Engine::CLayer*			pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, );

	// 오브젝트 추가
	Engine::CGameObject*		pGameObject = nullptr;
	OBJINFO tObj;
	ZeroMemory(&tObj, sizeof(OBJINFO));
	tObj.eSceneID = RESOURCE_STATIC;

	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pBossName = pGameObject->Is<CImageUI>();
	m_pBossName->Set_Image(L"BossName");
	m_pBossName->Set_Size(_vec3{ 194.f, 32.f, 0.f });
	m_pBossName->Set_Pos(_vec3{ 180.f, -65.f, 0.f });
	m_pBossName->Set_RenderID(RENDER_UI);
	m_pBossName->Set_PassIndex(2);
	m_pBossName->Set_TextureIndex(1);
	NULL_CHECK_RETURN(pGameObject, );
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"BossName", pGameObject), );
	m_pBossName->AddRef();

	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pBossHp = pGameObject->Is<CImageUI>();
	m_pBossHp->Set_Image(L"HP");
	m_pBossHp->Set_Size(_vec3{ 1600.f, 50.f, 0.f });
	m_pBossHp->Set_Pos(_vec3{ 160.f, -90.f, 0.f });
	m_pBossHp->Set_RenderID(RENDER_MENUUI);
	m_pBossHp->Set_PassIndex(4);
	m_pBossHp->Set_TextureIndex(0);
	m_pBossHp->Set_Amount(1.f);
	NULL_CHECK_RETURN(pGameObject, );
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"BossHP", pGameObject), );
	m_pBossHp->AddRef();

	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pBossHpbg = pGameObject->Is<CImageUI>();
	m_pBossHpbg->Set_Image(L"HP");
	m_pBossHpbg->Set_Size(_vec3{ 1600.f, 50.f, 0.f });
	m_pBossHpbg->Set_Pos(_vec3{ 160.f, -90.f, 0.f });
	m_pBossHpbg->Set_RenderID(RENDER_UI);
	m_pBossHpbg->Set_PassIndex(5);
	m_pBossHpbg->Set_TextureIndex(0);
	NULL_CHECK_RETURN(pGameObject, );
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"BossHPBG", pGameObject), );
	m_pBossHpbg->AddRef();

	m_mapLayer.emplace(pLayerTag, pLayer);
}

void CChessBoss::Check_BossHp()
{
	_int iBossHp = CChessTile::GetInstance()->Get_BossHp();
	if (iBossHp <= 0)
	{
		m_pBossHp->Set_Visible(false);
		m_pBossHpbg->Set_Visible(false);
		m_pBossName->Set_Visible(false);
	}

	m_pBossHp->Set_Amount((_float)iBossHp / (_float)m_iBossMaxHp);
}

CChessBoss * CChessBoss::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CChessBoss*		pInstance = new CChessBoss(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
		Safe_Release(pInstance);

	return pInstance;
}

void CChessBoss::Free(void)
{
	// 삭제
	CCollisionMgr::DestroyInstance();

	Safe_Release(m_pBossHp);
	Safe_Release(m_pBossHpbg);
	Safe_Release(m_pBossName);

	for (_uint i = 0; i < m_vecObject.size(); ++i)
		Safe_Delete_Array(m_vecObject[i]);

	m_vecObject.clear();
	m_vecObject.shrink_to_fit();

	Engine::Release_Sound(RESOURCE_CF);

	Engine::CScene::Free();
}
