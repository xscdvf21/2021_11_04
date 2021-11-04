#include "stdafx.h"
#include "VacuumField.h"
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

#include "CollisionMgr.h"
#include "Orb.h"
#include "TestWall.h"
#include "MiniVacuum.h"
#include "Map.h"
#include "FuseBox.h"
#include "FuseNPC.h"
#include "PowerSwitch.h"

#include "VacuumToolBox.h"

#include "VacuumFieldPart5.h"
#include "SceneChangeSpace.h"
#include "CameraSpace.h"
#include "PowerSwitch.h"
#include "BandSaw.h"
#include "DeadSpace.h"
#include "SaveSpace.h"
#include "WallSlideCylinder.h"
#include "WallSlideCylinderSurface.h"
#include "GrindStone.h"
#include "Cogs.h"
#include "PowerButton.h"
#include "CircleSaw.h"
#include "Pedestal.h"

#include "BigPlug.h"
#include "Cord.h"
#include "TriggerView.h"
#include "LineObject.h"
#include "StaticHoze.h"
#include "LightObject.h"
#include "SoundSpace.h"


CVacuumField::CVacuumField(LPDIRECT3DDEVICE9 pDevice)
	: CScene(pDevice)
{
}

CVacuumField::~CVacuumField()
{
}

HRESULT CVacuumField::Ready_Scene()
{
	Engine::Set_SoundScene(RESOURCE_VF);
	Engine::Start_Split();
	Engine::Set_BlendPass(1);

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

	//pPlane->setName((char*)pPlane);
	//setupFiltering(Engine::Get_Allocator(), pPlane, FilterGroup::eGround, FilterGroup::eCody | FilterGroup::eMay);
	//m_pScene->addActor(*pPlane);
	m_pScene->setGravity(PxVec3(0.0f, -20.f, 0.0f));
	FAILED_CHECK_RETURN(Ready_Environment_Layer(L"Environment"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_GameObject_Layer(L"GameObject"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Interact_Layer(L"Interact"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Trigger_Layer(L"Trigger"), E_FAIL);


	StopAll();
	PlayBGM(L"VacummField_BGM.wav", 1.f);

	//FAILED_CHECK_RETURN(Ready_LoadMap_Layer(L"LoadObject", L"../../Data/ITT_Test/Test.dat"), E_FAIL);

	Ready_LightInfo();

	return S_OK;
}

_int CVacuumField::Update_Scene(const _float & fTimeDelta)
{
	if (m_bStart)
	{
		StopNPlaySound(L"VF_Start.ogg", CSoundMgr::CHANNELID::CrusherHitPoint, 0.3f);
		m_bStart = false;
	}

	m_pScene->simulate(fTimeDelta);

	g_vBackBufferColor;

	m_pScene->fetchResults(true);
	_int iExit = Engine::CScene::Update_Scene(fTimeDelta);
	if (Engine::Key_Down(DIK_Y))
	{

		CHANGE_SCENE(m_pGraphicDev, SCENE_VF2, SCENE_CHANGE);

		return SCENE_CHANGE;
	}
	return iExit;
}

_int CVacuumField::LateUpdate_Scene(const _float & fTimeDelta)
{
	_int iExit = Engine::CScene::LateUpdate_Scene(fTimeDelta);

	return iExit;
}

void CVacuumField::Render_Scene()
{
	CCollisionMgr::GetInstance()->Collision_Player_Monster();
	CCollisionMgr::GetInstance()->Clear_List();
}

void CVacuumField::onConstraintBreak(PxConstraintInfo* constraints, PxU32 count)
{

	return;
}

void CVacuumField::onWake(PxActor** actors, PxU32 count)
{
	OutputDebugString(L"깼음");
	return;
}

void CVacuumField::onSleep(PxActor** actors, PxU32 count)
{
	OutputDebugString(L"잠듬");

	return;
}

void CVacuumField::onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs)
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

void CVacuumField::onTrigger(PxTriggerPair* pairs, PxU32 count)
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
		if (Trigger.otherShape->getSimulationFilterData().word0 &(FilterGroup::eCody | FilterGroup::eMay | FilterGroup::eArrow))
		{
			auto* pTrigger = ToTrigger(Trigger.triggerShape->getName());

			if (!pTrigger)
				return;
			switch (pTrigger->Get_Type())
			{
			case TRIGGER_FUSE:
				Event_Fuse(&Trigger);
				break;
			case TRIGGER_SAVESPACE:
				Event_Save(&Trigger);
				break;
			case TRIGGER_FUSE_SLOT:
				Event_Insert_Fuse(&Trigger);
				break;
			case TRIGGER_VACUUM_ENTER:
				Event_Vacuum_Enter(&Trigger);
				break;
			case TRIGGER_POWERSWITCH:
				Event_PowerSwitch(&Trigger);
				break;
			case TRIGGER_SCENE_CHANGE:
				Event_Scene_Change(&Trigger);
				break;
			case TRIGGER_CAMERA_CHANGE:
				Event_Camera_Change(&Trigger);
				break;
			case TRIGGER_CAMERA_CHANGE_FIX:
				Event_Camera_Change_Fix(&Trigger);
				break;
			case TRIGGER_DEADSPACE:
				Event_Dead(&Trigger);
				break;
			case TRIGGER_SHOOT:
				Event_Fly(&Trigger);
				break;
			case TRIGGER_SAW_JUMP:
				Event_SawJump(&Trigger);
				break;
			case TRIGGER_SOUND:
				Event_Sound(&Trigger);
				break;
			}
		}
	}
}

void CVacuumField::onAdvance(const PxRigidBody*const* bodyBuffer, const PxTransform* poseBuffer, const PxU32 count)
{
	OutputDebugString(L"업데이트");
}


