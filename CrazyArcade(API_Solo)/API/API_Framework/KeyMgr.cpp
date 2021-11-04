#include "stdafx.h"
#include "KeyMgr.h"

CKeyMgr* CKeyMgr::m_pInstance = nullptr;

CKeyMgr::CKeyMgr()
{
	ZeroMemory(m_bState, sizeof(m_bState));
}


CKeyMgr::~CKeyMgr()
{
}

bool CKeyMgr::Key_Pressing(int _key)
{
	if (GetAsyncKeyState(_key) & 0x8000)
		return true;
	return false;
}

bool CKeyMgr::Key_Down(int _key)
{
	if (!m_bState[_key] && (GetAsyncKeyState(_key) & 0x8000))
	{
		m_bState[_key] = !m_bState[_key];
		return true;
	}
	return false;
}

bool CKeyMgr::Key_Up(int _key)
{
	if (m_bState[_key] && !(GetAsyncKeyState(_key) & 0x8000))
	{
		m_bState[_key] = !m_bState[_key];
		return true;
	}
	return false;
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

