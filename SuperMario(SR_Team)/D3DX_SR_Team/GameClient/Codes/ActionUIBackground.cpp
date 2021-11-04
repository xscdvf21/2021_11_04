#include "framework.h"
#include "ActionUIBackground.h"

#include "Transform.h"
#include "VIBuffer.h"
#include "Texture.h"
#include "BattleCamera.h"

CActionUIBackground::CActionUIBackground(_Device pDevice)
    : CGameObject(pDevice)
{
}

CActionUIBackground::CActionUIBackground(const CActionUIBackground& other)
    : CGameObject(other)
{
}

HRESULT CActionUIBackground::Ready_GameObject_Prototype()
{
    if (FAILED(CGameObject::Ready_GameObject_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CActionUIBackground::Ready_GameObject(void* pArg)
{
    if (FAILED(CGameObject::Ready_GameObject(pArg)))
        return E_FAIL;

	m_pTransformCom->Initialize_vPos(_float3(1.f, 1.f, 1.f));
	m_pTransformCom->Initialize_vScale(_float3(24.f, 10.f, 1.f));
	m_pTransformCom->Initialize_fSpeed(0.f);
	m_pTransformCom->Initialize_fRotate(D3DXToRadian(0.f));

    if (FAILED(Add_Component()))
        return E_FAIL;

    D3DXCreateFont(m_pDevice, 50, 30, 500, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, L"나눔손글씨 펜", &m_pFont);

    return S_OK;
}

_uint CActionUIBackground::Update_GameObject(_float fDeltaTime)
{
    CGameObject::Update_GameObject(fDeltaTime);

    return _uint();
}

_uint CActionUIBackground::LateUpdate_GameObject(_float fDeltaTime)
{
    CGameObject::LateUpdate_GameObject(fDeltaTime);

    auto pManagement = CManagement::Get_Instance();

    if (nullptr == pManagement)
        return 0;

    if (FAILED(pManagement->Add_RenderList(ERenderPriority::UI, this)))
        return 0;

    return m_pTransformCom->Update_Transform();
}

HRESULT CActionUIBackground::Render_GameObject()
{
    auto pManagement = CManagement::Get_Instance();
    if (nullptr == pManagement)
        return E_FAIL;

    CBattleCamera* pCamera = dynamic_cast<CBattleCamera*>(pManagement->Get_GameObject((_uint)ESceneType::Battle, L"Layer_Camera"));

    pCamera->Set_Ortho_For_UI(_float3(10.f, 10.f, 1.f),_float3(-300.f,200.f,0.f));

    ////아래는 그냥 랜더러

    if (FAILED(m_pTransformCom->Set_Transform()))
        return E_FAIL;

    if (FAILED(CGameObject::Render_GameObject()))
        return E_FAIL;

    if (FAILED(m_pTextureCom->Set_Texture(0)))
        return E_FAIL;

    if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
        return E_FAIL;

    TCHAR szBuf[128] = L"";			//테스트 문구 수정해주세요
    swprintf_s(szBuf, L" X %d", m_iActionCount);

    //사각형 영역 설정 , -1은 옵션에 NOCLIP을 넣었기 때문에 , 값을 주지 않은것이다
    //=위치
    RECT rc = { 30, 80, -1, -1 };

    m_pFont->DrawText(NULL, szBuf, -1, &rc, DT_LEFT | DT_NOCLIP, 0xff22AA00);

    return S_OK;
}

CActionUIBackground* CActionUIBackground::Create(_Device pDevice)
{
    CActionUIBackground* pInstance = new CActionUIBackground(pDevice);
    if (FAILED(pInstance->Ready_GameObject_Prototype()))
    {
        PRINT_LOG(L"Error", L"Failed To Create BattleUI_ActionUIBackground");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CActionUIBackground::Clone(void* pArg)
{
    CActionUIBackground* pInstance = new CActionUIBackground(*this); /*복사생성호출*/
    if (FAILED(pInstance->Ready_GameObject(pArg)))
    {
        PRINT_LOG(L"Error", L"Failed To Clone BattleUI_ActionUIBackground");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CActionUIBackground::Free()
{
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);

    CGameObject::Free();
}

void CActionUIBackground::Set_ActionCount(_uint iActionCount)
{
    m_iActionCount = iActionCount;
}

HRESULT CActionUIBackground::Add_Component()
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
        L"Component_Texture_BattleUI_ActionBG",
        L"Com_Texture",
        (CComponent**)&m_pTextureCom)))
        return E_FAIL;

    return S_OK;
}
