#include "VIBuffer_CubeTexture.h"

USING(Engine)

CVIBuffer_CubeTexture::CVIBuffer_CubeTexture(_Device pDevice)
	: CVIBuffer(pDevice)
{
}

CVIBuffer_CubeTexture::CVIBuffer_CubeTexture(const CVIBuffer_CubeTexture& other)
	: CVIBuffer(other)
{
}

HRESULT CVIBuffer_CubeTexture::Ready_Component_Prototype()
{
	m_iVertexSize = sizeof(VTX_CUBETEXTURE);
	m_iVertexCount = 8;
	m_iTriCount = 12;
	m_iFVF = VTX_CUBETEXTURE_FVF;

	m_iIndexSize = sizeof(INDEX16);
	m_IndexFmt = D3DFMT_INDEX16;

	if (FAILED(CVIBuffer::Ready_Component_Prototype()))
		return E_FAIL;

	VTX_CUBETEXTURE* pVertices = nullptr;

	/* 버텍스 버퍼가 갖고 있는 버텍스 배열의 주소를 뱉어낸다. */
	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	/*
	반드시 Lock, Unlock 사이에서 버텍스를 조작해야한다.
	상당히 무거운 작업이므로 이러한 작업은 매 프레임마다 수행하는건 바람직하지 않다. 최소화 해야한다.
	*/
	pVertices[0].vPosition = _float3(-0.5f, 0.5f, -0.5f);
	pVertices[0].vUV = pVertices[0].vPosition;

	pVertices[1].vPosition = _float3(0.5f, 0.5f, -0.5f);
	pVertices[1].vUV = pVertices[1].vPosition;

	pVertices[2].vPosition = _float3(0.5f, -0.5f, -0.5f);
	pVertices[2].vUV = pVertices[2].vPosition;

	pVertices[3].vPosition = _float3(-0.5f, -0.5f, -0.5f);
	pVertices[3].vUV = pVertices[3].vPosition;

	pVertices[4].vPosition = _float3(-0.5f, 0.5f, 0.5f);
	pVertices[4].vUV = pVertices[4].vPosition;

	pVertices[5].vPosition = _float3(0.5f, 0.5f, 0.5f);
	pVertices[5].vUV = pVertices[5].vPosition;

	pVertices[6].vPosition = _float3(0.5f, -0.5f, 0.5f);
	pVertices[6].vUV = pVertices[6].vPosition;

	pVertices[7].vPosition = _float3(-0.5f, -0.5f, 0.5f);
	pVertices[7].vUV = pVertices[7].vPosition;

	m_pVertexArr = new VTX_CUBETEXTURE[m_iVertexCount];
	memcpy(m_pVertexArr, pVertices, m_iVertexSize * m_iVertexCount);

	m_pVB->Unlock();

	INDEX16* pIndexArr = nullptr;

	/* 인덱스버퍼가 보관하고 있던 인덱스 배열을 얻어옴 */
	m_pIB->Lock(0, 0, (void**)&pIndexArr, 0);

	// +x
	pIndexArr[0]._1 = 1;
	pIndexArr[0]._2 = 5;
	pIndexArr[0]._3 = 6;

	pIndexArr[1]._1 = 1;
	pIndexArr[1]._2 = 6;
	pIndexArr[1]._3 = 2;

	// -x
	pIndexArr[2]._1 = 4;
	pIndexArr[2]._2 = 0;
	pIndexArr[2]._3 = 3;

	pIndexArr[3]._1 = 4;
	pIndexArr[3]._2 = 3;
	pIndexArr[3]._3 = 7;

	// +y
	pIndexArr[4]._1 = 4;
	pIndexArr[4]._2 = 5;
	pIndexArr[4]._3 = 1;

	pIndexArr[5]._1 = 4;
	pIndexArr[5]._2 = 1;
	pIndexArr[5]._3 = 0;

	// -y
	pIndexArr[6]._1 = 6;
	pIndexArr[6]._2 = 7;
	pIndexArr[6]._3 = 3;

	pIndexArr[7]._1 = 6;
	pIndexArr[7]._2 = 3;
	pIndexArr[7]._3 = 2;

	// +z
	pIndexArr[8]._1 = 5;
	pIndexArr[8]._2 = 4;
	pIndexArr[8]._3 = 7;

	pIndexArr[9]._1 = 5;
	pIndexArr[9]._2 = 7;
	pIndexArr[9]._3 = 6;

	// -z
	pIndexArr[10]._1 = 0;
	pIndexArr[10]._2 = 1;
	pIndexArr[10]._3 = 2;

	pIndexArr[11]._1 = 0;
	pIndexArr[11]._2 = 2;
	pIndexArr[11]._3 = 3;

	m_pIndexArr = new INDEX16[m_iTriCount];
	memcpy(m_pIndexArr, pIndexArr, m_iIndexSize * m_iTriCount);

	m_pIB->Unlock();

	return S_OK;
}

HRESULT CVIBuffer_CubeTexture::Ready_Component(void* pArg)
{
	if (FAILED(CVIBuffer::Ready_Component(pArg)))
		return E_FAIL;

	return S_OK;
}

HRESULT CVIBuffer_CubeTexture::Render_VIBuffer()
{
	if (FAILED(CVIBuffer::Render_VIBuffer()))
		return E_FAIL;

	/* 버텍스버퍼와 인덱스버퍼를 가지고 도형을 그리는 함수 */
	return m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_iVertexCount, 0, m_iTriCount);
}

HRESULT CVIBuffer_CubeTexture::Change_VertexColor(D3DCOLOR _Color, _uint iIndex /*= 0*/)
{
	//텍스쳐 큐브에서는 호출하면 안됨.

	PRINT_LOG(L"Error", L"Try To Calling ChangeColor in CVIBuffer_CubeTexture");

	return E_FAIL;
}

CVIBuffer_CubeTexture* CVIBuffer_CubeTexture::Create(_Device pDevice)
{
	CVIBuffer_CubeTexture* pInstance = new CVIBuffer_CubeTexture(pDevice);
	if (FAILED(pInstance->Ready_Component_Prototype()))
	{
		PRINT_LOG(L"Error", L"Failed To Create CVIBuffer_CubeTexture");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CVIBuffer_CubeTexture::Clone(void* pArg)
{
	CVIBuffer_CubeTexture* pClone = new CVIBuffer_CubeTexture(*this);
	if (FAILED(pClone->Ready_Component(pArg)))
	{
		PRINT_LOG(L"Error", L"Failed To Clone CVIBuffer_CubeTexture");
		Safe_Release(pClone);
	}

	return pClone;
}

void CVIBuffer_CubeTexture::Free()
{
	CVIBuffer::Free();
}
