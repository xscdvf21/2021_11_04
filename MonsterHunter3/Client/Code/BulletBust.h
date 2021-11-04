#ifndef __BULLETBUST_H__

#include "GameObject.h"
#include "Base.h"
#include "define.h"

BEGIN(Engine)
class CRcTex;
class CTexture;
class CRenderer;
class CTransform;
END



class CBulletBust : public Engine::CGameObject
{
public:
	explicit CBulletBust(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBulletBust(void);


public:
	virtual HRESULT Ready_Object(_vec3 vPos);
	virtual _int Update_Object(const _float& TimeDelta) override;
	virtual void Render_Object(void) override;

private:
	HRESULT			Add_Component(void);


	_float			m_fFrame = 0.f;
	_float			m_fLifeTime = 30.f; //라이프타임
	_float			m_fLiftDelTime = 0.f;//라이프 타임 시간 누적할 변수.

private:
	Engine::CRcTex*			m_pBufferCom = nullptr;
	Engine::CTexture*		m_pTextureCom = nullptr;
	Engine::CRenderer*		m_pRendererCom = nullptr;
	Engine::CTransform*		m_pTransformCom = nullptr;


public:
	static CBulletBust*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 _Pos = { 0.f, 0.f, 0.f });
private:
	virtual void Free(void) override;
};



#define __BULLETBUST_H__
#endif // !__BULLETBUST_H__



