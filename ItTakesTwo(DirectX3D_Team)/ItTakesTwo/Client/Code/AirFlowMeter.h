#ifndef AirFlowMeter_h__
#define AirFlowMeter_h__

#include "GameObject.h"
#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CTransform;
class CStaticMesh;
class CShader;

END

class CAirFlowMeter : public Engine::CGameObject
{
private:
	explicit CAirFlowMeter(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CAirFlowMeter(void);

public:
	virtual HRESULT			Ready_Object(void* pArg = nullptr) override;
	virtual _int			Update_Object(const _float& TimeDelta) override;
	virtual _int			LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void			Render_Object(const _int& iIndex) override;

private:
	HRESULT					Add_Component(void* pArg = nullptr);
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT& pEffect, const _int& iIndex);

private:
	Engine::CStaticMesh*	m_pMeshCom = nullptr;
	Engine::CTransform*		m_pTransformCom = nullptr;
	Engine::CShader*		m_pShaderCom = nullptr;

public:
	static CAirFlowMeter*	Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg = nullptr);
	OBJINFO					m_tObjInfo;
	_float					m_fRotSpeed;

private:
	_bool					m_bPlaySound = false;

private:
	virtual void Free(void) override;
};

#endif // __FAN_H__
