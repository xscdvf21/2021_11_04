#include "stdafx.h"
#include "Scene_Manager.h"
#include "Start.h"
#include "Stage.h"
#include "WeaponStore.h"
#include "Stage2.h"
#include "StageBefore.h"
#include "StageStart.h"
#include "StagePalying.h"
#include "StageEnd.h"
IMPLEMENT_SINGLETON(CScene_Manager)

CScene_Manager::CScene_Manager()
	:m_eCurScene(SCENE_END)
{
}


CScene_Manager::~CScene_Manager()
{
}

HRESULT CScene_Manager::SceneChange_SceneManager(SCENEID eSceneID)
{
	m_eNextScene = eSceneID; 
	if (m_eCurScene != m_eNextScene)
	{
		Safe_Delete(m_pScene); 
		switch (m_eNextScene)
		{
		case CScene_Manager::SCENE_START: //겜 시작 씬
			m_pScene = new CStart;
			break;
		case CScene_Manager::SCENE_STAGEBRFORE: // 스테이지 전에 작전화면
			m_pScene = new CStageBefore;
			break;
		case CScene_Manager::SCENE_STAGESTART: // 스테이지1 시작전에 컷씬
			m_pScene = new CStageStart;
			break;
		case CScene_Manager::SCENE_STAGE: //스테이지1
			m_pScene = new CStage; 
			break;
		case CScene_Manager::SCENE_STAGEPLAYING: //스테이지1 도중에 지원군 도착.
			m_pScene = new CStagePalying;
			break;
		case CScene_Manager::SCENE_STAGEEND: //스테이지1 끝나고 무기상점 가기 전 컷씬
			m_pScene = new CStageEnd;
			break;
		case CScene_Manager::SCENE_WEAPONSTORE: // 스테이지1 끝난 후, 무기상점
			m_pScene = new CWeaponStore;
			break;
		case CScene_Manager::SCENE_STAGE2: // 스테이지2
			m_pScene = new CStage2;
			break;
		default:
			break;
		}
		if (FAILED(m_pScene->Ready_Scene()))
		{
			ERR_MSG(L"Ready_FAILED- Scene_Manager.cpp "); 
			return E_FAIL; 
		}
		m_eCurScene = m_eNextScene; 
	}
	return S_OK;
}

void CScene_Manager::Update_SceneManager()
{
	m_pScene->Update_Scene(); 
}

void CScene_Manager::Late_Update_SceneManager()
{
	m_pScene->LateUpdate_Scene();
}

void CScene_Manager::Render_SceneManager()
{
	m_pScene->Render_Scene();
}

void CScene_Manager::Release_SceneManager()
{
	Safe_Delete(m_pScene); 
}
