#ifndef GravityButton_h__
#define GravityButton_h__

#include "GameObject.h"
#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CPhysXMesh;
class CStaticMesh;
class CShader;

END

class CGroundPoundButton : public Engine::CGameObject
{
private:
	explicit CGroundPoundButton(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CGroundPoundButton(void);

	_bool m_bButton_Click = false;
	void Move(_vec3 vDir, _float fTime, const _float& fTimeDelta, _bool bMoveAll = false);
	void MoveButton(const _float& fTimeDelta, _float fMoveTime);
	void MoveAll(const _float& fTimeDelta, _float fMoveTime);
public:
	virtual HRESULT			Ready_Object(void* pArg = nullptr) override;
	virtual _int			Update_Object(const _float& TimeDelta) override;
	virtual _int			LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void			Render_Object(const _int& iIndex) override;
	virtual void*			On_Active(void* pCaller) override;
	//void					Active();
private:
	HRESULT					Add_Component(void* pArg = nullptr);
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT& pEffect, const _int& iIndex);

public:
	void					Set_Base(_bool bBase) { m_bBase = bBase; }

public:
	_vec3					Get_MoveDir() { return m_vMoveDir; }

private:
	Engine::CStaticMesh*	m_pMeshCom = nullptr;
	Engine::CPhysXMesh*		m_pPhysMesh = nullptr;
	Engine::CShader*		m_pShaderCom = nullptr;

public:
	_bool					Is_Active() { return m_bActive; }
public:

public:
	static CGroundPoundButton*	Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg = nullptr, _bool bBase = true);
	OBJINFO					m_tObjInfo;

	_bool					m_bActive = false;
	_bool					m_bBase = false;
	_float					m_fMoveTime = 0.f;
	PxVec3					m_vStartPos;
	_bool					m_bEnd = false;
	_bool					m_bButtonDown = false;
	_vec3					m_vMoveDir = {};
	_float					m_fTime = 0.0f;

	_bool					m_bButtonMove = false;
	_bool					m_bMoveAll = false;
	_bool					m_bMoveAllEnd = false;
	_vec3					m_vButtonPos = {};
	_bool					m_bButtonMoveEnd = false;

	_vec3					m_vButtonDir = {};
	_vec3					m_vAllDir = {};
	_bool					m_bPlaySound = false;

private:
	virtual void Free(void) override;
};

#endif // __PowerButton_H__
