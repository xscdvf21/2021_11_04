#include "framework.h"
#include "..\Headers\GressTree.h"





CGressTree::CGressTree(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject(pDevice)
{
}

CGressTree::CGressTree(const CGressTree & other)
	: CGameObject(other)
{
}

HRESULT CGressTree::Ready_GameObject_Prototype()
{
	if (FAILED(CGameObject::Ready_GameObject_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CGressTree::Ready_GameObject(void * pArg)
{
	if (FAILED(CGameObject::Ready_GameObject(pArg)))
		return E_FAIL;

	if (FAILED(Add_Component()))
		return E_FAIL;


	switch (m_iTextureIndex)
	{
	case GressTree::Gress:
		break;
	case GressTree::Tree:
		m_pTransformCom->Set_Scale(_float3(5.f, 5.f, 5.f));
		break;

	}

	return S_OK;
}

_uint CGressTree::Update_GameObject(_float fDeltaTime)
{
	CGameObject::Update_GameObject(fDeltaTime);

	return _uint();
}

_uint CGressTree::LateUpdate_GameObject(_float fDeltaTime)
{
	auto pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return 0;


	if (FAILED(pManagement->Add_RenderList(ERenderPriority::Alpha, this)))
		return 0;

	return m_pTransformCom->Update_Transform();

}

HRESULT CGressTree::Render_GameObject()
{
	if (FAILED(m_pTransformCom->Set_Transform()))
		return E_FAIL;

	if (FAILED(CGameObject::Render_GameObject()))
		return E_FAIL;

	if (FAILED(m_pTextureCom[m_iTextureIndex]->Set_Texture(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
		return E_FAIL;
}

HRESULT CGressTree::Add_Component()
{
		if (FAILED(CGameObject::Add_Component(
			(_uint)ESceneType::Static,
			L"Component_VIBuffer_RectTexture",
			L"Com_VIBuffer",
			(CComponent**)&m_pVIBufferCom
		)))
		{
			return E_FAIL;
		}


		if (FAILED(CGameObject::Add_Component(
			(_uint)ESceneType::Static,
			L"Component_Texture_Gress",
			L"Com_Texture",
			(CComponent**)&m_pTextureCom[GressTree::Gress]
		)))
		{
			return E_FAIL;
		}

	
		if (FAILED(CGameObject::Add_Component(
			(_uint)ESceneType::Static,
			L"Component_Texture_Tree",
			L"Com_Texture",
			(CComponent**)&m_pTextureCom[GressTree::Tree]
		)))
		{
			return E_FAIL;
		}


}

CGressTree * CGressTree::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CGressTree*		pInstance = new CGressTree(pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		PRINT_LOG(L"Error", L"CGressTree 프로토타입 생성 실패.");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CGressTree::Clone(void * pArg)
{
	CGressTree*		pInstance = new CGressTree(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		PRINT_LOG(L"Error", L"CGressTree 클론 생성 실패.");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGressTree::Free()
{
	CGameObject::Free();
}
