#pragma once

#ifndef __SCROLLMGR_H__
#define __SCROLLMGR_H__


class CScrollMgr
{
private:
	CScrollMgr();
	~CScrollMgr();
public:
	void Set_ScrollX(float _x) { m_fScrollX += _x; }

public:
	float Get_ScrollX() { return m_fScrollX; }

public:
	static CScrollMgr* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CScrollMgr;
		return m_pInstance;
	}
	static void Destroy_Instance()
	{
		SAFE_DELETE(m_pInstance);
	}

private:
	static CScrollMgr*	m_pInstance;
	float				m_fScrollX;
};


#endif // !__SCROLLMGR_H__
