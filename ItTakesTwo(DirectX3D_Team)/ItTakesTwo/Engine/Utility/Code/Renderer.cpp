#include "Renderer.h"
#include "GameObject.h"
#include "Shader.h"
#include "Export_Utility.h"
#include "RcTex.h"
#include "GraphicDevice.h"
#include "..\Client\Code\MainCamera.h"

USING(Engine)
IMPLEMENT_SINGLETON(CRenderer)


Engine::CRenderer::CRenderer()
	:m_bSplit(false), m_bStart(false)
{
	_mat matView;
	D3DXMatrixIdentity(&matView);

	for (_uint i = 0; i < 3; ++i)
		m_vecViewMatrix.emplace_back(matView);

	m_pScreenBuffer[0] = nullptr;
	m_pScreenBuffer[1] = nullptr;
}

Engine::CRenderer::~CRenderer()
{
	Free();
}

HRESULT CRenderer::Set_Device(LPDIRECT3DDEVICE9 pDevice)
{
	if (nullptr == pDevice)
		return E_FAIL;

	m_pGraphicDev = pDevice;

	if (!m_bCheck)
		m_pGraphicDev->AddRef();

	m_bCheck = true;

	return S_OK;
}

void CRenderer::Set_ViewMatrix(_uint iIdex, _float4x4 matViewMatrix)
{
	if (iIdex >= 3)
		return;

	m_vecViewMatrix[iIdex] = matViewMatrix;

}

void Engine::CRenderer::Add_RenderGroup(RENDERID eGroup, CGameObject* pGameObject)
{
	if (RENDER_END <= eGroup)
		return;

	m_RenderGroup[eGroup].push_back(pGameObject);
	pGameObject->AddRef();
}

HRESULT CRenderer::Ready_Renderer(LPDIRECT3DDEVICE9 pGraphicDev)
{
	m_pGraphicDev = pGraphicDev;
	m_pGraphicDev->AddRef();

	D3DVIEWPORT9			ViewPort;
	pGraphicDev->GetViewport(&ViewPort);
	m_pScreenBuffer[0] = CRcTex::Create(m_pGraphicDev, true);
	m_pScreenBuffer[1] = CRcTex::Create(m_pGraphicDev, true);
	Start_Split();

	FAILED_CHECK_RETURN(m_pGraphicDev->CreateDepthStencilSurface(WINCX * 4,
		WINCY * 4,
		D3DFMT_D24S8,
		D3DMULTISAMPLE_NONE,
		0,
		TRUE,
		&m_pTargetSurface,
		NULL),
		E_FAIL);

	return S_OK;
}

void Engine::CRenderer::Render_GameObject(const _float& fTimeDelta, const _vec3& vBackBuffer)
{
	for (int i = 0; i < SplitCount; ++i)
	{
		m_vSize[i] = m_pScreenBuffer[i]->Update_Buffer(fTimeDelta);
	}
	Render_ForwardUI();
	//Render_VTF();
	Render_Deferred(vBackBuffer);
	Render_LightAcc();
	Render_Blend();

	FAILED_CHECK_RETURN(Render_UI(), );

	if (GetAsyncKeyState(VK_TAB) & 0x8000)
	{
		Engine::Render_DebugBuffer(L"MRT_Deferred_May");
		Engine::Render_DebugBuffer(L"MRT_LightAcc_May");
		Engine::Render_DebugBuffer(L"MRT_Deferred_Cody");
		Engine::Render_DebugBuffer(L"MRT_LightAcc_Cody");
		Engine::Render_DebugBuffer(L"MRT_LightDepth_May");
		Engine::Render_DebugBuffer(L"MRT_LightDepth_Cody");
		Engine::Render_DebugBuffer(L"MRT_Stencil_May");
		Engine::Render_DebugBuffer(L"MRT_Stencil_Cody");

		Engine::Render_DebugBuffer(L"MRT_VTF_May");
		Engine::Render_DebugBuffer(L"MRT_VTF_Cody");
	}

	Clear_RenderGroup();
}

void Engine::CRenderer::Clear_RenderGroup()
{
	for (_uint i = 0; i < RENDER_END; ++i)
	{
		for_each(m_RenderGroup[i].begin(), m_RenderGroup[i].end(), CDeleteObj());
		m_RenderGroup[i].clear();
	}
}

HRESULT CRenderer::Render_Priority(const _int& iIndex)
{
	for (auto& iter : m_RenderGroup[RENDER_PRIORITY])
		iter->Render_Object(iIndex);

	return S_OK;
}

