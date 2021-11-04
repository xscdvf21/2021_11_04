#include "VIBuffer_Alpha_Round.h"

USING(Engine)

CVIBuffer_Alpha_Round::CVIBuffer_Alpha_Round(_Device pDevice)
    : CVIBuffer(pDevice)
{
}

CVIBuffer_Alpha_Round::CVIBuffer_Alpha_Round(const CVIBuffer_Alpha_Round& other)
    : CVIBuffer(other)
{
}

HRESULT CVIBuffer_Alpha_Round::Ready_Component_Prototype()
{
    m_iVertexSize = sizeof(VTX_COLOR);
    m_iVertexCount = 36;            //5 + 3*11
    m_iTriCount = 36;               //3 * 12
    m_iFVF = VTX_COLOR_FVF| D3DFVF_SPECULAR;

    m_iIndexSize = sizeof(INDEX16);
    m_IndexFmt = D3DFMT_INDEX16;

    if (FAILED(CVIBuffer::Ready_Component_Prototype()))
        return E_FAIL;

    VTX_COLOR* pVertexArr = nullptr;

    //중심 : 500,500
    m_pVB->Lock(0, 0,(void**)&pVertexArr, 0);
    pVertexArr[0].vPosition = _float3(130.f, 0.2f, 50.f);
    pVertexArr[1].vPosition = _float3(210.f, 0.2f, 70.f);

    pVertexArr[2].vPosition = _float3(225.f, 0.2f, 0.f);

    pVertexArr[3].vPosition = _float3(130.f, 0.2f, -50.f);
    pVertexArr[4].vPosition = _float3(210.f, 0.2f, -70.f);

    pVertexArr[5].vPosition = _float3(195.f, 0.2f,-112.f);

    pVertexArr[6].vPosition = _float3(110.f, 0.2f,-100.f);
    pVertexArr[7].vPosition = _float3(165.f, 0.2f,-150.f);

    pVertexArr[8].vPosition = _float3(125.f, 0.2f,-190.f);

    pVertexArr[9].vPosition = _float3(45.f, 0.2f,-145.f);
    pVertexArr[10].vPosition = _float3(70.f, 0.2f,-215.f);

    pVertexArr[11].vPosition = _float3(0.f, 0.2f,-225.f);

    pVertexArr[12].vPosition = _float3(-45.f, 0.2f,-145.f);
    pVertexArr[13].vPosition = _float3(-70.f, 0.2f,-215.f);

    pVertexArr[14].vPosition = _float3(-125.f, 0.2f,-190.f);

    pVertexArr[15].vPosition = _float3(-110.f, 0.2f,-100.f);
    pVertexArr[16].vPosition = _float3(-165.f, 0.2f,-150.f);

    pVertexArr[17].vPosition = _float3(-195.f, 0.2f,-112.f);

    pVertexArr[18].vPosition = _float3(-130.f, 0.2f,-50.f);
    pVertexArr[19].vPosition = _float3(-210.f, 0.2f,-70.f);

    pVertexArr[20].vPosition = _float3(-225.f, 0.2f, 0.f);

    pVertexArr[21].vPosition = _float3(-130.f, 0.2f, 50.f);
    pVertexArr[22].vPosition = _float3(-210.f, 0.2f, 70.f);

    pVertexArr[23].vPosition = _float3(-195.f, 0.2f, 112.f);

    pVertexArr[24].vPosition = _float3(-110.f, 0.2f, 100.f);
    pVertexArr[25].vPosition = _float3(-165.f, 0.2f, 150.f);

    pVertexArr[26].vPosition = _float3(-195.f, 0.2f, 112.f);

    pVertexArr[27].vPosition = _float3(-45.f, 0.2f, 145.f);
    pVertexArr[28].vPosition = _float3(-70.f, 0.2f, 215.f);

    pVertexArr[29].vPosition = _float3(0.f, 0.2f, 225.f);

    pVertexArr[30].vPosition = _float3(45.f, 0.2f, 145.f);
    pVertexArr[31].vPosition = _float3(70.f, 0.2f, 215.f);

    pVertexArr[32].vPosition = _float3(195.f, 0.2f, 112.f);

    pVertexArr[33].vPosition = _float3(110.f, 0.2f, 100.f);
    pVertexArr[34].vPosition = _float3(165.f, 0.2f, 150.f);

    pVertexArr[35].vPosition = _float3(195.f, 0.2f, 112.f);

    for (int i = 0; i < 36; i++) {
        pVertexArr[i].iColor = D3DCOLOR_ARGB(205, 50, 50, 255);
    }
    m_pVertexArr = new VTX_COLOR[m_iVertexCount];

    memcpy(m_pVertexArr, pVertexArr, m_iVertexSize * m_iVertexCount);

    m_pVB->Unlock();


    //인덱스
    INDEX16* pIndexArr = nullptr;

    m_pIB->Lock(0, 0, (void**)&pIndexArr, 0);

    for (int i = 0; i < 11; i++)
    {
        pIndexArr[i*3]._1 = i * 3;
        pIndexArr[i*3]._2 = i * 3 + 1;
        pIndexArr[i*3]._3 = i * 3 + 2;

        pIndexArr[i*3+1]._1 = i * 3;
        pIndexArr[i*3+1]._2 = i * 3 + 2;
        pIndexArr[i*3+1]._3 = i * 3 + 3;

        pIndexArr[i*3+2]._1 = i * 3 + 3;
        pIndexArr[i*3+2]._2 = i * 3 + 2;
        pIndexArr[i*3+2]._3 = i * 3 + 4;
    }
    pIndexArr[33]._1 = 33;
    pIndexArr[33]._2 = 34;
    pIndexArr[33]._3 = 35;

    pIndexArr[34]._1 = 33;
    pIndexArr[34]._2 = 35;
    pIndexArr[34]._3 = 0;

    pIndexArr[35]._1 = 0;
    pIndexArr[35]._2 = 35;
    pIndexArr[35]._3 = 1;


    m_pIndexArr = new INDEX16[m_iTriCount];
    memcpy(m_pIndexArr, pIndexArr, m_iIndexSize * m_iTriCount);

    m_pIB->Unlock();

    return S_OK;
}

