
#include "framework.h"
#include "..\Headers\Tree.h"
#include "Management.h"



CTree::CTree(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject(pDevice)
{
}

CTree::CTree(const CTree & other)
	: CGameObject(other)
{
}

HRESULT CTree::Ready_GameObject_Prototype()
{
	if (FAILED(CGameObject::Ready_GameObject_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTree::Ready_GameObject(void * pArg /*= nullptr*/)
{
	if (FAILED(CGameObject::Ready_GameObject(pArg)))
		return E_FAIL;

	m_eSceneType = (_uint)ESceneType::Static;

	if (FAILED(Add_Component()))
		return E_FAIL;

	return S_OK;
}

_uint CTree::Update_GameObject(_float fDeltaTime)
{
	CGameObject::Update_GameObject(fDeltaTime);


	return _uint();
}

_uint CTree::LateUpdate_GameObject(_float fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);

	auto pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return 0;

	if (FAILED(pManagement->Add_RenderList(ERenderPriority::NoAlpha, this)))
		return 0;

	return m_pTransformCom->Update_Transform();
}

HRESULT CTree::Render_GameObject()
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

HRESULT CTree::Add_Component()
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



CTree * CTree::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CTree* pInstance = new CTree(pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		PRINT_LOG(L"Error", L"Failed To CTree (Client - CTree.cpp)");
		Safe_Release(pInstance);

	}
	return pInstance;
}

CGameObject * CTree::Clone(void * pArg/* = nullptr*/)
{
	CTree* pInstance = new CTree(*this);
	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		PRINT_LOG(L"Error", L"Failed To CTree (Client - CTree.cpp)");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTree::Free()
{
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);

	CGameObject::Free();
}