#include "stdafx.h"
#include "DeadGold.h"
#include "Texture_Manager.h"
#include "ScrollMgr.h"
#include "GameObject_Manger.h"
#include "Time_Manager.h"

CDeadGold::CDeadGold()
{
}


CDeadGold::~CDeadGold()
{
}

HRESULT CDeadGold::Ready_GameObject()
{
	m_pTextureKey = L"DeadGold";
	return S_OK;
}

int CDeadGold::Update_GameObject()
{
	if (m_fTurnDelay >= 2.f)
	{
		return OBJ_DEAD;
	}
	m_fTurnDelay += CTime_Manager::Get_Instance()->Get_TimeDelta();

	return OBJ_NOEVENT;
}

void CDeadGold::LateUpdate_GameObject()
{
}

void CDeadGold::Render_GameObject()
{
	const TEXINFO*	pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"SystemSelect", m_pTextureKey, 0);

	if (nullptr == pTexInfo)
	{
		ERR_MSG(L"UnitGold.Cpp, TexInfo°¡ ¿À·ù.");
		return;
	}

	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	_matrix matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	D3DXMatrixTranslation(&matTrans, (WINCX >> 1), (WINCY >> 1) - 50.f, 0.f);

	matWorld = matScale * matTrans;

	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &_vec3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));


	TCHAR szLevel[64] = L"";
	swprintf_s(szLevel, L"%dP¸¦ È¹µæÇÔ (ºÎÀÚ°¡ ²Þ) ", m_tMonInfo.iGold);
	D3DXMatrixTranslation(&matTrans, 170.f, 230.f , 0.f);
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
	CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
	szLevel, lstrlen(szLevel), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CDeadGold::Release_GameObject()
{
}
