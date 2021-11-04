#include "stdafx.h"
#include "Knight.h"
#include "Cody_Wizard.h"
#include "May_Castle.h"
#include "MainCamera.h"
#include "CollisionMgr.h"
#include "MorningStar.h"
#include "Particle.h"

#include "Export_Function.h"


CKnight::CKnight(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEnermy(pGraphicDev)
	, m_bMove(false)
{
}

CKnight::~CKnight()
{
}

HRESULT CKnight::Ready_Object(void * pArg, void* pOBJINFO)
{
	m_iHPMax = 10;
	m_iHP = m_iHPMax;
	
	// 콜라이더
	ZeroMemory(&m_tColInfo, sizeof(SPHERE_INFO));
	m_tColInfo.fRadius = 0.7f;

	FAILED_CHECK_RETURN(Add_Component(pOBJINFO), E_FAIL);

	OBJINFO tTemp;
	ZeroMemory(&tTemp, sizeof(OBJINFO));

	if (nullptr != pArg)
	{
		m_pMorningStar = (CMorningStar*)pArg;
		m_pMorningStar->AddRef();

		memcpy(&tTemp, pArg, sizeof(OBJINFO));
	}

	OBJINFO tObjINFO;
	ZeroMemory(&tObjINFO, sizeof(OBJINFO));
	if (nullptr != pOBJINFO)
	{
		memcpy(&tObjINFO, pOBJINFO, sizeof(OBJINFO));
	}

	m_pTransformCom->Set_Scale(tObjINFO.vScale);
	m_pTransformCom->Rotation(ROT_X, tObjINFO.vAngle.x);
	m_pTransformCom->Rotation(ROT_Y, tObjINFO.vAngle.y);
	m_pTransformCom->Rotation(ROT_Z, tObjINFO.vAngle.z);
	m_pTransformCom->Set_Pos(tObjINFO.vPos);

	m_pMeshCom->Set_Animationset((ToyKnight_Idle));

	m_iRandom = rand() % 2;

	Create_DefaultUI(1.2f, 2.f);

	return S_OK;
}

_int CKnight::Update_Object(const _float & fTimeDelta)
{
	m_fTimeDelta = fTimeDelta;

	Update_DefaultUI(fTimeDelta);

	if (m_iHP <= 0)
	{
		m_pMorningStar->Set_AttAble(false);

		Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);

		m_pMeshCom->Set_Animationset(ToyKnight_Dead);
		m_pMeshCom->Play_Animationset(fTimeDelta);

		if (m_pMeshCom->Is_AnimationSetEnd(0.1f))
		{
			m_pMorningStar->Set_Dead();
			m_bDead = true;
		}

		if (m_bDead)
		{
			Dead_Effect();

			CRandoms Random;
			_uint	iRandom = Random(1, 3);
			if (iRandom == 1)
			{
				Engine::StopSound(CSoundMgr::CHANNELID::ToyKnight);
				Engine::PlaySoundW(L"ToyKnightDeath1.ogg", CSoundMgr::CHANNELID::ToyKnight, 0.3f);
			}
			else if (iRandom == 2)
			{
				Engine::StopSound(CSoundMgr::CHANNELID::ToyKnight);
				Engine::PlaySoundW(L"ToyKnightDeath2.ogg", CSoundMgr::CHANNELID::ToyKnight, 0.3f);
			}
			else if (iRandom == 3)
			{
				Engine::StopSound(CSoundMgr::CHANNELID::ToyKnight);
				Engine::PlaySoundW(L"ToyKnightDeath3.ogg", CSoundMgr::CHANNELID::ToyKnight, 0.3f);
			}

			Delete_UI();

			return OBJ_DEAD;
		}
		return 0;
	}

	Engine::CGameObject::Update_Object(fTimeDelta);

	if (m_bHit)
	{
		Hit_State(fTimeDelta);
		m_bMove = true;
		m_pMorningStar->Set_AttAble(false);
	}
	else if (m_bMove)
		Move(fTimeDelta);

	m_pMeshCom->Play_Animationset(fTimeDelta);

	CCollisionMgr::GetInstance()->Add_EnermyList(this);

	Tick_Damage(fTimeDelta);
	Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);


	_vec3 vPos;
	vPos = m_pTransformCom->Get_Position();

	//if (vPos.y > 1.0f)
	//{
	//	vPos.y = 0.f;
	//}

	PxTransform pxTransform = m_pRigidActor->getGlobalPose();
	pxTransform.p.x = vPos.x;
	pxTransform.p.y = vPos.y;
	pxTransform.p.z = vPos.z;
	m_pRigidActor->setGlobalPose(pxTransform);

	return OBJ_NOEVENT;
}

