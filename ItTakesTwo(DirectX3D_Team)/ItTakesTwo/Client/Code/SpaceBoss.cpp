#include "stdafx.h"
#include "SpaceBoss.h"
#include "Export_Function.h"
#include "UI.h"
#include "May_Space.h"
#include "Cody_Space.h"
#include "CodyCamera.h"
#include "MayCamera.h"
#include "StaticObject.h"
#include "SpaceBoss.h"
#include "Map.h"
#include "SkyBox.h"
#include "SceneChangeSpace.h"
#include "DeadSpace.h"
#include "CollisionMgr.h"
#include "SpaceBaboon.h"
#include "BaboonGround.h"
#include "LaserMirror.h"
#include "BaboonBomb.h"
#include "WindLaser.h"
#include "BaboonMissile.h"


CSpaceBoss::CSpaceBoss(LPDIRECT3DDEVICE9 pDevice)
	: CScene(pDevice)
{
}

CSpaceBoss::~CSpaceBoss()
{
}

HRESULT CSpaceBoss::Ready_Scene()
{
	Engine::Set_SoundScene(RESOURCE_SF);

	Engine::Start_Split();
	g_bMenu = false;
	g_bSlideFinal = false;

	auto* pPhysics = Engine::Get_Physics();
	auto* pDispatcher = Engine::Get_Dispatcher();
	auto* pCudaMgr = Engine::Get_CudaMgr();
	CScene::Ready_PhysX(this, pPhysics, pDispatcher, pCudaMgr);


	if (!m_pScene)
		return E_FAIL;

	//pPlane->setGlobalPose(PxTransform(0, 0.f, 0));

	auto* pPlane = PxCreatePlane(*pPhysics, PxPlane(0, 1, 0, 1), *pPhysics->createMaterial(0.5f, 0.5f, 0.f));

	pPlane->setName((char*)pPlane);
	setupFiltering(Engine::Get_Allocator(), pPlane, FilterGroup::eGround, FilterGroup::eCody | FilterGroup::eMay);
	m_pScene->addActor(*pPlane);
	m_pScene->setGravity(PxVec3(0.0f, -20.f, 0.0f));
	FAILED_CHECK_RETURN(Ready_Environment_Layer(L"Environment"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_GameObject_Layer(L"GameObject"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Interact_Layer(L"Interact"), E_FAIL);

	//FAILED_CHECK_RETURN(Ready_LoadMap_Layer(L"LoadObject", L"../../Data/ITT_Test/Test.dat"), E_FAIL);

	Ready_LightInfo();

	StopAll();
	PlayBGM(L"SpaceBoss_BGM.wav", 1.f);

	return S_OK;
}

_int CSpaceBoss::Update_Scene(const _float & fTimeDelta)
{
	if (m_bStart)
	{
		Engine::StopNPlaySound(L"SB_Start.ogg", CSoundMgr::CHANNELID::Crusher, 0.3f);

		m_bStart = false;
	}

	m_pScene->simulate(fTimeDelta);

	m_pScene->fetchResults(true);
	_int iExit = Engine::CScene::Update_Scene(fTimeDelta);

	LightMove(fTimeDelta);

	if (Engine::Key_Down(DIK_Y))
	{
		m_bSceneChange = true;
	}
	SCENE_FADE(fTimeDelta, SCENE_ENDING);
	return iExit;
}

_int CSpaceBoss::LateUpdate_Scene(const _float & fTimeDelta)
{
	_int iExit = Engine::CScene::LateUpdate_Scene(fTimeDelta);

	return iExit;
}

void CSpaceBoss::Render_Scene()
{
	CCollisionMgr::GetInstance()->Clear_List();
}

void CSpaceBoss::onConstraintBreak(PxConstraintInfo* constraints, PxU32 count)
{

	return;
}

void CSpaceBoss::onWake(PxActor** actors, PxU32 count)
{
	OutputDebugString(L"깼음");
	return;
}

void CSpaceBoss::onSleep(PxActor** actors, PxU32 count)
{
	OutputDebugString(L"잠듬");

	return;
}

void CSpaceBoss::onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs)
{
	OutputDebugString(L"충돌있음\n");
	for (PxU32 i = 0; i < nbPairs; ++i)
	{
		if ((pairs[i].shapes[0]->getSimulationFilterData().word0 | pairs[i].shapes[1]->getSimulationFilterData().word0)
			== (FilterGroup::eWall | FilterGroup::eArrow))
		{
			if (pairs[i].shapes[0]->getSimulationFilterData().word0 & FilterGroup::eArrow)
			{
				auto* pCol = ToObj<CBaboonBomb>(pairs[i].shapes[0]->getName());
				pCol->Set_Dead();
			}
			else if (pairs[i].shapes[1]->getSimulationFilterData().word0 & FilterGroup::eArrow)
			{
				auto* pCol = ToObj<CBaboonBomb>(pairs[i].shapes[1]->getName());
				pCol->Set_Dead();

			}
			//			OutputDebugString(L"화살과 벽 충돌\n");

		}
		/*if ((!strcmp(pairHeader.actors[0]->getName(), "Cody") && !strcmp(pairHeader.actors[1]->getName(), "Wall")) ||
			(!strcmp(pairHeader.actors[1]->getName(), "Cody") && !strcmp(pairHeader.actors[0]->getName(), "Wall")))
		{
			Engine::Get_GameObject(L"GameObject", L"Cody")->Is<CCody>()->Set_Climbing(true);
		}*/

	}
}

void CSpaceBoss::onTrigger(PxTriggerPair* pairs, PxU32 count)
{
	for (PxU32 i = 0; i < count; ++i)
	{
		if (pairs[i].triggerShape->getSimulationFilterData().word0 == FilterGroup::eWall &&
			pairs[i].otherShape->getSimulationFilterData().word0 == FilterGroup::eCody)
		{
			pairs[i].triggerActor->getGlobalPose();
			CCody* pCody = (CCody*)Engine::Get_GameObject(L"GameObject", L"Cody");
		}
		auto Trigger = pairs[i];
		

		if (Trigger.otherShape->getSimulationFilterData().word0 &(FilterGroup::eCody | FilterGroup::eMay))
		{
			auto* pTrigger = ToTrigger(Trigger.triggerShape->getName());
			if (Trigger.status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
				pTrigger->Set_Active(true);
			else if (Trigger.status == PxPairFlag::eNOTIFY_TOUCH_LOST)
				pTrigger->Set_Active(false);


			if (!pTrigger)
				return;
			switch (pTrigger->Get_Type())
			{
			case TRIGGER_SCENE_CHANGE:
				Event_Scene_Change(&Trigger);
				break;
			case TRIGGER_SPACE_LASER_DEFAULT:
				Event_LaserMirror(&Trigger);
				break;
			case TRIGGER_SPACE_LASER_GRAVITY:
				Event_LaserMirror(&Trigger);
				break;
			case TRIGGER_SPACE_MISSILE_BOSS:
				// 테스트
				Event_Missile_Boss(&Trigger);
				break;
			case TRIGGER_SPACE_MISSILE_PLAYER:
				Event_Missile_Player(&Trigger);
				break;
			case TRIGGER_SPACE_MiSSILE_RIDE:
				Event_Missile_Ride(&Trigger);
				break;
			}
		}
		if (Trigger.otherShape->getSimulationFilterData().word0 &(FilterGroup::eMonster))
		{
			auto* pTrigger = ToTrigger(Trigger.triggerShape->getName());
			if (Trigger.status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
				pTrigger->Set_Active(true);
			else if (Trigger.status == PxPairFlag::eNOTIFY_TOUCH_LOST)
				pTrigger->Set_Active(false);


			if (!pTrigger)
				return;

			switch (pTrigger->Get_Type())
			{
			case TRIGGER_SPACE_MISSILE_PLAYER:
				Event_Missile_Player(&Trigger);
				break;
			}
		}
	}
}

void CSpaceBoss::onAdvance(const PxRigidBody*const* bodyBuffer, const PxTransform* poseBuffer, const PxU32 count)
{
	OutputDebugString(L"업데이트");
}


void CSpaceBoss::Event_Scene_Change(PxTriggerPair* pairs)
{
	//ToTrigger(pairs->triggerActor->getName())->Activate();
}

void CSpaceBoss::Event_LaserMirror(PxTriggerPair * pairs)
{
	CLaserMirror*		pLaserMirror = ToTrigger(pairs->triggerShape->getActor()->getName())->Is<CLaserMirror>();
	if (nullptr == pLaserMirror)
		return;

	if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
	{

		OutputDebugString(L"레이저 미러위에 있음\n");
		pLaserMirror->Set_MoveUp(true);
		pLaserMirror->Set_MoveDown(false);
	}
	if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_LOST)
	{

		OutputDebugString(L"레이저 미러에서 나감\n");
		pLaserMirror->Set_MoveUp(false);
		pLaserMirror->Set_MoveDown(true);
	}
}

void CSpaceBoss::Event_Missile_Boss(PxTriggerPair * pairs)
{
	auto* pTrigger = ToTrigger(pairs->triggerShape->getName());

	CGameObject* pMissile = pTrigger->Is<CGameObject>();


	if (pairs->otherShape->getSimulationFilterData().word0 &FilterGroup::eCody)
	{

		CCody_Space* pCody = ToObj<CCody_Space>(pairs->otherShape->getName());
		if (!pCody)
			return;

		if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			pCody->Set_PlayerDead();
			pMissile->Set_Dead();
		}
	}
	else if (pairs->otherShape->getSimulationFilterData().word0 &FilterGroup::eMay)
	{
		CMay_Space* pMay = ToObj<CMay_Space>(pairs->otherShape->getName());
		if (!pMay)
			return;

		if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{

			pMay->Set_PlayerDead();
			pMissile->Set_Dead();
		}
	}
}

void CSpaceBoss::Event_Missile_Player(PxTriggerPair * pairs)
{
	auto* pTrigger = ToTrigger(pairs->triggerShape->getName());
	CBaboonMissile* pMissile = pTrigger->Is<CBaboonMissile>();

	if (pairs->otherShape->getSimulationFilterData().word0 & FilterGroup::eMonster)
	{

		CSpaceBaboon* pBaboon = ToObj<CSpaceBaboon>(pairs->otherShape->getName());
		if (!pBaboon)
			return;

		if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			pBaboon->Set_Damage(-1);
			pMissile->Set_Dead();
		}
	}

}

