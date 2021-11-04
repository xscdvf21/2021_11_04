#include "stdafx.h"
#include "SpaceField3.h"
#include "Export_Function.h"
#include "UI.h"
#include "Cody.h"
#include "May.h"
#include "CodyCamera.h"
#include "MayCamera.h"
#include "StaticObject.h"
#include "SpaceField3.h"
#include "Map.h"
#include "SceneChangeSpace.h"
#include "DeadSpace.h"
#include "Cody_Space.h"
#include "May_Space.h"
#include "GeneratorBattery.h"
#include "SpaceTube.h"
#include "Player.h"
#include "GeneratorLever.h"
#include "SpaceSpringBoardSpring.h"
#include "GroundPoundBox.h"
#include "GroundPoundButton.h"
#include "WarpGateStar.h"
#include "TeleportSpace.h"
#include "SpaceGenerator.h"
#include "SpaceSeesaw.h"
#include "GravityFloor.h"
#include "RobotHead.h"
#include "SpaceGlassBox.h"
#include "SpaceGlassHandle.h"
#include "SaveSpace.h"
#include "BatteryBox.h"
#include "SpaceRing.h"



CSpaceField3::CSpaceField3(LPDIRECT3DDEVICE9 pDevice)
	: CScene(pDevice)
{
}

CSpaceField3::~CSpaceField3()
{
}

HRESULT CSpaceField3::Ready_Scene()
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

	//FAILED_CHECK_RETURN(Ready_LoadMap_Layer(L"LoadObject", L"../../Data/ITT_Test/Test.dat"), E_FAIL);

	Ready_LightInfo();
	Effect_Warp();

	Engine::StopAll();
	Engine::PlayBGM(L"SpaceField3_BGM.wav", 1.f);

	return S_OK;
}

_int CSpaceField3::Update_Scene(const _float & fTimeDelta)
{


	m_pScene->simulate(fTimeDelta);

	m_pScene->fetchResults(true);

	Clear_SetPos(fTimeDelta);

	_int iExit = Engine::CScene::Update_Scene(fTimeDelta);

	CGeneratorLever* pLever =Engine::Get_GameObject(L"GameObject", L"GeneratorLever")->Is<CGeneratorLever>();
	if (pLever && pLever->Get_MoveEnd() &&m_bStart)
	{

		StopNPlaySound(L"ClearScene.wav", Engine::CSoundMgr::CHANNELID::Space2_ClearScene, 0.5f);
		CGameObject* pMay = Engine::Get_GameObject(L"GameObject", L"May");
		CGameObject* pCody= Engine::Get_GameObject(L"GameObject", L"Cody");
		pMay->Set_Pos({ 122.08f,-2.f,-21.57f });
		pMay->Is<CMay_Space>()->Set_Up({ 0.f,1.f,0.f });

		pCody->Set_Pos({ 121.08f,-2.f,-21.57f });

		m_bStart = false;
	}
	if (Engine::Key_Down(DIK_Y))
	{
		m_bSceneChange = true;
	}
	SCENE_FADE(fTimeDelta,  SCENE_SB);

	return iExit;
}

_int CSpaceField3::LateUpdate_Scene(const _float & fTimeDelta)
{

	_int iExit = Engine::CScene::LateUpdate_Scene(fTimeDelta);

	return iExit;
}

void CSpaceField3::Render_Scene()
{

}

void CSpaceField3::onConstraintBreak(PxConstraintInfo* constraints, PxU32 count)
{

	return;
}

void CSpaceField3::onWake(PxActor** actors, PxU32 count)
{
	return;
}

void CSpaceField3::onSleep(PxActor** actors, PxU32 count)
{

	return;
}

void CSpaceField3::onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs)
{
	for (PxU32 i = 0; i < nbPairs; ++i)
	{

		/*if ((!strcmp(pairHeader.actors[0]->getName(), "Cody") && !strcmp(pairHeader.actors[1]->getName(), "Wall")) ||
			(!strcmp(pairHeader.actors[1]->getName(), "Cody") && !strcmp(pairHeader.actors[0]->getName(), "Wall")))
		{
			Engine::Get_GameObject(L"GameObject", L"Cody")->Is<CCody>()->Set_Climbing(true);
		}*/

	}
}

void CSpaceField3::onTrigger(PxTriggerPair* pairs, PxU32 count)
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
			case TRIGGER_SPACE_TUBE:
				Event_Tube(&Trigger);
				break;
			case TRIGGER_SPACE_GENERATOR_BATTERY:
				Event_Generator_Battery(&Trigger);
				break;
			case TRIGGER_SPACE_GENERATOR_LEVER:
				Event_Generator_Lever(&Trigger);
				break;
			case TRIGGER_DEADSPACE:
				Event_Dead(&Trigger);
				break;
			case TRIGGER_SAVESPACE:
				Event_Save(&Trigger);
				break;
			case TRIGGER_SPRING:
				Event_Spring(&Trigger);
				break;
			case TRIGGER_TELEPORT:
				Event_Teleport(&Trigger);
				break;
			case TRIGGER_BOUNCE_SWITCH:
				Event_Bounce_Switch(&Trigger);
				break;
			case TRIGGER_PUSHGRAP:
				Event_PushGrap(&Trigger);
				break;
			case TRIGGER_GLASSBOX:
				Event_GlassBox(&Trigger);
				break;
			}
		}
	}
}

void CSpaceField3::onAdvance(const PxRigidBody*const* bodyBuffer, const PxTransform* poseBuffer, const PxU32 count)
{
}

void CSpaceField3::Event_Scene_Change(PxTriggerPair* pairs)
{
	CGeneratorLever*	pLever = (CGeneratorLever*)Get_GameObject(L"GameObject", L"GeneratorLever2");

	_bool bTemp;
	bTemp = pLever->Get_MoveEnd();

	if (bTemp)
		ToTrigger(pairs->triggerActor->getName())->Activate((void*)pairs->otherShape->getName());
}

void CSpaceField3::Event_Tube(PxTriggerPair * pairs)
{
	CSpaceTube* pTube = ToTrigger(pairs->triggerShape->getActor()->getName())->Is<CSpaceTube>();
	if (nullptr == pTube)
		return;

	if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
	{
		if (pairs->otherShape->getSimulationFilterData().word0 & FilterGroup::eMay)
		{
			auto* pMay = (CMay_Space*)Engine::Get_GameObject(L"GameObject", L"May")->Is<CMay_Space>();
			pMay->Set_EventJump(2.0f);
			pMay->Set_EventJumpStart(true);
			pTube->Set_SizeChange();

		}
		else if (pairs->otherShape->getSimulationFilterData().word0 & FilterGroup::eCody)
		{
			auto* pCody = (CCody_Space*)Engine::Get_GameObject(L"GameObject", L"Cody")->Is<CCody_Space>();
			pCody->Set_EventJump(2.0f);
			pCody->Set_EventJumpStart(true);
			pTube->Set_SizeChange();
		}
	}
}

