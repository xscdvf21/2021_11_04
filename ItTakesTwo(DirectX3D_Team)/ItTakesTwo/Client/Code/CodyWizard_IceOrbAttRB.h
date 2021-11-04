#ifndef __CODYWIZARD_ICEORBATTRB_H__

#include "Player.h"

BEGIN(Engine)

class CStaticMesh;
class CCalculator;
class CCollider;
class CShader;

END

class CCodyWizard_IceOrbAttRB : public CPlayer
{
private:
	explicit CCodyWizard_IceOrbAttRB(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCodyWizard_IceOrbAttRB();


public:
	virtual HRESULT Ready_Object(_vec3 vDir, CLight* pLight, void* pVoid = nullptr);
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual _int	LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void	Render_Object(const _int& iIndex = 0) override;


	virtual void	Set_Dead(_bool bStack = false) override;
private:
	HRESULT			Add_Component(void* pVoid = nullptr);
	HRESULT			SetUp_ConstantTable(LPD3DXEFFECT& pEffect, const _int& iIndex);

	void			Move(const _float& fTimeDelta);
	void			PhyscisCreate();

private:
	Engine::CStaticMesh*		m_pMeshCom = nullptr;
	Engine::CCalculator*		m_pCalculatorCom = nullptr;
	Engine::CCollider*			m_pColliderCom = nullptr;
	Engine::CShader*			m_pShaderCom = nullptr;

private:
	_vec3						m_vDir = { 0.f, 0.f, 0.f }; //나아갈 방향벡터.
	_float						m_fLifeTime = 0.f;
	_float						m_fLifeDir = 8.f;
	_vec3						m_vStartPos;
	_float						m_fAttRadius;

private:
	void						Effect();
	void						Effect_Explosive();

private:
	_float						m_fEffectTime = 0.f;
	CLight*						m_pLight = nullptr;

public:
	static CCodyWizard_IceOrbAttRB*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vDir, CLight* pLight, void* pVoid = nullptr);
	virtual void					Free(void)		override;


};

#define __CODYWIZARD_ICEORBATTRB_H__
#endif

