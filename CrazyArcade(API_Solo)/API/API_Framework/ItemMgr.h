#pragma once
#ifndef __CITEMMGR_H__
#define __CITEMMGR_H__


class CObj;
class CItemMgr
{
public:
	CItemMgr();
	~CItemMgr();

public :
	void Initialize();
	void Update();
	void Late_Update();
	void Render(HDC _DC);
	void Release();


public :
	void Picking_Item();
	void Save_Item();
	void Load_Item();


public :
	static CItemMgr*	Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CItemMgr;
		return	m_pInstance;
	}

	static void Destroy_Instance()
	{
		SAFE_DELETE(m_pInstance);
	}

private :
	static CItemMgr*	m_pInstance;
	vector<CObj*>		m_vecItem;

};

#endif // !__CITEMMGR_H__



