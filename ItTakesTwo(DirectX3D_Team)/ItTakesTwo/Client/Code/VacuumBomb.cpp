#include "stdafx.h"
#include "VacuumBomb.h"
#include "CollisionMgr.h"
#include "Export_Function.h"
#include "MainCamera.h"
#include "CodyCamera.h"
#include "MayCamera.h"

CVacuumBomb::CVacuumBomb(LPDIRECT3DDEVICE9 pGraphicDev)
	:CPlayer(pGraphicDev)
	, m_bTram(false), m_bTramStart(true), m_fTramPower(0.f)
	, m_fTramTime(0.f), m_fTramHeight(0.f), m_fDuration(2.f)
	, m_bCol(false), m_fColSize(0.f)
{
}

CVacuumBomb::~CVacuumBomb()
{
}

void CVacuumBomb::Set_Shoot(_vec3 vTargetPos1, _float fPower, _float fDuration)
{

	m_vShootDir = vTargetPos1*fPower;
	m_fShootDuration = fDuration;
	m_fShootTime = 0.f;
	m_bCol = true;
	m_bShoot = true;
}

void CVacuumBomb::Shoot(const _float& fTimeDelta)
{
	if (!m_bShoot)
		return;

	if (m_fShootTime > m_fShootDuration)
	{
		//m_bRideGround = true;
		m_bShoot = false;
	}

	_vec3 vMovePos = m_vShootDir*fTimeDelta / m_fShootDuration;
	PxU32 flags = m_pController->move(PxVec3(vMovePos.x, vMovePos.y, vMovePos.z), 0, fTimeDelta, m_ControllFilter);
	m_pController->move(PxVec3(0.f, -3.8f, 0.f)*m_fShootTime*m_fShootTime, 0, fTimeDelta, m_ControllFilter);

	m_fShootTime += fTimeDelta;
}

HRESULT CVacuumBomb::Ready_Object(_vec3 vPos, _vec3 vTargetPos)
{
	ZeroMemory(&m_tColInfo, sizeof(SPHERE_INFO));
	m_tColInfo.fRadius = 0.7f;


	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Set_Scale(_vec3(0.01f, 0.01f, 0.01f));
	m_pTransformCom->Set_Pos(vPos);
	m_vTargetPos = vTargetPos;

	return S_OK;
}

