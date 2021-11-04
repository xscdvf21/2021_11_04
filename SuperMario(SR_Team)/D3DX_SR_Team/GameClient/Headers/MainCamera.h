#pragma once
#ifndef __MAINCAMERA_H__

#include "Camera.h"

USING(Engine)
class CMainCamera final : public CCamera
{
private:
	explicit CMainCamera(LPDIRECT3DDEVICE9 pDevice);
	explicit CMainCamera(const CMainCamera& other);
	virtual ~CMainCamera() = default;


public:
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void* pArg = nullptr) override;
	virtual _uint Update_GameObject(_float fDeltaTime) override;
	virtual _uint LateUpdate_GameObject(_float fDeltaTime) override;
	virtual HRESULT Render_GameObject() override;

private:
	HRESULT KeyInput(_float fDeltaTime);
	HRESULT Movement(_float fDeltaTime);
	 

public :
	static CMainCamera*	Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	virtual void Free() override;

private:
	_float m_fDistanceToTarget = 10.f;	//타겟과의 거리.
	_float m_fCamAngle = 45.f;			//카메라의 앵글각도.

};


#define __MAINCAMERA_H__
#endif // !__MAINCAMERA_H__


