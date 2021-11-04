#pragma once
#ifndef __ENGINE_FUNCTION_H__
#define __ENGINE_FUNCTION_H__

template <typename T>
void Safe_Delete(T& ptr)
{
	if (ptr)
	{
		delete ptr;
		ptr = nullptr;
	}
}

template <typename T>
void Safe_Delete_Array(T& ptr)
{
	if (ptr)
	{
		delete[] ptr;
		ptr = nullptr;
	}
}

template <typename T>
_uint Safe_AddReference(T& ptr)
{
	_uint iRefCount = 0;

	if (ptr)
	{
		iRefCount = ptr->AddRef();
	}

	return iRefCount;
}

template <typename T>
_uint Safe_Release(T& ptr)
{
	_uint iRefCount = 0;

	if (ptr)
	{
		iRefCount = ptr->Release();
		ptr = nullptr;
	}


	return iRefCount;
}

template <typename T>
bool isEqual(T& tA, T& tB)
{
	if (abs(tA - tB) < 0.01f)
	{
		return true;
	}
	return false;
}

template <typename T>
bool isSmall(T tA, T tB, T tC)
{
	if ((T)abs(tA - tB) < tC)
	{
		return true;
	}
	return false;
}


#endif