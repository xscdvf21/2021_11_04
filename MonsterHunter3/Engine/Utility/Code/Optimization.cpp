#include "Optimization.h"

USING(Engine)

Engine::COptimization::COptimization(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
	, m_pFrustum(nullptr)
	, m_pQuadTree(nullptr)
	, m_bClone(false)
{
	m_pGraphicDev->AddRef();
}

Engine::COptimization::COptimization(const COptimization& rhs)
	: m_pGraphicDev(rhs.m_pGraphicDev)
	, m_pFrustum(rhs.m_pFrustum)
	, m_pQuadTree(rhs.m_pQuadTree)
	, m_bClone(true)
{
	m_pGraphicDev->AddRef();
}

Engine::COptimization::~COptimization(void)
{

}

HRESULT Engine::COptimization::Ready_Optimization(_bool bChoice, const _ulong& dwCntX, const _ulong& dwCntZ)
{
	m_pFrustum = CFrustum::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(m_pFrustum, E_FAIL);

	if (false == bChoice)
		return S_OK;

	m_pQuadTree = CQuadTree::Create(dwCntX, dwCntZ);
	NULL_CHECK_RETURN(m_pQuadTree, E_FAIL);

	return S_OK;
}

Engine::_bool Engine::COptimization::IsIn_FrustumForObject(const _vec3* pPos, const _float& fRadius)
{
	return m_pFrustum->IsIn_FrustumForObject(pPos, fRadius);
}

COptimization* Engine::COptimization::Create(LPDIRECT3DDEVICE9 pGraphicDev, _bool bChoice, const _ulong& dwCntX, const _ulong& dwCntZ)
{
	COptimization*	pInstance = new COptimization(pGraphicDev);

	if (FAILED(pInstance->Ready_Optimization(bChoice, dwCntX, dwCntZ)))
		Safe_Release(pInstance);

	return pInstance;
}

CComponent* Engine::COptimization::Clone(void)
{
	return new COptimization(*this);
}

void Engine::COptimization::Free(void)
{
	if (false == m_bClone)
	{
		Safe_Release(m_pFrustum);
		Safe_Release(m_pQuadTree);
	}
	
	Safe_Release(m_pGraphicDev);	

}

void Engine::COptimization::IsIn_FrustumForTerrain(const _vec3* pVtxPos, const _ulong& dwCntX, const _ulong& dwCntZ, INDEX32* pIndex, _ulong* pTriCnt)
{
	m_pFrustum->IsIn_FrustumForTerrain(pVtxPos, dwCntX,dwCntZ, pIndex, pTriCnt, m_pQuadTree);
}

