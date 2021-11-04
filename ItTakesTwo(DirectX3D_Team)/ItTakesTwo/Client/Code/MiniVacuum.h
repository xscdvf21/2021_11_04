#ifndef __MiniVacuum_H__

#include "GameObject.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Blow.h"

BEGIN(Engine)

class CStaticMesh;
class CDynamicMesh;
class CShader;

END

class CMiniVacuum : public Engine::CGameObject
{
public:
	explicit							 CMiniVacuum(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual								~CMiniVacuum();

	void Set_Suck(bool bState);

	virtual void* On_Trigger_Active(void* pTrigger,void* pCaller) override;

public:
	HRESULT								Ready_Object(_uint iDir, _uint iLength, void* pArg /*= nullptr*/);
	virtual _int						Update_Object(const _float& fTimeDelta) override;
	virtual _int						LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void						Render_Object(const _int& iIndex = 0) override;

public:
	PxArticulationReducedCoordinate*	Get_Articulation() { return m_pArticulation; }
	vector<PxRigidDynamic*>				Get_HozeBody() { return m_vecHozeRigid; }
	PxAggregate*						Get_Hoze() { return m_pHoze; }
	vector<_vec3>*						Get_IOPath();
	CDynamicMesh*						Get_HoseHead() { return m_pMeshHead; }
	CTransform*							Get_TransformHead() { return m_pTransformHeadCom; }
	virtual _mat						Get_EventPos() override;

public:
	void								Move_Right();
	void								Move_Left();
	void								Move_Up();
	void								Set_HeadPos(const _int& iIndex) { m_iHeadPos = iIndex; }
	
public:
	void								Set_Rotation(const _int& iDir);

	void								Set_UpForce(const _float& fPower) { m_vUpForce = { 0.f,fPower,0.f }; }
private:

	HRESULT								Add_Component(void* pArg, _uint iLength);

	HRESULT								SetUp_ConstantTable(LPD3DXEFFECT& pEffect, const _int& iIndex);
private:


	PxJoint*							createDampedD6(PxRigidActor* a0, const PxTransform& t0, PxRigidActor* a1, const PxTransform& t1);
	void								createChain(const PxTransform& t, PxU32 length, PxReal separation);
private:
	Engine::CDynamicMesh*				m_pMeshHub = nullptr;
	Engine::CDynamicMesh*				m_pMeshHead = nullptr;
	Engine::CDynamicMesh*				m_pMeshHoze = nullptr;

	Engine::CTransform*					m_pTransformHeadCom = nullptr;
	Engine::CShader*					m_pShaderCom = nullptr;
	PxArticulationLink*					m_pLink = nullptr;

	vector<PxRigidDynamic*>				m_vecHozeRigid;
	PxArticulationReducedCoordinate*	m_pArticulation = nullptr;
	PxArticulationCache*				m_pCache =nullptr;
	PxAggregate*						m_pHoze = nullptr;
	CBlow*								m_pBlow = nullptr;
	_bool								m_bSuck = false;
	_bool								m_bBlow = true;
	vector<_vec3>						m_vecIOPath;

	PxVec3								m_vDirForce = {0.f,0.f,1.f};
	PxVec3								m_vRight = { 1.f,0.f,0.f };
	PxVec3								m_vUpForce = { 0.f,1.f,0.f };
	_int								m_iTotalLink = 0;
	CTrigger*							m_pTrigger = nullptr;
	CTrigger*							m_pHeadTrigger = nullptr;
	vector<vector<_vec3>*>				m_vecOut;
	CMiniVacuum*						m_pPrevObj = nullptr;
	CMiniVacuum*						m_pNextObj = nullptr;
	_int								m_iHeadPos = 1;
	_float								m_fEffectTime = 0.f;
	_mat								m_matBlowWorld;

public:
	static CMiniVacuum*					Create(LPDIRECT3DDEVICE9 pGraphicDev, _uint iDir, void* pArg = nullptr, _uint iLength = 0);
	virtual void Free(void)		override;

};



#define __MiniVacuum_H__
#endif


