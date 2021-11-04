#include "stdafx.h"
#include "ChessField.h"
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

#include "CodyWizard_IceOrbAtt.h"
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
#include "Arrow.h"
#include "TestCol.h"
#include "OgreChain.h"
#include "Ogre.h"
#include "OgreStartDoor.h"
#include "CenterHatch.h"
#include "ChessOpenDoor.h"
#include "ChessDoorSwitch.h"
#include "ChessDoorSwitchFrame.h"
#include "ChessBridge.h"
#include "ChessTrab.h"
#include "ChessDuoDoor.h"
#include "CrusherBreakWall.h"
#include "CrusherBridge.h"
#include "CrusherBridgeBase.h"
#include "CrusherBridgeHolder.h"
#include "CrusherHitPoint.h"
#include "CrusherBreakWall.h"
#include "Crusher.h"
#include "CrusherStartDoor.h"
#include "DeadSpace.h"
#include "Orb.h"
#include "Map.h"
#include "CameraSpace.h"
#include "DeadSpace.h"
#include "SaveSpace.h"


CChessField::CChessField(LPDIRECT3DDEVICE9 pDevice)
	: CScene(pDevice)
	, m_iKnightCount(0), m_iKnightCountMax(0), m_iArbalistCount(0), m_iArbalistCountMax(0), m_iSheilderCountMax(0), m_iSheilderCount(0)
{
}

CChessField::~CChessField()
{
}

HRESULT CChessField::Ready_Scene()
{
	Engine::Set_SoundScene(RESOURCE_CF);

	//CChessTile::GetInstance()->Ready_ChessTile();
	Engine::End_Split();

	Engine::Ready_Font(m_pGraphicDev, L"Font_Jinji", L"궁서", 20, 20, FW_BOLD);
	g_bMenu = false;
	g_bSlideFinal = false;

	auto* pPhysics = Engine::Get_Physics();
	auto* pDispatcher = Engine::Get_Dispatcher();
	auto* pCudaMgr = Engine::Get_CudaMgr();
	CScene::Ready_PhysX(this, pPhysics, pDispatcher, pCudaMgr);

	if (!m_pScene)
		return E_FAIL;


	//auto* pPlane = PxCreatePlane(*pPhysics, PxPlane(0, 1, 0, 0), *pPhysics->createMaterial(0.5f, 0.5f, 0.f));
	//m_pScene->addActor(*pPlane);

	//m

	////pPlane->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, false);

	m_pScene->setGravity(PxVec3(0.0f, -50.f, 0.0f));
	FAILED_CHECK_RETURN(Ready_Environment_Layer(L"Environment"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_GameObject_Layer(L"GameObject"), E_FAIL);
	//FAILED_CHECK_RETURN(Ready_LoadMap_Layer(L"LoadObject", L"../../Data/ITT_Test/Test.dat"), E_FAIL);

	StopAll();
	PlayBGM(L"ChessField_BGM.wav", 1.f);

	Ready_LightInfo();



	return S_OK;
}

_int CChessField::Update_Scene(const _float & fTimeDelta)
{
	_int iExit = Engine::CScene::Update_Scene(fTimeDelta);
	m_pScene->simulate(fTimeDelta);

	m_pScene->fetchResults(true);
	if (Engine::Key_Down(DIK_Y))
	{
		CHANGE_SCENE(m_pGraphicDev, SCENE_CB, OBJ_NOEVENT);

		return SCENE_CHANGE;
	}
	return iExit;
}

_int CChessField::LateUpdate_Scene(const _float & fTimeDelta)
{
	_int iExit = Engine::CScene::LateUpdate_Scene(fTimeDelta);
	 
	return iExit;
}

void CChessField::Render_Scene()
{
	CCollisionMgr::GetInstance()->Collision_Player_Monster();
	CCollisionMgr::GetInstance()->Collision_Enermy_Player();
	CCollisionMgr::GetInstance()->Collision_EnermyWeapon_Player();
	CCollisionMgr::GetInstance()->Collision_Bullet_Monster();
	CCollisionMgr::GetInstance()->Collision_SplashBullet_Monster();
	CCollisionMgr::GetInstance()->Clear_List();
}

void CChessField::onConstraintBreak(PxConstraintInfo* constraints, PxU32 count)
{

	return;
}

void CChessField::onWake(PxActor** actors, PxU32 count)
{
	OutputDebugString(L"깼음");
	return;
}

void CChessField::onSleep(PxActor** actors, PxU32 count)
{
	OutputDebugString(L"잠듬");

	return;
}

void CChessField::onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs)
{


	for (PxU32 i = 0; i < nbPairs; ++i)
	{
		if ((pairs[i].shapes[0]->getSimulationFilterData().word0 | pairs[i].shapes[1]->getSimulationFilterData().word0)
			== (FilterGroup::eWall | FilterGroup::eOgre))
		{

			OutputDebugString(L"벽과 오우거 충돌\n");
			COgre* pOgre = (COgre*)Engine::Get_GameObject(L"GameObject", L"ToyOgre");
			pOgre->Set_Stun(true);
		}
		/*return PxFilterFlag::eKILL;*/

		if ((pairs[i].shapes[0]->getSimulationFilterData().word0 | pairs[i].shapes[1]->getSimulationFilterData().word0)
			== (FilterGroup::eWall | FilterGroup::eIceOrb))
		{
			if (pairs[i].shapes[0]->getSimulationFilterData().word0 & FilterGroup::eIceOrb)
			{
				auto* pCol = ToObj<CGameObject>(pairs[i].shapes[0]->getName());
				pCol->Set_Dead();
			}
			else if (pairs[i].shapes[1]->getSimulationFilterData().word0 & FilterGroup::eIceOrb)
			{
				auto* pCol = ToObj<CGameObject>(pairs[i].shapes[1]->getName());
				pCol->Set_Dead();
			}

			OutputDebugString(L"아이스 오브와 벽 충돌\n");
		}

		if ((pairs[i].shapes[0]->getSimulationFilterData().word0 | pairs[i].shapes[1]->getSimulationFilterData().word0)
			== (FilterGroup::eWall | FilterGroup::eArrow))
		{
			if (pairs[i].shapes[0]->getSimulationFilterData().word0 & FilterGroup::eArrow)
			{
				auto* pCol = ToObj<CArrow>(pairs[i].shapes[0]->getName());
				pCol->Set_Dead();
			}
			else if(pairs[i].shapes[1]->getSimulationFilterData().word0 & FilterGroup::eArrow)
			{
				auto* pCol = ToObj<CArrow>(pairs[i].shapes[1]->getName());
				pCol->Set_Dead();

			}
//			OutputDebugString(L"화살과 벽 충돌\n");

		}
	}


}

void CChessField::onTrigger(PxTriggerPair* pairs, PxU32 count)
{


	for (PxU32 i = 0; i < count; ++i)
	{
		PxTriggerPair pPair = pairs[i];
		if (pPair.triggerShape->getSimulationFilterData().word0 & FilterGroup::eStone &&
			pPair.otherShape->getSimulationFilterData().word0 & FilterGroup::eOgre)
		{
			OutputDebugString(L"석상과 몬스터 충돌\n");
			COgre* pOgre = (COgre*)Engine::Get_GameObject(L"GameObject", L"ToyOgre");
			pOgre->Set_Stun(true);
			auto* pCol = ToObj<COgreChain>(pPair.triggerActor->getName());
			//auto* pCol =ToObj<CTestCol>(pPair.triggerActor->getName());
			if (!pCol->IsCol())
			{
				pCol->Set_OgreCol(true);
				pOgre->Add_StoneCol();

			}
		}

		if (pPair.triggerShape->getSimulationFilterData().word0 & FilterGroup::eHatch &&
			pPair.otherShape->getSimulationFilterData().word0 & FilterGroup::eOgre)
		{
			OutputDebugString(L"해치와 오우거 충돌\n");
			CCenterHatch* pHatch = (CCenterHatch*)Engine::Get_GameObject(L"GameObject", L"OgreCenterHatch1");
			COgre* pOgre = (COgre*)Engine::Get_GameObject(L"GameObject", L"ToyOgre");

			_bool bHatch;
			bHatch = pHatch->Get_HatchOpen();

			if (true == bHatch)
			{
				COgre* pOgre = (COgre*)Engine::Get_GameObject(L"GameObject", L"ToyOgre");
				pOgre->Set_HatchOpne(true);
			}

		}


		auto Trigger = pairs[i];
		auto* pTrigger = ToTrigger(Trigger.triggerShape->getName());

		if (!pTrigger)
			return;
		switch (pTrigger->Get_Type())
		{
		case TRIGGER_ATTACK:
			Event_ATTACK(&Trigger);
			break;
		}

		if (Trigger.otherShape->getSimulationFilterData().word0 &(FilterGroup::eOgre))
		{
			auto* pTrigger = ToTrigger(Trigger.triggerShape->getName());
			if (!pTrigger)
				return;


			switch (pTrigger->Get_Type())
			{
			case TRIGGER_CHESS_OGRE_DOOR:
				Event_OgreField_DoorStun(&Trigger);
				break;
			}
		}
		if (Trigger.otherShape->getSimulationFilterData().word0 &(FilterGroup::eCrusher))
		{
			auto* pTrigger = ToTrigger(Trigger.triggerShape->getName());
			if (!pTrigger)
				return;


			switch (pTrigger->Get_Type())
			{
			case TRIGGER_CHESS_CRUSHER_HIT:
				Event_Crusher_Hit(&Trigger);
				break;
			}
		}


		if (Trigger.otherShape->getSimulationFilterData().word0 &(FilterGroup::eCody | FilterGroup::eMay))
		{
			auto* pTrigger = ToTrigger(Trigger.triggerShape->getName());
			if (!pTrigger)
				return;

			switch (pTrigger->Get_Type())
			{
			case TRIGGER_CHESS_DOORBUTTON:
				Event_DoorButton(&Trigger);
				break;
			case TRIGGER_CHESS_FIREBUTTON:
				Event_FireButton(&Trigger);
				break;
			case TRIGGER_CHESS_CRUSHERBUTTON:
				Event_CrusherButton(&Trigger);
				break;
			case TRIGGER_CHESS_BRIDGE:
				Event_BridgeButton(&Trigger);
				break;
			case TRIGGER_CHESS_ARROW:
				Event_ArrowHit(&Trigger);
				break;
			case TRIGGER_CHESS_ONE_KNIGHTMOVE:
				Event_KnightMove(&Trigger);
				break;
			case TRIGGER_CHESS_TWO_KNIGHTMOVE:
				Event_Two_KinghtMove(&Trigger);
				break;
			case TRIGGER_CHESS_FIELD2_CREATE: //두번째 맵 몬스터 생성 트리거.
				Event_Field2_CreateMonster(&Trigger);
				break;
			case TRIGGER_CHESS_FIELD2_MOVE_ARBALIST:
				Event_Field2_MoveArbalist(&Trigger);
				break;
			case TRIGGER_CHESS_FIELD2_MOVE_KNIGHT:
				Event_Field2_MoveKnight(&Trigger);
				break;
			case TRIGGER_CHESS_FIELD3_CREATE:
				Event_Field3_CreateMonster(&Trigger);
				break;
			case TRIGGER_CHESS_FIELD3_MOVEROOM1:
				Event_Field3_Room1Move(&Trigger);
				break;
			case TRIGGER_CHESS_FIELD3_MOVEROOM2:
				Event_Field3_Room2Move(&Trigger);
				break;
			case TRIGGER_CHESS_FIELD3_MOVEROOM3:
				Event_Field3_Room3Move(&Trigger);
				break;
			case TRIGGER_CHESS_FIELD3_MOVEROOM4:
				Event_Field3_Room4Move(&Trigger);
				break;
			case TRIGGER_CHESS_FIELD3_MOVEROOM5:
				Event_Field3_Room5Move(&Trigger);
				break;
			case TRIGGER_CHESS_FIELD3_MOVEROOM6:
				Event_Field3_Room6Move(&Trigger);
				break;
			case TRIGGER_CHESS_OGRE_STONE:
				Event_OgreStone(&Trigger);
				break;
			case TRIGGER_DEADSPACE:
				Event_Dead(&Trigger);
				break;
			case TRIGGER_SAVESPACE:
				Event_Save(&Trigger);
				break;

			}
		}
	}


}

void CChessField::onAdvance(const PxRigidBody*const* bodyBuffer, const PxTransform* poseBuffer, const PxU32 count)
{
	OutputDebugString(L"업데이트");
}

void CChessField::Event_ATTACK(PxTriggerPair * pairs)
{
	auto* pTrigger = ToTrigger(pairs->triggerShape->getName());
	if (pairs->otherShape->getSimulationFilterData().word0 & FilterGroup::eMonster)
	{
		if (pairs->status &PxPairFlag::eNOTIFY_TOUCH_FOUND)
			ToObj<CEnermy>(pairs->otherShape->getName())->Set_TickDamage(true, 2, false);
		else if (pairs->status &PxPairFlag::eNOTIFY_TOUCH_LOST)
			ToObj<CEnermy>(pairs->otherShape->getName())->Set_TickOff(false);
	}
}

void CChessField::Event_DoorButton(PxTriggerPair * pairs)
{
	CChessDoorSwitch* pSwitch = ToTrigger(pairs->triggerShape->getActor()->getName())->Is<CChessDoorSwitch>();
	if (nullptr == pSwitch)
		return;

	if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
	{
		if (!pSwitch->IsColDoor())
		{
			OutputDebugString(L"문 발판과 충돌\n");
			pSwitch->Set_MoveDoor(true);
		}
	}

}

void CChessField::Event_FireButton(PxTriggerPair * pairs)
{
	CChessDoorSwitch* pSwitch = ToTrigger(pairs->triggerShape->getActor()->getName())->Is<CChessDoorSwitch>();
	if (nullptr == pSwitch)
		return;


	if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
	{
		OutputDebugString(L"불기둥 발판과 충돌\n");
		pSwitch->Set_CreateFire(true);
	}
	if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_LOST)
	{

		OutputDebugString(L"불기둥 발판에서 나감\n");
		pSwitch->Set_CreateFire(false);
	}

}

void CChessField::Event_BridgeButton(PxTriggerPair * pairs)
{
	CChessDoorSwitch* pSwitch = ToTrigger(pairs->triggerShape->getActor()->getName())->Is<CChessDoorSwitch>();
	if (nullptr == pSwitch)
		return;

	if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
	{
		OutputDebugString(L"다리 발판과 충돌\n");
		pSwitch->Set_MoveBridge(true);

		for (_uint i = 0; i < 2; ++i)
		{
			_tchar tagTemp[MAX_PATH];
			wsprintf(tagTemp, L"OneToyArbalist %d", i);

			CArbalist*	pArbalist = (CArbalist*)Engine::Get_GameObject(L"GameObject", tagTemp);
			if (nullptr == pArbalist)
				return;

			pArbalist->Set_Move(true);
		}
	}
	if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_LOST)
	{

		OutputDebugString(L"다리 발판에서 나감\n");
		pSwitch->Set_MoveBridge(false);
	}
}

void CChessField::Event_KnightMove(PxTriggerPair * pairs)
{
	if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
	{
		OutputDebugString(L"첫번째 기사들 움직이는 트리거\n");
		for (_uint i = 0; i < 5; ++i)
		{
			_tchar tagTemp[MAX_PATH];
			wsprintf(tagTemp, L"FristToyKnight %d", i);

			CKnight*	pKnight = (CKnight*)Engine::Get_GameObject(L"GameObject", tagTemp);
			if (nullptr == pKnight)
				return;

			pKnight->Set_Move(true);
		}
	}
	m_pTrigger[TRIGGER_CHESS_ONE_KNIGHTMOVE]->Set_Interactable(false);

}

void CChessField::Event_Two_KinghtMove(PxTriggerPair * pairs)
{
	if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
	{
		OutputDebugString(L"두번째 기사들 움직이는 트리거\n");
		for (_uint i = 0; i < 3; ++i)
		{
			_tchar tagTemp[MAX_PATH];
			wsprintf(tagTemp, L"TwoToyKnight %d", i);

			CKnight*	pKnight = (CKnight*)Engine::Get_GameObject(L"GameObject", tagTemp);
			if (nullptr == pKnight)
				return;

			pKnight->Set_Move(true);
		}
	}
	m_pTrigger[TRIGGER_CHESS_TWO_KNIGHTMOVE]->Set_Interactable(false);
}

