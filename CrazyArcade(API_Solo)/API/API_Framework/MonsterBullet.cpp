#include "stdafx.h"
#include "MonsterBullet.h"
#include "BmpMgr.h"
#include "ObjMgr.h"
#include "ScrollMgr.h"
#include "MonsterLeft.h"
#include "MonsterRight.h"
#include "MonsterUp.h"
#include "MonsterDown.h"
#include "CollisionMgr.h"

CMonsterBullet::CMonsterBullet()
	:m_iBoomSize(5)
{
}


CMonsterBullet::~CMonsterBullet()
{
	Release();
}

void CMonsterBullet::Initialize()
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
}

int CMonsterBullet::Update()
{
	if (m_bDead)
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

void CMonsterBullet::Late_Update()
{
}

void CMonsterBullet::Render(HDC _DC)
{

	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();

	if (m_dwTime + 2000 > GetTickCount()) // 물풍선 터지기 전
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


	if (m_dwTime + 2000 < GetTickCount() || m_BoomWave) // 물풍선 터진 후
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

		for (int i = 1; i <= m_iBoomSize; i++)
		{
			int num = 0;
			CObj* pTemp = Create_Pado_Left<CMonsterLeft>(i);
			CObjMgr::Get_Instance()->Add_Object(pTemp, OBJID::MONSTERLEFTPADO);

			num = CCollisionMgr::Collision_PadoAndBlock2(pTemp, CObjMgr::Get_Instance()->Get_list(OBJID::REDBLOCK)
				, CObjMgr::Get_Instance()->Get_list(OBJID::YELLOWBLOCK)
				, CObjMgr::Get_Instance()->Get_list(OBJID::HOUSE)
				, CObjMgr::Get_Instance()->Get_list(OBJID::TREE)
				, CObjMgr::Get_Instance()->Get_list(OBJID::TOWNBLOCK));
			if (num == 1)
				break;
		}

		for (int i = 1; i <= m_iBoomSize; i++)
		{

			int num = 0;
			if (581 > m_tInfo.fX + (i*40))
			{
				CObj* pTemp = Create_Pado_Right<CMonsterRight>(i);
				CObjMgr::Get_Instance()->Add_Object(pTemp, OBJID::MONSTERRIGHTPADO);

				num = CCollisionMgr::Collision_PadoAndBlock2(pTemp, CObjMgr::Get_Instance()->Get_list(OBJID::REDBLOCK)
					, CObjMgr::Get_Instance()->Get_list(OBJID::YELLOWBLOCK)
					, CObjMgr::Get_Instance()->Get_list(OBJID::HOUSE)
					, CObjMgr::Get_Instance()->Get_list(OBJID::TREE)
					, CObjMgr::Get_Instance()->Get_list(OBJID::TOWNBLOCK));
				if (num == 1)
					break;;
			}

		}

		for (int i = 1; i <= m_iBoomSize; i++)
		{
			int num = 0;
			CObj* pTemp = Create_Pado_Up<CMonsterUp>(i);
			CObjMgr::Get_Instance()->Add_Object(pTemp, OBJID::MONSTERUPPADO);

			num = CCollisionMgr::Collision_PadoAndBlock2(pTemp, CObjMgr::Get_Instance()->Get_list(OBJID::REDBLOCK)
				, CObjMgr::Get_Instance()->Get_list(OBJID::YELLOWBLOCK)
				, CObjMgr::Get_Instance()->Get_list(OBJID::HOUSE)
				, CObjMgr::Get_Instance()->Get_list(OBJID::TREE)
				, CObjMgr::Get_Instance()->Get_list(OBJID::TOWNBLOCK));
			if (num == 1)
				break;
		}

		for (int i = 1; i <= m_iBoomSize; i++)
		{
			int num = 0;
			CObj* pTemp = Create_Pado_Down<CMonsterDown>(i);
			CObjMgr::Get_Instance()->Add_Object(pTemp, OBJID::MONSTERDOWNPADO);

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

	if (m_dwTime + 2300 < GetTickCount())
	{
		m_BoomWave = false;
		m_bDead = true;
	}
}

void CMonsterBullet::Release()
{
}
