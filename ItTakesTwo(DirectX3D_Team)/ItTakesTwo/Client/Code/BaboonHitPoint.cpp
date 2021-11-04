#include "stdafx.h"
#include "BaboonHitPoint.h"

#include "Export_Function.h"
CBaboonHitPoint::CBaboonHitPoint(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
	, m_bUp(false), m_bDown(false), m_bMoveDir(false)
{
}

CBaboonHitPoint::~CBaboonHitPoint()
{
}

HRESULT CBaboonHitPoint::Ready_Object(void * pArg)
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

_int CBaboonHitPoint::Update_Object(const _float & fTimeDelta)
{
	Move(fTimeDelta);

	Engine::CGameObject::Update_Object(fTimeDelta);
	Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);

	return OBJ_NOEVENT;
}

_int CBaboonHitPoint::LateUpdate_Object(const _float & fTimeDelta)
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

	return _int();
}

void CBaboonHitPoint::Render_Object(const _int & iIndex)
{
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect, iIndex), );

	_uint	iPassMax = 0;

	pEffect->Begin(&iPassMax, 8);		// 1���� : ���� ���̴� ������ ���� �ִ� pass�� �ִ� ����, 2���� : �����ϴ� ���(default)
										//pEffect->BeginPass(0);
	m_pMeshCom->Render_Meshes(pEffect);

	pEffect->End();

	Safe_Release(pEffect);
}

HRESULT CBaboonHitPoint::Add_Component(void * pArg)
{
	Engine::CComponent*		pComponent = nullptr;

	OBJINFO tTemp;
	if (pArg)
	{
		memcpy(&tTemp, pArg, sizeof(OBJINFO));
	}


	pComponent = m_pMeshCom = static_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(tTemp.eSceneID, L"ChessOpenDoor"));
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

	pShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true);
	pShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, false);
	pShape->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, true);

	m_pRigidActor = PxCreateDynamic(*pPhysics, tTransform, *pShape, 1);

	auto* pBody = static_cast<PxRigidDynamic*>(m_pRigidActor);

	// Ʈ�������� ������������ �������� �ʴ¾ֵ�
	pBody->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
	//pBody->setKinematicTarget()
	// �׷���Ƽ�� ����.
	pBody->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);
	// ��������
	pBody->setMass(0);
	//�̵� ������
	pBody->setName("BaboonHitPoint");

	return S_OK;
}

HRESULT CBaboonHitPoint::SetUp_ConstantTable(LPD3DXEFFECT & pEffect, const _int & iIndex)
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

void CBaboonHitPoint::Move(const _float & fTimeDelta)
{
	Move_Up(fTimeDelta);
	Move_Down(fTimeDelta);
}


void CBaboonHitPoint::Move_Up(const _float & fTimeDelta)
{
	if (!m_bUp)
		return;

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

	if (fDir < 2.f)
	{
		m_pTransformCom->Move_Pos(&_vec3(0.f, 1.f, 0.f), 3.f, fTimeDelta);
	}
	else
	{
		m_bUp = false;
		m_bMoveDir = false;
	}
}

void CBaboonHitPoint::Move_Down(const _float & fTimeDelta)
{
	if (!m_bDown)
		return;

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

	if (fDir < 2.f)
	{
		m_pTransformCom->Move_Pos(&_vec3(0.f, -1.f, 0.f), 3.f, fTimeDelta);
	}
	else
	{
		m_bDown = false;
		m_bMoveDir = false;
	}

}



CBaboonHitPoint * CBaboonHitPoint::Create(LPDIRECT3DDEVICE9 pGraphicDev, void * pArg)
{
	CBaboonHitPoint*	pInstance = new CBaboonHitPoint(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pArg)))
	{
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBaboonHitPoint::Free(void)
{
	Engine::CGameObject::Free();
}
