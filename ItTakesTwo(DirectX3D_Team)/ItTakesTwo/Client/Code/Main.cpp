#include "stdafx.h"
#include "Main.h"
#include "Export_Function.h"
#include "ChessTile.h"


CMain::CMain()
{
}

CMain::~CMain()
{
}

HRESULT CMain::Ready_Main(void)
{
	ShowCursor(false);
	FAILED_CHECK_RETURN(SetUp_DefaultSetting(&m_pGraphicDev), E_FAIL);
	Engine::Reserve_ContainerSize(RESOURCE_END);
	m_pLoading = CLoading::Create(m_pGraphicDev, (LOADINGID)LOADING_STATIC);

	FAILED_CHECK_RETURN(Engine::Ready_Physics(&m_pPhysicsClass), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Get_Renderer()->Ready_Renderer(m_pGraphicDev), E_FAIL);

	return S_OK;
}

int CMain::Update_Main(const _float & fTimeDelta)
{
	m_fTime += fTimeDelta;

	if (Engine::Key_Down(DIK_F8))
	{
		g_bRenderBox = !g_bRenderBox;
	}
	
	++m_dwRenderCnt;
	if (m_fTime >= 1.f)
	{
		wsprintf(m_szFPS, L"FPS : %d", m_dwRenderCnt);
		m_fTime = 0.f;
		m_dwRenderCnt = 0;
		SetWindowText(g_hWnd, m_szFPS);
	}

	if (m_bStart && m_pLoading->Get_Finish())
	{
		FAILED_CHECK_RETURN(Ready_Scene(m_pGraphicDev, &m_pManagementClass), E_FAIL);
		Engine::Initialize();
		
		Engine::CScene*			pScene = nullptr;

		//#¾Àº¯°æ
		pScene = CLogo::Create(m_pGraphicDev, SCENE_CB);

		NULL_CHECK_RETURN(pScene, 0)
		FAILED_CHECK_RETURN((m_pManagementClass)->Set_Scene(pScene, m_pGraphicDev), 0);
		Safe_Release(m_pLoading);
		m_bStart = false;
		return 0;
	}
	if (nullptr == m_pManagementClass)
		return -1;

	Engine::Set_InputDev();
	if(m_pManagementClass)
		return m_pManagementClass->Update_Scene(fTimeDelta);

	return 0;
}

int CMain::LateUpdate_Main(const _float & fTimeDelta)
{
	if (nullptr != m_pManagementClass)
		m_pManagementClass->LateUpdate_Scene(fTimeDelta);

	return 0;
}

void CMain::Render_Main(const _float & fTimeDelta)
{
	if (nullptr == m_pManagementClass)
		return;

	Engine::Render_Begin(D3DXCOLOR(g_vBackBufferColor.x, g_vBackBufferColor.y, g_vBackBufferColor.z, 1.0f));

	m_pManagementClass->Render_Scene(fTimeDelta, g_vBackBufferColor);


	Engine::Render_End();
}

HRESULT CMain::SetUp_DefaultSetting(LPDIRECT3DDEVICE9 * ppGraphicDev)
{
	FAILED_CHECK_RETURN(Engine::Ready_GraphicDev(g_hWnd, Engine::MODE_WIN, WINCX, WINCY,
		&m_pDeviceClass), E_FAIL);

	m_pDeviceClass->AddRef();

	*ppGraphicDev = m_pDeviceClass->GetDevice();
	(*ppGraphicDev)->AddRef();

	// dinput
	FAILED_CHECK_RETURN(Engine::Ready_InputDev(g_hInst, g_hWnd), E_FAIL);

	return S_OK;
}

HRESULT CMain::Ready_Scene(LPDIRECT3DDEVICE9 & pGraphicDev, Engine::CManagement ** ppManagementClass)
{
	Engine::CScene*			pScene = nullptr;

	FAILED_CHECK_RETURN(Engine::Create_Management(ppManagementClass, pGraphicDev), E_FAIL);
	(*ppManagementClass)->AddRef();

	return S_OK;
}

CMain * CMain::Create(void)
{
	CMain*	pInstance = new CMain;

	if (FAILED(pInstance->Ready_Main()))
	{
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

void CMain::Free(void)
{
	CChessTile::DestroyInstance();

	Engine::Safe_Release(m_pGraphicDev);
	Engine::Safe_Release(m_pDeviceClass);
	Engine::Safe_Release(m_pManagementClass);
	CTrigger::Clear_Trigger();
	Engine::Release_Utility();
	Engine::Release_Resources();
	Engine::Release_System();
	Engine::Safe_Single_Destory(m_pPhysicsClass);
}