#include "stdafx.h"
#include "..\Headers\CubeTexture.h"

CCubeTexture::CCubeTexture(LPDIRECT3DDEVICE9 pDevice)
	: CGameObj(pDevice)
{
}

CCubeTexture::CCubeTexture(const CCubeTexture & other)
	: CGameObj(other)
{

}

HRESULT CCubeTexture::Ready_GameObject_Prototype()
{
	if (FAILED(CGameObj::Ready_GameObject_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCubeTexture::Ready_GameObject(void * pArg)
{
	if (FAILED(CGameObj::Ready_GameObject(pArg)))
	{
		PRINT_LOG(L"CCubeColor", L"CCubeColor Ready_GameObj");
		return E_FAIL;
	}

	if (FAILED(Add_Component()))
	{
		PRINT_LOG(L"CCubeColor", L"CCubeColor AddComponent");
		return E_FAIL;
	}


	return S_OK;
}

_uint CCubeTexture::Update_GameObject(_float fDeltaTime)
{
	if (true == m_bDead)
		return OBJ_DEAD;
	CGameObj::Update_GameObject(fDeltaTime);

	return _uint();
}

_uint CCubeTexture::LateUpdate_GameObject(_float fDeltaTime)
{
	CGameObj::LateUpdate_GameObject(fDeltaTime);

	auto pManagement = CManagementTool::Get_Instance();
	if (nullptr == pManagement)
		return 0;

	return m_pTransformCom->Update_Transform();
}

HRESULT CCubeTexture::Render_GameObject()
{
	

	if (FAILED(m_pTransformCom->Set_Transform()))
		return E_FAIL;

	if (FAILED(CGameObj::Render_GameObject()))
		return E_FAIL;

	if (nullptr != m_pTextureCom)
	{
		if (FAILED(m_pTextureCom->Set_Texture(m_iTextuer)))
			return E_FAIL;
	}
	
	if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
		return E_FAIL;


	return S_OK;
}

HRESULT CCubeTexture::Add_Component()
{
	if (FAILED(CGameObj::Add_Component(
		L"Component_VIBuffer_CubeTexture",
		L"Com_VIBuffer",
		(CComponentTool**)&m_pVIBufferCom)))
		return E_FAIL;

	
	//if (FAILED(CGameObj::Add_Component(
	//	L"Component_Texture_CubeTexture",
	//	L"Com_Texture",
	//	(CComponentTool**)&m_pTextureCom)))
	//	return E_FAIL;

	if (FAILED(CGameObj::Add_Component(
		L"Component_Texture_Terrain",
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

CCubeTexture * CCubeTexture::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CCubeTexture* pInstance = new CCubeTexture(pDevice);
	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		PRINT_LOG(L"CCubeTexture", L"CCubeTexture Create 실패");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObj * CCubeTexture::Clone(void * pArg)
{
	CCubeTexture* pInstance = new CCubeTexture(*this); /*복사생성호출*/
	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		PRINT_LOG(L"CCubeTexture", L"CCubeTexture Clone 실패");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCubeTexture::Free()
{
	CGameObj::Free();

}

HRESULT CCubeTexture::ImageClone(wstring _strProtoTypeTag, int _imageCount)
{
	m_iTextuer = _imageCount;

	if (FAILED(CGameObj::Add_Component(
		_strProtoTypeTag,
		L"Com_Texture",
		(CComponentTool**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}




