#include "stdafx.h"
#include "SpaceField2.h"
#include "Export_Function.h"
#include "UI.h"
#include "Cody.h"
#include "May.h"
#include "CodyCamera.h"
#include "MayCamera.h"
#include "StaticObject.h"
#include "SpaceField2.h"
#include "Map.h"
#include "SceneChangeSpace.h"
#include "DeadSpace.h"
#include "May_Space.h"
#include "Cody_Space.h"
#include "GroundBounce.h"
#include "SpaceCup.h"
#include "SpaceWall.h"
#include "SpacePlatform.h"
#include "SpaceValve.h"
#include "SpaceValveSpring.h"
#include "GroundBouncePanel.h"
#include "GeneratorBattery.h"
#include "SpacePlatformWall.h"
#include "Lever.h"
#include "WarpGateStar.h"
#include "SpaceLever.h"
#include "GeneratorBattery.h"
#include "GeneratorLever.h"
#include "GravityFloor.h"
#include "RobotHead.h"
#include "PosChange.h"
#include "SpaceLaser.h"
#include "TriggerView.h"
#include "SaveSpace.h"
#include "BatteryBox.h"
#include "SpaceGenerator.h"


CSpaceField2::CSpaceField2(LPDIRECT3DDEVICE9 pDevice)
	: CScene(pDevice)
{
}

CSpaceField2::~CSpaceField2()
{
}

HRESULT CSpaceField2::Ready_Scene()
{
	Engine::Set_SoundScene(RESOURCE_SF);

	Engine::End_Split();
	//Engine::Start_Split();
	Engine::Set_BlendPass(0);

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
	FAILED_CHECK_RETURN(Ready_Valve_Layer(L"Valve"), E_FAIL);

	//FAILED_CHECK_RETURN(Ready_LoadMap_Layer(L"LoadObject", L"../../Data/ITT_Test/Test.dat"), E_FAIL);

	Ready_LightInfo();

	Engine::StopAll();
	Engine::PlayBGM(L"SpaceField2_BGM.wav", 1.f);

	return S_OK;
}

_int CSpaceField2::Update_Scene(const _float & fTimeDelta)
{
	if (m_bStart)
	{
		Effect_Warp();

		m_bStart = false;
	}

	m_pScene->simulate(fTimeDelta);
	m_pScene->fetchResults(true);
	_int iExit = Engine::CScene::Update_Scene(fTimeDelta);
	SceneChange(fTimeDelta);

	
	if (Engine::Key_Down(DIK_Y) && GetFocus() == g_hWnd)
	{
		CHANGE_SCENE(m_pGraphicDev, SCENE_SF3,OBJ_NOEVENT);
		return 0;
	}
	return iExit;
}

_int CSpaceField2::LateUpdate_Scene(const _float & fTimeDelta)
{
	_int iExit = Engine::CScene::LateUpdate_Scene(fTimeDelta);

	return iExit;
}

void CSpaceField2::Render_Scene()
{

}

void CSpaceField2::onConstraintBreak(PxConstraintInfo* constraints, PxU32 count)
{

	return;
}

void CSpaceField2::onWake(PxActor** actors, PxU32 count)
{
	OutputDebugString(L"깼음");
	return;
}

void CSpaceField2::onSleep(PxActor** actors, PxU32 count)
{
	OutputDebugString(L"잠듬");

	return;
}

void CSpaceField2::onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs)
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

void CSpaceField2::onTrigger(PxTriggerPair* pairs, PxU32 count)
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
			case TRIGGER_LEVER_CODY:
				Event_Lever_Cody(&Trigger);
				break;
			case TRIGGER_LEVER_MAY:
				Event_Lever_May(&Trigger);
				break;
			case TRIGGER_SCENE_CHANGE:
				Event_Scene_Change(&Trigger);
				break;
			case TRIGGER_SPACE_WALL_CODY:
				Event_SpaceWall(&Trigger);
				break;
			case TRIGGER_BOUNCE_SWITCH:
				Event_Bounce_Switch(&Trigger);
				break;
			case TRIGGER_HANDLE:
				Event_Handle(&Trigger);
				break;
			case TRIGGER_SPACE_GENERATOR_BATTERY:
				Event_Generator_Battery(&Trigger);
				break;
			case TRIGGER_PLATFORM:
				Event_SpringPlatform(&Trigger);
				break;
			case TRIGGER_SPACE_GENERATOR_LEVER:
				Event_Generator_Lever(&Trigger);
				break;
			case TRIGGER_POSCHANGE:
				Event_Pos_Change(&Trigger);
				break;
			case TRIGGER_DEADSPACE:
				Event_Dead(&Trigger);
				break;
			case TRIGGER_PLATFORMDEAD:
				Event_PlatformDead(&Trigger);
				break;
			case TRIGGER_PLATFORMRESPWAN:
				Event_PlatformRespwan(&Trigger);
				break;
			case TRIGGER_SAVESPACE:
				Event_Save(&Trigger);
				break;
			}
		}
	}
}

void CSpaceField2::onAdvance(const PxRigidBody*const* bodyBuffer, const PxTransform* poseBuffer, const PxU32 count)
{
	OutputDebugString(L"업데이트");
}

void CSpaceField2::Event_Scene_Change(PxTriggerPair* pairs)
{
	ToTrigger(pairs->triggerActor->getName())->Activate((void*)pairs->otherShape->getName());
}

void CSpaceField2::Event_SpaceWall(PxTriggerPair * pairs)
{
	auto* pTrigger = ToTrigger(pairs->triggerShape->getName());
	CSpaceWall* pWall = pTrigger->Is<CSpaceWall>();

	if (pairs->otherShape->getSimulationFilterData().word0 == FilterGroup::eCody)
	{
		CCody* pCody = ToObj<CCody>(pairs->otherShape->getName());
		NULL_CHECK_RETURN(pCody, );

		if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			pCody->Set_Push(true, pWall, pWall->Get_Target1(), CCody::SPACE_WALL, {1.0f, 0.0f, 0.0f});
		}
		else if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_LOST)
		{
			pCody->Set_Push(false);
	
		}
	}
	else if (pairs->otherShape->getSimulationFilterData().word0 == FilterGroup::eMay)
	{
		CMay* pMay = (CMay*)Engine::Get_GameObject(L"GameObject", L"May");
		NULL_CHECK_RETURN(pMay, );

		if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			pMay->Set_Push(true, pWall, pWall->Get_Target2(), CMay::SPACE_WALL);
		}

		else if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_LOST)
		{
			pMay->Set_Push(false);
		}
	}
}

void CSpaceField2::Event_Handle(PxTriggerPair * pairs)
{
	auto* pTrigger = ToTrigger(pairs->triggerShape->getName());
	CSpaceValve* pHandle = ToTrigger(pairs->triggerShape->getActor()->getName())->Is<CSpaceValve>();

	if (pairs->otherShape->getSimulationFilterData().word0 == FilterGroup::eCody)
	{
		if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
			Engine::Get_GameObject(L"GameObject", L"Cody")->Is<CCody>()->Set_Handle(true, pHandle->Get_ObjTag());
		else if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_LOST)
			Engine::Get_GameObject(L"GameObject", L"Cody")->Is<CCody>()->Set_Handle(false);
	}
	else if (pairs->otherShape->getSimulationFilterData().word0 == FilterGroup::eMay)
	{
		if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
			Engine::Get_GameObject(L"GameObject", L"May")->Is<CMay>()->Set_Handle(true, pHandle->Get_ObjTag());
		else if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_LOST)
			Engine::Get_GameObject(L"GameObject", L"May")->Is<CMay>()->Set_Handle(false);
	}
}


