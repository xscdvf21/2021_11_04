#ifndef Logo_h__
#define Logo_h__

#include "Scene.h"
#include "Base.h"
#include "define.h"

#include "Loading.h"

#include "LogoBack.h"
#include "TestPlayer.h"
#include "TestMonster.h"


class CLogo : public Engine::CScene
{
private:
	explicit CLogo(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLogo(void);

public:
	virtual HRESULT Ready_Scene(void) override;
	virtual _int Update_Scene(const _float& fTimeDelta) override;
	virtual void Render_Scene(void) override;
	

	_bool			Get_LoadingEnd() { return m_bLoadingEnd; }

private:
	HRESULT			Ready_Resources(LPDIRECT3DDEVICE9& pGraphicDev, RESOURCEID eMax);
	HRESULT			Ready_Environment_Layer(const _tchar*  pLayerTag);
	HRESULT			Ready_GameLogic_Layer(const _tchar*  pLayerTag) { return S_OK; }
	HRESULT			Ready_UI_Layer(const _tchar* pLayerTag) { return S_OK; }

private:
	_matrix					m_matWorld, m_matView, m_matProj;

	CLoading*				m_pLoading;

	_bool					m_bLoadingEnd = false;

public:
	static CLogo*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free(void) override;

};

#endif // Logo_h__