void CVacuumField::Event_Fuse(PxTriggerPair* pairs)
{
	CFuseNPC* pFuse = ToTrigger(pairs->triggerShape->getActor()->getName())->Is<CFuseNPC>();

	NULL_CHECK_RETURN(pFuse, );

	auto* pTrigger = ToTrigger(pairs->triggerShape->getName());

	if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
	{
		if (pairs->otherShape->getSimulationFilterData().word0&FilterGroup::eCody &&
			!pFuse->Get_Carried())
		{
			CCody* pCody = ToObj<CCody>(pairs->otherShape->getName());
			if(pCody && pCody->Get_PlayerState() != CCody::PL_PICKUP)
				pCody->Set_PickUp(true, CCody::FUSE, pFuse);	
		}
		else if (pairs->otherShape->getSimulationFilterData().word0&FilterGroup::eMay && 
			!pFuse->Get_Carried())
		{
			CMay* pMay = ToObj<CMay>(pairs->otherShape->getName());
			if (pMay && pMay->Get_PlayerState() != CCody::PL_PICKUP)
				pMay->Set_PickUp(true, CCody::FUSE, pFuse);
		}
	}
	else if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_LOST)
	{
		if (pairs->otherShape->getSimulationFilterData().word0&FilterGroup::eCody)
		{
			CCody* pCody = ToObj<CCody>(pairs->otherShape->getName());
			if (!pCody)
				return;
			if(pCody->Get_PlayerState() != CCody::PL_PICKUP)
				pCody->Set_PickUp(false, CCody::FUSE, nullptr);
		}
		else if (pairs->otherShape->getSimulationFilterData().word0&FilterGroup::eMay)
		{
			CMay* pMay = ToObj<CMay>(pairs->otherShape->getName());
			if (!pMay)
				return;
			if (pMay->Get_PlayerState() != CCody::PL_PICKUP)
				pMay->Set_PickUp(false, CCody::FUSE, nullptr);
		}
	}
}

void CVacuumField::Event_Lever(PxTriggerPair* pairs)
{

}

void CVacuumField::Event_Vacuum_Enter(PxTriggerPair* pairs)
{
	if (pairs->status != PxPairFlag::eNOTIFY_TOUCH_FOUND)
		return;
	if (pairs->otherShape->getSimulationFilterData().word0 == FilterGroup::eCody)
	{
		CTrigger* pTrigger = ToTrigger(pairs->triggerShape->getActor()->getName());
		void* pArg = pTrigger->Activate((void*)pairs->otherShape->getName());
		if (!pArg)
			return;
		vector<vector<_vec3>*> vecPath;
		vecPath = *static_cast<vector<vector<_vec3>*>*>(pArg);

		ToObj<CCody>(pairs->otherActor->getName())->Set_Slide(false, pTrigger->Get_FloatValue(0), pTrigger->Get_FloatValue(1), vecPath, pTrigger->Get_FloatValue(2));
	}
	if (pairs->otherShape->getSimulationFilterData().word0 == FilterGroup::eMay)
	{
		CTrigger* pTrigger = ToTrigger(pairs->triggerShape->getActor()->getName());

		void* pArg = pTrigger->Activate((void*)pairs->otherShape->getName());
		if (!pArg)
			return;
		vector<vector<_vec3>*> vecPath;
		vecPath = *static_cast<vector<vector<_vec3>*>*>(pArg);
		ToObj<CMay>(pairs->otherActor->getName())->Set_Slide(false, pTrigger->Get_FloatValue(0), pTrigger->Get_FloatValue(1), vecPath, pTrigger->Get_FloatValue(2));
	}
}

void CVacuumField::Event_Insert_Fuse(PxTriggerPair* pairs)
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
		CMay* pCody = ToObj<CMay>(pairs->otherShape->getName());
		if (!pCody)
			return;
		if (pCody->Is_Carring() &&
			pCody->Get_CarringType() == CMay::PICKUP_TYPE::FUSE)
		{
			if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
				pCody->Set_Interactable((void*)pairs->triggerActor->getName());
			if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_LOST)
				pCody->Set_Interactable((void*)pairs->triggerActor->getName(), false);
		}
	}

}

void CVacuumField::Event_PowerSwitch(PxTriggerPair* pairs)
{
	auto* pTrigger = ToTrigger(pairs->triggerShape->getName());

	CGameObject* pPowerSwitch = pTrigger->Is<CGameObject>();

	if (pairs->otherShape->getSimulationFilterData().word0&FilterGroup::eCody)
	{
		CCody* pCody = ToObj<CCody>(pairs->otherShape->getName());
		if (!pCody)
			return;

		if (pCody->Get_PlayerState() == CCody::PL_PICKUP)
			return;

		if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			pCody->Set_HangSwitch(true, pPowerSwitch);
			pTrigger->Set_Active(true);
		}
		else if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_LOST)
		{
			pCody->Set_HangSwitch(false, nullptr);
			pTrigger->Set_Active(false);
		}
	}
	else if (pairs->otherShape->getSimulationFilterData().word0&FilterGroup::eMay)
	{
		CMay* pMay = ToObj<CMay>(pairs->otherShape->getName());

		if (pMay->Get_PlayerState() == CCody::PL_PICKUP)
			return;

		if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{

			pMay->Set_HangSwitch(true, pPowerSwitch);
			pTrigger->Set_Active(true);
		}
		else if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_LOST)
		{
			pMay->Set_HangSwitch(false, nullptr);
			pTrigger->Set_Active(false);
		}
	}
}

void CVacuumField::Event_Scene_Change(PxTriggerPair* pairs)
{
	ToTrigger(pairs->triggerActor->getName())->Activate((void*)pairs->otherShape->getName());
}

