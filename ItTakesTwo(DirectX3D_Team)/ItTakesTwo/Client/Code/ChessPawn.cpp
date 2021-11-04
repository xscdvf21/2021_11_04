#include "stdafx.h"
#include "ChessPawn.h"
#include "ChessTile.h"
#include "PawnBullet.h"
#include "CollisionMgr.h"
#include "Export_Function.h"
#include "MainCamera.h"

CChessPawn::CChessPawn(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEnermy(pGraphicDev)
	, m_iUnitName(0)
{
}

CChessPawn::~CChessPawn()
{
}

HRESULT CChessPawn::Ready_Object(_uint iTileIndex, _uint iUnitName)
{
	m_iHP = 15;
	m_fColDis = 1.5f;

	// 콜라이더
	ZeroMemory(&m_tColInfo, sizeof(SPHERE_INFO));
	m_tColInfo.fRadius = m_fColDis;

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_iUnitName = iUnitName;
	CHESSTILE tTemp;
	ZeroMemory(&tTemp, sizeof(tTemp));
	tTemp = CChessTile::GetInstance()->Get_TileIndex(iTileIndex);

	m_pTransformCom->Set_Scale(_vec3(0.01f, 0.01f, 0.01f));
	m_pTransformCom->Set_Pos(tTemp.vPos);

	m_pMeshCom->Set_Animationset((animID::Enemy_PlayRoom_ChessPawn_Summon));
	m_iAniNum = m_pMeshCom->Get_AniIndex();

	PhysicsCreate();

	if (!m_bSound[Ready1])
	{
		Engine::StopSound(CSoundMgr::CHANNELID::ChessPawn);
		Engine::PlaySoundW(L"ChessPawn_Ready1.wav", CSoundMgr::CHANNELID::ChessPawn, 0.3f);
		m_bSound[Ready1] = true;
	}

	m_tShader.m_fRimDist = 5.f;
	m_tShader.m_iRimPower = 30;
	m_tShader.m_vRimColor = _vec4(0.15f, 0.15f, 0.15f, 1.f);

	return S_OK;
}

_int CChessPawn::Update_Object(const _float & fTimeDelta)
{
	if (m_bDead)
	{
		Dead_Effect2(false);
		m_pRigidActor->setActorFlag(PxActorFlag::eDISABLE_SIMULATION, true);
		return OBJ_DEAD;
	}
	if (m_iHP <= 0)
	{
		Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);
		Dead_State(fTimeDelta);

		return OBJ_NOEVENT;
	}

	Effect_Attack();
	Engine::CGameObject::Update_Object(fTimeDelta);

	Move(fTimeDelta);

	m_pMeshCom->Play_Animationset(fTimeDelta);

	Tick_Damage(fTimeDelta);
	CCollisionMgr::GetInstance()->Add_EnermyList(this);
	Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);
	return OBJ_NOEVENT;
}

_int CChessPawn::LateUpdate_Object(const _float & fTimeDelta)
{
	_vec3 vPos = m_pTransformCom->Get_Position();

	PxTransform pxTransform = m_pRigidActor->getGlobalPose();
	pxTransform.p.x = vPos.x;
	pxTransform.p.y = vPos.y + 1.0f;
	pxTransform.p.z = vPos.z;
	m_pRigidActor->setGlobalPose(pxTransform);

	Hit_RimLight(fTimeDelta);

	return _int();
}

void CChessPawn::Render_Object(const _int & iIndex)
{
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect, iIndex), );

	_uint	iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);		// 1인자 : 현재 쉐이더 파일이 갖고 있는 pass의 최대 개수, 2인자 : 시작하는 방식(default)
										//pEffect->BeginPass(0);
	m_pMeshCom->Render_Meshes_VTF(pEffect, 5);

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

HRESULT CChessPawn::Add_Component()
{
	Engine::CComponent*		pComponent = nullptr;

	// CDynamicMesh
	pComponent = m_pMeshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Clone_Resource(RESOURCE_CB, L"Chess_Pawn"));
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

	return S_OK;
}

