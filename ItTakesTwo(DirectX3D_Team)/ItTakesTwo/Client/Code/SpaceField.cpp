
#include "stdafx.h"
#include "SpaceField.h"
#include "Export_Function.h"
#include "UI.h"
#include "Cody.h"
#include "May.h"
#include "CodyCamera.h"
#include "MayCamera.h"
#include "StaticObject.h"
#include "SpaceField.h"
#include "Map.h"
#include "SceneChangeSpace.h"
#include "DeadSpace.h"
#include "Cody_Space.h"
#include "May_Space.h"
#include "GravityFloor.h"
#include "SpacePowerButton.h"
#include "SpaceBridge.h"
#include "GeneratorBattery.h"
#include "GeneratorLever.h"
#include "PowerButton.h"
#include "HangDoor.h"
#include "RobotHead.h"
#include "Escape_Shuttle.h"
#include "Escape_ShuttleRamp.h"
#include "Earth.h"
#include "BatteryBox.h"
#include "SaveSpace.h"
#include "SpaceGenerator.h"

CSpaceField::CSpaceField(LPDIRECT3DDEVICE9 pDevice)
	: CScene(pDevice)
{
}

CSpaceField::~CSpaceField()
{
}


HRESULT CSpaceField::Ready_Scene()
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

	//auto* pPlane = PxCreatePlane(*pPhysics, PxPlane(0, 1, 0, 1), *pPhysics->createMaterial(0.5f, 0.5f, 0.f));

	//pPlane->setName((char*)pPlane);
	//setupFiltering(Engine::Get_Allocator(), pPlane, FilterGroup::eGround, FilterGroup::eCody | FilterGroup::eMay);
	//m_pScene->addActor(*pPlane);
	m_pScene->setGravity(PxVec3(0.0f, -20.f, 0.0f));
	FAILED_CHECK_RETURN(Ready_Environment_Layer(L"Environment"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_GameObject_Layer(L"GameObject"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Interact_Layer(L"Interact"), E_FAIL);

	Load_Map(L"");

	//FAILED_CHECK_RETURN(Ready_LoadMap_Layer(L"LoadObject", L"../../Data/ITT_Test/Test.dat"), E_FAIL);

	Ready_LightInfo();
	Effect_Warp();

	Engine::StopAll();
	Engine::PlayBGM(L"SpaceField1_BGM.wav", 1.f);

	return S_OK;
}

_int CSpaceField::Update_Scene(const _float & fTimeDelta)
{


	m_pScene->simulate(fTimeDelta);

	m_pScene->fetchResults(true);
	_int iExit = Engine::CScene::Update_Scene(fTimeDelta);
	if (iExit == SCENE_CHANGE)
		return iExit;

	SceneChange(fTimeDelta);
	_bool bBatteryMoveEnd;
	_bool bLeverMoveEnd;
	CGeneratorBattery*	pBattery = (CGeneratorBattery*)Engine::Get_GameObject(L"GameObject", L"GeneratorBattery");
	CGeneratorLever*	pLever = (CGeneratorLever*)Engine::Get_GameObject(L"GameObject", L"GeneratorLever");

	bBatteryMoveEnd = pBattery->Get_MoveEnd();
	bLeverMoveEnd = pLever->Get_MoveEnd();
	if (m_bStart)
	{
		Engine::Get_GameObject(L"GameObject", L"SceneChange_SF_To_SF2")->On_Active(nullptr);
		Engine::Get_GameObject(L"GameObject", L"SceneChange_SF_To_SF3")->On_Active(nullptr);
		Engine::Get_GameObject(L"GameObject", L"SceneChange_SF_To_SB")->On_Active(nullptr);
		m_bStart = false;


		if (g_iClearScene == 1)
		{
			m_bCylinder = true;
			Cylinder_Effect();
		}
		if (g_iClearScene == 2)
		{
			m_bCylinder = true;
			m_bCylinder2 = true;
			Cylinder_Effect();
			Cylinder_Effect2();
		}
		if (g_iClearScene == 0)
		{
			CCamera* pCamera =(CCamera*)Engine::Get_GameObject(L"GameObject", L"MayCamera");
			if (!pCamera)
				return 0;
			CTransform* pShuttlePos = (CTransform*)(Engine::Get_Component(L"GameObject", L"Escape_Shuttle", L"Com_TransformShuttle", ID_DYNAMIC));
			if (!pShuttlePos)
				return 0;
			Engine::End_Split();
			pCamera->Set_Mode(CAMERA_FIX);
			pCamera->Set_Fix(pShuttlePos->Get_Position(), { 14.8277092f,7.76227856f,12.4480476f }, true);
			pCamera->Set_ResetTimer(4.f, false);
		}
		


	}
	if (!m_bCylinder &&bBatteryMoveEnd && bLeverMoveEnd)
	{
		m_bCylinder = true;
		Cylinder_Effect();
		CBatteryBox* pBBox = (CBatteryBox*)Engine::Get_GameObject(L"GameObject", L"BatteryBox");
		pBBox->Set_Light(true);

		pLever->Set_Field_CutScene();
	}

	if (Engine::Key_Down(DIK_Y))
	{
		CHANGE_SCENE(m_pGraphicDev, SCENE_SF2, OBJ_NOEVENT);

		return 0;
	}
	return iExit;
}

_int CSpaceField::LateUpdate_Scene(const _float & fTimeDelta)
{
	_int iExit = Engine::CScene::LateUpdate_Scene(fTimeDelta);

	return iExit;
}

void CSpaceField::Render_Scene()
{

}

void CSpaceField::onConstraintBreak(PxConstraintInfo* constraints, PxU32 count)
{

	return;
}

void CSpaceField::onWake(PxActor** actors, PxU32 count)
{
	OutputDebugString(L"깼음");
	return;
}

void CSpaceField::onSleep(PxActor** actors, PxU32 count)
{
	OutputDebugString(L"잠듬");

	return;
}

void CSpaceField::onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs)
{
	OutputDebugString(L"충돌있음\n");
	for (PxU32 i = 0; i < nbPairs; ++i)
	{

		/*if ((!strcmp(pairHeader.actors[0]->getName(), "Cody") && !strcmp(pairHeader.actors[1]->getName(), "Wall")) ||
			(!strcmp(pairHeader.actors[1]->getName(), "Cody") && !strcmp(pairHeader.actors[0]->getName(), "Wall")))
		{
			Engine::Get_GameObject(L"GameObject", L"Cody")->Is<CCody>()->Set_Climbing(true);
		}*/

	}
}

