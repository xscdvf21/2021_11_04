#include "stdafx.h"
#include "LogoBack.h"

#include "Export_Fucntion.h"


CLogoBack::CLogoBack(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{

}

CLogoBack::~CLogoBack(void)
{

}


HRESULT CLogoBack::Ready_Object(void)
{

	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Logo_Iru",
		Engine::MESH_DYNAMIC,
		L"../../Resource/Mesh/DynamicMesh/Iru/",
		L"Black_Iru.x"),
		E_FAIL);


	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->m_vScale.x = 2.f;
	m_pTransformCom->m_vScale.y = 2.f;

	//m_pTransformCom->Set_Pos(_vec3(0.f, 0.f, 1.f));


	return S_OK;
}

Engine::_int CLogoBack::Update_Object(const _float& fTimeDelta)
{



	Engine::CGameObject::Update_Object(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_PRIORITY, this);

	return 0;
}

void CLogoBack::Render_Object(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	

	m_pTextureCom->Set_Texture();
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
}

CLogoBack* CLogoBack::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CLogoBack*	pInstance = new CLogoBack(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CLogoBack::Free(void)
{
	Engine::CGameObject::Free();
}

HRESULT CLogoBack::Add_Component(void)
{

	Engine::CComponent*		pComponent = nullptr;

	// Buffer
	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Buffer", pComponent);

	// Texture
	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone_Resource(RESOURCE_STATIC, L"Texture_Logoback"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture", pComponent);

	// Renderer
	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Renderer", pComponent);

	// Transform
	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);
	
	return S_OK;
}

