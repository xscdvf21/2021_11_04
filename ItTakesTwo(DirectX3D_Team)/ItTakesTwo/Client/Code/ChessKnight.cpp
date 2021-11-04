#include "stdafx.h"
#include "ChessKnight.h"
#include "ChessTile.h"
#include "Export_Function.h"
#include "MainCamera.h"
#include "CollisionMgr.h"

CChessKnight::CChessKnight(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEnermy(pGraphicDev)
{
}

CChessKnight::~CChessKnight()
{
}

HRESULT CChessKnight::Ready_Object(_uint iTileIndex)
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

	m_pTransformCom->Set_Scale(_vec3(0.01f, 0.01f, 0.01f));
	m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(180));
	m_pTransformCom->Set_Pos(tTemp.vPos);

	m_pMeshCom->Set_Animationset((animID::Enemy_PlayRoom_ChessKnight_Summon));
	m_iAniNum = m_pMeshCom->Get_AniIndex();
	PhysicsCreate();

	m_tShader.m_fRimDist = 5.f;
	m_tShader.m_iRimPower = 30;
	m_tShader.m_vRimColor = _vec4(0.15f, 0.15f, 0.15f, 1.f);

	return S_OK;
}

_int CChessKnight::Update_Object(const _float & fTimeDelta)
{
	// HP가 0이하일 경우 실행
	if (m_iHP <= 0)
	{
		Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);
		Dead_State(fTimeDelta);

		m_bDead = true;
	}

	if (m_bDead)
	{
		Dead_Effect2(true);

		m_pRigidActor->setActorFlag(PxActorFlag::eDISABLE_SIMULATION, true);

		Engine::StopSound(CSoundMgr::CHANNELID::ChessKnight);
		Engine::PlaySoundW(L"ChessKnight_Death2.wav", CSoundMgr::CHANNELID::ChessKnight, 0.7f);
		return OBJ_DEAD;
	}
	Engine::CGameObject::Update_Object(fTimeDelta);

	Move(fTimeDelta);

	m_pMeshCom->Play_Animationset(fTimeDelta);

	if (m_bAttAble)
		CCollisionMgr::GetInstance()->Add_EnermyWeaponList(this);

	Tick_Damage(fTimeDelta);
	CCollisionMgr::GetInstance()->Add_EnermyList(this);
	Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);

	return OBJ_NOEVENT;
}

_int CChessKnight::LateUpdate_Object(const _float & fTimeDelta)
{
	m_fEffecTime += fTimeDelta;

	_vec3 vPos = m_pTransformCom->Get_Position();

	PxTransform pxTransform = m_pRigidActor->getGlobalPose();
	pxTransform.p.x = vPos.x;
	pxTransform.p.y = vPos.y + 1.0f;
	pxTransform.p.z = vPos.z;
	m_pRigidActor->setGlobalPose(pxTransform);

	Hit_RimLight(fTimeDelta);

	return _int();
}

void CChessKnight::Render_Object(const _int & iIndex)
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

HRESULT CChessKnight::Add_Component()
{
	Engine::CComponent*		pComponent = nullptr;

	// CDynamicMesh
	pComponent = m_pMeshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Clone_Resource(RESOURCE_CB, L"Chess_Knight"));
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

	return S_OK;
}

HRESULT CChessKnight::SetUp_ConstantTable(LPD3DXEFFECT & pEffect, const _int & iIndex)
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

void CChessKnight::Set_Damage(_int iDamage)
{
	CEnermy::Set_Damage(iDamage);

	Hit_Effect(m_pTransformCom->Get_Position() + _vec3(0.f, 2.f, 0.f), 5.f);
	Hit_Damage(1.5f);
	Hit_Rim();
}