void CSpaceField::onTrigger(PxTriggerPair* pairs, PxU32 count)
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
			case TRIGGER_SPACE_GENERATOR_BATTERY:
				Event_Generator_Battery(&Trigger);
				break;
			case TRIGGER_SPACE_GENERATOR_LEVER:
				Event_Generator_Lever(&Trigger);
				break;
			case TRIGGER_SPACE_GRAVITY_FLY:
				Event_Gravity_Fly(&Trigger);
				break;
			case TRIGGER_SPACE_GRAVITY_FLYBOSSROOM:
				Event_Gravity_FlyBossRoom(&Trigger);
				break;
			case TRIGGER_HANG:
				Event_Hang(&Trigger);
				break;
			case TRIGGER_DEADSPACE:
				Event_Dead(&Trigger);
				break;
			}
		}
	}
}


void CSpaceField::onAdvance(const PxRigidBody*const* bodyBuffer, const PxTransform* poseBuffer, const PxU32 count)
{
	OutputDebugString(L"업데이트");
}


void CSpaceField::Event_Scene_Change(PxTriggerPair* pairs)
{
	ToTrigger(pairs->triggerActor->getName())->Activate((void*)pairs->otherShape->getName());

}

void CSpaceField::Event_Gravity_Fly(PxTriggerPair * pairs)
{
	auto* pTrigger = ToTrigger(pairs->triggerShape->getName());

	//제네레이터 완성
	_bool bBatteryMoveEnd;
	_bool bLeverMoveEnd;
	CGeneratorBattery*	pBattery = (CGeneratorBattery*)Engine::Get_GameObject(L"GameObject", L"GeneratorBattery");
	CGeneratorLever*	pLever = (CGeneratorLever*)Engine::Get_GameObject(L"GameObject", L"GeneratorLever");

	bBatteryMoveEnd = pBattery->Get_MoveEnd();
	bLeverMoveEnd = pLever->Get_MoveEnd();

	if ((bBatteryMoveEnd && bLeverMoveEnd) || g_iClearScene>0)
	{
		if (pairs->otherShape->getSimulationFilterData().word0 == FilterGroup::eCody)
		{

			if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
			{
				ToObj<CCody_Space>(pairs->otherShape->getName())->Set_FreeGravity();
			}
			else if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_LOST)
			{
				ToObj<CCody_Space>(pairs->otherShape->getName())->Set_FreeGravity(false);
			}
		}
		else if (pairs->otherShape->getSimulationFilterData().word0 == FilterGroup::eMay)
		{
			if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
			{
				ToObj<CMay_Space>(pairs->otherShape->getName())->Set_FreeGravity();
			}
			else if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_LOST)
			{
				ToObj<CMay_Space>(pairs->otherShape->getName())->Set_FreeGravity(false);
			}
		}
	}
}

void CSpaceField::Event_Gravity_FlyBossRoom(PxTriggerPair * pairs)
{
	auto* pTrigger = ToTrigger(pairs->triggerShape->getName());

	if (g_iClearScene >= 2)
	{
		if (pairs->otherShape->getSimulationFilterData().word0 == FilterGroup::eCody)
		{

			if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
			{
				ToObj<CCody_Space>(pairs->otherShape->getName())->Set_FreeGravity();
			}
			else if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_LOST)
			{
				ToObj<CCody_Space>(pairs->otherShape->getName())->Set_FreeGravity(false);
			}
		}
		else if (pairs->otherShape->getSimulationFilterData().word0 == FilterGroup::eMay)
		{
			if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
			{
				ToObj<CMay_Space>(pairs->otherShape->getName())->Set_FreeGravity();
			}
			else if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_LOST)
			{
				ToObj<CMay_Space>(pairs->otherShape->getName())->Set_FreeGravity(false);
			}
		}
	}
}

void CSpaceField::Event_Generator_Battery(PxTriggerPair * pairs)
{
	auto* pTrigger = ToTrigger(pairs->triggerShape->getName());
	CGeneratorBattery*		pBattery = pTrigger->Is<CGeneratorBattery>();

	if (pairs->otherShape->getSimulationFilterData().word0 == FilterGroup::eCody)
	{
		CCody_Space* pCody = ToObj<CCody_Space>(pairs->otherShape->getName());
		NULL_CHECK_RETURN(pCody, );

		if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			OutputDebugString(L"코디가 배터리에 붙을 수 있음");
			pCody->Set_Push(true, pBattery, pBattery->Get_Push_StartPos(), CCody_Space::GENERATOR_BATTERY, _vec3(1.f, 0.f, 0.f));
		}
		else if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_LOST)
		{
			pCody->Set_Push(false);
		}
	}
}

void CSpaceField::Event_Generator_Lever(PxTriggerPair * pairs)
{
	auto* pTrigger = ToTrigger(pairs->triggerShape->getName());
	CGeneratorLever*	pLever = pTrigger->Is<CGeneratorLever>();

	if (pairs->otherShape->getSimulationFilterData().word0 == FilterGroup::eMay)
	{
		CMay_Space* pMay = ToObj<CMay_Space>(pairs->otherShape->getName());
		NULL_CHECK_RETURN(pMay, );

		if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			OutputDebugString(L"메이가 레버에 붙을 수 있음");
			pMay->Set_PickUp(true, CPlayerValue::LEVER, pLever);
			pMay->Set_Slap(true);
			//pMay->Set_PickUp(true, CMay_Space::GENERATOR_LEVER, pLever);
		}
		else if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_LOST)
		{
			pMay->Set_PickUp(false, CPlayerValue::LEVER, pLever);
			//pMay->Set_PickUp(false, CMay_Space::GENERATOR_LEVER, nullptr);
		}

	}
}

