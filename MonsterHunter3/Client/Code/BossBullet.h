#ifndef __BOSSBULLET_H__

#include "GameObject.h"
#include "define.h"
#include "Base.h"
#include "Player.h"


BEGIN(Engine)

class CDynamicMesh;
class CTransform;
class CRenderer;
class CCollider;
class CSphere;
class CCalculator;


END

class CBossBullet : public CGameObject
{
private:
	explicit CBossBullet(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBossBullet(void);

public:
	virtual HRESULT Ready_Object(void) override;
	virtual HRESULT Ready_Object(_vec3 vPos);
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void Render_Object(void) override;

private :
	HRESULT			Add_Component(void);
	void			SetUp_Sphere(void);


	_bool			Collision_ToObject(const _tchar* pLayerTag, const _tchar* pObjTag, const _float& fTimeDelta);


private :
	Engine::CDynamicMesh*	m_pMeshCom = nullptr;
	Engine::CTransform*		m_pTransformCom = nullptr;
	Engine::CRenderer*		m_pRendererCom = nullptr;
	Engine::CCollider*		m_pColliderCom = nullptr;
	Engine::CSphere*		m_pSphere = nullptr;
	Engine::CCalculator*	m_pCalculatorCom = nullptr;


	const _matrix*			m_pParentBoneMatrix_Sphere;
	const _matrix*			m_pParentWorldMatrix_Sphere = nullptr;
	_matrix					m_pFinalMatrix_Sphere;


	_vec3					m_vDir = { 0.f, 0.f, 0.f }; //날아갈 방향벡터.

	_bool					m_bCollOne = false; //콜리전 한번만 충돌하기위해. 시간값 넣을것임.


	_float fDelta = 0.f;


public :
	static CBossBullet* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos = { 0.f, 0.f,0.f });

private:
	virtual void		Free(void) override;
};



#define __BOSSBULLET_H__
#endif // !__BOSSBULLET_H__



