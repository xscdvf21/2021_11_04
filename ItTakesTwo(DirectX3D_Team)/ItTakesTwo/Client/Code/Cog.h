#ifndef __Cog_H__

#include "GameObject.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CStaticMesh;
class CShader;

END

class CCog : public CGameObject
{
private:
	explicit CCog(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCog();



public:
	virtual HRESULT					Ready_Object(void* pArg = nullptr);
	virtual _int					Update_Object(const _float& fTimeDelta) override;
	virtual _int					LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void					Render_Object(const _int& iIndex = 0) override;
private:
	void							Move(const _float & fTimeDelta);
public:
	virtual void*					On_Active(void* pCaller) override;
public:
	void							Set_Dir(const _vec3& vDir) { m_vDir = vDir; }
	void							Set_Dist(const _float& fDist) { m_fDist = fDist; }
	void							Set_Duration(const _float& fSecond) { m_fTotalTime = fSecond; }
public:

private:
	HRESULT							Add_Component(void* pArg = nullptr);
	HRESULT							SetUp_ConstantTable(LPD3DXEFFECT& pEffect, const _int& iIndex);


private:
	Engine::CStaticMesh*			m_pMeshCom = nullptr;
	Engine::CShader*				m_pShaderCom = nullptr;
private:
	_bool							m_bMove = false;

	_float							m_fMoveTime = 0.f;
	_float							m_fDist = 5.f;

	_vec3							m_vDir = { 0.f,0.f,0.f };
	_vec3							m_vStartPos = { 0.f,0.f,0.f };
	_vec3							m_vGoal;
	_float							m_fTotalTime = 0.5f;
	_float							m_fRotSpeed = 1.f;
	_float							m_fRotValue = 0.f;
public:
	static CCog*			Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg = nullptr);
	virtual void					Free(void)		override;

};

#define __Cog_H__
#endif

