#include "QuadTree.h"

USING(Engine)

Engine::CQuadTree::CQuadTree(void)
{
	ZeroMemory(m_pChild, sizeof(CQuadTree*) * CHILD_END);
	ZeroMemory(m_pNeighbor, sizeof(CQuadTree*)* NEIGHBOR_END);
}

Engine::CQuadTree::CQuadTree(const CQuadTree& rhs)
	: m_dwCenter(rhs.m_dwCenter)
	, m_fRadius(rhs.m_fRadius)
{
	for (_ulong i = 0; i < CHILD_END; ++i)
		m_pChild[i] = rhs.m_pChild[i];

	for (_ulong i = 0; i < CORNER_END; ++i)
		m_dwCorner[i] = rhs.m_dwCorner[i];

	for (_ulong i = 0; i < NEIGHBOR_END; ++i)
		m_pNeighbor[i] = rhs.m_pNeighbor[i];
}
 

Engine::CQuadTree::~CQuadTree(void)
{

}

HRESULT Engine::CQuadTree::Ready_QuadTree(const _ulong& dwCntX, const _ulong& dwCntZ)
{
	m_dwCorner[CORNER_LT] = dwCntX * dwCntZ - dwCntX;
	m_dwCorner[CORNER_RT] = dwCntX * dwCntZ - 1;
	m_dwCorner[CORNER_LB] = 0;
	m_dwCorner[CORNER_RB] = dwCntX - 1;

	m_dwCenter = (m_dwCorner[CORNER_LT] + m_dwCorner[CORNER_RT] + m_dwCorner[CORNER_LB] + m_dwCorner[CORNER_RB]) >> 2;

	Make_ChildQuadTree();

	return S_OK;
}

HRESULT CQuadTree::Ready_Neighbor(void)
{
	Make_Neighbor();

	return S_OK;
}