_int CVacuumBomb::Update_Object(const _float & fTimeDelta)
{
	if (!m_bActive)
		return OBJ_NOEVENT;
	if (m_bStart)
	{

		auto* pController = this->CreateController(Engine::Get_Scene()->Get_PxControllMgr());
		//CurScene->Get_PxScene()->addActor(*pController->getActor());
		PxShape* pShape;
		pController->getActor()->getShapes(&pShape, 1);
		pShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true);
		pShape->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, true);

		Engine::Get_Scene()->setupFiltering(Engine::Get_Allocator(), Get_RigidActor(), FilterGroup::eInteractItem, FilterGroup::eGround);
		m_bStart = false;
	}

	m_fLifeTime += fTimeDelta;

	if (m_bDead)
	{
		// DEAD EFFECT
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

		m_vecSlidePos.clear();
		m_pController->setFootPosition(PxExtendedVec3(0.f, -10000.f, 0.f));
		m_bActive = false;
		PlaySoundOnce(L"Rocket_Dead.wav", CSoundMgr::CHANNELID::Space_Missile_Dead);

		return OBJ_DEAD;
	}

	// IDLE EFFECT
	m_fEffectTime += fTimeDelta;

	if (m_fEffectTime > 0.05f)
	{
		PARTICLE tParticle;
		Engine::Reset_Particle(tParticle);
		Engine::Set_AnimState(tParticle, 8, 8, 1.f);

		tParticle.fAcc = 1.f;
		tParticle.fCurrentTime = 0.f;
		tParticle.vStartSize = _vec3(4.f, 4.f, 4.f);
		tParticle.vEndSize = _vec3(4.f, 4.f, 4.f);
		tParticle.fRotateSpeed = 0.f;
		tParticle.vStartColor = { 100.f , 100.f, 100.f, 255.f };
		tParticle.vEndColor = { 100.f, 100.f, 100.f , 255.f };
		tParticle.vRot = { 0.f, 0.f, 0.f };
		tParticle.fVelocity = 0.f;
		tParticle.fLifeTime = 0.3f;
		tParticle.vRot = { 0.f, 0.f, 0.f };

		tParticle.vDir = { 0.f, 0.f, 0.f };
		tParticle.vPos = m_pTransformCom->Get_Position() + _vec3(CRandoms()(-3, 3) * 0.1f, 1.f + CRandoms()(-3, 3) * 0.1f, CRandoms()(-3, 3) * 0.1f);
		Engine::Emit_ImageParticle(tParticle, 23, 4);

		tParticle.fLifeTime = 0.7f;
		tParticle.vStartSize = _vec3(2.f, 2.f, 2.f);
		tParticle.vEndSize = _vec3(2.f, 2.f, 2.f);
		tParticle.vStartColor = { 255.f * 4.f , 110.f * 4.f, 90.f * 4.f, 255.f * 4.f };
		tParticle.vEndColor = { 255.f * 4.f, 110.f * 4.f, 90.f * 4.f, 255.f };
		tParticle.vPos = m_pTransformCom->Get_Position() + _vec3(CRandoms()(-5, 5) * 0.1f, 1.f + CRandoms()(-5, 5) * 0.1f, CRandoms()(-5, 5) * 0.1f);
		Engine::Emit_ImageParticle(tParticle, 24, 4);

		m_fEffectTime = 0.f;
	}



	if (m_fLifeTime > 23.f)
	{

		PARTICLE tParticle;
		Engine::Reset_Particle(tParticle);
		Engine::Set_AnimState(tParticle, 7, 7, 1.5f);

		tParticle.fAcc = 1.f;
		tParticle.fCurrentTime = 0.f;
		tParticle.vStartSize = _vec3(4.f, 4.f, 4.f);
		tParticle.vEndSize = _vec3(4.f, 4.f, 4.f);
		tParticle.fRotateSpeed = 0.f;
		tParticle.vStartColor = { 255.f * 2.5f, 255.f* 2.5f, 255.f* 2.5f, 255.f };
		tParticle.vEndColor = { 255.f* 2.5f, 255.f* 2.5f, 255.f* 2.5f , 255.f };
		tParticle.vRot = { 0.f, 0.f, 0.f };
		tParticle.fVelocity = 0.f;
		tParticle.fLifeTime = 0.6f;
		tParticle.vRot = { 0.f, 0.f, 0.f };

		tParticle.vDir = { 0.f, 0.f, 0.f };
		tParticle.vPos = m_pTransformCom->Get_Position() + _vec3(0.f, 1.f, 0.f);
		Engine::Emit_ImageParticle(tParticle, 11, 4);


		m_bDead = true;
	}
	Slide_State(fTimeDelta);
	Move(fTimeDelta, m_vTargetPos);


	Engine::CGameObject::Update_Object(fTimeDelta);

	if (!m_bSlide)
		Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);

	if (m_bCol)
		CCollisionMgr::GetInstance()->Add_PlayerBulletList(this);

	_vec3 vPos = m_pTransformCom->Get_Position();

	return OBJ_NOEVENT;
}

_int CVacuumBomb::LateUpdate_Object(const _float & fTimeDelta)
{
	if (!m_bActive)
		return OBJ_NOEVENT;
	_vec3 vPos;
	if (!m_pController)
		return OBJ_NOEVENT;
	PxTransform Trnasform = m_pController->getActor()->getGlobalPose();
	vPos.x = Trnasform.p.x;
	vPos.y = Trnasform.p.y - 0.8f;
	vPos.z = Trnasform.p.z;
	if (m_bPhys)
		m_pTransformCom->Set_Pos(vPos);
	return OBJ_NOEVENT;
}

void CVacuumBomb::Render_Object(const _int & iIndex)
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

	m_tColInfo.matWorld = *m_pTransformCom->Get_WorldMatrix();
	m_pColliderCom->Render_Collider(m_tColInfo.matWorld, false);
}

