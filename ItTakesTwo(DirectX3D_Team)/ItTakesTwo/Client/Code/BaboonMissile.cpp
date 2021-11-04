#include "stdafx.h"
#include "BaboonMissile.h"
#include "Enermy.h"
#include "CollisionMgr.h"
#include "SpaceBaboon.h"
#include "Export_Function.h"
#include "MayCamera.h"
#include "CodyCamera.h"

CBaboonMissile::CBaboonMissile(LPDIRECT3DDEVICE9 pGraphicDev)
	:CPlayer(pGraphicDev)
	, m_fSpeed(5.f), m_fChangeTime(0.f), m_fChaseTime(0.f)
	, m_bChange(false), m_fLifeTime(0.f)
	, m_bChaseMove(true), m_bDownMove(false), m_bAtt(false), m_fAngle(0.f), m_fTurnSpeed(D3DXToRadian(2.f)), m_bTarget(false)
	, m_bStartMissile(false), m_bPlayerAtt(false)
{
}

CBaboonMissile::~CBaboonMissile()
{
}

HRESULT CBaboonMissile::Ready_Object(void * pArg, CGameObject* pPlayer, _bool bTarget, _uint iIndex)
{
	m_tColInfo.fRadius = 1.f;
	FAILED_CHECK_RETURN(Add_Component(pArg), E_FAIL);


	if (nullptr != pPlayer)
	{
		m_pTarget = pPlayer;
		m_pTarget->AddRef();
	}

	m_bTarget = bTarget;
	m_iIndex = iIndex;

	return S_OK;
}

