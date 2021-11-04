#include "stdafx.h"
#include "BossMonster.h"
#include "ObjMgr.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
#include "KeyMgr.h"
#include "MonsterBullet.h"
#include "TileMgr.h"



CBossMonster::CBossMonster()
	:m_life(100)
{
	ZeroMemory(m_tPoint, sizeof(m_tPoint));
}


CBossMonster::~CBossMonster()
{
	Release();
}

void CBossMonster::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/role/King_Left.bmp", L"King_Left");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/role/King_Right.bmp", L"King_Right");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/role/Boss_Hp1.bmp", L"Boss_Hp1");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/role/Boss_Hp2.bmp", L"Boss_Hp2");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/role/Boss_Hp3.bmp", L"Boss_Hp3");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/role/Boss_Hp4.bmp", L"Boss_Hp4");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/role/Boss_Hp5.bmp", L"Boss_Hp5");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/role/Boss_Hp6.bmp", L"Boss_Hp6");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/role/Bosslife.bmp", L"Bosslife");

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/Plg.bmp", L"Plg");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/Reset.bmp", L"Reset");



	m_tInfo.iCX = 150;
	m_tInfo.iCY = 150;

	m_fSpeed = 2.f;

	m_dwTime = GetTickCount();

	m_iNum = rand();
	m_iNumSeed = m_iNum % 8;

	m_eGroupID = RENDERID::BOSS;
}

int CBossMonster::Update()
{
	if (m_life <= 0)
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
		m_iNumSeed = m_iNum % 12;
		m_dwTime = GetTickCount();
	}
	return OBJ_NOEVENT;
}

void CBossMonster::Late_Update()
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

