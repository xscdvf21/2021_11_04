#ifndef Export_System_h__
#define Export_System_h__
#ifdef NDEBUG
#ifdef _DEBUG
#undef _DEBUG
#endif
#endif
#include "GraphicDevice.h"
#include "TimerMgr.h"
#include "FrameMgr.h"
#include "Collision.h"
#include "InputDev.h"
#include "SoundMgr.h"
#include "FontMgr.h"
#include "Phys_Manager.h"

BEGIN(Engine)

// GraphicDev
inline HRESULT	Ready_GraphicDev(HWND hWnd,WINMODE eMode,const _uint& iSizeX,	const _uint& iSizeY,	CGraphicDevice** ppGraphicDev);
inline void		Render_Begin(D3DXCOLOR Color);
inline void		Render_End(void);

// TimerMgr
inline _float	Get_TimeDelta(const _tchar* pTimerTag);
inline void		Set_TimeDelta(const _tchar* pTimerTag);
inline HRESULT	Ready_Timer(const _tchar* pTimerTag);
inline void		Release_System(void);

// FrameMgr
inline _bool	IsPermit_Call(const _tchar* pFrameTag, const _float& fTimeDelta);
inline HRESULT	Ready_Frame(const _tchar* pFrameTag, const _float& fCallLimit);

// Physics
inline HRESULT						Ready_Physics(Engine::CPhys_Manager** pManager);
inline PxPhysics*					Get_Physics();
inline PxDefaultCpuDispatcher*		Get_Dispatcher();
inline PxDefaultAllocator*			Get_Allocator();
inline PxCudaContextManager*		Get_CudaMgr();




// DInput
inline _bool	Key_Pressing(_ubyte byKeyID);
inline _bool	Key_Down(_ubyte byKeyID);
inline _bool	Key_Up(_ubyte byKeyID);
inline _bool	Key_Pressing(MOUSEKEYSTATE eMouse);
inline _bool	Key_Down(MOUSEKEYSTATE eMouse);
inline _bool	Key_Up(MOUSEKEYSTATE eMouse);
inline _bool	Key_Pressing(PADKEYSTATE ePad);
inline _bool	Key_Down(PADKEYSTATE ePad);
inline _bool	Key_Up(PADKEYSTATE ePad);
inline _byte	Get_DIKeyState(_ubyte byKeyID);
inline _byte	Get_DIMouseState(MOUSEKEYSTATE eMouse);
inline _long	Get_DIMouseMove(MOUSEMOVESTATE eMouseState);
inline _float	Get_DIPadMove(PADMOVESTATE eMouseState);


inline HRESULT	Ready_InputDev(HINSTANCE hInst, HWND hWnd);
inline void		SetUp_Key();
inline void		SetUp_Mouse();
inline void		Set_InputDev(void);

// Sound
inline void		Initialize();
inline void		PlaySoundW(TCHAR* pSoundKey, Engine::CSoundMgr::CHANNELID eID, const _float& fVolume = 0.5f);
inline void		PlayBGM(TCHAR* pSoundKey, const _float& fVolume = 1.f);
inline void		StopSound(Engine::CSoundMgr::CHANNELID eID);
inline void		StopAll();
inline void		Load_SoundFile(const string tagPath, const RESOURCEID& eResourceID);
inline void		Set_SoundScene(const RESOURCEID& eResourceID);
inline void		Release_Sound(RESOURCEID eResourceID);
inline _bool	Is_Play_End(Engine::CSoundMgr::CHANNELID eID);
inline void		StopNPlaySound(TCHAR * pSoundKey, Engine::CSoundMgr::CHANNELID eID, const _float& fVolum = 0.5f);




// Font
inline HRESULT	Ready_Font(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFontTag, const _tchar* pFontType, const _uint& iWidth, const _uint& iHeight, const _uint& iWeight);
inline void	Render_Font(const _tchar* pFontTag, const _tchar* pString, const _vec2* pPos, D3DXCOLOR Color);



#include "Export_System.inl"

END


#endif // Export_System_h__