_int CBaboonMissile::Update_Object(const _float & fTimeDelta)
{
	if (m_bStart)
	{
		m_pTrigger = CTrigger::CreateSphereTirgger(m_pScene, this, m_pTransformCom->Get_Position(), 1.f, TRIGGER_SPACE_MISSILE_BOSS, FilterGroup::eDefaultTrigger);
		m_pTrigger2 = CTrigger::CreateSphereTirgger(m_pScene, this, m_pTransformCom->Get_Position(), 1.f, TRIGGER_SPACE_MiSSILE_RIDE, FilterGroup::eDefaultTrigger);
		m_pTrigger2->Set_Interactable(false);
		m_bStart = false;
	}

	_vec3 vPos;
	vPos = m_pTransformCom->Get_Position();
	m_pTrigger->Set_Pos(m_pTransformCom->Get_Position());
	m_pTrigger2->Set_Pos(m_pTransformCom->Get_Position());

	if (m_bDead)
	{
		if (m_bTarget) // 트루일때가 코디.
		{
			CSpaceBaboon*	pBaboon = (CSpaceBaboon*)Engine::Get_GameObject(L"GameObject", L"SpaceBoss");
			if (pBaboon)
				pBaboon->Set_TargetCody();
		}
		else if (!m_bTarget)
		{
			CSpaceBaboon*	pBaboon = (CSpaceBaboon*)Engine::Get_GameObject(L"GameObject", L"SpaceBoss");
			if (pBaboon)
				pBaboon->Set_TargetMay();
		}

		m_pTrigger->Set_Interactable(false);
		Engine::StopSound(CSoundMgr::CHANNELID::Space_Missile_Move);
		Engine::StopSound(CSoundMgr::CHANNELID::Space_Missile_Start);
		PlaySoundOnce(L"Rocket_Dead.wav", CSoundMgr::CHANNELID::Space_Missile_Dead);

		CMayCamera* pMayCam = (CMayCamera*)Engine::Get_GameObject(L"GameObject", L"MayCamera");
		CCodyCamera* pCodyCam = (CCodyCamera*)Engine::Get_GameObject(L"GameObject", L"CodyCamera");
		pMayCam->Set_Shake();
		pCodyCam->Set_Shake();


		PARTICLE tParticle;
		Engine::Reset_Particle(tParticle);
		Engine::Set_AnimState(tParticle, 8, 8, 1.f);
		tParticle.fAcc = 1.f;
		tParticle.fCurrentTime = 0.f;
		tParticle.vStartSize = _vec3(10.f, 10.f, 10.f);
		tParticle.vEndSize = _vec3(10.f, 10.f, 10.f);
		tParticle.fRotateSpeed = 0.f;
		tParticle.vStartColor = { 255.f * 2.f, 130.f * 2.f, 85.f * 2.f, 255.f };
		tParticle.vEndColor = { 255.f * 2.f, 130.f * 2.f, 85.f * 2.f, 0.f };
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
		tParticle.vStartColor = { 0.f ,0.f, 0.f, 255.f };
		tParticle.vEndColor = { 0.f ,0.f, 0.f, 155.f };
		tParticle.vPos = m_pTransformCom->Get_Position();
		Engine::Emit_ImageParticle(tParticle, 29, 4);
		tParticle.vStartSize = _vec3(6.f, 6.f, 6.f);
		tParticle.vEndSize = _vec3(6.f, 6.f, 6.f);
		Engine::Emit_ImageParticle(tParticle, 30, 4);
		tParticle.vStartColor = { 255.f * 2.f, 130.f * 2.f, 85.f * 2.f, 255.f * 3.5f };
		tParticle.vEndColor = { 255.f * 2.f, 130.f * 2.f, 85.f * 2.f, 0.f };
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

		tParticle2.vStartColor = { 255.f * 2, 130.f * 2, 85.f * 2.f, 255.f * 5 };
		tParticle2.vEndColor = { 255.f * 2, 130.f * 2, 85.f * 2.f, 255.f * 5 };
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

		tParticle3.vStartColor = { 255.f * 2, 130.f * 2, 85.f * 2, 255.f * 2.5f };
		tParticle3.vEndColor = { 255.f, 130.f, 85.f, 50.f };
		tParticle3.vRot = { 0.f, 0.f, 0.f };

		for (_int i = 0; i < 50; ++i)
		{
			_float fRand = CRandoms()(50, 100) * 0.003f;
			tParticle3.vStartSize = _vec3(fRand, fRand, fRand);
			tParticle3.fVelocity = CRandoms()(0, 10) * 0.1f + 2.5f;
			tParticle3.vDir = { CRandoms()(-100, 100) * 0.01f,CRandoms()(-100, 100) * 0.01f , CRandoms()(-100, 100) * 0.01f };
			tParticle3.fLifeTime = CRandoms()(1, 5) * 0.1f + 0.8f;
			tParticle3.vPos = m_pTransformCom->Get_Position() + _vec3(CRandoms()(-30, 30) * 0.01f, CRandoms()(-30, 30) * 0.01f, CRandoms()(-30, 30) * 0.01f);
			Engine::Emit_ImageParticle(tParticle3, 15, 11);
		}


		return OBJ_DEAD;
	}

	m_fLifeTime += fTimeDelta;
	if (m_fLifeTime > m_fDeadTime)
		m_bDead = true;

	Engine::CGameObject::Update_Object(fTimeDelta);
	Move(fTimeDelta);

	if (m_bChange)
	{
		m_pTrigger->Set_TriggerType(TRIGGER_SPACE_MISSILE_PLAYER);
		m_bChange = false;
	}

	Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);

	return OBJ_NOEVENT;
}

_int CBaboonMissile::LateUpdate_Object(const _float & fTimeDelta)
{
	return _int();
}

void CBaboonMissile::Render_Object(const _int & iIndex)
{
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect, iIndex), );

	_uint	iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);		// 1인자 : 현재 쉐이더 파일이 갖고 있는 pass의 최대 개수, 2인자 : 시작하는 방식(default)
										//pEffect->BeginPass(0);
	m_pMeshCom->Render_Meshes(pEffect, 6);

	pEffect->End();

	Safe_Release(pEffect);

	m_tColInfo.matWorld = *m_pTransformCom->Get_WorldMatrix();
	m_pColliderCom->Render_Collider(m_tColInfo.matWorld, false);
}

