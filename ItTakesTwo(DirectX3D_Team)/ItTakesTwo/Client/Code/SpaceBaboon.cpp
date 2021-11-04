#include "stdafx.h"
#include "SpaceBaboon.h"
#include "BaboonMissile.h"
#include "BaboonLaser.h"
#include "BaboonBomb.h"
#include "BaboonGround.h"
#include "BaboonHitPoint.h"
#include "WindLaser.h"
#include "LaserMirror.h"
#include "CollisionMgr.h"
#include "Export_Function.h"
#include "MayCamera.h"
#include "CodyCamera.h"
CSpaceBaboon::CSpaceBaboon(LPDIRECT3DDEVICE9 pGraphicDev)
	:CEnermy(pGraphicDev)
	, m_bMove(false)
	, m_fPatturnTime(0.f)
	, m_iPatturn(0)
	, m_iTargetSelect(0)
	, m_fAngle(0.f)
	, m_fTurnSpeed(D3DXToRadian(5.f))
	, m_bLaserStart(false)
	, m_bMoveUpDir(false)
	, m_bGroundOne(false), m_bGroundTwo(false), m_fMoveUpDir(25.f), m_bGroundThree(false), m_fMissileTime(100.f)
	, m_fMaxDistance(50.f)
{
}

CSpaceBaboon::~CSpaceBaboon()
{
}

HRESULT CSpaceBaboon::Ready_Object(void * pArg)
{

	FAILED_CHECK_RETURN(Add_Component(pArg), E_FAIL);

	PhysicsCreate();

	m_pMeshCom->Set_Animationset(animID::FlyingSaucer_Ufo_Laser_Mh);

	m_iHP = 7;
	m_iHPMax = 7;

	//m_bPatturn[Patturn_Missile] = true;
	//m_iHP = 4;
	//m_iHPMax = 4;

	return S_OK;
}

_int CSpaceBaboon::Update_Object(const _float & fTimeDelta)
{
	auto* pBody = m_pRigidActor->is<PxRigidDynamic>();
	PxTransform Transform = pBody->getGlobalPose();
	m_iAniIndex = m_pMeshCom->Get_AniIndex();

	if (m_bDead)
	{
		//한번에 될까요 ?
		if (m_pTargetMay)
		{
			m_pTargetMay->Set_Dead();
		}

		if (m_pTargetCody)
		{
			m_pTargetCody->Set_Dead();
		}

		Engine::CLayer*		pLayer = Engine::Get_Layer(L"GameObject");
		Engine::CGameObject*		pGameObject = nullptr;
		Engine::CGameObject*		pMaySpace = Engine::Get_GameObject(L"GameObject", L"May");
		Engine::CGameObject*		pCodySpace = Engine::Get_GameObject(L"GameObject", L"Cody");

		OBJINFO pObj;
		pObj.vScale = { 0.01f, 0.01f, 0.01f };
		pObj.vAngle = { 0.f, 0.f, 0.f };
		pObj.eSceneID = RESOURCE_SB;
		//pObj.vPos = { 0.f, 1.f, 0.f };


		const D3DXFRAME_DERIVED* pLeftBone = m_pMeshCom->Get_FrameByName("LeftFrontRocketHatch");
		_matrix matLeftBoneMatrix = pLeftBone->CombinedTranformationMatrix;
		_matrix matLeftBaboonWorld = matLeftBoneMatrix * *m_pTransformCom->Get_WorldMatrix();

		pObj.vPos = { matLeftBaboonWorld._41, matLeftBaboonWorld._42, matLeftBaboonWorld._43 };

		pGameObject = CBaboonMissile::Create(m_pGraphicDev, &pObj, pMaySpace, false);
		NULL_CHECK_RETURN(pGameObject, 0);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Target_May_DeadMissile", pGameObject, Engine::Get_Scene()), 0);

		const D3DXFRAME_DERIVED* pRightBone = m_pMeshCom->Get_FrameByName("RightFrontRocketHatch");
		_matrix matRightBoneMatrix = pRightBone->CombinedTranformationMatrix;
		_matrix matRightBaboonWorld = matRightBoneMatrix * *m_pTransformCom->Get_WorldMatrix();

		pObj.vPos = { matRightBaboonWorld._41, matRightBaboonWorld._42, matRightBaboonWorld._43 };

		pGameObject = CBaboonMissile::Create(m_pGraphicDev, &pObj, pCodySpace, true);
		NULL_CHECK_RETURN(pGameObject, 0);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Target_Cody_DeadMissile", pGameObject, Engine::Get_Scene()), 0);

		CBaboonMissile*		pMayMessile = (CBaboonMissile*)Engine::Get_GameObject(L"GameObject", L"Target_May_DeadMissile");
		pMayMessile->Set_BaboonDead();

		CBaboonMissile*		pCodyMessile = (CBaboonMissile*)Engine::Get_GameObject(L"GameObject", L"Target_Cody_DeadMissile");
		pCodyMessile->Set_BaboonDead();


		return OBJ_DEAD;
	}
	m_pMeshCom->Play_Animationset(fTimeDelta);


	Engine::CGameObject::Update_Object(fTimeDelta);

	if (m_iHP > 0)
	{
		Move(fTimeDelta);
	}
	Update_UI(fTimeDelta);
	Effect(fTimeDelta);
	Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);
	Engine::PlaySoundW(L"UFO_Loop.wav", CSoundMgr::CHANNELID::ChessPawn, 0.3f);

	return OBJ_NOEVENT;
}

_int CSpaceBaboon::LateUpdate_Object(const _float & fTimeDelta)
{
	auto* pBody = m_pRigidActor->is<PxRigidDynamic>();

	_matrix matWorld = *m_pTransformCom->Get_WorldMatrix();
	_vec3 vPos = _vec3(matWorld._41, matWorld._42, matWorld._43);
	PxTransform Transform = pBody->getGlobalPose();
	Transform.p = PxVec3(vPos.x, vPos.y, vPos.z);
	pBody->setGlobalPose(Transform);

	return _int();
}

