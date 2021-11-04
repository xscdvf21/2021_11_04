#include "Layer.h"
#include "CollisionMgr.h"

USING(Engine)

Engine::CLayer::CLayer(void)
{

}

Engine::CLayer::~CLayer(void)
{

}

HRESULT Engine::CLayer::Ready_Layer(void)
{
	return S_OK;
}

Engine::_int Engine::CLayer::Update_Layer(const _float& fTimeDelta)
{
	auto& iter = m_mapObject.begin();

	for (; iter != m_mapObject.end(); )
	{
		int iEvent = iter->second->Update_Object(fTimeDelta);

		if (OBJ_DEAD == iEvent)
		{
			Safe_Release(iter->second);
			iter = m_mapObject.erase(iter);
			m_mapObject.begin();
		}
		else
		{
			++iter;
		}
	}

	



	/*CCollisionMgr::Collision_Sphere(m_mapObject.find(L"Player_Weapon")->second, m_mapObject.find(L"Boss")->second);*/
	//for (auto& iter : m_mapObject)
	//{
	//	int iEvent = iter.second->Update_Object(fTimeDelta);
	//	if (OBJ_DEAD == iEvent)
	//	{
	//		Safe_Release(iter.second);
	//		m_mapObject.erase(iter.first);
	//	}	
	//}
	return 0;
}



CLayer* Engine::CLayer::Create(void)
{
	CLayer*		pInstance = new CLayer;

	if (FAILED(pInstance->Ready_Layer()))
		Safe_Release(pInstance);

	return pInstance;
}

void Engine::CLayer::Free(void)
{
	for_each(m_mapObject.begin(), m_mapObject.end(), CDeleteMap());
	m_mapObject.clear();
}

HRESULT Engine::CLayer::Add_GameObject(const _tchar* pObjTag, CGameObject* pGameObject)
{
	if (nullptr == pGameObject)
		return E_FAIL;

	m_mapObject.emplace(pObjTag, pGameObject);

	return S_OK;
}

CGameObject * CLayer::Get_GameObject(const _tchar * pObjTag)
{
	auto	iter = find_if(m_mapObject.begin(), m_mapObject.end(), CTag_Finder(pObjTag));


	if (iter == m_mapObject.end())
		return nullptr;

	return iter->second;
}

multimap<const _tchar*, CGameObject*>* CLayer::Get_LayerMap()
{
	return &m_mapObject;
}

CComponent* Engine::CLayer::Get_Component(const _tchar* pObjTag, const _tchar* pComponentTag, COMPONENTID eID)
{
	auto	iter = find_if(m_mapObject.begin(), m_mapObject.end(), CTag_Finder(pObjTag));

	if (iter == m_mapObject.end())
		return nullptr;

	return iter->second->Get_Component(pComponentTag, eID);
}

