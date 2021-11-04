#ifndef Fodder_h__
#define Fodder_h__

#include "GameObject.h"
#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CDynamicMesh;
class CTransform;
class CCalculator;
class CCollider;
class CShader;

END

enum Fodder_AnimID {
	Fodder_Attack,                 //0	frame:[23]
	Fodder_Dead,                   //1	frame:[11]
	Fodder_LS_IntroCutscene_ChumBucket_Capture_Plankton,//2	frame:[16]
	Fodder_LS_IntroCutscene_ChumBucket_Walk_01,//3	frame:[16]
	Fodder_LS_IntroCutscene_ChumBucket_Walk_02,//4	frame:[16]
	Fodder_Notice,                 //5	frame:[16]
	Fodder_Victory,                //6	frame:[46]
	Fodder_Walk,                   //7	frame:[16]
	Fodder_End
};

class CFodder : public Engine::CGameObject
{
private:
	explicit CFodder(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CFodder();

public:
	virtual HRESULT Ready_Object(void* pArg = nullptr) override;
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual _int	LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void	Render_Object(const _int& iIndex = 0) override;

private:
	HRESULT			Add_Component();
	void			Flat_Moving(_float fTimeDelta);
	void			Check_Hit();
	_bool			Check_Collision(const _tchar* pLayerTag, const _tchar* pObjTag);
	_bool			Check_PatickCollision();
	HRESULT			SetUp_ConstantTable(LPD3DXEFFECT& pEffect);

private:
	HRESULT			Watch_Player(_float fTimeDelta);
	HRESULT			Attack(_float fTimeDelta);
	HRESULT			KnockBack(_float fTimeDelta);
	HRESULT			Gravity(_float fTimeDelta);
	HRESULT			Debris(_float fTimeDelta);
	HRESULT			Flower();

public: // Get
	CDynamicMesh*	Get_Mesh() { return m_pMeshCom; }
	CTransform*		Get_Transform() { return m_pTransformCom; }
	CCollider*		Get_BodyCollider() { return m_pBodyColliderCom; }
	CCollider*		Get_WeaponCollider() { return m_pWeaponColliderCom; }

public: // Set
	void			Set_Animation(Fodder_AnimID FodderAnim) { m_eFodder_Anim = FodderAnim; }

private: // Normal
	_vec3					m_vDir;
	_bool					m_bAttack;
	_float					m_fAttackTime;
	_float					m_fBodyDis;
	const _matrix*			m_pParentBoneMatrix = nullptr;
	const _matrix*			m_pParentBoneMatrix2 = nullptr;
	const _matrix*			m_pParentWorldMatrix = nullptr;
	_float					m_fWatchTime;
	_float					m_fAnimationTime;
	_bool					m_bWatch;
	_vec3					m_vParticle;

private: // KnockBack
	_bool					m_bKnockBack;
	_bool					m_bKnockBackStop;
	_float					m_fKnockBackTime;
	_float					m_fKnockBackPower;
	_float					m_fKnockBackHeight;

private:
	_bool					m_bSound;
	_bool					m_bStart;
	_bool					m_bGravity;
	_float					m_fGravityTime;
	_uint					m_iNaviGroupIndex;
	_tchar*					m_tagLayer;
	_tchar*					m_tagFlower;
	_tchar*					m_tagDebris;

private:
	Fodder_AnimID			m_eFodder_Anim;

private:
	Engine::CDynamicMesh*	m_pMeshCom = nullptr;
	Engine::CTransform*		m_pTransformCom = nullptr;
	Engine::CCalculator*	m_pCalculatorCom = nullptr;
	Engine::CCollider*		m_pWeaponColliderCom = nullptr;
	Engine::CCollider*		m_pBodyColliderCom = nullptr;
	Engine::CShader*		m_pShaderCom = nullptr;

public:
	static CFodder*		Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg = nullptr);

private:
	virtual void Free(void) override;

};

#endif // Fodder_h__
