#ifndef __CRUSHERBRIDGE_H__

#include "GameObject.h"
#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CStaticMesh;
class CCalculator;
class CCollider;
class CShader;

END

class CCrusherBridge : public CGameObject
{
private:
	explicit CCrusherBridge(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCrusherBridge();

public:
	virtual HRESULT		Ready_Object(void* pArg = nullptr);
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual _int		LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void		Render_Object(const _int& iIndex = 0) override;

public :
	void				Set_Move() { m_bMove = true; }
private:
	HRESULT				Add_Component(void* pArg = nullptr);
	HRESULT				SetUp_ConstantTable(LPD3DXEFFECT& pEffect, const _int& iIndex);

private:
	void				Move(const _float& fTimeDelta);

private:
	Engine::CStaticMesh*			m_pMeshCom = nullptr;
	Engine::CCalculator*			m_pCalculatorCom = nullptr;
	Engine::CCollider*				m_pColliderCom = nullptr;
	Engine::CShader*				m_pShaderCom = nullptr;

private:
	_bool							m_bMove;
	_bool							m_bCrusherDead; //한번만 하기위해,
	_uint							m_iHitPointCount; 

	_float							m_fTurnSpeed;
	_float							m_fAngle;
	_float							m_fCurrentAngle;

	_bool							m_bSoundStart = false;
public:
	static CCrusherBridge*			Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg = nullptr);
	virtual void					Free(void)		override;

};

#define __CRUSHERBRIDGE_H__
#endif