void CSpaceField3::Event_Generator_Battery(PxTriggerPair * pairs)
{
	auto* pTrigger = ToTrigger(pairs->triggerShape->getName());
	CGeneratorBattery*		pBattery = pTrigger->Is<CGeneratorBattery>();

	if (pairs->otherShape->getSimulationFilterData().word0 == FilterGroup::eCody)
	{
		CCody_Space* pCody = ToObj<CCody_Space>(pairs->otherShape->getName());
		NULL_CHECK_RETURN(pCody, );

		if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			pCody->Set_Push(true, pBattery, pBattery->Get_Push_StartPos(), CCody_Space::GENERATOR_BATTERY, pBattery->Get_MoveDIr());
		}
		else if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_LOST)
		{
			pCody->Set_Push(false);
		}
	}
}

void CSpaceField3::Event_Generator_Lever(PxTriggerPair * pairs)
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



void CSpaceField3::Event_Teleport(PxTriggerPair* pairs)
{
	if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
	{
		ToTrigger(pairs->triggerShape->getActor()->getName())->Is<CTeleportSpace>()->On_Active((void*)pairs->otherShape->getName());
	}

}

void CSpaceField3::Event_PushGrap(PxTriggerPair * pairs)
{
	auto* pTrigger = ToTrigger(pairs->triggerShape->getName());
	CSpaceGlassHandle*	pHandle = pTrigger->Is<CSpaceGlassHandle>();

	if (pairs->otherShape->getSimulationFilterData().word0 == FilterGroup::eMay)
	{
		CMay_Space* pMay = ToObj<CMay_Space>(pairs->otherShape->getName());
		NULL_CHECK_RETURN(pMay, );

		if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			pMay->Set_Push(true, pHandle, pHandle->Get_Pos(), CMay_Space::DUMBBELL, pHandle->Get_PushDir(), true);
			//pMay->Set_PickUp(true, CMay_Space::GENERATOR_LEVER, pLever);
		}
		else if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_LOST)
		{
			pMay->Set_Push(false);
		}
	}
}

void CSpaceField3::Event_GlassBox(PxTriggerPair * pairs)
{
	if (pairs->otherShape->getSimulationFilterData().word0 == FilterGroup::eCody)
	{
		CCody_Space* pCody = ToObj<CCody_Space>(pairs->otherShape->getName());
		NULL_CHECK_RETURN(pCody, );

		if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			pCody->Set_RideBox(true);
		}
		else if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_LOST)
		{
			pCody->Set_RideBox(false);
		}
	}
}

void CSpaceField3::Event_Save(PxTriggerPair * pairs)
{
	auto* pTrigger = ToTrigger(pairs->triggerShape->getName());

	CSaveSpace* pSave = pTrigger->Is<CSaveSpace>();

	if (pairs->otherShape->getSimulationFilterData().word0 == FilterGroup::eCody &&
		pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
	{
		CCody* pCody = ToObj<CCody>(pairs->otherShape->getName());
		NULL_CHECK_RETURN(pCody, );

		pCody->Set_SavePoint(pSave->Get_CodyRespwanPoint());
	}
	else if (pairs->otherShape->getSimulationFilterData().word0 == FilterGroup::eMay&&
		pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
	{
		CMay* pMay = ToObj<CMay>(pairs->otherShape->getName());
		NULL_CHECK_RETURN(pMay, );

		pMay->Set_SavePoint(pSave->Get_MayRespwanPoint());
		pMay->Set_SaveUp(pSave->Get_MayRespwanUp());
	}
}

void CSpaceField3::Clear_SetPos(const _float& fTimeDelta)
{

	if (m_bClearField)
		return;

	CGeneratorLever*	pLever = (CGeneratorLever*)Get_GameObject(L"GameObject", L"GeneratorLever2");

	_bool bTemp;
	bTemp = pLever->Get_MoveEnd();

	if (bTemp)
	{
		Engine::End_Split();
		CMayCamera*		pCamera = (CMayCamera*)Engine::Get_GameObject(L"GameObject", L"MayCamera");
		pCamera->Set_Mode(CAMERA_FIX);
		pCamera->Set_ReachTime(2.f);
		pCamera->Set_Fix(_vec3(0.f, 5.f, 0.f), _vec3(30.f, 5.f, 0.f));
		pCamera->Set_ResetTimer(7.f, false);


		CMay_Space*		pMay = (CMay_Space*)Get_GameObject(L"GameObject", L"May");
		pMay->SpaceField3_Clear_SetPos(fTimeDelta);

		CCody_Space*	pCody = (CCody_Space*)Get_GameObject(L"GameObject", L"Cody");
		pCody->SpaceField3_Clear_SetPos(fTimeDelta);

		Engine::Get_GameObject(L"GameObject", L"SceneChange_SF3_To_SF")->On_Active(nullptr);

		g_iClearScene++;
		m_bClearField = true;
	}
}

void CSpaceField3::SCENE_FADE(const _float & fTimeDelta, SCENEID eID)
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
		CHANGE_SCENE(m_pGraphicDev, SCENE_SB, );
		return;
	}
}

void CSpaceField3::Event_Bounce_Switch(PxTriggerPair* pairs)
{
	CSpaceTube* pTube = ToTrigger(pairs->triggerShape->getActor()->getName())->Is<CSpaceTube>();
	if (nullptr == pTube)
		return;

	if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
	{
		if (pairs->otherShape->getSimulationFilterData().word0 & FilterGroup::eMay)
		{
			auto* pMay = (CMay_Space*)Engine::Get_GameObject(L"GameObject", L"May")->Is<CMay_Space>();
			pMay->Set_OnPlatform(true);
		}
		else if (pairs->otherShape->getSimulationFilterData().word0 & FilterGroup::eCody)
		{
			auto* pCody = (CCody_Space*)Engine::Get_GameObject(L"GameObject", L"Cody")->Is<CCody_Space>();
			pCody->Set_OnPlatform(true);
		}
	}
	else if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_LOST)
	{
		if (pairs->otherShape->getSimulationFilterData().word0 & FilterGroup::eMay)
		{
			auto* pMay = (CMay_Space*)Engine::Get_GameObject(L"GameObject", L"May")->Is<CMay_Space>();
			pMay->Set_OnPlatform(false);
		}
		else if (pairs->otherShape->getSimulationFilterData().word0 & FilterGroup::eCody)
		{
			auto* pCody = (CCody_Space*)Engine::Get_GameObject(L"GameObject", L"Cody")->Is<CCody_Space>();
			pCody->Set_OnPlatform(false);
		}
	}
}



void CSpaceField3::Event_Float(PxTriggerPair* pairs)
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