void CSpaceBoss::Event_Missile_Ride(PxTriggerPair * pairs)
{
	auto* pTrigger = ToTrigger(pairs->triggerShape->getName());
	CBaboonMissile* pMissile = pTrigger->Is<CBaboonMissile>();

	if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
	{
		if (pairs->otherShape->getSimulationFilterData().word0 == FilterGroup::eCody
			&& pMissile->Get_CodyMissile())
		{
			CCody_Space* pCody = ToObj<CCody_Space>(pairs->otherShape->getName());
			pCody->Set_Ride(true, pMissile);
		}
		if (pairs->otherShape->getSimulationFilterData().word0 == FilterGroup::eMay
			&& !pMissile->Get_CodyMissile())
		{
			CMay_Space* pMay = ToObj<CMay_Space>(pairs->otherShape->getName());
			pMay->Set_Ride(true, pMissile);
		}
	}
	else if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_LOST)
	{
		if (pairs->otherShape->getSimulationFilterData().word0 == FilterGroup::eCody)
		{
			CCody_Space* pCody = ToObj<CCody_Space>(pairs->otherShape->getName());
			pCody->Set_Ride(false);
		}
		if (pairs->otherShape->getSimulationFilterData().word0 == FilterGroup::eMay)
		{
			CMay_Space* pMay = ToObj<CMay_Space>(pairs->otherShape->getName());
			pMay->Set_Ride(false);
		}
	}
}


