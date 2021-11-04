
#include "framework.h"
#include "..\Headers\TriggerBox.h"
#include "Management.h"

#include "Player.h"

CTriggerBox::CTriggerBox(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject(pDevice)
{
}

CTriggerBox::CTriggerBox(const CTriggerBox & other)
	: CGameObject(other)
{
}

HRESULT CTriggerBox::Ready_GameObject_Prototype()
{
	if (FAILED(CGameObject::Ready_GameObject_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTriggerBox::Ready_GameObject(void * pArg /*= nullptr*/)
{
	if (FAILED(CGameObject::Ready_GameObject(pArg)))
		return E_FAIL;

	m_eSceneType = (_uint)ESceneType::Static;

	if (FAILED(Add_Component()))
		return E_FAIL;

	return S_OK;
}

_uint CTriggerBox::Update_GameObject(_float fDeltaTime)
{
	CGameObject::Update_GameObject(fDeltaTime);

	if (m_bEvent)
	{
		auto pManagement = CManagement::Get_Instance();
		if (nullptr == pManagement)
			return E_FAIL;

		pManagement->Get_GameObject((_uint)ESceneType::TestStage, L"Layer_Player")
			->Set_Event(true);
		// �׽�Ʈ������������ ����Ǵ°� �ƴ��ٵ�......
		// ������������� �۵��ɰ��� �˼��ֳ�?
	}


	return _uint();
}

_uint CTriggerBox::LateUpdate_GameObject(_float fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);

	auto pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return 0;

	if (FAILED(pManagement->Add_RenderList(ERenderPriority::NoAlpha, this)))
		return 0;

	return m_pTransformCom->Update_Transform();
}

HRESULT CTriggerBox::Render_GameObject()
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

HRESULT CTriggerBox::Add_Component()
{
	if (FAILED(CGameObject::Add_Component(
		(_uint)ESceneType::Static,
		L"Component_VIBuffer_CubeInvisible",
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



CTriggerBox * CTriggerBox::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CTriggerBox* pInstance = new CTriggerBox(pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		PRINT_LOG(L"Error", L"Failed To CTriggerBox (Client - CTriggerBox.cpp)");
		Safe_Release(pInstance);

	}
	return pInstance;
}

CGameObject * CTriggerBox::Clone(void * pArg/* = nullptr*/)
{
	CTriggerBox* pInstance = new CTriggerBox(*this);
	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		PRINT_LOG(L"Error", L"Failed To CTriggerBox (Client - CTriggerBox.cpp)");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTriggerBox::Free()
{
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);

	CGameObject::Free();
}