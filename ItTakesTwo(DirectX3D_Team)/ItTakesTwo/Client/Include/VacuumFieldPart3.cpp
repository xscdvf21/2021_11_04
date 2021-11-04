#include "stdafx.h"
#include "VacuumFieldPart3.h"
#include "VacuumFieldPart4.h"
#include "Export_Function.h"
#include "Cody.h"
#include "May.h"


CVacuumFieldPart3::CVacuumFieldPart3(LPDIRECT3DDEVICE9 pGraphicDev)
	:Engine::CGameObject(pGraphicDev)
{
}

CVacuumFieldPart3::~CVacuumFieldPart3()
{
}

HRESULT CVacuumFieldPart3::Ready_Object(void* pArg)
{
	FAILED_CHECK_RETURN(Add_Component(pArg), E_FAIL);



	return S_OK;
}

_int CVacuumFieldPart3::Update_Object(const _float & fTimeDelta)
{
	Engine::CGameObject::Update_Object(fTimeDelta);

	if(m_bMoveUp)
		MoveUp(fTimeDelta);
	
	if(m_bMoveDown)
		MoveDown(fTimeDelta);
	Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);

	return OBJ_NOEVENT;
}

_int CVacuumFieldPart3::LateUpdate_Object(const _float & fTimeDelta)
{
	//Engine::Get_GameObject(L"GameObject", L"Cody")->Is<CCody>()->Get_RigidActor()->is<PxRigidDynamic>()->wakeUp();
	//Engine::Get_GameObject(L"GameObject", L"May")->Is<CMay>()->Get_RigidActor()->is<PxRigidDynamic>()->wakeUp();
	_vec3 vPos;
	vPos = m_pTransformCom->Get_Position();
	PxQuat cQuat = PxQuat(m_pTransformCom->Get_Angle().x, { 1.f,0.f,0.f })*PxQuat(m_pTransformCom->Get_Angle().y, { 0.f,1.f,0.f })*PxQuat(m_pTransformCom->Get_Angle().z, { 0.f,0.f,1.f });

	m_pRigidActor->is<PxRigidDynamic>()->setKinematicTarget({ vPos.x,vPos.y,vPos.z,cQuat });
	//m_pRigidActor->setGlobalPose({ vPos.x,vPos.y,vPos.z });
	return _int();
}

void CVacuumFieldPart3::Render_Object(const _int & iIndex)
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

CVacuumFieldPart3 * CVacuumFieldPart3::Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg)
{
	CVacuumFieldPart3*	pInstance = new CVacuumFieldPart3(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pArg)))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVacuumFieldPart3::Free(void)
{
	Engine::CGameObject::Free();
}

HRESULT CVacuumFieldPart3::Add_Component(void* pArg)
{
	Engine::CComponent*		pComponent = nullptr;
	OBJINFO tTemp;
	if (pArg)
	{
		memcpy(&tTemp, pArg, sizeof(OBJINFO));
	}
	// CDy
	// CStaticMesh
	pComponent = m_pMeshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(tTemp.eSceneID, L"VacuumPlatform_Part03"));
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
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_All"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);

	m_pTransformCom->Set_Scale(_vec3(0.01f, 0.01f, 0.01f));
	m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(180));
	m_pTransformCom->Set_Pos(tTemp.vPos);

	auto* pPhys = Engine::Get_Physics();
	PxQuat cQuat = PxQuat(m_pTransformCom->Get_Angle().x, { 1.f,0.f,0.f })*PxQuat(m_pTransformCom->Get_Angle().y, { 0.f,1.f,0.f })*PxQuat(m_pTransformCom->Get_Angle().z, { 0.f,0.f,1.f });
	PxRigidDynamic* pBody = pPhys->createRigidDynamic(PxTransform(tTemp.vPos.x, tTemp.vPos.y, tTemp.vPos.z, cQuat));
	m_pRigidActor = pBody;
	pBody->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
	PxTriangleMeshGeometry tGeo{};
	tGeo.triangleMesh = m_pMeshCom->Get_PxMesh();
	tGeo.scale = PxMeshScale(0.01f);

	PxShape* pShape = pPhys->createShape(tGeo, *pPhys->createMaterial(0.5f, 0.5f, 0.f), true);
	pShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true);
	pShape->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, true);
	m_pRigidBody = pShape;
	m_pRigidActor->attachShape(*pShape);
	return S_OK;
}

HRESULT CVacuumFieldPart3::SetUp_ConstantTable(LPD3DXEFFECT & pEffect, const _int & iIndex)
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

void CVacuumFieldPart3::MoveUp(const _float & fTimeDelta)
{
	_vec3 vPos;
	_vec3 vUp;
	vPos = m_pTransformCom->Get_Position();
	m_pTransformCom->Get_Info(INFO_UP, &vUp);
	D3DXVec3Normalize(&vUp, &vUp);


	if (vPos.y < 8.7f)
	{
		m_pTransformCom->Move_Pos(&vUp, 2.f, fTimeDelta);

	}
	else
	{
		m_bMoveUp = false;
	}

}

void CVacuumFieldPart3::MoveDown(const _float & fTimeDelta)
{
	_vec3 vPos;
	_vec3 vUp;
	vPos = m_pTransformCom->Get_Position();
	m_pTransformCom->Get_Info(INFO_UP, &vUp);
	D3DXVec3Normalize(&vUp, &vUp);

	if (vPos.y > 0.f)
	{
		m_pTransformCom->Move_Pos(&vUp, -2.f, fTimeDelta);
	}
	else
	{
		m_bMoveDown = false;
	}
}