HRESULT CRenderer::Render_NoAlpha(const _int& iIndex)
{
	for (auto& iter : m_RenderGroup[RENDER_NONALPHA])
		iter->Render_Object(iIndex);

	return S_OK;
}

_bool	Compare_Z(CGameObject* pDest, CGameObject* pSrc)
{
	return pDest->Get_ViewZ() > pSrc->Get_ViewZ();
}

HRESULT CRenderer::Render_Alpha(const _int& iIndex)
{
	if (m_bSplit)
	{
		for (auto& iter : m_RenderGroup[RENDER_ALPHA])
			iter->Compute_ViewZ();

		m_RenderGroup[RENDER_ALPHA].sort(Compare_Z);

		for (auto& iter : m_RenderGroup[RENDER_ALPHA])
			iter->Render_Object(iIndex);
	}
	else
	{
		m_RenderGroup[RENDER_ALPHA].sort(Compare_Z);

		for (auto& iter : m_RenderGroup[RENDER_ALPHA])
			iter->Render_Object(iIndex);
	}

	return S_OK;
}

HRESULT CRenderer::Render_ShadowDepth(const _int & iIndex, const _tchar* MRT)
{
	Engine::Begin_MRT(MRT);

	m_pGraphicDev->GetDepthStencilSurface(&m_pOldTargetSurface);
	m_pGraphicDev->SetDepthStencilSurface(m_pTargetSurface);

	m_pGraphicDev->Clear(0, 0, D3DCLEAR_ZBUFFER, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), 1.f, 0);

	for (auto& iter : m_RenderGroup[RENDER_SHADOW])
		iter->Render_Shadow(&m_vLightEye, &m_vLightAt, iIndex);


	m_pGraphicDev->SetDepthStencilSurface(m_pOldTargetSurface);
	Safe_Release(m_pOldTargetSurface);
	Engine::End_MRT(MRT);

	return S_OK;
}

HRESULT CRenderer::Render_StencilDepth(const _int & iIndex, const _tchar * MRT)
{
	Engine::Begin_MRT(MRT);

	m_pGraphicDev->GetDepthStencilSurface(&m_pOldTargetSurface);
	m_pGraphicDev->SetDepthStencilSurface(m_pTargetSurface);

	m_pGraphicDev->Clear(0, 0, D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), 1.f, 0);

	for (auto& iter : m_RenderGroup[RENDER_STENCIL])
		iter->Render_Stencil(iIndex);

	m_pGraphicDev->SetDepthStencilSurface(m_pOldTargetSurface);
	Safe_Release(m_pOldTargetSurface);
	Engine::End_MRT(MRT);
	return S_OK;
}

HRESULT CRenderer::Render_UI()
{
	m_RenderGroup[RENDER_UI].sort(Compare_Z);
	for (auto& iter : m_RenderGroup[RENDER_UI])
		iter->Render_Object();

	for (auto& iter : m_RenderGroup[RENDER_MENUUI])
		iter->Render_Object();

	for (auto& iter : m_RenderGroup[RENDER_PRIUI])
		iter->Render_Object();

	for (auto& iter : m_RenderGroup[RENDER_FINALUI])
		iter->Render_Object();

	return S_OK;
}

void CRenderer::Render_Deferred(const _vec3& vBackBuffer)
{
	if (m_bSplit)
	{
		m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_vecViewMatrix[0]);

		FAILED_CHECK_RETURN(Render_ShadowDepth(0, L"MRT_LightDepth_May"), );
		Engine::Begin_MRT(L"MRT_Deferred_May");
		FAILED_CHECK_RETURN(Render_Priority(0), );
		FAILED_CHECK_RETURN(Render_NoAlpha(0), );
		FAILED_CHECK_RETURN(Render_Alpha(0), );
		Engine::End_MRT(L"MRT_Deferred_May");

		FAILED_CHECK_RETURN(Render_StencilDepth(0, L"MRT_Stencil_May"), );

		m_pGraphicDev->Clear(0,
			nullptr,
			D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
			D3DXCOLOR(vBackBuffer.x, vBackBuffer.y, vBackBuffer.z, 1.f),
			1.f,
			0);

		m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_vecViewMatrix[1]);

		FAILED_CHECK_RETURN(Render_ShadowDepth(1, L"MRT_LightDepth_Cody"), );
		Engine::Begin_MRT(L"MRT_Deferred_Cody");
		FAILED_CHECK_RETURN(Render_Priority(1), );
		FAILED_CHECK_RETURN(Render_NoAlpha(1), );
		FAILED_CHECK_RETURN(Render_Alpha(1), );

		Engine::End_MRT(L"MRT_Deferred_Cody");

		FAILED_CHECK_RETURN(Render_StencilDepth(1, L"MRT_Stencil_Cody"), );

		m_pGraphicDev->Clear(0,
			nullptr,
			D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
			D3DXCOLOR(vBackBuffer.x, vBackBuffer.y, vBackBuffer.z, 1.f),
			1.f,
			0);
	}
	else
	{
		m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_vecViewMatrix[2]);
		Engine::Begin_MRT(L"MRT_Deferred_May");

		FAILED_CHECK_RETURN(Render_Priority(2), );
		FAILED_CHECK_RETURN(Render_NoAlpha(2), );
		FAILED_CHECK_RETURN(Render_Alpha(2), );

		Engine::End_MRT(L"MRT_Deferred_May");
		FAILED_CHECK_RETURN(Render_StencilDepth(2, L"MRT_Stencil_May"), );
		FAILED_CHECK_RETURN(Render_ShadowDepth(2, L"MRT_LightDepth_May"), );

		//m_pGraphicDev->Clear(0,
		//	nullptr,
		//	D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		//	D3DXCOLOR(0.f, 0.f, 0.f, 1.f),
		//	1.f,
		//	0);
	}
}

