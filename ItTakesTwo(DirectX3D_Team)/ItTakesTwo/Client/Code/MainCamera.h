#ifndef MainCamera_h__
#define MainCamera_h__

#include "Camera.h"
#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

END

class CMainCamera : public Engine::CCamera
{
private:
	explicit	CMainCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual		~CMainCamera(void);

	_vec3 m_vOffset = {0.f,0.f,0.f};
public:
	HRESULT		Ready_Object(const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, const _float& fFovY, const _float& fAspect,	const _float& fNear, const _float& fFar, const _vec3* vCampos, const _float& fDis);
	virtual _int Update_Object(const _float& TimeDelta) override;
	virtual _int LateUpdate_Object(const _float& TimeDelta) override;

public:
	void		Update_View_Proj();

private:
	void		Key_Input(const _float& fTimeDelta);
	void		Fix_Mouse(void);
	void		Update_CameraPos(const _float& fTimeDelta);
	void		Update_CameraDis(const _float& fTimeDelta, _float3 vCodyPos, _float3 vMayPos);
	void		Update_Default(const _float& fTimeDelta);
	void		Update_Fix();
	void		Update_Stalking();
	void		Update_Tracing();
public:
	_matrix		Get_CameraStateMatrix();
	_vec3		Get_CameraLook();
	_vec3		Get_CameraRight();
	_vec3		Get_CameraUp();
	_matrix		Get_CamMatrix();

public:
	void		Set_Offset(const _vec3& vOffset) { m_vOffset = vOffset; }
	void		Set_EyeDir(_vec3 vDir) { m_vEyeDir = vDir; }
	void		Set_DisAdd(_float fDis) { m_fDisAdd = fDis; }
	void		Set_MinDis(_float fDIs) { m_fMinDistance = fDIs; }
private:
	_float		m_fSpeed = 10.f;
	_bool		m_bClick = false;

private:
	_float		m_fViewAngle;
	_float		m_fViewDistance;
	_float		m_fViewHeight;
	_float		m_fViewAtHeight;
	_matrix		m_matCameraState;
private:
	_bool		m_bEyePos = true;

private:
	_float		m_fDistance = 0.0f;
	_vec3		m_vEyeDir;
	_vec3		m_vAngle;

private:
	_float		m_fLength = 10.0f;
	_float		m_fMinDistance = 15.0f;
	_float		m_fMaxDistance = 30.0f;

	_float		m_fDisAdd = 0.f;

public:
	static CMainCamera*	Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, const _float& fFovY, const _float& fAspect, const _float& fNear, const _float& fFar, const _vec3* vCampos, const _float& fDis);

public:
	virtual void Free(void) override;
};

#endif // MainCamera_h__
