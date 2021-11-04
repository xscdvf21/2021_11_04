#include "stdafx.h"
#include "BaboonBomb.h"
#include "CollisionMgr.h"
#include "BaboonGround.h"
#include "BaboonBombBullet.h"
#include "BaboonBombUpBullet.h"
#include "Export_Function.h"
CBaboonBomb::CBaboonBomb(LPDIRECT3DDEVICE9 pGraphicDev)
	:CEnermy(pGraphicDev)
	, m_bTram(false), m_bTramStart(true), m_fTramPower(0.f)
	, m_fTramTime(0.f), m_fTramHeight(0.f), m_fDuration(3.f)
	, m_bAnglecheck(false)
{
}

CBaboonBomb::~CBaboonBomb()
{
}

HRESULT CBaboonBomb::Ready_Object(void * pArg, CGameObject * pPlayer, _int iIndex)
{
	m_tColInfo.fRadius = 1.f;
	
	FAILED_CHECK_RETURN(Add_Component(pArg), E_FAIL);


	if (nullptr != pPlayer)
	{
		m_pTarget = pPlayer;
		m_pTarget->AddRef();
	}

	m_tShader.m_fRimDist = 10.f;
	m_tShader.m_iRimPower = 50;
	m_tShader.m_vRimColor = _vec4(0.15f, 0.15f, 0.15f, 1.f);

	PhyscisCreate();

	m_iCount = iIndex;

	//PhyscisCreate();
	return S_OK;
}

