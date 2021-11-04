#include "..\Headers\Management.h"

USING(Engine)
IMPLEMENT_SINGLETON(CManagement)

CManagement::CManagement()
	: m_pDeviceManager(CGraphicDevice::Get_Instance())
	, m_pRenderer(CRenderer::Get_Instance())
	, m_pComponent_Manager(CComponent_Manager::Get_Instance())
	, m_pKey_Manager(CKey_Manager::Get_Instance())
	, m_pScene_Manager(CScene_Manager::Get_Instance())
	, m_pGameObject_Manager(CGameObject_Manager::Get_Instance())
	, m_pTime_Manager(CTime_Manager::Get_Instance())
	, m_pSound_Manager(CSoundManager::Get_Instance())
	
{
	Safe_AddReference(m_pDeviceManager);
	Safe_AddReference(m_pRenderer);
	Safe_AddReference(m_pComponent_Manager);
	Safe_AddReference(m_pKey_Manager);
	Safe_AddReference(m_pScene_Manager);
	Safe_AddReference(m_pGameObject_Manager);
	Safe_AddReference(m_pTime_Manager);
	Safe_AddReference(m_pSound_Manager);
}

HRESULT CManagement::Ready_Engine(HWND hWnd, int iWinCX, int iWinCY, EDisplayMode eDisplayMode, _uint iSceneCount)
{
	if (nullptr == hWnd ||
		nullptr == m_pDeviceManager ||
		nullptr == m_pRenderer ||
		nullptr == m_pComponent_Manager ||
		nullptr == m_pScene_Manager ||
		nullptr == m_pGameObject_Manager ||
		nullptr == m_pTime_Manager ||
		nullptr == m_pSound_Manager)
	{
		return E_FAIL;
	}

	if (FAILED(m_pDeviceManager->Ready_GraphicDevice(hWnd, iWinCX, iWinCY, eDisplayMode)))
	{
		PRINT_LOG(L"Error", L"Failed To Ready_GraphicDevice");
		return E_FAIL;
	}

	if (FAILED(m_pRenderer->Set_Device(m_pDeviceManager->Get_Device(), m_pDeviceManager->Get_Sprite(), m_pDeviceManager->Get_Font())))
	{
		PRINT_LOG(L"Error", L"Failed To Set_Device to Renderer");
		return E_FAIL;
	}

	if (FAILED(m_pGameObject_Manager->Ready_GameObject_Manager(iSceneCount)))
	{
		PRINT_LOG(L"Error", L"Failed To Ready_GameObject_Manager");
		return E_FAIL;
	}

	if (FAILED(m_pComponent_Manager->Ready_Component_Manager(iSceneCount)))
	{
		PRINT_LOG(L"Error", L"Failed To Ready_Component_Manager (Management.cpp)");
		return E_FAIL;
	}

	return S_OK;
}

_uint CManagement::Update_Engine() 
{
	if (nullptr == m_pScene_Manager ||
		nullptr == m_pGameObject_Manager ||
		nullptr == m_pTime_Manager ||
		nullptr == m_pKey_Manager)
		return 0;

	_uint iEvent = 0;
	_float fDeltaTime = m_pTime_Manager->Update_Time_Manager();

	m_pKey_Manager->Update_KeyManager();

	if (iEvent = m_pScene_Manager->Update_Scene(fDeltaTime))
		return iEvent;

	if (iEvent = m_pGameObject_Manager->Update_GameObject(fDeltaTime))
		return iEvent;

	if (iEvent = m_pScene_Manager->LateUpdate_Scene(fDeltaTime))
		return iEvent;

	if (iEvent = m_pGameObject_Manager->LateUpdate_GameObject(fDeltaTime))
		return iEvent;

	return iEvent;
}

HRESULT CManagement::Render_Engine(HWND hWnd)
{
	if (nullptr == m_pRenderer)
		return E_FAIL;

	if (FAILED(m_pRenderer->Render_RenderList(hWnd)))
	{
		PRINT_LOG(L"Error", L"Failed To Render_RenderList");
		return E_FAIL;
	}

	return S_OK;
}