HRESULT CChessPawn::SetUp_ConstantTable(LPD3DXEFFECT & pEffect, const _int & iIndex)
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

	pEffect->SetFloat("g_fRimDist", m_tShader.m_fRimDist);
	pEffect->SetInt("g_iRimPower", m_tShader.m_iRimPower);
	pEffect->SetVector("g_vRimColor", &m_tShader.m_vRimColor);
	pEffect->SetBool("g_bRim", m_bRimLight);

	_matrix matViewInv;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matViewInv);
	D3DXMatrixInverse(&matViewInv, nullptr, &matViewInv);
	pEffect->SetVector("g_vCamPos", &_vec4(matViewInv._41, matViewInv._42, matViewInv._43, 1.f));

	pEffect->CommitChanges();

	return S_OK;
}

void CChessPawn::Move(const _float & fTimeDelta)
{
	m_iAniNum = m_pMeshCom->Get_AniIndex();


	m_fAttackTime += fTimeDelta;
	//필요할지모르겠음.
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

	if (m_iAniNum == animID::Enemy_PlayRoom_ChessPawn_Summon && m_pMeshCom->Is_AnimationSetEnd(0.1f))
	{
		m_pMeshCom->Set_Animationset(animID::Enemy_PlayRoom_ChessBishop_Anticipation_mh_Max, 1.0F, 0.08F);
	}
	else if (m_iAniNum == animID::Enemy_PlayRoom_ChessPawn_Summon && m_pMeshCom->Is_AnimationSetEnd(1.f))
	{
		if (!m_bSound[Ready0])
		{
			Engine::StopSound(CSoundMgr::CHANNELID::ChessPawn);
			Engine::PlaySoundW(L"ChessPawn_Ready0.wav", CSoundMgr::CHANNELID::ChessPawn, 0.3f);
			m_bSound[Ready0] = true;
		}
	}

	//너무 애니메이션이 길다싶으면 비숍꺼 빼고 하면 괜찮을듯.
	if (m_iAniNum == animID::Enemy_PlayRoom_ChessBishop_Anticipation_mh_Max && m_pMeshCom->Is_AnimationSetEnd(0.1f))
	{
		if (!m_bSound[Attack])
		{
			m_iAttackEffect = 1;
			m_bAttackEffect = true;
			Engine::StopSound(CSoundMgr::CHANNELID::ChessPawn);
			Engine::PlaySoundW(L"ChessPawn_Attack0.wav", CSoundMgr::CHANNELID::ChessPawn, 0.3f);
			m_bSound[Attack] = true;
		}

		m_pMeshCom->Set_Animationset(animID::Enemy_PlayRoom_ChessPawn_Anticipation_mh_Max);
		m_iAniNum = m_pMeshCom->Get_AniIndex();

	}

	if (m_iAniNum == animID::Enemy_PlayRoom_ChessPawn_Anticipation_mh_Max && m_pMeshCom->Is_AnimationSetEnd(0.1f))
	{
		m_pMeshCom->Set_Animationset(animID::Enemy_PlayRoom_ChessPawn_Attack_mh);
		m_iAniNum = m_pMeshCom->Get_AniIndex();
	}

	if (m_iAniNum == animID::Enemy_PlayRoom_ChessPawn_Attack_mh && m_pMeshCom->Is_AnimationSetEnd(0.1f))
	{
		m_iAttackEffect = 2;
		m_pMeshCom->Set_Animationset(animID::Enemy_PlayRoom_ChessPawn_Attack_Start);
		m_iAniNum = m_pMeshCom->Get_AniIndex();
	}

	if (m_iAniNum == animID::Enemy_PlayRoom_ChessPawn_Attack_Start && m_pMeshCom->Is_AnimationSetEnd(0.1f))
	{
		m_bDead = true;

		CLayer*				pLayer = nullptr;
		pLayer = Engine::Get_Layer(L"GameObject");
		Engine::CGameObject*		pGameObject = nullptr;
		///////////////////////////정면
		_vec3 vLook;
		_vec3 vPos;


		_tchar tagTemp[MAX_PATH];
		wsprintf(tagTemp, L"ChessPawn_Bullet %d", m_iUnitName);

		vPos = m_pTransformCom->Get_Position();
		m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
		D3DXVec3Normalize(&vLook, &vLook);
		vLook = vLook * 2.8f;
		vPos = vPos + vLook;
		D3DXVec3Normalize(&vLook, &vLook);


		pGameObject = CPawnBullet::Create(m_pGraphicDev, vPos);
		pLayer->Add_GameObject(tagTemp, pGameObject);
		/////////////////////////////45도


		wsprintf(tagTemp, L"ChessPawn_Bullet1 %d", m_iUnitName);
		_vec3 vLook1;
		_vec3 vPos1;
		_matrix matRotY;
		D3DXMatrixRotationY(&matRotY, D3DXToRadian(45));
		vPos1 = m_pTransformCom->Get_Position();
		m_pTransformCom->Get_Info(INFO_LOOK, &vLook1);
		D3DXVec3Normalize(&vLook1, &vLook1);
		D3DXVec3TransformNormal(&vLook1, &vLook1, &matRotY);

		vLook1 = vLook1 * 4.2f;
		vPos1 = vPos1 + vLook1;
		D3DXVec3Normalize(&vLook1, &vLook1);

		pGameObject = CPawnBullet::Create(m_pGraphicDev, vPos1);
		pLayer->Add_GameObject(tagTemp, pGameObject);
		///////////////////////////////-45도.

		wsprintf(tagTemp, L"ChessPawn_Bullet2 %d", m_iUnitName);
		_vec3 vLook2;
		_vec3 vPos2;
		_matrix matRotY2;
		D3DXMatrixRotationY(&matRotY2, D3DXToRadian(90));
		vPos2 = m_pTransformCom->Get_Position();
		m_pTransformCom->Get_Info(INFO_LOOK, &vLook2);
		D3DXVec3Normalize(&vLook2, &vLook2);
		D3DXVec3TransformNormal(&vLook2, &vLook2, &matRotY2);

		vLook2 = vLook2 * 2.8f;
		vPos2 = vPos2 + vLook2;
		D3DXVec3Normalize(&vLook2, &vLook2);

		pGameObject = CPawnBullet::Create(m_pGraphicDev, vPos2);
		pLayer->Add_GameObject(tagTemp, pGameObject);


		wsprintf(tagTemp, L"ChessPawn_Bullet3 %d", m_iUnitName);
		_vec3 vLook3;
		_vec3 vPos3;
		_matrix matRotY3;
		D3DXMatrixRotationY(&matRotY3, D3DXToRadian(135));
		vPos3 = m_pTransformCom->Get_Position();
		m_pTransformCom->Get_Info(INFO_LOOK, &vLook3);
		D3DXVec3Normalize(&vLook3, &vLook3);
		D3DXVec3TransformNormal(&vLook3, &vLook3, &matRotY3);

		vLook3 = vLook3 * 4.2f;
		vPos3 = vPos3 + vLook3;
		D3DXVec3Normalize(&vLook3, &vLook3);

		pGameObject = CPawnBullet::Create(m_pGraphicDev, vPos3);
		pLayer->Add_GameObject(tagTemp, pGameObject);

		wsprintf(tagTemp, L"ChessPawn_Bullet4 %d", m_iUnitName);
		_vec3 vLook4;
		_vec3 vPos4;
		_matrix matRotY4;
		D3DXMatrixRotationY(&matRotY4, D3DXToRadian(180));
		vPos4 = m_pTransformCom->Get_Position();
		m_pTransformCom->Get_Info(INFO_LOOK, &vLook4);
		D3DXVec3Normalize(&vLook4, &vLook4);
		D3DXVec3TransformNormal(&vLook4, &vLook4, &matRotY4);

		vLook4 = vLook4 * 2.8f;
		vPos4 = vPos4 + vLook4;
		D3DXVec3Normalize(&vLook4, &vLook4);

		pGameObject = CPawnBullet::Create(m_pGraphicDev, vPos4);

		pLayer->Add_GameObject(tagTemp, pGameObject);


		wsprintf(tagTemp, L"ChessPawn_Bullet5 %d", m_iUnitName);
		_vec3 vLook5;
		_vec3 vPos5;
		_matrix matRotY5;
		D3DXMatrixRotationY(&matRotY5, D3DXToRadian(225));
		vPos5 = m_pTransformCom->Get_Position();
		m_pTransformCom->Get_Info(INFO_LOOK, &vLook5);
		D3DXVec3Normalize(&vLook5, &vLook5);
		D3DXVec3TransformNormal(&vLook5, &vLook5, &matRotY5);

		vLook5 = vLook5 * 4.2f;
		vPos5 = vPos5 + vLook5;
		D3DXVec3Normalize(&vLook5, &vLook5);

		pGameObject = CPawnBullet::Create(m_pGraphicDev, vPos5);
		pLayer->Add_GameObject(tagTemp, pGameObject);

		wsprintf(tagTemp, L"ChessPawn_Bullet6 %d", m_iUnitName);
		_vec3 vLook6;
		_vec3 vPos6;
		_matrix matRotY6;
		D3DXMatrixRotationY(&matRotY6, D3DXToRadian(270));
		vPos6 = m_pTransformCom->Get_Position();
		m_pTransformCom->Get_Info(INFO_LOOK, &vLook6);
		D3DXVec3Normalize(&vLook6, &vLook6);
		D3DXVec3TransformNormal(&vLook6, &vLook6, &matRotY6);

		vLook6 = vLook6 * 2.8f;
		vPos6 = vPos6 + vLook6;
		D3DXVec3Normalize(&vLook6, &vLook6);

		pGameObject = CPawnBullet::Create(m_pGraphicDev, vPos6);

		pLayer->Add_GameObject(tagTemp, pGameObject);

		wsprintf(tagTemp, L"ChessPawn_Bullet7 %d", m_iUnitName);
		_vec3 vLook7;
		_vec3 vPos7;
		_matrix matRotY7;
		D3DXMatrixRotationY(&matRotY7, D3DXToRadian(315));
		vPos7 = m_pTransformCom->Get_Position();
		m_pTransformCom->Get_Info(INFO_LOOK, &vLook7);
		D3DXVec3Normalize(&vLook7, &vLook7);
		D3DXVec3TransformNormal(&vLook7, &vLook7, &matRotY7);

		vLook7 = vLook7 * 4.2f;
		vPos7 = vPos7 + vLook7;
		D3DXVec3Normalize(&vLook7, &vLook7);

		pGameObject = CPawnBullet::Create(m_pGraphicDev, vPos7);

		pLayer->Add_GameObject(tagTemp, pGameObject);
	}


}

