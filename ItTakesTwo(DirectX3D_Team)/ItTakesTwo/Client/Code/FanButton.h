#ifndef FanButton_h__
#define FanButton_h__

#include "GameObject.h"
#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CStaticMesh;
class CShader;

END

class CFanButton : public Engine::CGameObject
{
private:
	explicit CFanButton(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CFanButton(void);

public:
	virtual HRESULT			Ready_Object(void* pArg = nullptr) override;
	virtual _int			Update_Object(const _float& TimeDelta) override;
	virtual _int			LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void			Render_Object(const _int& iIndex) override;


	virtual void*			On_Active(void* pCaller) override;

private:
	HRESULT					Add_Component(void* pArg = nullptr);
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT& pEffect, const _int& iIndex);

public:
	void					Set_AddAngle(_float fAddAngle) { m_fAddAngle = fAddAngle; }
	void					Set_Move(_bool bMove, _float fDir) { m_bMove = bMove, m_fDir = fDir; }
	void					Set_Switch() {  }

private:
	Engine::CStaticMesh*	m_pMeshCom = nullptr;
	Engine::CStaticMesh*	m_pBaseMeshCom = nullptr;
	Engine::CTransform*		m_pBaseTransform = nullptr;
	Engine::CShader*		m_pShaderCom = nullptr;
	CTrigger*				m_pTrigger = nullptr;

private:
	OBJINFO					m_tObjInfo;
	_bool					m_bSwitchOn = false;
	_bool					m_bSwitch = false;
	_float					m_fAddAngle = 0.0f;
	_bool					m_bMove = false;
	_float					m_fRotSpeed = 0.0f;
	_float					m_fDir = 0.0f;
	_vec3					m_vAddDir{};

public:
	static CFanButton*		Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg = nullptr);

private:
	virtual void Free(void) override;
};

#endif // __FAN_H__
