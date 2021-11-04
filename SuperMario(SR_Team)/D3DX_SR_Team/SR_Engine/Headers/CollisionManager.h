#pragma once

#include "Base.h"
#include "GameObject.h"
#include "Layer.h"


BEGIN(Engine)

class CCollisionManager :public CBase
{
public:
	CCollisionManager();
	//~CCollisionManager ()

public: // push
	static void Collision_RectEx_Push(list<CGameObject*>& _Dst, list<class CGameObject*>& _Src);
	static bool Check_Rect(CGameObject* _pDst, CGameObject* _pSrc, float* _x, float* _y, float* _z);

	static void Collision_RectEx_PlayerAndMonster(list<CGameObject*>& _Dst, list<class CGameObject*>& _Src);
	static void Collision_RectEx_PlayerAndCoin(list<CGameObject*>& _Dst, list<class CGameObject*>& _Src);
	static void Collision_RectEx_BulletAndMonster(list<CGameObject*>& _Dst, list<class CGameObject*>& _Src);
	static void Collision_RectEx_BulletAndPlayer(list<CGameObject*>& _Dst, list<class CGameObject*>& _Src); //���� �Ѿ˰� �÷��̾� �浹.
	static void Collision_RectEx_BattleMonsterAndPlayer(list<CGameObject*>& _Dst, list<class CGameObject*>& _Src);
public: // terrain
	static void Collision_Terrain(list<CGameObject*>& _Dst, list<class CGameObject*>& _Src);
	static bool Check_Terrain(CGameObject * _pDst, CGameObject * _pSrc, float * _x, float * _y, float * _z);
	static bool Check_Terrain_Scaffolding(CGameObject * _pDst, CGameObject * _pSrc);
	// �߷� �δ��� ����.

public:// �̺�Ʈ 
	static void Collision_Event_Bidirectional(list<CGameObject*>& _Dst, list<class CGameObject*>& _Src);
	static void Collision_Event_Unidirectional(list<CGameObject*>& _Dst, list<class CGameObject*>& _Src);
	static void Collision_Sphrase_Push(list<CGameObject*>& _Dst, list<class CGameObject*>& _Src);

	static bool Collision_Sphrase(CGameObject * _pDst, CGameObject * _pSrc, float * _x, float * _y, float * _z);
};
END