void CVacuumField::Event_Camera_Change(PxTriggerPair* pairs)
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

			if (CAMERA_STALKING == pCameraSpace->Get_Mode())
				StopNPlaySound(L"VF_Tunnel.ogg", CSoundMgr::CHANNELID::CrusherHitPoint, 0.3f);


		}
		else if (pairs->otherShape->getSimulationFilterData().word0 & FilterGroup::eCody)
		{
			auto* pCamera = Engine::Get_GameObject(L"GameObject", L"CodyCamera")->Is<CCodyCamera>();
			pCamera->Set_Mode(pCameraSpace->Get_Mode());
			if (CAMERA_FIX == pCameraSpace->Get_Mode())
				pCamera->Set_Fix(pCameraSpace->Get_At(), pCameraSpace->Get_Eye());
		}
	}
	else if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_LOST)
	{
		if (pairs->otherShape->getSimulationFilterData().word0 & FilterGroup::eMay)
		{
			auto* pCamera = Engine::Get_GameObject(L"GameObject", L"MayCamera")->Is<CMayCamera>();
			pCamera->Set_Mode(CAMERA_DEFAULT);

		}
		else if (pairs->otherShape->getSimulationFilterData().word0 & FilterGroup::eCody)
		{
			auto* pCamera = Engine::Get_GameObject(L"GameObject", L"CodyCamera")->Is<CCodyCamera>();
			pCamera->Set_Mode(CAMERA_DEFAULT);

		}
	}
}

void CVacuumField::Event_Camera_Change_Fix(PxTriggerPair * pairs)
{
	if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
	{
		if (pairs->otherShape->getSimulationFilterData().word0 & FilterGroup::eMay)
		{
			auto* pCamera = Engine::Get_GameObject(L"GameObject", L"MayCamera")->Is<CMayCamera>();
			pCamera->Set_Mode(CAMERA_FIX);
			pCamera->Set_Fix({ 55.2f, 18.3f, 35.0f }, { 55.2f, 18.3f, 18.0f });

		}
		else if (pairs->otherShape->getSimulationFilterData().word0 & FilterGroup::eCody)
		{
			auto* pCamera = Engine::Get_GameObject(L"GameObject", L"CodyCamera")->Is<CCodyCamera>();
			pCamera->Set_Mode(CAMERA_FIX);
			pCamera->Set_Fix({ 55.2f, 18.3f, 35.0f }, { 55.2f, 18.3f, 18.0f });
		}
	}
	else if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_LOST)
	{
		if (pairs->otherShape->getSimulationFilterData().word0 & FilterGroup::eMay)
		{
			auto* pCamera = Engine::Get_GameObject(L"GameObject", L"MayCamera")->Is<CMayCamera>();
			pCamera->Set_Mode(CAMERA_DEFAULT);

		}
		else if (pairs->otherShape->getSimulationFilterData().word0 & FilterGroup::eCody)
		{
			auto* pCamera = Engine::Get_GameObject(L"GameObject", L"CodyCamera")->Is<CCodyCamera>();
			pCamera->Set_Mode(CAMERA_DEFAULT);

		}
	}
}

void CVacuumField::Event_Dead(PxTriggerPair * pairs)
{
	if (pairs->otherShape->getSimulationFilterData().word0 == FilterGroup::eCody &&
		pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
	{
		CCody* pCody = ToObj<CCody>(pairs->otherShape->getName());
		NULL_CHECK_RETURN(pCody, );

		pCody->Set_PlayerDead();
	}
	else if (pairs->otherShape->getSimulationFilterData().word0 == FilterGroup::eMay&&
		pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
	{
		CMay* pMay = ToObj<CMay>(pairs->otherShape->getName());
		NULL_CHECK_RETURN(pMay, );

		pMay->Set_PlayerDead();
	}
}

void CVacuumField::Event_Save(PxTriggerPair * pairs)
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

void CVacuumField::Event_Shoot(PxTriggerPair * pairs)
{
	if (pairs->otherShape->getSimulationFilterData().word0 == FilterGroup::eArrow &&
		pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
	{
		ToObj<CGameObject>(pairs->otherActor->getName())->On_Active(nullptr);
	}
}

void CVacuumField::Event_Fly(PxTriggerPair * pairs)
{
	auto* pTrigger = ToTrigger(pairs->triggerShape->getName());

	CGameObject* pCircleSaw = pTrigger->Is<CGameObject>();

	if (pairs->otherShape->getSimulationFilterData().word0&FilterGroup::eCody)
	{
		CCody* pCody = ToObj<CCody>(pairs->otherShape->getName());
		if (!pCody)
			return;
		if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			pCody->Set_Fly(true, pCircleSaw->Get_ObjTag());
		}
		else if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_LOST)
		{
			pCody->Set_Fly(false, pCircleSaw->Get_ObjTag());
		}
	}
	else if (pairs->otherShape->getSimulationFilterData().word0&FilterGroup::eMay)
	{
		CMay* pMay = ToObj<CMay>(pairs->otherShape->getName());
		if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			pMay->Set_Fly(true, pCircleSaw->Get_ObjTag());
		}
		else if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_LOST)
		{
			pMay->Set_Fly(false);
		}
	}
}

void CVacuumField::Event_SawJump(PxTriggerPair * pairs)
{
	if (pairs->otherShape->getSimulationFilterData().word0&FilterGroup::eCody)
	{
		CCody* pCody = ToObj<CCody>(pairs->otherShape->getName());
		if (!pCody)
			return;
		pCody->Set_SawJumpTrigger(true);
	}
	else if (pairs->otherShape->getSimulationFilterData().word0&FilterGroup::eMay)
	{
		CMay* pMay = ToObj<CMay>(pairs->otherShape->getName());
		if (!pMay)
			return;
		pMay->Set_SawJumpTrigger(true);
	}
}

void CVacuumField::Event_Sound(PxTriggerPair * pairs)
{
	auto* pTrigger = ToTrigger(pairs->triggerShape->getName());
	auto* pSound =pTrigger->Is<CSoundSpace>();

	if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
	{

		switch (pSound->Get_SoundEnum())
		{
		case 0:
			StopNPlaySound(L"VF_Concent.ogg", CSoundMgr::CHANNELID::CrusherHitPoint, 0.3f);

			break;

		case 1:
			break;

		default:
			break;
		}
		pTrigger->Set_Interactable(false);
	}
	
}

