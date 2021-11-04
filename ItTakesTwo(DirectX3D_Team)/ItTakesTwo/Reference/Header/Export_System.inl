
// GraphicDev
HRESULT	Ready_GraphicDev(HWND hWnd, WINMODE eMode, const _uint& iSizeX, const _uint& iSizeY, CGraphicDevice** ppGraphicDev)
{
	return CGraphicDevice::GetInstance()->Ready_GraphicDev(hWnd, eMode, iSizeX, iSizeY, ppGraphicDev);
}

void Render_Begin(D3DXCOLOR Color)
{
	CGraphicDevice::GetInstance()->Render_Begin(Color);
}

void Render_End(void)
{
	CGraphicDevice::GetInstance()->Render_End();
}


// TimerMgr
_float Get_TimeDelta(const _tchar* pTimerTag)
{
	return CTimerMgr::GetInstance()->Get_TimeDelta(pTimerTag);
}

void Set_TimeDelta(const _tchar* pTimerTag)
{
	CTimerMgr::GetInstance()->Set_TimeDelta(pTimerTag);
}

HRESULT Ready_Timer(const _tchar* pTimerTag)
{
	return CTimerMgr::GetInstance()->Ready_Timer(pTimerTag);
}


// FrameMgr
_bool IsPermit_Call(const _tchar* pFrameTag, const _float& fTimeDelta)
{
	return CFrameMgr::GetInstance()->IsPermit_Call(pFrameTag, fTimeDelta);
}

HRESULT	Ready_Frame(const _tchar* pFrameTag, const _float& fCallLimit)
{
	return CFrameMgr::GetInstance()->Ready_Frame(pFrameTag, fCallLimit);
}

// PhysX
HRESULT Engine::Ready_Physics(Engine::CPhys_Manager** pManager)
{
	return CPhys_Manager::GetInstance()->Ready_Physics(pManager);
}
physx::PxPhysics* Engine::Get_Physics()
{
	return CPhys_Manager::GetInstance()->Get_Physics();
}

physx::PxDefaultCpuDispatcher* Engine::Get_Dispatcher()
{
	return CPhys_Manager::GetInstance()->Get_Dispatcher();
}
physx::PxDefaultAllocator* Engine::Get_Allocator()
{
	return CPhys_Manager::GetInstance()->Get_Allocator();

}
physx::PxCudaContextManager* Engine::Get_CudaMgr()
{
	return CPhys_Manager::GetInstance()->Get_CudaMgr();

}


// DInput
_bool	Key_Pressing(_ubyte byKeyID)
{
	return CInputDev::GetInstance()->Key_Pressing(byKeyID);
}
_bool	Key_Down(_ubyte byKeyID)
{
	return CInputDev::GetInstance()->Key_Down(byKeyID);
}
_bool	Key_Up(_ubyte byKeyID)
{
	return CInputDev::GetInstance()->Key_Up(byKeyID);
}
_bool	Key_Pressing(MOUSEKEYSTATE eMouse)
{
	return CInputDev::GetInstance()->Key_Pressing(eMouse);
}
_bool	Key_Down(MOUSEKEYSTATE eMouse)
{
	return CInputDev::GetInstance()->Key_Down(eMouse);
}
_bool	Key_Up(MOUSEKEYSTATE eMouse)
{
	return CInputDev::GetInstance()->Key_Up(eMouse);
}

_bool	Key_Pressing(PADKEYSTATE ePad)
{
	return CInputDev::GetInstance()->Key_Pressing(ePad);
}
_bool	Key_Down(PADKEYSTATE ePad)
{
	return CInputDev::GetInstance()->Key_Down(ePad);
}
_bool	Key_Up(PADKEYSTATE ePad)
{
	return CInputDev::GetInstance()->Key_Up(ePad);
}
_byte	Get_DIKeyState(_ubyte byKeyID)
{
	return CInputDev::GetInstance()->Get_DIKeyState(byKeyID);
}

