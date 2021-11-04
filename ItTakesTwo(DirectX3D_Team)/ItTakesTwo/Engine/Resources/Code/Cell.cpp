#include "Cell.h"

USING(Engine)

Engine::CCell::CCell(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
	, m_pD3DXLine(nullptr)
{
	ZeroMemory(m_pNeighbor, sizeof(CCell*) * NEIGHBOR_END);
	ZeroMemory(m_pLine, sizeof(CLine*) * LINE_END);

	m_pGraphicDev->AddRef();
}

Engine::CCell::~CCell(void)
{

}

_bool CCell::Get_State()
{
	if (m_eCellState == STATE_GROUND)
		return true;
	else if (m_eCellState == STATE_FALL)
		return false;

	return true;
}

HRESULT Engine::CCell::Ready_Cell(const _ulong&	 dwCellIndex, const _vec3* pPointA, const _vec3* pPointB, const _vec3* pPointC, _uint eState, _bool lock)
{
	m_dwCellIndex = dwCellIndex;

	m_vPoint[POINT_A] = *pPointA;
	m_vPoint[POINT_B] = *pPointB;
	m_vPoint[POINT_C] = *pPointC;

	m_pLine[LINE_AB] = CLine::Create(&_vec2(m_vPoint[POINT_A].x, m_vPoint[POINT_A].z),
		&_vec2(m_vPoint[POINT_B].x, m_vPoint[POINT_B].z));

	m_pLine[LINE_BC] = CLine::Create(&_vec2(m_vPoint[POINT_B].x, m_vPoint[POINT_B].z),
		&_vec2(m_vPoint[POINT_C].x, m_vPoint[POINT_C].z));

	m_pLine[LINE_CA] = CLine::Create(&_vec2(m_vPoint[POINT_C].x, m_vPoint[POINT_C].z),
		&_vec2(m_vPoint[POINT_A].x, m_vPoint[POINT_A].z));

	m_eCellState = (CCell::STATE)eState;
	m_bLockArea = lock;


	if (FAILED(D3DXCreateLine(m_pGraphicDev, &m_pD3DXLine)))
		return E_FAIL;


	return S_OK;
}

Engine::_bool Engine::CCell::Compare_Point(const _vec3* pFirst, const _vec3* pSecond, CCell* pCell)
{
	if (m_vPoint[POINT_A] == *pFirst)
	{
		if (m_vPoint[POINT_B] == *pSecond)
		{
			m_pNeighbor[NEIGHBOR_AB] = pCell;
			return true;
		}

		if (m_vPoint[POINT_C] == *pSecond)
		{
			m_pNeighbor[NEIGHBOR_CA] = pCell;
			return true;
		}
	}

	if (m_vPoint[POINT_B] == *pFirst)
	{
		if (m_vPoint[POINT_C] == *pSecond)
		{
			m_pNeighbor[NEIGHBOR_BC] = pCell;
			return true;
		}

		if (m_vPoint[POINT_A] == *pSecond)
		{
			m_pNeighbor[NEIGHBOR_AB] = pCell;
			return true;
		}
	}

	if (m_vPoint[POINT_C] == *pFirst)
	{
		if (m_vPoint[POINT_B] == *pSecond)
		{
			m_pNeighbor[NEIGHBOR_BC] = pCell;
			return true;
		}

		if (m_vPoint[POINT_A] == *pSecond)
		{
			m_pNeighbor[NEIGHBOR_CA] = pCell;
			return true;
		}
	}

	return false;
}

void Engine::CCell::Render_Cell(void)
{
	if (m_pGraphicDev == nullptr)
		return;

	_vec3		vPoint[4];

	vPoint[0] = m_vPoint[POINT_A];
	vPoint[1] = m_vPoint[POINT_B];
	vPoint[2] = m_vPoint[POINT_C];
	vPoint[3] = m_vPoint[POINT_A];

	_matrix	matView, matProj;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	for (_ulong i = 0; i < 4; ++i)
	{
		D3DXVec3TransformCoord(&vPoint[i], &vPoint[i], &matView);

		if (vPoint[i].z <= 0.1f)
			vPoint[i].z = 0.1f;

		D3DXVec3TransformCoord(&vPoint[i], &vPoint[i], &matProj);
	}

	m_pD3DXLine->SetWidth(3.f); // 선의 굵기를 결정

	m_pGraphicDev->EndScene();
	m_pGraphicDev->BeginScene();

	m_pD3DXLine->Begin();

	//3d 라인을 그려주는 함수
	_matrix		matTemp;
	if (m_eCellState == STATE_GROUND)
		m_pD3DXLine->DrawTransform(vPoint, 4, D3DXMatrixIdentity(&matTemp), D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
	else if (m_eCellState == STATE_FALL)
		m_pD3DXLine->DrawTransform(vPoint, 4, D3DXMatrixIdentity(&matTemp), D3DXCOLOR(0.f, 0.f, 1.f, 1.f));

	m_pD3DXLine->End();
}


CCell* Engine::CCell::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ulong&	dwCellIndex, const _vec3* pPointA, const _vec3* pPointB, const _vec3* pPointC, _uint eState, _bool lock)
{
	CCell*	pInstance = new CCell(pGraphicDev);

	if (FAILED(pInstance->Ready_Cell(dwCellIndex, pPointA, pPointB, pPointC, eState, lock)))
		Safe_Release(pInstance);

	return pInstance;
}

void Engine::CCell::Free(void)
{
	for (_uint i = 0; i < LINE_END; ++i)
		Safe_Release(m_pLine[i]);

	Safe_Release(m_pD3DXLine);
	Safe_Release(m_pGraphicDev);
}

Engine::CCell::COMPAREMOVE Engine::CCell::Compare(const _vec3* pEndPos, _ulong* pCellIndex)
{
	for (_ulong i = 0; i < LINE_END; ++i)
	{
		if (COMPARE_LEFT == m_pLine[i]->Compare(&_vec2(pEndPos->x, pEndPos->z)))
		{
			if (nullptr == m_pNeighbor[i])
				return CCell::STOP;

			else
			{
				*pCellIndex = *m_pNeighbor[i]->Get_CellIndex();
				return CCell::MOVE;
			}
		}
	}

	return CCell::MOVE;
}

Engine::CCell::COMPAREMOVE Engine::CCell::Compare(const _vec3* pEndPos, _ulong* pCellIndex, _vec2* vNormal)
{
	for (_ulong i = 0; i < LINE_END; ++i)
	{
		if (COMPARE_LEFT == m_pLine[i]->Compare(&_vec2(pEndPos->x, pEndPos->z)))
		{
			if (nullptr == m_pNeighbor[i])
			{
				vNormal->x = m_pLine[i]->Get_Normal().x;
				vNormal->y = m_pLine[i]->Get_Normal().y;
				return CCell::STOP;
			}
			else
			{
				*pCellIndex = *m_pNeighbor[i]->Get_CellIndex();
				return CCell::MOVE;
			}
		}
	}

	return CCell::MOVE;
}

_bool CCell::Find_Cell(_vec3 pEndPos, _ulong * pCellIndex)
{
	for (_ulong i = 0; i < LINE_END; ++i)
	{
		if (COMPARE_LEFT == m_pLine[i]->Compare(&_vec2(pEndPos.x, pEndPos.z)))
		{
			return true;
		}
	}
	*pCellIndex = (_ulong)*(this->Get_CellIndex());
	return false;
}