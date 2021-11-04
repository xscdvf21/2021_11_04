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
	for (_uint i = 0; i < m_vecCell.size(); ++i)
	{
		for (_uint j = 0; j < m_vecCell[i].size(); ++j)
			m_vecCell[i][j]->AddRef();
	}
}

Engine::CNaviMesh::~CNaviMesh(void)
{
}

HRESULT Engine::CNaviMesh::Ready_NaviMesh(void)
{
	m_vecCell.reserve(10);

	return S_OK;
}

void CNaviMesh::Render_NaviMesh(void)
{
	for (_uint i = 0; i < m_vecCell.size(); ++i)
	{
		for (_uint j = 0; j < m_vecCell[i].size(); ++j)
			m_vecCell[i][j]->Render_Cell();
	}
}

_vec3 CNaviMesh::Move_OnNaviMesh(const _vec3 * pTargetPos, const _vec3 * pTargetDir, _uint vecIndex, _bool* State, _vec3* vFallPos)
{
	_vec3	vEndPos = *pTargetPos + *pTargetDir;
	_vec2	vNormal = _vec2{};
	_vec2	vSlide = _vec2{};
	_vec2	vTargetDir = _vec2{};
	_vec3	vec3Silde = _vec3{};

	_ulong Index = 0;

	for (_uint i = 0; i < m_vecCell[vecIndex].size(); ++i)
		m_vecCell[vecIndex][i]->Find_Cell(*pTargetPos, &Index);

	if (CCell::MOVE == m_vecCell[vecIndex][Index]->Compare(&vEndPos, &Index) || !m_vecCell[vecIndex][Index]->Get_State())
	{
		if (m_vecCell[vecIndex][Index]->Get_State())
		{
			D3DXPLANE tPlane;
			D3DXPlaneFromPoints(&tPlane,
				m_vecCell[vecIndex][Index]->Get_Point(CCell::POINT_A),
				m_vecCell[vecIndex][Index]->Get_Point(CCell::POINT_B),
				m_vecCell[vecIndex][Index]->Get_Point(CCell::POINT_C));
			vEndPos.y = -(vEndPos.x * tPlane.a + vEndPos.z * tPlane.c + tPlane.d) / tPlane.b;

			if (vFallPos != nullptr)
				*vFallPos = vEndPos;
		}
		else
		{
			if (State != nullptr)
				*State = true;
		}
		m_dwCellIndex = Index;
		return vEndPos;
	}
	else if (CCell::STOP == m_vecCell[vecIndex][Index]->Compare(&vEndPos, &Index, &vNormal))
	{
		//S = P - n(P·n)
		// P = pTargetDir
		// n = 라인에 법선벡터
		vTargetDir.x = pTargetDir->x;
		vTargetDir.y = pTargetDir->z;

		vSlide = vTargetDir - vNormal * ((D3DXVec2Dot(&vTargetDir, &vNormal)));
		vec3Silde = _vec3(vSlide.x, pTargetDir->y, vSlide.y);
		vec3Silde = *pTargetPos + vec3Silde;

		D3DXPLANE tPlane;
		D3DXPlaneFromPoints(&tPlane,
			m_vecCell[vecIndex][Index]->Get_Point(CCell::POINT_A),
			m_vecCell[vecIndex][Index]->Get_Point(CCell::POINT_B),
			m_vecCell[vecIndex][Index]->Get_Point(CCell::POINT_C));
		vec3Silde.y = -(vec3Silde.x * tPlane.a + vec3Silde.z * tPlane.c + tPlane.d) / tPlane.b;

		m_dwCellIndex = Index;
		return vec3Silde;
	}
	m_dwCellIndex = Index;
	return _vec3();
}

