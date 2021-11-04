#ifndef __CRUSHERSTARTDOOR_H__

#include "GameObject.h"
#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CStaticMesh;
class CTransform;
class CCalculator;
class CCollider;
class CShader;

END

class CCrusherStartDoor : public CGameObject
{
private:
	explicit CCrusherStartDoor(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCrusherStartDoor();

public:
	virtual HRESULT		Ready_Object(void* pArg = nullptr);
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual _int		LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void		Render_Object(const _int& iIndex = 0) override;

public:
	void				Set_Close(_bool _bClose) { m_bClose = _bClose; }

private:
	HRESULT				Add_Component(void* pArg = nullptr);
	HRESULT				SetUp_ConstantTable(LPD3DXEFFECT& pEffect, const _int& iIndex);

private:
	void				Move(const _float& fTimeDelta);

private:
	Engine::CStaticMesh*			m_pMeshCom = nullptr;
	Engine::CTransform*				m_pTransformCom = nullptr;
	Engine::CCalculator*			m_pCalculatorCom = nullptr;
	Engine::CCollider*				m_pColliderCom = nullptr;
	Engine::CShader*				m_pShaderCom = nullptr;

private:
	_bool							m_bClose;

	_float							m_fCloseSpeed;
	_float							m_fAngle;
	_float							m_fCurrentAngle;
private:
	_bool							m_bSwitch1;
	_bool							m_bSwitch2;
	_bool							m_bCrusherCreate;


	_bool							m_bSoundStart = false;
private:
	_bool							m_bCreateTrigger;
	CTrigger*						m_pTrigger = nullptr;
public:
	static CCrusherStartDoor*			Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg = nullptr);
	virtual void						Free(void)		override;
};

#define __CRUSHERSTARTDOOR_H__
#endif

