#pragma once
#ifndef __CSPEEDITEMMGR_H__
#define __CSPEEDITEMMGR_H__

class CObj;
class CSpeedItemMgr
{
public:
	CSpeedItemMgr();
	~CSpeedItemMgr();

public :
	void Initialize();
	void Update();
	void Late_Update();
	void Render(HDC _DC);
	void Release();

public :
	void Speed_Item();
	void Save_SpeedItem();
	void Load_SpeedItem();

	void Save_SpeedItem2();
	void Load_SpeedItem2();

public:
	static CSpeedItemMgr* Get_Instance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CSpeedItemMgr;
		}
		return m_pInstance;
	}

	static void Destroy_Instance()
	{
		SAFE_DELETE(m_pInstance);
	}

private:
	static	CSpeedItemMgr*		m_pInstance;
	vector<CObj*>				m_vecSpeedItem;
};

#endif // !__CSPEEDITEMMGR_H__



