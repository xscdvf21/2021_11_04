#ifndef __SpacePowerButton_H__
#define __SpacePowerButton_H__

#include "GameObject.h"
#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CPhysXMesh;
class CStaticMesh;
class CShader;

END

class CSpacePowerButton : public Engine::CGameObject
{
private:
	explicit CSpacePowerButton(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSpacePowerButton(void);

public:
	virtual HRESULT			Ready_Object(void* pArg = nullptr) override;
	HRESULT					Ready_Object(void* pArg, const _int& iInteractableSize);
	virtual _int			Update_Object(const _float& TimeDelta) override;
	virtual _int			LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void			Render_Object(const _int& iIndex) override;
	virtual void*			On_Active(void* pCaller) override;
	//void					Active();
	void Move(const _float& fTimeDelta);

private:
	HRESULT					Add_Component(void* pArg = nullptr);
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT& pEffect, const _int& iIndex);

public:
	void					Set_Base(_bool bBase) { m_bBase = bBase; }

private:
	Engine::CStaticMesh*	m_pMeshCom = nullptr;

	Engine::CStaticMesh*	m_pBodyMeshCom = nullptr;
	Engine::CStaticMesh*	m_pHolderMeshCom = nullptr;
	Engine::CTransform*		m_pBodyTransformCom = nullptr;

	Engine::CPhysXMesh*		m_pPhysMesh = nullptr;
	Engine::CShader*		m_pShaderCom = nullptr;

public:
	_bool					Is_Active() { return m_bActive; }
public:

public:
	static CSpacePowerButton*	Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg = nullptr, const _int& iInteractableSize = 1, _bool bBase = true);
	OBJINFO					m_tObjInfo;

	_bool					m_bButton_Click = false;
	_bool					m_bActive = false;
	_bool					m_bBase = false;
	_float					m_fMoveTime = 0.f;
	PxVec3					m_vStartPos;
	_int					m_iInteractableSize = 1;

private:
	virtual void Free(void) override;
};

#endif // __SpacePowerButton_H__
