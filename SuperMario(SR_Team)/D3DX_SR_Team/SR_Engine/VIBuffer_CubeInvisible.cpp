#include "VIBuffer_CubeInvisible.h"

USING(Engine)

CVIBuffer_CubeInvisible::CVIBuffer_CubeInvisible(_Device pDevice)
	: CVIBuffer(pDevice)
{
}

CVIBuffer_CubeInvisible::CVIBuffer_CubeInvisible(const CVIBuffer_CubeInvisible & other)
	: CVIBuffer(other)
{
}

HRESULT CVIBuffer_CubeInvisible::Ready_Component_Prototype()
{
	m_iVertexSize = sizeof(VTX_CUBEINV);
	m_iVertexCount = 8;
	m_iTriCount = 16; 
	m_iFVF = VTX_CUBEINVISIBLE_FVF;

	m_iIndexSize = sizeof(INDEX8);
	m_IndexFmt = D3DFMT_INDEX16;

	if (FAILED(CVIBuffer::Ready_Component_Prototype()))
		return E_FAIL;

	VTX_CUBEINV* pVertexArr = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertexArr, 0);

	//-X 평면
	pVertexArr[0].vPosition = _float3(-0.5, -0.5, -0.5);
	pVertexArr[1].vPosition = _float3(-0.5, -0.5, -0.5);
	pVertexArr[2].vPosition = _float3(-0.5, 0.5, 0.5);
	pVertexArr[3].vPosition = _float3(-0.5, 0.5, -0.5);

	//+X 평면
	pVertexArr[4].vPosition = _float3(0.5, -0.5, 0.5);
	pVertexArr[5].vPosition = _float3(0.5, -0.5, -0.5);
	pVertexArr[6].vPosition = _float3(0.5, 0.5, -0.5);
	pVertexArr[7].vPosition = _float3(0.5, 0.5, 0.5);

	//+Y 평면
	pVertexArr[8].vPosition = _float3(0.5, 0.5, -0.5);
	pVertexArr[9].vPosition = _float3(-0.5, 0.5, -0.5);
	pVertexArr[10].vPosition = _float3(-0.5, 0.5, 0.5);
	pVertexArr[11].vPosition = _float3(0.5, 0.5, 0.5);

	//-Y 평면
	pVertexArr[12].vPosition = _float3(-0.5, -0.5, -0.5);
	pVertexArr[13].vPosition = _float3(0.5, -0.5, -0.5);
	pVertexArr[14].vPosition = _float3(0.5, -0.5, 0.5);
	pVertexArr[15].vPosition = _float3(-0.5, -0.5, 0.5);

	//-Z 평면
	pVertexArr[16].vPosition = _float3(0.5, -0.5, -0.5);
	pVertexArr[17].vPosition = _float3(-0.5, -0.5, -0.5);
	pVertexArr[18].vPosition = _float3(-0.5, 0.5, -0.5);
	pVertexArr[19].vPosition = _float3(0.5, 0.5, -0.5);

	//+Z 평면
	pVertexArr[20].vPosition = _float3(-0.5, -0.5, 0.5);
	pVertexArr[21].vPosition = _float3(0.5, -0.5, 0.5);
	pVertexArr[22].vPosition = _float3(0.5, 0.5, 0.5);
	pVertexArr[23].vPosition = _float3(-0.5, 0.5, 0.5);

	m_pVertexArr = new VTX_CUBEINV[m_iVertexCount];
	memcpy(m_pVertexArr, pVertexArr, m_iVertexSize * m_iVertexCount);

	m_pVB->Unlock();

	INDEX8* pIndexArr = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndexArr, 0);

	for (int i = 0; i < 6; ++i) //이해후 재점검
	{
		pIndexArr[i * 2]._1 = i * 4 + 0;
		pIndexArr[i * 2]._2 = i * 4 + 1;
		pIndexArr[i * 2]._3 = i * 4 + 2;

		pIndexArr[i * 2 + 1]._1 = i * 4 + 0;
		pIndexArr[i * 2 + 1]._2 = i * 4 + 1;
		pIndexArr[i * 2 + 1]._3 = i * 4 + 2;
	}

	m_pIndexArr = new INDEX8[m_iTriCount];
	memcpy(m_pIndexArr, pIndexArr, m_iIndexSize * m_iTriCount);

	m_pIB->Unlock();

	return S_OK;
}

HRESULT CVIBuffer_CubeInvisible::Ready_Component(void * pArg)
{
	if (FAILED(CVIBuffer::Ready_Component(pArg)))
		return E_FAIL;

	return S_OK;
}

HRESULT CVIBuffer_CubeInvisible::Render_VIBuffer()
{
	if (FAILED(CVIBuffer::Render_VIBuffer()))
		return E_FAIL;

	/* 버텍스버퍼와 인덱스버퍼를 가지고 도형을 그리는 함수 */
	return m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_iVertexCount, 0, m_iTriCount);
}

HRESULT Engine::CVIBuffer_CubeInvisible::Change_VertexColor(D3DCOLOR _Color, _uint iIndex)
{
	PRINT_LOG(L"Error", L"Try To Calling ChangeColor in CVIBuffer_CubeTexture");

	return E_FAIL;
}

CVIBuffer_CubeInvisible * CVIBuffer_CubeInvisible::Create(_Device pDevice)
{
	CVIBuffer_CubeInvisible* pInstance = new CVIBuffer_CubeInvisible(pDevice);

	if (FAILED(pInstance->Ready_Component_Prototype()))
	{
		PRINT_LOG(L"Error", L"Failed To Create CVIBuffer_CubeInvisible");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CVIBuffer_CubeInvisible::Clone(void * pArg)
{
	CVIBuffer_CubeInvisible* pClone = new CVIBuffer_CubeInvisible(*this);
	if (FAILED(pClone->Ready_Component(pArg)))
	{
		PRINT_LOG(L"Error", L"Failed To Clone CVIBuffer_CubeTexture");
		Safe_Release(pClone);
	}

	return pClone;
}

void Engine::CVIBuffer_CubeInvisible::Free()
{
	CVIBuffer_CubeInvisible::Free();
}