void CBossMonster::Render(HDC _DC)
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
		, m_tFrame.iFrameStart * 80, m_tFrame.iFrameScene * 0
		, 80, 80
		, RGB(255, 255, 255));


	HDC	 hMonsterDC3 = CBmpMgr::Get_Instance()->Find_Bmp(L"Bosslife");

	/*PlgBlt(hPlgDC, m_tPoint, hMemDC, 0, 0, 300, 300, NULL, NULL, NULL);*/
	GdiTransparentBlt(_DC
		, 630, 230
		, 40, 40
		, hMonsterDC3
		, m_tFrame.iFrameStart * 0, m_tFrame.iFrameScene * 0
		, 60, 60
		, RGB(255, 255, 255));
	
		
	if (m_life <= 100 && m_life > 80) 
	{
		HDC	 hMonsterDC2 = CBmpMgr::Get_Instance()->Find_Bmp(L"Boss_Hp1");

		/*PlgBlt(hPlgDC, m_tPoint, hMemDC, 0, 0, 300, 300, NULL, NULL, NULL);*/
		GdiTransparentBlt(_DC
			, m_tRect.left + 20, m_tRect.top + 40
			, 130, 10
			, hMonsterDC2
			, m_tFrame.iFrameStart * 0, m_tFrame.iFrameScene * 0
			, 136, 21
			, RGB(255, 255, 255));

		HDC	 hMonsterDC3 = CBmpMgr::Get_Instance()->Find_Bmp(L"Boss_Hp1");

		/*PlgBlt(hPlgDC, m_tPoint, hMemDC, 0, 0, 300, 300, NULL, NULL, NULL);*/
		GdiTransparentBlt(_DC
			, 675, 235
			, 70, 30
			, hMonsterDC2
			, m_tFrame.iFrameStart * 0, m_tFrame.iFrameScene * 0
			, 136, 21
			, RGB(255, 255, 255));
	}

	if (m_life <= 80 && m_life > 60)
	{
		HDC	 hMonsterDC2 = CBmpMgr::Get_Instance()->Find_Bmp(L"Boss_Hp2");

		/*PlgBlt(hPlgDC, m_tPoint, hMemDC, 0, 0, 300, 300, NULL, NULL, NULL);*/
		GdiTransparentBlt(_DC
			, m_tRect.left + 20, m_tRect.top + 40
			, 130, 10
			, hMonsterDC2
			, m_tFrame.iFrameStart * 0, m_tFrame.iFrameScene * 0
			, 136, 21
			, RGB(255, 255, 255));

		HDC	 hMonsterDC3 = CBmpMgr::Get_Instance()->Find_Bmp(L"Boss_Hp2");

		/*PlgBlt(hPlgDC, m_tPoint, hMemDC, 0, 0, 300, 300, NULL, NULL, NULL);*/
		GdiTransparentBlt(_DC
			, 675, 235
			, 70, 30
			, hMonsterDC2
			, m_tFrame.iFrameStart * 0, m_tFrame.iFrameScene * 0
			, 136, 21
			, RGB(255, 255, 255));
	}

	if (m_life <= 60 && m_life > 40)
	{
		HDC	 hMonsterDC2 = CBmpMgr::Get_Instance()->Find_Bmp(L"Boss_Hp3");

		/*PlgBlt(hPlgDC, m_tPoint, hMemDC, 0, 0, 300, 300, NULL, NULL, NULL);*/
		GdiTransparentBlt(_DC
			, m_tRect.left + 20, m_tRect.top + 40
			, 130, 10
			, hMonsterDC2
			, m_tFrame.iFrameStart * 0, m_tFrame.iFrameScene * 0
			, 136, 21
			, RGB(255, 255, 255));

		HDC	 hMonsterDC3 = CBmpMgr::Get_Instance()->Find_Bmp(L"Boss_Hp3");

		/*PlgBlt(hPlgDC, m_tPoint, hMemDC, 0, 0, 300, 300, NULL, NULL, NULL);*/
		GdiTransparentBlt(_DC
			, 675, 235
			, 70, 30
			, hMonsterDC2
			, m_tFrame.iFrameStart * 0, m_tFrame.iFrameScene * 0
			, 136, 21
			, RGB(255, 255, 255));
	}

	if (m_life <= 40 && m_life > 20)
	{
		HDC	 hMonsterDC2 = CBmpMgr::Get_Instance()->Find_Bmp(L"Boss_Hp4");

		/*PlgBlt(hPlgDC, m_tPoint, hMemDC, 0, 0, 300, 300, NULL, NULL, NULL);*/
		GdiTransparentBlt(_DC
			, m_tRect.left + 20, m_tRect.top + 40
			, 130, 10
			, hMonsterDC2
			, m_tFrame.iFrameStart * 0, m_tFrame.iFrameScene * 0
			, 136, 21
			, RGB(255, 255, 255));

		HDC	 hMonsterDC3 = CBmpMgr::Get_Instance()->Find_Bmp(L"Boss_Hp4");

		/*PlgBlt(hPlgDC, m_tPoint, hMemDC, 0, 0, 300, 300, NULL, NULL, NULL);*/
		GdiTransparentBlt(_DC
			, 675, 235
			, 70, 30
			, hMonsterDC2
			, m_tFrame.iFrameStart * 0, m_tFrame.iFrameScene * 0
			, 136, 21
			, RGB(255, 255, 255));
	}
	if (m_life <= 20 && m_life > 1)
	{
		HDC	 hMonsterDC2 = CBmpMgr::Get_Instance()->Find_Bmp(L"Boss_Hp5");

		/*PlgBlt(hPlgDC, m_tPoint, hMemDC, 0, 0, 300, 300, NULL, NULL, NULL);*/
		GdiTransparentBlt(_DC
			, m_tRect.left + 20, m_tRect.top + 40
			, 130, 10
			, hMonsterDC2
			, m_tFrame.iFrameStart * 0, m_tFrame.iFrameScene * 0
			, 136, 21
			, RGB(255, 255, 255));

		HDC	 hMonsterDC3 = CBmpMgr::Get_Instance()->Find_Bmp(L"Boss_Hp5");

		/*PlgBlt(hPlgDC, m_tPoint, hMemDC, 0, 0, 300, 300, NULL, NULL, NULL);*/
		GdiTransparentBlt(_DC
			, 675, 235
			, 70, 30
			, hMonsterDC2
			, m_tFrame.iFrameStart * 0, m_tFrame.iFrameScene * 0
			, 136, 21
			, RGB(255, 255, 255));
	}
	//TCHAR	szBuff[32] = L"";
	//swprintf_s(szBuff, L"보스 피: %d", m_life);
	//TextOut(_DC, m_tInfo.fX-30, m_tInfo.fY-70, szBuff, lstrlen(szBuff));


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

void CBossMonster::Release()
{
}