void CChessField::Event_Field2_CreateMonster(PxTriggerPair * pairs)
{
	if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
	{
		Engine::CLayer*				pLayer = Engine::Get_Layer(L"GameObject");
		Engine::CGameObject*		pGameObject = nullptr;


		_tchar tagTemp[MAX_PATH];
		_tchar tagWeaponTemp[MAX_PATH];
		OBJINFO tTemp;
		OBJINFO pObj;


		//마법사 생성
		CStaff* pStaff = nullptr;
		pGameObject = pStaff = CStaff::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, );
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ToyMage_Staff", pGameObject), );

		pObj.vScale = { 0.01f, 0.01f, 0.01f };
		pObj.vAngle = { 0.f, -90.f, 0.f };
		pObj.vPos = { 28.4f, 0.f, -51.5f };
		pGameObject = CMage::Create(m_pGraphicDev, pStaff, &pObj);
		NULL_CHECK_RETURN(pGameObject, );
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ToyMage", pGameObject, this), );
		setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eDefaultTrigger| FilterGroup::eWall | FilterGroup::eMonster);


		//아처 1페이즈 생성


		m_iArbalistCount = 0;
		m_iArbalistCountMax = 0;

		wsprintf(tagTemp, L"Field2_OneToyArbalist %d", m_iArbalistCount);
		wsprintf(tagWeaponTemp, L"Field2_OneToyArbalist_CrossBow %d", m_iArbalistCount);
		tTemp.tParentBone = tagTemp;

		CCrossBow* pCrossBow = nullptr;
		pGameObject = pCrossBow = CCrossBow::Create(m_pGraphicDev, &tTemp);
		NULL_CHECK_RETURN(pGameObject, );
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagWeaponTemp, pGameObject), );

		pObj.vScale = { 0.015f, 0.015f, 0.015f };
		pObj.vAngle = { 0.f, 0.f, 0.f };
		pObj.vPos = { 43.5f , 0.f, -50.f };
		pObj.tParentBone = tTemp.tParentBone;
		pGameObject = CArbalist::Create(m_pGraphicDev, pCrossBow, &pObj);
		NULL_CHECK_RETURN(pGameObject, );
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObj.tParentBone, pGameObject, this), );
		setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eDefaultTrigger);


		m_iArbalistCount++;
		m_iArbalistCountMax++;

		tagTemp[MAX_PATH];
		wsprintf(tagTemp, L"Field2_OneToyArbalist %d", m_iArbalistCount);

		tagWeaponTemp[MAX_PATH];
		wsprintf(tagWeaponTemp, L"Field2_OneToyArbalist_CrossBow %d", m_iArbalistCount);


		tTemp.tParentBone = tagTemp;
		pCrossBow = nullptr;
		pGameObject = pCrossBow = CCrossBow::Create(m_pGraphicDev, &tTemp);
		NULL_CHECK_RETURN(pGameObject, );
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagWeaponTemp, pGameObject), );

		pObj.vScale = { 0.015f, 0.015f, 0.015f };
		pObj.vAngle = { 0.f, 0.f, 0.f };
		pObj.vPos = { 41.5f , 0.f, -50.f };
		pObj.tParentBone = tTemp.tParentBone;
		pGameObject = CArbalist::Create(m_pGraphicDev, pCrossBow, &pObj);
		NULL_CHECK_RETURN(pGameObject, );
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObj.tParentBone, pGameObject, this), );

		setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eDefaultTrigger);


		m_iArbalistCount++;
		m_iArbalistCountMax++;

		tagTemp[MAX_PATH];
		wsprintf(tagTemp, L"Field2_OneToyArbalist %d", m_iArbalistCount);

		tagWeaponTemp[MAX_PATH];
		wsprintf(tagWeaponTemp, L"Field2_OneToyArbalist_CrossBow %d", m_iArbalistCount);


		tTemp.tParentBone = tagTemp;
		pCrossBow = nullptr;
		pGameObject = pCrossBow = CCrossBow::Create(m_pGraphicDev, &tTemp);
		NULL_CHECK_RETURN(pGameObject, );
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagWeaponTemp, pGameObject), );

		pObj.vScale = { 0.015f, 0.015f, 0.015f };
		pObj.vAngle = { 0.f, -180.f, 0.f };
		pObj.vPos = { 43.5f , 0.f, -35.f };
		pObj.tParentBone = tTemp.tParentBone;
		pGameObject = CArbalist::Create(m_pGraphicDev, pCrossBow, &pObj);
		NULL_CHECK_RETURN(pGameObject, );
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObj.tParentBone, pGameObject, this), );

		setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eDefaultTrigger);


		m_iArbalistCount++;
		m_iArbalistCountMax++;

		tagTemp[MAX_PATH];
		wsprintf(tagTemp, L"Field2_OneToyArbalist %d", m_iArbalistCount);

		tagWeaponTemp[MAX_PATH];
		wsprintf(tagWeaponTemp, L"Field2_OneToyArbalist_CrossBow %d", m_iArbalistCount);


		tTemp.tParentBone = tagTemp;
		pCrossBow = nullptr;
		pGameObject = pCrossBow = CCrossBow::Create(m_pGraphicDev, &tTemp);
		NULL_CHECK_RETURN(pGameObject, );
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagWeaponTemp, pGameObject), );

		pObj.vScale = { 0.015f, 0.015f, 0.015f };
		pObj.vAngle = { 0.f, 180.f, 0.f };
		pObj.vPos = { 41.5f , 0.f, -35.f };
		pObj.tParentBone = tTemp.tParentBone;
		pGameObject = CArbalist::Create(m_pGraphicDev, pCrossBow, &pObj);
		NULL_CHECK_RETURN(pGameObject, );
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObj.tParentBone, pGameObject, this), );

		setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eDefaultTrigger);


		m_iArbalistCount++;
		m_iArbalistCountMax++;

		tagTemp[MAX_PATH];
		wsprintf(tagTemp, L"Field2_OneToyArbalist %d", m_iArbalistCount);

		tagWeaponTemp[MAX_PATH];
		wsprintf(tagWeaponTemp, L"Field2_OneToyArbalist_CrossBow %d", m_iArbalistCount);


		tTemp.tParentBone = tagTemp;
		pCrossBow = nullptr;
		pGameObject = pCrossBow = CCrossBow::Create(m_pGraphicDev, &tTemp);
		NULL_CHECK_RETURN(pGameObject, );
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagWeaponTemp, pGameObject), );

		pObj.vScale = { 0.015f, 0.015f, 0.015f };
		pObj.vAngle = { 0.f, 180.f, 0.f };
		pObj.vPos = { 39.5f , 0.f, -35.f };
		pObj.tParentBone = tTemp.tParentBone;
		pGameObject = CArbalist::Create(m_pGraphicDev, pCrossBow, &pObj);
		NULL_CHECK_RETURN(pGameObject, );
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObj.tParentBone, pGameObject, this), );

		setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eDefaultTrigger);


		m_iArbalistCount++;
		m_iArbalistCountMax++;


		//아처 2페이즈 생성.
		m_iArbalistCount = 0;
		m_iArbalistCountMax = 0;

		tagTemp[MAX_PATH];
		wsprintf(tagTemp, L"Field2_TwoToyArbalist %d", m_iArbalistCount);

		tagWeaponTemp[MAX_PATH];
		wsprintf(tagWeaponTemp, L"Field2_TwoToyArbalist_CrossBow %d", m_iArbalistCount);


		tTemp.tParentBone = tagTemp;
		pCrossBow = nullptr;
		pGameObject = pCrossBow = CCrossBow::Create(m_pGraphicDev, &tTemp);
		NULL_CHECK_RETURN(pGameObject, );
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagWeaponTemp, pGameObject), );

		pObj.vScale = { 0.015f, 0.015f, 0.015f };
		pObj.vAngle = { 0.f, -85.f, 0.f };
		pObj.vPos = { 46.f , 0.f, -56.f };
		pObj.tParentBone = tTemp.tParentBone;
		pGameObject = CArbalist::Create(m_pGraphicDev, pCrossBow, &pObj);
		NULL_CHECK_RETURN(pGameObject, );
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObj.tParentBone, pGameObject, this), );

		setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eDefaultTrigger);


		m_iArbalistCount++;
		m_iArbalistCountMax++;

		tagTemp[MAX_PATH];
		wsprintf(tagTemp, L"Field2_TwoToyArbalist %d", m_iArbalistCount);

		tagWeaponTemp[MAX_PATH];
		wsprintf(tagWeaponTemp, L"Field2_TwoToyArbalist_CrossBow %d", m_iArbalistCount);


		tTemp.tParentBone = tagTemp;
		pCrossBow = nullptr;
		pGameObject = pCrossBow = CCrossBow::Create(m_pGraphicDev, &tTemp);
		NULL_CHECK_RETURN(pGameObject, );
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagWeaponTemp, pGameObject), );

		pObj.vScale = { 0.015f, 0.015f, 0.015f };
		pObj.vAngle = { 0.f, -85.f, 0.f };
		pObj.vPos = { 50.f , 0.f, -61.f };
		pObj.tParentBone = tTemp.tParentBone;
		pGameObject = CArbalist::Create(m_pGraphicDev, pCrossBow, &pObj);
		NULL_CHECK_RETURN(pGameObject, );
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObj.tParentBone, pGameObject, this), );

		setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eDefaultTrigger);

		m_iArbalistCount++;
		m_iArbalistCountMax++;

		tagTemp[MAX_PATH];
		wsprintf(tagTemp, L"Field2_TwoToyArbalist %d", m_iArbalistCount);

		tagWeaponTemp[MAX_PATH];
		wsprintf(tagWeaponTemp, L"Field2_TwoToyArbalist_CrossBow %d", m_iArbalistCount);


		tTemp.tParentBone = tagTemp;
		pCrossBow = nullptr;
		pGameObject = pCrossBow = CCrossBow::Create(m_pGraphicDev, &tTemp);
		NULL_CHECK_RETURN(pGameObject, );
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagWeaponTemp, pGameObject), );

		pObj.vScale = { 0.015f, 0.015f, 0.015f };
		pObj.vAngle = { 0.f, -85.f, 0.f };
		pObj.vPos = { 47.f , 0.f, -70.f };
		pObj.tParentBone = tTemp.tParentBone;
		pGameObject = CArbalist::Create(m_pGraphicDev, pCrossBow, &pObj);
		NULL_CHECK_RETURN(pGameObject, );
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObj.tParentBone, pGameObject, this), );

		setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eDefaultTrigger);

		m_iArbalistCount++;
		m_iArbalistCountMax++;


		m_iKnightCount = 0;
		m_iKnightCountMax = 0;

		tagTemp[MAX_PATH];
		wsprintf(tagTemp, L"Field2_OneToyKnight %d", m_iKnightCount);

		tagWeaponTemp[MAX_PATH];
		wsprintf(tagWeaponTemp, L"Field2_OneToyKnight_MorningStar %d", m_iKnightCount);


		tTemp.tParentBone = tagTemp;

		CMorningStar* pMornigStar = nullptr;
		pGameObject = pMornigStar = CMorningStar::Create(m_pGraphicDev, &tTemp);
		NULL_CHECK_RETURN(pGameObject, );
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagWeaponTemp, pGameObject), );

		pObj.vScale = { 0.01f, 0.01f, 0.01f };
		pObj.vAngle = { 0.f, 50.f, 0.f };
		pObj.vPos = { 49.f , 0.f, -55.f };
		pObj.tParentBone = tTemp.tParentBone;
		pGameObject = CKnight::Create(m_pGraphicDev, pMornigStar, &pObj);
		NULL_CHECK_RETURN(pGameObject, );
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObj.tParentBone, pGameObject, this), );
		setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster,FilterGroup::eDefaultTrigger| FilterGroup::eWall | FilterGroup::eMonster | FilterGroup::eGround);

		m_iKnightCount++;
		m_iKnightCountMax++;


		tagTemp[MAX_PATH];
		wsprintf(tagTemp, L"Field2_OneToyKnight %d", m_iKnightCount);

		tagWeaponTemp[MAX_PATH];
		wsprintf(tagWeaponTemp, L"Field2_OneToyKnight_MorningStar %d", m_iKnightCount);


		tTemp.tParentBone = tagTemp;

		pMornigStar = nullptr;
		pGameObject = pMornigStar = CMorningStar::Create(m_pGraphicDev, &tTemp);
		NULL_CHECK_RETURN(pGameObject, );
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagWeaponTemp, pGameObject), );

		pObj.vScale = { 0.01f, 0.01f, 0.01f };
		pObj.vAngle = { 0.f, 50.f, 0.f };
		pObj.vPos = { 49.f , 0.f, -54.f };
		pObj.tParentBone = tTemp.tParentBone;
		pGameObject = CKnight::Create(m_pGraphicDev, pMornigStar, &pObj);
		NULL_CHECK_RETURN(pGameObject, );
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObj.tParentBone, pGameObject, this), );
		setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eDefaultTrigger | FilterGroup::eWall | FilterGroup::eMonster | FilterGroup::eGround);

		m_iKnightCount++;
		m_iKnightCountMax++;


		tagTemp[MAX_PATH];
		wsprintf(tagTemp, L"Field2_OneToyKnight %d", m_iKnightCount);

		tagWeaponTemp[MAX_PATH];
		wsprintf(tagWeaponTemp, L"Field2_OneToyKnight_MorningStar %d", m_iKnightCount);


		tTemp.tParentBone = tagTemp;

		pMornigStar = nullptr;
		pGameObject = pMornigStar = CMorningStar::Create(m_pGraphicDev, &tTemp);
		NULL_CHECK_RETURN(pGameObject, );
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagWeaponTemp, pGameObject), );

		pObj.vScale = { 0.01f, 0.01f, 0.01f };
		pObj.vAngle = { 0.f, 50.f, 0.f };
		pObj.vPos = { 50.f , 0.f, -55.f };
		pObj.tParentBone = tTemp.tParentBone;
		pGameObject = CKnight::Create(m_pGraphicDev, pMornigStar, &pObj);
		NULL_CHECK_RETURN(pGameObject, );
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObj.tParentBone, pGameObject, this), );
		setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eDefaultTrigger | FilterGroup::eWall | FilterGroup::eMonster | FilterGroup::eGround);

		m_iKnightCount++;
		m_iKnightCountMax++;


		tagTemp[MAX_PATH];
		wsprintf(tagTemp, L"Field2_OneToyKnight %d", m_iKnightCount);

		tagWeaponTemp[MAX_PATH];
		wsprintf(tagWeaponTemp, L"Field2_OneToyKnight_MorningStar %d", m_iKnightCount);


		tTemp.tParentBone = tagTemp;

		pMornigStar = nullptr;
		pGameObject = pMornigStar = CMorningStar::Create(m_pGraphicDev, &tTemp);
		NULL_CHECK_RETURN(pGameObject, );
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagWeaponTemp, pGameObject), );

		pObj.vScale = { 0.01f, 0.01f, 0.01f };
		pObj.vAngle = { 0.f, 50.f, 0.f };
		pObj.vPos = { 50.f , 0.f, -54.f };
		pObj.tParentBone = tTemp.tParentBone;
		pGameObject = CKnight::Create(m_pGraphicDev, pMornigStar, &pObj);
		NULL_CHECK_RETURN(pGameObject, );
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObj.tParentBone, pGameObject, this), );
		setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eDefaultTrigger | FilterGroup::eWall | FilterGroup::eMonster | FilterGroup::eGround);

		m_iKnightCount++;
		m_iKnightCountMax++;


		tagTemp[MAX_PATH];
		wsprintf(tagTemp, L"Field2_OneToyKnight %d", m_iKnightCount);
		tagWeaponTemp[MAX_PATH];
		wsprintf(tagWeaponTemp, L"Field2_OneToyKnight_MorningStar %d", m_iKnightCount);
		tTemp.tParentBone = tagTemp;

		pMornigStar = nullptr;
		pGameObject = pMornigStar = CMorningStar::Create(m_pGraphicDev, &tTemp);
		NULL_CHECK_RETURN(pGameObject, );
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagWeaponTemp, pGameObject), );

		pObj.vScale = { 0.01f, 0.01f, 0.01f };
		pObj.vAngle = { 0.f, 50.f, 0.f };
		pObj.vPos = { 50.f , 0.f, -56.f };
		pObj.tParentBone = tTemp.tParentBone;
		pGameObject = CKnight::Create(m_pGraphicDev, pMornigStar, &pObj);
		NULL_CHECK_RETURN(pGameObject, );
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObj.tParentBone, pGameObject, this), );
		setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eDefaultTrigger | FilterGroup::eWall | FilterGroup::eMonster | FilterGroup::eGround);

		m_iKnightCount++;
		m_iKnightCountMax++;

		tagTemp[MAX_PATH];
		wsprintf(tagTemp, L"Field2_OneToyKnight %d", m_iKnightCount);
		tagWeaponTemp[MAX_PATH];
		wsprintf(tagWeaponTemp, L"Field2_OneToyKnight_MorningStar %d", m_iKnightCount);
		tTemp.tParentBone = tagTemp;

		pMornigStar = nullptr;
		pGameObject = pMornigStar = CMorningStar::Create(m_pGraphicDev, &tTemp);
		NULL_CHECK_RETURN(pGameObject, );
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagWeaponTemp, pGameObject), );

		pObj.vScale = { 0.01f, 0.01f, 0.01f };
		pObj.vAngle = { 0.f, 50.f, 0.f };
		pObj.vPos = { 47.f , 0.f, -67.f };
		pObj.tParentBone = tTemp.tParentBone;
		pGameObject = CKnight::Create(m_pGraphicDev, pMornigStar, &pObj);
		NULL_CHECK_RETURN(pGameObject, );
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObj.tParentBone, pGameObject, this), );
		setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eDefaultTrigger | FilterGroup::eWall | FilterGroup::eMonster | FilterGroup::eGround);

		m_iKnightCount++;
		m_iKnightCountMax++;

		tagTemp[MAX_PATH];
		wsprintf(tagTemp, L"Field2_OneToyKnight %d", m_iKnightCount);
		tagWeaponTemp[MAX_PATH];
		wsprintf(tagWeaponTemp, L"Field2_OneToyKnight_MorningStar %d", m_iKnightCount);
		tTemp.tParentBone = tagTemp;

		pMornigStar = nullptr;
		pGameObject = pMornigStar = CMorningStar::Create(m_pGraphicDev, &tTemp);
		NULL_CHECK_RETURN(pGameObject, );
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagWeaponTemp, pGameObject), );

		pObj.vScale = { 0.01f, 0.01f, 0.01f };
		pObj.vAngle = { 0.f, 50.f, 0.f };
		pObj.vPos = { 47.f , 0.f, -66.f };
		pObj.tParentBone = tTemp.tParentBone;
		pGameObject = CKnight::Create(m_pGraphicDev, pMornigStar, &pObj);
		NULL_CHECK_RETURN(pGameObject, );
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObj.tParentBone, pGameObject, this), );
		setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eDefaultTrigger | FilterGroup::eWall | FilterGroup::eMonster | FilterGroup::eGround);

		m_iKnightCount++;
		m_iKnightCountMax++;

		tagTemp[MAX_PATH];
		wsprintf(tagTemp, L"Field2_OneToyKnight %d", m_iKnightCount);
		tagWeaponTemp[MAX_PATH];
		wsprintf(tagWeaponTemp, L"Field2_OneToyKnight_MorningStar %d", m_iKnightCount);
		tTemp.tParentBone = tagTemp;

		pMornigStar = nullptr;
		pGameObject = pMornigStar = CMorningStar::Create(m_pGraphicDev, &tTemp);
		NULL_CHECK_RETURN(pGameObject, );
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagWeaponTemp, pGameObject), );

		pObj.vScale = { 0.01f, 0.01f, 0.01f };
		pObj.vAngle = { 0.f, 50.f, 0.f };
		pObj.vPos = { 46.f , 0.f, -67.f };
		pObj.tParentBone = tTemp.tParentBone;
		pGameObject = CKnight::Create(m_pGraphicDev, pMornigStar, &pObj);
		NULL_CHECK_RETURN(pGameObject, );
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObj.tParentBone, pGameObject, this), );
		setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eDefaultTrigger | FilterGroup::eWall | FilterGroup::eMonster | FilterGroup::eGround);

		m_iKnightCount++;
		m_iKnightCountMax++;

		tagTemp[MAX_PATH];
		wsprintf(tagTemp, L"Field2_OneToyKnight %d", m_iKnightCount);
		tagWeaponTemp[MAX_PATH];
		wsprintf(tagWeaponTemp, L"Field2_OneToyKnight_MorningStar %d", m_iKnightCount);
		tTemp.tParentBone = tagTemp;

		pMornigStar = nullptr;
		pGameObject = pMornigStar = CMorningStar::Create(m_pGraphicDev, &tTemp);
		NULL_CHECK_RETURN(pGameObject, );
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagWeaponTemp, pGameObject), );

		pObj.vScale = { 0.01f, 0.01f, 0.01f };
		pObj.vAngle = { 0.f, 50.f, 0.f };
		pObj.vPos = { 46.f , 0.f, -66.f };
		pObj.tParentBone = tTemp.tParentBone;
		pGameObject = CKnight::Create(m_pGraphicDev, pMornigStar, &pObj);
		NULL_CHECK_RETURN(pGameObject, );
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObj.tParentBone, pGameObject, this), );
		setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eDefaultTrigger | FilterGroup::eWall | FilterGroup::eMonster | FilterGroup::eGround);

		m_iKnightCount++;
		m_iKnightCountMax++;

		tagTemp[MAX_PATH];
		wsprintf(tagTemp, L"Field2_OneToyKnight %d", m_iKnightCount);
		tagWeaponTemp[MAX_PATH];
		wsprintf(tagWeaponTemp, L"Field2_OneToyKnight_MorningStar %d", m_iKnightCount);
		tTemp.tParentBone = tagTemp;

		pMornigStar = nullptr;
		pGameObject = pMornigStar = CMorningStar::Create(m_pGraphicDev, &tTemp);
		NULL_CHECK_RETURN(pGameObject, );
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagWeaponTemp, pGameObject), );

		pObj.vScale = { 0.01f, 0.01f, 0.01f };
		pObj.vAngle = { 0.f, 50.f, 0.f };
		pObj.vPos = { 48.f , 0.f, -67.f };
		pObj.tParentBone = tTemp.tParentBone;
		pGameObject = CKnight::Create(m_pGraphicDev, pMornigStar, &pObj);
		NULL_CHECK_RETURN(pGameObject, );
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObj.tParentBone, pGameObject, this), );
		setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eDefaultTrigger | FilterGroup::eWall | FilterGroup::eMonster | FilterGroup::eGround);

		m_iKnightCount++;
		m_iKnightCountMax++;
		OutputDebugString(L"두번째 필드 몬스터 생성\n");
	}
	m_pTrigger[TRIGGER_CHESS_FIELD2_CREATE]->Set_Interactable(false);
}

void CChessField::Event_Field2_MoveArbalist(PxTriggerPair * pairs)
{
	if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
	{
		OutputDebugString(L"Field2 5명 아처 움직이는 트리거\n");
		for (_uint i = 0; i < 5; ++i)
		{
			_tchar tagTemp[MAX_PATH];
			wsprintf(tagTemp, L"Field2_OneToyArbalist %d", i);

			CArbalist*	pArbalist = (CArbalist*)Engine::Get_GameObject(L"GameObject", tagTemp);
			if (nullptr == pArbalist)
				return;

			pArbalist->Set_Move(true);
		}

	}
	m_pTrigger[TRIGGER_CHESS_FIELD2_MOVE_ARBALIST]->Set_Interactable(false);
}

void CChessField::Event_Field2_MoveKnight(PxTriggerPair * pairs)
{
	if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
	{
		OutputDebugString(L"Field2 문뒤에 기사,아처 움직이는 트리거\n");



		for (_uint i = 0; i < 10; ++i)
		{
			_tchar tagTemp[MAX_PATH];
			wsprintf(tagTemp, L"Field2_OneToyKnight %d", i);

			CKnight*	pKnight = (CKnight*)Engine::Get_GameObject(L"GameObject", tagTemp);
			if (nullptr == pKnight)
				return;

			pKnight->Set_Move(true);
		}

		for (_uint i = 0; i < 3; ++i)
		{
			_tchar tagTemp[MAX_PATH];
			wsprintf(tagTemp, L"Field2_TwoToyArbalist %d", i);

			CArbalist*	pArbalist = (CArbalist*)Engine::Get_GameObject(L"GameObject", tagTemp);
			if (nullptr == pArbalist)
				return;

			pArbalist->Set_Move(true);
		}
	}

	CGameObject*		pMap = Engine::Get_GameObject(L"GameObject", L"Field1");
	if (pMap)
		pMap->Set_Dead();

	m_pTrigger[TRIGGER_CHESS_FIELD2_MOVE_KNIGHT]->Set_Interactable(false);
}

void CChessField::Event_Crusher_Hit(PxTriggerPair * pairs)
{
	if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
	{
		for (_uint i = 1; i < 5; ++i)
		{
			_tchar tagTemp[MAX_PATH];
			wsprintf(tagTemp, L"CrusherHitPoint%d", i);

			CCrusherHitPoint*	pHitPoint = (CCrusherHitPoint*)Engine::Get_GameObject(L"GameObject", tagTemp);
			if (nullptr == pHitPoint)
				return;

			pHitPoint->Set_Hit(true);
		}
		OutputDebugString(L"크러셔와 Hit가능 트리거 충돌.");
	}

	m_pTrigger[TRIGGER_CHESS_CRUSHER_HIT]->Set_Interactable(false);
}

