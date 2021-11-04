#include "stdafx.h"
#include "Terrain.h"
#include "ToolView.h"

CTerrain::CTerrain()
{
}


CTerrain::~CTerrain()
{
	Release_Terrain(); 
}

void CTerrain::Tile_Change_Terrain(const D3DXVECTOR3 & vMousePos, const BYTE & byDrawID, const BYTE & byOption)
{
	int iIndex = GetTileIndex(vMousePos); 

	if (-1 == iIndex)
		return; 

	m_vecTile[iIndex]->byDrawID = byDrawID;
	m_vecTile[iIndex]->byOption = byOption; 
}

int CTerrain::GetTileIndex(const D3DXVECTOR3 & vPos)
{
	size_t iSize = m_vecTile.size(); 
	size_t i = 0; 
	for (; i < iSize; ++i)
	{
		if (IsPicking(i, vPos))
			return i;
	}
	return -1;
}

bool CTerrain::IsPicking(const int & iIndex, const D3DXVECTOR3 & vMousePos)
{
	//직선의 방정식을 이용한 방식. 
	// y = ax + b; 
	//0 = ax + b - y
	// 기울기부터.
// 	float fGradiant[4] = 
// 	{
// 		(TILECY * 0.5f) / (TILECX * 0.5f),
// 		-(TILECY * 0.5f) / (TILECX * 0.5f),
// 		(TILECY * 0.5f) / (TILECX * 0.5f),
// 		-(TILECY * 0.5f) / (TILECX * 0.5f)
// 	}; 
// 
// 	
// 	D3DXVECTOR3 vVertexPoint[4] = 
// 	{
// 		{m_vecTile[iIndex]->vPos.x, m_vecTile[iIndex]->vPos.y + (TILECY >> 1), 0.f},
// 		{ m_vecTile[iIndex]->vPos.x + (TILECX >> 1), m_vecTile[iIndex]->vPos.y , 0.f },
// 		{ m_vecTile[iIndex]->vPos.x, m_vecTile[iIndex]->vPos.y - (TILECY >> 1), 0.f },
// 		{ m_vecTile[iIndex]->vPos.x - (TILECX >> 1), m_vecTile[iIndex]->vPos.y , 0.f }
// 	}; 
// 	// y = ax+ b; 
// 	// b = y - ax;
// 
// 	float fB[4] = 
// 	{
// 		vVertexPoint[0].y - fGradiant[0] * vVertexPoint[0].x,
// 		vVertexPoint[1].y - fGradiant[1] * vVertexPoint[1].x,
// 		vVertexPoint[2].y - fGradiant[2] * vVertexPoint[2].x,
// 		vVertexPoint[3].y - fGradiant[3] * vVertexPoint[3].x
// 	}; 
// 
// 	// 0 = ax + b - y 
// 
// 	if (0 < fGradiant[0] * vMousePos.x + fB[0] - vMousePos.y &&
// 		0 < fGradiant[1] * vMousePos.x + fB[1] - vMousePos.y &&
// 		0 > fGradiant[2] * vMousePos.x + fB[2] - vMousePos.y &&
// 		0 > fGradiant[3] * vMousePos.x + fB[3] - vMousePos.y)
// 	{
// 		return true; 
// 	}
// 	return false;


	// 내적을 이용한 방식. 
	//챕터 1! 꼭지점을 구하자!! 12시 3시 6시 9시 순으로 구해줌. 
	D3DXVECTOR3 vVertexPoint[4] =
	{
		{m_vecTile[iIndex]->vPos.x, m_vecTile[iIndex]->vPos.y + (TILECY >> 1), 0.f},
		{ m_vecTile[iIndex]->vPos.x + (TILECX >> 1), m_vecTile[iIndex]->vPos.y , 0.f },
		{ m_vecTile[iIndex]->vPos.x, m_vecTile[iIndex]->vPos.y - (TILECY >> 1), 0.f },
		{ m_vecTile[iIndex]->vPos.x - (TILECX >> 1), m_vecTile[iIndex]->vPos.y , 0.f }
	};

	// 각각 꼭지점을 바라보는 방향벡터를 구하자! 
	D3DXVECTOR3 vVerDir[4] = 
	{
		vVertexPoint[1] - vVertexPoint[0], 
		vVertexPoint[2] - vVertexPoint[1],
		vVertexPoint[3] - vVertexPoint[2],
		vVertexPoint[0] - vVertexPoint[3]
	};

	// 꼭지점을 바라보는 방향벡터의 법선벡터를 구해라! 
	D3DXVECTOR3 vNormal[4] = 
	{
		{ -vVerDir[0].y, vVerDir[0].x, 0.f},
		{ -vVerDir[1].y, vVerDir[1].x, 0.f },
		{ -vVerDir[2].y, vVerDir[2].x, 0.f },
		{ -vVerDir[3].y, vVerDir[3].x, 0.f }
	}; 

	D3DXVECTOR3 vMouseDir[4] = 
	{
		vMousePos - vVertexPoint[0],
		vMousePos - vVertexPoint[1],
		vMousePos - vVertexPoint[2],
		vMousePos - vVertexPoint[3]
	};
	for (int i = 0 ; i < 4 ; ++i )
	{
		D3DXVec3Normalize(&vNormal[i], &vNormal[i]);
		D3DXVec3Normalize(&vMouseDir[i], &vMouseDir[i]);

		if (0 < D3DXVec3Dot(&vNormal[i], &vMouseDir[i]))
			return false; 
	}

	return true;
	/////////////////////////////////////////////////////////////////////
}

