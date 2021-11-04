#ifndef __ROOKMOVEBULLET_H__

#include "GameObject.h"
#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CStaticMesh;
class CCalculator;
class CCollider;
class CShader;

END

class CRookMoveBullet : public Engine::CGameObject
{
private:
	explicit CRookMoveBullet(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CRookMoveBullet();

public:
	virtual HRESULT Ready_Object(_vec3 vPos, _vec3 vDir, _uint iCount);
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual _int	LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void	Render_Object(const _int& iIndex = 0) override;

private:
	HRESULT			Add_Component();
	HRESULT			SetUp_ConstantTable(LPD3DXEFFECT& pEffect, const _int& iIndex);

	void			Move(const _float& fTimeDelta);

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
	_vec3						m_vCurrentPos; //ó�� ������ ���� ���.


private:
	void			Effect_Walk();

private:
	_float			m_fEffecTime = 0.f;

public:
	static CRookMoveBullet*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, _vec3 vDir, _uint iCount);
	virtual void Free(void)		override;
};

#define __ROOKMOVEBULLET_H__
#endif


