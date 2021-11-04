#ifndef Ending_h__
#define Ending_h__


#include "Scene.h"
#include "Base.h"
#include "Loading.h"
#include "UI.h"

class CEnding : public CScene
{
private:
	explicit CEnding(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CEnding();

public:
	virtual HRESULT Ready_Scene(void) override;
	virtual _int	Update_Scene(const _float& fTimeDelta) override;
	virtual _int	LateUpdate_Scene(const _float& fTimeDelta) override;
	virtual void	Render_Scene(void) override;

private:
	HRESULT			Ready_GameObject_Layer(const _tchar*  pLayerTag);

private:
	_bool			m_bChange;
	_bool			m_bLock;
	_bool			m_bLock2;
	_bool			m_bSelMenu;
	_float			m_fLock;
	_bool			m_bSceneChange;

private:
	CImageUI*		m_pEnding = nullptr;

public:
	static CEnding*	Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void	Free() override;

private:
	_matrix			m_matWorld, m_matView, m_matProj;
};

#endif // Ending_h__
