#include "stdafx.h"
#include "Arbalist.h"
#include "Cody_Wizard.h"
#include "May_Castle.h"
#include "Arrow.h"
#include "Export_Function.h"
#include "MainCamera.h"
#include "CollisionMgr.h"
#include "CrossBow.h"

CArbalist::CArbalist(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEnermy(pGraphicDev)
	, m_fAngle(0.f)
	, m_bMove(false), m_fTurnSpeed(D3DXToRadian(2))
{
}

CArbalist::~CArbalist()
{
}

HRESULT CArbalist::Ready_Object(void * pArg, void* pOBJINFO)
{
	m_iHPMax = 15;
	m_iHP = m_iHPMax;
	m_fColDis = 1.f;

	ZeroMemory(&m_tColInfo, sizeof(SPHERE_INFO));
	m_tColInfo.fRadius = m_fColDis;

	FAILED_CHECK_RETURN(Add_Component(pArg), E_FAIL);

	if (nullptr != pArg)
	{
		m_pCrossBow = (CCrossBow*)pArg;
		m_pCrossBow->AddRef();
	}


	OBJINFO tOBJINFO;
	ZeroMemory(&tOBJINFO, sizeof(OBJINFO));
	if (nullptr != pOBJINFO)
	{
		memcpy(&tOBJINFO, pOBJINFO,sizeof(OBJINFO));
	}

	lstrcpy(m_tArrowTag, tOBJINFO.tParentBone);

	m_pTransformCom->Set_Scale(tOBJINFO.vScale);
	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(tOBJINFO.vAngle.x));
	m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(tOBJINFO.vAngle.y));
	m_pTransformCom->Rotation(ROT_Z, D3DXToRadian(tOBJINFO.vAngle.z));
	m_pTransformCom->Set_Pos(tOBJINFO.vPos);

	m_pMeshCom->Set_Animationset((animID::Idle));


	PhysicsCreate();
	Create_DefaultUI(1.4f, 2.f);

	return S_OK;
}

_int CArbalist::Update_Object(const _float & fTimeDelta)
{
	if (m_bStart)
	{
		Create_Cone(L"Cone");
		m_bStart = false;
	}

	m_fTimeDelta = fTimeDelta;
	Update_DefaultUI(fTimeDelta);

	_vec3 vPos;
	vPos = m_pTransformCom->Get_Position();

	if (vPos.y >= 1.5f)
	{
		m_pTransformCom->Set_Pos(vPos.x, 1.f, vPos.z);
	}

	if (m_iHP <= 0)
	{
		if (m_bDead)
		{
			Dead_Effect();

			CRandoms Random;
			_uint	iRandom = Random(1, 3);
			if (iRandom == 1)
			{
				Engine::StopSound(CSoundMgr::CHANNELID::ToyArbalist);
				Engine::PlaySoundW(L"ToyKnightDeath1.ogg", CSoundMgr::CHANNELID::ToyArbalist);
			}
			else if (iRandom == 2)
			{
				Engine::StopSound(CSoundMgr::CHANNELID::ToyArbalist);
				Engine::PlaySoundW(L"ToyKnightDeath2.ogg", CSoundMgr::CHANNELID::ToyArbalist);
			}
			else if (iRandom == 3)
			{
				Engine::StopSound(CSoundMgr::CHANNELID::ToyArbalist);
				Engine::PlaySoundW(L"ToyKnightDeath3.ogg", CSoundMgr::CHANNELID::ToyArbalist);
			}

			m_pRigidActor->setActorFlag(PxActorFlag::eDISABLE_SIMULATION, true);
			Delete_UI();
			return OBJ_DEAD;
		}
		Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);

		m_pMeshCom->Set_Animationset(Dead);
		m_pMeshCom->Play_Animationset(fTimeDelta);

		if (m_pMeshCom->Is_AnimationSetEnd(0.1f))
		{
			m_pCrossBow->Set_Dead();
			m_bDead = true;
		}

		return 0;
	}

	Engine::CGameObject::Update_Object(fTimeDelta);

	if (m_bHit)
	{
		Hit_State(fTimeDelta);
		m_bMove = true;
	}
	else if(m_bMove)
		Move(fTimeDelta);

	m_pMeshCom->Play_Animationset(fTimeDelta);

	CCollisionMgr::GetInstance()->Add_EnermyList(this);
	Tick_Damage(fTimeDelta);
	Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);

	if (m_pCone)
	{
		m_pCone->Update_Object(fTimeDelta);

		_matrix matWorld;
		const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("RightHand");
		const _matrix* pMatrix = &pFrame->CombinedTranformationMatrix;


		m_pTransformCom->Get_WorldMatrix(&matWorld);
		matWorld._42 = + 1.f;
		//matWorld = *pMatrix * matWorld;


		CTransform* pConeTrans = (CTransform*)m_pCone->Get_Component(L"Com_Transform", ID_DYNAMIC);
		_vec3 vPos = { matWorld._41, matWorld._42, matWorld._43 };
		pConeTrans->Set_ParentMatrix(/**pMatrix **/ matWorld);

		//pConeTrans->Set_AngleX(D3DXToRadian(-90.f));
		pConeTrans->Set_AngleY(D3DXToRadian(-90.f));
		//pConeTrans->Set_AngleZ(D3DXToRadian(90.f));
	}

	//PxTransform pxTransform = m_pRigidActor->getGlobalPose();
	//pxTransform.p.x = vPos.x;
	//pxTransform.p.y = vPos.y;
	//pxTransform.p.z = vPos.z;
	//m_pRigidActor->setGlobalPose(pxTransform);


	return OBJ_NOEVENT;
}