HRESULT CVacuumBomb::Add_Component()
{
	Engine::CComponent*		pComponent = nullptr;

	// CStaticMesh
	pComponent = m_pMeshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(RESOURCE_VB, L"VacuumBomb"));
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


	_vec3 vPos = m_pTransformCom->Get_Position();
	Create_Actor(Engine::Get_Physics(), 1.f, 1.f, this, this, vPos);
	//auto* pPhys = Engine::Get_Physics();
	//PxQuat cQuat = PxQuat(m_pTransformCom->Get_Angle().x, { 1.f,0.f,0.f })*PxQuat(m_pTransformCom->Get_Angle().y, { 0.f,1.f,0.f })*PxQuat(m_pTransformCom->Get_Angle().z, { 0.f,0.f,1.f });
	//PxRigidDynamic* pBody = pPhys->createRigidDynamic(PxTransform(vPos.x,vPos.y,vPos.z));
	//m_pRigidActor = pBody;
	////pBody->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
	//PxTriangleMeshGeometry tGeo{};
	//tGeo.triangleMesh = m_pMeshCom->Get_PxMesh();
	//tGeo.scale = PxMeshScale(0.01f);

	//PxShape* pShape = pPhys->createShape(tGeo, *pPhys->createMaterial(0.5f, 0.5f, 0.f), true);
	//pShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true);
	//pShape->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, true);
	//m_pRigidBody = pShape;
	//m_pRigidActor->attachShape(*pShape);

	return S_OK;
}

HRESULT CVacuumBomb::SetUp_ConstantTable(LPD3DXEFFECT & pEffect, const _int & iIndex)
{
	_matrix			matWorld, matView, matProj;

	m_pTransformCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	pEffect->SetVector("g_vColor", &_vec4(255.f, 255.f, 255.f, 255.f));
	pEffect->SetFloat("g_fAmount", 0.f);
	pEffect->SetFloat("g_fMaterialIndex", 1.f);
	pEffect->CommitChanges();

	return S_OK;
}

void CVacuumBomb::Move(const _float & fTimeDelta, const _vec3 & TargetPos)
{
	if (m_bMoveEnd || !m_bActive)
		return;
	_float fStartPower = 0.f;

	if (m_bTramStart)
	{
		m_vSetPos = TargetPos;
		m_vTramDir = TargetPos - m_pTransformCom->Get_Position();
		D3DXVec3Normalize(&m_vTramDir, &m_vTramDir);

		m_fTramPower = ((m_vTramDir.y + 1.f) - m_pTransformCom->Get_Position().y - (m_fDuration * m_fDuration) * -GRAVITY) / m_fDuration;

		m_vTramDis = TargetPos - m_pTransformCom->Get_Position();
		m_vTramDis.y = 0.f;
		m_fTramHeight = m_pTransformCom->Get_Position().y;
		m_fTramTime = 0.f;
		m_bTramStart = false;
	}

	m_fTramTime += fTimeDelta;

	_float fHeight = m_fTramHeight + ((m_fTramTime * m_fTramTime) * (-GRAVITY) / 1) + (m_fTramTime * m_fTramPower);
	_vec3 vDstPos = m_pTransformCom->Get_Position() + (m_vTramDir * D3DXVec3Length(&m_vTramDis) * fTimeDelta / m_fDuration);

	m_pTransformCom->Set_Pos(_vec3(vDstPos.x, fHeight, vDstPos.z));
	if (m_pTransformCom->Get_Position().y <= 0.f)
	{
		CRandoms Random;
		_uint	iRandom = Random(1, 4);

		if (iRandom == 1)
		{
			Engine::StopSound(CSoundMgr::CHANNELID::Vacuum_Bomb);
			Engine::PlaySoundW(L"Vacuum_Bomb0.wav", CSoundMgr::CHANNELID::Vacuum_Bomb, 0.3f);
		}
		else if (iRandom == 2)
		{
			Engine::StopSound(CSoundMgr::CHANNELID::Vacuum_Bomb);
			Engine::PlaySoundW(L"Vacuum_Bomb1.wav", CSoundMgr::CHANNELID::Vacuum_Bomb, 0.3f);
		}
		else if (iRandom == 3)
		{
			Engine::StopSound(CSoundMgr::CHANNELID::Vacuum_Bomb);
			Engine::PlaySoundW(L"Vacuum_Bomb2.wav", CSoundMgr::CHANNELID::Vacuum_Bomb, 0.3f);
		}

		else if (iRandom == 4)
		{
			Engine::StopSound(CSoundMgr::CHANNELID::Vacuum_Bomb);
			Engine::PlaySoundW(L"Vacuum_Bomb3.wav", CSoundMgr::CHANNELID::Vacuum_Bomb, 0.3f);
		}
		_vec3 vPosTemp = { m_vSetPos.x + 2.f, 0.f, m_vSetPos.z };
		m_pTransformCom->Set_Pos(vPosTemp);
		m_pController->setFootPosition(PxExtendedVec3(vPosTemp.x, vPosTemp.y, vPosTemp.z));
		m_bOnGround = true;
		m_bMoveEnd = true;
	}
}

