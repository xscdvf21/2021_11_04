#pragma once
#ifndef __CBUSHMGR_H__
#define __CBUSHMGR_H__

class CObj;
class CBushMgr
{
public:
	CBushMgr();
	~CBushMgr();

public:
	void Initialize();
	void Update();
	void Late_Update();
	void Render(HDC _DC);
	void Release();

public:
	void Bush();
	void Save_Bush();
	void Load_Bush();

	void Save_Bush2();
	void Load_Bush2();

public:
	static CBushMgr* Get_Instance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CBushMgr;
		}
		return m_pInstance;
	}

	static void Destroy_Instance()
	{
		SAFE_DELETE(m_pInstance);
	}

private:
	static	CBushMgr*				m_pInstance;
	vector<CObj*>					m_vecBush;
};

#endif // !__CBUSHMGR_H__