_int CBaboonBomb::Update_Object(const _float & fTimeDelta)
{
	if (m_bDead)
	{
		Engine::CLayer*		pLayer = Engine::Get_Layer(L"GameObject");
		Engine::CGameObject*		pGameObject = nullptr;

		_tchar tagTemp[MAX_PATH];
		wsprintf(tagTemp, L"BombBullt %d", m_iCount);

		pGameObject = CBaboonBombBullet::Create(m_pGraphicDev, m_pTransformCom->Get_Position());
		NULL_CHECK_RETURN(pGameObject, 0);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagTemp, pGameObject, Engine::Get_Scene()), 0 );


		wsprintf(tagTemp, L"BombUpBullt %d", m_iCount);
		pGameObject = CBaboonBombUpBullet::Create(m_pGraphicDev, m_pTransformCom->Get_Position());
		NULL_CHECK_RETURN(pGameObject, 0);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagTemp, pGameObject, Engine::Get_Scene()), 0);



		PARTICLE tParticle;
		Engine::Reset_Particle(tParticle);

		tParticle.fAcc = 1.f;
		tParticle.fCurrentTime = 0.f;
		tParticle.vEndSize = _vec3(0.01f, 0.01f, 0.01f);
		tParticle.fRotateSpeed = 0.f;

		tParticle.vStartColor = { 255.f * 3.5f, 54.f* 3.5f, 0.f* 3.5f, 255.f * 3.5f };
		tParticle.vEndColor = { 255.f, 54.f, 0.f, 255.f  * 3.5f };
		tParticle.vRot = { 0.f, 0.f, 0.f };
		tParticle.bGravity = true;
		tParticle.fGravityForce = 1.f;
		_float fRand;
		for (_int i = 0; i < 50; ++i)
		{
			tParticle.fVelocity = CRandoms()(12.5f, 17.5f);
			tParticle.fLifeTime = CRandoms()(1.5f, 3.f);
			fRand = CRandoms()(0.1f, 0.2f);
			tParticle.vStartSize = _vec3(fRand, fRand, fRand);

			tParticle.vDir = { CRandoms()(-1.f, 1.f),  CRandoms()(1.f, 3.f), CRandoms()(-1.f, 1.f) };
			tParticle.vPos = m_pTransformCom->Get_Position() + _vec3(CRandoms()(-0.3f, 0.3f), CRandoms()(-0.3f, 0.3f), CRandoms()(-0.3f, 0.3f));
			Engine::Emit_ImageParticle(tParticle, 15, 11);
		}

		for (_int i = 0; i < 30; ++i)
		{
			tParticle.fVelocity = CRandoms()(12.5f, 17.5f);
			tParticle.fLifeTime = CRandoms()(1.5f, 3.f);
			fRand = CRandoms()(0.2f, 0.4f);
			tParticle.vStartSize = _vec3(fRand, fRand, fRand);
			tParticle.vStartColor = { 255.f * 3.5f, 54.f* 3.5f, 0.f* 3.5f, 255.f * 3.5f};

			tParticle.vDir = { CRandoms()(-1.f, 1.f),  CRandoms()(1.f, 3.f), CRandoms()(-1.f, 1.f) };
			tParticle.vPos = m_pTransformCom->Get_Position() + _vec3(CRandoms()(-0.3f, 0.3f), CRandoms()(-0.3f, 0.3f), CRandoms()(-0.3f, 0.3f));
			Engine::Emit_ImageParticle(tParticle, 15, 11);
		}

		PARTICLE tParticle4;
		Engine::Reset_Particle(tParticle4);
		Engine::Set_AnimState(tParticle4, 2, 2, 5.f);
		tParticle4.fAcc = 1.f;
		tParticle4.fCurrentTime = 0.f;
		tParticle4.fRotateSpeed = 0.f;
		tParticle4.vEndSize = _vec3(0.01f, 0.01f, 0.01f);

		tParticle4.vStartColor = { 255.f * 2, 255.f * 2, 255.f * 3.f, 255.f  };
		tParticle4.vEndColor = { 255.f * 2, 255.f * 2, 255.f * 3.f, 255.f };
		tParticle4.vPos = m_pTransformCom->Get_Position();

		for (_int i = 0; i < 30; ++i)
		{
			tParticle4.fLifeTime = CRandoms()(1.f, 1.5f);
			_float fRand = CRandoms()(0.1f, 0.15f);
			tParticle4.vStartSize = _vec3(fRand, fRand, fRand);
			tParticle4.fVelocity = CRandoms()(15.f, 20.f);

			//tParticle4.vDir = { 1.f, 0.f, 0.f };
			tParticle4.vDir = { CRandoms()(-1.f, 1.f), CRandoms()(1.f, 2.f), CRandoms()(-1.f, 1.f)};

			Engine::Emit_ImageParticle(tParticle4, 15, 11);
		}

		PARTICLE tParticle2;
		Engine::Reset_Particle(tParticle2);
		Engine::Set_AnimState(tParticle2, 8, 8, 1.f);

		tParticle2.fAcc = 1.f;
		tParticle2.fCurrentTime = 0.f;
		tParticle2.vStartSize = _vec3(10.f, 10.f, 10.f);
		tParticle2.vEndSize = _vec3(10.f, 10.f, 10.f);
		tParticle2.fRotateSpeed = 0.f;
		tParticle2.vStartColor = { 255.f * 3.5f, 54.f* 3.5f, 0.f* 3.5f, 255.f };
		tParticle2.vEndColor = { 255.f * 3.5f, 54.f* 3.5f, 0.f* 3.5f, 155.f };
		tParticle2.vRot = { 0.f, 0.f, 0.f };
		tParticle2.fVelocity = 0.f;
		tParticle2.fLifeTime = 0.5f;
		tParticle2.vRot = { 0.f, 0.f, 0.f };

		tParticle2.vDir = { 0.f, 0.f, 0.f };
		for (_int i = 0; i < 3; ++i)
		{
			tParticle2.vPos = m_pTransformCom->Get_Position() + _vec3(CRandoms()(-20, 20) * 0.1f, 1.3f + CRandoms()(0, 15) * 0.1f, CRandoms()(-20, 20) * 0.1f);
			Engine::Emit_ImageParticle(tParticle2, 23, 4);
		}

		return OBJ_DEAD;
	}


	CBaboonGround*			pGround = (CBaboonGround*)Get_GameObject(L"GameObject", L"BaboonGround");
	CTransform*				pGroundTrans =(CTransform*)pGround->Get_Component(L"Com_Transform", ID_DYNAMIC);
	

	_vec3 vGroundPos = pGroundTrans->Get_Position();
	
	if (vGroundPos.y + 0.5f >= m_pTransformCom->Get_Position().y)
	{
		m_bDead = true;
	}
	

	//_vec3 vPos;
	//vPos = m_pTransformCom->Get_Position();

	//PxTransform pxTransform = m_pRigidActor->getGlobalPose();
	//pxTransform.p.x = vPos.x;
	//pxTransform.p.y = vPos.y;
	//pxTransform.p.z = vPos.z;

	//m_pRigidActor->setGlobalPose(pxTransform);


	Move(fTimeDelta);

	CCollisionMgr::GetInstance()->Add_EnermyBulletList(this);

	Engine::CGameObject::Update_Object(fTimeDelta);
	Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);

	return OBJ_NOEVENT;
}

_int CBaboonBomb::LateUpdate_Object(const _float & fTimeDelta)
{
	return _int();
}

void CBaboonBomb::Render_Object(const _int & iIndex)
{
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect, iIndex), );

	_uint	iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);		// 1인자 : 현재 쉐이더 파일이 갖고 있는 pass의 최대 개수, 2인자 : 시작하는 방식(default)
										//pEffect->BeginPass(0);
	m_pMeshCom->Render_Meshes(pEffect, 28);

	pEffect->End();

	Safe_Release(pEffect);

	m_tColInfo.matWorld = *m_pTransformCom->Get_WorldMatrix();
	m_pColliderCom->Render_Collider(m_tColInfo.matWorld, false);
}

