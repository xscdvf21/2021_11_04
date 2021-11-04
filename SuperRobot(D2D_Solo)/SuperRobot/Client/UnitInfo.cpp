#include "stdafx.h"
#include "UnitInfo.h"
#include "Texture_Manager.h"
#include "ScrollMgr.h"
#include "GameObject_Manger.h"

CUnitInfo::CUnitInfo()
{
}


CUnitInfo::~CUnitInfo()
{
}

HRESULT CUnitInfo::Ready_GameObject()
{

	m_pTextureKey = L"UnitINFO";
	return S_OK;
}

int CUnitInfo::Update_GameObject()
{
	return 0;
}

void CUnitInfo::LateUpdate_GameObject()
{
}

void CUnitInfo::Render_GameObject()
{
	if (static_iAllCheck == 4)
	{
		const TEXINFO*	pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"SystemSelect", m_pTextureKey, 0);
		const TEXINFO*	pTexInfo2 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"Unit", m_pTextureUnit, 4);
		if (nullptr == pTexInfo)
		{
			ERR_MSG(L"UnitInfo.Cpp, TexInfo가 오류.");
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

		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
		D3DXMatrixTranslation(&matTrans, 415.f, 365.f, 0.f);

		matWorld = matScale * matTrans;

		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo2->pTexture, nullptr, &_vec3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));



		TCHAR szChar[64] = L"";
		swprintf_s(szChar, L"유닛능력");
		D3DXMatrixTranslation(&matTrans, 210.f, 100.f, 0.f);
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
		CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
			szChar, lstrlen(szChar), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

		TCHAR szUnitName[64] = L"";
		swprintf_s(szUnitName, L"%s", m_tInfo.szUnitName);
		D3DXMatrixTranslation(&matTrans, 400.f, 150.f, 0.f);
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
		CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
			szUnitName, lstrlen(szUnitName), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

		TCHAR szChar2[64] = L"";
		swprintf_s(szChar2, L"리소스 없으면... ");
		D3DXMatrixTranslation(&matTrans, 400.f, 200.f, 0.f);
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
		CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
			szChar2, lstrlen(szChar2), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

		TCHAR szChar3[64] = L"";
		swprintf_s(szChar3, L"딴게임 하세요...");
		D3DXMatrixTranslation(&matTrans, 400.f, 230.f, 0.f);
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
		CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
			szChar3, lstrlen(szChar3), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));



		TCHAR szUnitHP[64] = L"";
		swprintf_s(szUnitHP, L"HP %d/%d", m_tInfo.iHp, m_tInfo.iMaxHp);
		D3DXMatrixTranslation(&matTrans, 170.f, 330.f, 0.f);
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
		CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
			szUnitHP, lstrlen(szUnitHP), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

		TCHAR szUnitEN[64] = L"";
		swprintf_s(szUnitEN, L"EN %d/%d", m_tInfo.iEnergy, m_tInfo.iMaxEnergy);
		D3DXMatrixTranslation(&matTrans, 170.f, 380.f, 0.f);
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
		CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
			szUnitEN, lstrlen(szUnitEN), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));


		TCHAR szUnitAttack[64] = L"";
		swprintf_s(szUnitAttack, L"공격 %d", m_tInfo.iAttack);
		D3DXMatrixTranslation(&matTrans, 380.f, 330.f, 0.f);
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
		CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
			szUnitAttack, lstrlen(szUnitAttack), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

		TCHAR szUnitMove[64] = L"";
		swprintf_s(szUnitMove, L"이동 %d", m_tInfo.iMove);
		D3DXMatrixTranslation(&matTrans, 380.f, 370.f, 0.f);
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
		CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
			szUnitMove, lstrlen(szUnitMove), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));


		TCHAR szUnitArmor[64] = L"";
		swprintf_s(szUnitArmor, L"장갑 %d", m_tInfo.iArmor);
		D3DXMatrixTranslation(&matTrans, 380.f, 410.f, 0.f);
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
		CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
			szUnitArmor, lstrlen(szUnitArmor), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));





	}
}

void CUnitInfo::Release_GameObject()
{
}
