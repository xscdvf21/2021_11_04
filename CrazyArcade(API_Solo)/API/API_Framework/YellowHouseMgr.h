#pragma once
#ifndef __CYELLOWHOUSEMGR_H__
#define __CYELLOWHOUSEMGR_H__

class CObj;
class CYellowHouseMgr
{
public:
	CYellowHouseMgr();
	~CYellowHouseMgr();

public :
	void Initialize();
	void Update();
	void Late_Update();
	void Render(HDC _DC);
	void Release();

public:
	void Yellow_House();
	void Save_Yellow_House();
	void Load_Yellow_House();

	void Save_Yellow_House2();

	void Load_Yellow_House2();

public:
	static CYellowHouseMgr* Get_Instance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CYellowHouseMgr;
		}
		return m_pInstance;
	}

	static void Destroy_Instance()
	{
		SAFE_DELETE(m_pInstance);
	}

private:
	static	CYellowHouseMgr*		m_pInstance;
	vector<CObj*>					m_vecYellowHouse;
};
#endif // !__CYELLOWHOUSEMGR_H__