void CSpaceBaboon::Render_Object(const _int & iIndex)
{

	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect, iIndex), );

	_uint	iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);		// 1인자 : 현재 쉐이더 파일이 갖고 있는 pass의 최대 개수, 2인자 : 시작하는 방식(default)
										//pEffect->BeginPass(0);
	m_pMeshCom->Render_Meshes_VTF(pEffect, 0);

	pEffect->End();

	Safe_Release(pEffect);

}

HRESULT CSpaceBaboon::Add_Component(void* pArg)
{
	Engine::CComponent*		pComponent = nullptr;

	// CDynamicMesh
	pComponent = m_pMeshCom = static_cast<Engine::CDynamicMesh*>(Engine::Clone_Resource(RESOURCE_SB, L"SpaceShip"));
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
	pComponent = m_pColliderCom = Engine::CCollider::Create(m_pGraphicDev, m_fColDis, _vec4(50.f, 244.f, 234.f, 0.f));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Collider", pComponent);

	// Shader
	pComponent = m_pShaderCom = static_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_VTF"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);


	OBJINFO tTemp;
	ZeroMemory(&tTemp, sizeof(OBJINFO));

	if (nullptr != pArg)
	{
		memcpy(&tTemp, pArg, sizeof(OBJINFO));
	}


	m_pTransformCom->Set_Scale(tTemp.vScale);
	m_pTransformCom->Rotation(ROT_X, tTemp.vAngle.x);
	m_pTransformCom->Rotation(ROT_Y, tTemp.vAngle.y);
	m_pTransformCom->Rotation(ROT_Z, tTemp.vAngle.z);
	m_pTransformCom->Set_Pos(tTemp.vPos);
	return S_OK;
}

HRESULT CSpaceBaboon::SetUp_ConstantTable(LPD3DXEFFECT & pEffect, const _int & iIndex)
{
	_matrix			matWorld, matView, matProj;

	m_pTransformCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	pEffect->SetVector("g_vColor", &_vec4(255.f, 255.f, 255.f, 255.f));
	pEffect->CommitChanges();

	return S_OK;
}

void CSpaceBaboon::Move(const _float & fTimeDelta)
{
	if (m_bStart)
	{
		CreateLaser(L"SpaceShipLaser");
		Create_UI(L"SB_UI");
		m_bStart = false;
	}

	m_fPatturnTime += fTimeDelta;
	if (Key_Down(DIK_F3))
	{
		m_iHP--;
	}
	//체력이 어느정도 이하로 내려가면, 땅올라감.

	Engine::CGameObject*		pMaySpace = Engine::Get_GameObject(L"GameObject", L"May");
	Engine::CGameObject*		pCodySpace = Engine::Get_GameObject(L"GameObject", L"Cody");


	if (Key_Down(DIK_F4)) //테스트용
	{
		m_iHP--;
		//m_bPatturn[Patturn_BackMove] = true;
		//PatturnMissile(fTimeDelta);

	}

	if (!m_bPatturn[Patturn_HitReaction])
	{
		if (m_iHP == 7)
		{
			PatturnLaser(fTimeDelta, pMaySpace);
			CLaserMirror*  pMirror = (CLaserMirror*)Engine::Get_GameObject(L"GameObject", L"LaserMirror2");
			_bool bTemp;
			bTemp = pMirror->Get_State(Mirror_MovePossible);
			if (!bTemp)
			{
				pMirror->Set_State(Mirror_MovePossible, true);
			}

		}
		else if (m_iHP == 6 && m_bNextPatturn)
		{
			PatturnLaser(fTimeDelta, pCodySpace);
			CLaserMirror*  pMirror = (CLaserMirror*)Engine::Get_GameObject(L"GameObject", L"LaserMirror3");
			_bool bTemp;
			bTemp = pMirror->Get_State(Mirror_MovePossible);
			if (!bTemp)
			{
				pMirror->Set_State(Mirror_MovePossible, true);
			}

		}
		else if (m_iHP == 5 && m_bNextPatturn2)
		{
			PatturnLaser(fTimeDelta, pMaySpace);
			CLaserMirror*  pMirror = (CLaserMirror*)Engine::Get_GameObject(L"GameObject", L"LaserMirror");
			_bool bTemp;
			bTemp = pMirror->Get_State(Mirror_MovePossible);
			if (!bTemp)
			{
				pMirror->Set_State(Mirror_MovePossible, true);
			}
		}
		else if (m_iHP == 4)
		{
			m_bPatturn[Patturn_Missile] = true;
		}
		//else if (m_iHP <= 0)
		//{
		//	m_bDead = true;
		//}

		PatturnMissile(fTimeDelta);
		PatturnBomb(fTimeDelta);
		PatturnMoveUp(fTimeDelta);
		PatturnMove(fTimeDelta);
		PatturnBackMove(fTimeDelta);
	}

	PatturnHitReaction(fTimeDelta);

	//Effect_Laser(fTimeDelta);


}

void CSpaceBaboon::CreateLaser(const _tchar * pLayerTag)
{
	Engine::CGameObject*		pMaySpace = Engine::Get_GameObject(L"GameObject", L"May");
	Engine::CGameObject*		pCodySpace = Engine::Get_GameObject(L"GameObject", L"Cody");


	Engine::CLayer*			pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, );

	// 오브젝트 추가
	Engine::CGameObject*		pGameObject = nullptr;

	OBJINFO tObj;
	ZeroMemory(&tObj, sizeof(OBJINFO));
	tObj.eSceneID = m_tObjInfo.eSceneID;
	tObj.vScale = { 0.5f, 0.005f, 0.005f };

	//-90이 룩방향임.

	_tchar tagTemp[MAX_PATH];
	wsprintf(tagTemp, L"BaboonLaser555");

	pGameObject = CBaboonLaser::Create(m_pGraphicDev, &tObj, pCodySpace);
	m_pLaser = pGameObject->Is<CBaboonLaser>();
	m_pLaser->Set_Index(0);
	NULL_CHECK_RETURN(pGameObject, );
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagTemp, pGameObject), );
	m_pLaser->AddRef();

	Engine::Add_Layer(pLayerTag, pLayer);
}

