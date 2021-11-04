#include "stdafx.h"
#include "TestField.h"
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
#include "Ogre.h"
#include "ChessTile.h"
#include "CollisionMgr.h"

#include "Orb.h"
#include "TestWall.h"
#include "MiniVacuum.h"
#include "Map.h"

#include "FuseNPC.h"
#include "DeadSpace.h"
#include "Lever.h"
#include "Float.h"
#include "Fan.h"


CTestField::CTestField(LPDIRECT3DDEVICE9 pDevice)
	: CScene(pDevice)
{
}

CTestField::~CTestField()
{
}

HRESULT CTestField::Ready_Scene()
{
	Engine::Start_Split();
	g_bMenu = false;
	g_bSlideFinal = false;

	auto* pPhysics = Engine::Get_Physics();
	auto* pDispatcher = Engine::Get_Dispatcher();
	auto* pCudaMgr = Engine::Get_CudaMgr();
	CScene::Ready_PhysX(this, pPhysics, pDispatcher, pCudaMgr);

	Engine::Start_Split();
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

	if (!Engine::Is_LightExsist())
		Ready_LightInfo();

	return S_OK;
}

_int CTestField::Update_Scene(const _float & fTimeDelta)
{
	m_pScene->simulate(fTimeDelta);

	m_pScene->fetchResults(true);
	_int iExit = Engine::CScene::Update_Scene(fTimeDelta);
	/*if (Engine::Key_Down(DIK_Y))
	{
		Engine::CScene*		pScene = nullptr;

		pScene = CLogo::Create(m_pGraphicDev, SCENE_VB);
		NULL_CHECK_RETURN(pScene, 0);
		FAILED_CHECK_RETURN(Set_Scene(pScene, m_pGraphicDev), 0);
		return 0;
	}*/
	return iExit;
}

_int CTestField::LateUpdate_Scene(const _float & fTimeDelta)
{
	_int iExit = Engine::CScene::LateUpdate_Scene(fTimeDelta);

	return iExit;
}

void CTestField::Render_Scene()
{
	CCollisionMgr::GetInstance()->Collision_Player_Monster();
	CCollisionMgr::GetInstance()->Clear_List();
}

void CTestField::onConstraintBreak(PxConstraintInfo* constraints, PxU32 count)
{

	return;
}

void CTestField::onWake(PxActor** actors, PxU32 count)
{
	OutputDebugString(L"깼음");
	return;
}

void CTestField::onSleep(PxActor** actors, PxU32 count)
{
	OutputDebugString(L"잠듬");

	return;
}

void CTestField::onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs)
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

void CTestField::onTrigger(PxTriggerPair* pairs, PxU32 count)
{
	for (PxU32 i = 0; i < count; ++i)
	{
		auto Trigger = pairs[i];
		if (Trigger.triggerShape->getSimulationFilterData().word0 == FilterGroup::eWall)
		{
			if (Trigger.otherShape->getSimulationFilterData().word0 == FilterGroup::eCody)
			{
				/*			CCody* pCody = (CCody*)Engine::Get_GameObject(L"GameObject", L"Cody");
							if (pCody->Get_PlayerState() == CCody::PL_CLIMBING)
								pCody->Set_Hang();*/
			}
			else if (Trigger.otherShape->getSimulationFilterData().word0 == FilterGroup::eMay)
			{
				CMay* pMay = (CMay*)Engine::Get_GameObject(L"GameObject", L"May");
				if (pMay->Get_PlayerState() == CCody::PL_CLIMBING)
					pMay->Set_Hang();
			}
		}
		else if (Trigger.otherShape->getSimulationFilterData().word0 &(FilterGroup::eCody | FilterGroup::eMay))
		{
			auto* pTrigger = ToTrigger(Trigger.triggerShape->getName());
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
				//Event_Insert_Fuse(&Trigger);
				break;
			case TRIGGER_VACUUM_ENTER:
				Event_Vacuum_Enter(&Trigger);
				break;
			case TRIGGER_DEADSPACE:
				Event_Dead(&Trigger);
				break;
			case TRIGGER_FLOAT:
				Event_Float(&Trigger);
				break;
			case TRIGGER_WING:
				Event_Wing(&Trigger);
				break;
			default:
				break;
			}
		}
	}
	OutputDebugString(L"트리거 발동\n");


	//if (Trigger.otherShape->getSimulationFilterData().word0 == FilterGroup::eCody)
	//{
	//	Engine::Get_GameObject(L"GameObject", L"Cody")->Is<CCody>()->Set_Slide(false, false, static_cast<CMiniVacuum*>((void*)Trigger.triggerActor->getName())->Get_IOPath());
	//}
	//else if (Trigger.otherShape->getSimulationFilterData().word0 == FilterGroup::eMay)
	//{

	//}
}


