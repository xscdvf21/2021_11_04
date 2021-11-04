#include "framework.h"
#include "..\Headers\BattlePlateSelectLine.h"

CBattlePlateSelectLine::CBattlePlateSelectLine(_Device pDevice)
    : CBattlePlateEffect(pDevice)
{
}

CBattlePlateSelectLine::CBattlePlateSelectLine(const CBattlePlateSelectLine& other)
    : CBattlePlateEffect(other)
{
    Safe_AddReference(m_pVIBufferCom);
    Safe_AddReference(m_pTextureCom);
}

HRESULT CBattlePlateSelectLine::Ready_GameObject_Prototype()
{
    if (FAILED(CGameObject::Ready_GameObject_Prototype()))
    {
        return E_FAIL;
    }

    return S_OK;
}

HRESULT CBattlePlateSelectLine::Ready_GameObject(void* pArg)
{
    if (FAILED(CGameObject::Ready_GameObject(pArg)))
    {
        return E_FAIL;
    }

    m_pTransformCom->Initialize_vPos(_float3(-11.52f, 0.1f, -11.52f));
    m_pTransformCom->Initialize_vScale(_float3(0.18f, 0.1f, 0.18f));
    m_pTransformCom->Initialize_fSpeed(0.f);
    m_pTransformCom->Initialize_fRotate(0);

    if (FAILED(Add_Component()))
    {
        return E_FAIL;
    }

    m_pTransformCom->Update_Transform();


    return S_OK;

}

_uint CBattlePlateSelectLine::Update_GameObject(_float fDeltaTime)
{
    CGameObject::LateUpdate_GameObject(fDeltaTime);

    _mat matWorld, matRotY;

    //Slide와 같은 방식 씀
    D3DXMatrixRotationY(&matRotY, D3DXToRadian((avIndex - bavIndex) * 30.f));

    matWorld = m_pTransformCom->Get_WorldMatrix();
    matWorld *= matRotY;

    m_pTransformCom->Set_WorldMatrix(matWorld);

    bavIndex = avIndex;

    auto pManagement = CManagement::Get_Instance();
    if (nullptr == pManagement)
        return 0;



    return _uint();
}

_uint CBattlePlateSelectLine::LateUpdate_GameObject(_float fDeltaTime)
{
    CGameObject::LateUpdate_GameObject(fDeltaTime);

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

HRESULT CBattlePlateSelectLine::Render_GameObject()
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

CBattlePlateSelectLine* CBattlePlateSelectLine::Create(_Device pDevice)
{
    CBattlePlateSelectLine* pInstance = new CBattlePlateSelectLine(pDevice);
    if (FAILED(pInstance->Ready_GameObject_Prototype()))
    {
        PRINT_LOG(L"Error", L"Failed To Create BattlePlateSelectLine");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CBattlePlateSelectLine::Clone(void* pArg)
{
    CBattlePlateSelectLine* pInstance = new CBattlePlateSelectLine(*this); /*복사생성호출*/
    if (FAILED(pInstance->Ready_GameObject(pArg)))
    {
        PRINT_LOG(L"Error", L"Failed To Clone BattlePlateSelectLine");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CBattlePlateSelectLine::Free()
{
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);

    CBattlePlateEffect::Free();
}

HRESULT CBattlePlateSelectLine::Add_Component()
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
        L"Component_Texture_BattlePlate_Select_Slide",
        L"Com_Texture",
        (CComponent**)&m_pTextureCom)))
        return E_FAIL;

    return S_OK;
}
