#include "stdafx.h"
#include "..\Headers\BoxColor.h"



CBoxColor::CBoxColor(LPDIRECT3DDEVICE9 pDevice)
	: CGameObj(pDevice)
{
}

CBoxColor::CBoxColor(const CBoxColor & other)
	: CGameObj(other)
{

}

HRESULT CBoxColor::Ready_GameObject_Prototype()
{
	if (FAILED(CGameObj::Ready_GameObject_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBoxColor::Ready_GameObject(void * pArg)
{
	if (FAILED(CGameObj::Ready_GameObject(pArg)))
	{
		PRINT_LOG(L"CBoxColor", L"CBoxColor Ready_GameObj");
		return E_FAIL;
	}

	if (FAILED(Add_Component()))
	{
		PRINT_LOG(L"CBoxColor", L"CBoxColor AddComponent");
		return E_FAIL;
	}


	return S_OK;
}

_uint CBoxColor::Update_GameObject(_float fDeltaTime)
{
	if (true == m_bDead)
		return OBJ_DEAD;
	CGameObj::Update_GameObject(fDeltaTime);

	return _uint();
}

_uint CBoxColor::LateUpdate_GameObject(_float fDeltaTime)
{
	CGameObj::LateUpdate_GameObject(fDeltaTime);

	auto pManagement = CManagementTool::Get_Instance();
	if (nullptr == pManagement)
		return 0;

	return m_pTransformCom->Update_Transform();
}

HRESULT CBoxColor::Render_GameObject()
{


	if (FAILED(m_pTransformCom->Set_Transform()))
		return E_FAIL;

	if (FAILED(CGameObj::Render_GameObject()))
		return E_FAIL;


	if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
		return E_FAIL;


	return S_OK;
}

HRESULT CBoxColor::Add_Component()
{
	if (FAILED(CGameObj::Add_Component(
		L"Component_VIBuffer_CubeColor",
		L"Com_VIBuffer",
		(CComponentTool**)&m_pVIBufferCom)))
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

CBoxColor * CBoxColor::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CBoxColor* pInstance = new CBoxColor(pDevice);
	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		PRINT_LOG(L"CBoxColor", L"CBoxColor Create 실패");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObj * CBoxColor::Clone(void * pArg)
{
	CBoxColor* pInstance = new CBoxColor(*this); /*복사생성호출*/
	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		PRINT_LOG(L"CBoxColor", L"CBoxColor Clone 실패");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBoxColor::Free()
{
	CGameObj::Free();

}