_float3 CNaviMesh::Jump_OnNaviMesh(const _float3* pPos, const _float* fHeight, _float* fDis, _uint* pCellIndex, _bool* bJump)
{
	_vec3	vEndPos = *pPos;
	_float vPlaneY = 0.0f;
	D3DXPLANE plane;

	D3DXPlaneFromPoints(
		&plane,
		m_vecCell[0][*pCellIndex]->Get_Point(CCell::POINT_A),
		m_vecCell[0][*pCellIndex]->Get_Point(CCell::POINT_B),
		m_vecCell[0][*pCellIndex]->Get_Point(CCell::POINT_C));

	vPlaneY = -(vEndPos.x * plane.a + vEndPos.z * plane.c + plane.d) / plane.b;


	if (vPlaneY >= vEndPos.y + *fHeight)
	{
		vEndPos.y = vPlaneY;
		*bJump = false;
	}
	else
		vEndPos.y += *fHeight;

	*fDis = vEndPos.y - vPlaneY;

	return vEndPos;
}

_vec3 CNaviMesh::Slide_OnNaviMesh(const _vec3 * pTargetPos, const _vec3 * pTargetDir, _uint vecIndex, _bool * State)
{
	_vec3	vEndPos = *pTargetPos + *pTargetDir;
	_vec2	vNormal = _vec2{};
	_vec2	vSlide = _vec2{};
	_vec2	vTargetDir = _vec2{};
	_vec3	vec3Silde = _vec3{};

	_ulong Index = 0;

	if (CCell::MOVE == m_vecCell[vecIndex][m_dwCellIndex]->Compare(&vEndPos, &m_dwCellIndex) || !m_vecCell[vecIndex][m_dwCellIndex]->Get_State())
	{
		if (m_vecCell[vecIndex][m_dwCellIndex]->Get_State())
		{
			D3DXPLANE tPlane;
			D3DXPlaneFromPoints(&tPlane,
				m_vecCell[vecIndex][m_dwCellIndex]->Get_Point(CCell::POINT_A),
				m_vecCell[vecIndex][m_dwCellIndex]->Get_Point(CCell::POINT_B),
				m_vecCell[vecIndex][m_dwCellIndex]->Get_Point(CCell::POINT_C));
			vEndPos.y = -(vEndPos.x * tPlane.a + vEndPos.z * tPlane.c + tPlane.d) / tPlane.b;
		}
		else
		{
			if (State != nullptr)
				*State = true;
		}

		return vEndPos;
	}
	else if (CCell::STOP == m_vecCell[vecIndex][m_dwCellIndex]->Compare(&vEndPos, &m_dwCellIndex, &vNormal))
	{
		//S = P - n(P·n)
		// P = pTargetDir
		// n = 라인에 법선벡터
		vTargetDir.x = pTargetDir->x;
		vTargetDir.y = pTargetDir->z;

		vSlide = vTargetDir - vNormal * ((D3DXVec2Dot(&vTargetDir, &vNormal)));
		vec3Silde = _vec3(vSlide.x, pTargetDir->y, vSlide.y);
		vec3Silde = *pTargetPos + vec3Silde;

		D3DXPLANE tPlane;
		D3DXPlaneFromPoints(&tPlane,
			m_vecCell[vecIndex][m_dwCellIndex]->Get_Point(CCell::POINT_A),
			m_vecCell[vecIndex][m_dwCellIndex]->Get_Point(CCell::POINT_B),
			m_vecCell[vecIndex][m_dwCellIndex]->Get_Point(CCell::POINT_C));
		vec3Silde.y = -(vec3Silde.x * tPlane.a + vec3Silde.z * tPlane.c + tPlane.d) / tPlane.b;
		return vec3Silde;
	}


	//	_vec3	vEndPos = *pTargetPos + *pTargetDir;

	//	if (CCell::MOVE == m_vecCell[m_dwCellIndex]->Compare(&vEndPos, &m_dwCellIndex))
	//	{
	//		return vEndPos;
	//	}
	//	else if (CCell::STOP == m_vecCell[m_dwCellIndex]->Compare(&vEndPos, &m_dwCellIndex))
	//	{
	//		return *pTargetPos;
	//	}

	return _vec3();
}

void CNaviMesh::Add_CellGroup()
{
	vector<CCell*> vecCell;

	m_vecCell.push_back(vecCell);
}

