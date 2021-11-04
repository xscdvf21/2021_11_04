#include "framework.h"
#include "FireTriger.h"
#include "Management.h"

CFireTriger::CFireTriger(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject(pDevice)
{

}

CFireTriger::CFireTriger(const CFireTriger & other)
	:CGameObject(other)
{

}

HRESULT CFireTriger::Ready_GameObject_Prototype()
{
	if (FAILED(CGameObject::Ready_GameObject_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CFireTriger::Ready_GameObject(void * pArg)
{
	



	if (FAILED(CGameObject::Ready_GameObject(pArg)))
		return E_FAIL;

	if (FAILED(Add_Component()))
		return E_FAIL;

	return S_OK;


}

_uint CFireTriger::Update_GameObject(_float fDeltaTime)
{
	CGameObject::Update_GameObject(fDeltaTime);
	return _uint();
}

_uint CFireTriger::LateUpdate_GameObject(_float fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);

	auto pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return 0;

	if (FAILED(pManagement->Add_RenderList(ERenderPriority::NoAlpha, this)))
		return 0;

	return m_pTransformCom->Update_Transform();
}

HRESULT CFireTriger::Render_GameObject()
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

HRESULT CFireTriger::Add_Component()
{

	return S_OK;
}

HRESULT CFireTriger::Movement(_float fDeltaTime)
{
	return E_NOTIMPL;
}

HRESULT CFireTriger::IsOnTerrain()
{
	return E_NOTIMPL;
}

HRESULT CFireTriger::Set_Transform()
{
	return E_NOTIMPL;
}

CFireTriger * CFireTriger::Create(LPDIRECT3DDEVICE9 pDevice)
{
	return nullptr;
}

CGameObject * CFireTriger::Clone(void * pArg)
{
	return nullptr;
}

void CFireTriger::Free()
{
	CGameObject::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);

}
