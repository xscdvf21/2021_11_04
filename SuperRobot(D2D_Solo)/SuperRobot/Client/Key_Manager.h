#pragma once

#define VK_MAX 0xFF

class CKey_Manager
{
public:
	CKey_Manager();
	~CKey_Manager();

public :
	bool Key_Pressing(int _key);
	bool Key_Down(int _key);
	bool Key_Up(int _key);

	void Key_Update();

public :
	static CKey_Manager* Get_Instance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CKey_Manager;
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

private :
	static CKey_Manager*		m_pInstance;
	bool						m_bState[VK_MAX];

};

