#include "stdafx.h"
#include "VacuumBoss.h"
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
#include "Vacuum.h"
#include "Orb.h"

#include "VacuumFieldPart.h"
#include "VacuumFieldPart3.h"
#include "VacuumFieldPart2.h"
#include "VacuumFieldPart4.h"
#include "VacuumFieldPart5.h"
#include "Map.h"
#include "FuseNPC.h"

#include <PxSimulationEventCallback.h>
#include "MiniVacuum.h"
#include "VacuumBomb.h"
#include "DeadSpace.h"
#include "Cody_Space.h"

CVacuumBoss::CVacuumBoss(LPDIRECT3DDEVICE9 pDevice)
	: CScene(pDevice)
{
}

CVacuumBoss::~CVacuumBoss()
{
}



HRESULT CVacuumBoss::Ready_Scene()
{
	Engine::Set_SoundScene(RESOURCE_VF);

	Engine::Start_Split();

	CChessTile::GetInstance()->Ready_ChessTile();

	g_bMenu = false;
	g_bSlideFinal = false;


	auto* pPhysics = Engine::Get_Physics();
	auto* pDispatcher = Engine::Get_Dispatcher();
	auto* pCudaMgr = Engine::Get_CudaMgr();
	CScene::Ready_PhysX(this, pPhysics, pDispatcher, pCudaMgr);

	if (!m_pScene)
		return E_FAIL;

	//pPlane->setGlobalPose(PxTransform(0, 0.f, 0));

	//auto* pPlane = PxCreatePlane(*pPhysics, PxPlane(0, 1, 0, 0), *pPhysics->createMaterial(0.5f, 0.5f, 0.f));

	//pPlane->setName((char*)pPlane);
	//m_pScene->addActor(*pPlane);
	//pPlane->setGlobalPose(PxTransform(0.f, -5.f, 0.f, PxQuat(D3DXToRadian(90), { 0.f,0.f,1.f })));
	//setupFiltering(Engine::Get_Allocator(), pPlane, FilterGroup::eGround, FilterGroup::ePlayer);

	m_pScene->setGravity(PxVec3(0.0f, -20.f, 0.0f));
	FAILED_CHECK_RETURN(Ready_Environment_Layer(L"Environment"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_GameObject_Layer(L"GameObject"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Interact_Layer(L"Interact"), E_FAIL);

	//FAILED_CHECK_RETURN(Ready_LoadMap_Layer(L"LoadObject", L"../../Data/ITT_Test/Test.dat"), E_FAIL);
	Ready_LightInfo();


	StopAll();
	PlayBGM(L"VacuumBoss_BGM.wav", 1.f);
	return S_OK;
}

_int CVacuumBoss::Update_Scene(const _float & fTimeDelta)
{
	if (m_bStart)
	{
		StopNPlaySound(L"VB_Start.ogg", CSoundMgr::CHANNELID::May_Castle, 0.3f);
		m_bStart = false;
	}

	_int iExit = Engine::CScene::Update_Scene(fTimeDelta);
	m_pScene->simulate(fTimeDelta);

	m_pScene->fetchResults(true);
	if (Engine::Key_Down(DIK_Y))
	{
		m_bSceneChange = true;
	}
	SCENE_FADE(fTimeDelta, SCENE_CF);
	return iExit;
}

_int CVacuumBoss::LateUpdate_Scene(const _float & fTimeDelta)
{
	_int iExit = Engine::CScene::LateUpdate_Scene(fTimeDelta);

	return iExit;
}

void CVacuumBoss::Render_Scene()
{
	CCollisionMgr::GetInstance()->Collision_Player_Monster();
	CCollisionMgr::GetInstance()->Collision_Enermy_Player();
	CCollisionMgr::GetInstance()->Collision_Bullet_Monster();
	CCollisionMgr::GetInstance()->Clear_List();
}

void CVacuumBoss::onConstraintBreak(PxConstraintInfo* constraints, PxU32 count)
{

	return;
}

// 움직이면 Wake
// 플래그가 켜져있는 Shape가 움직이면 호출
// Shape가 있고 여러개의 Shape를 가지고있는 actor가 있다.
// 객체->액터, 충돌체->쉐이프 라고 생각하면된다.?

void CVacuumBoss::onWake(PxActor** actors, PxU32 count)
{
	OutputDebugString(L"깼음");
	return;
}

// 움직이지 않으면 Sleep
// 플래그가 켜져있는 Shape가 멈추면 호출
void CVacuumBoss::onSleep(PxActor** actors, PxU32 count)
{
	OutputDebugString(L"잠듬");

	return;
}

// 필터
void CVacuumBoss::onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs)
{
	OutputDebugString(L"충돌있음\n");
	for (PxU32 i = 0; i < nbPairs; ++i)
	{
		//if ((pairs[i].shapes[0]->getSimulationFilterData().word0 & (FilterGroup::eCody|FilterGroup::eMay))&&)
		//{
		//	Engine::Get_GameObject(L"GameObject", L"Cody")->Is<CCody>()->Set_JumpEnd();
		//}
	}
}

// 다른 쉐이프랑 충돌했을때
// Count가 갯수, pairs 배열

void CVacuumBoss::onTrigger(PxTriggerPair* pairs, PxU32 count)
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
		if (Trigger.otherShape->getSimulationFilterData().word0 &(FilterGroup::eCody | FilterGroup::eMay | FilterGroup::eInteractItem))
		{
			auto* pTrigger = ToTrigger(Trigger.triggerShape->getName());

			if (!pTrigger)
				return;
			switch (pTrigger->Get_Type())
			{
			case TRIGGER_VACUUM_ENTER:
				Event_Vacuum_Enter(&Trigger);
				break;
			case TRIGGER_VACUUM_RIDE:
				Event_Ride(&Trigger);
				break;
			case TRIGGER_VACUUMBOSS_STONE:
				Event_Stone(&Trigger);
				break;
			case TRIGGER_VACUUMBOSS_VACUUM:
				Event_Bomb(&Trigger);
				break;
			case TRIGGER_VACUUM_BOSS_CODY:
				Event_Ending_Cody(&Trigger);
				break;
			case TRIGGER_VACUUM_BOSS_MAY:
				Event_Ending_May(&Trigger);
				break;
			case TRIGGER_DEADSPACE:
				Event_Dead(&Trigger);
				break;
			default:
				break;
			}
		}


	}
}

