#ifndef __BOOSTAIL_H__

#include "GameObject.h"
#include "Base.h"
#include "define.h"

BEGIN(Engine)

class CDynamicMesh;
class CTransform;
class CRenderer;
class CCalculator;
class CCollider;

END

class CBosstail : public Engine::CGameObject
{
private:
	explicit CBosstail(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBosstail(void);

public:
	HRESULT				Ready_Object(const _uint& iFlag);
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		Render_Object(void) override;

public :
	void				Set_TailDestory(_bool _bTail) { m_bTailDestory = _bTail; }


private:
	HRESULT			Add_Component(void);
	_bool			Collision_ToObject(const _tchar* pLayerTag, const _tchar* pObjTag);


private:
	Engine::CDynamicMesh*	m_pMeshCom = nullptr;
	Engine::CTransform*		m_pTransformCom = nullptr;
	Engine::CRenderer*		m_pRendererCom = nullptr;
	Engine::CCalculator*	m_pCalculatorCom = nullptr;
	Engine::CCollider*		m_pColliderCom = nullptr;


	const _matrix*			m_pParentBoneMatrix = nullptr;
	const _matrix*			m_pParentWorldMatrix = nullptr;

	_vec3					m_vDir;
	_bool					m_bColl;
	_uint					m_iFlag = 0;


	_bool					m_bTailDestory = false; //꼬리 파괴 
	_bool					m_bTailDestoryOne = false; //꼬리 파괴 후 위치값 한번만 구해서 잡아주기위해.


public:
	static CBosstail*		Create(LPDIRECT3DDEVICE9 pGraphicDev, const _uint& iFlag);
private:
	virtual void			Free(void) override;


};



#define __BOOSTAIL_H__
#endif // !__BOOSTAIL_H__