_int CKnight::LateUpdate_Object(const _float & fTimeDelta)
{

	PxTransform pxTransform = m_pRigidActor->getGlobalPose();
	m_pTransformCom->Set_Pos(pxTransform.p.x, pxTransform.p.y /*- 0.5f*/, pxTransform.p.z);

	return 0;
}

void CKnight::Render_Object(const _int & iIndex)
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

	m_pMorningStar->Update_Object(m_fTimeDelta);
	m_pMorningStar->Render_Object();
}

void CKnight::Set_Damage(_int iDamage)
{
	CEnermy::Set_Damage(iDamage);

	StopSound(CSoundMgr::ToyKnight);
	PlaySoundW(L"ToyKnight_Damage.wav", CSoundMgr::ToyKnight, 1.f);

	Hit_Effect(m_pTransformCom->Get_Position() + _vec3(0.f, 1.f, 0.f), 2.f);
	Hit_Damage(1.5f);
}

HRESULT CKnight::Add_Component(void* pArg)
{
	Engine::CComponent*		pComponent = nullptr;

	OBJINFO tObjINFO;
	ZeroMemory(&tObjINFO, sizeof(OBJINFO));
	if (nullptr != pArg)
	{
		memcpy(&tObjINFO, pArg, sizeof(OBJINFO));
	}

	// CDynamicMesh
	pComponent = m_pMeshCom = static_cast<Engine::CDynamicMesh*>(Engine::Clone_Resource(RESOURCE_CF, L"ToyKnight"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Mesh", pComponent);

	// Transform
	pComponent = m_pTransformCom = static_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	// Collider
	pComponent = m_pColliderCom = Engine::CCollider::Create(m_pGraphicDev, 0.7f, _vec4(50.f, 125.0f, 125.0f, 125.0f));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Collider", pComponent);

	// Shader
	pComponent = m_pShaderCom = static_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_VTF"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);


	m_pTransformCom->Set_Scale(tObjINFO.vScale);
	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(tObjINFO.vAngle.x));
	m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(tObjINFO.vAngle.y));
	m_pTransformCom->Rotation(ROT_Z, D3DXToRadian(tObjINFO.vAngle.z));
	m_pTransformCom->Set_Pos(tObjINFO.vPos);


	auto* pPhysics = Engine::Get_Physics();
	_vec3 vPos = m_pTransformCom->Get_Position();
	PxTransform tTransform(vPos.x, vPos.y, vPos.z);

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
	return S_OK;
}

HRESULT CKnight::SetUp_ConstantTable(LPD3DXEFFECT & pEffect, const _int & iIndex)
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

