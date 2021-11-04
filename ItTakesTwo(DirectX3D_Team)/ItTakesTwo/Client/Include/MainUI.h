#ifndef __MAINUI_H__

#include "GameObject.h"
#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CRenderer;
class CTransform;
class CShader;

END

class CMainUI : public Engine::CGameObject
{
protected:
	explicit	CMainUI(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual		~CMainUI();

public:
	virtual HRESULT			Ready_Object(void* pArg = nullptr) override;
	virtual _int			Update_Object(const _float& TimeDelta) override;
	virtual _int			LateUpdate_Object(const _float& TimeDelta) override;
	virtual void			Render_Object(const _int& iIndex = 0) override;


public:
	void					Set_UIAddPos(_float X, _float Y) { m_fX += X; m_fY += Y; }
	void					Set_UIPos(_float X, _float Y) { m_fX = X; m_fY = Y; }


public:
	_vec2					Get_UIPos() { return _vec2(m_fX, m_fY); }

private:
	HRESULT					Add_Component(void* pArg = nullptr);
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT& pEffect);


private:
	Engine::CRcTex*			m_pBufferCom = nullptr;
	Engine::CTexture*		m_pTextureCom = nullptr;
	Engine::CTransform*		m_pTransformCom = nullptr;
	Engine::CShader*		m_pShaderCom = nullptr;

public:
	_matrix					m_matWorld;
	_matrix					m_matOldView;
	_matrix					m_matOldProj;

public:
	_matrix					m_matProj;
	_float					m_fX, m_fY;
	_float					m_fSizeX, m_fSizeY;
	_uint					m_iRenderState;
	TCHAR					m_tagTexture[MAX_PATH];
	_float					m_fAlpha;

public:
	static CMainUI*				Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg = nullptr);

private:
	virtual void Free(void) override;
};

#define __MAINUI_H__
#endif


