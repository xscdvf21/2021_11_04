#include "stdafx.h"
#include "VacuumGenerator.h"
#include "Export_Function.h"
#include "MainCamera.h"
#include "PhysXMesh.h"


CVacuumGenerator::CVacuumGenerator(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{

}

CVacuumGenerator::~CVacuumGenerator(void)
{
}

HRESULT CVacuumGenerator::Ready_Object(void* pArg)
{
	FAILED_CHECK_RETURN(Add_Component(pArg), E_FAIL);

	OBJINFO Temp;
	if (pArg)
	{
		memcpy(&Temp, pArg, sizeof(OBJINFO));
	}
	m_tObjInfo = Temp;

	return S_OK;
}

Engine::_int CVacuumGenerator::Update_Object(const _float& fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	if (m_bStart)
	{
		m_pCodyTransform = Engine::Get_Component(L"GameObject", L"Cody", L"Com_Transform", ID_DYNAMIC)->Is<CTransform>();
		m_pMayTransform = Engine::Get_Component(L"GameObject", L"May", L"Com_Transform", ID_DYNAMIC)->Is<CTransform>();
		m_bStart = false;
	}

	_vec3 vPos = m_pTransformCom->Get_Position();
	vPos.y = 0.0f;
	_float3 vDis1 = m_pCodyTransform->Get_Position() - vPos;
	_float3 vDis2 = m_pMayTransform->Get_Position() - vPos;

	_float fDis1 = D3DXVec3Length(&vDis1);
	_float fDis2 = D3DXVec3Length(&vDis2);

	if (fDis2 <= 75.0f || fDis1 <= 75.0f)
	{
		CSoundMgr::GetInstance()->PlaySoundW(L"Fan3.wav", CSoundMgr::CHANNELID::Vaccum_Generator, g_fEffecctSound * (1 - fDis1 / 75.0f));
		CSoundMgr::GetInstance()->Set_Volum(g_fEffecctSound * (1 - fDis1 / 75.0f), CSoundMgr::CHANNELID::Vaccum_Generator);
	}

	Engine::CGameObject::Update_Object(fTimeDelta);
	Engine::Add_RenderGroup(Engine::RENDER_ALPHA, this);
	return OBJ_NOEVENT;
}

_int CVacuumGenerator::LateUpdate_Object(const _float & fTimeDelta)
{
	return _int();
}

void CVacuumGenerator::Render_Object(const _int& iIndex)
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

CVacuumGenerator* CVacuumGenerator::Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg)
{
	CVacuumGenerator*	pInstance = new CVacuumGenerator(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CVacuumGenerator::Free(void)
{
	Engine::CGameObject::Free();
}

HRESULT CVacuumGenerator::Add_Component(void* pArg)
{
	Engine::CComponent*		pComponent = nullptr;

	OBJINFO tTemp;
	ZeroMemory(&tTemp, sizeof(OBJINFO));
	if (nullptr != pArg)
	{
		memcpy(&tTemp, pArg, sizeof(OBJINFO));
	}

	// CStaticMesh
	pComponent = m_pMeshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(tTemp.eSceneID, L"VacuumGenerator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_StaticMesh", pComponent);

	// Transform
	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);
	m_pTransformCom->Set_Scale(_float3(0.01f, 0.01f, 0.01f));
	m_pTransformCom->Set_Pos(tTemp.vPos);
	m_pTransformCom->Set_AddAngleY(D3DXToRadian(90.0f));

	// Shader
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_All"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);


	auto* pPhysics = Engine::Get_Physics();
	_vec3 vPos = m_pTransformCom->Get_Position();


	PxTransform tTransform(vPos.x, vPos.y, vPos.z, PxQuat(m_pTransformCom->Get_Angle().y, { 0.f,1.f,0.f }));
	PxMaterial* pMaterial = pPhysics->createMaterial(0.5f, 0.5f, 0.0f);

	PxTriangleMeshGeometry pMeshGeo;
	pMeshGeo.triangleMesh = m_pMeshCom->Get_PxMesh();//m_pPhysMesh->Get_PxMesh();
	pMeshGeo.scale = PxMeshScale(PxVec3(0.01f, 0.01f, 0.01f));

	PxShape* pShape = pPhysics->createShape(pMeshGeo, *pMaterial, true);

	pShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
	pShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, false);
	pShape->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, true);
	pShape->setQueryFilterData(PxFilterData(FilterGroup::eGround, 0, 0, 0));
	m_pRigidActor = PxCreateDynamic(*pPhysics, tTransform, *pShape, 1);
	m_pRigidBody = pShape;
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

HRESULT CVacuumGenerator::SetUp_ConstantTable(LPD3DXEFFECT & pEffect, const _int & iIndex)
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