void CBossMonster::Scene_Change()
{
	if (m_ePreScene != m_eCurScene)
	{
		switch (m_eCurScene)
		{
		case CBossMonster::IDLE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 8;
			m_tFrame.iFrameScene = IDLE;
			m_tFrame.dwFrameTime = 200;
			m_tFrame.dwTime = GetTickCount();
			break;
		case CBossMonster::WALK:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 4;
			m_tFrame.iFrameScene = WALK;
			m_tFrame.dwFrameTime = 400;
			m_tFrame.dwTime = GetTickCount();
			break;
		case CBossMonster::ATTACK:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iFrameScene = 1;
			m_tFrame.dwFrameTime = 200;
			m_tFrame.dwTime = GetTickCount();
			break;
		case CBossMonster::HIT:
			break;
		case CBossMonster::DEAD:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 2;
			m_tFrame.iFrameScene = 4;
			m_tFrame.dwFrameTime = 400;
			m_tFrame.dwTime = GetTickCount();
			break;
		case CBossMonster::DEAD_SCENE:
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



void CBossMonster::Moving_Check()
{
	if (m_PlayerDeadTime) //  파도와 충돌을 하면,
	{
		m_life--;
		m_PlayerDeadTime = false;
	}

	if (m_PlayerDeadTime || CKeyMgr::Get_Instance()->Key_Down('P'))
	{
		m_pFramekey = L"MonsterDie";
		m_eCurScene = DEAD;
	}

	if (m_iNumSeed <= 2)
	{

		m_pFramekey = L"King_Left";
		m_eCurScene = WALK;
		m_tInfo.fX -= m_fSpeed;
		if (m_tInfo.fX <= 50)
		{
			m_iNum = rand();
			m_iNumSeed = m_iNum % 12;
			m_tInfo.fX = 50;
		}
	}
	else if (m_iNumSeed > 2 && m_iNumSeed <= 4)
	{

		m_pFramekey = L"King_Right";
		m_eCurScene = WALK;
		m_tInfo.fX += m_fSpeed;

		if (m_tInfo.fX >= 550)
		{
			m_iNum = rand();
			m_iNumSeed = m_iNum % 12;
			m_tInfo.fX = 550;
		}
	}
	else if (m_iNumSeed > 4 && m_iNumSeed <= 6) // Up 
	{
		if (m_tInfo.fX >= 300)
		{
			m_pFramekey = L"King_Left";
			m_eCurScene = WALK;
			m_tInfo.fY -= m_fSpeed;
		}
		else if (m_tInfo.fX <= 300)
		{
			m_pFramekey = L"King_Right";
			m_eCurScene = WALK;
			m_tInfo.fY -= m_fSpeed;
		}

		//m_pFramekey = L"BossLeft";
		//m_eCurScene = WALK;
		//m_tInfo.fY -= m_fSpeed;

		if (m_tInfo.fY <= 50)
		{
			m_iNum = rand();
			m_iNumSeed = m_iNum % 12;
			m_tInfo.fY = 50;

		}
	}
	else if (m_iNumSeed > 6 && m_iNumSeed <= 8) // Down
	{


		if (m_tInfo.fX >= 300)
		{
			m_pFramekey = L"King_Left";
			m_eCurScene = WALK;
			m_tInfo.fY += m_fSpeed;
		}
		else if (m_tInfo.fX <= 300)
		{
			m_pFramekey = L"King_Right";
			m_eCurScene = WALK;
			m_tInfo.fY += m_fSpeed;
		}
		//m_pFramekey = L"Monster_Down";
		//m_eCurScene = WALK;
		//m_tInfo.fY += m_fSpeed;

		if (m_tInfo.fY >= 470)
		{
			m_iNum = rand();
			m_iNumSeed = m_iNum % 12;
			m_tInfo.fY = 470;
		}

	}
	else if (m_iNumSeed == 11)
	{

		m_pTemp = CTileMgr::Get_Instance()->Get_Index(m_tInfo.fX, m_tInfo.fY);
		float fBulletx = m_pTemp->Get_Info().fX;
		float fBullety = m_pTemp->Get_Info().fY;

		CObjMgr::Get_Instance()->Add_Object(Create_Bullet<CMonsterBullet>(fBulletx + 0, fBullety + 80), OBJID::MONSTERBULLET);
		CObjMgr::Get_Instance()->Add_Object(Create_Bullet<CMonsterBullet>(fBulletx + 0, fBullety - 80), OBJID::MONSTERBULLET);
		CObjMgr::Get_Instance()->Add_Object(Create_Bullet<CMonsterBullet>(fBulletx + 80, fBullety + 0), OBJID::MONSTERBULLET);
		CObjMgr::Get_Instance()->Add_Object(Create_Bullet<CMonsterBullet>(fBulletx - 80, fBullety + 0), OBJID::MONSTERBULLET);
		CObjMgr::Get_Instance()->Add_Object(Create_Bullet<CMonsterBullet>(fBulletx + 80, fBullety + 80), OBJID::MONSTERBULLET);
		CObjMgr::Get_Instance()->Add_Object(Create_Bullet<CMonsterBullet>(fBulletx - 80, fBullety - 80), OBJID::MONSTERBULLET);

		m_iNum = rand();
		m_iNumSeed = m_iNum % 8;
	}
	else if (m_iNumSeed > 8 && m_iNumSeed <= 10)
	{

		m_pTemp = CTileMgr::Get_Instance()->Get_Index(m_tInfo.fX, m_tInfo.fY);
		float fBulletx = m_pTemp->Get_Info().fX;
		float fBullety = m_pTemp->Get_Info().fY;
		if (!m_BoomAndPlayer)
		{
			CObjMgr::Get_Instance()->Add_Object(Create_Bullet<CMonsterBullet>(fBulletx, fBullety), OBJID::MONSTERBULLET);
		}
		m_iNum = rand();
		m_iNumSeed = m_iNum % 12;
	}
	m_BoomAndPlayer = false;

}

void CBossMonster::off_Set()
{
	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iOffsetX = WINCX >> 1;

	//if (iOffsetX < m_tInfo.fX + iScrollX)
	//	CScrollMgr::Get_Instance()->Set_ScrollX(iOffsetX - (m_tInfo.fX + iScrollX));
	//if (iOffsetX > m_tInfo.fX + iScrollX)
	//	CScrollMgr::Get_Instance()->Set_ScrollX(iOffsetX - (m_tInfo.fX + iScrollX));
}
