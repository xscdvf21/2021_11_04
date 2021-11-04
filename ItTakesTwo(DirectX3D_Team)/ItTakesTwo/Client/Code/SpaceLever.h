#ifndef SpaceLever_h__
#define SpaceLever_h__

#include "GameObject.h"
#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CPhysXMesh;
class CStaticMesh;
class CShader;

END

class CSpaceLever : public Engine::CGameObject
{
public:
	enum DIR { LEFT, RIGHT };
	enum PlayerType { CODY, MAY };

private:
	explicit CSpaceLever(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSpaceLever(void);

public:
	virtual HRESULT			Ready_Object(void* pArg = nullptr) override;
	virtual _int			Update_Object(const _float& TimeDelta) override;
	virtual _int			LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void			Render_Object(const _int& iIndex) override;

private:
	HRESULT					Add_Component(void* pArg = nullptr);
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT& pEffect, const _int& iIndex);

private:
	Engine::CStaticMesh*	m_pMeshCom = nullptr;
	Engine::CTransform*		m_pTransformCom = nullptr;
	Engine::CShader*		m_pShaderCom = nullptr;

public:
	void					Reset_State();

public:
	void					Set_Dir(DIR eDir);// { m_eDir = eDir; }
	void					Move(const _float& TimeDelta);
	void					Set_RotTime(_float fRotTime);

public:
	DIR						Get_Dir() { return m_eDir; }
	_vec3					Get_Point() { return m_vPoint; }
	_vec3					Get_LookingAtDir() { return m_vLookingAtDir; }
	_bool					Get_Slap() { return m_bSlap; }
	_vec3					Get_Normal() { return m_vNormal; }

public:
	virtual void* On_Active(void* pCaller) override;
	virtual void* On_Trigger_Active(void* pTrigger, void* pCaller) override;

public:
	void					ReSet() { m_bReSet = true; }

public:
	static CSpaceLever*		Create(LPDIRECT3DDEVICE9 pGraphicDev,  _vec3 vNormal, _vec3 vTargetPos, TRIGGERTYPE eTriggerType, void* pArg = nullptr, PlayerType eType = CODY);
	_bool					m_bSlap = false;
	OBJINFO					m_tObjInfo;
	CTrigger*				m_pTrigger = nullptr;
	DIR						m_eDir;
	_bool					m_bMove = false;
	_float					m_fAngle = 120.0f;
	_matrix					m_matCombinedMatrix;
	_float3					m_vPoint;
	_vec3					m_vLookingAtDir;
	TRIGGERTYPE				m_eTriggerType = TRIGGER_LEVER_CODY;
	_vec3					m_vNormal{};
	_float					m_fRotTime = 0.0f;
	_float					m_fTime = 0.0f;
	_bool					m_bWork = false;
	_bool					m_bComplete = false;
	_bool					m_bReSet = false;
	_vec3					m_vOrgAngle = {};
	PlayerType				m_ePlayer = CODY;
	_float					m_fLimitAngle = 0.0f;
	_vec3					m_vStartPos = {};
	_vec3					m_vStartAngle = {};

private:
	virtual void Free(void) override;
};



#endif // TestWall_h__
