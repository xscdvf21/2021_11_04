#include "VIBuffer_CubeColor.h"

USING(Engine)

CVIBuffer_CubeColor::CVIBuffer_CubeColor(LPDIRECT3DDEVICE9 pDevice)
	: CVIBuffer(pDevice)
{
}

CVIBuffer_CubeColor::CVIBuffer_CubeColor(const CVIBuffer_CubeColor& other)
	: CVIBuffer(other)
{
}

HRESULT CVIBuffer_CubeColor::Ready_Component_Prototype()
{
	m_iVertexSize = sizeof(VTX_CUBECOLOR);
	m_iVertexCount = 24;		//4x6
	m_iTriCount = 48;			//count x 2
	m_iFVF = VTX_CUBECOLOR_FVF;

	m_iIndexSize = sizeof(INDEX16);
	m_IndexFmt = D3DFMT_INDEX16;

	if (FAILED(CVIBuffer::Ready_Component_Prototype()))
		return E_FAIL;

	VTX_CUBECOLOR* pVertexArr = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertexArr, 0);

	//-X 평면
	pVertexArr[0].vPosition = _float3(-0.5, -0.5, -0.5);
	pVertexArr[1].vPosition = _float3(-0.5, -0.5, 0.5);
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
	//테스트용 배색
	for (int i = 0; i < 6; ++i)
	{
		pVertexArr[i * 4].iColor = D3DCOLOR_ARGB(255, i * 40, i * 40, i * 40);
		pVertexArr[i * 4 + 1].iColor = D3DCOLOR_ARGB(255, i * 40, i * 40, i * 40);
		pVertexArr[i * 4 + 2].iColor = D3DCOLOR_ARGB(255, i * 40, i * 40, i * 40);
		pVertexArr[i * 4 + 3].iColor = D3DCOLOR_ARGB(255, i * 40, i * 40, i * 40);
	}

	m_pVertexArr = new VTX_CUBECOLOR[m_iVertexCount];
	memcpy(m_pVertexArr, pVertexArr, m_iVertexSize * m_iVertexCount);

	m_pVB->Unlock();

	INDEX16* pIndexArr = nullptr;

	/* 인덱스버퍼가 보관하고 있던 인덱스 배열을 얻어옴 */
	m_pIB->Lock(0, 0, (void**)&pIndexArr, 0);

	for (int i = 0; i < 6; ++i) {
		pIndexArr[i * 2]._1 = i * 4 + 0;
		pIndexArr[i * 2]._2 = i * 4 + 1;
		pIndexArr[i * 2]._3 = i * 4 + 2;

		pIndexArr[i * 2 + 1]._1 = i * 4 + 2;
		pIndexArr[i * 2 + 1]._2 = i * 4 + 3;
		pIndexArr[i * 2 + 1]._3 = i * 4 + 0;
	}

	m_pIndexArr = new INDEX16[m_iTriCount];
	memcpy(m_pIndexArr, pIndexArr, m_iIndexSize * m_iTriCount);

	m_pIB->Unlock();

	return S_OK;
}

HRESULT CVIBuffer_CubeColor::Ready_Component(void* pArg)
{
	if (FAILED(CVIBuffer::Ready_Component(pArg)))
		return E_FAIL;

	return S_OK;
}

HRESULT CVIBuffer_CubeColor::Render_VIBuffer()
{
	if (FAILED(CVIBuffer::Render_VIBuffer()))
		return E_FAIL;

	/* 버텍스버퍼와 인덱스버퍼를 가지고 도형을 그리는 함수 */
	return m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_iVertexCount, 0, m_iTriCount);
}

HRESULT CVIBuffer_CubeColor::Change_VertexColor(D3DCOLOR _Color, _uint iIndex)
{
	/**
	*	1,6 : X
	*	2,5 : Y
	*	3,4 : Z
	*/
	//이거 특정 면 선택하면 그 면이 빛나게 (혹은 무슨 면인지 알 수 있게) 해줘야 할거 같은데

	//인덱스 초과하면 미리 거르기
	if (iIndex > 7)
	{
		PRINT_LOG(L"Error", L"Not vailed index for change Cube Color : " + iIndex);
		return E_FAIL;
	}

	//아니면 버텍스 세팅할거임
	VTX_CUBECOLOR* pVertexArr = nullptr;
	m_pVB->Lock(0, 0, (void**)&pVertexArr, 0);

	// 단일 컬러링
	if (iIndex == 0)
	{
		for (int i = 0; i < 24; ++i)
		{
			pVertexArr[i].iColor = _Color;
		}
	}
	else //각 면별로 다른 컬러링
	{
		m_pVB->Lock(0, 0, (void**)&pVertexArr, 0);

		switch (iIndex)
		{
		case 1:			//-X
			pVertexArr[0].iColor = _Color;
			pVertexArr[1].iColor = _Color;
			pVertexArr[2].iColor = _Color;
			pVertexArr[3].iColor = _Color;
			break;
		case 6:			//+X
			pVertexArr[4].iColor = _Color;
			pVertexArr[5].iColor = _Color;
			pVertexArr[6].iColor = _Color;
			pVertexArr[7].iColor = _Color;
			break;
		case 2:			//-Y
			pVertexArr[8].iColor = _Color;
			pVertexArr[9].iColor = _Color;
			pVertexArr[10].iColor = _Color;
			pVertexArr[11].iColor = _Color;
			break;
		case 5:			//+Y
			pVertexArr[12].iColor = _Color;
			pVertexArr[13].iColor = _Color;
			pVertexArr[14].iColor = _Color;
			pVertexArr[15].iColor = _Color;
			break;
		case 3:			//-Z
			pVertexArr[16].iColor = _Color;
			pVertexArr[17].iColor = _Color;
			pVertexArr[18].iColor = _Color;
			pVertexArr[19].iColor = _Color;
			break;
		case 4:			//+Z
			pVertexArr[20].iColor = _Color;
			pVertexArr[21].iColor = _Color;
			pVertexArr[22].iColor = _Color;
			pVertexArr[23].iColor = _Color;
			break;

		default:
			PRINT_LOG(L"Error", L"Not vailed index for change Cube Color : " + iIndex);
			break;
		}
	}
	m_pVertexArr = new VTX_CUBECOLOR[m_iVertexCount];
	memcpy(m_pVertexArr, pVertexArr, m_iVertexSize * m_iVertexCount);		//세팅 바꿨으니까 다시 넣기

	m_pVB->Unlock();

	return S_OK;			//일단 안씀
}

CVIBuffer_CubeColor* CVIBuffer_CubeColor::Create(_Device pDevice)
{
	CVIBuffer_CubeColor* pInstance = new CVIBuffer_CubeColor(pDevice);
	if (FAILED(pInstance->Ready_Component_Prototype()))
	{
		PRINT_LOG(L"Error", L"Failed To Create CVIBuffer_CubeColor");
		Safe_Release(pInstance);
	}

	return pInstance;
}


CComponent* CVIBuffer_CubeColor::Clone(void* pArg)
{
	CVIBuffer_CubeColor* pClone = new CVIBuffer_CubeColor(*this);
	if (FAILED(pClone->Ready_Component(pArg)))
	{
		PRINT_LOG(L"Error", L"Failed To Clone CVIBuffer_CubeTexture");
		Safe_Release(pClone);
	}

	return pClone;
}

void CVIBuffer_CubeColor::Free()
{
	CVIBuffer::Free();
}
