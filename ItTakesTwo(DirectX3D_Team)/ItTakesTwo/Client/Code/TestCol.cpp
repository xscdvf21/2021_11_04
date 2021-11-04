#include "stdafx.h"
#include "TestCol.h"
#include "..\Engine\Resources\Code\PhysXMesh.h"
#include "Export_Function.h"



CTestCol::CTestCol(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
	, m_bOgreCol(false)
{
}

CTestCol::~CTestCol()
{
}

HRESULT CTestCol::Ready_Object(void * pArg)
{


	FAILED_CHECK_RETURN(Add_Component(pArg), E_FAIL);


	OBJINFO Temp;
	if (pArg)
	{
		memcpy(&Temp, pArg, sizeof(OBJINFO));
	}

	m_eRender = 3;

	m_iPass = 0;

	return S_OK;
}

_int CTestCol::Update_Object(const _float & TimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;


	Engine::CGameObject::Update_Object(TimeDelta);

	if (m_eRender == Engine::RENDER_NONALPHA)
		Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);
	else if (m_eRender == Engine::RENDER_ALPHA)
		Engine::Add_RenderGroup(Engine::RENDER_ALPHA, this);


	return OBJ_NOEVENT;
}

_int CTestCol::LateUpdate_Object(const _float & fTimeDelta)
{
	m_pTransformCom->Set_Pos(m_pRigidActor->getGlobalPose());
	return _int();
}

void CTestCol::Render_Object(const _int & iIndex)
{
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect), );

	_uint	iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);		// 1인자 : 현재 쉐이더 파일이 갖고 있는 pass의 최대 개수, 2인자 : 시작하는 방식(default)
	pEffect->BeginPass(m_iPass);

	//m_pCubeCom->Render_Buffer();
	//m_pStaticMeshCom->Render_Meshes(pEffect);

	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);
}

HRESULT CTestCol::Add_Component(void * pArg)
{
	Engine::CComponent*		pComponent = nullptr;
	
	OBJINFO Temp;
	if (pArg)
	{
		memcpy(&Temp, pArg, sizeof(OBJINFO));
	}
	m_eMesh = Temp.eMeshType;


	// CStaticMesh
	//pComponent = m_pStaticMeshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(0, L"TestCol"));
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Mesh", pComponent);

	pComponent = m_pCubeCom = dynamic_cast<Engine::CCubeTex*>(Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_CubeTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Cube", pComponent);

	// Transform
	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	// Shader
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_StaticObject"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);

	m_pTransformCom->Set_Scale(1.f, 1.f, 1.f);
	m_pTransformCom->Set_Pos(Temp.vPos);


	auto* pPhysics = Engine::Get_Physics();
	_vec3 vPos = m_pTransformCom->Get_Position();

	PxTransform tTransform(vPos.x , vPos.y, vPos.z);

	PxMaterial* pMaterial = pPhysics->createMaterial(0.5f, 0.5f, 0.0f);

	PxShape* pTrigger = pPhysics->createShape(PxBoxGeometry(2, 2, 2), *pMaterial, true);

	pTrigger->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
	pTrigger->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
	pTrigger->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, true);;

	
	m_pRigidActor = PxCreateDynamic(*pPhysics, tTransform, *pTrigger, 1);

	auto* pBody = static_cast<PxRigidDynamic*>(m_pRigidActor);
	
	pBody->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, false);
	// 그래비티를 끈다.
	pBody->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);
	// 질량설정
	pBody->setMass(0);
	//이동 제동력
	pBody->setName((char*)this);

	PxTransform Transform = pBody->getGlobalPose();
	
	PxQuat pxQ = Transform.q;

	PxTransform TriggerTransform(0, 0, 0);

	pTrigger->setLocalPose(TriggerTransform);



	return S_OK;
}

HRESULT CTestCol::SetUp_Material(void)
{
	D3DMATERIAL9			tMtrlInfo;
	ZeroMemory(&tMtrlInfo, sizeof(tMtrlInfo));

	tMtrlInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrlInfo.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f);
	tMtrlInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrlInfo.Emissive = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);
	tMtrlInfo.Power = 0.f;

	m_pGraphicDev->SetMaterial(&tMtrlInfo);

	return S_OK;
}

HRESULT CTestCol::SetUp_ConstantTable(LPD3DXEFFECT & pEffect)
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

CTestCol * CTestCol::Create(LPDIRECT3DDEVICE9 pGraphicDev, void * pArg)
{
	CTestCol*	pInstance = new CTestCol(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CTestCol::Free(void)
{
	Engine::CGameObject::Free();
}
