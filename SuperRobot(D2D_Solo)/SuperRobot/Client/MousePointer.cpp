#include "stdafx.h"
#include "MousePointer.h"
#include "ScrollMgr.h"
#include "Key_Manager.h"
#include "Texture_Manager.h"
#include "Time_Manager.h"
#include "Tile.h"
#include "GameObject_Manger.h"


CMousePointer::CMousePointer()
{
}


CMousePointer::~CMousePointer()
{
}

HRESULT CMousePointer::Ready_GameObject()
{

	m_tInfo.vPos = _vec3(60.f, 60.f, 0.f);
	m_tInfo.vSize = { 1.f, 1.f, 0.f };


	pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"MenuSelect", L"Cursor", 0); //충돌 판정을 위해 이니셜라이즈에서 이미지의 정보를 미리 가져와야함.

	return E_NOTIMPL;
}

int CMousePointer::Update_GameObject()
{
	OffSet();
	//if(static_AllCheck)
	//	Key_Check();

	if (0 == static_iAllCheck || 2 == static_iAllCheck || static_iAllCheck == 6)
	{
		Key_Check();
	}
	return OBJ_NOEVENT;
}

void CMousePointer::LateUpdate_GameObject()
{
}

void CMousePointer::Render_GameObject()
{
	if (static_iAllCheck == 6)
	{
		m_iSceneNum = 1;
		pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"MenuSelect", L"Cursor", m_iSceneNum);
	}
	else
	{
		m_iSceneNum = 0;
		pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"MenuSelect", L"Cursor", m_iSceneNum);
	}
	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();


	if (nullptr == pTexInfo)
		return;

	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	_matrix matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + iScrollX ,m_tInfo.vPos.y +iScrollY, 0.f);

	matWorld = matScale * matTrans;

	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &_vec3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

}

void CMousePointer::Release_GameObject()
{
}

void CMousePointer::OffSet()
{
	/*int iScrollX = (int)CScroll_Manager::Get_Scroll(CScroll_Manager::SCROLL_X);
	int iOffsetX = WINCX >> 1;*/

	//if (iOffsetX < m_tInfo.vPos.x + iScrollX)
	//	CScroll_Manager::Set_Scroll(_vec3(iOffsetX - (m_tInfo.vPos.x + iScrollX), 0.f, 0.f));
	//if (iOffsetX > m_tInfo.vPos.x + iScrollX)
	//	CScroll_Manager::Set_Scroll(_vec3(iOffsetX - (m_tInfo.vPos.x + iScrollX), 0.f, 0.f));

	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	int iOffsetX = WINCX >> 1;
	int iOffsetY = WINCY >> 1;

	if (iOffsetX < m_tInfo.vPos.x + iScrollX)
		CScrollMgr::Get_Instance()->Set_ScrollX(iOffsetX - (m_tInfo.vPos.x + iScrollX));
	if (iOffsetX > m_tInfo.vPos.x + iScrollX)
		CScrollMgr::Get_Instance()->Set_ScrollX(iOffsetX - (m_tInfo.vPos.x + iScrollX));

	if (iOffsetY < m_tInfo.vPos.y + iScrollY)
		CScrollMgr::Get_Instance()->Set_ScrollY(iOffsetY - (m_tInfo.vPos.y + iScrollY));
	if (iOffsetY > m_tInfo.vPos.y + iScrollY)
		CScrollMgr::Get_Instance()->Set_ScrollY(iOffsetY - (m_tInfo.vPos.y + iScrollY));

}

void CMousePointer::Key_Check()
{
	float m_fSpeed = 500.f * CTime_Manager::Get_Instance()->Get_TimeDelta();
	if (CKey_Manager::Get_Instance()->Key_Down(VK_LEFT))
	{
		CTile*		m_pTile;
		m_pTile = new CTile;
		m_pTile->Ready_GameObject();

		GROUND*		m_pTemp = m_pTile->Get_Index(m_tInfo.vPos);


		m_tInfo.vPos = m_pTemp->vPos + _vec3{-(float)TILECX, 0.f, 0.f };
			
		/*m_tInfo.vPos.x -= 2.f;*/
		if (m_tInfo.vPos.x < 0 + 20)
		{

			m_tInfo.vPos.x = 0 + 20;
		}
	}
	if (CKey_Manager::Get_Instance()->Key_Down(VK_RIGHT))
	{
		CTile*		m_pTile;
		m_pTile = new CTile;
		m_pTile->Ready_GameObject();

		GROUND*		m_pTemp = m_pTile->Get_Index(m_tInfo.vPos);

		for (int i = 0; i <= TILECX; ++i)
		{
			float fSpeed = 1.f;
			m_tInfo.vPos = m_pTemp->vPos + _vec3{ float(i), 0.f, 0.f };
			if (m_tInfo.vPos == m_pTemp->vPos + _vec3{ (float)TILECX, 0.f, 0.f })
				return;
		}

		if (m_tInfo.vPos.x > 1180)
		{
			m_tInfo.vPos.x = 1180;
		}
		/*CScroll_Manager::Set_Scroll({ -m_fSpeed, 0.f, 0.f });*/
	}
	if (CKey_Manager::Get_Instance()->Key_Down(VK_UP))
	{
		CTile*		m_pTile;
		m_pTile = new CTile;
		m_pTile->Ready_GameObject();

		GROUND*		m_pTemp = m_pTile->Get_Index(m_tInfo.vPos);
		m_tInfo.vPos = m_pTemp->vPos + _vec3{ 0.f, -(float)TILECY, 0.f };

		if (m_tInfo.vPos.y < 0 + 20) //커서사진의 중점 기준이므로 + 사진크기의 반
		{
			m_tInfo.vPos.y = 0 + 20;
		}
		/*CScroll_Manager::Set_Scroll({ 0.f, m_fSpeed, 0.f });*/
	}
	if (CKey_Manager::Get_Instance()->Key_Down(VK_DOWN))
	{
		CTile*		m_pTile;
		m_pTile = new CTile;
		m_pTile->Ready_GameObject();

		GROUND*		m_pTemp = m_pTile->Get_Index(m_tInfo.vPos);
		m_tInfo.vPos = m_pTemp->vPos + _vec3{ 0.f, (float)TILECY, 0.f };

		if (m_tInfo.vPos.y > 1180)
		{
			m_tInfo.vPos.y = 1180;
		}
		/*CScroll_Manager::Set_Scroll({ 0.f, -m_fSpeed, 0.f });*/
	}
}
