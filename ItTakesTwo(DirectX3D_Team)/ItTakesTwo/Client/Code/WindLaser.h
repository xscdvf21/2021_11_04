#ifndef __WINDLASER_H__

#include "GameObject.h"
#include "Base.h"
#include "Engine_Define.h"
#include "SpaceLaser.h"

BEGIN(Engine)

class CStaticMesh;
class CTransform;
class CCalculator;
class CCollider;
class CShader;

END

class CWindLaser : public CGameObject
{

	enum LaserAngle
	{
		LaserLook,
		LaserLook45,
		LaserLook90,
		LaserLook135,
		LaserLook180,
		LaserLook225,
		LaserLook270,
		LaserLook315,
		LaserLookEnd
	};
private:
	explicit CWindLaser(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CWindLaser();

public:
	virtual HRESULT		Ready_Object(void* pArg = nullptr);
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual _int		LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void		Render_Object(const _int& iIndex = 0) override;

public:
	void				Set_AttUp() { m_bAttUp = true; }

private:
	HRESULT				Add_Component(void* pArg = nullptr);
	HRESULT				SetUp_ConstantTable(LPD3DXEFFECT& pEffect, const _int& iIndex);

private:
	void				MoveUp(const _float& fTimeDelta);
	void				Effect_LaserHit(const _float & fTimeDelta);
	void				AttUp(const _float& fTimeDelta);
	void				AttDown(const _float& fTimeDelta);
	void				Create_Laser(const _tchar* pLayerTag);
	void				Effect_Laser(const _float& fTimeDelta);

private:
	Engine::CStaticMesh*			m_pMeshCom = nullptr;
	Engine::CTransform*				m_pTransformCom = nullptr;
	Engine::CCalculator*			m_pCalculatorCom = nullptr;
	Engine::CCollider*				m_pColliderCom = nullptr;
	Engine::CShader*				m_pShaderCom = nullptr;
private:
	_bool							m_bUp;
	_bool							m_bMoveDir;
	_bool							m_bAttUpDir;
	_bool							m_bAttDownDir;
	_bool							m_bAttUp;
	_bool							m_bAttDown;

	_float							m_fAttSpeed;

	_vec3							m_vAttUpStartPos;
	_vec3							m_vMoveUpStartPos;
	_vec3							m_vAttDownStartPos;


	_vec3							m_vMoveDir;
	_vec3							m_vAttDir;

	_float							m_fUpDis;
	_float							m_fAttDis;

private: //Ray
	_bool		m_pLaserEffect = false;

	_float		m_fDistance = 0.f;
	_float		m_fMaxDistance = 200.f;
	_vec3		m_vUltHitPos{};

	CSpaceLaser*			m_pSpaceLaser[LaserLookEnd];

	

	_float					m_fTimeDelta = 0.f;
	_int					m_iCount = 0;
	_float					m_fAmount = 0.f;

	_uint					m_iRayCol = 0;

public:
	static CWindLaser*				Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg = nullptr);
	virtual void					Free(void)		override;

};

#define __WINDLASER_H__
#endif

