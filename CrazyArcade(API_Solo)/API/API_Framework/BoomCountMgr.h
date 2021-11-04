#pragma once
#ifndef __CBOOMCOUNTMGR_H__
#define __CBOOMCOUNTMGR_H__

class CBoomCountMgr
{
public:
	CBoomCountMgr();
	~CBoomCountMgr();

public :
	void Initialize();
	void Update();
	void Late_Update();
	void Render(HDC _DC);
	void Release();

public :
	void BoomCount_Item();
	void Save_BoomCountItem();
	void Load_BoomCountItem();

	void Save_BoomCountItem2();
	void Load_BoomCountItem2();

public:
	static CBoomCountMgr* Get_Instance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CBoomCountMgr;
		}
		return m_pInstance;
	}

	static void Destroy_Instance()
	{
		SAFE_DELETE(m_pInstance);
	}

private :
	static	CBoomCountMgr*		m_pInstance;
	vector<CObj*>				m_vecPadoItem;
};

#endif // !__CBOOMCOUNTMGR_H__



