#include "stdafx.h"
#include "VacuumFieldPart.h"
#include "VacuumFieldPart4.h"

#include "Export_Function.h"
#include "Blow.h"


CVacuumFieldPart::CVacuumFieldPart(LPDIRECT3DDEVICE9 pGraphicDev)
	:Engine::CGameObject(pGraphicDev)
{
}

CVacuumFieldPart::~CVacuumFieldPart()
{
}

HRESULT CVacuumFieldPart::Ready_Object(void* pArg)
{
	FAILED_CHECK_RETURN(Add_Component(pArg), E_FAIL);
	//_vec3 vPos = m_pTransformCom->Get_Position();
	//PxTransform t = PxTransform(vPos.x, vPos.y, vPos.z);
	//_int iCount = m_pMeshCom->Get_FrameCount(0);
	//m_pHoze = Engine::Get_Physics()->createAggregate(iCount, false);

	////0.6 //1칸
	//createChain(t, iCount, 0.6f);
	////createChain(t, iCount,2.f);
	//auto* EndHoze = m_vecHozeRigid.back();

	//auto HozeEndPos = m_vecHozeRigid.back()->getGlobalPose();
	//EndHoze->setMaxLinearVelocity(10.f);
	//m_pTransformHeadCom->Set_Pos(HozeEndPos);
	//m_pTransformHeadCom->Set_Scale(_vec3(0.01f, 0.01f, 0.01f));

	return S_OK;
}

_int CVacuumFieldPart::Update_Object(const _float & fTimeDelta)
{


	Engine::CGameObject::Update_Object(fTimeDelta);

	if (m_bMoveUp)
		MoveUp(fTimeDelta);

	if (m_bMoveDown)
		MoveDown(fTimeDelta);

	Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);

	return OBJ_NOEVENT;
}

_int CVacuumFieldPart::LateUpdate_Object(const _float & fTimeDelta)
{
	_vec3 vPos;
	vPos = m_pTransformCom->Get_Position();
	PxQuat cQuat = PxQuat(m_pTransformCom->Get_Angle().x, { 1.f,0.f,0.f })*PxQuat(m_pTransformCom->Get_Angle().y, { 0.f,1.f,0.f })*PxQuat(m_pTransformCom->Get_Angle().z, { 0.f,0.f,1.f });
	m_pRigidActor->setGlobalPose({ vPos.x,vPos.y,vPos.z ,cQuat });
	return _int();
}

void CVacuumFieldPart::Render_Object(const _int & iIndex)
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

CVacuumFieldPart* CVacuumFieldPart::Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg)
{
	CVacuumFieldPart*	pInstance = new CVacuumFieldPart(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pArg)))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVacuumFieldPart::Free(void)
{
	Engine::CGameObject::Free();
}

HRESULT CVacuumFieldPart::Add_Component(void* pArg)
{
	Engine::CComponent*		pComponent = nullptr;
	OBJINFO tTemp;
	if (pArg)
	{
		memcpy(&tTemp, pArg, sizeof(OBJINFO));
	}
	pComponent = m_pMeshCom = static_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(tTemp.eSceneID, L"VacuumPlatform_Part01"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Mesh", pComponent);

	//// CDynamicMesh
	//pComponent = m_pMeshCom = static_cast<Engine::CDynamicMesh*>(Engine::Clone_Resource(tTemp.eSceneID, L"MiniVacuumHoze"));
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_mapComponent[Engine::ID_STATIC].emplace(L"Com_MeshHoze", pComponent);
	//// CDynamicMesh
	//pComponent = m_pMeshHead = static_cast<Engine::CDynamicMesh*>(Engine::Clone_Resource(tTemp.eSceneID, L"MiniVacuumHead"));
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_mapComponent[Engine::ID_STATIC].emplace(L"Com_MeshHead", pComponent);
	// Transform
	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	//pComponent = m_pTransformHeadCom = static_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_TransformHead", pComponent);
	
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
	PxTriangleMeshGeometry tGeo{};
	tGeo.triangleMesh = m_pMeshCom->Get_PxMesh();
	tGeo.scale = PxMeshScale(0.01f);

	PxShape* pShape = pPhys->createShape(tGeo, *pPhys->createMaterial(0.5f, 0.5f, 0.f), true);
	pBody->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
	pShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true);
	pShape->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, true);
	m_pRigidBody = pShape;
	m_pRigidActor->attachShape(*pShape);
	return S_OK;
}

HRESULT CVacuumFieldPart::SetUp_ConstantTable(LPD3DXEFFECT & pEffect, const _int & iIndex)
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

