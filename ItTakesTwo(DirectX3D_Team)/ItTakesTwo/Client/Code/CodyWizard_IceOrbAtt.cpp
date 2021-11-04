#include "stdafx.h"
#include "CodyWizard_IceOrbAtt.h"
#include "CollisionMgr.h"
#include "Export_Function.h"



CCodyWizard_IceOrbAtt::CCodyWizard_IceOrbAtt(LPDIRECT3DDEVICE9 pGraphicDev)
	:CPlayer(pGraphicDev)
	,m_fLifeTime(0.F)
	,m_fLifeDir(8.f)

{
}

CCodyWizard_IceOrbAtt::~CCodyWizard_IceOrbAtt()
{
}

HRESULT CCodyWizard_IceOrbAtt::Ready_Object(_vec3 vDir, void * pVoid, _float fLifeDir)
{
	m_fColDis = 1.0f;

	ZeroMemory(&m_tColInfo, sizeof(SPHERE_INFO));
	m_tColInfo.fRadius = m_fColDis;

	FAILED_CHECK_RETURN(Add_Component(pVoid), E_FAIL);

	OBJINFO tTemp;
	if (pVoid)
	{
		memcpy(&tTemp, pVoid, sizeof(OBJINFO));
	}

	m_pTransformCom->Set_Scale(tTemp.vScale);
	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(tTemp.vAngle.x));
	m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(tTemp.vAngle.y));
	m_pTransformCom->Rotation(ROT_Z, D3DXToRadian(tTemp.vAngle.z));
	m_pTransformCom->Set_Pos(tTemp.vPos);

	m_vStartPos = tTemp.vPos;

	m_fLifeDir = fLifeDir;
	m_vDir = vDir;
	
	PhyscisCreate();

	return S_OK;
}

_int CCodyWizard_IceOrbAtt::Update_Object(const _float & fTimeDelta)
{


	_vec3 vPos;
	vPos = m_pTransformCom->Get_Position();

	PxTransform pxTransform = m_pRigidActor->getGlobalPose();
	pxTransform.p.x = vPos.x;
	pxTransform.p.y = vPos.y;
	pxTransform.p.z = vPos.z;

	m_pRigidActor->setGlobalPose(pxTransform);


	if (m_bDead)
		return OBJ_DEAD;

	if (m_fLifeTime > 5.f)
		m_bDead = true;

	m_fLifeTime += fTimeDelta;

	Move(fTimeDelta);

	CCollisionMgr::GetInstance()->Add_PlayerBulletList(this);
	Engine::CGameObject::Update_Object(fTimeDelta);
	Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);

	return OBJ_NOEVENT;
}

_int CCodyWizard_IceOrbAtt::LateUpdate_Object(const _float & fTimeDelta)
{
	return _int();
}

void CCodyWizard_IceOrbAtt::Render_Object(const _int & iIndex)
{
	//LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	//NULL_CHECK(pEffect);
	//pEffect->AddRef();

	//FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect, iIndex), );

	//_uint	iPassMax = 0;

	//pEffect->Begin(&iPassMax, 0);		// 1인자 : 현재 쉐이더 파일이 갖고 있는 pass의 최대 개수, 2인자 : 시작하는 방식(default)
	//									//pEffect->BeginPass(0);
	//m_pMeshCom->Render_Meshes(pEffect, 8);

	//pEffect->End();

	//Safe_Release(pEffect);

	//Spine2
	m_tColInfo.matWorld = *m_pTransformCom->Get_WorldMatrix();
	m_pColliderCom->Render_Collider(m_tColInfo.matWorld, false);
}

void CCodyWizard_IceOrbAtt::Set_Dead(_bool bStack)
{
	_bool bTemp = false;

	bTemp = bStack;
	if (bTemp)
	{
		CPlayer* pCody = (CPlayer*)Engine::Get_GameObject(L"GameObject", L"Cody");
		pCody->Add_UltiStack(1);
	}

	m_bDead = true;
}

HRESULT CCodyWizard_IceOrbAtt::Add_Component(void* pVoid)
{
	Engine::CComponent*		pComponent = nullptr;


	OBJINFO tTemp;
	if (pVoid)
	{
		memcpy(&tTemp, pVoid, sizeof(OBJINFO));
	}


	// CStaticMesh
	pComponent = m_pMeshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(RESOURCE_PLAYER , L"IceOrb"));
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
	pComponent = m_pColliderCom = Engine::CCollider::Create(m_pGraphicDev, m_tColInfo.fRadius, _vec4(50.f, 244.f, 234.f, 0.f));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Collider", pComponent);

	// Shader
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_All"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);

	return S_OK;
}

HRESULT CCodyWizard_IceOrbAtt::SetUp_ConstantTable(LPD3DXEFFECT & pEffect, const _int & iIndex)
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

void CCodyWizard_IceOrbAtt::Move(const _float & fTimeDelta)
{

	_vec3 vPos;
	vPos = m_pTransformCom->Get_Position();

	_vec3 vDir = m_vStartPos - vPos;
	_float fDir = D3DXVec3Length(&vDir);

	if (fDir > m_fLifeDir)
	{
		m_bDead = true;
	}

	m_pTransformCom->Move_Pos(&m_vDir, 20.f, fTimeDelta);
}

void CCodyWizard_IceOrbAtt::PhyscisCreate()
{
	auto* pPhysics = Engine::Get_Physics();
	_vec3 vPos = m_pTransformCom->Get_Position();
	PxTransform tTransform(vPos.x, vPos.y, vPos.z);

	PxMaterial* pMaterial = pPhysics->createMaterial(0.5f, 0.5f, 0.0f);

	PxShape* pShape = pPhysics->createShape(PxBoxGeometry(0.5f, 0.1f, 0.5f), *pMaterial, true);
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

CCodyWizard_IceOrbAtt * CCodyWizard_IceOrbAtt::Create(LPDIRECT3DDEVICE9 pGraphicDev, _int iDamage, _vec3 vDir, void* pVoid, _float fLifeDir)
{
	CCodyWizard_IceOrbAtt*	pInstance = new CCodyWizard_IceOrbAtt(pGraphicDev);

	pInstance->Set_AttDamage(iDamage);
	if (FAILED(pInstance->Ready_Object(vDir, pVoid, fLifeDir)))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCodyWizard_IceOrbAtt::Free(void)
{
	m_pRigidActor->release();
	CPlayer::Free();
}
