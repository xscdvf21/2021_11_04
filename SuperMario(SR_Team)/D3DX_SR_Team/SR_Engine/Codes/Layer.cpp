#include "Layer.h"
#include "GameObject.h"

USING(Engine)

CLayer::CLayer()
{
}

CGameObject * CLayer::Get_GameObject(_uint iIndex)
{
	auto iter_begin = m_GameObjects.begin();
	if (m_GameObjects.end() == iter_begin)
		return nullptr;

	for (_uint i = 0; i < iIndex; ++i)
	{
		++iter_begin;
	}

	return *iter_begin;
}

CComponent * CLayer::Get_Component(const wstring & ComponentTag, _uint iIndex)
{
	auto iter_begin = m_GameObjects.begin();
	if (m_GameObjects.end() == iter_begin)
		return nullptr;

	for (_uint i = 0; i < iIndex; ++i)
	{
		++iter_begin;
	}

	return (*iter_begin)->Get_Component(ComponentTag);
}

HRESULT CLayer::Add_GameObject(CGameObject * pGameObject)
{
	if (nullptr == pGameObject)
		return E_FAIL;

	/* find: <algorithm>에서 제공하는 함수. */
	auto iter = find(m_GameObjects.begin(), m_GameObjects.end(), pGameObject);
	if (m_GameObjects.end() == iter)
	{
		m_GameObjects.push_back(pGameObject);
	}
	else
	{
		Safe_Release(pGameObject);
	}

	return S_OK;
}

_uint CLayer::Update_Layer(_float fDeltaTime)
{
	_uint iEvent = 0;

	//담겨있는 복제본 오브젝트의 업데이트 수행.
	//클라에서 실제로 도는애들 인듯.
	//for (auto& pGameObject : m_GameObjects)
	//{

	//	
	//	if (iEvent = pGameObject->Update_GameObject(fDeltaTime))
	//		return iEvent;
	//}

	list<CGameObject*>::iterator iter;

	for (iter = m_GameObjects.begin() ; iter != m_GameObjects.end();)
	{
		if (iEvent = (*iter)->Update_GameObject(fDeltaTime))
			return iEvent;

		if ((*iter)->Get_Dead() == true) {
			Safe_Release(*iter);
			iter = m_GameObjects.erase(iter);
		}
		else
		{
			++iter;
		}
	}

	return iEvent;
}

_uint CLayer::LateUpdate_Layer(_float fDeltaTime)
{
	_uint iEvent = 0;
	//담겨있는 복제본 오브젝트의 레이트업데이트 수행.
	//클라에서 실제로 도는애들 인듯.
	for (auto& pGameObject : m_GameObjects)
	{
		if (iEvent = pGameObject->LateUpdate_GameObject(fDeltaTime))
			return iEvent;
	}

	return iEvent;
}

CLayer * CLayer::Create()
{
	return new CLayer;
}

void CLayer::Free()
{
	for (auto& pGameObject : m_GameObjects)
	{
		Safe_Release(pGameObject);
	}

	m_GameObjects.clear();
}
