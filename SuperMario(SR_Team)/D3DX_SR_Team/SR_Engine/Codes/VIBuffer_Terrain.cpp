#include "..\Headers\VIBuffer_Terrain.h"

USING(Engine)

//얘는 사실 임시임. 나중에 어떨게 쓸지는 고민해볼것.

CVIBuffer_Terrain::CVIBuffer_Terrain(_Device pDevice)
	: CVIBuffer(pDevice)
{
}

Engine::CVIBuffer_Terrain::CVIBuffer_Terrain(_Device pDevice, _uint iVertexCountX, _uint iVertexCountZ, _float fVertexItv)
	: CVIBuffer(pDevice)
	, m_iVertexCountX(iVertexCountX)
	, m_iVertexCountZ(iVertexCountZ)
	, m_fVertexInterval(fVertexItv)
{
}

CVIBuffer_Terrain::CVIBuffer_Terrain(const CVIBuffer_Terrain& other)
	: CVIBuffer(other)
	, m_iVertexCountX(other.m_iVertexCountX)
	, m_iVertexCountZ(other.m_iVertexCountZ)
	, m_fVertexInterval(other.m_fVertexInterval)
{
}

HRESULT CVIBuffer_Terrain::Ready_Component_Prototype()
{
	m_iVertexSize = sizeof(VTX_TEXTURE);
	m_iVertexCount = m_iVertexCountX * m_iVertexCountZ;
	m_iTriCount = (m_iVertexCountX - 1) * (m_iVertexCountZ - 1) * 2;
	m_iFVF = VTX_TEXTURE_FVF;

	m_iIndexSize = sizeof(INDEX16);
	m_IndexFmt = D3DFMT_INDEX16;

	if (FAILED(CVIBuffer::Ready_Component_Prototype()))
		return E_FAIL;

	VTX_TEXTURE* pVertices = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	_uint iIndex = 0;

	for (_uint z = 0; z < m_iVertexCountZ; ++z)
	{
		for (_uint x = 0; x < m_iVertexCountX; ++x)
		{
			iIndex = z * m_iVertexCountX + x;
			if (m_iVertexCount <= iIndex)
				continue;

			pVertices[iIndex].vPosition = _float3(x * m_fVertexInterval, 0.f, z * m_fVertexInterval);
			pVertices[iIndex].vUV = _float2((float)x / (float)(m_iVertexCountX - 1), 1.f - (float)(z / (float)(m_iVertexCountZ - 1)));
		}
	}

	m_pVertexArr = new VTX_TEXTURE[m_iVertexCount];
	memcpy(m_pVertexArr, pVertices, m_iVertexSize * m_iVertexCount);

	m_pVB->Unlock();

	INDEX16* pIndexArr = nullptr;

	/* 인덱스버퍼가 보관하고 있던 인덱스 배열을 얻어옴 */
	m_pIB->Lock(0, 0, (void**)&pIndexArr, 0);

	_uint iTriIndex = 0;

	for (_uint z = 0; z < m_iVertexCountZ - 1; ++z)
	{
		for (_uint x = 0; x < m_iVertexCountX - 1; ++x)
		{
			iIndex = z * m_iVertexCountX + x;

			pIndexArr[iTriIndex]._1 = iIndex + m_iVertexCountX;
			pIndexArr[iTriIndex]._2 = iIndex + m_iVertexCountX + 1;
			pIndexArr[iTriIndex++]._3 = iIndex + 1;

			pIndexArr[iTriIndex]._1 = iIndex + m_iVertexCountX;
			pIndexArr[iTriIndex]._2 = iIndex + 1;
			pIndexArr[iTriIndex++]._3 = iIndex;
		}
	}

	m_pIndexArr = new INDEX16[m_iTriCount];
	memcpy(m_pIndexArr, pIndexArr, m_iIndexSize * m_iTriCount);

	m_pIB->Unlock();

	return S_OK;
}

HRESULT CVIBuffer_Terrain::Ready_Component(void* pArg)
{
	if (FAILED(CVIBuffer::Ready_Component(pArg)))
		return E_FAIL;

	return S_OK;
}

HRESULT CVIBuffer_Terrain::Render_VIBuffer()
{
	if (FAILED(CVIBuffer::Render_VIBuffer()))
		return E_FAIL;

	/* 버텍스버퍼와 인덱스버퍼를 가지고 도형을 그리는 함수 */
	return m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_iVertexCount, 0, m_iTriCount);
}

//하면 안됨
HRESULT CVIBuffer_Terrain::Change_VertexColor(D3DCOLOR _Color, _uint iIndex)
{
	PRINT_LOG(L"Error", L"Try To Change Color in CVIBuffer_Terrain");
	return E_FAIL;
}

CVIBuffer_Terrain* CVIBuffer_Terrain::Create(_Device pDevice, _uint iVertexCountX, _uint iVertexCountZ, _float fVertexItv)
{
	CVIBuffer_Terrain* pInstance = new CVIBuffer_Terrain(pDevice, iVertexCountX, iVertexCountZ, fVertexItv);
	if (FAILED(pInstance->Ready_Component_Prototype()))
	{
		PRINT_LOG(L"Error", L"Failed To Create CVIBuffer_Terrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CVIBuffer_Terrain::Clone(void* pArg)
{
	CVIBuffer_Terrain* pClone = new CVIBuffer_Terrain(*this);
	if (FAILED(pClone->Ready_Component(pArg)))
	{
		PRINT_LOG(L"Error", L"Failed To Clone CVIBuffer_TerrainTexture");
		Safe_Release(pClone);
	}

	return pClone;
}

void CVIBuffer_Terrain::Free()
{
	CVIBuffer::Free();
}
