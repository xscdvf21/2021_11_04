#include "framework.h"
#include "FlipPuzzle.h"
#include "Management.h"

CFlipPuzzle::CFlipPuzzle(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject(pDevice)
{

}

CFlipPuzzle::CFlipPuzzle(const CFlipPuzzle & other)
	: CGameObject(other)
{

}

HRESULT CFlipPuzzle::Ready_GameObject_Prototype()
{
	if (FAILED(CGameObject::Ready_GameObject_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CFlipPuzzle::Ready_GameObject(void * pArg)
{
	if (FAILED(CGameObject::Ready_GameObject(pArg)))
		return E_FAIL;

	if (FAILED(Add_Component()))
		return E_FAIL;

	return S_OK;
}

_uint CFlipPuzzle::Update_GameObject(_float fDeltaTime)
{
	CGameObject::Update_GameObject(fDeltaTime);
	return _uint();
}

_uint CFlipPuzzle::LateUpdate_GameObject(_float fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);

	auto pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return 0;

	if (FAILED(pManagement->Add_RenderList(ERenderPriority::NoAlpha, this)))
		return 0;

	return m_pTransformCom->Update_Transform();
}

HRESULT CFlipPuzzle::Render_GameObject()
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

HRESULT CFlipPuzzle::Add_Component()
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

CFlipPuzzle * CFlipPuzzle::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CFlipPuzzle* pInstance = new CFlipPuzzle(pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		PRINT_LOG(L"CFlipPuzzle", L"플립퍼즐 prototype 실패");
		Safe_Release(pInstance);

	}
	return pInstance;
}

CGameObject * CFlipPuzzle::Clone(void * pArg)
{
	CFlipPuzzle* pInstance = new CFlipPuzzle(*this);
	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		PRINT_LOG(L"CFlipPuzzle", L"플립퍼즐 클론 실패.");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFlipPuzzle::Free()
{
	CGameObject::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);

}
