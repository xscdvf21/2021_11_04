#include "framework.h"
#include "..\Headers\BattlePlateCircle.h"

CBattlePlateCircle::CBattlePlateCircle(_Device pDevice)
    : CBattlePlateEffect(pDevice)
{
    ZeroMemory(&avPos, sizeof(avPos));
    ZeroMemory(&avScale, sizeof(avScale));
}

CBattlePlateCircle::CBattlePlateCircle(const CBattlePlateCircle& other)
    : CBattlePlateEffect(other)
{
    ZeroMemory(&avPos, sizeof(avPos));
    ZeroMemory(&avScale, sizeof(avScale));

    Safe_AddReference(m_pVIBufferCom);
    Safe_AddReference(m_pTextureCom);

}

HRESULT CBattlePlateCircle::Ready_GameObject_Prototype()
{
    if (FAILED(CGameObject::Ready_GameObject_Prototype()))
    {
        return E_FAIL;
    }

    return S_OK;
}

HRESULT CBattlePlateCircle::Ready_GameObject(void* pArg)
{
    if (FAILED(CGameObject::Ready_GameObject(pArg)))
    {
        return E_FAIL;
    }
    for (int i = 0; i < 4; i++) 
    {
        avPos[i] = _float3(-3.2 * (i + 2), 0.1f, -3.2 * (i + 2));
        avScale[i] = _float3(0.05 * (i + 2), 0.f, 0.05*(i + 2));
    }
    avPos[4] = _float3(0.f,0.f,0.f);
    avScale[4] = _float3(0.f, 0.f, 0.f);

    

    m_pTransformCom->Initialize_vPos(avPos[0]);
    m_pTransformCom->Initialize_vScale(avScale[0]);
    m_pTransformCom->Initialize_fSpeed(0.f);
    m_pTransformCom->Initialize_fRotate(D3DXToRadian(90.f));

    if (FAILED(Add_Component()))
    {
        return E_FAIL;
    }

    return S_OK;
}

_uint CBattlePlateCircle::Update_GameObject(_float fDeltaTime)
{
    CGameObject::Update_GameObject(fDeltaTime);



    return _uint();
}

_uint CBattlePlateCircle::LateUpdate_GameObject(_float fDeltaTime)
{
    CGameObject::LateUpdate_GameObject(fDeltaTime);

    m_pTransformCom->Set_Position(avPos[avIndex]);
    m_pTransformCom->Set_Scale(avScale[avIndex]);

    auto pManagement = CManagement::Get_Instance();
    if (nullptr == pManagement)
        return 0;

    if (m_bVisual == true) 
    {
        if (FAILED(pManagement->Add_RenderList(ERenderPriority::HalfAlpha, this)))
            return 0;
       m_bVisual = false;
    }
    return m_pTransformCom->Update_Transform();

}

HRESULT CBattlePlateCircle::Render_GameObject()
{
    if (FAILED(m_pTransformCom->Set_Transform()))
        return E_FAIL;

    if (FAILED(CGameObject::Render_GameObject()))
        return E_FAIL;

    if (FAILED(m_pTextureCom->Set_Texture(0)))
        return E_FAIL;

    if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
        return E_FAIL;


    return S_OK;
}

CBattlePlateCircle* CBattlePlateCircle::Create(_Device pDevice)
{
    CBattlePlateCircle* pInstance = new CBattlePlateCircle(pDevice);
    if (FAILED(pInstance->Ready_GameObject_Prototype()))
    {
        PRINT_LOG(L"Error", L"Failed To Create BattlePlateCircle");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CBattlePlateCircle::Clone(void* pArg)
{
    CBattlePlateCircle* pInstance = new CBattlePlateCircle(*this); /*복사생성호출*/
    if (FAILED(pInstance->Ready_GameObject(pArg)))
    {
        PRINT_LOG(L"Error", L"Failed To Clone BattlePlateCircle");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CBattlePlateCircle::Free()
{
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);

    CBattlePlateEffect::Free();
}

HRESULT CBattlePlateCircle::Add_Component()
{
    ////대충 바닥 버텍스 버퍼 추가하는 코드
    //if (FAILED(CGameObject::Add_Component(
    //    (_uint)ESceneType::Static,
    //    L"Component_VIBuffer_Alpha_Round",
    //    L"Com_VIBuffer",
    //    (CComponent**)&m_pVIBufferCom)))
    //    return E_FAIL;

        //대충 바닥 버텍스 버퍼 추가하는 코드
    if (FAILED(CGameObject::Add_Component(
        (_uint)ESceneType::Static,
        L"Component_VIBuffer_Terrain",
        L"Com_VIBuffer",
        (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;


    //대충 텍스쳐 추가하는 코드
    if (FAILED(CGameObject::Add_Component(
        (_uint)ESceneType::Static,
        L"Component_Texture_BattlePlate_Circle",
        L"Com_Texture",
        (CComponent**)&m_pTextureCom)))
        return E_FAIL;

    return S_OK;
}
