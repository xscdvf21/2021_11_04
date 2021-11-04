#ifndef __SP_POTION_H__

#include "GameObject.h"
#include "Base.h"
#include "define.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CRenderer;
class CTransform;

END


class CSP_Potion : public Engine::CGameObject
{
private:
	explicit CSP_Potion(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSP_Potion(void);

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& TimeDelta) override;
	virtual void Render_Object(void) override;

private:
	HRESULT			Add_Component(void);


private:
	_matrix					m_matProj;
	_float					m_fX, m_fY;
	_float					m_fSizeX, m_fSizeY;



private:
	Engine::CRcTex*			m_pBufferCom = nullptr;
	Engine::CTexture*		m_pTextureCom = nullptr;
	Engine::CRenderer*		m_pRendererCom = nullptr;
	Engine::CTransform*		m_pTransformCom = nullptr;

public:
	static CSP_Potion*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free(void) override;

};


#define __SP_POTION_H__
#endif // !__SP_POTION_H__



