#include "stdafx.h"
#include "VacuumField3.h"
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

#include "ChessTile.h"
#include "CollisionMgr.h"

#include "Orb.h"
#include "TestWall.h"
#include "MiniVacuum.h"
#include "Map.h"
#include "FuseBox.h"
#include "FuseNPC.h"
#include "PowerSwitch.h"
#include "Fan.h"
#include "FanHolder.h"
#include "Lever.h"
#include "VacuumCrusher.h"
#include "DeadSpace.h"
#include "VacuumBigFan.h"

#include "AirFlowMeter.h"

#include "SceneChangeSpace.h"
#include "Dumbbell.h"
#include "SmallFan.h"
#include "ValveBase.h"
#include "ValveHandle.h"
#include "FerrisWheelBody.h"
#include "FerrisWheelModule.h"
#include "VacuumGenerator.h"
#include "FanButton.h"
#include "CameraSpace.h"
#include "TriggerView.h"
#include "SaveSpace.h"
#include "LineObject.h"

CVacuumField3::CVacuumField3(LPDIRECT3DDEVICE9 pDevice)
	: CScene(pDevice)
{
}

CVacuumField3::~CVacuumField3()
{
}

HRESULT CVacuumField3::Ready_Scene()
{
	Engine::Set_BlendPass(1);
	Engine::Set_SoundScene(RESOURCE_VF);

	Engine::Start_Split();
	g_bMenu = false;
	g_bSlideFinal = false;

	auto* pPhysics = Engine::Get_Physics();
	auto* pDispatcher = Engine::Get_Dispatcher();
	auto* pCudaMgr = Engine::Get_CudaMgr();
	CScene::Ready_PhysX(this, pPhysics, pDispatcher, pCudaMgr);


	if (!m_pScene)
		return E_FAIL;


	//auto* pPlane = PxCreatePlane(*pPhysics, PxPlane(0, 1, 0, 1), *pPhysics->createMaterial(0.5f, 0.5f, 0.f));

	//pPlane->setName((char*)pPlane);
	//setupFiltering(Engine::Get_Allocator(), pPlane, FilterGroup::eGround, FilterGroup::eCody | FilterGroup::eMay);


	//m_pScene->addActor(*pPlane);

	m_pScene->setGravity(PxVec3(0.0f, -20.f, 0.0f));
	FAILED_CHECK_RETURN(Ready_Environment_Layer(L"Environment"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_GameObject_Layer(L"GameObject"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Interact_Layer(L"Interact"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Trigger_Layer(L"Trigger"), E_FAIL);

	//FAILED_CHECK_RETURN(Ready_LoadMap_Layer(L"LoadObject", L"../../Data/ITT_Test/Test.dat"), E_FAIL);

	Ready_LightInfo();

	StopAll();
	PlayBGM(L"VacummField_BGM.wav", 1.f);

	return S_OK;
}

_int CVacuumField3::Update_Scene(const _float & fTimeDelta)
{
	if (m_bStart)
	{
		g_bChinema = true;
		Engine::End_Split();

		m_bStart = false;
	}
	Scene_Start_Event(fTimeDelta);

	m_pScene->simulate(fTimeDelta);

	m_pScene->fetchResults(true);
	_int iExit = Engine::CScene::Update_Scene(fTimeDelta);
	if (Engine::Key_Down(DIK_Y))
	{

		CHANGE_SCENE(m_pGraphicDev, SCENE_VB, OBJ_NOEVENT);

		return 0;
	}
	return iExit;
}

_int CVacuumField3::LateUpdate_Scene(const _float & fTimeDelta)
{
	_int iExit = Engine::CScene::LateUpdate_Scene(fTimeDelta);

	return iExit;
}

void CVacuumField3::Render_Scene()
{
	CCollisionMgr::GetInstance()->Collision_Player_Monster();
	CCollisionMgr::GetInstance()->Clear_List();
}

void CVacuumField3::onConstraintBreak(PxConstraintInfo* constraints, PxU32 count)
{

	return;
}

void CVacuumField3::onWake(PxActor** actors, PxU32 count)
{
	OutputDebugString(L"깼음");
	return;
}

void CVacuumField3::onSleep(PxActor** actors, PxU32 count)
{
	OutputDebugString(L"잠듬");

	return;
}

void CVacuumField3::onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs)
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

void CVacuumField3::onTrigger(PxTriggerPair* pairs, PxU32 count)
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
			case TRIGGER_LEVER:
				Event_Lever(&Trigger);
				break;
			case TRIGGER_FUSE:
				Event_Fuse(&Trigger);
				break;
			case TRIGGER_VACUUM_RIDE:
				Event_Ride(&Trigger);
				break;
			case TRIGGER_FUSE_SLOT:
				Event_Insert_Fuse(&Trigger);
				break;
			case TRIGGER_VACUUM_ENTER:
				Event_Vacuum_Enter(&Trigger);
				break;
			case TRIGGER_DEADSPACE:
				Event_Dead(&Trigger);
				break;
			case TRIGGER_SAVESPACE:
				Event_Save(&Trigger);
				break;
			case TRIGGER_POWERSWITCH:
				Event_PowerSwitch(&Trigger);
				break;
			case TRIGGER_FLOAT:
				Event_Float(&Trigger);
				break;
			case TRIGGER_WING:
				Event_Wing(&Trigger);
				break;
			case TRIGGER_SCENE_CHANGE:
				Event_Scene_Change(&Trigger);
				break;
			case TRIGGER_CAMERA_CHANGE:
				Event_Camera_Change(&Trigger);
				break;
			case TRIGGER_VACUUM_DUMBBELL_CODY:
				Event_Dumbbell_Cody(&Trigger);
				break;
			case TRIGGER_VACUUM_DUMBBELL_MAY:
				Event_Dumbbell_May(&Trigger);
				break;
			case TRIGGER_HANDLE:
				Event_Handle(&Trigger);
				break;
			case TRIGGER_FANBUTTON:
				Event_FanButton(&Trigger);
				break;

			}
		}
	}
}

void CVacuumField3::onAdvance(const PxRigidBody*const* bodyBuffer, const PxTransform* poseBuffer, const PxU32 count)
{
	OutputDebugString(L"업데이트");
}

void CVacuumField3::Event_Scene_Change(PxTriggerPair* pairs)
{
	auto* pPSw = Engine::Get_GameObject(L"GameObject", L"PowerSwitch")->Is<CPowerSwitch>();
	ToTrigger(pairs->triggerActor->getName())->Activate((void*)pairs->otherShape->getName());
	OutputDebugString(L"충돌");
}

