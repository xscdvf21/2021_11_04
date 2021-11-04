#ifndef Renderer_h__
#define Renderer_h__

#include "Engine_Define.h"
#include "Base.h"
#include "Component.h"


#define SplitCount 2
/*
0번 메이 카메라
1번 코디 카메라
2번 통합 카메라
*/
BEGIN(Engine)
class ENGINE_DLL CRenderer : public CBase
{
	DECLARE_SINGLETON(CRenderer)

private:
	explicit CRenderer();
	virtual ~CRenderer();

public:
	HRESULT Set_Device(LPDIRECT3DDEVICE9 pDevice);
	void	Set_ViewMatrix(_uint iIdex, _float4x4 matViewMatrix);

public:
	HRESULT	Ready_Renderer(LPDIRECT3DDEVICE9 pGraphicDev);
	void	Add_RenderGroup(RENDERID eGroup, class CGameObject* pGameObject);
	void	Render_GameObject(const _float& fTimeDelta, const _vec3& vBackBuffer);
	void	Clear_RenderGroup();

public:
	HRESULT Render_Priority(const _int& iIndex);
	HRESULT Render_NoAlpha(const _int& iIndex);
	HRESULT Render_Alpha(const _int& iIndex);
	HRESULT Render_ShadowDepth(const _int& iIndex, const _tchar* MRT);
	HRESULT Render_StencilDepth(const _int& iIndex, const _tchar* MRT);
	HRESULT Render_UI();

public:
	void	Render_Deferred(const _vec3& vBackBuffer);
	void	Render_LightAcc();
	void	Render_Blend();
public:
	//0 : Fog Off 
	//1 : ZFog On
	//2 : YFog On
	//3 : AllFog On
	void	Set_BlendPass(const _int iPass) { m_iBlendPass = iPass; }
	void	Change_Buffer(const _int& iIndex,const _float& fSecond,const _float& fX,const _float& fY,const _float& fSizeX,const _float& fSizeY);
	void	Set_Split(const _bool& bState=true) { m_bSplit = bState; }
	void	Start_Split();
	void	End_Split();
	void	Set_CodyGreyScale(_bool greyscale) { m_bCodyGreyScale = greyscale; }
	void	Set_MayGreyScale(_bool greyscale) { m_bMayGreyScale = greyscale; }
	
public:
	_bool	Get_Split() { return m_bSplit; }
	void	Set_Buffer(const _int& iIndex, const _float& fX, const _float& fY, const _float& fSizeX, const _float& fSizeY);

private:
	list<class CGameObject*>	m_RenderGroup[RENDER_END];
	class CRcTex*				m_pScreenBuffer[2];
	LPDIRECT3DDEVICE9			m_pGraphicDev;
	_bool						m_bCheck = false;
	_bool						m_bSplit = false;
	_bool						m_bStencil = false;
	_bool						m_bStart;
	_bool						m_bCodyGreyScale = false;
	_bool						m_bMayGreyScale = false;
	_vec3						m_vLightEye{};
	_vec3						m_vLightAt{};
	_vec4						m_vSize[2];

	vector<_float4x4>			m_vecViewMatrix;

	LPDIRECT3DSURFACE9			m_pTargetSurface = nullptr;
	LPDIRECT3DSURFACE9			m_pOldTargetSurface = nullptr;

	LPDIRECT3DVERTEXBUFFER9		m_pVB;
	LPDIRECT3DINDEXBUFFER9		m_pIB;
	
private:
	virtual void	Free(void);
	void Render_VTF();
	_int m_iBlendPass = 0;
	void Render_ForwardUI();
};
END

#endif // Renderer_h__
