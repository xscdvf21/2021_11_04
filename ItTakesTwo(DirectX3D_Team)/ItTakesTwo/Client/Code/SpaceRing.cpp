#include "stdafx.h"
#include "SpaceRing.h"
#include "Export_Function.h"



CSpaceRing::CSpaceRing(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CSpaceRing::~CSpaceRing()
{
}

HRESULT CSpaceRing::Ready_Object(void * pArg)
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

_int CSpaceRing::Update_Object(const _float & fTimeDelta)
{
	if (m_bStart)
	{
		m_bStart = false;
	}
	
	switch (m_eRot)
	{
	case CSpaceRing::RotX:
			m_pTransformCom->Set_AddAngleX(D3DXToRadian(m_fRotSpeed) * fTimeDelta);
		break;
	case CSpaceRing::RotY:
			m_pTransformCom->Set_AddAngleY(D3DXToRadian(m_fRotSpeed) * fTimeDelta);
		break;
	case CSpaceRing::RotZ:
			m_pTransformCom->Set_AddAngleZ(D3DXToRadian(m_fRotSpeed) * fTimeDelta);
		break;
	}
	

	Engine::CGameObject::Update_Object(fTimeDelta);
	Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);

	return OBJ_NOEVENT;
}

_int CSpaceRing::LateUpdate_Object(const _float & fTimeDelta)
{
	return _int();
}

void CSpaceRing::Render_Object(const _int & iIndex)
{
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect, iIndex), );

	_uint	iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);			//	1���� : ���� ���̴� ������ ���� �ִ� pass�� �ִ� ����, 2���� : �����ϴ� ���(default)
	m_pMeshCom->Render_Meshes(pEffect, 8);	//	pEffect->BeginPass(0);
											//	����ƽ�Ž�8
											//	���̳���6


	pEffect->End();

	Safe_Release(pEffect);
}

HRESULT CSpaceRing::Add_Component(void * pArg)
{
	Engine::CComponent*		pComponent = nullptr;

	OBJINFO tTemp;
	if (pArg)
	{
		memcpy(&tTemp, pArg, sizeof(OBJINFO));
	}
	


	pComponent = m_pMeshCom = static_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(tTemp.eSceneID, L"SpaceRing"));
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

	// Ʈ�������� ������������ �������� �ʴ¾ֵ�
	pBody->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
	//pBody->setKinematicTarget()
	// �׷���Ƽ�� ����.
	pBody->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);
	// ��������
	pBody->setMass(0);
	//�̵� ������
	pBody->setName((char*)this);



	return S_OK;
}

HRESULT CSpaceRing::SetUp_ConstantTable(LPD3DXEFFECT & pEffect, const _int & iIndex)
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

CSpaceRing * CSpaceRing::Create(LPDIRECT3DDEVICE9 pGraphicDev, void * pArg)
{
	CSpaceRing*	pInstance = new CSpaceRing(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pArg)))
	{
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CSpaceRing::Free(void)
{
	Engine::CGameObject::Free();
}
