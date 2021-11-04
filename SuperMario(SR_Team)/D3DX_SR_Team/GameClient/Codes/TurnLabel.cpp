#include "framework.h"
#include "TurnLabel.h"

#include "Transform.h"
#include "VIBuffer.h"
#include "Texture.h"
#include "BattleCamera.h"

CTurnLabel::CTurnLabel(_Device pDevice)
	: CGameObject(pDevice)
	, bIsVisual(false)
{
}

CTurnLabel::CTurnLabel(const CTurnLabel& other)
	: CGameObject(other)
	, bIsVisual(false)
{
}

HRESULT CTurnLabel::Ready_GameObject_Prototype()
{
	if (FAILED(CGameObject::Ready_GameObject_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTurnLabel::Ready_GameObject(void* pArg)
{
	if (FAILED(CGameObject::Ready_GameObject(pArg)))
		return E_FAIL;

	m_pTransformCom->Initialize_vPos(_float3(1.f, 1.f, 1.f));
	m_pTransformCom->Initialize_vScale(_float3(24.f, 10.f, 1.f));
	m_pTransformCom->Initialize_fSpeed(0.f);
	m_pTransformCom->Initialize_fRotate(D3DXToRadian(0.f));

	if (FAILED(Add_Component()))
		return E_FAIL;

	return S_OK;
}

_uint CTurnLabel::Update_GameObject(_float fDeltaTime)
{
	CGameObject::Update_GameObject(fDeltaTime);

	return _uint();
}

_uint CTurnLabel::LateUpdate_GameObject(_float fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);

	auto pManagement = CManagement::Get_Instance();

	if (nullptr == pManagement)
		return 0;

	if (bIsVisual == true)
	{
		if (FAILED(pManagement->Add_RenderList(ERenderPriority::UI, this)))
			return 0;

		bIsVisual = false;
	}

	return m_pTransformCom->Update_Transform();
}

HRESULT CTurnLabel::Render_GameObject()
{
	auto pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CBattleCamera* pCamera = dynamic_cast<CBattleCamera*>(pManagement->Get_GameObject((_uint)ESceneType::Battle, L"Layer_Camera"));

	pCamera->Set_Ortho_For_UI(_float3(10.f, 10.f, 1.f), _float3(0.f, 50.f, 0.f));

	////아래는 그냥 랜더러

	if (eTurn != ETurnState::NOEVENT)
	{

		if (FAILED(m_pTransformCom->Set_Transform()))
			return E_FAIL;

		if (FAILED(CGameObject::Render_GameObject()))
			return E_FAIL;

		if (FAILED(m_pTextureCom[(_uint)eTurn]->Set_Texture(0)))
			return E_FAIL;

		if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
			return E_FAIL;
	}

	return S_OK;
}

CTurnLabel* CTurnLabel::Create(_Device pDevice)
{
	CTurnLabel* pInstance = new CTurnLabel(pDevice);
	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		PRINT_LOG(L"Error", L"Failed To Create BattleUI_TurnLabel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CTurnLabel::Clone(void* pArg)
{
	CTurnLabel* pInstance = new CTurnLabel(*this); /*복사생성호출*/
	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		PRINT_LOG(L"Error", L"Failed To Clone BattleUI_ActionUIBackground");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTurnLabel::Free()
{
	CGameObject::Free();
}

HRESULT CTurnLabel::Add_Component()
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
		(CComponent**)&m_pTextureCom[(_uint)ETurnState::PLAYER])))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(
		(_uint)ESceneType::Static,
		L"Component_Texture_BattleUI_ActionBG",
		L"Com_Texture",
		(CComponent**)&m_pTextureCom[(_uint)ETurnState::ENEMY])))
		return E_FAIL;

	return S_OK;
}
