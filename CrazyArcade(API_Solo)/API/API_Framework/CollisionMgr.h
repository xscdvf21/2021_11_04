#pragma once

#ifndef __COLLISIONMGR_H__
#define __COLLISIONMGR_H__

class CObj;
class CCollisionMgr
{
public:
	CCollisionMgr();
	~CCollisionMgr();

public:
	static void Collision_Rect(list<CObj*>& _Dst, list<CObj*>& _Src);
	static void Collision_Sphere(list<CObj*>& _Dst, list<CObj*>& _Src);
	static void Collision_RectEx(list<CObj*>& _Dst, list<CObj*>& _Src);

	static bool Check_Rect(CObj* _Dst, CObj * _Src, float * _x, float * _y);

	static void Collision_SpeedItem(list<CObj*>& _Dst, list<CObj*>& _Src); //달리기 속도
	static void Collision_BoomItem(list<CObj*>& _Dst, list<CObj*>& _Src); //물폭탄 숫자 
	static void Collision_PadoItem(list<CObj*>& _Dst, list<CObj*>& _Src); //물폭탄 사이즈

	static void Collision_Boom(list<CObj*>& _Dst, list<CObj*>& _Src); //물폭탄 겹쳐있는지
	static void Collision_BoomWave(list<CObj*>& _Dst, list<CObj*>& _Src);
	static bool Collision_PadoAndBlock2(CObj*& _Dst, list<CObj*>& _YellowBlock
		, list<CObj*>& _RedBlock, list<CObj*>& _Tree, list<CObj*>& _House
		, list<CObj*>& _TownBlock);
	//파도에 물폭탄
	static void Collision_PadoAndBlock(list<CObj*>& _Dst, list<CObj*>& _Src); //파도와 블럭이 충돌했을 때 파도를 멈추기 위해
	static void Collision_BoomAndPlayer(list<CObj*>& _Dst, list<CObj*>& _Src); // 물폭탄과 플레이어가 충돌 시 물풍선을 못 설치하게 하기위해
	static void Collision_PadoAndHouse(list<CObj*>& _Dst, list<CObj*>& _Src);  // 파도와 집 충돌 시 파도 삭제
	static void Collision_PadoAndBoss(list<CObj*>& _Dst, list<CObj*>& _Src);
	static void Collision_PadoAndPlayer(list<CObj*>& _Dst, list<CObj*>& _Src); // 파도와 플레이어 충돌 시, 플레이어 죽음
	static bool Collision_PadoAndBlock2(CObj*& _Dst, list<CObj*>& _Src);

	

	static bool Check_Sphere(CObj* _Dst, CObj* _Src);

	static bool m_LeftPadoColl;
	static bool m_RightPadoColl;
	static bool m_UpPadoColl;
	static bool m_DownPadoColl;
};


#endif // !__COLLISIONMGR_H__
