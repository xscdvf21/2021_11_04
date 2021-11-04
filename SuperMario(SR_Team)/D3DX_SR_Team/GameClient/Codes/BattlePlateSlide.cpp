#include "framework.h"
#include "..\Headers\BattlePlateSlide.h"

CBattlePlateSlide::CBattlePlateSlide(_Device pDevice)
    : CBattlePlateEffect(pDevice)
{
}

CBattlePlateSlide::CBattlePlateSlide(const CBattlePlateSlide& other)
    : CBattlePlateEffect(other)
{
    Safe_AddReference(m_pVIBufferCom);
    Safe_AddReference(m_pTextureCom);
}

HRESULT CBattlePlateSlide::Ready_GameObject_Prototype()

{
    if (FAILED(CGameObject::Ready_GameObject_Prototype()))
    {
        return E_FAIL;
    }

    return S_OK;
}

HRESULT CBattlePlateSlide::Ready_GameObject(void* pArg)
{
    if (FAILED(CGameObject::Ready_GameObject(pArg)))
    {
        return E_FAIL;
    }

    m_pTransformCom->Initialize_vPos(_float3(-12.8f,0.1f,-12.8f));
    m_pTransformCom->Initialize_vScale(_float3(0.2f,0.2f,0.2f));
    m_pTransformCom->Initialize_fSpeed(0.f);
    m_pTransformCom->Initialize_fRotate(0);

    if (FAILED(Add_Component()))
    {
        return E_FAIL;
    }

    m_pTransformCom->Update_Transform();

    return S_OK;
}

_uint CBattlePlateSlide::Update_GameObject(_float fDeltaTime)
{
    CGameObject::Update_GameObject(fDeltaTime);

    return _uint();
}

_uint CBattlePlateSlide::LateUpdate_GameObject(_float fDeltaTime)
{
    CGameObject::LateUpdate_GameObject(fDeltaTime);

    _mat matWorld, matRotY;

    //이거... 회전을...
    D3DXMatrixRotationY(&matRotY, D3DXToRadian((avIndex - bavIndex)*30.f));

    matWorld = m_pTransformCom->Get_WorldMatrix();
    matWorld *= matRotY;

    m_pTransformCom->Set_WorldMatrix(matWorld);

    bavIndex = avIndex;

    auto pManagement = CManagement::Get_Instance();
    if (nullptr == pManagement)
        return 0;

    if (m_bVisual == true)
    {
        if (FAILED(pManagement->Add_RenderList(ERenderPriority::HalfAlpha, this)))
            return 0;
        m_bVisual = false;
    }

    return 0;
}

HRESULT CBattlePlateSlide::Render_GameObject()
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

CBattlePlateSlide* CBattlePlateSlide::Create(_Device pDevice)
{
    CBattlePlateSlide* pInstance = new CBattlePlateSlide(pDevice);
    if (FAILED(pInstance->Ready_GameObject_Prototype()))
    {
        PRINT_LOG(L"Error", L"Failed To Create CBattlePlateSlide");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CBattlePlateSlide::Clone(void* pArg)
{
    CBattlePlateSlide* pInstance = new CBattlePlateSlide(*this); /*복사생성호출*/
    if (FAILED(pInstance->Ready_GameObject(pArg)))
    {
        PRINT_LOG(L"Error", L"Failed To Clone BattlePlateSlide");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CBattlePlateSlide::Free()
{
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);

    CBattlePlateEffect::Free();
}

HRESULT CBattlePlateSlide::Add_Component()
{
    if (FAILED(CGameObject::Add_Component(
        (_uint)ESceneType::Static,
        L"Component_VIBuffer_Terrain",
        L"Com_VIBuffer",
        (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;


    //대충 텍스쳐 추가하는 코드
    if (FAILED(CGameObject::Add_Component(
        (_uint)ESceneType::Static,
        L"Component_Texture_BattlePlate_Slide",
        L"Com_Texture",
        (CComponent**)&m_pTextureCom)))
        return E_FAIL;

    return S_OK;
}
