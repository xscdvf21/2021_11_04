#ifndef __LOGOIRU_H__

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



class CLogoIru : public Engine::CGameObject
{
private:
	explicit CLogoIru(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLogoIru(void);

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void Render_Object(void) override;


	void				Set_Loading(_bool _bLoading) { m_bLoading = _bLoading; }

private:
	HRESULT			Add_Component(void);

private:
	Engine::CDynamicMesh*	m_pMeshCom = nullptr;
	Engine::CNaviMesh*		m_pNaviMeshCom = nullptr;
	Engine::CTransform*		m_pTransformCom = nullptr;
	Engine::CRenderer*		m_pRendererCom = nullptr;
	Engine::CCalculator*	m_pCalculatorCom = nullptr;
	Engine::CCollider*		m_pColliderCom = nullptr;

	_bool					m_bLoading = false;
	_bool					m_bLoadingEnd = false;

public:
	static CLogoIru*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free(void) override;
};



#define __LOGOIRU_H__
#endif // !__LOGOIRU_H__