void CVacuumBoss::Event_Vacuum_Enter(PxTriggerPair * pairs)
{
	//if (pairs->otherShape->getSimulationFilterData().word0 == FilterGroup::eCody)
	//{
	//	CVacuumFieldPart5* pVaccum = ToTrigger(pairs->triggerShape->getActor()->getName())->Is<CVacuumFieldPart5>();
	//	if (pVaccum->Is_Active())
	//		ToObj<CCody>(pairs->otherActor->getName())->Set_Slide(false, false, pVaccum->Get_IOPath());
	//}
	//else if (pairs->otherShape->getSimulationFilterData().word0 == FilterGroup::eMay)
	//{
	//	CVacuumFieldPart5* pVaccum = ToTrigger(pairs->triggerShape->getActor()->getName())->Is<CVacuumFieldPart5>();
	//	if (pVaccum->Is_Active())
	//		ToObj<CMay>(pairs->otherActor->getName())->Set_Slide(false, false, pVaccum->Get_IOPath());
	//}
	if (pairs->otherShape->getSimulationFilterData().word0 == FilterGroup::eInteractItem)
	{
		//CVacuumFieldPart5* pVaccum = ToTrigger(pairs->triggerShape->getActor()->getName())->Is<CVacuumFieldPart5>();
		//if (pVaccum->Is_Active())
		//{
		//	vector<vector<_vec3>*> vecPath;
		//	vecPath.emplace_back( Engine::Get_GameObject(L"Interact", L"VacuumFieldHead_2")->Is<CVacuumFieldPart5>()->Get_IOPath());
		//	vecPath.emplace_back(Engine::Get_GameObject(L"Interact", L"VacuumFieldHead_1")->Is<CVacuumFieldPart5>()->Get_IOPath());


		//	ToObj<CVacuumBomb>(pairs->otherActor->getName())->Set_Slide(false, false, vecPath);
		//}
		CTrigger* pTrigger = ToTrigger(pairs->triggerShape->getActor()->getName());
		void* pArg = pTrigger->Activate((void*)pairs->otherShape->getName());
		if (!pArg)
			return;
		vector<vector<_vec3>*> vecPath;
		vecPath = *static_cast<vector<vector<_vec3>*>*>(pArg);
		vecPath.emplace_back(Engine::Get_GameObject(L"Interact", L"VacuumFieldHead_1")->Is<CVacuumFieldPart5>()->Get_IOPath());
		ToObj<CVacuumBomb>(pairs->otherActor->getName())->Set_Slide(false, pTrigger->Get_FloatValue(0), pTrigger->Get_FloatValue(1), vecPath, pTrigger->Get_FloatValue(2));
	}
}

