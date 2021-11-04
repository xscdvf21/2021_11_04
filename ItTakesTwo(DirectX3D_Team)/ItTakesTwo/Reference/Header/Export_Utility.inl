#include "Export_Utility.h"


// Management
CComponent*		Get_Component(const _tchar* pLayerTag, const _tchar* pObjTag, const _tchar* pComponentTag, COMPONENTID eID)
{
	return CManagement::GetInstance()->Get_Component(pLayerTag, pObjTag, pComponentTag, eID);
}

CGameObject*	Get_GameObject(const _tchar* pLayerTag, const _tchar* pObjTag)
{
	return CManagement::GetInstance()->Get_GameObject(pLayerTag, pObjTag);
}

CLayer*		Get_Layer(const _tchar * pLayerTag)
{
	return CManagement::GetInstance()->Get_Layer(pLayerTag);
}

CScene*		Get_Scene()
{
	return CManagement::GetInstance()->Get_Scene();
}

HRESULT		Add_Layer(const _tchar* pLayerTag, CLayer* pLayer)
{
	return CManagement::GetInstance()->Add_Layer(pLayerTag, pLayer);
}

HRESULT		Create_Management(CManagement** ppManagement, LPDIRECT3DDEVICE9 pGraphicDev)
{
	CManagement*		pManagement = CManagement::GetInstance();

	if (nullptr == pManagement)
		return E_FAIL;

	if (FAILED(pManagement->Ready_Shader(pGraphicDev)))
		return E_FAIL;

	*ppManagement = pManagement;

	return S_OK	;
}

HRESULT		Set_Scene(CScene* pScene, LPDIRECT3DDEVICE9 pDevice)
{
	return CManagement::GetInstance()->Set_Scene(pScene, pDevice);
}

_int		Update_Scene(const _float& fTimeDelta)
{
	return CManagement::GetInstance()->Update_Scene(fTimeDelta);
}

_int		LateUpdate_Scene(const _float& fTimeDelta)
{
	return CManagement::GetInstance()->LateUpdate_Scene(fTimeDelta);
}

void		Render_Scene(const _float& fTimeDelta, const _vec3& vBackBuffer)
{
	CManagement::GetInstance()->Render_Scene(fTimeDelta, vBackBuffer);
}


// ParticleMgr
HRESULT		Ready_Particle_Manager(LPDIRECT3DDEVICE9 pDevice, _uint iSize)
{
	return CParticle_Manager::GetInstance()->Ready_Particle_Manager(pDevice, iSize);
}

Engine::CParticle* Emit_DotParticle( PARTICLE& desc, _uint Count, _uint PassIndex /*= 0*/)
{
	return CParticle_Manager::GetInstance()->Emit_DotParticle(desc, Count, PassIndex);
}

Engine::CParticle* Emit_ImageParticle( PARTICLE& desc, _uint TextureIndex, _uint PassIndex)
{
	return CParticle_Manager::GetInstance()->Emit_ImageParticle(desc, TextureIndex, PassIndex);
}

Engine::CParticle* Emit_MeshParticle( PARTICLE& desc, const _wstr& tagMesh, _uint PassIndex)
{
	return CParticle_Manager::GetInstance()->Emit_MeshParticle(desc, tagMesh, PassIndex);
}

void		Stop_All_Particle() 
{
	CParticle_Manager::GetInstance()->Stop_All_Particle();
}

void Reset_Particle(PARTICLE& desc)
{
	Engine::CParticle_Manager::GetInstance()->Reset_Particle(desc);
}

void Set_AnimState(PARTICLE& desc, _int iX, _int iY, _float fSpeed)
{
	CParticle_Manager::GetInstance()->Set_AniState(desc, iX, iY, fSpeed);
}
void Engine::Reset_AllParticle()
{
	Engine::CParticle_Manager::GetInstance()->Reset();

}



// ProtoMgr
HRESULT			Ready_Prototype(const _tchar* pProtoTag, CComponent* pInstance)
{
	return CProtoMgr::GetInstance()->Ready_Prototype(pProtoTag, pInstance);
}
CComponent*		Clone_Prototype(const _tchar* pProtoTag)
{
	return CProtoMgr::GetInstance()->Clone_Prototype(pProtoTag);
}