void CNaviMesh::Add_Cell(_vec3 first, _vec3 second, _vec3 third, _uint vecIndex, _uint eState, _bool lock)
{
	CCell*		pCell = nullptr;

	pCell = CCell::Create(m_pGraphicDev, _ulong(m_vecCell[vecIndex].size()), &first, &second, &third, eState, lock);
	NULL_CHECK_RETURN(pCell, );
	m_vecCell[vecIndex].push_back(pCell);
}

void CNaviMesh::Delete_CellGroup(_int GroupiIndex)
{
	for (auto& iter : m_vecCell[GroupiIndex])
	{
		Safe_Release(iter);
	}
	m_vecCell[GroupiIndex].clear();
	m_vecCell.erase(m_vecCell.begin() + GroupiIndex);
	m_vecCell.shrink_to_fit();
}

void CNaviMesh::Delete_Cell(_int GroupIndex, _int CellIndex)
{
	Safe_Release(m_vecCell[GroupIndex][CellIndex]);
	m_vecCell[GroupIndex].erase(m_vecCell[GroupIndex].begin() + CellIndex);
}

_bool CNaviMesh::Find_Cell(_vec3 vPos, _float* Height, _uint* GroupIndex)
{
	_ulong Index = 0;
	_vec3 a, b, c;
	_float avgY = 0;

	for (_uint i = 0; i < m_vecCell.size(); ++i)
	{
		for (_uint j = 0; j < m_vecCell[i].size(); ++j)
		{
			if (!m_vecCell[i][j]->Find_Cell(vPos, &Index))
			{
				a = *m_vecCell[i][Index]->Get_Point(CCell::POINT_A);
				b = *m_vecCell[i][Index]->Get_Point(CCell::POINT_B);
				c = *m_vecCell[i][Index]->Get_Point(CCell::POINT_C);

				avgY = (a.y + b.y + c.y) / 3.f;
				if (Height)
					*Height = avgY;
				*GroupIndex = i;
				if (vPos.y < avgY)
					return m_vecCell[i][Index]->Get_State();

				// TRUE = GROUND
				// FALSE = FALL
			}
		}
	}
	return false;
}