void CChessField::Event_Field3_CreateMonster(PxTriggerPair * pairs)
{
	if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
	{
		//Engine::CLayer*				pLayer = Engine::Get_Layer(L"GameObject");
		//Engine::CGameObject*		pGameObject = nullptr;

		//OutputDebugString(L"세번째 필드 몬스터 생성\n");

		//_tchar tagTemp[MAX_PATH];
		//_tchar tagWeaponTemp[MAX_PATH];
		//OBJINFO tTemp;
		//OBJINFO pObj;
		//CMorningStar* pMornigStar = nullptr;
		//CCrossBow* pCrossBow = nullptr;
		//CSheild* pSheild = nullptr;

		//m_iKnightCount = 0;
		//m_iKnightCountMax = 0;

		//m_iArbalistCount = 0;
		//m_iArbalistCountMax = 0;
		////1번룸
		//wsprintf(tagTemp, L"Field3_Room1ToyArbalist %d", m_iArbalistCount);
		//wsprintf(tagWeaponTemp, L"Field3_Room1ToyArbalist_CrossBow %d", m_iArbalistCount);
		//tTemp.tParentBone = tagTemp;

		//pCrossBow = nullptr;
		//pGameObject = pCrossBow = CCrossBow::Create(m_pGraphicDev, &tTemp);
		//NULL_CHECK_RETURN(pGameObject, );
		//FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagWeaponTemp, pGameObject), );

		//pObj.vScale = { 0.015f, 0.015f, 0.015f };
		//pObj.vAngle = { 0.f, 155.f, 0.f };
		//pObj.vPos = { 54.4f , 0.f, -77.4f };
		//pObj.tParentBone = tTemp.tParentBone;
		//pGameObject = CArbalist::Create(m_pGraphicDev, pCrossBow, &pObj);
		//NULL_CHECK_RETURN(pGameObject, );
		//FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObj.tParentBone, pGameObject, this), );
		//setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eWall | FilterGroup::eMonster);

		//m_iArbalistCount++;
		//m_iArbalistCountMax++;

		//tagTemp[MAX_PATH];
		//wsprintf(tagTemp, L"Field3_Room1ToyKnight %d", m_iKnightCount);
		//tagWeaponTemp[MAX_PATH];
		//wsprintf(tagWeaponTemp, L"Field3_Room1ToyKnight_MorningStar %d", m_iKnightCount);
		//tTemp.tParentBone = tagTemp;

		//pMornigStar = nullptr;
		//pGameObject = pMornigStar = CMorningStar::Create(m_pGraphicDev, &tTemp);
		//NULL_CHECK_RETURN(pGameObject, );
		//FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagWeaponTemp, pGameObject), );

		//pObj.vScale = { 0.01f, 0.01f, 0.01f };
		//pObj.vAngle = { 0.f, 68.f, 0.f };
		//pObj.vPos = { 52.9f , 0.f, -79.4f };
		//pObj.tParentBone = tTemp.tParentBone;
		//pGameObject = CKnight::Create(m_pGraphicDev, pMornigStar, &pObj);
		//NULL_CHECK_RETURN(pGameObject, );
		//FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObj.tParentBone, pGameObject, this), );
		//setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eWall | FilterGroup::eMonster | FilterGroup::eGround);

		//m_iKnightCount++;
		//m_iKnightCountMax++;

		//tagTemp[MAX_PATH];
		//wsprintf(tagTemp, L"Field3_Room1ToyKnight %d", m_iKnightCount);
		//tagWeaponTemp[MAX_PATH];
		//wsprintf(tagWeaponTemp, L"Field3_Room1ToyKnight_MorningStar %d", m_iKnightCount);
		//tTemp.tParentBone = tagTemp;

		//pMornigStar = nullptr;
		//pGameObject = pMornigStar = CMorningStar::Create(m_pGraphicDev, &tTemp);
		//NULL_CHECK_RETURN(pGameObject, );
		//FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagWeaponTemp, pGameObject), );

		//pObj.vScale = { 0.01f, 0.01f, 0.01f };
		//pObj.vAngle = { 0.f, 48.f, 0.f };
		//pObj.vPos = { 52.9f , 0.f, -80.9f };
		//pObj.tParentBone = tTemp.tParentBone;
		//pGameObject = CKnight::Create(m_pGraphicDev, pMornigStar, &pObj);
		//NULL_CHECK_RETURN(pGameObject, );
		//FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObj.tParentBone, pGameObject, this), );
		//setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eWall | FilterGroup::eMonster | FilterGroup::eGround);

		//m_iKnightCount++;
		//m_iKnightCountMax++;

		//tagTemp[MAX_PATH];
		//wsprintf(tagTemp, L"Field3_Room1ToyKnight %d", m_iKnightCount);
		//tagWeaponTemp[MAX_PATH];
		//wsprintf(tagWeaponTemp, L"Field3_Room1ToyKnight_MorningStar %d", m_iKnightCount);
		//tTemp.tParentBone = tagTemp;

		//pMornigStar = nullptr;
		//pGameObject = pMornigStar = CMorningStar::Create(m_pGraphicDev, &tTemp);
		//NULL_CHECK_RETURN(pGameObject, );
		//FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagWeaponTemp, pGameObject), );

		//pObj.vScale = { 0.01f, 0.01f, 0.01f };
		//pObj.vAngle = { 0.f, 34.f, 0.f };
		//pObj.vPos = { 51.4f , 0.f, -80.9f };
		//pObj.tParentBone = tTemp.tParentBone;
		//pGameObject = CKnight::Create(m_pGraphicDev, pMornigStar, &pObj);
		//NULL_CHECK_RETURN(pGameObject, );
		//FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObj.tParentBone, pGameObject, this), );
		//setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eWall | FilterGroup::eMonster | FilterGroup::eGround);

		//m_iKnightCount++;
		//m_iKnightCountMax++;

		//tagTemp[MAX_PATH];
		//wsprintf(tagTemp, L"Field3_Room1ToyKnight %d", m_iKnightCount);
		//tagWeaponTemp[MAX_PATH];
		//wsprintf(tagWeaponTemp, L"Field3_Room1ToyKnight_MorningStar %d", m_iKnightCount);
		//tTemp.tParentBone = tagTemp;

		//pMornigStar = nullptr;
		//pGameObject = pMornigStar = CMorningStar::Create(m_pGraphicDev, &tTemp);
		//NULL_CHECK_RETURN(pGameObject, );
		//FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagWeaponTemp, pGameObject), );

		//pObj.vScale = { 0.01f, 0.01f, 0.01f };
		//pObj.vAngle = { 0.f, 56.f, 0.f };
		//pObj.vPos = { 51.4f , 0.f, -79.4f };
		//pObj.tParentBone = tTemp.tParentBone;
		//pGameObject = CKnight::Create(m_pGraphicDev, pMornigStar, &pObj);
		//NULL_CHECK_RETURN(pGameObject, );
		//FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObj.tParentBone, pGameObject, this), );
		//setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eWall | FilterGroup::eMonster | FilterGroup::eGround);

		//m_iKnightCount++;
		//m_iKnightCountMax++;
		////////////////////////////////////////////////////////////////////////////


		////2번룸
		//m_iKnightCount = 0;
		//m_iKnightCountMax = 0;

		//m_iSheilderCount = 0;
		//m_iSheilderCountMax = 0;

		//tagTemp[MAX_PATH];
		//wsprintf(tagTemp, L"Field3_Room2ToyKnight %d", m_iKnightCount);
		//tagWeaponTemp[MAX_PATH];
		//wsprintf(tagWeaponTemp, L"Field3_Room2ToyKnight_MorningStar %d", m_iKnightCount);
		//tTemp.tParentBone = tagTemp;

		//pMornigStar = nullptr;
		//pGameObject = pMornigStar = CMorningStar::Create(m_pGraphicDev, &tTemp);
		//NULL_CHECK_RETURN(pGameObject, );
		//FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagWeaponTemp, pGameObject), );

		//pObj.vScale = { 0.01f, 0.01f, 0.01f };
		//pObj.vAngle = { 0.f, 5.f, 0.f };
		//pObj.vPos = { 42.0f , 0.f, -91.f };
		//pObj.tParentBone = tTemp.tParentBone;
		//pGameObject = CKnight::Create(m_pGraphicDev, pMornigStar, &pObj);
		//NULL_CHECK_RETURN(pGameObject, );
		//FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObj.tParentBone, pGameObject, this), );
		//setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eWall | FilterGroup::eMonster | FilterGroup::eGround);

		//m_iKnightCount++;
		//m_iKnightCountMax++;

		//tagTemp[MAX_PATH];
		//wsprintf(tagTemp, L"Field3_Room2ToyKnight %d", m_iKnightCount);
		//tagWeaponTemp[MAX_PATH];
		//wsprintf(tagWeaponTemp, L"Field3_Room2ToyKnight_MorningStar %d", m_iKnightCount);
		//tTemp.tParentBone = tagTemp;

		//pMornigStar = nullptr;
		//pGameObject = pMornigStar = CMorningStar::Create(m_pGraphicDev, &tTemp);
		//NULL_CHECK_RETURN(pGameObject, );
		//FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagWeaponTemp, pGameObject), );

		//pObj.vScale = { 0.01f, 0.01f, 0.01f };
		//pObj.vAngle = { 0.f, 21.f, 0.f };
		//pObj.vPos = { 46.5f , 0.f, -91.f };
		//pObj.tParentBone = tTemp.tParentBone;
		//pGameObject = CKnight::Create(m_pGraphicDev, pMornigStar, &pObj);
		//NULL_CHECK_RETURN(pGameObject, );
		//FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObj.tParentBone, pGameObject, this), );
		//setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eWall | FilterGroup::eMonster | FilterGroup::eGround);

		//m_iKnightCount++;
		//m_iKnightCountMax++;


		//tagTemp[MAX_PATH];
		//wsprintf(tagTemp, L"Field3_Room2ToyKnight %d", m_iKnightCount);
		//tagWeaponTemp[MAX_PATH];
		//wsprintf(tagWeaponTemp, L"Field3_Room2ToyKnight_MorningStar %d", m_iKnightCount);
		//tTemp.tParentBone = tagTemp;

		//pMornigStar = nullptr;
		//pGameObject = pMornigStar = CMorningStar::Create(m_pGraphicDev, &tTemp);
		//NULL_CHECK_RETURN(pGameObject, );
		//FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagWeaponTemp, pGameObject), );

		//pObj.vScale = { 0.01f, 0.01f, 0.01f };
		//pObj.vAngle = { 0.f, 94.f, 0.f };
		//pObj.vPos = { 42.0f , 0.f, -94.f };
		//pObj.tParentBone = tTemp.tParentBone;
		//pGameObject = CKnight::Create(m_pGraphicDev, pMornigStar, &pObj);
		//NULL_CHECK_RETURN(pGameObject, );
		//FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObj.tParentBone, pGameObject, this), );
		//setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eWall | FilterGroup::eMonster | FilterGroup::eGround);

		//m_iKnightCount++;
		//m_iKnightCountMax++;

		//tagTemp[MAX_PATH];
		//wsprintf(tagTemp, L"Field3_Room2ToyKnight %d", m_iKnightCount);
		//tagWeaponTemp[MAX_PATH];
		//wsprintf(tagWeaponTemp, L"Field3_Room2ToyKnight_MorningStar %d", m_iKnightCount);
		//tTemp.tParentBone = tagTemp;

		//pMornigStar = nullptr;
		//pGameObject = pMornigStar = CMorningStar::Create(m_pGraphicDev, &tTemp);
		//NULL_CHECK_RETURN(pGameObject, );
		//FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagWeaponTemp, pGameObject), );

		//pObj.vScale = { 0.01f, 0.01f, 0.01f };
		//pObj.vAngle = { 0.f, 20.f, 0.f };
		//pObj.vPos = { 46.5f , 0.f, -94.f };
		//pObj.tParentBone = tTemp.tParentBone;
		//pGameObject = CKnight::Create(m_pGraphicDev, pMornigStar, &pObj);
		//NULL_CHECK_RETURN(pGameObject, );
		//FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObj.tParentBone, pGameObject, this), );
		//setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eWall | FilterGroup::eMonster | FilterGroup::eGround);

		//m_iKnightCount++;
		//m_iKnightCountMax++;

		//wsprintf(tagTemp, L"Field3_Room2ToySheilder %d", m_iSheilderCount);
		//tagWeaponTemp[MAX_PATH];
		//wsprintf(tagWeaponTemp, L"Field3_Room2ToySheilder_Sheild %d", m_iSheilderCount);
		//tTemp.tParentBone = tagTemp;

		//pObj.vScale = { 0.01f, 0.01f, 0.01f };
		//pObj.vAngle = { 0.f, 56.f, 0.f };
		//pObj.vPos = { 44.5f , 0.f, -92.f };
		//pObj.tParentBone = tTemp.tParentBone;

		//pSheild = nullptr;
		//pGameObject = pSheild = CSheild::Create(m_pGraphicDev, &tTemp);
		//NULL_CHECK_RETURN(pGameObject, );
		//FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagWeaponTemp, pGameObject), );

		//pGameObject = CSheilder::Create(m_pGraphicDev, pSheild, &pObj);
		//NULL_CHECK_RETURN(pGameObject, );
		//FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObj.tParentBone, pGameObject, this), );
		//setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eWall | FilterGroup::eMonster | FilterGroup::eGround);

		//m_iSheilderCount++;
		//m_iSheilderCountMax++;
		////////////////////////////////////////////////////////////////////////////

		////3번룸
		//m_iKnightCount = 0;
		//m_iKnightCountMax = 0;
		//m_iArbalistCount = 0;
		//m_iArbalistCountMax = 0;

		//tagTemp[MAX_PATH];
		//wsprintf(tagTemp, L"Field3_Room3ToyKnight %d", m_iKnightCount);
		//tagWeaponTemp[MAX_PATH];
		//wsprintf(tagWeaponTemp, L"Field3_Room3ToyKnight_MorningStar %d", m_iKnightCount);
		//tTemp.tParentBone = tagTemp;

		//pMornigStar = nullptr;
		//pGameObject = pMornigStar = CMorningStar::Create(m_pGraphicDev, &tTemp);
		//NULL_CHECK_RETURN(pGameObject, );
		//FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagWeaponTemp, pGameObject), );

		//pObj.vScale = { 0.01f, 0.01f, 0.01f };
		//pObj.vAngle = { 0.f, 50.f, 0.f };
		//pObj.vPos = { 32.7f , 0.f, -97.f };
		//pObj.tParentBone = tTemp.tParentBone;
		//pGameObject = CKnight::Create(m_pGraphicDev, pMornigStar, &pObj);
		//NULL_CHECK_RETURN(pGameObject, );
		//FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObj.tParentBone, pGameObject, this), );
		//setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eWall | FilterGroup::eMonster | FilterGroup::eGround);

		//m_iKnightCount++;
		//m_iKnightCountMax++;

		//tagTemp[MAX_PATH];
		//wsprintf(tagTemp, L"Field3_Room3ToyKnight %d", m_iKnightCount);
		//tagWeaponTemp[MAX_PATH];
		//wsprintf(tagWeaponTemp, L"Field3_Room3ToyKnight_MorningStar %d", m_iKnightCount);
		//tTemp.tParentBone = tagTemp;

		//pMornigStar = nullptr;
		//pGameObject = pMornigStar = CMorningStar::Create(m_pGraphicDev, &tTemp);
		//NULL_CHECK_RETURN(pGameObject, );
		//FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagWeaponTemp, pGameObject), );

		//pObj.vScale = { 0.01f, 0.01f, 0.01f };
		//pObj.vAngle = { 0.f, 35.f, 0.f };
		//pObj.vPos = { 32.7f , 0.f, -99.f };
		//pObj.tParentBone = tTemp.tParentBone;
		//pGameObject = CKnight::Create(m_pGraphicDev, pMornigStar, &pObj);
		//NULL_CHECK_RETURN(pGameObject, );
		//FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObj.tParentBone, pGameObject, this), );
		//setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eWall | FilterGroup::eMonster | FilterGroup::eGround);

		//m_iKnightCount++;
		//m_iKnightCountMax++;


		//wsprintf(tagTemp, L"Field3_Room3ToyArbalist %d", m_iArbalistCount);
		//wsprintf(tagWeaponTemp, L"Field3_Room3ToyArbalist_CrossBow %d", m_iArbalistCount);
		//tTemp.tParentBone = tagTemp;

		//pCrossBow = nullptr;
		//pGameObject = pCrossBow = CCrossBow::Create(m_pGraphicDev, &tTemp);
		//NULL_CHECK_RETURN(pGameObject, );
		//FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagWeaponTemp, pGameObject), );

		//pObj.vScale = { 0.015f, 0.015f, 0.015f };
		//pObj.vAngle = { 0.f, 0.f, 0.f };
		//pObj.vPos = { 31.5f , 0.f, -99.f };
		//pObj.tParentBone = tTemp.tParentBone;
		//pGameObject = CArbalist::Create(m_pGraphicDev, pCrossBow, &pObj);
		//NULL_CHECK_RETURN(pGameObject, );
		//FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObj.tParentBone, pGameObject, this), );
		//setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eWall | FilterGroup::eMonster);

		//m_iArbalistCount++;
		//m_iArbalistCountMax++;

		////////////////////////////////////////////////////////////////////////////


		////4번룸
		//m_iKnightCount = 0;
		//m_iKnightCountMax = 0;

		//tagTemp[MAX_PATH];
		//wsprintf(tagTemp, L"Field3_Room4ToyKnight %d", m_iKnightCount);
		//tagWeaponTemp[MAX_PATH];
		//wsprintf(tagWeaponTemp, L"Field3_Room4ToyKnight_MorningStar %d", m_iKnightCount);
		//tTemp.tParentBone = tagTemp;

		//pMornigStar = nullptr;
		//pGameObject = pMornigStar = CMorningStar::Create(m_pGraphicDev, &tTemp);
		//NULL_CHECK_RETURN(pGameObject, );
		//FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagWeaponTemp, pGameObject), );

		//pObj.vScale = { 0.01f, 0.01f, 0.01f };
		//pObj.vAngle = { 0.f, 87.f, 0.f };
		//pObj.vPos = { 31.9f , 0.f, -107.1f };
		//pObj.tParentBone = tTemp.tParentBone;
		//pGameObject = CKnight::Create(m_pGraphicDev, pMornigStar, &pObj);
		//NULL_CHECK_RETURN(pGameObject, );
		//FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObj.tParentBone, pGameObject, this), );
		//setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eWall | FilterGroup::eMonster | FilterGroup::eGround);

		//m_iKnightCount++;
		//m_iKnightCountMax++;

		//tagTemp[MAX_PATH];
		//wsprintf(tagTemp, L"Field3_Room4ToyKnight %d", m_iKnightCount);
		//tagWeaponTemp[MAX_PATH];
		//wsprintf(tagWeaponTemp, L"Field3_Room4ToyKnight_MorningStar %d", m_iKnightCount);
		//tTemp.tParentBone = tagTemp;

		//pMornigStar = nullptr;
		//pGameObject = pMornigStar = CMorningStar::Create(m_pGraphicDev, &tTemp);
		//NULL_CHECK_RETURN(pGameObject, );
		//FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagWeaponTemp, pGameObject), );

		//pObj.vScale = { 0.01f, 0.01f, 0.01f };
		//pObj.vAngle = { 0.f, 45.f, 0.f };
		//pObj.vPos = { 31.9f , 0.f, -109.1f };
		//pObj.tParentBone = tTemp.tParentBone;
		//pGameObject = CKnight::Create(m_pGraphicDev, pMornigStar, &pObj);
		//NULL_CHECK_RETURN(pGameObject, );
		//FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObj.tParentBone, pGameObject, this), );
		//setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eWall | FilterGroup::eMonster | FilterGroup::eGround);

		//m_iKnightCount++;
		//m_iKnightCountMax++;

		//tagTemp[MAX_PATH];
		//wsprintf(tagTemp, L"Field3_Room4ToyKnight %d", m_iKnightCount);
		//tagWeaponTemp[MAX_PATH];
		//wsprintf(tagWeaponTemp, L"Field3_Room4ToyKnight_MorningStar %d", m_iKnightCount);
		//tTemp.tParentBone = tagTemp;

		//pMornigStar = nullptr;
		//pGameObject = pMornigStar = CMorningStar::Create(m_pGraphicDev, &tTemp);
		//NULL_CHECK_RETURN(pGameObject, );
		//FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagWeaponTemp, pGameObject), );

		//pObj.vScale = { 0.01f, 0.01f, 0.01f };
		//pObj.vAngle = { 0.f, 15.f, 0.f };
		//pObj.vPos = { 31.9f , 0.f, -105.1f };
		//pObj.tParentBone = tTemp.tParentBone;
		//pGameObject = CKnight::Create(m_pGraphicDev, pMornigStar, &pObj);
		//NULL_CHECK_RETURN(pGameObject, );
		//FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObj.tParentBone, pGameObject, this), );
		//setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eWall | FilterGroup::eMonster | FilterGroup::eGround);

		//m_iKnightCount++;
		//m_iKnightCountMax++;

		//tagTemp[MAX_PATH];
		//wsprintf(tagTemp, L"Field3_Room4ToyKnight %d", m_iKnightCount);
		//tagWeaponTemp[MAX_PATH];
		//wsprintf(tagWeaponTemp, L"Field3_Room4ToyKnight_MorningStar %d", m_iKnightCount);
		//tTemp.tParentBone = tagTemp;

		//pMornigStar = nullptr;
		//pGameObject = pMornigStar = CMorningStar::Create(m_pGraphicDev, &tTemp);
		//NULL_CHECK_RETURN(pGameObject, );
		//FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagWeaponTemp, pGameObject), );

		//pObj.vScale = { 0.01f, 0.01f, 0.01f };
		//pObj.vAngle = { 0.f, 22.f, 0.f };
		//pObj.vPos = { 33.4f , 0.f, -107.1f };
		//pObj.tParentBone = tTemp.tParentBone;
		//pGameObject = CKnight::Create(m_pGraphicDev, pMornigStar, &pObj);
		//NULL_CHECK_RETURN(pGameObject, );
		//FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObj.tParentBone, pGameObject, this), );
		//setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eWall | FilterGroup::eMonster | FilterGroup::eGround);

		//m_iKnightCount++;
		//m_iKnightCountMax++;

		//tagTemp[MAX_PATH];
		//wsprintf(tagTemp, L"Field3_Room4ToyKnight %d", m_iKnightCount);
		//tagWeaponTemp[MAX_PATH];
		//wsprintf(tagWeaponTemp, L"Field3_Room4ToyKnight_MorningStar %d", m_iKnightCount);
		//tTemp.tParentBone = tagTemp;

		//pMornigStar = nullptr;
		//pGameObject = pMornigStar = CMorningStar::Create(m_pGraphicDev, &tTemp);
		//NULL_CHECK_RETURN(pGameObject, );
		//FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagWeaponTemp, pGameObject), );

		//pObj.vScale = { 0.01f, 0.01f, 0.01f };
		//pObj.vAngle = { 0.f, 66.f, 0.f };
		//pObj.vPos = { 33.4f , 0.f, -105.1f };
		//pObj.tParentBone = tTemp.tParentBone;
		//pGameObject = CKnight::Create(m_pGraphicDev, pMornigStar, &pObj);
		//NULL_CHECK_RETURN(pGameObject, );
		//FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObj.tParentBone, pGameObject, this), );
		//setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eWall | FilterGroup::eMonster | FilterGroup::eGround);

		//m_iKnightCount++;
		//m_iKnightCountMax++;

		////////////////////////////////////////////////////////////////////////////

		////5번룸
		//m_iKnightCount = 0;
		//m_iKnightCountMax = 0;

		//tagTemp[MAX_PATH];
		//wsprintf(tagTemp, L"Field3_Room5ToyKnight %d", m_iKnightCount);
		//tagWeaponTemp[MAX_PATH];
		//wsprintf(tagWeaponTemp, L"Field3_Room5ToyKnight_MorningStar %d", m_iKnightCount);
		//tTemp.tParentBone = tagTemp;

		//pMornigStar = nullptr;
		//pGameObject = pMornigStar = CMorningStar::Create(m_pGraphicDev, &tTemp);
		//NULL_CHECK_RETURN(pGameObject, );
		//FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagWeaponTemp, pGameObject), );

		//pObj.vScale = { 0.01f, 0.01f, 0.01f };
		//pObj.vAngle = { 0.f, 61.f, 0.f };
		//pObj.vPos = { 56.2f , 0.f, -105.1f };
		//pObj.tParentBone = tTemp.tParentBone;
		//pGameObject = CKnight::Create(m_pGraphicDev, pMornigStar, &pObj);
		//NULL_CHECK_RETURN(pGameObject, );
		//FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObj.tParentBone, pGameObject, this), );
		//setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eWall | FilterGroup::eMonster | FilterGroup::eGround);

		//m_iKnightCount++;
		//m_iKnightCountMax++;

		//tagTemp[MAX_PATH];
		//wsprintf(tagTemp, L"Field3_Room5ToyKnight %d", m_iKnightCount);
		//tagWeaponTemp[MAX_PATH];
		//wsprintf(tagWeaponTemp, L"Field3_Room5ToyKnight_MorningStar %d", m_iKnightCount);
		//tTemp.tParentBone = tagTemp;

		//pMornigStar = nullptr;
		//pGameObject = pMornigStar = CMorningStar::Create(m_pGraphicDev, &tTemp);
		//NULL_CHECK_RETURN(pGameObject, );
		//FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagWeaponTemp, pGameObject), );

		//pObj.vScale = { 0.01f, 0.01f, 0.01f };
		//pObj.vAngle = { 0.f, 57.f, 0.f };
		//pObj.vPos = { 56.2f , 0.f, -105.1f };
		//pObj.tParentBone = tTemp.tParentBone;
		//pGameObject = CKnight::Create(m_pGraphicDev, pMornigStar, &pObj);
		//NULL_CHECK_RETURN(pGameObject, );
		//FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObj.tParentBone, pGameObject, this), );
		//setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eWall | FilterGroup::eMonster | FilterGroup::eGround);

		//m_iKnightCount++;
		//m_iKnightCountMax++;

		//tagTemp[MAX_PATH];
		//wsprintf(tagTemp, L"Field3_Room5ToyKnight %d", m_iKnightCount);
		//tagWeaponTemp[MAX_PATH];
		//wsprintf(tagWeaponTemp, L"Field3_Room5ToyKnight_MorningStar %d", m_iKnightCount);
		//tTemp.tParentBone = tagTemp;

		//pMornigStar = nullptr;
		//pGameObject = pMornigStar = CMorningStar::Create(m_pGraphicDev, &tTemp);
		//NULL_CHECK_RETURN(pGameObject, );
		//FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagWeaponTemp, pGameObject), );

		//pObj.vScale = { 0.01f, 0.01f, 0.01f };
		//pObj.vAngle = { 0.f, 26.f, 0.f };
		//pObj.vPos = { 56.2f , 0.f, -103.7f };
		//pObj.tParentBone = tTemp.tParentBone;
		//pGameObject = CKnight::Create(m_pGraphicDev, pMornigStar, &pObj);
		//NULL_CHECK_RETURN(pGameObject, );
		//FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObj.tParentBone, pGameObject, this), );
		//setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eWall | FilterGroup::eMonster | FilterGroup::eGround);

		//m_iKnightCount++;
		//m_iKnightCountMax++;


		//tagTemp[MAX_PATH];
		//wsprintf(tagTemp, L"Field3_Room5ToyKnight %d", m_iKnightCount);
		//tagWeaponTemp[MAX_PATH];
		//wsprintf(tagWeaponTemp, L"Field3_Room5ToyKnight_MorningStar %d", m_iKnightCount);
		//tTemp.tParentBone = tagTemp;

		//pMornigStar = nullptr;
		//pGameObject = pMornigStar = CMorningStar::Create(m_pGraphicDev, &tTemp);
		//NULL_CHECK_RETURN(pGameObject, );
		//FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagWeaponTemp, pGameObject), );

		//pObj.vScale = { 0.01f, 0.01f, 0.01f };
		//pObj.vAngle = { 0.f, 41.f, 0.f };
		//pObj.vPos = { 54.7f , 0.f, -105.1f };
		//pObj.tParentBone = tTemp.tParentBone;
		//pGameObject = CKnight::Create(m_pGraphicDev, pMornigStar, &pObj);
		//NULL_CHECK_RETURN(pGameObject, );
		//FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObj.tParentBone, pGameObject, this), );
		//setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eWall | FilterGroup::eMonster | FilterGroup::eGround);

		//m_iKnightCount++;
		//m_iKnightCountMax++;

		//tagTemp[MAX_PATH];
		//wsprintf(tagTemp, L"Field3_Room5ToyKnight %d", m_iKnightCount);
		//tagWeaponTemp[MAX_PATH];
		//wsprintf(tagWeaponTemp, L"Field3_Room5ToyKnight_MorningStar %d", m_iKnightCount);
		//tTemp.tParentBone = tagTemp;

		//pMornigStar = nullptr;
		//pGameObject = pMornigStar = CMorningStar::Create(m_pGraphicDev, &tTemp);
		//NULL_CHECK_RETURN(pGameObject, );
		//FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagWeaponTemp, pGameObject), );

		//pObj.vScale = { 0.01f, 0.01f, 0.01f };
		//pObj.vAngle = { 0.f, 87.f, 0.f };
		//pObj.vPos = { 54.7f , 0.f, -106.6f };
		//pObj.tParentBone = tTemp.tParentBone;
		//pGameObject = CKnight::Create(m_pGraphicDev, pMornigStar, &pObj);
		//NULL_CHECK_RETURN(pGameObject, );
		//FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObj.tParentBone, pGameObject, this), );
		//setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eWall | FilterGroup::eMonster | FilterGroup::eGround);

		//m_iKnightCount++;
		//m_iKnightCountMax++;
		////////////////////////////////////////////////////////////////////////////

		////6번룸

		//m_iKnightCount = 0;
		//m_iKnightCountMax = 0;
		//m_iArbalistCount = 0;
		//m_iArbalistCountMax = 0;

		//tagTemp[MAX_PATH];
		//wsprintf(tagTemp, L"Field3_Room6ToyKnight %d", m_iKnightCount);
		//tagWeaponTemp[MAX_PATH];
		//wsprintf(tagWeaponTemp, L"Field3_Room6ToyKnight_MorningStar %d", m_iKnightCount);
		//tTemp.tParentBone = tagTemp;

		//pMornigStar = nullptr;
		//pGameObject = pMornigStar = CMorningStar::Create(m_pGraphicDev, &tTemp);
		//NULL_CHECK_RETURN(pGameObject, );
		//FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagWeaponTemp, pGameObject), );

		//pObj.vScale = { 0.01f, 0.01f, 0.01f };
		//pObj.vAngle = { 0.f, 15.f, 0.f };
		//pObj.vPos = { 52.1f , 0.f, -113.f };
		//pObj.tParentBone = tTemp.tParentBone;
		//pGameObject = CKnight::Create(m_pGraphicDev, pMornigStar, &pObj);
		//NULL_CHECK_RETURN(pGameObject, );
		//FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObj.tParentBone, pGameObject, this), );
		//setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eWall | FilterGroup::eMonster | FilterGroup::eGround);

		//m_iKnightCount++;
		//m_iKnightCountMax++;

		//tagTemp[MAX_PATH];
		//wsprintf(tagTemp, L"Field3_Room6ToyKnight %d", m_iKnightCount);
		//tagWeaponTemp[MAX_PATH];
		//wsprintf(tagWeaponTemp, L"Field3_Room6ToyKnight_MorningStar %d", m_iKnightCount);
		//tTemp.tParentBone = tagTemp;

		//pMornigStar = nullptr;
		//pGameObject = pMornigStar = CMorningStar::Create(m_pGraphicDev, &tTemp);
		//NULL_CHECK_RETURN(pGameObject, );
		//FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagWeaponTemp, pGameObject), );

		//pObj.vScale = { 0.01f, 0.01f, 0.01f };
		//pObj.vAngle = { 0.f, 35.f, 0.f };
		//pObj.vPos = { 52.1f , 0.f, -115.f };
		//pObj.tParentBone = tTemp.tParentBone;
		//pGameObject = CKnight::Create(m_pGraphicDev, pMornigStar, &pObj);
		//NULL_CHECK_RETURN(pGameObject, );
		//FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObj.tParentBone, pGameObject, this), );
		//setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eWall | FilterGroup::eMonster | FilterGroup::eGround);

		//m_iKnightCount++;
		//m_iKnightCountMax++;

		//tagTemp[MAX_PATH];
		//wsprintf(tagTemp, L"Field3_Room6ToyKnight %d", m_iKnightCount);
		//tagWeaponTemp[MAX_PATH];
		//wsprintf(tagWeaponTemp, L"Field3_Room6ToyKnight_MorningStar %d", m_iKnightCount);
		//tTemp.tParentBone = tagTemp;

		//pMornigStar = nullptr;
		//pGameObject = pMornigStar = CMorningStar::Create(m_pGraphicDev, &tTemp);
		//NULL_CHECK_RETURN(pGameObject, );
		//FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagWeaponTemp, pGameObject), );

		//pObj.vScale = { 0.01f, 0.01f, 0.01f };
		//pObj.vAngle = { 0.f, 24.f, 0.f };
		//pObj.vPos = { 52.1f , 0.f, -117.f };
		//pObj.tParentBone = tTemp.tParentBone;
		//pGameObject = CKnight::Create(m_pGraphicDev, pMornigStar, &pObj);
		//NULL_CHECK_RETURN(pGameObject, );
		//FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObj.tParentBone, pGameObject, this), );
		//setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eWall | FilterGroup::eMonster | FilterGroup::eGround);

		//m_iKnightCount++;
		//m_iKnightCountMax++;

		//tagTemp[MAX_PATH];
		//wsprintf(tagTemp, L"Field3_Room6ToyKnight %d", m_iKnightCount);
		//tagWeaponTemp[MAX_PATH];
		//wsprintf(tagWeaponTemp, L"Field3_Room6ToyKnight_MorningStar %d", m_iKnightCount);
		//tTemp.tParentBone = tagTemp;

		//pMornigStar = nullptr;
		//pGameObject = pMornigStar = CMorningStar::Create(m_pGraphicDev, &tTemp);
		//NULL_CHECK_RETURN(pGameObject, );
		//FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagWeaponTemp, pGameObject), );

		//pObj.vScale = { 0.01f, 0.01f, 0.01f };
		//pObj.vAngle = { 0.f, 67.f, 0.f };
		//pObj.vPos = { 50.5f , 0.f, -115.f };
		//pObj.tParentBone = tTemp.tParentBone;
		//pGameObject = CKnight::Create(m_pGraphicDev, pMornigStar, &pObj);
		//NULL_CHECK_RETURN(pGameObject, );
		//FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObj.tParentBone, pGameObject, this), );
		//setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eWall | FilterGroup::eMonster | FilterGroup::eGround);

		//m_iKnightCount++;
		//m_iKnightCountMax++;

		//tagTemp[MAX_PATH];
		//wsprintf(tagTemp, L"Field3_Room6ToyKnight %d", m_iKnightCount);
		//tagWeaponTemp[MAX_PATH];
		//wsprintf(tagWeaponTemp, L"Field3_Room6ToyKnight_MorningStar %d", m_iKnightCount);
		//tTemp.tParentBone = tagTemp;

		//pMornigStar = nullptr;
		//pGameObject = pMornigStar = CMorningStar::Create(m_pGraphicDev, &tTemp);
		//NULL_CHECK_RETURN(pGameObject, );
		//FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagWeaponTemp, pGameObject), );

		//pObj.vScale = { 0.01f, 0.01f, 0.01f };
		//pObj.vAngle = { 0.f, 41.f, 0.f };
		//pObj.vPos = { 50.5f , 0.f, -117.f };
		//pObj.tParentBone = tTemp.tParentBone;
		//pGameObject = CKnight::Create(m_pGraphicDev, pMornigStar, &pObj);
		//NULL_CHECK_RETURN(pGameObject, );
		//FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObj.tParentBone, pGameObject, this), );
		//setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eWall | FilterGroup::eMonster | FilterGroup::eGround);

		//m_iKnightCount++;
		//m_iKnightCountMax++;


		//wsprintf(tagTemp, L"Field3_Room6ToyArbalist %d", m_iArbalistCount);
		//wsprintf(tagWeaponTemp, L"Field3_Room6ToyArbalist_CrossBow %d", m_iArbalistCount);
		//tTemp.tParentBone = tagTemp;

		//pCrossBow = nullptr;
		//pGameObject = pCrossBow = CCrossBow::Create(m_pGraphicDev, &tTemp);
		//NULL_CHECK_RETURN(pGameObject, );
		//FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagWeaponTemp, pGameObject), );

		//pObj.vScale = { 0.015f, 0.015f, 0.015f };
		//pObj.vAngle = { 0.f, 0.f, 0.f };
		//pObj.vPos = { 53.5f , 0.f, -118.f };
		//pObj.tParentBone = tTemp.tParentBone;
		//pGameObject = CArbalist::Create(m_pGraphicDev, pCrossBow, &pObj);
		//NULL_CHECK_RETURN(pGameObject, );
		//FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObj.tParentBone, pGameObject, this), );
		//setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eWall | FilterGroup::eMonster);

		//m_iArbalistCount++;
		//m_iArbalistCountMax++;

		//wsprintf(tagTemp, L"Field3_Room6ToyArbalist %d", m_iArbalistCount);
		//wsprintf(tagWeaponTemp, L"Field3_Room6ToyArbalist_CrossBow %d", m_iArbalistCount);
		//tTemp.tParentBone = tagTemp;

		//pCrossBow = nullptr;
		//pGameObject = pCrossBow = CCrossBow::Create(m_pGraphicDev, &tTemp);
		//NULL_CHECK_RETURN(pGameObject, );
		//FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagWeaponTemp, pGameObject), );

		//pObj.vScale = { 0.015f, 0.015f, 0.015f };
		//pObj.vAngle = { 0.f, 0.f, 0.f };
		//pObj.vPos = { 55.7f , 0.f, -116.f };
		//pObj.tParentBone = tTemp.tParentBone;
		//pGameObject = CArbalist::Create(m_pGraphicDev, pCrossBow, &pObj);
		//NULL_CHECK_RETURN(pGameObject, );
		//FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObj.tParentBone, pGameObject, this), );
		//setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eWall | FilterGroup::eMonster);

		//m_iArbalistCount++;
		//m_iArbalistCountMax++;
		//////////////////////////////////////////////////////////////////////////
	}

	CGameObject*	pMap = Engine::Get_GameObject(L"GameObject", L"ChessField_Dark2");
	if (pMap)
		pMap->Set_Dead();

	pMap = Engine::Get_GameObject(L"GameObject", L"ChessField_Dark3");
	if (pMap)
		pMap->Set_Dead();

	pMap = Engine::Get_GameObject(L"GameObject", L"ChessField_Dark4");
	if (pMap)
		pMap->Set_Dead();

	pMap = Engine::Get_GameObject(L"GameObject", L"ChessField_Dark10");
	if (pMap)
		pMap->Set_Dead();

	m_pTrigger[TRIGGER_CHESS_FIELD3_CREATE]->Set_Interactable(false);
}

