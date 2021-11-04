#ifndef __SpaceRing_H__

#include "GameObject.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CStaticMesh;

class CShader;

END

class CSpaceRing : public CGameObject
{
public:
	enum Rot {RotX, RotY, RotZ};
private:
	explicit CSpaceRing(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSpaceRing();

public:
	virtual HRESULT		Ready_Object(void* pArg = nullptr);
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual _int		LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void		Render_Object(const _int& iIndex = 0) override;
public:
	void				Set_Rot(Rot eRot) { m_eRot = eRot; }
	void				Set_RotSpeed(_float fSpeed) { m_fRotSpeed = fSpeed; }

private:
	HRESULT				Add_Component(void* pArg = nullptr);
	HRESULT				SetUp_ConstantTable(LPD3DXEFFECT& pEffect, const _int& iIndex);

private:
	Engine::CStaticMesh*			m_pMeshCom = nullptr;
	Engine::CShader*				m_pShaderCom = nullptr;
private:
public:
	static CSpaceRing*				Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg = nullptr);
	virtual void					Free(void)		override;

private:
	Rot m_eRot = RotX;
	_float		m_fRotSpeed = 36.0f;

};

#define __SpaceRing_H__
#endif