void CSpaceField3::Event_Spring(PxTriggerPair * pairs)
{
	CSpaceSpringBoardSpring* pSpring = Engine::Get_GameObject(L"GameObject", L"SpaceSpring")->Is<CSpaceSpringBoardSpring>();

	if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_LOST)
	{
		CGameObject* pObj = ToObj<CGameObject>(pairs->otherShape->getName());
		if (pairs->otherShape->getSimulationFilterData().word0 == FilterGroup::eCody)
		{
			pSpring->Set_Weight(true, 0);
		}
		else
		{
			pSpring->Set_Weight(false, 0);
		}
	}
}

void CSpaceField3::Event_Dead(PxTriggerPair * pairs)
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

HRESULT CSpaceField3::Ready_Environment_Layer(const _tchar * pLayerTag)
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
//#스테이지	스페이스 필드3
HRESULT CSpaceField3::Ready_GameObject_Layer(const _tchar * pLayerTag)
{
	Engine::CLayer*			pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	OBJINFO tObj;
	ZeroMemory(&tObj, sizeof(OBJINFO));
	tObj.eSceneID = RESOURCE_SF3;
	// 오브젝트 추가
	Engine::CGameObject*		pGameObject = nullptr;
	tObj.vScale = { 0.01f, 0.01f ,0.01f };


	//씬 체인지.
	pGameObject = CSceneChangeSpace::Create(m_pGraphicDev, SCENE_SF, { -2.5f, 5.f,  0.f }, 4.f, 4.f, 4.f, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SceneChange_SF3_To_SF", pGameObject, this), E_FAIL);
	pGameObject->Is<CSceneChangeSpace>()->Set_Next_Scene_StartPos({ 190.5f, 99.1f, -21.f });

	// CodyCamera
	pGameObject = CCodyCamera::Create(m_pGraphicDev, &_vec3(0.f, 10.f, -5.f), &_vec3(0.f, 0.f, 1.f), &_vec3(0.f, 1.f, 0.f), D3DXToRadian(60.f), _float(WINCX) / WINCY, 0.1f, 1000.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CodyCamera", pGameObject, this), E_FAIL);

	// MayCamera
	pGameObject = CMayCamera::Create(m_pGraphicDev, &_vec3(0.f, 10.f, -5.f), &_vec3(0.f, 0.f, 1.f), &_vec3(0.f, 1.f, 0.f), D3DXToRadian(60.f), _float(WINCX) / WINCY, 0.1f, 1000.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MayCamera", pGameObject, this), E_FAIL);
	//780.886, 10794.9, 2694.33
	tObj.vPos = { 97.97f,22.59f,37.8f };//씬마지막
	tObj.vPos = { 7.8f,26.9f,107.19f };//씬처음

	//tObj.vPos = { 147.31f,2.f,14.15f };
	//tObj.vPos = { 170.f ,40.f, -165.f }; // 박스.


	//tObj.vPos = { 125.f ,30.f, -31.f };
	//tObj.vPos = { 122.f, 23.f, -38.f };


	tObj.vPos = { 140.31f,2.f,14.15f }; //시소
	tObj.vPos = { 161.6f ,25.f, -17.f }; //제네레이터 1번.
	tObj.vPos = { 172.9f ,50.f, -240.8f }; //제네레이터 2번.
	tObj.vPos = { 0.f,5.f,0.f };


	pGameObject = CCody_Space::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Cody", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eCody, FilterGroup::eGround | FilterGroup::eWall);



	//tObj.vPos = { 178.f, 0.927f, -178.725f }; // 박스.
	//tObj.vPos = { 158.456f,2.f,-10.724f }; //시소
	tObj.vPos = { 161.6f ,10.f, -17.f }; //제네레이터 1번.
	tObj.vPos = { 172.9f ,50.f, -240.8f }; //제네레이터 2번.
	tObj.vPos = { 0.f,5.f,0.f };

	pGameObject = CMay_Space::Create(m_pGraphicDev, &tObj);
	//pGameObject->Is<CMay_Space>()->Set_Up({ -1.0f,0.f,0.f });
	pGameObject->Is<CMay_Space>()->Set_Up({ 0.f,1.f,0.f });
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"May", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMay, FilterGroup::eGround | FilterGroup::eWall);

	//#설정해야함 헤드 위치
	tObj.vPos = { 172.57f ,45.0f, -219.33f };
	tObj.vAngle = { 0.f,3.13f,0.f };
	pGameObject = CRobotHead::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"GeneratorHead", pGameObject, this), E_FAIL);

	tObj.vPos = { 158.001709f, 20.643457f, -16.568203f };
	tObj.vAngle = { 1.635f,0.f,1.76f };
	pGameObject = CRobotHead::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"GeneratorHead2", pGameObject, this), E_FAIL);


	tObj.vPos = { 122.08f,-2.f,-24.57f };
	tObj.vAngle = { 0.f,180.f,0.f };
	tObj.vScale = { 0.01f, 0.01f ,0.01f };
	pGameObject = CSpaceSpringBoardSpring::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SpaceSpring", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eOgre, FilterGroup::eCody | FilterGroup::eMay);

	//시소
	tObj.vPos = { 117.09f,-0.3f,-17.13f };
	tObj.vAngle = { 0.f,90.f,0.f };
	tObj.vScale = { 0.01f, 0.01f ,0.01f };
	pGameObject = CSpaceSeesaw::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SpaceSeesaw", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eButton, FilterGroup::eCody | FilterGroup::eMay);


	//시소
	tObj.vPos = { 145.f,-0.3f,9.13f };
	tObj.vAngle = { 0.f,0.f,0.f };
	tObj.vScale = { 0.01f, 0.01f ,0.01f };
	pGameObject = CSpaceSeesaw::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SpaceSeesaw2", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eButton, FilterGroup::eCody | FilterGroup::eMay);

	//유리박스 

	CGameObject* pGlassBox = nullptr;
	//tObj.vPos = { 162.043f,14.1f,-7.857f };
	tObj.vPos = { 160.781f,14.1f,-7.857f };
	tObj.vAngle = { 0.f,0.f,0.f };
	tObj.vScale = { 0.01f, 0.01f ,0.01f };
	tObj.vTriggerPos = { 160.858f,15.891f,-7.742f };
	tObj.vTriggerScale = { 1.25f, 0.55f, 1.15f };
	pGameObject = pGlassBox = CSpaceGlassBox::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"GlassBox", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eRideObj, FilterGroup::eCody | FilterGroup::eMay);

	tObj.vPos = { 161.013f,14.1f,-6.3f };
	tObj.vAngle = { 0.f,90.f,0.f };
	tObj.vScale = { 0.01f, 0.01f ,0.01f };
	tObj.vTriggerPos = { 161.0f,14.1f,-5.455f };
	tObj.vTriggerScale = { 1.0f, 1.6f, 1.0f };
	pGameObject = CSpaceGlassHandle::Create(m_pGraphicDev, _vec3(0.0f, 0.0f, -1.0f), &tObj);
	pGameObject->Set_InteractObj(pGlassBox);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"GlassHandle", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eButton, FilterGroup::eCody | FilterGroup::eMay);


	//버튼상자

	//튜브
	tObj.vScale = _vec3{ 0.01f,0.01f,0.01f };
	tObj.vAngle = { 0.f, 0.f, 0.f };
	tObj.vPos = { 42.f ,-3.f, 2.f };

	pGameObject = CSpaceTube::Create(m_pGraphicDev, &tObj, TubeColor_Blue);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"TubeColor_Blue", pGameObject, this), E_FAIL);

	tObj.vPos = { 52.f ,-1.f, 6.f };
	pGameObject = CSpaceTube::Create(m_pGraphicDev, &tObj, TubeColor_Orange);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"TubeColor_Orange", pGameObject, this), E_FAIL);

	tObj.vPos = { 50.f ,-2.f, -2.f };
	pGameObject = CSpaceTube::Create(m_pGraphicDev, &tObj, TubeColor_Purple);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"TubeColor_Purple", pGameObject, this), E_FAIL);

	tObj.vPos = { 62.f , 0.f, -8.f };
	pGameObject = CSpaceTube::Create(m_pGraphicDev, &tObj, TubeColor_Blue);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"TubeColor_Blue2", pGameObject, this), E_FAIL);

	tObj.vPos = { 62.f , 1.f, 10.f };
	pGameObject = CSpaceTube::Create(m_pGraphicDev, &tObj, TubeColor_Purple);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"TubeColor_Purple2", pGameObject, this), E_FAIL);

	tObj.vPos = { 72.f ,-3.f, 6.f };
	pGameObject = CSpaceTube::Create(m_pGraphicDev, &tObj, TubeColor_Blue);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"TubeColor_Blue3", pGameObject, this), E_FAIL);


	tObj.vPos = { 72.f ,-3.f, -2.f };
	pGameObject = CSpaceTube::Create(m_pGraphicDev, &tObj, TubeColor_Orange);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"TubeColor_Orange2", pGameObject, this), E_FAIL);

	_vec3 TriggerPos;
	_vec3 StartPos;
	CGameObject* pLever = nullptr;

	//첫 배터리 세트.
	tObj.vScale = _vec3{ 0.01f,0.01f,0.01f };
	tObj.vAngle = { 30.f, 0.f, 180.f };
	tObj.vTriggerScale = { 1.f, 1.f, 1.f };
	tObj.vPos = { 160.9f ,19.05f, -16.5f };
	TriggerPos = tObj.vPos + _vec3(-0.f, -1.f, -1.f);
	StartPos = tObj.vPos + _vec3(1.f, -0.5f, 0.f);

	pLever = CGeneratorLever::Create(m_pGraphicDev, &tObj, TriggerPos, StartPos, L"GeneratorBattery", 1, _vec3(0.0f, 1.0f, 0.0f));
	pLever->Is<CGeneratorLever>()->Set_LeverDir(1);
	NULL_CHECK_RETURN(pLever, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"GeneratorLever", pLever, this), E_FAIL);

	tObj.vScale = _vec3{ 0.01f,0.01f,0.01f };
	tObj.vAngle = { 0.f, 20.f, 0.f };
	tObj.vTriggerScale = { 0.3f, 0.3f, 0.2f };
	tObj.vPos = { 159.55f ,19.05f, -16.15f };
	TriggerPos = tObj.vPos + _vec3(-0.2f, -0.115f, -0.75f);
	StartPos = tObj.vPos + _vec3(-0.45f, -0.14f, -0.756f);
	CGameObject* pBattery = nullptr;
	pBattery = CGeneratorBattery::Create(m_pGraphicDev, &tObj, TriggerPos, StartPos);
	NULL_CHECK_RETURN(pBattery, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"GeneratorBattery", pBattery, this), E_FAIL);
	pLever->Set_InteractObj(pBattery);


	tObj.vPos = { 159.56f, 18.87f, -16.6f };
	tObj.vAngle = { 0.f, D3DXToRadian(180.f), 0.f };
	pGameObject = CBatteryBox::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"BatteryBox", pGameObject, this), E_FAIL);
	pBattery->Set_InteractObj(pGameObject);

	tObj.vAngle = { 0.f, 90.f, 0.f };
	tObj.vPos = { 127.3f, 27.f, -41.2f };
	pGameObject = CWarpGateStar::Create(m_pGraphicDev, &tObj, 1);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"WarbGate_Star", pGameObject, this), E_FAIL);
	pLever->Set_InteractObj(pGameObject);


	//옆에 큰 행성 문 스타
	tObj.vAngle = { 0.f, 0.f, 0.f };
	tObj.vPos = { 97.5f ,8.f, -246.f };
	pGameObject = CWarpGateStar::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"WarbGate2_Star", pGameObject, this), E_FAIL);
	pLever->Set_InteractObj(pGameObject);

	// _vec3(122.f, 23.f, -42.14f)
	pGameObject = CTeleportSpace::Create(m_pGraphicDev, _vec3(104.93f, 13.f, -243.52f),_vec3(122.f, 23.f, -45.14f), 4.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"GateTeleport", pGameObject, this), E_FAIL);
	pLever->Set_InteractObj(pGameObject);


	//옆에 제네레이터와 시작 문
	tObj.vScale = _vec3{ 0.01f,0.01f,0.01f };
	tObj.vAngle = { -90.f, 0.f, 30.f };
	tObj.vTriggerScale = { 0.3f, 0.3f, 0.2f };
	tObj.vPos = { 172.9f ,42.f, -220.8f };
	//tObj.vPos = { 172.9f ,51.f, -224.8f };
	TriggerPos = tObj.vPos + _vec3(-0.2f, -0.115f, -0.75f);
	StartPos = tObj.vPos + _vec3(-0.45f, -1.14f, -0.756f);

	pLever = CGeneratorLever::Create(m_pGraphicDev, &tObj, TriggerPos, StartPos, L"GeneratorBattery2");
	NULL_CHECK_RETURN(pLever, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"GeneratorLever2", pLever, this), E_FAIL);

	tObj.vScale = _vec3{ 0.01f,0.01f,0.01f };
	tObj.vAngle = { 0.f, -70.f, 0.f };
	tObj.vTriggerScale = { 0.3f, 0.3f, 0.2f };
	tObj.vPos = { 172.1f ,43.35f, -218.5f };
	//tObj.vPos = { 172.1f ,52.35f, -222.5f };
	TriggerPos = tObj.vPos + _vec3(+0.75f, -0.115f, -0.3f);
	StartPos = tObj.vPos + _vec3(+0.756f, -0.14f, -0.45f);
	pBattery = CGeneratorBattery::Create(m_pGraphicDev, &tObj, TriggerPos, StartPos, _vec3(0.f, 0.f, 1.f));
	NULL_CHECK_RETURN(pBattery, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"GeneratorBattery2", pBattery, this), E_FAIL);
	pLever->Set_InteractObj(pBattery);

	tObj.vPos = { 172.52f, 43.17f, -218.51f };
	tObj.vAngle = { 0.f, D3DXToRadian(90.f), 0.f };
	pGameObject = CBatteryBox::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"BatteryBox2", pGameObject, this), E_FAIL);
	pBattery->Set_InteractObj(pGameObject);


	tObj.vAngle = { 0.f, 0.f, 0.f };
	tObj.vPos = { -0.f ,9.f, -4.f };
	pGameObject = CWarpGateStar::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"WarbGate3_Star", pGameObject, this), E_FAIL);
	pLever->Set_InteractObj(pGameObject);

	tObj.vAngle = { 0.f, 0.f, 0.f };
	tObj.vPos = { -0.f ,9.f, -4.f };
	pGameObject = CWarpGateStar::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"WarbGate3_Star2", pGameObject, this), E_FAIL);
	pLever->Set_InteractObj(pGameObject);

	pGameObject = CTeleportSpace::Create(m_pGraphicDev, _vec3(5.f, 8.f, 0.f), _vec3(93.f, 13.f, -243.52f), 4.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"GateTeleport2", pGameObject, this), E_FAIL);
	pLever->Set_InteractObj(pGameObject);


	CGameObject* pBox = nullptr;
	tObj.vPos = { 172.417f ,0.927f, -235.3427f };
	pGameObject = pBox = CGroundPoundBox::Create(m_pGraphicDev, &tObj);
	pGameObject->Set_ObjTag(L"Box");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"GroundPoundBox", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eRideObj, FilterGroup::ePlayer);


	// 버튼1
	tObj.vPos.x -= 0.3f;
	pGameObject = CGroundPoundButton::Create(m_pGraphicDev, &tObj);
	pGameObject->Set_InteractObj(pBox);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"GroundPoundButton", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eButton, FilterGroup::ePlayer);

	// 박스2
	tObj.vPos = { 172.417f ,0.927f, -243.3427f };
	pGameObject = pBox = CGroundPoundBox::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"GroundPoundBox2", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eRideObj, FilterGroup::ePlayer);

	// 버튼2
	tObj.vPos.x -= 0.3f;
	pGameObject = CGroundPoundButton::Create(m_pGraphicDev, &tObj);
	pGameObject->Set_InteractObj(pBox);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"GroundPoundButton2", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eButton, FilterGroup::ePlayer);

	// 박스3
	tObj.vPos = { 172.417f ,4.927f, -235.3427f };
	pGameObject = pBox = CGroundPoundBox::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"GroundPoundBox3", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eRideObj, FilterGroup::ePlayer);

	// 버튼3
	tObj.vPos.x -= 0.3f;
	pGameObject = CGroundPoundButton::Create(m_pGraphicDev, &tObj);
	pGameObject->Set_InteractObj(pBox);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"GroundPoundButton3", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eButton, FilterGroup::ePlayer);

	// 박스4
	tObj.vPos = { 172.417f ,4.927f, -243.3427f };
	pGameObject = pBox = CGroundPoundBox::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"GroundPoundBox4", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eRideObj, FilterGroup::ePlayer);

	// 버튼4
	tObj.vPos.x -= 0.3f;
	pGameObject = CGroundPoundButton::Create(m_pGraphicDev, &tObj);
	pGameObject->Set_InteractObj(pBox);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"GroundPoundButton4", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eButton, FilterGroup::ePlayer);

	// 박스5
	tObj.vPos = { 172.417f ,8.927f, -235.3427f };
	pGameObject = pBox = CGroundPoundBox::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"GroundPoundBox5", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eRideObj, FilterGroup::ePlayer);

	// 버튼5
	tObj.vPos.x -= 0.3f;
	pGameObject = CGroundPoundButton::Create(m_pGraphicDev, &tObj);
	pGameObject->Set_InteractObj(pBox);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"GroundPoundButton5", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eButton, FilterGroup::ePlayer);

	// 박스
	tObj.vPos = { 172.417f ,8.927f, -243.3427f };
	pGameObject = pBox = CGroundPoundBox::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"GroundPoundBox6", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eRideObj, FilterGroup::ePlayer);


	// 버튼
	tObj.vPos.x -= 0.3f;
	pGameObject = CGroundPoundButton::Create(m_pGraphicDev, &tObj);
	pGameObject->Set_InteractObj(pBox);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"GroundPoundButton6", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eButton, FilterGroup::ePlayer);


	// 박스
	tObj.vPos = { 172.417f ,12.927f, -235.3427f };
	pGameObject = pBox = CGroundPoundBox::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"GroundPoundBox7", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eRideObj, FilterGroup::ePlayer);


	// 버튼
	tObj.vPos.x -= 0.3f;
	pGameObject = CGroundPoundButton::Create(m_pGraphicDev, &tObj);
	pGameObject->Set_InteractObj(pBox);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"GroundPoundButton7", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eButton, FilterGroup::ePlayer);


	// 박스
	tObj.vPos = { 172.417f ,12.927f, -243.3427f };
	pGameObject = pBox = CGroundPoundBox::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"GroundPoundBox8", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eRideObj, FilterGroup::ePlayer);


	// 버튼
	tObj.vPos.x -= 0.3f;
	pGameObject = CGroundPoundButton::Create(m_pGraphicDev, &tObj);
	pGameObject->Set_InteractObj(pBox);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"GroundPoundButton8", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eButton, FilterGroup::ePlayer);


	// 박스
	tObj.vPos = { 172.417f ,16.927f, -235.3427f };
	pGameObject = pBox = CGroundPoundBox::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"GroundPoundBox9", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eRideObj, FilterGroup::ePlayer);


	// 버튼
	tObj.vPos.x -= 0.3f;
	pGameObject = CGroundPoundButton::Create(m_pGraphicDev, &tObj);
	pGameObject->Set_InteractObj(pBox);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"GroundPoundButton9", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eButton, FilterGroup::ePlayer);


	//박스
	tObj.vPos = { 172.417f ,16.927f, -243.3427f };
	pGameObject = pBox = CGroundPoundBox::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"GroundPoundBox10", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eRideObj, FilterGroup::ePlayer);


	// 버튼
	tObj.vPos.x -= 0.3f;
	pGameObject = CGroundPoundButton::Create(m_pGraphicDev, &tObj);
	pGameObject->Set_InteractObj(pBox);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"GroundPoundButton10", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eButton, FilterGroup::ePlayer);


	// 박스
	tObj.vPos = { 172.417f ,20.927f, -235.3427f };
	pGameObject = pBox = CGroundPoundBox::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"GroundPoundBox11", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eRideObj, FilterGroup::ePlayer);

	// 버튼
	tObj.vPos.x -= 0.3f;
	pGameObject = CGroundPoundButton::Create(m_pGraphicDev, &tObj);
	pGameObject->Set_InteractObj(pBox);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"GroundPoundButton11", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eButton, FilterGroup::ePlayer);

	//박스
	tObj.vPos = { 172.417f ,20.927f, -243.3427f };
	pGameObject = pBox = CGroundPoundBox::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"GroundPoundBox12", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eRideObj, FilterGroup::ePlayer);

	// 버튼
	tObj.vPos.x -= 0.3f;
	pGameObject = CGroundPoundButton::Create(m_pGraphicDev, &tObj);
	pGameObject->Set_InteractObj(pBox);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"GroundPoundButton12", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eButton, FilterGroup::ePlayer);

	// 박스
	tObj.vPos = { 172.417f ,24.927f, -235.3427f };
	pGameObject = pBox = CGroundPoundBox::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"GroundPoundBox13", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eRideObj, FilterGroup::ePlayer);

	// 버튼
	tObj.vPos.x -= 0.3f;
	pGameObject = CGroundPoundButton::Create(m_pGraphicDev, &tObj);
	pGameObject->Set_InteractObj(pBox);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"GroundPoundButton13", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eButton, FilterGroup::ePlayer);

	// 버튼
	tObj.vPos = { 172.417f ,24.927f, -243.3427f };
	pGameObject = pBox = CGroundPoundBox::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"GroundPoundBox14", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eRideObj, FilterGroup::ePlayer);

	// 버튼
	tObj.vPos.x -= 0.3f;
	pGameObject = CGroundPoundButton::Create(m_pGraphicDev, &tObj);
	pGameObject->Set_InteractObj(pBox);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"GroundPoundButton14", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eButton, FilterGroup::ePlayer);


	//tObj.vScale = _vec3{ 0.01f,0.01f,0.01f };
	//tObj.vAngle = { 0, 0.f, 0.f };

	//tObj.eMeshType = 1;
	tObj.eRenderType = RENDER_NONALPHA;
	//tObj.vPos = { 0.f,0.f,0.f };

	lstrcpy(tObj.tagMesh, L"Area1_GravityCorner");
	tObj.vPos = { 0.f,0.f,0.f };
	tObj.vScale = _vec3{ 0.01f,0.01f,0.01f };
	//tObj.vAngle = { 0.f,D3DXToRadian(-90.f),0.f };
	pGameObject = CGravityFloor::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Field4", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eGravityFloorIN, FilterGroup::eCody | FilterGroup::eMay);

	lstrcpy(tObj.tagMesh, L"Area2_Gravity_OutCorner");
	pGameObject = CGravityFloor::Create(m_pGraphicDev, &tObj, GRAVITY_OUTCORNER);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Field9", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eGravityFloor, FilterGroup::eCody | FilterGroup::eMay);

	lstrcpy(tObj.tagMesh, L"Area2_Gravity_InCorner");
	pGameObject = CGravityFloor::Create(m_pGraphicDev, &tObj, GRAVITY_INCORNER);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Field10", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eGravityFloorIN, FilterGroup::eCody | FilterGroup::eMay);


	lstrcpy(tObj.tagMesh, L"Area1_Platform");
	pGameObject = CMap::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Field", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eGround | FilterGroup::eWall, FilterGroup::eCody | FilterGroup::eMay);



	lstrcpy(tObj.tagMesh, L"Area1_Platform2");
	pGameObject = CMap::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Field0", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eGround | FilterGroup::eWall, FilterGroup::eCody | FilterGroup::eMay);


	tObj.vPos = { 162.f, 20.50f, -16.5f };
	tObj.vAngle = { 180.f, 0.f, 90.f };
	lstrcpy(tObj.tagMesh, L"Area1_Generator");
	pGameObject = CSpaceGenerator::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Field2", pGameObject, this), E_FAIL);

	tObj.vPos = { 0.f,0.f,0.f };
	tObj.vAngle = { 0.f, 0.f, 0.f };
	lstrcpy(tObj.tagMesh, L"Area1_Gravity");
	pGameObject = CMap::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Field3", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eGround | FilterGroup::eWall, FilterGroup::eCody | FilterGroup::eMay);





	tObj.vScale = { 0.01f, 0.01f, 0.01f };
	tObj.vPos = { 0.f,0.f,0.f };
	tObj.vAngle = { 0.f, 0.f, 0.f };
	lstrcpy(tObj.tagMesh, L"Area1_Window");
	pGameObject = CMap::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Field5", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eGround | FilterGroup::eWall, FilterGroup::eCody | FilterGroup::eMay);


	lstrcpy(tObj.tagMesh, L"Area2_Platform");
	pGameObject = CMap::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Field6", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eGround | FilterGroup::eWall, FilterGroup::eCody | FilterGroup::eMay);




	tObj.vPos = { 172.52f, 40.90f, -219.35f };
	tObj.vAngle = { 0.f, 90.f, 0.f };
	lstrcpy(tObj.tagMesh, L"Area2_Generator");
	pGameObject = CSpaceGenerator::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Field7", pGameObject, this), E_FAIL);

	tObj.vPos = { 172.52f, 40.90f, -219.35f };
	tObj.vAngle = { 0.f, 90.f, 0.f };
	lstrcpy(tObj.tagMesh, L"GeneratorStair");
	pGameObject = CSpaceGenerator::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"GeneratorStair", pGameObject, this), E_FAIL);

	tObj.vPos = { 0.f,0.f,0.f };
	tObj.vAngle = { 0.f, 0.f, 0.f };
	lstrcpy(tObj.tagMesh, L"Area2_Gravity");
	pGameObject = CMap::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Field8", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eGround | FilterGroup::eWall, FilterGroup::eCody | FilterGroup::eMay);

	lstrcpy(tObj.tagMesh, L"Area2_Alpha");
	pGameObject = CMap::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Field11", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eGround | FilterGroup::eWall, FilterGroup::eCody | FilterGroup::eMay);



	tObj.vPos = { 0.f,0.f,0.f };
	tObj.vAngle = { 0.f, 0.f, 0.f };
	// 데드
	{
		// 1번
		// 맵전체
		tObj.vTriggerPos = { 180.0f, -20.0f, 0.0f };
		tObj.vTriggerScale = _vec3{ 200.0f, 1.0f, 50.0f };
		pGameObject = CDeadSpace::Create(m_pGraphicDev, L"", true, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DeadSpaceMap1", pGameObject, this), E_FAIL);

		// 맵전체
		tObj.vTriggerPos = { 180.0f, 60.0f, 0.0f };
		tObj.vTriggerScale = _vec3{ 200.0f, 1.0f, 50.0f };
		pGameObject = CDeadSpace::Create(m_pGraphicDev, L"", true, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DeadSpaceMap2", pGameObject, this), E_FAIL);

		// 맵전체
		tObj.vTriggerPos = { 180.0f, 20.0f, 50.0f };
		tObj.vTriggerScale = _vec3{ 200.0f, 40.0f, 1.0f };
		pGameObject = CDeadSpace::Create(m_pGraphicDev, L"", true, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DeadSpaceMap3", pGameObject, this), E_FAIL);

		// 맵전체
		tObj.vTriggerPos = { 180.0f, 20.0f, -50.0f };
		tObj.vTriggerScale = _vec3{ 200.0f, 40.0f, 1.0f };
		pGameObject = CDeadSpace::Create(m_pGraphicDev, L"", true, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DeadSpaceMap4", pGameObject, this), E_FAIL);

		// 맵전체
		tObj.vTriggerPos = { 380.0f, 20.0f, 0.0f };
		tObj.vTriggerScale = _vec3{ 1.0f, 40.0f, 50.0f };
		pGameObject = CDeadSpace::Create(m_pGraphicDev, L"", true, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DeadSpaceMap5", pGameObject, this), E_FAIL);

		// 맵전체
		tObj.vTriggerPos = { -20.0f, 20.0f, 00.0f };
		tObj.vTriggerScale = _vec3{ 1.0f, 40.0f, 50.0f };
		pGameObject = CDeadSpace::Create(m_pGraphicDev, L"", true, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DeadSpaceMap6", pGameObject, this), E_FAIL);

		//2번
		// 맵전체
		// 130 -230
		tObj.vTriggerPos = { 240.0f, 0.0f, -240.0f };
		tObj.vTriggerScale = _vec3{ 1.0f, 110.0f, 110.0f };
		pGameObject = CDeadSpace::Create(m_pGraphicDev, L"", true, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DeadSpaceMap7", pGameObject, this), E_FAIL);

		// 맵전체
		tObj.vTriggerPos = { 20.0f, 0.0f, -240.0f };
		tObj.vTriggerScale = _vec3{ 1.0f, 110.0f, 110.0f };
		pGameObject = CDeadSpace::Create(m_pGraphicDev, L"", true, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DeadSpaceMap8", pGameObject, this), E_FAIL);

		// 맵전체
		tObj.vTriggerPos = { 130.0f, -0.0f, -350.0f };
		tObj.vTriggerScale = _vec3{ 110.0f, 110.0f, 1.0f };
		pGameObject = CDeadSpace::Create(m_pGraphicDev, L"", true, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DeadSpaceMap9", pGameObject, this), E_FAIL);

		// 맵전체
		tObj.vTriggerPos = { 130.0f, -0.0f, -130.0f };
		tObj.vTriggerScale = _vec3{ 110.0f, 110.0f, 1.0f };
		pGameObject = CDeadSpace::Create(m_pGraphicDev, L"", true, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DeadSpaceMap10", pGameObject, this), E_FAIL);

		// 맵전체
		tObj.vTriggerPos = { 130.0f, 110.0f, -240.0f };
		tObj.vTriggerScale = _vec3{ 110.0f, 1.0f, 110.0f };
		pGameObject = CDeadSpace::Create(m_pGraphicDev, L"", true, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DeadSpaceMap11", pGameObject, this), E_FAIL);

		// 맵전체
		tObj.vTriggerPos = { 130.0f, -110.0f, -240.0f };
		tObj.vTriggerScale = _vec3{ 110.0f, 1.0f, 110.0f };
		pGameObject = CDeadSpace::Create(m_pGraphicDev, L"", true, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DeadSpaceMap12", pGameObject, this), E_FAIL);
	}

	// 세이브
	{
		tObj.vPos = { 85.0f, -2.0f, 2.8f };
		tObj.vScale = _vec3{ 1.0f, 20.0f, 20.0f };
		pGameObject = CSaveSpace::Create(m_pGraphicDev, { 214.8f, 23.055f, -1.126f }, L"Yellow_Bg", g_bRenderBox, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SavePoint1", pGameObject, this), E_FAIL);
		pGameObject->Is<CSaveSpace>()->Set_CodyRespwanPoint({ 122.36f, -2.0f, -1.9f });
		pGameObject->Is<CSaveSpace>()->Set_MayRespwanPoint({ 122.36f, -2.0f, -1.9f });
		pGameObject->Is<CSaveSpace>()->Set_MayRespwanUp({ 0.0f, 1.0f, 0.0f });

		tObj.vPos = { 123.0f, 20.0f, -30.3f };
		tObj.vScale = _vec3{ 20.0f, 10.0f, 1.0f };
		pGameObject = CSaveSpace::Create(m_pGraphicDev, { 214.8f, 23.055f, -1.126f }, L"Yellow_Bg", g_bRenderBox, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SavePoint2", pGameObject, this), E_FAIL);
		pGameObject->Is<CSaveSpace>()->Set_CodyRespwanPoint({ 123.0f, 15.0f, -30.3f });
		pGameObject->Is<CSaveSpace>()->Set_MayRespwanPoint({ 123.0f, 15.0f, -30.3f });
		pGameObject->Is<CSaveSpace>()->Set_MayRespwanUp({ 0.0f, 1.0f, 0.0f });

		tObj.vPos = { 150.752f, 0.83226f, -238.347f };
		tObj.vScale = _vec3{ 5.0f, 5.0f, 5.0f };
		pGameObject = CSaveSpace::Create(m_pGraphicDev, { 214.8f, 23.055f, -1.126f }, L"Yellow_Bg", g_bRenderBox, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SavePoint3", pGameObject, this), E_FAIL);
		pGameObject->Is<CSaveSpace>()->Set_CodyRespwanPoint({ 150.752f, 0.83226f, -238.347f });
		pGameObject->Is<CSaveSpace>()->Set_MayRespwanPoint({ 150.752f, 0.83226f, -238.347f });
		pGameObject->Is<CSaveSpace>()->Set_MayRespwanUp({ 0.0f, 1.0f, 0.0f });

		tObj.vPos = { 172.38f, 33.0f, -242.55f };
		tObj.vScale = _vec3{ 10.0f, 10.0f, 1.0f };
		pGameObject = CSaveSpace::Create(m_pGraphicDev, { 214.8f, 23.055f, -1.126f }, L"Yellow_Bg", g_bRenderBox, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SavePoint4", pGameObject, this), E_FAIL);
		pGameObject->Is<CSaveSpace>()->Set_CodyRespwanPoint({ 172.38f, 33.0f, -242.55f });
		pGameObject->Is<CSaveSpace>()->Set_MayRespwanPoint({ 172.38f, 33.0f, -242.55f });
		pGameObject->Is<CSaveSpace>()->Set_MayRespwanUp({ 0.0f, 1.0f, 0.0f });
	}

	// 
	tObj.vScale = { 0.015f, 0.015f, 0.015f };
	tObj.vPos = { 137.02f, 0.0f, -239.35f };
	tObj.vAngle = { 90.f, 0.0, 0.f };
	pGameObject = CSpaceRing::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SpaceRing", pGameObject, this), E_FAIL);
	pGameObject->Is<CSpaceRing>()->Set_Rot(CSpaceRing::RotY);

	tObj.vPos = { 137.02f, 0.0f, -239.35f };
	tObj.vAngle = { 0.f, 0.0f, 0.f };
	pGameObject = CSpaceRing::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SpaceRing2", pGameObject, this), E_FAIL);
	pGameObject->Is<CSpaceRing>()->Set_Rot(CSpaceRing::RotZ);
	/*pGameObject->Is<CSpaceRing>()->Set_RotSpeed()*/

	m_mapLayer.emplace(pLayerTag, pLayer);


	return S_OK;
}

HRESULT CSpaceField3::Ready_LightInfo(void)
{
	D3DLIGHT9				tLightInfo;
	ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));
	tLightInfo.Type = D3DLIGHT_POINT;
	tLightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Ambient = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.f);


	tLightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Position = _vec3(-8.86f, 9.42f, 2.f);
	tLightInfo.Range = 30.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 0);

	tLightInfo.Diffuse = D3DXCOLOR(1.f, 0.8f, 0.6f, 1.f);
	tLightInfo.Position = _vec3(39.85f, 23.17f, 23.92f);
	tLightInfo.Range = 60.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 1);

	tLightInfo.Diffuse = D3DXCOLOR(1.f, 0.8f, 0.6f, 1.f);
	tLightInfo.Position = _vec3(39.85f, 23.17f, -23.97f);
	tLightInfo.Range = 60.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 2);

	tLightInfo.Diffuse = D3DXCOLOR(1.f, 0.8f, 0.6f, 1.f);
	tLightInfo.Position = _vec3(96.00f, 33.74f, -10.00f);
	tLightInfo.Range = 60.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 3);

	tLightInfo.Diffuse = D3DXCOLOR(1.f, 0.8f, 0.6f, 1.f);
	tLightInfo.Position = _vec3(126.25f, 24.12f, 24.00f);
	tLightInfo.Range = 60.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 4);

	tLightInfo.Diffuse = D3DXCOLOR(1.f, 0.8f, 0.6f, 1.f);
	tLightInfo.Position = _vec3(158.83f, 25.50f, 24.00f);
	tLightInfo.Range = 60.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 5);

	tLightInfo.Diffuse = D3DXCOLOR(1.f, 0.8f, 0.6f, 1.f);
	tLightInfo.Position = _vec3(158.83f, 25.50f, -24.50f);
	tLightInfo.Range = 60.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 6);

	tLightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Position = _vec3(-152.94f, 14.81f, -7.22f);
	tLightInfo.Range = 30.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 7);

	tLightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Position = _vec3(122.00f, 26.57f, -53.20f);
	tLightInfo.Range = 30.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 8);

	tLightInfo.Diffuse = D3DXCOLOR(1.f, 0.8f, 0.6f, 1.f);
	tLightInfo.Position = _vec3(122.00f, 31.13f, -27.17f);
	tLightInfo.Range = 60.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 9);

	tLightInfo.Diffuse = D3DXCOLOR(1.f, 0.8f, 0.6f, 1.f);
	tLightInfo.Position = _vec3(58.78f, 16.28f, 1.0f);
	tLightInfo.Range = 30.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 10);


	//////////////////////////////////////////////////////////////////////////


	tLightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Position = _vec3(88.39f, 7.68f, -240.21f);
	tLightInfo.Range = 30.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 11);

	tLightInfo.Diffuse = D3DXCOLOR(1.f, 0.8f, 0.6f, 1.f);
	tLightInfo.Position = _vec3(119.08f, 40.05f, -280.03f);
	tLightInfo.Range = 80.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 12);

	tLightInfo.Diffuse = D3DXCOLOR(1.f, 0.8f, 0.6f, 1.f);
	tLightInfo.Position = _vec3(187.28f, 18.38f, -238.50f);
	tLightInfo.Range = 30.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 13);

	tLightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Position = _vec3(172.86f, 51.85f, -218.50f);
	tLightInfo.Range = 40.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 14);

	tLightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Position = _vec3(248.75f, 89.98f, -122.57f);
	tLightInfo.Range = 200.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 15);

	tLightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Position = _vec3(-6.35f, -122.02f, -239.35f);
	tLightInfo.Range = 150.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 16);

	tLightInfo.Diffuse = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.f);
	tLightInfo.Position = _vec3(169.10f, 31.82f, -272.14f);
	tLightInfo.Range = 25.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 17);

	tLightInfo.Diffuse = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.f);
	tLightInfo.Position = _vec3(159.61f, 14.99f, -235.85f);
	tLightInfo.Range = 20.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 18);

	tLightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Position = _vec3(137.02f, 94.92f, -239.35f);
	tLightInfo.Range = 80.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 19);

	tLightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Position = _vec3(137.02f, -90.31f, -239.35f);
	tLightInfo.Range = 80.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 20);

	tLightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Position = _vec3(237.02f, 10.f, -239.35f);
	tLightInfo.Range = 70.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 21);

	tLightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Position = _vec3(37.02f, 10.f, -239.35f);
	tLightInfo.Range = 70.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 22);

	tLightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Position = _vec3(137.02f, 10.f, -339.35f);
	tLightInfo.Range = 70.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 23);

	tLightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Position = _vec3(137.02f, 10.f, -139.35f);
	tLightInfo.Range = 70.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 24);

	tLightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Position = _vec3(207.02f, 10.f, -169.35f);
	tLightInfo.Range = 70.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 25);

	tLightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Position = _vec3(67.02f, 10.f, -179.35f);
	tLightInfo.Range = 70.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 26);

	tLightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Position = _vec3(67.02f, 10.f, -309.35f);
	tLightInfo.Range = 70.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 27);

	tLightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Position = _vec3(207.02f, 10.f, -309.35f);
	tLightInfo.Range = 70.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 28);

	return S_OK;
}

