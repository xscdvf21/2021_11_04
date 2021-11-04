#ifndef __HITPLAYER_H__


#include "GameObject.h"
#include "Base.h"
#include "define.h"

BEGIN(Engine)
class CRcTex;
class CTexture;
class CRenderer;
class CTransform;
END

class CHitPlayer : public Engine::CGameObject
{
private:
	explicit			CHitPlayer(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual				~CHitPlayer(void);

public:
	virtual HRESULT		Ready_Object(_vec3 vPos = { 0.f, 0.f,0.f });
	virtual _int		Update_Object(const _float& TimeDelta) override;
	virtual void		Render_Object(void) override;

private:
	HRESULT				Add_Component(void);

	_float				m_fFrame = 0.f;
	_float				m_fLifeTime = 0.5f; //라이프타임
	_float				m_fLiftDelTime = 0.f;//라이프 타임 시간 누적할 변수.

private:
	Engine::CRcTex*			m_pBufferCom = nullptr;
	Engine::CTexture*		m_pTextureCom = nullptr;
	Engine::CRenderer*		m_pRendererCom = nullptr;
	Engine::CTransform*		m_pTransformCom = nullptr;


public:
	static CHitPlayer*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos = { 0.f, 0.f, 0.f });
private:
	virtual void			Free(void) override;


};


#define __HITPLAYER_H__
#endif // !__HITPLAYER_H__


