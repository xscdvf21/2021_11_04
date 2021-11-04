#include "stdafx.h"
#include "VacuumField2.h"
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

#include "TestWall.h"
#include "MiniVacuum.h"
#include "Map.h"
#include "FuseBox.h"
#include "FuseNPC.h"
#include "PowerSwitch.h"
#include "PowerButton.h"
#include "Fan.h"
#include "SceneChangeSpace.h"
#include "DeadSpace.h"
#include "HubPlatform.h"
#include "StaticHoze.h"
#include "DoubleHoze.h"
#include "VacuumHub.h"
#include "SaveSpace.h"
#include "LineObject.h"


CVacuumField2::CVacuumField2(LPDIRECT3DDEVICE9 pDevice)
	: CScene(pDevice)
{
}

CVacuumField2::~CVacuumField2()
{
}

HRESULT CVacuumField2::Ready_Scene()
{
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

	//pPlane->setGlobalPose(PxTransform(0, 0.f, 0));


	PxShape* pShape = pPhysics->createShape(PxBoxGeometry(30.f, 1.f, 30.f), *pPhysics->createMaterial(0.5f, 0.5f, 0.f), true);
	auto* pBox = PxCreateKinematic(*pPhysics, PxTransform(0.f, 0.f, 80.0f), *pShape, 1);

	//auto* pPlane = PxCreatePlane(*pPhysics, PxPlane(0, 1, 0, 1), *pPhysics->createMaterial(0.5f, 0.5f, 0.f));

	pBox->setName((char*)pBox);
	setupFiltering(Engine::Get_Allocator(), pBox, FilterGroup::eGround, FilterGroup::eCody | FilterGroup::eMay);
	m_pScene->addActor(*pBox);
	m_pScene->setGravity(PxVec3(0.0f, -20.f, 0.0f));
	FAILED_CHECK_RETURN(Ready_Environment_Layer(L"Environment"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_GameObject_Layer(L"GameObject"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Interact_Layer(L"Interact"), E_FAIL);

	//FAILED_CHECK_RETURN(Ready_LoadMap_Layer(L"LoadObject", L"../../Data/ITT_Test/Test.dat"), E_FAIL);

	g_vBackBufferColor = { 175.f, 175.f, 200.f };
	Ready_LightInfo();

	StopAll();
	PlayBGM(L"VacummField_BGM.wav", 1.f);

	return S_OK;
}

_int CVacuumField2::Update_Scene(const _float & fTimeDelta)
{
	if (m_bStart)
	{
		CStaticHoze* pHoze = Engine::Get_GameObject(L"Interact", L"StaticHoze2")->Is<CStaticHoze>();

		vector<vector<_vec3>*>* vecPath;
		vecPath = ((vector<vector<_vec3>*>*)(pHoze->On_Trigger_Active(nullptr,this)));
		if (vecPath)
		{

			Engine::Get_GameObject(L"GameObject", L"Cody")->Is<CCody>()->Set_Slide(false, pHoze->Get_Power(), pHoze->Get_Duration(), *vecPath, 2);

			Engine::Get_GameObject(L"GameObject", L"May")->Is<CMay>()->Set_Slide(false, pHoze->Get_Power(), pHoze->Get_Duration(), *vecPath, 2);
			m_bStart = false;
		}
	}

	m_pScene->simulate(fTimeDelta);

	m_pScene->fetchResults(true);
	_int iExit = Engine::CScene::Update_Scene(fTimeDelta);



	if (!m_bEnd&&m_pButton&&m_pButton->Is_Active())
	{
		m_bEnd = true;
	}
	if (Engine::Key_Down(DIK_Y))
	{

		CHANGE_SCENE(m_pGraphicDev, SCENE_VF3, OBJ_NOEVENT);

		return 0;
	}
	return iExit;
}

_int CVacuumField2::LateUpdate_Scene(const _float & fTimeDelta)
{
	_int iExit = Engine::CScene::LateUpdate_Scene(fTimeDelta);

	return iExit;
}

void CVacuumField2::Render_Scene()
{
	CCollisionMgr::GetInstance()->Collision_Player_Monster();
	CCollisionMgr::GetInstance()->Clear_List();
}

void CVacuumField2::onConstraintBreak(PxConstraintInfo* constraints, PxU32 count)
{

	return;
}

void CVacuumField2::onWake(PxActor** actors, PxU32 count)
{
	OutputDebugString(L"깼음");
	return;
}

void CVacuumField2::onSleep(PxActor** actors, PxU32 count)
{
	OutputDebugString(L"잠듬");

	return;
}

void CVacuumField2::onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs)
{
	OutputDebugString(L"충돌있음\n");
	for (PxU32 i = 0; i < nbPairs; ++i)
	{

		if ((!strcmp(pairHeader.actors[0]->getName(), "Cody") && !strcmp(pairHeader.actors[1]->getName(), "Wall")) ||
			(!strcmp(pairHeader.actors[1]->getName(), "Cody") && !strcmp(pairHeader.actors[0]->getName(), "Wall")))
		{
			Engine::Get_GameObject(L"GameObject", L"Cody")->Is<CCody>()->Set_Climbing(true);
		}

	}
}

void CVacuumField2::onTrigger(PxTriggerPair* pairs, PxU32 count)
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
			case TRIGGER_VACUUM_RIDE:
				Event_Vacuum_Ride(&Trigger);
				break;
			case TRIGGER_SAVESPACE:
				Event_Save(&Trigger);
				break;
			case TRIGGER_VACUUM_ENTER:
				Event_Vacuum_Enter(&Trigger);
				break;
			case TRIGGER_BOUNCE_SWITCH:
				Event_Bounce_Switch(&Trigger);
				break;
			case TRIGGER_FLOAT:
				Event_Float(&Trigger);
				break;
			case TRIGGER_WING:
				Event_Wing(&Trigger);
				break;
			case TRIGGER_DEADSPACE:
				Event_Dead(&Trigger);
				break;
			case TRIGGER_SCENE_CHANGE:
				Event_Scene_Change(&Trigger);
				break;
			default:
				break;
			}


		}
	}
}

