#ifndef ControllerCallback_h__
#define ControllerCallback_h__

#include "Engine_Define.h"
class CControllerCallback : public PxControllerBehaviorCallback, public PxUserControllerHitReport,public PxControllerFilterCallback,public PxQueryFilterCallback, public PxFilterData
{
public:
	CControllerCallback():m_ControllFilter(this,this,this) {}
	virtual PxControllerBehaviorFlags getBehaviorFlags(const PxShape& shape, const PxActor& actor) PURE;
	virtual PxControllerBehaviorFlags getBehaviorFlags(const PxController& controller) PURE;
	virtual PxControllerBehaviorFlags getBehaviorFlags(const PxObstacle& obstacle) PURE;
	virtual void onShapeHit(const PxControllerShapeHit& hit) PURE;
	virtual void onControllerHit(const PxControllersHit& hit) PURE;
	virtual void onObstacleHit(const PxControllerObstacleHit& hit) PURE;
	virtual bool filter(const PxController& a, const PxController& b)PURE;
	virtual PxQueryHitType::Enum preFilter(const PxFilterData& filterData, const PxShape* shape, const PxRigidActor* actor, PxHitFlags& queryFlags) PURE;
	virtual PxQueryHitType::Enum postFilter(const PxFilterData& filterData, const PxQueryHit& hit) PURE;
protected:
	PxControllerFilters m_ControllFilter;
};
#endif // ControllerCallback_h__