void CRenderer::Render_LightAcc()
{
	Engine::Begin_MRT(L"MRT_LightAcc_May");

	// 빛만 그리기 위한 공간
	CShader*		pShader = dynamic_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_Shade"));
	NULL_CHECK(pShader);

	LPD3DXEFFECT pEffect = pShader->Get_EffectHandle();
	pEffect->AddRef();
	if (m_bSplit)
	{
		m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_vecViewMatrix[0]);

		// 노말 텍스처를 얻어와서 상수 테이블로 넘겨주자
		Engine::SetUp_OnShader(pEffect, L"Target_Normal_May", "g_NormalTexture");
		Engine::SetUp_OnShader(pEffect, L"Target_Depth_May", "g_DepthTexture");

		pEffect->Begin(NULL, 0);

		Engine::Render_Light(pEffect);

		pEffect->End();

		Engine::End_MRT(L"MRT_LightAcc_May");
		/////////
		Engine::Begin_MRT(L"MRT_LightAcc_Cody");
		m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_vecViewMatrix[1]);

		Engine::SetUp_OnShader(pEffect, L"Target_Normal_Cody", "g_NormalTexture");
		Engine::SetUp_OnShader(pEffect, L"Target_Depth_Cody", "g_DepthTexture");

		pEffect->Begin(NULL, 0);

		Engine::Render_Light(pEffect);

		pEffect->End();

		Engine::End_MRT(L"MRT_LightAcc_Cody");
	}
	else
	{
		m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_vecViewMatrix[2]);

		// 노말 텍스처를 얻어와서 상수 테이블로 넘겨주자
		Engine::SetUp_OnShader(pEffect, L"Target_Normal_May", "g_NormalTexture");
		Engine::SetUp_OnShader(pEffect, L"Target_Depth_May", "g_DepthTexture");

		pEffect->Begin(NULL, 0);

		Engine::Render_Light(pEffect);

		pEffect->End();

		Engine::End_MRT(L"MRT_LightAcc_May");
	}

	Safe_Release(pShader);

	Safe_Release(pEffect);
}

