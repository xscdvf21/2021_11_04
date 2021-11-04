#include "stdafx.h"

#include "BiShopStopBullet.h"
#include "BiShopMoveBullet.h"
#include "ChessTile.h"
#include "Export_Function.h"
#include "ChessBishop.h"
#include "CollisionMgr.h"
#include "MainCamera.h"

CChessBishop::CChessBishop(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEnermy(pGraphicDev)
	, m_iUnitName(0)
{
}

CChessBishop::~CChessBishop()
{
}

HRESULT CChessBishop::Ready_Object(_uint iTileIndex, _uint iCountName)
{
	m_iHP = 15;
	m_fColDis = 1.5f;

	// 콜라이더
	ZeroMemory(&m_tColInfo, sizeof(SPHERE_INFO));
	m_tColInfo.fRadius = m_fColDis;

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	CHESSTILE tTemp;
	ZeroMemory(&tTemp, sizeof(tTemp));
	tTemp = CChessTile::GetInstance()->Get_TileIndex(iTileIndex);

	m_iUnitName = iCountName;

	m_pTransformCom->Set_Scale(_vec3(0.01f, 0.01f, 0.01f));
	m_pTransformCom->Set_Pos(tTemp.vPos);

	m_pMeshCom->Set_Animationset((animID::Enemy_PlayRoom_ChessBishop_Summon));
	m_iAniNum = m_pMeshCom->Get_AniIndex();

	Engine::StopSound(CSoundMgr::CHANNELID::ChessBishop);
	Engine::PlaySoundW(L"ChessBishop_Ready1.wav", CSoundMgr::CHANNELID::ChessBishop, 0.3f);

	PhysicsCreate();

	m_tShader.m_fRimDist = 5.f;
	m_tShader.m_iRimPower = 30;
	m_tShader.m_vRimColor = _vec4(0.15f, 0.15f, 0.15f, 1.f);



	return S_OK;
}

_int CChessBishop::Update_Object(const _float & fTimeDelta)
{
	if (m_bDead)
	{
		Dead_Effect2(false);
		m_pRigidActor->setActorFlag(PxActorFlag::eDISABLE_SIMULATION, true);
		Engine::StopSound(CSoundMgr::CHANNELID::ChessBishop);
		Engine::PlaySoundW(L"ChessBishop_Death0.wav", CSoundMgr::CHANNELID::ChessBishop, 0.7f);
		return OBJ_DEAD;
	}

	if (m_iHP <= 0)
	{
		Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);
		Dead_State(fTimeDelta);
		return OBJ_NOEVENT;

	}

	
	Engine::CGameObject::Update_Object(fTimeDelta);

	Move(fTimeDelta);

	m_pMeshCom->Play_Animationset(fTimeDelta);

	Tick_Damage(fTimeDelta);
	CCollisionMgr::GetInstance()->Add_EnermyList(this);
	Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);

	return OBJ_NOEVENT;
}

_int CChessBishop::LateUpdate_Object(const _float & fTimeDelta)
{
	Effect_Attack();

	_vec3 vPos = m_pTransformCom->Get_Position();

	PxTransform pxTransform = m_pRigidActor->getGlobalPose();
	pxTransform.p.x = vPos.x;
	pxTransform.p.y = vPos.y + 1.0f;
	pxTransform.p.z = vPos.z;
	m_pRigidActor->setGlobalPose(pxTransform);

	Hit_RimLight(fTimeDelta);

	return _int();
}

void CChessBishop::Render_Object(const _int & iIndex)
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

HRESULT CChessBishop::Add_Component()
{
	Engine::CComponent*		pComponent = nullptr;

	// CDynamicMesh
	pComponent = m_pMeshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Clone_Resource(RESOURCE_CB, L"Chess_Bishop"));
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

HRESULT CChessBishop::SetUp_ConstantTable(LPD3DXEFFECT & pEffect, const _int & iIndex)
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

