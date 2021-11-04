#pragma once
#ifndef __CPADOITEMMGR_H__
#define __CPADOITEMMGR_H__

class CObj;
class CPadoItemMgr
{
public:
	CPadoItemMgr();
	~CPadoItemMgr();

public :
	void Initialize();
	void Update();
	void Late_Update();
	void Render(HDC _DC);
	void Release();

public :
	void Pado_Item();
	void Save_PadoItem();
	void Load_PadoItem();

	void Save_PadoItem2();
	void Load_PadoItem2();

public :
	static CPadoItemMgr* Get_Instance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CPadoItemMgr;
		}
		return m_pInstance;
	}

	static void Destroy_Instance()
	{
		SAFE_DELETE(m_pInstance);
	}

private :
	static	CPadoItemMgr*		m_pInstance;
	vector<CObj*>				m_vecPadoItem;
};


#endif // !__CPADOITEMMGR_H__



