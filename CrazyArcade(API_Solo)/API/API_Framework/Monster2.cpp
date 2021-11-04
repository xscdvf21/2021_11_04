#include "stdafx.h"
#include "Monster2.h"
#include "ObjMgr.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
#include "KeyMgr.h"
#include "MonsterBullet.h"
#include "TileMgr.h"


CMonster2::CMonster2()
{
	ZeroMemory(m_tPoint, sizeof(m_tPoint));
}


CMonster2::~CMonster2()
{
	Release();
}

void CMonster2::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/role/BossLeft.bmp", L"BossLeft");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/role/BossRight.bmp", L"BossRight");


	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/Plg.bmp", L"Plg");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/Reset.bmp", L"Reset");



	m_tInfo.iCX = 35;
	m_tInfo.iCY = 35;

	m_fSpeed = 2.f;

	m_dwTime = GetTickCount();

	m_iNum = rand();
	m_iNumSeed = m_iNum % 8;

	m_eGroupID = RENDERID::OBJECT;
}

int CMonster2::Update()
{
	if (m_PlayerDeadTime)
		return OBJ_DEAD;

	//if (m_PlayerDeadTime)
	//{
	//	Scene_Change();
	//	Update_Frame();
	//	Update_Rect();
	//}

	Moving_Check();
	off_Set();
	Scene_Change();
	Update_Frame();
	Update_Rect();

	if (m_dwTime + 1000 < GetTickCount())
	{
		m_iNum = rand();
		m_iNumSeed = m_iNum % 100;
		m_dwTime = GetTickCount();
	}
	return OBJ_NOEVENT;
}

void CMonster2::Late_Update()
{
	//float x = (float)(m_tInfo.iCX >> 1);
	//float y = (float)(m_tInfo.iCY >> 1);
	//float fDia = sqrtf(x * x + y * y);

	//m_tPoint[0].x = (LONG)((m_tInfo.iCX >> 1) + cosf((135.f + m_fAngle) * PI / 180.f) * fDia);
	//m_tPoint[0].y = (LONG)((m_tInfo.iCY >> 1) - sinf((135.f + m_fAngle) * PI / 180.f) * fDia);

	//m_tPoint[1].x = (LONG)((m_tInfo.iCX >> 1) + cosf((45.f + m_fAngle) * PI / 180.f) * fDia);
	//m_tPoint[1].y = (LONG)((m_tInfo.iCY >> 1) - sinf((45.f + m_fAngle) * PI / 180.f) * fDia);

	//m_tPoint[2].x = (LONG)((m_tInfo.iCX >> 1) + cosf((225.f + m_fAngle) * PI / 180.f) * fDia);
	//m_tPoint[2].y = (LONG)((m_tInfo.iCY >> 1) - sinf((225.f + m_fAngle) * PI / 180.f) * fDia);
}

void CMonster2::Render(HDC _DC)
{
	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	//HDC hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(L"Monster");
	//HDC hPlgDC = CBmpMgr::Get_Instance()->Find_Bmp(L"Plg");
	//HDC hResetDC = CBmpMgr::Get_Instance()->Find_Bmp(L"Reset");

	HDC	 hMonsterDC = CBmpMgr::Get_Instance()->Find_Bmp(m_pFramekey);

	/*PlgBlt(hPlgDC, m_tPoint, hMemDC, 0, 0, 300, 300, NULL, NULL, NULL);*/
	GdiTransparentBlt(_DC
		, m_tRect.left + iScrollX, m_tRect.top
		, m_tInfo.iCX, m_tInfo.iCY
		, hMonsterDC
		, m_tFrame.iFrameStart * 60, m_tFrame.iFrameScene * 0
		, 60, 60
		, RGB(255, 255, 255));


	//if (m_PlayerDeadTime)
	//{
	//	HDC	 hMonsterDC = CBmpMgr::Get_Instance()->Find_Bmp(L"MonsterDie");

	//	/*PlgBlt(hPlgDC, m_tPoint, hMemDC, 0, 0, 300, 300, NULL, NULL, NULL);*/
	//	GdiTransparentBlt(_DC
	//		, m_tRect.left + iScrollX, m_tRect.top
	//		, m_tInfo.iCX, m_tInfo.iCY
	//		, hMonsterDC
	//		, m_tFrame.iFrameStart * 60, m_tFrame.iFrameScene * 0
	//		, 60, 60
	//		, RGB(255, 255, 255));
	//}
	/*BitBlt(hPlgDC, 0, 0, 300, 300, hResetDC, 0, 0, SRCCOPY);*/
}

void CMonster2::Release()
{
}


