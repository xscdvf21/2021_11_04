#include "stdafx.h"
#include "LaserMirror.h"
#include "SpaceBaboon.h"
#include "WindLaser.h"
#include "Export_Function.h"
CLaserMirror::CLaserMirror(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
	, m_bCreateTrigger(false), m_eType(Mirror_End), m_fHeight(25.f), m_fAllUpDir(25.f), m_bAllMoveDir(false)
{

}

CLaserMirror::~CLaserMirror()
{
}

HRESULT CLaserMirror::Ready_Object(void * pArg, LaserMirrorType _eType)
{
	FAILED_CHECK_RETURN(Add_Component(pArg), E_FAIL);
	OBJINFO tTemp;
	ZeroMemory(&tTemp, sizeof(OBJINFO));

	if (nullptr != pArg)
	{
		memcpy(&tTemp, pArg, sizeof(OBJINFO));
	}

	m_eType = _eType;

	if (m_eType == Mirror_Default)
	{
		m_fHeight = 25.f;
	}
	else if (m_eType == Mirror_Gravity)
	{
		m_fHeight = 55.f;
	}
	return S_OK;
}

_int CLaserMirror::Update_Object(const _float & fTimeDelta)
{

	if (!m_bCreateTrigger)
	{
		TriggerCreate();
	}


	if (m_bDead)
		return OBJ_DEAD;

	if (Key_Pressing(DIK_F5))
	{
		CSpaceBaboon*		pBaboon = (CSpaceBaboon*)Get_GameObject(L"GameObject", L"SpaceBoss");
		pBaboon->Set_Patturn(Patturn_BackMove);

		m_bMove[AllMove_Up] = true;
	}

	_vec3 vPos;
	vPos = m_pTransformCom[Mesh_Mirror]->Get_Position();
	m_pTrigger->Set_Pos(vPos);

	PxTransform pxTransform = m_pRigidActor->getGlobalPose();
	pxTransform.p.x = vPos.x;
	pxTransform.p.y = vPos.y;
	pxTransform.p.z = vPos.z;
	_vec3 vRot = m_pTransformCom[Mesh_Mirror]->Get_Angle();
	pxTransform.q = PxQuat(vRot.x, { 1.f,0.f,0.f })*PxQuat(vRot.y, { 0.f,1.f,0.f })*PxQuat(vRot.z, { 0.f,0.f,1.f });
	m_pRigidActor->setGlobalPose(pxTransform);

	if (m_bState[Mirror_Hit] && !m_bState[Mirror_HitEnd])
	{
		CSpaceBaboon*		pBaboon = (CSpaceBaboon*)Get_GameObject(L"GameObject", L"SpaceBoss");
		pBaboon->Set_Damage(-1);
		m_bState[Mirror_HitEnd] = true;
		m_pTrigger->Set_Interactable(false);

		if (m_eType == Mirror_Gravity)
		{
			pBaboon->Set_Patturn(Patturn_BackMove);
		}
	}

	if (m_bState[Mirror_MovePossible] && !m_bMove[AllMove_Up])
	{
		MoveUp(fTimeDelta);
		MoveDown(fTimeDelta);
	}
	AllMoveUp(fTimeDelta);


	Engine::CGameObject::Update_Object(fTimeDelta);
	Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);

	return OBJ_NOEVENT;
}

_int CLaserMirror::LateUpdate_Object(const _float & fTimeDelta)
{
	return _int();
}

void CLaserMirror::Render_Object(const _int & iIndex)
{
	if (!m_bState[Mirror_HitEnd])
	{
		for (_uint i = 0; i < Mesh_End; ++i)
		{
			LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
			NULL_CHECK(pEffect);
			pEffect->AddRef();

			FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect, iIndex, i), );

			_uint	iPassMax = 0;
			pEffect->Begin(&iPassMax, 0);		// 1인자 : 현재 쉐이더 파일이 갖고 있는 pass의 최대 개수, 2인자 : 시작하는 방식(default)
											//pEffect->BeginPass(0);
			if (i == Mesh_Mirror)
				m_pMeshCom[i]->Render_Meshes(pEffect, 9);
			else
				m_pMeshCom[i]->Render_Meshes(pEffect, 8);

			pEffect->End();

			Safe_Release(pEffect);
		}
	}
	else if (m_bState[Mirror_HitEnd])
	{
		PlaySoundOnce(L"Mirror_Break.wav", Engine::CSoundMgr::CHANNELID::Space_Boss_Mirror_Break, 0.2f);

		for (_uint i = 0; i < Mesh_End; ++i)
		{
			if (i == Mesh_Glass)
			{
				continue;
			}

			LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
			NULL_CHECK(pEffect);
			pEffect->AddRef();

			FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect, iIndex, i), );

			_uint	iPassMax = 0;

			pEffect->Begin(&iPassMax, 0);		// 1인자 : 현재 쉐이더 파일이 갖고 있는 pass의 최대 개수, 2인자 : 시작하는 방식(default)
												//pEffect->BeginPass(0);
			if (i == Mesh_Mirror)
				m_pMeshCom[i]->Render_Meshes(pEffect, 9);
			else
				m_pMeshCom[i]->Render_Meshes(pEffect, 8);

			pEffect->End();

			Safe_Release(pEffect);
		}

	}

}

