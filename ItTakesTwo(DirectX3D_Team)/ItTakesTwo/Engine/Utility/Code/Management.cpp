#include "Export_Utility.h"
#include "Particle_Manager.h"

USING(Engine)
IMPLEMENT_SINGLETON(CManagement)

Engine::CManagement::CManagement(void)
	: m_pScene(nullptr)
{
}

Engine::CManagement::~CManagement(void)
{
	Free();
}

HRESULT Engine::CManagement::Set_Scene(CScene* pScene, LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr != m_pScene)
		Safe_Release(m_pScene);

	Get_Renderer()->Clear_RenderGroup();

	//Get_Renderer()->Set_Device(pDevice);

	m_pScene = pScene;

	return S_OK;
}


Engine::_int Engine::CManagement::Update_Scene(const _float& fTimeDelta)
{
	if (nullptr == m_pScene)
		return -1;

	CParticle_Manager::GetInstance()->Update_Particle(fTimeDelta);
	m_pScene->Update_Scene(fTimeDelta);

	return 0;
}

_int CManagement::LateUpdate_Scene(const _float & fTimeDelta)
{
	if (nullptr == m_pScene)
		return -1;

	CParticle_Manager::GetInstance()->LateUpdate_Particle();
	m_pScene->LateUpdate_Scene(fTimeDelta);

	return _int();
}

void Engine::CManagement::Render_Scene(const _float& fTimeDelta, const _vec3& vBackBuffer)
{
	Get_Renderer()->Render_GameObject(fTimeDelta, vBackBuffer);

	if (nullptr == m_pScene)
	return;

	m_pScene->Render_Scene();
}

void Engine::CManagement::Free(void)
{
	Safe_Release(m_pScene);
}

CComponent* Engine::CManagement::Get_Component(const _tchar* pLayerTag, const _tchar* pObjTag, const _tchar* pComponentTag, COMPONENTID eID)
{
	if (nullptr == m_pScene)
		return nullptr;

	return m_pScene->Get_Component(pLayerTag, pObjTag, pComponentTag, eID);
}

CGameObject * CManagement::Get_GameObject(const _tchar * pLayerTag, const _tchar * pObjTag)
{
	if (nullptr == m_pScene)
		return nullptr;

	return m_pScene->Get_GameObject(pLayerTag, pObjTag);
}

CLayer * CManagement::Get_Layer(const _tchar * pLayerTag)
{
	if (nullptr == m_pScene)
		return nullptr;

	return m_pScene->Get_Layer(pLayerTag);
}