void CSpaceField2::Event_Lever_Cody(PxTriggerPair * pairs)
{
	auto* pTrigger = ToTrigger(pairs->triggerShape->getName());
	CSpaceLever* pLever = ToTrigger(pairs->triggerShape->getActor()->getName())->Is<CSpaceLever>();

	if (pairs->otherShape->getSimulationFilterData().word0 == FilterGroup::eCody)
	{
		if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			Engine::Get_GameObject(L"GameObject", L"Cody")->Is<CCody>()->Set_PickUp(true, CCody::LEVER, pTrigger->Is<CLever>());
			ToObj<CCody>(pairs->otherShape->getName())->Set_Slap(pLever->Get_Slap());
		}
		else if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_LOST)
		{
			Engine::Get_GameObject(L"GameObject", L"Cody")->Is<CCody>()->Set_PickUp(false, CCody::LEVER, nullptr);
		}
	}
}

void CSpaceField2::Event_Lever_May(PxTriggerPair * pairs)
{
	auto* pTrigger = ToTrigger(pairs->triggerShape->getName());
	CSpaceLever* pLever = ToTrigger(pairs->triggerShape->getActor()->getName())->Is<CSpaceLever>();

	if (pairs->otherShape->getSimulationFilterData().word0 == FilterGroup::eMay)
	{
		if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			Engine::Get_GameObject(L"GameObject", L"May")->Is<CMay>()->Set_PickUp(true, CMay::LEVER, pTrigger->Is<CLever>());
			ToObj<CMay>(pairs->otherShape->getName())->Set_Slap(pLever->Get_Slap());
		}
		else if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_LOST)
			Engine::Get_GameObject(L"GameObject", L"May")->Is<CMay>()->Set_PickUp(false, CMay::LEVER, nullptr);
	}
}



void CSpaceField2::Event_Generator_Battery(PxTriggerPair * pairs)
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
			pCody->Set_Push(true, pBattery, pBattery->Get_Push_StartPos(), CCody_Space::GENERATOR_BATTERY);
		}
		else if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_LOST)
		{
			pCody->Set_Push(false);
		}
	}
}


void CSpaceField2::Event_SpringPlatform(PxTriggerPair * pairs)
{
	if (pairs->otherShape->getSimulationFilterData().word0 == FilterGroup::eCody)
	{
		if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
			Engine::Get_GameObject(L"GameObject", L"Cody")->Is<CCody>()->Set_OnPlatform(true);
		else if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_LOST)
			Engine::Get_GameObject(L"GameObject", L"Cody")->Is<CCody>()->Set_OnPlatform(false);
	}
	else if (pairs->otherShape->getSimulationFilterData().word0 == FilterGroup::eMay)
	{
		if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
			Engine::Get_GameObject(L"GameObject", L"May")->Is<CMay>()->Set_OnPlatform(true);
		else if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_LOST)
			Engine::Get_GameObject(L"GameObject", L"May")->Is<CMay>()->Set_OnPlatform(false);
	}
}
void CSpaceField2::Event_Generator_Lever(PxTriggerPair * pairs)
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

void CSpaceField2::Event_Pos_Change(PxTriggerPair * pairs)
{
	auto* pTrigger = ToTrigger(pairs->triggerShape->getName());
	CPosChange*	pPosChange = pTrigger->Is<CPosChange>();

	if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
	{
		if (pairs->otherShape->getSimulationFilterData().word0 == FilterGroup::eCody)
		{
			CCody_Space* pCody = ToObj<CCody_Space>(pairs->otherShape->getName());
			pCody->Set_NextZ(pPosChange->Get_Z());
		}
		if (pairs->otherShape->getSimulationFilterData().word0 == FilterGroup::eMay)
		{
			CMay_Space* pMay = ToObj<CMay_Space>(pairs->otherShape->getName());
			pMay->Set_NextZ(pPosChange->Get_Z());
		}
	}
}

void CSpaceField2::Event_PlatformDead(PxTriggerPair * pairs)
{
	auto* pTrigger = ToTrigger(pairs->triggerShape->getName());
	CTriggerView*	pPosChange = pTrigger->Is<CTriggerView>();

	if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
	{
		if (pairs->otherShape->getSimulationFilterData().word0 == FilterGroup::eCody)
		{
			CCody_Space* pCody = ToObj<CCody_Space>(pairs->otherShape->getName());
			pCody->Set_SceneWait(true);

		}
		if (pairs->otherShape->getSimulationFilterData().word0 == FilterGroup::eMay)
		{
			CMay_Space* pMay = ToObj<CMay_Space>(pairs->otherShape->getName());
			pMay->Set_SceneWait(true);
		}
	}
}

void CSpaceField2::Event_PlatformRespwan(PxTriggerPair * pairs)
{
	auto* pTrigger = ToTrigger(pairs->triggerShape->getName());
	CTriggerView*	pTriggerView = pTrigger->Is<CTriggerView>();

	if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_LOST)
	{
		if (pairs->otherShape->getSimulationFilterData().word0 == FilterGroup::eCody)
		{
			CCody_Space* pCody = ToObj<CCody_Space>(pairs->otherShape->getName());
			CMay_Space* pMay = Engine::Get_GameObject(L"GameObject", L"May")->Is<CMay_Space>();

			if (pCody->Get_PlayerDead())
				return;
			pCody->Set_SceneWait(false);
			pCody->Set_SavePoint(pTriggerView->Get_CodyRespwanPoint());
			if(pMay->Get_PlayerDead())
			{
				pMay->Set_SceneWait(false);
				pMay->Set_SavePoint(pTriggerView->Get_MayRespwanPoint());
				pMay->Set_SaveUp(pTriggerView->Get_MayRespwanUp());
			}
		}
		if (pairs->otherShape->getSimulationFilterData().word0 == FilterGroup::eMay)
		{
			CMay_Space* pMay = ToObj<CMay_Space>(pairs->otherShape->getName());
			CCody_Space* pCody = Engine::Get_GameObject(L"GameObject", L"Cody")->Is<CCody_Space>();

			if (pMay->Get_PlayerDead())
				return;
			pMay->Set_SceneWait(false);
			pMay->Set_SavePoint(pTriggerView->Get_MayRespwanPoint());
			pMay->Set_SaveUp(pTriggerView->Get_MayRespwanUp());
			if (pCody->Get_PlayerDead())
			{
				pCody->Set_SceneWait(false);
				pCody->Set_SavePoint(pTriggerView->Get_CodyRespwanPoint());
			}
		}
	}
}

void CSpaceField2::Event_Save(PxTriggerPair * pairs)
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

void CSpaceField2::Event_Bounce_Switch(PxTriggerPair* pairs)
{
	if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
	{
		if (pairs->otherShape->getSimulationFilterData().word0 == FilterGroup::eCody)
		{
			if (ToObj<CCody_Space>(pairs->otherShape->getName())->Is_GroundFound())
			{
				ToTrigger(pairs->triggerShape->getName())->Activate((void*)pairs->otherShape->getName());
			}
		}
		if (pairs->otherShape->getSimulationFilterData().word0 == FilterGroup::eMay)
		{
			if (ToObj<CMay_Space>(pairs->otherShape->getName())->Is_GroundFound())
			{
				ToTrigger(pairs->triggerShape->getName())->Activate((void*)pairs->otherShape->getName());
			}
		}
	}
}



