#include "stdafx.h"
#include "Ogre.h"
#include "OgreStartDoor.h"
#include "MainCamera.h"
#include "Export_Function.h"
#include "OgreStone.h"
#include "Knight.h"
#include "Arbalist.h"
#include "MorningStar.h"
#include "CenterHatch.h"
#include "CrossBow.h"
#include "CollisionMgr.h"
COgre::COgre(LPDIRECT3DDEVICE9 pGraphicDev)
	:CEnermy(pGraphicDev)
	, m_fJumpTurnSpeed(D3DXToRadian(7)), m_fJumpStartTime(0.f)
	, m_bJumpStart(true), m_fJumpPower(7.f), m_fJumpTime(0.f), m_fJumpHeight(0.f), m_bJump(false), m_bJumpTurn(false)
	, m_bCloseDoor(false), m_iPlayerCount(0), m_bMayCheck(false), m_bCodyCheck(false)
	, m_bFirstPattern(false), m_iStoneCountMax(0)
	, m_bRunning(false), m_bRunReady(false), m_fRunReadyTime(0.f), m_bRun(false), m_fRunReadyTurnSpeed(D3DXToRadian(2)), m_fRunSpeed(10.f)
	, m_bStun(false), m_bBack(false), m_fStunTime(0.f), m_iStoneCount(0), m_fStoneCreateTime(0.f), m_iKnightCount(0), m_iKnightCountMax(0),m_iStoneCol(0), m_iArbalistCountMax(0), m_iArbalistCount(0), m_fStunDamageRadius(3.f), m_iStunDamage(2)
	, m_bHatchOpen(false), m_bHatchStart(false), m_bHatchHold(false), m_fHactHeight(0.f), m_fHatchTime(0.f), m_bDeadAni(false), m_bHit(false)
{
}

COgre::~COgre()
{
}

HRESULT COgre::Ready_Object(void * pArg)
{

	ZeroMemory(&m_tColInfo, sizeof(SPHERE_INFO));
	m_tColInfo.fRadius = 2.5f;


	FAILED_CHECK_RETURN(Add_Component(pArg), E_FAIL);

	OBJECTINFO tTemp;
	ZeroMemory(&tTemp, sizeof(OBJECTINFO));

	if (nullptr != pArg)
	{
		memcpy(&tTemp, pArg, sizeof(OBJECTINFO));
	}
	
	m_pTransformCom->Set_Scale(_vec3(0.01f, 0.01f, 0.01f));
	m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(215.f));
	//m_pTransformCom->Set_Pos(tTemp.vPos);
	m_fMoveSpeed = tTemp.fSpeed;

	m_pMeshCom->Set_Animationset(animID::ToyOgre_mh);
	
	m_iHP = 100;
	return S_OK;
}

_int COgre::Update_Object(const _float & fTimeDelta)
{

	if (m_iHP <= 0)
	{
		DeathJump(fTimeDelta);
	}
		


	if (m_bDead)
	{
		StopAll();
		PlayBGM(L"ChessField_BGM.wav");

		return OBJ_DEAD;
	}

	//처음에 플레이어가 들어오면 문을 닫는 패턴
	if (!m_bCloseDoor)
	{
		CloseDoor();
		//m_bRecover = true;
		//Recover(fTimeDelta);
	}
	else if (m_bCloseDoor && m_iHP > 0)
	{
		Move(fTimeDelta);
	}
	m_pMeshCom->Play_Animationset(fTimeDelta);
	
	Engine::CGameObject::Update_Object(fTimeDelta);

	if (m_bHit)
	{
		CCollisionMgr::GetInstance()->Add_EnermyList(this);
	}
	Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);

	return OBJ_NOEVENT;
}

_int COgre::LateUpdate_Object(const _float & fTimeDelta)
{
	
	PxTransform pxTransform = m_pRigidActor->getGlobalPose();
	m_pTransformCom->Set_Pos(pxTransform);
	return _int();
}

void COgre::Render_Object(const _int & iIndex)
{
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect, iIndex), );

	_uint	iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);		// 1인자 : 현재 쉐이더 파일이 갖고 있는 pass의 최대 개수, 2인자 : 시작하는 방식(default)
										//pEffect->BeginPass(0);
	m_pMeshCom->Render_Meshes_VTF(pEffect, 0);

	pEffect->EndPass();
	pEffect->End();

	_matrix matWorld;
	m_pTransformCom->Get_WorldMatrix(&matWorld);
	matWorld._41 += 0.f;
	matWorld._42 += 3.f;

	m_tColInfo.matWorld = matWorld;
	if(m_bHit)
		m_pColliderCom->Render_Collider(m_tColInfo.matWorld, false, pEffect);

	Safe_Release(pEffect);


}

void COgre::Set_Damage(_int iDamage)
{
	CEnermy::Set_Damage(iDamage);

	Hit_Effect(m_pTransformCom->Get_Position() + _vec3(0.f, 0.f, 0.f), 2.f);
}

