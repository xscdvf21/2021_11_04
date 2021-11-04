#include "stdafx.h"
#include "..\Headers\CubeLine.h"

CCubeLine::CCubeLine(LPDIRECT3DDEVICE9 pDevice)
	:CGameObj(pDevice)
{
}

CCubeLine::CCubeLine(const CCubeLine & other)
	:CGameObj(other)
{

}

HRESULT CCubeLine::Ready_GameObject_Prototype()
{
	if (FAILED(CGameObj::Ready_GameObject_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCubeLine::Ready_GameObject(void * pArg)
{
	if (FAILED(CGameObj::Ready_GameObject(pArg)))
	{
		PRINT_LOG(L"CCubeLine", L"CCubeLine Ready_GameObj");
		return E_FAIL;
	}

	if (FAILED(Add_Component()))
	{
		PRINT_LOG(L"CCubeLine", L"CCubeLine AddComponent");
		return E_FAIL;
	}


	

	return S_OK;
}

_uint CCubeLine::Update_GameObject(_float fDeltaTime)
{
	if (true == m_bDead)
		return OBJ_DEAD;

	CGameObj::Update_GameObject(fDeltaTime);



	if (1.5f < m_fScale)
	{
		m_fScaleAccel = -0.01f;
	}
	else if(1.f > m_fScale)
	{
		m_fScaleAccel = 0.01f;
	}

	m_fScale += m_fScaleAccel;

	m_pTransformCom->Set_Scale(_float3(m_fScale, m_fScale, m_fScale));


	return _uint();
}

_uint CCubeLine::LateUpdate_GameObject(_float fDeltaTime)
{
	CGameObj::LateUpdate_GameObject(fDeltaTime);

	auto pManagement = CManagementTool::Get_Instance();
	if (nullptr == pManagement)
		return 0;

	return m_pTransformCom->Update_Transform();
}

HRESULT CCubeLine::Render_GameObject()
{
	m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	if (FAILED(m_pTransformCom->Set_Transform()))
		return E_FAIL;

	if (FAILED(CGameObj::Render_GameObject()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
		return E_FAIL;
	m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	return S_OK;

}

HRESULT CCubeLine::Add_Component()
{
	if (FAILED(CGameObj::Add_Component(
		L"Component_VIBuffer_CubeLine",
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

CCubeLine * CCubeLine::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CCubeLine* pInstance = new CCubeLine(pDevice);
	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		PRINT_LOG(L"CCubeLine", L"CCubeLine Create 실패");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObj * CCubeLine::Clone(void * pArg)
{
	CCubeLine* pInstance = new CCubeLine(*this); /*복사생성호출*/
	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		PRINT_LOG(L"CCubeLine", L"CCubeLine Clone 실패");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCubeLine::Free()
{
	CGameObj::Free();
}

HRESULT CCubeLine::Set_ChangeColor(int _iA, int _iR, int _iG, int _iB, _uint _iIndex)
{
	if (FAILED(dynamic_cast<CVIBuffer_CubeColorTool*>(m_pVIBufferCom)
		->Change_VertexColor(D3DCOLOR_ARGB(_iA, _iR, _iG, _iB), _iIndex)))
	{
		PRINT_LOG(L"CCubeLine", L"색 변경 오류");
	}
	return S_OK;
}
