#ifndef __SpaceWall_H__

#include "GameObject.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CStaticMesh;
class CShader;

END

class CSpaceWall : public CGameObject
{
public:
	enum SPACEWALLPLAYER { SPACEWALL_MAY, SPACEWALL_CODY, };
private:
	explicit CSpaceWall(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSpaceWall();

public:
	HRESULT				Ready_Object(_vec3 vCodyPos, _vec3 vMayPos, void* pArg = nullptr);
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual _int		LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void		Render_Object(const _int& iIndex = 0) override;

public:
	void					Move(_float fTimeDelta, SPACEWALLPLAYER ePlayer);
	_vec3					Get_Target1() { return m_vTargetPos1; }
	_vec3					Get_Target2() { return m_vTargetPos2; }
	_vec3					Get_Dir() { return m_vDir; }
	_bool					Get_Move() { return m_bMove; }
public:

private:
	HRESULT					Add_Component(void* pArg = nullptr);
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT& pEffect, const _int& iIndex);

private:
	Engine::CStaticMesh*			m_pMeshCom = nullptr;
	Engine::CShader*				m_pShaderCom = nullptr;
public:
	static CSpaceWall*				Create(LPDIRECT3DDEVICE9 pGraphicDev,_vec3 vCodyPos, _vec3 vMayPos, void* pArg = nullptr);
	virtual void					Free(void)		override;

private:
	CTrigger*				m_pTrigger1 = nullptr;
	CTrigger*				m_pTrigger2 = nullptr;

	_vec3					m_vTargetPos1{};
	_vec3					m_vTargetPos2{};
	_vec3					m_vTriggerPos1{};
	_vec3					m_vTriggerPos2{};

	OBJINFO					m_tObjInfo;
	_float					m_fRotSpeed;

	_bool					m_bMoveCody = false;
	_bool					m_bMoveMay = false;
	_bool					m_bMove = false;

	_vec3					m_vDir{};

};

#define __SpaceWall_H__
#endif

