#ifndef __MONSTERJAGI_H__


#include "GameObject.h"
#include "Base.h"
#include "define.h"

BEGIN(Engine)

class CDynamicMesh;
class CTransform;
class CRenderer;
class CCalculator;
class CCollider;
class CNaviMesh;

END

class CMonsterJagi : public Engine::CGameObject
{
private:
	explicit CMonsterJagi(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMonsterJagi(void);

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void Render_Object(void) override;

private:
	HRESULT			Add_Component(void);
	void			SetUp_OnTerrain(void);
	_vec3			PickUp_OnTerrain(void);

private :
	Engine::CDynamicMesh*	m_pMeshCom = nullptr;
	Engine::CNaviMesh*		m_pNaviMeshCom = nullptr;
	Engine::CTransform*		m_pTransformCom = nullptr;
	Engine::CRenderer*		m_pRendererCom = nullptr;
	Engine::CCalculator*	m_pCalculatorCom = nullptr;
	Engine::CCollider*		m_pColliderCom = nullptr;

public:
	static CMonsterJagi*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free(void) override;


};


#define __MONSTERJAGI_H__
#endif // !__MONSTERJAGI_H__


