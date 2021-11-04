#ifndef GameObject_h__
#define GameObject_h__

#include "Engine_Define.h"
#include "Base.h"
#include "Component.h"
#include "Transform.h"
#include "SoundMgr.h"

BEGIN(Engine)

class ENGINE_DLL CGameObject : public CBase
{
public:
	struct ControlledActorDesc
	{
		ControlledActorDesc() :
			mType(PxControllerShapeType::eFORCE_DWORD),
			mPosition(PxExtendedVec3(0, 0, 0)),
			mSlopeLimit(0.0f),
			mContactOffset(0.1f),
			mStepOffset(0.0f),
			mInvisibleWallHeight(0.0f),
			mMaxJumpHeight(0.0f),
			mRadius(0.0f),
			mHeight(0.0f),
			mCrouchHeight(0.0f),
			mProxyDensity(10.0f),
			//	mProxyScale			(0.8f)
			mProxyScale(0.9f),
			mVolumeGrowth(1.5f),
			mReportCallback(NULL),
			mBehaviorCallback(NULL)
		{
		}

		PxControllerShapeType::Enum		mType;
		PxExtendedVec3					mPosition;
		float							mSlopeLimit;
		float							mContactOffset;
		float							mStepOffset;
		float							mInvisibleWallHeight;
		float							mMaxJumpHeight;
		float							mRadius;
		float							mHeight;
		float							mCrouchHeight;
		float							mProxyDensity;
		float							mProxyScale;
		float							mVolumeGrowth;
		PxUserControllerHitReport*		mReportCallback;
		PxControllerBehaviorCallback*	mBehaviorCallback;
	};
protected:
	explicit CGameObject(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CGameObject(void);

public:
	CComponent*		Get_Component(const _tchar* pComponentTag, COMPONENTID eID);
	CComponent*		Find_Component(const _tchar* pComponentTag, COMPONENTID eID);
	void			Compute_ViewZ(const _vec3 * pPos);
	void			Compute_ViewZ();

public:
	template <typename T>
	T*	Is();
public:
	void				PlaySoundOnce(TCHAR* tagFile, const CSoundMgr::CHANNELID& eID, const _float& fVolum=0.5f);
public:
	virtual HRESULT		Ready_Object(void* pArg = nullptr);
	virtual _int		Update_Object(const _float& TimeDelta);
	virtual _int		LateUpdate_Object(const _float & TimeDelta);
	virtual void		Render_Object(const _int& iIndex = 0);
	virtual void		Render_Shadow(_vec3* vEye, _vec3* vAt, const _int& iIndex = 0) {};
	virtual void		Render_Stencil(const _int& iIndex = 0) {};
	virtual void		Render_VTF(const _int& iIndex = 0) {};
	virtual void		Set_ActorSetting(PxRigidDynamic* pActor, PxShape* pBody);
	virtual void		Move_Left();
	virtual void		Move_Right();
	virtual void		Move_Up();
	virtual void		Move_Down();
	virtual _mat		Get_EventPos();
public:
	virtual	void		Set_Dead(_bool bStack = false) { m_bDead = true; }
	virtual	void		Set_Hit(_bool hit) { m_bHit = hit; }
	virtual void		Set_HP(_int hp) { m_tInfo.iHp += hp; }
	void				Set_Controllable(ControlledActorDesc* pDescconst);
	void				Set_Controller(PxController* pController);
	void				Create_Actor(PxPhysics* pPhys, const _float& fHeight = 1.f, const _float& fRadius = 1.f, PxUserControllerHitReport* pHitCallback = nullptr, PxControllerBehaviorCallback* pBehaviorCallback = nullptr, _vec3 vPos = _vec3{ 0.f,0.f,0.f }, _vec3 vDir = _vec3{ 0.f,0.f,0.f }, const PxMaterials& tMatrial = PxMaterials());
	void				Set_Interactable(void* pInteracter, const _bool& bState = true);
	void				Set_InteractObj(void* pInteracter);;
	void				Set_Scene(PxScene* pScene);
	void				Set_ControllerFilter();
	void				Set_Contact(PxShape* pShape, _bool bState = true);
	void				Set_Trigger(PxShape* pShape, _bool bState = true);
	void				Set_RotStart(_bool bStart) { m_bRotStart = bStart; }
	void				Set_CodyOn(_bool On) {m_bCodyOn = On;}
	void				Set_MayOn(_bool On) { m_bMayOn = On; }
	void				Set_Value(const _float& fValue) { m_fValue = fValue; }
	void				Set_ObjTag(wstring ObjTag) { m_ObjTag = ObjTag; }
	void				Set_Pos(const _vec3& vPos);
	void				Set_ReservePos(const _vec3& vPos);
	void				Set_SimulationFilter(const PxFilterData& filterData);
	void				Set_QueryFilter(const PxFilterData& filterData);
	virtual void		Set_Damage(_int iDamage) { m_iHp += iDamage; }
	
protected:
	void				Update_Reserve(const _float& fTimeDelta);

public:
	PxController*		Get_Controller() { return m_pController; }
	const _uint&		Get_Dir() { return m_iDir; }
	_int				Get_Size() { return m_iSize; }

public:
	virtual void*		On_Active(void* pCaller);
	virtual void*		On_Trigger_Active(void* pTrigger, void* pCaller);
	virtual void*		On_Attach(void* pCaller);


public:
	virtual _float		Get_ColDis() { return m_fColDis; }
	_float				Get_ViewZ(void) { return m_fViewZ; }
	OBJECTINFO			Get_ObjInfo() { return m_tInfo; }
	OBJINFO				Get_INFO() { return m_tObjInfo; }
	PxRigidActor*		Get_RigidActor() { return m_pRigidActor; }
	PxShape*			Get_RigidBody() { return m_pRigidBody; }
	const _bool&		Is_Dead() { return m_bDead; }
	const _bool&		Is_Controllable() { return m_bControllable; }
	wstring				Get_ObjTag() { return m_ObjTag; }
	PxVec3				Get_Up() { return m_vUp; }
	_bool				Get_Dead() { return m_bDead; }


public:
	PxController*		CreateController(PxControllerManager* pManager);
	void				Set_SimulationState(const _bool& bState);

protected:
	Engine::CTransform* m_pTransformCom = nullptr;

protected:
	map<_wstr, CComponent*>		m_mapComponent[ID_END];
	LPDIRECT3DDEVICE9			m_pGraphicDev;
	_float						m_fViewZ = 0.f;
	PxRigidActor*				m_pRigidActor = nullptr;
	PxMaterial*					m_pMaterial = nullptr;

protected:
	_bool						m_bDead;
	_bool						m_bHit;
	_bool						m_bStart = true;
	_float						m_fColDis;
	OBJECTINFO					m_tInfo;
	SHADER						m_tShader;
	PxShape*					m_pRigidBody = nullptr;

	_bool						m_bControllable = false;
	PxController*				m_pController = nullptr;
	ControlledActorDesc			m_tControllerDesc;
	_bool						m_bInteractable = false;
	_int						m_iInteractCount = 0;
	PxShape*					m_pInteractShape = nullptr;
	void*						m_pInteractActor = nullptr;
	vector<void*>				m_vecInteractActor;
	PxScene*					m_pScene = nullptr;
	OBJINFO						m_tObjInfo;
	_uint						m_iDir = 0;
	_bool						m_bSimulate = true;
	_bool						m_bRotStart = false;
	_bool						m_bCodyOn = false;
	_bool						m_bMayOn = false;

	_bool						m_bSound = false;
	_int						m_iSize = 1;
	wstring						m_ObjTag = L"";
	PxVec3						m_vUp = { 0.f,1.f,0.f };
	_float						m_fValue = 0.f;
	_int						m_iHp;
	_int						m_iMaxHp;
	_bool						m_bReserve = false;
	_vec3						m_vMovePos;
	map<_wstr, _bool>			m_mapSound;
public:
	virtual void	Free(void);

private:
};

template <typename T>
T* Engine::CGameObject::Is()
{
	return static_cast<T*>(this);
}

END
#endif // GameObject_h__