void CSpaceField::Event_Hang(PxTriggerPair * pairs)
{
	auto* pTrigger = ToTrigger(pairs->triggerShape->getName());
	CHangDoor*		pHangDoor = pTrigger->Is<CHangDoor>();

	if (pairs->otherShape->getSimulationFilterData().word0 == FilterGroup::eMay)
	{
		CMay_Space* pMay = ToObj<CMay_Space>(pairs->otherShape->getName());
		NULL_CHECK_RETURN(pMay, );

		if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			pMay->Set_Hang(CCody::HANGTYPE::HANG, true, pHangDoor->Get_GoalPos(), pHangDoor->Get_Normal());;
		}
		else if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_LOST)
		{
			pMay->Set_Hang(CCody::HANGTYPE::LEDGE, false, _vec3(0.0f, 0.0f, 0.0f), _vec3(0.0f, 0.0f, 0.0f));
		}
	}
}

void CSpaceField::SceneChange(const _float & fTimeDelta)
{
	//튜토리얼 제네레이터 활성화되면, 씬 체인지 트리거 켜줌.
	if (!m_bGenerator)
	{
		CGeneratorLever*	pLever = (CGeneratorLever*)Get_GameObject(L"GameObject", L"GeneratorLever");

		_bool bTemp;
		bTemp = pLever->Get_MoveEnd();
		
		if (bTemp)
		{
			pLever->Set_Field_CutScene();
			m_bGenerator = true;
		}

	}
}


void CSpaceField::Event_Bounce_Switch(PxTriggerPair* pairs)
{

}

void CSpaceField::Event_Dead(PxTriggerPair * pairs)
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

HRESULT CSpaceField::Ready_Environment_Layer(const _tchar * pLayerTag)
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

	OBJINFO tObj;
	ZeroMemory(&tObj, sizeof(OBJINFO));
	tObj.eSceneID = RESOURCE_SF;

	pGameObject = CEarth::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Earth", pGameObject, this), E_FAIL);

	m_mapLayer.emplace(pLayerTag, pLayer);
	return S_OK;
}
//#스테이지	스페이스 필드
HRESULT CSpaceField::Ready_GameObject_Layer(const _tchar * pLayerTag)
{
	Engine::CLayer*			pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	OBJINFO tObj;
	ZeroMemory(&tObj, sizeof(OBJINFO));
	tObj.eSceneID = RESOURCE_SF;
	// 오브젝트 추가
	Engine::CGameObject*		pGameObject = nullptr;

	//씬 체인지
	pGameObject = CSceneChangeSpace::Create(m_pGraphicDev, SCENE_SF2, { 193.5f, 99.1f, 25.f }, 5.f, 5.f, 2.f, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SceneChange_SF_To_SF2", pGameObject, this), E_FAIL);


	pGameObject = CSceneChangeSpace::Create(m_pGraphicDev, SCENE_SF3, { 193.5f, 99.1f, -25.f }, 5.f, 5.f, 2.f, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SceneChange_SF_To_SF3", pGameObject, this), E_FAIL);


	pGameObject = CSceneChangeSpace::Create(m_pGraphicDev, SCENE_SB, { 193.5f, 140.f, 0.f }, 15.f, 2.f, 15.f, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SceneChange_SF_To_SB", pGameObject, this), E_FAIL);




	tObj.vScale = { 0.01f, 0.01f ,0.01f };

	// CodyCamera
	pGameObject = CCodyCamera::Create(m_pGraphicDev, &_vec3(0.f, 10.f, -5.f), &_vec3(0.f, 0.f, 1.f), &_vec3(0.f, 1.f, 0.f), D3DXToRadian(60.f), _float(WINCX) / WINCY, 0.1f, 1000.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CodyCamera", pGameObject, this), E_FAIL);

	// MayCamera
	pGameObject = CMayCamera::Create(m_pGraphicDev, &_vec3(0.f, 10.f, -5.f), &_vec3(0.f, 0.f, 1.f), &_vec3(0.f, 1.f, 0.f), D3DXToRadian(60.f), _float(WINCX) / WINCY, 0.1f, 1000.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MayCamera", pGameObject, this), E_FAIL);




	//pGameObject->Is<CMayCamera>()->Set_FreeMode();

	////780.886, 10794.9, 2694.33
	//tObj.vPos = { 97.97f,22.59f,37.8f };//씬마지막
	//tObj.vPos = { 7.8f,26.9f,107.19f };//씬처음
	//tObj.vPos = { 175.f,50.f, -7.f };



	////tObj.vPos = { 175.f,150.f, -7.f };

	tObj.vPos = { 0.f,5.f,0.f };
	tObj.vPos = { 176.87f, 23.1f, 0.0f }; // 제너레이터
	tObj.vPos = { 0.f,3.f,-3.5f };
	tObj.vPos = m_vStartPos;

	pGameObject = CCody_Space::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Cody", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eCody, FilterGroup::eGround | FilterGroup::eWall);
	//tObj.vPos = { 25.944864f, 0.257836f, 0.0f };

	//tObj.vPos = { 176.87f, 25.1f, 0.0f }; // 제너레이터

	pGameObject = CMay_Space::Create(m_pGraphicDev, &tObj);
	//pGameObject->Is<CMay_Space>()->Set_Up({ 0.f,1.f,0.f });
	pGameObject->Is<CMay_Space>()->Set_Up({ 0.f, 1.f, 0.f });
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"May", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMay, FilterGroup::eGround | FilterGroup::eWall);

	lstrcpy(tObj.tagMesh, L"Gravity_Corner4");
	tObj.vPos = { 0.f,0.f,0.f };
	tObj.vScale = _vec3{ 0.01f,0.01f,0.01f };
	//tObj.vAngle = { 0.f,D3DXToRadian(-90.f),0.f };
	pGameObject = CGravityFloor::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Gravity_Corner", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eGravityFloorIN | FilterGroup::eWall, FilterGroup::eCody | FilterGroup::eMay);

	tObj.vPos = { 176.577f, 26.135f, -8.539f };
	tObj.vAngle = { 0, -89.549f, 0.f };
	pGameObject = CRobotHead::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"GeneratorHead", pGameObject, this), E_FAIL);

	tObj.vPos = { 177.35f, 24.56f, -8.4f };
	tObj.vTriggerScale = { 0.3f, 0.3f, 0.2f };
	tObj.vAngle = { 0.f, 20.f, 0.f };
	_vec3 TriggerPos = tObj.vPos + _vec3(-0.2f, -0.115f, -0.75f);
	_vec3 StartPos = tObj.vPos + _vec3(-0.45f, -0.14f, -0.756f);
	//_vec3 StartPos = tObj.vPos + _vec3(-0.45f, -0.14f, -0.756f);
	CGameObject* pBattery = nullptr;
	pBattery = CGeneratorBattery::Create(m_pGraphicDev, &tObj, TriggerPos, StartPos);
	NULL_CHECK_RETURN(pBattery, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"GeneratorBattery", pBattery, this), E_FAIL);

	tObj.vPos = { 175.f, 23.2f, -8.9f };
	tObj.vTriggerScale = { 1.f, 2.f, 1.f };
	tObj.vAngle = { 0, 25.f, 90.f };
	TriggerPos = tObj.vPos + _vec3(-1.f, -0.f, 0.f);
	StartPos = tObj.vPos + _vec3(-1.f, -1.15f, -0.f);
	pGameObject = CGeneratorLever::Create(m_pGraphicDev, &tObj, TriggerPos, StartPos, L"GeneratorBattery", 0, _vec3(1.0f, 0.0f, 0.0f));
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"GeneratorLever", pGameObject, this), E_FAIL);




	// FireDoor
	tObj.vPos = { 31.0f, 0.257836f, 0.0f };
	tObj.vScale = { 0.01f, 0.01f, 0.01f };
	tObj.vAngle = { 0.f, 0.f, 0.f };
	tObj.vTriggerScale = { 2.f, 3.f, 1.f };
	tObj.vTriggerPos = { 31.0f, 15.0f, 0.0f };
	pGameObject = CHangDoor::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"FireDoor", pGameObject, this), E_FAIL);

	tObj.vTriggerPos = { 31.0f, 5.415f, 0.0f };
	tObj.vTriggerScale = _vec3{ 0.01f, 4.55f, 3.0f };
	//tObj.
	pGameObject = CDeadSpace::Create(m_pGraphicDev, L"", g_bRenderBox, &tObj);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DeadSpace1", pGameObject, this), E_FAIL);


	tObj.vPos = { 0.f, 0.f, -4.f };
	tObj.vAngle = { 0.f, -45.f, 0.f };
	pGameObject = CEscape_Shuttle::Create(m_pGraphicDev, &tObj, true);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Escape_Shuttle", pGameObject, this), E_FAIL);

	tObj.vPos = { 0.f, 0.f, -4.f };
	tObj.vAngle = { 0.f, -45.f, 0.f };
	pGameObject = CEscape_ShuttleRamp::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Escape_ShuttleRamp", pGameObject, this), E_FAIL);






