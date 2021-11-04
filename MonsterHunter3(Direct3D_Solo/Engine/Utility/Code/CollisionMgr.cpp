#include "CollisionMgr.h"



CCollisionMgr::CCollisionMgr()
{
}


CCollisionMgr::~CCollisionMgr()
{
}

//void CCollisionMgr::Collision_Sphere(multimap<const _tchar*, CGameObject*>& _Dst, multimap<const _tchar*, CGameObject*>& _Src)
//{
//	for (auto& Dst : _Dst)
//	{
//		
//		for (auto& Src : _Src)
//		{
//			_float fX = 0.f; _float fY = 0.f; _float fZ = 0.f;
//			if (Colision_SphereCheck(Dst.second, Src.second, &fX, &fY, &fZ))
//			{
//				//충돌했음.
//				Src.second->Set_CollCheck(true);
//			}
//
//		}
//	}
//}

void CCollisionMgr::Collision_Sphere(CGameObject * _Dst, CGameObject * _Src)
{
	//for (auto& Dst : _Dst)
	//{

	//	for (auto& Src : _Src)
	//	{
			_float fX = 0.f; _float fY = 0.f; _float fZ = 0.f;
			if (Colision_SphereCheck(_Dst, _Src, &fX, &fY, &fZ))
			{
				//충돌했음.
				_Src->Set_CollCheck(true);
			}

	//	}
	//}
}

_bool CCollisionMgr::Colision_SphereCheck(CGameObject * _Dst, CGameObject * _Src, _float * _x, _float * _y, _float * _z)
{
	for (auto& Dst : _Dst->Get_Spherelist())
	{
		for (auto& Src : _Src->Get_Spherelist())
		{
			_float fDir = D3DXVec3Length(&(Dst.first - Src.first));

			_float fRadiusDir = Dst.second + Src.second;

			if (fDir < fRadiusDir)
			{
				return true;
			}
		}
	}
		
	return false;
}


