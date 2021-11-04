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

	static void Collision_SpeedItem(list<CObj*>& _Dst, list<CObj*>& _Src); //�޸��� �ӵ�
	static void Collision_BoomItem(list<CObj*>& _Dst, list<CObj*>& _Src); //����ź ���� 
	static void Collision_PadoItem(list<CObj*>& _Dst, list<CObj*>& _Src); //����ź ������

	static void Collision_Boom(list<CObj*>& _Dst, list<CObj*>& _Src); //����ź �����ִ���
	static void Collision_BoomWave(list<CObj*>& _Dst, list<CObj*>& _Src);
	static bool Collision_PadoAndBlock2(CObj*& _Dst, list<CObj*>& _YellowBlock
		, list<CObj*>& _RedBlock, list<CObj*>& _Tree, list<CObj*>& _House
		, list<CObj*>& _TownBlock);
	//�ĵ��� ����ź
	static void Collision_PadoAndBlock(list<CObj*>& _Dst, list<CObj*>& _Src); //�ĵ��� ���� �浹���� �� �ĵ��� ���߱� ����
	static void Collision_BoomAndPlayer(list<CObj*>& _Dst, list<CObj*>& _Src); // ����ź�� �÷��̾ �浹 �� ��ǳ���� �� ��ġ�ϰ� �ϱ�����
	static void Collision_PadoAndHouse(list<CObj*>& _Dst, list<CObj*>& _Src);  // �ĵ��� �� �浹 �� �ĵ� ����
	static void Collision_PadoAndBoss(list<CObj*>& _Dst, list<CObj*>& _Src);
	static void Collision_PadoAndPlayer(list<CObj*>& _Dst, list<CObj*>& _Src); // �ĵ��� �÷��̾� �浹 ��, �÷��̾� ����
	static bool Collision_PadoAndBlock2(CObj*& _Dst, list<CObj*>& _Src);

	

	static bool Check_Sphere(CObj* _Dst, CObj* _Src);

	static bool m_LeftPadoColl;
	static bool m_RightPadoColl;
	static bool m_UpPadoColl;
	static bool m_DownPadoColl;
};


#endif // !__COLLISIONMGR_H__
