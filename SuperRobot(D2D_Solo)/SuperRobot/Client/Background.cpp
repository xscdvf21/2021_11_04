#include "stdafx.h"
#include "Background.h"
#include "Texture_Manager.h"
#include "ScrollMgr.h"
#include "GameObject_Manger.h"

CBackground::CBackground()
	:m_iCount(0)
{
}


CBackground::~CBackground()
{
}

HRESULT CBackground::Ready_GameObject()
{
	m_pTextureKey = L"Background";

	m_iAlpha = 0;
	m_bAlphaCheck = false;
	return S_OK;
}

int CBackground::Update_GameObject()
{
	if (m_iCount == 3)
		m_bDead = true;

	if (m_bDead)
		return OBJ_DEAD;


	return OBJ_NOEVENT;
}

void CBackground::LateUpdate_GameObject()
{
}

void CBackground::Render_GameObject()
{


	if (m_iAlpha >= 255)
	{
		m_bAlphaCheck = true;
		++m_iCount;
	}
	else if (m_iAlpha <= 0)
	{
		m_bAlphaCheck = false;
		++m_iCount;
	}

	if (m_bAlphaCheck)
		m_iAlpha -= 3;
	else if(!m_bAlphaCheck)
		m_iAlpha += 3;

	const TEXINFO*	pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"Map", m_pTextureKey, 0);

	if (nullptr == pTexInfo)
	{
		ERR_MSG(L"BackGround.Cpp, TexInfo°¡ ¿À·ù.");
		return;
	}

	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	_matrix matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	D3DXMatrixTranslation(&matTrans, 0.f + iScrollX, 0.f + iScrollY, 0.f);

	matWorld = matScale * matTrans;

	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(m_iAlpha, 0, 0, 0));
}


void CBackground::Release_GameObject()
{
}