_int CArbalist::LateUpdate_Object(const _float & fTimeDelta)
{
	//_vec3 vPos = m_pTransformCom->Get_Position();
	//PxTransform pxTransform = m_pRigidActor->getGlobalPose();
	//pxTransform.p.x = vPos.x;
	//pxTransform.p.y = vPos.y + 0.5f;
	//pxTransform.p.z = vPos.z;
	//m_pRigidActor->setGlobalPose(pxTransform);

	//PxTransform pxTransform = m_pRigidActor->getGlobalPose();
	//m_pTransformCom->Set_Pos(pxTransform.p.x, pxTransform.p.y - 0.2f, pxTransform.p.z);


	if (m_pCone)
		m_pCone->LateUpdate_Object(fTimeDelta);

	return _int();
}

void CArbalist::Render_Object(const _int & iIndex)
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

	m_pCrossBow->Update_Object(m_fTimeDelta);
	m_pCrossBow->Render_Object();

	if (m_pCone && m_fAttackTime > 1.7f)
	{
		m_pCone->Set_Active(true);
		//m_pCone->Render_Object(iIndex);
	}
}

HRESULT CArbalist::Add_Component(void* pArg)
{
	Engine::CComponent*		pComponent = nullptr;


	OBJINFO tTemp;
	ZeroMemory(&tTemp, sizeof(OBJINFO));
	if (nullptr != pArg)
	{
		memcpy(&tTemp, pArg, sizeof(OBJINFO));
	}

	// CDynamicMesh
	pComponent = m_pMeshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Clone_Resource(RESOURCE_CF, L"ToyArbalist"));
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

	m_pTransformCom->Set_Scale(tTemp.vScale);
	//m_pTransformCom->Rotation(ROT_X, D3DXToRadian(tTemp.vAngle.x));
	//m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(tTemp.vAngle.y));
	//m_pTransformCom->Rotation(ROT_Z, D3DXToRadian(tTemp.vAngle.z));
	m_pTransformCom->Set_Pos(tTemp.vPos);

	return S_OK;
}

HRESULT CArbalist::SetUp_ConstantTable(LPD3DXEFFECT & pEffect, const _int & iIndex)
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

void CArbalist::Move(const _float & fTimeDelta)
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
	vLook.y = 0.f;
	m_pTransformCom->Get_Info(INFO_RIGHT, &vRight);

	vMayPos = pMayTransformCom->Get_Position();
	vCodyPos = pCodyTransformCom->Get_Position();
	vPos = m_pTransformCom->Get_Position();

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
	
	if (m_bAttackStart) //플레이어를 인지를 했고, 어택 스타트 애니메이션 이후.
	{
		if (m_iAniNum == animID::Attack_Start && m_pMeshCom->Is_AnimationSetEnd(0.1f)) //어택 스타트 애니메이션이 끝나면?
		{
			m_pMeshCom->Set_Animationset(animID::Attack_Idle);
			m_iAniNum = animID::Attack_Idle;
			m_fAttackTime = 0.f;
		}
		m_fAttackTime += fTimeDelta;
	}

	//거리가 둘중에 하나라도 10.f 보다 작으면, 플레이어 인지.
	if (fMayDir <= 20.f || fCodyDir <= 20.f)
	{
		
		if (fMayDir < fCodyDir && m_pTargetPlayer == nullptr)
		{
			
			if (!m_bAttackStart)
			{
				m_pMeshCom->Set_Animationset(animID::Attack_Start);
				m_iAniNum = animID::Attack_Start;
				m_bAttackStart = true;
			}
			m_pTargetPlayer = dynamic_cast<CGameObject*>(Engine::Get_GameObject(L"GameObject", L"May"));
		}
		else if (fCodyDir < fMayDir && m_pTargetPlayer == nullptr)
		{
			if (!m_bAttackStart)
			{
				m_pMeshCom->Set_Animationset(animID::Attack_Start);
				m_iAniNum = animID::Attack_Start;
				m_bAttackStart = true;
			}
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
		vPos.y = 0.f;
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


		//기본적으로 얘는 안움직임.
		if (fTargetDir <= 20.f) 
		{

			if (m_fAttackTime > 3.f && m_iAniNum == animID::Attack_Idle && m_fAngle <= D3DXToDegree(m_fTurnSpeed))
			{
				m_pMeshCom->Set_Animationset(animID::Attack);
				m_iAniNum = animID::Attack;

				CLayer*				pLayer = nullptr;
				pLayer = Engine::Get_Layer(L"GameObject");
				Engine::CGameObject*		pGameObject = nullptr;

				const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("RightHand");

				_matrix matBoneWorld, matWorld;
				matBoneWorld = pFrame->CombinedTranformationMatrix;
				matWorld = *(m_pTransformCom->Get_WorldMatrix());

				matWorld = matBoneWorld * matWorld;

				_vec3 vTemp = { matWorld._41, matWorld._42, matWorld._43 };

				_tchar tagTemp[MAX_PATH];
				wsprintf(tagTemp, L"%d", m_iArrowCount);

				lstrcat(m_tArrowTag, tagTemp);

				vTargetPos = pTargetTransCom->Get_Position();
				pGameObject = m_pArrow = CArrow::Create(m_pGraphicDev, vTemp, vTargetPos);
				NULL_CHECK_RETURN(pGameObject, );
				FAILED_CHECK_RETURN(pLayer->Add_GameObject(m_tArrowTag, pGameObject,Engine::Get_Scene()), );
				Engine::Get_Scene()->setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eArrow, FilterGroup::eGround | FilterGroup::eWall | FilterGroup::eMay | FilterGroup::eCody);
				m_iArrowCount++;
			}
			if (m_iAniNum == animID::Attack && m_pMeshCom->Is_AnimationSetEnd(0.1f)) //애니메이션이 끝날때 생성
			{
				m_pCone->Set_Active(false);
				m_iAniNum = animID::Attack_Idle;
				m_pMeshCom->Set_Animationset(animID::Attack_Idle);
				m_fAttackTime = 0.f; //시간 초기화.		
				m_pTargetPlayer = nullptr;
			}


		}

		if (fTargetDir >= 25.f) //거리가 멀어져서 타겟이 사라지면 다시 아이들 상태
		{
			m_pMeshCom->Set_Animationset(animID::Attack_Idle);
			m_pTargetPlayer = nullptr;
		}

	}

}

