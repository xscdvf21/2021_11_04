#include "stdafx.h"
#include "Bullet.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
#include "ObjMgr.h"
#include "LeftBullet.h"
#include "RightBullet.h"
#include "UpBullet.h"
#include "DownBullet.h"
#include "Player.h"
#include "CollisionMgr.h"
#include "SoundMgr.h"
CBullet::CBullet()
	: m_eDir(BULLET::END), m_boomcheck(false), m_iBoomSize(1)
{
}


CBullet::~CBullet()
{
	Release();
}

void CBullet::Initialize()
{

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/effect/BlueBub.bmp", L"Boom_Popo");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/effect/Boomcenter.bmp", L"Boom_center");


	m_dwTime = GetTickCount();

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 3;
	m_tFrame.dwFrameTime = 200;
	m_tFrame.dwTime = GetTickCount();

	m_tInfo.iCX = 40;
	m_tInfo.iCY = 40;

	m_fSpeed = 10.f;

	m_eGroupID = RENDERID::OBJECT;

	Update_Rect();
}

int CBullet::Update()
{
	if (m_bDead )
	{
		return OBJ_DEAD;
	}


	Update_Frame();
	Update_Rect();

  	if (m_PadoItem)
	{
		m_iBoomSize++;
		m_PadoItem = false;
	}

	return OBJ_NOEVENT;
 }

void CBullet::Late_Update()
{
	//if(m_tRect.top <= 100 || m_tRect.left <= 100
	//	|| m_tRect.bottom >= WINCY - 100 || m_tRect.right >= WINCX - 100)
	//	m_bDead = true;


}


void CBullet::Render(HDC _DC)
{

	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();

	/*Ellipse(_DC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);*/

	
	if (m_dwTime + 1300 < GetTickCount())
		CSoundMgr::Get_Instance()->PlaySound(L"BoomSound.mp3", CSoundMgr::PLAYER);

	if (m_dwTime + 3000 > GetTickCount()) // 물풍선 터지기 전
	{

		HDC hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(L"Boom_Popo");

		GdiTransparentBlt(_DC
			, m_tRect.left + iScrollX, m_tRect.top
			, m_tInfo.iCX, m_tInfo.iCY
			, hMemDC
			, m_tFrame.iFrameStart * 46, m_tFrame.iFrameScene * 0
			, 46, 46
			, RGB(0, 0, 0));


	 
			
	}

	
	if (m_dwTime + 3000 < GetTickCount() || m_BoomWave) // 물풍선 터진 후
	{


		HDC hMemDC_center = CBmpMgr::Get_Instance()->Find_Bmp(L"Boom_center");

		if (L"Boom_center")
		{
			GdiTransparentBlt(_DC
				, m_tRect.left, m_tRect.top
				, m_tInfo.iCX, m_tInfo.iCY
				, hMemDC_center
				, m_tFrame.iFrameStart * 40, m_tFrame.iFrameScene * 0
				, 40, 40
				, RGB(0, 0, 0));
		}

		for (int i = 1; i <= CPlayer::m_BoomSize; i++)
		{
			int num = 0;
			CObj* pTemp = Create_Pado_Left<CLeftBullet>(i);
			CObjMgr::Get_Instance()->Add_Object(pTemp, OBJID::LEFTPADO);

			num = CCollisionMgr::Collision_PadoAndBlock2(pTemp, CObjMgr::Get_Instance()->Get_list(OBJID::REDBLOCK)
				, CObjMgr::Get_Instance()->Get_list(OBJID::YELLOWBLOCK)
				, CObjMgr::Get_Instance()->Get_list(OBJID::HOUSE)
				, CObjMgr::Get_Instance()->Get_list(OBJID::TREE)
				, CObjMgr::Get_Instance()->Get_list(OBJID::TOWNBLOCK));

			if (num == 1)
				break;

		}


		for (int i = 1; i <= CPlayer::m_BoomSize; i++)
		{
			if (581 > m_tInfo.fX + (i * 40))
			{
			
			int num = 0;
			CObj* pTemp = Create_Pado_Right<CRightBullet>(i);
			CObjMgr::Get_Instance()->Add_Object(pTemp, OBJID::RIGHTPADO);

			num = CCollisionMgr::Collision_PadoAndBlock2(pTemp, CObjMgr::Get_Instance()->Get_list(OBJID::REDBLOCK)
				, CObjMgr::Get_Instance()->Get_list(OBJID::YELLOWBLOCK)
				, CObjMgr::Get_Instance()->Get_list(OBJID::HOUSE)
				, CObjMgr::Get_Instance()->Get_list(OBJID::TREE)
				, CObjMgr::Get_Instance()->Get_list(OBJID::TOWNBLOCK));



			if (num == 1)
				break;
			}
	}

		for (int i = 1; i <= CPlayer::m_BoomSize; i++)
		{

			int num = 0;
			CObj* pTemp = Create_Pado_Up<CUpBullet>(i);
			CObjMgr::Get_Instance()->Add_Object(pTemp, OBJID::UPPADO);
			num = CCollisionMgr::Collision_PadoAndBlock2(pTemp, CObjMgr::Get_Instance()->Get_list(OBJID::REDBLOCK)
				, CObjMgr::Get_Instance()->Get_list(OBJID::YELLOWBLOCK)
				, CObjMgr::Get_Instance()->Get_list(OBJID::HOUSE)
				, CObjMgr::Get_Instance()->Get_list(OBJID::TREE)
				, CObjMgr::Get_Instance()->Get_list(OBJID::TOWNBLOCK));
			if (num == 1)
				break;
			//if (num2 == 1)
			//	break;
		}

		for (int i = 1; i <= CPlayer::m_BoomSize; i++)
		{
			int num = 0;
			CObj* pTemp = Create_Pado_Down<CDownBullet>(i);
			CObjMgr::Get_Instance()->Add_Object(pTemp, OBJID::DOWNPADO);

			num = CCollisionMgr::Collision_PadoAndBlock2(pTemp, CObjMgr::Get_Instance()->Get_list(OBJID::REDBLOCK)
				, CObjMgr::Get_Instance()->Get_list(OBJID::YELLOWBLOCK)
				, CObjMgr::Get_Instance()->Get_list(OBJID::HOUSE)
				, CObjMgr::Get_Instance()->Get_list(OBJID::TREE)
				, CObjMgr::Get_Instance()->Get_list(OBJID::TOWNBLOCK));
			if (num == 1)
				break;
		}
		m_bDead = true;
	}

		if (m_dwTime + 3300 < GetTickCount())
		{
			m_BoomWave = false;
			m_bDead = true;
		}

}

void CBullet::Release()
{
}


void CBullet::Scene_Change()
{



}