#include "stdafx.h"
#include "BigPlug.h"
#include "Export_Function.h"
#include "MainCamera.h"
#include "PhysXMesh.h"
#include "LineObject.h"


CBigPlug::CBigPlug(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{

}

CBigPlug::~CBigPlug(void)
{
}

HRESULT CBigPlug::Ready_Object(void* pArg)
{
	// 테스트
	FAILED_CHECK_RETURN(Add_Component(pArg), E_FAIL);

	OBJINFO Temp;
	if (pArg)
	{
		memcpy(&Temp, pArg, sizeof(OBJINFO));
	}
	m_tObjInfo = Temp;

	return S_OK;
}

Engine::_int CBigPlug::Update_Object(const _float& fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;
	
	// 테스트
	Move(fTimeDelta);
	CGameObject::Update_Object(fTimeDelta);
	Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);

	// -2.466
	return OBJ_NOEVENT;
}

_int CBigPlug::LateUpdate_Object(const _float & fTimeDelta)
{
	_matrix matWorld;
	m_pTransformCom->Get_WorldMatrix(&matWorld);
	PxTransform cTrans = m_pRigidActor->getGlobalPose();
	cTrans.p.x = matWorld._41;
	cTrans.p.y = matWorld._42;
	cTrans.p.z = matWorld._43;

	//PxTransform cTrans = m_pRigidActor->getGlobalPose();
	_vec3 vRot = m_pTransformCom->Get_Angle();

	auto* pBody = m_pRigidActor->is<PxRigidDynamic>();
	cTrans.q = PxQuat(vRot.y, { 0.f,1.f,0.f });
	pBody->setKinematicTarget(cTrans);
	return _int();
}

void CBigPlug::Render_Object(const _int& iIndex)
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
}

CBigPlug* CBigPlug::Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg)
{
	CBigPlug*	pInstance = new CBigPlug(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CBigPlug::Free(void)
{
	Engine::CGameObject::Free();
}

HRESULT CBigPlug::Add_Component(void* pArg)
{
	Engine::CComponent*		pComponent = nullptr;

	OBJINFO tTemp;
	ZeroMemory(&tTemp, sizeof(OBJINFO));
	if (nullptr != pArg)
	{
		memcpy(&tTemp, pArg, sizeof(OBJINFO));
	}


	// CStaticMesh
	pComponent = m_pMeshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(tTemp.eSceneID, L"BigPlug"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Mesh", pComponent);

	// Transform
	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);
	m_pTransformCom->Set_Scale(tTemp.vScale);
	m_pTransformCom->Set_Pos(tTemp.vPos);
	m_pTransformCom->Set_Angle(tTemp.vAngle);
	//m_pTransformCom->Set_AddAngleX(D3DXToRadian(-90.0f));
	//m_pTransformCom->Set_AddAngleY(D3DXToRadian(90.0f));

	// Shader
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_All"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);

	auto* pPhysics = Engine::Get_Physics();
	_vec3 vPos = m_pTransformCom->Get_Position();

	PxTransform tTransform(vPos.x, vPos.y, vPos.z, PxQuat(m_pTransformCom->Get_Angle().y, { 0.f,1.f,0.f }));
	m_vStartPos = tTransform.p;
	PxMaterial* pMaterial = pPhysics->createMaterial(0.5f, 0.5f, 0.0f);
	PxTriangleMeshGeometry pMeshGeo;
	pMeshGeo.triangleMesh = m_pMeshCom->Get_PxMesh();
	_vec3 vScale = m_pTransformCom->Get_Scale();
	pMeshGeo.scale = PxMeshScale(PxVec3(vScale.x, vScale.y, vScale.z));

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

HRESULT CBigPlug::SetUp_ConstantTable(LPD3DXEFFECT & pEffect, const _int & iIndex)
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

	pEffect->CommitChanges();

	return S_OK;
}

void CBigPlug::Move(const _float & fTimeDelta)
{
	if (!m_bButton_Click)
		return;

	if (m_bEnd)
		return;
	m_fMoveTime += fTimeDelta;
	//if (0.9f < m_fMoveTime)
	//{
	//	m_bActive = false;
	//	m_bButton_Click = false;
	//	m_pRigidActor->is<PxRigidDynamic>()->setKinematicTarget(PxTransform(m_vStartPos));
	//	m_pTransformCom->Set_Pos(PxTransform(m_vStartPos));

	//	return;
	//}
	if (m_fMoveTime < 0.3f)
	{

		m_pTransformCom->Move_Pos(&_vec3{ 0.f,-1.f,0.f }, 3, fTimeDelta);
		_vec3 vPos = m_pTransformCom->Get_Position();
		m_pRigidActor->is<PxRigidDynamic>()->setKinematicTarget(PxTransform(PxVec3(vPos.x, vPos.y, vPos.z)));
	}
	else
	{
		CLineObject* pLine = (CLineObject*)Engine::Get_GameObject(L"GameObject", L"CordLine");
		pLine->On_Active(nullptr);

		m_bEnd = true;
		return;
	}

	if (0.3f < m_fMoveTime)
	{
		m_bActive = false;
		m_bButton_Click = false;
		//m_pRigidActor->is<PxRigidDynamic>()->setKinematicTarget(PxTransform(m_vStartPos));
		//m_pTransformCom->Set_Pos(PxTransform(m_vStartPos));
		/*m_pTransformCom->Move_Pos(&_vec3{ 0.f,1.f,0.f }, 2, fTimeDelta);
		_vec3 vPos = m_pTransformCom->Get_Position();
		m_pRigidActor->is<PxRigidDynamic>()->setKinematicTarget(PxTransform(PxVec3(vPos.x, vPos.y, vPos.z)));*/
		return;
	}
}

void * CBigPlug::On_Active(void * pCaller)
{
	if (m_bActive)
		return nullptr;
	m_fMoveTime = 0.f;
	m_bActive = true;
	m_bButton_Click = true;

	return nullptr;
}
