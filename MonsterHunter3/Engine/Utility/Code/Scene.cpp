#include "Scene.h"
#include "CollisionMgr.h"
USING(Engine)

Engine::CScene::CScene(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
{
	m_pGraphicDev->AddRef();
}

Engine::CScene::~CScene(void)
{

}

HRESULT Engine::CScene::Ready_Scene(void)
{
	return S_OK;
}

Engine::_int Engine::CScene::Update_Scene(const _float& fTimeDelta)
{
	for (auto& iter : m_mapLayer)
		iter.second->Update_Layer(fTimeDelta);


	//for (auto& Pair : m_mapLayer)
	//{
	//	if (0 == wcsncmp(Pair.first, L"GameLogic", sizeof(Pair.first)))
	//		break;
	//	else
	//		return 0;
	//}

	//CCollisionMgr::Collision_Sphere(m_mapLayer.find(L"GameLogic")->second->Get_GameObject(L"Player_Weapon")
	//	,m_mapLayer.find(L"GameLogic")->second->Get_GameObject(L"Boss"));

	return 0;
}



void Engine::CScene::Free(void)
{
	for_each(m_mapLayer.begin(), m_mapLayer.end(), CDeleteMap());
	m_mapLayer.clear();

	Safe_Release(m_pGraphicDev);
}

CComponent* Engine::CScene::Get_Component(const _tchar* pLayerTag, const _tchar* pObjTag, const _tchar* pComponentTag, COMPONENTID eID)
{
	auto	iter = find_if(m_mapLayer.begin(), m_mapLayer.end(), CTag_Finder(pLayerTag));

	if (iter == m_mapLayer.end())
		return nullptr;

	return iter->second->Get_Component(pObjTag, pComponentTag, eID);
}

CGameObject * CScene::Get_GameObject(const _tchar * pLayerTag, const _tchar * pObjTag)
{
	auto iter = find_if(m_mapLayer.begin(), m_mapLayer.end(), CTag_Finder(pLayerTag));

	if (iter == m_mapLayer.end())
		return nullptr;

	return iter->second->Get_GameObject(pObjTag);
}

CLayer * CScene::Get_Layer(const _tchar * pLayerTag)
{
	auto iter = find_if(m_mapLayer.begin(), m_mapLayer.end(), CTag_Finder(pLayerTag));

	if (iter == m_mapLayer.end())
		return nullptr;

	return iter->second;
}