void CSpaceBaboon::Effect_Laser(const _float & fTimeDelta)
{
	if (!m_pLaserEffect)
		return;

}

void CSpaceBaboon::Set_Damage(_int iDamage)
{
	m_bPatturn[Patturn_HitReaction] = true;
	CEnermy::Set_Damage(iDamage);
	Hit_Effect(m_pTransformCom->Get_Position() + _vec3(0.f, 1.f, 0.f), 10.f);

	if (m_iHP == 6)
		Engine::StopNPlaySound(L"SB_BossScene1.ogg", CSoundMgr::CHANNELID::Crusher, 0.3f);
	else if (m_iHP == 5)
		Engine::StopNPlaySound(L"SB_BossScene3.ogg", CSoundMgr::CHANNELID::Crusher, 0.3f);
	else if (m_iHP == 4)
		Engine::StopNPlaySound(L"SB_Phase2_Start.ogg", CSoundMgr::CHANNELID::Crusher, 0.3f);
	else if (m_iHP == 2)
		Engine::StopNPlaySound(L"SB_BossScene2.ogg", CSoundMgr::CHANNELID::Crusher, 0.3f);

}

void CSpaceBaboon::PhysicsCreate()
{
	//PhysX
	auto* pPhysics = Engine::Get_Physics();
	_vec3 vPos = m_pTransformCom->Get_Position();
	PxTransform tTransform(vPos.x, vPos.y, vPos.z);

	PxMaterial* pMaterial = pPhysics->createMaterial(0.5f, 0.5f, 0.0f);

	PxShape* pShape = pPhysics->createShape(PxBoxGeometry(5, 2, 5), *pMaterial, true);
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

void CSpaceBaboon::TargetLook(const _float& fTimeDelta, CGameObject * pTarget)
{
	_vec3 vTargetPos; //타겟의 포스
	_vec3 vPos; //현재 몬스터(자신)	위치
	_vec3 vLook; //현재 몬스터(자신)	룩방향
	_vec3 vRight; //현재 몬스터(자신) 라이트
	_vec3 vLeft; //현재 몬스터(자신) 레프트
	_vec3 vDir;
	_float fDir;

	CTransform*		pTargetTransform = (CTransform*)pTarget->Get_Component(L"Com_Transform", ID_DYNAMIC);
	vPos = m_pTransformCom->Get_Position();
	vTargetPos = pTargetTransform->Get_Position();
	m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
	m_pTransformCom->Get_Info(INFO_RIGHT, &vRight);
	vLook.y = 0.f;
	vRight.y = 0.f;
	D3DXVec3Normalize(&vLook, &vLook);
	D3DXVec3Normalize(&vRight, &vRight);
	vLeft = vRight * -1.f;

	vDir = vTargetPos - vPos;
	fDir = D3DXVec3Length(&vDir);
	vDir.y = 0.f;
	D3DXVec3Normalize(&vDir, &vDir);

	m_fAngle = D3DXToDegree(acosf(D3DXVec3Dot(&vLook, &vDir)));


	_float fRightAngle = D3DXVec3Dot(&vDir, &vRight);
	_float fLeftAngle = D3DXVec3Dot(&vDir, &vLeft);

	if (m_fAngle >= D3DXToDegree(m_fTurnSpeed))
	{
		if (fRightAngle < 0 && fLeftAngle > 0)
		{
			m_pTransformCom->Rotation(ROT_Y, -m_fTurnSpeed);
		}
		else if (fRightAngle > 0 && fLeftAngle < 0)
			m_pTransformCom->Rotation(ROT_Y, m_fTurnSpeed);
	}

}

void CSpaceBaboon::LaserTargetLook(const _float & fTimeDelta, CGameObject * pTarget)
{
	CTransform*		pTargetTransform = (CTransform*)pTarget->Get_Component(L"Com_Transform", ID_DYNAMIC);

	_vec3 vTargetPos; //타겟의 포스
	_vec3 vPos; //현재 몬스터(자신)	위치
	_vec3 vLook; //현재 몬스터(자신)	룩방향
	_vec3 vRight; //현재 몬스터(자신) 라이트
	_vec3 vLeft; //현재 몬스터(자신) 레프트
	_vec3 vDir;
	_float fDir;

	vPos = m_pTransformCom->Get_Position();
	//vTargetPos = pTargetTransform->Get_Position();
	vTargetPos = m_vLaserStartPos;
	m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
	m_pTransformCom->Get_Info(INFO_RIGHT, &vRight);
	vLook.y = 0.f;
	vRight.y = 0.f;
	D3DXVec3Normalize(&vLook, &vLook);
	D3DXVec3Normalize(&vRight, &vRight);
	vLeft = vRight * -1.f;

	vDir = vTargetPos - vPos;
	fDir = D3DXVec3Length(&vDir);
	vDir.y = 0.f;
	D3DXVec3Normalize(&vDir, &vDir);

	//m_fAngle = atan2f(vDir.x,vDir.z)
	m_fAngle = D3DXToDegree(acosf(D3DXVec3Dot(&vLook, &vDir)));
	_float fRightAngle = D3DXVec3Dot(&vDir, &vRight);
	_float fLeftAngle = D3DXVec3Dot(&vDir, &vLeft);

	if (m_fAngle >= D3DXToDegree(m_fTurnSpeed))
	{
		if (fRightAngle < 0 && fLeftAngle > 0)
		{
			m_pTransformCom->Rotation(ROT_Y, -m_fTurnSpeed);
		}
		else if (fRightAngle > 0 && fLeftAngle < 0)
			m_pTransformCom->Rotation(ROT_Y, m_fTurnSpeed);
	}
}

void CSpaceBaboon::PatturnMissile(const _float & fTimeDelta)
{
	//추격 미사일
	if (!m_bPatturn[Patturn_Missile])
		return;

	Engine::CLayer*		pLayer = Engine::Get_Layer(L"GameObject");
	Engine::CGameObject*		pGameObject = nullptr;
	Engine::CGameObject*		pMaySpace = Engine::Get_GameObject(L"GameObject", L"May");
	Engine::CGameObject*		pCodySpace = Engine::Get_GameObject(L"GameObject", L"Cody");

	OBJINFO pObj;
	pObj.vScale = { 0.01f, 0.01f, 0.01f };
	pObj.vAngle = { 0.f, 0.f, 0.f };
	pObj.eSceneID = RESOURCE_SB;
	//pObj.vPos = { 0.f, 1.f, 0.f };


	if (m_pTargetMay == nullptr)
	{
		m_fMayMissileTime += fTimeDelta;

		if (m_fMayMissileTime > 3.f)
		{

			const D3DXFRAME_DERIVED* pLeftBone = m_pMeshCom->Get_FrameByName("LeftFrontRocketHatch");
			_matrix matLeftBoneMatrix = pLeftBone->CombinedTranformationMatrix;
			_matrix matLeftBaboonWorld = matLeftBoneMatrix * *m_pTransformCom->Get_WorldMatrix();

			pObj.vPos = { matLeftBaboonWorld._41, matLeftBaboonWorld._42, matLeftBaboonWorld._43 };


			pGameObject = CBaboonMissile::Create(m_pGraphicDev, &pObj, pMaySpace, false);
			NULL_CHECK_RETURN(pGameObject, );
			FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Target_May_Missile", pGameObject, Engine::Get_Scene()), );

			m_pTargetMay = pGameObject;

			m_fMayMissileTime = 0.f;
		}

	}

	if (m_pTargetCody == nullptr)
	{

		m_fCodyMissileTime += fTimeDelta;

		if (m_fCodyMissileTime > 3.f)
		{
			const D3DXFRAME_DERIVED* pRightBone = m_pMeshCom->Get_FrameByName("RightFrontRocketHatch");
			_matrix matRightBoneMatrix = pRightBone->CombinedTranformationMatrix;
			_matrix matRightBaboonWorld = matRightBoneMatrix * *m_pTransformCom->Get_WorldMatrix();

			pObj.vPos = { matRightBaboonWorld._41, matRightBaboonWorld._42, matRightBaboonWorld._43 };

			pGameObject = CBaboonMissile::Create(m_pGraphicDev, &pObj, pCodySpace, true);
			NULL_CHECK_RETURN(pGameObject, );
			FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Target_Cody_Missile", pGameObject, Engine::Get_Scene()), );

			m_pTargetCody = pGameObject;
			m_fCodyMissileTime = 0.f;
		}

	}

	// m_bPatturn[Patturn_Missile] = false;
}

void CSpaceBaboon::PatturnLaser(const _float & fTimeDelta, CGameObject* pTarget)
{
	//if (!m_bPatturn[Patturn_Laser])
	//	return;



	m_bPatturn[Patturn_Bomb] = false;

	if (!m_bTest)
	{
		CTransform*		pTargetTransform = (CTransform*)pTarget->Get_Component(L"Com_Transform", ID_DYNAMIC);
		m_pLaser->Set_Active(true);
		m_vLaserStartPos = _vec3(0.f, 0.f, 0.f);
		//m_vLaserStartPos = pTargetTransform->Get_Position() ;

		m_bTest = true;
	}


	PlaySoundW(L"Laser.wav", Engine::CSoundMgr::CHANNELID::Space_Boss_Laser, 0.5f);


	Engine::CLayer*				pLayer = Engine::Get_Layer(L"GameObject");
	Engine::CGameObject*		pGameObject = nullptr;
	Engine::CGameObject*		pMaySpace = Engine::Get_GameObject(L"GameObject", L"May");
	Engine::CGameObject*		pCodySpace = Engine::Get_GameObject(L"GameObject", L"Cody");


	LaserTargetLook(fTimeDelta, pTarget);


	m_pLaser->Set_Active(true);
	const D3DXFRAME_DERIVED* pBone = m_pMeshCom->Get_FrameByName("LaserGunRing1");
	_matrix matBoneMatrix = pBone->CombinedTranformationMatrix;
	_matrix matBaboonWorld = matBoneMatrix * *m_pTransformCom->Get_WorldMatrix();
	_vec3 vBonePos;
	vBonePos = { matBaboonWorld._41, matBaboonWorld._42, matBaboonWorld._43 };


	CTransform* pTargetTransform = pTarget->Get_Component(L"Com_Transform", ID_DYNAMIC)->Is<CTransform>();
	CTransform* pLaserTransform = m_pLaser->Get_Component(L"Com_Transform", ID_DYNAMIC)->Is<CTransform>();

	_mat matWorld, matRot, matTrans, matScale;
	_vec3 vSrcPos, vDir, vDstPos, vLook, vAxis;
	vDstPos = m_vLaserStartPos;
	vSrcPos = vBonePos;


	vDir = vDstPos - vSrcPos;
	D3DXVec3Normalize(&vDir, &vDir);




	_vec3 vAngle = { 0.f, atan2f(vDir.x, vDir.z) + D3DXToRadian(-90.f), atan2f(vDir.y,sqrtf(vDir.x*vDir.x + vDir.z*vDir.z)) };

	pLaserTransform->Set_Angle(vAngle);
	pLaserTransform->Set_Pos(vSrcPos);
	_vec3 vDir2 = (pTargetTransform->Get_Position() + _vec3(0.f, 1.f, 0.f)) - m_vLaserStartPos;
	m_vLaserStartPos += *D3DXVec3Normalize(&vDir2, &vDir2) * 10.f * fTimeDelta;

	const PxU32 bufferSize = 256;				// [in] size of 'hitBuffer'
	PxRaycastHit hitBuffer[bufferSize];			// [out] User provided buffer for results
	PxRaycastBuffer buf(hitBuffer, bufferSize); // [out] Blocking and touching hits stored here
	m_fDistance = 10000.f;
	CGameObject* pPlayer = nullptr;
	CLaserMirror* pMirror = nullptr;
	_int		iIndex = 0, iCurIndex = 0;

	PxQueryFilterData filterData = PxQueryFilterData();
	filterData.data.word0 = FilterGroup::eGround | FilterGroup::eCody | FilterGroup::eMay;

	if (m_pScene->raycast(PxVec3(vSrcPos.x, vSrcPos.y, vSrcPos.z),
		PxVec3(vDir.x, vDir.y, vDir.z),
		m_fMaxDistance,
		buf, PxHitFlag::eMESH_BOTH_SIDES,
		filterData))
	{
		for (PxU32 i = 0; i < buf.nbTouches; ++i)
		{
			if (buf.getTouch(i).shape->getSimulationFilterData().word0 &(FilterGroup::eMonster))
				continue;
			//m_fDistance = buf.getTouch(i).distance;
			if (buf.getTouch(i).shape->getSimulationFilterData().word0 & FilterGroup::eInteractItem)
			{
				pMirror = (CLaserMirror*)buf.getTouch(i).shape->getName();
				iCurIndex = 2;
			}
			if (buf.getTouch(i).shape->getSimulationFilterData().word0 &(FilterGroup::eGround))
			{
				iCurIndex = 3;
			}
			if (buf.getTouch(i).shape->getSimulationFilterData().word0 &(FilterGroup::eCody | FilterGroup::eMay))
			{
				pPlayer = ToObj<CGameObject>(buf.getTouch(i).shape->getName());
				iCurIndex = 1;
			}
			if (m_fDistance > buf.getTouch(i).distance)
			{

				iIndex = iCurIndex;
				m_fDistance = buf.getTouch(i).distance;
				m_vUltHitPos = ToVec3(buf.getTouch(i).position);
			}
		}
	}
	pLaserTransform->Set_Scale({ m_fDistance*0.01f ,0.005f,0.005f });



	switch (iIndex)
	{
	case 1:
	{
		pPlayer->Set_Damage(-1);

	}
	break;
	case 2:
	{
		if (pMirror->Check_Hit())
		{
			StopSound(Engine::CSoundMgr::CHANNELID::Space_Boss_Laser);
			m_pLaser->Set_Active(false);
		}
	}

	break;
	case 3:
	{
		PARTICLE tParticle3;
		Engine::Reset_Particle(tParticle3);

		tParticle3.fAcc = 1.f;
		tParticle3.fCurrentTime = 0.f;
		tParticle3.vStartSize = _vec3(0.3f, 0.3f, 0.3f);
		tParticle3.vEndSize = _vec3(0.3f, 0.3f, 0.3f);
		tParticle3.fRotateSpeed = 0.f;

		tParticle3.vStartColor = { 35.f, 35.f, 35.f, 255.f * 2.5f };
		tParticle3.vEndColor = { 35.f, 35.f, 35.f, 0.f };
		tParticle3.vRot = { 0.f, 0.f, 0.f };
		tParticle3.fLifeTime = 2.f;
		tParticle3.vPos = m_vUltHitPos + _vec3(0.f, 0.1f, 0.f);
		tParticle3.vDir = { 0.f, 0.f, 0.f };
		tParticle3.bBill = false;
		tParticle3.vRot = _vec3(D3DXToRadian(90.f), 0.f, 0.f);

		for (_int i = 0; i < 3; ++i)
		{
			tParticle3.vPos = m_vUltHitPos + _vec3(CRandoms()(-0.1f, 0.1f), 0.1f, CRandoms()(-0.1f, 0.1f));

			Engine::Emit_ImageParticle(tParticle3, 15, 11);
		}

	}
	break;

	default:
		break;
	}
	//_vec3 vDir2 = (vTargetPos /*+_vec3(0.f, 0.7f, 0.f)*/) - m_vLaserStartPos;


}

void CSpaceBaboon::PatturnBomb(const _float & fTimeDelta)
{
	if (!m_bPatturn[Patturn_Bomb])
		return;

	Engine::CLayer*		pLayer = Engine::Get_Layer(L"GameObject");
	Engine::CGameObject*		pGameObject = nullptr;
	Engine::CGameObject*		pMaySpace = Engine::Get_GameObject(L"GameObject", L"May");
	Engine::CGameObject*		pCodySpace = Engine::Get_GameObject(L"GameObject", L"Cody");

	OBJINFO pObj;
	pObj.vScale = { 0.01f, 0.01f, 0.01f };
	pObj.vAngle = { 0.f, 0.f, 0.f };
	pObj.eSceneID = RESOURCE_SB;
	pObj.vPos = m_pTransformCom->Get_Position();

	_tchar tagTemp[MAX_PATH];
	wsprintf(tagTemp, L"BaboonBome %d", m_iBombCount);

	pGameObject = CBaboonBomb::Create(m_pGraphicDev, &pObj, pMaySpace, m_iBombCount);
	NULL_CHECK_RETURN(pGameObject, );
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagTemp, pGameObject, Engine::Get_Scene()), );


	m_iBombCount++;

	wsprintf(tagTemp, L"BaboonBome %d", m_iBombCount);
	pGameObject = CBaboonBomb::Create(m_pGraphicDev, &pObj, pCodySpace, m_iBombCount);
	NULL_CHECK_RETURN(pGameObject, );
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagTemp, pGameObject, Engine::Get_Scene()), );


	m_iBombCount++;

	m_bPatturn[Patturn_Bomb] = false;



}

