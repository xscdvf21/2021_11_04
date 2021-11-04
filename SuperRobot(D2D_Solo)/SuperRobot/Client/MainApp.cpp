#include "stdafx.h"
#include "MainApp.h"
#include "GameObject_Manger.h"
#include "Scene_Manager.h"
#include "Texture_Manager.h"
#include "Frame_Manager.h"
#include "Key_Manager.h"
#include "ScrollMgr.h"
#include "TileMgr.h"
#include "Time_Manager.h"
#include "SoundMgr.h"
#include "StartBeforeKeyMgr.h"
CMainApp::CMainApp()
{
}


CMainApp::~CMainApp()
{
}

HRESULT CMainApp::Ready_MainApp()
{
	CSoundMgr::Get_Instance()->Initialize();
	CTime_Manager::Get_Instance()->Ready_Time_Manager(); 
	if (FAILED(CGraphic_Device::Get_Instance()->Ready_Graphic_Device(CGraphic_Device::MODE_WIN)))
	{
		ERR_MSG(L"Create Failed Graphic_Device - MainApp.cpp"); 
		return E_FAIL; 
	}


	if (FAILED(CTexture_Manager::Get_Instance()->LoadImagePath(L"../Data/PathInfo.txt"))) 
	{
		ERR_MSG(L"Create Failed LoadImagePath(Data/PathInfo.txt) - MainApp.cpp");
		return E_FAIL;
	}

	//if (FAILED(CTexture_Manager::Get_Instance()->Insert_TextureMgr(CTexture_Manager::MULTI_TEX, L"../Texture/MenuSelect/Mouse/Cursor0%d.png", L"MenuSelect", L"Cursor", 2)))
	//{
	//	ERR_MSG(L"Create Failed Insert_TextureMgr(MouseCursor) - MainApp.cpp");
	//	return E_FAIL;
	//}

	if (FAILED(CTexture_Manager::Get_Instance()->Insert_TextureMgr(CTexture_Manager::MULTI_TEX, L"../Texture/Map/Tile0%d.png", L"Map", L"Ground", 1))) //맵에 칸을 나눌 타일.
	{
		ERR_MSG(L"Create Failed Insert_TextureMgr(Tile) - MainApp.cpp");
		return E_FAIL;
	}

	//if (FAILED(CTexture_Manager::Get_Instance()->Insert_TextureMgr(CTexture_Manager::MULTI_TEX, L"../Texture/Unit/GunDamMK/GunDamMK0%d.png", L"Unit", L"GunDamMK", 1)))//유닛 하나 생성
	//{
	//	ERR_MSG(L"Create Failed Insert_TextureMgr(GunDamMK) - MainApp.cpp");
	//	return E_FAIL;
	//}


	if (FAILED(CScene_Manager::Get_Instance()->SceneChange_SceneManager(CScene_Manager::SCENE_START)))
	{
		ERR_MSG(L"Create Failed SceneChange_SceneManager(SCENE_STAGE) - MainApp.cpp");
		return E_FAIL;
	}
	return S_OK;/* E_FAIL; */
}
 //속도 = 거리 / 시간

void CMainApp::Update_MainApp()
{
	CTime_Manager::Get_Instance()->Update_Time_Manager();
	CScene_Manager::Get_Instance()->Update_SceneManager(); 

}

void CMainApp::Late_UpDate_MainApp()
{
	CScene_Manager::Get_Instance()->Late_Update_SceneManager();
	CKey_Manager::Get_Instance()->Key_Update();
	CStartBeforeKeyMgr::Get_Instance()->Key_Update();
	CScrollMgr::Get_Instance()->Scroll_Lock();
}

void CMainApp::Render_MainApp(CFrame_Manager& rFrame)
{
	CGraphic_Device::Get_Instance()->Render_Begin(); 
	CScene_Manager::Get_Instance()->Render_SceneManager();
	rFrame.Render_Frame_Manager();
	CGraphic_Device::Get_Instance()->Render_End(); 
}

void CMainApp::Release_MainApp()
{
	CTime_Manager::Destroy_Instance(); 
	CGameObject_Manger::Destroy_Instance(); 
	CScene_Manager::Destroy_Instance(); 
	CTexture_Manager::Destroy_Instance();
	CKey_Manager::Destroy_Instance();
	CStartBeforeKeyMgr::Destroy_Instance();
	CTileMgr::Destroy_Instance();
	CSoundMgr::Destroy_Instance();
	CGraphic_Device::Destroy_Instance();
}

CMainApp * CMainApp::Create()
{
	CMainApp* pInstance = new CMainApp;
	if (FAILED(pInstance->Ready_MainApp()))
	{
		delete pInstance; 
		pInstance = nullptr; 
	}
	return pInstance;
}

void CMainApp::Free()
{
	Release_MainApp(); 
	delete this; 
}
