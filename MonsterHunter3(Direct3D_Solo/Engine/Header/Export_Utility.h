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
#include "Optimization.h"

BEGIN(Engine)

// Management
// Get
inline CComponent*		Get_Component(const _tchar* pLayerTag, const _tchar* pObjTag, const _tchar* pComponentTag, COMPONENTID eID);
inline CGameObject*		Get_GameObject(const _tchar* pLayerTag, const _tchar* pObjTag);
inline CLayer*			Get_Layer(const _tchar* pLayerTag);


// Set
// General
inline HRESULT			Create_Management(CManagement** ppManagement);
inline  HRESULT			Set_Scene(CScene* pScene);
inline _int				Update_Scene(const _float& TimeDelta);
inline void				Render_Scene(LPDIRECT3DDEVICE9& pGraphicDev);

// ProtoMgr
// Get
// Set
// General
inline HRESULT			Ready_Prototype(const _tchar* pProtoTag, CComponent* pInstance);
inline CComponent*		Clone_Prototype(const _tchar* pProtoTag);

// Renderer
// Get
inline CRenderer*		Get_Renderer(void);

// LightMgr
// Get
// Set
// General
inline HRESULT		Ready_Light(LPDIRECT3DDEVICE9 pGraphicDev, const D3DLIGHT9* pLightInfo, const _uint& iIndex);


// Release Utility
inline void		Release_Utility(void);

#include "Export_Utility.inl"

END
#endif // Export_Utility_h__