void CVacuumField3::Event_Camera_Change(PxTriggerPair * pairs)
{
	auto* pTrigger = ToTrigger(pairs->triggerShape->getName());
	CCameraSpace* pCameraSpace = pTrigger->Is<CCameraSpace>();


	if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
	{
		if (pairs->otherShape->getSimulationFilterData().word0 & FilterGroup::eMay)
		{
			auto* pCamera = Engine::Get_GameObject(L"GameObject", L"MayCamera")->Is<CMayCamera>();
			pCamera->Set_Mode(pCameraSpace->Get_Mode());
			if (CAMERA_FIX == pCameraSpace->Get_Mode())
				pCamera->Set_Fix(pCameraSpace->Get_At(), pCameraSpace->Get_Eye());
			pCamera->Set_ForceRayCheck(false);


		}
		else if (pairs->otherShape->getSimulationFilterData().word0 & FilterGroup::eCody)
		{
			auto* pCamera = Engine::Get_GameObject(L"GameObject", L"CodyCamera")->Is<CCodyCamera>();
			pCamera->Set_Mode(pCameraSpace->Get_Mode());
			if (CAMERA_FIX == pCameraSpace->Get_Mode())
				pCamera->Set_Fix(pCameraSpace->Get_At(), pCameraSpace->Get_Eye());
			pCamera->Set_ForceRayCheck(false);
		}
	}
	else if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_LOST)
	{
		if (pairs->otherShape->getSimulationFilterData().word0 & FilterGroup::eMay)
		{
			auto* pCamera = Engine::Get_GameObject(L"GameObject", L"MayCamera")->Is<CMayCamera>();
			pCamera->Set_Mode(CAMERA_DEFAULT);
			pCamera->Set_ForceRayCheck();


		}
		else if (pairs->otherShape->getSimulationFilterData().word0 & FilterGroup::eCody)
		{
			auto* pCamera = Engine::Get_GameObject(L"GameObject", L"CodyCamera")->Is<CCodyCamera>();
			pCamera->Set_Mode(CAMERA_DEFAULT);
			pCamera->Set_ForceRayCheck();

			if (m_bSound)
			{
				Engine::StopNPlaySound(L"VF3_BigFanStart.ogg", CSoundMgr::CHANNELID::Crusher, 0.3f);
				m_bSound = false;
			}

		}
	}
}

void CVacuumField3::Event_Dumbbell_Cody(PxTriggerPair * pairs)
{
	auto* pTrigger = ToTrigger(pairs->triggerShape->getName());
	CDumbbell* pDumbbell = pTrigger->Is<CDumbbell>();

	if (pairs->otherShape->getSimulationFilterData().word0 == FilterGroup::eCody)
	{
		CCody* pCody = ToObj<CCody>(pairs->otherShape->getName());
		NULL_CHECK_RETURN(pCody, );

		if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			pCody->Set_Push(true, pDumbbell, pDumbbell->Get_Target1(), CCody::DUMBBELL);
		}
		else if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_LOST)
		{
			pCody->Set_Push(false);
		}
	}
}

void CVacuumField3::Event_Dumbbell_May(PxTriggerPair * pairs)
{
	auto* pTrigger = ToTrigger(pairs->triggerShape->getName());
	CDumbbell* pDumbbell = pTrigger->Is<CDumbbell>();

	if (pairs->otherShape->getSimulationFilterData().word0 == FilterGroup::eMay)
	{
		CMay* pMay = (CMay*)Engine::Get_GameObject(L"GameObject", L"May");
		NULL_CHECK_RETURN(pMay, );

		if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			pMay->Set_Push(true, pDumbbell, pDumbbell->Get_Target1(), CMay::DUMBBELL);
		}

		else if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_LOST)
		{
			pMay->Set_Push(false);
		}
	}
}

void CVacuumField3::Event_Fuse(PxTriggerPair* pairs)
{
	CFuseNPC* pFuse = ToTrigger(pairs->triggerShape->getActor()->getName())->Is<CFuseNPC>();

	NULL_CHECK_RETURN(pFuse, );
	CCody* pCody = ToObj<CCody>(pairs->otherShape->getName());
	if (!pCody)
		return;

	if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
		pCody->Set_PickUp(true, CCody::FUSE, pFuse);
	if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_LOST)
		pCody->Set_PickUp(false, CCody::FUSE, nullptr);
}

void CVacuumField3::Event_Lever(PxTriggerPair* pairs)
{
	auto* pTrigger = ToTrigger(pairs->triggerShape->getName());

	if (pairs->otherShape->getSimulationFilterData().word0 == FilterGroup::eCody)
	{
		if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
			Engine::Get_GameObject(L"GameObject", L"Cody")->Is<CCody>()->Set_PickUp(true, CCody::LEVER, pTrigger->Is<CLever>());
		else if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_LOST)
			Engine::Get_GameObject(L"GameObject", L"Cody")->Is<CCody>()->Set_PickUp(false, CCody::LEVER, nullptr);
	}
	else if (pairs->otherShape->getSimulationFilterData().word0 == FilterGroup::eMay)
	{
		if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
			Engine::Get_GameObject(L"GameObject", L"May")->Is<CMay>()->Set_PickUp(true, CMay::LEVER, pTrigger->Is<CLever>());
		else if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_LOST)
			Engine::Get_GameObject(L"GameObject", L"May")->Is<CMay>()->Set_PickUp(false, CMay::LEVER, nullptr);
	}
}

void CVacuumField3::Event_Vacuum_Enter(PxTriggerPair* pairs)
{
	//if (pairs->otherShape->getSimulationFilterData().word0 == FilterGroup::eCody)
	//{
	//	CMiniVacuum* pVaccum = ToTrigger(pairs->triggerShape->getActor()->getName())->Is<CMiniVacuum>();
	//	Engine::Get_GameObject(L"GameObject", L"Cody")->Is<CCody>()->Set_Slide(false, false, pVaccum->Get_IOPath());
	//}
	//else if (pairs->otherShape->getSimulationFilterData().word0 == FilterGroup::eMay)
	//{
	//	CMiniVacuum* pVaccum = ToTrigger(pairs->triggerShape->getActor()->getName())->Is<CMiniVacuum>();
	//	Engine::Get_GameObject(L"GameObject", L"May")->Is<CMay>()->Set_Slide(false, false, pVaccum->Get_IOPath());
	//}
}

void CVacuumField3::Event_Dead(PxTriggerPair * pairs)
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

