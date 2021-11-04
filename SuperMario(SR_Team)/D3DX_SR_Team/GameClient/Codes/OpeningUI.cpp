#include "framework.h"
#include "OpeningUI.h"

#include "OSCamera.h"

COpeningUI::COpeningUI(_Device pDevice)
	: CGameObject(pDevice)
{
}

COpeningUI::COpeningUI(const COpeningUI& other)
	: CGameObject(other)
{
}

HRESULT COpeningUI::Ready_GameObject_Prototype()
{
	if (FAILED(CGameObject::Ready_GameObject_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT COpeningUI::Ready_GameObject(void* pArg)
{
	if (FAILED(CGameObject::Ready_GameObject(pArg)))
		return E_FAIL;

	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pTransformCom->Initialize_vScale({ 10.f,10.f,1.f });

	return S_OK;
}

_uint COpeningUI::Update_GameObject(_float fDeltaTime)
{
	CGameObject::Update_GameObject(fDeltaTime);

	return _uint();
}

_uint COpeningUI::LateUpdate_GameObject(_float fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);


	auto pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return 0;

	if (FAILED(pManagement->Add_RenderList(ERenderPriority::UI, this)))
		return 0;

	return m_pTransformCom->Update_Transform();
}

HRESULT COpeningUI::Render_GameObject()
{
	auto pManagement = CManagement::Get_Instance();

	if (nullptr == pManagement)
		return E_FAIL;

	COSCamera* pCamera = dynamic_cast<COSCamera*>(pManagement->Get_GameObject((_uint)ESceneType::Logo, L"Opening_Scene"));

	pCamera->Set_Ortho_For_UI(_float3(80.f, 60.f, 1.f), _float3(0.f, 0.f, 1.f));


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

COpeningUI* COpeningUI::Create(_Device pDevice)
{
	COpeningUI* pInstance = new COpeningUI(pDevice);
	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		PRINT_LOG(L"Error", L"Failed To Create OpeningUI");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* COpeningUI::Clone(void* pArg)
{
	COpeningUI* pInstance = new COpeningUI(*this);
	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		PRINT_LOG(L"Error", L"Failed To Clone OpeningUI");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void COpeningUI::Free()
{
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);

	CGameObject::Free();
}

HRESULT COpeningUI::Add_Component()
{
	if (FAILED(CGameObject::Add_Component(
		(_uint)ESceneType::Static,
		L"Component_VIBuffer_RectTexture",
		L"Com_VIBuffer",
		(CComponent**)&m_pVIBufferCom)))
		return E_FAIL;
	
	if (FAILED(CGameObject::Add_Component(
		(_uint)ESceneType::Static,
		L"Component_Texture_OpeningScene",
		L"Com_Texture",
		(CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
	
}
