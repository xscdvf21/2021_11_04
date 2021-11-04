#ifndef __IRU_H__

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


class CIru : public Engine::CGameObject
{
private:
	explicit CIru(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CIru(void);

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void Render_Object(void) override;


private:
	HRESULT			Add_Component(void);

private:
	Engine::CDynamicMesh*	m_pMeshCom = nullptr;
	Engine::CNaviMesh*		m_pNaviMeshCom = nullptr;
	Engine::CTransform*		m_pTransformCom = nullptr;
	Engine::CRenderer*		m_pRendererCom = nullptr;
	Engine::CCalculator*	m_pCalculatorCom = nullptr;
	Engine::CCollider*		m_pColliderCom = nullptr;

	_float					m_TurnSpeed = D3DXToRadian(2);

	_float					m_fBuffTime = 0.f; //일정 시간이 쌓이면 버프주기위해.
	_float					m_bBuffing = false;
public:
	static CIru*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free(void) override;
};



#define __IRU_H__
#endif // !__IRU_H__