void CVacuumField3::Event_Float(PxTriggerPair * pairs)
{
	CFan* pFan = ToTrigger(pairs->triggerShape->getActor()->getName())->Is<CFan>();
	_float3 vPos = ToTrigger(pairs->triggerShape->getActor()->getName())->Get_Pos();
	NULL_CHECK_RETURN(pFan, );

	if (pairs->otherShape->getSimulationFilterData().word0 == FilterGroup::eCody)
	{
		CCody* pCody = ToObj<CCody>(pairs->otherShape->getName());
		NULL_CHECK_RETURN(pCody, );

		if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
			pCody->Set_Float(true, pFan->Get_Up(), pFan, vPos);

		else if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_LOST)
			pCody->Set_Float(false, pFan->Get_Up(), nullptr, vPos);
	}
	else if (pairs->otherShape->getSimulationFilterData().word0 == FilterGroup::eMay)
	{
		CMay* pMay = (CMay*)Engine::Get_GameObject(L"GameObject", L"May");
		NULL_CHECK_RETURN(pMay, );

		if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
			pMay->Set_Float(true, pFan->Get_Up(), pFan, vPos);

		else if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_LOST)
			pMay->Set_Float(false, pFan->Get_Up(), nullptr, vPos);
	}
}

void CVacuumField3::Event_Ride(PxTriggerPair * pairs)
{
	CMiniVacuum* pMiniVaccum = ToTrigger(pairs->triggerShape->getActor()->getName())->Is<CMiniVacuum>();
	NULL_CHECK_RETURN(pMiniVaccum, );

	if (pairs->otherShape->getSimulationFilterData().word0 == FilterGroup::eCody)
	{
		CCody* pCody = ToObj<CCody>(pairs->otherShape->getName());

		NULL_CHECK_RETURN(pCody, );
		pMiniVaccum->Get_HoseHead();

		if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			pCody->Set_RideTag("Base", L"Com_MeshHead", L"Com_TransformHead", _vec3(90.0f, 180.0f, 0.0f));
			pCody->Set_PickUp(true, CCody::HOSE, pMiniVaccum);
		}
		else if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_LOST)
			pCody->Set_PickUp(false, CCody::HOSE, nullptr);
	}
	else if (pairs->otherShape->getSimulationFilterData().word0 == FilterGroup::eMay)
	{
		CMay* pMay = (CMay*)Engine::Get_GameObject(L"GameObject", L"May");
		/*CMay* pMay = ToObj<CMay>(pairs->otherShape->getName());*/

		NULL_CHECK_RETURN(pMay, );
		pMiniVaccum->Get_HoseHead();

		if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			pMay->Set_RideTag("Base", L"Com_MeshHead", L"Com_TransformHead", _vec3(90.0f, 180.0f, 0.0f));
			pMay->Set_PickUp(true, CMay::HOSE, pMiniVaccum);
		}

		else if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_LOST)
			pMay->Set_PickUp(false, CMay::HOSE, nullptr);
	}
}

void CVacuumField3::Event_Wing(PxTriggerPair * pairs)
{
	auto* pTrigger = ToTrigger(pairs->triggerShape->getName());

	if (pairs->otherShape->getSimulationFilterData().word0 == FilterGroup::eCody)
	{
		CCody* pCody = ToObj<CCody>(pairs->otherShape->getName());
		NULL_CHECK_RETURN(pCody, );

		if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
			pCody->Set_ColWing(true);

		else if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_LOST)
			pCody->Set_ColWing(false);
	}
	else if (pairs->otherShape->getSimulationFilterData().word0 == FilterGroup::eMay)
	{
		CMay* pMay = (CMay*)Engine::Get_GameObject(L"GameObject", L"May");
		NULL_CHECK_RETURN(pMay, );

		if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
			pMay->Set_ColWing(true);

		else if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_LOST)
			pMay->Set_ColWing(false);
	}
}

void CVacuumField3::Event_Insert_Fuse(PxTriggerPair* pairs)
{

	if (pairs->otherShape->getSimulationFilterData().word0&FilterGroup::eCody)
	{
		CCody* pCody = ToObj<CCody>(pairs->otherShape->getName());
		if (!pCody)
			return;
		if (pCody->Is_Carring() &&
			pCody->Get_CarringType() == CCody::PICKUP_TYPE::FUSE)
		{
			if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
				pCody->Set_Interactable((void*)pairs->triggerActor->getName());
			if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_LOST)
				pCody->Set_Interactable((void*)pairs->triggerActor->getName(), false);
		}
	}
	else if (pairs->otherShape->getSimulationFilterData().word0&FilterGroup::eMay)
	{
		//CMay* pMay = ToObj<CMay>(Trigger.otherShape->getName());
		//if (pMay->Is_Carring() &&
		//	pMay->Get_CarringType() == CCody::PICKUP_TYPE::FUSE)
		//{
		//	if (Trigger.status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
		//		pMay->Set_Interactable();
		//	if (Trigger.status == PxPairFlag::eNOTIFY_TOUCH_LOST)
		//		pMay->Set_Interactable(false);

		//}
	}

}

void CVacuumField3::Event_PowerSwitch(PxTriggerPair* pairs)
{

}

void CVacuumField3::Event_Handle(PxTriggerPair * pairs)
{
	auto* pTrigger = ToTrigger(pairs->triggerShape->getName());
	CValveHandle* pHandle = ToTrigger(pairs->triggerShape->getActor()->getName())->Is<CValveHandle>();

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

void CVacuumField3::Event_FanButton(PxTriggerPair * pairs)
{
	auto* pTrigger = ToTrigger(pairs->triggerShape->getName());
	CFanButton* pButton = ToTrigger(pairs->triggerShape->getActor()->getName())->Is<CFanButton>();

	if (pairs->otherShape->getSimulationFilterData().word0 == FilterGroup::eCody)
	{
		if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
			Engine::Get_GameObject(L"GameObject", L"Cody")->Is<CCody>()->Set_FanButton(true, pButton->Get_ObjTag());
		else if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_LOST)
			Engine::Get_GameObject(L"GameObject", L"Cody")->Is<CCody>()->Set_FanButton(false);
	}
	else if (pairs->otherShape->getSimulationFilterData().word0 == FilterGroup::eMay)
	{
		if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
			Engine::Get_GameObject(L"GameObject", L"May")->Is<CCody>()->Set_FanButton(true, pButton->Get_ObjTag());
		else if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_LOST)
			Engine::Get_GameObject(L"GameObject", L"May")->Is<CMay>()->Set_FanButton(false);
	}
}

