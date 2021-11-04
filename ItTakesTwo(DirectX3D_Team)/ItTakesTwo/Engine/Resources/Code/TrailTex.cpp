#include "TrailTex.h"

USING(Engine)

Engine::CTrailTex::CTrailTex(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
{

}

Engine::CTrailTex::CTrailTex(const CTrailTex& rhs)
	: CVIBuffer(rhs)
{

}

Engine::CTrailTex::~CTrailTex(void)
{

}

HRESULT Engine::CTrailTex::Ready_Buffer(void)
{
	m_dwTriCnt = 2;
	m_dwVtxCnt = 4;
	m_dwVtxSize = sizeof(VTXTEX);
	m_dwIdxSize = sizeof(INDEX16);
	m_IdxFmt = D3DFMT_INDEX16;
	m_dwFVF = FVF_TEX;

	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL);

	VTXTEX* pVertex = nullptr;
	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPosition = _vec3(-0.5f, 0.5f, 0.f);
	pVertex[0].vTexUV = _vec2(0.f, 0.f);

	pVertex[1].vPosition = _vec3(0.5f, 0.5f, 0.f);
	pVertex[1].vTexUV = _vec2(1.f, 0.f);

	pVertex[2].vPosition = _vec3(0.5f, -0.5f, 0.f);
	pVertex[2].vTexUV = _vec2(1.f, 1.f);

	pVertex[3].vPosition = _vec3(-0.5f, -0.5f, 0.f);
	pVertex[3].vTexUV = _vec2(0.f, 1.f);

	m_pVB->Unlock();


	INDEX16* pIndices = nullptr;
	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	pIndices[0]._0 = 0;
	pIndices[0]._1 = 1;
	pIndices[0]._2 = 2;

	pIndices[1]._0 = 0;
	pIndices[1]._1 = 2;
	pIndices[1]._2 = 3;

	m_pIB->Unlock();

	return S_OK;
}

void Engine::CTrailTex::Render_Buffer(void)
{
	CVIBuffer::Render_Buffer();
}

void CTrailTex::Set_Vertices(vector<_vec3> vecStart, vector<_vec3> vecEnd)
{
	if (vecStart.size() <= 1)
		return;

	VTXTEX* pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPosition = vecStart[0];
	pVertex[0].vTexUV = _vec2(0.f, 0.f);

	pVertex[1].vPosition = vecStart[1];
	pVertex[1].vTexUV = _vec2(1.f, 0.f);

	pVertex[2].vPosition = vecEnd[0];
	pVertex[2].vTexUV = _vec2(1.f, 1.f);

	pVertex[3].vPosition = vecEnd[1];
	pVertex[3].vTexUV = _vec2(0.f, 1.f);

	m_pVB->Unlock();
}

CTrailTex* Engine::CTrailTex::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTrailTex*		pInstance = new CTrailTex(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer()))
		Safe_Release(pInstance);

	return pInstance;
}

CComponent * CTrailTex::Clone(void* pArg)
{
	return new CTrailTex(*this);
}

void Engine::CTrailTex::Free(void)
{
	CVIBuffer::Free();
}

