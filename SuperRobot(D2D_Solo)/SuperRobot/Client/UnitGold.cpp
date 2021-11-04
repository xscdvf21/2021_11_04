#include "stdafx.h"
#include "UnitGold.h"
#include "Texture_Manager.h"
#include "ScrollMgr.h"
#include "GameObject_Manger.h"
#include "Time_Manager.h"

CUnitGold::CUnitGold()
{
}


CUnitGold::~CUnitGold()
{
}

HRESULT CUnitGold::Ready_GameObject()
{
	m_pTextureKey = L"UnitGold";
	return S_OK;
}

int CUnitGold::Update_GameObject()
{
	if (m_fTurnDelay >= 2.f)
	{
		return OBJ_DEAD;
	}
	m_fTurnDelay += CTime_Manager::Get_Instance()->Get_TimeDelta();

	return OBJ_NOEVENT;
}

void CUnitGold::LateUpdate_GameObject()
{
}

void CUnitGold::Render_GameObject()
{
	const TEXINFO*	pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"SystemSelect", m_pTextureKey, 0);

	if (nullptr == pTexInfo)
	{
		ERR_MSG(L"UnitGold.Cpp, TexInfo가 오류.");
		return;
	}

	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	_matrix matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	D3DXMatrixTranslation(&matTrans, 700.f, 50.f, 0.f);

	matWorld = matScale * matTrans;

	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &_vec3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));


	TCHAR szLevel[64] = L"";
	swprintf_s(szLevel, L"부자가 꿈 \n  %dP", exGold);
	D3DXMatrixTranslation(&matTrans, 630.f, 20.f, 0.f);
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
	CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
		szLevel, lstrlen(szLevel), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CUnitGold::Release_GameObject()
{
}
