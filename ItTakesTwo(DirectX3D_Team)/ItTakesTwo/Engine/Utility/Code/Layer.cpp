#include "Layer.h"
#include "Scene.h"

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
	//for (auto& iter : m_mapObject)
	//{
	//	_int iEvent = iter.second->Update_Object(fTimeDelta);
	//	if (iEvent == OBJ_DEAD)
	//	{
	//		iter.second->Free();
	//		iter = m_mapObject.erase(iter);
	//	}
	//}

	for (auto iter = m_mapObject.begin(); iter != m_mapObject.end();)
	{

		if (_int iEvent = iter->second->Update_Object(fTimeDelta))
		{
			if (iEvent == OBJ_DEAD)
			{
				Safe_Release(iter->second);
				iter = m_mapObject.erase(iter);
				continue;
			}
			return iEvent;
		}
		++iter;
	}

	return OBJ_NOEVENT;
}

Engine::_int Engine::CLayer::LateUpdate_Layer(const _float& fTimeDelta)
{
	for (auto& iter : m_mapObject)
		iter.second->LateUpdate_Object(fTimeDelta);

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

CComponent* Engine::CLayer::Get_Component(const _tchar* pObjTag, const _tchar* pComponentTag, COMPONENTID eID)
{
	auto	iter = find_if(m_mapObject.begin(), m_mapObject.end(), CTag_Finder(pObjTag));

	if (iter == m_mapObject.end())
		return nullptr;

	return iter->second->Get_Component(pComponentTag, eID);
}

CGameObject * CLayer::Get_GameObject(const _tchar * pObjTag)
{
	auto	iter = find_if(m_mapObject.begin(), m_mapObject.end(), CTag_Finder(pObjTag));

	if (iter == m_mapObject.end())
		return nullptr;

	return iter->second;
}

HRESULT Engine::CLayer::Add_GameObject(const PxActor* pActor, CGameObject* pGameObject)
{
	if (nullptr == pGameObject || !pActor)
		return E_FAIL;

	m_mapPxObject.emplace((void*)pActor, pGameObject);
	return S_OK;
}

Engine::CGameObject* Engine::CLayer::Get_GameObject(const PxActor* pActor)
{
	auto iter = m_mapPxObject.find((void*)pActor);
	if (iter == m_mapPxObject.end())
		return nullptr;
	return iter->second;
}

HRESULT Engine::CLayer::Add_GameObject(const _tchar* pObjTag, CGameObject* pGameObject, CScene* CurScene)
{
	if (nullptr == pGameObject)
		return E_FAIL;

	m_mapObject.emplace(pObjTag, pGameObject);

	if (pGameObject->Is_Controllable())
	{
		auto* pController = pGameObject->CreateController(CurScene->Get_PxControllMgr());
		//CurScene->Get_PxScene()->addActor(*pController->getActor());
		PxShape* pShape;
		pController->getActor()->getShapes(&pShape, 1);
		pShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true);
		pShape->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, true);
	}
	else if (pGameObject->Get_RigidActor())
	{
		CurScene->Get_PxScene()->addActor(*pGameObject->Get_RigidActor());
		m_mapPxObject.emplace(pGameObject->Get_RigidActor(), pGameObject);
	}
	pGameObject->Set_Scene(CurScene->Get_PxScene());


	return S_OK;
}


