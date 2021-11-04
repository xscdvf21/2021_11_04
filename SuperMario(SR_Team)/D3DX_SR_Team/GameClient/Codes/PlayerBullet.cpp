#include "framework.h"
#include "..\Headers\PlayerBullet.h"





CPlayerBullet::CPlayerBullet(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject(pDevice)
{
}

CPlayerBullet::CPlayerBullet(const CPlayerBullet & other)
	:CGameObject(other)
{
}

HRESULT CPlayerBullet::Ready_GameObject_Prototype()
{
	if (FAILED(CGameObject::Ready_GameObject_Prototype()))
		return E_FAIL;



	return S_OK;
}

HRESULT CPlayerBullet::Ready_GameObject(void * pArg /*= nullptr*/)
{
	if (FAILED(CGameObject::Ready_GameObject(pArg)))
		return E_FAIL;

	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pTransformCom->Initialize_vScale(_float3(0.5f, 0.5f, 0.5f));


	return S_OK;
}

_uint CPlayerBullet::Update_GameObject(_float fDeltaTime)
{
	auto pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return 0;
	CGameObject::Update_GameObject(fDeltaTime);


	return _uint();
}

_uint CPlayerBullet::LateUpdate_GameObject(_float fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);

	auto pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return 0;

	if (FAILED(pManagement->Add_RenderList(ERenderPriority::Alpha, this)))
		return 0;

	m_pLifeTime += fDeltaTime;
	if (m_pLifeTime >= 0.3f)
	{
		bIsDead = true;
	}

	return m_pTransformCom->Update_Transform();

}

HRESULT CPlayerBullet::Render_GameObject()
{
	if (FAILED(m_pTransformCom->Set_Transform()))
		return E_FAIL;

	if (FAILED(CGameObject::Render_GameObject()))
		return E_FAIL;

	//if (FAILED(m_pTextureCom->Set_Texture(0)))
	//	return E_FAIL;


	//if (FAILED(m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE)))
	//	return E_FAIL;
	//if (FAILED(m_pDevice->SetRenderState(D3DRS_ALPHAREF, 1)))
	//	return E_FAIL;
	//if (FAILED(m_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER)))
	//	return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
		return E_FAIL;

	//if (FAILED(m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE)))
	//	return E_FAIL;


	return S_OK;
}

HRESULT CPlayerBullet::Add_Component()
{

	if (FAILED(CGameObject::Add_Component(
		(_uint)ESceneType::Static,
		L"Component_VIBuffer_RectTexture",
		L"Com_VIBuffer",
		(CComponent**)&m_pVIBufferCom)))
		return E_FAIL;



	return S_OK;
}

CPlayerBullet * CPlayerBullet::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CPlayerBullet*	pInstance = new CPlayerBullet(pDevice);
	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		PRINT_LOG(L"Error", L"플레이어 프로토타입 생성하다가 실패, CPlayerBullet.cpp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CPlayerBullet::Clone(void * pArg)
{
	CPlayerBullet*	pInstance = new CPlayerBullet(*this);
	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		PRINT_LOG(L"Error", L"플레이어 클론 생성하다가 실패, CPlayerBullet.cpp");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPlayerBullet::Free()
{
	CGameObject::Free();
}

