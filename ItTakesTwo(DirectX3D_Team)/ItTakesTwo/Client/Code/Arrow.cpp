#include "stdafx.h"
#include "Arrow.h"
#include "MainCamera.h"
#include "CollisionMgr.h"

#include "Export_Function.h"

CArrow::CArrow(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEnermy(pGraphicDev)
	, m_bCreateTrigger(false)
{
}

CArrow::~CArrow()
{
}


HRESULT CArrow::Ready_Object(_vec3 vPos, _vec3 vTargetPos)
{

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);


	m_pTransformCom->Set_Scale(_vec3(0.05f, 0.05f, 0.05f));
	m_pTransformCom->Set_Pos(vPos);


	m_vDir = vTargetPos - vPos;
	m_vDir.y = 0.f;
	D3DXVec3Normalize(&m_vDir, &m_vDir);

	m_pTransformCom->Set_Angle({ 0.f,atan2f(m_vDir.x,m_vDir.z)+D3DXToRadian(-90.f),0.f });

	PhyscisCreate();

	return S_OK;
}

_int CArrow::Update_Object(const _float & fTimeDelta)
{

	if (m_bStart)
	{
		m_pTrigger = CTrigger::CreateCapsuleTirgger(m_pScene, this, m_pTransformCom->Get_Position(), 0.5, 0.5, TRIGGER_CHESS_ARROW);
		m_bStart = false;
	}


	m_pTrigger->Set_Pos(m_pTransformCom->Get_Position());

	_vec3 vPos;
	vPos = m_pTransformCom->Get_Position();

	PxTransform pxTransform = m_pRigidActor->getGlobalPose();
	pxTransform.p.x = vPos.x;
	pxTransform.p.y = vPos.y;
	pxTransform.p.z = vPos.z;

	m_pRigidActor->setGlobalPose(pxTransform);

	PARTICLE tParticle;
	Engine::Reset_Particle(tParticle);

	tParticle.fAcc = 1.f;
	tParticle.fCurrentTime = 0.f;
	tParticle.vEndSize = _vec3(0.1f, 0.1f, 0.1f);
	tParticle.fRotateSpeed = 0.f;

	tParticle.vStartColor = { 255.f * 5.5f, 100.f  * 5.5f, 100.f * 5.5f , 255.f };
	tParticle.vEndColor = { 255.f, 100.f , 100.f , 155.f };
	tParticle.vRot = { 0.f, 0.f, 0.f };

	tParticle.fVelocity = 0.f;
	tParticle.fLifeTime = 0.5f;
	_float fRand = CRandoms()(0.2f, 0.4f);
	tParticle.vStartSize = _vec3(fRand, fRand, fRand);
	tParticle.vDir = { 0.f, 0.f, 0.f };

	for (_int i = 0; i < 5; ++i)
	{
		tParticle.vPos = vPos + _vec3(CRandoms()(-0.2f, 0.2f), CRandoms()(-0.2f, 0.2f), CRandoms()(-0.2f, 0.2f));
		Engine::Emit_ImageParticle(tParticle, 15, 11);
	}


	if (m_bDead)
	{
		m_pTrigger->Set_Interactable(false);
		return OBJ_DEAD;
	}
	m_fLifeTime += fTimeDelta;

	if (m_fLifeTime > 2.f)
	{
		m_bDead = true;
	}
	m_pTransformCom->Move_Pos(&m_vDir, 30.f, fTimeDelta);

	Engine::CGameObject::Update_Object(fTimeDelta);
	Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);


	


	return OBJ_NOEVENT;
}

_int CArrow::LateUpdate_Object(const _float & fTimeDelta)
{
	return _int();
}

void CArrow::Render_Object(const _int & iIndex)
{
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect, iIndex), );

	_uint	iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);		// 1인자 : 현재 쉐이더 파일이 갖고 있는 pass의 최대 개수, 2인자 : 시작하는 방식(default)
										//pEffect->BeginPass(0);
	m_pMeshCom->Render_Meshes(pEffect, 8);

	pEffect->End();

	Safe_Release(pEffect);

	CMainCamera* pCamera = (CMainCamera*)Engine::Get_GameObject(L"GameObject", L"MainCamera");
	pCamera->Update_View_Proj();

}

HRESULT CArrow::Add_Component()
{
	Engine::CComponent*		pComponent = nullptr;

	// CStaticMesh
	pComponent = m_pMeshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(RESOURCE_CF, L"Arrow"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Mesh", pComponent);

	// Transform
	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	// Shader
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_All"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);

	return S_OK;
}

HRESULT CArrow::SetUp_ConstantTable(LPD3DXEFFECT & pEffect, const _int & iIndex)
{
	_matrix			matWorld, matView, matProj;

	m_pTransformCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	return S_OK;
}

void CArrow::Move(const _float & fTimeDelta)
{
}

void CArrow::BoneUpdate(const _float & fTimeDelta)
{
}

void CArrow::PhyscisCreate()
{
	auto* pPhysics = Engine::Get_Physics();
	_vec3 vPos = m_pTransformCom->Get_Position();
	PxTransform tTransform(vPos.x, vPos.y, vPos.z);

	PxMaterial* pMaterial = pPhysics->createMaterial(0.5f, 0.5f, 0.0f);

	PxShape* pShape = pPhysics->createShape(PxSphereGeometry(0.2f), *pMaterial, true);
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
	pBody->setMass(1);
	pBody->setName((char*)this);
	pShape->setName((char*)this);
	pBody->setRigidDynamicLockFlags(PxRigidDynamicLockFlag::eLOCK_ANGULAR_X | PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z |PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y | PxRigidDynamicLockFlag::eLOCK_LINEAR_Y);
	//이동 제동력

}

CArrow * CArrow::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, _vec3 vTargetPos)
{
	CArrow*	pInstance = new CArrow(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(vPos, vTargetPos)))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CArrow::Free(void)
{
	m_pRigidActor->release();
	CEnermy::Free();
}
