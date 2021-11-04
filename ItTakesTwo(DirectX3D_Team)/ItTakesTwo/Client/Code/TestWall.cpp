#include "stdafx.h"
#include "TestWall.h"
#include "Export_Function.h"
#include "MainCamera.h"
#include "..\Engine\Resources\Code\PhysXMesh.h"


CTestWall::CTestWall(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{

}

CTestWall::~CTestWall(void)
{
}

HRESULT CTestWall::Ready_Object(void* pArg)
{
	FAILED_CHECK_RETURN(Add_Component(pArg), E_FAIL);


	OBJINFO Temp;
	if (pArg)
	{
		memcpy(&Temp, pArg, sizeof(OBJINFO));
	}

	m_eRender = 1;

	m_iPass = 0;




	return S_OK;
}

Engine::_int CTestWall::Update_Object(const _float& fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	Engine::CGameObject::Update_Object(fTimeDelta);
	if (m_eRender == Engine::RENDER_NONALPHA)
		Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);
	else if (m_eRender == Engine::RENDER_ALPHA)
		Engine::Add_RenderGroup(Engine::RENDER_ALPHA, this);


	return OBJ_NOEVENT;
}

_int CTestWall::LateUpdate_Object(const _float & fTimeDelta)
{
	CMainCamera* pCamera = (CMainCamera*)Engine::Get_GameObject(L"GameObject", L"MainCamera");
	_vec3 vCampos = pCamera->Get_CameraPos();

	//m_fFog = fabs(D3DXVec3Length(&(vCampos - m_pTransformCom->Get_Position())));

	return _int();
}

void CTestWall::Render_Object(const _int& iIndex)
{
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect), );

	_uint	iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);		// 1인자 : 현재 쉐이더 파일이 갖고 있는 pass의 최대 개수, 2인자 : 시작하는 방식(default)
	pEffect->BeginPass(m_iPass);


	m_pStaticMeshCom->Render_Meshes(pEffect);

	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);
}

CTestWall* CTestWall::Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg)
{
	CTestWall*	pInstance = new CTestWall(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CTestWall::Free(void)
{
	Engine::CGameObject::Free();
}

HRESULT CTestWall::Add_Component(void* pArg)
{
	Engine::CComponent*		pComponent = nullptr;

	OBJINFO Temp;
	if (pArg)
	{
		memcpy(&Temp, pArg, sizeof(OBJINFO));
	}
	m_eMesh = Temp.eMeshType;


	// CStaticMesh
	pComponent = m_pStaticMeshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(Temp.eSceneID, L"TestWall"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Mesh", pComponent);

	// Transform
	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);
	m_pTransformCom->Set_Pos(Temp.vPos);
	m_pTransformCom->Set_Scale(0.01f, 0.01f, 0.01f);

	// Shader
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_StaticObject"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);

	//m_pTransformCom->Set_Pos()

	auto* pPhysics = Engine::Get_Physics();
	_vec3 vPos = m_pTransformCom->Get_Position();
	
	PxTransform tTransform(vPos.x + 10, vPos.y + 10, vPos.z);

	//정지마찰계수 멈춰있을경우 다른충돌체와 마찰하는정도
	//움직임마찰계수 움직일때 다른충돌체와 마찰정도
	//탄성계수 0~1값
	PxMaterial* pMaterial = pPhysics->createMaterial(0.5f, 0.5f, 0.0f);
	//PxTriangleMeshGeometry pMeshGeo;
	//pMeshGeo.triangleMesh = m_pStaticMeshCom->Get_PxMesh();

	PxShape* pShape = pPhysics->createShape(PxBoxGeometry(7, 10, 1), *pMaterial, true);

	//밑 두개중 하나는 false 둘다 true안?
	pShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true);
	pShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, false);
	pShape->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, true);
	m_pRigidActor = PxCreateDynamic(*pPhysics, tTransform, *pShape, 1);
	

	auto* pBody = static_cast<PxRigidDynamic*>(m_pRigidActor);

	// 트랜스폼은 가지고있지만 움직이지 않는애들
	pBody->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
	// 그래비티를 끈다.
	pBody->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);
	// 질량설정
	pBody->setMass(0);
	//이동 제동력
	pBody->setName((char*)this);
	PxTransform Transform = pBody->getGlobalPose();

	PxQuat pxQ = Transform.q;

	PxShape* pTrigger = pPhysics->createShape(PxBoxGeometry(7.f, 1.f, 1.2f), *pMaterial, true);
	pTrigger->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
	pTrigger->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
	pTrigger->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, true);;

	PxTransform TriggerTransform(0, 10, 0);
	pTrigger->setLocalPose(TriggerTransform);

	pBody->attachShape(*pTrigger);

	// 별로 필요없음
	//	pBody->setAngularDamping(0.f);

	// 무게의 로컬 포스를 바꿔줌(무게중심)
	//pBody->setCMassLocalPose(PxTransform(0.f, 0.5f, 0.f));
	//pBody->setMassSpaceInertiaTensor(PxVec3(0.f, 10.f, 0.f));


	//PxShape* pTrigger = pPhysics->createShape(PxBoxGeometry(7, 10, 2), *pMaterial);
	////밑 두개중 하나는 false 둘다 true안?
	//pTrigger->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true);
	//pTrigger->setFlag(PxShapeFlag::eTRIGGER_SHAPE, false);

	//pTrigger->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, false);
	//pBody->attachShape(*pTrigger);

	// pTrigger = pPhysics->createShape(PxBoxGeometry(10, 1, 10), *pMaterial);
	////밑 두개중 하나는 false 둘다 true안?
	//pTrigger->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
	//pTrigger->setFlag(PxShapeFlag::eTRIGGER_SHAPE, false);

	//pTrigger->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, false);
	//pBody->attachShape(*pTrigger);



	return S_OK;
}

HRESULT CTestWall::SetUp_Material(void)
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

HRESULT CTestWall::SetUp_ConstantTable(LPD3DXEFFECT & pEffect)
{
	_matrix			matWorld, matView, matProj, matInv;

	m_pTransformCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	pEffect->SetFloat("g_fPower", 0.f);
	pEffect->SetFloat("g_Fog", m_fFog);

	pEffect->SetFloat("g_fPower", 0.f);
	D3DXMatrixInverse(&matInv, nullptr, &matView);

	pEffect->SetVector("g_vCamPos", &_vec4(matInv._41, matInv._42, matInv._43, 1.f));

	Engine::SetUp_OnShader(pEffect, L"Target_Depth", "g_DepthTexture");

	//m_pTextureCom->Set_Texture(pEffect, "g_LightMaskTexture", 0);

	pEffect->CommitChanges();


	return S_OK;
}