void Engine::CQuadTree::CullingForTerrain(CFrustum* pFrustum, const _vec3* pVtxPos, 
	INDEX32* pIndex, _ulong* pTriCnt)
{
	m_fRadius = D3DXVec3Length(&(pVtxPos[m_dwCorner[CORNER_LT]] - pVtxPos[m_dwCenter]));

	_bool	bIsIn = pFrustum->IsIn_Frustum(&pVtxPos[m_dwCenter], m_fRadius);

	if (true == bIsIn)
	{
//		if (nullptr == m_pChild[0]) // ������ ����Ʈ���϶�

		if(true == LevelOfDetail(pFrustum->Get_GraphicDev(), pVtxPos))
		{
			_bool		bNeighborDraw[NEIGHBOR_END] = { true, true, true, true };

			for (_ulong i = 0; i < NEIGHBOR_END; ++i)
			{
				if (nullptr != m_pNeighbor[i])
					bNeighborDraw[i] = m_pNeighbor[i]->LevelOfDetail(pFrustum->Get_GraphicDev(), pVtxPos);
			}
			
			_bool bVtxInFrustum[3] = { false };

			// ������ ��
			bVtxInFrustum[0] = pFrustum->IsIn_Frustum(&pVtxPos[m_dwCorner[CORNER_LT]]);
			bVtxInFrustum[1] = pFrustum->IsIn_Frustum(&pVtxPos[m_dwCorner[CORNER_RT]]);
			bVtxInFrustum[2] = pFrustum->IsIn_Frustum(&pVtxPos[m_dwCorner[CORNER_RB]]);

			if (true == bVtxInFrustum[0] || true == bVtxInFrustum[1] || true == bVtxInFrustum[2])
			{
				if (bNeighborDraw[NEIGHBOR_TOP] &&
					bNeighborDraw[NEIGHBOR_RIGHT])
				{
					if (nullptr == m_pChild[0])
					{
						pIndex[*pTriCnt]._0 = m_dwCorner[CORNER_LT];
						pIndex[*pTriCnt]._1 = m_dwCorner[CORNER_RT];
						pIndex[*pTriCnt]._2 = m_dwCorner[CORNER_RB];
						++(*pTriCnt);
					}
					else
					{
						pIndex[*pTriCnt]._0 = m_dwCorner[CORNER_LT];
						pIndex[*pTriCnt]._1 = m_dwCorner[CORNER_RT];
						pIndex[*pTriCnt]._2 = m_dwCenter;
						++(*pTriCnt);

						pIndex[*pTriCnt]._0 = m_dwCorner[CORNER_RT];
						pIndex[*pTriCnt]._1 = m_dwCorner[CORNER_RB];
						pIndex[*pTriCnt]._2 = m_dwCenter;
						++(*pTriCnt);
					}
				}
				else
				{
					if (false == bNeighborDraw[NEIGHBOR_TOP])
					{
						pIndex[*pTriCnt]._0 = m_dwCorner[CORNER_LT];
						pIndex[*pTriCnt]._1 = (m_dwCorner[CORNER_LT] + m_dwCorner[CORNER_RT]) >> 1;
						pIndex[*pTriCnt]._2 = m_dwCenter;
						++(*pTriCnt);

						pIndex[*pTriCnt]._0 = (m_dwCorner[CORNER_LT] + m_dwCorner[CORNER_RT]) >> 1;
						pIndex[*pTriCnt]._1 =m_dwCorner[CORNER_RT];
						pIndex[*pTriCnt]._2 = m_dwCenter;
						++(*pTriCnt);
					}

					else
					{
						pIndex[*pTriCnt]._0 = m_dwCorner[CORNER_LT];
						pIndex[*pTriCnt]._1 = m_dwCorner[CORNER_RT];
						pIndex[*pTriCnt]._2 = m_dwCenter;
						++(*pTriCnt);
					}

					if (false == bNeighborDraw[NEIGHBOR_RIGHT])
					{
						pIndex[*pTriCnt]._0 = m_dwCorner[CORNER_RT];
						pIndex[*pTriCnt]._1 = (m_dwCorner[CORNER_RT] + m_dwCorner[CORNER_RB]) >> 1;
						pIndex[*pTriCnt]._2 = m_dwCenter;
						++(*pTriCnt);

						pIndex[*pTriCnt]._0 = (m_dwCorner[CORNER_RT] + m_dwCorner[CORNER_RB]) >> 1;
						pIndex[*pTriCnt]._1 = m_dwCorner[CORNER_RB];
						pIndex[*pTriCnt]._2 = m_dwCenter;
						++(*pTriCnt);
					}

					else
					{
						pIndex[*pTriCnt]._0 = m_dwCorner[CORNER_RT];
						pIndex[*pTriCnt]._1 = m_dwCorner[CORNER_RB];
						pIndex[*pTriCnt]._2 = m_dwCenter;
						++(*pTriCnt);
					}
				}			
			}

			bVtxInFrustum[0] = pFrustum->IsIn_Frustum(&pVtxPos[m_dwCorner[CORNER_LT]]);
			bVtxInFrustum[1] = pFrustum->IsIn_Frustum(&pVtxPos[m_dwCorner[CORNER_RB]]);
			bVtxInFrustum[2] = pFrustum->IsIn_Frustum(&pVtxPos[m_dwCorner[CORNER_LB]]);

			if (true == bVtxInFrustum[0] || true == bVtxInFrustum[1] || true == bVtxInFrustum[2])
			{

				if (bNeighborDraw[NEIGHBOR_BOTTOM] && bNeighborDraw[NEIGHBOR_LEFT])
				{
					if (nullptr == m_pChild[0])
					{
						pIndex[*pTriCnt]._0 = m_dwCorner[CORNER_LT];
						pIndex[*pTriCnt]._1 = m_dwCorner[CORNER_RB];
						pIndex[*pTriCnt]._2 = m_dwCorner[CORNER_LB];
						++(*pTriCnt);
					}
					else
					{
						pIndex[*pTriCnt]._0 = m_dwCorner[CORNER_LT];
						pIndex[*pTriCnt]._1 = m_dwCenter;
						pIndex[*pTriCnt]._2 = m_dwCorner[CORNER_LB];
						++(*pTriCnt);

						pIndex[*pTriCnt]._0 = m_dwCenter;
						pIndex[*pTriCnt]._1 = m_dwCorner[CORNER_RB];
						pIndex[*pTriCnt]._2 = m_dwCorner[CORNER_LB];
						++(*pTriCnt);
					}
				}


				else
				{
					if (false == bNeighborDraw[NEIGHBOR_BOTTOM])
					{
						pIndex[*pTriCnt]._0 = m_dwCorner[CORNER_LB];
						pIndex[*pTriCnt]._1 = m_dwCenter;
						pIndex[*pTriCnt]._2 = (m_dwCorner[CORNER_LB] + m_dwCorner[CORNER_RB]) >> 1;
						++(*pTriCnt);

						pIndex[*pTriCnt]._0 = m_dwCenter;
						pIndex[*pTriCnt]._1 = m_dwCorner[CORNER_RB];
						pIndex[*pTriCnt]._2 = (m_dwCorner[CORNER_LB] + m_dwCorner[CORNER_RB]) >> 1;
						++(*pTriCnt);


					}
					else
					{
						pIndex[*pTriCnt]._0 = m_dwCorner[CORNER_LB];
						pIndex[*pTriCnt]._1 = m_dwCenter;
						pIndex[*pTriCnt]._2 = m_dwCorner[CORNER_RB];
						++(*pTriCnt);
					}

					if (false == bNeighborDraw[NEIGHBOR_LEFT])
					{
						pIndex[*pTriCnt]._0 = m_dwCorner[CORNER_LT];
						pIndex[*pTriCnt]._1 = m_dwCenter;
						pIndex[*pTriCnt]._2 = (m_dwCorner[CORNER_LT] + m_dwCorner[CORNER_LB]) >> 1;
						++(*pTriCnt);

						pIndex[*pTriCnt]._0 = m_dwCenter;
						pIndex[*pTriCnt]._1 = m_dwCorner[CORNER_LB];
						pIndex[*pTriCnt]._2 = (m_dwCorner[CORNER_LT] + m_dwCorner[CORNER_LB]) >> 1;
						++(*pTriCnt);


					}
					else
					{
						pIndex[*pTriCnt]._0 = m_dwCorner[CORNER_LT];
						pIndex[*pTriCnt]._1 = m_dwCenter;
						pIndex[*pTriCnt]._2 = m_dwCorner[CORNER_LB];
						++(*pTriCnt);
					}
				}

			}
			return;
		
		}

		for (_ulong i = 0; i < CHILD_END; ++i)
		{
			if (nullptr != m_pChild[i])
				m_pChild[i]->CullingForTerrain(pFrustum, pVtxPos, pIndex, pTriCnt);
		}
	}
}

