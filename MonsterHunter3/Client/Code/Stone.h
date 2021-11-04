#ifndef Stone_h__
#define Stone_h__

#include "GameObject.h"
#include "Base.h"
#include "define.h"

BEGIN(Engine)

class CStaticMesh;
class CTransform;
class CRenderer;
class CCalculator;
class CCollider;
class COptimization;

END

class CStone : public Engine::CGameObject
{
private:
	explicit CStone(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CStone(void);

public:
	 HRESULT Ready_Object(const _uint& iFlag) ;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void Render_Object(void) override;

private:
	HRESULT			Add_Component(void);
	void			SetUp_OnTerrain(void);
	_bool			Collision_ToObject(const _tchar* pLayerTag, const _tchar* pObjTag);

private:
	Engine::CStaticMesh*	m_pMeshCom = nullptr;
	Engine::CTransform*		m_pTransformCom = nullptr;
	Engine::CRenderer*		m_pRendererCom = nullptr;
	Engine::CCalculator*	m_pCalculatorCom = nullptr;
	Engine::CCollider*		m_pColliderCom = nullptr;
	Engine::COptimization*	m_pOptimizationCom = nullptr;

	_vec3					m_vDir;
	_bool					m_bColl;
	_uint					m_iFlag = 0;
	_bool					m_bDraw = false;

public:
	static CStone*		Create(LPDIRECT3DDEVICE9 pGraphicDev, const _uint& iFlag);
private:
	virtual void Free(void) override;

};

#endif // Stone_h__
