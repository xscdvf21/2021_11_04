#include "NormalTerrainTex.h"

USING(Engine)

Engine::CNormalTerrainTex::CNormalTerrainTex(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
{

}

Engine::CNormalTerrainTex::CNormalTerrainTex(const CNormalTerrainTex& rhs)
	: CVIBuffer(rhs)
	, m_fH(rhs.m_fH)
	, m_iH(rhs.m_iH)
{
}

Engine::CNormalTerrainTex::~CNormalTerrainTex(void)
{
}

void CNormalTerrainTex::Copy_Indices(INDEX32 * pIndex, const _ulong & dwTriCnt)
{
	Engine::INDEX32*		pOriginalIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pOriginalIndex, NULL);

	memcpy(pOriginalIndex, pIndex, sizeof(INDEX32) * dwTriCnt);

	m_pIB->Unlock();

	m_dwTriCnt = dwTriCnt;
}

HRESULT CNormalTerrainTex::Set_Terrain(void* pArg)
{
	Free();
	m_pGraphicDev->AddRef();
	Ready_Clone_Buffer(pArg);

	return S_OK;
}

HRESULT Engine::CNormalTerrainTex::Ready_Buffer(const _ulong& dwCntX,
	const _ulong& dwCntZ,
	const _ulong& dwVtxItv)
{
	m_dwTriCnt = (dwCntX - 1) * (dwCntZ - 1) * 2;
	m_dwVtxCnt = dwCntX * dwCntZ;
	m_dwVtxSize = sizeof(VTXTEX);
	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32;
	m_dwFVF = FVF_TEX;

	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL);

	VTXTEX* pVertices = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);


	_uint iIndex = 0;

	for (_uint z = 0; z < dwCntZ; ++z)
	{
		for (_uint x = 0; x < dwCntX; ++x)
		{
			iIndex = z * dwCntX + x;
			if (m_dwVtxCnt <= iIndex)
				continue;

			pVertices[iIndex].vPosition = _vec3(_float(x * dwVtxItv), 0.f, _float(z * dwVtxItv));
			pVertices[iIndex].vNormal = _vec3(0.f, 0.f, 0.f);
			pVertices[iIndex].vTexUV = _vec2(_float(x) / (dwCntX - 1) * 20.f, _float(z) / (dwCntZ - 1) * 20.f);
		}
	}

	INDEX32* pIndices = nullptr;

	/* 인덱스버퍼가 보관하고 있던 인덱스 배열을 얻어옴 */
	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	_ulong dwTriIndex = 0;

	for (_uint z = 0; z < dwCntZ - 1; ++z)
	{
		for (_uint x = 0; x < dwCntX - 1; ++x)
		{
			iIndex = z * dwCntX + x;

			pIndices[dwTriIndex]._0 = _ushort(iIndex + dwCntX);
			pIndices[dwTriIndex]._1 = _ushort(iIndex + dwCntX + 1);
			pIndices[dwTriIndex]._2 = iIndex + 1;

			_vec3 vNormal, vDest, vSour;

			vDest = pVertices[pIndices[dwTriIndex]._1].vPosition - pVertices[pIndices[dwTriIndex]._0].vPosition;
			vSour = pVertices[pIndices[dwTriIndex]._2].vPosition - pVertices[pIndices[dwTriIndex]._1].vPosition;
			D3DXVec3Cross(&vNormal, &vDest, &vSour);

			pVertices[pIndices[dwTriIndex]._0].vNormal += vNormal;
			pVertices[pIndices[dwTriIndex]._1].vNormal += vNormal;
			pVertices[pIndices[dwTriIndex]._2].vNormal += vNormal;

			dwTriIndex++;

			pIndices[dwTriIndex]._0 = _ushort(iIndex + dwCntX);
			pIndices[dwTriIndex]._1 = iIndex + 1;
			pIndices[dwTriIndex]._2 = iIndex;

			vDest = pVertices[pIndices[dwTriIndex]._1].vPosition - pVertices[pIndices[dwTriIndex]._0].vPosition;
			vSour = pVertices[pIndices[dwTriIndex]._2].vPosition - pVertices[pIndices[dwTriIndex]._1].vPosition;
			D3DXVec3Cross(&vNormal, &vDest, &vSour);

			pVertices[pIndices[dwTriIndex]._0].vNormal += vNormal;
			pVertices[pIndices[dwTriIndex]._1].vNormal += vNormal;
			pVertices[pIndices[dwTriIndex]._2].vNormal += vNormal;

			dwTriIndex++;
		}
	}

	for (_ulong i = 0; i < m_dwVtxCnt; ++i)
		D3DXVec3Normalize(&pVertices[i].vNormal, &pVertices[i].vNormal);

	m_pVB->Unlock();
	m_pIB->Unlock();

	return S_OK;
}