void CChessField::Event_Field3_Room1Move(PxTriggerPair * pairs)
{
	if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
	{
		CGameObject*	pMap = Engine::Get_GameObject(L"GameObject", L"ChessField_Dark5");
		if (pMap)
		{
			Engine::CLayer*				pLayer = Engine::Get_Layer(L"GameObject");
			Engine::CGameObject*		pGameObject = nullptr;

			OutputDebugString(L"세번째 필드 몬스터 생성\n");

			_tchar tagTemp[MAX_PATH];
			_tchar tagWeaponTemp[MAX_PATH];
			OBJINFO tTemp;
			OBJINFO pObj;
			CMorningStar* pMornigStar = nullptr;
			CCrossBow* pCrossBow = nullptr;
			CSheild* pSheild = nullptr;

			m_iKnightCount = 0;
			m_iKnightCountMax = 0;

			m_iArbalistCount = 0;
			m_iArbalistCountMax = 0;
			//1번룸
			wsprintf(tagTemp, L"Field3_Room1ToyArbalist %d", m_iArbalistCount);
			wsprintf(tagWeaponTemp, L"Field3_Room1ToyArbalist_CrossBow %d", m_iArbalistCount);
			tTemp.tParentBone = tagTemp;

			pCrossBow = nullptr;
			pGameObject = pCrossBow = CCrossBow::Create(m_pGraphicDev, &tTemp);
			NULL_CHECK_RETURN(pGameObject, );
			FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagWeaponTemp, pGameObject), );

			pObj.vScale = { 0.015f, 0.015f, 0.015f };
			pObj.vAngle = { 0.f, 155.f, 0.f };
			pObj.vPos = { 54.4f , 0.f, -77.4f };
			pObj.tParentBone = tTemp.tParentBone;
			pGameObject = CArbalist::Create(m_pGraphicDev, pCrossBow, &pObj);
			NULL_CHECK_RETURN(pGameObject, );
			FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObj.tParentBone, pGameObject, this), );

			setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eDefaultTrigger);



			m_iArbalistCount++;
			m_iArbalistCountMax++;

			tagTemp[MAX_PATH];
			wsprintf(tagTemp, L"Field3_Room1ToyKnight %d", m_iKnightCount);
			tagWeaponTemp[MAX_PATH];
			wsprintf(tagWeaponTemp, L"Field3_Room1ToyKnight_MorningStar %d", m_iKnightCount);
			tTemp.tParentBone = tagTemp;

			pMornigStar = nullptr;
			pGameObject = pMornigStar = CMorningStar::Create(m_pGraphicDev, &tTemp);
			NULL_CHECK_RETURN(pGameObject, );
			FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagWeaponTemp, pGameObject), );

			pObj.vScale = { 0.01f, 0.01f, 0.01f };
			pObj.vAngle = { 0.f, 68.f, 0.f };
			pObj.vPos = { 52.9f , 0.f, -79.4f };
			pObj.tParentBone = tTemp.tParentBone;
			pGameObject = CKnight::Create(m_pGraphicDev, pMornigStar, &pObj);
			NULL_CHECK_RETURN(pGameObject, );
			FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObj.tParentBone, pGameObject, this), );
			setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eDefaultTrigger | FilterGroup::eWall | FilterGroup::eMonster | FilterGroup::eGround);

			m_iKnightCount++;
			m_iKnightCountMax++;

			tagTemp[MAX_PATH];
			wsprintf(tagTemp, L"Field3_Room1ToyKnight %d", m_iKnightCount);
			tagWeaponTemp[MAX_PATH];
			wsprintf(tagWeaponTemp, L"Field3_Room1ToyKnight_MorningStar %d", m_iKnightCount);
			tTemp.tParentBone = tagTemp;

			pMornigStar = nullptr;
			pGameObject = pMornigStar = CMorningStar::Create(m_pGraphicDev, &tTemp);
			NULL_CHECK_RETURN(pGameObject, );
			FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagWeaponTemp, pGameObject), );

			pObj.vScale = { 0.01f, 0.01f, 0.01f };
			pObj.vAngle = { 0.f, 48.f, 0.f };
			pObj.vPos = { 52.9f , 0.f, -80.9f };
			pObj.tParentBone = tTemp.tParentBone;
			pGameObject = CKnight::Create(m_pGraphicDev, pMornigStar, &pObj);
			NULL_CHECK_RETURN(pGameObject, );
			FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObj.tParentBone, pGameObject, this), );
			setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eDefaultTrigger | FilterGroup::eWall | FilterGroup::eMonster | FilterGroup::eGround);

			m_iKnightCount++;
			m_iKnightCountMax++;

			tagTemp[MAX_PATH];
			wsprintf(tagTemp, L"Field3_Room1ToyKnight %d", m_iKnightCount);
			tagWeaponTemp[MAX_PATH];
			wsprintf(tagWeaponTemp, L"Field3_Room1ToyKnight_MorningStar %d", m_iKnightCount);
			tTemp.tParentBone = tagTemp;

			pMornigStar = nullptr;
			pGameObject = pMornigStar = CMorningStar::Create(m_pGraphicDev, &tTemp);
			NULL_CHECK_RETURN(pGameObject, );
			FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagWeaponTemp, pGameObject), );

			pObj.vScale = { 0.01f, 0.01f, 0.01f };
			pObj.vAngle = { 0.f, 34.f, 0.f };
			pObj.vPos = { 51.4f , 0.f, -80.9f };
			pObj.tParentBone = tTemp.tParentBone;
			pGameObject = CKnight::Create(m_pGraphicDev, pMornigStar, &pObj);
			NULL_CHECK_RETURN(pGameObject, );
			FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObj.tParentBone, pGameObject, this), );
			setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eDefaultTrigger | FilterGroup::eWall | FilterGroup::eMonster | FilterGroup::eGround);

			m_iKnightCount++;
			m_iKnightCountMax++;

			tagTemp[MAX_PATH];
			wsprintf(tagTemp, L"Field3_Room1ToyKnight %d", m_iKnightCount);
			tagWeaponTemp[MAX_PATH];
			wsprintf(tagWeaponTemp, L"Field3_Room1ToyKnight_MorningStar %d", m_iKnightCount);
			tTemp.tParentBone = tagTemp;

			pMornigStar = nullptr;
			pGameObject = pMornigStar = CMorningStar::Create(m_pGraphicDev, &tTemp);
			NULL_CHECK_RETURN(pGameObject, );
			FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagWeaponTemp, pGameObject), );

			pObj.vScale = { 0.01f, 0.01f, 0.01f };
			pObj.vAngle = { 0.f, 56.f, 0.f };
			pObj.vPos = { 51.4f , 0.f, -79.4f };
			pObj.tParentBone = tTemp.tParentBone;
			pGameObject = CKnight::Create(m_pGraphicDev, pMornigStar, &pObj);
			NULL_CHECK_RETURN(pGameObject, );
			FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObj.tParentBone, pGameObject, this), );
			setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eDefaultTrigger | FilterGroup::eWall | FilterGroup::eMonster | FilterGroup::eGround);

			m_iKnightCount++;
			m_iKnightCountMax++;

			pMap->Set_Dead();
		}
		OutputDebugString(L"Field3 1번방 몬스터 움직이는 트리거\n");

		for (_uint i = 0; i < 4; ++i)
		{
			_tchar tagTemp[MAX_PATH];
			wsprintf(tagTemp, L"Field3_Room1ToyKnight %d", i);

			CKnight*	pKnight = (CKnight*)Engine::Get_GameObject(L"GameObject", tagTemp);
			if (nullptr == pKnight)
				return;

			pKnight->Set_Move(true);
		}

		for (_uint i = 0; i < 1; ++i)
		{
			_tchar tagTemp[MAX_PATH];
			wsprintf(tagTemp, L"Field3_Room1ToyArbalist %d", i);

			CArbalist*	pArbalist = (CArbalist*)Engine::Get_GameObject(L"GameObject", tagTemp);
			if (nullptr == pArbalist)
				return;

			pArbalist->Set_Move(true);
		}


	}
	m_pTrigger[TRIGGER_CHESS_FIELD3_MOVEROOM1]->Set_Interactable(false);
}

