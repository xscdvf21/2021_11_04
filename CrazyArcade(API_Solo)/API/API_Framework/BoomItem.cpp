#include "stdafx.h"
#include "BoomItem.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"


CBoomItem::CBoomItem()
{
}


CBoomItem::~CBoomItem()
{
}

void CBoomItem::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/item/Gift1.bmp", L"Gift1"); 

	m_tInfo.fX = 200.f;
	m_tInfo.fY = 400.f;

	m_tInfo.iCX = 35;
	m_tInfo.iCY = 35;

	m_eGroupID = RENDERID::ITEM;
}

int CBoomItem::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	Update_Frame();
	Update_Rect();

	return OBJ_NOEVENT;
}

void CBoomItem::Late_Update()
{
}

void CBoomItem::Render(HDC _DC)
{

	Update_Rect();

	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(L"Gift1");

	GdiTransparentBlt(_DC
		, m_tRect.left + iScrollX, m_tRect.top
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, m_tFrame.iFrameStart * 42, m_tFrame.iFrameScene * 0
		, 42, 45
		, RGB(255, 0, 255));
}

void CBoomItem::Release()
{
}