void CVacuumBoss::Event_Ride(PxTriggerPair * pairs)
{
	CVacuumFieldPart5* pMiniVaccum = ToTrigger(pairs->triggerShape->getActor()->getName())->Is<CVacuumFieldPart5>();
	NULL_CHECK_RETURN(pMiniVaccum, );
	if (!pMiniVaccum->Is_Active())
		return;
	if (pairs->otherShape->getSimulationFilterData().word0 == FilterGroup::eCody)
	{
		CCody* pCody = ToObj<CCody>(pairs->otherShape->getName());

		NULL_CHECK_RETURN(pCody, );

		if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			pCody->Set_RideTag("Base", L"Com_MeshHead", L"Com_TransformHead", _vec3(90.0f, 180.0f, 0.0f));
			pCody->Set_PickUp(true, CCody::HOSE, pMiniVaccum);
		}
		else if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_LOST)
		{
			pCody->Set_PickUp(false, CCody::HOSE, nullptr);
		}
	}
	else if (pairs->otherShape->getSimulationFilterData().word0 == FilterGroup::eMay)
	{
		CMay* pMay = (CMay*)Engine::Get_GameObject(L"GameObject", L"May");
		/*CMay* pMay = ToObj<CMay>(pairs->otherShape->getName());*/

		NULL_CHECK_RETURN(pMay, );

		if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			pMay->Set_RideTag("Base", L"Com_MeshHead", L"Com_TransformHead", _vec3(90.0f, 180.0f, 0.0f));
			pMay->Set_PickUp(true, CMay::HOSE, pMiniVaccum);
		}

		else if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_LOST)
			pMay->Set_PickUp(false, CMay::HOSE, nullptr);
	}
}

void CVacuumBoss::Event_Stone(PxTriggerPair * pairs)
{
	auto* pTrigger = ToTrigger(pairs->triggerShape->getName());

	CGameObject* pStone = pTrigger->Is<CGameObject>();


	if (pairs->otherShape->getSimulationFilterData().word0 &FilterGroup::eCody)
	{

		CCody* pCody = ToObj<CCody>(pairs->otherShape->getName());
		if (!pCody)
			return;

		if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			pCody->Set_Damage(-1);
			pStone->Set_Dead();
		}
	}
	else if (pairs->otherShape->getSimulationFilterData().word0 &FilterGroup::eMay)
	{
		CMay* pMay = ToObj<CMay>(pairs->otherShape->getName());
		if (!pMay)
			return;

		if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{

			pMay->Set_Damage(-1);
			pStone->Set_Dead();
		}
	}
}

void CVacuumBoss::Event_Bomb(PxTriggerPair * pairs)
{
	auto* pTrigger = ToTrigger(pairs->triggerShape->getName());

	if (pairs->otherShape->getSimulationFilterData().word0 & FilterGroup::eInteractItem)
	{
		if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{

			ToTrigger(pairs->triggerShape->getName())->Activate((void*)pairs->otherShape->getName());
			ToObj<CVacuumBomb>(pairs->otherActor->getName())->Set_Dead();
		}
	}
}