HRESULT CVacuumField::Ready_Environment_Layer(const _tchar * pLayerTag)
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
//#스테이지	청소기 필드
HRESULT CVacuumField::Ready_GameObject_Layer(const _tchar * pLayerTag)
{
	Engine::CLayer*			pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	OBJINFO tObj;
	ZeroMemory(&tObj, sizeof(OBJINFO));
	tObj.eSceneID = RESOURCE_VF;
	// 오브젝트 추가
	Engine::CGameObject*		pGameObject = nullptr;
	tObj.vScale = { 0.01f, 0.01f ,0.01f };

	//// MainCamera
	//pGameObject = CMainCamera::Create(m_pGraphicDev, &_vec3(0.f, 10.f, -5.f), &_vec3(0.f, 0.f, 1.f), &_vec3(0.f, 1.f, 0.f), D3DXToRadian(60.f), _float(WINCX) / WINCY, 0.1f, 1000.f);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MainCamera", pGameObject), E_FAIL);

	// CodyCamera

	pGameObject = CCodyCamera::Create(m_pGraphicDev, &_vec3(0.f, 10.f, -5.f), &_vec3(0.f, 0.f, 1.f), &_vec3(0.f, 1.f, 0.f), D3DXToRadian(60.f), _float(WINCX) / WINCY, 0.1f, 1000.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CodyCamera", pGameObject, this), E_FAIL);

	// MayCamera
	pGameObject = CMayCamera::Create(m_pGraphicDev, &_vec3(0.f, 10.f, -5.f), &_vec3(0.f, 0.f, 1.f), &_vec3(0.f, 1.f, 0.f), D3DXToRadian(60.f), _float(WINCX) / WINCY, 0.1f, 1000.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MayCamera", pGameObject, this), E_FAIL);

#ifdef _DEBUG
	tObj.vPos = { 51.5f, -3.0f, -3.3f }; // 툴박스앞
	tObj.vPos = { -82.0f, 9.57f, 75.0f }; // BandSaw
	//tObj.vPos = { -83.8f, -4.52f, 0.8f };
#endif

	tObj.vPos = { -52.5f, 10.0f, -25.6f }; // 마지막
	tObj.vPos = { -18.335f, 45.22f, -29.22f }; // 미끄럼틀앞
	tObj.vPos = { 54.2f, 11.3f, 30.0f }; // 벽앞
	tObj.vPos = { -45.83f,21.0f,38.f }; // 터널 앞
	tObj.vPos = { 50.228f,-4.584f,-4.8683f }; // 파워스위치 앞
	tObj.vPos = { -82.8f, 15.3f, 113.7f }; // Test Machine1
	tObj.vPos = { -83.8f, -5.52f, 0.8f }; // 전기톱
	tObj.vPos = { 0.f,2.f,0.f };


	pGameObject = CCody::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Cody", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eCody, FilterGroup::eGround | FilterGroup::eWall);

	tObj.vPos.x += 2.0f;

	pGameObject = CMay::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"May", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMay, FilterGroup::eGround | FilterGroup::eWall);

	//pGameObject = CMiniVacuumHub::Create(m_pGraphicDev, 20);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MiniVacuumHub", pGameObject), E_FAIL);

	//pGameObject = CMiniVacuumHead::Create(m_pGraphicDev, &Objinfo);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MiniVacuumHead", pGameObject), E_FAIL);

	//56.19f,-2.69f,-12.48f
	tObj.vPos = { 56.19f,-2.69f,-12.48f };
	//tObj.vScale = _vec3{ 0.007f,0.007f,0.007f };

	pGameObject = CFuseBox::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"FuseBox", pGameObject, this), E_FAIL);

	tObj.vPos = { 53.62f,-0.98f,-4.61f };
	tObj.vAngle = { 0.f,0.f,D3DXToRadian(-15.f) };
	pGameObject = CPowerSwitch::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"PowerSwitch", pGameObject, this), E_FAIL);

	//tObj.vPos = { 50.19f,-2.69f,-12.48f };
	tObj.vScale = _vec3{ 0.01f, 0.01f, 0.01f };
	tObj.vAngle = { 0.f,0.f,0.f };

	tObj.vPos = { 56.71f, -2.1531F, -11.0056f };
	tObj.vAngle = { D3DXToRadian(0.0f), D3DXToRadian(-270.0f), D3DXToRadian(0.0f) };
	pGameObject = CFuseNPC::Create(m_pGraphicDev, CFuseNPC::Shed_Awakening_FuseSocket_JumpOut_FuseCrab2, 0, &tObj, true);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"FuseNPC", pGameObject, this), E_FAIL);
	tObj.vPos = { 56.04f,-2.829F,-12.320f };
	tObj.vAngle = { D3DXToRadian(0.0f), D3DXToRadian(-270.0f), D3DXToRadian(-90.0f) };
	pGameObject = CFuseNPC::Create(m_pGraphicDev, CFuseNPC::Shed_Awakening_FuseSocket_JumpOut_FuseCrab1, 1, &tObj, true);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"FuseNPC1", pGameObject, this), E_FAIL);
	tObj.vPos = { 56.0116f,-2.27996F,-13.2498f };
	tObj.vAngle = { D3DXToRadian(0.0f), D3DXToRadian(-270.0f), D3DXToRadian(0.0f) };
	pGameObject = CFuseNPC::Create(m_pGraphicDev, CFuseNPC::Shed_Awakening_FuseSocket_JumpOut_FuseCrab1, 2, &tObj, true);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"FuseNPC2", pGameObject, this), E_FAIL);

	lstrcpy(tObj.tagMesh, L"VacuumField");
	//tObj.vScale = _vec3{ 0.007f,0.007f,0.007f };
	tObj.eMeshType = 1;
	tObj.eRenderType = RENDER_NONALPHA;
	tObj.vPos = { 0.f,0.f,0.f };
	tObj.vAngle = { 0.0f, 0.0f, 0.0f };
	tObj.vScale = { 0.01f, 0.01f, 0.01f };
	pGameObject = CMap::Create(m_pGraphicDev, &tObj);
	static_cast<CMap*>(pGameObject)->Set_MaterialIndex(0);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Field", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eGround, FilterGroup::eCody | FilterGroup::eMay);

	lstrcpy(tObj.tagMesh, L"VacuumField_Metal");
	pGameObject = CMap::Create(m_pGraphicDev, &tObj);
	static_cast<CMap*>(pGameObject)->Set_MaterialIndex(1);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Field_Metal", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eGround, FilterGroup::eCody | FilterGroup::eMay);

	// 툴박스
	{
		tObj.vPos = { 0.9f, 25.0f, 35.0f };
		lstrcpy(tObj.tagMesh, L"Minivacuum");
		pGameObject = CVacuumToolBox::Create(m_pGraphicDev, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"AirFlowMeter", pGameObject, this), E_FAIL);
		setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eRideObj, FilterGroup::eCody | FilterGroup::eMay);
	}

	//BandSaw 세트
	{
		tObj.vPos = { -82.255f, 9.73f, 73.7f };
		pGameObject = CBandSaw::Create(m_pGraphicDev, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"BandSaw", pGameObject, this), E_FAIL);

		tObj.vTriggerPos = { -82.255f, 9.73f, 73.7f };
		tObj.vTriggerScale = _vec3{ 0.2f, 20.0f, 0.2f };
		//tObj.
		pGameObject = CDeadSpace::Create(m_pGraphicDev, L"", false, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DeadSpace1", pGameObject, this), E_FAIL);
	}

	// 머신1
	{
		tObj.vPos = { -84.2f, 15.7f, 87.2f };
		pGameObject = CWallSlideCylinder::Create(m_pGraphicDev, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"WallSlideCylinder", pGameObject, this), E_FAIL);
		setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eRideObj, FilterGroup::eCody | FilterGroup::eMay);

		tObj.vPos = { -84.2f, 15.7f, 87.2f };
		pGameObject = CWallSlideCylinderSurface::Create(m_pGraphicDev, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"WallSlideCylinderSurface", pGameObject, this), E_FAIL);
		setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eStone, FilterGroup::eCody | FilterGroup::eMay);
	}

	//돌
	{
		tObj.eRenderType = 1;
		tObj.vAngle = { D3DXToRadian(0.0f), D3DXToRadian(0.0f), D3DXToRadian(0.0f) };
		lstrcpy(tObj.tagMesh, L"LargeCogs");
		tObj.vPos = { -76.8f, 14.62f, 112.94f };
		pGameObject = CCogs::Create(m_pGraphicDev, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"LargeCogs1", pGameObject, this), E_FAIL);
		setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eCody | FilterGroup::eMay);

		tObj.eRenderType = 0;
		tObj.vAngle = { D3DXToRadian(0.9f), D3DXToRadian(0.0f), D3DXToRadian(0.0f) };
		lstrcpy(tObj.tagMesh, L"LargeCogs");
		tObj.vPos = { -76.8f, 29.8f, 107.79f };
		pGameObject = CCogs::Create(m_pGraphicDev, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"LargeCogs2", pGameObject, this), E_FAIL);
		setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eCody | FilterGroup::eMay);

		tObj.vPos = { -74.39f, 14.62f, 112.94f };
		tObj.vTriggerScale = { 3.0f, 3.0f, 3.0f };
		pGameObject = CGrindStone::Create(m_pGraphicDev, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"GrindStone", pGameObject, this), E_FAIL);
		setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eStone, FilterGroup::eCody | FilterGroup::eMay);
		pGameObject->Is<CGrindStone>()->Add_List(L"LargeCogs1");
		pGameObject->Is<CGrindStone>()->Add_List(L"LargeCogs2");

		CGameObject* pButton = nullptr;
		tObj.vPos = { -65.18f, 7.63f, 111.1f };
		pButton = CPowerButton::Create(m_pGraphicDev, &tObj, false);
		NULL_CHECK_RETURN(pButton, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"PowerButton1", pButton, this), E_FAIL);
		setupFiltering(Engine::Get_Allocator(), pButton->Get_RigidActor(), FilterGroup::eButton, FilterGroup::eCody | FilterGroup::eMay);
		pButton->Set_InteractObj(pGameObject);

		tObj.vPos = { -65.18f, 7.63f, 115.6f };
		pButton = CPowerButton::Create(m_pGraphicDev, &tObj, false);
		NULL_CHECK_RETURN(pButton, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"PowerButton2", pButton, this), E_FAIL);
		setupFiltering(Engine::Get_Allocator(), pButton->Get_RigidActor(), FilterGroup::eButton, FilterGroup::eCody | FilterGroup::eMay);
		pButton->Set_InteractObj(pGameObject);
	}

	Engine::CGameObject*		pSaw = nullptr;
	// 전기톱
	{
		// 받침대
		tObj.vPos = { -68.6f, -5.6F, -32.58f };
		tObj.vScale = _vec3{ 0.01f, 0.01f, 0.01f };
		tObj.vTriggerPos = { 25.05f, 33.85F, -28.97f };
		tObj.vTriggerScale = _vec3{ 38.79f, 5.15f, 7.1f };
		tObj.vTriggerAngle = { 0.0f, 0.0f, D3DXToRadian(-30.0f) };

		pGameObject = CPedestal::Create(m_pGraphicDev, &tObj);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Pedestal", pGameObject, this), E_FAIL);
		// 플러그

		tObj.vPos = { -87.39f, -2.63F, -32.94f };
		tObj.vScale = _vec3{ 0.015f, 0.015f, 0.015f };
		tObj.vAngle = _vec3{ 0.0f, 5.917f, 0.00037f };

		pGameObject = CBigPlug::Create(m_pGraphicDev, &tObj);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"BigPlug1", pGameObject, this), E_FAIL);
		setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eButton, FilterGroup::eCody | FilterGroup::eMay);

		// 전기톱
		tObj.vPos = { -66.1375f, -2.348F, -32.58f };
		tObj.vScale = _vec3{ 0.015f, 0.015f, 0.015f };
		tObj.vAngle = _vec3{ -0.82f, D3DXToRadian(90.0f), 0.0f };
		tObj.vTriggerPos = { -71.85f, -4.12F, -30.81f };
		tObj.vTriggerScale = { 1.25f, 1.0f, 3.15f };

		pSaw = CCircleSaw::Create(m_pGraphicDev, &tObj);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CircleSaw", pSaw, this), E_FAIL);
		pSaw->Set_ObjTag(L"CircleSaw");

		tObj.vPos = { -86.70f, -5.6F, -31.78f };
		tObj.vScale = _vec3{ 0.015f, 0.015f, 0.015f };
		tObj.vAngle = _vec3{ 0.0f, 0.516512f, 0.0f };

		pGameObject = CCord::Create(m_pGraphicDev, &tObj);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Cord1", pGameObject, this), E_FAIL);

		tObj.vPos = { -86.70f, -5.6F, -31.78f };
		tObj.vScale = _vec3{ 0.015f, 0.015f, 0.015f };
		pGameObject = CLineObject::Create(m_pGraphicDev, &tObj, L"../../Data/VF1Line.dat");
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CordLine", pGameObject, this), E_FAIL);
		pGameObject->Set_InteractObj(pSaw);

		CGameObject* pHozeHead = nullptr;
		tObj.vAngle = { 0.f ,-90.f,0.f };

		vector<_wstr> vecPath;
		vecPath.emplace_back(L"../../Data/VF1LineEnd.dat");
		pGameObject = pHozeHead = CStaticHoze::Create(m_pGraphicDev, L"StaticHoze4", vecPath, 1, true, &tObj);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"StaticHoze", pGameObject, this), E_FAIL);
		pGameObject->Is<CStaticHoze>()->Set_ShootData(0.f, 4, 2);

		CTrigger* pTrigger = CTrigger::CreateBoxTrigger(m_pScene, pGameObject, { 67.f,-3.6f,11.f }, 3.f, 1.f, 1.f, TRIGGERTYPE::TRIGGER_VACUUM_ENTER);
		pTrigger->Set_FloatValue(0.f);
		pTrigger->Set_FloatValue(4);
		pTrigger->Set_FloatValue(2);

		D3DLIGHT9				tLightInfo;
		ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));

		tLightInfo.Type = D3DLIGHT_POINT;
		tLightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
		tLightInfo.Ambient = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.f);


		tLightInfo.Diffuse = D3DXCOLOR(1.0f,0.8f,0.6f, 1.f);
		tLightInfo.Position = _vec3(77.27f, 7.89f, 10.52f);
		tLightInfo.Range = 15.f;
		CGameObject* pLight = nullptr;
		pLight = CLightObject::Create(m_pGraphicDev,&tLightInfo,14);
		NULL_CHECK_RETURN(pLight, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Light1", pLight, this), E_FAIL);

		tObj.vPos = { -86.70f, -5.6F, -31.78f };
		tObj.vScale = _vec3{ 0.015f, 0.015f, 0.015f };
		pGameObject = CLineObject::Create(m_pGraphicDev, &tObj, L"../../Data/VF1SceneChange.dat");
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SceneChangeLine", pGameObject, this), E_FAIL);
		pGameObject->Set_InteractObj(pHozeHead);
		pGameObject->Set_InteractObj(pLight);


		tObj.vTriggerPos = { 56.92f, 9.41F, -28.87f };
		tObj.vTriggerScale = { 1.25f, 4.55f, 3.15f };

		pGameObject = CTriggerView::Create(m_pGraphicDev, TRIGGER_SAW_JUMP, &tObj);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"TriggerJump", pGameObject, this), E_FAIL);
	}

	// 데드 존
	{
		// 맵전체
		tObj.vTriggerPos = { 0.0f, -30.0f, 0.0f };
		tObj.vTriggerScale = _vec3{ 300.0f, 1.0f, 300.0f };
		tObj.vPos = { 0.0f, -8, 0.0f };
		tObj.vScale= { 0.2f, 0.01f, 0.2f};
		pGameObject = CDeadSpace::Create(m_pGraphicDev, L"Water", true, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DeadSpace2", pGameObject, this), E_FAIL);

		// 툴박스
		tObj.vTriggerPos = { 0.8f, -10.0f, 33.6f };
		tObj.vTriggerScale = _vec3{ 20.0f, 1.0f, 20.0f };
		//tObj.
		pGameObject = CDeadSpace::Create(m_pGraphicDev, L"", g_bRenderBox, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DeadSpace3", pGameObject, this), E_FAIL);

		// 미끄럼틀 앞
		tObj.vTriggerPos = { 76.7f, -15.0f, -22.3f };
		tObj.vTriggerScale = _vec3{ 20.0f, 1.0f, 20.0f };
		//tObj.
		pGameObject = CDeadSpace::Create(m_pGraphicDev, L"", g_bRenderBox, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DeadSpace4", pGameObject, this), E_FAIL);

		// 버튼 뒤
		tObj.vTriggerPos = { -45.5f, -5.0f, 129.9f };
		tObj.vTriggerScale = _vec3{ 60.0f, 1.0f, 30.0f };
		//tObj.
		pGameObject = CDeadSpace::Create(m_pGraphicDev, L"", g_bRenderBox, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DeadSpace5", pGameObject, this), E_FAIL);

		// 머신
		tObj.vTriggerPos = { -86.0f, -20.0f, 68.7f };
		tObj.vTriggerScale = _vec3{ 40.0f, 1.0f, 40.0f };
		//tObj.
		pGameObject = CDeadSpace::Create(m_pGraphicDev, L"", g_bRenderBox, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DeadSpace6", pGameObject, this), E_FAIL);
	}

	// 세이브 존
	{
		tObj.vPos = { 25.85f, 25.7f + 10.0f, 30.17f };
		tObj.vScale = _vec3{ 1.0f, 10.0f, 10.0f };
		pGameObject = CSaveSpace::Create(m_pGraphicDev, _vec3(25.85f, 25.7f, 30.17f), L"Yellow_Bg", g_bRenderBox, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SaveSpace1", pGameObject, this), E_FAIL);

		tObj.vPos = { -21.56f, 23.3f + 10.0f, 36.0f };
		tObj.vScale = _vec3{ 1.0f, 10.0f, 10.0f };
		pGameObject = CSaveSpace::Create(m_pGraphicDev, _vec3(-21.56f, 23.3f, 36.0f), L"Yellow_Bg", g_bRenderBox, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SaveSpace2", pGameObject, this), E_FAIL);

		tObj.vPos = { -30.32f, 13.58f + 10.0f, 114.32f };
		tObj.vScale = _vec3{ 10.0f, 10.0f, 1.0f };
		pGameObject = CSaveSpace::Create(m_pGraphicDev, _vec3(-30.32f, 13.58f, 114.32f), L"Yellow_Bg", g_bRenderBox, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SaveSpace3", pGameObject, this), E_FAIL);

		tObj.vPos = { -51.64f, 11.2f + 10.0f, 130.14f };
		tObj.vScale = _vec3{ 1.0f, 10.0f, 15.0f };
		pGameObject = CSaveSpace::Create(m_pGraphicDev, _vec3(-51.64f, 11.2f, 130.14f), L"Yellow_Bg", g_bRenderBox, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SaveSpace4", pGameObject, this), E_FAIL);

		tObj.vPos = { -70.6f, 16.9f + 3.0f, 114.7f };
		tObj.vScale = _vec3{ 2.0f, 3.0f, 2.0f };
		pGameObject = CSaveSpace::Create(m_pGraphicDev, _vec3(-70.03f, 16.9f, 111.7f), L"Yellow_Bg", g_bRenderBox, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SaveSpace5", pGameObject, this), E_FAIL);

		tObj.vPos = { -83.7f, 14.7f + 10.0f, 100.0f };
		tObj.vScale = _vec3{ 5.0f, 10.0f, 1.0f };
		pGameObject = CSaveSpace::Create(m_pGraphicDev, _vec3(-83.7f, 14.7f, 100.0f), L"Yellow_Bg", g_bRenderBox, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SaveSpace6", pGameObject, this), E_FAIL);

		tObj.vPos = { -81.7f, 9.75f + 10.0f, 79.1f };
		tObj.vScale = _vec3{ 8.0f, 10.0f, 1.0f };
		pGameObject = CSaveSpace::Create(m_pGraphicDev, _vec3(-81.7f, 10.0f, 79.1f), L"Yellow_Bg", g_bRenderBox, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SaveSpace7", pGameObject, this), E_FAIL);

		tObj.vPos = { -81.85f, -5.24f + 20.0f, 38.57f };
		tObj.vScale = _vec3{ 20.0f, 20.0f, 1.0f };
		pGameObject = CSaveSpace::Create(m_pGraphicDev, _vec3(-81.85f, -5.24f, 38.57f), L"Yellow_Bg", g_bRenderBox, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SaveSpace8", pGameObject, this), E_FAIL);

		tObj.vPos = { 79.9797974f, -4.33551598f + 10.0f, -27.7822781f };
		tObj.vScale = _vec3{ 10.f, 10.f, 10.f };
		pGameObject = CSaveSpace::Create(m_pGraphicDev, _vec3(79.9797974f, -0.f, -27.7822781f), L"Yellow_Bg", g_bRenderBox, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SaveSpace9", pGameObject, this), E_FAIL);
	}



	m_mapLayer.emplace(pLayerTag, pLayer);

	return S_OK;
}

HRESULT CVacuumField::Ready_LoadMap_Layer(const _tchar * pLayerTag, const _tchar * pFilePath)
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

HRESULT CVacuumField::Ready_LightInfo(void)
{
	D3DLIGHT9				tLightInfo;
	ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));

	tLightInfo.Type = D3DLIGHT_POINT;
	tLightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Ambient = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.f);


	tLightInfo.Diffuse = D3DXCOLOR(1.1f * 1.4f, 0.9f * 1.4f, 0.8f * 1.4f, 1.f);
	tLightInfo.Position = _vec3(38.56f, 8.54f, 2.5f);
	tLightInfo.Range = 50.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 0);

	tLightInfo.Diffuse = D3DXCOLOR(0.6f * 2.5f, 0.4f * 2.5f, 0.2f * 2.5f, 1.f);
	tLightInfo.Position = _vec3(53.06f, 30.13f, 28.23f);
	tLightInfo.Range = 30.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 1);

	tLightInfo.Diffuse = D3DXCOLOR(1.f * 1.5f, 0.8f * 1.5f, 0.6f * 1.5f, 1.f);
	tLightInfo.Position = _vec3(-17.f, 34.48f, 35.18f);
	tLightInfo.Range = 30.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 2);

	tLightInfo.Diffuse = D3DXCOLOR(1.5f, 0.2f, 0.1f, 1.f);
	tLightInfo.Position = _vec3(-46.29f, 33.81f, 39.95f);
	tLightInfo.Range = 50.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 3);

	tLightInfo.Diffuse = D3DXCOLOR(1.f * 1.5f, 1.f * 1.5f, 1.1f * 1.5f, 1.f);
	tLightInfo.Position = _vec3(-34.35f, 30.30f, 114.02f);
	tLightInfo.Range = 40.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 4);

	tLightInfo.Diffuse = D3DXCOLOR(0.9f * 2.5f, 0.6f * 2.5f, 0.4f * 2.5f, 1.f);
	tLightInfo.Position = _vec3(-67.82f, 23.61f, 116.88f);
	tLightInfo.Range = 25.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 5);

	tLightInfo.Diffuse = D3DXCOLOR(1.f, 0.8f, 0.2f, 1.f);
	tLightInfo.Position = _vec3(-74.07f, 19.07f, 90.11f);
	tLightInfo.Range = 15.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 6);

	tLightInfo.Diffuse = D3DXCOLOR(1.1f, 1.2f, 1.5f, 0.5f);
	tLightInfo.Position = _vec3(-81.43f, 14.21f, -19.79f);
	tLightInfo.Range = 50.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 7);

	tLightInfo.Diffuse = D3DXCOLOR(1.f, 1.1f, 1.3f, 0.5f);
	tLightInfo.Position = _vec3(-19.67f, 63.05f, -19.79f);
	tLightInfo.Range = 70.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 8);

	tLightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Position = _vec3(80.29f, 6.14f, -27.01f);
	tLightInfo.Range = 20.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 9);

	tLightInfo.Diffuse = D3DXCOLOR(1.f, 1.2f, 1.2f, 1.f);
	tLightInfo.Position = _vec3(-93.03f, 26.54f, 122.73f);
	tLightInfo.Range = 20.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 10);

	tLightInfo.Diffuse = D3DXCOLOR(1.f * 2.f, 0.8f * 2.f, 0.6f * 2.f, 1.f);
	tLightInfo.Position = _vec3(-73.07f, 17.07f, 75.11f);
	tLightInfo.Range = 30.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 11);

	tLightInfo.Diffuse = D3DXCOLOR(1.1f * 1.4f, 0.9f * 1.4f, 0.8f * 1.4f, 1.f);
	tLightInfo.Position = _vec3(-3.24f, 14.01f, 5.91f);
	tLightInfo.Range = 25.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 12);

	tLightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.1f, 1.f);
	tLightInfo.Position = _vec3(-79.46f, 13.18f, 60.52f);
	tLightInfo.Range = 20.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 13);

	//tLightInfo.Type = D3DLIGHT_SPOT;
	//tLightInfo.Diffuse = D3DXCOLOR(3.f, 3.f, 3.f, 1.f);
	//tLightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	//tLightInfo.Ambient = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.f);
	//tLightInfo.Direction = _vec3(0.f, -1.f, 0.f);
	////tLightInfo.Position = _vec3(-67.49f, 15.5f, 86.47f);
	//tLightInfo.Position = _vec3(0.f, 5.f, 0.f);
	//tLightInfo.Range = 30.f;

	//FAILED_CHECK_RETURN(Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 6), E_FAIL);

	return S_OK;
}

