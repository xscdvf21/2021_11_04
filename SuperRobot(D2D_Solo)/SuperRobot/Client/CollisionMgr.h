#pragma once
class CGameObject;

class CCollisionMgr
{
public:
	CCollisionMgr();
	~CCollisionMgr();
public :
	static void Collision_Rect(list<CGameObject*>& _Dst, list<CGameObject*>& _Src);
	static void Collision_Rect2(list<CGameObject*>& _Dst, list<CGameObject*>& _Src);
	static void Collision_UnitAndMonster(list<CGameObject*>& _Dst, list<CGameObject*>& _Src);
	static void Collision_Sphere(list<CGameObject*>& _Dst, list<CGameObject*>& _Src);

public :
	static bool Check_Sphere(CGameObject* _Dst, CGameObject* _Src);
	static bool Check_Rect(CGameObject* _Dst, CGameObject* _Src, float* _x, float* _y);
	static bool Check_Rect2(CGameObject * _Dst, CGameObject * _Src, float * _x, float * _y);
};

