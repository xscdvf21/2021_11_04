
#include "stdafx.h"
#include "VIBuffer_CubeLineTool.h"


CVIBuffer_CubeLineTool::CVIBuffer_CubeLineTool(LPDIRECT3DDEVICE9 pDevice)
	: CVIBufferTool(pDevice)
{
}

CVIBuffer_CubeLineTool::CVIBuffer_CubeLineTool(const CVIBuffer_CubeLineTool& other)
	: CVIBufferTool(other)
{
}

HRESULT CVIBuffer_CubeLineTool::Ready_Component_Prototype()
{
	m_iVertexSize = sizeof(VTX_CUBECOLOR);
	m_iVertexCount = 24;		//4x6
	m_iTriCount = 48;			//count x 2
	m_iFVF = VTX_CUBECOLOR_FVF;

	m_iIndexSize = sizeof(INDEX16);
	m_IndexFmt = D3DFMT_INDEX16;

	if (FAILED(CVIBufferTool::Ready_Component_Prototype()))
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
		pVertexArr[i * 4].iColor = D3DCOLOR_ARGB(255,255,0,0);
		pVertexArr[i * 4 + 1].iColor = D3DCOLOR_ARGB(255, 255, 0, 0);
		pVertexArr[i * 4 + 2].iColor = D3DCOLOR_ARGB(255, 255, 0, 0);
		pVertexArr[i * 4 + 3].iColor = D3DCOLOR_ARGB(255, 255, 0, 0);
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

HRESULT CVIBuffer_CubeLineTool::Ready_Component(void* pArg)
{
	if (FAILED(CVIBufferTool::Ready_Component(pArg)))
		return E_FAIL;

	return S_OK;
}

HRESULT CVIBuffer_CubeLineTool::Render_VIBuffer()
{
	if (FAILED(CVIBufferTool::Render_VIBuffer()))
		return E_FAIL;

	/* 버텍스버퍼와 인덱스버퍼를 가지고 도형을 그리는 함수 */
	return m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_iVertexCount, 0, m_iTriCount);
}




CVIBuffer_CubeLineTool * CVIBuffer_CubeLineTool::Create(_Device pDevice)
{
	CVIBuffer_CubeLineTool* pInstance = new CVIBuffer_CubeLineTool(pDevice);
	if (FAILED(pInstance->Ready_Component_Prototype()))
	{
		PRINT_LOG(L"Error", L"Failed To Create CVIBuffer_CubeLineTool");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponentTool* CVIBuffer_CubeLineTool::Clone(void* pArg)
{
	CVIBuffer_CubeLineTool* pClone = new CVIBuffer_CubeLineTool(*this);

	if (FAILED(pClone->Ready_Component(pArg)))
	{
		PRINT_LOG(L"Error", L"Failed To Clone CVIBuffer_CubeLineTool");
		Safe_Release(pClone);
	}

	return pClone;
}

void CVIBuffer_CubeLineTool::Free()
{
	CVIBufferTool::Free();
}
