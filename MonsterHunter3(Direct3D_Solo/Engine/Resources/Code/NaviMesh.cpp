#include "NaviMesh.h"

USING(Engine)
Engine::CNaviMesh::CNaviMesh(LPDIRECT3DDEVICE9 pGraphicDev)
	: CMesh(pGraphicDev)
	, m_dwCellIndex(0)
{

}

Engine::CNaviMesh::CNaviMesh(const CNaviMesh& rhs)
	: CMesh(rhs)
	, m_vecCell(rhs.m_vecCell)
	, m_dwCellIndex(rhs.m_dwCellIndex)
{
	for (auto& iter : m_vecCell)
		iter->AddRef();
}

Engine::CNaviMesh::~CNaviMesh(void)
{

}

HRESULT Engine::CNaviMesh::Ready_NaviMesh(_uint iNaviSize, vector<CCell*> vecCell)
{
	//m_vecCell.reserve(4);

	//CCell*		pCell = nullptr;

	//// 0
	//pCell = CCell::Create(m_pGraphicDev, m_vecCell.size(), &_vec3(0.f, 0.f, 2.f), &_vec3(2.f, 0.f, 0.f), &_vec3(0.f, 0.f, 0.f));
	//NULL_CHECK_RETURN(pCell, E_FAIL);
	//m_vecCell.push_back(pCell);

	//// 1
	//pCell = CCell::Create(m_pGraphicDev, m_vecCell.size(), &_vec3(0.f, 0.f, 2.f), &_vec3(2.f, 0.f, 2.f), &_vec3(2.f, 0.f, 0.f));
	//NULL_CHECK_RETURN(pCell, E_FAIL);
	//m_vecCell.push_back(pCell);

	//// 2
	//pCell = CCell::Create(m_pGraphicDev, m_vecCell.size(), &_vec3(0.f, 0.f, 4.f), &_vec3(2.f, 0.f, 2.f), &_vec3(0.f, 0.f, 2.f));
	//NULL_CHECK_RETURN(pCell, E_FAIL);
	//m_vecCell.push_back(pCell);

	//// 3
	//pCell = CCell::Create(m_pGraphicDev, m_vecCell.size(), &_vec3(2.f, 0.f, 2.f), &_vec3(4.f, 0.f, 0.f), &_vec3(2.f, 0.f, 0.f));
	//NULL_CHECK_RETURN(pCell, E_FAIL);
	//m_vecCell.push_back(pCell);
	m_vecCell.reserve(iNaviSize);

	CCell*		pCell = nullptr;

	m_vecCell = vecCell;

	if (FAILED(Link_Cell()))
		return E_FAIL;

	return S_OK;

}

void CNaviMesh::Render_NaviMesh(void)
{
	for (auto& iter : m_vecCell)
		iter->Render_Cell();
}

_vec3 CNaviMesh::Move_OnNaviMesh(const _vec3 * pTargetPos, const _vec3 * pTargetDir)
{

	_vec3	vEndPos = *pTargetPos + *pTargetDir;
	_float vPlaneY = 0.0f;

	if (CCell::MOVE == m_vecCell[m_dwCellIndex]->Compare(&vEndPos, &m_dwCellIndex))
	{
		D3DXPLANE plane;

		D3DXPlaneFromPoints(
			&plane,
			m_vecCell[m_dwCellIndex]->Get_Point(CCell::POINT_A),
			m_vecCell[m_dwCellIndex]->Get_Point(CCell::POINT_B),
			m_vecCell[m_dwCellIndex]->Get_Point(CCell::POINT_C));

		vPlaneY = -(vEndPos.x * plane.a + vEndPos.z * plane.c + plane.d) / plane.b;

		if (vPlaneY >= vEndPos.y)
			vEndPos.y = vPlaneY;

		return vEndPos;
	}
	else if (CCell::STOP == m_vecCell[m_dwCellIndex]->Compare(&vEndPos, &m_dwCellIndex))
	{
		return *pTargetPos;
	}

	return *pTargetPos;


	//_vec3	vEndPos = *pTargetPos + *pTargetDir;

	//if (CCell::MOVE == m_vecCell[m_dwCellIndex]->Compare(&vEndPos, &m_dwCellIndex))
	//{
	//	return vEndPos;
	//}
	//else if (CCell::STOP == m_vecCell[m_dwCellIndex]->Compare(&vEndPos, &m_dwCellIndex))
	//{
	//	return *pTargetPos;
	//}
}

HRESULT Engine::CNaviMesh::Link_Cell(void)
{

	for (_ulong i = 0; i < m_vecCell.size(); ++i)
	{
		for (_ulong j = 0; j < m_vecCell.size(); ++j)
		{
			if(i == j)
				continue;

			// 이웃하는 셀 정보가 없는 경우, 내가 찾은 두개의 정점이 이웃 셀로서 일치하는 경우
			if (nullptr == m_vecCell[i]->Get_Neighbor(CCell::NEIGHBOR_AB) &&
				true == m_vecCell[j]->Compare_Point(m_vecCell[i]->Get_Point(CCell::POINT_A),
													m_vecCell[i]->Get_Point(CCell::POINT_B), 
													m_vecCell[i]))
			{
				m_vecCell[i]->Set_Neighbor(CCell::NEIGHBOR_AB, m_vecCell[j]);
				continue;
			}

			if (nullptr == m_vecCell[i]->Get_Neighbor(CCell::NEIGHBOR_BC) &&
				true == m_vecCell[j]->Compare_Point(m_vecCell[i]->Get_Point(CCell::POINT_B),
													m_vecCell[i]->Get_Point(CCell::POINT_C),
													m_vecCell[i]))
			{
				m_vecCell[i]->Set_Neighbor(CCell::NEIGHBOR_BC, m_vecCell[j]);
				continue;
			}


			if (nullptr == m_vecCell[i]->Get_Neighbor(CCell::NEIGHBOR_CA) &&
				true == m_vecCell[j]->Compare_Point(m_vecCell[i]->Get_Point(CCell::POINT_C),
													m_vecCell[i]->Get_Point(CCell::POINT_A),
													m_vecCell[i]))
			{
				m_vecCell[i]->Set_Neighbor(CCell::NEIGHBOR_CA, m_vecCell[j]);
				continue;
			}

		}
	}
	return S_OK;
}


CNaviMesh * CNaviMesh::Create(LPDIRECT3DDEVICE9 pGraphicDev, _uint iNaviSize, vector<CCell*> vecCell)
{
	CNaviMesh*	pInstance = new CNaviMesh(pGraphicDev);

	if (FAILED(pInstance->Ready_NaviMesh(iNaviSize, vecCell)))
		Safe_Release(pInstance);

	return pInstance;

}


CComponent* Engine::CNaviMesh::Clone(void)
{
	return new CNaviMesh(*this);
}

void Engine::CNaviMesh::Free(void)
{
	CMesh::Free();

	for_each(m_vecCell.begin(), m_vecCell.end(), CDeleteObj());
	m_vecCell.clear();
}