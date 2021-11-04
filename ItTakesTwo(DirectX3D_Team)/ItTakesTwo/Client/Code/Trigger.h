#ifndef Tigger_h__
#define Tigger_h__

#include "Export_Function.h"
#include "Base.h"
#include "Transform.h"
typedef enum TriggerType
{
	TRIGGER_VACUUM_ENTER,
	TRIGGER_VACUUM_EXIT,
	TRIGGER_VACUUM_RIDE,
	TRIGGER_VACUUM_BOSS_CODY,
	TRIGGER_VACUUM_BOSS_MAY,
	TRIGGER_VACUUM_HEAD,
	TRIGGER_VACUUM_LEFT,
	TRIGGER_VACUUM_RIGHT,
	TRIGGER_VACUUM_DUMBBELL_CODY,
	TRIGGER_VACUUM_DUMBBELL_MAY,
	TRIGGER_VACUUMBOSS_STONE,
	TRIGGER_VACUUMBOSS_VACUUM,
	TRIGGER_CHESS_ARROW,
	TRIGGER_CHESS_ONE_KNIGHTMOVE,
	TRIGGER_CHESS_TWO_KNIGHTMOVE,
	TRIGGER_CHESS_OGRE_HATCH,
	TRIGGER_CHESS_DOORBUTTON,
	TRIGGER_CHESS_FIREBUTTON,
	TRIGGER_CHESS_CRUSHERBUTTON,
	TRIGGER_CHESS_CRUSHER_HIT,
	TRIGGER_CHESS_BRIDGE,
	TRIGGER_CHESS_FIELD2_CREATE,
	TRIGGER_CHESS_FIELD2_MOVE_ARBALIST,
	TRIGGER_CHESS_FIELD2_MOVE_KNIGHT,
	TRIGGER_CHESS_FIELD3_CREATE,
	TRIGGER_CHESS_FIELD3_MOVEROOM1,
	TRIGGER_CHESS_FIELD3_MOVEROOM2,
	TRIGGER_CHESS_FIELD3_MOVEROOM3,
	TRIGGER_CHESS_FIELD3_MOVEROOM4,
	TRIGGER_CHESS_FIELD3_MOVEROOM5,
	TRIGGER_CHESS_FIELD3_MOVEROOM6,
	TRIGGER_CHESS_OGRE_CAMERA,
	TRIGGER_CHESS_OGRE_DOOR,
	TRIGGER_CHESS_OGRE_STONE,
	TRIGGER_CHESS_SHUTTLE,
	TRIGGER_CHESS_SHUTTLE_DOOR,
	TRIGGER_SPACE_LASER_DEFAULT,
	TRIGGER_SPACE_LASER_GRAVITY,
	TRIGGER_SPACE_MISSILE_BOSS,
	TRIGGER_SPACE_MISSILE_PLAYER,
	TRIGGER_SPACE_MiSSILE_RIDE,
	TRIGGER_SPACE_MOVEPLATFORM,
	TRIGGER_SPACE_TUBE,
	TRIGGER_SPACE_GENERATOR_BATTERY,
	TRIGGER_SPACE_GENERATOR_LEVER,
	TRIGGER_SPACE_GRAVITY_FLY,
	TRIGGER_SPACE_GRAVITY_FLYBOSSROOM,
	TRIGGER_SPACE_WALL_CODY,
	TRIGGER_SPACE_WALL_MAY,
	TRIGGER_FUSE,
	TRIGGER_LEVER,
	TRIGGER_FUSE_SLOT,
	TRIGGER_DEADSPACE,
	TRIGGER_SAVESPACE,
	TRIGGER_POWERSWITCH,
	TRIGGER_FLOAT,
	TRIGGER_WING,
	TRIGGER_BOUNCE_SWITCH,
	TRIGGER_SCENE_CHANGE,
	TRIGGER_CAMERA_CHANGE,
	TRIGGER_CAMERA_CHANGE_FIX,
	TRIGGER_HANDLE,
	TRIGGER_ATTACK,
	TRIGGER_FANBUTTON,
	TRIGGER_SHOOT,
	TRIGGER_GRAVITY,
	TRIGGER_SAW_SLIDING,
	TRIGGER_SAW_JUMP,
	TRIGGER_HANG,
	TRIGGER_LEVER_CODY,
	TRIGGER_LEVER_MAY,
	TRIGGER_SPRING,
	TRIGGER_TELEPORT,
	TRIGGER_PLATFORM,
	TRIGGER_GLASSBOX,
	TRIGGER_PUSHGRAP,
	TRIGGER_POSCHANGE,
	TRIGGER_PLATFORMDEAD,
	TRIGGER_PLATFORMRESPWAN,
	TRIGGER_SOUND,
	TRIGGER_END,

}TRIGGERTYPE;