HRESULT COgre::Add_Component(void* pArg)
{
	Engine::CComponent*		pComponent = nullptr;


	OBJINFO tTemp;
	if (pArg)
	{
		memcpy(&tTemp, pArg, sizeof(OBJINFO));
	}

	// CDynamicMesh
	pComponent = m_pMeshCom = static_cast<Engine::CDynamicMesh*>(Engine::Clone_Resource(tTemp.eSceneID, L"ToyOgre"));
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
	pComponent = m_pColliderCom = Engine::CCollider::Create(m_pGraphicDev, m_tColInfo.fRadius, _vec4(255.f, 0.f, 0.f, 0.f));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Collider", pComponent);


	// Shader
	pComponent = m_pShaderCom = static_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_VTF"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);

	m_pTransformCom->Set_Scale(_vec3(0.01f, 0.01f, 0.01f));
	//m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(215.f));
	m_pTransformCom->Set_Pos(_vec3(130.5f, 0.f, -140.7f));

	auto* pPhysics = Engine::Get_Physics();
	_vec3 vPos = m_pTransformCom->Get_Position();

	PxTransform tTransform(vPos.x, vPos.y, vPos.z);

	//정지마찰계수 멈춰있을경우 다른충돌체와 마찰하는정도
	//움직임마찰계수 움직일때 다른충돌체와 마찰정도
	//탄성계수 0~1값
	PxMaterial* pMaterial = pPhysics->createMaterial(0.5f, 0.5f, 0.0f);
	//PxTriangleMeshGeometry pMeshGeo;
	//pMeshGeo.triangleMesh = m_pStaticMeshCom->Get_PxMesh();

	PxShape* pShape = pPhysics->createShape(PxBoxGeometry(1, 1, 1), *pMaterial, true);

	//밑 두개중 하나는 false 둘다 true안?
	pShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true);
	pShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, false);
	pShape->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, true);

	m_pRigidActor = PxCreateDynamic(*pPhysics, tTransform, *pShape, 1);
	

	auto* pBody = static_cast<PxRigidDynamic*>(m_pRigidActor);

	// 트랜스폼은 가지고있지만 움직이지 않는애들
	pBody->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, false);
	// 그래비티를 끈다.
	pBody->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);
	// 질량설정
	pBody->setMass(0);
	//이동 제동력
	pBody->setName("Ogre");
	//PxTransform Transform = pBody->getGlobalPose();

	//PxQuat pxQ = Transform.q;

	//pBody->attachShape(*pShape);

	return S_OK;
}

HRESULT COgre::SetUp_ConstantTable(LPD3DXEFFECT & pEffect, const _int & iIndex)
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
	pEffect->CommitChanges();

	return S_OK;
}

void COgre::Move(const _float & fTimeDelta)
{
	m_iAniNum = m_pMeshCom->Get_AniIndex();
	
	HatchDir();
	if (!m_bHatchStart)
	{
		if (m_bHatchOpen && m_fHatchDir < 0.6f)
		{

			HatchJump(fTimeDelta);
			m_pMeshCom->Set_Animationset(animID::ToyOgre_Hole_Enter);
			return;
		}
		
	}
	else if (m_bHatchStart && m_iHP > 0) //가장 마지막 해치에 걸려있는 상태.
	{
		m_pMeshCom->Set_Animationset(animID::ToyOgre_Hole_mh);
		return;
	}

	//if (!m_bFirstPattern)
	//{
	//	Recover(fTimeDelta);
	//	/*m_bRun = true;*/
	//	m_bFirstPattern = true;
	//}
	Engine::CTransform*		pMayTransformCom = static_cast<Engine::CTransform*>(Engine::Get_Component(L"GameObject", L"May", L"Com_Transform", Engine::ID_DYNAMIC));
	Engine::CTransform*		pCodyTransformCom = static_cast<Engine::CTransform*>(Engine::Get_Component(L"GameObject", L"Cody", L"Com_Transform", Engine::ID_DYNAMIC));

	_vec3 vMayPos; //메이 위치
	_vec3 vCodyPos; //코디 위치
	_vec3 vPos; //현재 몬스터(자신)	위치
	_vec3 vLook; //현재 몬스터(자신)	룩방향
	_vec3 vRight; //현재 몬스터(자신) 라이트
	_vec3 vLeft; //현재 몬스터(자신) 레프트

	m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
	m_pTransformCom->Get_Info(INFO_RIGHT, &vRight);

	D3DXVec3Normalize(&vLook, &vLook);
	D3DXVec3Normalize(&vRight, &vRight);
	vLeft = vRight * -1.f;

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

	if (m_iAniNum == animID::ToyOgre_Dizzy_Recover && m_pMeshCom->Is_AnimationSetEnd(0.1f))
	{	
		m_bBack = false;
		m_bStun = false;
		m_bJumpTurn = false;
		m_bRun = false;
		m_fStunTime = 0.f;
		m_fRunReadyTime = 0.f;
		m_fSoundTime = 0.f;

		if (fMayDir < fCodyDir /*&& m_pTargetPlayer == nullptr*/)
		{
			m_bJumpTurn = true;
			m_pTargetPlayer = static_cast<CGameObject*>(Engine::Get_GameObject(L"GameObject", L"May"));
		}
		else if (fCodyDir < fMayDir /*&& m_pTargetPlayer == nullptr*/)
		{
			m_bJumpTurn = true;
			m_pTargetPlayer = static_cast<CGameObject*>(Engine::Get_GameObject(L"GameObject", L"Cody"));
		}
	}

	if (m_pTargetPlayer != nullptr /*&& m_iAniNum == animID::ToyOgre_Dizzy_Recover && m_pMeshCom->Is_AnimationSetEnd(0.1f)*/)
	{
		JumpTurn(fTimeDelta, m_pTargetPlayer);
		Run(fTimeDelta, m_pTargetPlayer);
	}

}