void CSpaceBaboon::PatturnMoveUp(const _float & fTimeDelta)
{
	if (!m_bPatturn[Patturn_MoveUp])
		return;

	if (!m_bMoveUpDir)
	{
		m_vMoveUpStartPos = m_pTransformCom->Get_Position();
		m_bMoveUpDir = true;
	}

	_vec3 vDir, vPos;
	_float fDir;
	vPos = m_pTransformCom->Get_Position();
	vDir = m_vMoveUpStartPos - vPos;
	fDir = D3DXVec3Length(&vDir);

	if (fDir < m_fMoveUpDir)
	{
		m_pTransformCom->Move_Pos(&_vec3(0.f, 1.f, 0.f), 2.f, fTimeDelta);
	}
	else
	{
		m_bPatturn[Patturn_MoveUp] = false;
		m_bMoveUpDir = false;
	}

}

void CSpaceBaboon::PatturnMove(const _float & fTimeDelta)
{

	if (!m_bPatturn[Patturn_Move])
		return;

	m_pMeshCom->Set_Animationset(animID::FlyingSaucer_Ufo_Left);

	if (m_pTargetCody == nullptr || m_pTargetMay == nullptr)
	{
		m_fMissileTime += fTimeDelta;
		if (m_fMissileTime > 3.f)
		{
			m_bPatturn[Patturn_Missile] = true;
			m_fMissileTime = 0.f;
		}
	}

	_vec3 vParentPos = { 0.f, 0.f, 0.f };
	_vec3 vPos;
	_vec3 vLook;

	_vec3 vDir;
	_vec3 vRight;
	_vec3 vLeft;


	_matrix matWorld;
	matWorld = *m_pTransformCom->Get_WorldMatrix();
	vPos = { matWorld._41, matWorld._42, matWorld._43 };
	vPos.y = 0.f;
	m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
	m_pTransformCom->Get_Info(INFO_RIGHT, &vRight);
	vRight.y = 0.f;
	vLook.y = 0.f;
	D3DXVec3Normalize(&vRight, &vRight);
	D3DXVec3Normalize(&vLook, &vLook);
	vDir = vParentPos - vPos;
	vDir.y = 0.f;
	D3DXVec3Normalize(&vDir, &vDir);
	vLeft = vRight * -1.f;

	_matrix matRot;
	_vec3 vLookAngle;
	m_fAngle = D3DXToDegree(acosf(D3DXVec3Dot(&vLook, &vDir)));

	_float fRightAngle = D3DXVec3Dot(&vDir, &vRight);
	_float fLeftAngle = D3DXVec3Dot(&vDir, &vLeft);

	if (m_fAngle >= D3DXToDegree(m_fTurnSpeed))
	{
		if (fRightAngle < 0 && fLeftAngle > 0)
		{
			m_pTransformCom->Rotation(ROT_Y, -m_fTurnSpeed);
		}
		else if (fRightAngle > 0 && fLeftAngle < 0)
			m_pTransformCom->Rotation(ROT_Y, m_fTurnSpeed);
	}


	/*m_pTransformCom->Set_AddAngleY(-D3DXToRadian(0.5f));*/
	m_pTransformCom->Set_Parent(vParentPos);
	m_pTransformCom->Set_RevAddAngleY(D3DXToRadian(0.5f));


}