void CChessKnight::Move(const _float & fTimeDelta)
{
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

	m_iAniNum = m_pMeshCom->Get_AniIndex();



	if (m_iAniNum == animID::Enemy_PlayRoom_ChessKnight_Summon && m_pMeshCom->Is_AnimationSetEnd(0.1f))
	{

		m_pMeshCom->Set_Animationset(animID::Enemy_PlayRoom_ChessKnight_AttackMove_var1);
	}
	else if (m_iAniNum == animID::Enemy_PlayRoom_ChessKnight_Summon && m_pMeshCom->Is_AnimationSetEnd(0.5f))
	{
		if (!m_bSound[Ready_Ground])
		{
			Engine::StopSound(CSoundMgr::CHANNELID::ChessKnight);
			Engine::PlaySoundW(L"ChessKnight_Ready1.wav", CSoundMgr::CHANNELID::ChessKnight, 0.3f);
			m_bSound[Ready_Ground] = true;
		}
	}
	else if (m_iAniNum == animID::Enemy_PlayRoom_ChessKnight_Summon && m_pMeshCom->Is_AnimationSetEnd(2.5f))
	{
		if (!m_bSound[Ready_Cry])
		{
			Engine::StopSound(CSoundMgr::CHANNELID::ChessKnight);
			Engine::PlaySoundW(L"ChessKnight_Ready0.wav", CSoundMgr::CHANNELID::ChessKnight, 0.3f);
			m_bSound[Ready_Cry] = true;
		}
	}

	if (m_iAniNum == animID::Enemy_PlayRoom_ChessKnight_AttackMove_var1)
	{
		m_fMoveStart += fTimeDelta;
		if (!m_bSound[Attack_Ready])
		{
			StopSound(CSoundMgr::CHANNELID::ChessKnight);
			PlaySoundW(L"ChessKnight_Attack0.wav", CSoundMgr::CHANNELID::ChessKnight, 0.3f);
			m_bSound[Attack_Ready] = true;
		}

		if (m_fMoveStart >= 2.f) //타이밍은 2초가맞는듯.
		{
			m_pTransformCom->Move_Pos(&vLook, 10.f, fTimeDelta);

			Effect_Walk();



			// 이시점부터 공격가능
			m_bAttAble = true;

			if (!m_bAttStart)
			{
				m_bAttStart = true;
				_float3 vDir;
				m_pTransformCom->Get_Info(INFO_LOOK, &vDir);
				CChessTile::GetInstance()->Set_ChessKnightDir(vDir);
			}
		}
	}

	CHESSTILE tTemp;
	ZeroMemory(&tTemp, sizeof(tTemp));
	tTemp = CChessTile::GetInstance()->Get_TileIndex(0);

	//죽을 곳 설정 맵의 z값보다 작아지면 사라지게 했음.
	if (vPos.z < tTemp.vPos.z)
	{
		m_bDead = true;
	}
}

void CChessKnight::Dead_State(const _float & fTimeDelta)
{
	/*m_pMeshCom->Set_Animationset(knight_);
	m_pMeshCom->Play_Animationset(fTimeDelta);
	if (m_pMeshCom->Is_AnimationSetEnd(0.1f))*/
	m_bDead = true;
}

void CChessKnight::PhysicsCreate()
{
	//PhysX
	auto* pPhysics = Engine::Get_Physics();
	_vec3 vPos = m_pTransformCom->Get_Position();
	PxTransform tTransform(vPos.x, vPos.y, vPos.z);

	PxMaterial* pMaterial = pPhysics->createMaterial(0.5f, 0.5f, 0.0f);

	PxShape* pShape = pPhysics->createShape(PxBoxGeometry(1, 1, 1), *pMaterial, true);
	//밑 두개중 하나는 false 둘다 true안?
	pShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
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

void CChessKnight::Effect_Walk()
{
	if (m_fEffecTime > 0.07f)
	{
		PARTICLE tParticle3;
		Engine::Reset_Particle(tParticle3);
		Engine::Set_AnimState(tParticle3, 8, 8);
		tParticle3.fAcc = 1.f;
		tParticle3.fCurrentTime = 0.f;
		_float fSize = CRandoms()(10, 70) * 0.01f + 0.4f;
		tParticle3.vStartSize = _vec3(fSize, fSize, fSize);
		tParticle3.vEndSize = _vec3(fSize, fSize, fSize);
		tParticle3.fRotateSpeed = 0.f;
		tParticle3.fVelocity = 1.f;
		tParticle3.fLifeTime = 1.5f;
		tParticle3.vDir = { 0.f, 0.f, 0.f };
		tParticle3.vStartColor = { 30.f, 30.f, 30.f, 255.f * 3.5f };
		tParticle3.vEndColor = { 30.f, 30.f, 30.f, 255.f* 3.5f };
		tParticle3.vRot = { 0.f, 0.f, 0.f };

		tParticle3.vPos = m_pTransformCom->Get_Position() + _vec3(CRandoms()(-4, 4) * 0.1f, 0.7f, CRandoms()(-4, 4) * 0.1f);
		Engine::Emit_ImageParticle(tParticle3, 5, 19);

		tParticle3.vStartSize = _vec3(fSize - 0.2f, fSize - 0.2f, fSize - 0.2f);
		tParticle3.vEndSize = _vec3(fSize - 0.2f, fSize - 0.2f, fSize - 0.2f);
		Engine::Emit_ImageParticle(tParticle3, 5, 4);

		m_fEffecTime = 0.f;
	}
}

CChessKnight * CChessKnight::Create(LPDIRECT3DDEVICE9 pGraphicDev, _uint iTileIndex)
{
	CChessKnight*	pInstance = new CChessKnight(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(iTileIndex)))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CChessKnight::Free(void)
{
	CEnermy::Free();
}
