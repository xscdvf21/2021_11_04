#include "stdafx.h"
#include "..\Headers\GameObjMgr.h"


USING(Engine)
IMPLEMENT_SINGLETON(CGameObjMgr)

CGameObjMgr::CGameObjMgr()
{
}

#pragma region 게임 오브젝트를 반복되도록 하는 함수.
_uint CGameObjMgr::Update_GameObject(_float fDeltaTime)
{
	_uint iEvent = 0;

	if ( 0 == m_GameObjClone.size())
	{
		return iEvent;
	}


	for (auto& iter = m_GameObjClone.begin(); iter != m_GameObjClone.end(); ++iter)
	{
		iEvent = (*iter)->Update_GameObject(fDeltaTime);

		if (OBJ_DEAD == iEvent)
		{
			Safe_Release((*iter));
			m_GameObjClone.erase(iter);
			return 0;
		}
	}
	//for (auto& Pair : m_GameObjClone)
	//{

	//	if (iEvent = Pair->Update_GameObject(fDeltaTime))
	//		return iEvent;
	//}

	return iEvent;
}

_uint CGameObjMgr::LateUpdate_GameObject(_float fDeltaTime)
{
	_uint iEvent = 0;

	if (0 == m_GameObjClone.size())
	{
		return iEvent;
	}

	for (auto& Pair : m_GameObjClone)
	{
		if (Pair->LateUpdate_GameObject(fDeltaTime))
			return iEvent;
		/*if (iEvent = Pair.second->LateUpdate_GameObject(fDeltaTime))
			return iEvent;*/
	}

	return iEvent;
}

_uint CGameObjMgr::Render_GameObject()
{
	_uint iEvent = 0;

	if ( 0 == m_GameObjClone.size())
	{
		return iEvent;
	}

	for (auto& Pair : m_GameObjClone)
	{
		if (iEvent = Pair->Render_GameObject())
			return iEvent;
	}
	

	return iEvent;
}
#pragma endregion


HRESULT CGameObjMgr::Ready_GameObject_Manager()
{
	m_pGameObjects = new GAMEOBJECTS();

	return S_OK;
}
HRESULT CGameObjMgr::Add_GameObject_Prototype(const wstring & PrototypeTag, CGameObj * pPrototype)
{
	if (nullptr == m_pGameObjects ||
		nullptr == pPrototype)
	{
		return E_FAIL;
	}

	auto iter_find = m_pGameObjects->find(PrototypeTag);

	if (m_pGameObjects->end() == iter_find)
	{
		m_pGameObjects->insert(make_pair(PrototypeTag, pPrototype));
	}
	else
	{
		Safe_Release(pPrototype);
	}

	return S_OK;
}
HRESULT CGameObjMgr::Clone_GameObject_ToLayer(const wstring & PrototypeTag, void * pArg)
{
	if (nullptr == m_pGameObjects)
		return E_FAIL;

	auto iter_Prototype = m_pGameObjects->find(PrototypeTag);
	if (m_pGameObjects->end() == iter_Prototype)
	{
		TCHAR szLogMsg[] = L"Not Found %s Prototype";
		swprintf_s(szLogMsg, szLogMsg, PrototypeTag.c_str());
		PRINT_LOG(L"Error", szLogMsg);
		return E_FAIL;
	}


	auto pClone = iter_Prototype->second->Clone(pArg);
	if (nullptr == pClone)
	{
		TCHAR szLogMsg[] = L"Failed To Clone %s Prototype";
		swprintf_s(szLogMsg, szLogMsg, PrototypeTag.c_str());
		PRINT_LOG(L"Error", szLogMsg);
		return E_FAIL;
	}

	//클론 들어옴.
	m_GameObjClone.push_back(pClone);


	return S_OK;
}

CGameObj * CGameObjMgr::Get_Clone_GameObject(const wstring & PrototypeTag, void * pArg)
{
	if (nullptr == m_pGameObjects)
		return nullptr;

	auto iter_Prototype = m_pGameObjects->find(PrototypeTag);
	if (m_pGameObjects->end() == iter_Prototype)
	{
		TCHAR szLogMsg[] = L"Not Found %s Prototype";
		swprintf_s(szLogMsg, szLogMsg, PrototypeTag.c_str());
		PRINT_LOG(L"Error", szLogMsg);
		return nullptr;
	}


	auto pClone = iter_Prototype->second->Clone(pArg);
	if (nullptr == pClone)
	{
		TCHAR szLogMsg[] = L"Failed To Clone %s Prototype";
		swprintf_s(szLogMsg, szLogMsg, PrototypeTag.c_str());
		PRINT_LOG(L"Error", szLogMsg);
		return nullptr;
	}

	//클론 들어옴.
	m_GameObjClone.push_back(pClone);



	return pClone;
}

HRESULT CGameObjMgr::Set_Clone(CGameObj * pObj)
{
	m_GameObjClone.push_back(pObj);
	return S_OK;
}

void CGameObjMgr::Free()
{
	for (auto& Pair : *m_pGameObjects)
	{
		Safe_Release(Pair.second);
	}

	/*for (auto& Pair : m_GameObjClone)
	{
		Safe_Release(m_GameObjClone);
	}*/

	Safe_Delete_Array(m_pGameObjects);
}
