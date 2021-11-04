#include "GameObject_Manager.h"
#include "GameObject.h"
#include "Layer.h"
#include "CollisionManager.h"
USING(Engine)
IMPLEMENT_SINGLETON(CGameObject_Manager)

CGameObject_Manager::CGameObject_Manager()
{
}

CGameObject * CGameObject_Manager::Get_GameObject(_uint iSceneIndex, const wstring & LayerTag, _uint iIndex)
{
	if (m_iSceneCount <= iSceneIndex ||
		nullptr == m_pLayers)
		return nullptr;

	auto iter_find = m_pLayers[iSceneIndex].find(LayerTag);
	if (m_pLayers[iSceneIndex].end() == iter_find)
		return nullptr;

	return iter_find->second->Get_GameObject(iIndex);
}

CComponent * CGameObject_Manager::Get_Component(_uint iSceneIndex, const wstring & LayerTag, const wstring & ComponentTag, _uint iIndex)
{
	
	if (m_iSceneCount <= iSceneIndex ||
		nullptr == m_pLayers)
		return nullptr;

	auto iter_find = m_pLayers[iSceneIndex].find(LayerTag);
	if (m_pLayers[iSceneIndex].end() == iter_find)
		return nullptr;

	return iter_find->second->Get_Component(ComponentTag, iIndex);
}

HRESULT CGameObject_Manager::Ready_GameObject_Manager(_uint iSceneCount)
{
	if (0 == iSceneCount) //���� ������ 0����
	{
		return E_FAIL;
	}


	//������Ʈ�� ���̾��� ������ �����̳��� �����迭�� ���� �� ������ŭ �Ҵ�
	m_pGameObjects = new GAMEOBJECTS[iSceneCount];
	m_pLayers = new LAYERS[iSceneCount];

	m_iSceneCount = iSceneCount; //������ ���� ������ ��Ƶ�.

	return S_OK;
}

HRESULT CGameObject_Manager::Add_GameObject_Prototype(
	_uint iSceneIndex, 
	const wstring & PrototypeTag, 
	CGameObject * pPrototype)
{
	

	if (m_iSceneCount <= iSceneIndex ||
		nullptr == m_pGameObjects ||
		nullptr == pPrototype) 	//���� �� �ѹ��� ���� �� �������� Ŭ���,
	{
		return E_FAIL;
	}

	//�� �� ����, ��ȸ�ϸ鼭 ������Ÿ���� �±װ� ���ٸ�, �����̳ʿ� ����.
	auto iter_find = m_pGameObjects[iSceneIndex].find(PrototypeTag);
	if (m_pGameObjects[iSceneIndex].end() == iter_find)
	{
		m_pGameObjects[iSceneIndex].insert(make_pair(PrototypeTag, pPrototype));
	}
	else
	{
		//�ִٸ� ���� ���� ���� ������
		Safe_Release(pPrototype);
	}

	return S_OK;
}

HRESULT CGameObject_Manager::Clone_GameObject_ToLayer(_uint iFromSceneIndex, const wstring & PrototypeTag, _uint iToSceneIndex, const wstring & LayerTag, void * pArg)
{
	//���� �� ���ڵ��� ���� �� �������� ������
	if (m_iSceneCount <= iFromSceneIndex ||
		m_iSceneCount <= iToSceneIndex ||
		nullptr == m_pGameObjects ||
		nullptr == m_pLayers)
	{
		return E_FAIL;
	}
	
	//�������� �߰� �Ҳ���, ������ �����ϴ� m_pGameObejct[]�� ������ �˷���.
	auto iter_Prototype = m_pGameObjects[iFromSceneIndex].find(PrototypeTag);
	if (m_pGameObjects[iFromSceneIndex].end() == iter_Prototype)
	{
		TCHAR szOriginLogMsg[] = L"Not Found %s Prototype";
		TCHAR szLogMsg[256] = L"";
		swprintf_s(szLogMsg, szOriginLogMsg, PrototypeTag.c_str());
		PRINT_LOG(L"Error", szLogMsg);
		return E_FAIL;
	}

	//������ ������ ���н� �˷���.
	auto pClone = iter_Prototype->second->Clone(pArg);
	if (nullptr == pClone)
	{
		TCHAR szOriginLogMsg[] = L"Failed To Clone %s Prototype";
		TCHAR szLogMsg[256] = L"";
		swprintf_s(szLogMsg, szOriginLogMsg, PrototypeTag.c_str());
		PRINT_LOG(L"Error", szLogMsg);
		return E_FAIL;
	}

	//������ ���������� �����Ѵ�.
	//���̾�� ã�Ҵµ�, ������ �����ϱ����� �����̳ʿ� ����.
	auto iter_Layer = m_pLayers[iToSceneIndex].find(LayerTag);
	if (m_pLayers[iToSceneIndex].end() == iter_Layer)
	{
		m_pLayers[iToSceneIndex].insert(make_pair(LayerTag, CLayer::Create()));
	}

	if (FAILED(m_pLayers[iToSceneIndex][LayerTag]->Add_GameObject(pClone)))
	{
		PRINT_LOG(L"Error", L"Failed To Add_GameObject in layer");
		return E_FAIL;
	}

	return S_OK;
}