void CVacuumBomb::Set_Move(const _vec3& vStartPos, const _vec3& vTargetPos)
{
	m_pTransformCom->Set_Pos(vStartPos);
	m_vTargetPos = vTargetPos;
	m_bActive = true;
	m_bTramStart = true;
	m_bSlideStart = false;
	m_bSlideEnd = false;
	m_bOnGround = false;
	m_bPhys = false;
	m_bMoveEnd = false;
	m_bDead = false;
	m_bTram = false;
	m_bSlide = false;

	m_vecSlidePos.clear();

	m_fLifeTime = 0.f;
	m_fTramTime = 0.f;
}

void CVacuumBomb::Set_Slide(_bool bHit, const _float & fPower, const _float & fShootDuration, vector<vector<_vec3>*> vecActor, const _float& fSpeed)
{

	m_vecSlidePos = vecActor;
	m_bSlideStart = true;
	m_iSlideIndex = 0;
	m_iPathCount = 0;

	m_fShootPower = fPower;
	m_fShootDuration = fShootDuration;
	m_fSlideSpeed = fSpeed;

	m_fLifeTime = 0;
}
void CVacuumBomb::Slide_State(const _float& fTimeDelta)
{
	if (m_bOnGround)
	{
		m_bOnGround = false;
		m_bSlideEnd = false;
		return;
	}
	if (true == m_bSlideEnd)
	{
		Shoot(fTimeDelta);

		return;
	}

	if (m_vecSlidePos.empty())
		return;

	_vec3 vPos = m_pTransformCom->Get_Position();

	if (m_bSlideStart)
	{
		//m_pRigidActor->setActorFlag(PxActorFlag::eDISABLE_SIMULATION, true);
		m_iSlideIndex = 0;
		m_iPathCount = 0;
		m_bSlideStart = false;
		m_iInterval = (m_vecSlidePos[m_iPathCount])->size();
		if (m_iInterval / 20 != 0)
			m_iInterval = size_t(m_iInterval / 20.f);
		else
			m_iInterval = 1;
		m_bPhys = false;
	}
	if (m_iPathCount >= m_vecSlidePos.size() - 1)
	{
		if (m_iSlideIndex >= m_vecSlidePos[m_iPathCount]->size())
		{

			//m_pRigidActor->setActorFlag(PxActorFlag::eDISABLE_SIMULATION, false);
			m_pController->setPosition(PxExtendedVec3(vPos.x, vPos.y, vPos.z));
			_vec3 vTargetPos1 = (*m_vecSlidePos[m_iPathCount])[m_vecSlidePos[m_iPathCount]->size() - 1];
			_vec3 vTargetPos2 = (*m_vecSlidePos[m_iPathCount])[m_vecSlidePos[m_iPathCount]->size() - 1 - m_iInterval];
			D3DXVec3Normalize(&vTargetPos1, &(vTargetPos1 - vTargetPos2));

			Set_Shoot(vTargetPos1, m_fShootPower, m_fShootDuration);
			StopSound(Engine::CSoundMgr::CHANNELID::Vacuum_PreOut);

			m_bPhys = true;
			m_bSlide = false;
			//////////////////////////////////////////////////////////////////////////
			m_vecSlidePos.clear();
			m_bSlideEnd = true;
			return;
		}
		else if (m_iSlideIndex + (m_iInterval * 4) >= m_vecSlidePos[m_iPathCount]->size())
		{
			Engine::StopNPlaySound(L"Play_Gameplay_Vehicles_VacuumHose_BeforeExit_Whoosh.bnk_1.wav", Engine::CSoundMgr::CHANNELID::Vacuum_PreOut);

		}
	}
	else
	{
		if (m_iSlideIndex >= m_vecSlidePos[m_iPathCount]->size())
		{
			++m_iPathCount;
			m_iInterval = m_vecSlidePos[m_iPathCount]->size();
			if (m_iInterval / 20 != 0)
				m_iInterval = size_t(m_iInterval / 20.f);
			else
				m_iInterval = 1;
			m_iSlideIndex = 0;
		}
	}
	if (m_iSlideIndex != 0)
		m_bSlide = true;

	_vec3 vTargetPos = (*m_vecSlidePos[m_iPathCount])[m_iSlideIndex];
	_vec3 vDir = vTargetPos - vPos;

	if (D3DXVec3Length(&(vDir)) > 0.5f*m_fSlideSpeed)
	{
		D3DXVec3Normalize(&vDir, &vDir);
		m_pTransformCom->Move_Pos(&vDir, m_fSlideSpeed * 10, fTimeDelta);
	}
	else
	{
		m_pTransformCom->Set_Pos(vTargetPos);
		m_iSlideIndex += (_int)m_iInterval;
		return;
	}

}

