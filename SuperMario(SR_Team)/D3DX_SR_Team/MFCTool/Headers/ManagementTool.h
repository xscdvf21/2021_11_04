#pragma once

#include "../../Reference/Headers/Base.h"
#include "GraphicDevice.h"
#include "Renderer.h"

#include "GameObjMgr.h"
#include "GameObj.h"
#include "ComponentMgr.h"

#include "TimeMgr.h"

USING(Engine)
class CManagementTool :
	public CBase
{
	DECLARE_SINGLETON(CManagementTool)

private:
	CManagementTool();
	virtual ~CManagementTool() = default;

public: /*For.General*/
	HRESULT Ready_Engine(HWND hWnd, int iWinCX, int iWinCY, EDisplayMode eDisplayMode, _uint iSceneCount);
	_uint Update_Engine();
	HRESULT Render_Engine(HWND hWnd = nullptr);

public: /*For.Graphic_Device*/
	_Device	Get_Device();

public: /*For.GameObject_Manager*/
	class CGameObject* Get_GameObject(_uint iIndex = 0);
	class CComponent* Get_Component(const wstring& ComponentTag, _uint iIndex = 0);
	HRESULT Add_GameObject_Prototype( const wstring& PrototypeTag, class CGameObj* pPrototype);
	HRESULT Clone_GameObject_ToLayer( const wstring& PrototypeTag, void* pArg = nullptr);

	CGameObj * Get_Clone_GameObject(const wstring & PrototypeTag, void * pArg = nullptr);

public: /*For.Component_Manager*/
	HRESULT Add_Component_Prototype( const wstring& PrototypeTag, CComponentTool* pPrototype);
	CComponentTool* Clone_Component(const wstring& PrototypeTag, void* pArg = nullptr);

public:
	virtual void Free() override;
	static void Release_Engine();

private:
	HRESULT Ready_Camera();
	HRESULT Update_Camera(_float fDeltaTime);

private:
	CGraphicDevice* m_pDeviceManager = nullptr;
	//CRenderer* m_pRenderer = nullptr;

	LPDIRECT3DDEVICE9 m_pDevice = nullptr;

private:
	_float4x4 m_matProj;
	_float4x4 m_matView;
	_float3 m_vEye = {}, m_vAt = {}, m_vUp = {};

	_float m_fAngle = 0.f;
};

