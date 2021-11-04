#ifndef __FLYWIND_H__

#include "GameObject.h"
#include "Base.h"
#include "define.h"

BEGIN(Engine)
class CRcTex;
class CTexture;
class CRenderer;
class CTransform;
END



class CFlyWind : public Engine::CGameObject
{
private:
	explicit			CFlyWind(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual				~CFlyWind(void);

public:
	virtual HRESULT		Ready_Object(_vec3 vPos);
	virtual _int		Update_Object(const _float& TimeDelta) override;
	virtual void		Render_Object(void) override;

private:
	HRESULT				Add_Component(void);


	_float				m_fFrame = 0.f;
	_float				m_fLifeTime = 0.5f; //������Ÿ��
	_float				m_fLiftDelTime = 0.f;//������ Ÿ�� �ð� ������ ����.

private:
	Engine::CRcTex*			m_pBufferCom = nullptr;
	Engine::CTexture*		m_pTextureCom = nullptr;
	Engine::CRenderer*		m_pRendererCom = nullptr;
	Engine::CTransform*		m_pTransformCom = nullptr;


public:
	static CFlyWind*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);
private:
	virtual void		Free(void) override;



};

#define __FLYWIND_H__
#endif // !__FLYWIND_H__

