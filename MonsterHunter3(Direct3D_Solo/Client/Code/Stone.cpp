#include "stdafx.h"
#include "Stone.h"

#include "Export_Fucntion.h"


CStone::CStone(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{

}

CStone::~CStone(void)
{

}


HRESULT CStone::Ready_Object(const _uint& iFlag)
{
	m_iFlag = iFlag;

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	//m_pTransformCom->Rotation(Engine::ROT_Y, D3DXToRadian(45.f));
	m_pTransformCom->Set_Scale(_vec3(0.02f, 0.01f, 0.02f));
	m_pTransformCom->Set_Pos(_vec3(50, 0.f, 60));


	return S_OK;
}

Engine::_int CStone::Update_Object(const _float& fTimeDelta)
{

	Engine::CGameObject::Update_Object(fTimeDelta);

	/*SetUp_OnTerrain();*/

	/*m_bColl = Collision_ToObject(L"GameLogic", L"Player");*/

	_vec3	vPos;
	m_pTransformCom->Get_Info(Engine::INFO_POS, &vPos);

	//m_bDraw = m_pOptimizationCom->IsIn_FrustumForObject(&vPos, 0.f);

	
	m_pRendererCom->Add_RenderGroup(Engine::RENDER_MAP, this);

	return 0;
}

void CStone::Render_Object(void)
{
	//if (false == m_bDraw)
	//	return;


	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());


	if (exMapSee)
	{
		m_pMeshCom->Render_Meshes();
	}
	_matrix		matWorld;

	/*if (0 == m_iFlag)
		m_pTransformCom->Get_WorldMatrix(&matWorld);
	else
		m_pTransformCom->Get_NRotWorldMatrix(&matWorld);*/

	//m_pTransformCom->Get_WorldMatrix(&matWorld);

	//m_pColliderCom->Render_Collider(Engine::COLTYPE(m_bColl), &matWorld);

}

CStone* CStone::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _uint& iFlag)
{
	CStone*	pInstance = new CStone(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(iFlag)))
		Safe_Release(pInstance);

	return pInstance;
}

void CStone::Free(void)
{
	Engine::CGameObject::Free();
}

HRESULT CStone::Add_Component(void)
{

	Engine::CComponent*		pComponent = nullptr;

	// StaticMesh
	pComponent = m_pMeshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(RESOURCE_STAGE, L"Map0_Ground"));
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


	// Optimization
	pComponent = m_pOptimizationCom = dynamic_cast<Engine::COptimization*>(Engine::Clone_Prototype(L"Proto_Optimization"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Optimization", pComponent);

	return S_OK;
}

void CStone::SetUp_OnTerrain(void)
{
	_vec3		vPos;
	m_pTransformCom->Get_Info(Engine::INFO_POS, &vPos);

	Engine::CTerrainTex*	pTerrainBufferCom = dynamic_cast<Engine::CTerrainTex*>
		(Engine::Get_Component(L"GameLogic", L"Terrain", L"Com_Buffer", Engine::ID_STATIC));
		
	_float fHeight = m_pCalculatorCom->Compute_HeightOnTerrain(&vPos,
																pTerrainBufferCom->Get_VtxPos(),
																pTerrainBufferCom->Get_VtxCntX(),
																pTerrainBufferCom->Get_VtxCntZ(), 1);

	m_pTransformCom->Set_Pos(vPos.x, fHeight,vPos.z);
}

Engine::_bool CStone::Collision_ToObject(const _tchar* pLayerTag, const _tchar* pObjTag)
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

