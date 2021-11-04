#include "Component_Manager.h"

USING(Engine)
IMPLEMENT_SINGLETON(CComponent_Manager)

CComponent_Manager::CComponent_Manager()
{
}

HRESULT CComponent_Manager::Ready_Component_Manager(_uint iSceneCount)
{
	
	if (0 == iSceneCount)
		return E_FAIL;

	//¾À °¹¼ö ¸¸Å­ ÄÄÆ÷³ÍÆ®¸¦ º¸°üÇÒ ¸Ê µ¿ÀûÇÒ´ç.
	m_pComponents = new COMPONENTS[iSceneCount];

	//¸â¹ö º¯¼ö·Î ¾À °¹¼ö º¸°ü
	m_iSceneCount = iSceneCount;

	return S_OK;
}

HRESULT CComponent_Manager::Add_Component_Prototype(
	_uint iSceneIndex, 
	const wstring & PrototypeTag, 
	CComponent * pPrototype)
{
	if (m_iSceneCount <= iSceneIndex ||
		nullptr == m_pComponents ||
		nullptr == pPrototype)
		return E_FAIL;

	auto iter_find = m_pComponents[iSceneIndex].find(PrototypeTag);
	if (m_pComponents[iSceneIndex].end() == iter_find)
	{
		m_pComponents[iSceneIndex].insert(make_pair(PrototypeTag, pPrototype));
	}
	else
	{
		Safe_Release(pPrototype);
	}

	return S_OK;
}

CComponent * CComponent_Manager::Clone_Component(
	_uint iSceneIndex, 
	const wstring & PrototypeTag, 
	void * pArg)
{
	if (m_iSceneCount <= iSceneIndex ||
		nullptr == m_pComponents)
		return nullptr;

	auto iter_find = m_pComponents[iSceneIndex].find(PrototypeTag);
	if (m_pComponents[iSceneIndex].end() == iter_find)
		return nullptr;

	return iter_find->second->Clone(pArg);
}

void CComponent_Manager::Clear_ForScene(_uint iSceneIndex)
{
	if (m_iSceneCount <= iSceneIndex)
		return;

	for (auto& Pair : m_pComponents[iSceneIndex])
	{
		Safe_Release(Pair.second);
	}
}

void CComponent_Manager::Free()
{
	for (_uint i = 0; i < m_iSceneCount; ++i)
	{
		for (auto& Pair : m_pComponents[i])
		{
			Safe_Release(Pair.second);
		}
	}

	Safe_Delete_Array(m_pComponents);
}