void CSpaceField2::Event_Float(PxTriggerPair* pairs)
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

void CSpaceField2::Event_Dead(PxTriggerPair * pairs)
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

HRESULT CSpaceField2::Ready_Environment_Layer(const _tchar * pLayerTag)
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
//#스테이지	스페이스 필드2
HRESULT CSpaceField2::Ready_GameObject_Layer(const _tchar * pLayerTag)
{
	Engine::CLayer*			pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	OBJINFO tObj;
	ZeroMemory(&tObj, sizeof(OBJINFO));
	tObj.eSceneID = RESOURCE_SF2;
	// 오브젝트 추가
	Engine::CGameObject*		pGameObject = nullptr;
	tObj.vScale = { 0.01f, 0.01f ,0.01f };

	//// CodyCamera
	//pGameObject = CCodyCamera::Create(m_pGraphicDev, &_vec3(0.f, 10.f, -5.f), &_vec3(0.f, 0.f, 1.f), &_vec3(0.f, 1.f, 0.f), D3DXToRadian(60.f), _float(WINCX) / WINCY, 0.1f, 1000.f);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CodyCamera", pGameObject, this), E_FAIL);

	//// MayCamera
	//pGameObject = CMayCamera::Create(m_pGraphicDev, &_vec3(0.f, 10.f, -5.f), &_vec3(0.f, 0.f, 1.f), &_vec3(0.f, 1.f, 0.f), D3DXToRadian(60.f), _float(WINCX) / WINCY, 0.1f, 1000.f);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MayCamera", pGameObject, this), E_FAIL);

	pGameObject = CSceneChangeSpace::Create(m_pGraphicDev, SCENE_SF, { -3.f, 9.f, 0.f }, 2.f, 5.f, 5.f, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SceneChange_SF2_To_SF", pGameObject, this), E_FAIL);
	pGameObject->Is<CSceneChangeSpace>()->Set_Next_Scene_StartPos({ 190.5f, 99.1f, 21.f });
	pGameObject->Is<CSceneChangeSpace>()->Set_InstanceChange();


	pGameObject = CMainCamera::Create(m_pGraphicDev, &_vec3(0.f, 0.f, -5.f), &_vec3(0.f, 0.f, 0.f), &_vec3(0.f, 1.f, 0.f), D3DXToRadian(60.f), _float(WINCX) / WINCY, 0.1f, 1000.f, &_vec3(0.f, 0.f, -5.0f), 20.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MainCamera", pGameObject), E_FAIL);
	pGameObject->Is<CMainCamera>()->Set_Offset({ 0.f,2.f,0.f });

	tObj.vPos = { 222.6f,23.81f,-1.49f };
	tObj.vPos = { 278.6f,15.f,-0.91f };//컵
	//tObj.vPos = { 111.084f, 23.5f, -1.f };// 무브 플렛폼 건녀편
	tObj.vPos = { 340.f, 10.f, 1.5f }; //제네레이터.
	tObj.vPos = { 181.2f,24.05f,-1.62f }; // 내려찍기
	tObj.vPos = { 311.272f, 8.451f, -1.12f };
	tObj.vPos = { 250.5774f, 13.46f,-1.0f };// 핸들
	tObj.vPos = { 69.5f, 23.5f, -1.f };// 무브 플렛폼
	tObj.vPos = { 269.564f,13.451f,-0.85773f };// 벽
	tObj.vPos = { 311.272f, 8.451f, -1.12f };// 마지막
	tObj.vPos = { 0.f,5.f,-1.62f };

	pGameObject = CCody_Space::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pGameObject->Is<CCody_Space>()->Set_CameraMode(CPlayerValue::Camera_2D);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Cody", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eCody, FilterGroup::eGround | FilterGroup::eWall);
	//pGameObject->Is<CCody_Space>()->Set_ChangeAble(false);
	//tObj.vPos = { 222.6f,17.81f,-1.49f }; // 시소
	tObj.vPos = { 111.084f, 21.5f, -1.f };// 무브 플렛폼 건녀편 - 메이
	tObj.vPos = { 340.f, 10.f, -1.f };
	tObj.vPos = { 181.2f,21.29f, -0.3f }; // 내려찍기 - 메이
	tObj.vPos = { 262.33f, 29.5159f, -1.0f };// 핸들 - 메이
	tObj.vPos = { 69.5f, 21.5f, -1.f };// 무브 플렛폼 - 메이
	tObj.vPos = { 269.564f,13.451f,-0.85773f };// 벽
	tObj.vPos = { 320.272f, 8.451f, -1.12f };// 마지막
	tObj.vPos = { 0.f,5.f,0.f };

	pGameObject = CMay_Space::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pGameObject->Is<CMay_Space>()->Set_CameraMode(CPlayerValue::Camera_2D);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"May", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMay, FilterGroup::eGround | FilterGroup::eWall);
	pGameObject->Is<CMay_Space>()->Set_Up({ 0.f,1.f,0.f });
	//pGameObject->Is<CMay_Space>()->Set_Up({ 0.f,1.f,0.f });

#pragma region 맵 트리거 //////////////////////////////////////////////////////////////////////////
	m_pTrigger[TRIGGER_SPACE_MOVEPLATFORM] = CTrigger::CreateBoxTrigger(m_pScene, L"TRIGGER_SPACE_MOVEPLATFORM", _vec3(-8.f, 0.f, -24.f), 2.f, TRIGGER_SPACE_MOVEPLATFORM, FilterGroup::eChessTrigger);
#pragma endregion

	//#설정해야함 헤드위치
	tObj.vPos = { 342.6f, 12.54f, 2.48f };
	tObj.vAngle = { 0.f, D3DXToRadian(180.f),0.f };
	tObj.vScale = { 0.01f, 0.01f,0.01f };
	pGameObject = CRobotHead::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"GeneratorHead", pGameObject, this), E_FAIL);

	lstrcpy(tObj.tagMesh, L"GroundBounce1");
	tObj.vPos = { 0.f,0.f,0.f };
	tObj.vAngle = { 0.f, 0.f,0.f };

	pGameObject = CGroundBounce::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Bounce1", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eCrusher, FilterGroup::eCody | FilterGroup::eMay);
	pGameObject->Is<CGroundBounce>()->Set_Dir({ 0.f,-1.f,0.f });
	pGameObject->Is<CGroundBounce>()->Set_Dist(3.1f);
	pGameObject->Is<CGroundBounce>()->Set_Duration(0.2f);

	//#설정해야함
	lstrcpy(tObj.tagMesh, L"GroundBounce2");
	tObj.vPos = { 0.f,0.1f,0.f };
	pGameObject = CGroundBounce::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Bounce2", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eCrusher, FilterGroup::eCody | FilterGroup::eMay);
	pGameObject->Is<CGroundBounce>()->Set_Dir({ 0.f,1.f,0.f });
	pGameObject->Is<CGroundBounce>()->Set_Dist(6.4f);
	pGameObject->Is<CGroundBounce>()->Set_Duration(0.5f);

	tObj.vPos = { 0.f,0.f,0.f };
	tObj.vAngle = { 0.0, 0.0f, 0.0f };
	lstrcpy(tObj.tagMesh, L"GroundBounce3");
	pGameObject = CGroundBounce::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Bounce3", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eCrusher, FilterGroup::eCody | FilterGroup::eMay);
	pGameObject->Is<CGroundBounce>()->Set_Dir({ 0.f,-1.f,0.f });
	pGameObject->Is<CGroundBounce>()->Set_Dist(8.85f);
	pGameObject->Is<CGroundBounce>()->Set_Duration(1.f);


	tObj.vAngle = { 0.f,-90.f,0.f };
	tObj.vPos = { 222.6f,23.81f,-1.49f };

	pGameObject = CGroundBouncePanel::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"GroundBouncePanel", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eGround | FilterGroup::eWall, FilterGroup::eCody | FilterGroup::eMay);

	// 플랫폼 레이저
	tObj.vPos = { 66.5f,23.6f,-1.f };
	tObj.vScale = _vec3{ 0.5f, 0.005f, 0.005f };
	tObj.vAngle = { 0.0f, 0.0f, 0.0f };
	pGameObject = CSpaceLaser::Create(m_pGraphicDev, &tObj);
	pGameObject->Is<CSpaceLaser>()->Set_2DMap();
	pGameObject->Is<CSpaceLaser>()->Set_Active(true);
	pGameObject->Is<CSpaceLaser>()->Set_Amount(1.0f);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"PlatFormLasser1", pGameObject, this), E_FAIL);

	// 데드
	tObj.vTriggerPos = { 92.756645f,23.6f,-1.f };
	tObj.vTriggerScale = _vec3{ 20.75f ,0.01f, 1.0f };
	pGameObject = CDeadSpace::Create(m_pGraphicDev, L"", true, &tObj);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DeadSpace1", pGameObject, this), E_FAIL);

	tObj.vPos = { 80.0635f,21.01f, -1.f };
	tObj.vScale = _vec3{ 0.05f, 0.005f, 0.005f };
	tObj.vAngle = { 0.0f, 0.0f, 90.0f };
	pGameObject = CSpaceLaser::Create(m_pGraphicDev, &tObj);
	pGameObject->Is<CSpaceLaser>()->Set_2DMap();
	pGameObject->Is<CSpaceLaser>()->Set_Active(true);
	pGameObject->Is<CSpaceLaser>()->Set_Amount(1.0f);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"PlatFormLasser2", pGameObject, this), E_FAIL);

	// 데드
	tObj.vTriggerPos = { 80.051f, 23.4f,-1.f };
	tObj.vTriggerScale = _vec3{ 0.15f ,2.5f, 1.1f };
	pGameObject = CDeadSpace::Create(m_pGraphicDev, L"", true, &tObj);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DeadSpace2", pGameObject, this), E_FAIL);

	tObj.vPos = { 94.6833f, 21.01f, -1.0f };
	tObj.vScale = _vec3{ 0.05f, 0.005f, 0.005f };
	tObj.vAngle = { 0.0f, 0.0f, 90.0f };
	pGameObject = CSpaceLaser::Create(m_pGraphicDev, &tObj);
	pGameObject->Is<CSpaceLaser>()->Set_2DMap();
	pGameObject->Is<CSpaceLaser>()->Set_Active(true);
	pGameObject->Is<CSpaceLaser>()->Set_Amount(1.0f);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"PlatFormLasser3", pGameObject, this), E_FAIL);

	// 데드
	tObj.vTriggerPos = { 94.67f, 23.4f,-1.f };
	tObj.vTriggerScale = _vec3{ 0.15f ,2.5f, 1.1f };
	pGameObject = CDeadSpace::Create(m_pGraphicDev, L"", true, &tObj);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DeadSpace3", pGameObject, this), E_FAIL);

	tObj.vPos = { 108.317f,21.4316f, -1.f };
	tObj.vScale = _vec3{ 0.05f, 0.005f, 0.005f };
	tObj.vAngle = { 0.0f, 0.0f, 180.0f };
	pGameObject = CSpaceLaser::Create(m_pGraphicDev, &tObj);
	pGameObject->Is<CSpaceLaser>()->Set_2DMap();
	pGameObject->Is<CSpaceLaser>()->Set_Active(true);
	pGameObject->Is<CSpaceLaser>()->Set_Amount(1.0f);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"PlatFormLasser4", pGameObject, this), E_FAIL);

	// 데드
	tObj.vTriggerPos = { 108.645f,26.7215f,-1.f };
	tObj.vTriggerScale = _vec3{ 0.15f ,0.75f, 1.1f };
	pGameObject = CDeadSpace::Create(m_pGraphicDev, L"", true, &tObj);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DeadSpace4", pGameObject, this), E_FAIL);

	tObj.vPos = { 108.317f,25.65f, -1.f };
	tObj.vScale = _vec3{ 0.05f, 0.005f, 0.005f };
	tObj.vAngle = { 0.0f, 0.0f, 180.0f };
	pGameObject = CSpaceLaser::Create(m_pGraphicDev, &tObj);
	pGameObject->Is<CSpaceLaser>()->Set_2DMap();
	pGameObject->Is<CSpaceLaser>()->Set_Active(true);
	pGameObject->Is<CSpaceLaser>()->Set_Amount(1.0f);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"PlatFormLasser5", pGameObject, this), E_FAIL);

	// 데드
	tObj.vTriggerPos = { 108.645f,20.39f,-1.f };
	tObj.vTriggerScale = _vec3{ 0.15f ,0.75f, 1.1f };
	pGameObject = CDeadSpace::Create(m_pGraphicDev, L"", true, &tObj);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DeadSpace5", pGameObject, this), E_FAIL);

	tObj.vPos = { 108.68f,19.64f, -1.f };
	tObj.vScale = _vec3{ 0.015f, 0.005f, 0.005f };
	tObj.vAngle = { 0.0f, 0.0f, 90.0f };
	pGameObject = CSpaceLaser::Create(m_pGraphicDev, &tObj);
	pGameObject->Is<CSpaceLaser>()->Set_2DMap();
	pGameObject->Is<CSpaceLaser>()->Set_Active(true);
	pGameObject->Is<CSpaceLaser>()->Set_Amount(1.0f);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"PlatFormLasser6", pGameObject, this), E_FAIL);

	// 데드
	tObj.vTriggerPos = { 105.754f,21.45f,-1.f };
	tObj.vTriggerScale = _vec3{ 2.45f ,0.15f, 1.05f };
	pGameObject = CDeadSpace::Create(m_pGraphicDev, L"", true, &tObj);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DeadSpace6", pGameObject, this), E_FAIL);

	tObj.vPos = { 108.68f,27.49f, -1.f };
	tObj.vScale = _vec3{ 0.015f, 0.005f, 0.005f };
	tObj.vAngle = { 0.0f, 0.0f, -90.0f };
	pGameObject = CSpaceLaser::Create(m_pGraphicDev, &tObj);
	pGameObject->Is<CSpaceLaser>()->Set_2DMap();
	pGameObject->Is<CSpaceLaser>()->Set_Active(true);
	pGameObject->Is<CSpaceLaser>()->Set_Amount(1.0f);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"PlatFormLasser7", pGameObject, this), E_FAIL);

	tObj.vTriggerPos = { 105.754f,25.71f,-1.f };
	tObj.vTriggerScale = _vec3{ 2.45f ,0.15f, 1.05f };
	pGameObject = CDeadSpace::Create(m_pGraphicDev, L"", true, &tObj);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DeadSpace7", pGameObject, this), E_FAIL);


	// 플랫폼 리스폰
	tObj.vTriggerPos = { 112.02f,23.5f,-1.f };
	tObj.vTriggerScale = { 1.3f, 9.8f, 3.15f };

	pGameObject = CTriggerView::Create(m_pGraphicDev, TRIGGER_PLATFORMRESPWAN, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"PlatformRespwan", pGameObject, this), E_FAIL);
	pGameObject->Is<CTriggerView>()->Set_TriggerActive();
	pGameObject->Is<CTriggerView>()->Set_CodyRespwanPoint(_vec3(114.17f, 24.04f, -1.516f));
	pGameObject->Is<CTriggerView>()->Set_MayRespwanPoint(_vec3(114.17f, 22.64f, -0.026f));
	pGameObject->Is<CTriggerView>()->Set_MayRespwanUp(_vec3(0.0f, -1.0f, 0.0f));


	tObj.vScale = { 0.01f, 0.01f, 0.01f };
	tObj.vPos = { 278.7f,18.6f,-1.f };
	tObj.vAngle = { 0.f,90.f,0.f };
	pGameObject = CSpaceCup::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SpaceCup", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eCrusher, FilterGroup::eCody | FilterGroup::eMay);

	// 스프링 레이저
	tObj.vPos = { 261.46f, 14.25f,-1.65f };
	tObj.vScale = { 0.035f, 0.004f, 0.004f };
	tObj.vAngle = { 0.0f, 0.0f, 0.0f };
	pGameObject = CSpaceLaser::Create(m_pGraphicDev, &tObj);
	pGameObject->Is<CSpaceLaser>()->Set_2DMap();
	pGameObject->Is<CSpaceLaser>()->Set_Active(true);
	pGameObject->Is<CSpaceLaser>()->Set_Amount(1.0f);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SpringLaser1", pGameObject, this), E_FAIL);

	tObj.vPos = { 261.46f, 14.25f,-1.15f };
	tObj.vScale = { 0.035f, 0.004f, 0.004f };
	tObj.vAngle = { 0.0f, 0.0f, 0.0f };
	pGameObject = CSpaceLaser::Create(m_pGraphicDev, &tObj);
	pGameObject->Is<CSpaceLaser>()->Set_2DMap();
	pGameObject->Is<CSpaceLaser>()->Set_Active(true);
	pGameObject->Is<CSpaceLaser>()->Set_Amount(1.0f);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SpringLaser2", pGameObject, this), E_FAIL);

	tObj.vPos = { 261.46f, 14.25f,-0.65f };
	tObj.vScale = { 0.035f, 0.004f, 0.004f };
	tObj.vAngle = { 0.0f, 0.0f, 0.0f };
	pGameObject = CSpaceLaser::Create(m_pGraphicDev, &tObj);
	pGameObject->Is<CSpaceLaser>()->Set_2DMap();
	pGameObject->Is<CSpaceLaser>()->Set_Active(true);
	pGameObject->Is<CSpaceLaser>()->Set_Amount(1.0f);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SpringLaser3", pGameObject, this), E_FAIL);

	// 데드
	tObj.vTriggerPos = { 263.278931f, 14.55f,-1.12f };
	tObj.vTriggerScale = _vec3{ 1.2f ,0.01f, 1.75f };
	pGameObject = CDeadSpace::Create(m_pGraphicDev, L"", true, &tObj);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SpringDead", pGameObject, this), E_FAIL);

	// 세이브
	tObj.vPos = { 238.1f, 13.4f, -1.516f };
	tObj.vScale = _vec3{ 1.0f, 20.0f, 10.0f };
	pGameObject = CSaveSpace::Create(m_pGraphicDev, { 263.283f, 13.451f, -1.516f }, L"Yellow_Bg", g_bRenderBox, &tObj);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SpringSavePoint", pGameObject, this), E_FAIL);
	pGameObject->Is<CSaveSpace>()->Set_CodyRespwanPoint({ 250.848f, 13.451f, -1.12f });
	pGameObject->Is<CSaveSpace>()->Set_MayRespwanPoint({ 260.1f, 30.0f, -1.12f });
	pGameObject->Is<CSaveSpace>()->Set_MayRespwanUp({ 0.0f, -1.0f, 0.0f });

	// 벽
	tObj.vPos = { 271.8f,13.4f,-0.3f };
	tObj.vAngle = { 0.f,0.f,0.f };
	tObj.vScale = { 0.01f, 0.01f, 0.01f };

	tObj.vTriggerPos = { 271.8f,16.4f,-0.3f };
	tObj.vTriggerScale = { 3.0f,3.0f,1.0f };
	pGameObject = CSpaceWall::Create(m_pGraphicDev, _vec3(269.814f, 13.451f, -1.316f), _vec3(270.044f, 18.431f, -1.316f), &tObj);

	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SpaceWall", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eRideObj, FilterGroup::eCody | FilterGroup::eMay);

	// 플랫폼 데드
	tObj.vTriggerPos = { 88.23f,23.5f,-1.f };
	tObj.vTriggerScale = { 20.45f, 9.8f, 3.15f };

	CGameObject* pDeadPlatform = nullptr;
	pGameObject = pDeadPlatform = CTriggerView::Create(m_pGraphicDev, TRIGGER_PLATFORMDEAD, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"PlatformDead", pGameObject, this), E_FAIL);
	pGameObject->Is<CTriggerView>()->Set_TriggerActive(false);

	// 스페이스 플랫폼
	tObj.vPos = { 69.5f,23.5f,-1.f };
	pGameObject = CSpacePlatform::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SpacePlatform", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eRideObj, FilterGroup::eCody | FilterGroup::eMay);
	pGameObject->Set_InteractObj(pDeadPlatform);

	// 세이브
	tObj.vPos = { 55.853f, 19.632f, -1.516f };
	tObj.vScale = _vec3{ 1.0f, 20.0f, 10.0f };
	pGameObject = CSaveSpace::Create(m_pGraphicDev, { 263.283f, 13.451f, -1.516f }, L"Yellow_Bg", g_bRenderBox, &tObj);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"PlatformSavePoint", pGameObject, this), E_FAIL);
	pGameObject->Is<CSaveSpace>()->Set_CodyRespwanPoint({ 55.853f, 19.632f, -1.516f });
	pGameObject->Is<CSaveSpace>()->Set_MayRespwanPoint({ 55.853f, 27.0f, -1.12f });
	pGameObject->Is<CSaveSpace>()->Set_MayRespwanUp({ 0.0f, -1.0f, 0.0f });

	tObj.vPos = { 69.5f,24.2f,-1.f };
	tObj.vScale = _vec3{ 0.01f, 0.01f, 0.01f };
	tObj.vTriggerPos = { 69.5f,24.2f,-1.f };
	tObj.vTriggerScale = { 3.0f,3.0f,3.0f };
	tObj.vAngle = _vec3(D3DXToRadian(45.0f), D3DXToRadian(90.0f), 0.0f);
	pGameObject = CSpaceLever::Create(m_pGraphicDev, _vec3(0.0f, 0.0f, 1.0f), _vec3(69.5524f, 23.905f, -2.15f), TRIGGER_LEVER_CODY, &tObj);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SpaceLever", pGameObject, this), E_FAIL);

	tObj.vPos = { 69.5f,22.7f,-1.f };
	tObj.vTriggerPos = { 69.5f,22.7f,-1.f };
	tObj.vTriggerScale = { 3.0f,3.0f,3.0f };
	tObj.vAngle = _vec3(D3DXToRadian(225.0f), D3DXToRadian(90.0f), 0.0f);
	pGameObject = CSpaceLever::Create(m_pGraphicDev, _vec3(0.0f, 0.0f, -1.0f), _vec3(69.5524f, 23.1f, 0.081f), TRIGGER_LEVER_MAY, &tObj, CSpaceLever::MAY);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SpaceLever2", pGameObject, this), E_FAIL);

	tObj.vPos = { 67.5f,30.5f,-1.f };
	tObj.vAngle = { 0.0f, 0.0f, 0.0f };
	pGameObject = CSpacePlatformWall::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoveWall1", pGameObject, this), E_FAIL);

	tObj.vPos = { 67.5f,16.569f,-1.f };
	pGameObject = CSpacePlatformWall::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoveWall2", pGameObject, this), E_FAIL);


	tObj.vPos = { 106.109f,30.453f,-1.f };
	pGameObject = CSpacePlatformWall::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoveWall3", pGameObject, this), E_FAIL);

	tObj.vPos = { 106.109f,16.433f,-1.f };
	pGameObject = CSpacePlatformWall::Create(m_pGraphicDev, &tObj);
	pGameObject->Set_ObjTag(L"SpaceWall");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoveWall4", pGameObject, this), E_FAIL);

	tObj.vPos = { 250.8f,19.4f,1.5f };
	tObj.vAngle = { 0.f,90.f,0.f };
	tObj.vScale = { 0.03f,0.03f,0.03f };
	tObj.vTriggerPos = { 250.8f,19.4f,1.5f };
	tObj.vTriggerScale = { 3.0f, 3.0f, 3.0f };
	pGameObject = CSpaceValve::Create(m_pGraphicDev, _vec3(250.97f, 13.336f, -1.12f), _vec3(0.0f, 0.0f, 1.0f), L"SpaceValve", &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SpaceValve", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eRideObj, FilterGroup::eCody | FilterGroup::eMay);

	tObj.vPos = { 157.836f,25.5f, -1.12f };
	tObj.vScale = { 1.f,7.0f,1.0f };
	pGameObject = CPosChange::Create(m_pGraphicDev, -1.12f, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"PosChange1", pGameObject, this), E_FAIL);

	tObj.vPos = { 311.272f,8.451f, -1.12f };
	tObj.vScale = { 1.f,3.0f,3.0f };
	pGameObject = CPosChange::Create(m_pGraphicDev, 2.48f, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"PosChange2", pGameObject, this), E_FAIL);

	_vec3 TriggerPos;
	_vec3 StartPos;
	CGameObject* pLever = nullptr;

	tObj.vScale = _vec3{ 0.01f,0.01f,0.01f };
	tObj.vAngle = { -90.f, 30.f, 0.f };
	tObj.vTriggerScale = { 1.5f, 1.f, 1.f };
	tObj.vPos = { 342.9f ,9.6f, 0.911f };
	TriggerPos = tObj.vPos + _vec3(-0.2f, -0.f, -1.f);
	StartPos = tObj.vPos + _vec3(-0.2f, -1.15f, -1.f);
	pLever = CGeneratorLever::Create(m_pGraphicDev, &tObj, TriggerPos, StartPos, L"GeneratorBattery");
	NULL_CHECK_RETURN(pLever, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"GeneratorLever", pLever, this), E_FAIL);


	tObj.vScale = _vec3{ 0.01f,0.01f,0.01f };
	tObj.vAngle = { 0.f, -70.f, 0.f };
	tObj.vTriggerScale = { 1.f, 0.5f, 0.5f };
	tObj.vPos = { 342.26f , 10.9f, 3.f };
	//tObj.vPos = { 342.46f , 10.9f, 3.f };
	TriggerPos = tObj.vPos + _vec3(+0.4f, -0.115f, -0.2f);
	StartPos = tObj.vPos + _vec3(+0.756f, -0.14f, -0.45f);
	CGameObject* pBattery = nullptr;

	pBattery = CGeneratorBattery::Create(m_pGraphicDev, &tObj, TriggerPos, StartPos);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"GeneratorBattery", pBattery, this), E_FAIL);
	pLever->Set_InteractObj(pBattery);

	tObj.vPos = { -1.7f,8.f,-6.f };
	tObj.vAngle = { 0.f,0.f,0.f };
	pGameObject = CWarpGateStar::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"WarpGate_Star", pGameObject, this), E_FAIL);
	pLever->Set_InteractObj(pGameObject);


	tObj.vScale = { 0.1f,0.1f,0.1f };

	lstrcpy(tObj.tagMesh, L"SpacePlatform");
	tObj.vScale = _vec3{ 0.01f,0.01f,0.01f };
	tObj.vAngle = { 0, 0.f, 0.f };

	tObj.eMeshType = 1;
	tObj.eRenderType = RENDER_NONALPHA;
	tObj.vPos = { 0.f,0.f,0.f };
	pGameObject = CMap::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Field", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eGround, FilterGroup::eCody | FilterGroup::eMay);

	lstrcpy(tObj.tagMesh, L"Panel");
	pGameObject = CMap::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Field2", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eGround, FilterGroup::eCody | FilterGroup::eMay);

	lstrcpy(tObj.tagMesh, L"Gravity");
	pGameObject = CMap::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Field3", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eGround, FilterGroup::eCody | FilterGroup::eMay);


	lstrcpy(tObj.tagMesh, L"Background");
	pGameObject = CMap::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Field5", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eGround, FilterGroup::eCody | FilterGroup::eMay);

	lstrcpy(tObj.tagMesh, L"Wall");
	pGameObject = CMap::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Field7", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eGround|FilterGroup::eWall, FilterGroup::eCody | FilterGroup::eMay);



	lstrcpy(tObj.tagMesh, L"Alpha");
	tObj.vPos = { 342.608f, 8.46f, 2.57f };
	tObj.vAngle = { 0.f, 90.f, 0.f };
	pGameObject = CSpaceGenerator::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Field6", pGameObject, this), E_FAIL);

	lstrcpy(tObj.tagMesh, L"GravityCorner");
	tObj.vPos = { 0.f,0.f,0.f };
	tObj.vAngle = { 0.f, 0.f, 0.f };
	tObj.vScale = _vec3{ 0.01f, 0.01f, 0.01f };
	//tObj.vAngle = { 0.f,D3DXToRadian(-90.f),0.f };
	pGameObject = CGravityFloor::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Field4", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eGravityFloorIN | FilterGroup::eWall, FilterGroup::eCody | FilterGroup::eMay);

	tObj.vPos = { 342.60f, 10.73f, 3.39f };
	tObj.vAngle = { 0.f, D3DXToRadian(90.f), 0.f };
	pGameObject = CBatteryBox::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"BatteryBox", pGameObject, this), E_FAIL);
	pBattery->Set_InteractObj(pGameObject);
	tObj.vPos = { 0.f,0.f,0.f };
	tObj.vAngle = { 0.f, 0.f, 0.f };
	tObj.vScale = _vec3{ 0.01f, 0.01f, 0.01f };

	// 데드 스페이스
	{
		// 맵전체
		tObj.vTriggerPos = { 180.0f, -20.0f, 0.0f };
		tObj.vTriggerScale = _vec3{ 200.0f, 1.0f, 20.0f };
		pGameObject = CDeadSpace::Create(m_pGraphicDev, L"", true, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DeadSpaceMap1", pGameObject, this), E_FAIL);

		// 맵전체
		tObj.vTriggerPos = { 180.0f, 60.0f, 0.0f };
		tObj.vTriggerScale = _vec3{ 200.0f, 1.0f, 20.0f };
		pGameObject = CDeadSpace::Create(m_pGraphicDev, L"", true, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DeadSpaceMap2", pGameObject, this), E_FAIL);

		// 맵전체
		tObj.vTriggerPos = { 180.0f, 20.0f, 20.0f };
		tObj.vTriggerScale = _vec3{ 200.0f, 40.0f, 1.0f };
		pGameObject = CDeadSpace::Create(m_pGraphicDev, L"", true, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DeadSpaceMap3", pGameObject, this), E_FAIL);

		// 맵전체
		tObj.vTriggerPos = { 180.0f, 20.0f, -20.0f };
		tObj.vTriggerScale = _vec3{ 200.0f, 40.0f, 1.0f };
		pGameObject = CDeadSpace::Create(m_pGraphicDev, L"", true, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DeadSpaceMap4", pGameObject, this), E_FAIL);

		// 맵전체
		tObj.vTriggerPos = { 380.0f, 20.0f, 0.0f };
		tObj.vTriggerScale = _vec3{ 1.0f, 40.0f, 20.0f };
		pGameObject = CDeadSpace::Create(m_pGraphicDev, L"", true, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DeadSpaceMap5", pGameObject, this), E_FAIL);

		// 맵전체
		tObj.vTriggerPos = { -20.0f, 20.0f, 00.0f };
		tObj.vTriggerScale = _vec3{ 1.0f, 40.0f, 20.0f };
		pGameObject = CDeadSpace::Create(m_pGraphicDev, L"", true, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DeadSpaceMap6", pGameObject, this), E_FAIL);
	}

	// 세이브
	{
		tObj.vPos = { 180.0f, 24.055f, -1.12f };
		tObj.vScale = _vec3{ 1.0f, 20.0f, 10.0f };
		pGameObject = CSaveSpace::Create(m_pGraphicDev, { 214.8f, 23.055f, -1.126f }, L"Yellow_Bg", g_bRenderBox, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SavePointGB", pGameObject, this), E_FAIL);
		pGameObject->Is<CSaveSpace>()->Set_CodyRespwanPoint({ 180.0f, 24.055f, -1.12f });
		pGameObject->Is<CSaveSpace>()->Set_MayRespwanPoint({ 180.0f, 23.0f, -1.12f });
		pGameObject->Is<CSaveSpace>()->Set_MayRespwanUp({ 0.0f, -1.0f, 0.0f });

		tObj.vPos = { 214.8f, 23.055f, -1.126f };
		tObj.vScale = _vec3{ 1.0f, 20.0f, 10.0f };
		pGameObject = CSaveSpace::Create(m_pGraphicDev, { 214.8f, 23.055f, -1.126f } , L"Yellow_Bg", g_bRenderBox, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SavePointGBP", pGameObject, this), E_FAIL);
		pGameObject->Is<CSaveSpace>()->Set_CodyRespwanPoint({ 214.8f, 24.055f, -1.126f });
		pGameObject->Is<CSaveSpace>()->Set_MayRespwanPoint({ 214.8f, 23.055f, -1.126f });
		pGameObject->Is<CSaveSpace>()->Set_MayRespwanUp({ 0.0f, -1.0f, 0.0f });

		tObj.vPos = { 266.08f, 13.451f, -1.12f };
		tObj.vScale = _vec3{ 1.0f, 20.0f, 10.0f };
		pGameObject = CSaveSpace::Create(m_pGraphicDev, { 214.8f, 23.055f, -1.126f }, L"Yellow_Bg", g_bRenderBox, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SavePointWall", pGameObject, this), E_FAIL);
		pGameObject->Is<CSaveSpace>()->Set_CodyRespwanPoint({ 266.08f, 13.451f, -1.12f });
		pGameObject->Is<CSaveSpace>()->Set_MayRespwanPoint({ 266.08f, 18.533f, -1.12f });
		pGameObject->Is<CSaveSpace>()->Set_MayRespwanUp({ 0.0f, -1.0f, 0.0f });

		tObj.vPos = { 297.28f, 28.45f, -1.12f };
		tObj.vScale = _vec3{ 1.0f, 20.0f, 10.0f };
		pGameObject = CSaveSpace::Create(m_pGraphicDev, { 214.8f, 23.055f, -1.126f }, L"Yellow_Bg", g_bRenderBox, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SavePoint1", pGameObject, this), E_FAIL);
		pGameObject->Is<CSaveSpace>()->Set_CodyRespwanPoint({ 297.28f, 14.0f, -1.12f });
		pGameObject->Is<CSaveSpace>()->Set_MayRespwanPoint({ 297.28f, 28.45f, -1.12f });
		pGameObject->Is<CSaveSpace>()->Set_MayRespwanUp({ 0.0f, -1.0f, 0.0f });

		tObj.vPos = { 314.35f, 8.45f, 2.48f };
		tObj.vScale = _vec3{ 1.0f, 20.0f, 10.0f };
		pGameObject = CSaveSpace::Create(m_pGraphicDev, { 214.8f, 23.055f, -1.126f }, L"Yellow_Bg", g_bRenderBox, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SavePoint2", pGameObject, this), E_FAIL);
		pGameObject->Is<CSaveSpace>()->Set_CodyRespwanPoint({ 314.35f, 8.45f, 2.48f });
		pGameObject->Is<CSaveSpace>()->Set_MayRespwanPoint({ 314.35f, 18.76f, -1.12f });
		pGameObject->Is<CSaveSpace>()->Set_MayRespwanUp({ 0.0f, -1.0f, 0.0f });
	}

	m_mapLayer.emplace(pLayerTag, pLayer);


	return S_OK;
}

HRESULT CSpaceField2::Ready_LightInfo(void)
{
	D3DLIGHT9				tLightInfo;
	ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));
	tLightInfo.Type = D3DLIGHT_POINT;
	tLightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Ambient = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.f);


	tLightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Position = _vec3(-2.64f, 8.08f, 0.f);
	tLightInfo.Range = 40.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 0);

	tLightInfo.Diffuse = D3DXCOLOR(1.f, 0.8f, 0.6f, 1.f);
	tLightInfo.Position = _vec3(32.64f, 10.92f, -20.f);
	tLightInfo.Range = 35.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 1);

	tLightInfo.Diffuse = D3DXCOLOR(1.f, 0.8f, 0.6f, 1.f);
	tLightInfo.Position = _vec3(26.05f, 37.91f, -20.f);
	tLightInfo.Range = 35.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 2);

	tLightInfo.Diffuse = D3DXCOLOR(1.f, 0.8f, 0.6f, 1.f);
	tLightInfo.Position = _vec3(59.27f, 37.91f, -20.f);
	tLightInfo.Range = 50.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 3);

	tLightInfo.Diffuse = D3DXCOLOR(1.f, 0.8f, 0.6f, 1.f);
	tLightInfo.Position = _vec3(105.81f, 37.91f, -20.f);
	tLightInfo.Range = 50.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 4);

	tLightInfo.Diffuse = D3DXCOLOR(1.f, 0.8f, 0.6f, 1.f);
	tLightInfo.Position = _vec3(151.02f, 37.91f, -20.f);
	tLightInfo.Range = 50.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 5);

	tLightInfo.Diffuse = D3DXCOLOR(1.f, 0.8f, 0.6f, 1.f);
	tLightInfo.Position = _vec3(186.02f, 37.91f, -20.f);
	tLightInfo.Range = 50.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 6);

	tLightInfo.Diffuse = D3DXCOLOR(1.f, 0.8f, 0.6f, 1.f);
	tLightInfo.Position = _vec3(214.07f, 24.20f, -20.f);
	tLightInfo.Range = 50.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 7);

	tLightInfo.Diffuse = D3DXCOLOR(1.f, 0.8f, 0.6f, 1.f);
	tLightInfo.Position = _vec3(236.92f, 24.20f, -20.f);
	tLightInfo.Range = 50.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 8);

	tLightInfo.Diffuse = D3DXCOLOR(1.f, 0.8f, 0.6f, 1.f);
	tLightInfo.Position = _vec3(253.50f, 16.07f, -20.f);
	tLightInfo.Range = 40.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 10);

	tLightInfo.Diffuse = D3DXCOLOR(1.f, 0.8f, 0.6f, 1.f);
	tLightInfo.Position = _vec3(291.50f, 21.24f, -20.f);
	tLightInfo.Range = 50.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 11);

	tLightInfo.Diffuse = D3DXCOLOR(1.f, 0.8f, 0.6f, 1.f);
	tLightInfo.Position = _vec3(308.04f, 16.72f, -20.f);
	tLightInfo.Range = 40.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 12);

	tLightInfo.Diffuse = D3DXCOLOR(1.f, 0.8f, 0.6f, 1.f);
	tLightInfo.Position = _vec3(328.29f, 19.32f, -20.f);
	tLightInfo.Range = 50.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 13);

	tLightInfo.Diffuse = D3DXCOLOR(1.f, 0.8f, 0.6f, 1.f);
	tLightInfo.Position = _vec3(349.89f, 21.24f, -20.f);
	tLightInfo.Range = 50.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 14);

	//////////////////////////////////////////////////////////////////////////

	tLightInfo.Diffuse = D3DXCOLOR(0.4f, 0.7f, 0.9f, 1.f);
	tLightInfo.Position = _vec3(22.44f, 37.91f, 67.54f);
	tLightInfo.Range = 90.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 15);

	tLightInfo.Diffuse = D3DXCOLOR(0.4f, 0.7f, 0.9f, 1.f);
	tLightInfo.Position = _vec3(129.71f, 37.91f, 106.54f);
	tLightInfo.Range = 90.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 16);

	tLightInfo.Diffuse = D3DXCOLOR(0.4f, 0.7f, 0.9f, 1.f);
	tLightInfo.Position = _vec3(231.75f, 37.91f, 106.54f);
	tLightInfo.Range = 90.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 17);

	tLightInfo.Diffuse = D3DXCOLOR(0.4f, 0.7f, 0.9f, 1.f);
	tLightInfo.Position = _vec3(330.71f, 37.91f, 106.54f);
	tLightInfo.Range = 90.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 18);

	return S_OK;
}

