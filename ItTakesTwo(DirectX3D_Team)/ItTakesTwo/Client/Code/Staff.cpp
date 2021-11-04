#include "stdafx.h"
#include "Staff.h"
#include "Mage.h"
#include "Export_Function.h"
CStaff::CStaff(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CStaff::~CStaff()
{
}

HRESULT CStaff::Ready_Object(void * pArg)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	if (nullptr != pArg)
	{

	}
	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(90.f));

	return S_OK;
}

_int CStaff::Update_Object(const _float & fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	Engine::CGameObject::Update_Object(fTimeDelta);

	if (nullptr == m_pParentBoneMatrix)
	{
		Engine::CDynamicMesh*		pToyMage = dynamic_cast<Engine::CDynamicMesh*>(Engine::Get_Component(L"GameObject", L"ToyMage", L"Com_Mesh", Engine::ID_STATIC));
		NULL_CHECK_RETURN(pToyMage, 0);

		const Engine::D3DXFRAME_DERIVED* pFrame = pToyMage->Get_FrameByName("RightHand");

		m_pParentBoneMatrix = &pFrame->CombinedTranformationMatrix;

		Engine::CTransform*		pMageTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(L"GameObject", L"ToyMage", L"Com_Transform", Engine::ID_DYNAMIC));
		NULL_CHECK_RETURN(pMageTransformCom, 0);


		m_pParentWorldMatrix = pMageTransformCom->Get_WorldMatrix();

	}

	m_pTransformCom->Set_ParentMatrix((*m_pParentBoneMatrix * *m_pParentWorldMatrix));


	Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);

	return OBJ_NOEVENT;
}

_int CStaff::LateUpdate_Object(const _float & fTimeDelta)
{
	return _int();
}

void CStaff::Render_Object(const _int & iIndex)
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

HRESULT CStaff::Add_Component()
{
	Engine::CComponent*		pComponent = nullptr;

	// CStaticMesh
	pComponent = m_pMeshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(RESOURCE_CF, L"ToyMage_Staff"));
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

	return S_OK;
}

HRESULT CStaff::SetUp_ConstantTable(LPD3DXEFFECT & pEffect, const _int & iIndex)
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

CStaff * CStaff::Create(LPDIRECT3DDEVICE9 pGraphicDev, void * pArg)
{
	CStaff*	pInstance = new CStaff(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pArg)))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStaff::Free(void)
{
	Engine::CGameObject::Free();
}
