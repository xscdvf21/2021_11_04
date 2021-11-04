#include "Frustum.h"
#include "QuadTree.h"

USING(Engine)

Engine::CFrustum::CFrustum(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
{
	m_pGraphicDev->AddRef();
}

CFrustum::CFrustum(const CFrustum & rhs)
	: m_pGraphicDev(rhs.m_pGraphicDev)
{
	for (_uint i = 0; i < 6; ++i)
		m_Plane[i] = rhs.m_Plane[i];

	for (_uint i = 0; i < 8; ++i)
		m_vPoint[i] = rhs.m_vPoint[i];

	m_pGraphicDev->AddRef();
}

Engine::CFrustum::~CFrustum(void)
{

}

HRESULT Engine::CFrustum::Ready_Frustum(void)
{
	m_vPoint[0] = _vec3(-1.f, 1.f, 0.f);
	m_vPoint[1] = _vec3(1.f, 1.f, 0.f);
	m_vPoint[2] = _vec3(1.f, -1.f, 0.f);
	m_vPoint[3] = _vec3(-1.f, -1.f, 0.f);

	m_vPoint[4] = _vec3(-1.f, 1.f, 1.f);
	m_vPoint[5] = _vec3(1.f, 1.f, 1.f);
	m_vPoint[6] = _vec3(1.f, -1.f, 1.f);
	m_vPoint[7] = _vec3(-1.f, -1.f, 1.f);

	return S_OK;
}

Engine::_bool Engine::CFrustum::IsIn_FrustumForObject(const _vec3* pPos, const _float& fRadius)
{
	FAILED_CHECK_RETURN(Ready_Frustum(), false);

	_matrix		matProj;
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, NULL, &matProj);

	_matrix		matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, NULL, &matView);

	for (_uint i = 0; i < 8; ++i)
	{
		D3DXVec3TransformCoord(&m_vPoint[i], &m_vPoint[i], &matProj);
		D3DXVec3TransformCoord(&m_vPoint[i], &m_vPoint[i], &matView);
	}

	// x+
	D3DXPlaneFromPoints(&m_Plane[0], &m_vPoint[1], &m_vPoint[5], &m_vPoint[6]);

	// x-
	D3DXPlaneFromPoints(&m_Plane[1], &m_vPoint[4], &m_vPoint[0], &m_vPoint[3]);
	
	// y+
	D3DXPlaneFromPoints(&m_Plane[2], &m_vPoint[4], &m_vPoint[5], &m_vPoint[1]);
	
	// y-
	D3DXPlaneFromPoints(&m_Plane[3], &m_vPoint[3], &m_vPoint[2], &m_vPoint[6]);
	
	// z+
	D3DXPlaneFromPoints(&m_Plane[4], &m_vPoint[7], &m_vPoint[6], &m_vPoint[5]);
	
	// z-
	D3DXPlaneFromPoints(&m_Plane[5], &m_vPoint[0], &m_vPoint[1], &m_vPoint[2]);
	
	return IsIn_Frustum(pPos);
}

Engine::_bool Engine::CFrustum::IsIn_Frustum(const _vec3* pPos)
{
	_float		fResult = 0.f;

	for (_uint i = 0; i < 6; ++i)
	{
		fResult = D3DXPlaneDotCoord(&m_Plane[i], pPos);

		if (fResult > 0.f)
			return false;
	}
	
	return true;
}

Engine::_bool Engine::CFrustum::IsIn_Frustum(const _vec3* pPos, const _float& fRadius)
{
	_float		fResult = 0.f;

	for (_uint i = 0; i < 6; ++i)
	{
		fResult = D3DXPlaneDotCoord(&m_Plane[i], pPos);

		if (fResult > fRadius)
			return false;
	}

	return true;
}

CFrustum* Engine::CFrustum::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CFrustum*	pInstance = new CFrustum(pGraphicDev);

	if (FAILED(pInstance->Ready_Frustum()))
		Safe_Release(pInstance);

	return pInstance;
}


void Engine::CFrustum::Free(void)
{
	Safe_Release(m_pGraphicDev);
}

void Engine::CFrustum::IsIn_FrustumForTerrain(const _vec3* pVtxPos, const _ulong& dwCntX, 
	const _ulong& dwCntZ, INDEX32* pIndex, _ulong* pTriCnt, CQuadTree* pQuadTree)
{
	FAILED_CHECK_RETURN(Ready_Frustum(), );

	_matrix		matProj;
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, NULL, &matProj);

	_matrix		matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, NULL, &matView);

	for (_uint i = 0; i < 8; ++i)
	{
		D3DXVec3TransformCoord(&m_vPoint[i], &m_vPoint[i], &matProj);
		D3DXVec3TransformCoord(&m_vPoint[i], &m_vPoint[i], &matView);
	}

	// x+
	D3DXPlaneFromPoints(&m_Plane[0], &m_vPoint[1], &m_vPoint[5], &m_vPoint[6]);

	// x-
	D3DXPlaneFromPoints(&m_Plane[1], &m_vPoint[4], &m_vPoint[0], &m_vPoint[3]);

	// y+
	D3DXPlaneFromPoints(&m_Plane[2], &m_vPoint[4], &m_vPoint[5], &m_vPoint[1]);

	// y-
	D3DXPlaneFromPoints(&m_Plane[3], &m_vPoint[3], &m_vPoint[2], &m_vPoint[6]);

	// z+
	D3DXPlaneFromPoints(&m_Plane[4], &m_vPoint[7], &m_vPoint[6], &m_vPoint[5]);

	// z-
	D3DXPlaneFromPoints(&m_Plane[5], &m_vPoint[0], &m_vPoint[1], &m_vPoint[2]);
	
	*pTriCnt = 0;

	if (nullptr != pQuadTree)
	{
		pQuadTree->CullingForTerrain(this, pVtxPos, pIndex, pTriCnt);
	}
	else
	{
		_bool	bIsIn[3] = { false };
		_ulong	dwTriCnt = 0;


		for (_ulong i = 0; i < dwCntZ - 1; ++i)
		{
			for (_ulong j = 0; j < dwCntX - 1; ++j)
			{
				_ulong	dwIndex = i * dwCntX + j;

				// ������ ��
				bIsIn[0] = IsIn_Frustum(&pVtxPos[dwIndex + dwCntX]);
				bIsIn[1] = IsIn_Frustum(&pVtxPos[dwIndex + dwCntX + 1]);
				bIsIn[2] = IsIn_Frustum(&pVtxPos[dwIndex + 1]);

				if (true == bIsIn[0] || true == bIsIn[1] || true == bIsIn[2])
				{
					pIndex[dwTriCnt]._0 = dwIndex + dwCntX;
					pIndex[dwTriCnt]._1 = dwIndex + dwCntX + 1;
					pIndex[dwTriCnt]._2 = dwIndex + 1;
					++dwTriCnt;
				}

				// ���� �Ʒ�
				bIsIn[0] = IsIn_Frustum(&pVtxPos[dwIndex + dwCntX]);
				bIsIn[1] = IsIn_Frustum(&pVtxPos[dwIndex + 1]);
				bIsIn[2] = IsIn_Frustum(&pVtxPos[dwIndex]);

				if (true == bIsIn[0] || true == bIsIn[1] || true == bIsIn[2])
				{
					pIndex[dwTriCnt]._0 = dwIndex + dwCntX;
					pIndex[dwTriCnt]._1 = dwIndex + 1;
					pIndex[dwTriCnt]._2 = dwIndex;
					++dwTriCnt;
				}
			}
		}

		*pTriCnt = dwTriCnt;

	}


}

