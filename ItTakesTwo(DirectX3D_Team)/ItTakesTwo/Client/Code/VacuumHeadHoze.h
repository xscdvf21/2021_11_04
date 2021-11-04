#pragma once
#ifndef VacuumHeadHoze_h__
#define VacuumHeadHoze_h__

#include "GameObject.h"
#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CDynamicMesh;
class CCalculator;
class CCollider;
class CShader;

END

class CVacuumHeadHoze : public CGameObject
{
private:
	explicit CVacuumHeadHoze(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CVacuumHeadHoze();

public:
	virtual HRESULT						Ready_Object(void* pArg);
	HRESULT								Ready_Object(_uint iDir, _uint iLength, _float fPower, _float fDuration,void* pArg);
	virtual _int						Update_Object(const _float& fTimeDelta) override;
	virtual _int						LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void						Render_Object(const _int& iIndex = 0) override;


	virtual void*						On_Trigger_Active(void* pTrigger, void* pCaller) override;

public:
	virtual	_mat						Get_EventPos() override;
	virtual void						Move_Right();
	virtual void						Move_Left();
	virtual void						Move_Up();

private:
	physx::PxJoint*						createDampedD6(PxRigidActor* a0, const PxTransform& t0, PxRigidActor* a1, const PxTransform& t1);
	void								createChain(const PxTransform& t, PxU32 length, PxReal separation);
private:
	HRESULT								Add_Component(void* pArg, _uint iLength);
	HRESULT								SetUp_ConstantTable(LPD3DXEFFECT& pEffect, const _int& iIndex);

	void								Set_Rotation(const _int& iDir);
	void								Set_Parent(void* pParent) { m_pParent = pParent; }
	void								MoveUp(const _float& fTimeDelta);
	void								MoveUpFront(const _float & fTimeDelta);
	void								MoveDownFront(const _float & fTimeDelta);
	void								MoveDown(const _float& fTimeDelta);
public:
	_bool								Is_Active() { return m_bActive; }
public:
	void								Set_MoveCheck(_bool bMove) { m_bMove = bMove; }


	void								Set_MoveUpCheck(_bool bMove);
	void								Set_MoveUpCheckFront(_bool bMove);
	void								Set_MoveDownCheck(_bool bMove);
	void								Set_MoveDownCheckFront(_bool bMove);
	void								Set_Suck(_bool bState);;
	void								Set_UpForce(const _float& fPower) { m_vUpForce = { 0.f,fPower,0.f }; }
	void								Set_MoveLock(const _uint LockFlag) { m_iMoveLock = LockFlag; }
public:
	_bool								Get_MoveCheck() { return m_bMove; }
	_bool								Get_MoveUpCheck() { return m_bMoveUp; }
	_bool								Get_MoveDownCheck() { return m_bMoveDown; }

	vector<PxRigidDynamic*>				Get_HozeBody() { return m_vecHozeRigid; }
	PxAggregate*						Get_Hoze() { return m_pHoze; }
	vector<_vec3>*						Get_IOPath();
	
	CDynamicMesh*						Get_HoseHead() { return m_pMeshHead; }
	CTransform*							Get_TransformHead() { return m_pTransformHeadCom; }
private:
	Engine::CDynamicMesh*				m_pMeshCom = nullptr;
	Engine::CDynamicMesh*				m_pMeshHead = nullptr;
	Engine::CTransform*					m_pTransformHeadCom = nullptr;
	Engine::CCalculator*				m_pCalculatorCom = nullptr;
	Engine::CCollider*					m_pColliderCom = nullptr;
	Engine::CShader*					m_pShaderCom = nullptr;

private:
	class CBlow*						m_pBlow = nullptr;

	PxAggregate*						m_pHoze = nullptr;
	PxRigidDynamic*						m_pPlane = nullptr;
	vector<PxRigidDynamic*>				m_vecHozeRigid;
	vector<_vec3>						m_vecIOPath;
	vector<PxJoint*>					m_vecHozeJoint;
	PxVec3								m_vDirForce = { 0.f,0.f,1.f };
	PxVec3								m_vRight = { 1.f,0.f,0.f };
	_int								m_iTotalLink = 0;
	CTrigger*							m_pHeadTrigger = nullptr;
	CTrigger*							m_pEnterTrigger = nullptr;
private:
	_bool								m_bSuck = false;
	_bool								m_bActive = false;
private :
	_bool								m_bMove = true;
	_uint								m_iMoveLock = 0;
	PxVec3								m_vUpForce = { 0.f,1.f,0.f };

	void*								m_pParent=nullptr;

	_bool								m_bMoveUpFront = false;
	_bool								m_bMoveUp = false;
	_bool								m_bMoveDown = false;
	_bool								m_bMoveDownFront = false;

	_float								m_fDuration = 2.f;
	_float								m_fPower = 30.f;

private:
	void								Effect(const _float& fTimeDelta);

private:
	_float								m_fEffectTime = 0.f;
	


public:
	static CVacuumHeadHoze*			Create(LPDIRECT3DDEVICE9 pGraphicDev, _uint iDir, _uint iLength = 0, _float fPower = 80.f , _float fDuration=2.f, void* pArg = nullptr, void* Parent = nullptr);
	virtual void						Free(void)		override;

};
#endif // VacuumHeadHoze_h__
