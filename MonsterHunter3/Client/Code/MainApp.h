#ifndef MainApp_h__
#define MainApp_h__

#include "Base.h"
#include "Engine_Define.h"
#include "define.h"

#include "Logo.h"
#include "Stage.h"

BEGIN(Engine)

class CGraphicDev;
class CManagement;

END

class CMainApp : public CBase
{
private:					// 생성자, 소멸자
	explicit CMainApp(void);
	virtual ~CMainApp(void);

	// public 함수 ,변수
	// protected
	// private

public:
	HRESULT			Ready_MainApp(void);
	int				Update_MainApp(const _float& fTimeDelta);
	void			Render_MainApp(void);

private:
	HRESULT			SetUp_DefaultSetting(LPDIRECT3DDEVICE9* ppGraphicDev);
	HRESULT			Ready_Scene(LPDIRECT3DDEVICE9& pGraphicDev, Engine::CManagement** ppManagementClass);

private:
	LPDIRECT3DDEVICE9			m_pGraphicDev;
	Engine::CGraphicDev*		m_pDeviceClass;
	Engine::CManagement*		m_pManagementClass;

	_ulong						m_dwRenderCnt = 0;
	_tchar						m_szFPS[256] = L"";
	_float						m_fTime = 0.f;

	
public:	// 생성함수
	static CMainApp*		Create(void);
private:// 소멸함수
	virtual void			Free(void);

};

#endif // MainApp_h__
