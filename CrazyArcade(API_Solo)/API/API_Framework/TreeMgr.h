#pragma once
#ifndef __CTREEMGR_H__
#define __CTREEMGR_H__

class CObj;
class CTreeMgr
{
public:
	CTreeMgr();
	~CTreeMgr();

public:
	void Initialize();
	void Update();
	void Late_Update();
	void Render(HDC _DC);
	void Release();

public:
	void Tree();
	void Save_Tree();
	void Load_Tree();

	void Save_Tree2();
	void Load_Tree2();

public:
	static CTreeMgr* Get_Instance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CTreeMgr;
		}
		return m_pInstance;
	}

	static void Destroy_Instance()
	{
		SAFE_DELETE(m_pInstance);
	}

private:
	static	CTreeMgr*				m_pInstance;
	vector<CObj*>					m_vecTree;
};


#endif // !__CTREEMGR_H__



