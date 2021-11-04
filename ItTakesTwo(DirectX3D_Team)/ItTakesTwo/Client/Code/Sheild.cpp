#include "stdafx.h"
#include "Sheild.h"
#include "Sheilder.h"
#include "Export_Function.h"
#include "MainCamera.h"
#include "CollisionMgr.h"


CSheild::CSheild(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEnermy(pGraphicDev)
{
}

CSheild::~CSheild()
{
}

HRESULT CSheild::Ready_Object(void * pArg)
{
	m_fColDis = 1.0f;

	// 콜라이더
	ZeroMemory(&m_tColInfo, sizeof(SPHERE_INFO));
	m_tColInfo.fRadius = m_fColDis;

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	OBJINFO tTemp;
	ZeroMemory(&tTemp, sizeof(OBJINFO));

	if (nullptr != pArg)
	{
		memcpy(&tTemp, pArg, sizeof(OBJINFO));
	}
	lstrcpy(m_tParentBone, tTemp.tParentBone);
	//m_pTransformCom->Rotation(ROT_X, D3DXToRadian(90.f));
	m_pTransformCom->Rotation(ROT_Z, D3DXToRadian(90.f));
	return S_OK;
}

_int CSheild::Update_Object(const _float & fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	Engine::CGameObject::Update_Object(fTimeDelta);


	if (nullptr == m_pParentBoneMatrix)
	{
		Engine::CDynamicMesh*		pToySheilder = static_cast<Engine::CDynamicMesh*>(Engine::Get_Component(L"GameObject", m_tParentBone, L"Com_Mesh", Engine::ID_STATIC));
		NULL_CHECK_RETURN(pToySheilder, 0);

		const Engine::D3DXFRAME_DERIVED* pFrame = pToySheilder->Get_FrameByName("LeftHand");

		m_pParentBoneMatrix = &pFrame->CombinedTranformationMatrix;

		Engine::CTransform*		pToySheilderTransformCom = static_cast<Engine::CTransform*>(Engine::Get_Component(L"GameObject", m_tParentBone, L"Com_Transform", Engine::ID_DYNAMIC));
		NULL_CHECK_RETURN(pToySheilderTransformCom, 0);


		m_pParentWorldMatrix = pToySheilderTransformCom->Get_WorldMatrix();

	}

	if (m_bAttAble)
		CCollisionMgr::GetInstance()->Add_EnermyWeaponList(this);
	m_pTransformCom->Set_ParentMatrix((*m_pParentBoneMatrix * *m_pParentWorldMatrix));

	_matrix temp;
	//무기 디테일 위치.
	m_pTransformCom->Get_WorldMatrix(&temp);

	temp._43 = temp._43 + 0.1f;
	m_pTransformCom->Set_WorldMatrix(temp);

	Tick_Damage(fTimeDelta);
	Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);

	return OBJ_NOEVENT;
}

_int CSheild::LateUpdate_Object(const _float & fTimeDelta)
{
	_vec3 vPos = m_pTransformCom->Get_Position();
	PxTransform pxTransform = m_pRigidActor->getGlobalPose();
	pxTransform.p.x = vPos.x;
	pxTransform.p.y = vPos.y + 0.5f;
	pxTransform.p.z = vPos.z;
	m_pRigidActor->setGlobalPose(pxTransform);

	return _int();
}

void CSheild::Render_Object(const _int & iIndex)
{
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect, iIndex), );

	_uint	iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);		// 1인자 : 현재 쉐이더 파일이 갖고 있는 pass의 최대 개수, 2인자 : 시작하는 방식(default)
										//pEffect->BeginPass(0);
	m_pMeshCom->Render_Meshes(pEffect, 6);

	pEffect->End();

	Safe_Release(pEffect);

	CMainCamera* pCamera = (CMainCamera*)Engine::Get_GameObject(L"GameObject", L"MainCamera");
	pCamera->Update_View_Proj();

	//Spine2
	m_tColInfo.matWorld = *m_pTransformCom->Get_WorldMatrix();
	m_pColliderCom->Render_Collider(m_tColInfo.matWorld, false);
}

HRESULT CSheild::Add_Component()
{
	Engine::CComponent*		pComponent = nullptr;

	// CStaticMesh
	pComponent = m_pMeshCom = static_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(RESOURCE_CF, L"ToySheild"));
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

	auto* pPhysics = Engine::Get_Physics();
	_vec3 vPos = m_pTransformCom->Get_Position();
	PxTransform tTransform(vPos.x, vPos.y, vPos.z);

	PxMaterial* pMaterial = pPhysics->createMaterial(0.5f, 0.5f, 0.0f);

	PxShape* pShape = pPhysics->createShape(PxBoxGeometry(0.5f, 0.5f, 0.5f), *pMaterial, true);
	//밑 두개중 하나는 false 둘다 true안?
	pShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true);
	pShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, false);
	pShape->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, true);
	m_pRigidActor = PxCreateDynamic(*pPhysics, tTransform, *pShape, 1);

	auto* pBody = static_cast<PxRigidDynamic*>(m_pRigidActor);

	// 트랜스폼은 가지고있지만 움직이지 않는애들
	pBody->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, false);

	// 그래비티를 끈다.

	pBody->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, false);

	// 질량설정
	pBody->setMass(5);
	pBody->setName((char*)this);
	pShape->setName((char*)this);
	pBody->setRigidDynamicLockFlags(PxRigidDynamicLockFlag::eLOCK_ANGULAR_X | PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z);
	//이동 제동력
	pBody->setLinearDamping(0.05f);

	return S_OK;
}

HRESULT CSheild::SetUp_ConstantTable(LPD3DXEFFECT & pEffect, const _int & iIndex)
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

CSheild * CSheild::Create(LPDIRECT3DDEVICE9 pGraphicDev, void * pArg)
{
	CSheild*	pInstance = new CSheild(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pArg)))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSheild::Free(void)
{
	CEnermy::Free();
}
