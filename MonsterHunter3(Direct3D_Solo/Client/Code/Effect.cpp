#include "stdafx.h"
#include "Effect.h"

#include "Export_Fucntion.h"


CEffect::CEffect(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{

}

CEffect::~CEffect(void)
{

}


HRESULT CEffect::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Set_Pos(rand() % 20, 0.f, rand() % 20);

	return S_OK;
}

HRESULT CEffect::Ready_Object(EFFECTID _Effectid, _float LifeTime)
{

	return S_OK;
}

Engine::_int CEffect::Update_Object(const _float& fTimeDelta)
{

	m_fFrame += 90.f * fTimeDelta;

	if (90.f < m_fFrame)
		m_fFrame = 0.f;

	Engine::CGameObject::Update_Object(fTimeDelta);

	_matrix		matBill, matView, matWorld;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixIdentity(&matBill);

	matBill._11 = matView._11;
	matBill._13 = matView._13;
	matBill._31 = matView._31;
	matBill._33 = matView._33;

	D3DXMatrixInverse(&matBill, NULL, &matBill);
	m_pTransformCom->Get_WorldMatrix(&matWorld);

	m_pTransformCom->Set_WorldMatrix(&(matBill * matWorld));
	

	_vec3 vPos;

	m_pTransformCom->Get_Info(Engine::INFO_POS, &vPos);

	Compute_ViewZ(&vPos);

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_ALPHA, this);

	return 0;
}

void CEffect::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	m_pTextureCom->Set_Texture((_uint)m_fFrame);
	m_pBufferCom->Render_Buffer();

}

CEffect * CEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev, EFFECTID _Effectid, _float LifeTime)
{
	CEffect*	pInstance = new CEffect(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(_Effectid, LifeTime)))
		Safe_Release(pInstance);

	return pInstance;
}

void CEffect::Free(void)
{
	Engine::CGameObject::Free();
}

HRESULT CEffect::Add_Component(void)
{

	Engine::CComponent*		pComponent = nullptr;

	// Buffer
	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Buffer", pComponent);


	// Texture
	pComponent = m_pTextureCom[EFFECT_AURA] = dynamic_cast<Engine::CTexture*>(Engine::Clone_Resource(RESOURCE_STAGE, L"Proto_Texture_Effect"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture", pComponent);

	// Renderer
	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Renderer", pComponent);

	// Transform
	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);
	
	return S_OK;
}

