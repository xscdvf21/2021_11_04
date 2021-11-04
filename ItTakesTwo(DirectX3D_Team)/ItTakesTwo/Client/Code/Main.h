#ifndef Main_h__
#define Main_h__

#include "Base.h"
#include "Engine_Define.h"
#include "Logo.h"

BEGIN(Engine)

class CGraphicDevice;
class CManagement;
class CPhys_Manager;
END

class CMain : public CBase
{
private:					// 생성자, 소멸자
	explicit CMain(void);
	virtual ~CMain(void);

	// public 함수 ,변수
	// protected
	// private

public:
	HRESULT			Ready_Main(void);
	int				Update_Main(const _float& fTimeDelta);
	int				LateUpdate_Main(const _float& fTimeDelta);
	void			Render_Main(const _float & fTimeDelta);

private:
	HRESULT			SetUp_DefaultSetting(LPDIRECT3DDEVICE9* ppGraphicDev);
	HRESULT			Ready_Scene(LPDIRECT3DDEVICE9& pGraphicDev, Engine::CManagement** ppManagementClass);

private:
	LPDIRECT3DDEVICE9			m_pGraphicDev;
	Engine::CGraphicDevice*		m_pDeviceClass= nullptr;
	Engine::CManagement*		m_pManagementClass=nullptr;
	Engine::CPhys_Manager*		m_pPhysicsClass = nullptr;
	CLoading*					m_pLoading;
	_bool						m_bStart =true;
	_ulong						m_dwRenderCnt = 0;
	_tchar						m_szFPS[256] = L"";
	_float						m_fTime = 0.f;


public:
	static CMain*				Create(void);

private:
	virtual void				Free(void);
};
#endif // Main_h__