void CVacuumField3::Event_Save(PxTriggerPair * pairs)
{
	auto* pTrigger = ToTrigger(pairs->triggerShape->getName());

	CSaveSpace* pSave = pTrigger->Is<CSaveSpace>();

	if (pairs->otherShape->getSimulationFilterData().word0 == FilterGroup::eCody &&
		pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
	{
		CCody* pCody = ToObj<CCody>(pairs->otherShape->getName());
		NULL_CHECK_RETURN(pCody, );

		pCody->Set_SavePoint(pSave->Get_SavePoint());
	}
	else if (pairs->otherShape->getSimulationFilterData().word0 == FilterGroup::eMay&&
		pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
	{
		CMay* pMay = ToObj<CMay>(pairs->otherShape->getName());
		NULL_CHECK_RETURN(pMay, );

		pMay->Set_SavePoint(pSave->Get_SavePoint());
	}
}

HRESULT CVacuumField3::Ready_Environment_Layer(const _tchar * pLayerTag)
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
//#스테이지	청소기 필드3
HRESULT CVacuumField3::Ready_GameObject_Layer(const _tchar * pLayerTag)
{
	Engine::CLayer*			pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	OBJINFO tObj;
	ZeroMemory(&tObj, sizeof(OBJINFO));
	tObj.eSceneID = RESOURCE_VF3;
	// 오브젝트 추가
	Engine::CGameObject*		pGameObject = nullptr;
	tObj.vScale = { 0.01f, 0.01f ,0.01f };

	// CodyCamera
	pGameObject = CCodyCamera::Create(m_pGraphicDev, &_vec3(0.f, 10.f, -5.f), &_vec3(0.f, 0.f, 1.f), &_vec3(0.f, 1.f, 0.f), D3DXToRadian(60.f), _float(WINCX) / WINCY, 0.1f, 1000.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CodyCamera", pGameObject, this), E_FAIL);
	pGameObject->Is<CCamera>()->Set_ForceRayCheck(false);

	// MayCamera
	pGameObject = CMayCamera::Create(m_pGraphicDev, &_vec3(0.f, 10.f, -5.f), &_vec3(0.f, 0.f, 1.f), &_vec3(0.f, 1.f, 0.f), D3DXToRadian(60.f), _float(WINCX) / WINCY, 0.1f, 1000.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MayCamera", pGameObject, this), E_FAIL);
	pGameObject->Is<CMayCamera>()->Set_Fix(_vec3{ 0.f,7.f,0.f }, _vec3{ -5.f,10.f,-10.f });
	pGameObject->Is<CMayCamera>()->Set_Mode(CAMERAMODE::CAMERA_FIX);
	pGameObject->Is<CCamera>()->Set_ForceRayCheck(false);


	{

		tObj.vPos = { 3.5f, 0.0f, -23.8f }; // 풍량계 호스 앞

		tObj.vPos = { 0.9f, -4.3f, -126.0f }; //빅팬 앞
		tObj.vPos = { 104.74f, -4.02f, -115.3f }; // 관람차 건너편
		tObj.vPos = { 91.f,-0.21f,-31.f };	// 팬앞
		tObj.vPos = { 0.f, 0.0f, -0.f }; // 시작
		tObj.vPos = { 100.0f, -4.0f, -175.0f }; // 핸들 앞
		tObj.vPos = { 88.7471f, -3.30f, 12.84f }; // 덤벨 앞
	}
	tObj.vAngle = { 0.f,D3DXToRadian(180.f),0.f };

	pGameObject = CCody::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Cody", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eCody, FilterGroup::eGround | FilterGroup::eWall);

	tObj.vPos.x += 2.0f;
	pGameObject = CMay::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"May", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMay, FilterGroup::eGround | FilterGroup::eWall);

	pGameObject = CSceneChangeSpace::Create(m_pGraphicDev, SCENE_VB, { 92.2285f ,5.1548f,48.5f }, 20.f, 2.0f, 4.f, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SceneChange", pGameObject, this), E_FAIL);


	tObj.vScale = _vec3{ 0.01f, 0.01f, 0.01f };
	tObj.vAngle = { 0.f,0.f,0.f };

	lstrcpy(tObj.tagMesh, L"VacuumField3_Part01");
	//tObj.vScale = _vec3{ 0.007f,0.007f,0.007f };
	tObj.eMeshType = 1;
	tObj.eRenderType = RENDER_NONALPHA;
	tObj.vPos = { 0.f,0.f,0.f };
	pGameObject = CMap::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Field", pGameObject, this), E_FAIL);

	//lstrcpy(tObj.tagMesh, L"VacuumField3_PartWall");
	////tObj.vScale = _vec3{ 0.007f,0.007f,0.007f };
	//tObj.eMeshType = 1;
	//tObj.eRenderType = RENDER_NONALPHA;
	//tObj.vPos = { 0.f,0.f,0.f };
	//pGameObject = CMap::Create(m_pGraphicDev, &tObj);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Fieldtest", pGameObject, this), E_FAIL);

	lstrcpy(tObj.tagMesh, L"VacuumField3_Part02");
	//tObj.vScale = _vec3{ 0.007f,0.007f,0.007f };
	tObj.eMeshType = 1;
	tObj.eRenderType = RENDER_NONALPHA;
	tObj.vPos = { 0.f,0.f,0.f };
	pGameObject = CMap::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Field2", pGameObject, this), E_FAIL);
	pGameObject->Set_QueryFilter(PxFilterData(FilterGroup::eInteractItem, 0, 0, 0));

	lstrcpy(tObj.tagMesh, L"VacuumField3_Part03");
	//tObj.vScale = _vec3{ 0.007f,0.007f,0.007f };
	tObj.eMeshType = 1;
	tObj.eRenderType = RENDER_NONALPHA;
	tObj.vPos = { 0.f,0.f,0.f };
	pGameObject = CMap::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Field3", pGameObject, this), E_FAIL);
	pGameObject->Set_QueryFilter(PxFilterData(FilterGroup::eInteractItem, 0, 0, 0));


	lstrcpy(tObj.tagMesh, L"VacuumField3_Metal");
	//tObj.vScale = _vec3{ 0.007f,0.007f,0.007f };
	tObj.eMeshType = 1;
	tObj.eRenderType = RENDER_NONALPHA;
	tObj.vPos = { 0.f,0.f,0.f };
	pGameObject = CMap::Create(m_pGraphicDev, &tObj);
	static_cast<CMap*>(pGameObject)->Set_MaterialIndex(1);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Field4", pGameObject, this), E_FAIL);


	tObj.vPos = { 4.3f,10.4f,15.f };
	tObj.vScale = { 0.015f,0.015f,0.015f };
	tObj.vAngle = { 0.f,-D3DXToRadian(180.f),0.f };
	pGameObject = CMiniVacuumHead::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"StartHead", pGameObject, this), E_FAIL);

	// 레버 종합세트1
	{
		tObj.vPos = { 21.7f,-1.3f, -0.0f };
		tObj.vScale = _vec3{ 2.f, 10.0f, 2.0f };
		tObj.eMeshType = 1;
		pGameObject = CFan::Create(m_pGraphicDev, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Float4", pGameObject, this), E_FAIL);

		tObj.vPos = { 21.8f,-1.3f, -12.0f };
		tObj.vScale = _vec3{ 2.f, 10.0f, 2.0f };
		tObj.eMeshType = 0;
		pGameObject = CFan::Create(m_pGraphicDev, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Float5", pGameObject, this), E_FAIL);

		tObj.vPos = { 12.25f, -0.03f, -2.0f };
		tObj.vScale = _vec3{ 1.f, 1.0f, 1.0f };
		// Fan의 갯수
		tObj.eRenderType = 2;
		tObj.vAngle = _vec3(0.0f, 0.0f, 0.0f);
		pGameObject = CLever::Create(m_pGraphicDev, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Lever3", pGameObject, this), E_FAIL);
		pGameObject->Is<CLever>()->Add_FanList(L"Float4");
		pGameObject->Is<CLever>()->Add_FanList(L"Float5");
	}

	// 레버 종합세트2
	{
		// 이거 앞에뒤면 맵스케일이 바뀜
		// 팬바람의 방향
		// 0이면 아래로 1이면 위로
		tObj.vPos = { 84.4267f, -6.9685f, -10.8f };
		tObj.vScale = _vec3{ 2.f, 8.f, 2.0f };
		//tObj.vTriggerPos = {}
		tObj.eMeshType = 1;
		pGameObject = CFan::Create(m_pGraphicDev, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Float1", pGameObject, this), E_FAIL);

		tObj.vPos = { 93.4267f, -6.9685f, -10.8f };
		tObj.vScale = _vec3{ 2.f, 8.f, 2.0f };
		// 팬바람의 방향
		// 0이면 아래로 1이면 위로
		tObj.eMeshType = 0;
		pGameObject = CFan::Create(m_pGraphicDev, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Float2", pGameObject, this), E_FAIL);

		tObj.vPos = { 102.4267f, -6.9685f, -10.8f };
		tObj.vScale = _vec3{ 2.f, 8.f, 2.0f };
		// 팬바람의 방향
		// 0이면 아래로 1이면 위로
		tObj.eMeshType = 1;
		pGameObject = CFan::Create(m_pGraphicDev, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Float3", pGameObject, this), E_FAIL);

		tObj.vPos = { 102.4267f, -5.9685f, -10.8f };
		// 팬바람의 방향
		// 0이면 아래로 1이면 위로
		pGameObject = CFanHolder::Create(m_pGraphicDev, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"FanHolder1", pGameObject, this), E_FAIL);

		tObj.vPos = { 93.4267f, -5.9685f, -10.8f };
		// 팬바람의 방향
		// 0이면 아래로 1이면 위로
		pGameObject = CFanHolder::Create(m_pGraphicDev, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"FanHolder2", pGameObject, this), E_FAIL);

		tObj.vPos = { 92.45f, -4.0f, -1.0f };
		tObj.vScale = _vec3{ 1.f, 1.0f, 1.0f };
		// Fan의 갯수
		tObj.eRenderType = 3;
		tObj.vAngle = _vec3(0.0f, 0.0f, 0.0f);
		pGameObject = CLever::Create(m_pGraphicDev, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Lever1", pGameObject, this), E_FAIL);
		pGameObject->Is<CLever>()->Add_FanList(L"Float1");
		pGameObject->Is<CLever>()->Add_FanList(L"Float2");
		pGameObject->Is<CLever>()->Add_FanList(L"Float3");

		tObj.vPos = { 92.45f, -4.0f, -20.5f };
		tObj.vScale = _vec3{ 1.f, 1.0f, 1.0f };
		// Fan의 갯수
		tObj.eRenderType = 3;
		tObj.vAngle = _vec3(0.0f, D3DXToRadian(180.0f), 0.0f);
		pGameObject = CLever::Create(m_pGraphicDev, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Lever2", pGameObject, this), E_FAIL);
		pGameObject->Is<CLever>()->Add_FanList(L"Float1");
		pGameObject->Is<CLever>()->Add_FanList(L"Float2");
		pGameObject->Is<CLever>()->Add_FanList(L"Float3");

		tObj.vPos = { 93.4267f, -9.9685f, -9.1f };
		tObj.eMeshType = 1;
		// 팬바람의 방향
		// 0이면 아래로 1이면 위로
		pGameObject = CVacuumCrusher::Create(m_pGraphicDev, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"VacuumCrusher1", pGameObject, this), E_FAIL);

		tObj.vPos = { 93.4267f, -9.9685f, -12.9f };
		tObj.eMeshType = 0;
		// 팬바람의 방향
		// 0이면 아래로 1이면 위로
		pGameObject = CVacuumCrusher::Create(m_pGraphicDev, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"VacuumCrusher2", pGameObject, this), E_FAIL);
	}

	// 빅팬 종합세트0.

	{
		tObj.vPos = { -12.0f, -4.3f, -160.0f };
		// 팬바람의 방향
		// 0이면 아래로 1이면 위로
		pGameObject = CVacuumBigFan::Create(m_pGraphicDev, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"BigFan1", pGameObject, this), E_FAIL);
		setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eBigFan, FilterGroup::eCody | FilterGroup::eMay);
		pGameObject->Is<CVacuumBigFan>()->Set_Sound();

		tObj.vPos = { 5.0f, -4.3f, -200.0f };
		// 팬바람의 방향
		// 0이면 아래로 1이면 위로
		pGameObject = CVacuumBigFan::Create(m_pGraphicDev, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"BigFan2", pGameObject, this), E_FAIL);
		setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eBigFan, FilterGroup::eCody | FilterGroup::eMay);
	}

	// 팬바람의 방향
	// 0이면 아래로 1이면 위로
	tObj.vPos = { 6.5f, 0.0f, -33.0f };
	pGameObject = CMiniVacuum::Create(m_pGraphicDev, DIRUP, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Minivacuum", pGameObject, this), E_FAIL);
	auto vecBody = pGameObject->Is<CMiniVacuum>()->Get_HozeBody();

	m_pScene->addAggregate(*pGameObject->Is<CMiniVacuum>()->Get_Hoze());
	for (auto pBody : vecBody)
		setupFiltering(Engine::Get_Allocator(), pBody, FilterGroup::eHoze, FilterGroup::eCody | FilterGroup::eMay);

	// 풍량계
	tObj.vPos = { 5.857f, -2.0f, -12.2f };
	lstrcpy(tObj.tagMesh, L"Minivacuum");
	pGameObject = CAirFlowMeter::Create(m_pGraphicDev, &tObj);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"AirFlowMeter", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eRideObj, FilterGroup::eCody | FilterGroup::eMay);

	// 덤벨
	{
		//tObj.vPos = { 106.4267f, -3.22f, 18.75f };
		tObj.vPos = { 92.5303f, -3.22f, 18.75f };
		tObj.vTriggerScale = { 3.0f, 3.0f, 3.0f };
		pGameObject = CDumbbell::Create(m_pGraphicDev, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Dumbbell", pGameObject, this), E_FAIL);
		setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eRideObj, FilterGroup::eCody | FilterGroup::eMay);
	}

	tObj.vPos = { 93.4767f, -40.5015f, -10.47f };
	pGameObject = CVacuumGenerator::Create(m_pGraphicDev, &tObj);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"VacuumGenerator", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eGround, FilterGroup::eCody | FilterGroup::eMay);
	pGameObject->Set_QueryFilter(PxFilterData(FilterGroup::eGround, FilterGroup::eRay, 0, 0));
	//pGameObject->Set_QueryFilter(PxFilterData(FilterGroup::eInteractItem, 0, 0, 0));

	// 관람차
	{
		tObj.vPos = { 109.5f, -1.5f, -144.0f };
		// 팬바람의 방향
		// 0이면 아래로 1이면 위로
		pGameObject = CSmallFan::Create(m_pGraphicDev, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SmallFan1", pGameObject, this), E_FAIL);
		pGameObject->Is<CSmallFan>()->Add_BodyList(L"FerrisWheelBody1");
		pGameObject->Is<CSmallFan>()->Add_BodyList(L"FerrisWheelBody2");
		pGameObject->Is<CSmallFan>()->Add_BodyList(L"FerrisWheelBody3");

		tObj.vPos = { -86.70f, -5.6F, -31.78f };
		tObj.vScale = _vec3{ 0.015f, 0.015f, 0.015f };
		CGameObject* pLine = nullptr;
		pLine = CLineObject::Create(m_pGraphicDev, &tObj, L"../../Data/VF3Line.dat", true);
		NULL_CHECK_RETURN(pLine, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CordLine", pLine, this), E_FAIL);
		pLine->Set_InteractObj(pGameObject);

		// Body
		tObj.vPos = _vec3(109.5f, -1.5f, -148.5f) + _vec3(-4.07f, -0.45f, -0.06f);
		lstrcpy(tObj.tagMesh, L"SmallFan1");
		pGameObject = CFerriswheelBody::Create(m_pGraphicDev, _vec3(0.0f, -1.0f, 0.0f) * 18.0f, _vec3(0.0f, 0.0f, 0.0f), &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"FerrisWheelBody1", pGameObject, this), E_FAIL);
		setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eStone, FilterGroup::eCody | FilterGroup::eMay);

		tObj.vPos = _vec3(109.5f, -1.5f, -148.5f) + _vec3(-4.07f, -0.45f, -0.06f);
		lstrcpy(tObj.tagMesh, L"SmallFan1");
		pGameObject = CFerriswheelBody::Create(m_pGraphicDev, _vec3(0.0f, -1.0f, 0.0f) * 18.0f, _vec3(120.0f, 0.0f, 0.0f), &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"FerrisWheelBody2", pGameObject, this), E_FAIL);
		setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eStone, FilterGroup::eCody | FilterGroup::eMay);

		tObj.vPos = _vec3(109.5f, -1.5f, -148.5f) + _vec3(-4.07f, -0.45f, -0.06f);
		lstrcpy(tObj.tagMesh, L"SmallFan1");
		pGameObject = CFerriswheelBody::Create(m_pGraphicDev, _vec3(0.0f, -1.0f, 0.0f) * 18.0f, _vec3(240.0f, 0.0f, 0.0f), &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"FerrisWheelBody3", pGameObject, this), E_FAIL);
		setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eStone, FilterGroup::eCody | FilterGroup::eMay);

		tObj.vPos = { 94.77f, -4.18f, -171.34f };
		// 팬바람의 방향
		// 0이면 아래로 1이면 위로
		pGameObject = CValveBase::Create(m_pGraphicDev, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ValveBase1", pGameObject, this), E_FAIL);

		tObj.vPos = { 94.77f, -2.33f, -171.8f };
		tObj.vTriggerPos = { 94.8f, -2.3f, -174.0f };
		tObj.vTriggerScale = { 2.05f, 1.45f, 2.2f };
		lstrcpy(tObj.tagMesh, L"SmallFan1");
		// 팬바람의 방향
		// 0이면 아래로 1이면 위로
		pGameObject = CValveHandle::Create(m_pGraphicDev, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ValveHandle1", pGameObject, this), E_FAIL);
		pGameObject->Is<CValveHandle>()->Set_DefaultSetting(L"ValveHandle1", _vec3(94.77f, -4.0f, -172.8f), _vec3(0.0f, 0.0f, 1.0f));

		tObj.vPos = { 61.96f, 2.47f, -127.88f };
		tObj.vTriggerPos = { 61.95f, 4.77f, -126.46f };
		tObj.vTriggerScale = { 3.0f, 1.87f, 1.1f };
		lstrcpy(tObj.tagMesh, L"CordLine");
		// 팬바람의 방향
		// 0이면 아래로 1이면 위로
		pGameObject = CFanButton::Create(m_pGraphicDev, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"FanButton1", pGameObject, this), E_FAIL);
		pGameObject->Set_ObjTag(L"FanButton1");


	}

	// 데드 존
	{
		tObj.vTriggerPos = { 19.546f, -7.76f, -27.17f };
		tObj.vTriggerScale = _vec3{ 51.3f ,1.0f, 49.64f };
		pGameObject = CDeadSpace::Create(m_pGraphicDev, L"", false, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DeadSpace1", pGameObject, this), E_FAIL);

		tObj.vTriggerPos = { 0.9f, -35.3f, -126.0f };
		tObj.vTriggerScale = _vec3{ 300.0f,2.0f,300.0f };
		pGameObject = CDeadSpace::Create(m_pGraphicDev, L"", false, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DeadSpace2", pGameObject, this), E_FAIL);

		tObj.vTriggerPos = { 93.0067f,-8.7685f,-13.63f };
		tObj.vTriggerScale = _vec3{ 14.2f,1.05f,12.1f };
		pGameObject = CDeadSpace::Create(m_pGraphicDev, L"", false, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DeadSpace3", pGameObject, this), E_FAIL);

		tObj.vTriggerPos = { 0.f,-36.f,0.f };
		tObj.vTriggerScale = _vec3{ 300.f,0.1f,300.f };
		tObj.vPos = { 150.f,-36.f,-100.f };
		tObj.vScale = { 0.3f,0.01f,0.3f };
		pGameObject = CDeadSpace::Create(m_pGraphicDev, L"Water", true, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DeadSpace4", pGameObject, this), E_FAIL);

	}

	// 세이브 존
	{
		tObj.vPos = { 13.245f, -0.2f + 10.0f, 0.0f };
		tObj.vScale = _vec3{ 1.0f, 5.0f, 5.0f };
		pGameObject = CSaveSpace::Create(m_pGraphicDev, _vec3(13.245f, -0.2f, 0.0f), L"Yellow_Bg", g_bRenderBox, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SaveSpace1", pGameObject, this), E_FAIL);

		tObj.vPos = { 17.4f, -0.2f + 10.0f, -19.67f };
		tObj.vScale = _vec3{ 10.0f, 10.0f, 1.0f };
		pGameObject = CSaveSpace::Create(m_pGraphicDev, _vec3(10.05f, -0.2f, -26.65f), L"Yellow_Bg", g_bRenderBox, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SaveSpace2", pGameObject, this), E_FAIL);

		tObj.vPos = { 0.9f, -4.3f + 10.0f, -126.0f };
		tObj.vScale = _vec3{ 10.0f, 10.0f, 1.0f };
		pGameObject = CSaveSpace::Create(m_pGraphicDev, _vec3(0.9f, -4.3f, -126.0f), L"Yellow_Bg", g_bRenderBox, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SaveSpace3", pGameObject, this), E_FAIL);

		tObj.vPos = { 34.52f, -4.1f + 10.0f, -190.37f };
		tObj.vScale = _vec3{ 2.0f, 10.0f, 10.0f };
		pGameObject = CSaveSpace::Create(m_pGraphicDev, _vec3(34.52f, -4.1f, -190.37f), L"Yellow_Bg", g_bRenderBox, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SaveSpace4", pGameObject, this), E_FAIL);

		tObj.vPos = { 82.28f, -4.2f + 10.0f, -176.02f };
		tObj.vScale = _vec3{ 2.0f, 10.0f, 10.0f };
		pGameObject = CSaveSpace::Create(m_pGraphicDev, _vec3(82.28f, -4.2f, -176.02f), L"Yellow_Bg", g_bRenderBox, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SaveSpace5", pGameObject, this), E_FAIL);

		tObj.vPos = { 104.74f, -4.02f, -115.3f };
		tObj.vScale = _vec3{ 20.0f, 10.0f, 1.0f };
		pGameObject = CSaveSpace::Create(m_pGraphicDev, _vec3(104.74f, -4.02f, -115.3f), L"Yellow_Bg", g_bRenderBox, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SaveSpace6", pGameObject, this), E_FAIL);

		tObj.vPos = { 92.58f, -4.2f + 10.0f, -30.68f };
		tObj.vScale = _vec3{ 20.0f, 10.0f, 1.0f };
		pGameObject = CSaveSpace::Create(m_pGraphicDev, _vec3(92.58f, -4.2f, -30.68f), L"Yellow_Bg", g_bRenderBox, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SaveSpace7", pGameObject, this), E_FAIL);

		// 3팬 건너편
		tObj.vPos = { 111.06f, -3.3f + 5.0f, -10.4f };
		tObj.vScale = _vec3{ 1.0f, 5.0f, 5.0f };
		pGameObject = CSaveSpace::Create(m_pGraphicDev, _vec3(113.7f, -3.31f, -2.43f), L"Yellow_Bg", g_bRenderBox, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SaveSpace8", pGameObject, this), E_FAIL);

		//// 시작
		tObj.vPos = { 3.98119140f,2.936103415f, 4.69469261f };
		tObj.vScale = _vec3{ 5.0f, 5.0f, 5.0f };
		pGameObject = CSaveSpace::Create(m_pGraphicDev, _vec3(3.98119140f, -0.236103415f, 4.69469261f), L"Yellow_Bg", g_bRenderBox, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SaveSpace9", pGameObject, this), E_FAIL);
	}

	m_mapLayer.emplace(pLayerTag, pLayer);

	return S_OK;
}

HRESULT CVacuumField3::Ready_LoadMap_Layer(const _tchar * pLayerTag, const _tchar * pFilePath)
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

HRESULT CVacuumField3::Ready_LightInfo(void)
{
	D3DLIGHT9				tLightInfo;
	ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));

	tLightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Ambient = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.f);
	tLightInfo.Type = D3DLIGHT_POINT;


	tLightInfo.Diffuse = D3DXCOLOR(0.9f, 0.7f, 0.5f, 1.f);
	tLightInfo.Position = _vec3(4.29f, 11.17f, -1.49f);
	tLightInfo.Range = 30.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 0);

	tLightInfo.Diffuse = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.f);
	tLightInfo.Position = _vec3(24.13f, 10.17f, -11.63f);
	tLightInfo.Range = 40.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 1);

	tLightInfo.Diffuse = D3DXCOLOR(1.1f, 1.2f, 1.5f, 1.f);
	tLightInfo.Position = _vec3(5.67f, 13.53f, -36.17f);
	tLightInfo.Range = 30.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 2);

	tLightInfo.Diffuse = D3DXCOLOR(1.1f, 1.2f, 1.5f, 1.f);
	tLightInfo.Position = _vec3(21.12f, 11.87f, -58.32f);
	tLightInfo.Range = 20.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 3);

	tLightInfo.Diffuse = D3DXCOLOR(1.5f, 0.2f, 0.3f, 1.f);
	tLightInfo.Position = _vec3(14.95f, 1.42f, -96.19f);
	tLightInfo.Range = 20.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 4);

	tLightInfo.Diffuse = D3DXCOLOR(1.4f, 1.3f, 1.1f, 1.f);
	tLightInfo.Position = _vec3(2.22f, 4.10f, -136.75f);
	tLightInfo.Range = 17.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 5);

	tLightInfo.Diffuse = D3DXCOLOR(1.4f, 1.3f, 1.1f, 1.f);
	tLightInfo.Position = _vec3(-12.0f, 3.43f, -160.0f);
	tLightInfo.Range = 30.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 6);

	tLightInfo.Diffuse = D3DXCOLOR(1.4f, 1.3f, 1.1f, 1.f);
	tLightInfo.Position = _vec3(5.0f, 3.43f, -200.0f);
	tLightInfo.Range = 30.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 7);

	tLightInfo.Diffuse = D3DXCOLOR(1.4f, 1.3f, 1.1f, 1.f);
	tLightInfo.Position = _vec3(26.69f, 2.06f, -193.81f);
	tLightInfo.Range = 25.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 8);

	tLightInfo.Diffuse = D3DXCOLOR(1.6f, 1.5f, 1.3f, 1.f);
	tLightInfo.Position = _vec3(75.36f, 5.77f, -190.69f);
	tLightInfo.Range = 40.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 9);

	tLightInfo.Diffuse = D3DXCOLOR(1.f, 1.1f, 1.4f, 1.f);
	tLightInfo.Position = _vec3(101.97f, -1.31f, -148.45f);
	tLightInfo.Range = 45.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 10);

	tLightInfo.Diffuse = D3DXCOLOR(1.f, 0.8f, 0.6f, 1.f);
	tLightInfo.Position = _vec3(93.46f, 3.19f, -120.86f);
	tLightInfo.Range = 20.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 11);

	tLightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Position = _vec3(64.15f, 6.09f, -123.46f);
	tLightInfo.Range = 12.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 12);

	tLightInfo.Diffuse = D3DXCOLOR(1.f * 2.5f, 0.8f * 2.5f, 0.5f * 2.5f, 1.f);
	tLightInfo.Position = _vec3(169.11f, -1.31f, -148.45f);
	tLightInfo.Range = 50.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 13);

	tLightInfo.Diffuse = D3DXCOLOR(0.9f, 0.7f, 0.5f, 1.f);
	tLightInfo.Position = _vec3(92.76f, 25.00f, -64.76f);
	tLightInfo.Range = 30.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 14);

	tLightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Position = _vec3(92.76f, 4.68f, -10.97f);
	tLightInfo.Range = 20.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 15);

	tLightInfo.Diffuse = D3DXCOLOR(1.f, 0.8f, 0.6f, 1.f);
	tLightInfo.Position = _vec3(73.91f, 10.42f, -3.25f);
	tLightInfo.Range = 40.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 16);

	tLightInfo.Diffuse = D3DXCOLOR(1.f* 2.5f, 0.8f* 2.5f, 0.7f* 2.5f, 1.f);
	tLightInfo.Position = _vec3(92.76f, 17.50f, 32.69f);
	tLightInfo.Range = 50.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 17);


	return S_OK;
}

