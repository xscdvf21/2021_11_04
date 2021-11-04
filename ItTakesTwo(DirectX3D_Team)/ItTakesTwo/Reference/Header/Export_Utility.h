#ifndef Export_Utility_h__
#define Export_Utility_h__

#include "Management.h"
#include "ProtoMgr.h"
#include "Transform.h"
#include "Renderer.h"
#include "Camera.h"
#include "Calculator.h"
#include "LightMgr.h"
#include "Collider.h"
#include "Shader.h"
#include "RenderTargetMgr.h"
#include "Light.h"
#include "Particle_Manager.h"
#include "Randoms.h"



BEGIN(Engine)

// Management
inline CComponent*	Get_Component(const _tchar* pLayerTag, const _tchar* pObjTag, const _tchar* pComponentTag, COMPONENTID eID);
inline CGameObject*	Get_GameObject(const _tchar* pLayerTag, const _tchar* pObjTag);
inline CLayer*		Get_Layer(const _tchar * pLayerTag);
inline CScene*		Get_Scene();
inline HRESULT		Add_Layer(const _tchar* pLayerTag, CLayer* pLayer);
inline HRESULT		Create_Management(CManagement** ppManagement, LPDIRECT3DDEVICE9 pGraphicDev);
inline  HRESULT		Set_Scene(CScene* pScene, LPDIRECT3DDEVICE9 pDevice);
inline _int			Update_Scene(const _float& TimeDelta);
inline _int			LateUpdate_Scene(const _float& TimeDelta);
inline void			Render_Scene(const _float& fTimeDelta, const _vec3& vBackBuffer);


// ParticleMgr
inline HRESULT		Ready_Particle_Manager(LPDIRECT3DDEVICE9 pDevice, _uint iSize);
inline CParticle*	Emit_DotParticle( PARTICLE& desc, _uint Count, _uint PassIndex = 0);
inline CParticle*	Emit_ImageParticle( PARTICLE& desc, _uint TextureIndex, _uint PassIndex);
inline CParticle*	Emit_MeshParticle( PARTICLE& desc, const _wstr& tagMesh, _uint PassIndex);
inline void			Reset_Particle(PARTICLE& desc);
inline void			Set_AnimState(PARTICLE& desc, _int iX, _int iY, _float fSpeed = 1.f);
inline void			Reset_AllParticle();





inline void			Stop_All_Particle();

// ProtoMgr
inline HRESULT		Ready_Prototype(const _tchar* pProtoTag, CComponent* pInstance);
inline CComponent*	Clone_Prototype(const _tchar* pProtoTag);


// Renderer
inline void			Add_RenderGroup(RENDERID eGroup, class CGameObject* pGameObject);
inline CRenderer*	Get_Renderer(void);
inline void			Start_Split();
inline void			End_Split();
inline void			Set_CodyGreyScale(_bool greyscale);
inline void			Set_MayGreyScale(_bool greyscale);
inline void			Set_BlendPass(const _int& iPass);




// LightMgr
inline CLight*		Ready_Light(LPDIRECT3DDEVICE9 pGraphicDev, const D3DLIGHT9* pLightInfo, const _uint& iIndex);
inline void			Render_Light(LPD3DXEFFECT& pEffect);
inline _bool		Is_LightExsist();
inline void			Clear_Light();



// RenderTarget
inline HRESULT		Ready_RenderTarget(const _tchar* pTargetTag, LPDIRECT3DDEVICE9 pGraphpicDev, const _uint& iWidth, const _uint& iHeight, D3DFORMAT Format, D3DXCOLOR Color);
inline HRESULT		Ready_MRT(const _tchar* pMRTTag, const _tchar* pTargetTag);

inline HRESULT		Begin_MRT(const _tchar* pMRTTag);
inline HRESULT		End_MRT(const _tchar* pMRTTag);
inline HRESULT		Ready_DebugBuffer(const _tchar* pTargetTag, const _float& fX, const _float& fY, const _float& fSizeX, const _float& fSizeY);
inline void			Render_DebugBuffer(const _tchar* pMRTTag);
inline void			SetUp_OnShader(LPD3DXEFFECT& pEffect, const _tchar* pTargetTag, const char* pConstantTable);
inline void			SetUp_OnRegister(_int iRegisterIndex, const _tchar* pTargetTag);



// Release Utility
inline void			Release_Utility(void);

#include "Export_Utility.inl"

END
#endif // Export_Utility_h__
