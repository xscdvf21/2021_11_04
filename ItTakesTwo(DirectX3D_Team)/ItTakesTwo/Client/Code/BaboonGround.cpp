#include "stdafx.h"
#include "BaboonGround.h"
#include "SpaceBaboon.h"
#include "Export_Function.h"
#include "Cody_Space.h"
#include "May_Space.h"

CBaboonGround::CBaboonGround(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
	,m_bMoveDir(false), m_bUp(false),m_fUpDis(25.f), m_fBombTime(0.f)
{
}

CBaboonGround::~CBaboonGround()
{
}

HRESULT CBaboonGround::Ready_Object(void * pArg)
{
	FAILED_CHECK_RETURN(Add_Component(pArg), E_FAIL);
	OBJINFO tTemp;
	ZeroMemory(&tTemp, sizeof(OBJINFO));

	if (nullptr != pArg)
	{
		memcpy(&tTemp, pArg, sizeof(OBJINFO));
	}
	return S_OK;
}

_int CBaboonGround::Update_Object(const _float & fTimeDelta)
{

	Move(fTimeDelta);

	_vec3 vPos;
	vPos = m_pTransformCom->Get_Position();

	PxTransform pxTransform = m_pRigidActor->getGlobalPose();
	pxTransform.p.x = vPos.x;
	pxTransform.p.y = vPos.y;
	pxTransform.p.z = vPos.z;
	_vec3 vRot = m_pTransformCom->Get_Angle();
	pxTransform.q = PxQuat(vRot.x, { 1.f,0.f,0.f })*PxQuat(vRot.y, { 0.f,1.f,0.f })*PxQuat(vRot.z, { 0.f,0.f,1.f });
	m_pRigidActor->setGlobalPose(pxTransform);


	Engine::CGameObject::Update_Object(fTimeDelta);
	Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);

	return OBJ_NOEVENT;
}

_int CBaboonGround::LateUpdate_Object(const _float & fTimeDelta)
{

	return _int();
}

void CBaboonGround::Render_Object(const _int & iIndex)
{
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect, iIndex), );

	_uint	iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);		// 1인자 : 현재 쉐이더 파일이 갖고 있는 pass의 최대 개수, 2인자 : 시작하는 방식(default)
										//pEffect->BeginPass(0);
	m_pMeshCom->Render_Meshes(pEffect,8);

	pEffect->End();

	Safe_Release(pEffect);
}

HRESULT CBaboonGround::Add_Component(void * pArg)
{
	Engine::CComponent*		pComponent = nullptr;

	OBJINFO tTemp;
	if (pArg)
	{
		memcpy(&tTemp, pArg, sizeof(OBJINFO));
	}


	pComponent = m_pMeshCom = static_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(tTemp.eSceneID, L"BaboonGround"));
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
	pComponent = m_pShaderCom = static_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_All"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);

	m_pTransformCom->Set_Scale(tTemp.vScale);
	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(tTemp.vAngle.x));
	m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(tTemp.vAngle.y));
	m_pTransformCom->Rotation(ROT_Z, D3DXToRadian(tTemp.vAngle.z));
	m_pTransformCom->Set_Pos(tTemp.vPos);

	auto* pPhysics = Engine::Get_Physics();
	_vec3 vPos = m_pTransformCom->Get_Position();

	PxTransform tTransform(vPos.x, vPos.y, vPos.z, PxQuat(m_pTransformCom->Get_Angle().y, { 0.f,1.f,0.f }));
	PxMaterial* pMaterial = pPhysics->createMaterial(0.5f, 0.5f, 0.0f);

	PxTriangleMeshGeometry pMeshGeo;
	pMeshGeo.triangleMesh = m_pMeshCom->Get_PxMesh();//m_pPhysMesh->Get_PxMesh();
	pMeshGeo.scale = PxMeshScale(PxVec3(tTemp.vScale.x, tTemp.vScale.y, tTemp.vScale.z));

	PxShape* pShape = pPhysics->createShape(pMeshGeo, *pMaterial, true);

	pShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
	pShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, false);
	pShape->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, true);
	pShape->setQueryFilterData(PxFilterData(FilterGroup::eGround, 0, 0, 0));


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


	return S_OK;
}

HRESULT CBaboonGround::SetUp_ConstantTable(LPD3DXEFFECT & pEffect, const _int & iIndex)
{
	_matrix			matWorld, matView, matProj;

	m_pTransformCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	pEffect->SetVector("g_vColor", &_vec4(255.f, 255.f, 255.f, 255.f));

	return S_OK;
}

void CBaboonGround::Move(const _float & fTimeDelta)
{
	if (!m_bUp)
		return;

	m_fBombTime += fTimeDelta;

	if (!m_bSoundStart[SOUNDID::Ground_Up])
	{
		StopSound(CSoundMgr::CHANNELID::Effect);
		PlaySoundW(L"SpaceBoss_GroundUP.wav", CSoundMgr::CHANNELID::Effect, 0.5f);

		m_bSoundStart[SOUNDID::Ground_Up] = true;

	}

	if (!m_bMoveDir)
	{
		m_vStartPos = m_pTransformCom->Get_Position();
		m_bMoveDir = true;
	}

	_vec3 vDir, vPos;
	_float fDir;
	vPos = m_pTransformCom->Get_Position();
	vDir = m_vStartPos - vPos;
	fDir = D3DXVec3Length(&vDir);

	if (m_fBombTime > 2.f)
	{
		CSpaceBaboon*		pBaboon = (CSpaceBaboon*)Engine::Get_GameObject(L"GameObject", L"SpaceBoss");
		pBaboon->Set_Patturn(BaboonPatturnID::Patturn_Bomb);
		m_fBombTime = 0.f;
	}


	if (fDir < m_fUpDis)
	{
		m_pTransformCom->Move_Pos(&_vec3(0.f, 1.f, 0.f), 2.f, fTimeDelta);
		

		Engine::Get_GameObject(L"GameObject", L"Cody")->Is<CCody_Space>()->Set_SavePointY(m_pTransformCom->Get_Position().y + 2.0f);
		Engine::Get_GameObject(L"GameObject", L"May")->Is<CMay_Space>()->Set_SavePointY(m_pTransformCom->Get_Position().y + 2.0f);
	}
	else
	{
		StopSound(CSoundMgr::CHANNELID::Effect);
		PlaySoundW(L"SpaceBoss_GroundStop.wav", CSoundMgr::CHANNELID::Effect, 0.5f);

		m_bUp = false;
		m_bMoveDir = false;

		m_bSoundStart[SOUNDID::Ground_Up] = false;
	}

}

CBaboonGround * CBaboonGround::Create(LPDIRECT3DDEVICE9 pGraphicDev, void * pArg)
{
	CBaboonGround*	pInstance = new CBaboonGround(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pArg)))
	{
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBaboonGround::Free(void)
{
	Engine::CGameObject::Free();
}
