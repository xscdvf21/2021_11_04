#ifndef __CODYWIZARD_ICEORBATT_H__

#include "Player.h"

BEGIN(Engine)

class CStaticMesh;
class CCalculator;
class CCollider;
class CShader;

END

class CCodyWizard_IceOrbAtt : public CPlayer
{
private:
	explicit CCodyWizard_IceOrbAtt(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCodyWizard_IceOrbAtt();


public:
	virtual HRESULT Ready_Object(_vec3 vDir, void* pVoid = nullptr, _float fLifeDir = 8.f);
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

private :
	_vec3						m_vDir = { 0.f, 0.f, 0.f }; //나아갈 방향벡터.
	_float						m_fLifeTime = 0.f;

	_float						m_fLifeDir = 8.f;
	_vec3						m_vStartPos;

	_bool						m_bAddSize = false;
	

public:
	static CCodyWizard_IceOrbAtt*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _int iDamage, _vec3 vDir, void* pVoid = nullptr ,_float fLifeDir = 8.f);
	virtual void					Free(void)		override;


};

#define __CODYWIZARD_ICEORBATT_H__
#endif

