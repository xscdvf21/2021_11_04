#ifndef MenuUI_h__
#define MenuUI_h__

#include "GameObject.h"
#include "Base.h"
#include "Engine_Define.h"
#include "UI.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CRenderer;
class CShader;

END

class CMenuUI : public CUI
{
private:
	explicit CMenuUI(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMenuUI(void);

public:
	virtual HRESULT Ready_Object(void* pArg = nullptr) override;
	virtual _int	Update_Object(const _float& TimeDelta) override;
	virtual _int	LateUpdate_Object(const _float& TimeDelta) override;
	virtual void	Render_Object(const _int& iIndex = 0) override;

private:
	HRESULT					Add_Component(void* pArg = nullptr);
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT& pEffect);

public:
	void					Set_MenuRender(_bool menurender) { m_bMenuRender = menurender; }
	void					Set_PosUI(_float fx, _float fy) { m_fX = fx; m_fY = fy; }
	void					Set_Number(_int number) { m_iNumber = number; }
	_vec2					Get_PosUI() { return _vec2(m_fX, m_fY); }

private:
	_matrix					m_matProj;
	_float					m_fX, m_fY;
	_float					m_fSizeX, m_fSizeY;
	_float					m_fAlpha;
	_uint					m_iRenderState;
	_int					m_iNumber = 0;
	TCHAR					m_tagTexture[MAX_PATH];

private:
	_float					m_fBubbleTime;
	_matrix					m_matWorld;
	_bool					m_bFade;
	_bool					m_bMenuRender;

private:
	Engine::CRcTex*			m_pBufferCom = nullptr;
	Engine::CTexture*		m_pTextureCom = nullptr;
	Engine::CShader*		m_pShaderCom = nullptr;

public:
	static CMenuUI*		Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg = nullptr);

private:
	virtual void Free(void) override;



};



#endif // MenuUI_h__