void CVacuumField2::onAdvance(const PxRigidBody*const* bodyBuffer, const PxTransform* poseBuffer, const PxU32 count)
{
	OutputDebugString(L"업데이트");
}

void CVacuumField2::Event_Scene_Change(PxTriggerPair* pairs)
{
	auto* pPSw = Engine::Get_GameObject(L"GameObject", L"PowerSwitch")->Is<CPowerSwitch>();

	ToTrigger(pairs->triggerActor->getName())->Activate((void*)pairs->otherShape->getName());
}
void CVacuumField2::Event_Vacuum_Enter(PxTriggerPair* pairs)
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

void CVacuumField2::Event_Bounce_Switch(PxTriggerPair* pairs)
{

}


void CVacuumField2::Event_Vacuum_Ride(PxTriggerPair* pairs)
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

void CVacuumField2::Event_Float(PxTriggerPair* pairs)
{
	CFan* pFan = ToTrigger(pairs->triggerShape->getActor()->getName())->Is<CFan>();
	_float3 vPos = ToTrigger(pairs->triggerShape->getActor()->getName())->Get_Pos();
	NULL_CHECK_RETURN(pFan, );

	if (pairs->otherShape->getSimulationFilterData().word0 == FilterGroup::eCody)
	{
		CCody* pCody = ToObj<CCody>(pairs->otherShape->getName());
		NULL_CHECK_RETURN(pCody, );

		if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
			pCody->Set_Float(true, pFan->Get_Up(), pFan, vPos, 19.0f);

		else if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_LOST)
			pCody->Set_Float(false, pFan->Get_Up(), nullptr, vPos);
	}
	else if (pairs->otherShape->getSimulationFilterData().word0 == FilterGroup::eMay)
	{
		CMay* pMay = (CMay*)Engine::Get_GameObject(L"GameObject", L"May");
		NULL_CHECK_RETURN(pMay, );

		if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
			pMay->Set_Float(true, pFan->Get_Up(), pFan, vPos, 19.0f);

		else if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_LOST)
			pMay->Set_Float(false, pFan->Get_Up(), nullptr, vPos);
	}
}

void CVacuumField2::Event_Wing(PxTriggerPair* pairs)
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

void CVacuumField2::Event_Save(PxTriggerPair * pairs)
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

void CVacuumField2::Event_Dead(PxTriggerPair * pairs)
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

