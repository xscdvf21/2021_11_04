#include "stdafx.h"
#include "Mage.h"
#include "IceBullet.h"
#include "Knight.h"
#include "MorningStar.h"
#include "Arbalist.h"
#include "CrossBow.h"
#include "Export_Function.h"
#include "MainCamera.h"
#include "CollisionMgr.h"
#include "ChessDuoDoor.h"
#include "Staff.h"
#include "Cody_Wizard.h"
#include "May_Castle.h"


//공격 모션은 AttackStart 이거 하나만 쓰는듯.

//기본적으로 텔레포트는 최대체력의 25프로 까일때마다 움직이는듯.
CMage::CMage(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEnermy(pGraphicDev)
	, m_bCreateOne(false), m_bCreateThree(false), m_bCreateTwo(false)
	, m_iBulltCount(0)
	, m_iAniIndex(0)
	, m_bBulletCreate(false), m_bAttack(false), m_bMoveOne(false), m_bMoveTwo(false), m_bMoveThree(false), m_bMoveFour(false)
	, m_bMoveStart(false), m_bAniStop(false), m_bMoveTime(0.f)
	, m_iKnightCount(0), m_iArbalistCount(0)
{
}

CMage::~CMage()
{
}


HRESULT CMage::Ready_Object(void * pArg, void* pOBJINFO)
{
	m_fColDis = 1.5f;

	ZeroMemory(&m_tColInfo, sizeof(SPHERE_INFO));
	m_tColInfo.fRadius = m_fColDis;
	FAILED_CHECK_RETURN(Add_Component(pOBJINFO), E_FAIL);

	if (nullptr != pArg)
	{
		m_pStaff = (CStaff*)pArg;
		m_pStaff->AddRef();
	}

	m_iHPMax = 30;
	m_iHP = 30;

	m_pMeshCom->Set_Animationset(animID::Attack_Idle);
	Create_DefaultUI(2.5f, 3.f);

	return S_OK;
}

_int CMage::Update_Object(const _float & fTimeDelta)
{
	Update_DefaultUI(fTimeDelta);

	m_iAniIndex = m_pMeshCom->Get_AniIndex();
	if (m_iHP <= m_iHPMax * 0.66f && !m_bMoveOne)
	{
		m_bCreateOne = true;
		m_bMoveOne = true;
	}
	else if (m_iHP <= m_iHPMax * 0.33f && !m_bMoveTwo)
	{
		m_bCreateTwo = true;
		m_bMoveTwo = true;
	}
	else if (m_iHP <= m_iHPMax * 0.25f && !m_bMoveThree)
	{
		m_bCreateThree = true;
		m_bMoveThree = true;
	}

	_vec3 vPos;
	vPos = m_pTransformCom->Get_Position();

	if (vPos.y >= 1.5f)
	{
		m_pTransformCom->Set_Pos(vPos.x, 1.f, vPos.z);
	}

	MonsterCreate(fTimeDelta);

	if (m_iHP <= 0)
	{
		Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);

		m_pMeshCom->Set_Animationset(Dead);
		m_pMeshCom->Play_Animationset(fTimeDelta);

		if (m_pMeshCom->Is_AnimationSetEnd(0.1f))
		{
			m_bDead = true;
			m_pStaff->Set_Dead();

			CChessDuoDoor*		pDoorLeft = (CChessDuoDoor*)Engine::Get_GameObject(L"GameObject", L"ChessDuoDoorLeft");
			pDoorLeft->Set_Open(true);

			pDoorLeft = (CChessDuoDoor*)Engine::Get_GameObject(L"GameObject", L"ChessDuoDoorRight");
			pDoorLeft->Set_Open(true);

		}
		if (m_bDead)
		{
			Engine::Get_GameObject(L"GameObject", L"Cody")->Is<CCodyWizard>()->Set_SavePoint({ 25.4484901f, 0.0f, -51.3361664f });
			Engine::Get_GameObject(L"GameObject", L"May")->Is<CMayCastle>()->Set_SavePoint({ 25.4484901f, 0.0f, -51.3361664f });

			Dead_Effect();
			Delete_UI();
			m_pRigidActor->setActorFlag(PxActorFlag::eDISABLE_SIMULATION, true);
			return OBJ_DEAD;
		}

		return 0;
	}


	Engine::CGameObject::Update_Object(fTimeDelta);

	if (m_bHit)
		Hit_State(fTimeDelta);
	else /*if(m_bAttack)*/
		Attack(fTimeDelta);


	m_pMeshCom->Play_Animationset(fTimeDelta);
	Tick_Damage(fTimeDelta);

	CCollisionMgr::GetInstance()->Add_EnermyList(this);

	Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);
	Tick_Damage(fTimeDelta);


	return OBJ_NOEVENT;
}