void CRenderer::Render_Blend()
{
	CShader*	pShader = dynamic_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_Blend"));

	NULL_CHECK(pShader);

	LPD3DXEFFECT	pEffect = pShader->Get_EffectHandle();
	pEffect->AddRef();

	_mat matTrans, matScale;
	_mat matWorld, matView, matProj, matInv;

	//그림자
	_matrix			matLightView, matLightProj;

	pEffect->Begin(NULL, 0);
	pEffect->BeginPass(m_iBlendPass);

	if (m_bSplit)
	{
		// MayCamera
		matTrans = m_pScreenBuffer[0]->Get_Pos();
		matScale = m_pScreenBuffer[0]->Get_Scale();
		matWorld = matScale * matTrans;

		D3DXMatrixIdentity(&matView);
		D3DXMatrixOrthoLH(&matProj, WINCX, WINCY, 0.f, 1.f);

		pEffect->SetMatrix("g_matWorld", &matWorld);
		pEffect->SetMatrix("g_matView", &matView);
		pEffect->SetMatrix("g_matProj", &matProj);
		pEffect->SetVector("g_vSize", (_vec4*)&m_vSize);
		pEffect->SetBool("g_vGreyScale", m_bMayGreyScale);

		m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
		m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
		D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(60.f), _float(WINCX) / _float(WINCY), 0.1f, 1000.f);
		D3DXMatrixInverse(&matView, nullptr, &m_vecViewMatrix[0]);
		pEffect->SetMatrix("g_matProjInv", D3DXMatrixInverse(&matProj, nullptr, &matProj));
		pEffect->SetMatrix("g_matViewInv", &matView);

		D3DXMatrixLookAtLH(&matLightView, &m_vLightEye, &m_vLightAt, &vUP);
		pEffect->SetMatrix("g_matLightView", &matLightView);

		D3DXMatrixPerspectiveFovLH(&matLightProj, D3DXToRadian(60.f), _float(WINCX) / _float(WINCY), 0.1f, 1000.f);
		pEffect->SetMatrix("g_matLightProj", &matLightProj);

		Engine::SetUp_OnShader(pEffect, L"Target_Albedo_May", "g_AlbedoTexture");
		Engine::SetUp_OnShader(pEffect, L"Target_Shade_May", "g_ShadeTexture");
		Engine::SetUp_OnShader(pEffect, L"Target_Specular_May", "g_SpecularTexture");
		Engine::SetUp_OnShader(pEffect, L"Target_Depth_May", "g_DepthTexture");
		Engine::SetUp_OnShader(pEffect, L"Target_Emissive_May", "g_EmissiveTexture");
		Engine::SetUp_OnShader(pEffect, L"Target_SpecularTexture_May", "g_CustomSpecularTexture");
		Engine::SetUp_OnShader(pEffect, L"Target_ShadowDepth_May", "g_ShadowTexture");
		Engine::SetUp_OnShader(pEffect, L"Target_Stencil_May", "g_StencilTexture");
		pEffect->CommitChanges();

		m_pScreenBuffer[0]->Render_Buffer();

		// CodyCamera
		matTrans = m_pScreenBuffer[1]->Get_Pos();
		matScale = m_pScreenBuffer[1]->Get_Scale();
		matWorld = matScale * matTrans;

		D3DXMatrixIdentity(&matView);
		D3DXMatrixOrthoLH(&matProj, WINCX, WINCY, 0.f, 1.f);

		pEffect->SetMatrix("g_matWorld", &matWorld);
		pEffect->SetMatrix("g_matView", &matView);
		pEffect->SetMatrix("g_matProj", &matProj);
		pEffect->SetVector("g_vSize", (_vec4*)&m_vSize);
		pEffect->SetBool("g_vGreyScale", m_bCodyGreyScale);

		m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
		m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
		D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(60.f), _float(WINCX) / _float(WINCY), 0.1f, 1000.f);
		D3DXMatrixInverse(&matView, nullptr, &m_vecViewMatrix[1]);
		pEffect->SetMatrix("g_matProjInv", D3DXMatrixInverse(&matProj, nullptr, &matProj));
		pEffect->SetMatrix("g_matViewInv", &matView);

		D3DXMatrixLookAtLH(&matLightView, &m_vLightEye, &m_vLightAt, &vUP);
		pEffect->SetMatrix("g_matLightView", &matLightView);

		D3DXMatrixPerspectiveFovLH(&matLightProj, D3DXToRadian(60.f), _float(WINCX) / _float(WINCY), 0.1f, 1000.f);
		pEffect->SetMatrix("g_matLightProj", &matLightProj);

		Engine::SetUp_OnShader(pEffect, L"Target_Albedo_Cody", "g_AlbedoTexture");
		Engine::SetUp_OnShader(pEffect, L"Target_Shade_Cody", "g_ShadeTexture");
		Engine::SetUp_OnShader(pEffect, L"Target_Specular_Cody", "g_SpecularTexture");
		Engine::SetUp_OnShader(pEffect, L"Target_Depth_Cody", "g_DepthTexture");
		Engine::SetUp_OnShader(pEffect, L"Target_Emissive_Cody", "g_EmissiveTexture");
		Engine::SetUp_OnShader(pEffect, L"Target_SpecularTexture_Cody", "g_CustomSpecularTexture");
		Engine::SetUp_OnShader(pEffect, L"Target_ShadowDepth_Cody", "g_ShadowTexture");
		Engine::SetUp_OnShader(pEffect, L"Target_Stencil_Cody", "g_StencilTexture");
		pEffect->CommitChanges();

		m_pScreenBuffer[1]->Render_Buffer();
	}
	else
	{
		matTrans = m_pScreenBuffer[0]->Get_Pos();
		matScale = m_pScreenBuffer[0]->Get_Scale();
		matWorld = matScale * matTrans;

		D3DXMatrixIdentity(&matView);
		D3DXMatrixOrthoLH(&matProj, WINCX, WINCY, 0.f, 1.f);

		pEffect->SetMatrix("g_matWorld", &matWorld);
		pEffect->SetMatrix("g_matView", &matView);
		pEffect->SetMatrix("g_matProj", &matProj);
		pEffect->SetVector("g_vSize", (_vec4*)&m_vSize);

		m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
		m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
		D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(60.f), _float(WINCX) / _float(WINCY), 0.1f, 1000.f);

		pEffect->SetMatrix("g_matProjInv", D3DXMatrixInverse(&matProj, nullptr, &matProj));
		pEffect->SetMatrix("g_matViewInv", D3DXMatrixInverse(&matView, nullptr, &m_vecViewMatrix[2]));

		_vec3		vLightEye = _vec3(4.f, 0.f, -4.f);
		D3DXMatrixLookAtLH(&matLightView, &(m_vLightEye + vLightEye), &m_vLightAt, &vUP);
		pEffect->SetMatrix("g_matLightView", &matLightView);

		D3DXMatrixPerspectiveFovLH(&matLightProj, D3DXToRadian(60.f), _float(WINCX) / _float(WINCY), 0.1f, 1000.f);
		pEffect->SetMatrix("g_matLightProj", &matLightProj);

		Engine::SetUp_OnShader(pEffect, L"Target_Albedo_May", "g_AlbedoTexture");
		Engine::SetUp_OnShader(pEffect, L"Target_Shade_May", "g_ShadeTexture");
		Engine::SetUp_OnShader(pEffect, L"Target_Specular_May", "g_SpecularTexture");
		Engine::SetUp_OnShader(pEffect, L"Target_Depth_May", "g_DepthTexture");
		Engine::SetUp_OnShader(pEffect, L"Target_Emissive_May", "g_EmissiveTexture");
		Engine::SetUp_OnShader(pEffect, L"Target_SpecularTexture_May", "g_CustomSpecularTexture");
		Engine::SetUp_OnShader(pEffect, L"Target_ShadowDepth_May", "g_ShadowTexture");
		Engine::SetUp_OnShader(pEffect, L"Target_Stencil_May", "g_StencilTexture");
		pEffect->CommitChanges();
		m_pScreenBuffer[0]->Render_Buffer();
	}

	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pShader);
	Safe_Release(pEffect);

}



