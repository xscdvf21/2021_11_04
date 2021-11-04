#include "stdafx.h"
#include "Monster.h"
#include "ObjMgr.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
#include "KeyMgr.h"
#include "MonsterBullet.h"
#include "TileMgr.h"


CMonster::CMonster()
{
	ZeroMemory(m_tPoint, sizeof(m_tPoint));
}


CMonster::~CMonster()
{
	Release();
}

void CMonster::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/role/Monster_Left.bmp", L"Monster_Left");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/role/Monster_Right.bmp", L"Monster_Right");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/role/Monster_Up.bmp", L"Monster_Up");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/role/Monster_Down.bmp", L"Monster_Down");

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/role/MonsterDie.bmp", L"MonsterDie");

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/Plg.bmp", L"Plg");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/Reset.bmp", L"Reset");


	m_tInfo.fX = 580.f;
	m_tInfo.fY = 500.f;
	m_tInfo.iCX = 35;
	m_tInfo.iCY = 35;

	m_fSpeed = 2.f;

	m_dwTime = GetTickCount();

	m_iNum = rand();
	m_iNumSeed = m_iNum % 8;

	m_eGroupID = RENDERID::OBJECT;
}

int CMonster::Update()
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

void CMonster::Late_Update()
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

void CMonster::Render(HDC _DC)
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
		, m_tFrame.iFrameStart * 50, m_tFrame.iFrameScene * 0
		, 50, 60
		, RGB(255, 255, 255));


	//if(m_PlayerDeadTime)
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

void CMonster::Release()
{
}


void CMonster::Scene_Change()
{
	if (m_ePreScene != m_eCurScene)
	{
		switch (m_eCurScene)
		{
		case CMonster::IDLE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 8;
			m_tFrame.iFrameScene = IDLE;
			m_tFrame.dwFrameTime = 200;
			m_tFrame.dwTime = GetTickCount();
			break;
		case CMonster::WALK:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 6;
			m_tFrame.iFrameScene = WALK;
			m_tFrame.dwFrameTime = 200;
			m_tFrame.dwTime = GetTickCount();
			break;
		case CMonster::ATTACK:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iFrameScene = 1;
			m_tFrame.dwFrameTime = 200;
			m_tFrame.dwTime = GetTickCount();
			break;
		case CMonster::HIT:
			break;
		case CMonster::DEAD:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 2;
			m_tFrame.iFrameScene = 4;
			m_tFrame.dwFrameTime = 400;
			m_tFrame.dwTime = GetTickCount();
			break;
		case CMonster::DEAD_SCENE:
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

void CMonster::Moving_Check()
{


	if (m_PlayerDeadTime || CKeyMgr::Get_Instance()->Key_Down('P'))
	{
		m_pFramekey = L"MonsterDie";
		m_eCurScene = DEAD;
	}

	if	(m_iNumSeed <= 2 )
	{

		m_pFramekey = L"Monster_Left";
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

		m_pFramekey = L"Monster_Right";
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

		m_pFramekey = L"Monster_Up";
		m_eCurScene = WALK;
		m_tInfo.fY -= m_fSpeed;
		

		if (m_tInfo.fY <= 20)
		{
			m_iNum = rand();
			m_iNumSeed = m_iNum % 100;
			m_tInfo.fY = 20;

		}
	}
	else if (m_iNumSeed > 6 && m_iNumSeed <= 8) // Down
	{

		m_pFramekey = L"Monster_Down";
		m_eCurScene = WALK;
		m_tInfo.fY += m_fSpeed;

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

void CMonster::off_Set()
{
	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iOffsetX = WINCX >> 1;

	//if (iOffsetX < m_tInfo.fX + iScrollX)
	//	CScrollMgr::Get_Instance()->Set_ScrollX(iOffsetX - (m_tInfo.fX + iScrollX));
	//if (iOffsetX > m_tInfo.fX + iScrollX)
	//	CScrollMgr::Get_Instance()->Set_ScrollX(iOffsetX - (m_tInfo.fX + iScrollX));
}