_int CMage::LateUpdate_Object(const _float & fTimeDelta)
{
	PxTransform pxTransform = m_pRigidActor->getGlobalPose();
	m_pTransformCom->Set_Pos(pxTransform);
	return _int();
}

void CMage::Render_Object(const _int & iIndex)
{
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect, iIndex), );

	_uint	iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);		// 1인자 : 현재 쉐이더 파일이 갖고 있는 pass의 최대 개수, 2인자 : 시작하는 방식(default)
										//pEffect->BeginPass(0);
	m_pMeshCom->Render_Meshes_VTF(pEffect, 0);

	pEffect->End();

	Safe_Release(pEffect);

	CMainCamera* pCamera = (CMainCamera*)Engine::Get_GameObject(L"GameObject", L"MainCamera");
	pCamera->Update_View_Proj();

	const D3DXFRAME_DERIVED* pBone = m_pMeshCom->Get_FrameByName("Spine2");
	_float4x4 matBoneMatrix = pBone->CombinedTranformationMatrix;

	//Spine2
	m_tColInfo.matWorld = matBoneMatrix * *m_pTransformCom->Get_WorldMatrix();
	m_pColliderCom->Render_Collider(m_tColInfo.matWorld, false);
}

HRESULT CMage::Add_Component(void* pArg)
{
	Engine::CComponent*		pComponent = nullptr;

	OBJINFO tObjINFO;
	ZeroMemory(&tObjINFO, sizeof(OBJINFO));
	if (nullptr != pArg)
	{
		memcpy(&tObjINFO, pArg, sizeof(OBJINFO));
	}

	// CDynamicMesh
	pComponent = m_pMeshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Clone_Resource(RESOURCE_CF, L"ToyMage"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Mesh", pComponent);

	// Transform
	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	// Calculator
	pComponent = m_pCalculatorCom = dynamic_cast<Engine::CCalculator*>(Engine::Clone_Prototype(L"Proto_Calculator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Calculator", pComponent);

	// Collider
	pComponent = m_pColliderCom = Engine::CCollider::Create(m_pGraphicDev, m_fColDis, _vec4(50.f, 244.f, 234.f, 0.f));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Collider", pComponent);

	// Shader
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_VTF"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);


	m_pTransformCom->Set_Scale(tObjINFO.vScale);
	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(tObjINFO.vAngle.x));
	m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(tObjINFO.vAngle.y));
	m_pTransformCom->Rotation(ROT_Z, D3DXToRadian(tObjINFO.vAngle.z));
	m_pTransformCom->Set_Pos(tObjINFO.vPos);

	//PhysX
	auto* pPhysics = Engine::Get_Physics();
	_vec3 vPos = m_pTransformCom->Get_Position();
	PxTransform tTransform(vPos.x, vPos.y, vPos.z);

	//정지마찰계수 멈춰있을경우 다른충돌체와 마찰하는정도
	//움직임마찰계수 움직일때 다른충돌체와 마찰정도
	//탄성계수 0~1값
	PxMaterial* pMaterial = pPhysics->createMaterial(0.5f, 0.5f, 0.0f);
	PxShape* pShape = pPhysics->createShape(PxBoxGeometry(1, 1, 1), *pMaterial, true);

	//밑 두개중 하나는 false 둘다 true안?
	pShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
	pShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, false);
	pShape->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, true);
	m_pRigidActor = PxCreateDynamic(*pPhysics, tTransform, *pShape, 1);

	auto* pBody = static_cast<PxRigidDynamic*>(m_pRigidActor);

	// 트랜스폼은 가지고있지만 움직이지 않는애들
	pBody->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, false);

	// 그래비티를 끈다.
	pBody->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);

	// 질량설정
	pBody->setMass(5);
	pBody->setName((char*)this);
	pShape->setName((char*)this);
	pBody->setRigidDynamicLockFlags(PxRigidDynamicLockFlag::eLOCK_ANGULAR_X | PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z);
	//이동 제동력
	pBody->setLinearDamping(0.05f);

	return S_OK;
}

