#include "VIBuffer_SlideCubeColor.h"

USING(Engine)

CVIBuffer_SlideCubeColor::CVIBuffer_SlideCubeColor(_Device pDevice)
    : CVIBuffer(pDevice)
{
}

CVIBuffer_SlideCubeColor::CVIBuffer_SlideCubeColor(const CVIBuffer_SlideCubeColor& other)
    : CVIBuffer(other)
{
}

HRESULT CVIBuffer_SlideCubeColor::Ready_Component_Prototype()
{
    m_iVertexSize = sizeof(VTX_CUBECOLOR);
    m_iVertexCount = 6;             //단일 블럭
    m_iTriCount = 8;
    m_iFVF = VTX_CUBECOLOR_FVF;

    m_iIndexSize = sizeof(INDEX16);
    m_IndexFmt = D3DFMT_INDEX16;

    if (FAILED(CVIBuffer::Ready_Component_Prototype()))
    {
        return E_FAIL;
    }

    VTX_CUBECOLOR* pVertexArr = nullptr;

    m_pVB->Lock(0, 0, (void**)&pVertexArr, 0);

    pVertexArr[0].vPosition = _float3(-0.5f,-0.5f,-0.5f);
    pVertexArr[1].vPosition = _float3(0.5f,-0.5f,-0.5f);
    pVertexArr[2].vPosition = _float3(0.5f,-0.5f,0.5f);
    pVertexArr[3].vPosition = _float3(-0.5f,-0.5f,0.5f);
    pVertexArr[4].vPosition = _float3(-0.5f,0.5f,-0.5f);
    pVertexArr[5].vPosition = _float3(-0.5f,0.5f,0.5f);

    for (int i = 0; i < 6; ++i)
    {
        pVertexArr[i].iColor = D3DCOLOR_ARGB(255, 100, 100, 100);
    }
    pVertexArr[0].iColor = D3DCOLOR_ARGB(255, 200, 200, 200);
    pVertexArr[3].iColor = D3DCOLOR_ARGB(255, 200, 200, 200);

    m_pVertexArr = new VTX_CUBECOLOR[m_iVertexCount];
    memcpy(m_pVertexArr, pVertexArr, m_iVertexSize * m_iVertexCount);

    m_pVB->Unlock();

    INDEX16* pIndexArr = nullptr;

    m_pIB->Lock(0, 0, (void**)&pIndexArr, 0);
    //밑면
    pIndexArr[0]._1 = 0;
    pIndexArr[0]._2 = 1;
    pIndexArr[0]._3 = 3;

    pIndexArr[1]._1 = 1;
    pIndexArr[1]._2 = 2;
    pIndexArr[1]._3 = 3;

    //정면 -Z
    pIndexArr[2]._1 = 0;
    pIndexArr[2]._2 = 4;
    pIndexArr[2]._3 = 1;

    //후면 +Z
    pIndexArr[3]._1 = 3;
    pIndexArr[3]._2 = 2;
    pIndexArr[3]._3 = 5;

    //슬라이딩 면
    pIndexArr[4]._1 = 1;
    pIndexArr[4]._2 = 4;
    pIndexArr[4]._3 = 2;

    pIndexArr[5]._1 = 2;
    pIndexArr[5]._2 = 4;
    pIndexArr[5]._3 = 5;

    pIndexArr[6]._1 = 3;
    pIndexArr[6]._2 = 5;
    pIndexArr[6]._3 = 4;

    pIndexArr[7]._1 = 0;
    pIndexArr[7]._2 = 3;
    pIndexArr[7]._3 = 4;


    m_pIndexArr = new INDEX16[m_iTriCount];
    memcpy(m_pIndexArr, pIndexArr, m_iIndexSize * m_iTriCount);

    m_pIB->Unlock();

    return S_OK;
}

HRESULT CVIBuffer_SlideCubeColor::Ready_Component(void* pArg)
{
    if (FAILED(CVIBuffer::Ready_Component(pArg)))
        return E_FAIL;

    return S_OK;
}

HRESULT CVIBuffer_SlideCubeColor::Render_VIBuffer()
{
    if (FAILED(CVIBuffer::Render_VIBuffer()))
        return E_FAIL;

    /* 버텍스버퍼와 인덱스버퍼를 가지고 도형을 그리는 함수 */
    return m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_iVertexCount, 0, m_iTriCount);
}

HRESULT CVIBuffer_SlideCubeColor::Change_VertexColor(D3DCOLOR _Color, _uint iIndex)
{
    VTX_CUBECOLOR* pVertexArr = nullptr;
    m_pVB->Lock(0, 0, (void**)&pVertexArr, 0);

    for (int i = 0; i < 8; ++i)
    {
        pVertexArr[i].iColor = _Color;
    }
    m_pVertexArr = new VTX_CUBECOLOR[m_iVertexCount];
    memcpy(m_pVertexArr, pVertexArr, m_iVertexSize * m_iVertexCount);		//세팅 바꿨으니까 다시 넣기

    m_pVB->Unlock();

    return S_OK;

}

CVIBuffer_SlideCubeColor* CVIBuffer_SlideCubeColor::Create(_Device pDevice)
{
    CVIBuffer_SlideCubeColor* pInstance = new CVIBuffer_SlideCubeColor(pDevice);
    if (FAILED(pInstance->Ready_Component_Prototype()))
    {
        PRINT_LOG(L"Error", L"Failed To Create CVIBuffer_CubeColor");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CComponent* CVIBuffer_SlideCubeColor::Clone(void* pArg)
{
    CVIBuffer_SlideCubeColor* pClone = new CVIBuffer_SlideCubeColor(*this);
    if (FAILED(pClone->Ready_Component(pArg)))
    {
        PRINT_LOG(L"Error", L"Failed To Clone CVIBuffer_CubeTexture");
        Safe_Release(pClone);
    }

    return pClone;
}

void CVIBuffer_SlideCubeColor::Free()
{
    CVIBuffer::Free();
}
