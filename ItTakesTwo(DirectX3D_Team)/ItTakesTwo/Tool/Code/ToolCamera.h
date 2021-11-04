#ifndef ToolCamera_h__
#define ToolCamera_h__

#include "Camera.h"
#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

END

class CToolCamera : public Engine::CCamera
{
private:
	explicit	CToolCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual		~CToolCamera(void);

public:
	HRESULT		Ready_Object(const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, const _float& fFovY, const _float& fAspect,	const _float& fNear, const _float& fFar);
	virtual _int Update_Object(const _float& TimeDelta) override;

private:
	void		Key_Input(const _float& fTimeDelta);
	void		Mouse_Move(const _float& fTimeDelta);
	void		Fix_Mouse(void);

private:
	_float		m_fSpeed = 10.f;
	_bool		m_bClick = false;
	_bool		m_bMouseFix = true;

public:
	static CToolCamera*	Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, const _float& fFovY, const _float& fAspect, const _float& fNear, const _float& fFar);

public:
	virtual void Free(void) override;
};

#endif // ToolCamera_h__
