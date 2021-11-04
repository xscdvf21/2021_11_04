#include "stdafx.h"
#include "UnitWeapon.h"
#include "Texture_Manager.h"
#include "ScrollMgr.h"
#include "GameObject_Manger.h"

CUnitWeapon::CUnitWeapon()
{
}


CUnitWeapon::~CUnitWeapon()
{
}

HRESULT CUnitWeapon::Ready_GameObject()
{
	m_pTextureKey = L"UnitWeaponSelect";
	return S_OK;
}

int CUnitWeapon::Update_GameObject()
{
	return 0;
}

void CUnitWeapon::LateUpdate_GameObject()
{
}

void CUnitWeapon::Render_GameObject()
{
	if (static_iAllCheck == 7)
	{
		const TEXINFO*	pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"SystemSelect", m_pTextureKey, m_iSceneNum);

		if (nullptr == pTexInfo)
		{
			ERR_MSG(L"UnitWeapon.Cpp, UnitINFO.png, 즉 pTexInfo가 오류.");
			return;
		}

		float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

		int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
		int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

		_matrix matScale, matTrans, matWorld;
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
		D3DXMatrixTranslation(&matTrans, WINCX >> 1, WINCY >> 1, 0.f);

		matWorld = matScale * matTrans;

		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &_vec3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	
		//무기 폰트.
		TCHAR szUnitWeapon[64] = L"";
		swprintf_s(szUnitWeapon, L"%s", m_tInfo.szWeaponName);
		D3DXMatrixTranslation(&matTrans, 170.f, 200.f, 0.f);
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
		CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
		szUnitWeapon, lstrlen(szUnitWeapon), 0, 0, D3DCOLOR_ARGB(255, 255, 255, 255));

		TCHAR szUnitDamage[64] = L"";
		swprintf_s(szUnitDamage, L" %d", m_tInfo.iAttack);
		D3DXMatrixTranslation(&matTrans, 380.f, 200.f, 0.f);
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
		CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
			szUnitDamage, lstrlen(szUnitDamage), 0, 0, D3DCOLOR_ARGB(255, 255, 255, 255));


		TCHAR szUnitRange[64] = L"";
		swprintf_s(szUnitRange, L" %d", m_tInfo.iRange);
		D3DXMatrixTranslation(&matTrans, 470.f, 200.f, 0.f);
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
		CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
		szUnitRange, lstrlen(szUnitRange), 0, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
}

void CUnitWeapon::Release_GameObject()
{
}