HRESULT CBaboonMissile::Add_Component(void * pArg)
{
	Engine::CComponent*		pComponent = nullptr;

	OBJINFO tTemp;
	ZeroMemory(&tTemp, sizeof(OBJINFO));

	if (nullptr != pArg)
	{
		memcpy(&tTemp, pArg, sizeof(OBJINFO));
	}

	// CStaticMesh
	pComponent = m_pMeshCom = static_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(RESOURCE_SB, L"BaboonMissile"));
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


	m_pTransformCom->Set_Scale(tTemp.vScale);
	m_pTransformCom->Rotation(ROT_X, tTemp.vAngle.x);
	m_pTransformCom->Rotation(ROT_Y, tTemp.vAngle.y);
	m_pTransformCom->Rotation(ROT_Z, tTemp.vAngle.z);
	m_pTransformCom->Set_Pos(tTemp.vPos);

	////PhysX
	//auto* pPhysics = Engine::Get_Physics();
	//_vec3 vPos = m_pTransformCom->Get_Position();
	//PxTransform tTransform(vPos.x, vPos.y, vPos.z);

	//PxMaterial* pMaterial = pPhysics->createMaterial(0.5f, 0.5f, 0.0f);

	//PxShape* pShape = pPhysics->createShape(PxBoxGeometry(1, 1, 1), *pMaterial, true);
	////밑 두개중 하나는 false 둘다 true안?
	//pShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true);
	//pShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, false);

	//pShape->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, true);
	//m_pRigidActor = PxCreateDynamic(*pPhysics, tTransform, *pShape, 1);

	//auto* pBody = static_cast<PxRigidDynamic*>(m_pRigidActor);

	//// 그래비티를 끈다.
	//pBody->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, false);

	//// 질량설정
	//pBody->setRigidDynamicLockFlags(PxRigidDynamicLockFlag::eLOCK_ANGULAR_X | PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y | PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z);
	//pBody->setName((char*)this);
	//pShape->setName((char*)this);
	return S_OK;
}

HRESULT CBaboonMissile::SetUp_ConstantTable(LPD3DXEFFECT & pEffect, const _int & iIndex)
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
	pEffect->SetFloat("g_fMaterialIndex", 1.f);
	pEffect->CommitChanges();


	return S_OK;
}