void CChessField::Event_Field3_Room2Move(PxTriggerPair * pairs)
{
	if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
	{
		Engine::CLayer*				pLayer = Engine::Get_Layer(L"GameObject");
		Engine::CGameObject*		pGameObject = nullptr;

		OutputDebugString(L"세번째 필드 몬스터 생성\n");

		_tchar tagTemp[MAX_PATH];
		_tchar tagWeaponTemp[MAX_PATH];
		OBJINFO tTemp;
		OBJINFO pObj;
		CMorningStar* pMornigStar = nullptr;
		CCrossBow* pCrossBow = nullptr;
		CSheild* pSheild = nullptr;

		CGameObject*	pMap = Engine::Get_GameObject(L"GameObject", L"ChessField_Dark6");
		if (pMap)
		{
			m_iKnightCount = 0;
			m_iKnightCountMax = 0;

			m_iSheilderCount = 0;
			m_iSheilderCountMax = 0;

			tagTemp[MAX_PATH];
			wsprintf(tagTemp, L"Field3_Room2ToyKnight %d", m_iKnightCount);
			tagWeaponTemp[MAX_PATH];
			wsprintf(tagWeaponTemp, L"Field3_Room2ToyKnight_MorningStar %d", m_iKnightCount);
			tTemp.tParentBone = tagTemp;

			pMornigStar = nullptr;
			pGameObject = pMornigStar = CMorningStar::Create(m_pGraphicDev, &tTemp);
			NULL_CHECK_RETURN(pGameObject, );
			FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagWeaponTemp, pGameObject), );

			pObj.vScale = { 0.01f, 0.01f, 0.01f };
			pObj.vAngle = { 0.f, 5.f, 0.f };
			pObj.vPos = { 42.0f , 0.f, -91.f };
			pObj.tParentBone = tTemp.tParentBone;
			pGameObject = CKnight::Create(m_pGraphicDev, pMornigStar, &pObj);
			NULL_CHECK_RETURN(pGameObject, );
			FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObj.tParentBone, pGameObject, this), );
			setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eDefaultTrigger | FilterGroup::eWall | FilterGroup::eMonster | FilterGroup::eGround);

			m_iKnightCount++;
			m_iKnightCountMax++;

			tagTemp[MAX_PATH];
			wsprintf(tagTemp, L"Field3_Room2ToyKnight %d", m_iKnightCount);
			tagWeaponTemp[MAX_PATH];
			wsprintf(tagWeaponTemp, L"Field3_Room2ToyKnight_MorningStar %d", m_iKnightCount);
			tTemp.tParentBone = tagTemp;

			pMornigStar = nullptr;
			pGameObject = pMornigStar = CMorningStar::Create(m_pGraphicDev, &tTemp);
			NULL_CHECK_RETURN(pGameObject, );
			FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagWeaponTemp, pGameObject), );

			pObj.vScale = { 0.01f, 0.01f, 0.01f };
			pObj.vAngle = { 0.f, 21.f, 0.f };
			pObj.vPos = { 46.5f , 0.f, -91.f };
			pObj.tParentBone = tTemp.tParentBone;
			pGameObject = CKnight::Create(m_pGraphicDev, pMornigStar, &pObj);
			NULL_CHECK_RETURN(pGameObject, );
			FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObj.tParentBone, pGameObject, this), );
			setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eDefaultTrigger | FilterGroup::eWall | FilterGroup::eMonster | FilterGroup::eGround);

			m_iKnightCount++;
			m_iKnightCountMax++;


			tagTemp[MAX_PATH];
			wsprintf(tagTemp, L"Field3_Room2ToyKnight %d", m_iKnightCount);
			tagWeaponTemp[MAX_PATH];
			wsprintf(tagWeaponTemp, L"Field3_Room2ToyKnight_MorningStar %d", m_iKnightCount);
			tTemp.tParentBone = tagTemp;

			pMornigStar = nullptr;
			pGameObject = pMornigStar = CMorningStar::Create(m_pGraphicDev, &tTemp);
			NULL_CHECK_RETURN(pGameObject, );
			FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagWeaponTemp, pGameObject), );

			pObj.vScale = { 0.01f, 0.01f, 0.01f };
			pObj.vAngle = { 0.f, 94.f, 0.f };
			pObj.vPos = { 42.0f , 0.f, -94.f };
			pObj.tParentBone = tTemp.tParentBone;
			pGameObject = CKnight::Create(m_pGraphicDev, pMornigStar, &pObj);
			NULL_CHECK_RETURN(pGameObject, );
			FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObj.tParentBone, pGameObject, this), );
			setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eDefaultTrigger | FilterGroup::eWall | FilterGroup::eMonster | FilterGroup::eGround);

			m_iKnightCount++;
			m_iKnightCountMax++;

			tagTemp[MAX_PATH];
			wsprintf(tagTemp, L"Field3_Room2ToyKnight %d", m_iKnightCount);
			tagWeaponTemp[MAX_PATH];
			wsprintf(tagWeaponTemp, L"Field3_Room2ToyKnight_MorningStar %d", m_iKnightCount);
			tTemp.tParentBone = tagTemp;

			pMornigStar = nullptr;
			pGameObject = pMornigStar = CMorningStar::Create(m_pGraphicDev, &tTemp);
			NULL_CHECK_RETURN(pGameObject, );
			FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagWeaponTemp, pGameObject), );

			pObj.vScale = { 0.01f, 0.01f, 0.01f };
			pObj.vAngle = { 0.f, 20.f, 0.f };
			pObj.vPos = { 46.5f , 0.f, -94.f };
			pObj.tParentBone = tTemp.tParentBone;
			pGameObject = CKnight::Create(m_pGraphicDev, pMornigStar, &pObj);
			NULL_CHECK_RETURN(pGameObject, );
			FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObj.tParentBone, pGameObject, this), );
			setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eDefaultTrigger | FilterGroup::eWall | FilterGroup::eMonster | FilterGroup::eGround);

			m_iKnightCount++;
			m_iKnightCountMax++;

			wsprintf(tagTemp, L"Field3_Room2ToySheilder %d", m_iSheilderCount);
			tagWeaponTemp[MAX_PATH];
			wsprintf(tagWeaponTemp, L"Field3_Room2ToySheilder_Sheild %d", m_iSheilderCount);
			tTemp.tParentBone = tagTemp;

			pObj.vScale = { 0.01f, 0.01f, 0.01f };
			pObj.vAngle = { 0.f, 56.f, 0.f };
			pObj.vPos = { 44.5f , 0.f, -92.f };
			pObj.tParentBone = tTemp.tParentBone;

			pSheild = nullptr;
			pGameObject = pSheild = CSheild::Create(m_pGraphicDev, &tTemp);
			NULL_CHECK_RETURN(pGameObject, );
			FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagWeaponTemp, pGameObject), );

			pGameObject = CSheilder::Create(m_pGraphicDev, pSheild, &pObj);
			NULL_CHECK_RETURN(pGameObject, );
			FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObj.tParentBone, pGameObject, this), );
			setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster,FilterGroup::eDefaultTrigger| FilterGroup::eWall | FilterGroup::eMonster | FilterGroup::eGround);

			m_iSheilderCount++;
			m_iSheilderCountMax++;

			pMap->Set_Dead();
		}

		pMap = Engine::Get_GameObject(L"GameObject", L"ChessField_Dark7");
		
		if (pMap)
		{
			m_iKnightCount = 0;
			m_iKnightCountMax = 0;
			m_iArbalistCount = 0;
			m_iArbalistCountMax = 0;

			tagTemp[MAX_PATH];
			wsprintf(tagTemp, L"Field3_Room3ToyKnight %d", m_iKnightCount);
			tagWeaponTemp[MAX_PATH];
			wsprintf(tagWeaponTemp, L"Field3_Room3ToyKnight_MorningStar %d", m_iKnightCount);
			tTemp.tParentBone = tagTemp;

			pMornigStar = nullptr;
			pGameObject = pMornigStar = CMorningStar::Create(m_pGraphicDev, &tTemp);
			NULL_CHECK_RETURN(pGameObject, );
			FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagWeaponTemp, pGameObject), );

			pObj.vScale = { 0.01f, 0.01f, 0.01f };
			pObj.vAngle = { 0.f, 50.f, 0.f };
			pObj.vPos = { 32.7f , 0.f, -97.f };
			pObj.tParentBone = tTemp.tParentBone;
			pGameObject = CKnight::Create(m_pGraphicDev, pMornigStar, &pObj);
			NULL_CHECK_RETURN(pGameObject, );
			FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObj.tParentBone, pGameObject, this), );
			setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eDefaultTrigger | FilterGroup::eWall | FilterGroup::eMonster | FilterGroup::eGround);

			m_iKnightCount++;
			m_iKnightCountMax++;

			tagTemp[MAX_PATH];
			wsprintf(tagTemp, L"Field3_Room3ToyKnight %d", m_iKnightCount);
			tagWeaponTemp[MAX_PATH];
			wsprintf(tagWeaponTemp, L"Field3_Room3ToyKnight_MorningStar %d", m_iKnightCount);
			tTemp.tParentBone = tagTemp;

			pMornigStar = nullptr;
			pGameObject = pMornigStar = CMorningStar::Create(m_pGraphicDev, &tTemp);
			NULL_CHECK_RETURN(pGameObject, );
			FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagWeaponTemp, pGameObject), );

			pObj.vScale = { 0.01f, 0.01f, 0.01f };
			pObj.vAngle = { 0.f, 35.f, 0.f };
			pObj.vPos = { 32.7f , 0.f, -99.f };
			pObj.tParentBone = tTemp.tParentBone;
			pGameObject = CKnight::Create(m_pGraphicDev, pMornigStar, &pObj);
			NULL_CHECK_RETURN(pGameObject, );
			FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObj.tParentBone, pGameObject, this), );
			setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eDefaultTrigger | FilterGroup::eWall | FilterGroup::eMonster | FilterGroup::eGround);

			m_iKnightCount++;
			m_iKnightCountMax++;


			wsprintf(tagTemp, L"Field3_Room3ToyArbalist %d", m_iArbalistCount);
			wsprintf(tagWeaponTemp, L"Field3_Room3ToyArbalist_CrossBow %d", m_iArbalistCount);
			tTemp.tParentBone = tagTemp;

			pCrossBow = nullptr;
			pGameObject = pCrossBow = CCrossBow::Create(m_pGraphicDev, &tTemp);
			NULL_CHECK_RETURN(pGameObject, );
			FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagWeaponTemp, pGameObject), );

			pObj.vScale = { 0.015f, 0.015f, 0.015f };
			pObj.vAngle = { 0.f, 0.f, 0.f };
			pObj.vPos = { 31.5f , 0.f, -99.f };
			pObj.tParentBone = tTemp.tParentBone;
			pGameObject = CArbalist::Create(m_pGraphicDev, pCrossBow, &pObj);
			NULL_CHECK_RETURN(pGameObject, );
			FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObj.tParentBone, pGameObject, this), );

			setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eDefaultTrigger);



			m_iArbalistCount++;
			m_iArbalistCountMax++;
			pMap->Set_Dead();
		}

		OutputDebugString(L"Field3 2번방 몬스터 움직이는 트리거\n");
		for (_uint i = 0; i < 4; ++i)
		{
			_tchar tagTemp[MAX_PATH];
			wsprintf(tagTemp, L"Field3_Room2ToyKnight %d", i);

			CKnight*	pKnight = (CKnight*)Engine::Get_GameObject(L"GameObject", tagTemp);
			if (nullptr == pKnight)
				return;

			pKnight->Set_Move(true);
		}

		for (_uint i = 0; i < 1; ++i)
		{
			_tchar tagTemp[MAX_PATH];
			wsprintf(tagTemp, L"Field3_Room2ToySheilder %d", i);

			CSheilder*	pSheilder = (CSheilder*)Engine::Get_GameObject(L"GameObject", tagTemp);
			if (nullptr == pSheilder)
				return;

			pSheilder->Set_Move(true);
		}
	}

	m_pTrigger[TRIGGER_CHESS_FIELD3_MOVEROOM2]->Set_Interactable(false);
}

void CChessField::Event_Field3_Room3Move(PxTriggerPair * pairs)
{
	if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
	{
		OutputDebugString(L"Field3 3번방 몬스터 움직이는 트리거\n");
		for (_uint i = 0; i < 2; ++i)
		{
			_tchar tagTemp[MAX_PATH];
			wsprintf(tagTemp, L"Field3_Room3ToyKnight %d", i);

			CKnight*	pKnight = (CKnight*)Engine::Get_GameObject(L"GameObject", tagTemp);
			if (nullptr == pKnight)
				return;

			pKnight->Set_Move(true);
		}

		for (_uint i = 0; i < 1; ++i)
		{
			_tchar tagTemp[MAX_PATH];
			wsprintf(tagTemp, L"Field3_Room3ToyArbalist %d", i);

			CArbalist*	pArbalist = (CArbalist*)Engine::Get_GameObject(L"GameObject", tagTemp);
			if (nullptr == pArbalist)
				return;

			pArbalist->Set_Move(true);
		}
	}
	m_pTrigger[TRIGGER_CHESS_FIELD3_MOVEROOM3]->Set_Interactable(false);
}

void CChessField::Event_Field3_Room4Move(PxTriggerPair * pairs)
{
	if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
	{

		CGameObject*	pMap = Engine::Get_GameObject(L"GameObject", L"ChessField_Dark8");
		if (pMap)
		{
			Engine::CLayer*				pLayer = Engine::Get_Layer(L"GameObject");
			Engine::CGameObject*		pGameObject = nullptr;

			OutputDebugString(L"세번째 필드 몬스터 생성\n");

			_tchar tagTemp[MAX_PATH];
			_tchar tagWeaponTemp[MAX_PATH];
			OBJINFO tTemp;
			OBJINFO pObj;
			CMorningStar* pMornigStar = nullptr;
			CCrossBow* pCrossBow = nullptr;
			CSheild* pSheild = nullptr;

			m_iKnightCount = 0;
			m_iKnightCountMax = 0;

			tagTemp[MAX_PATH];
			wsprintf(tagTemp, L"Field3_Room4ToyKnight %d", m_iKnightCount);
			tagWeaponTemp[MAX_PATH];
			wsprintf(tagWeaponTemp, L"Field3_Room4ToyKnight_MorningStar %d", m_iKnightCount);
			tTemp.tParentBone = tagTemp;

			pMornigStar = nullptr;
			pGameObject = pMornigStar = CMorningStar::Create(m_pGraphicDev, &tTemp);
			NULL_CHECK_RETURN(pGameObject, );
			FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagWeaponTemp, pGameObject), );

			pObj.vScale = { 0.01f, 0.01f, 0.01f };
			pObj.vAngle = { 0.f, 87.f, 0.f };
			pObj.vPos = { 31.9f , 0.f, -107.1f };
			pObj.tParentBone = tTemp.tParentBone;
			pGameObject = CKnight::Create(m_pGraphicDev, pMornigStar, &pObj);
			NULL_CHECK_RETURN(pGameObject, );
			FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObj.tParentBone, pGameObject, this), );
			setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eDefaultTrigger | FilterGroup::eWall | FilterGroup::eMonster | FilterGroup::eGround);

			m_iKnightCount++;
			m_iKnightCountMax++;

			tagTemp[MAX_PATH];
			wsprintf(tagTemp, L"Field3_Room4ToyKnight %d", m_iKnightCount);
			tagWeaponTemp[MAX_PATH];
			wsprintf(tagWeaponTemp, L"Field3_Room4ToyKnight_MorningStar %d", m_iKnightCount);
			tTemp.tParentBone = tagTemp;

			pMornigStar = nullptr;
			pGameObject = pMornigStar = CMorningStar::Create(m_pGraphicDev, &tTemp);
			NULL_CHECK_RETURN(pGameObject, );
			FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagWeaponTemp, pGameObject), );

			pObj.vScale = { 0.01f, 0.01f, 0.01f };
			pObj.vAngle = { 0.f, 45.f, 0.f };
			pObj.vPos = { 31.9f , 0.f, -109.1f };
			pObj.tParentBone = tTemp.tParentBone;
			pGameObject = CKnight::Create(m_pGraphicDev, pMornigStar, &pObj);
			NULL_CHECK_RETURN(pGameObject, );
			FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObj.tParentBone, pGameObject, this), );
			setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eDefaultTrigger | FilterGroup::eWall | FilterGroup::eMonster | FilterGroup::eGround);

			m_iKnightCount++;
			m_iKnightCountMax++;

			tagTemp[MAX_PATH];
			wsprintf(tagTemp, L"Field3_Room4ToyKnight %d", m_iKnightCount);
			tagWeaponTemp[MAX_PATH];
			wsprintf(tagWeaponTemp, L"Field3_Room4ToyKnight_MorningStar %d", m_iKnightCount);
			tTemp.tParentBone = tagTemp;

			pMornigStar = nullptr;
			pGameObject = pMornigStar = CMorningStar::Create(m_pGraphicDev, &tTemp);
			NULL_CHECK_RETURN(pGameObject, );
			FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagWeaponTemp, pGameObject), );

			pObj.vScale = { 0.01f, 0.01f, 0.01f };
			pObj.vAngle = { 0.f, 15.f, 0.f };
			pObj.vPos = { 31.9f , 0.f, -105.1f };
			pObj.tParentBone = tTemp.tParentBone;
			pGameObject = CKnight::Create(m_pGraphicDev, pMornigStar, &pObj);
			NULL_CHECK_RETURN(pGameObject, );
			FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObj.tParentBone, pGameObject, this), );
			setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eDefaultTrigger | FilterGroup::eWall | FilterGroup::eMonster | FilterGroup::eGround);

			m_iKnightCount++;
			m_iKnightCountMax++;

			tagTemp[MAX_PATH];
			wsprintf(tagTemp, L"Field3_Room4ToyKnight %d", m_iKnightCount);
			tagWeaponTemp[MAX_PATH];
			wsprintf(tagWeaponTemp, L"Field3_Room4ToyKnight_MorningStar %d", m_iKnightCount);
			tTemp.tParentBone = tagTemp;

			pMornigStar = nullptr;
			pGameObject = pMornigStar = CMorningStar::Create(m_pGraphicDev, &tTemp);
			NULL_CHECK_RETURN(pGameObject, );
			FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagWeaponTemp, pGameObject), );

			pObj.vScale = { 0.01f, 0.01f, 0.01f };
			pObj.vAngle = { 0.f, 22.f, 0.f };
			pObj.vPos = { 33.4f , 0.f, -107.1f };
			pObj.tParentBone = tTemp.tParentBone;
			pGameObject = CKnight::Create(m_pGraphicDev, pMornigStar, &pObj);
			NULL_CHECK_RETURN(pGameObject, );
			FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObj.tParentBone, pGameObject, this), );
			setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eDefaultTrigger | FilterGroup::eWall | FilterGroup::eMonster | FilterGroup::eGround);

			m_iKnightCount++;
			m_iKnightCountMax++;

			tagTemp[MAX_PATH];
			wsprintf(tagTemp, L"Field3_Room4ToyKnight %d", m_iKnightCount);
			tagWeaponTemp[MAX_PATH];
			wsprintf(tagWeaponTemp, L"Field3_Room4ToyKnight_MorningStar %d", m_iKnightCount);
			tTemp.tParentBone = tagTemp;

			pMornigStar = nullptr;
			pGameObject = pMornigStar = CMorningStar::Create(m_pGraphicDev, &tTemp);
			NULL_CHECK_RETURN(pGameObject, );
			FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagWeaponTemp, pGameObject), );

			pObj.vScale = { 0.01f, 0.01f, 0.01f };
			pObj.vAngle = { 0.f, 66.f, 0.f };
			pObj.vPos = { 33.4f , 0.f, -105.1f };
			pObj.tParentBone = tTemp.tParentBone;
			pGameObject = CKnight::Create(m_pGraphicDev, pMornigStar, &pObj);
			NULL_CHECK_RETURN(pGameObject, );
			FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObj.tParentBone, pGameObject, this), );
			setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eDefaultTrigger | FilterGroup::eWall | FilterGroup::eMonster | FilterGroup::eGround);

			m_iKnightCount++;
			m_iKnightCountMax++;
			pMap->Set_Dead();
		}

		OutputDebugString(L"Field3 4번방 몬스터 움직이는 트리거\n");
		for (_uint i = 0; i < 5; ++i)
		{
			_tchar tagTemp[MAX_PATH];
			wsprintf(tagTemp, L"Field3_Room4ToyKnight %d", i);

			CKnight*	pKnight = (CKnight*)Engine::Get_GameObject(L"GameObject", tagTemp);
			if (nullptr == pKnight)
				return;

			pKnight->Set_Move(true);
		}
	}
	m_pTrigger[TRIGGER_CHESS_FIELD3_MOVEROOM4]->Set_Interactable(false);
}

void CChessField::Event_Field3_Room5Move(PxTriggerPair * pairs)
{
	if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
	{

		CGameObject*	pMap = Engine::Get_GameObject(L"GameObject", L"ChessField_Dark9");
		if (pMap)
		{
			Engine::CLayer*				pLayer = Engine::Get_Layer(L"GameObject");
			Engine::CGameObject*		pGameObject = nullptr;

			OutputDebugString(L"세번째 필드 몬스터 생성\n");

			_tchar tagTemp[MAX_PATH];
			_tchar tagWeaponTemp[MAX_PATH];
			OBJINFO tTemp;
			OBJINFO pObj;
			CMorningStar* pMornigStar = nullptr;
			CCrossBow* pCrossBow = nullptr;
			CSheild* pSheild = nullptr;


			m_iKnightCount = 0;
			m_iKnightCountMax = 0;

			tagTemp[MAX_PATH];
			wsprintf(tagTemp, L"Field3_Room5ToyKnight %d", m_iKnightCount);
			tagWeaponTemp[MAX_PATH];
			wsprintf(tagWeaponTemp, L"Field3_Room5ToyKnight_MorningStar %d", m_iKnightCount);
			tTemp.tParentBone = tagTemp;

			pMornigStar = nullptr;
			pGameObject = pMornigStar = CMorningStar::Create(m_pGraphicDev, &tTemp);
			NULL_CHECK_RETURN(pGameObject, );
			FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagWeaponTemp, pGameObject), );

			pObj.vScale = { 0.01f, 0.01f, 0.01f };
			pObj.vAngle = { 0.f, 61.f, 0.f };
			pObj.vPos = { 56.2f , 0.f, -105.1f };
			pObj.tParentBone = tTemp.tParentBone;
			pGameObject = CKnight::Create(m_pGraphicDev, pMornigStar, &pObj);
			NULL_CHECK_RETURN(pGameObject, );
			FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObj.tParentBone, pGameObject, this), );
			setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eDefaultTrigger | FilterGroup::eWall | FilterGroup::eMonster | FilterGroup::eGround);


			m_iKnightCount++;
			m_iKnightCountMax++;

			tagTemp[MAX_PATH];
			wsprintf(tagTemp, L"Field3_Room5ToyKnight %d", m_iKnightCount);
			tagWeaponTemp[MAX_PATH];
			wsprintf(tagWeaponTemp, L"Field3_Room5ToyKnight_MorningStar %d", m_iKnightCount);
			tTemp.tParentBone = tagTemp;

			pMornigStar = nullptr;
			pGameObject = pMornigStar = CMorningStar::Create(m_pGraphicDev, &tTemp);
			NULL_CHECK_RETURN(pGameObject, );
			FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagWeaponTemp, pGameObject), );

			pObj.vScale = { 0.01f, 0.01f, 0.01f };
			pObj.vAngle = { 0.f, 57.f, 0.f };
			pObj.vPos = { 56.2f , 0.f, -105.1f };
			pObj.tParentBone = tTemp.tParentBone;
			pGameObject = CKnight::Create(m_pGraphicDev, pMornigStar, &pObj);
			NULL_CHECK_RETURN(pGameObject, );
			FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObj.tParentBone, pGameObject, this), );
			setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eDefaultTrigger | FilterGroup::eWall | FilterGroup::eMonster | FilterGroup::eGround);

			m_iKnightCount++;
			m_iKnightCountMax++;

			tagTemp[MAX_PATH];
			wsprintf(tagTemp, L"Field3_Room5ToyKnight %d", m_iKnightCount);
			tagWeaponTemp[MAX_PATH];
			wsprintf(tagWeaponTemp, L"Field3_Room5ToyKnight_MorningStar %d", m_iKnightCount);
			tTemp.tParentBone = tagTemp;

			pMornigStar = nullptr;
			pGameObject = pMornigStar = CMorningStar::Create(m_pGraphicDev, &tTemp);
			NULL_CHECK_RETURN(pGameObject, );
			FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagWeaponTemp, pGameObject), );

			pObj.vScale = { 0.01f, 0.01f, 0.01f };
			pObj.vAngle = { 0.f, 26.f, 0.f };
			pObj.vPos = { 56.2f , 0.f, -103.7f };
			pObj.tParentBone = tTemp.tParentBone;
			pGameObject = CKnight::Create(m_pGraphicDev, pMornigStar, &pObj);
			NULL_CHECK_RETURN(pGameObject, );
			FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObj.tParentBone, pGameObject, this), );
			setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eDefaultTrigger | FilterGroup::eWall | FilterGroup::eMonster | FilterGroup::eGround);

			m_iKnightCount++;
			m_iKnightCountMax++;


			tagTemp[MAX_PATH];
			wsprintf(tagTemp, L"Field3_Room5ToyKnight %d", m_iKnightCount);
			tagWeaponTemp[MAX_PATH];
			wsprintf(tagWeaponTemp, L"Field3_Room5ToyKnight_MorningStar %d", m_iKnightCount);
			tTemp.tParentBone = tagTemp;

			pMornigStar = nullptr;
			pGameObject = pMornigStar = CMorningStar::Create(m_pGraphicDev, &tTemp);
			NULL_CHECK_RETURN(pGameObject, );
			FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagWeaponTemp, pGameObject), );

			pObj.vScale = { 0.01f, 0.01f, 0.01f };
			pObj.vAngle = { 0.f, 41.f, 0.f };
			pObj.vPos = { 54.7f , 0.f, -105.1f };
			pObj.tParentBone = tTemp.tParentBone;
			pGameObject = CKnight::Create(m_pGraphicDev, pMornigStar, &pObj);
			NULL_CHECK_RETURN(pGameObject, );
			FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObj.tParentBone, pGameObject, this), );
			setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eDefaultTrigger | FilterGroup::eWall | FilterGroup::eMonster | FilterGroup::eGround);

			m_iKnightCount++;
			m_iKnightCountMax++;

			tagTemp[MAX_PATH];
			wsprintf(tagTemp, L"Field3_Room5ToyKnight %d", m_iKnightCount);
			tagWeaponTemp[MAX_PATH];
			wsprintf(tagWeaponTemp, L"Field3_Room5ToyKnight_MorningStar %d", m_iKnightCount);
			tTemp.tParentBone = tagTemp;

			pMornigStar = nullptr;
			pGameObject = pMornigStar = CMorningStar::Create(m_pGraphicDev, &tTemp);
			NULL_CHECK_RETURN(pGameObject, );
			FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagWeaponTemp, pGameObject), );

			pObj.vScale = { 0.01f, 0.01f, 0.01f };
			pObj.vAngle = { 0.f, 87.f, 0.f };
			pObj.vPos = { 54.7f , 0.f, -106.6f };
			pObj.tParentBone = tTemp.tParentBone;
			pGameObject = CKnight::Create(m_pGraphicDev, pMornigStar, &pObj);
			NULL_CHECK_RETURN(pGameObject, );
			FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObj.tParentBone, pGameObject, this), );
			setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eDefaultTrigger | FilterGroup::eWall | FilterGroup::eMonster | FilterGroup::eGround);

			m_iKnightCount++;
			m_iKnightCountMax++;
			pMap->Set_Dead();
		}
		OutputDebugString(L"Field3 5번방 몬스터 움직이는 트리거\n");
		for (_uint i = 0; i < 5; ++i)
		{
			_tchar tagTemp[MAX_PATH];
			wsprintf(tagTemp, L"Field3_Room5ToyKnight %d", i);

			CKnight*	pKnight = (CKnight*)Engine::Get_GameObject(L"GameObject", tagTemp);
			if (nullptr == pKnight)
				return;

			pKnight->Set_Move(true);
		}

	}
	m_pTrigger[TRIGGER_CHESS_FIELD3_MOVEROOM5]->Set_Interactable(false);
}