HRESULT CTerrain::Ready_Terrain()
{
	TILE* pTile = nullptr; 
	m_vecTile.reserve(TILEX * TILEY); 
	float fx = 0, fy = 0; 
	for (int i = 0 ; i < TILEY ; ++i)
	{
		for (int j = 0 ; j < TILEX ; ++j)
		{
			fx = float((TILECX * j) + ((i % 2) * (TILECX >> 1)));
			fy = float((TILECY >> 1) * i);

			pTile = new TILE;
			pTile->vPos = D3DXVECTOR3(fx, fy, 0.f);
			pTile->vSize = { 1.f, 1.f, 0.f }; 
			pTile->byDrawID = 9; 
			pTile->byOption = 0; 

			m_vecTile.emplace_back(pTile);
		}
	}
	return S_OK;
}

void CTerrain::Render_Terrain()
{
	D3DXMATRIX matScale, matTrans, matWorld; 
	TCHAR szBuf[64] = L""; 
	int iIndex = 0; 
	for (size_t i = 0 ; i < m_vecTile.size(); ++i)
	{
		swprintf_s(szBuf, L"%d", iIndex); 

		const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"Terrain", L"Tile", m_vecTile[i]->byDrawID); 
		assert(!(nullptr == pTexInfo)); 
// 		if (nullptr == pTexInfo)
// 		{
// 			return; 
// 		}
		float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);
		//Toolview 
		/*GetScrollPos();*/
		D3DXMatrixScaling(&matScale, 1.0f, 1.0f, 0.f);
		D3DXMatrixTranslation(&matTrans, m_vecTile[i]->vPos.x - m_pView->GetScrollPos(0), m_vecTile[i]->vPos.y - m_pView->GetScrollPos(1), 0.f);
		matWorld = matScale  * matTrans;
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
		
		CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
			szBuf, lstrlen(szBuf), 0, 0, D3DCOLOR_ARGB(255, 0, 0, 0));

		++iIndex;
	}
}

void CTerrain::MiniRender_Terrain()
{
	D3DXMATRIX matScale, matTrans, matWorld;
	for (size_t i = 0; i < m_vecTile.size(); ++i)
	{
		const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"Map", L"Stage", m_vecTile[i]->byDrawID);
		assert(!(nullptr == pTexInfo));
		// 		if (nullptr == pTexInfo)
		// 		{
		// 			return; 
		// 		}
		float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);
		//Toolview 
		/*GetScrollPos();*/
		D3DXMatrixScaling(&matScale, 1.0f, 1.0f, 0.f);
		D3DXMatrixTranslation(&matTrans, m_vecTile[i]->vPos.x - m_pView->GetScrollPos(0), m_vecTile[i]->vPos.y - m_pView->GetScrollPos(1), 0.f);
		matWorld = matScale  * matTrans;
		D3DXMatrixScaling(&matScale, 0.2f, 0.5f, 0.f);
		matWorld *= matScale;
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	}
}

void CTerrain::Release_Terrain()
{
	///////////////////////////3개다 동일한 문구///////////////////////////////////////////////
	for (auto& pTile : m_vecTile)
	{
		Safe_Delete(pTile); 
		pTile = nullptr; 
	}
	m_vecTile.clear(); 
	m_vecTile.shrink_to_fit(); 
	//////////////////////////////////////////////////////////////////////////
// 	for_each(m_vecTile.begin(), m_vecTile.end(), Safe_Delete<TILE*>);
// 	m_vecTile.clear();
// 	m_vecTile.shrink_to_fit();
	//////////////////////////////////////////////////////////////////////////
// 	for_each(m_vecTile.begin(), m_vecTile.end(), [](auto& pTile) 
// 	{
// 		if (pTile)
// 		{
// 			delete pTile; 
// 			pTile = nullptr; 
// 		}
// 	});
// 	m_vecTile.clear(); 
// 	m_vecTile.shrink_to_fit(); 
}
