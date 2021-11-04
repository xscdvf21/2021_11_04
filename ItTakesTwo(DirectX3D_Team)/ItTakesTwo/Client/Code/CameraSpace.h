#ifndef __CameraSpace_H__
#define __CameraSpace_H__

#include "GameObject.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Trigger.h"

BEGIN(Engine)

class CTransform;
class CPhysXMesh;

END

class CCameraSpace : public Engine::CGameObject
{
private:
	explicit CCameraSpace(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCameraSpace(void);

	_bool m_bSceneChange = false;
	_bool Fade_Out(const _float& fTimeDelta);
public:
	virtual HRESULT			Ready_Object(void* pArg = nullptr) override;
	virtual _int			Update_Object(const _float& TimeDelta) override;
	virtual _int			LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void			Render_Object(const _int& iIndex) override;


	virtual void*			On_Trigger_Active(void* pTrigger,void* pCaller) override;

private:
	HRESULT					Add_Component(void* pArg = nullptr);

public:

	void					Set_Pos(const _vec3& vPos) { m_vPos = vPos; }
	void					Set_Scale(const _float& fX, const _float& fY, const _float& fZ) { m_vScale = { fX,fY,fZ }; }
	void					Set_Scale(const _float& fHalf) { m_vScale = { fHalf,fHalf,fHalf }; }
	void					Set_Stalking() { m_eCameraMode = CAMERA_FIX; }
	void					Set_Fix(_vec3 vEye, _vec3 vAt);

public:
	_vec3					Get_Eye() { return m_vEye; }
	_vec3					Get_At() { return m_vAt; }
	CAMERAMODE				Get_Mode() { return m_eCameraMode; }


public:
	static CCameraSpace* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vPos, const _float& fX, const _float& fY, const _float& fZ, void* pArg = nullptr);
	static CCameraSpace* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vPos, const _float& fHalf, void* pArg = nullptr);
public:

	OBJINFO					m_tObjInfo;
	CTrigger*				m_pTrigger = nullptr;
	SCENEID					m_eNextScene;
	_vec3					m_vPos;
	_vec3					m_vScale;
	CAMERAMODE				m_eCameraMode = CAMERA_STALKING;
	_vec3					m_vEye;
	_vec3					m_vAt;

private:
	Engine::CShader*		m_pShaderCom = nullptr;

private:
	virtual void Free(void) override;
};



#endif // TestWall_h__