void CSpaceBoss::Event_Bounce_Switch(PxTriggerPair* pairs)
{

}



void CSpaceBoss::Event_Float(PxTriggerPair* pairs)
{
	//CFan* pFan = ToTrigger(pairs->triggerShape->getActor()->getName())->Is<CFan>();
	//_float3 vPos = ToTrigger(pairs->triggerShape->getActor()->getName())->Get_Pos();
	//NULL_CHECK_RETURN(pFan, );

	//if (pairs->otherShape->getSimulationFilterData().word0 == FilterGroup::eCody)
	//{
	//	CCody* pCody = ToObj<CCody>(pairs->otherShape->getName());
	//	NULL_CHECK_RETURN(pCody, );

	//	if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
	//		pCody->Set_Float(true, pFan->Get_Up(), pFan, vPos, 20.f);

	//	else if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_LOST)
	//		pCody->Set_Float(false, pFan->Get_Up(), nullptr, vPos);
	//}
	//else if (pairs->otherShape->getSimulationFilterData().word0 == FilterGroup::eMay)
	//{
	//	CMay* pMay = (CMay*)Engine::Get_GameObject(L"GameObject", L"May");
	//	NULL_CHECK_RETURN(pMay, );

	//	if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
	//		pMay->Set_Float(true, pFan->Get_Up(), pFan, vPos, 20.f);

	//	else if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_LOST)
	//		pMay->Set_Float(false, pFan->Get_Up(), nullptr, vPos);
	//}
}