void COgre::StunSound()
{
	if (!m_bSound[Sound_Stun])
	{
		if (m_iSound[Sound_Stun] == 0)
		{
			StopSound(CSoundMgr::CHANNELID::Ogre);
			PlaySoundW(L"Ogre_Stun0.wav", CSoundMgr::CHANNELID::Ogre, 0.3f);
		}
		else if (m_iSound[Sound_Stun] == 1)
		{
			StopSound(CSoundMgr::CHANNELID::Ogre);
			PlaySoundW(L"Ogre_Stun1.wav", CSoundMgr::CHANNELID::Ogre, 0.3f);
		}
		else if (m_iSound[Sound_Stun] == 2)
		{
			StopSound(CSoundMgr::CHANNELID::Ogre);
			PlaySoundW(L"Ogre_Stun2.wav", CSoundMgr::CHANNELID::Ogre, 0.3f);
		}


		_vec3 vLook, vUp, vDir;
		m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
		m_pTransformCom->Get_Info(INFO_UP, &vUp);
		D3DXVec3Normalize(&vDir, &(vLook + vUp));

		PARTICLE tParticle;
		Engine::Reset_Particle(tParticle);

		_float fRand = 0.f;
		if (CRandoms()(0, 1) == 1)
			fRand = 0.f;
		else
			fRand = 0.5f;
		Engine::Set_AnimState(tParticle, 2, 2, fRand);

		tParticle.fAcc = 1.f;
		tParticle.fCurrentTime = 0.f;
		tParticle.fRotateSpeed = 0.f;

		tParticle.fLifeTime = 3.f;
		tParticle.bGravity = true;
		tParticle.fGravityForce = 2.f;

		D3DXVec3Normalize(&vLook, &vLook);
		tParticle.vPos = m_pTransformCom->Get_Position() + _vec3(0.f, 2.f, 0.f) + (vLook * 2.f);

		for (_int i = 0; i < 15; ++i)
		{
			if (CRandoms()(0, 1) == 1)
				tParticle.fAmount = 0.f;
			else
				tParticle.fAmount = 0.5f;

			tParticle.vRot = { 0.f, D3DXToRadian(CRandoms()(0, 360)), 0.f };
			_float fSize = CRandoms()(1.f, 2.f);
			tParticle.vStartColor = { 255.f, 255.f, 255.f, 255.f };
			tParticle.vEndColor = { 255.f, 255.f, 255.f, 255.f };
			_float fStart = CRandoms()(0.3f, 1.f);
			tParticle.vStartSize = _vec3(fStart, fStart, fStart);
			tParticle.vEndSize = _vec3(fStart, fStart, fStart);
			tParticle.fVelocity = _float(CRandoms()(1, 10) + 3.f);
			_vec3 vAddDir = { CRandoms()(-1.f, 1.f), CRandoms()(0.f, 5.f) , CRandoms()(-1.f, 1.f) };
			D3DXVec3Normalize(&vAddDir, &(vDir + vAddDir));
			vAddDir.x *= -1.f;
			vAddDir.z *= -1.f;
			tParticle.vDir = vAddDir;
			Engine::Emit_ImageParticle(tParticle, 27, 17);
		}

		PARTICLE tParticle2;
		Engine::Reset_Particle(tParticle2);
		tParticle2.fAcc = 1.f;
		tParticle2.fCurrentTime = 0.f;
		tParticle2.vEndSize = _vec3(0.2f, 0.2f, 0.2f);
		tParticle2.fRotateSpeed = 0.f;
		tParticle2.vPos = m_pTransformCom->Get_Position() + _vec3(0.f, 2.f, 0.f) + (vLook * 2.f);
		tParticle2.bGravity = true;
		tParticle2.vStartColor = { 255.f * 2.5f, 255.f * 2.5f, 25.f * 2.5f, 255.f };
		tParticle2.vEndColor = { 255.f * 2.5f, 255.f * 2.5f, 25.f * 2.5f, 255.f };

		for (_int i = 0; i < 17; ++i)
		{
			tParticle2.vRot = { 0.f, 0.f, D3DXToRadian(CRandoms()(0.f, 360.f)) };
			tParticle2.vDir = { CRandoms()(-1.f, 1.f), CRandoms()(0.f, 1.f), CRandoms()(-1.f, 1.f) };
			tParticle2.fVelocity = CRandoms()(8.f, 10.f);
			_float fSize = CRandoms()(0.5f, 0.8f);
			tParticle2.vStartSize = _vec3(fSize, fSize, fSize);
			tParticle2.fLifeTime = CRandoms()(0.5f, 0.8f);;

			tParticle2.fGravityForce = CRandoms()(1.f, 2.f);
			Engine::Emit_ImageParticle(tParticle2, 26, 31);
		}

		PARTICLE tParticle3;
		Engine::Reset_Particle(tParticle3);
		Engine::Set_AnimState(tParticle3, 8, 8, 1.f);

		tParticle3.fAcc = 1.f;
		tParticle3.fCurrentTime = 0.f;
		tParticle3.vStartSize = _vec3(8.f, 8.f, 8.f);
		tParticle3.vEndSize = _vec3(8.f, 8.f, 8.f);
		tParticle3.fRotateSpeed = 0.f;
		tParticle3.vStartColor = { 60.f , 60.f, 60.f, 255.f };
		tParticle3.vEndColor = { 60.f, 60.f, 60.f , 100.f };
		tParticle3.vRot = { 0.f, 0.f, 0.f };
		tParticle3.fVelocity = 0.f;
		tParticle3.fLifeTime = 0.5f;
		tParticle3.vRot = { 0.f, 0.f, 0.f };

		tParticle3.vDir = { 0.f, 0.f, 0.f };
		for (_int i = 0; i < 3; ++i)
		{
			tParticle3.vPos = m_pTransformCom->Get_Position() + _vec3(0.f, 2.f, 0.f) + _vec3(CRandoms()(-1.f, 1.f), CRandoms()(-1.f, 1.f), CRandoms()(-1.f, 1.f)) + (vLook * 2.f);
			Engine::Emit_ImageParticle(tParticle3, 23, 4);
		}

		m_bSound[Sound_Stun] = true;
		m_iSound[Sound_Stun] ++;
	}

	if (m_iSound[Sound_Stun] > 2)
	{
		m_iSound[Sound_Stun] = 0;
	}

	m_bSound[Sound_Stun] = false;
}

