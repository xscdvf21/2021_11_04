#ifndef MenuCamera_h__
#define MenuCamera_h__

#include "Camera.h"
#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

END

class CMenuCamera : public Engine::CCamera
{
private:
	explicit	CMenuCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual		~CMenuCamera(void);

public:
	HRESULT		Ready_Object(const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, const _float& fFovY, const _float& fAspect, const _float& fNear, const _float& fFar);
	virtual _int Update_Object(const _float& TimeDelta) override;
	virtual _int LateUpdate_Object(const _float& TimeDelta) override;

private:
	void		Mouse_Move(const _float& fTimeDelta);
	void		Fix_Mouse(void);
	HRESULT		Movement(_float fDeltaTime);

public:
	_matrix		Get_CameraStateMatrix();
	_vec3		Get_CameraLook();
	_vec3		Get_CameraRight();
	_vec3		Get_CameraUp();

	void		Set_EyePos(_bool eye) { m_bEyePos = eye; }

private:
	_float		m_fSpeed = 10.f;
	_bool		m_bClick = false;
	_bool		m_bMouseFix = true;

private:
	_float		m_fViewAngle;
	_float		m_fViewDistance;
	_float		m_fViewHeight;
	_float		m_fViewAtHeight;
	_matrix		m_matCameraState;

private:
	_bool		m_bStopCamera = false;

private:
	_bool		m_bChange;
	_float		m_fChangeTime;

private:
	_bool		m_bMission = false;
	_bool		m_bEyePos = true;

public:
	static CMenuCamera*	Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, const _float& fFovY, const _float& fAspect, const _float& fNear, const _float& fFar);

public:
	virtual void Free(void) override;
};

#endif // MenuCamera_h__