void CSpaceField2::SceneChange(const _float & fTimeDelta)
{
	if (!m_bGenerator)
	{

		CGeneratorLever*	pLever = (CGeneratorLever*)Get_GameObject(L"GameObject", L"GeneratorLever");

		_bool bTemp;
		bTemp = pLever->Get_MoveEnd();

		if (bTemp)
		{

			
			
			CMainCamera*		pCamera = (CMainCamera*)Engine::Get_GameObject(L"GameObject", L"MainCamera");
			pCamera->Set_Mode(CAMERA_FIX);
			pCamera->Set_ReachTime(2.f);
			pCamera->Set_Fix(_vec3(170.f, 10.f, 0.f), _vec3(170.f, 10.f, -150.f));
			pCamera->Set_ResetTimer(2.5f, true);
			CMay_Space*		pMay = (CMay_Space*)Get_GameObject(L"GameObject", L"May");
			pMay->SpaceField2_Clear_SetPos(fTimeDelta);

			CCody_Space*	pCody = (CCody_Space*)Get_GameObject(L"GameObject", L"Cody");
			pCody->SpaceField2_Clear_SetPos(fTimeDelta);

			Engine::Get_GameObject(L"GameObject", L"SceneChange_SF2_To_SF")->On_Active(nullptr);

			m_bGenerator = true;
			g_iClearScene++;
		}
	}
}

