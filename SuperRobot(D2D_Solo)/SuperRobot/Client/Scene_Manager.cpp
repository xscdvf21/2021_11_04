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
		case CScene_Manager::SCENE_START: //�� ���� ��
			m_pScene = new CStart;
			break;
		case CScene_Manager::SCENE_STAGEBRFORE: // �������� ���� ����ȭ��
			m_pScene = new CStageBefore;
			break;
		case CScene_Manager::SCENE_STAGESTART: // ��������1 �������� �ƾ�
			m_pScene = new CStageStart;
			break;
		case CScene_Manager::SCENE_STAGE: //��������1
			m_pScene = new CStage; 
			break;
		case CScene_Manager::SCENE_STAGEPLAYING: //��������1 ���߿� ������ ����.
			m_pScene = new CStagePalying;
			break;
		case CScene_Manager::SCENE_STAGEEND: //��������1 ������ ������� ���� �� �ƾ�
			m_pScene = new CStageEnd;
			break;
		case CScene_Manager::SCENE_WEAPONSTORE: // ��������1 ���� ��, �������
			m_pScene = new CWeaponStore;
			break;
		case CScene_Manager::SCENE_STAGE2: // ��������2
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
