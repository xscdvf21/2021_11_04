#include "stdafx.h"
#include "Tile.h"
#include "Texture_Manager.h"
#include "ScrollMgr.h"
#include "GunDamMK.h"
#include "GameObject_Manger.h"


CTile::CTile()
	/*:TileIndex(0)*/
{
}


CTile::~CTile()
{
	Release_GameObject();
}

HRESULT CTile::Ready_GameObject()
{

	m_vecGround.reserve(TILEX*TILEY);

	GROUND*		pGround = nullptr;

	float fX = 0;
	float fY = 0;

	int iTempIndex = 0;

	for (int i = 0; i < TILEY; ++i)
	{
		for (int j = 0; j < TILEX; ++j)
		{
			fX = float((TILECX >> 1) + (j * TILECY));
			fY = float((TILECY >> 1) + (i * TILECX));



			pGround = new GROUND;

			pGround->vPos = { fX, fY, 1.0f };			//둘다 표현가능.
			pGround->vSize = _vec3(1.f, 1.f, 0.f);
			pGround->iIndex = iTempIndex;
			pGround->byOption = 0;
			pGround->dwParentIndex = 0;
			pGround->dwIndex = j + (i * TILEX);
			++iTempIndex;

			m_vecGround.emplace_back(pGround);
		}
	}

	CreateGraph();
	return S_OK;
}

int CTile::Update_GameObject()
{
	return 0;
}

void CTile::LateUpdate_GameObject()
{
}

void CTile::Render_GameObject()
{

	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	const TEXINFO*	pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"Map", L"Ground", 0);
	if (pTexInfo == nullptr)
	{
		ERR_MSG(L"Tile.Cpp에서  Render부분에서 오류 ");
		return;
	}

	float	fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float	fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	D3DXMATRIX		matScale, matTrans, matWorld;
	if (2 != static_iAllCheck)
	{
		for (size_t i = 0; i < m_vecGround.size(); ++i)
		{
			const TEXINFO*	pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"Map", L"Ground", 0);
			if (pTexInfo == nullptr)
			{
				ERR_MSG(L"Tile.Cpp에서  Render부분에서 오류 ");
				return;
			}

			float	fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
			float	fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

			D3DXMatrixScaling(&matScale, 1.0f, 1.0f, 0.f);
			D3DXMatrixTranslation(&matTrans, m_vecGround[i]->vPos.x + iScrollX, m_vecGround[i]->vPos.y + iScrollY, 0.f);

			matWorld = matScale * matTrans;

			CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
			CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &_vec3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(0, 255, 255, 255));
		}
	}


	if (m_bTileCheck || m_bTileCheck && static_iAllCheck == 6) //이동 타일이 보여진 후 다른 유닛에게 가면, 그유닛의 이동 타일 나오는것을 해결.
	{
		for (auto& pList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_UNIT))
		{
			if ((pList->Get_Info().vPos == CGameObject_Manger::Get_Instance()->Get_Mouse()->Get_Info().vPos))
			{
				temp = pList->Get_vecTemp();
				m_bTileCheck = false;
			}
		}
	}

	if (m_bTileCheck || m_bTileCheck && static_iAllCheck == 6)
	{
		for (auto& pList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_MONSTER))
		{
			if ((pList->Get_MonsterInfo().vPos == CGameObject_Manger::Get_Instance()->Get_Mouse()->Get_Info().vPos))
			{
				temp = pList->Get_vecTemp();
			}
		}
	}


	if (m_bMoveTile || m_bTileCheck)
	{
		for (auto& pTemp : temp) //이동 타일 표시
		{
			if (2 == static_iAllCheck /*|| 6 == static_iAllCheck*/ && pTemp->byOption == 1)
			{
				D3DXMATRIX		matScale, matTrans, matWorld;
				D3DXMatrixScaling(&matScale, 1.0f, 1.0f, 0.f);
				D3DXMatrixTranslation(&matTrans, pTemp->vPos.x + iScrollX, pTemp->vPos.y + iScrollY, 0.f);
				matWorld = matScale * matTrans;
				CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
				CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &_vec3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(100, 0, 0, 0));
			}
		}
	}
	

	


	for (auto& pTemp : temp) //공격 타일 표시.
	{
		if (6 == static_iAllCheck && pTemp->byOption == 1)
		{
			D3DXMATRIX		matScale, matTrans, matWorld;
			D3DXMatrixScaling(&matScale, 1.0f, 1.0f, 0.f);
			D3DXMatrixTranslation(&matTrans, pTemp->vPos.x + iScrollX, pTemp->vPos.y + iScrollY, 0.f);
			matWorld = matScale * matTrans;
			CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
			CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &_vec3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(100, 255, 0, 0)); //공격일떄는 타일의 색을 다르게.
		}
	}


}

void CTile::Release_GameObject()
{
}

GROUND* CTile::Get_Index(_vec3 _vec)
{
	int		iX = (int)_vec.x / TILECX;
	int		iY = (int)_vec.y / TILECY;

	int iIndex = iY * TILEX + iX;

	return m_vecGround[iIndex];
}

GROUND* CTile::Get_TileMove(int _TilePos)
{
	return m_vecGround[_TilePos];
}

void CTile::CreateGraph()
{

	m_vecGraph.resize(m_vecGround.size());


	for (int i = 0; i < TILEY; ++i)
	{
		for (int j = 0; j < TILEX; ++j)
		{
			iIndex = j + (i * TILEX);
			//타일의 좌측 
			if ((0 != j) && (iIndex % (TILEX * 2) != 0))
			{
				m_vecGraph[iIndex].emplace_back(m_vecGround[iIndex - 1]);
			}	
			//타일의 우측

			if ((0 != j) && (((TILEX * 2) - 1) != iIndex % (TILEX * 2)))
			{

					m_vecGraph[iIndex].emplace_back(m_vecGround[iIndex + 1]);

			}


			if ((0 != i) && (((TILEX * 2) - 1) != iIndex % (TILEX * 2)))
			{
					m_vecGraph[iIndex].emplace_back(m_vecGround[iIndex - (TILEX)]);

			}
			//타일의 상단
			//if ((TILEY - 1 != i) && 0 != (iIndex % (TILEX * 2)))
			//{
			//		m_vecGraph[iIndex].emplace_back(m_vecGround[iIndex - (TILEX)]);

			//}
			//타일의 하단
			if ((TILEY - 1 != i) && ((TILEX * 2) - 1 != (iIndex % (TILEX * 2))))
			{
					m_vecGraph[iIndex].emplace_back(m_vecGround	[iIndex + TILEX]);
			}
		}
	}

}