void CVacuumFieldPart::MoveUp(const _float & fTimeDelta)
{
	_vec3 vPos;
	_vec3 vUp;
	vPos = m_pTransformCom->Get_Position();
	m_pTransformCom->Get_Info(INFO_UP, &vUp);
	D3DXVec3Normalize(&vUp, &vUp);

	if (vPos.y < 5.5f)
	{
		m_pTransformCom->Move_Pos(&vUp, 2.f, fTimeDelta);
	}
	else
	{
		m_bMoveUp = false;
	}

}

void CVacuumFieldPart::MoveDown(const _float & fTimeDelta)
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

physx::PxJoint* CVacuumFieldPart::createDampedD6(PxRigidActor* a0, const PxTransform& t0, PxRigidActor* a1, const PxTransform& t1)
{
	PxFixedJoint* j = PxFixedJointCreate(*Engine::Get_Physics(), a0, t0, a1, t1);
	return j;
}

void CVacuumFieldPart::createChain(const PxTransform& t, PxU32 length, PxReal separation)
{
	PxVec3 offset;
	PxRigidDynamic* prev = NULL;
	PxBoxGeometry g;
	if (m_iDir == DIRUP || m_iDir == DIRDOWN)
	{
		if (m_iDir == DIRDOWN)
			separation *= -1;
		offset = PxVec3(0, 0, separation / 2);
		g = PxBoxGeometry(0.5f, 0.5f, 0.1f);
	}
	else
	{
		if (m_iDir == DIRLEFT)
			separation *= -1;
		offset = PxVec3(separation / 2, 0, 0);
		g = PxBoxGeometry(0.1f, 0.5f, 0.5f);
	}
	PxTransform localTm(offset);
	PxJoint* j = nullptr;
	for (PxU32 i = 0; i < length; i++)
	{
		PxRigidDynamic* current = PxCreateDynamic(*Engine::Get_Physics(), t*localTm, g, *Engine::Get_Physics()->createMaterial(0.5f, 0.5f, 0.f), 1.0f);
		m_pHoze->addActor(*current);

		current->setMass(0.1f);
		if (i == 0)
		{
			//헤드부분에 연결
			j = PxFixedJointCreate(*Engine::Get_Physics(), prev, t, current, PxTransform(-offset));

		}
		else if (i == length - 1)
		{
			j = createDampedD6(prev, prev ? PxTransform(offset) : t, current, PxTransform(-offset));
		}
		else
		{

			j = createDampedD6(prev, prev ? PxTransform(offset) : t, current, PxTransform(-offset));
		}
		m_vecHozeRigid.emplace_back(current);
		prev = current;
		switch (m_iDir)
		{
		case DIRUP:
			//if (i == length - 1)
			current->setRigidDynamicLockFlags(PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z);
			if (j->is<PxD6Joint>())
			{

				j->is<PxD6Joint>()->setMotion(PxD6Axis::eX, PxD6Motion::eLIMITED);
				j->is<PxD6Joint>()->setMotion(PxD6Axis::eY, PxD6Motion::eLIMITED);
			}
			localTm.p.z += separation;
			break;
		case DIRRIGHT:
			//if (i == length - 1)
			current->setRigidDynamicLockFlags(PxRigidDynamicLockFlag::eLOCK_ANGULAR_X);
			if (j->is<PxD6Joint>())

			{

				j->is<PxD6Joint>()->setMotion(PxD6Axis::eZ, PxD6Motion::eLIMITED);
				j->is<PxD6Joint>()->setMotion(PxD6Axis::eY, PxD6Motion::eLIMITED);
			}
			localTm.p.x += separation;
			break;
		case DIRLEFT:
			//if (i == length - 1)
			current->setRigidDynamicLockFlags(PxRigidDynamicLockFlag::eLOCK_ANGULAR_X);
			if (j->is<PxD6Joint>())

			{

				j->is<PxD6Joint>()->setMotion(PxD6Axis::eZ, PxD6Motion::eLIMITED);
				j->is<PxD6Joint>()->setMotion(PxD6Axis::eY, PxD6Motion::eLIMITED);
			}
			localTm.p.x += separation;
			break;
		case DIRDOWN:
			//if (i == length - 1)
			current->setRigidDynamicLockFlags(PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z);
			if (j->is<PxD6Joint>())
			{

				j->is<PxD6Joint>()->setMotion(PxD6Axis::eX, PxD6Motion::eLIMITED);
				j->is<PxD6Joint>()->setMotion(PxD6Axis::eY, PxD6Motion::eLIMITED);
			}
			localTm.p.z += separation;
			break;
		default:
			break;
		}
	}
}