//�ѽ���
#pragma region �ѽ��� CGameObject_Manager - Loading
CGameObject* CGameObject_Manager::Clone_GameObject_For_Load(_uint iFromSceneIndex, const wstring & PrototypeTag, _uint iToSceneIndex, const wstring & LayerTag, void * pArg)
{
	if (m_iSceneCount <= iFromSceneIndex ||
		m_iSceneCount <= iToSceneIndex ||
		nullptr == m_pGameObjects ||
		nullptr == m_pLayers)
	{
		PRINT_LOG(L"CGameObject_Manager", L"Loading part");
	}

	auto iter_Prototype = m_pGameObjects[iFromSceneIndex].find(PrototypeTag);
	if (m_pGameObjects[iFromSceneIndex].end() == iter_Prototype)
	{
		TCHAR szLogMsg[256] = L"Not Found %s Prototype";
		swprintf_s(szLogMsg, szLogMsg, PrototypeTag.c_str());
		PRINT_LOG(L"Error", szLogMsg);
		PRINT_LOG(L"CGameObject_Manager", L"Loading part");
	}

	auto pClone = iter_Prototype->second->Clone(pArg);
	if (nullptr == pClone)
	{
		TCHAR szLogMsg[] = L"Failed To Clone %s Prototype";
		swprintf_s(szLogMsg, szLogMsg, PrototypeTag.c_str());
		PRINT_LOG(L"Error", szLogMsg);
		PRINT_LOG(L"CGameObject_Manager", L"Loading part");
	}

	auto iter_Layer = m_pLayers[iToSceneIndex].find(LayerTag);
	if (m_pLayers[iToSceneIndex].end() == iter_Layer)
	{
		m_pLayers[iToSceneIndex].insert(make_pair(LayerTag, CLayer::Create()));
	}

	if (FAILED(m_pLayers[iToSceneIndex][LayerTag]->Add_GameObject(pClone)))
	{
		PRINT_LOG(L"Error", L"Failed To Add_GameObject in layer");
		PRINT_LOG(L"CGameObject_Manager", L"Loading part");
	}

	//auto pm = m_pLayers[m_iLayerSelect][LayerTag];

	return pClone;
}
#pragma endregion

_uint CGameObject_Manager::Update_GameObject(_float fDeltaTime)
{
	_uint iEvent = 0;

	//LayerSelect�� ���������� Update
	for (auto& Pair : m_pLayers[m_iLayerSelect])
	{
		if (iEvent = Pair.second->Update_Layer(fDeltaTime))
			return iEvent;
	}

	//���⼭ �浹.
	//�ͷ����� ���� ���̽�(�ͷ��� �浹�� �Ⱥ��� �Ǵ����̽�) �� �����Ͽ�
	//�ͷ��� ���翩�ε� �˻��ϰ� ����.

	auto iter = m_pLayers[m_iLayerSelect].find(L"Layer_Player");
	auto titer = m_pLayers[m_iLayerSelect].find(L"Layer_Terrain");;
	if (m_pLayers[m_iLayerSelect].end() == iter || m_pLayers[m_iLayerSelect].end() == titer)
	{
		return iEvent;
	}
	
	//���� ��.
	CCollisionManager::Collision_Terrain(
		m_pLayers[m_iLayerSelect].find(L"Layer_Player")->second->Get_GameObjects()
		, m_pLayers[m_iLayerSelect].find(L"Layer_Terrain")->second->Get_GameObjects());



	return iEvent;
}

_uint CGameObject_Manager::LateUpdate_GameObject(_float fDeltaTime)
{
	_uint iEvent = 0;
	
	//LayerSelect�� ���������� Update
	for (auto& Pair : m_pLayers[m_iLayerSelect])
	{
		if (iEvent = Pair.second->LateUpdate_Layer(fDeltaTime))
			return iEvent;
	}
	

	return iEvent;
}

void CGameObject_Manager::Clear_ForScene(_uint iSceneIndex)
{

	//���� ���� ������, ���������� �Ѿ�� �����.
	if (m_iSceneCount <= iSceneIndex)
		return;
	m_pGameObjects[0];
	m_pGameObjects[1];

	for (auto& Pair : m_pGameObjects[iSceneIndex])
	{
		Safe_Release(Pair.second);
	}

	m_pLayers[0];
	m_pLayers[1];
	for (auto& Pair : m_pLayers[iSceneIndex])
	{
		Safe_Release(Pair.second);
	}
	m_pGameObjects[iSceneIndex].clear();
	m_pLayers[iSceneIndex].clear();
	
	int num1 = 10;
}

void CGameObject_Manager::Free()
{
	for (_uint i = 0; i < m_iSceneCount; ++i)
	{
		for (auto& Pair : m_pLayers[i])
		{
			Safe_Release(Pair.second);
		}

		for (auto& Pair : m_pGameObjects[i])
		{
			Safe_Release(Pair.second);
		}
	}

	Safe_Delete_Array(m_pLayers);
	Safe_Delete_Array(m_pGameObjects);
}
