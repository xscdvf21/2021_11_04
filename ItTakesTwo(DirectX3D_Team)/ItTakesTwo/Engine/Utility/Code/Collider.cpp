#include "Collider.h"
USING(Engine)

Engine::CCollider::CCollider(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
{
	m_pGraphicDev->AddRef();
}

Engine::CCollider::~CCollider(void)
{

}

HRESULT Engine::CCollider::Ready_Collider(_float fScale, _vec4 argb)
{
	m_fDis = fScale;

	LPD3DXMESH texMesh;
	FAILED_CHECK_RETURN(D3DXCreateSphere(m_pGraphicDev, fScale * 100, 32, 32, &texMesh, nullptr), E_FAIL);
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


	D3DXCreateTexture(m_pGraphicDev,
		5,
		5,
		1, // miplevel
		0,	// 텍스처의 용도
		D3DFMT_A8R8G8B8,
		D3DPOOL_MANAGED,
		&m_pTexture);

	D3DLOCKED_RECT LockRect;
	m_pTexture->LockRect(0, &LockRect, NULL, 0);

	for (_int i = 0; i < 5; ++i)
	{
		for (_int j = 0; j < 5; ++j)
		{
			_int	iIndex = i * 5 + j;

			*(((_ulong*)LockRect.pBits) + iIndex) = D3DCOLOR_ARGB((_int)argb.x, (_int)argb.y, (_int)argb.z, (_int)argb.w);
		}
	}
	
	return S_OK;
}

void Engine::CCollider::Render_Collider(_matrix pParentMatrix, _bool debug)
{
	m_matColMatrix = pParentMatrix;
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 1);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matColMatrix);

	m_pGraphicDev->SetTexture(0, m_pTexture);

	if (m_pBuffer && debug)
	{
		m_pBuffer->DrawSubset(0);
	}

	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

void Engine::CCollider::Render_Collider(_matrix pParentMatrix, _bool debug, LPD3DXEFFECT & pEffect)
{
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	pEffect->SetMatrix("g_matWorld", &pParentMatrix);

	_uint	iPassMax = 0;
	pEffect->Begin(&iPassMax, 0);
	pEffect->BeginPass(6);
	if (m_pBuffer && debug)
	{
		m_pBuffer->DrawSubset(0);
	}

	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);
}

CCollider* Engine::CCollider::Create(LPDIRECT3DDEVICE9 pGraphicDev, _float fScale, _vec4 argb)
{
	CCollider*	pInstance = new CCollider(pGraphicDev);

	if (FAILED(pInstance->Ready_Collider(fScale, argb)))
		Safe_Release(pInstance);

	return pInstance;
}

void Engine::CCollider::Free(void)
{
	Safe_Release(m_pGraphicDev);
	Safe_Release(m_pTexture);

	if (m_pBuffer)
		m_pBuffer->Release();
}

CComponent* Engine::CCollider::Clone(void* pArg)
{
	return new CCollider(*this);
}

