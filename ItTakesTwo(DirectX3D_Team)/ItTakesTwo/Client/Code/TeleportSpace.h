#ifndef __TeleportSpace_H__
#define __TeleportSpace_H__

#include "GameObject.h"
#include "Base.h"
#include "Engine_Define.h"
#include "ImageUI.h"

BEGIN(Engine)

class CTransform;
class CPhysXMesh;

END

class CTeleportSpace : public Engine::CGameObject
{
private:
	explicit CTeleportSpace(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTeleportSpace(void);

	_bool					m_bSceneChange = false;
	_bool					Fade_Out(const _float& fTimeDelta);
public:
	virtual HRESULT			Ready_Object(void* pArg = nullptr) override;
	virtual _int			Update_Object(const _float& TimeDelta) override;
	virtual _int			LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void			Render_Object(const _int& iIndex) override;

	virtual void*			On_Trigger_Active(void* pTrigger, void* pCaller) override;




	virtual void*			On_Active(void* pCaller) override;

private:
	HRESULT					Add_Component(void* pArg = nullptr);

public:
	void					Set_TargetPos(const _vec3& vPos) { m_vTargetPos = vPos; }
	void					Set_Pos(const _vec3& vPos) { m_vPos = vPos; }
	void					Set_Scale(const _float& fX, const _float& fY, const _float& fZ) { m_vScale = { fX,fY,fZ }; }
	void					Set_Scale(const _float& fHalf) { m_vScale = { fHalf,fHalf,fHalf }; }
	void					Set_Active() { m_bActive = true; }

public:
	static CTeleportSpace* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vTargetPos, const _vec3& vPos, const _float& fX, const _float& fY, const _float& fZ, void* pArg = nullptr);
	static CTeleportSpace* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vTargetPos, const _vec3& vPos, const _float& fHalf,void* pArg = nullptr);
public:
	CImageUI*				m_pUI = nullptr;
public:
	_vec3 m_vTargetPos;
	OBJINFO					m_tObjInfo;
	CTrigger*				m_pTrigger = nullptr;
	_bool					m_bActive = false;
	_vec3					m_vPos;
	_vec3					m_vScale;
	_bool					m_bEnd = true;
private:
	virtual void Free(void) override;
};



#endif // TestWall_h__
