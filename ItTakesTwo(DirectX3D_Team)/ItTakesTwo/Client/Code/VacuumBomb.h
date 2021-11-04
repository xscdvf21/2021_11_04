#ifndef __VACUUMBOMB_H__

#include "Player.h"
#include "ControllerCallback.h"


BEGIN(Engine)

class CStaticMesh;
class CCalculator;
class CCollider;
class CShader;

END


class CVacuumBomb : public CPlayer, public CControllerCallback
{
private:
	explicit CVacuumBomb(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CVacuumBomb();


	void Set_Shoot(_vec3 vTargetPos1, _float fPower, _float fDuration);
	void Shoot(const _float& fTimeDelta);
public:
	SPHERE_INFO		Get_Collider_Info() { return m_tColInfo; };

public:
	virtual HRESULT Ready_Object(_vec3 vPos, _vec3 vTargetPos);
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual _int	LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void	Render_Object(const _int& iIndex = 0) override;

	void			Set_Slide(_bool bHit, const _float & fPower, const _float & fShootDuration, vector<vector<_vec3>*> vecActor, const _float& fSpeed);
	void			Slide_State(const _float& fTimeDelta);

	virtual PxControllerBehaviorFlags getBehaviorFlags(const PxShape& shape, const PxActor& actor) override;


	virtual PxControllerBehaviorFlags getBehaviorFlags(const PxController& controller) override;


	virtual PxControllerBehaviorFlags getBehaviorFlags(const PxObstacle& obstacle) override;


	virtual void onShapeHit(const PxControllerShapeHit& hit) override;


	virtual void onControllerHit(const PxControllersHit& hit) override;


	virtual void onObstacleHit(const PxControllerObstacleHit& hit) override;


	virtual void Set_ActorSetting(PxRigidDynamic* pActor, PxShape* pBody) override;


	virtual bool filter(const PxController& a, const PxController& b) override;


	virtual PxQueryHitType::Enum preFilter(const PxFilterData& filterData, const PxShape* shape, const PxRigidActor* actor, PxHitFlags& queryFlags) override;


	virtual PxQueryHitType::Enum postFilter(const PxFilterData& filterData, const PxQueryHit& hit) override;

	void			Set_Move(const _vec3& vStartPos, const _vec3& vTargetPos);
private:
	HRESULT			Add_Component();
	HRESULT			SetUp_ConstantTable(LPD3DXEFFECT& pEffect, const _int& iIndex);

	void			Move(const _float& fTimeDelta, const _vec3& TargetPos);
private:
	Engine::CStaticMesh*		m_pMeshCom = nullptr;
	Engine::CCalculator*		m_pCalculatorCom = nullptr;
	Engine::CCollider*			m_pColliderCom = nullptr;
	Engine::CShader*			m_pShaderCom = nullptr;

private:
	_vec3						m_vDir = { 0.f, 0.f, 0.f }; //나아갈 방향벡터.
	_vec3						m_vTargetPos = { 0.f, 0.f, 0.f }; //인자로 받은 타켓 포스.
	_vec3						m_vPos = { 0.f, 0.f, 0.f }; //생성된 자신의 위치
	_float						m_fLifeTime = 0.f;

private:
	_vec3			m_vTramDis;
	_vec3			m_vTramDst;
	_vec3			m_vTramDir;
	_bool			m_bTram;
	_bool			m_bTramStart;
	_float			m_fTramPower;
	_float			m_fTramTime;
	_float			m_fTramHeight;
	_float			m_fDuration;
	_float			m_fJumpTestTime = 0.f;
	_vec3			m_vSetPos; //스타트할때의 목표점.

	vector<vector<_vec3> *> m_vecSlidePos;
	_int m_iSlideIndex = 0;
	_int m_iPathCount = 0;
	_bool m_bSlideStart = false;
	_bool m_bSlideEnd = false;
	_bool m_bOnGround = false;
	_bool m_bPhys = false;
	_bool m_bMoveEnd = false;
	_bool m_bActive = false;
	_float	m_fEffectTime = 0.f;

protected:
	SPHERE_INFO		m_tColInfo;

private :
	_bool			m_bCol;
	_float			m_fColSize;

public:
	static CVacuumBomb*			Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, _vec3 vTargetPos);
	virtual void Free(void)		override;
private:
	bool m_bShoot =false;
	_float m_fShootTime =0.f;
	_float m_fShootDuration = 0.f;
	_vec3 m_vShootDir = {0.f,0.f,0.f};
	_bool m_bSlide = false;
	_float m_fShootPower = 80.f;
	_float m_fSlideSpeed = 1.f;
	size_t m_iInterval = 0;
};

#define __VACUUMBOMB_H__
#endif