_Device CManagement::Get_Device()
{
	if (nullptr == m_pDeviceManager)
		return nullptr;

	return m_pDeviceManager->Get_Device();
}

LPD3DXFONT Engine::CManagement::Get_Font()
{
	return m_pDeviceManager->Get_Font();
}

LPD3DXSPRITE Engine::CManagement::Get_Sprite()
{
	return m_pDeviceManager->Get_Sprite();
}

_bool CManagement::Key_Pressing(_uint iKey)
{
	return m_pKey_Manager->Key_Pressing(iKey);
}

_bool CManagement::Key_Down(_uint iKey)
{
	return m_pKey_Manager->Key_Down(iKey);
}

_bool CManagement::Key_Up(_uint iKey)
{
	return m_pKey_Manager->Key_Up(iKey);
}

HRESULT CManagement::Setup_CurrentScene(_int iSceneIndex, CScene * pCurrentScene)
{
	return m_pScene_Manager->Setup_CurrentScene(iSceneIndex, pCurrentScene);
}

HRESULT Engine::CManagement::SetUp_ScnenSwap(_int iSceneIndex, CScene * pCurrentScene)
{
	return  m_pScene_Manager->SetUp_ScnenSwap(iSceneIndex, pCurrentScene);
}

HRESULT CManagement::Add_Component_Prototype(_uint iSceneIndex, const wstring & PrototypeTag, CComponent * pPrototype)
{
	if (nullptr == m_pComponent_Manager)
		return E_FAIL;

	return m_pComponent_Manager->Add_Component_Prototype(iSceneIndex, PrototypeTag, pPrototype);
}

CComponent * CManagement::Clone_Component(_uint iSceneIndex, const wstring & PrototypeTag, void * pArg)
{
	if (nullptr == m_pComponent_Manager)
		return nullptr;

	return m_pComponent_Manager->Clone_Component(iSceneIndex, PrototypeTag, pArg);
}

HRESULT Engine::CManagement::Load_Sound()
{
	if (m_pSound_Manager == nullptr)
	{
		return E_FAIL;
	}
	m_pSound_Manager->Initialize();

	return S_OK;
}

HRESULT CManagement::Play_Sound(TCHAR* FileName, SOUND_CHANNELID ChannelID)
{
	if (m_pSound_Manager == nullptr)
	{
		return E_FAIL;
	}
	m_pSound_Manager->PlaySoundW(FileName, ChannelID);

	return S_OK;
}

HRESULT CManagement::Stop_Sound(SOUND_CHANNELID ChannelID)
{
	if (m_pSound_Manager == nullptr)
	{
		return E_FAIL;
	}
	m_pSound_Manager->StopSound(ChannelID);

	return S_OK;
}

HRESULT CManagement::Play_BGM(TCHAR* FileName)
{
	if (m_pSound_Manager == nullptr)
	{
		return E_FAIL;
	}
	m_pSound_Manager->PlayBGM(FileName);

	return S_OK;
}

HRESULT CManagement::Stop_AllSound()
{
	if (m_pSound_Manager == nullptr)
	{
		return E_FAIL;
	}
	m_pSound_Manager->StopAll();
	return S_OK;
}

CGameObject* CManagement::Get_GameObject(_uint iSceneIndex, const wstring& LayerTag, _uint iIndex)
{
	if (nullptr == m_pGameObject_Manager)
		return nullptr;

	return m_pGameObject_Manager->Get_GameObject(iSceneIndex, LayerTag, iIndex);
}

CComponent* CManagement::Get_Component(
	_uint iSceneIndex,
	const wstring& LayerTag,
	const wstring& ComponentTag,
	_uint iIndex)
{
	if (nullptr == m_pGameObject_Manager)
		return nullptr;

	return m_pGameObject_Manager->Get_Component(iSceneIndex, LayerTag, ComponentTag, iIndex);
}