HRESULT CBaboonBomb::Add_Component(void * pArg)
{
	Engine::CComponent*		pComponent = nullptr;

	OBJINFO tTemp;
	ZeroMemory(&tTemp, sizeof(OBJINFO));

	if (nullptr != pArg)
	{
		memcpy(&tTemp, pArg, sizeof(OBJINFO));
	}

	// CStaticMesh
	pComponent = m_pMeshCom = static_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(RESOURCE_SB, L"IceOrb"));
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

	// Texture
	pComponent = m_pTextureCom = static_cast<Engine::CTexture*>(Engine::Clone_Resource(0, L"Particle"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture", pComponent);

	m_pTransformCom->Set_Scale(tTemp.vScale*0.5f);
	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(tTemp.vAngle.x));
	m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(tTemp.vAngle.y));
	m_pTransformCom->Rotation(ROT_Z, D3DXToRadian(tTemp.vAngle.z));
	m_pTransformCom->Set_Pos(tTemp.vPos);

	return S_OK;
}

HRESULT CBaboonBomb::SetUp_ConstantTable(LPD3DXEFFECT & pEffect, const _int & iIndex)
{
	_matrix			matWorld, matView, matProj, matInv;

	m_pTransformCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	D3DXMatrixInverse(&matInv, nullptr, &matView);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	m_pTextureCom->Set_Texture(pEffect, "g_Add1Texture", 15);

	pEffect->SetVector("g_vColor", &_vec4(255.f, 255.f, 255.f, 255.f));
	pEffect->SetFloat("g_fAmount", 0.f);
	pEffect->SetFloat("g_fMaterialIndex", 0.f);

	pEffect->SetFloat("g_fRimDist", m_tShader.m_fRimDist);
	pEffect->SetInt("g_iRimPower", m_tShader.m_iRimPower);
	pEffect->SetVector("g_vRimColor", &m_tShader.m_vRimColor);
	pEffect->SetBool("g_bRim", m_bRimLight);

	pEffect->SetVector("g_vCamPos", &_vec4(matInv._41, matInv._42, matInv._43, 1.f));

	return S_OK;
}

void CBaboonBomb::Move(const _float & fTimeDelta)
{
	if (m_bTramStart)
	{
		CTransform*		pTarget = (CTransform*)m_pTarget->Get_Component(L"Com_Transform", ID_DYNAMIC);
		_vec3 vTargetPos = pTarget->Get_Position();
		m_vSetPos = pTarget->Get_Position();
		m_vTramDir = vTargetPos - m_pTransformCom->Get_Position();
		D3DXVec3Normalize(&m_vTramDir, &m_vTramDir);

		m_fTramPower = ((m_vTramDir.y + 1.f) - m_pTransformCom->Get_Position().y - (m_fDuration * m_fDuration) * -GRAVITY) / m_fDuration;

		m_vTramDis = vTargetPos - m_pTransformCom->Get_Position();
		m_vTramDis.y = 0.f;
		m_fTramHeight = m_pTransformCom->Get_Position().y;
		m_bTramStart = false;
	}

	m_fTramTime += fTimeDelta;

	_float fHeight = m_fTramHeight + ((m_fTramTime * m_fTramTime) * (-GRAVITY) / 1) + (m_fTramTime * m_fTramPower);
	_vec3 vDstPos = m_pTransformCom->Get_Position() + (m_vTramDir * D3DXVec3Length(&m_vTramDis) * fTimeDelta / m_fDuration);

	m_pTransformCom->Set_Pos(_vec3(vDstPos.x, fHeight, vDstPos.z));
	//Effect_Thrust(fTimeDelta);
	Effect();
}

