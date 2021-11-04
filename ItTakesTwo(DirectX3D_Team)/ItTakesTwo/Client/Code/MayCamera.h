#ifndef __MAYCAMERA_H__
#define __MAYCAMERA_H__

#include "Camera.h"
#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

END

class CMayCamera : public Engine::CCamera
{
private:
	explicit	CMayCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual		~CMayCamera(void);

	void LateUpdate_FreeCamera(const _float & TimeDelta);
	void Update_FreeCamera(const _float& TimeDelta);
public:
	HRESULT		Ready_Object(const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, const _float& fFovY, const _float& fAspect,	const _float& fNear, const _float& fFar);
	virtual _int Update_Object(const _float& TimeDelta) override;
	virtual _int LateUpdate_Object(const _float& TimeDelta) override;

	void		Set_FreeMode();
	void		Set_EyeDir(const _vec3& vDir);
	void		Set_TargetUpRotate(_vec3 param1,_bool bState =true);
private:
	void		Key_Input(const _float& fTimeDelta);
	void		Mouse_Move(const _float& fTimeDelta);
	void		Fix_Mouse(void);
	void		CheckRayHit(const _float & fTimeDelta);

	void		Update_CameraPos();
	void		Update_Default();
	void		Update_Stalking();
	void		Update_Fix();
	void		Update_Tracing();

public:

public:
	void		Set_Slide(_bool slide) { m_bSilde = slide; }
	void		Set_Spetula(_bool spetula) { m_bSpetulaScene = spetula; }
	void		Set_StopCamera(_bool stop) { m_bStopCamera = stop; }
	void		Set_ChangeCharacter(_bool change) { m_bChange = change; }
	void		Set_Mission(_bool mission) { m_bMission = mission; }
	void		Set_EyePos(_bool eye) { m_bEyePos = eye; }
	virtual void	Set_TargetUp(_vec3 vUp) override;

	void		Pitch(const _float& fValue);
	void		Roll(const _float& fValue);
	void		Yaw(const _float& fValue);
private:
	_float		m_fSpeed = 10.f;
	_bool		m_bClick = false;

private:
	_float		m_fViewAngle;
	_float		m_fViewDistance;
	_float		m_fViewHeight;
	_float		m_fViewAtHeight;
	
	_float		m_fOffset = 4.f;

private:
	_bool		m_bSilde;
	_bool		m_bSpetulaScene;

private:
	_bool		m_bStopCamera = false;

private:
	_bool		m_bChange;
	_float		m_fChangeTime;

private:
	_bool		m_bMission = false;
	_bool		m_bEyePos = true;
	_bool		m_bFreeCamera = false;
private:
	_float3		m_vAngle;
	_vec3		xAxis;
	_vec3		yAxis;
	_vec3		zAxis;
	void		Mouse_Rev(const _float & fTimeDelta);

	_float		m_fCorrectAngle = 0.0f;
	_vec3		m_vOffset{};



	_vec3		m_vCamUp = {};
public:
	static CMayCamera*	Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, const _float& fFovY, const _float& fAspect, const _float& fNear, const _float& fFar);

public:
	virtual void Free(void) override;
};

#endif // MainCamera_h__
