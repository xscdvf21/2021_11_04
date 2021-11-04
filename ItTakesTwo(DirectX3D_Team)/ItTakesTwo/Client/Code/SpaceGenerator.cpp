#include "stdafx.h"
#include "SpaceGenerator.h"

#include "Export_Function.h"
CSpaceGenerator::CSpaceGenerator(LPDIRECT3DDEVICE9 pGraphicDev)
	:Engine::CGameObject(pGraphicDev)
{
}

CSpaceGenerator::~CSpaceGenerator()
{
}

HRESULT CSpaceGenerator::Ready_Object(void * pArg, _uint Index)
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

_int CSpaceGenerator::Update_Object(const _float & fTimeDelta)
{
	Engine::CGameObject::Update_Object(fTimeDelta);
	Engine::Add_RenderGroup(Engine::RENDER_ALPHA, this);

	return OBJ_NOEVENT;
}

_int CSpaceGenerator::LateUpdate_Object(const _float & fTimeDelta)
{
	return _int();
}

void CSpaceGenerator::Render_Object(const _int & iIndex)
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

HRESULT CSpaceGenerator::Add_Component(void * pArg)
{
	Engine::CComponent*		pComponent = nullptr;

	OBJINFO tTemp;
	if (pArg)
	{
		memcpy(&tTemp, pArg, sizeof(OBJINFO));
	}
	
	pComponent = m_pMeshCom = static_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(tTemp.eSceneID, tTemp.tagMesh));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Mesh", pComponent);

	// Transform
	pComponent = m_pTransformCom = static_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"m_pTransformCom", pComponent);

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

	//PxTransform pxTransform = m_pRigidActor->getGlobalPose();
	//pxTransform.p.x = vPos.x;
	//pxTransform.p.y = vPos.y;
	//pxTransform.p.z = vPos.z;
	//_vec3 vRot = m_pTransformCom->Get_Angle();
	//pxTransform.q = PxQuat(vRot.x, { 1.f,0.f,0.f })*PxQuat(vRot.y, { 0.f,1.f,0.f })*PxQuat(vRot.z, { 0.f,0.f,1.f });
	//m_pRigidActor->setGlobalPose(pxTransform);

	PxTransform tTransform(vPos.x, vPos.y, vPos.z, PxQuat(m_pTransformCom->Get_Angle().x, { 1.f,0.f,0.f })*PxQuat(m_pTransformCom->Get_Angle().y, { 0.f,1.f,0.f })*PxQuat(m_pTransformCom->Get_Angle().z, { 0.f,0.f,1.f }));
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
	pBody->setName((char*) this);

	return S_OK;
}

HRESULT CSpaceGenerator::SetUp_ConstantTable(LPD3DXEFFECT & pEffect, const _int & iIndex, const _int & iMeshNum)
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

CSpaceGenerator * CSpaceGenerator::Create(LPDIRECT3DDEVICE9 pGraphicDev, void * pArg, _uint Index)
{
	CSpaceGenerator*	pInstance = new CSpaceGenerator(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pArg)))
	{
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CSpaceGenerator::Free(void)
{
	Engine::CGameObject::Free();
}
