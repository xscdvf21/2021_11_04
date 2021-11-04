#include "..\Headers\VIBuffer_RectTexture.h"


USING(Engine)

//타입 인자 안 넣어줬음 넣어줘야됨.

CVIBuffer_RectTexture::CVIBuffer_RectTexture(_Device pDevice)
    : CVIBuffer(pDevice)
{
}

CVIBuffer_RectTexture::CVIBuffer_RectTexture(const CVIBuffer_RectTexture& other)
    : CVIBuffer(other)
{
}

HRESULT CVIBuffer_RectTexture::Ready_Component_Prototype()
{
    m_iVertexSize = sizeof(VTX_TEXTURE);
    m_iVertexCount = 4;
    m_iTriCount = 2;
    m_iFVF = VTX_TEXTURE_FVF;

    m_iIndexSize = sizeof(INDEX16);
    m_IndexFmt = D3DFMT_INDEX16;

    if (FAILED(CVIBuffer::Ready_Component_Prototype()))
    {
        return E_FAIL;
    }

    VTX_TEXTURE* pVertexArr = nullptr;

    m_pVB->Lock(0, 0, (void**)&pVertexArr, 0);

    pVertexArr[0].vPosition = _float3(-0.5f, 0.5f, 0.f);
    pVertexArr[0].vUV = _float2(0.f, 0.f);

    pVertexArr[1].vPosition = _float3(0.5f, 0.5f, 0.f);
    pVertexArr[1].vUV = _float2(1.f, 0.f);

    pVertexArr[2].vPosition = _float3(0.5f, -0.5f, 0.f);
    pVertexArr[2].vUV = _float2(1.f, 1.f);

    pVertexArr[3].vPosition = _float3(-0.5f, -0.5f, 0.f);
    pVertexArr[3].vUV = _float2(0.f, 1.f);

    m_pVertexArr = new VTX_TEXTURE[m_iVertexCount];
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

HRESULT CVIBuffer_RectTexture::Ready_Component(void* pArg)
{
    if (FAILED(CVIBuffer::Ready_Component(pArg)))
        return E_FAIL;

    //인자가 뭔가 들어온 경우
    if (pArg != nullptr)
    {
        //인자 검사 해줘야 됨
        VTX_TEXTURE* pArgVertexArr = static_cast<VTX_TEXTURE*>(pArg);

        VTX_TEXTURE* pVertexArr = nullptr;
        m_pVB->Lock(0, 0, (void**)&pVertexArr, 0);
        for (int i = 0; i < 4; ++i)
        {
            pVertexArr[i].vPosition = pArgVertexArr[i].vPosition;
        }

        m_pVertexArr = new VTX_TEXTURE[m_iVertexCount];
        memcpy(m_pVertexArr, pVertexArr, m_iVertexSize * m_iVertexCount);

        m_pVB->Unlock();
    }

    return S_OK;
}

HRESULT CVIBuffer_RectTexture::Render_VIBuffer()
{
    if (FAILED(CVIBuffer::Render_VIBuffer()))
        return E_FAIL;

    return m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_iVertexCount, 0, m_iTriCount);
}

//자주 호출하지 마시오.
//얘는 컬러 인덱스 안씀. 어차피 한장짜린데
HRESULT CVIBuffer_RectTexture::Change_VertexColor(D3DCOLOR _Color, _uint iIndex)
{
    PRINT_LOG(L"Error", L"Try To Change Color CVIBuffer_RectTexture");
    return E_FAIL;
}

CVIBuffer_RectTexture* CVIBuffer_RectTexture::Create(_Device pDevice)
{
    CVIBuffer_RectTexture* pInstance = new CVIBuffer_RectTexture(pDevice);
    if (FAILED(pInstance->Ready_Component_Prototype()))
    {
        PRINT_LOG(L"Error", L"Failed To Create CVIBuffer_RectTexture");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CComponent* CVIBuffer_RectTexture::Clone(void* pArg)
{
    CVIBuffer_RectTexture* pClone = new CVIBuffer_RectTexture(*this);
    if (FAILED(pClone->Ready_Component(pArg)))
    {
        if (pArg != nullptr)
        {
            PRINT_LOG(L"Error", L"Failed To Clone CVIBuffer_RectTexture using Argument");
        }
        else
        {
            PRINT_LOG(L"Error", L"Failed To Clone CVIBuffer_RectTexture");
        }

        Safe_Release(pClone);
    }

    return pClone;
}

void CVIBuffer_RectTexture::Free()
{
    CVIBuffer::Free();
}
