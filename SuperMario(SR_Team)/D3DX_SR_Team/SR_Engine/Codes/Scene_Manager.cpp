#include "..\Headers\Scene_Manager.h"
#include "Scene.h"
#include "GameObject_Manager.h"

USING(Engine)
IMPLEMENT_SINGLETON(CScene_Manager)

CScene_Manager::CScene_Manager()
{
}

HRESULT CScene_Manager::Setup_CurrentScene(_int iSceneIndex, CScene* pCurrentScene)
{
	if (nullptr == pCurrentScene)
		return E_FAIL;

	if (m_iCurrentSceneIndex != iSceneIndex)
	{
		Safe_Release(m_pCurrentScene);

		m_pCurrentScene = pCurrentScene;

		m_iCurrentSceneIndex = iSceneIndex;
		m_iPreSceneIndex = iSceneIndex;
		CGameObject_Manager::Get_Instance()->Set_GameObjectLayer(iSceneIndex);
	}
	else
	{
		Safe_Release(pCurrentScene);
	}

	return S_OK;
}


HRESULT CScene_Manager::SetUp_ScnenSwap(_int iSceneIndex, CScene* pCurrentScene)
{



	Safe_Release(m_pCurrentScene);
	m_pCurrentScene = pCurrentScene;

	m_iPreSceneIndex = m_iCurrentSceneIndex;
	m_iCurrentSceneIndex = iSceneIndex;
	
	CGameObject_Manager::Get_Instance()->Set_GameObjectLayer(iSceneIndex);

	return S_OK;
}





_uint CScene_Manager::Update_Scene(_float fDeltaTime)
{
	if (nullptr == m_pCurrentScene)
		return 0;

	return m_pCurrentScene->Update_Scene(fDeltaTime);
}

_uint CScene_Manager::LateUpdate_Scene(_float fDeltaTime)
{
	if (nullptr == m_pCurrentScene)
		return 0;

	return m_pCurrentScene->LateUpdate_Scene(fDeltaTime);
}

void CScene_Manager::Free()
{
	Safe_Release(m_pCurrentScene);
}
