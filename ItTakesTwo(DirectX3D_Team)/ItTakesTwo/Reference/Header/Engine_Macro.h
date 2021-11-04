#ifndef Engine_Macro_h__
#define Engine_Macro_h__

namespace Engine
{
	#define		WINCX 1920
	#define		WINCY 1080

	#define		OBJ_DEAD 1
	#define		OBJ_NOEVENT 0
	
	#define		SCENE_CHANGE 99
	#define vRIGHT	_vec3(1.f, 0.f, 0.f)
	#define vUP		_vec3(0.f, 1.f, 0.f)
	#define vLOOK	_vec3(0.f, 0.f, 1.f)

	#define PI		3.14f

	#ifndef MSG_BOX
	#define	MSG_BOX(_message)			MessageBox(NULL, TEXT(_message), L"System Message", MB_OK)
	#endif

	#define			BEGIN(NAMESPACE)		namespace NAMESPACE {
	#define			END						}

	#define			USING(NAMESPACE)	using namespace NAMESPACE;

	#ifdef	ENGINE_EXPORTS
	#define ENGINE_DLL		_declspec(dllexport)
	#else
	#define ENGINE_DLL		_declspec(dllimport)
	#endif

	#define NULL_CHECK( _ptr)	\
	{if( _ptr == 0){return;}}

	#define NULL_CHECK_RETURN( _ptr, _return)	\
	{if( _ptr == 0){return _return;}}

	#define NULL_CHECK_MSG( _ptr, _message )		\
	{if( _ptr == 0){DEBUGMSG(L"실패"); MessageBox(NULL, _message, L"System Message",MB_OK);}}

	#define NULL_CHECK_RETURN_MSG( _ptr, _return, _message )	\
	{if( _ptr == 0){DEBUGMSG(L"실패"); MessageBox(NULL, _message, L"System Message",MB_OK);return _return;}}


	#define FAILED_CHECK(_hr)	if( ((HRESULT)(_hr)) < 0 )	\
	{DEBUGMSG(L"실패"); MessageBoxW(NULL, L"Failed", L"System Error",MB_OK);  return E_FAIL;}

	#define FAILED_CHECK_RETURN(_hr, _return)	if( ((HRESULT)(_hr)) < 0 )		\
	{ DEBUGMSG(L"실패"); MessageBoxW(NULL, L"Failed", L"System Error",MB_OK);  return _return;}

	#define FAILED_CHECK_MSG( _hr, _message)	if( ((HRESULT)(_hr)) < 0 )	\
	{DEBUGMSG(_message); MessageBoxW(NULL, _message, L"System Message",MB_OK); return E_FAIL;}

	#define FAILED_CHECK_RETURN_MSG( _hr, _return, _message)	if( ((HRESULT)(_hr)) < 0 )	\
	{DEBUGMSG(_message); MessageBoxW(NULL, _message, L"System Message",MB_OK); return _return;}

#define SAFE_DELETE(p) if (p) { delete p; p = nullptr; }



	#define NO_COPY(CLASSNAME)							\
		private:										\
		CLASSNAME(const CLASSNAME&);					\
		CLASSNAME& operator = (const CLASSNAME&);		

	#define DECLARE_SINGLETON(CLASSNAME)				\
		NO_COPY(CLASSNAME)								\
		private:										\
		static CLASSNAME*	m_pInstance;				\
		public:											\
		static CLASSNAME*	GetInstance( void );		\
		static void DestroyInstance( void );			

	#define IMPLEMENT_SINGLETON(CLASSNAME)				\
		CLASSNAME*	CLASSNAME::m_pInstance = NULL;		\
		CLASSNAME*	CLASSNAME::GetInstance( void )	{	\
			if(NULL == m_pInstance) {					\
				m_pInstance = new CLASSNAME;			\
			}											\
			return m_pInstance;							\
		}												\
		void CLASSNAME::DestroyInstance( void ) {		\
			if(NULL != m_pInstance)	{					\
				delete m_pInstance;						\
				m_pInstance = NULL;						\
			}											\
		}

}

#define DEBUGMSG(msg)												\
	_tchar szBuf[MAX_PATH]{};										\
	swprintf_s(szBuf, L"%s(%d) : %s\n", __FILEW__, __LINE__, msg);	\
	OutputDebugString(szBuf)






#endif // Engine_Macro_h__
