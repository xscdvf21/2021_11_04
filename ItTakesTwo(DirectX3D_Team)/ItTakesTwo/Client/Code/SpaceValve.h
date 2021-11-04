#ifndef __SpaceValve_H__

#include "GameObject.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CStaticMesh;

class CShader;

END

class CSpaceValve : public CGameObject
{
private:
	explicit CSpaceValve(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSpaceValve();


public:
	virtual HRESULT			Ready_Object(_vec3 vTargetPos, _vec3 vNormal, wstring ObjTag, void* pArg = nullptr);
	virtual _int			Update_Object(const _float& fTimeDelta) override;
	virtual _int			LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void			Render_Object(const _int& iIndex = 0) override;


	void					Move(const _float & fTimeDelta);
	void					Rotate(const _float& fValue);
	void					Release_Valve();
	CLayer*					m_pLayer = nullptr;
	_bool					m_bResetAngle = false;
	_bool					m_bResetStart = false;
	_float					m_fTargetAngle = 0.0f;
	_float					m_fResetTime = 0.3f;
	_float					m_fTime = 0.0f;
public:
	void					Set_AddAngle(_float fRad);

private:
	HRESULT					Add_Component(void* pArg = nullptr);
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT& pEffect, const _int& iIndex);
	void					ResetAngle(const _float& fTimeDelta);

public:
	_vec3					Get_TargetPos() { return m_vTargetPos; }
	_vec3					Get_Normal() { return m_vNormal; }

private:
	_float m_fRotValue = 0.f;
	_float m_fTotalTime = 1.f;
	_vec3 m_vTargetPos{};
	_vec3 m_vNormal{};

private:
	Engine::CStaticMesh*			m_pMeshCom = nullptr;
	Engine::CShader*				m_pShaderCom = nullptr;
private:
public:
	static CSpaceValve*				Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vTargetPos, _vec3 vNormal, wstring ObjTag, void* pArg = nullptr);
	virtual void					Free(void)		override;

};

#define __SpaceValve_H__
#endif

