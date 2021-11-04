#ifndef __VACUUMFIELDPART_H__

#include "GameObject.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Blow.h"

BEGIN(Engine)

class CStaticMesh;
class CCalculator;
class CCollider;
class CShader;

END

class CVacuumFieldPart : public CGameObject
{
private:
	explicit CVacuumFieldPart(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CVacuumFieldPart();

public:
	HRESULT Ready_Object(void* pArg);
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual _int	LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void	Render_Object(const _int& iIndex = 0) override;

private:
	HRESULT			Add_Component(void* pArg);
	HRESULT			SetUp_ConstantTable(LPD3DXEFFECT& pEffect, const _int& iIndex);

	void			MoveUp(const _float& fTimeDelta);
	void			MoveDown(const _float& fTimeDelta);

	PxJoint*							createDampedD6(PxRigidActor* a0, const PxTransform& t0, PxRigidActor* a1, const PxTransform& t1);
	void								createChain(const PxTransform& t, PxU32 length, PxReal separation);

public:
	_bool			Get_MoveUpCheck() { return m_bMoveUp; }
	_bool			Get_MoveDownCheck() { return m_bMoveDown; }

	void			Set_MoveUpCheck(_bool bMove) { m_bMoveUp = bMove; }
	void			Set_MoveDownCheck(_bool bMove) { m_bMoveDown = bMove; }

private:
	Engine::CStaticMesh*		m_pMeshCom = nullptr;
	Engine::CCalculator*		m_pCalculatorCom = nullptr;
	Engine::CCollider*			m_pColliderCom = nullptr;
	Engine::CShader*			m_pShaderCom = nullptr;

	Engine::CTransform*					m_pTransformHeadCom = nullptr;
	Engine::CDynamicMesh*				m_pMeshHead = nullptr;

	CBlow*								m_pBlow = nullptr;

	PxAggregate*						m_pHoze = nullptr;
	vector<PxRigidDynamic*>				m_vecHozeRigid;
	vector<_vec3>						m_vecIOPath;
	PxVec3								m_vDirForce = { 0.f,0.f,1.f };
	PxVec3								m_vRight = { 1.f,0.f,0.f };
	_int								m_iTotalLink = 0;
	_int								m_iDir = DIRUP;
	CTrigger*							m_pHeadTrigger = nullptr;

private:
	_bool						m_bMoveUp = false;
	_bool						m_bMoveDown = false;

public:
	static CVacuumFieldPart*	Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg=nullptr);
	virtual void Free(void)		override;
};
#define __VACUUMFIELDPART_H__
#endif

