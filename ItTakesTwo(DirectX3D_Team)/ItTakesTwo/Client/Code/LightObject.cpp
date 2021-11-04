#include "stdafx.h"
#include "LightObject.h"
#include "Export_Function.h"
#include "Light.h"



CLightObject::CLightObject(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CLightObject::~CLightObject()
{
}

HRESULT CLightObject::Ready_Object(void * pArg)
{
	return S_OK;
}

HRESULT CLightObject::Ready_Object(const D3DLIGHT9* pLightInfo, const _uint& iIndex)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_tLightInfo = *pLightInfo;
	m_iIndex = iIndex;
	return S_OK;
}

_int CLightObject::Update_Object(const _float & fTimeDelta)
{
	return OBJ_NOEVENT;
}

_int CLightObject::LateUpdate_Object(const _float & fTimeDelta)
{
	return _int();
}

void CLightObject::Render_Object(const _int & iIndex)
{
	return;
}

void* CLightObject::On_Active(void* pCaller)
{
	Engine::Ready_Light(m_pGraphicDev, &m_tLightInfo, m_iIndex);
	return nullptr;
}

void* CLightObject::On_Trigger_Active(void* pTrigger, void* pCaller)
{
	Engine::Ready_Light(m_pGraphicDev, &m_tLightInfo, m_iIndex);

	return nullptr;
}

void* CLightObject::On_Attach(void* pCaller)
{
	Engine::Ready_Light(m_pGraphicDev, &m_tLightInfo, m_iIndex);
	return nullptr;

}

HRESULT CLightObject::Add_Component(void * pArg)
{
	return S_OK;
}

HRESULT CLightObject::SetUp_ConstantTable(LPD3DXEFFECT & pEffect, const _int & iIndex)
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

CLightObject* CLightObject::Create(LPDIRECT3DDEVICE9 pGraphicDev, const D3DLIGHT9* pLightInfo, const _uint& iIndex)
{
	CLightObject*	pInstance = new CLightObject(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pLightInfo, iIndex)))
	{
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLightObject::Free(void)
{
	Engine::CGameObject::Free();
}