const _bool CLaserMirror::Check_Hit()
{
	if (true == m_bState[Mirror_HitPossible])
	{

		Set_MoveDown(true);
		Set_State(Mirror_Hit, true);
		Effect_Explosive();
		return true;
	}
	return false;
}
void CLaserMirror::Effect_Explosive()
{
	PARTICLE tParticle;
	Engine::Reset_Particle(tParticle);
	Engine::Set_AnimState(tParticle, 8, 8, 1.f);
	tParticle.fAcc = 1.f;
	tParticle.fCurrentTime = 0.f;
	tParticle.vStartSize = _vec3(15.f, 15.f, 15.f);
	tParticle.vEndSize = _vec3(15.f, 15.f, 15.f);
	tParticle.fRotateSpeed = 0.f;
	tParticle.vStartColor = { 180.f * 1.5f, 200.f * 1.5f, 255.f * 2.f, 255.f };
	tParticle.vEndColor = { 180.f * 1.5f, 200.f * 1.5f, 255.f * 2.f, 0.f };
	tParticle.fVelocity = 0.f;
	tParticle.fLifeTime = 1.f;

	tParticle.vRot = { 0.f, 0.f, 0.f };
	tParticle.vDir = { 0.f, 0.f, 0.f };

	tParticle.vPos = m_pTransformCom[Mesh_Sphere]->Get_Position();
	Engine::Emit_ImageParticle(tParticle, 23, 4);


	Engine::Set_AnimState(tParticle, 8, 8, 1.f);
	tParticle.fLifeTime = 1.f;
	tParticle.vStartSize = _vec3(15.f, 15.f, 15.f);
	tParticle.vEndSize = _vec3(15.f, 15.f, 15.f);
	tParticle.vStartColor = { 180.f * 5.5f, 200.f * 5.5f, 255.f * 7.5f, 255.f * 3.5f };
	tParticle.vPos = m_pTransformCom[Mesh_Sphere]->Get_Position();
	Engine::Emit_ImageParticle(tParticle, 29, 4);
	tParticle.vStartSize = _vec3(12.f, 12.f, 12.f);
	tParticle.vEndSize = _vec3(12.f, 12.f, 12.f);
	Engine::Emit_ImageParticle(tParticle, 30, 4);
	tParticle.vStartSize = _vec3(9.f, 9.f, 9.f);
	tParticle.vEndSize = _vec3(9.f, 9.f, 9.f);
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
	tParticle2.vPos = m_pTransformCom[Mesh_Sphere]->Get_Position();

	for (_int i = 0; i < 50; ++i)
	{
		tParticle2.fLifeTime = CRandoms()(0, 2) * 0.1f + 0.5f;
		tParticle2.vRot = { D3DXToRadian(CRandoms()(1, 360)), D3DXToRadian(CRandoms()(1, 360)), D3DXToRadian(CRandoms()(1, 360)) };
		_float fRand = CRandoms()(10, 30) * 0.00015f;
		tParticle2.vStartSize = _vec3(fRand, fRand, fRand);
		tParticle2.fVelocity = _float(CRandoms()(1, 10) + 4.f);
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
		_float fRand = CRandoms()(50, 100) * 0.005f;
		tParticle3.vStartSize = _vec3(fRand, fRand, fRand);
		tParticle3.fVelocity = CRandoms()(0, 10) * 0.1f + 4.5f;
		tParticle3.vDir = { CRandoms()(-100, 100) * 0.01f,CRandoms()(-100, 100) * 0.01f , CRandoms()(-100, 100) * 0.01f };
		tParticle3.fLifeTime = CRandoms()(1, 5) * 0.1f + 0.8f;
		tParticle3.vPos = m_pTransformCom[Mesh_Sphere]->Get_Position() + _vec3(CRandoms()(-30, 30) * 0.01f, CRandoms()(-30, 30) * 0.01f, CRandoms()(-30, 30) * 0.01f);
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
	tParticle4.vPos = m_pTransformCom[Mesh_Sphere]->Get_Position();

	for (_int i = 0; i < 30; ++i)
	{
		tParticle4.fLifeTime = CRandoms()(0.3f, 0.5f);
		_float fRand = CRandoms()(3.f, 6.f);
		tParticle4.vStartSize = _vec3(CRandoms()(1.f, 2.f), fRand, 2.f);
		tParticle4.fVelocity = CRandoms()(10.f, 15.f);

		//tParticle4.vDir = { 1.f, 0.f, 0.f };
		tParticle4.vDir = { CRandoms()(-100, 100) * 0.01f, 0.f, CRandoms()(-100, 100) * 0.01f };
		_float fRadian = atan2f(tParticle4.vDir.x, tParticle4.vDir.z);
		tParticle4.vRot = { D3DXToRadian(90.f), fRadian, 0.f };

		Engine::Emit_ImageParticle(tParticle4, 15, 21);
	}
}

HRESULT CLaserMirror::Add_Component(void * pArg)
{
	Engine::CComponent*		pComponent = nullptr;

	OBJINFO tTemp;
	if (pArg)
	{
		memcpy(&tTemp, pArg, sizeof(OBJINFO));
	}

	pComponent = m_pMeshCom[Mesh_Mirror] = static_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(tTemp.eSceneID, L"CorePillar"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_MeshMirror", pComponent);

	pComponent = m_pMeshCom[Mesh_BrokeGlass] = static_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(tTemp.eSceneID, L"CoreGlass_Broken"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_MeshBrokeMirror", pComponent);

	pComponent = m_pMeshCom[Mesh_Cover] = static_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(tTemp.eSceneID, L"CoreCover"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_MeshCover", pComponent);

	pComponent = m_pMeshCom[Mesh_Glass] = static_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(tTemp.eSceneID, L"CoreGlass"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_MeshGlass", pComponent);

	pComponent = m_pMeshCom[Mesh_Sphere] = static_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(tTemp.eSceneID, L"IceOrb"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_MeshSphere", pComponent);

	// Transform
	pComponent = m_pTransformCom[Mesh_Mirror] = static_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_TransformMirror", pComponent);

	pComponent = m_pTransformCom[Mesh_BrokeGlass] = static_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_TransformBrokeMirror", pComponent);

	pComponent = m_pTransformCom[Mesh_Cover] = static_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_TransformCover", pComponent);

	pComponent = m_pTransformCom[Mesh_Glass] = static_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_TransformGlass", pComponent);

	pComponent = m_pTransformCom[Mesh_Sphere] = static_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_TransformSphere", pComponent);

	// Calculator
	pComponent = m_pCalculatorCom = static_cast<Engine::CCalculator*>(Engine::Clone_Prototype(L"Proto_Calculator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Calculator", pComponent);

	// Collider
	pComponent = m_pColliderCom = Engine::CCollider::Create(m_pGraphicDev, m_fColDis, _vec4(50.f, 244.f, 234.f, 0.f));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Collider", pComponent);

	// Shader
	pComponent = m_pShaderCom = static_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_All"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);

	for (_uint i = 0; i < Mesh_End; ++i)
	{
		m_pTransformCom[i]->Set_Scale(tTemp.vScale);
		m_pTransformCom[i]->Rotation(ROT_X, D3DXToRadian(tTemp.vAngle.x));
		m_pTransformCom[i]->Rotation(ROT_Y, D3DXToRadian(tTemp.vAngle.y));
		m_pTransformCom[i]->Rotation(ROT_Z, D3DXToRadian(tTemp.vAngle.z));
		m_pTransformCom[i]->Set_Pos(tTemp.vPos);
	}

	m_pTransformCom[Mesh_Sphere]->Set_Pos(tTemp.vPos - _vec3(0.f, 4.f, 0.f));

	auto* pPhysics = Engine::Get_Physics();
	_vec3 vPos = m_pTransformCom[Mesh_Mirror]->Get_Position();

	PxTransform tTransform(vPos.x, vPos.y, vPos.z, PxQuat(m_pTransformCom[Mesh_Mirror]->Get_Angle().y, { 0.f,1.f,0.f }));
	PxMaterial* pMaterial = pPhysics->createMaterial(0.5f, 0.5f, 0.0f);

	PxTriangleMeshGeometry pMeshGeo;
	pMeshGeo.triangleMesh = m_pMeshCom[Mesh_Mirror]->Get_PxMesh();//m_pPhysMesh->Get_PxMesh();
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
	pShape->setName((char*)this);


	pMeshGeo.triangleMesh = m_pMeshCom[Mesh_Glass]->Get_PxMesh();//m_pPhysMesh->Get_PxMesh();
	pMeshGeo.scale = PxMeshScale(PxVec3(tTemp.vScale.x, tTemp.vScale.y, tTemp.vScale.z));

	pShape = pPhysics->createShape(PxCapsuleGeometry(1.f, 1.f), *pMaterial, true);
	pShape->setLocalPose(PxTransform(0.f, -4.f, 0.f, PxQuat(D3DXToRadian(90.F), { 0.f,0.f,1.f })));
	pShape->setName((char*)this);
	pShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
	pShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, false);
	pShape->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, true);
	pShape->setQueryFilterData(PxFilterData(FilterGroup::eGround, 0, 0, 0));

	pBody->attachShape(*pShape);

	return S_OK;
}

HRESULT CLaserMirror::SetUp_ConstantTable(LPD3DXEFFECT & pEffect, const _int & iIndex, const _int& iMeshNum)
{
	_matrix			matWorld, matView, matProj;


	m_pTransformCom[iMeshNum]->Get_WorldMatrix(&matWorld);

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);


	//pEffect->SetVector("g_vColor", &_vec4(255.f, 255.f, 255.f, 255.f));

	if (m_bState[Mirror_MovePossible] && iMeshNum == Mesh_Mirror)
		pEffect->SetVector("g_vColor", &_vec4(255.f, 255.f, 255.f, 255.f));
	else
		pEffect->SetVector("g_vColor", &_vec4(0.f, 0.f, 0.f, 255.f));
	return S_OK;
}

void CLaserMirror::MoveUp(const _float & fTimeDelta)
{
	if (!m_bMove[Move_Up])
		return;

	if (m_fMoveDir < m_fHeight) //높이
	{
		for (_uint i = 0; i < Mesh_End; ++i)
		{
			m_pTransformCom[i]->Move_Pos(&_vec3(0.f, 1.f, 0.f), 2.f, fTimeDelta);
			m_fMoveDir += 2.f *fTimeDelta;
		}
		PlaySoundW(L"SpaceBoss_GroundUP.wav", CSoundMgr::CHANNELID::Space_Boss_Mirror_Move, 0.2f);
		m_bSound = true;
	}
	else
	{
		StopSound(CSoundMgr::CHANNELID::Space_Boss_Mirror_Move);
		if (m_bSound)
		{
			StopNPlaySound(L"SpaceBoss_GroundStop.wav", CSoundMgr::CHANNELID::Space_Boss_Mirror_Move_End, 0.2f);
			m_bSound = false;
		}

		if (m_fCoverMoveDir < 3.f)
		{
			m_pTransformCom[Mesh_Cover]->Move_Pos(&_vec3(0.f, -1.f, 0.f), 2.f, fTimeDelta);
			m_fCoverMoveDir += 2.f * fTimeDelta;
		}
		else // 덮게 다 열렸는지.
		{
			m_bState[Mirror_HitPossible] = true;
		}
	}
}

void CLaserMirror::MoveDown(const _float & fTimeDelta)
{
	if (!m_bMove[Move_Down])
		return;
	m_bSound = true;

	m_bState[Mirror_HitPossible] = false;

	if (!m_bState[Mirror_HitEnd])
	{
		if (m_fCoverMoveDir > 0.f)
		{
			m_pTransformCom[Mesh_Cover]->Move_Pos(&_vec3(0.f, 1.f, 0.f), 2.f, fTimeDelta);
			m_fCoverMoveDir -= 2.f * fTimeDelta;
		}
		else
		{
			if (m_fMoveDir > 0.f)
			{
				for (_uint i = 0; i < Mesh_End; ++i)
				{
					m_pTransformCom[i]->Move_Pos(&_vec3(0.f, -1.f, 0.f), 2.f, fTimeDelta);
					m_fMoveDir -= 2.f *fTimeDelta;
				}
				PlaySoundW(L"SpaceBoss_GroundUP.wav", CSoundMgr::CHANNELID::Space_Boss_Mirror_Move, 0.2f);

			}
			else if (m_fMoveDir < 0.f)
			{

				StopSound(CSoundMgr::CHANNELID::Space_Boss_Mirror_Move);
				StopNPlaySound(L"SpaceBoss_GroundStop.wav", CSoundMgr::CHANNELID::Space_Boss_Mirror_Move_End, 0.2f);
				m_bMove[Move_Down] = false;
			}

		}
	}
	else if (m_bState[Mirror_HitEnd])
	{


		if (m_fMoveDir > 0.f)
		{
			for (_uint i = 0; i < Mesh_End; ++i)
			{
				m_pTransformCom[i]->Move_Pos(&_vec3(0.f, -1.f, 0.f), 2.f, fTimeDelta);
				m_fMoveDir -= 2.f *fTimeDelta;
			}
		}
		else if (m_fMoveDir < 0.f && m_bState[Mirror_HitEnd])
		{
			if (m_eType == Mirror_Default)
			{
				if (!m_bWindLaserMove)
				{

					CWindLaser*  pWindLaser = (CWindLaser*)Engine::Get_GameObject(L"GameObject", L"WindLaser");
					pWindLaser->Set_AttUp();
					m_bWindLaserMove = true;
					m_bState[Mirror_MovePossible] = false;
					m_bMove[Move_Down] = false;

					StopSound(CSoundMgr::CHANNELID::Space_Boss_Mirror_Move);
					PlaySoundW(L"SpaceBoss_GroundStop.wav", CSoundMgr::CHANNELID::Space_Boss_Mirror_Move_End, 0.2f);

				}
			}

		}
	}


}

void CLaserMirror::AllMoveUp(const _float & fTimeDelta)
{
	if (!m_bMove[AllMove_Up])
		return;

	if (!m_bAllMoveDir)
	{
		for (_uint i = 0; i < Mesh_End; ++i)
		{
			m_vMoveUpStartPos[i] = m_pTransformCom[i]->Get_Position();
		}
		m_bAllMoveDir = true;
	}

	_vec3 vDir, vPos;
	_float fDir;
	vPos = m_pTransformCom[Mesh_Mirror]->Get_Position();
	vDir = m_vMoveUpStartPos[Mesh_Mirror] - vPos;
	fDir = D3DXVec3Length(&vDir);

	if (fDir < m_fAllUpDir)
	{
		for (_uint i = 0; i < Mesh_End; ++i)
		{
			m_pTransformCom[i]->Move_Pos(&_vec3(0.f, 1.f, 0.f), 2.f, fTimeDelta);
		}
	}
	else
	{
		for (_uint i = 0; i < Mesh_End; ++i)
		{
			m_pTransformCom[i]->Set_Pos(m_vMoveUpStartPos[i].x, m_vMoveUpStartPos[i].y + m_fAllUpDir, m_vMoveUpStartPos[i].z);
		}

		m_bMove[AllMove_Up] = false;
		m_bAllMoveDir = false;
	}

}

void CLaserMirror::TriggerCreate()
{


	switch (m_eType)
	{
	case Mirror_Default:
		m_pTrigger = CTrigger::CreateBoxTrigger(m_pScene, this, m_pTransformCom[Mesh_Mirror]->Get_Position(), 0.5f, TRIGGER_SPACE_LASER_DEFAULT, FilterGroup::eButton);
		break;
	case Mirror_Gravity:
		m_pTrigger = CTrigger::CreateBoxTrigger(m_pScene, this, m_pTransformCom[Mesh_Mirror]->Get_Position(), 0.5f, TRIGGER_SPACE_LASER_GRAVITY, FilterGroup::eButton);
		break;
	}

	m_bCreateTrigger = true;
}


CLaserMirror * CLaserMirror::Create(LPDIRECT3DDEVICE9 pGraphicDev, void * pArg, LaserMirrorType _eType)
{
	CLaserMirror*	pInstance = new CLaserMirror(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pArg, _eType)))
	{
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLaserMirror::Free(void)
{
	Engine::CGameObject::Free();
}