void CSpaceBoss::Event_Dead(PxTriggerPair * pairs)
{
	if (pairs->otherShape->getSimulationFilterData().word0 == FilterGroup::eCody &&
		pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
	{
		CCody* pCody = ToObj<CCody>(pairs->otherShape->getName());
		NULL_CHECK_RETURN(pCody, );

		pCody->Set_PlayerDead();
	}
	else if (pairs->otherShape->getSimulationFilterData().word0 == FilterGroup::eMay &&
		pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
	{
		CMay* pMay = ToObj<CMay>(pairs->otherShape->getName());
		NULL_CHECK_RETURN(pMay, );
		pMay->Set_PlayerDead();
	}
}

HRESULT CSpaceBoss::Ready_Environment_Layer(const _tchar * pLayerTag)
{
	Engine::CLayer*			pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	// 오브젝트 추가
	Engine::CGameObject*		pGameObject = nullptr;

	OBJINFO tInfo;
	_int iX = 0, iY = 0, iZ = 0, iScale = 0;

	pGameObject = CSkyBox::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SpaceSkybox", pGameObject, this), E_FAIL);

	m_mapLayer.emplace(pLayerTag, pLayer);


	return S_OK;
}
//#스테이지	스페이스 보스
HRESULT CSpaceBoss::Ready_GameObject_Layer(const _tchar * pLayerTag)
{
	Engine::CLayer*			pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	OBJINFO tObj;
	ZeroMemory(&tObj, sizeof(OBJINFO));
	tObj.eSceneID = RESOURCE_SB;
	// 오브젝트 추가
	Engine::CGameObject*		pGameObject = nullptr;
	tObj.vScale = { 0.01f, 0.01f ,0.01f };

	// CodyCamera
	pGameObject = CCodyCamera::Create(m_pGraphicDev, &_vec3(0.f, 10.f, -5.f), &_vec3(0.f, 0.f, 1.f), &_vec3(0.f, 1.f, 0.f), D3DXToRadian(60.f), _float(WINCX) / WINCY, 0.1f, 1000.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CodyCamera", pGameObject, this), E_FAIL);
	

	// MayCamera
	pGameObject = CMayCamera::Create(m_pGraphicDev, &_vec3(0.f, 10.f, -5.f), &_vec3(0.f, 0.f, 1.f), &_vec3(0.f, 1.f, 0.f), D3DXToRadian(60.f), _float(WINCX) / WINCY, 0.1f, 1000.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MayCamera", pGameObject, this), E_FAIL);
	//780.886, 10794.9, 2694.33

	tObj.vPos = { 0.f,2.f,40.f };


	pGameObject = CCody_Space::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Cody", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eCody, FilterGroup::eGround | FilterGroup::eWall);
	setupQueryFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eGround, FilterGroup::eGround);

	tObj.vPos = { 2.f,2.f,40.f };

	pGameObject = CMay_Space::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"May", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMay, FilterGroup::eGround | FilterGroup::eWall);
	setupQueryFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eGround, FilterGroup::eGround);
	//tObj.vPos = { 28.08f,12.81f,-1.49f };
	//pGameObject = CPowerButton::Create(m_pGraphicDev, &tObj);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"PowerButton", pGameObject, this), E_FAIL);
	//setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eButton, FilterGroup::eCody | FilterGroup::eMay);


	//가운데 윈드 레이저
	tObj.vPos = { 0.f, -4.5f, 0.f };
	tObj.vScale = { 0.01f, 0.01f, 0.01f };
	pGameObject = CWindLaser::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"WindLaser", pGameObject, this), E_FAIL);


	//보스
	tObj.vPos = { 5.f,10.f,0.f };

	tObj.vScale = { 0.01f, 0.01f, 0.01f };
	pGameObject = CSpaceBaboon::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SpaceBoss", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eGround | FilterGroup::eWall);

	//보스 필드 레이저 반사 높이 올라가는거.
	tObj.vPos = { 17.4f, 0.5f, 42.f };
	tObj.vScale = { 0.01f, 0.01f, 0.01f };
	pGameObject = CLaserMirror::Create(m_pGraphicDev, &tObj, Mirror_Gravity);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"LaserMirror", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eInteractItem, FilterGroup::eDefaultTrigger);

	//보스 필드 레이저 반사
	tObj.vPos = { -36.1f, 0.5f, -15.f };
	tObj.vScale = { 0.01f, 0.01f, 0.01f };
	pGameObject = CLaserMirror::Create(m_pGraphicDev, &tObj, Mirror_Default);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"LaserMirror2", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eInteractItem, FilterGroup::eDefaultTrigger);

	//보스 필드 레이저 반사
	tObj.vPos = { 15.f, 0.5f, -36.1f };
	tObj.vScale = { 0.01f, 0.01f, 0.01f };
	pGameObject = CLaserMirror::Create(m_pGraphicDev, &tObj, Mirror_Default);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"LaserMirror3", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eInteractItem, FilterGroup::eDefaultTrigger);



	//보스 필드 땅.
	tObj.vPos = { 0.f, 0.f, 0.f };
	tObj.vScale = { 0.01f, 0.01f, 0.01f };
	pGameObject = CBaboonGround::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"BaboonGround", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eGround|FilterGroup::eWall, FilterGroup::eArrow);
	setupQueryFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eGround, FilterGroup::eGround);

	lstrcpy(tObj.tagMesh, L"SpaceBossField");
	tObj.vScale = _vec3{ 0.01f,0.01f,0.01f };
	tObj.vAngle = { 0, 0.f, 0.f };

	tObj.eMeshType = 1;
	tObj.eRenderType = RENDER_NONALPHA;
	tObj.vPos = { 0.f,0.f,0.f };
	lstrcpy(tObj.tagMesh, L"SpaceBossFieldWall");
	pGameObject = CMap::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Field2", pGameObject, this), E_FAIL);
	setupQueryFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eGround, FilterGroup::eGround);

	lstrcpy(tObj.tagMesh, L"SpaceBoss_Gravity");
	pGameObject = CMap::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Field4", pGameObject, this), E_FAIL);
	setupQueryFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eGround, FilterGroup::eGround);

	lstrcpy(tObj.tagMesh, L"SpaceBoss_Gravity_InCorner");
	pGameObject = CMap::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Field5", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eGravityFloorIN, FilterGroup::eCody | FilterGroup::eMay);
	setupQueryFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eGround, FilterGroup::eGround);


	tObj.eMeshType = 1;
	tObj.eRenderType = RENDER_ALPHA;
	tObj.vPos = { 0.f,0.f,0.f };

	lstrcpy(tObj.tagMesh, L"SpaceBossFieldGlassWall");
	pGameObject = CMap::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Field3", pGameObject, this), E_FAIL);
	setupQueryFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eGround, FilterGroup::eGround);



	m_mapLayer.emplace(pLayerTag, pLayer);


	return S_OK;
}



