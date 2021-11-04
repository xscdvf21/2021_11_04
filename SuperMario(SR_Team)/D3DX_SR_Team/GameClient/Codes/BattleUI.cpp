#include "framework.h"
#include "..\Headers\BattleUI.h"

#include "Transform.h"
#include "VIBuffer.h"
#include "Texture.h"
#include "BattleCamera.h"

#include "ActionUIBackground.h"
#include "TimerMark.h"
#include "RingCountBackground.h"

CBattleUI::CBattleUI(_Device pDevice)
    : CGameObject(pDevice)
    , m_bVisualHP(false)
    , m_bVisualTime(false)
    , m_bVisualRing(false)
    , m_bVisualAttack(false)
    , m_bVisualCoin(false)
    , m_pTimerMark_UI(nullptr)
    , m_pActionBG_UI(nullptr)
    , m_pRingBG_UI(nullptr)
{
}

CBattleUI::CBattleUI(const CBattleUI& other)
    : CGameObject(other)
    , m_bVisualHP(false)
    , m_bVisualTime(false)
    , m_bVisualRing(false)
    , m_bVisualAttack(false)
    , m_bVisualCoin(false)
    , m_pTimerMark_UI(other.m_pTimerMark_UI)
    , m_pActionBG_UI(other.m_pActionBG_UI)
    , m_pRingBG_UI(other.m_pRingBG_UI)
{
    Safe_AddReference(m_pActionBG_UI);
    Safe_AddReference(m_pTimerMark_UI);
    Safe_AddReference(m_pRingBG_UI);
}

HRESULT CBattleUI::Ready_GameObject_Prototype()
{
    if (FAILED(CGameObject::Ready_GameObject_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CBattleUI::Ready_GameObject(void* pArg)
{
    if (FAILED(CGameObject::Ready_GameObject(pArg)))
        return E_FAIL;

    if (FAILED(Add_Component()))
        return E_FAIL;

    //폰트 추가
    D3DXCreateFont(m_pDevice, 50, 50, 1000, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, L"맑은고딕", &m_pFont);

    return S_OK;
}

_uint CBattleUI::Update_GameObject(_float fDeltaTime)
{
    CGameObject::Update_GameObject(fDeltaTime);

    return _uint();
}

_uint CBattleUI::LateUpdate_GameObject(_float fDeltaTime)
{
    CGameObject::LateUpdate_GameObject(fDeltaTime);

   
    auto pManagement = CManagement::Get_Instance();
    if (nullptr == pManagement)
        return 0;

    if (FAILED(pManagement->Add_RenderList(ERenderPriority::UI, this)))
        return 0;



    return m_pTransformCom->Update_Transform();
}

HRESULT CBattleUI::Render_GameObject()
{
    auto pManagement = CManagement::Get_Instance();

    if (nullptr == pManagement)
        return E_FAIL;

    CBattleCamera* pCamera = dynamic_cast<CBattleCamera*>(pManagement->Get_GameObject((_uint)ESceneType::Battle, L"Layer_Camera"));

	 pCamera->Set_Ortho_For_UI(_float3(10.f,10.f,1.f), _float3(10.f, 10.f,1.f));


    if (FAILED(m_pTransformCom->Set_Transform()))
        return E_FAIL;

    if (FAILED(CGameObject::Render_GameObject()))
        return E_FAIL;


    //if (m_bVisualTime) m_pTimerMark_UI->Render_GameObject();
    //if (m_bVisualRing) m_pRingBG_UI->Render_GameObject();
    //if (m_bVisualAttack) m_pActionBG_UI->Render_GameObject();

    m_bVisualTime = false;
    m_bVisualRing = false;
    m_bVisualAttack = false;


    return S_OK;
}

CBattleUI* CBattleUI::Create(_Device pDevice)
{
    CBattleUI* pInstance = new CBattleUI(pDevice);
    if (FAILED(pInstance->Ready_GameObject_Prototype()))
    {
        PRINT_LOG(L"Error", L"Failed To Create BattleUI");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CBattleUI::Clone(void* pArg)
{
    CBattleUI* pInstance = new CBattleUI(*this);
    if (FAILED(pInstance->Ready_GameObject(pArg)))
    {
        PRINT_LOG(L"Error", L"Failed To Clone BattleUI");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CBattleUI::Free()
{
     Safe_Release(m_pActionBG_UI);
     Safe_Release(m_pTimerMark_UI);
     Safe_Release(m_pRingBG_UI);

    
     Safe_Release(m_pVIBufferCom);
     Safe_Release(m_pTextureCom);

    CGameObject::Free();
}

void CBattleUI::Set_Timer(_float fTime)
{
    if (m_pTimerMark_UI)
    m_pTimerMark_UI->Set_RemainTime(fTime);
}

void CBattleUI::Set_Ring(_uint iRing)
{
    if (m_pRingBG_UI)
    m_pRingBG_UI->Set_RingCount(iRing);
}

void CBattleUI::Set_AttackCount(_uint iAttack)
{
    if (m_pActionBG_UI)
    m_pActionBG_UI->Set_ActionCount(iAttack);
}

HRESULT CBattleUI::Add_Component()
{
    /*For.Com_VIBuffer*/
    if (FAILED(CGameObject::Add_Component(
        (_uint)ESceneType::Static,
        L"Component_VIBuffer_RectTexture",
        L"Com_VIBuffer",
        (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;

    /*For.Com_Transform*/
    
    
    m_pTransformCom->Initialize_vPos({ 1.f, 2.f, 5.f });
    m_pTransformCom->Initialize_vScale({10.f, 10.f, 1.f });
    


    auto pManagement = CManagement::Get_Instance();
    if (nullptr == pManagement)
        return E_FAIL;

    if (FAILED(pManagement->Clone_GameObject_ToLayer(
        (_uint)ESceneType::Static,
        L"GameObject_BattleUI_ActionCounter",
        (_uint)ESceneType::Battle,
        L"Layer_BattleUI_COMP")))
    {
        return E_FAIL;
    }
    m_pActionBG_UI = dynamic_cast<CActionUIBackground*>(pManagement->Get_GameObject((_uint)ESceneType::Battle, L"Layer_BattleUI_COMP", 0));


    if (FAILED(pManagement->Clone_GameObject_ToLayer(
        (_uint)ESceneType::Static,
        L"GameObject_BattleUI_TimeMark",
        (_uint)ESceneType::Battle,
        L"Layer_BattleUI_COMP")))
    {
        return E_FAIL;
    }
    m_pTimerMark_UI = dynamic_cast<CTimerMark*>(pManagement->Get_GameObject((_uint)ESceneType::Battle, L"Layer_BattleUI_COMP", 1));


    if (FAILED(pManagement->Clone_GameObject_ToLayer(
        (_uint)ESceneType::Static,
        L"GameObject_BattleUI_RingCounter",
        (_uint)ESceneType::Battle,
        L"Layer_BattleUI_COMP")))
    {
        return E_FAIL;
    }
    m_pRingBG_UI = dynamic_cast<CRingCountBackground*>(pManagement->Get_GameObject((_uint)ESceneType::Battle, L"Layer_BattleUI_COMP", 2));



    return S_OK;
}
