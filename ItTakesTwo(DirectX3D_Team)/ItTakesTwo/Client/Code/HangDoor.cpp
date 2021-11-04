#include "stdafx.h"
#include "HangDoor.h"
#include "Export_Function.h"



CHangDoor::CHangDoor(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CHangDoor::~CHangDoor()
{
}

HRESULT CHangDoor::Ready_Object(void * pArg)
{
	FAILED_CHECK_RETURN(Add_Component(pArg), E_FAIL);

	OBJINFO tTemp;
	ZeroMemory(&tTemp, sizeof(OBJINFO));

	if (nullptr != pArg)
	{
		memcpy(&tTemp, pArg, sizeof(OBJINFO));
	}

	m_tObjInfo = tTemp;


	m_vGoalPos = _vec3(30.2f, 12.157836f, 0.0f);

	m_vNormal = _vec3(1.0f, 0.0f, 0.0f);
	m_vOrgPos = m_pTransformCom->Get_Position();


	PARTICLE tParticle;
	Engine::Reset_Particle(tParticle);

	tParticle.fAcc = 1.f;
	tParticle.fCurrentTime = 0.f;
	tParticle.fLifeTime = 100000.0f;
	tParticle.fRotateSpeed = 0.f;
	tParticle.fVelocity = 0.f;
	tParticle.vDir = _vec3(0.f, 0.f, 0.f);
	tParticle.vStartSize = _vec3(5.f, 10.f, 1.f);
	tParticle.vEndSize = _vec3(5.f, 10.f, 1.f);
	tParticle.vStartColor = { 255.f, 255.f, 25.f, 255.f };
	tParticle.vEndColor = { 255.f, 255.f, 25.f, 255.f };
	tParticle.vRot = { 0.f, D3DXToRadian(90.f), 0.f };
	tParticle.vPos = m_pTransformCom->Get_Position() + _vec3(0.f, 5.2f, 0.f);
	tParticle.bBill = false;

	m_pParticle = Engine::Emit_ImageParticle(tParticle, 15, 30);



	return S_OK;
}

_int CHangDoor::Update_Object(const _float & fTimeDelta)
{
	if (m_bStart)
	{
		m_pTrigger = CTrigger::CreateBoxTrigger(m_pScene, this, m_tObjInfo.vTriggerPos, m_tObjInfo.vTriggerScale.x, m_tObjInfo.vTriggerScale.y, m_tObjInfo.vTriggerScale.z, TRIGGER_HANG);
		m_bStart = false;
	}

	if (true == m_bFall)
	{
		m_pTransformCom->Move_Pos(&_vec3(0.0f, -1.0f, 0.0f), 1.0f, fTimeDelta);

		if (m_pTransformCom->Get_Position().y <= m_vOrgPos.y)
		{
			m_pTransformCom->Set_Pos(m_vOrgPos);
			m_bFall = false;
		}
	}

	m_pParticle->Set_ParticlePos(m_pTransformCom->Get_Position() + _vec3(0.f, 5.2f, 0.f));
	
	Engine::CGameObject::Update_Object(fTimeDelta);
	Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);

	return OBJ_NOEVENT;
}

_int CHangDoor::LateUpdate_Object(const _float & fTimeDelta)
{
	_vec3 vPos = m_pTransformCom->Get_Position();
	PxTransform tTransform = m_pRigidActor->getGlobalPose();
	tTransform.p.x = vPos.x;
	tTransform.p.y = vPos.y;
	tTransform.p.z = vPos.z;

	m_pRigidActor->is<PxRigidDynamic>()->setKinematicTarget(tTransform);

	return _int();
}

void CHangDoor::Render_Object(const _int & iIndex)
{
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect, iIndex), );

	_uint	iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);			//	1인자 : 현재 쉐이더 파일이 갖고 있는 pass의 최대 개수, 2인자 : 시작하는 방식(default)
	m_pMeshCom->Render_Meshes(pEffect, 8);	//	pEffect->BeginPass(0);
											//	스태틱매쉬8
											//	다이나믹6


	pEffect->End();

	Safe_Release(pEffect);
}

HRESULT CHangDoor::Add_Component(void * pArg)
{
	Engine::CComponent*		pComponent = nullptr;

	OBJINFO tTemp;
	if (pArg)
	{
		memcpy(&tTemp, pArg, sizeof(OBJINFO));
	}
	


	pComponent = m_pMeshCom = static_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(tTemp.eSceneID, L"HangDoor"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Mesh", pComponent);

	// Transform
	pComponent = m_pTransformCom = static_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	// Shader
	pComponent = m_pShaderCom = static_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_All"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);

	m_pTransformCom->Set_Scale(tTemp.vScale);
	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(tTemp.vAngle.x));
	m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(tTemp.vAngle.y));
	m_pTransformCom->Rotation(ROT_Z, D3DXToRadian(tTemp.vAngle.z));
	m_pTransformCom->Set_Pos(tTemp.vPos);


	auto* pPhysics = Engine::Get_Physics();
	_vec3 vPos = m_pTransformCom->Get_Position();

	PxTransform tTransform(vPos.x, vPos.y, vPos.z, PxQuat(tTemp.vAngle.y, { 0.f,1.f,0.f }));
	PxMaterial* pMaterial = pPhysics->createMaterial(0.5f, 0.5f, 0.0f);

	PxTriangleMeshGeometry pMeshGeo;
	pMeshGeo.triangleMesh = m_pMeshCom->Get_PxMesh();//m_pPhysMesh->Get_PxMesh();
	pMeshGeo.scale = PxMeshScale(PxVec3(tTemp.vScale.x, tTemp.vScale.y, tTemp.vScale.z));

	PxShape* pShape = pPhysics->createShape(pMeshGeo, *pMaterial, true);

	pShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
	pShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, false);
	pShape->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, true);

	m_pRigidActor = PxCreateDynamic(*pPhysics, tTransform, *pShape, 1);

	auto* pBody = static_cast<PxRigidDynamic*>(m_pRigidActor);

	// 트랜스폼은 가지고있지만 움직이지 않는애들
	pBody->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
	//pBody->setKinematicTarget()
	// 그래비티를 끈다.
	pBody->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);
	// 질량설정
	pBody->setMass(0);
	//이동 제동력
	pBody->setName((char*)this);



	return S_OK;
}

HRESULT CHangDoor::SetUp_ConstantTable(LPD3DXEFFECT & pEffect, const _int & iIndex)
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

CHangDoor * CHangDoor::Create(LPDIRECT3DDEVICE9 pGraphicDev, void * pArg)
{
	CHangDoor*	pInstance = new CHangDoor(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pArg)))
	{
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CHangDoor::Free(void)
{
	Engine::CGameObject::Free();
}

void CHangDoor::Move(const _float & fTimeDelta)
{
}
