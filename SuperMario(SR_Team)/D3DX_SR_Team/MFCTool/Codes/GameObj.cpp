#include "stdafx.h"
#include "..\Headers\GameObj.h"


CGameObj::CGameObj(LPDIRECT3DDEVICE9 pDevice)
	:m_pDevice(pDevice)
{
	Safe_AddReference(m_pDevice);
}

CGameObj::CGameObj(const CGameObj & other)
	:m_pDevice(other.m_pDevice)
{
	Safe_AddReference(m_pDevice);
}

CComponentTool * CGameObj::Get_Component(const wstring & ComponentTag)
{
	return nullptr;
}

HRESULT CGameObj::Ready_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CGameObj::Ready_GameObject(void * pArg)
{
	return S_OK;
}

_uint CGameObj::Update_GameObject(_float fDeltaTime)
{
	return _uint();
}

_uint CGameObj::LateUpdate_GameObject(_float fDeltaTime)
{
	return _uint();
}

HRESULT CGameObj::Render_GameObject()
{
	return S_OK;
}

HRESULT CGameObj::Add_Component( const wstring & PrototypeTag, const wstring & ComponentTag, CComponentTool ** ppOut, void * pArg)
{
	CManagementTool* pManagement = CManagementTool::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	
	CComponentTool* pComponent = pManagement->Clone_Component(PrototypeTag, pArg);
	if (nullptr == pComponent)
		return E_FAIL;
	
	m_Components.insert(make_pair(ComponentTag, pComponent));

	if (ppOut)
	{
		*ppOut = pComponent;
		Safe_AddReference(pComponent);
	}

	return S_OK;
}

CGameObj * CGameObj::Clone(void * pArg)
{
	return nullptr;
}

void CGameObj::Free()
{
	for (auto& iter : m_Components)
	{
		Safe_Release(iter.second);
	}

	m_Components.clear();

	Safe_Release(m_pDevice);
}