CVacuumField * CVacuumField::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CVacuumField*		pInstance = new CVacuumField(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
		Safe_Release(pInstance);

	return pInstance;
}

void CVacuumField::Free(void)
{
	// 삭제
	CCollisionMgr::DestroyInstance();

	for (_uint i = 0; i < m_vecObject.size(); ++i)
		Safe_Delete_Array(m_vecObject[i]);

	m_vecObject.clear();
	m_vecObject.shrink_to_fit();
	Engine::Delete_AllResource(RESOURCE_VF);
	Engine::CScene::Free();
}


HRESULT CVacuumField::Ready_Interact_Layer(const _tchar* szLayer)
{
	//Engine::CLayer*			pLayer = Engine::CLayer::Create();
	//NULL_CHECK_RETURN(pLayer, E_FAIL);

	//OBJINFO tObj;
	//ZeroMemory(&tObj, sizeof(OBJINFO));
	//tObj.eSceneID = RESOURCE_VF;
	//// 오브젝트 추가

	//Engine::CGameObject*		pGameObject = nullptr;
	//tObj.vPos = _vec3(80.86f, -4.43f, 7.86f);

	//pGameObject = CVacuumFieldPart5::Create(m_pGraphicDev, DIRLEFT, &tObj);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"VacuumFieldHead", pGameObject, this), E_FAIL);
	//auto vecBody = pGameObject->Is<CVacuumFieldPart5>()->Get_HozeBody();
	//pGameObject->Is<CVacuumFieldPart5>()->Set_Suck(true);
	//m_pScene->addAggregate(*pGameObject->Is<CVacuumFieldPart5>()->Get_Hoze());
	//for (auto pBody : vecBody)
	//	setupFiltering(Engine::Get_Allocator(), pBody, FilterGroup::eHoze, FilterGroup::eCody | FilterGroup::eMay);

	//m_mapLayer.emplace(szLayer, pLayer);

	return S_OK;
}