HRESULT CNormalTerrainTex::Ready_Clone_Buffer(void* pArg)
{
	VECVTX temp;
	if (pArg)
	{
		memcpy(&temp, pArg, sizeof(VECVTX));
	}

	m_dwTriCnt = _ulong((temp.XZScale.x - 1) * _ulong(temp.XZScale.z - 1) * 2);
	m_dwVtxCnt = _ulong(temp.XZScale.x * temp.XZScale.z);
	m_dwVtxSize = sizeof(VTXTEX);
	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32;
	m_dwFVF = FVF_TEX;

	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL);

	VTXTEX* pVertices = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	_uint iIndex = 0;

	if (temp.pVtx == nullptr)
	{
		for (_uint z = 0; z < temp.XZScale.z; ++z)
		{
			for (_uint x = 0; x < temp.XZScale.x; ++x)
			{
				iIndex = z * _ulong(temp.XZScale.x) + x;
				if (m_dwVtxCnt <= iIndex)
					continue;

				pVertices[iIndex].vPosition = _vec3(_float(x * temp.XZScale.y), 0.f, _float(z * temp.XZScale.y));
				pVertices[iIndex].vNormal = _vec3(0.f, 0.f, 0.f);
				pVertices[iIndex].vTexUV = _vec2(_float(x) / (temp.XZScale.x - 1) * 20.f, _float(z) / (temp.XZScale.z - 1) * 20.f);
			}
		}
	}
	else
	{
		for (_uint z = 0; z < temp.XZScale.z; ++z)
		{
			for (_uint x = 0; x < temp.XZScale.x; ++x)
			{
				iIndex = z * _ulong(temp.XZScale.x) + x;
				if (m_dwVtxCnt <= iIndex)
					continue;

				pVertices[iIndex].vPosition = temp.pVtx[iIndex].vPosition;
				pVertices[iIndex].vNormal = _vec3(0.f, 0.f, 0.f);
				pVertices[iIndex].vTexUV = _vec2(_float(x) / (temp.XZScale.x - 1) * 20.f, _float(z) / (temp.XZScale.z - 1) * 20.f);
			}
		}
	}

	INDEX32* pIndices = nullptr;

	/* 인덱스버퍼가 보관하고 있던 인덱스 배열을 얻어옴 */
	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	_ulong dwTriIndex = 0;

	for (_uint z = 0; z < temp.XZScale.z - 1; ++z)
	{
		for (_uint x = 0; x < temp.XZScale.x - 1; ++x)
		{
			iIndex = z * _ulong(temp.XZScale.x) + x;

			pIndices[dwTriIndex]._0 = _ushort(iIndex + temp.XZScale.x);
			pIndices[dwTriIndex]._1 = _ushort(iIndex + temp.XZScale.x + 1);
			pIndices[dwTriIndex]._2 = iIndex + 1;

			_vec3 vNormal, vDest, vSour;

			vDest = pVertices[pIndices[dwTriIndex]._1].vPosition - pVertices[pIndices[dwTriIndex]._0].vPosition;
			vSour = pVertices[pIndices[dwTriIndex]._2].vPosition - pVertices[pIndices[dwTriIndex]._1].vPosition;
			D3DXVec3Cross(&vNormal, &vDest, &vSour);

			pVertices[pIndices[dwTriIndex]._0].vNormal += vNormal;
			pVertices[pIndices[dwTriIndex]._1].vNormal += vNormal;
			pVertices[pIndices[dwTriIndex]._2].vNormal += vNormal;

			dwTriIndex++;

			pIndices[dwTriIndex]._0 = _ushort(iIndex + temp.XZScale.x);
			pIndices[dwTriIndex]._1 = iIndex + 1;
			pIndices[dwTriIndex]._2 = iIndex;

			vDest = pVertices[pIndices[dwTriIndex]._1].vPosition - pVertices[pIndices[dwTriIndex]._0].vPosition;
			vSour = pVertices[pIndices[dwTriIndex]._2].vPosition - pVertices[pIndices[dwTriIndex]._1].vPosition;
			D3DXVec3Cross(&vNormal, &vDest, &vSour);

			pVertices[pIndices[dwTriIndex]._0].vNormal += vNormal;
			pVertices[pIndices[dwTriIndex]._1].vNormal += vNormal;
			pVertices[pIndices[dwTriIndex]._2].vNormal += vNormal;

			dwTriIndex++;
		}
	}

	for (_ulong i = 0; i < m_dwVtxCnt; ++i)
		D3DXVec3Normalize(&pVertices[i].vNormal, &pVertices[i].vNormal);

	m_pVertices = new VTXTEX[m_dwVtxCnt];
	memcpy(m_pVertices, pVertices, m_dwVtxSize * m_dwVtxCnt);

	m_pIndices = new INDEX32[m_dwTriCnt];
	memcpy(m_pIndices, pIndices, m_dwIdxSize * m_dwTriCnt);

	m_pVB->Unlock();
	m_pIB->Unlock();

	return S_OK;
}

void Engine::CNormalTerrainTex::Render_Buffer(void)
{
	CVIBuffer::Render_Buffer();
}

void Engine::CNormalTerrainTex::Free(void)
{
	CVIBuffer::Free();
}

CComponent* Engine::CNormalTerrainTex::Clone(void* pArg)
{
	CNormalTerrainTex*	pInstance = new CNormalTerrainTex(*this);

	if (FAILED(pInstance->Set_Terrain(pArg)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CNormalTerrainTex* Engine::CNormalTerrainTex::Create(LPDIRECT3DDEVICE9 pGraphicDev,
	const _ulong& dwCntX,
	const _ulong& dwCntZ,
	const _ulong& dwVtxItv)
{
	CNormalTerrainTex*	pInstance = new CNormalTerrainTex(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer(dwCntX, dwCntZ, dwVtxItv)))
		Safe_Release(pInstance);

	return pInstance;
}