CSpaceField2 * CSpaceField2::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSpaceField2*		pInstance = new CSpaceField2(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
		Safe_Release(pInstance);

	return pInstance;
}

void CSpaceField2::Effect_Warp()
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
	tParticle.bBill = false;

	tParticle.vRot = { 0.f, D3DXToRadian(90.f), 0.f };
	tParticle.bRotAdd = false;
	tParticle.vPos = _vec3(-2.5f, 8.5f, 0.f);
	Engine::Emit_ImageParticle(tParticle, 45, 33);

}

void CSpaceField2::Free(void)
{
	// 삭제

	for (_uint i = 0; i < m_vecObject.size(); ++i)
		Safe_Delete_Array(m_vecObject[i]);

	m_vecObject.clear();
	m_vecObject.shrink_to_fit();
	Engine::Delete_AllResource(RESOURCE_SF2);
	Engine::CScene::Free();
}


HRESULT CSpaceField2::Ready_Valve_Layer(const _tchar* szLayer)
{
	Engine::CLayer*			pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	OBJINFO tObj;
	ZeroMemory(&tObj, sizeof(OBJINFO));
	tObj.eSceneID = RESOURCE_SF2;
	// 오브젝트 추가
	tObj.vPos = { 0.f,0.f ,0.f };
	tObj.vScale = { 0.01f,0.01f ,0.01f };
	Engine::CGameObject*		pGameObject = nullptr;
	Engine::CGameObject*		pButton = nullptr;

	tObj.vPos = { 263.35f,35.5f,-0.3f };
	tObj.vAngle = { 0.f,90.f,180.f };
	tObj.vScale = { 0.01f, 0.01f ,0.01f };
	tObj.vTriggerPos = { 263.35f,35.5f,-0.3f };
	tObj.vTriggerScale = { 2.0f, 5.0f ,2.0f };
	pGameObject = CSpaceValveSpring::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SpaceSpring", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eRideObj, FilterGroup::eCody | FilterGroup::eMay);

	m_mapLayer.emplace(szLayer, pLayer);

	return S_OK;
}

HRESULT CSpaceField2::Ready_Interact_Layer(const _tchar* szLayer)
{
	Engine::CLayer*			pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	OBJINFO tObj;
	ZeroMemory(&tObj, sizeof(OBJINFO));
	tObj.eSceneID = RESOURCE_SF2;
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