HRESULT CSpaceBoss::Ready_LightInfo(void)
{
	D3DLIGHT9				tLightInfo;
	ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));
	tLightInfo.Type = D3DLIGHT_POINT;
	tLightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Ambient = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.f);


	tLightInfo.Diffuse = D3DXCOLOR(0.2f, 0.4f, 1.f, 1.f);
	tLightInfo.Position = _vec3(0.f, 10.f, 0.f);
	tLightInfo.Range = 30.f;
	m_pLight[0] = Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 0);

	tLightInfo.Diffuse = D3DXCOLOR(0.2f, 0.4f, 1.f, 1.f);
	tLightInfo.Position = _vec3(15.15f, 10.f, 36.58f);
	tLightInfo.Range = 20.f;
	m_pLight[1] = Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 1);

	tLightInfo.Diffuse = D3DXCOLOR(0.2f, 0.4f, 1.f, 1.f);
	tLightInfo.Position = _vec3(-15.30f, 10.f, 36.95f);
	tLightInfo.Range = 20.f;
	m_pLight[2] = Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 2);

	tLightInfo.Diffuse = D3DXCOLOR(0.2f, 0.4f, 1.f, 1.f);
	tLightInfo.Position = _vec3(-36.58f, 10.f, 15.15f);
	tLightInfo.Range = 20.f;
	m_pLight[3] = Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 3);

	tLightInfo.Diffuse = D3DXCOLOR(0.2f, 0.4f, 1.f, 1.f);
	tLightInfo.Position = _vec3(-36.95f, 10.f, -15.30f);
	tLightInfo.Range = 20.f;
	m_pLight[4] = Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 4);

	tLightInfo.Diffuse = D3DXCOLOR(0.2f, 0.4f, 1.f, 1.f);
	tLightInfo.Position = _vec3(-15.15f, 10.f, -36.58f);
	tLightInfo.Range = 20.f;
	m_pLight[5] = Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 5);

	tLightInfo.Diffuse = D3DXCOLOR(0.2f, 0.4f, 1.f, 1.f);
	tLightInfo.Position = _vec3(15.30f, 10.f, -36.95f);
	tLightInfo.Range = 20.f;
	m_pLight[6] = Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 6);

	tLightInfo.Diffuse = D3DXCOLOR(0.2f, 0.4f, 1.f, 1.f);
	tLightInfo.Position = _vec3(36.58f, 10.f, -15.15f);
	tLightInfo.Range = 20.f;
	m_pLight[7] = Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 7);

	tLightInfo.Diffuse = D3DXCOLOR(0.2f, 0.4f, 1.f, 1.f);
	tLightInfo.Position = _vec3(36.95f, 10.f, 15.30f);
	tLightInfo.Range = 20.f;
	m_pLight[8] = Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 8);

	tLightInfo.Diffuse = D3DXCOLOR(0.2f, 0.4f, 1.f, 1.f);
	tLightInfo.Position = _vec3(15.15f, 10.f, 36.58f);
	tLightInfo.Range = 20.f;
	m_pLight[9] = Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 9);

	tLightInfo.Diffuse = D3DXCOLOR(0.9f * 1.5f, 0.5f * 1.5f, 0.7f * 1.5f, 1.f);
	tLightInfo.Position = _vec3(60.f, 5.f, 0.f);
	tLightInfo.Range = 30.f;
	m_pLight[10] = Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 10);

	tLightInfo.Diffuse = D3DXCOLOR(0.9f * 1.5f, 0.5f * 1.5f, 0.7f * 1.5f, 1.f);
	tLightInfo.Position = _vec3(-60.f, 5.f, 0.f);
	tLightInfo.Range = 30.f;
	m_pLight[11] = Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 11);

	tLightInfo.Diffuse = D3DXCOLOR(0.9f * 1.5f, 0.5f * 1.5f, 0.7f * 1.5f, 1.f);
	tLightInfo.Position = _vec3(0.f, 5.f, 60.f);
	tLightInfo.Range = 30.f;
	m_pLight[12] = Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 12);

	tLightInfo.Diffuse = D3DXCOLOR(0.9f * 1.5f, 0.5f * 1.5f, 0.7f * 1.5f, 1.f);
	tLightInfo.Position = _vec3(0.f, 5.f, -60.f);
	tLightInfo.Range = 30.f;
	m_pLight[13] = Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 13);

	tLightInfo.Diffuse = D3DXCOLOR(0.9f * 1.5f, 0.5f * 1.5f, 0.7f * 1.5f, 1.f);
	tLightInfo.Position = _vec3(42.42f, 5.f, 42.42f);
	tLightInfo.Range = 30.f;
	m_pLight[14] = Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 14);

	tLightInfo.Diffuse = D3DXCOLOR(0.9f * 1.5f, 0.5f * 1.5f, 0.7f * 1.5f, 1.f);
	tLightInfo.Position = _vec3(42.42f, 5.f, -42.42f);
	tLightInfo.Range = 30.f;
	m_pLight[15] = Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 15);

	tLightInfo.Diffuse = D3DXCOLOR(0.9f * 1.5f, 0.5f * 1.5f, 0.7f * 1.5f, 1.f);
	tLightInfo.Position = _vec3(-42.42f, 5.f, 42.42f);
	tLightInfo.Range = 30.f;
	m_pLight[16] = Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 16);

	tLightInfo.Diffuse = D3DXCOLOR(0.9f * 1.5f, 0.5f * 1.5f, 0.7f * 1.5f, 1.f);
	tLightInfo.Position = _vec3(-42.42f, 5.f, -42.42f);
	tLightInfo.Range = 30.f;
	m_pLight[17] = Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 17);

	tLightInfo.Diffuse = D3DXCOLOR(0.1f, 0.1f, 0.1f, 0.1f);
	tLightInfo.Position = _vec3(0.f, 50.f, 0.f);
	tLightInfo.Range = 130.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 18);

	tLightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Position = _vec3(0.f, 200.f, 0.f);
	tLightInfo.Range = 180.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 19);

	return S_OK;
}

