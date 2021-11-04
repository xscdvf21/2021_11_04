#pragma once
#ifndef __OPENING_SCENE_CAMERA_H__
#define __OPENING_SCENE_CAMERA_H__

#include "Camera.h"

USING(Engine);
//직교투영 카메라
class COSCamera : public CCamera
{
private:
	explicit COSCamera(_Device pDevice);
	explicit COSCamera(const COSCamera& other);
	virtual ~COSCamera() = default;


public:
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void* pArg = nullptr) override;
	virtual _uint Update_GameObject(_float fDeltaTime) override;
	virtual _uint LateUpdate_GameObject(_float fDeltaTime) override;
	virtual HRESULT Render_GameObject() override;

public:
	static COSCamera* Create(_Device pDevice);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;



};


#endif // !__OPENING_SCENE_CAMERA_H__