HRESULT CVIBuffer_Alpha_Round::Ready_Component(void* pArg)
{
    if (FAILED(CVIBuffer::Ready_Component(pArg)))
        return E_FAIL;

    return S_OK;
}

HRESULT CVIBuffer_Alpha_Round::Render_VIBuffer()
{
    if (FAILED(CVIBuffer::Render_VIBuffer()))
        return E_FAIL;

    /* 버텍스버퍼와 인덱스버퍼를 가지고 도형을 그리는 함수 */
    return m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_iVertexCount, 0, m_iTriCount);
}

HRESULT CVIBuffer_Alpha_Round::Change_VertexColor(D3DCOLOR _Color, _uint iIndex)
{
    VTX_COLOR* pVertexArr = nullptr;
    m_pVB->Lock(0, 0, (void**)&pVertexArr, 0);

    for (int i = 0; i < 36; i++) {
        pVertexArr[i].iColor = _Color;
    }

    m_pVertexArr = new VTX_COLOR[m_iVertexCount];

    memcpy(m_pVertexArr, pVertexArr, m_iVertexSize * m_iVertexCount);

    m_pVB->Unlock();

    return S_OK;
}

CVIBuffer_Alpha_Round* CVIBuffer_Alpha_Round::Create(_Device pDevice)
{
    CVIBuffer_Alpha_Round* pInstance = new CVIBuffer_Alpha_Round(pDevice);
    if (FAILED(pInstance->Ready_Component_Prototype()))
    {
        PRINT_LOG(L"Error", L"Failed To Create CVIBuffer_Alpha_Round");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CComponent* CVIBuffer_Alpha_Round::Clone(void* pArg)
{
    CVIBuffer_Alpha_Round* pClone = new CVIBuffer_Alpha_Round(*this);
    if (FAILED(pClone->Ready_Component(pArg)))
    {
        PRINT_LOG(L"Error", L"Failed To Clone CVIBuffer_Alpha_Round");
        Safe_Release(pClone);
    }

    return pClone;
}

void CVIBuffer_Alpha_Round::Free()
{
    CVIBuffer::Free();
}
