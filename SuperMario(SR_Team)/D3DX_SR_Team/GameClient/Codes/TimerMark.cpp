#include "framework.h"
#include "..\Headers\TimerMark.h"
#include "Transform.h"
#include "VIBuffer.h"
#include "Texture.h"
#include "BattleCamera.h"


CTimerMark::CTimerMark(_Device pDevice)
    : CGameObject(pDevice)
    , m_fRemainTime(0.f)
{
}

CTimerMark::CTimerMark(const CTimerMark& other)
    : CGameObject(other)
    , m_fRemainTime(other.m_fRemainTime)
{
}

HRESULT CTimerMark::Ready_GameObject_Prototype()
{
    if (FAILED(CGameObject::Ready_GameObject_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CTimerMark::Ready_GameObject(void* pArg)
{
    if (FAILED(CGameObject::Ready_GameObject(pArg)))
        return E_FAIL;

	m_pTransformCom->Initialize_vPos(_float3(1.f, 1.f, 1.f));
	m_pTransformCom->Initialize_vScale(_float3(24.f, 10.f, 1.f));
	m_pTransformCom->Initialize_fSpeed(0.f);
	m_pTransformCom->Initialize_fRotate(D3DXToRadian(0.f));



    if (FAILED(Add_Component()))
        return E_FAIL;

    D3DXCreateFont(m_pDevice, 30, 30, 500, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, L"�����ձ۾� ��", &m_pFont);

    return S_OK;
}

_uint CTimerMark::Update_GameObject(_float fDeltaTime)
{
    CGameObject::Update_GameObject(fDeltaTime);

    return _uint();
}

_uint CTimerMark::LateUpdate_GameObject(_float fDeltaTime)
{
    CGameObject::LateUpdate_GameObject(fDeltaTime);

    auto pManagement = CManagement::Get_Instance();

    if (nullptr == pManagement)
        return 0;

    if (FAILED(pManagement->Add_RenderList(ERenderPriority::UI, this)))
        return 0;

    return m_pTransformCom->Update_Transform();
}

HRESULT CTimerMark::Render_GameObject()
{
    auto pManagement = CManagement::Get_Instance();
    if (nullptr == pManagement)
        return E_FAIL;

    CBattleCamera* pCamera = dynamic_cast<CBattleCamera*>(pManagement->Get_GameObject((_uint)ESceneType::Battle, L"Layer_Camera"));

    pCamera->Set_Ortho_For_UI(_float3(10.f, 10.f, 1.f),_float3(300.f,200.f,0.f));

    ////�Ʒ��� �׳� ������

    if (FAILED(m_pTransformCom->Set_Transform()))
        return E_FAIL;

    if (FAILED(CGameObject::Render_GameObject()))
        return E_FAIL;

    if (FAILED(m_pTextureCom->Set_Texture(0)))
        return E_FAIL;

    if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
        return E_FAIL;


    TCHAR szBuf[128] = L"";			//�׽�Ʈ ���� �������ּ���
    swprintf_s(szBuf, L"%2.2f", m_fRemainTime);

    //�簢�� ���� ���� , -1�� �ɼǿ� NOCLIP�� �־��� ������ , ���� ���� �������̴�
    //=��ġ
    RECT rc = { 650, 50, -1, -1 };

    m_pFont->DrawText(NULL, szBuf, -1, &rc, DT_LEFT | DT_NOCLIP, 0xff222200);


    return S_OK;
}

CTimerMark* CTimerMark::Create(_Device pDevice)
{
    CTimerMark* pInstance = new CTimerMark(pDevice);
    if (FAILED(pInstance->Ready_GameObject_Prototype()))
    {
        PRINT_LOG(L"Error", L"Failed To Create BattleUI_TimeMarkUI");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CTimerMark::Clone(void* pArg)
{
    CTimerMark* pInstance = new CTimerMark(*this); /*�������ȣ��*/
    if (FAILED(pInstance->Ready_GameObject(pArg)))
    {
        PRINT_LOG(L"Error", L"Failed To Clone BattleUI_TimeMarkUI");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CTimerMark::Free()
{
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);


    CGameObject::Free();
}

HRESULT CTimerMark::Add_Component()
{
    /*For.Com_VIBuffer*/
    if (FAILED(CGameObject::Add_Component(
        (_uint)ESceneType::Static,
        L"Component_VIBuffer_RectTexture",
        L"Com_VIBuffer",
        (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;

    /*For.Com_Texture*/
    if (FAILED(CGameObject::Add_Component(
        (_uint)ESceneType::Static,
        L"Component_Texture_BattleUI_TimerBG",
        L"Com_Texture",
        (CComponent**)&m_pTextureCom)))
        return E_FAIL;

    return S_OK;
}
