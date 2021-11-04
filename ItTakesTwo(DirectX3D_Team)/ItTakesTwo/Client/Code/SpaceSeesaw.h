#ifndef __SpaceSeesaw_H__

#include "GameObject.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CStaticMesh;

class CShader;

END

class CSpaceSeesaw : public CGameObject
{
private:
	explicit CSpaceSeesaw(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSpaceSeesaw();

	_bool m_bResetStart = false;
	_float m_fTargetAngle = 0.f;
	_float m_fTime = 0.f;
	_float m_fResetTime = 0.f;
	_float m_bResetAngle = 0.f;
	_bool m_bMove = false;
	_float m_fSpeed = 2.f;
public:
	virtual HRESULT		Ready_Object(void* pArg = nullptr);
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual _int		LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void		Render_Object(const _int& iIndex = 0) override;




	virtual void* On_Active(void* pCaller) override;

public:
	void				Push();
private:
	HRESULT				Add_Component(void* pArg = nullptr);
	HRESULT				SetUp_ConstantTable(LPD3DXEFFECT& pEffect, const _int& iIndex);

private:
	void				Move(const _float& fTimeDelta);

	void ResetAngle(const _float & fTimeDelta);
private:
	Engine::CStaticMesh*			m_pMeshCom = nullptr;
	Engine::CStaticMesh*			m_pMeshBaseCom = nullptr;
	Engine::CTransform*				m_pTransformBaseCom = nullptr;
	Engine::CShader*				m_pShaderCom = nullptr;
private:
	_vec3  m_vAxis;
	_float m_fPushWeight = 0.f;
public:
	static CSpaceSeesaw*				Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg = nullptr);
	virtual void					Free(void)		override;

private:
	_float m_fTargetValue = 0.f;
};

#define __SpaceSeesaw_H__
#endif