void CBaboonMissile::Effect_Thrust(const _float & fTimeDelta, const _int& iIndex)
{
	//PARTICLE tParticle;
	//Engine::Reset_Particle(tParticle);

	//tParticle.fAcc = 1.f;
	//tParticle.fCurrentTime = 0.f;
	//tParticle.vEndSize = _vec3(0.01f, 0.01f, 0.01f);
	//tParticle.fRotateSpeed = 0.f;

	//tParticle.vStartColor = { 255.f * 2, 125.f * 2, 85.f * 2, 255.f * 2.5f };
	//tParticle.vEndColor = { 255.f, 125.f, 255.f, 85.f };
	//tParticle.vRot = { 0.f, 0.f, 0.f };
	//tParticle.bGravity = true;
	//tParticle.fGravityForce = 0.5f;

	//for (_int i = 0; i < 5; ++i)
	//{
	//	tParticle.fVelocity = CRandoms()(3.5f, 5.5f);
	//	tParticle.fLifeTime = CRandoms()(0.5f, 1.f);
	//	_float fRand = CRandoms()(30, 80) * 0.001f;
	//	tParticle.vStartSize = _vec3(fRand, fRand, fRand);

	//	tParticle.vDir = { CRandoms()(-1.f, 1.f),  CRandoms()(0.f, 1.f), CRandoms()(-1.f, 1.f) };
	//	tParticle.vPos = m_vUltHitPos + _vec3(CRandoms()(-0.3f, 0.3f), CRandoms()(-0.3f, 0.3f), CRandoms()(-0.3f, 0.3f));
	//	Engine::Emit_ImageParticle(tParticle, 15, 11);
	//}

	//for (_int i = 0; i < 3; ++i)
	//{
	//	tParticle.fVelocity = CRandoms()(10.5f, 15.5f);
	//	tParticle.fLifeTime = CRandoms()(0.3f, 0.5f);
	//	tParticle.vStartSize = _vec3(0.02f, 0.02f, 0.02f);
	//	tParticle.vStartColor = { 255.f * 2, 200.f * 2, 85.f * 2, 255.f * 2.5f };

	//	tParticle.vDir = { CRandoms()(-1.f, 1.f),  CRandoms()(0.f, 1.f), CRandoms()(-1.f, 1.f) };
	//	tParticle.vPos = m_vUltHitPos + _vec3(CRandoms()(-0.3f, 0.3f), CRandoms()(-0.3f, 0.3f), CRandoms()(-0.3f, 0.3f));
	//	Engine::Emit_ImageParticle(tParticle, 15, 11);
	//}
	if (m_iEffectOn > 0)
	{
		--m_iEffectOn;
		return;
	}
	else
	{
		m_iEffectOn = 3;
	}
	_vec3 vPos = m_pTransformCom->Get_Position();
	PARTICLE tParticle2;
	Engine::Reset_Particle(tParticle2);
	Engine::Set_AnimState(tParticle2, 8, 8, 1.f);
	tParticle2.fAcc = 1.f;
	tParticle2.fCurrentTime = 0.f;
	tParticle2.vStartSize = _vec3(5.f, 5.f, 5.f);
	tParticle2.vEndSize = _vec3(4.f, 4.f, 4.f);
	tParticle2.fRotateSpeed = 0.f;
	tParticle2.vStartColor = { 30.f, 30.f, 30.f, 255.f * 1.5f };
	tParticle2.vEndColor = { 30.f, 30.f, 30.f, 0.f };
	tParticle2.vRot = { 0.f, 0.f, D3DXToRadian(CRandoms()(0.f, 360.f)) };
	tParticle2.fVelocity = 0.f;
	tParticle2.fLifeTime = 0.7f;
	tParticle2.vRot = { 0.f, 0.f, 0.f };

	tParticle2.vDir = { 0.f, 0.f, 0.f };
	tParticle2.vPos = vPos + _vec3(CRandoms()(-0.1f, 0.1f), 0.f, CRandoms()(-0.1f, 0.1f));
	Engine::Emit_ImageParticle(tParticle2, 23, 4);


	tParticle2.vStartSize = _vec3(2.f, 2.f, 2.f);
	tParticle2.vEndSize = _vec3(3.2f, 3.2f, 3.2f);
	tParticle2.fLifeTime = 0.6f;
	//tParticle2.vStartColor = { 255.f * 3.5f, 125.f* 3.5f, 85.f* 3.5f, 255.f };

	//tParticle2.vStartColor = { 255.f * CRandoms()(3.5f,5.5f), 125.f* CRandoms()(3.5f,5.5f), 85.f* CRandoms()(3.5f,5.5f), 255.f };

	switch (iIndex)
	{
	case 0:
		tParticle2.vStartColor = { 255.f * 3.5f, 45.f* 3.5f, 170.f* 3.5f, 255.f };
		break;
	case 1:

		tParticle2.vStartColor = { 75.f * 2.5f, 255.f * 2.5f, 125.f * 2.5f, 255.f };
		break;
	case 2:
		tParticle2.vStartColor = { 75.f * 3.5f, 155.f * 3.5f, 255.f * 3.5f, 255.f };
		break;
	default:
		break;
	}
	tParticle2.vEndColor = { 255.f, 65.f, 35.f, 0.f };
	tParticle2.vPos = vPos + _vec3(CRandoms()(-0.1f, 0.1f), 0.f, CRandoms()(-0.1f, 0.1f));
	Engine::Emit_ImageParticle(tParticle2, 30, 26);
}
void CBaboonMissile::Set_Att(_bool bAtt)
{
	m_bAtt = bAtt;
	m_pTrigger->Set_Interactable(true);
}

void CBaboonMissile::Set_BaboonDead()
{
	m_fDeadTime = 9999.f;
	m_fChaseTime = 6.f;
}

void CBaboonMissile::Move(const _float & fTimeDelta)
{
	Chase_Move(fTimeDelta); //추격
	Down_Move(fTimeDelta);
	Att_Move(fTimeDelta);
}

