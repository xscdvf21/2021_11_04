#pragma once
#ifndef __CTOWNBLOCKMGR_H__
#define __CTOWNBLOCKMGR_H__

class CObj;
class CTownBlockMgr
{
public:
	CTownBlockMgr();
	~CTownBlockMgr();

public :
	void Initialize();
	void Update();
	void Late_Update();
	void Render(HDC _DC);
	void Release();

public:
	void Town_Block();
	void Save_Town_Block();
	void Load_Town_Block();

	void Save_Town_Block2();
	void Load_Town_Block2();

public:
	static CTownBlockMgr* Get_Instance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CTownBlockMgr;
		}
		return m_pInstance;
	}

	static void Destroy_Instance()
	{
		SAFE_DELETE(m_pInstance);
	}


private:
	static	CTownBlockMgr*			m_pInstance;
	vector<CObj*>					m_vecTownBlock;

};



#endif // !__CTOWNBLOCKMGR_H__



