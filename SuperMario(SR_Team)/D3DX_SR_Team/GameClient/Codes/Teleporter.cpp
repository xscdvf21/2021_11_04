#include "framework.h"
#include "..\Headers\Teleporter.h"
#include "Management.h"


CTeleporter::CTeleporter(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject(pDevice)
{
}

CTeleporter::CTeleporter(const CTeleporter & other)
	: CGameObject(other)
{
}

HRESULT CTeleporter::Ready_GameObject_Prototype()
{
	if (FAILED(CGameObject::Ready_GameObject_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTeleporter::Ready_GameObject(void * pArg /*= nullptr*/)
{
	if (FAILED(CGameObject::Ready_GameObject(pArg)))
		return E_FAIL;

	m_eSceneType = (_uint)ESceneType::Static;

	if (FAILED(Add_Component()))
		return E_FAIL;

	return S_OK;
}

_uint CTeleporter::Update_GameObject(_float fDeltaTime)
{
	CGameObject::Update_GameObject(fDeltaTime);
	//LayerTag�� ����� ã�Ƽ� ����� ��ġ�� �÷��̾�� �ѱ��, 
	//�÷��̾��� m_bEvent�� true�� �ٲٰ�
	//�ڷ����Ϳ� �ڽ��� ��ġ ������ �ѱ�� �ڽ��� m_bEvent�� true��

	if (m_bEvent)
	{
		auto pManagement = CManagement::Get_Instance();
		if (nullptr == pManagement)
			return E_FAIL;

		//LayerTag��?

		pManagement->Get_GameObject((_uint)ESceneType::TestStage, L"Layer_Player")
			->Set_ObjPosition(m_vPosition); 
		// ����� ��ġ�� �÷��̾����� �ѱ�ٴ°� �÷��̾��� ��ġ�� �缳���ȴٴ� �̾߱��ΰ�?

		pManagement->Get_GameObject((_uint)ESceneType::TestStage, L"Layer_Player")
			->Set_Event(true);

		m_bEvent = false;
	}
	

	return _uint();
}

_uint CTeleporter::LateUpdate_GameObject(_float fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);

	auto pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return 0;

	if (FAILED(pManagement->Add_RenderList(ERenderPriority::NoAlpha, this)))
		return 0;

	return m_pTransformCom->Update_Transform();
}

HRESULT CTeleporter::Render_GameObject()
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

HRESULT CTeleporter::Add_Component()
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



CTeleporter * CTeleporter::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CTeleporter* pInstance = new CTeleporter(pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		PRINT_LOG(L"Error", L"Failed To CFakeTerrain (Client - CFakeTerrain.cpp)");
		Safe_Release(pInstance);

	}
	return pInstance;
}

CGameObject * CTeleporter::Clone(void * pArg/* = nullptr*/)
{
	CTeleporter* pInstance = new CTeleporter(*this);
	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		PRINT_LOG(L"Error", L"Failed To CFakeTerrain (Client - CFakeTerrain.cpp)");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTeleporter::Free()
{
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);

	CGameObject::Free();
}

