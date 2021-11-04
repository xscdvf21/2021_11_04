#pragma once
#include "GameObject.h"

enum BULLETTYPE
{
	TYPE_A
	, TYPE_B
	, TYPE_C
	, TYPE_D
	, TYPE_END
};
USING(Engine)
class CMiddleBossABullet :
	public CGameObject
{
private:
	explicit CMiddleBossABullet(LPDIRECT3DDEVICE9 pDevice);
	explicit CMiddleBossABullet(const CMiddleBossABullet& other);
	virtual ~CMiddleBossABullet() = default;


public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void * pArg = nullptr) override;
	virtual _uint Update_GameObject(_float fDeltaTime) override;
	virtual _uint LateUpdate_GameObject(_float fDeltaTime) override;
	virtual HRESULT Render_GameObject() override;

private:
	HRESULT Add_Component();

public:
	static CMiddleBossABullet* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	virtual void Free() override;

public://Get
	void Set_Dir(const _float3 _vDir) { m_vDir = _vDir; }
	void Set_BulletPos(const _float3 _vPos) { m_pTransformCom->Initialize_vPos(_vPos);}
	void Set_BulletRot(const _float3 _vRot) { m_pTransformCom->Initialize_vRotate(_vRot); }
	void Set_Accel(const _float _fAccel) { m_fAccel = _fAccel; }
	void Set_BulletType(const BULLETTYPE _Type) { m_eBulletType = _Type; }

private:
	class CVIBuffer*		m_pVIBufferCom = nullptr;
	class CTexture*			m_pTextureCom;

	float m_fRotY = 0.f;
	float m_fLifeTime;

	
	float m_fAccel = 0.f;
	float m_fAccel2 = 0.f;

	float m_fDeccel = 0.f;
	float m_fDeccel2 = 0.f;



	BULLETTYPE m_eBulletType = BULLETTYPE::TYPE_END;

	_float3 m_vDir = {};
};