void CChessField::Event_Field3_Room6Move(PxTriggerPair * pairs)
{
	if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
	{

		CGameObject*	pMap = Engine::Get_GameObject(L"GameObject", L"ChessField_Dark11");
		if (pMap)
		{
			Engine::CLayer*				pLayer = Engine::Get_Layer(L"GameObject");
			Engine::CGameObject*		pGameObject = nullptr;

			OutputDebugString(L"세번째 필드 몬스터 생성\n");

			_tchar tagTemp[MAX_PATH];
			_tchar tagWeaponTemp[MAX_PATH];
			OBJINFO tTemp;
			OBJINFO pObj;
			CMorningStar* pMornigStar = nullptr;
			CCrossBow* pCrossBow = nullptr;
			CSheild* pSheild = nullptr;

			m_iKnightCount = 0;
			m_iKnightCountMax = 0;
			m_iArbalistCount = 0;
			m_iArbalistCountMax = 0;

			tagTemp[MAX_PATH];
			wsprintf(tagTemp, L"Field3_Room6ToyKnight %d", m_iKnightCount);
			tagWeaponTemp[MAX_PATH];
			wsprintf(tagWeaponTemp, L"Field3_Room6ToyKnight_MorningStar %d", m_iKnightCount);
			tTemp.tParentBone = tagTemp;

			pMornigStar = nullptr;
			pGameObject = pMornigStar = CMorningStar::Create(m_pGraphicDev, &tTemp);
			NULL_CHECK_RETURN(pGameObject, );
			FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagWeaponTemp, pGameObject), );

			pObj.vScale = { 0.01f, 0.01f, 0.01f };
			pObj.vAngle = { 0.f, 15.f, 0.f };
			pObj.vPos = { 52.1f , 0.f, -113.f };
			pObj.tParentBone = tTemp.tParentBone;
			pGameObject = CKnight::Create(m_pGraphicDev, pMornigStar, &pObj);
			NULL_CHECK_RETURN(pGameObject, );
			FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObj.tParentBone, pGameObject, this), );
			setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eDefaultTrigger |  FilterGroup::eWall | FilterGroup::eMonster | FilterGroup::eGround);

			m_iKnightCount++;
			m_iKnightCountMax++;

			tagTemp[MAX_PATH];
			wsprintf(tagTemp, L"Field3_Room6ToyKnight %d", m_iKnightCount);
			tagWeaponTemp[MAX_PATH];
			wsprintf(tagWeaponTemp, L"Field3_Room6ToyKnight_MorningStar %d", m_iKnightCount);
			tTemp.tParentBone = tagTemp;

			pMornigStar = nullptr;
			pGameObject = pMornigStar = CMorningStar::Create(m_pGraphicDev, &tTemp);
			NULL_CHECK_RETURN(pGameObject, );
			FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagWeaponTemp, pGameObject), );

			pObj.vScale = { 0.01f, 0.01f, 0.01f };
			pObj.vAngle = { 0.f, 35.f, 0.f };
			pObj.vPos = { 52.1f , 0.f, -115.f };
			pObj.tParentBone = tTemp.tParentBone;
			pGameObject = CKnight::Create(m_pGraphicDev, pMornigStar, &pObj);
			NULL_CHECK_RETURN(pGameObject, );
			FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObj.tParentBone, pGameObject, this), );
			setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eDefaultTrigger | FilterGroup::eWall | FilterGroup::eMonster | FilterGroup::eGround);

			m_iKnightCount++;
			m_iKnightCountMax++;

			tagTemp[MAX_PATH];
			wsprintf(tagTemp, L"Field3_Room6ToyKnight %d", m_iKnightCount);
			tagWeaponTemp[MAX_PATH];
			wsprintf(tagWeaponTemp, L"Field3_Room6ToyKnight_MorningStar %d", m_iKnightCount);
			tTemp.tParentBone = tagTemp;

			pMornigStar = nullptr;
			pGameObject = pMornigStar = CMorningStar::Create(m_pGraphicDev, &tTemp);
			NULL_CHECK_RETURN(pGameObject, );
			FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagWeaponTemp, pGameObject), );

			pObj.vScale = { 0.01f, 0.01f, 0.01f };
			pObj.vAngle = { 0.f, 24.f, 0.f };
			pObj.vPos = { 52.1f , 0.f, -117.f };
			pObj.tParentBone = tTemp.tParentBone;
			pGameObject = CKnight::Create(m_pGraphicDev, pMornigStar, &pObj);
			NULL_CHECK_RETURN(pGameObject, );
			FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObj.tParentBone, pGameObject, this), );
			setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eDefaultTrigger | FilterGroup::eWall | FilterGroup::eMonster | FilterGroup::eGround);

			m_iKnightCount++;
			m_iKnightCountMax++;

			tagTemp[MAX_PATH];
			wsprintf(tagTemp, L"Field3_Room6ToyKnight %d", m_iKnightCount);
			tagWeaponTemp[MAX_PATH];
			wsprintf(tagWeaponTemp, L"Field3_Room6ToyKnight_MorningStar %d", m_iKnightCount);
			tTemp.tParentBone = tagTemp;

			pMornigStar = nullptr;
			pGameObject = pMornigStar = CMorningStar::Create(m_pGraphicDev, &tTemp);
			NULL_CHECK_RETURN(pGameObject, );
			FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagWeaponTemp, pGameObject), );

			pObj.vScale = { 0.01f, 0.01f, 0.01f };
			pObj.vAngle = { 0.f, 67.f, 0.f };
			pObj.vPos = { 50.5f , 0.f, -115.f };
			pObj.tParentBone = tTemp.tParentBone;
			pGameObject = CKnight::Create(m_pGraphicDev, pMornigStar, &pObj);
			NULL_CHECK_RETURN(pGameObject, );
			FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObj.tParentBone, pGameObject, this), );
			setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eDefaultTrigger | FilterGroup::eWall | FilterGroup::eMonster | FilterGroup::eGround);

			m_iKnightCount++;
			m_iKnightCountMax++;

			tagTemp[MAX_PATH];
			wsprintf(tagTemp, L"Field3_Room6ToyKnight %d", m_iKnightCount);
			tagWeaponTemp[MAX_PATH];
			wsprintf(tagWeaponTemp, L"Field3_Room6ToyKnight_MorningStar %d", m_iKnightCount);
			tTemp.tParentBone = tagTemp;

			pMornigStar = nullptr;
			pGameObject = pMornigStar = CMorningStar::Create(m_pGraphicDev, &tTemp);
			NULL_CHECK_RETURN(pGameObject, );
			FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagWeaponTemp, pGameObject), );

			pObj.vScale = { 0.01f, 0.01f, 0.01f };
			pObj.vAngle = { 0.f, 41.f, 0.f };
			pObj.vPos = { 50.5f , 0.f, -117.f };
			pObj.tParentBone = tTemp.tParentBone;
			pGameObject = CKnight::Create(m_pGraphicDev, pMornigStar, &pObj);
			NULL_CHECK_RETURN(pGameObject, );
			FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObj.tParentBone, pGameObject, this), );
			setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eDefaultTrigger | FilterGroup::eWall | FilterGroup::eMonster | FilterGroup::eGround);

			m_iKnightCount++;
			m_iKnightCountMax++;


			wsprintf(tagTemp, L"Field3_Room6ToyArbalist %d", m_iArbalistCount);
			wsprintf(tagWeaponTemp, L"Field3_Room6ToyArbalist_CrossBow %d", m_iArbalistCount);
			tTemp.tParentBone = tagTemp;

			pCrossBow = nullptr;
			pGameObject = pCrossBow = CCrossBow::Create(m_pGraphicDev, &tTemp);
			NULL_CHECK_RETURN(pGameObject, );
			FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagWeaponTemp, pGameObject), );

			pObj.vScale = { 0.015f, 0.015f, 0.015f };
			pObj.vAngle = { 0.f, 0.f, 0.f };
			pObj.vPos = { 53.5f , 0.f, -118.f };
			pObj.tParentBone = tTemp.tParentBone;
			pGameObject = CArbalist::Create(m_pGraphicDev, pCrossBow, &pObj);
			NULL_CHECK_RETURN(pGameObject, );
			FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObj.tParentBone, pGameObject, this), );

			setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eDefaultTrigger);


			m_iArbalistCount++;
			m_iArbalistCountMax++;

			wsprintf(tagTemp, L"Field3_Room6ToyArbalist %d", m_iArbalistCount);
			wsprintf(tagWeaponTemp, L"Field3_Room6ToyArbalist_CrossBow %d", m_iArbalistCount);
			tTemp.tParentBone = tagTemp;

			pCrossBow = nullptr;
			pGameObject = pCrossBow = CCrossBow::Create(m_pGraphicDev, &tTemp);
			NULL_CHECK_RETURN(pGameObject, );
			FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagWeaponTemp, pGameObject), );

			pObj.vScale = { 0.015f, 0.015f, 0.015f };
			pObj.vAngle = { 0.f, 0.f, 0.f };
			pObj.vPos = { 55.7f , 0.f, -116.f };
			pObj.tParentBone = tTemp.tParentBone;
			pGameObject = CArbalist::Create(m_pGraphicDev, pCrossBow, &pObj);
			NULL_CHECK_RETURN(pGameObject, );
			FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObj.tParentBone, pGameObject, this), );

			setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eDefaultTrigger);


			m_iArbalistCount++;
			m_iArbalistCountMax++;
			pMap->Set_Dead();
		}
		pMap = Engine::Get_GameObject(L"GameObject", L"ChessField_Dark12");
		if (pMap)
			pMap->Set_Dead();

		OutputDebugString(L"Field3 6번방 몬스터 움직이는 트리거\n");
		for (_uint i = 0; i < 5; ++i)
		{
			_tchar tagTemp[MAX_PATH];
			wsprintf(tagTemp, L"Field3_Room6ToyKnight %d", i);

			CKnight*	pKnight = (CKnight*)Engine::Get_GameObject(L"GameObject", tagTemp);
			if (nullptr == pKnight)
				return;

			pKnight->Set_Move(true);
		}

		for (_uint i = 0; i < 2; ++i)
		{
			_tchar tagTemp[MAX_PATH];
			wsprintf(tagTemp, L"Field3_Room6ToyArbalist %d", i);

			CArbalist*	pArbalist = (CArbalist*)Engine::Get_GameObject(L"GameObject", tagTemp);
			if (nullptr == pArbalist)
				return;

			pArbalist->Set_Move(true);
		}
	}

	CGameObject*		pMap = Engine::Get_GameObject(L"GameObject", L"Field2");
	if (pMap)
		pMap->Set_Dead();

	m_pTrigger[TRIGGER_CHESS_FIELD3_MOVEROOM6]->Set_Interactable(false);
}



void CChessField::Event_OgreField_DoorStun(PxTriggerPair * pairs)
{
	if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
	{
		OutputDebugString(L"문 오우거 충돌\n");
		COgre* pOgre = (COgre*)Engine::Get_GameObject(L"GameObject", L"ToyOgre");
		pOgre->Set_Stun(true);
	}
}

void CChessField::Event_OgreStone(PxTriggerPair * pairs)
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

