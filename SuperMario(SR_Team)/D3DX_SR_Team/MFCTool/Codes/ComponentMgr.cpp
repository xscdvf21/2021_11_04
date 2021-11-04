#include "stdafx.h"
#include "ComponentMgr.h"

IMPLEMENT_SINGLETON(CComponentMgr)
CComponentMgr::CComponentMgr()
{
}


HRESULT CComponentMgr::Ready_Component_Manager()
{
	m_pComponents = new COMPONENTS;
	return S_OK;
}

HRESULT CComponentMgr::Add_Component_Prototype( const wstring & PrototypeTag, CComponentTool * pPrototype)
{
	if (nullptr == m_pComponents || nullptr == pPrototype)
	{
		return E_FAIL;
	}

	//auto iter_find = m_pComponents[iSceneIndex].find(PrototypeTag);
	auto iter_find = m_pComponents->find(PrototypeTag);

	if (m_pComponents->end() == iter_find)
	{
		m_pComponents->insert(make_pair(PrototypeTag, pPrototype));
	}
	else
	{
		Safe_Release(pPrototype);
	}

	return S_OK;
}

CComponentTool * CComponentMgr::Clone_Component( const wstring & PrototypeTag, void * pArg)
{
	if (nullptr == m_pComponents)
	{
		return nullptr;
	}

	auto iter_find = m_pComponents->find(PrototypeTag);
	if (m_pComponents->end() == iter_find)
		return nullptr;

	return iter_find->second->Clone(pArg);
}

void CComponentMgr::Clear_ForScene(_uint iSceneIndex)
{
	if(nullptr == m_pComponents)
	{
		return;
	}

	for (auto& Pair : *m_pComponents)
	{
		Safe_Release(Pair.second);
	}
}

void CComponentMgr::Free()
{
	for (auto& Pair : *m_pComponents)
	{
		Safe_Release(Pair.second);
	}
	Safe_Delete_Array(m_pComponents);
}