HRESULT CMage::SetUp_ConstantTable(LPD3DXEFFECT & pEffect, const _int & iIndex)
{
	_matrix			matWorld, matView, matProj;

	m_pTransformCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	pEffect->SetVector("g_vColor", &_vec4(255.f, 255.f, 255.f, 255.f));
	pEffect->SetFloat("g_fAmount", 0.f);
	pEffect->SetFloat("g_fMaterialIndex", 0.f);
	pEffect->CommitChanges();

	return S_OK;
}

void CMage::Attack(const _float & fTimeDelta)
{
	

	Engine::CTransform*		pMayTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(L"GameObject", L"May", L"Com_Transform", Engine::ID_DYNAMIC));
	Engine::CTransform*		pCodyTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(L"GameObject", L"Cody", L"Com_Transform", Engine::ID_DYNAMIC));

	_vec3 vMayPos; //메이 위치
	_vec3 vCodyPos; //코디 위치
	_vec3 vPos; //현재 몬스터(자신)	위치.
	_vec3 vLook; //현재 몬스터(자신)	룩방향
	_vec3 vRight; //현재 몬스터(자신) 라이트
	_vec3 vLeft; //현재 몬스터(자신) 레프트

	m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
	m_pTransformCom->Get_Info(INFO_RIGHT, &vRight);

	vMayPos = pMayTransformCom->Get_Position();
	vCodyPos = pCodyTransformCom->Get_Position();
	vPos = m_pTransformCom->Get_Position();
	vPos.y = 0.f;

	_vec3 vMayDir;	//메이와의 거리
	_vec3 vCodyDir; //코디와의 거리

	_float fMayDir;
	_float fCodyDir;

	vMayDir = vMayPos - vPos;
	vCodyDir = vCodyPos - vPos;

	fMayDir = D3DXVec3Length(&vMayDir);
	fCodyDir = D3DXVec3Length(&vCodyDir);


	D3DXVec3Normalize(&vLook, &vLook);
	D3DXVec3Normalize(&vRight, &vRight);
	vLeft = vRight * -1.f;


	//거리가 둘중에 하나라도 10.f 보다 작으면, 플레이어 인지.
	if (fMayDir <= 15.f || fCodyDir <= 15.f)
	{
		if (fMayDir < fCodyDir && m_pTargetPlayer == nullptr)
		{
			m_pTargetPlayer = dynamic_cast<CGameObject*>(Engine::Get_GameObject(L"GameObject", L"May"));
		}
		else if (fCodyDir < fMayDir && m_pTargetPlayer == nullptr)
		{
			m_pTargetPlayer = dynamic_cast<CGameObject*>(Engine::Get_GameObject(L"GameObject", L"Cody"));
		}
	}

	if (m_pTargetPlayer != nullptr)
	{

		//타겟의 트랜스를 가져옴.
		CTransform*		pTargetTransCom = dynamic_cast<CTransform*>(m_pTargetPlayer->Get_Component(L"Com_Transform", Engine::ID_DYNAMIC));

		_vec3	vTargetPos;
		_vec3	vTargetDir;
		_float	 fTargetDir;


		vTargetPos = pTargetTransCom->Get_Position();
		vTargetPos.y = 0.f;
		vTargetDir = vTargetPos - vPos;

		fTargetDir = D3DXVec3Length(&vTargetDir);
		vTargetDir.y = 0.f;
		D3DXVec3Normalize(&vTargetDir, &vTargetDir);

		m_fAngle = D3DXToDegree(acosf(D3DXVec3Dot(&vLook, &vTargetDir)));

		_float fRightAngle = D3DXVec3Dot(&vTargetDir, &vRight);
		_float fLeftAngle = D3DXVec3Dot(&vTargetDir, &vLeft);

		if (m_fAngle >= D3DXToDegree(m_fTurnSpeed))
		{
			if (fRightAngle < 0 && fLeftAngle > 0)
			{
				m_pTransformCom->Rotation(ROT_Y, -m_fTurnSpeed);
			}
			else if (fRightAngle > 0 && fLeftAngle < 0)
				m_pTransformCom->Rotation(ROT_Y, m_fTurnSpeed);
		}

		if (fTargetDir <= 14.f) //가까워지면 공격
		{
			
			m_fAttackTime += fTimeDelta;

			if (m_fAttackTime > 3.f)
			{
				m_pMeshCom->Set_Animationset(animID::Attack_Start);
				if (!m_bBulletCreate)
				{
					CLayer*				pLayer = nullptr;
					pLayer = Engine::Get_Layer(L"GameObject");
					Engine::CGameObject*		pGameObject = nullptr;
					///////////////////////////정면
					_vec3 vLook;
					_vec3 vPos;

					TCHAR szbuf[MAX_PATH] = L"ToyMage_Bullet%d";
					TCHAR StrObjName[MAX_PATH] = L"";

					vPos = m_pTransformCom->Get_Position();
					m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
					D3DXVec3Normalize(&vLook, &vLook);
					vLook = vLook * 1.3f;
					vPos.y += 1.f;
					vPos = vPos + vLook;
					D3DXVec3Normalize(&vLook, &vLook);

					pGameObject = CIceBullet::Create(m_pGraphicDev, vPos, vLook);

					NULL_CHECK_RETURN(pGameObject, );

					wsprintf(StrObjName, szbuf, m_iBulltCount);
					FAILED_CHECK_RETURN(pLayer->Add_GameObject(StrObjName, pGameObject), );
					m_iBulltCount++;


					_vec3 vLook1;
					_vec3 vPos1;
					_matrix matRotY;
					D3DXMatrixRotationY(&matRotY, D3DXToRadian(15));
					vPos1 = m_pTransformCom->Get_Position();
					vPos1.y += 1.f;
					m_pTransformCom->Get_Info(INFO_LOOK, &vLook1);
					D3DXVec3Normalize(&vLook1, &vLook1);
					D3DXVec3TransformNormal(&vLook1, &vLook1, &matRotY);

					vLook1 = vLook1 * 1.3f;
					vPos1 = vPos1 + vLook1;
					D3DXVec3Normalize(&vLook1, &vLook1);

					pGameObject = CIceBullet::Create(m_pGraphicDev, vPos1, vLook1);

					NULL_CHECK_RETURN(pGameObject, );

					wsprintf(StrObjName, szbuf, m_iBulltCount);
					FAILED_CHECK_RETURN(pLayer->Add_GameObject(StrObjName, pGameObject), );
					m_iBulltCount++;

					_vec3 vLook2;
					_vec3 vPos2;
					_matrix matRotY2;
					D3DXMatrixRotationY(&matRotY2, D3DXToRadian(-15));
					vPos2 = m_pTransformCom->Get_Position();
					vPos2.y += 1.f;
					m_pTransformCom->Get_Info(INFO_LOOK, &vLook2);
					D3DXVec3Normalize(&vLook2, &vLook2);
					D3DXVec3TransformNormal(&vLook2, &vLook2, &matRotY2);

					vLook2 = vLook2 * 1.3f;
					vPos2 = vPos2 + vLook2;
					D3DXVec3Normalize(&vLook2, &vLook2);

					pGameObject = CIceBullet::Create(m_pGraphicDev, vPos2, vLook2);
					NULL_CHECK_RETURN(pGameObject, );

					wsprintf(StrObjName, szbuf, m_iBulltCount);
					FAILED_CHECK_RETURN(pLayer->Add_GameObject(StrObjName, pGameObject), );
					m_iBulltCount++;
					m_bBulletCreate = true;
				}
			}

			if (m_iAniIndex == animID::Attack_Start && m_pMeshCom->Is_AnimationSetEnd(0.1f))
			{

				m_fAttackTime = 0.f;
				m_pMeshCom->Set_Animationset(animID::Attack_Idle, 1.f, 0.1f);
				m_bBulletCreate = false;
			}
		
		}

		if (fTargetDir >= 16.f) //거리가 멀어져서 타겟이 사라지면 다시 아이들 상태
		{
			m_pMeshCom->Set_Animationset(animID::Attack_Idle);
			m_pTargetPlayer = nullptr;
		}
	}
}

