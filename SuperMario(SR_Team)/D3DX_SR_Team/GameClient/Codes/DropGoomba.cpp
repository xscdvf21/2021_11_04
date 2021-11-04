#include "framework.h"
#include "..\Headers\DropGoomba.h"

CDropGoomba::CDropGoomba(_Device pDevice)
    : CGameObject(pDevice)
{
}

CDropGoomba::CDropGoomba(const CDropGoomba& other)
    : CGameObject(other)
{
}

HRESULT CDropGoomba::Ready_GameObject_Prototype()
{
    if (FAILED(CGameObject::Ready_GameObject_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CDropGoomba::Ready_GameObject(void* pArg)
{
    if (FAILED(CGameObject::Ready_GameObject(pArg)))
        return E_FAIL;

    if (FAILED(Add_Component()))
        return E_FAIL;

    m_pTransformCom->Initialize_vScale(_float3(2.5f, 2.5f, 2.5f));
    m_pTransformCom->Initialize_vPos(_float3(0.f, 25.f, 0.f));

    m_fDropEnd = 1.f;
    m_fDropStart = m_pTransformCom->Get_Position().y;
    m_fDroptime = 2.f;
    return S_OK;
}

_uint CDropGoomba::Update_GameObject(_float fDeltaTime)
{
    if (bisTrigger == true)
    {
        _float3 vPos = m_pTransformCom->Get_Position();
        if (vPos.y < m_fDropEnd)
        {
            bisTrigger = false;
        }
        else
        {
            vPos.y -= fDeltaTime * (m_fDropStart - m_fDropEnd) / m_fDroptime;
            m_pTransformCom->Set_Position(vPos);
        }
    }
    if (bisTrigger != true)
    {
        m_pTransformCom->Set_Position(_float3(0.f, 5.f, 0.f));
    }

    return _uint();
}

_uint CDropGoomba::LateUpdate_GameObject(_float fDeltaTime)
{
    auto pManagement = CManagement::Get_Instance();
    if (nullptr == pManagement)
        return 0;

    if (FAILED(pManagement->Add_RenderList(ERenderPriority::Alpha, this)))
        return 0;

    return m_pTransformCom->Update_Transform();
}

HRESULT CDropGoomba::Render_GameObject()
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

CDropGoomba* CDropGoomba::Create(_Device pDevice)
{
    CDropGoomba* pInstance = new CDropGoomba(pDevice);
    if (FAILED(pInstance->Ready_GameObject_Prototype()))
    {
        PRINT_LOG(L"Error", L"Failed To Create DropGoomba");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CDropGoomba::Clone(void* pArg)
{
    CDropGoomba* pInstance = new CDropGoomba(*this); /*복사생성호출*/
    if (FAILED(pInstance->Ready_GameObject(pArg)))
    {
        PRINT_LOG(L"Error", L"Failed To Clone DropGoomba");
        Safe_Release(pInstance);
    }

    return pInstance;
}
void CDropGoomba::Free()
{
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);

    CGameObject::Free();
}

HRESULT CDropGoomba::Add_Component()
{
    if (FAILED(CGameObject::Add_Component(
        (_uint)ESceneType::Static,
        L"Component_VIBuffer_RectTexture",
        L"Com_VIBuffer",
        (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;

    if (FAILED(CGameObject::Add_Component(
        (_uint)ESceneType::Static,
        L"Component_Texture_DropAttack_Goomba",
        L"Com_Texture",
        (CComponent**)&m_pTextureCom)))
        return E_FAIL;
}
