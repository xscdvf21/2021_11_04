
#ifndef Client_Function_h__
#define Client_Function_h__

#include "stdafx.h"

static _vec3 ToVec3(const PxExtendedVec3& vPos)
{
	return _vec3{ (_float)vPos.x,(_float)vPos.y,(_float)vPos.z };
}

static _vec3 ToVec3(const PxTransform& vPos)
{
	return _vec3{ vPos.p.x,vPos.p.y,vPos.p.z };
}

static _vec3 ToVec3(const PxVec3& vPos)
{
	return _vec3{ vPos.x,vPos.y,vPos.z };
}

static  CTrigger* ToTrigger(const void* pObj)
{
	return static_cast<CTrigger*>((void*)pObj);
}
template<typename T>
static T* ToObj(const char* pObj)
{
	return static_cast<T*>((void*)pObj);
}
template<typename T>
static T* ToObj(const void* pObj)
{
	return static_cast<T*>((void*)pObj);
}
//static _vec3 ToPxTransform(const PxVec3& vPos)
//{
//	return _vec3{ vPos.x,vPos.y,vPos.z };
//}
template <typename T>
static void ZeroMemoryArray(const void* pDst,const _uint& iCount )
{
	ZeroMemory(&pDst, sizeof(T)*iCount);
}
#endif // Client_Function_h__