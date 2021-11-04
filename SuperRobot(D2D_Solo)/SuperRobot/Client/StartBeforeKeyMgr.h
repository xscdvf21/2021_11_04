#pragma once

#define VK_MAX 0xFF

class CStartBeforeKeyMgr
{
public:
	CStartBeforeKeyMgr();
	~CStartBeforeKeyMgr();

public:
	bool Key_Pressing(int _key);
	bool Key_Down(int _key);
	bool Key_Up(int _key);

	void Key_Update();

public:
	static CStartBeforeKeyMgr* Get_Instance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CStartBeforeKeyMgr;
		}
		return m_pInstance;
	}

	static void Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

private:
	static CStartBeforeKeyMgr*		m_pInstance;
	bool						m_bState[VK_MAX];

};