#pragma region 맵 트리거 ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	m_pTrigger[SpaceTrigger::Trigger_GravityFly] = CTrigger::CreateCapsuleTirgger(m_pScene, L"GravityFly", _vec3(193.5f, 40.f, 0.f), 13.f, 58.f, TRIGGER_SPACE_GRAVITY_FLY, FilterGroup::eDefaultTrigger);
	m_pTrigger[SpaceTrigger::Trigger_GravityFly]->Set_Rotation(_vec3(0.f, 0.f, D3DXToRadian(90.f)));

	m_pTrigger[SpaceTrigger::Trigger_GravityFly_BossRoom] = CTrigger::CreateCapsuleTirgger(m_pScene, L"GravityFly_BossRoom", _vec3(193.5f, 110.f, 0.f), 13.f, 34.f, TRIGGER_SPACE_GRAVITY_FLYBOSSROOM, FilterGroup::eDefaultTrigger);
	m_pTrigger[SpaceTrigger::Trigger_GravityFly_BossRoom]->Set_Rotation(_vec3(0.f, 0.f, D3DXToRadian(90.f)));


#pragma  endregion


	tObj.vScale = _vec3{ 0.01f,0.01f,0.01f };
	tObj.vAngle = { 0, 0.f, 0.f };
	tObj.vPos = { 0.f,0.f,0.f };
	tObj.eMeshType = 1;
	tObj.eRenderType = RENDER_NONALPHA;



	lstrcpy(tObj.tagMesh, L"SpaceFloor");
	pGameObject = CMap::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SpaceFloor", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eGround | FilterGroup::eWall, FilterGroup::eCody | FilterGroup::eMay);

	lstrcpy(tObj.tagMesh, L"Gravity_Straight");
	pGameObject = CMap::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Gravity_Straight", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eGround | FilterGroup::eWall, FilterGroup::eCody | FilterGroup::eMay);



	lstrcpy(tObj.tagMesh, L"SpaceTutorial");
	pGameObject = CMap::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SpaceTutorial", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eGround | FilterGroup::eWall, FilterGroup::eCody | FilterGroup::eMay);


	lstrcpy(tObj.tagMesh, L"SpaceWindow");
	pGameObject = CMap::Create(m_pGraphicDev, &tObj);
	//static_cast<CMap*>(pGameObject)->Set_PassIndex(14);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SpaceWindow", pGameObject, this), E_FAIL);

	lstrcpy(tObj.tagMesh, L"Lamp");
	pGameObject = CMap::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Lamp", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eGround | FilterGroup::eWall, FilterGroup::eCody | FilterGroup::eMay);

	lstrcpy(tObj.tagMesh, L"DoorWindow");
	pGameObject = CMap::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DoorWindow", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eGround | FilterGroup::eWall, FilterGroup::eCody | FilterGroup::eMay);


	tObj.vPos = { 176.30f, 22.09f, -8.82f };
	tObj.vAngle = { 0.f, 180.f, 0.f };
	lstrcpy(tObj.tagMesh, L"Generator");
	pGameObject = CSpaceGenerator::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Generator", pGameObject, this), E_FAIL);

	tObj.vPos = { 176.30f, 22.09f, -8.82f };
	tObj.vAngle = { 0.f, 180.f, 0.f };
	lstrcpy(tObj.tagMesh, L"GeneratorStair");
	pGameObject = CSpaceGenerator::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"GeneratorStair", pGameObject, this), E_FAIL);
	

	tObj.vPos = { 177.34f, 24.36f, -8.82f };
	tObj.vAngle = { 0.f, D3DXToRadian(180.f), 0.f };
	pGameObject = CBatteryBox::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"BatteryBox", pGameObject, this), E_FAIL);
	pBattery->Set_InteractObj(pGameObject);
	tObj.vAngle = { 0, 0.f, 0.f };
	tObj.vPos = { 0.f,0.f,0.f };

	// 데드 스페이스
	{
		// 맵전체
		tObj.vTriggerPos = { 0.0f, -40.0f, 0.0f };
		tObj.vTriggerScale = _vec3{ 220.0f, 1.0f, 220.0f };
		pGameObject = CDeadSpace::Create(m_pGraphicDev, L"", true, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DeadSpace2", pGameObject, this), E_FAIL);

		// 맵전체
		tObj.vTriggerPos = { 220.0f, 0.0f, 0.0f };
		tObj.vTriggerScale = _vec3{ 1.0f, 220.0f, 220.0f };
		pGameObject = CDeadSpace::Create(m_pGraphicDev, L"", true, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DeadSpace3", pGameObject, this), E_FAIL);

		// 맵전체
		tObj.vTriggerPos = { -30.0f, 0.0f, 0.0f };
		tObj.vTriggerScale = _vec3{ 1.0f, 220.0f, 220.0f };
		pGameObject = CDeadSpace::Create(m_pGraphicDev, L"", true, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DeadSpace4", pGameObject, this), E_FAIL);

		// 맵전체
		tObj.vTriggerPos = { 0.0f, 0.0f, 30.0f };
		tObj.vTriggerScale = _vec3{ 220.0f, 220.0f, 1.0f };
		pGameObject = CDeadSpace::Create(m_pGraphicDev, L"", true, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DeadSpace5", pGameObject, this), E_FAIL);

		// 맵전체
		tObj.vTriggerPos = { 0.0f, 0.0f, -30.0f };
		tObj.vTriggerScale = _vec3{ 220.0f, 220.0f, 1.0f };
		pGameObject = CDeadSpace::Create(m_pGraphicDev, L"", true, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DeadSpace6", pGameObject, this), E_FAIL);

		// 맵전체
		tObj.vTriggerPos = { 0.0f, 200.0f, 0.0f };
		tObj.vTriggerScale = _vec3{ 220.0f, 10.0f, 220.0f };
		pGameObject = CDeadSpace::Create(m_pGraphicDev, L"", true, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DeadSpace7", pGameObject, this), E_FAIL);
	}

	// 세이브 포인트
	{
		tObj.vPos = { 29.55f, 0.2f, -0.12f };
		tObj.vScale = _vec3{ 0.5f, 20.0f, 10.0f };
		pGameObject = CSaveSpace::Create(m_pGraphicDev, { 214.8f, 23.055f, -1.126f }, L"Yellow_Bg", g_bRenderBox, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SavePoint1", pGameObject, this), E_FAIL);
		pGameObject->Is<CSaveSpace>()->Set_CodyRespwanPoint({ 29.55f, 0.2f, -0.12f });
		pGameObject->Is<CSaveSpace>()->Set_MayRespwanPoint({ 27.9259f, 16.5f, 0.136f });
		pGameObject->Is<CSaveSpace>()->Set_MayRespwanUp({ 0.0f, -1.0f, 0.0f });

		tObj.vPos = { 34.55f, 0.2f, -0.12f };
		tObj.vScale = _vec3{ 0.5f, 20.0f, 10.0f };
		pGameObject = CSaveSpace::Create(m_pGraphicDev, { 214.8f, 23.055f, -1.126f }, L"Yellow_Bg", g_bRenderBox, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SavePoint2", pGameObject, this), E_FAIL);
		pGameObject->Is<CSaveSpace>()->Set_CodyRespwanPoint({ 34.55f, 0.2f, -0.12f });
		pGameObject->Is<CSaveSpace>()->Set_MayRespwanPoint({ 34.9259f, 16.5f, 0.136f });
		pGameObject->Is<CSaveSpace>()->Set_MayRespwanUp({ 0.0f, -1.0f, 0.0f });

		tObj.vPos = { 156.67f, 20.8f, -0.12f };
		tObj.vScale = _vec3{ 1.0f, 20.0f, 10.0f };
		pGameObject = CSaveSpace::Create(m_pGraphicDev, { 214.8f, 23.055f, -1.126f }, L"Yellow_Bg", g_bRenderBox, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SavePoint3", pGameObject, this), E_FAIL);
		pGameObject->Is<CSaveSpace>()->Set_CodyRespwanPoint({ 156.67f, 20.8f, -0.12f });
		pGameObject->Is<CSaveSpace>()->Set_MayRespwanPoint({ 156.67f, 20.8f, -0.12f });
		pGameObject->Is<CSaveSpace>()->Set_MayRespwanUp({ 0.0f, 1.0f, 0.0f });

		tObj.vPos = { 195.98f, 77.358f, -0.12f };
		tObj.vScale = _vec3{ 20.0f, 1.0f, 20.0f };
		pGameObject = CSaveSpace::Create(m_pGraphicDev, { 214.8f, 23.055f, -1.126f }, L"Yellow_Bg", g_bRenderBox, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SavePoint4", pGameObject, this), E_FAIL);
		pGameObject->Is<CSaveSpace>()->Set_CodyRespwanPoint({ 175.88f, 97.358f, 0.81f });
		pGameObject->Is<CSaveSpace>()->Set_MayRespwanPoint({ 175.88f, 97.358f, 0.81f });
		pGameObject->Is<CSaveSpace>()->Set_MayRespwanUp({ 0.0f, 1.0f, 0.0f });
	}

	m_mapLayer.emplace(pLayerTag, pLayer);


	return S_OK;
}

HRESULT CSpaceField::Ready_LightInfo(void)
{
	D3DLIGHT9				tLightInfo;
	ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));
	tLightInfo.Type = D3DLIGHT_POINT;
	tLightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Ambient = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.f);


	tLightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Position = _vec3(3.07f, 10.25f, 0.f);
	tLightInfo.Range = 40.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 0);

	tLightInfo.Diffuse = D3DXCOLOR(1.f * 1.5f, 0.7f * 1.5f, 0.4f * 1.5f, 1.f);
	tLightInfo.Position = _vec3(31.01f, 7.43f, 0.f);
	tLightInfo.Range = 20.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 1);

	tLightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Position = _vec3(54.80f, 8.10f, 0.f);
	tLightInfo.Range = 30.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 2);

	tLightInfo.Diffuse = D3DXCOLOR(0.4f * 1.5f, 0.6f * 1.5f, 0.9f * 1.5f, 1.f);
	tLightInfo.Position = _vec3(111.31f, 25.33f, -27.44f);
	tLightInfo.Range = 70.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 3);

	tLightInfo.Diffuse = D3DXCOLOR(0.4f * 1.5f, 0.6f * 1.5f, 0.9f * 1.5f, 1.f);
	tLightInfo.Position = _vec3(162.30f, 37.30f, 0.f);
	tLightInfo.Range = 30.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 4);

	tLightInfo.Diffuse = D3DXCOLOR(0.6f * 3.5f, 0.8f * 3.5f, 1.f * 3.5f, 1.f);
	tLightInfo.Position = _vec3(193.48f, 25.26f, 0.f);
	tLightInfo.Range = 18.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 5);

	tLightInfo.Diffuse = D3DXCOLOR(0.4f * 2.5f, 0.6f * 2.5f, 0.9f * 2.5f, 1.f);
	tLightInfo.Position = _vec3(193.48f, 37.10f, 0.f);
	tLightInfo.Range = 50.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 6);

	tLightInfo.Diffuse = D3DXCOLOR(0.6f * 3.5f, 0.8f * 3.5f, 1.f * 3.5f, 1.f);
	tLightInfo.Position = _vec3(193.48f, 72.99f, 0.f);
	tLightInfo.Range = 18.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 7);

	tLightInfo.Diffuse = D3DXCOLOR(0.6f * 3.5f, 0.8f * 3.5f, 1.f * 3.5f, 1.f);
	tLightInfo.Position = _vec3(193.48f, 91.76f, 0.f);
	tLightInfo.Range = 18.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 8);

	tLightInfo.Diffuse = D3DXCOLOR(0.4f * 2.5f, 0.6f * 2.5f, 0.9f * 2.5f, 1.f);
	tLightInfo.Position = _vec3(193.48f, 113.94f, 0.f);
	tLightInfo.Range = 50.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 9);

	tLightInfo.Diffuse = D3DXCOLOR(0.6f * 3.5f, 0.8f * 3.5f, 1.f * 3.5f, 1.f);
	tLightInfo.Position = _vec3(193.48f, 109.62f, 0.f);
	tLightInfo.Range = 18.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 10);

	tLightInfo.Diffuse = D3DXCOLOR(0.6f * 3.5f, 0.8f * 3.5f, 1.f * 3.5f, 1.f);
	tLightInfo.Position = _vec3(193.48f, 124.46f, 0.f);
	tLightInfo.Range = 18.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 11);

	tLightInfo.Diffuse = D3DXCOLOR(0.6f * 3.5f, 0.8f * 3.5f, 1.f * 3.5f, 1.f);
	tLightInfo.Position = _vec3(193.48f, 139.51f, 0.f);
	tLightInfo.Range = 18.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 12);

	tLightInfo.Diffuse = D3DXCOLOR(0.6f * 3.5f, 0.8f * 3.5f, 1.f * 3.5f, 1.f);
	tLightInfo.Position = _vec3(193.48f, 154.15f, 0.f);
	tLightInfo.Range = 18.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 13);

	tLightInfo.Diffuse = D3DXCOLOR(1.f * 1.5f, 0.8f * 1.5f, 0.6f * 1.5f, 1.f);
	tLightInfo.Position = _vec3(193.48f, 144.31f, 0.f);
	tLightInfo.Range = 50.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 14);

	tLightInfo.Diffuse = D3DXCOLOR(1.f * 1.5f, 0.8f * 1.5f, 0.6f * 1.5f, 1.f);
	tLightInfo.Position = _vec3(193.48f, 80.46f, 0.f);
	tLightInfo.Range = 50.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 15);

	tLightInfo.Diffuse = D3DXCOLOR(1.f * 1.5f, 0.8f * 1.5f, 0.6f * 1.5f, 1.f);
	tLightInfo.Position = _vec3(193.48f, 6.64f, 0.f);
	tLightInfo.Range = 50.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 16);

	tLightInfo.Diffuse = D3DXCOLOR(1.f * 1.5f, 0.8f * 1.5f, 0.6f * 1.5f, 1.f);
	tLightInfo.Position = _vec3(44.11f, 60.00f, 0.f);
	tLightInfo.Range = 60.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 17);

	tLightInfo.Diffuse = D3DXCOLOR(1.f * 1.5f, 0.8f * 1.5f, 0.6f * 1.5f, 1.f);
	tLightInfo.Position = _vec3(7.76f, 60.00f, 0.f);
	tLightInfo.Range = 60.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 18);


	return S_OK;
}