void CSpaceBaboon::PatturnBackMove(const _float & fTimeDelta)
{
	if (!m_bPatturn[Patturn_BackMove])
		return;

	m_pMeshCom->Set_Animationset(animID::FlyingSaucer_Ufo_Back);


	_vec3 vCenter = { 0.f, 0.f, 0.f };
	_vec3 vPos;
	_vec3 vLook;
	_vec3 vDir;
	_float fDir;
	_vec3 vRight;
	_vec3 vLeft;
	_vec3 vBack;

	vPos = m_pTransformCom->Get_Position();
	vPos.y = 10.f;
	m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
	m_pTransformCom->Get_Info(INFO_RIGHT, &vRight);
	vLook.y = 0.f;
	vRight.y = 0.f;
	D3DXVec3Normalize(&vRight, &vRight);
	D3DXVec3Normalize(&vLook, &vLook);
	vBack = vLook * -1.f;
	vDir = vCenter - vPos;
	fDir = D3DXVec3Length(&vDir);
	vDir.y = 0.f;
	D3DXVec3Normalize(&vDir, &vDir);
	vLeft = vRight * -1.f;

	m_fAngle = D3DXToDegree(acosf(D3DXVec3Dot(&vLook, &vDir)));

	_float fRightAngle = D3DXVec3Dot(&vDir, &vRight);
	_float fLeftAngle = D3DXVec3Dot(&vDir, &vLeft);

	if (m_fAngle >= D3DXToDegree(m_fTurnSpeed))
	{
		if (fRightAngle < 0 && fLeftAngle > 0)
		{
			m_pTransformCom->Rotation(ROT_Y, -m_fTurnSpeed);
		}
		else if (fRightAngle > 0 && fLeftAngle < 0)
			m_pTransformCom->Rotation(ROT_Y, m_fTurnSpeed);
	}

	if (fDir < 40.f)
	{
		m_pTransformCom->Move_Pos(&vLook, -20.f, fTimeDelta);
	}
	else
	{
		m_bPatturn[Patturn_BackMove] = false;
		m_bPatturn[Patturn_Move] = true;
	}

}

