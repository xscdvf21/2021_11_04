#pragma once

#ifndef __SCENEMGR_H__
#define __SCENEMGR_H__

class CScene;
class CSceneMgr
{
private:
	CSceneMgr();
	~CSceneMgr();

public:
	enum SCENEID { LOGO, MENU, EDIT, STAGE,STAGE2,END };

public:
	void Scene_Change(SCENEID _eID);
	void Update();
	void Late_Update();
	void Render(HDC _DC);
	void Release();

public:
	static CSceneMgr* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CSceneMgr;
		return m_pInstance;
	}
	static void Destroy_Instance()
	{
		SAFE_DELETE(m_pInstance);
	}

private:
	static CSceneMgr*	m_pInstance;
	CScene*				m_pScene;
	SCENEID				m_pPreScene;
	SCENEID				m_pCurScene;
};


#endif // !__SCENEMGR_H__