void CTestField::onAdvance(const PxRigidBody*const* bodyBuffer, const PxTransform* poseBuffer, const PxU32 count)
{
	OutputDebugString(L"업데이트");
}

void CTestField::Event_Ride(PxTriggerPair * pairs)
{
	CMiniVacuum* pMiniVaccum = ToTrigger(pairs->triggerShape->getActor()->getName())->Is<CMiniVacuum>();
	NULL_CHECK_RETURN(pMiniVaccum, );

	if (pairs->otherShape->getSimulationFilterData().word0 == FilterGroup::eCody)
	{
		CCody* pCody = ToObj<CCody>(pairs->otherShape->getName());

		NULL_CHECK_RETURN(pCody, );
		pMiniVaccum->Get_HoseHead();

		if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
			pCody->Set_PickUp(true, CCody::HOSE, pMiniVaccum);
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
			pMay->Set_PickUp(true, CMay::HOSE, pMiniVaccum);

		else if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_LOST)
			pMay->Set_PickUp(false, CMay::HOSE, nullptr);
	}
}

void CTestField::Event_Fuse(PxTriggerPair * pairs)
{
	CFuseNPC* pFuse = ToTrigger(pairs->triggerShape->getActor()->getName())->Is<CFuseNPC>();
	NULL_CHECK_RETURN(pFuse, );

	if (pairs->otherShape->getSimulationFilterData().word0 == FilterGroup::eCody)
	{
		CCody* pCody = ToObj<CCody>(pairs->otherShape->getName());
		NULL_CHECK_RETURN(pCody, );

		if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
			pCody->Set_PickUp(true, CCody::FUSE, pFuse);

		else if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_LOST)
			pCody->Set_PickUp(false, CCody::FUSE, nullptr);
	}
	else if (pairs->otherShape->getSimulationFilterData().word0 == FilterGroup::eMay)
	{
		CMay* pMay = (CMay*)Engine::Get_GameObject(L"GameObject", L"May");
		NULL_CHECK_RETURN(pMay, );

		if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
			pMay->Set_PickUp(true, CMay::FUSE, pFuse);

		else if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_LOST)
			pMay->Set_PickUp(false, CMay::FUSE, nullptr);
	}
}

void CTestField::Event_Dead(PxTriggerPair * pairs)
{
	if (pairs->otherShape->getSimulationFilterData().word0 == FilterGroup::eCody)
	{
		CCody* pCody = ToObj<CCody>(pairs->otherShape->getName());
		NULL_CHECK_RETURN(pCody, );

		pCody->Set_PlayerDead();
	}
	else if (pairs->otherShape->getSimulationFilterData().word0 == FilterGroup::eMay)
	{
		CMay* pMay = ToObj<CMay>(pairs->otherShape->getName());
		NULL_CHECK_RETURN(pMay, );
	}
}

void CTestField::Event_Lever(PxTriggerPair * pairs)
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

void CTestField::Event_Vacuum_Enter(PxTriggerPair * pairs)
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

void CTestField::Event_Float(PxTriggerPair * pairs)
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

		//if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
		//	pMay->Set_Float(true, pFloat->Get_Up());

		//else if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_LOST)
		//	pMay->Set_Float(false, pFloat->Get_Up());
	}
}

void CTestField::Event_Wing(PxTriggerPair * pairs)
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
	}
}