void CChessField::Event_Dead(PxTriggerPair * pairs)
{
	if (pairs->otherShape->getSimulationFilterData().word0 == FilterGroup::eCody &&
		pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
	{
		CCodyWizard* pCody = ToObj<CCodyWizard>(pairs->otherShape->getName());
		NULL_CHECK_RETURN(pCody, );

		pCody->Set_Dead();
	}
	else if (pairs->otherShape->getSimulationFilterData().word0 == FilterGroup::eMay&&
		pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
	{
		CMayCastle* pMay = ToObj<CMayCastle>(pairs->otherShape->getName());
		NULL_CHECK_RETURN(pMay, );

		pMay->Set_Dead();
	}
}

void CChessField::Event_Save(PxTriggerPair* pairs)
{
	auto* pTrigger = ToTrigger(pairs->triggerShape->getName());

	CSaveSpace* pSave = pTrigger->Is<CSaveSpace>();

	if (pairs->otherShape->getSimulationFilterData().word0 == FilterGroup::eCody &&
		pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
	{
		CCodyWizard* pCody = ToObj<CCodyWizard>(pairs->otherShape->getName());
		NULL_CHECK_RETURN(pCody, );

		pCody->Set_SavePoint(pSave->Get_SavePoint());
	}
	else if (pairs->otherShape->getSimulationFilterData().word0 == FilterGroup::eMay&&
		pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
	{
		CMayCastle* pMay = ToObj<CMayCastle>(pairs->otherShape->getName());
		NULL_CHECK_RETURN(pMay, );

		pMay->Set_SavePoint(pSave->Get_SavePoint());
	}
}

void CChessField::Event_Arrow_Dead(PxTriggerPair * pairs)
{
	if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
	{
		OutputDebugString(L"화살이 벽과 충돌\n");
		CArrow* pArrow = ToTrigger(pairs->triggerShape->getActor()->getName())->Is<CArrow>();
		if (nullptr == pArrow)
			return;

		pArrow->Set_Dead();
	}
}

void CChessField::Event_CrusherButton(PxTriggerPair * pairs)
{
	CChessDoorSwitch* pSwitch = ToTrigger(pairs->triggerShape->getActor()->getName())->Is<CChessDoorSwitch>();
	if (nullptr == pSwitch)
		return;


	if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
	{
		OutputDebugString(L"크러셔 시작 발판 충돌\n");
		pSwitch->Set_CrusherMove(true);
	}
	if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_LOST)
	{

		OutputDebugString(L"크러셔 시작 발판에서 나감\n");
		pSwitch->Set_CrusherMove(false);
	}
}

void CChessField::Event_ArrowHit(PxTriggerPair * pairs)
{
	auto* pTrigger = ToTrigger(pairs->triggerShape->getName());

	if (pairs->otherShape->getSimulationFilterData().word0 == FilterGroup::eMay)
	{
		CMayCastle* pMay = ToObj<CMayCastle>(pairs->otherShape->getName());
		if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			CArrow* pArrow = ToTrigger(pairs->triggerShape->getName())->Is<CArrow>();
			pMay->Set_Damage(-1);
			pArrow->Set_Dead();

			OutputDebugString(L"메이와 화살 충돌");
			//pMay->Set_PickUp(true, CMay_Space::GENERATOR_LEVER, pLever);
		}
	}
	else if (pairs->otherShape->getSimulationFilterData().word0 == FilterGroup::eCody)
	{
		CCodyWizard* pCody = ToObj<CCodyWizard>(pairs->otherShape->getName());
		if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			
			CArrow* pArrow = ToTrigger(pairs->triggerShape->getName())->Is<CArrow>();
			pCody->Set_Damage(-1);
			pArrow->Set_Dead();
			OutputDebugString(L"코디와 화살 충돌");

		}
	}
}

HRESULT CChessField::Ready_Environment_Layer(const _tchar * pLayerTag)
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
//#스테이지 체스 필드
HRESULT CChessField::Ready_GameObject_Layer(const _tchar * pLayerTag)
{
	Engine::CLayer*			pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	OBJINFO tObj;
	ZeroMemory(&tObj, sizeof(OBJINFO));
	tObj.eSceneID = RESOURCE_CF;
	// 오브젝트 추가
	Engine::CGameObject*		pGameObject = nullptr;

	// MainCamera
	// 카메라 원본
	pGameObject = CMainCamera::Create(m_pGraphicDev, &_vec3(0.f, 10.f, -5.f), &_vec3(0.f, 0.f, 1.f), &_vec3(0.f, 1.f, 0.f), D3DXToRadian(60.f), _float(WINCX) / WINCY, 0.1f, 1000.f, &_vec3(-8.0f, 15.0f, -5.0f), 15.f);
	//pGameObject = CMainCamera::Create(m_pGraphicDev, &_vec3(0.f, 10.f, -5.f), &_vec3(0.f, 0.f, 1.f), &_vec3(0.f, 1.f, 0.f), D3DXToRadian(60.f), _float(WINCX) / WINCY, 0.1f, 1000.f, &_vec3(0.0f, 15.0f, -5.0f), 15.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MainCamera", pGameObject), E_FAIL);

	
	//tObj.vPos = _vec3(100.f, 1.f, 10.f);

	tObj.vPos = { 48.f, 1.f, -72.f }; //3번째맵돌아가는 트랩 앞							
	tObj.vPos = { 55.f, 0.5f, -125.f }; //4번째 맵 Crusher앞
	tObj.vPos = { 16.f , 1.f, -30.4f }; //두번째맵 시작.
	tObj.vPos = { 117.f, 10.f, -157.f }; // Ogre 앞.
	tObj.vPos = { 0.f, 2.f, 2.f }; //시작점

	pGameObject = CCodyWizard::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Cody", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eCody, FilterGroup::eWall | FilterGroup::eMay | FilterGroup::eOgre | FilterGroup::eArrow);


	tObj.vPos += {0.f, 0.f, -2.f};
	pGameObject = CMayCastle::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"May", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMay, FilterGroup::eWall | FilterGroup::eCody | FilterGroup::eOgre | FilterGroup::eArrow);

	pGameObject = CToySword::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ToySword", pGameObject), E_FAIL);

	pGameObject = COrb::Create(m_pGraphicDev, "RightAttach", &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"IceOrb_Left", pGameObject), E_FAIL);

	pGameObject = COrb::Create(m_pGraphicDev, "LeftAttach", &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"IceOrb_Right", pGameObject), E_FAIL);

	//tObj.vPos = { 0.0f, -2.0f, 0.0f };
	//tObj.vScale = _vec3{ 300.0f, 1.0f, 300.0f };
	////tObj.
	//pGameObject = CDeadSpace::Create(m_pGraphicDev, L"Blue_Bg", true, &tObj);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DeadSpace1", pGameObject, this), E_FAIL);

	//{x = -1.26491487 y = 0.0282890201 z = -24.9108219 }
	//{x = 16.6584301 y = -0.000565707684 z = -36.1707573 }
	//{x = 25.4484901 y = -0.000226527452 z = -51.3361664 }
	//{x=44.8309479 y=0.000469326973 z=-91.8664780 }
	//{x = -1.26491487 y = 0.0282890201 z = -24.9108219 }
	{
	// 세이브 포인트
		tObj.vPos = { -1.26491487f, 0.0f, -24.9108219f };
		tObj.vScale = _vec3{ 4.0f, 2.0f, 4.0f };
		pGameObject = CSaveSpace::Create(m_pGraphicDev, { -1.26491487f, 0.0f, -24.9108219f }, L"Yellow_Bg", g_bRenderBox, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SaveSpace1", pGameObject, this), E_FAIL);

		tObj.vPos = { 16.6584301f, 0.0f, -36.1707573f };
		pGameObject = CSaveSpace::Create(m_pGraphicDev, { 16.6584301f, 0.0f, -36.1707573f }, L"Yellow_Bg", g_bRenderBox, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SaveSpace2", pGameObject, this), E_FAIL);


		tObj.vPos = { -1.26491487f, 0.0f, -24.9108219f };
		pGameObject = CSaveSpace::Create(m_pGraphicDev, { -1.26491487f, 0.0f, -24.9108219f }, L"Yellow_Bg", g_bRenderBox, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SaveSpace3", pGameObject, this), E_FAIL);


		tObj.vPos = { 46.8668900f, 0.0f, -68.5713806f };
		pGameObject = CSaveSpace::Create(m_pGraphicDev, { 46.8668900f, 0.0f, -68.5713806f }, L"Yellow_Bg", g_bRenderBox, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SaveSpace4", pGameObject, this), E_FAIL);

		tObj.vPos = { 44.8309479f, 0.0f, -91.8664780f};
		pGameObject = CSaveSpace::Create(m_pGraphicDev, { 44.8309479f, 0.0f, -91.8664780f }, L"Yellow_Bg", g_bRenderBox, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SaveSpace5", pGameObject, this), E_FAIL);

		tObj.vPos = { 47.7139587f, 0.0f, -110.272430f};
		pGameObject = CSaveSpace::Create(m_pGraphicDev, { 47.7139587f, 0.0f, -110.272430f }, L"Yellow_Bg", g_bRenderBox, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SaveSpace6", pGameObject, this), E_FAIL);

		tObj.vPos = { 120.031204f, 0.0f, -155.770828f };
		pGameObject = CSaveSpace::Create(m_pGraphicDev, { 122.780159f, 0.0f, -152.130295f }, L"Yellow_Bg", g_bRenderBox, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SaveSpace7", pGameObject, this), E_FAIL);

		// _D3DVECTOR = {47.7139587f, 0.0f -110.272430f } 크러셔 앞
		// _D3DVECTOR = { x = 120.031204 y = -0.179007262 z = -155.770828 } 오그레 앞 _D3DVECTOR = {x=122.780159 y=-0.000898629427 z=-152.130295 }

	}


	//맵 오브젝트
#pragma region 돌아가는 트랩 //////////////////////////////////////////////////////////////////////////
	tObj.vScale = { 0.007f, 0.007f, 0.007f };
	tObj.vPos = { 45.f, 0.f, -72.5f };
	_vec3 vStartPos = { 45.f, 0.f, -72.5f };
	_vec3 vEndPos = { 58.f, 0.f, -72.5f };
	pGameObject = CChessTrab::Create(m_pGraphicDev, vStartPos, vEndPos, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ChessSpinTrab1", pGameObject, this), E_FAIL);


	tObj.vScale = { 0.007f, 0.007f, 0.007f };
	tObj.vPos = { 60.f, 0.f, -81.7f };
	vStartPos = { 60.f, 0.f, -81.7f };
	vEndPos = { 56.f, 0.f, -87.f };
	pGameObject = CChessTrab::Create(m_pGraphicDev, vStartPos, vEndPos, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ChessSpinTrab2", pGameObject, this), E_FAIL);


	tObj.vScale = { 0.007f, 0.007f, 0.007f };
	tObj.vPos = { 46.f, 0.f, -83.1f };
	vStartPos = { 46.f, 0.f, -83.1f };
	vEndPos = { 52.f, 0.f, -87.8f };
	pGameObject = CChessTrab::Create(m_pGraphicDev, vStartPos, vEndPos, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ChessSpinTrab3", pGameObject, this), E_FAIL);


	tObj.vScale = { 0.007f, 0.007f, 0.007f };
	tObj.vPos = { 36.f, 0.f, -96.3f };
	vStartPos = { 36.f, 0.f, -96.3f };
	vEndPos = { 48.4f, 0.f, -103.3f };
	pGameObject = CChessTrab::Create(m_pGraphicDev, vStartPos, vEndPos, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ChessSpinTrab4", pGameObject, this), E_FAIL);

	tObj.vScale = { 0.007f, 0.007f, 0.007f };
	tObj.vPos = { 39.4f, 0.f, -98.6f };
	vStartPos = { 39.4f, 0.f, -98.6f };
	vEndPos = { 36.1f, 0.f, -103.3f };
	pGameObject = CChessTrab::Create(m_pGraphicDev, vStartPos, vEndPos, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ChessSpinTrab5", pGameObject, this), E_FAIL);

	tObj.vScale = { 0.007f, 0.007f, 0.007f };
	tObj.vPos = { 72.6f, 0.f, -130.8f };
	vStartPos = { 72.6f, 0.f, -130.8f };
	vEndPos = { 56.9f, 0.f, -141.1f };
	pGameObject = CChessTrab::Create(m_pGraphicDev, vStartPos, vEndPos, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ChessSpinTrab6", pGameObject, this), E_FAIL);

	tObj.vScale = { 0.007f, 0.007f, 0.007f };
	tObj.vPos = { 66.8f, 0.f, -150.f };
	vStartPos = { 66.8f, 0.f, -150.f };
	vEndPos = { 74.5f, 0.f, -135.f };
	pGameObject = CChessTrab::Create(m_pGraphicDev, vStartPos, vEndPos, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ChessSpinTrab7", pGameObject, this), E_FAIL);
#pragma endregion

#pragma region 발판 및 문 //////////////////////////////////////////////////////////////////////////
	//첫번? 문 발판.
	tObj.vScale = { 0.007f, 0.007f, 0.007f };
	tObj.vPos = { -3.f, 0.f, -16.f };
	pGameObject = CChessDoorSwitch::Create(m_pGraphicDev, BUTTON_DOOR, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ChessDoorSwitch", pGameObject, this), E_FAIL);

	tObj.vScale = { 0.007f, 0.007f, 0.007f };
	tObj.vPos = { -3.f, 0.f, -16.f };
	pGameObject = CChessDoorSwitchFrame::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ChessDoorSwitchFrame", pGameObject, this), E_FAIL);


	//첫번째 불기둥 발판.
	//tObj.vScale = { 0.007f, 0.007f, 0.007f };
	//tObj.vPos = { -12.f, 0.f, -16.f };
	//pGameObject = CChessDoorSwitch::Create(m_pGraphicDev, BUTTON_FIRE, &tObj);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ChessFireSwitch", pGameObject, this), E_FAIL);

	//tObj.vScale = { 0.007f, 0.007f, 0.007f };
	//tObj.vPos = { -12.f, 0.f, -16.f };
	//pGameObject = CChessDoorSwitchFrame::Create(m_pGraphicDev, &tObj);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ChessFireSwitchFrame", pGameObject, this), E_FAIL);


	//건너는 다리 발판.
	tObj.vScale = { 0.007f, 0.007f, 0.007f };
	tObj.vPos = { 0.f, 0.f, -35.f };
	pGameObject = CChessDoorSwitch::Create(m_pGraphicDev, BUTTON_BRIDGE, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ChessBridgeSwitch", pGameObject, this), E_FAIL);

	tObj.vScale = { 0.007f, 0.007f, 0.007f };
	tObj.vPos = { -0.f, 0.f, -35.f };
	pGameObject = CChessDoorSwitchFrame::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ChessBridgeSwitchFrame", pGameObject, this), E_FAIL);

	tObj.vScale = { 0.007f, 0.007f, 0.007f };
	tObj.vPos = { 1.7f, 0.f, -11.2f };
	pGameObject = CChessOpenDoor::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ChessOpenDoor", pGameObject, this), E_FAIL);

	tObj.vScale = { 0.007f, 0.007f, 0.007f };
	tObj.vPos = { 16.f, 0.f, -25.f };
	pGameObject = CChessBridge::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ChessBridge", pGameObject, this), E_FAIL);

	tObj.vScale = { 0.007f, 0.007f, 0.007f };
	tObj.vAngle = { 0.f, 0.f, 0.f };
	tObj.vPos = { 31.0f, 0.f, -49.5f };
	pGameObject = CChessDuoDoor::Create(m_pGraphicDev, false, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ChessDuoDoorLeft", pGameObject, this), E_FAIL);


	tObj.vScale = { 0.007f, 0.007f, 0.007f };
	tObj.vAngle = { 0.f, 180.f, 0.f };
	tObj.vPos = { 30.7f, 0.f, -53.1f };
	pGameObject = CChessDuoDoor::Create(m_pGraphicDev, true, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ChessDuoDoorRight", pGameObject, this), E_FAIL);

#pragma endregion

#pragma region 맵 트리거 //////////////////////////////////////////////////////////////////////////
	//1번맵
	m_pTrigger[TRIGGER_CHESS_ONE_KNIGHTMOVE] = CTrigger::CreateBoxTrigger(m_pScene, L"OneToyKnightTrigger", _vec3(-8.f, 0.f, -24.f), 2.f, TRIGGER_CHESS_ONE_KNIGHTMOVE, FilterGroup::eChessTrigger);
	m_pTrigger[TRIGGER_CHESS_TWO_KNIGHTMOVE] = CTrigger::CreateBoxTrigger(m_pScene, L"TwoToyKnightTrigger", _vec3(-0.7f, 0.f, -30.5f), 2.f, TRIGGER_CHESS_TWO_KNIGHTMOVE, FilterGroup::eChessTrigger);

	//2번맵
	m_pTrigger[TRIGGER_CHESS_FIELD2_CREATE] = CTrigger::CreateBoxTrigger(m_pScene, L"Field2_Monster_CreateTrigger", _vec3(16.f, 0.f, -36.4f), 2.f, TRIGGER_CHESS_FIELD2_CREATE, FilterGroup::eChessTrigger);
	m_pTrigger[TRIGGER_CHESS_FIELD2_MOVE_ARBALIST] = CTrigger::CreateBoxTrigger(m_pScene, L"Field2_Monster_ArbalistMove", _vec3(37.4f, 0.f, -42.6f), 3.f, TRIGGER_CHESS_FIELD2_MOVE_ARBALIST, FilterGroup::eChessTrigger);
	m_pTrigger[TRIGGER_CHESS_FIELD2_MOVE_KNIGHT] = CTrigger::CreateBoxTrigger(m_pScene, L"Field2_Monster_KnightMove", _vec3(50.f, 0.f, -48.6f), 2.f, TRIGGER_CHESS_FIELD2_MOVE_KNIGHT, FilterGroup::eChessTrigger);

	//3번맵
	m_pTrigger[TRIGGER_CHESS_CRUSHER_HIT] = CTrigger::CreateBoxTrigger(m_pScene, L"Crusher_Hit", _vec3(51.9f, 0.f, -128.f), 3.f, TRIGGER_CHESS_CRUSHER_HIT, FilterGroup::eChessTrigger);

	m_pTrigger[TRIGGER_CHESS_FIELD3_CREATE] = CTrigger::CreateBoxTrigger(m_pScene, L"Field3_Monster_CreateTrigger", _vec3(52.9f, 0.f, -72.6f), 2.f, TRIGGER_CHESS_FIELD3_CREATE, FilterGroup::eChessTrigger);
	m_pTrigger[TRIGGER_CHESS_FIELD3_MOVEROOM1] = CTrigger::CreateBoxTrigger(m_pScene, L"Field3_MoveRoom1", _vec3(47.9f, 0.f, -80.8f), 2.f, TRIGGER_CHESS_FIELD3_MOVEROOM1, FilterGroup::eChessTrigger);
	m_pTrigger[TRIGGER_CHESS_FIELD3_MOVEROOM2] = CTrigger::CreateBoxTrigger(m_pScene, L"Field3_MoveRoom2", _vec3(46.1f, 0.f, -90.4f), 3.f, TRIGGER_CHESS_FIELD3_MOVEROOM2, FilterGroup::eChessTrigger);
	m_pTrigger[TRIGGER_CHESS_FIELD3_MOVEROOM3] = CTrigger::CreateBoxTrigger(m_pScene, L"Field3_MoveRoom3", _vec3(36.1f, 0.f, -96.9f), 2.f, TRIGGER_CHESS_FIELD3_MOVEROOM3, FilterGroup::eChessTrigger);
	m_pTrigger[TRIGGER_CHESS_FIELD3_MOVEROOM4] = CTrigger::CreateBoxTrigger(m_pScene, L"Field3_MoveRoom4", _vec3(34.6f, 0.f, -105.6f), 2.f, TRIGGER_CHESS_FIELD3_MOVEROOM4, FilterGroup::eChessTrigger);
	m_pTrigger[TRIGGER_CHESS_FIELD3_MOVEROOM5] = CTrigger::CreateBoxTrigger(m_pScene, L"Field3_MoveRoom5", _vec3(53.f, 0.f, -102.7f), 2.f, TRIGGER_CHESS_FIELD3_MOVEROOM5, FilterGroup::eChessTrigger);
	m_pTrigger[TRIGGER_CHESS_FIELD3_MOVEROOM6] = CTrigger::CreateBoxTrigger(m_pScene, L"Field3_MoveRoom6", _vec3(47.5f, 0.f, -112.2f), 3.f, TRIGGER_CHESS_FIELD3_MOVEROOM6, FilterGroup::eChessTrigger);
	m_pTrigger[TRIGGER_CHESS_OGRE_CAMERA] = CTrigger::CreateBoxTrigger(m_pScene, L"OgreCamera", _vec3(109.5f, 1.f, -169.5f), 4.f, TRIGGER_CHESS_OGRE_CAMERA, FilterGroup::eDefaultTrigger);

	//Ogre 맵
	//m_pTrigger[TRIGGER_CHESS_FIELD3_MOVEROOM6] = CTrigger::CreateBoxTrigger(m_pScene, L"Field3_MoveRoom6", _vec3(47.5f, 0.f, -112.2f), 3.f, TRIGGER_CHESS_FIELD3_MOVEROOM6, FilterGroup::eChessTrigger);


#pragma endregion

#pragma region 첫번째 맵 몬스터 //////////////////////////////////////////////////////////////////////////

	#pragma region 첫번째 트리거 기사 몬스터

		m_iKnightCount = 0;
		m_iKnightCountMax = 0;

		_tchar tagTemp[MAX_PATH];
		wsprintf(tagTemp, L"FristToyKnight %d", m_iKnightCount);

		_tchar tagWeaponTemp[MAX_PATH];
		wsprintf(tagWeaponTemp, L"FristToyKnight_MorningStar %d", m_iKnightCount);

		OBJINFO tTemp;
		tTemp.tParentBone = tagTemp;

		CMorningStar* pMornigStar = nullptr;
		pGameObject = pMornigStar = CMorningStar::Create(m_pGraphicDev, &tTemp);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagWeaponTemp, pGameObject), E_FAIL);

		OBJINFO pObj;
		pObj.vScale = { 0.01f, 0.01f, 0.01f };
		pObj.vAngle = { 0.f, 30.f, 0.f };
		pObj.vPos = { -5.f , 0.f, -24.f };
		pObj.tParentBone = tTemp.tParentBone;
		pGameObject = CKnight::Create(m_pGraphicDev, pMornigStar, &pObj);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObj.tParentBone, pGameObject, this), E_FAIL);
		setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eDefaultTrigger | FilterGroup::eWall | FilterGroup::eMonster | FilterGroup::eGround);

		m_iKnightCount++;
		m_iKnightCountMax++;


		tagTemp[MAX_PATH];
		wsprintf(tagTemp, L"FristToyKnight %d", m_iKnightCount);

		tagWeaponTemp[MAX_PATH];
		wsprintf(tagWeaponTemp, L"FristToyKnight_MorningStar %d", m_iKnightCount);


		tTemp.tParentBone = tagTemp;

		pMornigStar = nullptr;
		pGameObject = pMornigStar = CMorningStar::Create(m_pGraphicDev, &tTemp);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagWeaponTemp, pGameObject), E_FAIL);

		pObj.vScale = { 0.01f, 0.01f, 0.01f };
		pObj.vAngle = { 0.f, 0.f, 0.f };
		pObj.vPos = { -3.f , 0.f, -24.f };
		pObj.tParentBone = tTemp.tParentBone;
		pGameObject = CKnight::Create(m_pGraphicDev, pMornigStar, &pObj);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObj.tParentBone, pGameObject, this), E_FAIL);
		setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eDefaultTrigger | FilterGroup::eWall | FilterGroup::eMonster | FilterGroup::eGround);

		m_iKnightCount++;
		m_iKnightCountMax++;

		tagTemp[MAX_PATH];
		wsprintf(tagTemp, L"FristToyKnight %d", m_iKnightCount);

		tagWeaponTemp[MAX_PATH];
		wsprintf(tagWeaponTemp, L"FristToyKnight_MorningStar %d", m_iKnightCount);

		tTemp.tParentBone = tagTemp;

		pMornigStar = nullptr;
		pGameObject = pMornigStar = CMorningStar::Create(m_pGraphicDev, &tTemp);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagWeaponTemp, pGameObject), E_FAIL);

		pObj.vScale = { 0.01f, 0.01f, 0.01f };
		pObj.vAngle = { 0.f, 0.f, 0.f };
		pObj.vPos = { -3.f , 0.f, -26.f };
		pObj.tParentBone = tTemp.tParentBone;
		pGameObject = CKnight::Create(m_pGraphicDev, pMornigStar, &pObj);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObj.tParentBone, pGameObject, this), E_FAIL);
		setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eDefaultTrigger | FilterGroup::eWall | FilterGroup::eMonster | FilterGroup::eGround);

		m_iKnightCount++;
		m_iKnightCountMax++;

	#pragma endregion

	#pragma region 두번째 트리거 기사 몬스터



	m_iKnightCount = 0;
	m_iKnightCountMax = 0;

	tagTemp[MAX_PATH];
	wsprintf(tagTemp, L"TwoToyKnight %d", m_iKnightCount);

	tagWeaponTemp[MAX_PATH];
	wsprintf(tagWeaponTemp, L"TwoToyKnight_MorningStar %d", m_iKnightCount);

	tTemp.tParentBone = tagTemp;

	pMornigStar = nullptr;
	pGameObject = pMornigStar = CMorningStar::Create(m_pGraphicDev, &tTemp);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagWeaponTemp, pGameObject), E_FAIL);

	pObj.vScale = { 0.01f, 0.01f, 0.01f };
	pObj.vAngle = { 0.f, 50.f, 0.f };
	pObj.vPos = { -0.5f , 0.f, -38.f };
	pObj.tParentBone = tTemp.tParentBone;
	pGameObject = CKnight::Create(m_pGraphicDev, pMornigStar, &pObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObj.tParentBone, pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eDefaultTrigger | FilterGroup::eWall | FilterGroup::eMonster | FilterGroup::eGround);

	m_iKnightCount++;
	m_iKnightCountMax++;

	tagTemp[MAX_PATH];
	wsprintf(tagTemp, L"TwoToyKnight %d", m_iKnightCount);

	tagWeaponTemp[MAX_PATH];
	wsprintf(tagWeaponTemp, L"TwoToyKnight_MorningStar %d", m_iKnightCount);


	tTemp.tParentBone = tagTemp;

	pMornigStar = nullptr;
	pGameObject = pMornigStar = CMorningStar::Create(m_pGraphicDev, &tTemp);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagWeaponTemp, pGameObject), E_FAIL);

	pObj.vScale = { 0.01f, 0.01f, 0.01f };
	pObj.vAngle = { 0.f, 50.f, 0.f };
	pObj.vPos = { -2.f , 0.f, -38.f };
	pObj.tParentBone = tTemp.tParentBone;
	pGameObject = CKnight::Create(m_pGraphicDev, pMornigStar, &pObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObj.tParentBone, pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eDefaultTrigger | FilterGroup::eWall | FilterGroup::eMonster | FilterGroup::eGround);

	m_iKnightCount++;
	m_iKnightCountMax++;


	tagTemp[MAX_PATH];
	wsprintf(tagTemp, L"TwoToyKnight %d", m_iKnightCount);

	tagWeaponTemp[MAX_PATH];
	wsprintf(tagWeaponTemp, L"TwoToyKnight_MorningStar %d", m_iKnightCount);

	tTemp.tParentBone = tagTemp;

	pMornigStar = nullptr;
	pGameObject = pMornigStar = CMorningStar::Create(m_pGraphicDev, &tTemp);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagWeaponTemp, pGameObject), E_FAIL);

	pObj.vScale = { 0.01f, 0.01f, 0.01f };
	pObj.vAngle = { 0.f, 50.f, 0.f };
	pObj.vPos = { 2.f , 0.f, -38.f };
	pObj.tParentBone = tTemp.tParentBone;
	pGameObject = CKnight::Create(m_pGraphicDev, pMornigStar, &pObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObj.tParentBone, pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eDefaultTrigger | FilterGroup::eWall | FilterGroup::eMonster | FilterGroup::eGround);

	m_iKnightCount++;
	m_iKnightCountMax++;


	#pragma endregion

	#pragma region 활쟁이 몬스터


	m_iArbalistCount = 0;
	m_iArbalistCountMax = 0;

	tagTemp[MAX_PATH];
	wsprintf(tagTemp, L"OneToyArbalist %d", m_iArbalistCount);

	tagWeaponTemp[MAX_PATH];
	wsprintf(tagWeaponTemp, L"OneToyArbalist_CrossBow %d", m_iArbalistCount);


	tTemp.tParentBone = tagTemp;

	CCrossBow* pCrossBow = nullptr;
	pGameObject = pCrossBow = CCrossBow::Create(m_pGraphicDev, &tTemp);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagWeaponTemp, pGameObject), E_FAIL);

	pObj.vScale = { 0.015f, 0.015f, 0.015f };
	pObj.vAngle = { 0.f, -95.f, 0.f };
	pObj.vPos = { 16.f , 0.f, -34.4f };
	pObj.tParentBone = tTemp.tParentBone;
	pGameObject = CArbalist::Create(m_pGraphicDev, pCrossBow, &pObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObj.tParentBone, pGameObject, this), E_FAIL);

	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eDefaultTrigger);


	m_iArbalistCount++;
	m_iArbalistCountMax++;

	tagTemp[MAX_PATH];
	wsprintf(tagTemp, L"OneToyArbalist %d", m_iArbalistCount);

	tagWeaponTemp[MAX_PATH];
	wsprintf(tagWeaponTemp, L"OneToyArbalist_CrossBow %d", m_iArbalistCount);


	tTemp.tParentBone = tagTemp;
	pCrossBow = nullptr;
	pGameObject = pCrossBow = CCrossBow::Create(m_pGraphicDev, &tTemp);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagWeaponTemp, pGameObject), E_FAIL);

	pObj.vScale = { 0.015f, 0.015f, 0.015f };
	pObj.vAngle = { 0.f, -85.f, 0.f };
	pObj.vPos = { 16.f , 0.f, -36.4f };
	pObj.tParentBone = tTemp.tParentBone;
	pGameObject = CArbalist::Create(m_pGraphicDev, pCrossBow, &pObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObj.tParentBone, pGameObject, this), E_FAIL);

	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eDefaultTrigger);


	m_iArbalistCount++;
	m_iArbalistCountMax++;

#pragma endregion

#pragma endregion

#pragma region Ogre //////////////////////////////////////////////////////////////////////////



	tObj.vScale = { 0.016f, 0.010f, 0.012f };
	tObj.vPos = { 118.6f, 2.9f, -153.3f };
	tObj.vAngle = { 0.f, 0.f, 0.f };
	pGameObject = COgreStartDoor::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"OgreStartDoor", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eWall, FilterGroup::eOgre);


	tObj.vScale = { 0.007f, 0.007f, 0.007f };
	tObj.vPos = { 130.5f,0.f, -140.7f };
	pGameObject = COgre::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ToyOgre", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eOgre, FilterGroup::eDefaultTrigger | FilterGroup::eStone | FilterGroup::eHatch | FilterGroup::eWall);


	tObj.vScale = { 0.007f, 0.007f, 0.007f };
	tObj.vAngle = { 0.f , 174.f , 0 };
	_vec3 temp = _vec3{ 179.77f, 5.f, -194.19f} * 0.7f;
	tObj.vPos = { 125.7f, 0.5f, -136.5f };
	temp = temp - tObj.vPos;
	pGameObject = COgreChain::Create(m_pGraphicDev, temp, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"OgreChain1", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eStone, FilterGroup::eOgre);

	tObj.vScale = { 0.007f, 0.007f, 0.007f };
	tObj.vAngle = { 0.f, -102.f, 0.f };
	temp = _vec3{ 197.11f, 5.f, -205.93f } * 0.7f;
	tObj.vPos = { 137.4f, 0.5f, -144.5f };
	temp = temp - tObj.vPos;
	pGameObject = COgreChain::Create(m_pGraphicDev, temp, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"OgreChain2", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eStone, FilterGroup::eOgre);


	tObj.vScale = { 0.007f, 0.007f, 0.007f };
	tObj.vAngle = { 0.f, -17.1f, 0.f };
	temp = _vec3{ 185.42f, 5.f, -223.07f } * 0.7f;
	tObj.vPos = { 130.f, 0.5f, -156.8f };
	temp = temp - tObj.vPos;
	pGameObject = COgreChain::Create(m_pGraphicDev, temp, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"OgreChain3", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eStone, FilterGroup::eOgre);

	tObj.vScale = { 0.007f, 0.007f, 0.008f };
	tObj.vAngle = { 0.f, 92.f, 0.f };
	temp = _vec3{ 165.81f, 5.f, -209.40f } * 0.7f;
	tObj.vPos = { 115.4f, 0.5f, -146.7f };
	temp = temp - tObj.vPos;
	pGameObject = COgreChain::Create(m_pGraphicDev, temp, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"OgreChain4", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eStone, FilterGroup::eOgre);




	tObj.vScale = { 0.007f, 0.007f, 0.007f };
	tObj.vPos = { 127.2f, 0.f, -146.6f };
	pGameObject = CTestCol::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"HatchCol", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eHatch, FilterGroup::eOgre);

	tObj.vScale = { 0.007f, 0.007f, 0.007f };
	tObj.vPos = { 124.5f, -1.f, -144.4f };
	pGameObject = CCenterHatch::Create(m_pGraphicDev, false, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"OgreCenterHatch1", pGameObject, this), E_FAIL);

	tObj.vScale = { 0.007f, 0.007f, 0.007f };
	tObj.vPos = { 129.9f, -1.f, -148.1f };
	pGameObject = CCenterHatch::Create(m_pGraphicDev, true, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"OgreCenterHatch2", pGameObject, this), E_FAIL);
