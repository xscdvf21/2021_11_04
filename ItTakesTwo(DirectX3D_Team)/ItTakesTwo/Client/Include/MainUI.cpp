#include "stdafx.h"
#include "MainUI.h"

#include "Export_Function.h"

CMainUI::CMainUI(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CMainUI::~CMainUI()
{
}

HRESULT CMainUI::Ready_Object(void * pArg)
{
	FAILED_CHECK_RETURN(Add_Component(pArg), E_FAIL);

	GUI Temp;
	if (pArg)
	{
		memcpy(&Temp, pArg, sizeof(GUI));
	}

	m_fX = Temp.PosXY.x;
	m_fY = Temp.PosXY.y;

	m_fSizeX = Temp.SizeXY.x;
	m_fSizeY = Temp.SizeXY.y;

	lstrcpy(m_tagTexture, Temp.tagTexture);

	m_iRenderState = Temp.iRender;

	m_fAlpha = 0.f;

	return S_OK;
}

_int CMainUI::Update_Object(const _float & TimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.f, 1.f);

	Engine::CGameObject::Update_Object(TimeDelta);

	if (m_iRenderState == 0)
		Engine::Add_RenderGroup(Engine::RENDER_PRIORITY, this);
	else if (m_iRenderState == 1)
		Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);
	else if (m_iRenderState == 2)
		Engine::Add_RenderGroup(Engine::RENDER_ALPHA, this);
	else if (m_iRenderState == 3)
		Engine::Add_RenderGroup(Engine::RENDER_UI, this);
	else if (m_iRenderState == 4)
		Engine::Add_RenderGroup(Engine::RENDER_MENUUI, this);
	else if (m_iRenderState == 5)
		Engine::Add_RenderGroup(Engine::RENDER_PRIUI, this);

	return OBJ_NOEVENT;
}

_int CMainUI::LateUpdate_Object(const _float & TimeDelta)
{
	return _int();
}

void CMainUI::Render_Object(const _int & iIndex)
{
	_matrix		matView, matOldView, matOldProj;

	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&matView);

	m_matWorld._11 = m_fSizeX;
	m_matWorld._22 = m_fSizeY;
	m_matWorld._33 = 1.f;
	m_matWorld._41 = m_fX - WINCX * 0.5f;
	m_matWorld._42 = -m_fY + WINCY * 0.5f;

	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect), );

	_uint	iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);
	pEffect->BeginPass(0);

	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);
}

HRESULT CMainUI::Add_Component(void * pArg)
{
	Engine::CComponent*		pComponent = nullptr;

	GUI Temp;
	if (pArg)
	{
		memcpy(&Temp, pArg, sizeof(GUI));
	}

	// Buffer
	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Buffer", pComponent);

	// Texture
	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone_Resource(RESOURCE_STATIC, Temp.tagTexture));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture", pComponent);

	// Transform
	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	// Shader
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_UI"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);

	return S_OK;
}

HRESULT CMainUI::SetUp_ConstantTable(LPD3DXEFFECT & pEffect)
{
	_matrix			matView;
	D3DXMatrixIdentity(&matView);

	//m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

	pEffect->SetMatrix("g_matWorld", &m_matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &m_matProj);
	pEffect->SetFloat("g_Alpha", m_fAlpha);

	m_pTextureCom->Set_Texture(pEffect, "g_BaseTexture", 0);

	return S_OK;
}

CMainUI * CMainUI::Create(LPDIRECT3DDEVICE9 pGraphicDev, void * pArg)
{
	CMainUI*	pInstance = new CMainUI(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CMainUI::Free(void)
{
	Engine::CGameObject::Free();
}