HRESULT CManagement::Add_GameObject_Prototype(_uint iSceneIndex, const wstring& PrototypeTag, CGameObject* pPrototype)
{
	if (nullptr == m_pGameObject_Manager)
		return E_FAIL;

	return m_pGameObject_Manager->Add_GameObject_Prototype(iSceneIndex, PrototypeTag, pPrototype);
}

HRESULT CManagement::Clone_GameObject_ToLayer(_uint iFromSceneIndex, const wstring& PrototypeTag, _uint iToSceneIndex, const wstring& LayerTag, void* pArg)
{
	if (nullptr == m_pGameObject_Manager)
		return E_FAIL;

	return m_pGameObject_Manager->Clone_GameObject_ToLayer(iFromSceneIndex, PrototypeTag, iToSceneIndex, LayerTag, pArg);
}

#pragma region 한승현 CManagement - Loading
CGameObject* CManagement::Clone_GameObject_For_Load(_uint iFromSceneIndex, const wstring& PrototypeTag, _uint iToSceneIndex, const wstring& LayerTag, void* pArg)
{
	if (nullptr == m_pGameObject_Manager)
	{
		PRINT_LOG(L"CManagement", L"Loading part");
	}
	
	return m_pGameObject_Manager->Clone_GameObject_For_Load(iFromSceneIndex, PrototypeTag, iToSceneIndex, LayerTag, pArg);
}
void Engine::CManagement::Clear_ForScene(_uint iSceneIndex)
{
	m_pGameObject_Manager->Clear_ForScene(iSceneIndex);
	m_pComponent_Manager->Clear_ForScene(iSceneIndex);
}
#pragma endregion


HRESULT Engine::CManagement::Add_RenderList(ERenderPriority ePriority, CGameObject* pGameObject)
{
	if (nullptr == m_pRenderer)
		return E_FAIL;

	return m_pRenderer->Add_RenderList(ePriority, pGameObject);
}


//해제는 생성의 역순
void CManagement::Free()
{
	Safe_Release(m_pSound_Manager);
	Safe_Release(m_pTime_Manager);
	Safe_Release(m_pGameObject_Manager);
	Safe_Release(m_pScene_Manager);
	Safe_Release(m_pKey_Manager);
	Safe_Release(m_pComponent_Manager);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pDeviceManager);
	
}

void CManagement::Release_Engine()
{
	/* 항상 CManagement의 Destroy부터 호출해라! */
	if (CManagement::Destroy_Instance())
		PRINT_LOG(L"Warning", L"Failed To Release CManagement");

	if (CSoundManager::Destroy_Instance())
		PRINT_LOG(L"Waring", L"Failed To Release CSoundManager (Management.cpp)");

	if(CTime_Manager::Destroy_Instance())
		PRINT_LOG(L"Waring", L"Failed To Release CTime_Manager (Management.cpp)");

	if(CGameObject_Manager::Destroy_Instance())
		PRINT_LOG(L"Waring", L"Failed To Release CGameObject_Manager (Management.cpp)");

	if(CScene_Manager::Destroy_Instance())
		PRINT_LOG(L"Waring", L"Failed To Release CScene_Manager (Management.cpp)");

	if (CKey_Manager::Destroy_Instance())
		PRINT_LOG(L"Waring", L"Failed To Release CKey_Manager (Management.cpp)");

	if (CComponent_Manager::Destroy_Instance())
		PRINT_LOG(L"Waring", L"Failed To Release CComponent_Manager (Management.cpp)");

	if (CRenderer::Destroy_Instance())
		PRINT_LOG(L"Warning", L"Failed To Release CRenderer");

	if (CGraphicDevice::Destroy_Instance())
		PRINT_LOG(L"Warning", L"Failed To Release CGraphicDevice");
}

const _int Engine::CManagement::Get_DuplicateChecker() const
{
	return CScene_Manager::Get_Instance()->Get_DuplicateChecker();
}