void CNaviMesh::Load_Cell(const _tchar * pFilePath)
{
	HANDLE hFile = CreateFile(pFilePath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
		return;

	DWORD dwByte = 0;

	_uint size = 0;
	_uint index = 0;
	_vec3 vPoint1, vPoint2, vPoint3;
	_uint eState;
	_bool bLock;

	while (true)
	{
		ReadFile(hFile, &size, sizeof(_uint), &dwByte, nullptr);

		if (dwByte == 0)
			break;

		Add_CellGroup();

		for (_uint i = 0; i < size; ++i)
		{
			ReadFile(hFile, &vPoint1, sizeof(_vec3), &dwByte, nullptr);
			ReadFile(hFile, &vPoint2, sizeof(_vec3), &dwByte, nullptr);
			ReadFile(hFile, &vPoint3, sizeof(_vec3), &dwByte, nullptr);
			ReadFile(hFile, &eState, sizeof(CCell::STATE), &dwByte, nullptr);
			ReadFile(hFile, &bLock, sizeof(_bool), &dwByte, nullptr);

			Add_Cell(vPoint1, vPoint2, vPoint3, index, eState, bLock);
		}
		Link_Cell(index);
		++index;
	}

	if (m_vecCell.size() > 2)
	{
		for (auto& iter : m_vecCell[2])
			iter->Set_Lock(false);
	}

	for (_uint i = 0; i < m_vecCell.size(); ++i)
	{
		_uint size = _uint(m_vecCell[i].size());
		for (_uint j = 0; j < size; ++j)
		{
			_vec3 vCenter = (m_vecCell[i][j]->Get_PointA() + m_vecCell[i][j]->Get_PointB() + m_vecCell[i][j]->Get_PointC());
			vCenter = _vec3(vCenter.x / 3, vCenter.y / 3, vCenter.z / 3);
			if (m_vecCell[i][j]->Get_Neighbor(CCell::NEIGHBOR_AB) == nullptr && !m_vecCell[i][j]->Get_Lock())
			{
				Add_Cell(m_vecCell[i][j]->Get_PointA(),
					m_vecCell[i][j]->Get_PointA() + m_vecCell[i][j]->Get_PointB() - vCenter,
					m_vecCell[i][j]->Get_PointB(),
					i, CCell::STATE_FALL, true);
			}

			if (m_vecCell[i][j]->Get_Neighbor(CCell::NEIGHBOR_BC) == nullptr && !m_vecCell[i][j]->Get_Lock())
			{
				Add_Cell(m_vecCell[i][j]->Get_PointB(),
					m_vecCell[i][j]->Get_PointB() + m_vecCell[i][j]->Get_PointC() - vCenter,
					m_vecCell[i][j]->Get_PointC(),
					i, CCell::STATE_FALL, true);
			}

			if (m_vecCell[i][j]->Get_Neighbor(CCell::NEIGHBOR_CA) == nullptr && !m_vecCell[i][j]->Get_Lock())
			{
				Add_Cell(m_vecCell[i][j]->Get_PointC(),
					m_vecCell[i][j]->Get_PointC() + m_vecCell[i][j]->Get_PointA() - vCenter,
					m_vecCell[i][j]->Get_PointA(),
					i, CCell::STATE_FALL, true);
			}
		}
		Link_Cell(i);
	}

	

	CloseHandle(hFile);
}

HRESULT Engine::CNaviMesh::Link_Cell(_uint vecIndex)
{
	for (_ulong i = 0; i < m_vecCell[vecIndex].size(); ++i)
	{
		for (_ulong j = 0; j < m_vecCell[vecIndex].size(); ++j)
		{
			if (i == j)
				continue;

			// 이웃하는 셀 정보가 없는 경우, 내가 찾은 두개의 정점이 이웃 셀로서 일치하는 경우
			if (nullptr == m_vecCell[vecIndex][i]->Get_Neighbor(CCell::NEIGHBOR_AB) &&
				true == m_vecCell[vecIndex][j]->Compare_Point(m_vecCell[vecIndex][i]->Get_Point(CCell::POINT_A),
					m_vecCell[vecIndex][i]->Get_Point(CCell::POINT_B),
					m_vecCell[vecIndex][i]))
			{
				m_vecCell[vecIndex][i]->Set_Neighbor(CCell::NEIGHBOR_AB, m_vecCell[vecIndex][j]);
				continue;
			}

			if (nullptr == m_vecCell[vecIndex][i]->Get_Neighbor(CCell::NEIGHBOR_BC) &&
				true == m_vecCell[vecIndex][j]->Compare_Point(m_vecCell[vecIndex][i]->Get_Point(CCell::POINT_B),
					m_vecCell[vecIndex][i]->Get_Point(CCell::POINT_C),
					m_vecCell[vecIndex][i]))
			{
				m_vecCell[vecIndex][i]->Set_Neighbor(CCell::NEIGHBOR_BC, m_vecCell[vecIndex][j]);
				continue;
			}


			if (nullptr == m_vecCell[vecIndex][i]->Get_Neighbor(CCell::NEIGHBOR_CA) &&
				true == m_vecCell[vecIndex][j]->Compare_Point(m_vecCell[vecIndex][i]->Get_Point(CCell::POINT_C),
					m_vecCell[vecIndex][i]->Get_Point(CCell::POINT_A),
					m_vecCell[vecIndex][i]))
			{
				m_vecCell[vecIndex][i]->Set_Neighbor(CCell::NEIGHBOR_CA, m_vecCell[vecIndex][j]);
				continue;
			}
		}
	}

	return S_OK;
}

CNaviMesh* Engine::CNaviMesh::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CNaviMesh*	pInstance = new CNaviMesh(pGraphicDev);

	if (FAILED(pInstance->Ready_NaviMesh()))
		Safe_Release(pInstance);

	return pInstance;
}

CComponent* Engine::CNaviMesh::Clone(void* pArg)
{
	return new CNaviMesh(*this);
}

void Engine::CNaviMesh::Free(void)
{
	CMesh::Free();

	for (_uint i = 0; i < m_vecCell.size(); ++i)
	{
		for_each(m_vecCell[i].begin(), m_vecCell[i].end(), CDeleteObj());
		m_vecCell[i].clear();
	}
}