CVacuumField3 * CVacuumField3::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CVacuumField3*		pInstance = new CVacuumField3(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
		Safe_Release(pInstance);

	return pInstance;
}

void CVacuumField3::Free(void)
{
	// 삭제
	CCollisionMgr::DestroyInstance();

	for (_uint i = 0; i < m_vecObject.size(); ++i)
		Safe_Delete_Array(m_vecObject[i]);

	m_vecObject.clear();
	m_vecObject.shrink_to_fit();
	Engine::Delete_AllResource(RESOURCE_VF3);
	Engine::CScene::Free();
}


void CVacuumField3::Scene_Start_Event(const _float& fTimeDelta)
{
	//return;
	if (!g_bChinema)
		return;
	m_fSceneTime += fTimeDelta;

	OBJINFO tObj;
	ZeroMemory(&tObj, sizeof(OBJINFO));
	tObj.eSceneID = RESOURCE_VF3;
	tObj.vAngle = { 0.f,D3DXToRadian(180.f),0.f };


	CLayer* pLayer = Engine::Get_Layer(L"GameObject");
	// 오브젝트 추가
	Engine::CGameObject*		pGameObject = nullptr;
	tObj.vScale = { 0.01f, 0.01f ,0.01f };
	tObj.vPos = { 3.5f,10.f,11.f };
	auto* pMayCamera = Engine::Get_GameObject(L"GameObject", L"MayCamera");


	switch (m_iSequence)
	{
	case 0:
		if (m_fSceneTime > 2.f)
			++m_iSequence;
		break;
	case 1:
		++m_iSequence;
		pGameObject = Engine::Get_GameObject(L"GameObject", L"Cody");
		pGameObject->Set_Pos(tObj.vPos);
		pGameObject->Is<CCody>()->Set_Shoot({ 0.f,0.f,-1.f }, 5, 1);
		break;
	case 2:
		if (m_fSceneTime > 3.f)
			++m_iSequence;
		break;
	case 3:
		++m_iSequence;
		tObj.vPos = { 4.f,10.f,11.f };

		pGameObject = Engine::Get_GameObject(L"GameObject", L"May");
		pGameObject->Set_Pos(tObj.vPos);
		pGameObject->Is<CMay>()->Set_Shoot({ 0.f,0.f,-1.f }, 8, 1);
		break;
	case 4:
		if (m_fSceneTime > 4.f)
			++m_iSequence;
		break;
	case 5:
		Engine::Start_Split();
		g_bChinema = false;
		pMayCamera->Is<CMayCamera>()->Set_Mode(CAMERAMODE::CAMERA_DEFAULT);
		pMayCamera->Is<CMayCamera>()->Set_RayCheck(false);
		++m_iSequence;

		break;
	default:
		break;
	}

}