void CMonster2::Scene_Change()
{
	if (m_ePreScene != m_eCurScene)
	{
		switch (m_eCurScene)
		{
		case CMonster2::IDLE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 8;
			m_tFrame.iFrameScene = IDLE;
			m_tFrame.dwFrameTime = 200;
			m_tFrame.dwTime = GetTickCount();
			break;
		case CMonster2::WALK:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iFrameScene = WALK;
			m_tFrame.dwFrameTime = 200;
			m_tFrame.dwTime = GetTickCount();
			break;
		case CMonster2::ATTACK:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iFrameScene = 1;
			m_tFrame.dwFrameTime = 200;
			m_tFrame.dwTime = GetTickCount();
			break;
		case CMonster2::HIT:
			break;
		case CMonster2::DEAD:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 2;
			m_tFrame.iFrameScene = 4;
			m_tFrame.dwFrameTime = 400;
			m_tFrame.dwTime = GetTickCount();
			break;
		case CMonster2::DEAD_SCENE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 8;
			m_tFrame.iFrameScene = 5;
			m_tFrame.dwFrameTime = 150;
			m_tFrame.dwTime = GetTickCount();
			break;
		}
		m_ePreScene = m_eCurScene;
	}
}

void CMonster2::Moving_Check()
{


	if (m_PlayerDeadTime || CKeyMgr::Get_Instance()->Key_Down('P'))
	{
		m_pFramekey = L"MonsterDie";
		m_eCurScene = DEAD;
	}

	if (m_iNumSeed <= 2)
	{

		m_pFramekey = L"BossLeft";
		m_eCurScene = WALK;
		m_tInfo.fX -= m_fSpeed;
		if (m_tInfo.fX <= 20)
		{
			m_iNum = rand();
			m_iNumSeed = m_iNum % 100;
			m_tInfo.fX = 20;
		}
	}
	else if (m_iNumSeed > 2 && m_iNumSeed <= 4)
	{

		m_pFramekey = L"BossRight";
		m_eCurScene = WALK;
		m_tInfo.fX += m_fSpeed;

		if (m_tInfo.fX >= 580)
		{
			m_iNum = rand();
			m_iNumSeed = m_iNum % 100;
			m_tInfo.fX = 580;
		}
	}
	else if (m_iNumSeed > 4 && m_iNumSeed <= 6) // Up 
	{
		if (m_tInfo.fX >= 300)
		{
			m_pFramekey = L"BossLeft";
			m_eCurScene = WALK;
			m_tInfo.fY -= m_fSpeed;
		}
		else if (m_tInfo.fX <= 300)
		{
			m_pFramekey = L"BossRight";
			m_eCurScene = WALK;
			m_tInfo.fY -= m_fSpeed;
		}

		//m_pFramekey = L"BossLeft";
		//m_eCurScene = WALK;
		//m_tInfo.fY -= m_fSpeed;

		if (m_tInfo.fY <= 20)
		{
			m_iNum = rand();
			m_iNumSeed = m_iNum % 100;
			m_tInfo.fY = 20;

		}
	}
	else if (m_iNumSeed > 6 && m_iNumSeed <= 8) // Down
	{


		if (m_tInfo.fX >= 300)
		{
			m_pFramekey = L"BossLeft";
			m_eCurScene = WALK;
			m_tInfo.fY += m_fSpeed;
		}
		else if (m_tInfo.fX <= 300)
		{
			m_pFramekey = L"BossRight";
			m_eCurScene = WALK;
			m_tInfo.fY += m_fSpeed;
		}
		//m_pFramekey = L"Monster_Down";
		//m_eCurScene = WALK;
		//m_tInfo.fY += m_fSpeed;

		if (m_tInfo.fY >= 500)
		{
			m_iNum = rand();
			m_iNumSeed = m_iNum % 100;
			m_tInfo.fY = 500;
		}

	}

	else if (m_iNumSeed > 8 && m_iNumSeed <= 100)
	{

		m_pTemp = CTileMgr::Get_Instance()->Get_Index(m_tInfo.fX, m_tInfo.fY);
		float fBulletx = m_pTemp->Get_Info().fX;
		float fBullety = m_pTemp->Get_Info().fY;
		if (!m_BoomAndPlayer)
		{
			CObjMgr::Get_Instance()->Add_Object(Create_Bullet<CMonsterBullet>(fBulletx, fBullety), OBJID::MONSTERBULLET);
		}
		m_iNum = rand();
		m_iNumSeed = m_iNum % 8;
	}
	m_BoomAndPlayer = false;

}

void CMonster2::off_Set()
{
	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iOffsetX = WINCX >> 1;

	//if (iOffsetX < m_tInfo.fX + iScrollX)
	//	CScrollMgr::Get_Instance()->Set_ScrollX(iOffsetX - (m_tInfo.fX + iScrollX));
	//if (iOffsetX > m_tInfo.fX + iScrollX)
	//	CScrollMgr::Get_Instance()->Set_ScrollX(iOffsetX - (m_tInfo.fX + iScrollX));
}
