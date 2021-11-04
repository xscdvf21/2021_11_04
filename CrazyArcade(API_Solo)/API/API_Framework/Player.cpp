#include "stdafx.h"
#include "Player.h"
#include "Shield.h"
#include "ScrewBullet.h"
#include "ObjMgr.h"
#include "GuideBullet.h"
#include "LineMgr.h"
#include "KeyMgr.h"
#include "ScrollMgr.h"
#include "BmpMgr.h"
#include "BoomPopo.h"
#include "Bullet.h"
#include "CollisionMgr.h"
#include "TileMgr.h"
#include "SoundMgr.h"
int		CPlayer::m_BoomSize = 1;
int		CPlayer::m_BoomCount = 1;
float	CPlayer::m_PlayerSpeed = 2.f;

CPlayer::CPlayer()
	: m_fPosinDis(0.f), m_bJump(false), m_fJumpPower(0.f)
	, m_fJumpTime(0.f), m_fJumpStart(0.f), m_bJumpStart(true)
	, m_ePreScene(END), m_eCurScene(END), m_bStretch(true)
	, m_bBoomCheck(false), m_pPlayerDel(false)
{
	ZeroMemory(&m_tPosin, sizeof(m_tPosin));
}


CPlayer::~CPlayer()
{
	Release();
}

void CPlayer::Initialize()
{

	//CSoundMgr::Get_Instance()->StopAll();
	//CSoundMgr::Get_Instance()->Initialize();

	
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/role/Bazzi_Up.bmp", L"Bazzi_Up");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/role/Bazzi_Down.bmp", L"Bazzi_Down");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/role/Bazzi_Right.bmp", L"Bazzi_Right");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/role/Bazzi_Left.bmp", L"Bazzi_Left");



	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/role/Up.bmp", L"Up");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/role/Down.bmp", L"Down");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/role/Right.bmp", L"Right");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/role/Left.bmp", L"Left");

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/role/Die.bmp", L"Bazzi_Die");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/role/Characbub0.bmp", L"Characbub0"); //플레이어 죽음
	m_tInfo.fX = 20.f;
	m_tInfo.fY = 20.f;
	m_tInfo.iCX = 35;
	m_tInfo.iCY = 35;

	m_fSpeed = 3.f;

	//m_fPosinDis = 100.f;

	//m_fJumpPower = 50.f;

	m_eCurScene = WALK;
	m_pFramekey = L"Down";

	m_eGroupID = RENDERID::OBJECT;
}

int CPlayer::Update()
{


	if (m_pPlayerDel)
	{
		return OBJ_DEAD;
	}

	if (m_bDead)
	{
		/*return OBJ_DEAD;*/
		Scene_Change();
		Update_Frame();
		Update_Rect();
	}
	else 
	{
		Key_Check();
		//Jumping();
		OffSet();
		Scene_Change();
		Update_Frame();
		Update_Rect();

		if (m_SpeedItem && m_PlayerSpeed <= 8) //신발 아이템 먹었을 때.
		{
			m_PlayerSpeed++; //신발 아이템 먹었을 때 스피드 증가 최대8로 설정함.
			m_SpeedItem = false;
			if (m_fSpeed > 8)
			{
				m_fSpeed = 8;
			}
		}

		if (m_BoomItem)
		{
			m_BoomCount++;
			m_BoomItem = false;
			if (m_BoomCount >= 6)
			{
				m_BoomCount = 6;
			}

		}
		
		if (m_PadoItem)
		{
			m_BoomSize ++;
			m_PadoItem = false;

			if (m_BoomSize >= 5)
			{
				m_BoomSize = 5;
			}
		}
	}

	return OBJ_NOEVENT;
}

void CPlayer::Late_Update()
{
}