HRESULT CVacuumField2::Ready_Environment_Layer(const _tchar * pLayerTag)
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
//#스테이지	청소기 필드2
HRESULT CVacuumField2::Ready_GameObject_Layer(const _tchar * pLayerTag)
{
	Engine::CLayer*			pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	OBJINFO tObj;
	ZeroMemory(&tObj, sizeof(OBJINFO));
	tObj.eSceneID = RESOURCE_VF2;
	// 오브젝트 추가
	Engine::CGameObject*		pGameObject = nullptr;
	tObj.vScale = { 0.01f, 0.01f ,0.01f };

	// CodyCamera
	pGameObject = CCodyCamera::Create(m_pGraphicDev, &_vec3(0.f, 10.f, -5.f), &_vec3(0.f, 0.f, 1.f), &_vec3(0.f, 1.f, 0.f), D3DXToRadian(60.f), _float(WINCX) / WINCY, 0.1f, 1000.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CodyCamera", pGameObject, this), E_FAIL);
	_vec3 vDir = _vec3(0.f, -2.f, -2.f);
	D3DXVec3Normalize(&vDir, &vDir);
	pGameObject->Is<CCodyCamera>()->Set_Dir(vDir);

	// MayCamera
	pGameObject = CMayCamera::Create(m_pGraphicDev, &_vec3(0.f, 10.f, -5.f), &_vec3(0.f, 0.f, 1.f), &_vec3(0.f, 1.f, 0.f), D3DXToRadian(60.f), _float(WINCX) / WINCY, 0.1f, 1000.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MayCamera", pGameObject, this), E_FAIL);
	pGameObject->Is<CMayCamera>()->Set_Dir(vDir);

	tObj.vPos = { 0.f,5.f,0.f };
	//780.886, 10794.9, 2694.33
	tObj.vPos = { 97.97f,22.59f,37.8f };//씬마지막
	tObj.vPos = { 7.8f,26.9f,107.19f };//씬처음

	tObj.vAngle = { 0.f,D3DXToRadian(180.f),0.f };
	pGameObject = CCody::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Cody", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eCody, FilterGroup::eGround | FilterGroup::eWall);
	tObj.vPos = { 0.f,5.f,0.f };
	tObj.vPos = { 7.8f,26.9f,107.19f };//씬처음


	pGameObject = CMay::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"May", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMay, FilterGroup::eGround | FilterGroup::eWall);

	tObj.vAngle = { 0.f,0.f,0.f };





	tObj.vAngle = { 0.f, 180.f, 0.f };
	tObj.vScale = _vec3{ 0.01f, 0.01f, 0.01f };
	tObj.vPos = { 70.3f,-4.f,-73.9f };


	//테스트용
	//pGameObject = CHubPlatform::Create(m_pGraphicDev, _vec3(0.f,-6.f,5.f), _vec3(0.f, -6.f, 10.f), &tObj);
	pGameObject = CHubPlatform::Create(m_pGraphicDev, _vec3(70.3f, -4.f, -73.9f), _vec3(70.7f, -4.f, -50.4f), &tObj);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"HubPlatform", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eRideObj, FilterGroup::eCody | FilterGroup::eMay);


	tObj.vScale = _vec3{ 0.2f, 0.1f, 0.2f };
	tObj.vAngle = { 0.f, 0.f, 0.f };
	tObj.vPos = { 0.f, -8.f, 0.f };

	tObj.vTriggerPos = { 0.f, -3.0f, 0.f };
	tObj.vTriggerScale = { 300.0f, 2.0f, 300.0f };

	pGameObject = CDeadSpace::Create(m_pGraphicDev, L"Water", true, &tObj);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DeadWater", pGameObject, this), E_FAIL);

	tObj.vScale = _vec3{ 0.01f, 0.01f, 0.01f };
	tObj.vAngle = { 0, 0.f, D3DXToRadian(90.f) };
	tObj.vPos = { 55.81f, 9.f, -36.53f };

	pGameObject = CMiniVacuumHub::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"VacuumHub", pGameObject, this), E_FAIL);

	tObj.vAngle = { 0, 0.f, 0.f };
	tObj.vPos = { 92.88f, 7.5f, 3.f };

	pGameObject = CMiniVacuumHub::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"VacuumHub2", pGameObject, this), E_FAIL);

	tObj.vAngle = { 0, 0.f, 0.f };
	tObj.vPos = { -3.86f, 19.8f, 58.18f };

	pGameObject = CVacuumHub::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"VacuumHub3", pGameObject, this), E_FAIL);


	lstrcpy(tObj.tagMesh, L"VacuumField");
	tObj.vScale = _vec3{ 0.01f,0.01f,0.01f };
	tObj.vAngle = { 0, 0.f, 0.f };

	tObj.eMeshType = 1;
	tObj.eRenderType = RENDER_NONALPHA;
	tObj.vPos = { 0.f,0.f,0.f };
	pGameObject = CMap::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Field", pGameObject, this), E_FAIL);


	{
		// 시작
		tObj.vPos = { -3.52508f, 28.6088f, 53.7763f };
		tObj.vScale = _vec3{ 10.0f, 10.0f, 1.0f };
		pGameObject = CSaveSpace::Create(m_pGraphicDev, _vec3(-9.38592f, 1.6429f, 28.1382f), L"Yellow_Bg", g_bRenderBox, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SaveSpace1", pGameObject, this), E_FAIL);

		// 버튼 앞
		tObj.vPos = { 25.253f, 12.52f, -4.725f };
		tObj.vScale = _vec3{ 1.0f, 5.0f, 5.0f };
		pGameObject = CSaveSpace::Create(m_pGraphicDev, _vec3(25.253f, 12.52f, -4.725f), L"Yellow_Bg", g_bRenderBox, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SaveSpace2", pGameObject, this), E_FAIL);

		// 스테틱호즈 건너편
		tObj.vPos = { 47.5329f, 1.1617f, -52.7675f };
		tObj.vScale = _vec3{ 1.0f, 5.0f, 5.0f };
		pGameObject = CSaveSpace::Create(m_pGraphicDev, _vec3(47.5329f, 1.1617f, -52.7675f), L"Yellow_Bg", g_bRenderBox, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SaveSpace3", pGameObject, this), E_FAIL);

		// 허브플렛폼 앞
		tObj.vPos = { 67.15f, 3.9447f, -89.63339f };
		tObj.vScale = _vec3{ 1.0f, 7.0f, 7.0f };
		pGameObject = CSaveSpace::Create(m_pGraphicDev, _vec3(67.15f, 3.9447f, -89.63339f), L"Yellow_Bg", g_bRenderBox, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SaveSpace4", pGameObject, this), E_FAIL);

		// 허브플렛폼 건녀편
		tObj.vPos = { 71.77f, 7.5533f, -38.5923f };
		tObj.vScale = _vec3{ 10.0f, 5.0f, 1.0f };
		pGameObject = CSaveSpace::Create(m_pGraphicDev, _vec3(71.77f, 7.5533f, -38.5923f), L"Yellow_Bg", g_bRenderBox, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SaveSpace5", pGameObject
			, this), E_FAIL);

		// 더블 호스 시작
		tObj.vPos = { 73.37f, 7.0f, -10.554f };
		tObj.vScale = _vec3{ 7.0f, 5.0f, 1.0f };
		pGameObject = CSaveSpace::Create(m_pGraphicDev, _vec3(73.37f, 7.0f, -10.554f), L"Yellow_Bg", g_bRenderBox, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SaveSpace6", pGameObject, this), E_FAIL);
	}

	m_mapLayer.emplace(pLayerTag, pLayer);

	return S_OK;
}

HRESULT CVacuumField2::Ready_LoadMap_Layer(const _tchar * pLayerTag, const _tchar * pFilePath)
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

HRESULT CVacuumField2::Ready_LightInfo(void)
{
	D3DLIGHT9				tLightInfo;
	ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));

	tLightInfo.Type = D3DLIGHT_POINT;
	tLightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Ambient = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.f);

	tLightInfo.Diffuse = D3DXCOLOR(1.f, 0.9f, 0.8f, 1.f);
	tLightInfo.Position = _vec3(1.83f, 42.87f, 85.07f);
	tLightInfo.Range = 40.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 0);

	tLightInfo.Diffuse = D3DXCOLOR(1.2f, 1.4f, 1.6f, 1.f);
	tLightInfo.Position = _vec3(-5.27f, 31.95f, 56.89f);
	tLightInfo.Range = 20.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 1);

	tLightInfo.Diffuse = D3DXCOLOR(2.2f * 1.3f, 1.9f * 1.3f, 1.6f * 1.3f, 1.f);
	tLightInfo.Position = _vec3(-10.95f, 17.34f, 8.06f);
	tLightInfo.Range = 30.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 2);

	tLightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.2f, 1.f);
	tLightInfo.Position = _vec3(12.89f, 2.85f, 1.9f);
	tLightInfo.Range = 7.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 3);

	tLightInfo.Diffuse = D3DXCOLOR(1.5f, 1.4f, 1.2f, 1.f);
	tLightInfo.Position = _vec3(25.57f, 23.03f, -8.89f);
	tLightInfo.Range = 33.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 4);

	tLightInfo.Diffuse = D3DXCOLOR(1.4f, 1.5f, 2.1f, 1.f);
	tLightInfo.Position = _vec3(38.09f, 13.49f, -91.44f);
	tLightInfo.Range = 60.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 5);

	tLightInfo.Diffuse = D3DXCOLOR(0.5f, 0.6f, 0.8f, 0.5f);
	tLightInfo.Position = _vec3(71.36f, 14.83f, -63.20f);
	tLightInfo.Range = 20.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 6);

	tLightInfo.Diffuse = D3DXCOLOR(1.5f, 0.2f, 0.3f, 1.f);
	tLightInfo.Position = _vec3(71.36f, 22.79f, -29.32f);
	tLightInfo.Range = 30.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 7);

	tLightInfo.Diffuse = D3DXCOLOR(1.7f, 1.5f, 1.1f, 1.f);
	tLightInfo.Position = _vec3(88.62f, 31.15f, 2.80f);
	tLightInfo.Range = 45.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 8);

	tLightInfo.Diffuse = D3DXCOLOR(0.7f, 0.7f, 0.8f, 1.f);
	tLightInfo.Position = _vec3(88.62f, 20.15f, 2.80f);
	tLightInfo.Range = 20.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 9);

	tLightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Position = _vec3(96.40f, 28.26f, 34.49f);
	tLightInfo.Range = 20.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 10);

	return S_OK;
}

