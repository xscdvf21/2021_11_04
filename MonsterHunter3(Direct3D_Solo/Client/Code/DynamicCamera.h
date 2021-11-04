#ifndef DynamicCamera_h__
#define DynamicCamera_h__

#include "Camera.h"
#include "Base.h"
#include "define.h"

BEGIN(Engine)

class CTransform;

END

class CDynamicCamera : public Engine::CCamera
{
private:
	explicit	CDynamicCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual		~CDynamicCamera(void);



public:
	HRESULT Ready_Object(const _vec3* pEye, const _vec3* pAt, const _vec3* pUp,
							const _float& fFovY, const _float& fAspect,
							const _float& fNear, const _float& fFar);
	virtual _int Update_Object(const _float& TimeDelta) override;

public :
	void			Camera_Shaking(const _float& TimeDelta);
	void			BossCamera_Shaking(const _float& TimeDelta);
	void			Set_Shake(_bool _bCheck) { m_bShake = _bCheck; }

private:
	void			Key_Input(const _float& fTimeDelta);
	void			Mouse_Move(const _float& fTimeDelta);
	void			Fix_Mouse(void);

	


	HRESULT			Add_Component();

private:

	CTransform*		m_pTransformCom = nullptr;

	

	_float			m_fSpeed = 10.f;
	_bool			m_bClick = false;
	_bool			m_bMouseFix = true;

	_vec3			vCameraLook;
	_vec3			vCameraRight;
	
	_vec3			m_pPlayerPos;
	_vec3			m_pPlayerStart = { 0.f, 2.f, 0.f };
	_vec3			vPlayerDis;
	_float			m_fCamAngle = 90.f;
	_float			m_fDistanceToTarget = 8.f;
	_float			fAngleMax = 0.f;

	_bool			m_bShakeTemp2 = false;
	_bool			m_bShake2 = false;
	_float			m_fShakeTime2 = 0.f;

	_bool			m_bShakeTemp = false;
	_bool			m_bShake = false;
	_float			m_fShakeTime = 0.f;
	_vec3			m_vCurrentEye = { 0.f, 0.f, 0.f };

public:
	static CDynamicCamera*	Create(LPDIRECT3DDEVICE9 pGraphicDev, 
									const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, 
									const _float& fFovY, const _float& fAspect,
									const _float& fNear, const _float& fFar);

public:
	virtual void Free(void) override;
};

#endif // DynamicCamera_h__