void Engine::CRenderer::Render_ForwardUI()
{
	for (auto& iter : m_RenderGroup[RENDER_FORWARDUI])
		iter->Render_Object();
}

void Engine::CRenderer::Render_VTF()
{
	Engine::Begin_MRT(L"MRT_VTF_May");

	for (auto& iter : m_RenderGroup[RENDER_VTF_MAY])
		iter->Render_VTF();

	Engine::End_MRT(L"MRT_VTF_May");


	Engine::Begin_MRT(L"MRT_VTF_Cody");

	for (auto& iter : m_RenderGroup[RENDER_VTF_CODY])
		iter->Render_VTF();

	Engine::End_MRT(L"MRT_VTF_Cody");

}

void Engine::CRenderer::End_Split()
{
	Change_Buffer(0, 1.0f, 0, 0, WINCX, WINCY);
	m_bSplit = false;
}

void Engine::CRenderer::Start_Split()
{
	Set_Buffer(0, 0, 0, _float(WINCX >> 1), WINCY);
	// <<<<<<< HEAD
		// Set_Buffer(1,  _float(WINCX>>1), 0, WINCX, WINCY);
	// =======

	Set_Buffer(1, _float(WINCX >> 1), 0, _float(WINCX), WINCY);
	m_bSplit = true;
}

void Engine::CRenderer::Change_Buffer(const _int& iIndex, const _float& fSecond, const _float& fX, const _float& fY, const _float& fSizeX, const _float& fSizeY)
{
	if (iIndex >= SplitCount)
		return;
	m_pScreenBuffer[iIndex]->Change_Size(fSecond, fX, fY, fSizeX, fSizeY);
}
void Engine::CRenderer::Set_Buffer(const _int& iIndex, const _float& fX, const _float& fY, const _float& fSizeX, const _float& fSizeY)
{
	if (iIndex >= SplitCount)
		return;
	m_pScreenBuffer[iIndex]->Set_Size(fX, fY, fSizeX, fSizeY);
}
void Engine::CRenderer::Free()
{
	Safe_Release(m_pScreenBuffer[0]);
	Safe_Release(m_pScreenBuffer[1]);
	Safe_Release(m_pTargetSurface);

	Clear_RenderGroup();
	Engine::Safe_Release(m_pGraphicDev);
}

