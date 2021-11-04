#ifndef __SpaceSpringBoardSpring_H__

#include "GameObject.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CStaticMesh;

class CShader;

END

class CSpaceSpringBoardSpring : public CGameObject
{
private:
	explicit CSpaceSpringBoardSpring(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSpaceSpringBoardSpring();

public:
	virtual HRESULT		Ready_Object(void* pArg = nullptr);
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual _int		LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void		Render_Object(const _int& iIndex = 0) override;



	void				Set_Weight(const _bool& IsCody, const _int& iWeight);
	virtual void*		On_Active(void* pCaller) override;


	virtual void*		On_Attach(void* pCaller) override;


	virtual _mat		Get_EventPos() override;

public:

private:
	HRESULT				Add_Component(void* pArg = nullptr);
	HRESULT				SetUp_ConstantTable(LPD3DXEFFECT& pEffect, const _int& iIndex);

private:
	void				Move(const _float& fTimeDelta);

private:
	_int  m_iWeight = 0;
	_int m_iCodyWeight = 0;
	_int m_iMayWeight = 0;
	_float m_fTime = 0.f;
	_vec3 m_vPower;
	_vec3 m_vPos;
	Engine::CStaticMesh*			m_pMeshCom = nullptr;
	Engine::CStaticMesh*			m_pMeshBaseCom = nullptr;
	Engine::CStaticMesh*			m_pMeshBoardCom = nullptr;
	Engine::CShader*				m_pShaderCom = nullptr;
	Engine::CTransform*				m_pTransformSpringCom = nullptr;
	Engine::CTransform*				m_pTransformBoardCom = nullptr;
	CTrigger*						m_pTrigger = nullptr;
private:
public:
	static CSpaceSpringBoardSpring*				Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg = nullptr);
	virtual void					Free(void)		override;

};

#define __SpaceSpringBoardSpring_H__
#endif

