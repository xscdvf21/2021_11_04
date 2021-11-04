#include "stdafx.h"
#include "KeyMgr.h"

#include "Export_Fucntion.h"


IMPLEMENT_SINGLETON(CKeyMgr)

CKeyMgr::CKeyMgr()
{
	ZeroMemory(m_bState, sizeof(m_bState));
}


CKeyMgr::~CKeyMgr()
{
}

void CKeyMgr::Key_Update()
{
	for (int i = 0; i < VK_MAX; ++i)
	{
		if (m_bState[i] && !(GetAsyncKeyState(i) & 0x8000))
			m_bState[i] = !m_bState[i];
		if (!m_bState[i] && (GetAsyncKeyState(i) & 0x8000))
			m_bState[i] = !m_bState[i];
	}
}

_bool CKeyMgr::Key_Pressing(_uint iKey)
{
	if (GetAsyncKeyState(iKey) & 0x8000)
		return true;
	return false;
}

_bool CKeyMgr::Key_Down(_uint iKey)
{
	if (!m_bState[iKey] && (GetAsyncKeyState(iKey) & 0x8000))
	{
		m_bState[iKey] = !m_bState[iKey];
		return true;
	}
	return false;
}

_bool CKeyMgr::Key_Up(_uint iKey)
{
	if (m_bState[iKey] && !(GetAsyncKeyState(iKey) & 0x8000))
	{
		m_bState[iKey] = !m_bState[iKey];
		return true;
	}
	return false;
}

void CKeyMgr::Free()
{
}