void CKnight::Move(const _float & fTimeDelta)
{
	Engine::CTransform*		pMayTransformCom = static_cast<Engine::CTransform*>(Engine::Get_Component(L"GameObject", L"May", L"Com_Transform", Engine::ID_DYNAMIC));
	Engine::CTransform*		pCodyTransformCom = static_cast<Engine::CTransform*>(Engine::Get_Component(L"GameObject", L"Cody", L"Com_Transform", Engine::ID_DYNAMIC));

	_vec3 vMayPos; //메이 위치
	_vec3 vCodyPos; //코디 위치
	_vec3 vPos; //현재 몬스터(자신)	위치.
	_vec3 vLook; //현재 몬스터(자신)	룩방향
	_vec3 vRight; //현재 몬스터(자신) 라이트
	_vec3 vLeft; //현재 몬스터(자신) 레프트

	m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
	vLook.y = 0.f;
	m_pTransformCom->Get_Info(INFO_RIGHT, &vRight);
	vRight.y = 0.f;

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

	//거리가 둘중에 하나라도 10.f 보다 작으면, 플레이어 인지.
	if (fMayDir <= 10.f || fCodyDir <= 10.f)
	{
		if (fMayDir < fCodyDir && m_pTargetPlayer == nullptr)
		{
			m_pTargetPlayer = static_cast<CGameObject*>(Engine::Get_GameObject(L"GameObject", L"May"));
		}
		else if (fCodyDir < fMayDir && m_pTargetPlayer == nullptr)
		{
			m_pTargetPlayer = static_cast<CGameObject*>(Engine::Get_GameObject(L"GameObject", L"Cody"));
		}
	}

	if (m_pTargetPlayer != nullptr)
	{

		//타겟의 트랜스를 가져옴.
		CTransform*		pTargetTransCom = static_cast<CTransform*>(m_pTargetPlayer->Get_Component(L"Com_Transform", Engine::ID_DYNAMIC));

		_vec3	vTargetPos;
		_vec3	vTargetDir;
		_float	 fTargetDir;


		vTargetPos = pTargetTransCom->Get_Position();
		vTargetPos.y = 0.f;
		vPos.y = 0.f;
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

		if (fTargetDir <= 1.f || m_bAttack) //가까워지면 공격
		{
			if (!m_bSound[Attack])
			{
				StopSound(CSoundMgr::ToyKnight);
				Engine::PlaySoundW(L"ToyKnight_Attack.wav", CSoundMgr::ToyKnight, 0.3f);
				m_bSound[Attack] = true;
			}
			if (m_iRandom == ToyKnight_Attack1)
			{
				m_pMorningStar->Set_AttAble(true);
				m_pMeshCom->Set_Animationset(ToyKnight_Attack1);
				m_bAttack = true;

				if (true == m_pMeshCom->Is_AnimationSetEnd(0.1f))
				{
					m_iRandom = rand() % 2;
					m_pMorningStar->Set_AttAble(false);
					m_pMorningStar->Clear_CollisionList();
					m_bAttack = false;
					m_bSound[Attack] = false;
				}
			}
			else if (m_iRandom == ToyKnight_Attack2)
			{
				m_pMorningStar->Set_AttAble(true);
				m_pMeshCom->Set_Animationset(ToyKnight_Attack2);
				m_bAttack = true;

				if (true == m_pMeshCom->Is_AnimationSetEnd(0.1f))
				{
					m_iRandom = rand() % 2;
					m_pMorningStar->Set_AttAble(false);
					m_pMorningStar->Clear_CollisionList();
					m_bAttack = false;
					m_bSound[Attack] = false;
				}
			}
		}
		else if (fTargetDir >= 1.f && !m_bAttack) //멀어지면 추격.
		{
			Engine::PlaySoundW(L"ToyKnight_Move.wav", CSoundMgr::ToyKnight, 0.3f);

			m_pMeshCom->Set_Animationset(ToyKnight_Walk);
			m_pTransformCom->Move_Pos(&vLook, 2.f, fTimeDelta);
			m_pMorningStar->Set_AttAble(false);
			m_pMorningStar->Clear_CollisionList();
		}

		if (fTargetDir >= 11.f) //거리가 멀어져서 타겟이 사라지면 다시 아이들 상태
		{
			m_pMeshCom->Set_Animationset(ToyKnight_Idle);
			m_pTargetPlayer = nullptr;
			m_pMorningStar->Clear_CollisionList();
		}
	}
}

void CKnight::Hit_State(const _float & fTimeDelta)
{
	m_pMeshCom->Set_Animationset(ToyKnight_Damage);
	if (m_pMeshCom->Is_AnimationSetEnd(0.1f))
	{
		m_bHit = false;
	}
	m_pTargetPlayer = nullptr;
	m_bAttack = false;

}

CKnight * CKnight::Create(LPDIRECT3DDEVICE9 pGraphicDev, void * pArg, void* pOBJINFO)
{
	CKnight*	pInstance = new CKnight(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pArg, pOBJINFO)))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CKnight::Free(void)
{
	Safe_Release(m_pMorningStar);

	CEnermy::Free();
}
