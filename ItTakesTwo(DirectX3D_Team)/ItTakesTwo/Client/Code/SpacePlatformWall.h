#ifndef __SpacePlatformWall_H__

#include "GameObject.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CStaticMesh;

class CShader;

END

class CSpacePlatformWall : public CGameObject
{
private:
	explicit CSpacePlatformWall(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSpacePlatformWall();

public:
	virtual HRESULT		Ready_Object(void* pArg = nullptr);
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual _int		LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void		Render_Object(const _int& iIndex = 0) override;



public:
	void				Reset_State();
	void				Set_Move(const _vec3& vPos) { m_bMove = true; m_vGoalPos = m_vStartPos+vPos; }
private:
	HRESULT				Add_Component(void* pArg = nullptr);
	HRESULT				SetUp_ConstantTable(LPD3DXEFFECT& pEffect, const _int& iIndex);

private:

private:
	Engine::CStaticMesh*			m_pMeshCom = nullptr;
	Engine::CShader*				m_pShaderCom = nullptr;
private:
public:
	static CSpacePlatformWall*				Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg = nullptr);
	virtual void					Free(void)		override;

private:
	void Move(const _float & fTimeDelta);
	_bool m_bMove = false;
	_float m_fTime = 0.f;
	_vec3 m_vGoalPos = {0.f,0.f,0.f};
	_vec3 m_vStartPos = { 0.f,0.f,0.f };
};

#define __SpacePlatformWall_H__
#endif