void Engine::CQuadTree::Make_ChildQuadTree(void)
{
	if (1 == m_dwCorner[CORNER_RT] - m_dwCorner[CORNER_LT])
		return;

	for (_ulong i = 0; i < CHILD_END; ++i)
	{
		_ulong	dwLT, dwRT, dwLB, dwRB;

		Make_ChildCorner(CHILD(i), 
			m_dwCorner[CORNER_LT], 
			m_dwCorner[CORNER_RT], 
			m_dwCorner[CORNER_LB], 
			m_dwCorner[CORNER_RB], 
			m_dwCenter, 
			&dwLT, 
			&dwRT, 
			&dwLB, 
			&dwRB);

		m_pChild[i] = CQuadTree::Create(dwLT, dwRT, dwLB, dwRB);

		if (nullptr != m_pChild[i])
			m_pChild[i]->Make_ChildQuadTree();
	}	
}

void CQuadTree::Make_Neighbor(void)
{
	if (nullptr == m_pChild[0])
		return;

	m_pChild[CHILD_LT]->m_pNeighbor[NEIGHBOR_RIGHT] = m_pChild[CHILD_RT];
	m_pChild[CHILD_LT]->m_pNeighbor[NEIGHBOR_BOTTOM] = m_pChild[CHILD_LB];

	m_pChild[CHILD_RT]->m_pNeighbor[NEIGHBOR_LEFT] = m_pChild[CHILD_LT];
	m_pChild[CHILD_RT]->m_pNeighbor[NEIGHBOR_BOTTOM] = m_pChild[CHILD_RB];

	m_pChild[CHILD_LB]->m_pNeighbor[NEIGHBOR_RIGHT] = m_pChild[CHILD_RB];
	m_pChild[CHILD_LB]->m_pNeighbor[NEIGHBOR_TOP] = m_pChild[CHILD_LT];

	m_pChild[CHILD_RB]->m_pNeighbor[NEIGHBOR_LEFT] = m_pChild[CHILD_LB];
	m_pChild[CHILD_RB]->m_pNeighbor[NEIGHBOR_TOP] = m_pChild[CHILD_RT];


	if (nullptr != m_pNeighbor[NEIGHBOR_LEFT])
	{
		m_pChild[CHILD_LT]->m_pNeighbor[NEIGHBOR_LEFT] = m_pNeighbor[NEIGHBOR_LEFT]->m_pChild[CHILD_RT];
		m_pChild[CHILD_LB]->m_pNeighbor[NEIGHBOR_LEFT] = m_pNeighbor[NEIGHBOR_LEFT]->m_pChild[CHILD_RB];
	}

	if (nullptr != m_pNeighbor[NEIGHBOR_RIGHT])
	{
		m_pChild[CHILD_RT]->m_pNeighbor[NEIGHBOR_RIGHT] = m_pNeighbor[NEIGHBOR_RIGHT]->m_pChild[CHILD_LT];
		m_pChild[CHILD_RB]->m_pNeighbor[NEIGHBOR_RIGHT] = m_pNeighbor[NEIGHBOR_RIGHT]->m_pChild[CHILD_LB];
	}

	if (nullptr != m_pNeighbor[NEIGHBOR_TOP])
	{
		m_pChild[CHILD_LT]->m_pNeighbor[NEIGHBOR_TOP] = m_pNeighbor[NEIGHBOR_TOP]->m_pChild[CHILD_LB];
		m_pChild[CHILD_RT]->m_pNeighbor[NEIGHBOR_TOP] = m_pNeighbor[NEIGHBOR_TOP]->m_pChild[CHILD_RB];
	}

	if (nullptr != m_pNeighbor[NEIGHBOR_BOTTOM])
	{
		m_pChild[CHILD_LB]->m_pNeighbor[NEIGHBOR_BOTTOM] = m_pNeighbor[NEIGHBOR_BOTTOM]->m_pChild[CHILD_LT];
		m_pChild[CHILD_RB]->m_pNeighbor[NEIGHBOR_BOTTOM] = m_pNeighbor[NEIGHBOR_BOTTOM]->m_pChild[CHILD_RT];
	}

	for (_ulong i = 0; i < CHILD_END; ++i)
		m_pChild[i]->Make_Neighbor();

}