void CMage::MonsterCreate(const _float & fTimeDelta)
{

	_tchar tagTemp[MAX_PATH];
	_tchar tagWeaponTemp[MAX_PATH];
	OBJINFO tTemp;
	OBJINFO pObj;
	CMorningStar* pMornigStar = nullptr;
	CCrossBow* pCrossBow = nullptr;

	CLayer*				pLayer = nullptr;
	pLayer = Engine::Get_Layer(L"GameObject");
	Engine::CGameObject*		pGameObject = nullptr;

	if (m_bCreateOne)
	{
		wsprintf(tagTemp, L"Mage_ToyKnight %d", m_iKnightCount);
		wsprintf(tagWeaponTemp, L"Mage_ToyKnight_MorningStar %d", m_iKnightCount);
		tTemp.tParentBone = tagTemp;

		pMornigStar = nullptr;
		pGameObject = pMornigStar = CMorningStar::Create(m_pGraphicDev, &tTemp);
		NULL_CHECK_RETURN(pGameObject, );
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagWeaponTemp, pGameObject), );

	
		pObj.vScale = { 0.01f, 0.01f, 0.01f };
		pObj.vAngle = { 0.f, 45.f,0.f };
		pObj.vPos = { 26.4f , 1.f, -51.5f };
		pObj.tParentBone = tTemp.tParentBone;
		pGameObject = CKnight::Create(m_pGraphicDev, pMornigStar, &pObj);
		NULL_CHECK_RETURN(pGameObject, );
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObj.tParentBone, pGameObject, Engine::Get_Scene()), );
		Engine::Get_Scene()->setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster,FilterGroup::eDefaultTrigger | FilterGroup::eWall | FilterGroup::eMonster | FilterGroup::eGround);

		CKnight*	pKnight = (CKnight*)pGameObject;
		pKnight->Set_Move(true);

		m_iKnightCount++;

		wsprintf(tagTemp, L"Mage_ToyKnight %d", m_iKnightCount);
		wsprintf(tagWeaponTemp, L"Mage_ToyKnight_MorningStar %d", m_iKnightCount);
		tTemp.tParentBone = tagTemp;

		pMornigStar = nullptr;
		pGameObject = pMornigStar = CMorningStar::Create(m_pGraphicDev, &tTemp);
		NULL_CHECK_RETURN(pGameObject, );
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagWeaponTemp, pGameObject), );


		pObj.vScale = { 0.01f, 0.01f, 0.01f };
		pObj.vAngle = { 0.f, 90.f,0.f };
		pObj.vPos = { 26.4f , 1.f, -53.f };
		pObj.tParentBone = tTemp.tParentBone;
		pGameObject = CKnight::Create(m_pGraphicDev, pMornigStar, &pObj);
		NULL_CHECK_RETURN(pGameObject, );
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObj.tParentBone, pGameObject, Engine::Get_Scene()), );
		Engine::Get_Scene()->setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eDefaultTrigger | FilterGroup::eWall | FilterGroup::eMonster | FilterGroup::eGround);

		pKnight = (CKnight*)pGameObject;
		pKnight->Set_Move(true);

		m_iKnightCount++;

		wsprintf(tagTemp, L"Mage_ToyKnight %d", m_iKnightCount);
		wsprintf(tagWeaponTemp, L"Mage_ToyKnight_MorningStar %d", m_iKnightCount);
		tTemp.tParentBone = tagTemp;

		pMornigStar = nullptr;
		pGameObject = pMornigStar = CMorningStar::Create(m_pGraphicDev, &tTemp);
		NULL_CHECK_RETURN(pGameObject, );
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagWeaponTemp, pGameObject), );


		pObj.vScale = { 0.01f, 0.01f, 0.01f };
		pObj.vAngle = { 0.f, 135.f,0.f };
		pObj.vPos = { 26.4f , 1.f, -50.f };
		pObj.tParentBone = tTemp.tParentBone;
		pGameObject = CKnight::Create(m_pGraphicDev, pMornigStar, &pObj);
		NULL_CHECK_RETURN(pGameObject, );
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObj.tParentBone, pGameObject, Engine::Get_Scene()), );
		Engine::Get_Scene()->setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eDefaultTrigger | FilterGroup::eWall | FilterGroup::eMonster | FilterGroup::eGround);

		pKnight = (CKnight*)pGameObject;
		pKnight->Set_Move(true);

		m_iKnightCount++;

		wsprintf(tagTemp, L"Mage_OneToyArbalist %d", m_iArbalistCount);
		wsprintf(tagWeaponTemp, L"Mage_OneToyArbalist_CrossBow %d", m_iArbalistCount);
		tTemp.tParentBone = tagTemp;

		pCrossBow = nullptr;
		pGameObject = pCrossBow = CCrossBow::Create(m_pGraphicDev, &tTemp);
		NULL_CHECK_RETURN(pGameObject, );
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagWeaponTemp, pGameObject), );


	
		pObj.vScale = { 0.015f, 0.015f, 0.015f };
		pObj.vAngle = { 0.f, 180.f,0.f };
		pObj.vPos = { 22.5f, 1.f, -41.f };
		pObj.tParentBone = tTemp.tParentBone;
		pGameObject = CArbalist::Create(m_pGraphicDev, pCrossBow, &pObj);
		NULL_CHECK_RETURN(pGameObject, );
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObj.tParentBone, pGameObject, Engine::Get_Scene()), );
		Engine::Get_Scene()->setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eDefaultTrigger);


		CArbalist*	pArbalist = (CArbalist*)pGameObject;

		pArbalist->Set_Move(true);


		m_iArbalistCount++;

		wsprintf(tagTemp, L"Mage_OneToyArbalist %d", m_iArbalistCount);
		wsprintf(tagWeaponTemp, L"Mage_OneToyArbalist_CrossBow %d", m_iArbalistCount);
		tTemp.tParentBone = tagTemp;

		pCrossBow = nullptr;
		pGameObject = pCrossBow = CCrossBow::Create(m_pGraphicDev, &tTemp);
		NULL_CHECK_RETURN(pGameObject, );
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagWeaponTemp, pGameObject), );


		pObj.vScale = { 0.015f, 0.015f, 0.015f };
		pObj.vAngle = { 0.f, 180.f,0.f };
		pObj.vPos = { 24.5f, 1.f, -41.f };
		pObj.tParentBone = tTemp.tParentBone;
		pGameObject = CArbalist::Create(m_pGraphicDev, pCrossBow, &pObj);
		NULL_CHECK_RETURN(pGameObject, );
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObj.tParentBone, pGameObject, Engine::Get_Scene()), );
		Engine::Get_Scene()->setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eDefaultTrigger);

		pArbalist = (CArbalist*)pGameObject;
		pArbalist->Set_Move(true);


		m_iArbalistCount++;

		m_bCreateOne = false;
	}
	else if (m_bCreateTwo)
	{

		wsprintf(tagTemp, L"Mage_ToyKnight %d", m_iKnightCount);
		wsprintf(tagWeaponTemp, L"Mage_ToyKnight_MorningStar %d", m_iKnightCount);
		tTemp.tParentBone = tagTemp;

		pMornigStar = nullptr;
		pGameObject = pMornigStar = CMorningStar::Create(m_pGraphicDev, &tTemp);
		NULL_CHECK_RETURN(pGameObject, );
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagWeaponTemp, pGameObject), );


		pObj.vScale = { 0.01f, 0.01f, 0.01f };
		pObj.vAngle = { 0.f, 45.f,0.f };
		pObj.vPos = { 15.4f , 1.f, -51.5f };
		pObj.tParentBone = tTemp.tParentBone;
		pGameObject = CKnight::Create(m_pGraphicDev, pMornigStar, &pObj);
		NULL_CHECK_RETURN(pGameObject, );
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObj.tParentBone, pGameObject, Engine::Get_Scene()), );
		Engine::Get_Scene()->setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eDefaultTrigger | FilterGroup::eWall | FilterGroup::eMonster | FilterGroup::eGround);

		CKnight*	pKnight = (CKnight*)pGameObject;
		pKnight->Set_Move(true);

		m_iKnightCount++;

		wsprintf(tagTemp, L"Mage_ToyKnight %d", m_iKnightCount);
		wsprintf(tagWeaponTemp, L"Mage_ToyKnight_MorningStar %d", m_iKnightCount);
		tTemp.tParentBone = tagTemp;

		pMornigStar = nullptr;
		pGameObject = pMornigStar = CMorningStar::Create(m_pGraphicDev, &tTemp);
		NULL_CHECK_RETURN(pGameObject, );
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagWeaponTemp, pGameObject), );


		pObj.vScale = { 0.01f, 0.01f, 0.01f };
		pObj.vAngle = { 0.f, 90.f,0.f };
		pObj.vPos = { 15.4f , 1.f, -53.f };
		pObj.tParentBone = tTemp.tParentBone;
		pGameObject = CKnight::Create(m_pGraphicDev, pMornigStar, &pObj);
		NULL_CHECK_RETURN(pGameObject, );
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObj.tParentBone, pGameObject, Engine::Get_Scene()), );
		Engine::Get_Scene()->setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eDefaultTrigger | FilterGroup::eWall | FilterGroup::eMonster | FilterGroup::eGround);

		pKnight = (CKnight*)pGameObject;
		pKnight->Set_Move(true);

		m_iKnightCount++;

		wsprintf(tagTemp, L"Mage_ToyKnight %d", m_iKnightCount);
		wsprintf(tagWeaponTemp, L"Mage_ToyKnight_MorningStar %d", m_iKnightCount);
		tTemp.tParentBone = tagTemp;

		pMornigStar = nullptr;
		pGameObject = pMornigStar = CMorningStar::Create(m_pGraphicDev, &tTemp);
		NULL_CHECK_RETURN(pGameObject, );
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagWeaponTemp, pGameObject), );


		pObj.vScale = { 0.01f, 0.01f, 0.01f };
		pObj.vAngle = { 0.f, 135.f,0.f };
		pObj.vPos = { 15.4f , 1.f, -50.f };
		pObj.tParentBone = tTemp.tParentBone;
		pGameObject = CKnight::Create(m_pGraphicDev, pMornigStar, &pObj);
		NULL_CHECK_RETURN(pGameObject, );
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObj.tParentBone, pGameObject, Engine::Get_Scene()), );
		Engine::Get_Scene()->setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eDefaultTrigger | FilterGroup::eWall | FilterGroup::eMonster | FilterGroup::eGround);

		pKnight = (CKnight*)pGameObject;
		pKnight->Set_Move(true);

		m_iKnightCount++;

		wsprintf(tagTemp, L"Mage_OneToyArbalist %d", m_iArbalistCount);
		wsprintf(tagWeaponTemp, L"Mage_OneToyArbalist_CrossBow %d", m_iArbalistCount);
		tTemp.tParentBone = tagTemp;

		pCrossBow = nullptr;
		pGameObject = pCrossBow = CCrossBow::Create(m_pGraphicDev, &tTemp);
		NULL_CHECK_RETURN(pGameObject, );
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagWeaponTemp, pGameObject), );


		pObj.vScale = { 0.015f, 0.015f, 0.015f };
		pObj.vAngle = { 0.f, 0.f,0.f };
		pObj.vPos = { 20.5f, 1.f, -58.f };
		pObj.tParentBone = tTemp.tParentBone;
		pGameObject = CArbalist::Create(m_pGraphicDev, pCrossBow, &pObj);
		NULL_CHECK_RETURN(pGameObject, );
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObj.tParentBone, pGameObject, Engine::Get_Scene()), );
		Engine::Get_Scene()->setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eDefaultTrigger);


		CArbalist*	pArbalist = (CArbalist*)pGameObject;
		pArbalist->Set_Move(true);


		m_iArbalistCount++;

		wsprintf(tagTemp, L"Mage_OneToyArbalist %d", m_iArbalistCount);
		wsprintf(tagWeaponTemp, L"Mage_OneToyArbalist_CrossBow %d", m_iArbalistCount);
		tTemp.tParentBone = tagTemp;

		pCrossBow = nullptr;
		pGameObject = pCrossBow = CCrossBow::Create(m_pGraphicDev, &tTemp);
		NULL_CHECK_RETURN(pGameObject, );
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagWeaponTemp, pGameObject), );



		pObj.vScale = { 0.015f, 0.015f, 0.015f };
		pObj.vAngle = { 0.f, 0.f,0.f };
		pObj.vPos = { 23.0f, 1.f, -58.f };
		pObj.tParentBone = tTemp.tParentBone;
		pGameObject = CArbalist::Create(m_pGraphicDev, pCrossBow, &pObj);
		NULL_CHECK_RETURN(pGameObject, );
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObj.tParentBone, pGameObject, Engine::Get_Scene()), );
		Engine::Get_Scene()->setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eDefaultTrigger);


		pArbalist = (CArbalist*)pGameObject;
		pArbalist->Set_Move(true);


		m_iArbalistCount++;

		m_bCreateTwo = false;
	}
	else if (m_bMoveThree)
	{
		m_bCreateThree = false;
	}
}

void CMage::Set_Damage(_int iDamage)
{
	CEnermy::Set_Damage(iDamage);

	Hit_Effect(m_pTransformCom->Get_Position() + _vec3(0.f, 1.f, 0.f), 3.f);
	Hit_Damage(1.5f);
}



void CMage::Hit_State(const _float & fTimeDelta)
{
	//m_pMeshCom->Set_Animationset(Damage);
	//if (m_pMeshCom->Is_AnimationSetEnd(0.1f))
	//{
		m_bHit = false;
		m_pTargetPlayer = nullptr;
	//}
}

CMage * CMage::Create(LPDIRECT3DDEVICE9 pGraphicDev, void * pArg, void* pOBJINFO)
{
	CMage*	pInstance = new CMage(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pArg, pOBJINFO)))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMage::Free(void)
{
	Safe_Release(m_pStaff);
	CEnermy::Free();
}
