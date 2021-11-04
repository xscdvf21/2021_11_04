#include "stdafx.h"
#include "FireSwordEffect.h"
#include "Export_Function.h"


CFireSwordEffect::CFireSwordEffect(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{

}

CFireSwordEffect::~CFireSwordEffect(void)
{

}

HRESULT CFireSwordEffect::Ready_Object(void* pArg)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Set_Pos(_vec3(0.f, 5.f, 0.f));
	m_pTransformCom->Set_Scale(_vec3(5.f, 5.f, 5.f));

	return S_OK;
}

Engine::_int CFireSwordEffect::Update_Object(const _float& fTimeDelta)
{
	if (Engine::Key_Pressing(DIK_RIGHT))
	{
		m_pTransformCom->Set_AddAngleY(D3DXToRadian(3.f));
	}
	if (Engine::Key_Pressing(DIK_LEFT))
	{
		m_pTransformCom->Set_AddAngleY(D3DXToRadian(-3.f));
	}


	Engine::CGameObject::Update_Object(fTimeDelta);

	Engine::Add_RenderGroup(Engine::RENDER_ALPHA, this);

	return 0;
}

_int CFireSwordEffect::LateUpdate_Object(const _float & TimeDelta)
{
	return _int();
}

void CFireSwordEffect::Render_Object(const _int& iIndex)
{
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect), );

	_uint	iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);	
	pEffect->BeginPass(3);

	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);
}

CFireSwordEffect* CFireSwordEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CFireSwordEffect*	pInstance = new CFireSwordEffect(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CFireSwordEffect::Free(void)
{
	Engine::CGameObject::Free();
}

HRESULT CFireSwordEffect::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	// Buffer
	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Buffer", pComponent);

	// Texture
	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone_Resource(RESOURCE_STATIC, L"SwordEffect"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture", pComponent);

	// Transform
	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	// Shader
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_All"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);

	return S_OK;
}

HRESULT CFireSwordEffect::SetUp_ConstantTable(LPD3DXEFFECT & pEffect)
{
	_matrix			matWorld, matView, matProj;

	m_pTransformCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	m_pTextureCom->Set_Texture(pEffect, "g_Add3Texture", 0);
	m_pTextureCom->Set_Texture(pEffect, "g_Add1Texture", 2);
	m_pTextureCom->Set_Texture(pEffect, "g_Add2Texture", 3);

	return S_OK;
}