void CChessBishop::Move(const _float & fTimeDelta)
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


	//애니메이션 순서 7 0 3 4

	if (m_iAniNum == animID::Enemy_PlayRoom_ChessBishop_Summon && m_pMeshCom->Is_AnimationSetEnd(0.1f))
	{
		if (!m_bSound[Attack])
		{
			m_iAttackEffect = 1;
			m_bAttackEffect = true;
			Engine::StopSound(CSoundMgr::CHANNELID::ChessBishop);
			Engine::PlaySoundW(L"ChessBishop_Attack0.wav", CSoundMgr::CHANNELID::ChessBishop, 0.3f);
			m_bSound[Attack] = true;
		}

		m_pMeshCom->Set_Animationset(animID::Enemy_PlayRoom_ChessBishop_Anticipation_mh/*, 1.0F, 0.08F*/);
		m_iAniNum = m_pMeshCom->Get_AniIndex();
	}
	else if (m_iAniNum == animID::Enemy_PlayRoom_ChessBishop_Summon && m_pMeshCom->Is_AnimationSetEnd(0.8f))
	{
		if (!m_bSound[Ready0])
		{
			Engine::StopSound(CSoundMgr::CHANNELID::ChessBishop);
			Engine::PlaySoundW(L"ChessBishop_Ready0.wav", CSoundMgr::CHANNELID::ChessBishop, 0.3f);
			m_bSound[Ready0] = true;
		}
	}

	if (m_iAniNum == animID::Enemy_PlayRoom_ChessBishop_Anticipation_mh && m_pMeshCom->Is_AnimationSetEnd(0.1f))
	{
		m_pMeshCom->Set_Animationset(animID::Enemy_PlayRoom_ChessBishop_Attack_mh/*, 1.0F, 0.08F*/);
		m_iAniNum = m_pMeshCom->Get_AniIndex();
	}

	if (m_iAniNum == animID::Enemy_PlayRoom_ChessBishop_Attack_mh && m_pMeshCom->Is_AnimationSetEnd(0.1f))
	{
		// 불렛발사
		m_iAttackEffect = 2;


		m_pMeshCom->Set_Animationset(animID::Enemy_PlayRoom_ChessBishop_Attack_Start/*, 1.0F, 0.08F*/);
		m_iAniNum = m_pMeshCom->Get_AniIndex();

		CLayer*				pLayer = nullptr;
		pLayer = Engine::Get_Layer(L"GameObject");
		Engine::CGameObject*		pGameObject = nullptr;
		/////////////////////////////45도

		_tchar tagTemp[MAX_PATH];

		wsprintf(tagTemp, L"Bishop_MoveBullet1 %d", m_iUnitName);

		_vec3 vLook1;
		_vec3 vPos1;
		_matrix matRotY;
		D3DXMatrixRotationY(&matRotY, D3DXToRadian(45));
		vPos1 = m_pTransformCom->Get_Position();
		m_pTransformCom->Get_Info(INFO_LOOK, &vLook1);
		D3DXVec3Normalize(&vLook1, &vLook1);
		D3DXVec3TransformNormal(&vLook1, &vLook1, &matRotY);
		pGameObject = CBiShopMoveBullet::Create(m_pGraphicDev, vPos1, vLook1, m_iUnitName, 1);
		NULL_CHECK_RETURN(pGameObject, );
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagTemp, pGameObject), );

		wsprintf(tagTemp, L"Bishop_MoveBullet2 %d", m_iUnitName);

		D3DXMatrixRotationY(&matRotY, D3DXToRadian(135));
		m_pTransformCom->Get_Info(INFO_LOOK, &vLook1);
		D3DXVec3TransformNormal(&vLook1, &vLook1, &matRotY);
		pGameObject = CBiShopMoveBullet::Create(m_pGraphicDev, vPos1, vLook1, m_iUnitName, 2);
		NULL_CHECK_RETURN(pGameObject, );
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagTemp, pGameObject), );


		wsprintf(tagTemp, L"Bishop_MoveBullet3 %d", m_iUnitName);

		D3DXMatrixRotationY(&matRotY, D3DXToRadian(225));
		m_pTransformCom->Get_Info(INFO_LOOK, &vLook1);
		D3DXVec3TransformNormal(&vLook1, &vLook1, &matRotY);
		pGameObject = CBiShopMoveBullet::Create(m_pGraphicDev, vPos1, vLook1, m_iUnitName, 3);
		NULL_CHECK_RETURN(pGameObject, );
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagTemp, pGameObject), );

		wsprintf(tagTemp, L"Bishop_MoveBullet4 %d", m_iUnitName);

		D3DXMatrixRotationY(&matRotY, D3DXToRadian(315));
		m_pTransformCom->Get_Info(INFO_LOOK, &vLook1);
		D3DXVec3TransformNormal(&vLook1, &vLook1, &matRotY);

		pGameObject = CBiShopMoveBullet::Create(m_pGraphicDev, vPos1, vLook1, m_iUnitName, 4);
		NULL_CHECK_RETURN(pGameObject, );
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagTemp, pGameObject), );
	}

	if (m_iAniNum == animID::Enemy_PlayRoom_ChessBishop_Attack_Start && m_pMeshCom->Is_AnimationSetEnd(0.1f))
	{
		m_bDead = true;
	}
}

