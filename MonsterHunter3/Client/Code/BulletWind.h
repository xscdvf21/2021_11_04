#ifndef __BULLETWIND_H__

#include "GameObject.h"
#include "Base.h"
#include "define.h"

BEGIN(Engine)
class CRcTex;
class CTexture;
class CRenderer;
class CTransform;
END

class CBulletWind : public Engine::CGameObject
{
private :
	explicit			CBulletWind(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual				~CBulletWind(void);

public:
	virtual HRESULT		Ready_Object(_vec3 vDir, _vec3 vPos, _float fDir = 1.f);
	virtual _int		Update_Object(const _float& TimeDelta) override;
	virtual void		Render_Object(void) override;

private:
	HRESULT				Add_Component(void);

	_float				m_fFrame = 0.f;
	_float				m_fLifeTime = 1.f; //������Ÿ��
	_float				m_fLiftDelTime = 0.f;//������ Ÿ�� �ð� ������ ����.

private:
	Engine::CRcTex*			m_pBufferCom = nullptr;
	Engine::CTexture*		m_pTextureCom = nullptr;
	Engine::CRenderer*		m_pRendererCom = nullptr;
	Engine::CTransform*		m_pTransformCom = nullptr;

	_vec3					m_vMoveDir;

public:
	static CBulletWind*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vDir, _vec3 vPos, _float fDir = 1.f);
private:
	virtual void			Free(void) override;


};

#define __BULLETWIND_H__
#endif // !__BULLETWIND_H__


