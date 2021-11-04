#include "stdafx.h"
#include "CodyWizard_IceOrbAttRB.h"
#include "Enermy.h"
#include "CollisionMgr.h"
#include "Export_Function.h"
#include "Cody_Wizard.h"

CCodyWizard_IceOrbAttRB::CCodyWizard_IceOrbAttRB(LPDIRECT3DDEVICE9 pGraphicDev)
	:CPlayer(pGraphicDev)
	, m_fLifeTime(0.F)
	, m_fAttRadius(3.f)

{
}

CCodyWizard_IceOrbAttRB::~CCodyWizard_IceOrbAttRB()
{
}

HRESULT CCodyWizard_IceOrbAttRB::Ready_Object(_vec3 vDir, CLight* pLight, void * pVoid)
{
	m_iDamage = CRandoms()(10, 15);
	m_fColDis = 1.0f;

	ZeroMemory(&m_tColInfo, sizeof(SPHERE_INFO));
	m_tColInfo.fRadius = m_fColDis;

	FAILED_CHECK_RETURN(Add_Component(pVoid), E_FAIL);

	m_pLight = pLight;


	OBJINFO tTemp;
	if (pVoid)
	{
		memcpy(&tTemp, pVoid, sizeof(OBJINFO));
	}

	m_pTransformCom->Set_Scale(_vec3(0.004f, 0.004f, 0.004f));
	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(tTemp.vAngle.x));
	m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(tTemp.vAngle.y));
	m_pTransformCom->Rotation(ROT_Z, D3DXToRadian(tTemp.vAngle.z));
	m_pTransformCom->Set_Pos(tTemp.vPos);

	m_vStartPos = tTemp.vPos;

	m_vDir = vDir;

	PhyscisCreate();

	m_tShader.m_fRimDist = 1.f;
	m_tShader.m_iRimPower = 100;
	m_tShader.m_vRimColor = _vec4(0.3f * 2.f, 0.7f* 2.f, 1.f* 2.f, 1.f);

	return S_OK;
}

_int CCodyWizard_IceOrbAttRB::Update_Object(const _float & fTimeDelta)
{
	if (m_bDead)
	{
		m_pLight->Set_LightSize(8.f);
		CCodyWizard* pCody = (CCodyWizard*)Engine::Get_GameObject(L"GameObject", L"Cody");
		pCody->Set_OrbAtt(false);

		Effect_Explosive();
		return OBJ_DEAD;
	}
	_vec3 vPos;
	vPos = m_pTransformCom->Get_Position();

	m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(360) * fTimeDelta);


	PxTransform pxTransform = m_pRigidActor->getGlobalPose();
	pxTransform.p.x = vPos.x;
	pxTransform.p.y = vPos.y;
	pxTransform.p.z = vPos.z;

	m_pRigidActor->setGlobalPose(pxTransform);



	m_fLifeTime += fTimeDelta;


	if (m_fLifeTime > 5.f)
		m_bDead = true;


	m_fEffectTime += fTimeDelta;
	Move(fTimeDelta);
	Effect();

	CCollisionMgr::GetInstance()->Add_PlayerSplashBulletList(this);



	m_pLight->Set_LightPos(m_pTransformCom->Get_Position());
	m_pLight->Set_LightSize(5.f);

	Engine::CGameObject::Update_Object(fTimeDelta);
	Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);

	return OBJ_NOEVENT;
}

_int CCodyWizard_IceOrbAttRB::LateUpdate_Object(const _float & fTimeDelta)
{
	return _int();
}

void CCodyWizard_IceOrbAttRB::Render_Object(const _int & iIndex)
{
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect, iIndex), );

	_uint	iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);		// 1인자 : 현재 쉐이더 파일이 갖고 있는 pass의 최대 개수, 2인자 : 시작하는 방식(default)
										//pEffect->BeginPass(0);
	m_pMeshCom->Render_Meshes(pEffect, 1);

	pEffect->End();

	Safe_Release(pEffect);

	//Spine2
	m_tColInfo.matWorld = *m_pTransformCom->Get_WorldMatrix();
	m_pColliderCom->Render_Collider(m_tColInfo.matWorld, false);
}