void COgre::Run(const _float & fTimeDelta, CGameObject * _pTarGet)
{
	if (false == m_bRun)
		return;
	
	m_fRunReadyTime += fTimeDelta;
	//타겟의 트랜스를 가져옴.
	CTransform*		pTargetTransCom = static_cast<CTransform*>(_pTarGet->Get_Component(L"Com_Transform", Engine::ID_DYNAMIC));

	_vec3 vPos; //현재 몬스터(자신)	위치
	_vec3 vLook; //현재 몬스터(자신)	룩방향
	_vec3 vRight; //현재 몬스터(자신) 라이트
	_vec3 vLeft; //현재 몬스터(자신) 레프트

	_vec3	vTargetPos;
	_vec3	vTargetDir;
	//_float	 fTargetDir;

	vPos = m_pTransformCom->Get_Position();
	m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
	m_pTransformCom->Get_Info(INFO_RIGHT, &vRight);
	D3DXVec3Normalize(&vLook, &vLook);
	D3DXVec3Normalize(&vRight, &vRight);
	vLeft = vRight * -1.f;

	vTargetPos = pTargetTransCom->Get_Position();
	vTargetPos.y = 0.f;

	vTargetDir = vTargetPos - vPos;
	D3DXVec3Normalize(&vTargetDir, &vTargetDir);

	m_fAngle = D3DXToDegree(acosf(D3DXVec3Dot(&vLook, &vTargetDir)));

	_float fRightAngle = D3DXVec3Dot(&vTargetDir, &vRight);
	_float fLeftAngle = D3DXVec3Dot(&vTargetDir, &vLeft);

	/* 정달영씨 세상에 속하지 아니하고 그 무엇에도 미련이 없는 남자 */
	if (m_fRunReadyTime < 2.f)
	{
		m_fSoundTime += fTimeDelta;
		PlaySoundW(L"Ogre_RunVoice.wav", CSoundMgr::CHANNELID::OgreRunVoice, 1.f);
		if (m_fSoundTime > 0.4f)
		{
			StopSound(CSoundMgr::CHANNELID::Ogre);
			PlaySoundW(L"Ogre_Run.wav", CSoundMgr::CHANNELID::Ogre, 0.05f);
			m_fSoundTime = 0.f;
		}
		m_pMeshCom->Set_Animationset(animID::ToyOgre_Telegraph_mh);

		if (m_fAngle >= D3DXToDegree(m_fRunReadyTurnSpeed))
		{
			if (fRightAngle < 0 && fLeftAngle > 0)
			{
				m_pTransformCom->Rotation(ROT_Y, -m_fRunReadyTurnSpeed);
			}
			else if (fRightAngle > 0 && fLeftAngle < 0)
			{
				m_pTransformCom->Rotation(ROT_Y, m_fRunReadyTurnSpeed);
			}
		}
	}
	else
	{
		
		if (!m_bStun)
		{	//달리는 부분.
	
			//PlaySoundW(L"Ogre_Run.wav", CSoundMgr::CHANNELID::Ogre, 0.3f);

			m_pMeshCom->Set_Animationset(animID::ToyOgre_Charge);

			_vec3 vLook;
			m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
			D3DXVec3Normalize(&vLook, &vLook);
			vLook = vLook * m_fRunSpeed;
			PxTransform pxTransform = m_pRigidActor->getGlobalPose();
			pxTransform.p.x += vLook.x * fTimeDelta;
			pxTransform.p.y += vLook.y * fTimeDelta;
			pxTransform.p.z += vLook.z * fTimeDelta;
			m_pRigidActor->setGlobalPose(pxTransform);
		}
		else
		{		
		
			m_bStun = true;
			Stun(fTimeDelta);
		}

	}

}

