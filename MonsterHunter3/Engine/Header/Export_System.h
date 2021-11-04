#ifndef Export_System_h__
#define Export_System_h__

#include "GraphicDev.h"
#include "TimerMgr.h"
#include "FrameMgr.h"
#include "FontMgr.h"
#include "InputDev.h"

BEGIN(Engine)

// GraphicDev
// Get
// Set
// General
inline HRESULT	Ready_GraphicDev(HWND hWnd,WINMODE eMode,const _uint& iSizeX,	const _uint& iSizeY,	CGraphicDev** ppGraphicDev);
inline void		Render_Begin(D3DXCOLOR Color);
inline void		Render_End(void);


// TimerMgr
// Get
inline _float		Get_TimeDelta(const _tchar* pTimerTag);
// Set
inline void			Set_TimeDelta(const _tchar* pTimerTag);
// General
inline HRESULT		Ready_Timer(const _tchar* pTimerTag);

inline void			Release_System(void);

// FrameMgr
// General
inline _bool				IsPermit_Call(const _tchar* pFrameTag, const _float& fTimeDelta);
inline HRESULT				Ready_Frame(const _tchar* pFrameTag, const _float& fCallLimit);


// FontMgr
// Get
// Set
// General
inline HRESULT	Ready_Font(LPDIRECT3DDEVICE9 pGraphicDev,
	const _tchar* pFontTag,
	const _tchar* pFontType,
	const _uint& iWidth,
	const _uint& iHeight,
	const _uint& iWeight);

inline void	Render_Font(const _tchar* pFontTag,
	const _tchar* pString,
	const _vec2* pPos,
	D3DXCOLOR Color);

inline void	Render_Font3D(const _tchar* pFontTag,  const _tchar* pString, const _vec2* pPos, const _matrix _PlayerWorld, D3DXCOLOR Color);

// dinput
// Get
inline _byte	Get_DIKeyState(_ubyte byKeyID);
inline _byte	Get_DIMouseState(MOUSEKEYSTATE eMouse);
inline _long	Get_DIMouseMove(MOUSEMOVESTATE eMouseState);
inline _bool	Key_Down(_ubyte byKeyID);

// Set
// General
inline	HRESULT Ready_InputDev(HINSTANCE hInst, HWND hWnd);
inline	void	Set_InputDev(void);
inline	void	Update_KeyMgr();


#include "Export_System.inl"

END


#endif // Export_System_h__
