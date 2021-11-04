#ifndef UI_h__
#define UI_h__

#include "GameObject.h"
#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CVIBuffer;
class CTexture;
class CRenderer;
class CShader;

END

class CUI : public Engine::CGameObject
{
protected:
	explicit CUI(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CUI(void);

public:
	virtual HRESULT Ready_Object(void* pArg = nullptr) override;
	virtual _int	Update_Object(const _float& TimeDelta) override;
	virtual _int	LateUpdate_Object(const _float& TimeDelta) override;
	virtual void	Render_Object(const _int& iIndex = 0) override;

public:
	void					Set_UIAddPos(_float X, _float Y) { m_fX += X; m_fY += Y; }
	void					Set_UIPos(_float X, _float Y) { m_fX = X; m_fY = Y; }
	void					Set_Visible(const _bool& bState) { m_bVisible = bState; }
	const _bool				Is_Visible() { return m_bVisible; }

public:
	_vec2					Get_UIPos() { return _vec2(m_fX, m_fY); }

protected:
	HRESULT					Add_Component(void* pArg = nullptr);
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT& pEffect);

protected:
	_matrix					m_matProj;
	_float					m_fX, m_fY;
	_float					m_fSizeX, m_fSizeY;
	_uint					m_iRenderState;
	TCHAR					m_tagTexture[MAX_PATH];
	_float					m_fAlpha;

protected:
	_float					m_fBubbleTime;
	_matrix					m_matWorld;
	_matrix					m_matOldView;
	_matrix					m_matOldProj;
	_bool					m_bVisible=true;

protected:
	Engine::CVIBuffer*		m_pVIBufferCom = nullptr;
	Engine::CTexture*		m_pTextureCom = nullptr;
	Engine::CShader*		m_pShaderCom = nullptr;

public:
	static CUI*				Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg = nullptr);

private:
	virtual void Free(void) override;



};



#endif // UI_h__