void CBaboonBomb::PhyscisCreate()
{
	auto* pPhysics = Engine::Get_Physics();
	_vec3 vPos = m_pTransformCom->Get_Position();
	PxTransform tTransform(vPos.x, vPos.y, vPos.z);

	PxMaterial* pMaterial = pPhysics->createMaterial(0.1f, 0.1f, 0.1f);

	PxShape* pShape = pPhysics->createShape(PxBoxGeometry(0.01f, 0.01f, 0.01f), *pMaterial, true);
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

void CBaboonBomb::Effect()
{
	PARTICLE tParticle;
	Engine::Reset_Particle(tParticle);

	tParticle.fAcc = 1.f;
	tParticle.fCurrentTime = 0.f;
	tParticle.vEndSize = _vec3(0.01f, 0.01f, 0.01f);
	tParticle.fRotateSpeed = 0.f;

	tParticle.vStartColor = { 255.f * 3.5f, 54.f* 3.5f, 0.f* 3.5f, 255.f };
	tParticle.vEndColor = { 255.f, 255.f, 255.f, 255.f };
	tParticle.vRot = { 0.f, 0.f, 0.f };
	tParticle.bGravity = true;
	tParticle.fGravityForce = 1.5f;
	_float fRand;
	for (_int i = 0; i < 5; ++i)
	{
		tParticle.fVelocity = CRandoms()(2.5f, 3.5f);
		tParticle.fLifeTime = CRandoms()(0.5f, 1.f);
		fRand = CRandoms()(0.1f, 0.2f);
		tParticle.vStartSize = _vec3(fRand, fRand, fRand);

		tParticle.vDir = { CRandoms()(-1.f, 1.f),  CRandoms()(0.f, 1.f), CRandoms()(-1.f, 1.f) };
		tParticle.vPos = m_pTransformCom->Get_Position() + _vec3(CRandoms()(-0.3f, 0.3f), CRandoms()(-0.3f, 0.3f), CRandoms()(-0.3f, 0.3f));
		Engine::Emit_ImageParticle(tParticle, 15, 11);
	}

	for (_int i = 0; i < 3; ++i)
	{
		tParticle.fVelocity = CRandoms()(2.5f, 3.5f);
		tParticle.fLifeTime = CRandoms()(0.3f, 0.5f);
		fRand = CRandoms()(0.2f, 0.4f);
		tParticle.vStartSize = _vec3(fRand, fRand, fRand);
		tParticle.vStartColor = { 255.f * 3.5f, 255.f* 3.5f, 255.f* 3.5f, 255.f };

		tParticle.vDir = { CRandoms()(-1.f, 1.f),  CRandoms()(0.f, 1.f), CRandoms()(-1.f, 1.f) };
		tParticle.vPos = m_pTransformCom->Get_Position() + _vec3(CRandoms()(-0.3f, 0.3f), CRandoms()(-0.3f, 0.3f), CRandoms()(-0.3f, 0.3f));
		Engine::Emit_ImageParticle(tParticle, 15, 11);
	}

	PARTICLE tParticle2;
	Engine::Reset_Particle(tParticle2);
	Engine::Set_AnimState(tParticle2, 8, 8, 1.f);
	tParticle2.fAcc = 1.f;
	tParticle2.fCurrentTime = 0.f;
	tParticle2.vStartSize = _vec3(2.3f, 2.3f, 2.3f);
	tParticle2.vEndSize = _vec3(0.5f, 0.5f, 0.5f);
	tParticle2.fRotateSpeed = 0.f;
	tParticle2.vStartColor = { 30.f, 30.f, 30.f, 255.f * 1.5f };
	tParticle2.vEndColor = { 30.f, 30.f, 30.f, 0.f };
	tParticle2.fVelocity = 0.f;
	tParticle2.fLifeTime = 0.5f;
	tParticle2.vRot = { 0.f, 0.f, 0.f };

	tParticle2.vDir = { 0.f, 0.f, 0.f };
	tParticle2.vPos = m_pTransformCom->Get_Position() + _vec3(CRandoms()(-0.1f, 0.1f), 0.f, CRandoms()(-0.1f, 0.1f));
	Engine::Emit_ImageParticle(tParticle2, 23, 4);


	tParticle2.vStartSize = _vec3(1.8f, 1.8f, 1.8f);
	tParticle2.fLifeTime = 0.35f;
	tParticle2.vStartColor = { 255.f * 3.5f, 54.f * 3.5f, 0.f * 3.5f, 255.f };
	tParticle2.vEndColor = { 255.f * 2.5f, 54.f * 2.5f, 0.f * 2.5f, 255.f };
	tParticle2.vPos = m_pTransformCom->Get_Position() + _vec3(CRandoms()(-0.1f, 0.1f), 0.f, CRandoms()(-0.1f, 0.1f));
	Engine::Emit_ImageParticle(tParticle2, 30, 26);
}

void CBaboonBomb::Effect_Thrust(const _float & fTimeDelta)
{

	
}


CBaboonBomb * CBaboonBomb::Create(LPDIRECT3DDEVICE9 pGraphicDev, void * pArg, CGameObject * pPlayer, _int iIndex)
{
	CBaboonBomb*	pInstance = new CBaboonBomb(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pArg, pPlayer, iIndex)))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBaboonBomb::Free(void)
{
	m_pTarget->Release();
	CEnermy::Free();
}
