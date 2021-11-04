#pragma once

#ifndef __SUPPORT_2D_CAMERA_H__
#define __SUPPORT_2D_CAMERA_H__

#include "Camera.h"

USING(Engine);

class CSupportCamera : public CCamera
{
private:
	explicit CSupportCamera(_Device pDevice);
	explicit CSupportCamera(const CSupportCamera& other);
	virtual ~CSupportCamera() = default;


public:
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void* pArg = nullptr) override;
	virtual _uint Update_GameObject(_float fDeltaTime) override;
	virtual _uint LateUpdate_GameObject(_float fDeltaTime) override;
	virtual HRESULT Render_GameObject() override;
	void Set_Scene(ESceneType eSceneType) { m_eSceneType = eSceneType; }

public :
	void Cam_Rotate_LEFT();
	void Cam_Rotate_RIGHT();

private :
	HRESULT KeyInput(_float fDeltaTime);
	HRESULT Movement(_float fDeltaTime);

public:
	static CSupportCamera* Create(_Device pDevice);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;


	ESceneType m_eSceneType;

	_float m_fSetCameraRotate;				//세팅된 카메라 회전값
	_float m_fNowCameraRotate;				//현재 카메라 회전값

};

#endif