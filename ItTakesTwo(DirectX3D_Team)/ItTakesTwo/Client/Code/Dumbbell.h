#ifndef Dumbbell_h__
#define Dumbbell_h__

#include "GameObject.h"
#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CTransform;
class CStaticMesh;
class CShader;

END

class CDumbbell : public Engine::CGameObject
{
public:
	enum DUMBBELLPLAYER { DUMBBELL_MAY, DUMBBELL_CODY, };
private:
	explicit CDumbbell(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CDumbbell(void);

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
	_vec3					Get_Target1() { return m_vTargetPos1; }
	_vec3					Get_Target2() { return m_vTargetPos2; }
	_vec3					Get_Dir() { return m_vDir; }
	_bool					Get_Move() { return m_bMove; }

public:
	void					Move(_float fTimeDelta, DUMBBELLPLAYER ePlayer);

public:
	static CDumbbell*		Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg = nullptr);
private:
	CTrigger*				m_pTrigger1 = nullptr;
	CTrigger*				m_pTrigger2 = nullptr;

	_vec3					m_vTargetPos1{};
	_vec3					m_vTargetPos2{};
	_vec3					m_vTriggerPos1{};
	_vec3					m_vTriggerPos2{};

	OBJINFO					m_tObjInfo;
	_float					m_fRotSpeed;

	_bool					m_bMoveCody = false;
	_bool					m_bMoveMay = false;
	_bool					m_bMove = false;

	_vec3					m_vDir{};

private:
	virtual void Free(void) override;
};

#endif // __FAN_H__