void CSpaceBaboon::PatturnHitReaction(const _float & fTimeDelta)
{
	if (!m_bPatturn[Patturn_HitReaction])
		return;

	if (!m_bHit)
	{
		m_pMeshCom->Set_Animationset(animID::FlyingSaucer_Ufo_HitReaction_Fwd);
		m_bHit = true;
	}

	if (m_iAniIndex == animID::FlyingSaucer_Ufo_HitReaction_Fwd && m_pMeshCom->Is_AnimationSetEnd(0.1f))
	{
		m_pMeshCom->Set_Animationset(animID::FlyingSaucer_Ufo_Mh);
		m_bPatturn[Patturn_HitReaction] = false;
		m_bHit = false;
	}
}



CSpaceBaboon * CSpaceBaboon::Create(LPDIRECT3DDEVICE9 pGraphicDev, void * pArg)
{
	CSpaceBaboon*	pInstance = new CSpaceBaboon(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pArg)))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSpaceBaboon::Free(void)
{
	Safe_Release(m_pBossHp);
	Safe_Release(m_pBossHpbg);
	Safe_Release(m_pBossName);
	CEnermy::Free();
	m_pLaser->Release();
}

void CSpaceBaboon::Effect(const _float & fTimeDelta)
{
	Effect_LaserHit(fTimeDelta);
	Effect_Idle(fTimeDelta);
	Effect_Dead(fTimeDelta);
}

