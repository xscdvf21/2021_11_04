#ifndef __VACUUMCIRCLE_H__

#include "GameObject.h"
#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CTexture;
class CRcTex;
class CShader;

END


enum SoundID {
	Sound_Effect,
	Sound_End
};


class CVacuumCircle : public CGameObject
{
private:
	explicit CVacuumCircle(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CVacuumCircle();

public:
	virtual HRESULT Ready_Object(_vec3 vPos);
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual _int	LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void	Render_Object(const _int& iIndex = 0) override;

private:
	HRESULT			Add_Component();
	HRESULT			SetUp_ConstantTable(LPD3DXEFFECT& pEffect, const _int& iIndex);

	void			Damage(const _float& fTimeDelta);
private:
	Engine::CRcTex*				m_pBufferCom = nullptr;
	Engine::CTexture*			m_pTextureCom = nullptr;
	Engine::CShader*			m_pShaderCom = nullptr;

private :
	_vec3						m_vDir = { 0.f, 0.f, 0.f }; //나아갈 방향벡터.
	_vec3						m_vTargetPos = { 0.f, 0.f, 0.f }; //인자로 받은 타켓 포스.
	_vec3						m_vPos = { 0.f, 0.f, 0.f }; //생성된 자신의 위치
	_float						m_fLifeTime = 0.f;

	_bool						m_bCodyHit = false;
	_bool						m_bMayHit = false;

	_vec3						m_vScale;
	_float						m_fCodyHitTime;
	_float						m_fMayHitTime;
public:
	static CVacuumCircle*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);
	virtual void Free(void)		override;



};

#define __VACUUMBOMB_H__
#endif

