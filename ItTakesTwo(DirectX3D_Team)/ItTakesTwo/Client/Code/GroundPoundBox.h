#ifndef GravityBox_h__
#define GravityBox_h__

#include "GameObject.h"
#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CPhysXMesh;
class CStaticMesh;
class CShader;

END

class CGroundPoundBox : public Engine::CGameObject
{
private:
	explicit CGroundPoundBox(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CGroundPoundBox(void);

	_bool m_bButton_Click = false;
	void Move(const _float& fTimeDelta);
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

private:
	Engine::CStaticMesh*	m_pMeshCom = nullptr;
	Engine::CPhysXMesh*		m_pPhysMesh = nullptr;
	Engine::CShader*		m_pShaderCom = nullptr;

public:
	_bool					Is_Active() { return m_bActive; }
public:

public:
	static CGroundPoundBox*	Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg = nullptr, _bool bBase = true);
	OBJINFO					m_tObjInfo;

	_bool					m_bActive = false;
	_bool					m_bBase = false;
	_float					m_fMoveTime = 0.f;
	PxVec3					m_vStartPos;
	_bool					m_bEnd = false;
	_vec3					m_vMoveDir = { 0.0f, 0.0f, 0.0f };

private:
	virtual void Free(void) override;
};

#endif // __PowerButton_H__
