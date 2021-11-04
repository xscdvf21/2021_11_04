#include "stdafx.h"
#include "Scroll_Manager.h"

_vec3 CScroll_Manager::m_vScroll = {}; 

CScroll_Manager::CScroll_Manager()
{
}


CScroll_Manager::~CScroll_Manager()
{
}

float CScroll_Manager::Get_Scroll(SCROLL eScrollID)
{
	switch (eScrollID)
	{
	case CScroll_Manager::SCROLL_X:
		return m_vScroll.x;

	case CScroll_Manager::SCROLL_Y:
		return m_vScroll.y; 
	default:
		break;
	}
	return 0.0f;
}

void CScroll_Manager::Set_Scroll(const _vec3 & vScroll)
{
	m_vScroll += vScroll; 
}

void CScroll_Manager::Scroll_Lock()
{
	int x = 0;
	//if (0 < m_vScroll.x)
	//	m_vScroll.x = 0.f;

	if (WINCX - (TILECX * TILEX) > m_vScroll.x)
		m_vScroll.x = WINCX - (TILECX * TILEX);
}
