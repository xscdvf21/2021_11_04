#ifndef Camera_h__
#define Camera_h__

#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CCamera : public CGameObject
{
protected:
	explicit CCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCamera(void);

public:
	virtual HRESULT Ready_Object(void* pArg = nullptr) override;
	virtual _int	Update_Object(const _float& TimeDelta) override;

public:
	void			Set_Shake() { m_bShake = true; }
	void			Set_ResetTimer(const _float& fTime, const _bool&  EndSplit);

public:
	_matrix			Get_View() { return m_matView; }
	_matrix			Get_Projection() { return m_matProj; }
	_vec3			Get_CameraPos() { return m_vEye; }
	_vec3			Get_CameraAt() { return m_vAt; }
	_matrix			Get_CameraStateMatrix();
	_vec3			Get_CameraLook();
	_vec3			Get_CameraRight();
	_vec3			Get_CameraUp();
	_matrix			Get_CamMatrix();
	_bool			Get_CameraFix() { return !m_bMouseFix; }
	const _bool&	Is_MoveEnd() { return m_bMoveEnd; }
public:
	void			Set_Fix(const _vec3& At,const _vec3& Eye,const _bool& bTelePort =false);
	virtual void	Set_TargetUp(_vec3 vUp);
	void			Set_Dist(const _float& fDist) { m_fDist = fDist; }
	void			Set_Mode(const CAMERAMODE& eMode);
	void			Set_Mode(const CAMERAMODE& eMode,CGameObject* pTarget);
	void			Set_RayCheck(const _bool& bState = true);
	void			Set_ForceRayCheck(const _bool& bState = true) { m_bForceRay = bState; }
	void			Set_Dir(const _vec3& vDir) { m_vEyeDir = vDir; }
	void			Set_Fov(const _float& fFov) { m_fFovY = fFov; CalculateProj(); }
	void			Set_CameraAt(_vec3 vAt) { m_vAt = vAt; }
	void			Set_CameraEye(_vec3 vEye) { m_vEye = vEye; }
	void			Set_ReachTime(_float fReach) { m_fReachTime = fReach; }
protected:
	void			CalculateProj();
	void			Update_View(const _float & TimeDelta);
	void			Move_To_Target(const _vec3& vTarget, _vec3& vSrc, const _float & TimeDelta);
	HRESULT			Shake(_float fDeltaTime);
protected:
	CAMERAMODE		m_eMode = CAMERA_DEFAULT;
	_bool			m_bRay = true;
	_bool			m_bRot = true;

	_vec3			m_vEye, m_vAt, m_vUp;
	_vec3			m_vTargetEye,m_vTargetAt,m_vTargetUp;
	_float			m_fAspect, m_fFovY, m_fNear, m_fFar;
	_matrix			m_matView, m_matProj;
	_float			m_fReachTime = 0.2f;
	_bool			m_bShake=false;
	_matrix			m_matCameraState;
	_vec3			m_vShake;
	_vec3			m_vFixAt;
	_vec3			m_vFixEye;
	_float3			m_vEyeDir;

	_float			m_fShakeTime;
	_bool			m_bMouseFix = true;
	_bool			m_bFixStart = false;

	CGameObject*	m_pTarget = nullptr;
protected:
	_bool			m_bForceRay =true;
	// 카메라 변환
	_float			m_fRotYRad = 0.0f;
	_float			m_fRotXRad = 0.0f;
	_float			m_fCurXRad = 0.0f;
	_float			m_fCurYRad = 0.0f;
	_float			m_fDirY = 0.0f;
	_float			m_fDirX = 0.0f;
	_float			m_fDist = 12.f;
	_vec3			m_vAngle;

	// 2D카메라
	_bool			m_b2dCamera = false;
	_bool			m_b2DCamRot = false;

	// 3D카메라
	_bool			m_b3dCamera = false;
	_bool			m_b3dCamRot = false;
	
protected:
	virtual void	Free(void);
	void Update_Timer(const _float& fTimeDelta);
private:
	_bool m_bTimer = false;
	_float m_fTime = 0.f;
	_bool m_bEndSplit = false;
	_bool m_bMoveEnd = false;
};

END
#endif // Camera_h__