HRESULT CVacuumField::Ready_Trigger_Layer(const _tchar* szLayer)
{
	Engine::CLayer*			pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	OBJINFO tObj;
	ZeroMemory(&tObj, sizeof(OBJINFO));
	tObj.eSceneID = RESOURCE_VF;
	// 오브젝트 추가

	Engine::CGameObject*		pGameObject = nullptr;

	pGameObject = CSceneChangeSpace::Create(m_pGraphicDev, SCENE_VF2, { 85.624f, -0.43f, 11.39f }, 2, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SceneChange", pGameObject, this), E_FAIL);

	// 터널
	pGameObject = CCameraSpace::Create(m_pGraphicDev, { -34.86f, 19.31f, 66.02f }, 20, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CameraEvent", pGameObject, this), E_FAIL);

	// 벽앞
	pGameObject = CCameraSpace::Create(m_pGraphicDev, { 54.8f, 16.18f, 33.11f }, 2.6f, 9.6f, 5.85f, &tObj);
	pGameObject->Is<CCameraSpace>()->Set_Fix({ 55.2f, 18.3f, 18.0f }, { 55.2f, 18.3f, 35.0f });
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CameraEvent2", pGameObject, this), E_FAIL);

	// 머신1
	pGameObject = CCameraSpace::Create(m_pGraphicDev, { -81.5f, 15.7f, 87.02f }, 6.3f, 15.0f, 2.7f, &tObj);
	pGameObject->Is<CCameraSpace>()->Set_Fix({ -61.5f, 20.7f, 87.32f }, { -81.5f, 15.7f, 87.32f });
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CameraEvent3", pGameObject, this), E_FAIL);


	// 머신1
	pGameObject = CSoundSpace::Create(m_pGraphicDev, { -86.70f, -5.6F, -31.78f }, 15.f, 15.f, 15.f, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SoundEvent", pGameObject, this), E_FAIL);
	pGameObject->Is<CSoundSpace>()->Set_SoundEnum(CSoundSpace::VF_Concent);
	m_mapLayer.emplace(szLayer, pLayer);

	return S_OK;
}
