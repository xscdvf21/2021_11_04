#include "stdafx.h"
#include "..\Headers\VIBuffer_PlateTexture.h"



CVIBuffer_PlateTexture::CVIBuffer_PlateTexture(LPDIRECT3DDEVICE9 pDevice)
	: CVIBufferTool(pDevice)
{
}

CVIBuffer_PlateTexture::CVIBuffer_PlateTexture(const CVIBuffer_PlateTexture & other)
	: CVIBufferTool(other)
{
}

HRESULT CVIBuffer_PlateTexture::Ready_Component_Prototype()
{
	float fVtixPointSize =5.f;

	m_iVertexSize = sizeof(VTX_TEXTURE);
	m_iVertexCount = 4;
	m_iTriCount = 2;
	m_iFVF = VTX_TEXTURE_FVF;

	m_iIndexSize = sizeof(INDEX16);
	m_IndexFmt = D3DFMT_INDEX16;

	if (FAILED(CVIBufferTool::Ready_Component_Prototype()))
		return E_FAIL;

	VTX_TEXTURE* pVertices = nullptr;

	/* ���ؽ� ���۰� ���� �ִ� ���ؽ� �迭�� �ּҸ� ����. */
	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	/*
	�ݵ�� Lock, Unlock ���̿��� ���ؽ��� �����ؾ��Ѵ�.
	����� ���ſ� �۾��̹Ƿ� �̷��� �۾��� �� �����Ӹ��� �����ϴ°� �ٶ������� �ʴ�. �ּ�ȭ �ؾ��Ѵ�.
	*/
	pVertices[0].vPosition = _float3(-fVtixPointSize, 0.f, fVtixPointSize);
	pVertices[0].vUV = _float2(0.f, 0.f);

	pVertices[1].vPosition = _float3(fVtixPointSize, 0.f, fVtixPointSize);
	pVertices[1].vUV = _float2(1.f, 0.f);

	pVertices[2].vPosition = _float3(fVtixPointSize, 0.f, -fVtixPointSize);
	pVertices[2].vUV = _float2(1.f, 1.f);

	pVertices[3].vPosition = _float3(-fVtixPointSize, 0.f, -fVtixPointSize);
	pVertices[3].vUV = _float2(0.f, 1.f);


	for(int i = 0; i < 4; ++i)
	{ 
		pVertices[i].vUV *= 15.f;
	}


	m_pVB->Unlock();

	INDEX16* pIndices = nullptr;

	/* �ε������۰� �����ϰ� �ִ� �ε��� �迭�� ���� */
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

HRESULT CVIBuffer_PlateTexture::Ready_Component(void * pArg)
{
	if (FAILED(CVIBufferTool::Ready_Component(pArg)))
		return E_FAIL;

	return S_OK;
}

HRESULT CVIBuffer_PlateTexture::Render_VIBuffer()
{
	if (FAILED(CVIBufferTool::Render_VIBuffer()))
		return E_FAIL;

	return m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_iVertexCount, 0, m_iTriCount);
}


CVIBuffer_PlateTexture * CVIBuffer_PlateTexture::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CVIBuffer_PlateTexture* pInstance = new CVIBuffer_PlateTexture(pDevice);
	if (FAILED(pInstance->Ready_Component_Prototype()))
	{
		PRINT_LOG(L"Error", L"Failed To Create CVIBuffer_PlateTexture");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponentTool * CVIBuffer_PlateTexture::Clone(void * pArg)
{
	CVIBuffer_PlateTexture* pClone = new CVIBuffer_PlateTexture(*this);
	if (FAILED(pClone->Ready_Component(pArg)))
	{
		PRINT_LOG(L"Error", L"Failed To Clone CVIBuffer_PlateTexture");
		Safe_Release(pClone);
	}

	return pClone;
}

void CVIBuffer_PlateTexture::Free()
{
	CVIBufferTool::Free();
}