void CVacuumBoss::Event_Dead(PxTriggerPair* pairs)
{
	if (pairs->otherShape->getSimulationFilterData().word0 == FilterGroup::eCody
		&& pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
	{
		CCody* pCody = ToObj<CCody>(pairs->otherShape->getName());
		NULL_CHECK_RETURN(pCody, );

		pCody->Set_PlayerDead();
	}
	else if (pairs->otherShape->getSimulationFilterData().word0 == FilterGroup::eMay
		&& pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
	{
		CMay* pMay = ToObj<CMay>(pairs->otherShape->getName());
		NULL_CHECK_RETURN(pMay, );

		pMay->Set_PlayerDead();
	}
}

void CVacuumBoss::Event_Ending_Cody(PxTriggerPair * pairs)
{
	auto* pTrigger = ToTrigger(pairs->triggerShape->getName());
	CVacuum*	pVacuum = pTrigger->Is<CVacuum>();

	if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
	{
		if (pairs->otherShape->getSimulationFilterData().word0 == FilterGroup::eCody)
		{
			CCody* pCody = ToObj<CCody>(pairs->otherShape->getName());
			pCody->Set_RideTag("RightHandBend", L"Com_Mesh", L"Com_Transform", _vec3(90, 180.0f, 180.0f));
			pCody->Set_PickUp(true, CCody::HOSE, pVacuum);

		}
	}
	else if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_LOST)
	{
		if (pairs->otherShape->getSimulationFilterData().word0 == FilterGroup::eCody)
		{
			CCody* pCody = ToObj<CCody>(pairs->otherShape->getName());
			pCody->Set_PickUp(false, CCody::HOSE, nullptr);
		}
	}
	//	Set_RideTag("LeftHandBend", L"Com_Mesh", L"Com_Transform", _vec3(-90.0f, 0.0f, 0.0f));
	//	m_pPickUpTarget = Engine::Get_GameObject(L"GameObject", L"VacuumBoss");
}

void CVacuumBoss::Event_Ending_May(PxTriggerPair * pairs)
{
	auto* pTrigger = ToTrigger(pairs->triggerShape->getName());
	CVacuum*	pVacuum = pTrigger->Is<CVacuum>();

	if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
	{
		if (pairs->otherShape->getSimulationFilterData().word0 == FilterGroup::eMay)
		{
			CMay* pMay = ToObj<CMay>(pairs->otherShape->getName());
			pMay->Set_RideTag("LeftHandBend", L"Com_Mesh", L"Com_Transform", _vec3(-90, 0.0f, 180.0f));
			pMay->Set_PickUp(true, CCody::HOSE, pVacuum);
		}
	}
	else if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_LOST)
	{
		if (pairs->otherShape->getSimulationFilterData().word0 == FilterGroup::eMay)
		{
			CMay* pMay = ToObj<CMay>(pairs->otherShape->getName());
			pMay->Set_PickUp(false, CCody::HOSE, nullptr);
		}
	}
}

void CVacuumBoss::onAdvance(const PxRigidBody*const* bodyBuffer, const PxTransform* poseBuffer, const PxU32 count)
{
	OutputDebugString(L"업데이트");
}


HRESULT CVacuumBoss::Ready_Environment_Layer(const _tchar * pLayerTag)
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
//#스테이지	청소기 보스
HRESULT CVacuumBoss::Ready_GameObject_Layer(const _tchar * pLayerTag)
{
	Engine::CLayer*			pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	OBJINFO tObj;
	ZeroMemory(&tObj, sizeof(OBJINFO));
	tObj.eSceneID = RESOURCE_VB;
	// 오브젝트 추가
	Engine::CGameObject*		pGameObject = nullptr;

	//// MainCamera
	//pGameObject = CMainCamera::Create(m_pGraphicDev, &_vec3(0.f, 10.f, -5.f), &_vec3(0.f, 0.f, 1.f), &_vec3(0.f, 1.f, 0.f), D3DXToRadian(60.f), _float(WINCX) / WINCY, 0.1f, 1000.f);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MainCamera", pGameObject), E_FAIL);

	// CodyCamera
	pGameObject = CCodyCamera::Create(m_pGraphicDev, &_vec3(0.f, 10.f, -5.f), &_vec3(0.f, 0.f, 1.f), &_vec3(0.f, 1.f, 0.f), D3DXToRadian(60.f), _float(WINCX) / WINCY, 0.1f, 1000.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CodyCamera", pGameObject), E_FAIL);

	// MayCamera
	pGameObject = CMayCamera::Create(m_pGraphicDev, &_vec3(0.f, 10.f, -5.f), &_vec3(0.f, 0.f, 1.f), &_vec3(0.f, 1.f, 0.f), D3DXToRadian(60.f), _float(WINCX) / WINCY, 0.1f, 1000.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MayCamera", pGameObject), E_FAIL);

	OBJECTINFO Objinfo; //_vec3(100.f, 1.f, 10.f)

	tObj.vPos = _vec3(0.f, 5.f, 0.f);
	Objinfo.iHp = 1;

	pGameObject = CCody::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Cody", pGameObject, this), E_FAIL);


	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eCody, FilterGroup::eGround | FilterGroup::eWall);


	pGameObject = CMay::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"May", pGameObject, this), E_FAIL);

	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMay, FilterGroup::eGround | FilterGroup::eWall);


