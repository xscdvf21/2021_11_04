#include "framework.h"
#include "..\Headers\UIFont.h"

#include "TestCamera.h"

CUIFont::CUIFont(_Device pDevice)
	:CGameObject(pDevice)
{
}

CUIFont::CUIFont(const CUIFont& other)
	: CGameObject(other)
{
}


HRESULT CUIFont::Ready_GameObject_Prototype()
{
	if (FAILED(CGameObject::Ready_GameObject_Prototype()))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CUIFont::Ready_GameObject(void * pArg /*= nullptr*/)
{
	if (FAILED(CGameObject::Ready_GameObject(pArg)))
		return E_FAIL;

	m_pTransformCom->Initialize_vPos(_float3(0.f, 10.f, 0.f));;
	m_pTransformCom->Initialize_fRotate(D3DXToRadian(45.f));


	if (FAILED(Add_Component()))
		return E_FAIL;

	return S_OK;
}

_uint CUIFont::Update_GameObject(_float fDeltaTime)
{
	CGameObject::Update_GameObject(fDeltaTime);

	return _uint();
}

_uint CUIFont::LateUpdate_GameObject(_float fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);

	auto pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return 0;

	if (FAILED(pManagement->Add_RenderList(ERenderPriority::UI, this)))
		return 0;

	return m_pTransformCom->Update_Transform();
}

HRESULT CUIFont::Render_GameObject()
{

	auto pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return 0;


	CTestCamera*	pCamera = dynamic_cast<CTestCamera*>(pManagement->Get_GameObject((_uint)ESceneType::Stage, L"Layer_Camera"));


	D3DXCOLOR bgColor = D3DXCOLOR(0, 0, 0, 1);
	



	pCamera->Set_Ortho_For_UI(_float3(10.f, 10.f, 1.f), _float3(10.f, 10.f, 1.f));

	//LPD3DXFONT  m_pFont = pManagement->Get_Font();
	//D3DXCreateFont(m_pDevice, 50, 50, 1000, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, L"맑은고딕", &m_pFont);

	//TCHAR szBuf[128] = L"ㅋㅋㅋㅋㅋㅋ";

	////사각형 영역 설정 , -1은 옵션에 NOCLIP을 넣었기 때문에 , 값을 주지 않은것이다

	//RECT rc = { 100, 100, -1, -1 };

	//m_pFont->DrawText(NULL, szBuf, -1, &rc, DT_LEFT | DT_NOCLIP, 0xffffff00);

	if (FAILED(m_pTransformCom->Set_Transform()))
		return E_FAIL;

	if (FAILED(CGameObject::Render_GameObject()))
		return E_FAIL;

	//if (FAILED(m_pTextureCom->Set_Texture(0)))
	//	return E_FAIL;

	if (FAILED(m_pFontCom->Set_Font(50, 50, 1000, L"zzzzzzz")))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUIFont::Add_Component()
{

	//For. Com.VIBuffer
	if (FAILED(CGameObject::Add_Component(
		(_uint)ESceneType::Static,
		L"Component_VIBuffer_Rect",
		L"Com_VIBuffer",
		(CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	//For Com.Font
	if (FAILED(CGameObject::Add_Component(
		(_uint)ESceneType::Static
		, L"Component_Font"
		, L"Com_Font"
		, (CComponent**)&m_pFontCom
	)))
		return E_FAIL;

	///*For. Com_Texture*/
	//if (FAILED(CGameObject::Add_Component(
	//	(_uint)ESceneType::Stage,
	//	L"Component_Texture_UIFont",
	//	L"Com_Texture",
	//	(CComponent**)&m_pTextureCom
	//)))
	//	return E_FAIL;




	return S_OK;
}

HRESULT CUIFont::Movement(_float fDeltaTime)
{
	return S_OK;
}

HRESULT CUIFont::IsOnTerrain()
{
	return S_OK;
}

HRESULT CUIFont::Set_Transform()
{
	return S_OK;
}

CUIFont * CUIFont::Create(_Device pDevice)
{
	CUIFont* pInstance = new CUIFont(pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		PRINT_LOG(L"Error", L"Failed To Create CUIFont");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CUIFont::Clone(void * pArg)
{
	CUIFont* pInstance = new CUIFont(*this);
	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		PRINT_LOG(L"Error", L"Failed To Create Clone CUIFont");
		Safe_Release(pInstance);
	}


	return pInstance;
}

void CUIFont::Free()
{


	CGameObject::Free();
}