HRESULT CTestField::Ready_Environment_Layer(const _tchar * pLayerTag)
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
//#스테이지 테스트필드
HRESULT CTestField::Ready_GameObject_Layer(const _tchar * pLayerTag)
{
	Engine::CLayer*			pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	OBJINFO tObj;
	ZeroMemory(&tObj, sizeof(OBJINFO));
	tObj.eSceneID = RESOURCE_TF;
	// 오브젝트 추가
	Engine::CGameObject*		pGameObject = nullptr;

	//// MainCamera
	//pGameObject = CMainCamera::Create(m_pGraphicDev, &_vec3(0.f, 10.f, -5.f), &_vec3(0.f, 0.f, 1.f), &_vec3(0.f, 1.f, 0.f), D3DXToRadian(60.f), _float(WINCX) / WINCY, 0.1f, 1000.f);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MainCamera", pGameObject, this), E_FAIL);

	// CodyCamera
	pGameObject = CCodyCamera::Create(m_pGraphicDev, &_vec3(0.f, 10.f, -5.f), &_vec3(0.f, 0.f, 1.f), &_vec3(0.f, 1.f, 0.f), D3DXToRadian(60.f), _float(WINCX) / WINCY, 0.1f, 1000.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CodyCamera", pGameObject, this), E_FAIL);

	// MayCamera
	pGameObject = CMayCamera::Create(m_pGraphicDev, &_vec3(0.f, 10.f, -5.f), &_vec3(0.f, 0.f, 1.f), &_vec3(0.f, 1.f, 0.f), D3DXToRadian(60.f), _float(WINCX) / WINCY, 0.1f, 1000.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MayCamera", pGameObject, this), E_FAIL);

	tObj.vPos = { 22.0f ,0.0f, 3.0f }; // 터널 앞

	pGameObject = CCody::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Cody", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eCody, FilterGroup::eGround | FilterGroup::eWall);

	pGameObject = CMay::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"May", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMay, FilterGroup::eGround | FilterGroup::eWall);

	/*pGameObject = CMiniVacuumHub::Create(m_pGraphicDev, 20);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MiniVacuumHub", pGameObject), E_FAIL);*/

	//pGameObject = CMiniVacuumHead::Create(m_pGraphicDev, &Objinfo);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MiniVacuumHead", pGameObject), E_FAIL);


	//lstrcpy(tObj.tagMesh, L"VacuumField");
	//tObj.vScale = _vec3{ 0.007f,0.007f,0.007f };
	//tObj.eMeshType = 1;
	//tObj.eRenderType = RENDER_NONALPHA;

	///*tObj.vPos = { 10,0,10 };
	//pGameObject = CTestWall::Create(m_pGraphicDev, &tObj);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"TestWall1", pGameObject, this), E_FAIL);
	//setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eWall, FilterGroup::eCody | FilterGroup::eMay);

	//tObj.vPos = { 10, 0, 20 };
	//pGameObject = CTestWall::Create(m_pGraphicDev, &tObj);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"TestWall2", pGameObject, this), E_FAIL);
	//setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eWall, FilterGroup::eCody | FilterGroup::eMay);*/

	//tObj.vPos = { 20, 0, 30 };
	//tObj.vScale = _vec3{ 10.0f,1.0f,10.0f };
	//pGameObject = CDeadSpace::Create(m_pGraphicDev,L"",false, &tObj);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DeadSpace1", pGameObject, this), E_FAIL);

	//// 레버 종합세트
	//{
	//	// 팬바람의 방향
	//	// 0이면 아래로 1이면 위로
	//	tObj.vPos = { 0, 0, 15 };
	//	tObj.vScale = _vec3{ 2.f, 10.0f, 2.0f };
	//	tObj.eMeshType = 1;
	//	pGameObject = CFan::Create(m_pGraphicDev, &tObj);
	//	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Float1", pGameObject, this), E_FAIL);
	//	
	//	tObj.vPos = { 0, 0, 25 };
	//	tObj.vScale = _vec3{ 2.f, 10.0f, 2.0f };
	//	// 팬바람의 방향
	//	// 0이면 아래로 1이면 위로
	//	tObj.eMeshType = 0;
	//	pGameObject = CFan::Create(m_pGraphicDev, &tObj);
	//	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Float2", pGameObject, this), E_FAIL);

	//	tObj.vPos = { 0, 0, 35 };
	//	tObj.vScale = _vec3{ 2.f, 10.0f, 2.0f };
	//	// 팬바람의 방향
	//	// 0이면 아래로 1이면 위로
	//	tObj.eMeshType = 1;
	//	pGameObject = CFan::Create(m_pGraphicDev, &tObj);
	//	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Float3", pGameObject, this), E_FAIL);

	//	tObj.vPos = { 5, -0.5, 25 };
	//	tObj.vScale = _vec3{ 1.f, 1.0f, 1.0f };
	//	// Fan의 갯수
	//	tObj.eRenderType = 3;
	//	pGameObject = CLever::Create(m_pGraphicDev, &tObj);
	//	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Lever1", pGameObject, this), E_FAIL);
	//	pGameObject->Is<CLever>()->Add_FanList(L"Float1");
	//	pGameObject->Is<CLever>()->Add_FanList(L"Float2");
	//	pGameObject->Is<CLever>()->Add_FanList(L"Float3");

	//	tObj.vPos = { -5, -0.5, 25 };
	//	tObj.vScale = _vec3{ 1.f, 1.0f, 1.0f };
	//	// Fan의 갯수
	//	tObj.eRenderType = 3;
	//	pGameObject = CLever::Create(m_pGraphicDev, &tObj);
	//	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Lever2", pGameObject, this), E_FAIL);
	//	pGameObject->Is<CLever>()->Add_FanList(L"Float1");
	//	pGameObject->Is<CLever>()->Add_FanList(L"Float2");
	//	pGameObject->Is<CLever>()->Add_FanList(L"Float3");
	//}
	////tObj.vPos = { 0, 0, 5 };
	////tObj.vScale = { 0.01f, 0.01f, 0.01f };
	////// 레버 방향이 0이면 왼쪽 1이면 오른쪽
	////tObj.eMeshType = 0;
	////pGameObject = CFuseNPC::Create(m_pGraphicDev, &tObj);
	////FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Fuse1", pGameObject, this), E_FAIL);

	lstrcpy(tObj.tagMesh, L"VacuumField");
	tObj.vScale = _vec3{ 0.007f,0.007f,0.007f };
	tObj.eMeshType = 1;
	tObj.eRenderType = RENDER_NONALPHA;
	tObj.vPos = { 0.f,0.f,0.f };
	pGameObject = CMap::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Field", pGameObject, this), E_FAIL);

	m_mapLayer.emplace(pLayerTag, pLayer);


	return S_OK;
}

HRESULT CTestField::Ready_LoadMap_Layer(const _tchar * pLayerTag, const _tchar * pFilePath)
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

HRESULT CTestField::Ready_Interact_Layer(const _tchar* pLayerTag)
{
	Engine::CLayer*			pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject*	pGameObject = nullptr;
	OBJINFO tObj;
	ZeroMemory(&tObj, sizeof(OBJINFO));
	tObj.eSceneID = RESOURCE_TF;

	//tObj.vPos = { 5, 0, 0 };
	//pGameObject = CMiniVacuum::Create(m_pGraphicDev, DIRRIGHT, &tObj);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Minivacuum", pGameObject, this), E_FAIL);
	//auto vecBody = pGameObject->Is<CMiniVacuum>()->Get_HozeBody();

	//m_pScene->addAggregate(*pGameObject->Is<CMiniVacuum>()->Get_Hoze());
	//for (auto pBody : vecBody)
	//	setupFiltering(Engine::Get_Allocator(), pBody, FilterGroup::eHoze, FilterGroup::eCody | FilterGroup::eMay);

	
	
	//tObj.vPos = { -5, 0, 0 };
	//pGameObject = CMiniVacuum::Create(m_pGraphicDev, &tObj);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Minivacuum2", pGameObject, this), E_FAIL);
	//vecBody = pGameObject->Is<CMiniVacuum>()->Get_HozeBody();

	//m_pScene->addAggregate(*pGameObject->Is<CMiniVacuum>()->Get_Hoze());
	//for (auto pBody : vecBody)
	//	setupFiltering(Engine::Get_Allocator(), pBody, FilterGroup::eHoze, FilterGroup::eCody | FilterGroup::eMay);



	m_mapLayer.emplace(pLayerTag, pLayer);
	return S_OK;
}

HRESULT CTestField::Ready_LightInfo(void)
{
	D3DLIGHT9				tLightInfo;
	ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));

	tLightInfo.Type = D3DLIGHT_DIRECTIONAL;
	tLightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Ambient = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.f);
	tLightInfo.Direction = _vec3(-1.f, -1.f, 1.f);

	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 0);

	return S_OK;
}

CTestField * CTestField::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTestField*		pInstance = new CTestField(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
		Safe_Release(pInstance);

	return pInstance;
}

void CTestField::Free(void)
{
	// 삭제
	CCollisionMgr::DestroyInstance();

	for (_uint i = 0; i < m_vecObject.size(); ++i)
		Safe_Delete_Array(m_vecObject[i]);

	m_vecObject.clear();
	m_vecObject.shrink_to_fit();
	Engine::CScene::Free();
}