physx::PxControllerBehaviorFlags CVacuumBomb::getBehaviorFlags(const PxShape& shape, const PxActor& actor)
{
	return PxControllerBehaviorFlags();
}

physx::PxControllerBehaviorFlags CVacuumBomb::getBehaviorFlags(const PxController& controller)
{
	return PxControllerBehaviorFlags();
}

physx::PxControllerBehaviorFlags CVacuumBomb::getBehaviorFlags(const PxObstacle& obstacle)
{
	return PxControllerBehaviorFlags();
}

void CVacuumBomb::onShapeHit(const PxControllerShapeHit& hit)
{
	if (hit.shape->getSimulationFilterData().word0 & FilterGroup::eGround)
		m_bOnGround = true;
}

void CVacuumBomb::onControllerHit(const PxControllersHit& hit)
{
}

void CVacuumBomb::onObstacleHit(const PxControllerObstacleHit& hit)
{
}

void CVacuumBomb::Set_ActorSetting(PxRigidDynamic* pActor, PxShape* pBody)
{
	pActor->setRigidDynamicLockFlags(PxRigidDynamicLockFlag::eLOCK_ANGULAR_X | PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y | PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z);

	// 그래비티를 끈다.
	//pActor->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, false);
	m_pController->setStepOffset(0);

	// 질량설정
	pActor->setMass(5);
	pActor->setName((char*)this);
	m_pRigidBody->setName((char*)this);

}

bool CVacuumBomb::filter(const PxController& a, const PxController& b)
{
	return false;
}

physx::PxQueryHitType::Enum CVacuumBomb::preFilter(const PxFilterData& filterData, const PxShape* shape, const PxRigidActor* actor, PxHitFlags& queryFlags)
{
	return PxQueryHitType::eNONE;
}

physx::PxQueryHitType::Enum CVacuumBomb::postFilter(const PxFilterData& filterData, const PxQueryHit& hit)
{
	return PxQueryHitType::eNONE;
}

CVacuumBomb * CVacuumBomb::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, _vec3 vTargetPos)
{
	CVacuumBomb*	pInstance = new CVacuumBomb(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(vPos, vTargetPos)))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVacuumBomb::Free(void)
{

	CPlayer::Free();
}