#pragma region 스테이지 붐 공격용 파츠들
	//////////////////////////////////////////////////////////////////////////
	tObj.vPos = _vec3(-23.8f, 0.f, 9.25f);
	pGameObject = CVacuumFieldPart::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"VacuumFieldPart_1", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eGround, FilterGroup::eCody | FilterGroup::eMay);

	tObj.vPos = _vec3(-23.8f, 0.f, -9.25f);
	pGameObject = CVacuumFieldPart::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"VacuumFieldPart_2", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eGround, FilterGroup::eCody | FilterGroup::eMay);

	tObj.vPos = _vec3(-23.8f, -2.5f, 0.05f);
	pGameObject = CVacuumFieldPart2::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"VacuumFieldPart2_1", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eGround, FilterGroup::eCody | FilterGroup::eMay);


	tObj.vPos = _vec3(-23.8f, 0.f, 3.8f);
	pGameObject = CVacuumFieldPart3::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"VacuumFieldPart3_1", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eGround, FilterGroup::eCody | FilterGroup::eMay);


	tObj.vPos = _vec3(-23.8f, 0.f, -3.8f);
	pGameObject = CVacuumFieldPart3::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"VacuumFieldPart3_2", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eGround, FilterGroup::eCody | FilterGroup::eMay);



	tObj.vScale = { 0.01f, 0.01f, 0.01f };
	tObj.vPos = { -13.8f, 0.05f, -0.6f };

	pGameObject = CVacuumFieldPart4::Create(m_pGraphicDev, false, 1, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"VacuumFieldPart4_1", pGameObject, this), E_FAIL);


	tObj.vScale = { 0.01f, 0.01f, 0.01f };
	tObj.vPos = { -13.8f, 0.05f, 0.6f };

	pGameObject = CVacuumFieldPart4::Create(m_pGraphicDev, true, 2, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"VacuumFieldPart4_2", pGameObject, this), E_FAIL);

	tObj.vScale = { 0.011f, 0.01f, 0.01f };
	tObj.vPos = { -23.8f, 0.05f, -0.6f };

	pGameObject = CVacuumFieldPart4::Create(m_pGraphicDev, false, 3, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"VacuumFieldPart4_3", pGameObject, this), E_FAIL);

	tObj.vScale = { 0.011f, 0.01f, 0.01f };
	tObj.vPos = { -23.8f, 0.05f, 0.6f };

	pGameObject = CVacuumFieldPart4::Create(m_pGraphicDev, true, 4, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"VacuumFieldPart4_4", pGameObject, this), E_FAIL);
	//////////////////////////////////////////////////////////////////////////
#pragma endregion

	tObj.vTriggerPos = { 0.f, -10.0f, 0.f };
	tObj.vTriggerScale = _vec3{ 200.f ,1.0f, 200.f };
	pGameObject = CDeadSpace::Create(m_pGraphicDev, L"", false, &tObj);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DeadSpace1", pGameObject, this), E_FAIL);


	pGameObject = CVacuum::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"VacuumBoss", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eDefaultTrigger);

	lstrcpy(tObj.tagMesh, L"VacuumBossField");
	tObj.vScale = _vec3{ 0.01f,0.01f,0.01f };
	tObj.eMeshType = 1;
	tObj.eRenderType = RENDER_NONALPHA;
	tObj.vPos = _vec3{ 0.f,0.f,0.f };

	pGameObject = CMap::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Field", pGameObject, this), E_FAIL);


	m_mapLayer.emplace(pLayerTag, pLayer);


	return S_OK;
}

HRESULT CVacuumBoss::Ready_LoadMap_Layer(const _tchar * pLayerTag, const _tchar * pFilePath)
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

