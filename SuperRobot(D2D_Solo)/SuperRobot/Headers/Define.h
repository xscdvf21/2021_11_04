#pragma once

#define WINCX	800
#define WINCY	600
#define TILEX	30
#define TILEY	30
#define TILECX	40
#define TILECY	40
#define OBJ_DEAD	1
#define OBJ_NOEVENT 0 
// 0000 0001
#define SWORD	0x01
// 0000 0010
#define DAGGER	0x02

// 0000 0100
#define BOW		0x04
#ifdef _AFX
#define ERR_MSG(message) AfxMessageBox(message)
#else
#define ERR_MSG(message) MessageBox(nullptr,message, L"SystemError", MB_OK)
#endif // _AFX

#define DECLEARE_SINGLETON(ClassName)				\
public:												\
	static ClassName* Get_Instance()				\
	{												\
		if (nullptr == m_pInstance)					\
			m_pInstance = new ClassName;			\
		return m_pInstance;							\
	}												\
	static void Destroy_Instance()					\
	{												\
		if (m_pInstance)							\
		{											\
			delete m_pInstance;						\
			m_pInstance = nullptr;					\
		}											\
	}												\
	static ClassName* m_pInstance;			

#define IMPLEMENT_SINGLETON(ClassName)				\
ClassName* ClassName::m_pInstance = nullptr;