void CArbalist::PhysicsCreate()
{
	auto* pPhysics = Engine::Get_Physics();
	_vec3 vPos = m_pTransformCom->Get_Position();
	PxTransform tTransform(vPos.x, vPos.y + 0.5f, vPos.z);

	PxMaterial* pMaterial = pPhysics->createMaterial(0.5f, 0.5f, 0.0f);


	PxShape* pShape = pPhysics->createShape(PxBoxGeometry(0.5f, 0.5f, 0.5f), *pMaterial, true);
	//밑 두개중 하나는 false 둘다 true안?
	pShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true);
	pShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, false);

	pShape->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, true);
	m_pRigidActor = PxCreateDynamic(*pPhysics, tTransform, *pShape, 1);

	auto* pBody = static_cast<PxRigidDynamic*>(m_pRigidActor);

	// 트랜스폼은 가지고있지만 움직이지 않는애들
	pBody->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, false);

	// 그래비티를 끈다.

	pBody->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, false);

	// 질량설정
	pBody->setMass(5);
	pBody->setName((char*)this);
	pShape->setName((char*)this);
	pBody->setRigidDynamicLockFlags(PxRigidDynamicLockFlag::eLOCK_ANGULAR_X | PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z);
	//이동 제동력
	pBody->setLinearDamping(0.05f);
}

void CArbalist::Set_Damage(_int iDamage)
{
	CEnermy::Set_Damage(iDamage);

	Hit_Effect(m_pTransformCom->Get_Position() + _vec3(0.f, 1.f, 0.f), 2.f);
	Hit_Damage(1.5f);
}

void CArbalist::Hit_State(const _float & fTimeDelta)
{
	m_pMeshCom->Set_Animationset(Damage);
	if (m_pMeshCom->Is_AnimationSetEnd(0.1f))
	{
		m_pMeshCom->Set_Animationset(Idle);
		m_bHit = false;
	}
}

void CArbalist::Create_Cone(const _tchar * tagLayer)
{ 
	Engine::CGameObject*		pGameObject = nullptr;

	OBJINFO tObj;
	ZeroMemory(&tObj, sizeof(OBJINFO));
	tObj.eSceneID = m_tObjInfo.eSceneID;
	tObj.vScale = { 0.3f, 0.1f, 1.f };

	pGameObject = CCone::Create(m_pGraphicDev, &tObj);
	m_pCone = pGameObject->Is<CCone>();
	m_pCone->Set_Index(2);
	//m_pCone->Set_Active(true);
	m_pCone->Set_Amount(1.f);
	NULL_CHECK_RETURN(pGameObject, );
}

CArbalist * CArbalist::Create(LPDIRECT3DDEVICE9 pGraphicDev, void * pArg, void* pOBJINFO)
{
	CArbalist*	pInstance = new CArbalist(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pArg, pOBJINFO)))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CArbalist::Free(void)
{
	Safe_Release(m_pCrossBow);
	Safe_Release(m_pCone);
	CEnermy::Free();
}
