#include "stdafx.h"
#include "TestMonster.h"

#include "Export_Fucntion.h"


CTestMonster::CTestMonster(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{

}

CTestMonster::~CTestMonster(void)
{

}


HRESULT CTestMonster::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

Engine::_int CTestMonster::Update_Object(const _float& fTimeDelta)
{
	Engine::CGameObject::Update_Object(fTimeDelta);

	Engine::CTransform*		pPlayerTransform = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(L"Environment", L"TestPlayer", L"Com_Transform", Engine::ID_DYNAMIC));
	NULL_CHECK_RETURN(pPlayerTransform, 0);

	_vec3	vPlayerPos;
	pPlayerTransform->Get_Info(Engine::INFO_POS, &vPlayerPos);

	m_pTransformCom->Chase_Target(&vPlayerPos, 1.f, fTimeDelta);


	return 0;
}

void CTestMonster::Render_Object(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
}

CTestMonster* CTestMonster::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTestMonster*	pInstance = new CTestMonster(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CTestMonster::Free(void)
{
	Engine::CGameObject::Free();
}

HRESULT CTestMonster::Add_Component(void)
{

	Engine::CComponent*		pComponent = nullptr;

	// Buffer
	pComponent = m_pBufferCom = dynamic_cast<Engine::CTriCol*>(Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_TriCol"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Buffer", pComponent);
	
	// Transform
	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Transform", pComponent);

	return S_OK;
}

