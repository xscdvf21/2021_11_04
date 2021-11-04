#include "stdafx.h"
#include "Key_Manager.h"

CKey_Manager*  CKey_Manager::m_pInstance = nullptr;

CKey_Manager::CKey_Manager()
{
}


CKey_Manager::~CKey_Manager()
{
}

bool CKey_Manager::Key_Pressing(int _key)
{
	if (GetAsyncKeyState(_key) & 0x8000)
		return true;
	return false;
}

bool CKey_Manager::Key_Down(int _key)
{
	if (!m_bState[_key] && (GetAsyncKeyState(_key) & 0x8000))
	{
		m_bState[_key] = !m_bState[_key];
		return true;
	}
	return false;
}

bool CKey_Manager::Key_Up(int _key)
{
	if (m_bState[_key] && !(GetAsyncKeyState(_key) & 0x8000))
	{
		m_bState[_key] = !m_bState[_key];
		return true;
	}
	return false;
}

void CKey_Manager::Key_Update()
{
	for (int i = 0; i < VK_MAX; ++i)
	{
		if (m_bState[i] && !(GetAsyncKeyState(i) & 0x8000))
			m_bState[i] = !m_bState[i];
		if (!m_bState[i] && (GetAsyncKeyState(i) & 0x8000))
			m_bState[i] = !m_bState[i];
	}
}
