#ifndef __GroundBouncePanel_H__

#include "GameObject.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CStaticMesh;
class CShader;

END

class CGroundBouncePanel : public CGameObject
{
private:
	explicit CGroundBouncePanel(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CGroundBouncePanel();



public:
	virtual HRESULT					Ready_Object(void* pArg = nullptr);
	virtual _int					Update_Object(const _float& fTimeDelta) override;
	virtual _int					LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void					Render_Object(const _int& iIndex = 0) override;


private:
	void							Move(const _float & fTimeDelta);
public:
	virtual void* On_Trigger_Active(void* pTrigger, void* pCaller) override;

	virtual void*					On_Active(void* pCaller) override;
public:
	void							Set_Dir(const _vec3& vDir) { m_vDir = vDir; }
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
	_int							m_iCount = 0;
	_float							m_fMoveTime = 0.f;
	_float							m_fTime = 0.f;

	_vec3							m_vDir = { 0.f,0.f,0.f };
	_vec3							m_vStartPos = { 0.f,0.f,0.f };
	_vec3							m_vGoal;
	_float							m_fTotalTime = 0.5f;
public:
	static CGroundBouncePanel*			Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg = nullptr);
	virtual void					Free(void)		override;

};

#define __GroundBouncePanel_H__
#endif

