#pragma once

#ifndef __BLOCKMGR_H__
#define __BLOCKMGR_H__

class CObj;
class CBlockMgr
{
public:
	CBlockMgr();
	~CBlockMgr();

	void Initialize();
	void Update();
	void Late_Update();
	void Render(HDC _DC);
	void Release();

public :
	void Red_Block(); //레드 블럭을 생성하기 위함

	void Save_RedBlock();
	void Load_RedBlock();

	void Save_RedBlock2();

	void Load_RedBlock2();

public :
	static CBlockMgr* Get_Instance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CBlockMgr;
		}
		return m_pInstance;
	}

	static void Destroy_Instance()
	{
		SAFE_DELETE(m_pInstance);
	}

private :
	static	CBlockMgr*		m_pInstance;
	vector<CObj*>			m_vecRedBlock;
};

#endif // !__BLOCKMGR_H__
