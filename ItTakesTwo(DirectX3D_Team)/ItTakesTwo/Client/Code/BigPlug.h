#ifndef BigPlug_h__
#define BigPlug_h__

#include "GameObject.h"
#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CTransform;
class CStaticMesh;
class CShader;

END

class CBigPlug : public Engine::CGameObject
{
private:
	explicit CBigPlug(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBigPlug(void);

public:
	virtual HRESULT			Ready_Object(void* pArg = nullptr);
	virtual _int			Update_Object(const _float& TimeDelta) override;
	virtual _int			LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void			Render_Object(const _int& iIndex) override;
	
private:
	HRESULT					Add_Component(void* pArg = nullptr);
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT& pEffect, const _int& iIndex);
	void					Move(const _float& fTimeDelta);
public:
	void*					On_Active(void* pCaller)override;

private:
	Engine::CStaticMesh*	m_pMeshCom = nullptr;
	Engine::CTransform*		m_pTransformCom = nullptr;
	Engine::CShader*		m_pShaderCom = nullptr;

public:
	static CBigPlug*		Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg = nullptr);
	OBJINFO					m_tObjInfo;

	_bool					m_bActive = false;
	_bool					m_bBase = false;
	_float					m_fMoveTime = 0.f;
	PxVec3					m_vStartPos;
	_bool					m_bButton_Click = false;
	_bool					m_bEnd = false;

private:
	virtual void Free(void) override;
};

#endif // __FAN_H__
