#pragma once
#ifndef __CBLUEHOUSEMGR_H__
#define __CBLUEHOUSEMGR_H__
class CObj;
class CBlueHouseMgr
{
public:
	CBlueHouseMgr();
	~CBlueHouseMgr();

public :
	void Initialize();
	void Update();
	void Late_Update();
	void Render(HDC _DC);
	void Release();

public :
	void Blue_House();
	void Save_Blue_House();
	void Load_Blue_House();

	void Save_Blue_House2();
	void Load_Blue_House2();

public:
	static CBlueHouseMgr* Get_Instance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CBlueHouseMgr;
		}
		return m_pInstance;
	}

	static void Destroy_Instance()
	{
		SAFE_DELETE(m_pInstance);
	}

private:
	static	CBlueHouseMgr*			m_pInstance;
	vector<CObj*>					m_vecBlueHouse;


};

#endif // !__CBLUEHOUSEMGR_H__