CSpaceField * CSpaceField::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSpaceField*		pInstance = new CSpaceField(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
		Safe_Release(pInstance);

	return pInstance;
}

CSpaceField* CSpaceField::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vPos)
{
	CSpaceField*		pInstance = new CSpaceField(pGraphicDev);
	if (pInstance)
		pInstance->Set_StartPos(vPos);
	if (FAILED(pInstance->Ready_Scene()))
		Safe_Release(pInstance);
	return pInstance;
}

void CSpaceField::Free(void)
{
	// 삭제

	for (_uint i = 0; i < m_vecObject.size(); ++i)
		Safe_Delete_Array(m_vecObject[i]);

	m_vecObject.clear();
	m_vecObject.shrink_to_fit();
	Engine::Delete_AllResource(RESOURCE_SF);
	Engine::CScene::Free();
}


void CSpaceField::Load_Map(const _wstr& tagFile)
{
	Engine::CLayer* pLayer = nullptr;
	pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, );
	m_mapLayer.emplace(L"Map", pLayer);
	Create_Map(pLayer, L"SFSpaceGlass");
	Create_Map(pLayer, L"SFSpaceBox");
	Create_Map(pLayer, L"SFRectGlass");



}
void CSpaceField::Create_Map(Engine::CLayer* pLayer, const _wstr& tagFile)
{
	OBJINFO tObj;
	ZeroMemory(&tObj, sizeof(OBJINFO));
	tObj.eSceneID = RESOURCE_SF;
	// 오브젝트 추가
	Engine::CGameObject*		pGameObject = nullptr;

	wifstream rd;
	wstring FilePath = L"../../Data/Instancing/";
	FilePath += tagFile + L".txt";

	rd.open(FilePath.c_str());
	_wstr Path;
	_wstr wPos;
	getline(rd, Path);
	_int iCount = 0;
	_int m_iNumInstance = stoi(Path.c_str());

	D3DXQUATERNION qQuat;
	_vec3 vPos, vScale;
	_mat matWorld, matTrans, matRot, matScale;

	D3DXMatrixScaling(&matScale, 0.1f, 0.1f, 0.1f);
	tObj.vScale = { 0.01f, 0.01f, 0.01f };
	size_t iFrom;
	size_t iTo;

	wstring tagLine;
	wstring tagName = tagFile;

	while (!rd.eof())
	{

		getline(rd, Path);

		if (Path.empty())
			break;
		iFrom = Path.find(L"=", 0) + 1;
		iTo = Path.find(L"\n", 0);
		tagLine = Path.substr(iFrom);

		vPos.x = stof(tagLine.c_str());

		getline(rd, Path);

		iFrom = Path.find(L"=", 0) + 1;
		iTo = Path.find(L"\n", 0);
		tagLine = Path.substr(iFrom);

		vPos.y = stof(tagLine.c_str());

		getline(rd, Path);

		iFrom = Path.find(L"=", 0) + 1;
		iTo = Path.find(L"\n", 0);
		tagLine = Path.substr(iFrom);

		vPos.z = stof(tagLine.c_str());

		getline(rd, Path);

		iFrom = Path.find(L"=", 0) + 1;
		iTo = Path.find(L"\n", 0);
		tagLine = Path.substr(iFrom);

		qQuat.x = stof(tagLine.c_str());

		getline(rd, Path);

		iFrom = Path.find(L"=", 0) + 1;
		iTo = Path.find(L"\n", 0);
		tagLine = Path.substr(iFrom);

		qQuat.z = stof(tagLine.c_str());

		getline(rd, Path);

		iFrom = Path.find(L"=", 0) + 1;
		iTo = Path.find(L"\n", 0);
		tagLine = Path.substr(iFrom);

		qQuat.y = stof(tagLine.c_str());

		getline(rd, Path);

		iFrom = Path.find(L"=", 0) + 1;
		iTo = Path.find(L"\n", 0);
		tagLine = Path.substr(iFrom);

		qQuat.w = -stof(tagLine.c_str());

		getline(rd, Path);

		iFrom = Path.find(L"=", 0) + 1;
		iTo = Path.find(L"\n", 0);
		tagLine = Path.substr(iFrom);

		vScale.x = stof(tagLine.c_str());

		getline(rd, Path);

		iFrom = Path.find(L"=", 0) + 1;
		iTo = Path.find(L"\n", 0);
		tagLine = Path.substr(iFrom);

		vScale.z = stof(tagLine.c_str());

		getline(rd, Path);

		iFrom = Path.find(L"=", 0) + 1;
		iTo = Path.find(L"\n", 0);
		tagLine = Path.substr(iFrom);

		vScale.y = stof(tagLine.c_str());

		D3DXMatrixTranslation(&matTrans, vPos.x, vPos.y, vPos.z);
		D3DXMatrixRotationQuaternion(&matRot, &qQuat);
		D3DXMatrixScaling(&matScale, vScale.x, vScale.y, vScale.z);


		matWorld = matScale*matRot*matTrans;
		tObj.pWorld = &matWorld;
		tObj.qQuat = qQuat;
		lstrcpy(tObj.tagMesh, tagName.c_str());
		pGameObject = CMap::Create(m_pGraphicDev, &tObj);
		NULL_CHECK_RETURN(pGameObject, );
		FAILED_CHECK_RETURN(pLayer->Add_GameObject((tagName + to_wstring(iCount)).c_str(), pGameObject, this), );

		++iCount;
	}
	rd.close();
}
HRESULT CSpaceField::Ready_Interact_Layer(const _tchar* szLayer)
{
	Engine::CLayer*			pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	OBJINFO tObj;
	ZeroMemory(&tObj, sizeof(OBJINFO));
	tObj.eSceneID = RESOURCE_SF;
	// 오브젝트 추가
	tObj.vPos = { 0.f,0.f ,0.f };
	tObj.vScale = { 0.01f,0.01f ,0.01f };
	Engine::CGameObject*		pGameObject = nullptr;
	Engine::CGameObject*		pButton = nullptr;


	tObj.vPos = { 66.f,0.f,0.f };
	tObj.vScale = _vec3{ 0.01f,0.01f,0.01f };
	//tObj.vAngle = { 0.f,D3DXToRadian(-90.f),0.f };
	pButton = CSpacePowerButton::Create(m_pGraphicDev, &tObj, 2);
	NULL_CHECK_RETURN(pButton, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SpaceBridgeButton", pButton, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pButton->Get_RigidActor(), FilterGroup::eButton, FilterGroup::eCody | FilterGroup::eMay);

	tObj.vPos = { 150.f,16.99f,0.f };
	tObj.vAngle = { 0.f,0.f,60.f };
	pGameObject = CSpaceBridge::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SpaceBridge", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eGround, FilterGroup::eCody | FilterGroup::eMay);

	pButton->Set_InteractObj(pGameObject);


	m_mapLayer.emplace(szLayer, pLayer);

	return S_OK;
}

void CSpaceField::Cylinder_Effect()
{
	if (!m_bCylinder)
		return;

	PARTICLE tParticle;
	Engine::Reset_Particle(tParticle);

	tParticle.fAcc = 1.f;
	tParticle.fCurrentTime = 0.f;
	tParticle.vEndSize = _vec3(0.01f, 0.01f, 0.01f);
	tParticle.fRotateSpeed = 0.f;

	tParticle.vStartColor = { 120.f * 3.5f, 200.f * 3.5f, 255.f * 3.5f, 255.f * 2.f };
	tParticle.vEndColor = { 120.f * 3.5f, 200.f * 3.5f, 255.f * 3.5f, 255.f * 2.f };
	tParticle.vRot = { 0.f, 0.f, 0.f };


	_float fRand = 0.f;
	for (_int i = 0; i < 1000; ++i)
	{
		tParticle.fVelocity = CRandoms()(0.f, 0.2f);
		tParticle.fLifeTime = CRandoms()(1000.f, 1500.f);
		fRand = CRandoms()(0.3f, 0.5f);
		tParticle.vStartSize = _vec3(fRand, fRand, fRand);
		tParticle.vDir = { CRandoms()(-0.1f, 0.1f), CRandoms()(-0.1f, 0.1f), CRandoms()(-0.1f, 0.1f) };

		tParticle.vPos = _vec3(CRandoms()(-13.f, 13.f) + 193.48f, CRandoms()(15.f, 110.f), CRandoms()(-13.f, 13.f));

		Engine::Emit_ImageParticle(tParticle, 15, 11);
	}
}

void CSpaceField::Cylinder_Effect2()
{
	if (!m_bCylinder2)
		return;

	PARTICLE tParticle;
	Engine::Reset_Particle(tParticle);

	tParticle.fAcc = 1.f;
	tParticle.fCurrentTime = 0.f;
	tParticle.vEndSize = _vec3(0.01f, 0.01f, 0.01f);
	tParticle.fRotateSpeed = 0.f;

	tParticle.vStartColor = { 120.f * 3.5f, 200.f * 3.5f, 255.f * 3.5f, 255.f * 2.f };
	tParticle.vEndColor = { 120.f * 3.5f, 200.f * 3.5f, 255.f * 3.5f, 255.f * 2.f };
	tParticle.vRot = { 0.f, 0.f, 0.f };


	_float fRand = 0.f;
	for (_int i = 0; i < 500; ++i)
	{
		tParticle.fVelocity = CRandoms()(0.f, 0.2f);
		tParticle.fLifeTime = CRandoms()(1000.f, 1500.f);
		fRand = CRandoms()(0.3f, 0.5f);
		tParticle.vStartSize = _vec3(fRand, fRand, fRand);
		tParticle.vDir = { CRandoms()(-0.1f, 0.1f), CRandoms()(-0.1f, 0.1f), CRandoms()(-0.1f, 0.1f) };

		tParticle.vPos = _vec3(CRandoms()(-13.f, 13.f) + 193.48f, CRandoms()(110.f, 150.f), CRandoms()(-13.f, 13.f));

		Engine::Emit_ImageParticle(tParticle, 15, 11);
	}
}

void CSpaceField::Effect_Warp()
{
	PARTICLE tParticle;
	Engine::Reset_Particle(tParticle);

	tParticle.fAcc = 1.f;
	tParticle.fCurrentTime = 0.f;
	tParticle.fLifeTime = 100000.0f;
	tParticle.fRotateSpeed = 0.f;
	tParticle.fVelocity = 0.f;
	tParticle.vDir = _vec3(0.f, 0.f, 0.f);
	tParticle.vStartSize = _vec3(13.f, 13.f, 13.f);
	tParticle.vEndSize = _vec3(13.f, 13.f, 13.f);
	tParticle.vStartColor = { 255.f, 255.f, 255.f, 255.f };
	tParticle.vEndColor = { 255.f, 255.f, 255.f, 255.f };
	tParticle.vPos = _vec3(193.53f, 103.99f, 24.6f);
	tParticle.bBill = false;

	Engine::Emit_ImageParticle(tParticle, 45, 33);

	tParticle.vRot = { 0.f , D3DXToRadian(180.f), 0.f };
	tParticle.vPos = _vec3(193.53f, 103.99f, -25.97f);
	Engine::Emit_ImageParticle(tParticle, 45, 33);

}
