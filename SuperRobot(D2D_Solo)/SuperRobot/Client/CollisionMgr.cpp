#include "stdafx.h"
#include "CollisionMgr.h"
#include "GameObject.h"
#include "GameObject_Manger.h"


CCollisionMgr::CCollisionMgr()
{
}


CCollisionMgr::~CCollisionMgr()
{
	
}

void CCollisionMgr::Collision_Rect(list<CGameObject*>& _Dst, list<CGameObject*>& _Src)
{
	float fX = 0.f, fY = 0.f;
	for (auto& Dst : _Dst)
	{
		for (auto& Src : _Src)
		{

			if (Check_Rect(Dst, Src, &fX, &fY))
			{
				Dst->Set_UnitSelect();

			}
		}
		
	}
}

void CCollisionMgr::Collision_Rect2(list<CGameObject*>& _Dst, list<CGameObject*>& _Src)
{
	float fX = 0.f, fY = 0.f;
	for (auto& Dst : _Dst)
	{
		for (auto& Src : _Src)
		{

			if (Check_Rect2(Dst, Src, &fX, &fY))
			{
				Dst->Set_UnitSelect();

			}
		}

	}
}

void CCollisionMgr::Collision_UnitAndMonster(list<CGameObject*>& _Dst, list<CGameObject*>& _Src) //�����̳� ���Ͱ� �� ��ġ�� ������. (��ġ�����ϵ���)
{
	float fX = 0.f, fY = 0.f;
	for (auto& Dst : _Dst)
	{
		for (auto& Src : _Src)
		{

			if (Check_Rect(Dst, Src, &fX, &fY))
			{

			}
		}

	}
}

void CCollisionMgr::Collision_Sphere(list<CGameObject*>& _Dst, list<CGameObject*>& _Src)
{
}

bool CCollisionMgr::Check_Sphere(CGameObject * _Dst, CGameObject * _Src)
{
	// abs()
	// -> ���밪�� �����ִ� �Լ�

	// sqrtf()
	// -> �������� �����ִ� �Լ�

	float fX = _Dst->Get_Info().vPos.x - _Src->Get_Info().vPos.x;
	float fY = _Dst->Get_Info().vPos.y - _Src->Get_Info().vPos.y;
	float fDia = sqrtf(fX * fX + fY * fY);

	float fCX = (float)((_Dst->Get_Info().vSize.x + _Src->Get_Info().vSize.x) * 0.5f);

	return fCX > fDia;
}

bool CCollisionMgr::Check_Rect(CGameObject * _Dst, CGameObject * _Src, float * _x, float * _y)
{
	float fDisX = abs(_Dst->Get_Info().vPos.x - _Src->Get_Info().vPos.x);
	float fDisY = abs(_Dst->Get_Info().vPos.y - _Src->Get_Info().vPos.y);

	float fCX = (float)((_Dst->Get_TexInfo()->tImageInfo.Width + _Src->Get_TexInfo()->tImageInfo.Width >> 1) >> 1);
	float fCY = (float)((_Dst->Get_TexInfo()->tImageInfo.Height + _Src->Get_TexInfo()->tImageInfo.Height >> 1) >> 1);

	if (fCX > fDisX && fCY > fDisY)
	{
		*_x = fCX - fDisX;
		*_y = fCY - fDisY;
		return true;
	}

	return false;
}

bool CCollisionMgr::Check_Rect2(CGameObject * _Dst, CGameObject * _Src, float * _x, float * _y)
{
	float fDisX = abs(_Dst->Get_MonsterInfo().vPos.x - _Src->Get_Info().vPos.x);
	float fDisY = abs(_Dst->Get_MonsterInfo().vPos.y - _Src->Get_Info().vPos.y);

	float fCX = (float)((_Dst->Get_TexInfo()->tImageInfo.Width + _Src->Get_TexInfo()->tImageInfo.Width >> 1) >> 1);
	float fCY = (float)((_Dst->Get_TexInfo()->tImageInfo.Height + _Src->Get_TexInfo()->tImageInfo.Height >> 1) >> 1);

	if (fCX > fDisX && fCY > fDisY)
	{
		*_x = fCX - fDisX;
		*_y = fCY - fDisY;
		return true;
	}

	return false;
}
