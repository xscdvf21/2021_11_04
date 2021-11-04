#include "stdafx.h"
#include "SceneMgr.h"
#include "Scene.h"
#include "Logo.h"
#include "MyMenu.h"
#include "Stage.h"
#include "MyEdit.h"
#include "Stage2.h"
#include "BossStage.h"


CSceneMgr*	CSceneMgr::m_pInstance = nullptr;
CSceneMgr::CSceneMgr()
	: m_pPreScene(END), m_pCurScene(END), m_pScene(nullptr)
{
}


CSceneMgr::~CSceneMgr()
{
	Release();
}

void CSceneMgr::Scene_Change(SCENEID _eID)
{
	m_pCurScene = _eID;

	if (m_pPreScene != m_pCurScene)
	{
		Release();
		switch (m_pCurScene)
		{
		case CSceneMgr::LOGO:
			m_pScene = new CLogo;
			break;
		case CSceneMgr::MENU:
			m_pScene = new CMyMenu;
			break;
		case CSceneMgr::EDIT:
			m_pScene = new CMyEdit;
			break;
		case CSceneMgr::STAGE:
			m_pScene = new CStage;
			break;
		case CSceneMgr::STAGE2:
			m_pScene = new CStage2;
			break;
		}
		m_pScene->Initialize();
		m_pPreScene = m_pCurScene;
	}
}

void CSceneMgr::Update()
{
	m_pScene->Update();
}

void CSceneMgr::Late_Update()
{
	m_pScene->Late_Update();
}

void CSceneMgr::Render(HDC _DC)
{
	m_pScene->Render(_DC);
}

void CSceneMgr::Release()
{
	SAFE_DELETE(m_pScene);
}