void COgre::Recover(const _float & fTimeDelta)
{
	if (!m_bRecover)
		return;

	if (!m_bSound[Sound_Recover])
	{
		StopSound(CSoundMgr::CHANNELID::Ogre);
		PlaySoundW(L"Ogre_Recover.wav", CSoundMgr::CHANNELID::Ogre, 1.f);
		m_bSound[Sound_Recover] = true;
	}

	m_pMeshCom->Set_Animationset(animID::ToyOgre_Dizzy_Recover);

	m_bRecover = false;
	m_bSound[Sound_Recover] = false;
}

void COgre::Stun(const _float & fTimeDelta)
{
	if (!m_bStun)
		return;

	if (!m_bBack)
	{
		StunSound();

		CMainCamera*	pCamera = static_cast<CMainCamera*>(Engine::Get_GameObject(L"GameObject", L"MainCamera"));
		pCamera->Set_Shake();

		m_pMeshCom->Set_Animationset(animID::ToyOgre_Charge_Hit);

		_vec3 vLook;
		_vec3 vBack;
		m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
		D3DXVec3Normalize(&vLook, &vLook);
		vBack = vLook * -1.f;
		vBack = vBack * 2.5f;
		PxTransform pxTransform = m_pRigidActor->getGlobalPose();
		pxTransform.p.x += vBack.x;
		pxTransform.p.y += vBack.y;
		pxTransform.p.z += vBack.z;
		m_pRigidActor->setGlobalPose(pxTransform);

		m_iStoneCountMax = 0;
		m_iKnightCountMax = 0;
		m_iArbalistCountMax = 0;

		StunDamage(fTimeDelta);

		m_bBack = true;
	}
	else if (m_bBack)
	{
		m_fStunTime += fTimeDelta;
		m_fStoneCreateTime += fTimeDelta;

		if (m_fStoneCreateTime >= 0.1f && m_iStoneCountMax <= 5)
		{
			CLayer*				pLayer = nullptr;
			pLayer = Engine::Get_Layer(L"GameObject");
			Engine::CGameObject*		pGameObject = nullptr;

			_tchar tagTemp[MAX_PATH];
			wsprintf(tagTemp, L"OgreStone %d", m_iStoneCount);

			_float fX;
			_float fZ;
			_float fAngle;
			_float fScale;
			_float fRotX;
			_float fRotY;
			_float fRotZ;

			CRandoms CRandom;
			fX = CRandom(-8.f, 8.f);
			fZ = CRandom(-8.f, 8.f);
			fScale = CRandom(0.004f, 0.006f);
			fAngle = CRandom(0.f, 360.f);

			fRotX = CRandom(-0.f, 360.f);
			fRotY = CRandom(-0.f, 360.f);
			fRotZ = CRandom(-10.f, 10.f);

			OBJINFO tTemp;
			tTemp.vScale = { fScale, fScale, fScale };
			tTemp.vAngle = { fAngle, fAngle, fAngle };
			tTemp.vPos = { 127.15f + fX, CRandoms()(10.f, 15.f), -146.f + fZ };
			tTemp.eSceneID = RESOURCE_CF;

			pGameObject = COgreStone::Create(m_pGraphicDev, &tTemp, _vec3(fRotX, fRotY, fRotZ));
			NULL_CHECK_RETURN(pGameObject, );
			FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagTemp, pGameObject, Engine::Get_Scene()), );

			m_iStoneCount++;
			m_iStoneCountMax++;
			m_fStoneCreateTime = 0.f;

			//유닛 생성
			if (m_iKnightCountMax < 2)
			{

				_tchar tagTemp[MAX_PATH];
				wsprintf(tagTemp, L"ToyKnight %d", m_iKnightCount);

				_tchar tagWeaponTemp[MAX_PATH];
				wsprintf(tagWeaponTemp, L"ToyKnight_MorningStar %d", m_iKnightCount);

				OBJINFO tTemp;
				tTemp.tParentBone = tagTemp;

				CMorningStar* pMornigStar = nullptr;
				pGameObject = pMornigStar = CMorningStar::Create(m_pGraphicDev, &tTemp);
				NULL_CHECK_RETURN(pGameObject, );
				FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagWeaponTemp, pGameObject), );

				fX = CRandom(-5.f, 5.f);
				fZ = CRandom(-5.f, 5.f);
				fAngle = CRandom(0.f, 360.f);

			
				OBJINFO pObj;
				pObj.vScale = { 0.01f, 0.01f, 0.01f };
				pObj.vAngle = { 0.f, fAngle,0.f };
				pObj.vPos = { 127.15f + fX, 1.f, -146.f + fZ };
				pObj.tParentBone = tTemp.tParentBone;
				pGameObject = CKnight::Create(m_pGraphicDev, pMornigStar, &pObj);
				NULL_CHECK_RETURN(pGameObject, );
				FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObj.tParentBone, pGameObject, Engine::Get_Scene()), );
				Engine::Get_Scene()->setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eDefaultTrigger |FilterGroup::eWall | FilterGroup::eMonster | FilterGroup::eGround);

				CKnight*	pKnight = (CKnight*)pGameObject;
				pKnight->Set_Move(true);

				m_iKnightCount++;
				m_iKnightCountMax++;
			}

			if (m_iArbalistCountMax < 1)
			{
				_tchar tagTemp[MAX_PATH];
				wsprintf(tagTemp, L"ToyArbalist %d", m_iArbalistCount);

				_tchar tagWeaponTemp[MAX_PATH];
				wsprintf(tagWeaponTemp, L"ToyArbalist_CrossBow %d", m_iArbalistCount);

				OBJINFO tTemp;
				tTemp.tParentBone = tagTemp;

				CCrossBow* pCrossBow = nullptr;
				pGameObject = pCrossBow = CCrossBow::Create(m_pGraphicDev, &tTemp);
				NULL_CHECK_RETURN(pGameObject, );
				FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagWeaponTemp, pGameObject), );

				fX = CRandom(-5.f, 5.f);
				fZ = CRandom(-5.f, 5.f);
				fAngle = CRandom(0.f, 360.f);
				OBJINFO pObj;
				pObj.vScale = { 0.01f, 0.01f, 0.01f };
				pObj.vAngle = { 0.f, fAngle,0.f };
				pObj.vPos = { 127.15f + fX, 1.f, -146.f + fZ };
				pObj.tParentBone = tTemp.tParentBone;
				pGameObject = CArbalist::Create(m_pGraphicDev, pCrossBow, &pObj);
				NULL_CHECK_RETURN(pGameObject, );
				FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObj.tParentBone, pGameObject, Engine::Get_Scene()), );
				Engine::Get_Scene()->setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eMonster, FilterGroup::eDefaultTrigger);

				CArbalist*	pArbalist = (CArbalist*)pGameObject;
				pArbalist->Set_Move(true);


				m_iArbalistCount++;
				m_iArbalistCountMax++;
			}

		}
	}
	if (m_iAniNum == animID::ToyOgre_Charge_Hit && m_pMeshCom->Is_AnimationSetEnd(0.1f))
	{
		m_pMeshCom->Set_Animationset(animID::ToyOgre_Dizzy, 1.0f, 0.1f);
	}

	if (m_fStunTime > 4.f)
	{
		if (m_iAniNum == animID::ToyOgre_Dizzy && m_pMeshCom->Is_AnimationSetEnd(0.1f))
		{
			m_bRecover = true;
			Recover(fTimeDelta);
			m_fStunTime = 0.f;
			//m_pMeshCom->Set_Animationset(animID::ToyOgre_Dizzy_Recover);
		}
	}

	//if (m_iAniNum == animID::ToyOgre_Dizzy_Recover && m_pMeshCom->Is_AnimationSetEnd(0.1f))
	//{
	//	m_bBack = false;
	//	m_bStun = false;
	//	//m_bJumpTurn = false;
	//	/*m_bRun = false;*/
	//	m_fStunTime = 0.f;
	//	m_fRunReadyTime = 0.f;
	//}
	



}