void CPlayer::Render(HDC _DC)
{
	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(m_pFramekey);

	if (!m_bStretch && !lstrcmp(m_pFramekey, L"Player_Right"))
	{
		HDC hStrechDC = CBmpMgr::Get_Instance()->Find_Bmp(L"Stretch");

		StretchBlt(hStrechDC, 0, 0, 200, 200
			, hMemDC
			, m_tFrame.iFrameStart * 200 + 200, m_tFrame.iFrameScene * 200
			, -200, 200
			, SRCCOPY);

		GdiTransparentBlt(_DC
			, m_tRect.left + iScrollX, m_tRect.top
			, m_tInfo.iCX, m_tInfo.iCY
			, hStrechDC
			, 0, 0
			, 200, 200
			, RGB(0, 0, 0));
	}
	else if (m_bDead)
	{
		if (m_PlayerDeadTime)
		{
			m_dwTime = GetTickCount();
			
			m_PlayerDeadTime = false;
		}

		if (m_dwTime + 3000 > GetTickCount())
		{

			m_pFramekey = L"Characbub0";
			m_eCurScene = DEAD;

			HDC hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(L"Characbub0");

			GdiTransparentBlt(_DC
				, m_tRect.left , m_tRect.top
				, 40, 40
				, hMemDC
				, m_tFrame.iFrameStart * 60, m_tFrame.iFrameScene * 0
				, 60, 65
				, RGB(252, 0, 252));


		}
		if (m_dwTime + 3000 < GetTickCount()) // 데드씬을 이용해서 8번까지 출력해야댐
		{
			HDC hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(L"Bazzi_Die");

			GdiTransparentBlt(_DC
				, m_tRect.left , m_tRect.top
				, 50, 50
				, hMemDC
				, m_tFrame.iFrameStart * 70, m_tFrame.iFrameScene * 0
				, 70, 110
				, RGB(252, 0, 252));
		}
		if (m_dwTime + 4000 < GetTickCount())
		{
			m_pPlayerDel = true;
		}


	}
	else
	{
		GdiTransparentBlt(_DC
			, m_tRect.left + iScrollX, m_tRect.top
			, m_tInfo.iCX, m_tInfo.iCY
			, hMemDC
			, m_tFrame.iFrameStart * 44, m_tFrame.iFrameScene * 0
			, 44, 62
			, RGB(252, 0, 252));
	}
	

}

void CPlayer::Release()
{
}

CObj* CPlayer::Create_Shield()
{
	CObj* pObj = CAbstractFactory<CShield>::Create(this);

	return pObj;
}