_byte	Get_DIMouseState(MOUSEKEYSTATE eMouse)
{
	return CInputDev::GetInstance()->Get_DIMouseState(eMouse);
}

_long	Get_DIMouseMove(MOUSEMOVESTATE eMouseState)
{
	return CInputDev::GetInstance()->Get_DIMouseMove(eMouseState);
}

Engine::_float Engine::Get_DIPadMove(PADMOVESTATE eMouseState)
{
	return CInputDev::GetInstance()->Get_DIControllerMove(eMouseState);
}
HRESULT Ready_InputDev(HINSTANCE hInst, HWND hWnd)
{
	return CInputDev::GetInstance()->Ready_InputDev(hInst, hWnd);
}

void	SetUp_Key()
{
	CInputDev::GetInstance()->SetUp_Key();
}

void	SetUp_Mouse()
{
	CInputDev::GetInstance()->SetUp_Mouse();
}

void	Set_InputDev(void)
{
	CInputDev::GetInstance()->Set_InputDev();
}


// Sound
void		Initialize()
{
	CSoundMgr::GetInstance()->Initialize();
}

void		PlaySoundW(TCHAR* pSoundKey, Engine::CSoundMgr::CHANNELID eID, const _float& fVolume)
{
	CSoundMgr::GetInstance()->PlaySoundW(pSoundKey, eID, fVolume);
}

void		PlayBGM(TCHAR* pSoundKey, const _float& fVolume)
{
	CSoundMgr::GetInstance()->PlayBGM(pSoundKey, fVolume);
}

void		StopSound(Engine::CSoundMgr::CHANNELID eID)
{
	CSoundMgr::GetInstance()->StopSound(eID);
}
void		StopAll()
{
	CSoundMgr::GetInstance()->StopAll();
}
void Engine::Load_SoundFile(const string tagPath, const RESOURCEID& eResourceID)
{
	CSoundMgr::GetInstance()->Load_SoundFile(tagPath, eResourceID);
}
void Engine::Set_SoundScene(const RESOURCEID& eResourceID)
{
	CSoundMgr::GetInstance()->Set_CurScene(eResourceID);
}
void Engine::Release_Sound(RESOURCEID eResourceID)
{
	CSoundMgr::GetInstance()->Release_Sound(eResourceID);
}
void Engine::StopNPlaySound(TCHAR * pSoundKey, CSoundMgr::CHANNELID eID, const _float& fVolum)
{
	CSoundMgr::GetInstance()->StopNPlaySound(pSoundKey,eID,fVolum);

}

_bool Engine::Is_Play_End(CSoundMgr::CHANNELID eID)
{
	return CSoundMgr::GetInstance()->Is_Play_End(eID);
}


// Font
HRESULT	Ready_Font(LPDIRECT3DDEVICE9 pGraphicDev,
	const _tchar* pFontTag,
	const _tchar* pFontType,
	const _uint& iWidth,
	const _uint& iHeight,
	const _uint& iWeight)
{
	return CFontMgr::GetInstance()->Ready_Font(pGraphicDev, pFontTag, pFontType, iWidth, iHeight, iWeight);
}

void	Render_Font(const _tchar* pFontTag,
	const _tchar* pString,
	const _vec2* pPos,
	D3DXCOLOR Color)
{
	CFontMgr::GetInstance()->Render_Font(pFontTag, pString, pPos, Color);
}

// Release System
void Release_System(void)
{
	CPhys_Manager::GetInstance()->DestroyInstance();
	CSoundMgr::GetInstance()->DestroyInstance();
	CInputDev::GetInstance()->DestroyInstance();
	CFontMgr::GetInstance()->DestroyInstance();
	CFrameMgr::GetInstance()->DestroyInstance();
	CTimerMgr::GetInstance()->DestroyInstance();
	CGraphicDevice::GetInstance()->DestroyInstance();
}