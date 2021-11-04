#ifndef __CODYCAMERA_H__
#define __CODYCAMERA_H__

#include "Camera.h"
#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

END

class CCodyCamera : public Engine::CCamera
{
private:
	explicit	CCodyCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual		~CCodyCamera(void);

	void Update_Default();
	void Update_Stalking();
	void Update_Fix();
	void Update_Tracing();
public:
	HRESULT		 Ready_Object(const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, const _float& fFovY, const _float& fAspect,	const _float& fNear, const _float& fFar);
	virtual _int Update_Object(const _float& TimeDelta) override;
	virtual _int LateUpdate_Object(const _float& TimeDelta) override;

private:
	void		Key_Input(const _float& fTimeDelta);
	void		Fix_Mouse(void);
	void		Update_CameraPos();
	void		CamView(const _float & fTimeDelta);

	void		Update3dView(const _float & fTimeDelta);
	void		Update2dView(const _float & fTimeDelta);

public:
	void		Change2dView(_float3 vGoalDir);
	void		Change3dView();
	void		CheckRayHit(const _float & fTimeDelta);

public:


public:
	void		Set_Slide(_bool slide) { m_bSilde = slide; }
	void		Set_Spetula(_bool spetula) { m_bSpetulaScene = spetula; }
	void		Set_Shake() { m_bShake = true; }
	void		Set_StopCamera(_bool stop) { m_bStopCamera = stop; }
	void		Set_ChangeCharacter(_bool change) { m_bChange = change; }

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
	_vec3		m_vAngle;
	
	void		Mouse_Rev(const _float & fTimeDelta);

public:
	static CCodyCamera*	Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, const _float& fFovY, const _float& fAspect, const _float& fNear, const _float& fFar);

public:
	virtual void Free(void) override;

private:


};

#endif // MainCamera_h__
