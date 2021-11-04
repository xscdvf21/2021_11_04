#ifndef __BISHOPMOVEBULLET_H__

#include "Enermy.h"

BEGIN(Engine)

class CStaticMesh;
class CCalculator;
class CCollider;
class CShader;

END

class CBiShopMoveBullet : public CEnermy
{
private:
	explicit CBiShopMoveBullet(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBiShopMoveBullet();

public:
	virtual HRESULT Ready_Object(_vec3 vPos, _vec3 vDir, _uint iUintCount ,_uint iCount);
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual _int	LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void	Render_Object(const _int& iIndex = 0) override;

private:
	HRESULT			Add_Component();
	HRESULT			SetUp_ConstantTable(LPD3DXEFFECT& pEffect, const _int& iIndex);

	void			Move(const _float& fTimeDelta);

	virtual void	Set_Dead(_bool bStack = false) override;
private:
	Engine::CStaticMesh*		m_pMeshCom = nullptr;
	Engine::CCalculator*		m_pCalculatorCom = nullptr;
	Engine::CCollider*			m_pColliderCom = nullptr;
	Engine::CShader*			m_pShaderCom = nullptr;

	_float						m_fLifeTime = 0.f;
	_vec3						m_vDir; //���ڷ� �޴� ���ư� ���� ����
	_float						m_fScale; //������� Ŀ���� ������ �ϱ�����.
	_uint						m_iBulletCount; //�ҷ��� �±׸� �ٸ����Ϸ���,

	_uint						m_iBulletName;
	_uint						m_iUnitName;
	_vec3						m_vCurrentPos; //ó�� ������ ���� ���.

	_bool						m_bCol;

private:
	void			Effect_Walk();

private:
	_float			m_fEffecTime = 0.f;

public:
	static CBiShopMoveBullet*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, _vec3 vDir, _uint iUintCount,_uint iCount);
	virtual void Free(void)		override;


};

#define __BISHOPMOVEBULLET_H__
#endif

