#include "stdafx.h"
#include "Sword.h"

#include "Export_Fucntion.h"


CSword::CSword(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{

}

CSword::~CSword(void)
{

}


HRESULT CSword::Ready_Object(const _uint& iFlag)
{
	m_iFlag = iFlag;

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Rotation(Engine::ROT_X, D3DXToRadian(90.f));


	return S_OK;
}

Engine::_int CSword::Update_Object(const _float& fTimeDelta)
{

	if (nullptr == m_pParentBoneMatrix)
	{
		Engine::CDynamicMesh*		pPlayerMeshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Get_Component(L"GameLogic", L"Player", L"Com_Mesh", Engine::ID_STATIC));
		NULL_CHECK_RETURN(pPlayerMeshCom, 0);

		const Engine::D3DXFRAME_DERIVED* pFrame = pPlayerMeshCom->Get_FrameByName("R_Hand");

		m_pParentBoneMatrix = &pFrame->CombinedTranformationMatrix;

		Engine::CTransform*		pPlayerTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(L"GameLogic", L"Player", L"Com_Transform", Engine::ID_DYNAMIC));
		NULL_CHECK_RETURN(pPlayerTransformCom, 0);
		
		m_pParentWorldMatrix = pPlayerTransformCom->Get_WorldMatrix();

	}

	Engine::CGameObject::Update_Object(fTimeDelta);

	m_pTransformCom->Set_ParentMatrix(&(*m_pParentBoneMatrix * *m_pParentWorldMatrix));


	//m_bColl = Collision_ToObject(L"GameLogic", L"Player");
	
	m_pRendererCom->Add_RenderGroup(Engine::RENDER_NONALPHA, this);

	return 0;
}

void CSword::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	m_pMeshCom->Render_Meshes();

	_matrix		matWorld;

	if (0 == m_iFlag)
		m_pTransformCom->Get_WorldMatrix(&matWorld);
	else
		m_pTransformCom->Get_NRotWorldMatrix(&matWorld);

	//m_pTransformCom->Get_WorldMatrix(&matWorld);

	//m_pColliderCom->Render_Collider(Engine::COLTYPE(m_bColl), &matWorld);

}

CSword* CSword::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _uint& iFlag)
{
	CSword*	pInstance = new CSword(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(iFlag)))
		Safe_Release(pInstance);

	return pInstance;
}

void CSword::Free(void)
{
	Engine::CGameObject::Free();
}

HRESULT CSword::Add_Component(void)
{

	Engine::CComponent*		pComponent = nullptr;

	// StaticMesh
	pComponent = m_pMeshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(RESOURCE_STAGE, L"Mesh_Sword"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Mesh", pComponent);
	
	// Transform
	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	// Renderer
	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Renderer", pComponent);

	// Calculator
	pComponent = m_pCalculatorCom = dynamic_cast<Engine::CCalculator*>(Engine::Clone_Prototype(L"Proto_Calculator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Calculator", pComponent);

	// Collider
	pComponent = m_pColliderCom = Engine::CCollider::Create(m_pGraphicDev,
		m_pMeshCom->Get_VtxPos(),
		m_pMeshCom->Get_VtxCnt(),
		m_pMeshCom->Get_Stride());
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Collider", pComponent);

	return S_OK;
}

Engine::_bool CSword::Collision_ToObject(const _tchar* pLayerTag, const _tchar* pObjTag)
{
	Engine::CCollider*		pPlayerColliderCom = dynamic_cast<Engine::CCollider*>(Engine::Get_Component(pLayerTag, pObjTag, L"Com_Collider", Engine::ID_STATIC));
	NULL_CHECK_RETURN(pPlayerColliderCom, false);

	if (0 == m_iFlag)
	{
		return m_pCalculatorCom->Collision_AABB(pPlayerColliderCom->Get_Min(), pPlayerColliderCom->Get_Max(), pPlayerColliderCom->Get_ColMatrix(),
			m_pColliderCom->Get_Min(), m_pColliderCom->Get_Max(), m_pColliderCom->Get_ColMatrix());
	}

	else
		return m_pCalculatorCom->Collision_OBB(pPlayerColliderCom->Get_Min(), pPlayerColliderCom->Get_Max(), pPlayerColliderCom->Get_ColMatrix(),
			m_pColliderCom->Get_Min(), m_pColliderCom->Get_Max(), m_pColliderCom->Get_ColMatrix());
	
}