void CSpaceBaboon::Effect_LaserHit(const _float & fTimeDelta)
{
	if (m_iHP <= 0)
		return;
	if (!m_pLaser->Get_Active())
		return;

	PARTICLE tParticle;
	Engine::Reset_Particle(tParticle);

	tParticle.fAcc = 1.f;
	tParticle.fCurrentTime = 0.f;
	tParticle.vEndSize = _vec3(0.01f, 0.01f, 0.01f);
	tParticle.fRotateSpeed = 0.f;

	tParticle.vStartColor = { 255.f * 2, 125.f * 2, 85.f * 2, 255.f * 2.5f };
	tParticle.vEndColor = { 255.f, 125.f, 255.f, 85.f };
	tParticle.vRot = { 0.f, 0.f, 0.f };
	tParticle.bGravity = true;
	tParticle.fGravityForce = 0.5f;

	for (_int i = 0; i < 5; ++i)
	{
		tParticle.fVelocity = CRandoms()(3.5f, 5.5f);
		tParticle.fLifeTime = CRandoms()(0.5f, 1.f);
		_float fRand = CRandoms()(30, 80) * 0.001f;
		tParticle.vStartSize = _vec3(fRand, fRand, fRand);

		tParticle.vDir = { CRandoms()(-1.f, 1.f),  CRandoms()(0.f, 1.f), CRandoms()(-1.f, 1.f) };
		tParticle.vPos = m_vUltHitPos + _vec3(CRandoms()(-0.3f, 0.3f), CRandoms()(-0.3f, 0.3f), CRandoms()(-0.3f, 0.3f));
		Engine::Emit_ImageParticle(tParticle, 15, 11);
	}

	for (_int i = 0; i < 3; ++i)
	{
		tParticle.fVelocity = CRandoms()(10.5f, 15.5f);
		tParticle.fLifeTime = CRandoms()(0.3f, 0.5f);
		tParticle.vStartSize = _vec3(0.02f, 0.02f, 0.02f);
		tParticle.vStartColor = { 255.f * 2, 200.f * 2, 85.f * 2, 255.f * 2.5f };

		tParticle.vDir = { CRandoms()(-1.f, 1.f),  CRandoms()(0.f, 1.f), CRandoms()(-1.f, 1.f) };
		tParticle.vPos = m_vUltHitPos + _vec3(CRandoms()(-0.3f, 0.3f), CRandoms()(-0.3f, 0.3f), CRandoms()(-0.3f, 0.3f));
		Engine::Emit_ImageParticle(tParticle, 15, 11);
	}

	PARTICLE tParticle2;
	Engine::Reset_Particle(tParticle2);
	Engine::Set_AnimState(tParticle2, 8, 8, 1.f);
	tParticle2.fAcc = 1.f;
	tParticle2.fCurrentTime = 0.f;
	tParticle2.vStartSize = _vec3(5.f, 5.f, 5.f);
	tParticle2.vEndSize = _vec3(5.f, 5.f, 5.f);
	tParticle2.fRotateSpeed = 0.f;
	tParticle2.vStartColor = { 30.f, 30.f, 30.f, 255.f * 1.5f };
	tParticle2.vEndColor = { 30.f, 30.f, 30.f, 0.f };
	tParticle2.vRot = { 0.f, 0.f, D3DXToRadian(CRandoms()(0.f, 360.f)) };
	tParticle2.fVelocity = 0.f;
	tParticle2.fLifeTime = 0.5f;
	tParticle2.vRot = { 0.f, 0.f, 0.f };

	tParticle2.vDir = { 0.f, 0.f, 0.f };
	tParticle2.vPos = m_vUltHitPos + _vec3(CRandoms()(-0.1f, 0.1f), 0.f, CRandoms()(-0.1f, 0.1f));
	Engine::Emit_ImageParticle(tParticle2, 23, 4);


	tParticle2.vStartSize = _vec3(2.f, 2.f, 2.f);
	tParticle2.vEndSize = _vec3(3.2f, 3.2f, 3.2f);
	tParticle2.fLifeTime = 0.4f;
	tParticle2.vStartColor = { 255.f * 3.5f, 125.f* 3.5f, 85.f* 3.5f, 255.f };
	tParticle2.vEndColor = { 255.f, 65.f, 35.f, 0.f };
	tParticle2.vPos = m_vUltHitPos + _vec3(CRandoms()(-0.1f, 0.1f), 0.f, CRandoms()(-0.1f, 0.1f));
	Engine::Emit_ImageParticle(tParticle2, 30, 26);



}

void CSpaceBaboon::Effect_Idle(const _float & fTimeDelta)
{
	if (m_iHP <= 0)
		return;

	PARTICLE tParticle;
	Engine::Reset_Particle(tParticle);

	tParticle.fAcc = 1.f;
	tParticle.fCurrentTime = 0.f;

	tParticle.vEndSize = _vec3(0.01f, 0.01f, 0.01f);
	tParticle.fRotateSpeed = 0.f;

	tParticle.vStartColor = { 120.f * 3.5f, 200.f * 3.5f, 255.f * 3.5f, 255.f * 2.f };
	tParticle.vEndColor = { 120.f * 3.5f, 200.f * 3.5f, 255.f * 3.5f, 255.f * 2.f };
	tParticle.vRot = { 0.f, 0.f, 0.f };
	tParticle.vDir = { 0.f, -1.f, 0.f };



	const D3DFRAME_DERIVED* pBone = m_pMeshCom->Get_FrameByName("Base");
	_mat matBase = pBone->CombinedTranformationMatrix **m_pTransformCom->Get_WorldMatrix();
	D3DXVec3TransformNormal(&tParticle.vDir, &tParticle.vDir, &matBase);

	_vec3 vPos = *((_vec3*)(&matBase._41));

	for (_int i = 0; i < 2; ++i)
	{
		_float	fRand = CRandoms()(0.3f, 0.5f);
		tParticle.vStartSize = _vec3(fRand, fRand, fRand);
		tParticle.fVelocity = CRandoms()(1.f, 2.f);
		tParticle.fLifeTime = CRandoms()(1.f, 1.5f);
		tParticle.vPos = vPos + _vec3{ 0.f,-1.f,0.f } +_vec3(CRandoms()(-2.f, 2.f), CRandoms()(-0.3f, 0.3f), CRandoms()(-2.f, 2.f));

		Engine::Emit_ImageParticle(tParticle, 15, 11);
	}
}