// Renderer
CRenderer*	Get_Renderer(void)
{
	return CRenderer::GetInstance();
}

void Add_RenderGroup(RENDERID eGroup, CGameObject * pGameObject)
{
	return CRenderer::GetInstance()->Add_RenderGroup(eGroup, pGameObject);
}

void Start_Split()
{
	return CRenderer::GetInstance()->Start_Split();
}

void	Set_CodyGreyScale(_bool greyscale) 
{
	return CRenderer::GetInstance()->Set_CodyGreyScale(greyscale);
}

void	Set_MayGreyScale(_bool greyscale)
{
	return CRenderer::GetInstance()->Set_MayGreyScale(greyscale);
}

void Engine::Set_BlendPass(const _int& iPass)
{
	return CRenderer::GetInstance()->Set_BlendPass(iPass);
}

void End_Split()
{
	return CRenderer::GetInstance()->End_Split();
}

// LightMgr
CLight*		Ready_Light(LPDIRECT3DDEVICE9 pGraphicDev, const D3DLIGHT9* pLightInfo, const _uint& iIndex)
{
	return CLightMgr::GetInstance()->Ready_Light(pGraphicDev, pLightInfo, iIndex);
}

void		Render_Light(LPD3DXEFFECT& pEffect)
{
	CLightMgr::GetInstance()->Render_Light(pEffect);
}

Engine::_bool Is_LightExsist()
{
	return CLightMgr::GetInstance()->Is_LightExsist();
}

void Clear_Light()
{
	CLightMgr::GetInstance()->Clear_Light();
}



// RenderTartetMgr
HRESULT		Ready_RenderTarget(const _tchar* pTargetTag, LPDIRECT3DDEVICE9 pGraphpicDev, const _uint& iWidth, const _uint& iHeight, D3DFORMAT Format, D3DXCOLOR Color)
{
	return CRenderTargetMgr::GetInstance()->Ready_RenderTarget(pTargetTag, pGraphpicDev, iWidth, iHeight, Format, Color);
}

HRESULT		Ready_MRT(const _tchar* pMRTTag, const _tchar* pTargetTag)
{
	return CRenderTargetMgr::GetInstance()->Ready_MRT(pMRTTag, pTargetTag);
}

HRESULT		Begin_MRT(const _tchar* pMRTTag)
{
	return CRenderTargetMgr::GetInstance()->Begin_MRT(pMRTTag);
}
HRESULT		End_MRT(const _tchar* pMRTTag)
{
	return CRenderTargetMgr::GetInstance()->End_MRT(pMRTTag);
}
HRESULT			Ready_DebugBuffer(const _tchar* pTargetTag, const _float& fX, const _float& fY, const _float& fSizeX, const _float& fSizeY)
{
	return CRenderTargetMgr::GetInstance()->Ready_DebugBuffer(pTargetTag, fX, fY, fSizeX, fSizeY);
}
void			Render_DebugBuffer(const _tchar* pMRTTag)
{
	CRenderTargetMgr::GetInstance()->Render_DebugBuffer(pMRTTag);
}

void			SetUp_OnShader(LPD3DXEFFECT& pEffect, const _tchar* pTargetTag, const char* pConstantTable)
{
	CRenderTargetMgr::GetInstance()->SetUp_OnShader(pEffect, pTargetTag, pConstantTable);
}
void			SetUp_OnRegister(_int iRegisterIndex , const _tchar* pTargetTag)
{
	CRenderTargetMgr::GetInstance()->SetUp_OnRegister(iRegisterIndex, pTargetTag);
}

// Release Utility
void		Release_Utility(void)
{
	CParticle_Manager::GetInstance()->DestroyInstance();
	CRenderTargetMgr::GetInstance()->DestroyInstance();
	CLightMgr::GetInstance()->DestroyInstance();
	CRenderer::GetInstance()->DestroyInstance();
	CProtoMgr::GetInstance()->DestroyInstance();
	CManagement::GetInstance()->DestroyInstance();
}