void CBaboonMissile::Chase_Move(const _float & fTimeDelta)
{
	if (!m_bChaseMove)
		return;
	if (m_bChaseStart)
	{
		PlaySoundW(L"Rocket_MoveStart.wav", CSoundMgr::CHANNELID::Space_Missile_Start, 0.2f);
		m_bChaseStart = false;
	}
	if (Engine::Is_Play_End(CSoundMgr::CHANNELID::Space_Missile_Start))
	{
		PlaySoundW(L"Rocket_Moving.wav", CSoundMgr::CHANNELID::Space_Missile_Move);
	}
	m_fChaseTime += fTimeDelta;


	_vec3 vTargetPos; //타겟의 포스
	_vec3 vPos; //현재 몬스터(자신)	위치
	_vec3 vLook; //현재 몬스터(자신)	룩방향
	_vec3 vBack;
	_vec3 vRight; //현재 몬스터(자신) 라이트
	_vec3 vLeft; //현재 몬스터(자신) 레프트
	_vec3 vDown;
	_vec3 vDir;
	_vec3 vUp;
	_float fDir = 0.f;

	CTransform*			pTargetTrans = (CTransform*)m_pTarget->Get_Component(L"Com_Transform", ID_DYNAMIC);


	m_vTargetPos = pTargetTrans->Get_Position();
	vPos = m_pTransformCom->Get_Position();
	m_vTargetPos.y += 1.f;
	m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
	m_pTransformCom->Get_Info(INFO_RIGHT, &vRight);
	m_pTransformCom->Get_Info(INFO_UP, &vUp);

	D3DXVec3Normalize(&vLook, &vLook);
	D3DXVec3Normalize(&vRight, &vRight);
	D3DXVec3Normalize(&vUp, &vUp);

	vDir = m_vTargetPos - vPos;
	D3DXVec3Normalize(&vDir, &vDir);

	_vec3 vAxis;

	D3DXVec3Cross(&vAxis, &vDir, &vLook);
	D3DXVec3Normalize(&vAxis, &vAxis);

	_matrix matWorld, matRot;
	D3DXMatrixRotationAxis(&matRot, &vAxis, -acosf(D3DXVec3Dot(&vLook, &vDir)));
	matWorld = *m_pTransformCom->Get_WorldMatrix();
	//D3DXMatrixScaling(&matWorld, 0.01f, 0.01f, 0.01f);
	m_pTransformCom->Set_WorldMatrix(matRot*matWorld);
	m_pTransformCom->Move_Pos(&vDir, 10.f, fTimeDelta);
	Effect_Thrust(fTimeDelta, Boss);
	if (m_fChaseTime >= 7.f)
	{
		Engine::StopSound(CSoundMgr::CHANNELID::Space_Missile_Move);
		Engine::StopSound(CSoundMgr::CHANNELID::Space_Missile_Move);
		m_bChaseStart = true;
		m_bChaseMove = false;
		m_bDownMove = true;
		m_pTransformCom->Set_Angle(_vec3(0.f, atan2f(vDir.x, vDir.z), 0.f));
	}

}


void CBaboonMissile::Down_Move(const _float & fTimeDelta)
{
	if (!m_bDownMove)
		return;

	_vec3 vPos;
	vPos = m_pTransformCom->Get_Position();

	m_pTransformCom->Move_Pos(&_vec3(0.f, -1.f, 0.f), m_fSpeed, fTimeDelta);
	//if (vPos.y <= 2.f)
	//{
	//	m_pTrigger2->Set_Interactable(true);
	//	m_pTransformCom->Set_Pos(vPos.x, 2.f, vPos.z);
	//	m_bDownMove = false;
	//	//m_bAtt = true;
	//	m_bPlayerAtt = true;
	//	m_bChange = true;
	//}
	if (vPos.y <= 51.f)
	{
		m_pTransformCom->Set_Pos(vPos.x, 51.f, vPos.z);
		m_pTrigger2->Set_Interactable(true);
		m_bDownMove = false;
		m_bPlayerAtt = true;
		m_bChange = true;
	}
}

