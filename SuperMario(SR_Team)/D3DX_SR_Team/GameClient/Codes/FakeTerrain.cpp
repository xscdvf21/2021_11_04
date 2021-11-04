#include "framework.h"
#include "..\Headers\FakeTerrain.h"
#include "Management.h"

CFakeTerrain::CFakeTerrain(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject(pDevice)
{
}

CFakeTerrain::CFakeTerrain(const CFakeTerrain & other)
	: CGameObject(other)
{
}

HRESULT CFakeTerrain::Ready_GameObject_Prototype()
{
	if (FAILED(CGameObject::Ready_GameObject_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CFakeTerrain::Ready_GameObject(void * pArg /*= nullptr*/)
{
	if (FAILED(CGameObject::Ready_GameObject(pArg)))
		return E_FAIL;

	m_eSceneType = (_uint)ESceneType::Static;

	if (FAILED(Add_Component()))
		return E_FAIL;

	return S_OK;
}

_uint CFakeTerrain::Update_GameObject(_float fDeltaTime)
{
	CGameObject::Update_GameObject(fDeltaTime);


	return _uint();
}

_uint CFakeTerrain::LateUpdate_GameObject(_float fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);

	auto pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return 0;

	if (FAILED(pManagement->Add_RenderList(ERenderPriority::NoAlpha, this)))
		return 0;

	return m_pTransformCom->Update_Transform();
}

HRESULT CFakeTerrain::Render_GameObject()
{
	if (FAILED(m_pTransformCom->Set_Transform()))
		return E_FAIL;

	if (FAILED(CGameObject::Render_GameObject()))
		return E_FAIL;

	//if (FAILED(m_pTextureCom->Set_Texture(m_iTextureNumber)))
	if (FAILED(m_pTextureCom->Set_Texture(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
		return E_FAIL;



	return S_OK;
}

HRESULT CFakeTerrain::Add_Component()
{

	if (FAILED(CGameObject::Add_Component(
		(_uint)ESceneType::Static,
		L"Component_VIBuffer_CubeTexture",
		L"Com_VIBuffer",
		(CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(
		(_uint)ESceneType::Static,
		L"Component_Texture_Terrain",
		L"Com_Texture",
		(CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}



CFakeTerrain * CFakeTerrain::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CFakeTerrain* pInstance = new CFakeTerrain(pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		PRINT_LOG(L"Error", L"Failed To CFakeTerrain (Client - CFakeTerrain.cpp)");
		Safe_Release(pInstance);

	}
	return pInstance;
}

CGameObject * CFakeTerrain::Clone(void * pArg/* = nullptr*/)
{
	CFakeTerrain* pInstance = new CFakeTerrain(*this);
	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		PRINT_LOG(L"Error", L"Failed To CFakeTerrain (Client - CFakeTerrain.cpp)");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFakeTerrain::Free()
{
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);

	CGameObject::Free();
}

