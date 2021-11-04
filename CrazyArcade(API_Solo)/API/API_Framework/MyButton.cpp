#include "stdafx.h"
#include "MyButton.h"
#include "BmpMgr.h"
#include "KeyMgr.h"
#include "SceneMgr.h"


CMyButton::CMyButton()
{
}


CMyButton::~CMyButton()
{
	Release();
}

void CMyButton::Initialize()
{
	m_tInfo.iCX = 100;
	m_tInfo.iCY = 100;

	m_eGroupID = RENDERID::UI;
}

int CMyButton::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	Update_Rect();
	return OBJ_NOEVENT;
}

void CMyButton::Late_Update()
{
	POINT pt = {};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);
	//ShowCursor(FALSE);

	if (PtInRect(&m_tRect, pt))
	{
		if (CKeyMgr::Get_Instance()->Key_Down(VK_LBUTTON))
		{
			if (!lstrcmp(m_pFramekey, L"Start"))
			{
				CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::STAGE);
				return;
			}
			else if (!lstrcmp(m_pFramekey, L"Edit"))
			{
				CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::EDIT);
				return;
			}
			else if (!lstrcmp(m_pFramekey, L"Exit"))
			{
				DestroyWindow(g_hWnd);
				return;
			}
		}
		m_iDrawID = 1;
	}
	else
		m_iDrawID = 0;
}

void CMyButton::Render(HDC _DC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(m_pFramekey);

	GdiTransparentBlt(_DC, m_tRect.left, m_tRect.top
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, m_iDrawID * 150, 0
		, 150, 150
		, RGB(255, 255, 255));
}

void CMyButton::Release()
{
}