void CPlayer::Scene_Change()
{
	if (m_ePreScene != m_eCurScene)
	{
		switch (m_eCurScene)
		{
		case CPlayer::IDLE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 4;
			m_tFrame.iFrameScene = WALK;
			m_tFrame.dwFrameTime = 200;
			m_tFrame.dwTime = GetTickCount();
			break;
		case CPlayer::WALK:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 8;
			m_tFrame.iFrameScene = WALK;
			m_tFrame.dwFrameTime = 200;
			m_tFrame.dwTime = GetTickCount();
			break;
		case CPlayer::ATTACK:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iFrameScene = 1;
			m_tFrame.dwFrameTime = 200;
			m_tFrame.dwTime = GetTickCount();
			break;
		case CPlayer::HIT:
			break;
		case CPlayer::DEAD:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 4;
			m_tFrame.iFrameScene = 4;
			m_tFrame.dwFrameTime = 400;
			m_tFrame.dwTime = GetTickCount();
			break;
		case CPlayer::DEAD_SCENE :
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

void CPlayer::Key_Check()
{
	//m_bBoomCheck = Get_BoomCheck();

 //	if (m_bBoomCheck)
	//{
	//	m_iBoomCount--;
	//	m_bBoomCheck = false;
	//	Set_BoomCount(m_bBoomCheck);
	//	/*Set_BoomCount(m_BoomCount);*/

	//}

	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LEFT))
	{
		m_pFramekey = L"Left";
		m_eCurScene = WALK;
		m_tInfo.fX -= m_PlayerSpeed;
		m_bStretch = false;

		if (m_tInfo.fX <= 20 )
		{
			m_tInfo.fX = 20;
		}
	}
	else if (CKeyMgr::Get_Instance()->Key_Pressing(VK_RIGHT))
	{
		m_pFramekey = L"Right";
		m_eCurScene = WALK;
		m_tInfo.fX += m_PlayerSpeed;
		m_bStretch = true;

		if (m_tInfo.fX >= 580)
		{
			m_tInfo.fX = 580;
		}
	}
	else if (CKeyMgr::Get_Instance()->Key_Pressing(VK_UP))
	{
		m_pFramekey = L"Up";
		m_eCurScene = WALK;
		m_tInfo.fY -= m_PlayerSpeed;

		if (m_tInfo.fY <= 20)
		{
			m_tInfo.fY = 20;
		}
	}
	else if (CKeyMgr::Get_Instance()->Key_Pressing(VK_DOWN))
	{
		m_pFramekey = L"Down";
		m_eCurScene = WALK;
		m_tInfo.fY += m_PlayerSpeed;

		if (m_tInfo.fY >= 500)
		{
			m_tInfo.fY = 500;
		}
	}
	//else if (CKeyMgr::Get_Instance()->Key_Pressing('P'))
	//{
	//	
	//	m_pFramekey = L"Characbub0";
	//	m_eCurScene = DEAD;
	//	/*m_bDead = true;*/
	//}
	//else if (m_dwTime + 3000 < GetTickCount())
	//{
	//	m_pFramekey = L"Bazzi_Die";
	//	m_eCurScene = DEAD_SCENE;
	//	m_bDead = true;
	//}
	else
	{
		m_eCurScene = WALK;
	}


	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_SPACE))
	{
		
		if (m_BoomCount > CObjMgr::Get_Instance()->Get_list(OBJID::BULLET).size() )
		{
				/*float x = (float)((TILECX >> 1) + ((m_tInfo.fX / TILECX) * TILECX)) - 20.f;
				float y = (float)((TILECY >> 1) + ((m_tInfo.fY / TILECY) * TILECY)) - 20.f;*/

			m_pTemp = CTileMgr::Get_Instance()->Get_Index(m_tInfo.fX, m_tInfo.fY);
			float fBulletx = m_pTemp->Get_Info().fX;
			float fBullety = m_pTemp->Get_Info().fY;
			if (!m_BoomAndPlayer)
			{
				CObjMgr::Get_Instance()->Add_Object(Create_Bullet<CBullet>(fBulletx, fBullety), OBJID::BULLET);
			}
		}
	}
	m_BoomAndPlayer = false;

	if (m_bDead)
	{
		/*m_pFramekey = L"Characbub0";*/
		m_eCurScene = DEAD_SCENE;
	}

	if (CKeyMgr::Get_Instance()->Key_Pressing('P'))
	{
		m_BoomSize = 5;
		m_BoomCount = 6;
		m_PlayerSpeed = 8.f;


	}

}


void CPlayer::Jumping()
{
	float fY = 0.f;
	bool bLineCol = CLineMgr::Get_Instance()->Collision_Line(m_tInfo.fX, &fY);

	if (m_bJump)
	{
		if (m_bJumpStart)
		{
			m_fJumpStart = m_tInfo.fY;
			m_bJumpStart = false;
		}

		m_tInfo.fY = m_fJumpStart - (m_fJumpPower * m_fJumpTime - 0.5f * 9.8f * m_fJumpTime * m_fJumpTime);
		m_fJumpTime += 0.2f;

		if (bLineCol && fY < m_tInfo.fY)
		{
			m_tInfo.fY = fY;
			m_bJump = false;
			m_fJumpTime = 0.f;
			m_bJumpStart = true;
		}
	}
	else if (bLineCol)
		m_tInfo.fY = fY;
}

void CPlayer::OffSet()
{
	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iOffsetX = WINCX >> 1;

	//if (iOffsetX < m_tInfo.fX + iScrollX)
	//	CScrollMgr::Get_Instance()->Set_ScrollX(iOffsetX - (m_tInfo.fX + iScrollX));
	//if (iOffsetX > m_tInfo.fX + iScrollX)
	//	CScrollMgr::Get_Instance()->Set_ScrollX(iOffsetX - (m_tInfo.fX + iScrollX));
}