HRESULT CManagement::Ready_Shader(LPDIRECT3DDEVICE9 pGraphicDev)
{
	D3DXCOLOR White = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	D3DXCOLOR Black = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);
	D3DVIEWPORT9		ViewPort;
	pGraphicDev->GetViewport(&ViewPort);
	_int iSize = 256;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//¸ÞÀÌ
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	FAILED_CHECK_RETURN(Engine::Ready_RenderTarget(L"Target_Albedo_May", pGraphicDev, ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 0.f)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_DebugBuffer(L"Target_Albedo_May", 0.f, 0.f, 300.f, 200.f), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_RenderTarget(L"Target_Normal_May", pGraphicDev, ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_DebugBuffer(L"Target_Normal_May", 0.f, 200.f, 300.f, 200.f), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_RenderTarget(L"Target_Shade_May", pGraphicDev, ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_DebugBuffer(L"Target_Shade_May", 300.f, 0.f, 300.f, 200.f), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_RenderTarget(L"Target_Specular_May", pGraphicDev, ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 0.f)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_DebugBuffer(L"Target_Specular_May", 300.f, 200.f, 300.f, 200.f), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_RenderTarget(L"Target_Depth_May", pGraphicDev, ViewPort.Width, ViewPort.Height, D3DFMT_A32B32G32R32F, D3DXCOLOR(0.f, 0.f, 0.f, 0.f)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_DebugBuffer(L"Target_Depth_May", 0.f, 400.f, 300.f, 200.f), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_RenderTarget(L"Target_ShadowDepth_May", pGraphicDev, WINCX * 4, WINCY * 4, D3DFMT_A32B32G32R32F, D3DXCOLOR(1.f, 1.f, 1.f, 1.f)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_DebugBuffer(L"Target_ShadowDepth_May", 300.f, 400.f, 300.f, 200.f), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_RenderTarget(L"Target_Emissive_May", pGraphicDev, ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 0.f)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_DebugBuffer(L"Target_Emissive_May", 0.f, 600.f, 300.f, 200.f), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_RenderTarget(L"Target_SpecularTexture_May", pGraphicDev, ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 0.f)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_DebugBuffer(L"Target_SpecularTexture_May", 300.f, 600.f, 300.f, 200.f), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_RenderTarget(L"Target_Stencil_May", pGraphicDev, ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(1.f, 1.f, 1.f, 1.f)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_DebugBuffer(L"Target_Stencil_May", 0.f, 800.f, 300.f, 200.f), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_RenderTarget(L"Target_VTF_Pos_May", pGraphicDev, iSize, iSize, D3DFMT_A32B32G32R32F, White), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_DebugBuffer(L"Target_VTF_Pos_May", 600.f, 0.f, 300.f, 200.f), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_RenderTarget(L"Target_VTF_Normal_May", pGraphicDev, iSize, iSize, D3DFMT_A32B32G32R32F, White), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_DebugBuffer(L"Target_VTF_Normal_May", 600.f, 200.f, 300.f, 200.f), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_RenderTarget(L"Target_VTF_Tangent_May", pGraphicDev, iSize, iSize, D3DFMT_A32B32G32R32F, White), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_DebugBuffer(L"Target_VTF_Tangent_May", 600.f, 400.f, 300.f, 200.f), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_RenderTarget(L"Target_VTF_Binormal_May", pGraphicDev, iSize, iSize, D3DFMT_A32B32G32R32F, White), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_DebugBuffer(L"Target_VTF_Binormal_May", 600.f, 600.f, 300.f, 200.f), E_FAIL);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// ¸ÖÆ¼ ·»´õ Å¸°Ù
	FAILED_CHECK_RETURN(Engine::Ready_MRT(L"MRT_Deferred_May", L"Target_Albedo_May"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_MRT(L"MRT_Deferred_May", L"Target_Normal_May"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_MRT(L"MRT_Deferred_May", L"Target_Depth_May"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_MRT(L"MRT_Deferred_May", L"Target_Emissive_May"), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_MRT(L"MRT_LightAcc_May", L"Target_Shade_May"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_MRT(L"MRT_LightAcc_May", L"Target_Specular_May"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_MRT(L"MRT_LightAcc_May", L"Target_SpecularTexture_May"), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_MRT(L"MRT_LightDepth_May", L"Target_ShadowDepth_May"), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_MRT(L"MRT_Stencil_May", L"Target_Stencil_May"), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_MRT(L"MRT_VTF_May", L"Target_VTF_Pos_May"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_MRT(L"MRT_VTF_May", L"Target_VTF_Normal_May"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_MRT(L"MRT_VTF_May", L"Target_VTF_Tangent_May"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_MRT(L"MRT_VTF_May", L"Target_VTF_Binormal_May"), E_FAIL);


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//ÄÚµð
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	FAILED_CHECK_RETURN(Engine::Ready_RenderTarget(L"Target_Albedo_Cody", pGraphicDev, ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 0.f)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_DebugBuffer(L"Target_Albedo_Cody",			WINCX - 600.f, 0.f, 300.f, 200.f), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_RenderTarget(L"Target_Normal_Cody", pGraphicDev, ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_DebugBuffer(L"Target_Normal_Cody",			WINCX - 600.f, 200.f, 300.f, 200.f), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_RenderTarget(L"Target_Shade_Cody", pGraphicDev, ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_DebugBuffer(L"Target_Shade_Cody",				WINCX - 300.f, 0.f, 300.f, 200.f), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_RenderTarget(L"Target_Specular_Cody", pGraphicDev, ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 0.f)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_DebugBuffer(L"Target_Specular_Cody",			WINCX - 300.f, 200.f, 300.f, 200.f), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_RenderTarget(L"Target_Depth_Cody", pGraphicDev, ViewPort.Width, ViewPort.Height, D3DFMT_A32B32G32R32F, D3DXCOLOR(0.f, 0.f, 0.f, 0.f)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_DebugBuffer(L"Target_Depth_Cody",				WINCX - 600.f, 400.f, 300.f, 200.f), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_RenderTarget(L"Target_ShadowDepth_Cody", pGraphicDev, WINCX * 4, WINCY * 4, D3DFMT_A32B32G32R32F, D3DXCOLOR(1.f, 1.f, 1.f, 1.f)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_DebugBuffer(L"Target_ShadowDepth_Cody",		WINCX - 300.f, 400.f, 300.f, 200.f), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_RenderTarget(L"Target_Emissive_Cody", pGraphicDev, ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 0.f)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_DebugBuffer(L"Target_Emissive_Cody",			WINCX - 600.f, 600.f, 300.f, 200.f), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_RenderTarget(L"Target_SpecularTexture_Cody", pGraphicDev, ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 0.f)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_DebugBuffer(L"Target_SpecularTexture_Cody",	WINCX - 300.f, 600.f, 300.f, 200.f), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_RenderTarget(L"Target_Stencil_Cody", pGraphicDev, ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(1.f, 1.f, 1.f, 1.f)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_DebugBuffer(L"Target_Stencil_Cody", WINCX - 300.f, 800.f, 300.f, 200.f), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_RenderTarget(L"Target_VTF_Pos_Cody", pGraphicDev, iSize, iSize, D3DFMT_A32B32G32R32F, White ), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_DebugBuffer(L"Target_VTF_Pos_Cody", WINCX - 900.f, 0.f, 300.f, 200.f), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_RenderTarget(L"Target_VTF_Normal_Cody", pGraphicDev, iSize, iSize, D3DFMT_A32B32G32R32F, White ), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_DebugBuffer(L"Target_VTF_Normal_Cody", WINCX - 900.f, 200.f, 300.f, 200.f), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_RenderTarget(L"Target_VTF_Tangent_Cody", pGraphicDev, iSize, iSize, D3DFMT_A32B32G32R32F, White ), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_DebugBuffer(L"Target_VTF_Tangent_Cody", WINCX - 900.f, 400.f, 300.f, 200.f), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_RenderTarget(L"Target_VTF_Binormal_Cody", pGraphicDev, iSize, iSize, D3DFMT_A32B32G32R32F, White ), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_DebugBuffer(L"Target_VTF_Binormal_Cody", WINCX - 900.f, 600.f, 300.f, 200.f), E_FAIL);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// ¸ÖÆ¼ ·»´õ Å¸°Ù
	FAILED_CHECK_RETURN(Engine::Ready_MRT(L"MRT_Deferred_Cody", L"Target_Albedo_Cody"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_MRT(L"MRT_Deferred_Cody", L"Target_Normal_Cody"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_MRT(L"MRT_Deferred_Cody", L"Target_Depth_Cody"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_MRT(L"MRT_Deferred_Cody", L"Target_Emissive_Cody"), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_MRT(L"MRT_LightAcc_Cody", L"Target_Shade_Cody"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_MRT(L"MRT_LightAcc_Cody", L"Target_Specular_Cody"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_MRT(L"MRT_LightAcc_Cody", L"Target_SpecularTexture_Cody"), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_MRT(L"MRT_LightDepth_Cody", L"Target_ShadowDepth_Cody"), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_MRT(L"MRT_Stencil_Cody", L"Target_Stencil_Cody"), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_MRT(L"MRT_VTF_Cody", L"Target_VTF_Pos_Cody"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_MRT(L"MRT_VTF_Cody", L"Target_VTF_Normal_Cody"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_MRT(L"MRT_VTF_Cody", L"Target_VTF_Tangent_Cody"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_MRT(L"MRT_VTF_Cody", L"Target_VTF_Binormal_Cody"), E_FAIL);

	CShader*		pShader = nullptr;

	pShader = CShader::Create(pGraphicDev, L"../../Reference/Header/Shader_Mesh.hpp");
	NULL_CHECK_RETURN(pShader, E_FAIL);
	FAILED_CHECK_RETURN(Ready_Prototype(L"Shader_Mesh", pShader), E_FAIL);

	pShader = CShader::Create(pGraphicDev, L"../../Reference/Header/Shader_Shade.hpp");
	NULL_CHECK_RETURN(pShader, E_FAIL);
	FAILED_CHECK_RETURN(Ready_Prototype(L"Shader_Shade", pShader), E_FAIL);

	pShader = CShader::Create(pGraphicDev, L"../../Reference/Header/Shader_Blend.hpp");
	NULL_CHECK_RETURN(pShader, E_FAIL);
	FAILED_CHECK_RETURN(Ready_Prototype(L"Shader_Blend", pShader), E_FAIL);

	pShader = CShader::Create(pGraphicDev, L"../../Reference/Header/Shader_Effect.hpp");
	NULL_CHECK_RETURN(pShader, E_FAIL);
	FAILED_CHECK_RETURN(Ready_Prototype(L"Shader_Effect", pShader), E_FAIL);

	pShader = CShader::Create(pGraphicDev, L"../../Reference/Header/Shader_StaticObject.hpp");
	NULL_CHECK_RETURN(pShader, E_FAIL);
	FAILED_CHECK_RETURN(Ready_Prototype(L"Shader_StaticObject", pShader), E_FAIL);

	pShader = CShader::Create(pGraphicDev, L"../../Reference/Header/Shader_UI.hpp");
	NULL_CHECK_RETURN(pShader, E_FAIL);
	FAILED_CHECK_RETURN(Ready_Prototype(L"Shader_UI", pShader), E_FAIL);

	pShader = CShader::Create(pGraphicDev, L"../../Reference/Header/Shader_Logo.hpp");
	NULL_CHECK_RETURN(pShader, E_FAIL);
	FAILED_CHECK_RETURN(Ready_Prototype(L"Shader_Logo", pShader), E_FAIL);

	pShader = CShader::Create(pGraphicDev, L"../../Reference/Header/Shader_Particle.hpp");
	NULL_CHECK_RETURN(pShader, E_FAIL);
	FAILED_CHECK_RETURN(Ready_Prototype(L"Shader_Particle", pShader), E_FAIL);

	pShader = CShader::Create(pGraphicDev, L"../../Reference/Header/Shader_All.hpp");
	NULL_CHECK_RETURN(pShader, E_FAIL);
	FAILED_CHECK_RETURN(Ready_Prototype(L"Shader_All", pShader), E_FAIL);

	pShader = CShader::Create(pGraphicDev, L"../../Reference/Header/Shader_VTF.hpp");
	NULL_CHECK_RETURN(pShader, E_FAIL);
	FAILED_CHECK_RETURN(Ready_Prototype(L"Shader_VTF", pShader), E_FAIL);

	//pShader = CShader::Create(pGraphicDev, L"../../Reference/Header/Shader_Flower.hpp");
	//NULL_CHECK_RETURN(pShader, E_FAIL);
	//FAILED_CHECK_RETURN(Ready_Prototype(L"Shader_test", pShader), E_FAIL);
	Engine::Ready_Particle_Manager(pGraphicDev, 5000);

	return S_OK;
}

HRESULT CManagement::Add_Layer(const _tchar * pLayerTag, CLayer * pLayer)
{
	return m_pScene->Add_Layer(pLayerTag, pLayer);
}

