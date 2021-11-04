#include "stdafx.h"
#include "ScrollMgr.h"


CScrollMgr* CScrollMgr::m_pInstance = nullptr;

CScrollMgr::CScrollMgr()
	: m_fScrollX(0.f)
	, m_fScrollY(0.f)
{
}


CScrollMgr::~CScrollMgr()
{
}

void CScrollMgr::Scroll_Lock()
{
	if (0 < m_fScrollX)
		m_fScrollX = 0.f;

	if (WINCX - (TILECX * TILEX) > m_fScrollX) // WINCX ũ���� �߾��� 400�̴ϱ�.
		m_fScrollX = WINCX - (TILECX * TILEX);

	if (0 < m_fScrollY)
		m_fScrollY = 0.f;

	if (WINCY - (TILECY * TILEY) > m_fScrollY) // WINCY ũ���� �߾��� 300�̴ϱ� .
		m_fScrollY = WINCY - (TILECY * TILEY);
}