void CChessBishop::Set_Damage(_int iDamage)
{
	CEnermy::Set_Damage(iDamage);

	Hit_Effect(m_pTransformCom->Get_Position() + _vec3(0.f, 2.f, 0.f), 5.f);
	Hit_Damage(1.5f);
	Hit_Rim();
}

void CChessBishop::Dead_State(const _float & fTimeDelta)
{
	m_bDead = true;
}

void CChessBishop::PhysicsCreate()
{
	//PhysX
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

void CChessBishop::Effect_Attack()
{
	switch (m_iAttackEffect)
	{
	case 0:
		return;

	case 1:
	{
		if (m_bAttackEffect)
		{
			PARTICLE tParticle;
			Engine::Reset_Particle(tParticle);
			Engine::Set_AnimState(tParticle, 8, 8, 1.f);
			tParticle.fAcc = 1.f;
			tParticle.fCurrentTime = 0.f;
			tParticle.vStartSize = _vec3(4.f, 1.5f, 1.5f);
			tParticle.vEndSize = _vec3(4.f, 1.5f, 1.5f);
			tParticle.fRotateSpeed = 0.f;
			tParticle.fVelocity = 1.f;
			tParticle.fLifeTime = 2.f;
			tParticle.vDir = { 0.f, 0.f, 0.f };
			tParticle.vStartColor = { 155.f * 3.5f, 155.f * 3.5f, 255.f * 3.5f, 255.f };
			tParticle.vEndColor = { 155.f * 3.5f, 155.f * 3.5f, 255.f * 3.5f, 255.f };
			tParticle.bBill = false;

			tParticle.vRot = { D3DXToRadian(90.f),D3DXToRadian(45.f), 0.f };
			tParticle.vPos = m_pTransformCom->Get_Position() + _vec3(-1.9f, 0.3f, 1.3f) + _vec3(0.3f, 0.f, -0.3f);
			Engine::Emit_ImageParticle(tParticle, 31, 20);
			Engine::Set_AnimState(tParticle, 8, 8, 1.1f);
			tParticle.vPos = m_pTransformCom->Get_Position() + _vec3(-1.75f, 0.3f, 1.45f) + _vec3(0.15f, 0.f, -0.15f);
			Engine::Emit_ImageParticle(tParticle, 31, 20);
			Engine::Set_AnimState(tParticle, 8, 8, 1.2f);
			tParticle.vPos = m_pTransformCom->Get_Position() + _vec3(-1.6f, 0.3f, 1.6f) + _vec3(0.f, 0.f, 0.f);
			Engine::Emit_ImageParticle(tParticle, 31, 20);
			Engine::Set_AnimState(tParticle, 8, 8, 1.3f);
			tParticle.vPos = m_pTransformCom->Get_Position() + _vec3(-1.45f, 0.3f, 1.75f) + _vec3(0.15f, 0.f, -0.15f);
			Engine::Emit_ImageParticle(tParticle, 31, 20);
			Engine::Set_AnimState(tParticle, 8, 8, 1.4f);
			tParticle.vPos = m_pTransformCom->Get_Position() + _vec3(-1.3f, 0.3f, 1.9f) + _vec3(0.3f, 0.f, -0.3f);
			Engine::Emit_ImageParticle(tParticle, 31, 20);

			tParticle.vRot = { D3DXToRadian(90.f),D3DXToRadian(45.f), 0.f };
			tParticle.vPos = m_pTransformCom->Get_Position() + _vec3(1.9f, 0.3f, -1.3f) + _vec3(-0.3f, 0.f, 0.3f);
			Engine::Emit_ImageParticle(tParticle, 31, 20);
			Engine::Set_AnimState(tParticle, 8, 8, 1.1f);
			tParticle.vPos = m_pTransformCom->Get_Position() + _vec3(1.75f, 0.3f, -1.45f) + _vec3(-0.15f, 0.f, 0.15f);
			Engine::Emit_ImageParticle(tParticle, 31, 20);
			Engine::Set_AnimState(tParticle, 8, 8, 1.2f);
			tParticle.vPos = m_pTransformCom->Get_Position() + _vec3(1.6f, 0.3f, -1.6f) + _vec3(0.f, 0.f, 0.f);
			Engine::Emit_ImageParticle(tParticle, 31, 20);
			Engine::Set_AnimState(tParticle, 8, 8, 1.3f);
			tParticle.vPos = m_pTransformCom->Get_Position() + _vec3(1.45f, 0.3f, -1.75f) + _vec3(-0.15f, 0.f, 0.15f);
			Engine::Emit_ImageParticle(tParticle, 31, 20);
			Engine::Set_AnimState(tParticle, 8, 8, 1.4f);
			tParticle.vPos = m_pTransformCom->Get_Position() + _vec3(1.3f, 0.3f, -1.9f) + _vec3(-0.3f, 0.f, 0.3f);
			Engine::Emit_ImageParticle(tParticle, 31, 20);



			tParticle.vRot = { D3DXToRadian(90.f),D3DXToRadian(-45.f), 0.f };
			tParticle.vPos = m_pTransformCom->Get_Position() + _vec3(-1.3f, 0.3f, -1.9f) + _vec3(0.3f, 0.f, 0.3f);
			Engine::Emit_ImageParticle(tParticle, 31, 20);
			Engine::Set_AnimState(tParticle, 8, 8, 1.1f);
			tParticle.vPos = m_pTransformCom->Get_Position() + _vec3(-1.45f, 0.3f, -1.75f) + _vec3(0.15f, 0.f, 0.15f);
			Engine::Emit_ImageParticle(tParticle, 31, 20);
			Engine::Set_AnimState(tParticle, 8, 8, 1.2f);
			tParticle.vPos = m_pTransformCom->Get_Position() + _vec3(-1.6f, 0.3f, -1.6f) + _vec3(0.f, 0.f, 0.f);
			Engine::Emit_ImageParticle(tParticle, 31, 20);
			Engine::Set_AnimState(tParticle, 8, 8, 1.3f);
			tParticle.vPos = m_pTransformCom->Get_Position() + _vec3(-1.75f, 0.3f, -1.45f) + _vec3(0.15f, 0.f, 0.15f);
			Engine::Emit_ImageParticle(tParticle, 31, 20);
			Engine::Set_AnimState(tParticle, 8, 8, 1.4f);
			tParticle.vPos = m_pTransformCom->Get_Position() + _vec3(-1.9f, 0.3f, -1.3f) + _vec3(0.3f, 0.f, 0.3f);
			Engine::Emit_ImageParticle(tParticle, 31, 20);

			tParticle.vRot = { D3DXToRadian(90.f),D3DXToRadian(-45.f), 0.f };
			tParticle.vPos = m_pTransformCom->Get_Position() + _vec3(1.3f, 0.3f, 1.9f) + _vec3(-0.3f, 0.f, -0.3f);
			Engine::Emit_ImageParticle(tParticle, 31, 20);
			Engine::Set_AnimState(tParticle, 8, 8, 1.1f);
			tParticle.vPos = m_pTransformCom->Get_Position() + _vec3(1.45f, 0.3f, 1.75f) + _vec3(-0.15f, 0.f, -0.15f);
			Engine::Emit_ImageParticle(tParticle, 31, 20);
			Engine::Set_AnimState(tParticle, 8, 8, 1.2f);
			tParticle.vPos = m_pTransformCom->Get_Position() + _vec3(1.6f, 0.3f, 1.6f) + _vec3(0.f, 0.f, 0.f);
			Engine::Emit_ImageParticle(tParticle, 31, 20);
			Engine::Set_AnimState(tParticle, 8, 8, 1.3f);
			tParticle.vPos = m_pTransformCom->Get_Position() + _vec3(1.75f, 0.3f, 1.45f) + _vec3(-0.15f, 0.f, -0.15f);
			Engine::Emit_ImageParticle(tParticle, 31, 20);
			Engine::Set_AnimState(tParticle, 8, 8, 1.4f);
			tParticle.vPos = m_pTransformCom->Get_Position() + _vec3(1.9f, 0.3f, 1.3f) + _vec3(-0.3f, 0.f, -0.3f);
			Engine::Emit_ImageParticle(tParticle, 31, 20);

			m_bAttackEffect = false;
		}

		PARTICLE tParticle2;
		Engine::Reset_Particle(tParticle2);
		Engine::Set_AnimState(tParticle2, 4, 4, 3.f);
		tParticle2.fAcc = 1.f;
		tParticle2.fCurrentTime = 0.f;
		_float fSize = CRandoms()(1, 10) * 0.1f;
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
		tParticle2.vStartColor = { 200.f * 2.5f, 200.f* 2.5f, 255.f* 2.5f, 255.f* 2.5f };
		tParticle2.vEndColor = { 200.f* 2.5f, 200.f* 2.5f, 255.f* 2.5f , 255.f* 2.5f };
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

CChessBishop * CChessBishop::Create(LPDIRECT3DDEVICE9 pGraphicDev, _uint iTileIndex, _uint iCountName)
{
	CChessBishop*	pInstance = new CChessBishop(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(iTileIndex, iCountName)))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CChessBishop::Free(void)
{
	CEnermy::Free();
}

