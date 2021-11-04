#ifndef Engine_Function_h__
#define Engine_Function_h__

#include "Engine_Typedef.h"

namespace Engine
{
	template <typename T>
	DWORD Safe_Release(T& pointer)
	{
		DWORD	dwRefCnt = 0;
		if (nullptr != pointer)
		{

			dwRefCnt = pointer->Release();
			if (dwRefCnt == 0)
				pointer = nullptr;
		}
		return dwRefCnt;
	}
	template <typename T>
	DWORD Force_Release(T& pointer)
	{
		DWORD	dwRefCnt = 0;
		do
		{
			if (nullptr != pointer)
			{
				dwRefCnt = pointer->Release();
				if (dwRefCnt == 0)
				{

					pointer = nullptr;
					return 0;
				}

			}
		} while (dwRefCnt != 0);
		//{
		//	if (nullptr != pointer)
		//	{
		//		dwRefCnt = pointer->Release();
		//		if (dwRefCnt == 0)
		//			pointer = nullptr;
		//	}
		//}
		return 0;
	}

	template <typename T>
	void Safe_Single_Destory(T& pointer)
	{
		if (nullptr != pointer)
		{
			pointer->DestroyInstance();
			pointer = nullptr;
		}
	}

	template <typename T>
	void Safe_Delete(T& pointer)
	{
		if (nullptr != pointer)
		{
			delete pointer;
			pointer = nullptr;
		}
	}

	template <typename T>
	void Safe_Delete_Array(T& pointer)
	{
		if (nullptr != pointer)
		{
			delete[] pointer;
			pointer = nullptr;
		}
	}
	//////////////////////////////////////////////////////////////////
	/////////////////////////////////Functor//////////////////////////

	class CTag_Finder
	{
	public:
		explicit CTag_Finder(const _tchar* pTag)
			: m_pTargetTag(pTag)
		{
		}
		~CTag_Finder() {		}
	public:
		template<typename T>
		bool operator()(const T& pair)
		{
			if (pair.first == m_pTargetTag)
			{
				return true;
			}

			return false;
		}

	private:
		const _tchar*		m_pTargetTag = nullptr;
	};

	class CDeleteObj
	{
	public:
		explicit CDeleteObj(void) {}
		~CDeleteObj(void) {}
	public: // operator
		template <typename T>
		void operator () (T& pInstance)
		{
			_ulong dwRefCnt = 0;

			dwRefCnt = pInstance->Release();

			if (0 == dwRefCnt)
				pInstance = nullptr;
		}
	};

	// 연관컨테이너 삭제용
	class CDeleteMap
	{
	public:
		explicit CDeleteMap(void) {}
		~CDeleteMap(void) {}
	public: // operator	
		template <typename T>
		void operator () (T& Pair)
		{
			_ulong dwRefCnt = 0;

			dwRefCnt = Pair.second->Release();

			if (0 == dwRefCnt)
				Pair.second = nullptr;
		}
	};

}


#endif // Engine_Function_h__