void COgre::StunDamage(const _float & fTimeDelta)
{
	
	vector<CPlayer*>*	vecPlayerList;
	vecPlayerList = CCollisionMgr::GetInstance()->Get_PlayerList();

	_vec3 vPos;
	_vec3 vDir;
	_float fDir;

 	vPos = m_pTransformCom->Get_Position();
	for (_uint i = 0; i < vecPlayerList->size(); ++i)
	{
		_vec3 vPlayerPos;
		vPlayerPos = (*vecPlayerList)[i]->Get_Component(L"Com_Transform", ID_DYNAMIC)->Is<CTransform>()->Get_Position();
		vDir = vPos - vPlayerPos;
		fDir = D3DXVec3Length(&vDir);

		if (fDir <= m_fStunDamageRadius)
		{
			(*vecPlayerList)[i]->Set_Damage(-m_iStunDamage);
		}

	}
}

void COgre::JumpTurn(const _float & fTimeDelta, CGameObject * _pTarget)
{
	if (!m_bJumpTurn)
		return;

	
	//타겟의 트랜스를 가져옴.
	CTransform*		pTargetTransCom = static_cast<CTransform*>(_pTarget->Get_Component(L"Com_Transform", Engine::ID_DYNAMIC));

	_vec3 vPos; //현재 몬스터(자신)	위치
	_vec3 vLook; //현재 몬스터(자신)	룩방향
	_vec3 vRight; //현재 몬스터(자신) 라이트
	_vec3 vLeft; //현재 몬스터(자신) 레프트

	_vec3	vTargetPos;
	_vec3	vTargetDir;
	_float	 fTargetDir;

	vPos = m_pTransformCom->Get_Position();
	m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
	m_pTransformCom->Get_Info(INFO_RIGHT, &vRight);
	D3DXVec3Normalize(&vLook, &vLook);
	D3DXVec3Normalize(&vRight, &vRight);
	vLeft = vRight * -1.f;

	vTargetPos = pTargetTransCom->Get_Position();
	vTargetPos.y = 0.f;
	vTargetDir = vTargetPos - vPos;

	fTargetDir = D3DXVec3Length(&vTargetDir);
	vTargetDir.y = 0.f;
	D3DXVec3Normalize(&vTargetDir, &vTargetDir);

	m_fAngle = D3DXToDegree(acosf(D3DXVec3Dot(&vLook, &vTargetDir)));

	_float fRightAngle = D3DXVec3Dot(&vTargetDir, &vRight);
	_float fLeftAngle = D3DXVec3Dot(&vTargetDir, &vLeft);

	if (m_fAngle >= D3DXToDegree(m_fJumpTurnSpeed))
	{
		m_fJumpStartTime += fTimeDelta;
		if (fRightAngle < 0 && fLeftAngle > 0)
		{
			m_pMeshCom->Set_Animationset(animID::ToyOgre_Turn_Left);
			if(m_fJumpStartTime >0.2f)
			{
				Jump(fTimeDelta);
				//m_bJump = true;
				m_pTransformCom->Rotation(ROT_Y, -m_fJumpTurnSpeed);
			}
		}
		else if (fRightAngle > 0 && fLeftAngle < 0)
		{
			m_pMeshCom->Set_Animationset(animID::ToyOgre_Turn_Right);
			if (m_fJumpStartTime > 0.2f)
			{
				Jump(fTimeDelta);
				//m_bJump = true;
				m_pTransformCom->Rotation(ROT_Y, m_fJumpTurnSpeed);
			}
		}
	}
	else
	{
		m_fJumpStartTime = 0.f;
		m_bJumpTurn = false;
		m_bRun = true;
	}



	if (m_iAniNum == animID::ToyOgre_Turn_Left && m_pMeshCom->Is_AnimationSetEnd(0.1f)
		|| m_iAniNum == animID::ToyOgre_Turn_Right && m_pMeshCom->Is_AnimationSetEnd(0.1f))
	{
		m_fJumpStartTime = 0.f;
		m_bJumpTurn = false;
		//m_bJump = false;
		m_bRun = true;
	}
}

