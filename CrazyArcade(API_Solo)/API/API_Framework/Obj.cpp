#include "stdafx.h"
#include "Obj.h"

CObj::CObj()
	: m_fSpeed(0.f), m_bDead(false), m_fAngle(0.f), m_pTarget(nullptr), m_bStart(true), m_bPopo(true)
	, m_pFramekey(nullptr), m_iDrawID(0), m_eGroupID(RENDERID::END), m_SpeedItem(false), m_PadoItem(false)
	, m_BoomItem(false), m_iBoomCount(1), m_iBoomCountMax(1), m_bBoomCheck(false), m_PadoAndBlock(false)
	, m_PlayerDeadTime(false), m_BoomWave(false), m_BoomAndPlayer(false)
{
	ZeroMemory(&m_tInfo, sizeof(m_tInfo));
	ZeroMemory(&m_tRect, sizeof(m_tRect));
	ZeroMemory(&m_tFrame, sizeof(m_tFrame));
}


CObj::~CObj()
{
}

void CObj::Update_Rect()

{
	m_tRect.left = (LONG)(m_tInfo.fX - (m_tInfo.iCX >> 1));
	m_tRect.right = (LONG)(m_tInfo.fX + (m_tInfo.iCX >> 1));
	m_tRect.top = (LONG)(m_tInfo.fY - (m_tInfo.iCY >> 1));
	m_tRect.bottom = (LONG)(m_tInfo.fY + (m_tInfo.iCY >> 1));
}

void CObj::Update_Frame()
{
	if (m_tFrame.dwTime + m_tFrame.dwFrameTime < GetTickCount())
	{
		m_tFrame.dwTime = GetTickCount();

		++m_tFrame.iFrameStart;
		if (m_tFrame.iFrameStart >= m_tFrame.iFrameEnd)
			m_tFrame.iFrameStart = 0;
	}
}
