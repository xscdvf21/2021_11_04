#include "stdafx.h"
#include "MainApp.h"
#include "NaviMgr.h"
#include "SoundMgr.h"
#include "KeyMgr.h"

#include "Export_Fucntion.h"

CMainApp::CMainApp(void)	
{

}

CMainApp::~CMainApp(void)
{

}

HRESULT CMainApp::Ready_MainApp(void)
{
	CSoundMgr::Get_Instance()->Initialize();
	FAILED_CHECK_RETURN(SetUp_DefaultSetting(&m_pGraphicDev), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Scene(m_pGraphicDev, &m_pManagementClass), E_FAIL);

	ShowCursor(FALSE);

	srand(time(NULL));
	return S_OK;
}

int CMainApp::Update_MainApp(const _float& fTimeDelta)
{
	if (nullptr == m_pManagementClass)
		return -1;

	m_fTime += fTimeDelta;

	Engine::Set_InputDev();


	/*_long	dwMouseMove = 0;

	if (dwMouseMove = Engine::Get_DIMouseMove(Engine::DIMS_Z))
	{
		int a = 10;
	}
*/	
	m_pManagementClass->Update_Scene(fTimeDelta);
	Engine::Update_KeyMgr();

	return 0;
}

void CMainApp::Render_MainApp(void)
{
	if (nullptr == m_pManagementClass)
		return;

	++m_dwRenderCnt;

	//if (m_fTime >= 1.f)
	//{
	//	wsprintf(m_szFPS, L"FPS : %d", m_dwRenderCnt);
	//	m_fTime = 0.f;
	//	m_dwRenderCnt = 0;
	//}

		
	Engine::Render_Begin(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.f));

	m_pManagementClass->Render_Scene(m_pGraphicDev);

	//Engine::Render_Font(L"Font_Jinji", m_szFPS, &_vec2(550.f, 10.f), D3DXCOLOR(1.f, 0.f, 0.f, 1.f));

	Engine::Render_End();
}

HRESULT CMainApp::SetUp_DefaultSetting(LPDIRECT3DDEVICE9 * ppGraphicDev)
{
	FAILED_CHECK_RETURN(Engine::Ready_GraphicDev(g_hWnd, Engine::MODE_FULL, WINCX, WINCY,
		&m_pDeviceClass), E_FAIL);

	m_pDeviceClass->AddRef();

	*ppGraphicDev = m_pDeviceClass->GetDevice();
	(*ppGraphicDev)->AddRef();

	// dinput
	FAILED_CHECK_RETURN(Engine::Ready_InputDev(g_hInst, g_hWnd), E_FAIL);

	// Font
	//FAILED_CHECK_RETURN(Engine::Ready_Font((*ppGraphicDev), L"Font_Default", L"¹ÙÅÁ", 15, 15, FW_HEAVY), E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Ready_Font((*ppGraphicDev), L"Font_Jinji", L"±Ã¼­", 20, 20, FW_BOLD), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Font((*ppGraphicDev), L"Font_Jinji", L"Mrs. Monster 3D Italic 400", 30, 30, FW_BOLD), E_FAIL);

	return S_OK;
}

HRESULT CMainApp::Ready_Scene(LPDIRECT3DDEVICE9& pGraphicDev, Engine::CManagement ** ppManagementClass)
{
	Engine::CScene*			pScene = nullptr;

	FAILED_CHECK_RETURN(Engine::Create_Management(ppManagementClass), E_FAIL);
	(*ppManagementClass)->AddRef();

	pScene = CLogo::Create(pGraphicDev);
	NULL_CHECK_RETURN(pScene, E_FAIL);

	FAILED_CHECK_RETURN((*ppManagementClass)->Set_Scene(pScene), E_FAIL);


	return S_OK;
}

CMainApp* CMainApp::Create(void)
{
	CMainApp*	pInstance = new CMainApp;

	if (FAILED(pInstance->Ready_MainApp()))
	{
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainApp::Free(void)
{
	CNaviMgr::DestroyInstance();
	CSoundMgr::Destroy_Instance();
	CKeyMgr::DestroyInstance();

	Engine::Safe_Release(m_pGraphicDev);
	Engine::Safe_Release(m_pDeviceClass);
	Engine::Safe_Release(m_pManagementClass);

	Engine::Release_Utility();
	Engine::Release_Resources();
	Engine::Release_System();
}

