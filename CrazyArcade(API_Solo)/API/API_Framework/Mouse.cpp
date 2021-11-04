#include "stdafx.h"
#include "Mouse.h"


CMouse::CMouse()
{
}


CMouse::~CMouse()
{
	Release();
}

void CMouse::Initialize()
{
	m_tInfo.iCX = 100;
	m_tInfo.iCY = 100;

	m_eGroupID = RENDERID::UI;
}

int CMouse::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	// POINT : LONG x, LONG y의 정보를 가지고 있는 구조체.
	POINT pt = {};

	// GetCursorPos : 마우스 커서의 위치를 인자로 전달하는 구조체에 넣어주는 함수.
	// 단, 윈도우창 좌표가 아닌 전체화면 좌표로 얻어오는 것이다.
	GetCursorPos(&pt);

	// ScreenToClient : 전체 스크린 기준의 좌표를 윈도우창 좌표로 변경해주는 함수.
	ScreenToClient(g_hWnd, &pt);

	ShowCursor(FALSE);


	m_tInfo.fX = (float)pt.x;
	m_tInfo.fY = (float)pt.y;


	Update_Rect();
	return OBJ_NOEVENT;
}

void CMouse::Late_Update()
{
}

void CMouse::Render(HDC _DC)
{
	Ellipse(_DC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
}

void CMouse::Release()
{
}