void COgre::Jump(const _float & fTimeDelta)
{
	if (!m_bJumpTurn)
		return;

	if (m_bJumpStart)
	{
		m_fJumpHeight = m_pTransformCom->Get_Position().y;
		m_bJumpStart = false;
	}
	
	m_fJumpTime += fTimeDelta;

	_float fHeight = m_fJumpHeight + ((m_fJumpTime * m_fJumpTime) * (-GRAVITY) / 1) + (m_fJumpTime * m_fJumpPower);

	m_pTransformCom->Set_Pos(_vec3(m_pTransformCom->Get_Position().x, fHeight, m_pTransformCom->Get_Position().z));

	if (m_pTransformCom->Get_Position().y <= 0.f)
	{

		m_pTransformCom->Set_Pos(_vec3(m_pTransformCom->Get_Position().x, 0.f, m_pTransformCom->Get_Position().z));
		m_fJumpTime = 0.f;
		m_bJumpStart = true;
		/*m_bRun = true;*/
		//m_bJump = false;
	}
}

void COgre::DeathJump(const _float & fTimeDelta)
{

	m_pMeshCom->Set_Animationset(animID::ToyOgre_Hole_RightHand_Hurt_Death);

	if (m_bDeathJumpStart)
	{
		StopNPlaySound(L"CF_End.ogg", CSoundMgr::CHANNELID::Space2_ClearScene, 0.3f);

		m_fDeathJumpHeight = m_pTransformCom->Get_Position().y;
		m_bDeathJumpStart = false;
	}

	m_fDeathJumpTime += fTimeDelta;

	_float fHeight = m_fDeathJumpHeight + ((m_fDeathJumpTime * m_fDeathJumpTime) * (-GRAVITY) / 1) + (m_fDeathJumpTime * m_fDeathJumpPower);


	m_pTransformCom->Set_Pos(_vec3(m_pTransformCom->Get_Position().x, fHeight, m_pTransformCom->Get_Position().z));


	if (m_pTransformCom->Get_Position().y <= -5.f)
	{
		m_bDead = true;
	}


}

