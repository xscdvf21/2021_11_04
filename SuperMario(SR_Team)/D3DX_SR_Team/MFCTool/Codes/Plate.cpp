#include "stdafx.h"
#include "..\Headers\Plate.h"

CPlate::CPlate(LPDIRECT3DDEVICE9 pDevice)
	: CGameObj(pDevice)
{

}

CPlate::CPlate(const CPlate & other)
	: CGameObj(other)
{

}

HRESULT CPlate::Ready_GameObject_Prototype()
{
	if (FAILED(CGameObj::Ready_GameObject_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlate::Ready_GameObject(void * pArg)
{
	if (FAILED(CGameObj::Ready_GameObject(pArg)))
	{
		PRINT_LOG(L"CPlate", L"판때기 Ready_GameObj");
		return E_FAIL;
	}

	if (FAILED(Add_Component()))
	{
		PRINT_LOG(L"CPlate", L"판때기 AddComponent");
		return E_FAIL;
	}
		

	return S_OK;
}

_uint CPlate::Update_GameObject(_float fDeltaTime)
{
	CGameObj::Update_GameObject(fDeltaTime);

	return _uint();
}

_uint CPlate::LateUpdate_GameObject(_float fDeltaTime)
{
	CGameObj::LateUpdate_GameObject(fDeltaTime);

	auto pManagement = CManagementTool::Get_Instance();
	if (nullptr == pManagement)
		return 0;

	return m_pTransformCom->Update_Transform();
}

HRESULT CPlate::Render_GameObject()
{
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	if (FAILED(m_pTransformCom->Set_Transform()))
		return E_FAIL;

	if (FAILED(CGameObj::Render_GameObject()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Set_Texture(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
		return E_FAIL;
	
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	return S_OK;
}

HRESULT CPlate::Add_Component()
{
	/*For.Com_VIBuffer*/
	/*if (FAILED(CGameObj::Add_Component(
		L"Component_VIBuffer_Plate",
		L"Com_VIBuffer",
		(CComponentTool**)&m_pVIBufferCom)))
		return E_FAIL;*/

	//VIBuffer
	if (FAILED(CGameObj::Add_Component(
		L"Component_VIBuffer_PlateTexture",
		L"Com_VIBuffer",
		(CComponentTool**)&m_pVIBufferCom)))
		return E_FAIL;

	//texture
	if (FAILED(CGameObj::Add_Component(
		L"Component_Texture_Plate",
		L"Com_Texture",
		(CComponentTool**)&m_pTextureCom)))
		return E_FAIL;

	/*For.Com_Transform*/
	CTransformTool::TRANSFORM_DESC TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransformTool::TRANSFORM_DESC));
	TransformDesc.vScale = { 1.f, 1.f, 1.f };
	TransformDesc.fSpeedPerSecond = 5.f; /* 초당 5만큼 움직여라 */
	TransformDesc.fRotatePerSecond = D3DXToRadian(90.f); /* 초당 90도 회전해라 */

	if (FAILED(CGameObj::Add_Component(
		L"Component_Transform",
		L"Com_Transform",
		(CComponentTool**)&m_pTransformCom,
		&TransformDesc)))
		return E_FAIL;


	return S_OK;
}

CPlate * CPlate::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CPlate* pInstance = new CPlate(pDevice);
	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		PRINT_LOG(L"CPlate", L"판때기 Create");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObj * CPlate::Clone(void * pArg)
{
	CPlate* pInstance = new CPlate(*this); /*복사생성호출*/
	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		PRINT_LOG(L"CPlate", L"판때기 Clone");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlate::Free()
{


	CGameObj::Free();
}
