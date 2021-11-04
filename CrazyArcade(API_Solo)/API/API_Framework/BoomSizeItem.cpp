#include "stdafx.h"
#include "BoomSizeItem.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"


CBoomSizeItem::CBoomSizeItem()
{
}


CBoomSizeItem::~CBoomSizeItem()
{
}

void CBoomSizeItem::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/item/Gift3.bmp", L"Gift3");

	m_tInfo.fX = 200.f;
	m_tInfo.fY = 400.f;

	m_tInfo.iCX = 35;
	m_tInfo.iCY = 35;

	m_eGroupID = RENDERID::ITEM;
}

int CBoomSizeItem::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	Update_Frame();
	Update_Rect();

	return OBJ_NOEVENT;
}

void CBoomSizeItem::Late_Update()
{
}

void CBoomSizeItem::Render(HDC _DC)
{
	Update_Rect();

	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(L"Gift3");

	GdiTransparentBlt(_DC
		, m_tRect.left + iScrollX, m_tRect.top
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, m_tFrame.iFrameStart * 42, m_tFrame.iFrameScene * 0
		, 42, 45
		, RGB(255, 0, 255));
}


void CBoomSizeItem::Release()
{
}