class CTrigger : public CBase
{
public:
	typedef enum eTriggerShape
	{
		BOX,
		SPHERE,
		CAPSULE,
		TRIMESH,
		CONVEX,
	}TRIGGERSHPAE;
private:
	explicit CTrigger() = delete;
	virtual ~CTrigger() = default;
	explicit CTrigger(PxScene* pScene, PxGeometry& psGeo, void* tagName, const _vec3& vPos, PxU32 FilterGroup, PxU32 FilterMask);
public:
	static CTrigger* CreateBoxTrigger(PxScene* pScene,  void* tagName, const _vec3& vPos, const _float& fXHalf, const _float& fYHalf, const _float& fZHalf, const TRIGGERTYPE& eTriggerType, PxU32 FilterGroup = FilterGroup::eDefaultTrigger, PxU32 FilterMask = FilterGroup::eCody | FilterGroup::eMay);
	static CTrigger* CreateBoxTrigger(PxScene* pScene, void* tagName, const _vec3& vPos, const _float& fHalf, const TRIGGERTYPE& eTriggerType, PxU32 FilterGroup = FilterGroup::eDefaultTrigger, PxU32 FilterMask = FilterGroup::eCody | FilterGroup::eMay);

	static CTrigger* CreateSphereTirgger(PxScene* pScene, void* tagName, const _vec3& vPos, const _float& fHalf, const TRIGGERTYPE& eTriggerType, PxU32 FilterGroup = FilterGroup::eDefaultTrigger, PxU32 FilterMask = FilterGroup::eCody | FilterGroup::eMay);

	static CTrigger* CreateCapsuleTirgger(PxScene* pScene, void* tagName, const _vec3& vPos, const _float& fRadius, const _float& fHeightHalf, const TRIGGERTYPE& eTriggerType, PxU32 FilterGroup = FilterGroup::eDefaultTrigger, PxU32 FilterMask = FilterGroup::eCody | FilterGroup::eMay);

	static void Clear_Trigger();

public:
	void Set_Pos(const _vec3& vPos);
	void Set_Pos(const PxVec3& vPos);
	void Set_Pos(const PxTransform& vPos);
	void Set_Pos(const PxExtendedVec3& vPos);

	void Set_Rotation(const _vec3& vAngle);
	void Set_Rotation(const PxQuat& vAngle);


	void Set_Index(const _uint& iIndex) { m_iIndex = iIndex; }
	void Set_Interactable(const _bool& bState);
	void Set_TriggerType(const TRIGGERTYPE& iType) { m_eTriggerType = iType; }
	void Set_Active(const _bool& bState) { m_bActive = bState; }
	const _uint Set_FloatValue(const _float& fValue);
	void Set_Scale(const _float& fScale);
	void Set_Scale(const _vec3& fScale);
public:
	const _float		Get_FloatValue(const _uint& iIndex=0);
	const TriggerType	Get_Type() { return m_eTriggerType; }
	const _uint			Get_Index() { return m_iIndex; }
	const _vec3			Get_Pos() { return _vec3(m_vPos.x, m_vPos.y, m_vPos.z); }
private:
	void Set_GlobalPos();
public:
	void* Activate(void* pCaller);
public:

	template<typename T>
	T* Is()
	{
		return static_cast<T*>(m_pConnectClass);
	}

public:
	virtual void Free(void) override;
private:
	Engine::CTransform* m_pTransform;
	PxRigidActor*		m_pRigidActor = nullptr;
	PxShape*			m_pShape = nullptr;
	TRIGGERSHPAE		m_eTriggerShape;
	TriggerType			m_eTriggerType = TRIGGER_END;
	
private:
	PxVec3				m_vPos;
	PxQuat				m_qRot;
	vector<_float>		m_vecFloatValues;
private:
	_bool				m_bActive = false;
private:
	void*				m_pConnectClass = nullptr;
private:
	_uint				m_iIndex;
	static vector<CTrigger*> m_vecTrigger;
public:
	const _bool Is_In();
};
#endif // Tigger_h__