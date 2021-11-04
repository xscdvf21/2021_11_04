#ifndef __BLOOD_H__

#include "GameObject.h"
#include "Base.h"
#include "define.h"

BEGIN(Engine)
class CRcTex;
class CTexture;
class CRenderer;
class CTransform;
END



class CBlood : public Engine::CGameObject
{
	enum BLOODID { BLOOD_0, BLOOD_1, BLOOD_2, BLOOD_3, BLOOD_END};
public:
	explicit			 CBlood(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual				~CBlood(void);

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& TimeDelta) override;
	virtual void		Render_Object(void) override;




private:
	HRESULT				Add_Component(void);


	void				Render_Blood0();
	void				Render_Blood1();
	void				Render_Blood2();
	void				Render_Blood3();
	void				Render_Blood4();


	_float				m_fFrame = 0.f;
	_float				m_fLifeTime = 2.f; //라이프타임
	_float				m_fLiftDelTime = 0.f;//라이프 타임 시간 누적할 변수.

private:
	Engine::CRcTex*			m_pBufferCom = nullptr;
	Engine::CTexture*		m_pTextureCom[BLOOD_END];
	Engine::CRenderer*		m_pRendererCom = nullptr;
	Engine::CTransform*		m_pTransformCom = nullptr;

	_matrix					m_matProj;
	_float					m_fX, m_fY;
	_float					m_fSizeX, m_fSizeY;


public:
	static CBlood*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free(void) override;


};




#define __BLOOD_H__
#endif // !__BLOOD_H__