HRESULT CVacuumField3::Ready_Interact_Layer(const _tchar* szLayer)
{
	//Engine::CLayer*			pLayer = Engine::CLayer::Create();
	//NULL_CHECK_RETURN(pLayer, E_FAIL);

	//OBJINFO tObj;
	//ZeroMemory(&tObj, sizeof(OBJINFO));
	//tObj.eSceneID = RESOURCE_VF;
	//// 오브젝트 추가

	//Engine::CGameObject*		pGameObject = nullptr;
	//pGameObject = CMiniVacuum::Create(m_pGraphicDev, &tObj);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Minivacuum", pGameObject, this), E_FAIL);
	//auto vecBody = pGameObject->Is<CMiniVacuum>()->Get_HozeBody();

	//m_pScene->addAggregate(*pGameObject->Is<CMiniVacuum>()->Get_Hoze());
	//for (auto pBody : vecBody)
	//	setupFiltering(Engine::Get_Allocator(), pBody, FilterGroup::eHoze, FilterGroup::eCody | FilterGroup::eMay);
	//m_mapLayer.emplace(szLayer, pLayer);

	return S_OK;
}

HRESULT CVacuumField3::Ready_Trigger_Layer(const _tchar * szLayer)
{
	Engine::CLayer*			pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	OBJINFO tObj;
	ZeroMemory(&tObj, sizeof(OBJINFO));
	tObj.eSceneID = RESOURCE_VF;
	// 오브젝트 추가

	Engine::CGameObject*		pGameObject = nullptr;

	//// 터널
	pGameObject = CCameraSpace::Create(m_pGraphicDev, { 13.67f, 2.55f, -97.85f }, 19.84f, 6.25f, 31.14f, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CameraEvent1", pGameObject, this), E_FAIL);

	// 관람차
	pGameObject = CCameraSpace::Create(m_pGraphicDev, { 107.933685f,-0.02f, -142.469f }, 8.34f, 20.05f, 17.84f, &tObj);
	pGameObject->Is<CCameraSpace>()->Set_Fix({ 58.933685f,4.02f, -144.469f }, { 107.933685f,4.02f, -144.469f });
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CameraEvent2", pGameObject, this), E_FAIL);

	//tObj.vTriggerPos = { 107.933685f,-4.02f, -142.469f };
	//tObj.vTriggerScale = { 8.34f, 17.05f, 17.34f };
	//pGameObject = CTriggerView::Create(m_pGraphicDev, TRIGGER_ATTACK, &tObj);;
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CameraEvent3", pGameObject, this), E_FAIL);

	m_mapLayer.emplace(szLayer, pLayer);
	return S_OK;
}
