#include "framework.h"
#include "..\Headers\StageUI.h"
#include "TestCamera.h"

#include "StageCoin.h"
#include "StageHP.h"
#include "PaperUI.h"

CStageUI::CStageUI(_Device _pDevice)
	:CGameObject(_pDevice)
{
}

CStageUI::CStageUI(const CStageUI & other)
	:CGameObject(other)
{
}

HRESULT CStageUI::Ready_GameObject_Prototype()
{
	if (FAILED(CGameObject::Ready_GameObject_Prototype()))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CStageUI::Ready_GameObject(void * pArg)
{
	if (FAILED(CGameObject::Ready_GameObject(pArg)))
	{
		return E_FAIL;
	}

	m_pTransformCom->Initialize_vPos(_float3(3.f, 3.f, 3.f));
	m_pTransformCom->Initialize_vScale(_float3(1.f, 1.f, 1.f));


	if (FAILED(Add_Component()))
	{
		return E_FAIL;
	}

	m_pCoin_UI = CStageCoin::Create(m_pDevice);
	m_pCoin_UI->Ready_GameObject();
	Safe_AddReference(m_pCoin_UI);

	//m_pHp_UI = CStageHP::Create(m_pDevice);
	//m_pHp_UI->Ready_GameObject();
	//Safe_AddReference(m_pHp_UI);

	//m_pPaper_UI = CPaperUI::Create(m_pDevice);
	//m_pPaper_UI->Ready_GameObject();
	//Safe_AddReference(m_pPaper_UI);

	return S_OK;
}

_uint CStageUI::Update_GameObject(_float fDeltaTime)
{
	CGameObject::Update_GameObject(fDeltaTime);

	m_pCoin_UI->Update_GameObject(fDeltaTime);
	//m_pHp_UI->Update_GameObject(fDeltaTime);
	//m_pPaper_UI->Update_GameObject(fDeltaTime);
	return _uint();
}

_uint CStageUI::LateUpdate_GameObject(_float fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);

	m_pCoin_UI->LateUpdate_GameObject(fDeltaTime);
	//m_pHp_UI->LateUpdate_GameObject(fDeltaTime);
	//m_pPaper_UI->LateUpdate_GameObject(fDeltaTime);

	auto pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
	{
		return 0;
	}

	if (FAILED(pManagement->Add_RenderList(ERenderPriority::UI, this)))
	{
		return 0;
	}

	return m_pTransformCom->Update_Transform();
}

HRESULT CStageUI::Render_GameObject()
{

	auto pManagement = CManagement::Get_Instance();

	if (nullptr == pManagement)
		return E_FAIL;

	CTestCamera* pCamera = dynamic_cast<CTestCamera*>(pManagement->Get_GameObject((_uint)ESceneType::TestStage, L"Layer_Camera"));

	pCamera->Set_Ortho_For_UI(_float3(10.f, 10.f, 1.f), _float3(10.f, 10.f, 1.f));

	if (FAILED(m_pTransformCom->Set_Transform()))
		return E_FAIL;

	if (FAILED(CGameObject::Render_GameObject()))
		return E_FAIL;

	//if (FAILED(m_pTextureCom->Set_Texture(0)))
	//	return E_FAIL;

	//if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
	//	return E_FAIL;

	m_pCoin_UI->Render_GameObject();
	//m_pHp_UI->Render_GameObject();
	//m_pPaper_UI->Render_GameObject();

	return S_OK;
}

HRESULT CStageUI::Add_Component()
{
	/*For.Com_VIBuffer*/
	if (FAILED(CGameObject::Add_Component(
		(_uint)ESceneType::Static,
		L"Component_VIBuffer_RectTexture",
		L"Com_VIBuffer",
		(CComponent**)&m_pVIBufferCom)))
		return E_FAIL;


	/*For.Com_Transform*/
	CTransform::TRANSFORM_DESC TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORM_DESC));
	TransformDesc.vPos = { 1.f, 2.f, 5.f };
	TransformDesc.vScale = { 10.f, 10.f, 1.f };
	TransformDesc.fSpeed = 0.f; /* �ʴ� 5��ŭ �������� */
	TransformDesc.fRotate = D3DXToRadian(0.f);

	if (FAILED(CGameObject::Add_Component(
		(_uint)ESceneType::Static,
		L"Component_Transform",
		L"Com_Transform",
		(CComponent**)&m_pTransformComp,
		&TransformDesc)))
		return E_FAIL;

	return S_OK;

}

HRESULT CStageUI::Set_Transform()
{
	auto pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;



	CTestCamera*	pCamera = dynamic_cast<CTestCamera*>(pManagement->Get_GameObject((_uint)ESceneType::Stage, L"Layer_Camera"));

	_mat	matWorld = pCamera->Get_CameraDesc().matView;


	D3DXMatrixInverse(&matWorld, NULL, &matWorld);

	_vec3 temp = { matWorld._31, matWorld._32, matWorld._33 };

	//CStageUI*	pStageUI = dynamic_cast<CStageUI*>(pManagement->Get_GameObject((_uint)ESceneType::Stage, L"Layer_UI"));
	//m_pTransformCom->Set_Position(temp);
	matWorld._41 += temp.x;
	matWorld._42 += temp.y;
	matWorld._43 += temp.z;

	//최종 행렬 세팅

	m_pTransformCom->Set_WorldMatrix(matWorld);

	
	return S_OK;
}

CStageUI * CStageUI::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CStageUI*	pInstance = new CStageUI(pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		PRINT_LOG(L"Error", L"Failed To Create CStageUI");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CStageUI::Clone(void * pArg)
{
	CStageUI* pInstance = new CStageUI(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		PRINT_LOG(L"Error", L"Failed To Create CStageUI Clone");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CStageUI::Free()
{
	//Safe_Release(m_pCoin_UI);
	//Safe_Release(m_pHp_UI);
	//Safe_Release(m_pPaper_UI);

	//Safe_Delete(m_pCoin_UI);
	//Safe_Delete(m_pHp_UI);
	//Safe_Delete(m_pPaper_UI);


	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);

	CGameObject::Free();
}
