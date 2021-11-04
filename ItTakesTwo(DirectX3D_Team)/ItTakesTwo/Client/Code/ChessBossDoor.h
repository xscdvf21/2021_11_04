#ifndef __CHESSBOSSDOOR_H__

#include "GameObject.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CDynamicMesh;
class CShader;

END


class CChessBossDoor : public CGameObject
{
private:
	explicit CChessBossDoor(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CChessBossDoor();

public:
	virtual HRESULT		Ready_Object(void* pArg = nullptr);
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual _int		LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void		Render_Object(const _int& iIndex = 0) override;

private:
	HRESULT				Add_Component(void* pArg = nullptr);
	HRESULT				SetUp_ConstantTable(LPD3DXEFFECT& pEffect, const _int& iIndex);

	
public :
	void				Set_Move()	{ m_bMove = true; }
private:
	Engine::CDynamicMesh*			m_pMeshCom = nullptr;
	Engine::CShader*				m_pShaderCom = nullptr;

private:
	_bool		m_bMove = false;

	CTrigger*	m_pTrigger = nullptr;

public:
	static CChessBossDoor*				Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg = nullptr);
	virtual void						Free(void)		override;
};

#define __CHESSBOSSDOOR_H__
#endif

