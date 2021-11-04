#pragma once

#ifndef __TESTCAMERA_H__
#define __TESTCAMERA_H__

#include "Camera.h"

USING(Engine)

class CTestCamera : public CCamera
{
private:
	explicit CTestCamera(_Device pDevice);
	explicit CTestCamera(const CTestCamera& other);
	virtual ~CTestCamera() = default;

public:
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void* pArg = nullptr) override;
	virtual _uint Update_GameObject(_float fDeltaTime) override;
	virtual _uint LateUpdate_GameObject(_float fDeltaTime) override;
	virtual HRESULT Render_GameObject() override;

private:
	HRESULT KeyInput(_float fDeltaTime);
	HRESULT Movement(_float fDeltaTime);

public:
	static CTestCamera* Create(_Device pDevice);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;

private:
	_float m_fDistanceToTarget = 2.f;		//거리
	_float m_fCamAngle = 45.f;				//Asp


	//_float3 m_vAt;							//카메라의 기준점 (플레이어 위치라던가)
	//_float3 m_vRight;						//카메라 Right
	//_float3 m_vLookAt;						//카메라가 바라보고 있는 방향
};

#endif // !__TESTCAMERA_H__