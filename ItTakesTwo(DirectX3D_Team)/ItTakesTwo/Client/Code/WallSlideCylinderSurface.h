#ifndef WallSlideCylinderSurface_h__
#define WallSlideCylinderSurface_h__

#include "GameObject.h"
#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CTransform;
class CStaticMesh;
class CShader;

END

class CWallSlideCylinderSurface : public Engine::CGameObject
{
private:
	explicit CWallSlideCylinderSurface(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CWallSlideCylinderSurface(void);

public:
	virtual HRESULT						Ready_Object(void* pArg = nullptr) override;
	virtual _int						Update_Object(const _float& TimeDelta) override;
	virtual _int						LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void						Render_Object(const _int& iIndex) override;

private:
	HRESULT								Add_Component(void* pArg = nullptr);
	HRESULT								SetUp_ConstantTable(LPD3DXEFFECT& pEffect, const _int& iIndex);

public:
	virtual void*						On_Attach(void* pCaller) override;

private:
	Engine::CStaticMesh*				m_pMeshCom = nullptr;
	Engine::CTransform*					m_pTransformCom = nullptr;
	Engine::CShader*					m_pShaderCom = nullptr;

public:
	static CWallSlideCylinderSurface*	Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg = nullptr);
	OBJINFO								m_tObjInfo;


private:
	virtual void Free(void) override;

	_float					m_fTime = 0.0f;
	_float					m_fPreTime = 0.0f;
	_float					m_fAddAngle = 0.0f;
	_float					m_fStopTime = 0.0f;
	_bool					m_bStopRot = false;
	_float					m_fMoveAngle = 0.0f;
	_vec3					m_vResult{};
	_bool					m_bChange = false;
	_float					m_fPreAngle = 0.0f;

};

#endif // __FAN_H__