void Engine::CQuadTree::Make_ChildCorner(CHILD eType, 
	const _ulong& dwPLT, 
	const _ulong& dwPRT, 
	const _ulong& dwPLB, 
	const _ulong& dwPRB,
	const _ulong& dwPCenter, _ulong* pLT, _ulong* pRT, _ulong* pLB, _ulong* pRB)
{
	_ulong		dwLT, dwRT, dwLB, dwRB;
	_ulong		dwLC, dwRC, dwTC, dwBC;

	dwLC = (dwPLT + dwPLB) >> 1;
	dwRC = (dwPRT + dwPRB) >> 1;
	dwTC = (dwPLT + dwPRT) >> 1;
	dwBC = (dwPLB + dwPRB) >> 1;
	
	switch (eType)
	{
	case CHILD_LT:
		dwLT = dwPLT;
		dwRT = dwTC;
		dwLB = dwLC;
		dwRB = dwPCenter;
		break;

	case CHILD_RT:
		dwLT = dwTC;
		dwRT = dwPRT;
		dwLB = dwPCenter;
		dwRB = dwRC;
		break;

	case CHILD_LB:
		dwLT = dwLC;
		dwRT = dwPCenter;
		dwLB = dwPLB;
		dwRB = dwBC;
		break;

	case CHILD_RB:
		dwLT = dwPCenter;
		dwRT = dwRC;
		dwLB = dwBC;
		dwRB = dwPRB;
		break;
	}

	*pLT = dwLT;
	*pRT = dwRT;
	*pLB = dwLB;
	*pRB = dwRB;

}

void Engine::CQuadTree::Set_Corner(const _ulong& dwLT, const _ulong& dwRT, const _ulong& dwLB, const _ulong& dwRB)
{
	m_dwCorner[CORNER_LT] = dwLT;
	m_dwCorner[CORNER_RT] = dwRT;
	m_dwCorner[CORNER_LB] = dwLB;
	m_dwCorner[CORNER_RB] = dwRB;
	
	m_dwCenter = (m_dwCorner[CORNER_LT] + m_dwCorner[CORNER_RT] + m_dwCorner[CORNER_LB] + m_dwCorner[CORNER_RB]) >> 2;
}

CQuadTree* Engine::CQuadTree::Create(const _ulong& dwCntX, const _ulong& dwCntZ)
{
	CQuadTree*	pInstance = new CQuadTree;

	if (FAILED(pInstance->Ready_QuadTree(dwCntX, dwCntZ)))
		Safe_Release(pInstance);

	if (FAILED(pInstance->Ready_Neighbor()))
		Safe_Release(pInstance);

	return pInstance;
}

CQuadTree* Engine::CQuadTree::Create(const _ulong& dwLT, const _ulong& dwRT, const _ulong& dwLB, const _ulong& dwRB)
{
	CQuadTree*		pInstance = new CQuadTree;

	pInstance->Set_Corner(dwLT, dwRT, dwLB, dwRB);

	return pInstance;
}

void Engine::CQuadTree::Free(void)
{
	for (_ulong i = 0; i < CHILD_END; ++i)
		Safe_Release(m_pChild[i]);
}

Engine::_bool Engine::CQuadTree::LevelOfDetail(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pVtxPos)
{
	_matrix	matCamWorld;
	pGraphicDev->GetTransform(D3DTS_VIEW, &matCamWorld);
	D3DXMatrixInverse(&matCamWorld, NULL, &matCamWorld);

	_vec3	vCamPos;
	memcpy(&vCamPos, &matCamWorld.m[3][0], sizeof(_vec3));

	_float	fDistance = D3DXVec3Length(&(vCamPos - pVtxPos[m_dwCenter]));

	_float	fWidth = D3DXVec3Length(&(pVtxPos[m_dwCorner[CORNER_RT]] - pVtxPos[m_dwCorner[CORNER_LT]]));

	return fDistance * 0.1f > fWidth;
}

