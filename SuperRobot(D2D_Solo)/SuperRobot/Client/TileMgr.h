#pragma once

class CGameObject;
class CTileMgr
{
public:
	CTileMgr();
	~CTileMgr();



public :
	static CTileMgr*	Get_Instance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CTileMgr;
		}
		return m_pInstance;
	}
	static void Destroy_Instance()
	{
		delete	m_pInstance;
		m_pInstance = nullptr;
	}
private :
	static CTileMgr*		m_pInstance;

};

