#ifndef HangDoor_h__

#include "GameObject.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CStaticMesh;
class CShader;

END

class CHangDoor : public CGameObject
{
	enum SoundID
	{
		Sound_Open,
		Sound_Close,
		Sound_End
	};
private:
	explicit CHangDoor(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CHangDoor();

public:
	virtual HRESULT		Ready_Object(void* pArg = nullptr);
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual _int		LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void		Render_Object(const _int& iIndex = 0) override;

public:
	_vec3				Get_GoalPos() { return m_vGoalPos; }
	_vec3				Get_Normal() { return m_vNormal; }

private:
	HRESULT				Add_Component(void* pArg = nullptr);
	HRESULT				SetUp_ConstantTable(LPD3DXEFFECT& pEffect, const _int& iIndex);

public:
	void				Set_Fall() { m_bFall = true; }

private:
	void				Move(const _float& fTimeDelta);

private:
	Engine::CStaticMesh*			m_pMeshCom = nullptr;
	Engine::CShader*				m_pShaderCom = nullptr;

private:
	CTrigger*						m_pTrigger = nullptr;
	_vec3							m_vGoalPos = _vec3(31.0f, 11.757836f, 0.0f);
	_vec3							m_vNormal = _vec3(0.0f, 0.0f, -1.0f);
	_vec3							m_vOrgPos = {};
	_bool							m_bFall = false;
	CParticle*						m_pParticle = nullptr;


	_bool							m_bSound[Sound_End]{};
public:
	static CHangDoor*				Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg = nullptr);
	virtual void					Free(void)		override;


};

#define __WarpGate_H__
#endif

