#include "..\Headers\VIBuffer_Rect.h"


USING(Engine)

//타입 인자 안 넣어줬음 넣어줘야됨.

CVIBuffer_Rect::CVIBuffer_Rect(_Device pDevice)
    : CVIBuffer(pDevice)
{
}

CVIBuffer_Rect::CVIBuffer_Rect(const CVIBuffer_Rect& other)
    : CVIBuffer(other)
{
}

HRESULT CVIBuffer_Rect::Ready_Component_Prototype()
{
    m_iVertexSize = sizeof(VTX_MIX);
    m_iVertexCount = 4;
    m_iTriCount = 2;
    m_iFVF = VTX_MIX_FVF;

    m_iIndexSize = sizeof(INDEX16);
    m_IndexFmt = D3DFMT_INDEX16;

    if (FAILED(CVIBuffer::Ready_Component_Prototype()))
    {
        return E_FAIL;
    }

    VTX_MIX* pVertexArr = nullptr;

    m_pVB->Lock(0, 0, (void**)&pVertexArr, 0);
    //기본 컬러링은 흰색
    pVertexArr[0].vPosition = _float3(-0.5f, 0.5f, 0.f);
    pVertexArr[0].iColor = D3DCOLOR_ARGB(255, 255, 255, 255);
    pVertexArr[0].vUV = _float2(0.f, 0.f);

    pVertexArr[1].vPosition = _float3(0.5f, 0.5f, 0.f);
    pVertexArr[1].iColor = D3DCOLOR_ARGB(255, 255, 255, 255);
    pVertexArr[1].vUV = _float2(1.f, 0.f);

    pVertexArr[2].vPosition = _float3(0.5f, -0.5f, 0.f);
    pVertexArr[2].iColor = D3DCOLOR_ARGB(255, 255, 255, 255);
    pVertexArr[2].vUV = _float2(1.f, 1.f);

    pVertexArr[3].vPosition = _float3(-0.5f, -0.5f, 0.f);
    pVertexArr[3].iColor = D3DCOLOR_ARGB(255, 255, 255, 255);
    pVertexArr[3].vUV = _float2(0.f, 1.f);

    m_pVertexArr = new VTX_MIX[m_iVertexCount];
    memcpy(m_pVertexArr, pVertexArr, m_iVertexSize * m_iVertexCount);

    m_pVB->Unlock();

    INDEX16* pIndexArr = nullptr;

    //인덱스버퍼가 보관하고 있던 인덱스 배열을 얻어옴
    m_pIB->Lock(0, 0, (void**)&pIndexArr, 0);

    pIndexArr[0]._1 = 0;
    pIndexArr[0]._2 = 1;
    pIndexArr[0]._3 = 2;

    pIndexArr[1]._1 = 0;
    pIndexArr[1]._2 = 2;
    pIndexArr[1]._3 = 3;


    m_pIndexArr = new INDEX16[m_iTriCount];
    memcpy(m_pIndexArr, pIndexArr, m_iIndexSize * m_iTriCount);

    m_pIB->Unlock();

    return S_OK;
}

HRESULT CVIBuffer_Rect::Ready_Component(void* pArg)
{
    if (FAILED(CVIBuffer::Ready_Component(pArg)))
        return E_FAIL;

    //인자가 뭔가 들어온 경우
    if (pArg != nullptr)
    {
        //인자 검사 해줘야 됨
        VTX_MIX* pArgVertexArr = static_cast<VTX_MIX*>(pArg);

        VTX_MIX* pVertexArr = nullptr;
        m_pVB->Lock(0, 0, (void**)&pVertexArr, 0);
        for (int i = 0; i < 4; ++i)
        {
            pVertexArr[i].vPosition = pArgVertexArr[i].vPosition;
            pVertexArr[i].iColor = pArgVertexArr[i].iColor;
        }

        m_pVertexArr = new VTX_MIX[m_iVertexCount];
        memcpy(m_pVertexArr, pVertexArr, m_iVertexSize * m_iVertexCount);

        m_pVB->Unlock();
    }

    return S_OK;
}

HRESULT CVIBuffer_Rect::Render_VIBuffer()
{
    if (FAILED(CVIBuffer::Render_VIBuffer()))
        return E_FAIL;

    return m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_iVertexCount, 0, m_iTriCount);
}

//자주 호출하지 마시오.
//얘는 컬러 인덱스 안씀. 어차피 한장짜린데
HRESULT CVIBuffer_Rect::Change_VertexColor(D3DCOLOR _Color, _uint iIndex)
{
    VTX_MIX* pVertexArr = nullptr;

    m_pVB->Lock(0, 0, (void**)&pVertexArr, 0);
    //색은 블럭별로 한번에 적용됨.
    for (int i = 0; i < 4; ++i)
    {
        pVertexArr[i].iColor = _Color;
    }

    m_pVertexArr = new VTX_MIX[m_iVertexCount];
    memcpy(m_pVertexArr, pVertexArr, m_iVertexSize * m_iVertexCount);

    m_pVB->Unlock();

    return S_OK;
}

CVIBuffer_Rect* CVIBuffer_Rect::Create(_Device pDevice)
{
    CVIBuffer_Rect* pInstance = new CVIBuffer_Rect(pDevice);
    if (FAILED(pInstance->Ready_Component_Prototype()))
    {
        PRINT_LOG(L"Error", L"Failed To Create CVIBuffer_Rect");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CComponent* CVIBuffer_Rect::Clone(void* pArg)
{
    CVIBuffer_Rect* pClone = new CVIBuffer_Rect(*this);
    if (FAILED(pClone->Ready_Component(pArg)))
    {
        if (pArg != nullptr)
        {
            PRINT_LOG(L"Error", L"Failed To Clone CVIBuffer_Rect using Argument");
        }
        else
        {
            PRINT_LOG(L"Error", L"Failed To Clone CVIBuffer_Rect");
        }

        Safe_Release(pClone);
    }

    return pClone;
}

void CVIBuffer_Rect::Free()
{
    CVIBuffer::Free();
}
