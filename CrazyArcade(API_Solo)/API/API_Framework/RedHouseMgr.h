#pragma once
#ifndef __CREDHOUSEMGR_H__
#define __CREDHOUSEMGR_H__

class CObj;
class CRedHouseMgr
{
public:
	CRedHouseMgr();
	~CRedHouseMgr();

public :
	void Initialize();
	void Update();
	void Late_Update();
	void Render(HDC _DC);
	void Release();

public:
	void Red_House();
	void Save_Red_House();
	void Load_Red_House();

	void Save_Red_House2();
	void Load_Red_House2();

public :
	static CRedHouseMgr* Get_Instance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CRedHouseMgr;
		}
		return m_pInstance;
	}

	static void Destroy_Instance()
	{
		SAFE_DELETE(m_pInstance);
	}

private:
	static	CRedHouseMgr*			m_pInstance;
	vector<CObj*>					m_vecRedHouse;

};

#endif // !__CREDHOUSEMGR_H__