void CChessPawn::Set_Damage(_int iDamage)
{
	CEnermy::Set_Damage(iDamage);

	Hit_Effect(m_pTransformCom->Get_Position() + _vec3(0.f, 1.f, 0.f), 5.f);
	Hit_Damage(1.5f);
	Hit_Rim();
}

void CChessPawn::Dead_State(const _float & fTimeDelta)
{
	m_bDead = true;
}

void CChessPawn::PhysicsCreate()
{
	auto* pPhysics = Engine::Get_Physics();
	_vec3 vPos = m_pTransformCom->Get_Position();
	PxTransform tTransform(vPos.x, vPos.y, vPos.z);

	PxMaterial* pMaterial = pPhysics->createMaterial(0.5f, 0.5f, 0.0f);

	PxShape* pShape = pPhysics->createShape(PxBoxGeometry(1, 1, 1), *pMaterial, true);
	//밑 두개중 하나는 false 둘다 true안?
	pShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true);
	pShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, false);

	pShape->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, true);
	m_pRigidActor = PxCreateDynamic(*pPhysics, tTransform, *pShape, 1);

	auto* pBody = static_cast<PxRigidDynamic*>(m_pRigidActor);

	// 그래비티를 끈다.
	pBody->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, false);

	// 질량설정
	pBody->setRigidDynamicLockFlags(PxRigidDynamicLockFlag::eLOCK_ANGULAR_X | PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y | PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z);
	pBody->setName((char*)this);
	pShape->setName((char*)this);
}

