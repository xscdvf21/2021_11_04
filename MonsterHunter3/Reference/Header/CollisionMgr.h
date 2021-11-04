#ifndef __COLLISIONMGR_H__

#include "Base.h"
#include "GameObject.h"
#include "Layer.h"




class CCollisionMgr : public CBase
{
public:
	CCollisionMgr();
	~CCollisionMgr();

	static void Collision_Sphere(CGameObject* _Dst, CGameObject* _Src); 
	static _bool Colision_SphereCheck(CGameObject* _Dst, CGameObject* _Src, _float * _x, _float * _y, _float * _z);
};




#define __COLLISIONMGR_H__
#endif // !__COLLISIONMGR_H__


