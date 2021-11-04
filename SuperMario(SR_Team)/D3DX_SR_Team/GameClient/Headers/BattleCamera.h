#pragma once

#ifndef __BATTLE_CAMERA_H__
#define __BATTLE_CAMERA_H__

#include "Camera.h"

USING(Engine);

class CBattleCamera : public CCamera
{
private:
	explicit CBattleCamera(_Device pDevice);
	explicit CBattleCamera(const CBattleCamera& other);
	virtual ~CBattleCamera() = default;

public :
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void* pArg = nullptr) override;
	virtual _uint Update_GameObject(_float fDeltaTime) override;
	virtual _uint LateUpdate_GameObject(_float fDeltaTime) override;
	virtual HRESULT Render_GameObject() override;

public :
	void Cam_Rotate_CW();
	void Cam_Rotate_CCW();
	void Cam_Move_Zoom();
	void Cam_Move_Release();

private:
	HRESULT KeyInput(_float fDeltaTime);
	HRESULT Movement(_float fDeltaTime);

public:
	static CBattleCamera* Create(_Device pDevice);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
	
private:
	class CTransform* m_pTransformCom = nullptr;
	_float m_fDistanceToTarget = 15.f;		//거리
	_float m_fCamAngle = 45.f;				//Asp

	_float m_fSetCameraRotate;				//세팅된 카메라 회전값
	_float m_fNowCameraRotate;				//현재 카메라 회전값

};


#endif // !__BATTLE_CAMERA_H__