void CSpaceBaboon::Effect_Dead(const _float & fTimeDelta)
{
	if (m_iHP > 0)
		return;

	m_fDeadTime += fTimeDelta;
	m_fDeadEffect += fTimeDelta;


	CMayCamera* pMayCam = (CMayCamera*)Engine::Get_GameObject(L"GameObject", L"MayCamera");
	CCodyCamera* pCodyCam = (CCodyCamera*)Engine::Get_GameObject(L"GameObject", L"CodyCamera");

	if (m_fDeadEffect > 0.2f)
	{
		StopNPlaySound(L"Rocket_Dead.wav", CSoundMgr::CHANNELID::Space_Missile_Dead);
		pMayCam->Set_Shake();
		pCodyCam->Set_Shake();

		_matrix matWorld;
		const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("Base");
		const _matrix* pMatrix = &pFrame->CombinedTranformationMatrix;

		m_pTransformCom->Get_WorldMatrix(&matWorld);
		matWorld = *pMatrix * matWorld;

		_vec3 vPos = { matWorld._41, matWorld._42, matWorld._43 };
		_vec3 FixPos = vPos + _vec3(CRandoms()(-5.f, 5.f), CRandoms()(-3.f, 5.f), CRandoms()(-5.f, 5.f));


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

		tParticle.vPos = FixPos;
		Engine::Emit_ImageParticle(tParticle, 23, 4);


		Engine::Set_AnimState(tParticle, 8, 8, 1.f);
		tParticle.fLifeTime = 1.f;
		tParticle.vStartSize = _vec3(8.f, 8.f, 8.f);
		tParticle.vEndSize = _vec3(8.f, 8.f, 8.f);
		tParticle.vStartColor = { 0.f ,0.f, 0.f, 255.f };
		tParticle.vEndColor = { 0.f ,0.f, 0.f, 155.f };
		tParticle.vPos = FixPos;
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
		tParticle2.vPos = FixPos;

		for (_int i = 0; i < 30; ++i)
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

		for (_int i = 0; i < 30; ++i)
		{
			_float fRand = CRandoms()(50, 100) * 0.003f;
			tParticle3.vStartSize = _vec3(fRand, fRand, fRand);
			tParticle3.fVelocity = CRandoms()(0, 10) * 0.1f + 2.5f;
			tParticle3.vDir = { CRandoms()(-100, 100) * 0.01f,CRandoms()(-100, 100) * 0.01f , CRandoms()(-100, 100) * 0.01f };
			tParticle3.fLifeTime = CRandoms()(1, 5) * 0.1f + 0.8f;
			tParticle3.vPos = FixPos + _vec3(CRandoms()(-30, 30) * 0.01f, CRandoms()(-30, 30) * 0.01f, CRandoms()(-30, 30) * 0.01f);
			Engine::Emit_ImageParticle(tParticle3, 15, 11);
		}
		m_fDeadEffect = 0.f;
	}

	if (m_fDeadTime > 5.f)
	{
		m_bDead = true;
		m_fDeadTime = 0.f;
	}
}

void CSpaceBaboon::Create_UI(const _tchar * pLayerTag)
{
	Engine::CLayer*			pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, );

	// 오브젝트 추가
	Engine::CGameObject*		pGameObject = nullptr;
	OBJINFO tObj;
	ZeroMemory(&tObj, sizeof(OBJINFO));
	tObj.eSceneID = RESOURCE_STATIC;

	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pBossName = pGameObject->Is<CImageUI>();
	m_pBossName->Set_Image(L"BossName");
	m_pBossName->Set_Size(_vec3{ 168.f, 25.f, 0.f });
	m_pBossName->Set_Pos(_vec3{ 180.f, -65.f, 0.f });
	m_pBossName->Set_RenderID(RENDER_UI);
	m_pBossName->Set_PassIndex(2);
	m_pBossName->Set_TextureIndex(2);
	NULL_CHECK_RETURN(pGameObject, );
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"BossName", pGameObject), );
	m_pBossName->AddRef();

	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pBossHp = pGameObject->Is<CImageUI>();
	m_pBossHp->Set_Image(L"HP");
	m_pBossHp->Set_Size(_vec3{ 1600.f, 50.f, 0.f });
	m_pBossHp->Set_Pos(_vec3{ 160.f, -90.f, 0.f });
	m_pBossHp->Set_RenderID(RENDER_MENUUI);
	m_pBossHp->Set_PassIndex(4);
	m_pBossHp->Set_TextureIndex(0);
	NULL_CHECK_RETURN(pGameObject, );
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"BossHP", pGameObject), );
	m_pBossHp->AddRef();

	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pBossHpbg = pGameObject->Is<CImageUI>();
	m_pBossHpbg->Set_Image(L"HP");
	m_pBossHpbg->Set_Size(_vec3{ 1600.f, 50.f, 0.f });
	m_pBossHpbg->Set_Pos(_vec3{ 160.f, -90.f, 0.f });
	m_pBossHpbg->Set_RenderID(RENDER_UI);
	m_pBossHpbg->Set_PassIndex(5);
	m_pBossHpbg->Set_TextureIndex(0);
	NULL_CHECK_RETURN(pGameObject, );
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"BossHPBG", pGameObject), );
	m_pBossHpbg->AddRef();

	Engine::Add_Layer(pLayerTag, pLayer);
}

void CSpaceBaboon::Update_UI(const _float & fTimeDelta)
{
	HP_UI(fTimeDelta);
}

void CSpaceBaboon::HP_UI(const _float & fTimeDelta)
{
	if (m_iHP <= 0)
	{
		m_pBossHp->Set_Visible(false);
		m_pBossHpbg->Set_Visible(false);
		m_pBossName->Set_Visible(false);
	}

	m_pBossHp->Set_Amount((_float)m_iHP / (_float)m_iHPMax);
}
