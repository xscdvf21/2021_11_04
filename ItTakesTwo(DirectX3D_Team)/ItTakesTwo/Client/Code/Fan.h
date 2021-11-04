#ifndef __FAN_H__
#define __FAN_H__

#include "GameObject.h"
#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CPhysXMesh;
class CStaticMesh;
class CShader;

END

class CFan : public Engine::CGameObject
{
private:
	explicit CFan(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CFan(void);

public:
	virtual HRESULT			Ready_Object(void* pArg = nullptr) override;
	virtual _int			Update_Object(const _float& TimeDelta) override;
	virtual _int			LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void			Render_Object(const _int& iIndex) override;


	virtual void* On_Active(void* pCaller) override;

private:
	HRESULT					Add_Component(void* pArg = nullptr);
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT& pEffect, const _int& iIndex);

private:
	Engine::CStaticMesh*	m_pMeshCom = nullptr;

	Engine::CStaticMesh*	m_pBodyMeshCom = nullptr;
	Engine::CStaticMesh*	m_pHolderMeshCom = nullptr;
	Engine::CTransform*		m_pBodyTransformCom = nullptr;

	Engine::CPhysXMesh*		m_pPhysMesh = nullptr;
	Engine::CShader*		m_pShaderCom = nullptr;

public:
	void					Set_Up(_bool bUp) { m_bUp = bUp; }

public:
	_bool					Get_Up() { return m_bUp; }

public:
	static CFan*			Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg = nullptr);
	OBJINFO					m_tObjInfo;
	CTrigger*				m_pTrigger = nullptr;
	_bool					m_bUp = false;
	class CBlow*			m_pBlow = nullptr;
	_float					m_fEffectTime = 0.f;

private:
	virtual void Free(void) override;
};

#endif // __FAN_H__
