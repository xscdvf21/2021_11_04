#include "stdafx.h"
#include "ChessBridge.h"

#include "Export_Function.h"




CChessBridge::CChessBridge(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
	, m_fMoveDir(0.f)
	, m_fMoveSpeed(5.f)
	, m_bMove(false)
{
}

CChessBridge::~CChessBridge()
{
}

HRESULT CChessBridge::Ready_Object(void * pArg)
{
	FAILED_CHECK_RETURN(Add_Component(pArg), E_FAIL);

	OBJINFO tTemp;
	ZeroMemory(&tTemp, sizeof(OBJINFO));

	if (nullptr != pArg)
	{
		memcpy(&tTemp, pArg, sizeof(OBJINFO));
	}
	m_pTransformCom->Set_Scale(tTemp.vScale);
	m_pTransformCom->Set_Pos(tTemp.vPos);

	return S_OK;
}

_int CChessBridge::Update_Object(const _float & fTimeDelta)
{

	Move(fTimeDelta);

	Engine::CGameObject::Update_Object(fTimeDelta);
	Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);

	return OBJ_NOEVENT;
}

_int CChessBridge::LateUpdate_Object(const _float & fTimeDelta)
{
	return _int();
}

void CChessBridge::Render_Object(const _int & iIndex)
{
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect, iIndex), );

	_uint	iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);		// 1���� : ���� ���̴� ������ ���� �ִ� pass�� �ִ� ����, 2���� : �����ϴ� ���(default)
										//pEffect->BeginPass(0);
	m_pMeshCom->Render_Meshes(pEffect, 8);

	pEffect->End();

	Safe_Release(pEffect);
}

HRESULT CChessBridge::Add_Component(void * pArg)
{
	Engine::CComponent*		pComponent = nullptr;

	OBJINFO tTemp;
	if (pArg)
	{
		memcpy(&tTemp, pArg, sizeof(OBJINFO));
	}


	pComponent = m_pMeshCom = static_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(tTemp.eSceneID, L"ChessBridge"));
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
	//m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(180.f));
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
	pBody->setName((char*)this);
	pBody->attachShape(*pShape);

	return S_OK;
}

HRESULT CChessBridge::SetUp_ConstantTable(LPD3DXEFFECT & pEffect, const _int & iIndex)
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

void CChessBridge::Move(const _float & fTimeDelta)
{
	_vec3 vPos;
	_vec3 vRight;
	m_pTransformCom->Get_Info(INFO_RIGHT, &vRight);
	D3DXVec3Normalize(&vRight, &vRight);

	vPos = m_pTransformCom->Get_Position();

	PxTransform pxTransform = m_pRigidActor->getGlobalPose();
	pxTransform.p.x = vPos.x;
	pxTransform.p.y = vPos.y;
	pxTransform.p.z = vPos.z;
	m_pRigidActor->setGlobalPose(pxTransform);

	if (m_bMove)
	{
		m_pTransformCom->Move_Pos(&vRight, -10.f, fTimeDelta);
		if (vPos.x <= 3.5f)
		{
			vPos.x = 3.5f;
			m_pTransformCom->Set_Pos(vPos);
			
		}
	}
	else if(!m_bMove)
	{
		m_pTransformCom->Move_Pos(&vRight, 10.f, fTimeDelta);
		if (vPos.x >= 16.f)
		{
			vPos.x = 16.f;
			m_pTransformCom->Set_Pos(vPos);
		}
	}
}

CChessBridge * CChessBridge::Create(LPDIRECT3DDEVICE9 pGraphicDev, void * pArg)
{
	CChessBridge*	pInstance = new CChessBridge(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pArg)))
	{
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CChessBridge::Free(void)
{
	Engine::CGameObject::Free();
}