CVacuumField2 * CVacuumField2::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CVacuumField2*		pInstance = new CVacuumField2(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
		Safe_Release(pInstance);

	return pInstance;
}

void CVacuumField2::Free(void)
{
	// 삭제
	CCollisionMgr::DestroyInstance();

	for (_uint i = 0; i < m_vecObject.size(); ++i)
		Safe_Delete_Array(m_vecObject[i]);

	m_vecObject.clear();
	m_vecObject.shrink_to_fit();
	Engine::Delete_AllResource(RESOURCE_VF2);
	Engine::CScene::Free();
}


HRESULT CVacuumField2::Ready_Interact_Layer(const _tchar* szLayer)
{
	Engine::CLayer*			pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	OBJINFO tObj;
	ZeroMemory(&tObj, sizeof(OBJINFO));
	tObj.eSceneID = RESOURCE_VF2;
	// 오브젝트 추가
	tObj.vPos = { -15.08f,1.08f,-1.41f };
	tObj.vScale = { 0.01f,0.01f ,0.01f };
	Engine::CGameObject*		pGameObject = nullptr;
	Engine::CGameObject*		pButton = nullptr;
	Engine::CGameObject*		pLine = nullptr;
	pGameObject = CMiniVacuum::Create(m_pGraphicDev, DIRRIGHT, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Minivacuum", pGameObject, this), E_FAIL);
	auto vecBody = pGameObject->Is<CMiniVacuum>()->Get_HozeBody();

	m_pScene->addAggregate(*pGameObject->Is<CMiniVacuum>()->Get_Hoze());
	for (auto pBody : vecBody)
		setupFiltering(Engine::Get_Allocator(), pBody, FilterGroup::eHoze, FilterGroup::eCody | FilterGroup::eMay);

	tObj.vPos = { 75.02f,21.72f,16.23f };
	pButton = CPowerButton::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pButton, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"PowerButton1", pButton, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pButton->Get_RigidActor(), FilterGroup::eButton, FilterGroup::eCody | FilterGroup::eMay);



	pLine = CLineObject::Create(m_pGraphicDev, &tObj,L"../../Data/VF2LastLine.dat",false);
	NULL_CHECK_RETURN(pLine, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"LastLine", pLine, this), E_FAIL);
	pButton->Set_InteractObj(pLine);

	pGameObject = CSceneChangeSpace::Create(m_pGraphicDev, SCENE_VF3, { 78.82f,-0.83f,25.29f }, 2, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SceneChange", pGameObject, this), E_FAIL);
	pLine->Set_InteractObj(pGameObject);


	tObj.vPos = { 28.08f,12.81f,-1.49f };
	pButton = CPowerButton::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pButton, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"PowerButton", pButton, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pButton->Get_RigidActor(), FilterGroup::eButton, FilterGroup::eCody | FilterGroup::eMay);


	tObj.vPos = { 11.83f,-0.83f,2.68f };
	tObj.vScale = _vec3{ 2.f, 20.0f, 2.0f };

	tObj.eMeshType = 0;
	CGameObject* pFan = CFan::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pFan, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Fan", pFan, this), E_FAIL);
	pButton->Set_InteractObj(pFan);

	tObj.vScale = { 0.01f, 0.01f, 0.01f };
	tObj.vPos = { 94.89f,7.13f,4.63f };
	tObj.vAngle = { -12.f ,-60.f,0.f };
	tObj.vScale = _vec3{ 0.01f, 0.01f, 0.01f };

	vector<_wstr> vecPath;
	vecPath.emplace_back(L"../../Data/Line1.dat");
	vecPath.emplace_back(L"../../Data/Line2.dat");
	pGameObject = CStaticHoze::Create(m_pGraphicDev, L"StaticHoze1", vecPath, 2, false, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"StaticHoze", pGameObject, this), E_FAIL);
	pButton->Set_InteractObj(pGameObject);
	CTrigger* pTrigger = CTrigger::CreateBoxTrigger(m_pScene, pGameObject, { 23.2f,12.82f,-9.61f }, 3.f, 1.f, 1.f, TRIGGERTYPE::TRIGGER_VACUUM_ENTER);
	pTrigger->Set_FloatValue(15.f);
	pTrigger->Set_FloatValue(4);
	pTrigger->Set_FloatValue(2);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	vecPath.clear();
	vecPath.emplace_back(L"../../Data/Line42.dat");
	pGameObject = CStaticHoze::Create(m_pGraphicDev, L"StaticHoze3", vecPath, 0, false, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"StaticHoze2", pGameObject, this), E_FAIL);
	pGameObject->Is<CStaticHoze>()->Set_Active();
	//#VF2 시작호스 슛파워
	pGameObject->Is<CStaticHoze>()->Set_Power(35.f);
	pTrigger = CTrigger::CreateBoxTrigger(m_pScene, pGameObject, { 0.f,20.f,-10.f }, 3.f, 1.f, 1.f, TRIGGERTYPE::TRIGGER_VACUUM_ENTER);
	pTrigger->Set_FloatValue(20.f);
	pTrigger->Set_FloatValue(4);
	pTrigger->Set_FloatValue(2);

	tObj.vAngle = { 0.f ,0.f,0.f };

	tObj.vPos = { 99.97f,22.59f,37.86f };
	pButton = CPowerButton::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pButton, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"PowerButton2", pButton, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pButton->Get_RigidActor(), FilterGroup::eButton, FilterGroup::eCody | FilterGroup::eMay);

	pLine = CLineObject::Create(m_pGraphicDev, &tObj, L"../../Data/VF2DoubleLine.dat",true);
	NULL_CHECK_RETURN(pLine, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DoubleHozeLine", pLine, this), E_FAIL);
	pButton->Set_InteractObj(pLine);

	vecPath.clear();
	vecPath.emplace_back(L"../../Data/Line3.dat");
	pGameObject = CDoubleHoze::Create(m_pGraphicDev, vecPath, true, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DoubleHoze", pGameObject, this), E_FAIL);
	pLine->Set_InteractObj(pGameObject);


	m_mapLayer.emplace(szLayer, pLayer);

	return S_OK;
}
