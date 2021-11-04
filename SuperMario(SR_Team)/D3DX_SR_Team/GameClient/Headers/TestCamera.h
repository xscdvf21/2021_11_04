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
	_float m_fDistanceToTarget = 2.f;		//�Ÿ�
	_float m_fCamAngle = 45.f;				//Asp


	//_float3 m_vAt;							//ī�޶��� ������ (�÷��̾� ��ġ�����)
	//_float3 m_vRight;						//ī�޶� Right
	//_float3 m_vLookAt;						//ī�޶� �ٶ󺸰� �ִ� ����
};

#endif // !__TESTCAMERA_H__