void COgre::HatchDir()
{

	CTransform*		pHatchColTrans = (CTransform*)Engine::Get_Component(L"GameObject", L"HatchCol", L"Com_Transform", COMPONENTID::ID_DYNAMIC);

	_vec3 vHatchPos;
	_vec3 vPos;
	_vec3 vDir;
	vHatchPos = pHatchColTrans->Get_Position();
	vPos = m_pTransformCom->Get_Position();
	vHatchPos.y = 0.f;
	vPos.y = 0.f;
	vDir = vHatchPos - vPos;

	m_fHatchDir = D3DXVec3Length(&vDir);
	
}

void COgre::HatchJump(const _float& fTimeDelta)
{

	_float temp;
	temp = 7.f;
	if (m_bJumpStart)
	{
		

		m_fJumpHeight = m_pTransformCom->Get_Position().y;
		m_bJumpStart = false;
	}

	m_fHatchTime += fTimeDelta;

	_float fHeight = m_fHactHeight + ((m_fHatchTime * m_fHatchTime) * (-GRAVITY) / 1) + (m_fHatchTime * temp);

	m_pTransformCom->Set_Pos(_vec3(m_pTransformCom->Get_Position().x, fHeight, m_pTransformCom->Get_Position().z));

	if (m_pTransformCom->Get_Position().y <= 0.f)
	{
		StopNPlaySound(L"CF_OgreDrop.ogg", CSoundMgr::CHANNELID::Space_Boss_Bomb, 0.3f);

		_vec3 vLook;
		m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
		D3DXVec3Normalize(&vLook, &vLook);
		vLook = vLook * m_fRunSpeed;
		PxTransform pxTransform = m_pRigidActor->getGlobalPose();
		pxTransform.p.y = -3.f;
		m_pRigidActor->setGlobalPose(pxTransform);


		//m_pTransformCom->Set_Pos(_vec3(m_pTransformCom->Get_Position().x, -8.f, m_pTransformCom->Get_Position().z));
		m_fHatchTime = 0.f;
		m_bJumpStart = true;
		m_bHatchStart = true;
		m_bHit = true;
		/*m_bRun = true;*/
		//m_bJump = false;
	}
}

COgre * COgre::Create(LPDIRECT3DDEVICE9 pGraphicDev, void * pArg)
{
	COgre*	pInstance = new COgre(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pArg)))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void COgre::Free(void)
{
	CEnermy::Free();
}

void COgre::CloseDoor()
{
	Engine::CTransform*		pMayTransformCom = static_cast<Engine::CTransform*>(Engine::Get_Component(L"GameObject", L"May", L"Com_Transform", Engine::ID_DYNAMIC));
	Engine::CTransform*		pCodyTransformCom = static_cast<Engine::CTransform*>(Engine::Get_Component(L"GameObject", L"Cody", L"Com_Transform", Engine::ID_DYNAMIC));

	_vec3 vMayPos; //메이 위치
	_vec3 vCodyPos; //코디 위치
	_vec3 vPos; //현재 몬스터(자신)	위치
	_vec3 vLook; //현재 몬스터(자신)	룩방향
	_vec3 vRight; //현재 몬스터(자신) 라이트
	_vec3 vLeft; //현재 몬스터(자신) 레프트

	m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
	m_pTransformCom->Get_Info(INFO_RIGHT, &vRight);

	D3DXVec3Normalize(&vLook, &vLook);
	D3DXVec3Normalize(&vRight, &vRight);
	vLeft = vRight * -1.f;

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

	
	if (fCodyDir < 16.f)
	{
		if (!m_bCodyCheck)
		{
			m_iPlayerCount++;
			m_bCodyCheck = true;
		}
	}
	
	if (fMayDir < 16.f)
	{
		if (!m_bMayCheck)
		{
			m_iPlayerCount++;
			m_bMayCheck = true;
		}
	}

	if (m_iPlayerCount == 2)
	{
		COgreStartDoor*		pDoor = static_cast<COgreStartDoor*>(Engine::Get_GameObject(L"GameObject", L"OgreStartDoor"));
		pDoor->Set_MoveStart(true);
		m_bRecover = true;
		Recover(0.f);
		m_bCloseDoor = true;
	}
}
