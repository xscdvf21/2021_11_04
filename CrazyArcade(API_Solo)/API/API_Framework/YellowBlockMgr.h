#pragma once
#ifndef __CYELLOWBLOCKMGR_H__
#define __CYELLOWBLOCKMGR_H__

class CObj;
class CYellowBlockMgr
{
public:
	CYellowBlockMgr();
	~CYellowBlockMgr();

public:
	void Initialize();
	void Update();
	void Late_Update();
	void Render(HDC _DC);
	void Release();

public :
	void Yellow_Block();
	void Save_YellowBlock();
	void Load_YellowBlock();

	void Save_YellowBlock2();

	void Load_YellowBlock2();

public :
	static CYellowBlockMgr* Get_Instance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CYellowBlockMgr;
		}
		return m_pInstance;
	}

	static void Destroy_Instance()
	{
		SAFE_DELETE(m_pInstance);
	}

private :
	static	CYellowBlockMgr*		m_pInstance;
	vector<CObj*>					m_vecYellowBlock;
};

#endif // !__CYELLOWBLOCKMGR_H__



