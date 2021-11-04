#include "stdafx.h"
#include "UnitMenu.h"
#include "Texture_Manager.h"
#include "ScrollMgr.h"
#include "GameObject_Manger.h"


CUnitMenu::CUnitMenu()
{
}


CUnitMenu::~CUnitMenu()
{
}

HRESULT CUnitMenu::Ready_GameObject()
{
	m_pTextureKey = L"UnitMenu";
	return S_OK;
}

int CUnitMenu::Update_GameObject()
{
	return 0;
}

void CUnitMenu::LateUpdate_GameObject()
{
}

void CUnitMenu::Render_GameObject()
{
	if (static_iAllCheck == 1)
	{
		const TEXINFO*	pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"SystemSelect", m_pTextureKey, m_iSceneNum);
		if (nullptr == pTexInfo)
		{
			ERR_MSG(L"UnitMenu.Cpp, pTexInfo°¡ ¿À·ù.");
			return;
		}

		float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

		int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
		int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

		_matrix matScale, matTrans, matWorld;
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + iScrollX, m_tInfo.vPos.y + iScrollY, 0.f);

		matWorld = matScale * matTrans;

		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
}

void CUnitMenu::Release_GameObject()
{
}
