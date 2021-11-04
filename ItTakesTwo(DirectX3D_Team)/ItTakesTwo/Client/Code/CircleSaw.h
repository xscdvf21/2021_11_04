#ifndef CircleSaw_h__

#include "GameObject.h"
#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CDynamicMesh;
class CCalculator;
class CCollider;
class CShader;

END

class CCircleSaw : public CGameObject
{

private:
	explicit		 CCircleSaw(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual			~CCircleSaw();

public:
	virtual HRESULT Ready_Object(void * pArg, _bool bRun = false);
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual _int	LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void	Render_Object(const _int& iIndex = 0) override;

private:
	HRESULT			Add_Component(void* pArg);
	HRESULT			SetUp_ConstantTable(LPD3DXEFFECT& pEffect, const _int& iIndex);

public:
	_vec3			Get_AttachCodyPos() { return m_vAttachCodyPos; }
	_vec3			Get_AttachMayPos() { return m_vAttachMayPos; }
	_bool			Get_Move() { return m_bMove; }
	_bool			Get_Stop() { return m_bStop; }
	_bool			Get_Word() { return m_bWork; }

public:
	void			Set_May(_bool bMay) { m_bMay = bMay; }
	void			Set_Cody(_bool bCody) { m_bCody = bCody; }

public:
	void*			On_Active(void* pCaller) override;

private:
	Engine::CDynamicMesh*		m_pMeshCom = nullptr;
	Engine::CCalculator*		m_pCalculatorCom = nullptr;
	Engine::CCollider*			m_pColliderCom = nullptr;
	Engine::CShader*			m_pShaderCom = nullptr;

private :
	CTrigger*					m_pTrigger = nullptr;
	_bool						m_bMove = false;
	_bool						m_bGravity = false;
	_float						m_fTimeDelta = 0.0f;
	_float						m_fSpeed = 0.0f;
	_bool						m_bWork = false;
	_vec3						m_vAttachCodyPos = { -71.87f, -5.67f, -31.15f };
	_vec3						m_vAttachMayPos = { -71.87f, -5.67f, -30.0f };;
	_vec3						m_vGoalPos = { -24.0f, 42.82f, -32.58f };
	_bool						m_bStop = false;
	_bool						m_bMay = false;
	_bool						m_bCody = false;
	_bool						m_bMovestart = false;
	_bool						m_bPlaySound = false;
	_bool						m_bPlaySoundWork = false;

public:
	static CCircleSaw*		Create(LPDIRECT3DDEVICE9 pGraphicDev, void * pArg, _bool bRun = false);
	virtual void Free(void)		override;
};


#define CircleSaw_h__
#endif