void CChessPawn::Effect_Attack()
{
	switch (m_iAttackEffect)
	{
	case 0:
		return;

	case 1:
	{

		PARTICLE tParticle;
		Engine::Reset_Particle(tParticle);
		tParticle.fAcc = 1.f;
		tParticle.fCurrentTime = 0.f;
		_float fSize = CRandoms()(0.25f, 0.4f);
		tParticle.vStartSize = _vec3(fSize, fSize, fSize);
		tParticle.vEndSize = _vec3(0.01f, 0.01f, 0.01f);
		tParticle.fRotateSpeed = 0.f;
		tParticle.fVelocity = CRandoms()(2.f, 4.f);
		tParticle.fLifeTime = 0.5f;

		_vec3 vRand = _vec3(CRandoms()(-4.f, 4.f), 0.5f, CRandoms()(-4.f, 4.f));
		_vec3 vPos = m_pTransformCom->Get_Position() + vRand;
		_vec3 vDir;
		vDir = m_pTransformCom->Get_Position() - vPos;
		D3DXVec3Normalize(&vDir, &vDir);
		tParticle.vDir = vDir;
		tParticle.vStartColor = { 200.f * 2.5f, 200.f * 2.5f, 255.f * 4.5f, 255.f * 3.5f };
		tParticle.vEndColor = { 200.f * 2.5f, 200.f * 2.5f, 255.f * 4.5f, 255.f * 3.5f };

		tParticle.vPos = vPos;
		Engine::Emit_ImageParticle(tParticle, 15, 11);

		vRand = _vec3(CRandoms()(-4.f, 4.f), 0.5f, CRandoms()(-4.f, 4.f));
		vPos = m_pTransformCom->Get_Position() + vRand;
		vDir = m_pTransformCom->Get_Position() - vPos;
		D3DXVec3Normalize(&vDir, &vDir);
		tParticle.vDir = vDir;
		tParticle.vPos = vPos;
		tParticle.vStartColor = { 0.f, 0.f, 0.f, 255.f };
		tParticle.vEndColor = { 0.f, 0.f, 0.f, 255.f };
		Engine::Emit_ImageParticle(tParticle, 15, 11);



		PARTICLE tParticle2;
		Engine::Reset_Particle(tParticle2);
		Engine::Set_AnimState(tParticle2, 4, 4, 3.f);
		tParticle2.fAcc = 1.f;
		tParticle2.fCurrentTime = 0.f;
		fSize = CRandoms()(1, 10) * 0.1f;
		tParticle2.vStartSize = _vec3(fSize, fSize, fSize);
		tParticle2.vEndSize = _vec3(fSize, fSize, fSize);
		tParticle2.fRotateSpeed = 0.f;
		tParticle2.fVelocity = 1.f;
		tParticle2.fLifeTime = 0.5f;
		tParticle2.vDir = { 0.f, 0.f, 0.f };
		tParticle2.vStartColor = { 200.f * 2.5f, 200.f * 2.5f, 255.f * 4.5f, 255.f * 3.5f };
		tParticle2.vEndColor = { 200.f * 2.5f, 200.f * 2.5f, 255.f * 4.5f, 255.f * 3.5f };

		tParticle2.vRot = { 0.f, D3DXToRadian(_float(CRandoms()(1, 360))), 0.f };
		tParticle2.vPos = m_pTransformCom->Get_Position() + _vec3(CRandoms()(-7, 7) * 0.1f, CRandoms()(5, 35) * 0.1f, CRandoms()(-7, 7) * 0.1f);
		Engine::Emit_ImageParticle(tParticle2, 33, 20);
	}
	return;

	case 2:
	{
		CMainCamera* pCamera = (CMainCamera*)Engine::Get_GameObject(L"GameObject", L"MainCamera");
		pCamera->Set_Shake();

		PARTICLE tParticle;
		Engine::Reset_Particle(tParticle);

		tParticle.fAcc = 1.f;
		tParticle.fCurrentTime = 0.f;
		tParticle.vStartSize = _vec3(0.f, 0.f, 0.f);
		tParticle.vEndSize = _vec3(10.f, 10.f, 10.f);
		tParticle.fRotateSpeed = 0.f;
		tParticle.vStartColor = { 200.f * 2.5f, 200.f* 2.5f, 255.f* 2.5f, 255.f };
		tParticle.vEndColor = { 200.f* 2.5f, 200.f* 2.5f, 255.f* 2.5f , 150.f };
		tParticle.fVelocity = 0.f;
		tParticle.fLifeTime = 0.3f;
		tParticle.bBill = false;
		tParticle.vRot = { D3DXToRadian(90.f), 0.f, 0.f };

		tParticle.vDir = { 0.f, 0.f, 0.f };
		tParticle.vPos = m_pTransformCom->Get_Position() + _vec3(0.f, 0.2f, 0.f);
		Engine::Emit_ImageParticle(tParticle, 35, 18);

		PARTICLE tParticle2;
		Engine::Reset_Particle(tParticle2);

		tParticle2.fAcc = 1.f;
		tParticle2.fCurrentTime = 0.f;
		tParticle2.vStartSize = _vec3(4.f, 4.f, 4.f);
		tParticle2.vEndSize = _vec3(3.f, 3.f, 3.f);
		tParticle2.fRotateSpeed = 0.f;
		tParticle2.vStartColor = { 200.f * 2.5f, 200.f* 2.5f, 255.f* 2.5f, 255.f * 2.5f };
		tParticle2.vEndColor = { 200.f* 2.5f, 200.f* 2.5f, 255.f* 2.5f , 255.f * 2.5f };
		tParticle2.fVelocity = 0.f;
		tParticle2.fLifeTime = 0.1f;
		tParticle2.bBill = false;
		tParticle2.vRot = { 0.f, D3DXToRadian(-45.f), 0.f };

		tParticle2.vDir = { 0.f, 0.f, 0.f };
		tParticle2.vPos = m_pTransformCom->Get_Position() + _vec3(CRandoms()(-3, 3) * 0.1f, 5.f, CRandoms()(-3, 3) * 0.1f);
		Engine::Emit_ImageParticle(tParticle2, 36, 18);

		PARTICLE tParticle3;
		Engine::Reset_Particle(tParticle3);
		Engine::Set_AnimState(tParticle3, 8, 8, 0.7f);
		tParticle3.fAcc = 1.f;
		tParticle3.fCurrentTime = 0.f;
		tParticle3.vStartSize = _vec3(5.f, 5.f, 5.f);
		tParticle3.vEndSize = _vec3(5.f, 5.f, 5.f);
		tParticle3.fRotateSpeed = 0.f;
		tParticle3.vStartColor = { 180.f * 2, 200.f * 2, 255.f * 2.5f, 255.f };
		tParticle3.vEndColor = { 180.f * 2, 200.f * 2, 255.f * 2.5f, 0.f };
		tParticle3.vRot = { 0.f, 0.f, 0.f };
		tParticle3.fVelocity = 0.f;
		tParticle3.fLifeTime = 0.7f;
		tParticle3.vRot = { 0.f, 0.f, 0.f };

		tParticle3.vDir = { 0.f, 0.f, 0.f };
		tParticle3.vPos = (m_pTransformCom->Get_Position() + _vec3(0.f, 1.f, 0.f));
		Engine::Emit_ImageParticle(tParticle3, 23, 4);


		m_iAttackEffect = 0;

	}
	return;
	}
}

CChessPawn * CChessPawn::Create(LPDIRECT3DDEVICE9 pGraphicDev, _uint iTileIndex, _uint iUnitName)
{
	CChessPawn*	pInstance = new CChessPawn(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(iTileIndex, iUnitName)))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CChessPawn::Free(void)
{
	CEnermy::Free();
}
