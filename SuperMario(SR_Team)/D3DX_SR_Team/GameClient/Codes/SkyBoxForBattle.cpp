#include "framework.h"
#include "SkyBoxForBattle.h"
#include "BattleCamera.h"

CSkyBoxForBattle::CSkyBoxForBattle(_Device _pDevice)
	:CGameObject(_pDevice)
{
}

CSkyBoxForBattle::CSkyBoxForBattle(const CSkyBoxForBattle& other)
	: CGameObject(other)
{
	
}

HRESULT CSkyBoxForBattle::Ready_GameObject_Prototype()
{
	if (FAILED(CGameObject::Ready_GameObject_Prototype()))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CSkyBoxForBattle::Ready_GameObject(void* pArg)
{
	if (FAILED(CGameObject::Ready_GameObject(pArg)))
	{
		return E_FAIL;
	}

	m_pTransformCom->Initialize_vScale(_float3(50.f, 50.f, 50.f));

	if (FAILED(Add_Component()))
	{
		return E_FAIL;
	}

	return S_OK;
}

_uint CSkyBoxForBattle::Update_GameObject(_float fDeltaTime)
{
	CGameObject::Update_GameObject(fDeltaTime);

	if (FAILED(Movement(fDeltaTime)))
	{
		return 0;
	}
	return _uint();
}

_uint CSkyBoxForBattle::LateUpdate_GameObject(_float fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);

	auto pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
	{
		return 0;
	}

	if (FAILED(pManagement->Add_RenderList(ERenderPriority::Priority, this)))
	{
		return 0;
	}


	return m_pTransformCom->Update_Transform();
}

HRESULT CSkyBoxForBattle::Render_GameObject()
{
	if (FAILED(m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE)))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Set_Transform()))
		return E_FAIL;

	if (FAILED(CGameObject::Render_GameObject()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Set_Texture(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
		return E_FAIL;

	if (FAILED(m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE)))
		return E_FAIL;

	return S_OK;
}

HRESULT CSkyBoxForBattle::Add_Component()
{

	/*For.Com_VIBuffer*/
	if (FAILED(CGameObject::Add_Component(
		(_uint)ESceneType::Static,
		L"Component_VIBuffer_CubeTexture",
		L"Com_VIBuffer",
		(CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/*For.Com_Texture*/
	if (FAILED(CGameObject::Add_Component(
		(_uint)ESceneType::Static,
		L"Component_Texture_Skybox_Battle",
		L"Com_Texture",
		(CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CSkyBoxForBattle::Movement(_float fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	//테스트 카메라 세팅
	CBattleCamera* pCamera = (CBattleCamera*)pManagement->Get_GameObject((_uint)ESceneType::Battle, L"Layer_Camera");
	if (nullptr == pCamera)
		return E_FAIL;

	//
	m_pTransformCom->Set_Position(pCamera->Get_CameraDesc().vEye);

	return S_OK;
}

CSkyBoxForBattle* CSkyBoxForBattle::Create(_Device _pDevice)
{
	CSkyBoxForBattle* pInstance = new CSkyBoxForBattle(_pDevice);
	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		PRINT_LOG(L"Error", L"Failed To Create SkyboxB (Client SkyboxBattle.cpp)");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSkyBoxForBattle::Clone(void* pArg)
{
	CSkyBoxForBattle* pInstance = new CSkyBoxForBattle(*this);
	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		PRINT_LOG(L"Error", L"Failed To Clone SkyboxB (Client SkyboxBattle.cpp)");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSkyBoxForBattle::Free()
{
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);

	CGameObject::Free();
}

