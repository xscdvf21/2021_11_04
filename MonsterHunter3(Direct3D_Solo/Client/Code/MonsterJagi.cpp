#include "stdafx.h"
#include "MonsterJagi.h"

#include "Export_Fucntion.h"

CMonsterJagi::CMonsterJagi(LPDIRECT3DDEVICE9 pGraphicDev)
	:Engine::CGameObject(pGraphicDev)
{

}

CMonsterJagi::~CMonsterJagi()
{
}

HRESULT CMonsterJagi::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pMeshCom->Set_Animationset(1);

	m_pTransformCom->Set_Rotation(EAxis::X, D3DXToRadian(-90.f));
	m_pTransformCom->m_vScale = _vec3(0.05f, 0.05f, 0.05f);
	m_pTransformCom->Set_Pos(_vec3(2.f, 1.f, 5.f));

	m_pNaviMeshCom->Set_CellIndex(0);

	return S_OK;
}

_int CMonsterJagi::Update_Object(const _float & fTimeDelta)
{
	
	Engine::CGameObject::Update_Object(fTimeDelta);

	m_pMeshCom->Play_Animationset(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_NONALPHA, this);


	return 0;
}

void CMonsterJagi::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	m_pMeshCom->Render_Meshes();
}

HRESULT CMonsterJagi::Add_Component(void)
{
	Engine::CComponent*			pComponent = nullptr;

	//다이나믹 메쉬
	pComponent = m_pMeshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Clone_Resource(RESOURCE_STAGE, L"Mesh_MonsterJagi"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Mesh", pComponent);

	//네비 메쉬
	pComponent = m_pNaviMeshCom = dynamic_cast<Engine::CNaviMesh*>(Engine::Clone_Resource(RESOURCE_STAGE, L"Mesh_Navi"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Navi", pComponent);

	//트랜스 폼
	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	//랜더러
	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Renderer", pComponent);

	//칼큘레이터
	pComponent = m_pCalculatorCom = dynamic_cast<Engine::CCalculator*>(Engine::Clone_Prototype(L"Proto_Calculator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Calculator", pComponent);

	//콜라이더

	return S_OK;
}

void CMonsterJagi::SetUp_OnTerrain(void)
{
}

_vec3 CMonsterJagi::PickUp_OnTerrain(void)
{
	return _vec3();
}

CMonsterJagi * CMonsterJagi::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMonsterJagi*		pInstance = new CMonsterJagi(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);


	return pInstance;
}

void CMonsterJagi::Free(void)
{
	Engine::CGameObject::Free();
}