void CSpaceBoss::LightMove(const _float fTimeDelta)
{
	if (!m_bLightMove)
		return;

	_vec3 vMovePos = _vec3(0.f, 2.f, 0.f) * fTimeDelta;
	m_fAddPos += 2.f * fTimeDelta;

	for (_int i = 0; i < 18; ++i)
		m_pLight[i]->Set_LightAddPos(vMovePos);

	if (m_fAddPos > 25.f)
	{
		m_fAddPos = 0.f;
		m_bLightMove = false;
	}
}

CSpaceBoss * CSpaceBoss::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSpaceBoss*		pInstance = new CSpaceBoss(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
		Safe_Release(pInstance);

	return pInstance;
}

void CSpaceBoss::Free(void)
{
	// 삭제
	CCollisionMgr::DestroyInstance();

	for (_uint i = 0; i < m_vecObject.size(); ++i)
		Safe_Delete_Array(m_vecObject[i]);

	m_vecObject.clear();
	m_vecObject.shrink_to_fit();
	Engine::Delete_AllResource(RESOURCE_SB);
	Engine::Release_Sound(RESOURCE_SF);
	Engine::CScene::Free();
}


HRESULT CSpaceBoss::Ready_Interact_Layer(const _tchar* szLayer)
{
	Engine::CLayer*			pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	OBJINFO tObj;
	ZeroMemory(&tObj, sizeof(OBJINFO));
	tObj.eSceneID = RESOURCE_SB;
	// 오브젝트 추가
	tObj.vPos = { 0.f,0.f ,0.f };
	tObj.vScale = { 0.01f,0.01f ,0.01f };
	Engine::CGameObject*		pGameObject = nullptr;
	Engine::CGameObject*		pButton = nullptr;

	//pGameObject = CMiniVacuum::Create(m_pGraphicDev, DIRRIGHT, &tObj);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Minivacuum", pGameObject, this), E_FAIL);

	//pButton = CPowerButton::Create(m_pGraphicDev, &tObj);
	//NULL_CHECK_RETURN(pButton, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"PowerButton1", pButton, this), E_FAIL);
	//pButton->Set_InteractObj(pGameObject);


	m_mapLayer.emplace(szLayer, pLayer);

	return S_OK;
}

void CSpaceBoss::SCENE_FADE(const _float & fTimeDelta, SCENEID eID)
{
	if (!m_bSceneChange)
		return;

	m_fSceneChange += fTimeDelta * 100.f;

	CCody_Space* pCody = (CCody_Space*)Engine::Get_GameObject(L"GameObject", L"Cody");
	CMay_Space* pMay = (CMay_Space*)Engine::Get_GameObject(L"GameObject", L"May");
	pCody->Get_FadeUI()->Set_Color(0.f, 0.f, 0.f, m_fSceneChange);
	pMay->Get_FadeUI()->Set_Color(0.f, 0.f, 0.f, m_fSceneChange);

	if (m_fSceneChange > 255.f)
	{
		m_fSceneChange = 255.f;
		m_bSceneChange = false;
		CHANGE_SCENE(m_pGraphicDev, eID, );
		return;
	}
}