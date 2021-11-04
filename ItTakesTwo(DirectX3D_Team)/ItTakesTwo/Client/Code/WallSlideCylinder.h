#ifndef WallSlideCylinder_h__
#define WallSlideCylinder_h__

#include "GameObject.h"
#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CTransform;
class CStaticMesh;
class CShader;

END

class CWallSlideCylinder : public Engine::CGameObject
{
private:
	explicit CWallSlideCylinder(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CWallSlideCylinder(void);

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

	_float					m_fTime = 0.0f;
	_float					m_fPreTime = 0.0f;
	_float					m_fAddAngle = 0.0f;
	_float					m_fStopTime = 0.0f;
	_bool					m_bStopRot = false;
	_bool					m_bChange = false;

private:
	_bool					m_bPlaySound = false;
	class CTransform*		m_pCodyTransform = nullptr;
	class CTransform*		m_pMayTransform = nullptr;


public:
	static CWallSlideCylinder*	Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg = nullptr);
	OBJINFO					m_tObjInfo;

private:
	virtual void Free(void) override;
};

#endif // __FAN_H__