#pragma endregion

#pragma region Crusher //////////////////////////////////////////////////////////////////////////

	tObj.vScale = { 0.007f, 0.007f, 0.007f };
	tObj.vAngle = { 0.f, -137.5f, 0.f };
	//tObj.vPos = { 55.f, 1.f, -125.f }; 
	tObj.vPos = { 104.27f * 0.7f, 0.f, -208.65f*0.7f };
	pGameObject = CCrusherBridgeBase::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CrusherBridgeBase", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eGround, FilterGroup::eCody | FilterGroup::eMay);

	tObj.vScale = { 0.007f, 0.007f, 0.007f };
	tObj.vAngle = { 0.f, -137.5f, 0.f };
	tObj.vPos = { 104.42f * 0.7f, -0.5f, -208.79f *0.7f };
	pGameObject = CCrusherBridge::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CrusherBridge", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eGround, FilterGroup::eCody | FilterGroup::eMay);


	tObj.vScale = { 0.008f, 0.007f, 0.007f };
	tObj.vAngle = { 0.f, 40.f, 0.f };
	tObj.vPos = { 55.8f, 0.f, -125.1f };
	pGameObject = CCrusherStartDoor::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CrusherStartDoor", pGameObject, this), E_FAIL);

	tObj.vScale = { 0.007f, 0.007f, 0.007f };
	tObj.vAngle = { 0.f, 0.f, 0.f };
	tObj.vPos = { 76.6f, 0.f, -145.2f };
	pGameObject = CCrusherHitPoint::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CrusherHitPoint1", pGameObject, this), E_FAIL);

	tObj.vScale = { 0.007f, 0.007f, 0.007f };
	tObj.vAngle = { 0.f, 0.f, 0.f };
	tObj.vPos = { 77.7f, 0.f, -147.9f };
	pGameObject = CCrusherHitPoint::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CrusherHitPoint2", pGameObject, this), E_FAIL);

	tObj.vScale = { 0.007f, 0.007f, 0.007f };
	tObj.vAngle = { 0.f, 0.f, 0.f };
	tObj.vPos = { 75.2f, 0.f, -150.7f };
	pGameObject = CCrusherHitPoint::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CrusherHitPoint3", pGameObject, this), E_FAIL);

	tObj.vScale = { 0.007f, 0.007f, 0.007f };
	tObj.vAngle = { 0.f, 0.f, 0.f };
	tObj.vPos = { 72.4f, 0.f,-149.8f };
	pGameObject = CCrusherHitPoint::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CrusherHitPoint4", pGameObject, this), E_FAIL);

	tObj.vScale = { 0.007f, 0.007f, 0.007f };
	tObj.vAngle = { 0.f, 40.f, 0.f };
	tObj.vPos = { 48.f, 0.f, -121.f };
	pGameObject = CCrusherBreakWall::Create(m_pGraphicDev, &tObj, false);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CrusherBreakWall1", pGameObject, this), E_FAIL);


	tObj.vScale = { 0.007f, 0.007f, 0.007f };
	tObj.vAngle = { 0.f, 40.f, 0.f };
	tObj.vPos = { 45.6f, 0.f,-123.9f };
	pGameObject = CCrusherBreakWall::Create(m_pGraphicDev, &tObj, false);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CrusherBreakWall2", pGameObject, this), E_FAIL);

	tObj.vScale = { 0.007f, 0.007f, 0.007f };
	tObj.vAngle = { 0.f, 44.f, 0.f };
	tObj.vPos = { 62.6f, 0.f,-134.f };
	pGameObject = CCrusherBreakWall::Create(m_pGraphicDev, &tObj, false);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CrusherBreakWall3", pGameObject, this), E_FAIL);

	tObj.vScale = { 0.007f, 0.007f, 0.007f };
	tObj.vAngle = { 0.f, 44.f, 0.f };
	tObj.vPos = { 60.f, 0.f,-136.7f };
	pGameObject = CCrusherBreakWall::Create(m_pGraphicDev, &tObj, false);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CrusherBreakWall4", pGameObject, this), E_FAIL);

	tObj.vScale = { 0.007f, 0.007f, 0.007f };
	tObj.vAngle = { 0.f, 43.f, 0.f };
	tObj.vPos = { 78.5f, 0.f,-149.f };
	pGameObject = CCrusherBreakWall::Create(m_pGraphicDev, &tObj, false);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CrusherBreakWall5", pGameObject, this), E_FAIL);

	tObj.vScale = { 0.007f, 0.007f, 0.007f };
	tObj.vAngle = { 0.f, 42.f, 0.f };
	tObj.vPos = { 76.05f, 0.f,-151.75f };
	pGameObject = CCrusherBreakWall::Create(m_pGraphicDev, &tObj, false);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CrusherBreakWall6", pGameObject, this), E_FAIL);


	tObj.vScale = { 0.007f, 0.007f, 0.007f };
	tObj.vAngle = { 0.f, 40.f, 0.f };
	tObj.vPos = { 50.f, 0.f, -123.f };
	pGameObject = CChessDoorSwitch::Create(m_pGraphicDev, BUTTON_CRUSHER, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CrusherSwitch1", pGameObject, this), E_FAIL);

	tObj.vScale = { 0.007f, 0.007f, 0.007f };
	tObj.vAngle = { 0.f, 40.f, 0.f };
	tObj.vPos = { 50.f, 0.f, -123.f };
	pGameObject = CChessDoorSwitchFrame::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CrusherSwitchhFrame1", pGameObject, this), E_FAIL);


	tObj.vScale = { 0.007f, 0.007f, 0.007f };
	tObj.vAngle = { 0.f, 40.f, 0.f };
	tObj.vPos = { 47.6f, 0.f,-125.9f };
	pGameObject = CChessDoorSwitch::Create(m_pGraphicDev, BUTTON_CRUSHER, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CrusherSwitch2", pGameObject, this), E_FAIL);

	tObj.vScale = { 0.007f, 0.007f, 0.007f };
	tObj.vAngle = { 0.f, 40.f, 0.f };
	tObj.vPos = { 47.6f, 0.f,-125.9f };
	pGameObject = CChessDoorSwitchFrame::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CrusherSwitchhFrame2", pGameObject, this), E_FAIL);


#pragma endregion

#pragma endregion


	tObj.vAngle = { 0.f,0.f, 0.f };
	//tObj.vScale = _vec3{ 500.f, 500.f, 1.f };
	//tObj.vAngle = { 90.f, 0.f, 0.f };
	//tObj.vPos = { 0.f, -3.f, 0.f };
	//pGameObject = CDeadSpace::Create(m_pGraphicDev, L"FireWall", true, &tObj);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"FireWall", pGameObject, this), E_FAIL);

	////////////////////////////////////////////////////////////////////////

#pragma region CHESS 맵 //////////////////////////////////////////////////////////////////////////
	//CHESSFIELD MAP
	lstrcpy(tObj.tagMesh, L"ChessField_Part01");
	tObj.vScale = _vec3{ 0.007f,0.007f,0.007f };
	tObj.eMeshType = 1;
	tObj.eRenderType = RENDER_NONALPHA;
	tObj.vPos = { 0.f,0.f,0.f };


	lstrcpy(tObj.tagMesh, L"ChessField_Part01");
	pGameObject = CMap::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Field1", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eWall, FilterGroup::eArrow | FilterGroup::eIceOrb);

	lstrcpy(tObj.tagMesh, L"ChessField_Part02");
	pGameObject = CMap::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Field2", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eWall, FilterGroup::eArrow | FilterGroup::eIceOrb);

	lstrcpy(tObj.tagMesh, L"ChessField_Part03");
	pGameObject = CMap::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Field3", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eWall, FilterGroup::eArrow | FilterGroup::eIceOrb);

	lstrcpy(tObj.tagMesh, L"ChessField_Part04");
	pGameObject = CMap::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Field4", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eWall, FilterGroup::eArrow | FilterGroup::eIceOrb);

	lstrcpy(tObj.tagMesh, L"ChessField_Part05_Wall");
	pGameObject = CMap::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Field5", pGameObject, this), E_FAIL);
	setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eWall, FilterGroup::eOgre | FilterGroup::eIceOrb);

	lstrcpy(tObj.tagMesh, L"ChessField_Part05_Floor");
	pGameObject = CMap::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Field6", pGameObject, this), E_FAIL);

	lstrcpy(tObj.tagMesh, L"ChessField_Light");
	pGameObject = CMap::Create(m_pGraphicDev, &tObj);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Field7", pGameObject, this), E_FAIL);

	
	#pragma region Dark //////////////////////////////////////////////////////////////////////////
		lstrcpy(tObj.tagMesh, L"ChessField_Dark");
		pGameObject = CMap::Create(m_pGraphicDev, &tObj);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Field8", pGameObject, this), E_FAIL);

		lstrcpy(tObj.tagMesh, L"ChessField_Dark1");
		pGameObject = CMap::Create(m_pGraphicDev, &tObj);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ChessField_Dark1", pGameObject, this), E_FAIL);

		lstrcpy(tObj.tagMesh, L"ChessField_Dark2");
		pGameObject = CMap::Create(m_pGraphicDev, &tObj);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ChessField_Dark2", pGameObject, this), E_FAIL);

		lstrcpy(tObj.tagMesh, L"ChessField_Dark3");
		pGameObject = CMap::Create(m_pGraphicDev, &tObj);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ChessField_Dark3", pGameObject, this), E_FAIL);

		lstrcpy(tObj.tagMesh, L"ChessField_Dark4");
		pGameObject = CMap::Create(m_pGraphicDev, &tObj);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ChessField_Dark4", pGameObject, this), E_FAIL);

		lstrcpy(tObj.tagMesh, L"ChessField_Dark5");
		pGameObject = CMap::Create(m_pGraphicDev, &tObj);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ChessField_Dark5", pGameObject, this), E_FAIL);

		lstrcpy(tObj.tagMesh, L"ChessField_Dark6");
		pGameObject = CMap::Create(m_pGraphicDev, &tObj);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ChessField_Dark6", pGameObject, this), E_FAIL);

		lstrcpy(tObj.tagMesh, L"ChessField_Dark7");
		pGameObject = CMap::Create(m_pGraphicDev, &tObj);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ChessField_Dark7", pGameObject, this), E_FAIL);

		lstrcpy(tObj.tagMesh, L"ChessField_Dark8");
		pGameObject = CMap::Create(m_pGraphicDev, &tObj);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ChessField_Dark8", pGameObject, this), E_FAIL);

		lstrcpy(tObj.tagMesh, L"ChessField_Dark9");
		pGameObject = CMap::Create(m_pGraphicDev, &tObj);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ChessField_Dark9", pGameObject, this), E_FAIL);

		lstrcpy(tObj.tagMesh, L"ChessField_Dark10");
		pGameObject = CMap::Create(m_pGraphicDev, &tObj);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ChessField_Dark10", pGameObject, this), E_FAIL);

		lstrcpy(tObj.tagMesh, L"ChessField_Dark11");
		pGameObject = CMap::Create(m_pGraphicDev, &tObj);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ChessField_Dark11", pGameObject, this), E_FAIL);

		lstrcpy(tObj.tagMesh, L"ChessField_Dark12");
		pGameObject = CMap::Create(m_pGraphicDev, &tObj);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ChessField_Dark12", pGameObject, this), E_FAIL);

		lstrcpy(tObj.tagMesh, L"ChessField_Dark13");
		pGameObject = CMap::Create(m_pGraphicDev, &tObj);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ChessField_Dark13", pGameObject, this), E_FAIL);
		//////////////////////////////////////////////////////////////////////

	#pragma endregion

		tObj.vScale = _vec3{ 0.2f, 0.1f, 0.2f };
		tObj.vAngle = { 0.f, 0.f, 0.f };
		tObj.vTriggerPos = { 0.0f, -2.f, 0.0f };
		tObj.vTriggerScale = { 300.0f, 1.0f, 300.0f };
		tObj.vPos = { 58.f, -16.f, -78.f };
		pGameObject = CDeadSpace::Create(m_pGraphicDev, L"Lava", true, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DeadWater", pGameObject, this), E_FAIL);

		tObj.vScale = _vec3{ 0.2f, 0.1f, 0.2f };
		tObj.vAngle = { 0.f, 0.f, 0.f };
		tObj.vTriggerPos = { 0.0f, -2.f, 0.0f };
		tObj.vTriggerScale = { 50.0f, 1.0f, 50.0f };
		tObj.vPos = { 114.12f, -14.f, -216.48f };
		pGameObject = CDeadSpace::Create(m_pGraphicDev, L"Lava", true, &tObj);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DeadWater2", pGameObject, this), E_FAIL);

#pragma endregion

	m_mapLayer.emplace(pLayerTag, pLayer);


	return S_OK;
}

HRESULT CChessField::Ready_LoadMap_Layer(const _tchar * pLayerTag, const _tchar * pFilePath)
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

HRESULT CChessField::Ready_LightInfo(void)
{
	D3DLIGHT9				tLightInfo;
	ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));


	tLightInfo.Type = D3DLIGHT_POINT;
	tLightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Ambient = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.f);


	tLightInfo.Diffuse = D3DXCOLOR(1.5f * 1.5f, 0.8f * 1.5f, 0.6f * 1.5f, 1.f);
	tLightInfo.Position = _vec3(3.95f, 7.f, -17.99f) * 0.7f;
	tLightInfo.Range = 15.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 2);

	tLightInfo.Diffuse = D3DXCOLOR(1.5f, 0.5f, 0.3f, 1.f);
	tLightInfo.Position = _vec3(-4.96f, 5.f, 4.31f) * 0.7f;
	tLightInfo.Range = 7.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 3);

	tLightInfo.Diffuse = D3DXCOLOR(0.25f * 2.5f, 0.55f * 2.5f, 0.9f * 2.5f, 1.f);
	tLightInfo.Position = _vec3(0.f, 5.f, 0.f);
	tLightInfo.Range = 11.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 4);

	tLightInfo.Diffuse = D3DXCOLOR(0.25f * 2.5f, 0.55f * 2.5f, 0.9f * 2.5f, 1.f);
	tLightInfo.Position = _vec3(-15.88f, 9.f, -30.92f) * 0.7f;
	tLightInfo.Range = 15.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 5);

	tLightInfo.Diffuse = D3DXCOLOR(0.25f * 2.5f, 0.55f * 2.5f, 0.9f * 2.5f, 1.f);
	tLightInfo.Position = _vec3(-1.26f, 7.f, -44.17f) * 0.7f;
	tLightInfo.Range = 12.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 6);

	tLightInfo.Diffuse = D3DXCOLOR(1.5f, 0.8f, 0.6f, 1.f);
	tLightInfo.Position = _vec3(22.73f, 6.f, -44.78f) * 0.7f;
	tLightInfo.Range = 8.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 7);

	tLightInfo.Diffuse = D3DXCOLOR(1.f * 1.5f, 0.6f * 1.4f, 0.5f * 1.4f, 1.f);
	tLightInfo.Position = _vec3(29.14f, 10.f, -69.88f) * 0.7f;
	tLightInfo.Range = 20.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 8);

	tLightInfo.Diffuse = D3DXCOLOR(0.25f * 2.5f, 0.55f * 2.5f, 0.9f * 2.5f, 1.f);
	tLightInfo.Position = _vec3(64.15f, 10.f, -71.27f) * 0.7f;
	tLightInfo.Range = 20.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 9);

	tLightInfo.Diffuse = D3DXCOLOR(1.5f, 0.5f, 0.3f, 1.f);
	tLightInfo.Position = _vec3(59.58f, 5.f, -47.34f) * 0.7f;
	tLightInfo.Range = 10.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 10);

	tLightInfo.Diffuse = D3DXCOLOR(0.25f * 2.5f, 0.55f * 2.5f, 0.9f * 2.5f, 1.f);
	tLightInfo.Position = _vec3(68.77f, 10.f, -99.32f) * 0.7f;
	tLightInfo.Range = 20.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 11);

	tLightInfo.Diffuse = D3DXCOLOR(1.5f * 1.5f, 0.8f * 1.5f, 0.6f * 1.5f, 1.f);
	tLightInfo.Position = _vec3(79.20f, 7.f, -119.68f) * 0.7f;
	tLightInfo.Range = 15.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 12);

	tLightInfo.Diffuse = D3DXCOLOR(1.5f * 1.5f, 0.8f * 1.5f, 0.6f * 1.5f, 1.f);
	tLightInfo.Position = _vec3(65.72f, 7.f, -131.42f) * 0.7f;
	tLightInfo.Range = 15.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 13);

	tLightInfo.Diffuse = D3DXCOLOR(1.5f * 1.5f, 0.8f * 1.5f, 0.6f * 1.5f, 1.f);
	tLightInfo.Position = _vec3(49.27f, 5.f, -144.73f) * 0.7f;
	tLightInfo.Range = 8.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 14);

	tLightInfo.Diffuse = D3DXCOLOR(0.25f * 2.5f, 0.55f * 2.5f, 0.9f * 2.5f, 1.f);
	tLightInfo.Position = _vec3(72.74f, 7.f, -151.83f) * 0.7f;
	tLightInfo.Range = 15.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 15);

	tLightInfo.Diffuse = D3DXCOLOR(1.5f * 1.5f, 0.8f * 1.5f, 0.6f * 1.5f, 1.f);
	tLightInfo.Position = _vec3(82.27f, 5.f, -174.01f) * 0.7f;
	tLightInfo.Range = 15.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 16);

	tLightInfo.Diffuse = D3DXCOLOR(0.25f * 2.5f, 0.55f * 2.5f, 0.9f * 2.5f, 1.f);
	tLightInfo.Position = _vec3(71.08f, 3.f, -175.84f) * 0.7f;
	tLightInfo.Range = 5.f;
	m_pLight[0] = Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 17);

	tLightInfo.Diffuse = D3DXCOLOR(1.5f * 1.5f, 0.8f * 1.5f, 0.6f * 1.5f, 1.f);
	tLightInfo.Position = _vec3(68.17f, 3.f, -179.94f) * 0.7f;
	tLightInfo.Range = 5.f;
	m_pLight[1] = Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 18);

	//tLightInfo.Diffuse = D3DXCOLOR(0.25f * 2.5f, 0.55f * 2.5f, 0.9f * 2.5f, 1.f);
	//tLightInfo.Position = _vec3(86.11f, 7.f, -190.54f) * 0.7f;
	//tLightInfo.Range = 15.f;
	//Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 19);

	tLightInfo.Diffuse = D3DXCOLOR(0.25f * 2.5f, 0.55f * 2.5f, 0.9f * 2.5f, 1.f);
	tLightInfo.Position = _vec3(118.58f, 5.f, -220.42f) * 0.7f;
	tLightInfo.Range = 10.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 20);

	tLightInfo.Diffuse = D3DXCOLOR(0.25f * 2.5f, 0.55f * 2.5f, 0.9f * 2.5f, 1.f);
	tLightInfo.Position = _vec3(144.48f, 5.f, -241.54f) * 0.7f;
	tLightInfo.Range = 10.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 21);

	tLightInfo.Diffuse = D3DXCOLOR(1.5f, 0.5f, 0.3f, 1.f);
	tLightInfo.Position = _vec3(167.82f, 5.f, -226.58f) * 0.7f;
	tLightInfo.Range = 7.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 22);

	tLightInfo.Diffuse = D3DXCOLOR(1.5f * 1.5f, 0.8f * 1.5f, 0.6f * 1.5f, 1.f);
	tLightInfo.Position = _vec3(173.47f, 15.f, -218.80f) * 0.7f;
	tLightInfo.Range = 15.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 23);

	tLightInfo.Diffuse = D3DXCOLOR(0.8f * 1.5f, 0.8f * 1.5f, 1.f * 1.5f, 1.f);
	tLightInfo.Position = _vec3(188.82f, 15.f, -198.66f) * 0.7f;
	tLightInfo.Range = 20.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 24);



	//LAVA
	tLightInfo.Diffuse = D3DXCOLOR(1.f * 5.5f, 0.8f * 5.5f, 0.6f * 5.5f, 1.f);
	tLightInfo.Position = _vec3(-10.74f, -15.f, -22.02f) * 0.7f;
	tLightInfo.Range = 20.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 33);

	tLightInfo.Diffuse = D3DXCOLOR(1.f * 5.5f, 0.8f * 5.5f, 0.6f * 5.5f, 1.f);
	tLightInfo.Position = _vec3(13.20f, -15.f, -43.08f) * 0.7f;
	tLightInfo.Range = 20.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 34);

	tLightInfo.Diffuse = D3DXCOLOR(1.f * 5.5f, 0.8f * 5.5f, 0.5f * 5.5f, 1.f);
	tLightInfo.Position = _vec3(20.56f, -20.f, -61.83f) * 0.7f;
	tLightInfo.Range = 20.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 35);

	tLightInfo.Diffuse = D3DXCOLOR(1.f * 5.5f, 0.8f * 5.5f, 0.5f * 5.5f, 1.f);
	tLightInfo.Position = _vec3(41.53f, -15.f, -57.16f) * 0.7f;
	tLightInfo.Range = 20.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 36);

	tLightInfo.Diffuse = D3DXCOLOR(1.f * 5.5f, 0.8f * 5.5f, 0.5f * 5.5f, 1.f);
	tLightInfo.Position = _vec3(40.97f, -20.f, -83.73f) * 0.7f;
	tLightInfo.Range = 20.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 37);

	tLightInfo.Diffuse = D3DXCOLOR(1.f * 5.5f, 0.8f * 5.5f, 0.5f * 5.5f, 1.f);
	tLightInfo.Position = _vec3(100.45f, -15.f, -207.15f) * 0.7f;
	tLightInfo.Range = 20.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 38);

	tLightInfo.Diffuse = D3DXCOLOR(1.f * 3.5f, 0.8f * 3.5f, 0.5f * 3.5f, 1.f);
	tLightInfo.Position = _vec3(140.41f, -15.f, -227.55f) * 0.7f;
	tLightInfo.Range = 20.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 39);

	tLightInfo.Diffuse = D3DXCOLOR(1.f * 3.5f, 0.8f * 3.5f, 0.5f * 3.5f, 1.f);
	tLightInfo.Position = _vec3(126.62f, -15.f, -239.60f) * 0.7f;
	tLightInfo.Range = 20.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 40);

	tLightInfo.Diffuse = D3DXCOLOR(1.f * 3.5f, 0.8f * 3.5f, 0.5f * 3.5f, 1.f);
	tLightInfo.Position = _vec3(182.55f, -15.f, -207.83f) * 0.7f;
	tLightInfo.Range = 23.f;
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 41);


	return S_OK;
}

CChessField * CChessField::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CChessField*		pInstance = new CChessField(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
		Safe_Release(pInstance);

	return pInstance;
}

void CChessField::Set_CrusherStart()
{
	m_pLight[0]->Set_LightRender(false);
	m_pLight[1]->Set_LightRender(false);
}

void CChessField::Free(void)
{
	// 삭제
	CCollisionMgr::DestroyInstance();

	for (_uint i = 0; i < m_vecObject.size(); ++i)
		Safe_Delete_Array(m_vecObject[i]);

	m_vecObject.clear();
	m_vecObject.shrink_to_fit();
	Engine::Delete_AllResource(RESOURCE_CF);
	Engine::CScene::Free();
}