void CCodyWizard_IceOrbAttRB::Set_Dead(_bool bStack)
{

	vector<CEnermy*>*		vecEnermyList;
	vecEnermyList = CCollisionMgr::GetInstance()->Get_EnermyList();

	_vec3	vPos;
	_vec3	vDir;
	_float	fDir;


	vPos = m_pTransformCom->Get_Position();
	for (_uint i = 0; i < vecEnermyList->size(); ++i)
	{
		_vec3 vEnermyPos;
		vEnermyPos = (*vecEnermyList)[i]->Get_Component(L"Com_Transform", ID_DYNAMIC)->Is<CTransform>()->Get_Position();
		vDir = vPos - vEnermyPos;
		fDir = D3DXVec3Length(&vDir);



		if (fDir < m_fAttRadius)
		{
			(*vecEnermyList)[i]->Set_Damage(-m_iDamage);
			CPlayer*	pPlayer = (CPlayer*)Engine::Get_GameObject(L"GameObject", L"Cody");
			pPlayer->Add_UltiStack(3);
		}
	}


	m_bDead = true;

	//return  OBJ_DEAD;
}

HRESULT CCodyWizard_IceOrbAttRB::Add_Component(void * pVoid)
{
	Engine::CComponent*		pComponent = nullptr;


	OBJINFO tTemp;
	if (pVoid)
	{
		memcpy(&tTemp, pVoid, sizeof(OBJINFO));
	}


	// CStaticMesh
	pComponent = m_pMeshCom = static_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(RESOURCE_PLAYER, L"IceOrb"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Mesh", pComponent);

	// Transform
	pComponent = m_pTransformCom = static_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	// Calculator
	pComponent = m_pCalculatorCom = static_cast<Engine::CCalculator*>(Engine::Clone_Prototype(L"Proto_Calculator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Calculator", pComponent);

	// Collider
	pComponent = m_pColliderCom = Engine::CCollider::Create(m_pGraphicDev, m_tColInfo.fRadius, _vec4(50.f, 244.f, 234.f, 0.f));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Collider", pComponent);

	// Shader
	pComponent = m_pShaderCom = static_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_All"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);

	return S_OK;
}

HRESULT CCodyWizard_IceOrbAttRB::SetUp_ConstantTable(LPD3DXEFFECT & pEffect, const _int & iIndex)
{
	_matrix			matWorld, matView, matProj, matInv;

	m_pTransformCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	D3DXMatrixInverse(&matInv, nullptr, &matView);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	pEffect->SetFloat("g_fRimDist", m_tShader.m_fRimDist);
	pEffect->SetInt("g_iRimPower", m_tShader.m_iRimPower);
	pEffect->SetVector("g_vRimColor", &m_tShader.m_vRimColor);

	pEffect->SetVector("g_vCamPos", &_vec4(matInv._41, matInv._42, matInv._43, 1.f));

	return S_OK;
}

void CCodyWizard_IceOrbAttRB::Move(const _float & fTimeDelta)
{
	//_vec3 vPos;
	//vPos = m_pTransformCom->Get_Position();

	//_vec3 vDir = m_vStartPos - vPos;
	//_float fDir = D3DXVec3Length(&vDir);

	//if (fDir > m_fLifeDir)
	//{
	//	m_bDead = true;
	//}
	m_pTransformCom->Move_Pos(&m_vDir, 20.f, fTimeDelta);
}

void CCodyWizard_IceOrbAttRB::PhyscisCreate()
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

void CCodyWizard_IceOrbAttRB::Effect()
{
	PARTICLE tParticle;
	Engine::Reset_Particle(tParticle);

	tParticle.fAcc = 1.f;
	tParticle.fCurrentTime = 0.f;
	tParticle.vEndSize = _vec3(0.01f, 0.01f, 0.01f);
	tParticle.fRotateSpeed = 0.f;

	tParticle.vStartColor = { 180.f * 2, 200.f * 2, 255.f * 2, 255.f * 2.5f };
	tParticle.vEndColor = { 255.f, 255.f, 255.f, 50.f };
	tParticle.vRot = { 0.f, 0.f, 0.f };

	for (_int i = 0; i < 10; ++i)
	{
		_float fRand = CRandoms()(50, 100) * 0.002f;
		tParticle.vStartSize = _vec3(fRand, fRand, fRand);
		tParticle.fVelocity = CRandoms()(0, 10) * 0.1f + 0.5f;
		tParticle.vDir = { CRandoms()(-100, 100) * 0.01f,CRandoms()(-100, 100) * 0.01f , CRandoms()(-100, 100) * 0.01f };
		tParticle.fLifeTime = CRandoms()(1, 5) * 0.1f + 0.8f;
		tParticle.vPos = m_pTransformCom->Get_Position() + _vec3(CRandoms()(-20, 20) * 0.01f, CRandoms()(-20, 20) * 0.01f, CRandoms()(-20, 20) * 0.01f);
		Engine::Emit_ImageParticle(tParticle, 15, 11);
	}

	PARTICLE tParticle2;
	Engine::Reset_Particle(tParticle2);
	Engine::Set_AnimState(tParticle2, 8, 8, 1.f);
	tParticle2.fAcc = 1.f;
	tParticle2.fCurrentTime = 0.f;
	tParticle2.vStartSize = _vec3(3.f, 3.f, 3.f);
	tParticle2.vEndSize = _vec3(3.f, 3.f, 3.f);
	tParticle2.fRotateSpeed = 0.f;
	tParticle2.vStartColor = { 180.f * 2, 200.f * 2, 255.f * 2.5f, 100.f };
	tParticle2.vEndColor = { 180.f * 2, 200.f * 2, 255.f * 2.5f, 0.f };
	tParticle2.vRot = { 0.f, 0.f, 0.f };
	tParticle2.fVelocity = 0.f;
	tParticle2.fLifeTime = 1.f;

	tParticle2.vRot = { 0.f, _float(CRandoms()(1, 360)), 0.f };

	tParticle2.vDir = { 0.f, 0.f, 0.f };
	tParticle2.vPos = m_pTransformCom->Get_Position();
	Engine::Emit_ImageParticle(tParticle2, 23, 4);
}

void CCodyWizard_IceOrbAttRB::Effect_Explosive()
{
	PARTICLE tParticle;
	Engine::Reset_Particle(tParticle);
	Engine::Set_AnimState(tParticle, 8, 8, 1.f);
	tParticle.fAcc = 1.f;
	tParticle.fCurrentTime = 0.f;
	tParticle.vStartSize = _vec3(10.f, 10.f, 10.f);
	tParticle.vEndSize = _vec3(10.f, 10.f, 10.f);
	tParticle.fRotateSpeed = 0.f;
	tParticle.vStartColor = { 180.f * 1.5f, 200.f * 1.5f, 255.f * 2.f, 255.f };
	tParticle.vEndColor = { 180.f * 1.5f, 200.f * 1.5f, 255.f * 2.f, 0.f };
	tParticle.fVelocity = 0.f;
	tParticle.fLifeTime = 1.f;

	tParticle.vRot = { 0.f, 0.f, 0.f };
	tParticle.vDir = { 0.f, 0.f, 0.f };

	tParticle.vPos = m_pTransformCom->Get_Position();
	Engine::Emit_ImageParticle(tParticle, 23, 4);


	Engine::Set_AnimState(tParticle, 8, 8, 1.f);
	tParticle.fLifeTime = 1.f;
	tParticle.vStartSize = _vec3(8.f, 8.f, 8.f);
	tParticle.vEndSize = _vec3(8.f, 8.f, 8.f);
	tParticle.vStartColor = { 180.f * 5.5f, 200.f * 5.5f, 255.f * 7.5f, 255.f * 3.5f };
	tParticle.vPos = m_pTransformCom->Get_Position();
	Engine::Emit_ImageParticle(tParticle, 29, 4);
	tParticle.vStartSize = _vec3(6.f, 6.f, 6.f);
	tParticle.vEndSize = _vec3(6.f, 6.f, 6.f);
	Engine::Emit_ImageParticle(tParticle, 30, 4);
	tParticle.vStartSize = _vec3(4.f, 4.f, 4.f);
	tParticle.vEndSize = _vec3(4.f, 4.f, 4.f);
	Engine::Emit_ImageParticle(tParticle, 29, 4);


	PARTICLE tParticle2;
	Engine::Reset_Particle(tParticle2);
	tParticle2.fAcc = 1.f;
	tParticle2.fCurrentTime = 0.f;
	tParticle2.fRotateSpeed = 0.f;
	tParticle2.vEndSize = _vec3(0.001f, 0.001f, 0.001f);

	tParticle2.vStartColor = { 180.f * 2, 200.f * 2, 255.f * 2.3f, 255.f * 5 };
	tParticle2.vEndColor = { 180.f * 2, 200.f * 2, 255.f * 2.3f, 255.f * 5 };
	tParticle2.bBill = false;
	tParticle2.bGravity = true;
	tParticle2.fGravityForce = 1.5f;
	tParticle2.vPos = m_pTransformCom->Get_Position();

	for (_int i = 0; i < 50; ++i)
	{
		tParticle2.fLifeTime = CRandoms()(0, 2) * 0.1f + 0.5f;
		tParticle2.vRot = { D3DXToRadian(CRandoms()(1, 360)), D3DXToRadian(CRandoms()(1, 360)), D3DXToRadian(CRandoms()(1, 360)) };
		_float fRand = CRandoms()(10, 30) * 0.0001f;
		tParticle2.vStartSize = _vec3(fRand, fRand, fRand);
		tParticle2.fVelocity = _float(CRandoms()(1, 10) + 2.f);
		tParticle2.vDir = { CRandoms()(-100, 100) * 0.1f, CRandoms()(100, 2000) * 0.01f, CRandoms()(-100, 100) * 0.1f };
		Engine::Emit_MeshParticle(tParticle2, L"BreakIce1", 13);
	}


	PARTICLE tParticle3;
	Engine::Reset_Particle(tParticle3);

	tParticle3.fAcc = 1.f;
	tParticle3.fCurrentTime = 0.f;
	tParticle3.vEndSize = _vec3(0.01f, 0.01f, 0.01f);
	tParticle3.fRotateSpeed = 0.f;

	tParticle3.vStartColor = { 180.f * 2, 200.f * 2, 255.f * 2, 255.f * 2.5f };
	tParticle3.vEndColor = { 255.f, 255.f, 255.f, 50.f };
	tParticle3.vRot = { 0.f, 0.f, 0.f };

	for (_int i = 0; i < 100; ++i)
	{
		_float fRand = CRandoms()(50, 100) * 0.003f;
		tParticle3.vStartSize = _vec3(fRand, fRand, fRand);
		tParticle3.fVelocity = CRandoms()(0, 10) * 0.1f + 2.5f;
		tParticle3.vDir = { CRandoms()(-100, 100) * 0.01f,CRandoms()(-100, 100) * 0.01f , CRandoms()(-100, 100) * 0.01f };
		tParticle3.fLifeTime = CRandoms()(1, 5) * 0.1f + 0.8f;
		tParticle3.vPos = m_pTransformCom->Get_Position() + _vec3(CRandoms()(-30, 30) * 0.01f, CRandoms()(-30, 30) * 0.01f, CRandoms()(-30, 30) * 0.01f);
		Engine::Emit_ImageParticle(tParticle3, 15, 11);
	}


	PARTICLE tParticle4;
	Engine::Reset_Particle(tParticle4);
	Engine::Set_AnimState(tParticle4, 2, 2, 5.f);
	tParticle4.fAcc = 1.f;
	tParticle4.fCurrentTime = 0.f;
	tParticle4.fRotateSpeed = 0.f;
	tParticle4.vEndSize = _vec3(0.1f, 0.1f, 0.1f);

	tParticle4.vStartColor = { 180.f * 2, 200.f * 2, 255.f * 3.f, 255.f * 5 };
	tParticle4.vEndColor = { 180.f * 2, 200.f * 2, 255.f * 3.f, 255.f * 5 };
	tParticle4.bBill = false;
	tParticle4.vPos = m_pTransformCom->Get_Position();

	for (_int i = 0; i < 30; ++i)
	{
		tParticle4.fLifeTime = CRandoms()(0.3f, 0.5f);
		_float fRand = CRandoms()(1.f, 4.f);
		tParticle4.vStartSize = _vec3(CRandoms()(1.f, 2.f), fRand, 2.f);
		tParticle4.fVelocity = CRandoms()(6.f, 10.f);

		//tParticle4.vDir = { 1.f, 0.f, 0.f };
		tParticle4.vDir = { CRandoms()(-100, 100) * 0.01f, 0.f, CRandoms()(-100, 100) * 0.01f };
		_float fRadian = atan2f(tParticle4.vDir.x, tParticle4.vDir.z);
		tParticle4.vRot = { D3DXToRadian(90.f), fRadian, 0.f };

		Engine::Emit_ImageParticle(tParticle4, 15, 21);
	}
}

CCodyWizard_IceOrbAttRB * CCodyWizard_IceOrbAttRB::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vDir, CLight* pLight, void * pVoid)
{
	CCodyWizard_IceOrbAttRB*	pInstance = new CCodyWizard_IceOrbAttRB(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(vDir, pLight, pVoid)))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCodyWizard_IceOrbAttRB::Free(void)
{
	m_pRigidActor->release();
	CPlayer::Free();
}
