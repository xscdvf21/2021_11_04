#ifndef VIBuffer_h__
#define VIBuffer_h__

#include "Resources.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer : public CResources
{
protected:
	explicit CVIBuffer(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CVIBuffer(const CVIBuffer& rhs);
	virtual ~CVIBuffer(void);

public:
	_ulong		Get_TriCnt() { return m_dwTriCnt; }
	_ulong		Get_VxCnt() { return m_dwVtxCnt; }
	_ulong		Get_VxSize() { return m_dwVtxSize; }
	void*		Get_Vertices() { return m_pVertices; }
	void*		Get_Indices() { return m_pIndices; }

public:
	virtual HRESULT		Ready_Buffer(void);
	virtual void		Render_Buffer(void);

protected:
	LPDIRECT3DVERTEXBUFFER9			m_pVB;
	LPDIRECT3DINDEXBUFFER9			m_pIB;

	_ulong							m_dwVtxSize;
	_ulong							m_dwVtxCnt;
	_ulong							m_dwFVF;
	_ulong							m_dwIdxSize;
	_ulong							m_dwTriCnt;
	D3DFORMAT						m_IdxFmt;

	void*	m_pVertices;
	void*	m_pIndices;

public:
	/* For.Mfc Mouse Picking */
public:
	template < typename T >
	_bool IsMousePicking(
		_vec3* pOutPos,
		_float* pDis,
		HWND hWnd,
		_uint iWinCX,
		_uint iWinCY,
		class CCamera* pCamera,
		_matrix* pWorldMatrix,
		_float fHeight,
		_float fRange,
		_int Round1Edge2Flat3)
	{
		if (FAILED(Translation_ToLocalSpace(
			hWnd,
			iWinCX,
			iWinCY,
			pCamera,
			pWorldMatrix)))
			return false;

		T* pVertices = (T*)m_pVertices;
		if (nullptr == pVertices)
			return false;

		T* pTempVertices = nullptr;

		INDEX32* pIndices = (INDEX32*)m_pIndices;
		if (nullptr == pIndices)
			return false;

		_vec3 v1, v2, v3;
		_float U, V, dist;

		for (_uint i = 0; i < m_dwTriCnt; ++i)
		{
			v1 = pVertices[pIndices[i]._0].vPosition;
			v2 = pVertices[pIndices[i]._1].vPosition;
			v3 = pVertices[pIndices[i]._2].vPosition;

			if (D3DXIntersectTri(
				&v1,
				&v2,
				&v3,
				&m_vRayPos,
				&m_vRayDir,
				&U,
				&V,
				&dist))
			{
				*pOutPos = v1 + U * (v2 - v1) + V * (v3 - v1);
				m_pVB->Lock(0, 0, (void**)&pTempVertices, 0);
				_vec3 vV1 = pVertices[pIndices[i]._0].vPosition;

				for (_uint i = 0; i < m_dwTriCnt; ++i)
				{
					_vec3 vV2 = pVertices[pIndices[i]._0].vPosition;

					if (Round1Edge2Flat3 == 1)
					{
						_float fDis0, fDis1, fDis2;
						fDis0 = fabs(D3DXVec3Length(&(*pOutPos - pVertices[pIndices[i]._0].vPosition)));
						fDis1 = fabs(D3DXVec3Length(&(*pOutPos - pVertices[pIndices[i]._1].vPosition)));
						fDis2 = fabs(D3DXVec3Length(&(*pOutPos - pVertices[pIndices[i]._2].vPosition)));
						if (D3DXVec3Length(&(vV1 - vV2)) < fRange)
						{
							pTempVertices[pIndices[i]._0].vPosition.y += 0.01f * fHeight * 100.f * (-1.f / fRange * fDis0 + 1.f);
							pTempVertices[pIndices[i]._1].vPosition.y += 0.01f * fHeight * 100.f * (-1.f / fRange * fDis1 + 1.f);
							pTempVertices[pIndices[i]._2].vPosition.y += 0.01f * fHeight * 100.f * (-1.f / fRange * fDis2 + 1.f);
						}
					}
					else if (Round1Edge2Flat3 == 2)
					{
						if (D3DXVec3Length(&(vV1 - vV2)) < fRange)
						{
							pTempVertices[pIndices[i]._0].vPosition.y += fHeight;
							pTempVertices[pIndices[i]._1].vPosition.y += fHeight;
							pTempVertices[pIndices[i]._2].vPosition.y += fHeight;
						}
					}
					else if (Round1Edge2Flat3 == 3)
					{
						if (D3DXVec3Length(&(vV1 - vV2)) < fRange)
						{
							T* Temp = (T*)m_pVertices;
							if (m_fMinY > Temp[pIndices[i]._0].vPosition.y)
								m_fMinY = Temp[pIndices[i]._0].vPosition.y;

							if (m_fMinY > Temp[pIndices[i]._1].vPosition.y)
								m_fMinY = Temp[pIndices[i]._1].vPosition.y;
										  
							if (m_fMinY > Temp[pIndices[i]._2].vPosition.y)
								m_fMinY = Temp[pIndices[i]._2].vPosition.y;

							pTempVertices[pIndices[i]._0].vPosition.y = m_fMinY;
							pTempVertices[pIndices[i]._1].vPosition.y = m_fMinY;
							pTempVertices[pIndices[i]._2].vPosition.y = m_fMinY;

							m_fMinY = 100.f;
						}
					}
				}
				m_pVB->Unlock();
				memcpy(m_pVertices, pTempVertices, m_dwIdxSize * m_dwVtxCnt);

				D3DXVec3TransformCoord(pOutPos, pOutPos, pWorldMatrix);
				return true;
			}
		}
		return false;
	}

private:
	HRESULT Translation_ToLocalSpace(
		HWND hWnd,
		_uint iWinCX,
		_uint iWinCY,
		class CCamera* pCamera,
		_matrix* pWorldMatrix);

private:
	_vec3	m_vRayPos;
	_vec3	m_vRayDir;
	_bool	m_bCheck = true;
	_float	m_fMinY = 100.f;

public:
	virtual void Free(void);
};

END
#endif // VIBuffer_h__