void CBaboonMissile::Att_Move(const _float & fTimeDelta)
{
	if (!m_bAtt)
		return;



	//이때부터 플레이어가 탈 수 있음.
	if (m_bAtt)//플레이어 타는게 ture면 움직이게 함. 플레이어에서 bool값 하나 만들어서 넘겨야함. 테스트용으로넣어둔거임.
	{

		if (m_bChaseStart)
		{
			PlaySoundW(L"Rocket_MoveStart.wav", CSoundMgr::CHANNELID::Space_Missile_Start, 0.3f);
			m_bChaseStart = false;
		}
		if (Engine::Is_Play_End(CSoundMgr::CHANNELID::Space_Missile_Start))
		{
			PlaySoundW(L"Rocket_Moving.wav", CSoundMgr::CHANNELID::Space_Missile_Move);
		}

		_vec3 vLook;
		_vec3 vUp = { 0.f, 1.f, 0.f };
		_vec3 vDown = { 0.f, -1.f, 0.f };
		m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
		D3DXVec3Normalize(&vLook, &vLook);

		_float fAngle;

			m_fSpeed = 7.f;
		if (!m_bStartMissile) //처음 돌진.
		{
			_vec3 vDir = vLook + vUp;

			m_pTransformCom->Move_Pos(&vDir, 10.f, fTimeDelta);

			m_bStartMissile = true;

		}
		else if (m_bStartMissile)
		{
			_matrix matRot, matWorld;
			matWorld = *m_pTransformCom->Get_WorldMatrix();

			if (m_bTarget)
			{
				Effect_Thrust(fTimeDelta, Cody);

				if (Key_Pressing(DIK_A))
				{

					m_pTransformCom->Set_AddAngleY(D3DXToRadian(5.f));
				}

				if (Key_Pressing(DIK_D))
				{

					m_pTransformCom->Set_AddAngleY(D3DXToRadian(-5.f));

				}

				if (Key_Pressing(DIK_W))
				{
					fAngle = D3DXToDegree(acosf(D3DXVec3Dot(&vDown, &vLook)));
					if (fAngle > 3.f)
						m_pTransformCom->Set_AddAngleX(D3DXToRadian(5.f));
				}




				if (Key_Pressing(DIK_S))
				{
					fAngle = D3DXToDegree(acosf(D3DXVec3Dot(&vUp, &vLook)));
					if (fAngle > 3.f)
						m_pTransformCom->Set_AddAngleX(D3DXToRadian(-5.f));
				}

			}
			else if (!m_bTarget)
			{
				Effect_Thrust(fTimeDelta, May);

				_float fDIC_Y = 0;
				_float fCheck = 0.0f;
				fDIC_Y = Engine::Get_DIPadMove(PADMOVESTATE::DIC_LS_Y);
				if (fabsf(fDIC_Y) <= 0.2f) {}
				else
				{
					if (fDIC_Y > 0)
					{
						fAngle = D3DXToDegree(acosf(D3DXVec3Dot(&vDown, &vLook)));
						if (fAngle > 3.f)
							m_pTransformCom->Set_AddAngleX(D3DXToRadian(5.f));
					}
					else if (fDIC_Y < 0)
					{
						fAngle = D3DXToDegree(acosf(D3DXVec3Dot(&vUp, &vLook)));
						if (fAngle > 3.f)
							m_pTransformCom->Set_AddAngleX(D3DXToRadian(-5.f));
					}

				}

				_float fDIC_X = 0;
				fDIC_X = Engine::Get_DIPadMove(PADMOVESTATE::DIC_LS_X);
				if (fabs(fDIC_X) <= 0.2f) {}
				else
				{
					if (fDIC_X > 0)
						m_pTransformCom->Set_AddAngleY(D3DXToRadian(5.f));
					else if (fDIC_X < 0)
						m_pTransformCom->Set_AddAngleY(D3DXToRadian(-5.f));
				}
			}

			m_pTransformCom->Move_Pos(&vLook, 10.f, fTimeDelta);
		}
	}
}

CBaboonMissile * CBaboonMissile::Create(LPDIRECT3DDEVICE9 pGraphicDev, void * pArg, CGameObject* pPlayer, _bool bTarget, _uint iIndex)
{

	CBaboonMissile*	pInstance = new CBaboonMissile(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pArg, pPlayer, bTarget)))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBaboonMissile::Free(void)
{
	m_pTarget->Release();
	CPlayer::Free();
}
