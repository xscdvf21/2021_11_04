#pragma once
#ifndef __CAMERA_H__
#define __CAMERA_H__
#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CCamera abstract : public CGameObject
{

public :
	typedef struct tagCameraDescription
	{
		_mat matView;
		_float3 vEye, vAt, vUp;

		_mat matProj;
		_float fFovY, fAspect, fZNear, fZFar;

		_mat matOrtho;
	}CAMERA_DESC;

protected:
	explicit CCamera(LPDIRECT3DDEVICE9 pDevice);
	explicit CCamera(const CCamera& other);
	virtual ~CCamera() = default;

public:
	const CAMERA_DESC& Get_CameraDesc() const;

public:
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void* pArg = nullptr) override;
	virtual _uint Update_GameObject(_float fDeltaTime) override;
	virtual _uint LateUpdate_GameObject(_float fDeltaTime) override;
	virtual HRESULT Render_GameObject() override;

	void Set_Ortho();
	void Set_Ortho_For_UI(_float3 vScale, _float3 vPos);

	_float3 Get_AtVector();

public:
	virtual CGameObject* Clone(void* pArg = nullptr) PURE;
	virtual void Free() override;

protected:
	CAMERA_DESC	m_CameraDesc;


};

END

#endif