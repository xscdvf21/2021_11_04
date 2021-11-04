#include "RoundTex.h"

USING(Engine)

Engine::CRoundTex::CRoundTex(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
{
}

Engine::CRoundTex::CRoundTex(const CRoundTex& rhs)
	: CVIBuffer(rhs)
{
}

Engine::CRoundTex::~CRoundTex(void)
{
}

HRESULT Engine::CRoundTex::Ready_Buffer(void)
{
	//if (FAILED(CVIBuffer::Ready_Buffer()))
	//	return E_FAIL;

	LPD3DXMESH texMesh;
	D3DXCreateSphere(m_pGraphicDev, 1.f, 64, 64, &texMesh, nullptr);
	texMesh->CloneMeshFVF(D3DXMESH_SYSTEMMEM, D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1, m_pGraphicDev, &m_pBuffer);
	texMesh->Release();
	VTXTEX* pVerts;
	if (SUCCEEDED(m_pBuffer->LockVertexBuffer(0, (void **)&pVerts))) {

		// get vertex count
		int numVerts = m_pBuffer->GetNumVertices();

		// loop through the vertices
		for (int i = 0; i < numVerts; i++) {

			// calculate texture coordinates
			pVerts->vTexUV.x = asinf(pVerts->vNormal.x) / D3DX_PI + 0.5f;
			pVerts->vTexUV.y = asinf(pVerts->vNormal.y) / D3DX_PI + 0.5f;

			// go to next vertex
			pVerts++;
		}

		// unlock the vertex buffer
		m_pBuffer->UnlockVertexBuffer();
	}

	return S_OK;
}

void Engine::CRoundTex::Render_Buffer(void)
{
	CVIBuffer::Render_Buffer();

	if (m_pBuffer)
	{
		m_pBuffer->DrawSubset(0);
	}
}

void Engine::CRoundTex::Free(void)
{
	if (m_pBuffer)
		m_pBuffer->Release();
	CVIBuffer::Free();
}

CRoundTex* Engine::CRoundTex::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CRoundTex*	pInstance = new CRoundTex(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer()))
		Safe_Release(pInstance);

	return pInstance;
}

CComponent* Engine::CRoundTex::Clone(void* pArg)
{
	return new CRoundTex(*this);
}