HRESULT CVacuumBoss::Ready_LightInfo(void)
{
	D3DLIGHT9				tLightInfo;
	ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));

	tLightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Ambient = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.f);
	tLightInfo.Type = D3DLIGHT_POINT;


	tLightInfo.Diffuse = D3DXCOLOR(0.9f, 0.7f, 0.7f, 1.f);
	tLightInfo.Position = _vec3(5.f, 20.f, 0.f);
	tLightInfo.Range = 50.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 0);

	tLightInfo.Diffuse = D3DXCOLOR(1.1f, 1.f, 0.6f, 1.f);
	tLightInfo.Position = _vec3(15.f, 25.f, -5.f);
	tLightInfo.Range = 50.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 1);

	tLightInfo.Diffuse = D3DXCOLOR(1.1f * 2.5f, 1.f * 2.5f, 0.7f * 2.5f, 1.f);
	tLightInfo.Position = _vec3(66.83f, 18.68f, -28.89f);
	tLightInfo.Range = 60.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 2);

	tLightInfo.Diffuse = D3DXCOLOR(1.1f * 2.5f, 1.f * 2.5f, 0.7f * 2.5f, 1.f);
	tLightInfo.Position = _vec3(90.41f, 18.68f, 22.52f);
	tLightInfo.Range = 70.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 3);

	tLightInfo.Diffuse = D3DXCOLOR(1.1f * 3.5f, 1.f * 3.5f, 0.9f * 3.5f, 1.f);
	tLightInfo.Position = _vec3(114.40f, 18.68f, 0.f);
	tLightInfo.Range = 70.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 4);

	tLightInfo.Diffuse = D3DXCOLOR(1.f * 1.5f, 1.2f * 1.5f, 1.4f * 1.5f, 1.f);
	tLightInfo.Position = _vec3(-40.f, 15.f, 0.f);
	tLightInfo.Range = 35.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 5);


	return S_OK;
}

CVacuumBoss * CVacuumBoss::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CVacuumBoss*		pInstance = new CVacuumBoss(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
		Safe_Release(pInstance);

	return pInstance;
}
HRESULT CVacuumBoss::Ready_Interact_Layer(const _tchar* szLayer)
{
	Engine::CLayer*			pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	OBJINFO tObj;
	ZeroMemory(&tObj, sizeof(OBJINFO));
	tObj.eSceneID = RESOURCE_VB;
	// 오브젝트 추가
	tObj.vPos = _vec3(-15.8f, -1.5f, 0.05f);


	Engine::CGameObject*		pGameObject = nullptr;
	pGameObject = CVacuumFieldPart5::Create(m_pGraphicDev, DIRRIGHT, 0, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"VacuumFieldHead_1", pGameObject, this), E_FAIL);
	auto vecBody = pGameObject->Is<CVacuumFieldPart5>()->Get_HozeBody();
	pGameObject->Is<CVacuumFieldPart5>()->Set_MoveLock(LOCK_LEFT | LOCK_RIGHT);

	m_pScene->addAggregate(*pGameObject->Is<CVacuumFieldPart5>()->Get_Hoze());
	for (auto pBody : vecBody)
		setupFiltering(Engine::Get_Allocator(), pBody, FilterGroup::eHoze, FilterGroup::eCody | FilterGroup::eMay);

	tObj.vPos = _vec3(-5.8f, -1.5f, 0.05f);

	pGameObject = CVacuumFieldPart5::Create(m_pGraphicDev, DIRRIGHT, 0, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"VacuumFieldHead_2", pGameObject, this), E_FAIL);
	vecBody = pGameObject->Is<CVacuumFieldPart5>()->Get_HozeBody();
	pGameObject->Is<CVacuumFieldPart5>()->Set_Suck(true);
	m_pScene->addAggregate(*pGameObject->Is<CVacuumFieldPart5>()->Get_Hoze());
	for (auto pBody : vecBody)
		setupFiltering(Engine::Get_Allocator(), pBody, FilterGroup::eHoze, FilterGroup::eCody | FilterGroup::eMay);


	m_mapLayer.emplace(szLayer, pLayer);

	return S_OK;
}
void CVacuumBoss::Free(void)
{
	// 삭제
	CCollisionMgr::DestroyInstance();

	for (_uint i = 0; i < m_vecObject.size(); ++i)
		Safe_Delete_Array(m_vecObject[i]);

	m_vecObject.clear();
	m_vecObject.shrink_to_fit();
	Engine::Delete_AllResource(RESOURCE_VB);

	Engine::Release_Sound(RESOURCE_VF);
	Engine::Delete_AllResource(RESOURCE_PLAYER);
	Engine::CScene::Free();
}

void CVacuumBoss::SCENE_FADE(const _float & fTimeDelta, SCENEID eID)
{
	if (!m_bSceneChange)
		return;

	m_fSceneChange += fTimeDelta * 100.f;

	CCody* pCody = (CCody*)Engine::Get_GameObject(L"GameObject", L"Cody");
	CMay* pMay = (CMay*)Engine::Get_GameObject(L"GameObject", L"May");
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