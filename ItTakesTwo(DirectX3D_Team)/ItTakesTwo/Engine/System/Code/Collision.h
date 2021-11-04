#ifndef Collision_h__
#define Collision_h__

#include "Engine_Define.h"
#include "Base.h"

BEGIN(Engine)
class ENGINE_DLL CCollision : public CBase
{
	DECLARE_SINGLETON(CCollision)

private:
	explicit CCollision();
	virtual ~CCollision();

public:
	virtual void Free(void) override;
};
END


#endif // Collision_h__
