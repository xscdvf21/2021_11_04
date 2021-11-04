#include "stdafx.h"
#include "SpaceBridge.h"
#include "Crusher.h"
#include "Export_Function.h"
#include "CodyCamera.h"
#include "MayCamera.h"



CSpaceBridge::CSpaceBridge(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
	, m_bMove(false)
	, m_fTurnSpeed(D3DXToRadian(0.45f))
	, m_iHitPointCount(0)
{
}

CSpaceBridge::~CSpaceBridge()
{
}

HRESULT CSpaceBridge::Ready_Object(void * pArg)
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

_int CSpaceBridge::Update_Object(const _float & fTimeDelta)
{	
	if (m_bMove)
	{
		Move(fTimeDelta);
	}

	Engine::CGameObject::Update_Object(fTimeDelta);
	Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);

	return OBJ_NOEVENT;
}

_int CSpaceBridge::LateUpdate_Object(const _float & fTimeDelta)
{
	return _int();
}

void CSpaceBridge::Render_Object(const _int & iIndex)
{
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect, iIndex), );

	_uint	iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);			//	1인자 : 현재 쉐이더 파일이 갖고 있는 pass의 최대 개수, 2인자 : 시작하는 방식(default)
	m_pMeshCom->Render_Meshes(pEffect, 8);	//	pEffect->BeginPass(0);
											//	스태틱매쉬8
											//	다이나믹6


	pEffect->End();

	Safe_Release(pEffect);
}

void* CSpaceBridge::On_Active(void* pCaller)
{
	m_bMove = true;
	StopNPlaySound(L"Play_Plat_PlayRoom_SpaceStation_PillowBridge_MovingRamp.bnk_7.wav", CSoundMgr::CHANNELID::ChessPawn);
	return nullptr;
}

HRESULT CSpaceBridge::Add_Component(void * pArg)
{
	Engine::CComponent*		pComponent = nullptr;

	OBJINFO tTemp;
	if (pArg)
	{
		memcpy(&tTemp, pArg, sizeof(OBJINFO));
	}



	pComponent = m_pMeshCom = static_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(tTemp.eSceneID, L"SpaceBridge"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Mesh", pComponent);

	// Transform
	pComponent = m_pTransformCom = static_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

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

	PxTransform tTransform(vPos.x, vPos.y, vPos.z, PxQuat(tTemp.vAngle.y, { 0.f,1.f,0.f }));
	PxMaterial* pMaterial = pPhysics->createMaterial(0.5f, 0.5f, 0.0f);

	PxTriangleMeshGeometry pMeshGeo;
	pMeshGeo.triangleMesh = m_pMeshCom->Get_PxMesh();//m_pPhysMesh->Get_PxMesh();
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



	return S_OK;
}

HRESULT CSpaceBridge::SetUp_ConstantTable(LPD3DXEFFECT & pEffect, const _int & iIndex)
{
	_matrix			matWorld, matView, matProj;

	m_pTransformCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	return S_OK;
}

void CSpaceBridge::Move(const _float & fTimeDelta)
{
	_vec3 vPos;
	vPos = m_pTransformCom->Get_Position();

	PxTransform pxTransform = m_pRigidActor->getGlobalPose();
	pxTransform.p.x = vPos.x;
	pxTransform.p.y = vPos.y;
	pxTransform.p.z = vPos.z;
	_vec3 vRot = m_pTransformCom->Get_Angle();
	pxTransform.q = PxQuat(vRot.x, { 1.f,0.f,0.f })*PxQuat(vRot.y, { 0.f,1.f,0.f })*PxQuat(vRot.z, { 0.f,0.f,1.f });
	m_pRigidActor->setGlobalPose(pxTransform);

	CCodyCamera* pCodyCam = (CCodyCamera*)Engine::Get_GameObject(L"GameObject", L"CodyCamera");
	CMayCamera* pMayCam = (CMayCamera*)Engine::Get_GameObject(L"GameObject", L"MayCamera");
	pCodyCam->Set_Shake();
	pMayCam->Set_Shake();


	m_fCurrentAngle += D3DXToDegree(m_fTurnSpeed);
	if (m_fAngle >= m_fCurrentAngle)
	{
		m_pTransformCom->Rotation(ROT_Z, -m_fTurnSpeed);
	}
	else if (m_fCurrentAngle >= m_fAngle)
	{
		m_bMove = false;
	}
}
CSpaceBridge * CSpaceBridge::Create(LPDIRECT3DDEVICE9 pGraphicDev, void * pArg)
{
	CSpaceBridge*	pInstance = new CSpaceBridge(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pArg)))
	{
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CSpaceBridge::Free(void)
{
	Engine::CGameObject::Free();
}
