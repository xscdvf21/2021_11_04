#include "stdafx.h"
#include "..\Headers\VIBuffer_Plate.h"


CVIBuffer_Plate::CVIBuffer_Plate(LPDIRECT3DDEVICE9 pDevice)
	: CVIBufferTool(pDevice)
{
}

CVIBuffer_Plate::CVIBuffer_Plate(const CVIBuffer_Plate & other)
	: CVIBufferTool(other)
{
}

HRESULT CVIBuffer_Plate::Ready_Component_Prototype()
{
	float fVtixPointSize = 50.f;
	
	m_iVertexSize = sizeof(VTX_COLOR);
	m_iVertexCount = 4;
	m_iTriCount = 2;
	m_iFVF = VTX_COLOR_FVF;

	m_iIndexSize = sizeof(INDEX16);
	m_IndexFmt = D3DFMT_INDEX16;

	if (FAILED(CVIBufferTool::Ready_Component_Prototype()))
		return E_FAIL;

	VTX_COLOR* pVertices = nullptr;


	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	pVertices[0].vPosition = _float3(-fVtixPointSize, 0.f, fVtixPointSize);
	pVertices[0].iColor = D3DCOLOR_ARGB(255, 0, 255, 0);

	pVertices[1].vPosition = _float3(fVtixPointSize, 0.f, fVtixPointSize);
	pVertices[1].iColor = D3DCOLOR_ARGB(255, 0, 255, 0);

	pVertices[2].vPosition = _float3(fVtixPointSize, 0.f, -fVtixPointSize);
	pVertices[2].iColor = D3DCOLOR_ARGB(255, 0, 255, 0);

	pVertices[3].vPosition = _float3(-fVtixPointSize, 0.f, -fVtixPointSize);
	pVertices[3].iColor = D3DCOLOR_ARGB(255, 0, 255, 0);

	m_pVB->Unlock();

	INDEX16* pIndices = nullptr;


	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	pIndices[0]._1 = 0;
	pIndices[0]._2 = 1;
	pIndices[0]._3 = 2;

	pIndices[1]._1 = 0;
	pIndices[1]._2 = 2;
	pIndices[1]._3 = 3;

	m_pIB->Unlock();

	return S_OK;
}

HRESULT CVIBuffer_Plate::Ready_Component(void * pArg)
{
	if (FAILED(CVIBufferTool::Ready_Component(pArg)))
		return E_FAIL;

	return S_OK;
}

HRESULT CVIBuffer_Plate::Render_VIBuffer()
{
	if (FAILED(CVIBufferTool::Render_VIBuffer()))
		return E_FAIL;

	return m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_iVertexCount, 0, m_iTriCount);
}

CVIBuffer_Plate * CVIBuffer_Plate::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CVIBuffer_Plate* pInstance = new CVIBuffer_Plate(pDevice);
	if (FAILED(pInstance->Ready_Component_Prototype()))
	{
		PRINT_LOG(L"Error", L"Failed To Create CVIBuffer_RectColor");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponentTool * CVIBuffer_Plate::Clone(void * pArg)
{
	CVIBuffer_Plate* pClone = new CVIBuffer_Plate(*this);
	if (FAILED(pClone->Ready_Component(pArg)))
	{
		PRINT_LOG(L"Error", L"Failed To Clone CVIBuffer_RectColor");
		Safe_Release(pClone);
	}

	return pClone;
}

void CVIBuffer_Plate::Free()
{
	CVIBufferTool::Free();
}