CSpaceField3 * CSpaceField3::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSpaceField3*		pInstance = new CSpaceField3(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
		Safe_Release(pInstance);

	return pInstance;
}

void CSpaceField3::Free(void)
{
	// 삭제

	for (_uint i = 0; i < m_vecObject.size(); ++i)
		Safe_Delete_Array(m_vecObject[i]);

	m_vecObject.clear();
	m_vecObject.shrink_to_fit();
	Engine::Delete_AllResource(RESOURCE_SF3);
	Engine::CScene::Free();
}


HRESULT CSpaceField3::Ready_Interact_Layer(const _tchar* szLayer)
{
	Engine::CLayer*			pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	OBJINFO tObj;
	ZeroMemory(&tObj, sizeof(OBJINFO));
	tObj.eSceneID = RESOURCE_SF3;
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

void CSpaceField3::Effect_Warp()
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
	tParticle.bRotAdd = false;
	tParticle.bBill = false;

	tParticle.vPos = _vec3(-1.f, 9.f, 2.f);
	tParticle.vRot = { 0.f, D3DXToRadian(90.f), 0.f };
	Engine::Emit_ImageParticle(tParticle, 45, 33);

	tParticle.vRot = { 0.f, D3DXToRadian(180.f), 0.f };
	tParticle.vPos = _vec3(122.f, 26.48f, -42.29f);
	Engine::Emit_ImageParticle(tParticle, 45, 33);

	tParticle.vRot = { 0.f, D3DXToRadian(90.f), 0.f };
	tParticle.vPos = _vec3(96.61f, 7.8f, -240.21f);
	Engine::Emit_ImageParticle(tParticle, 45, 33);

}