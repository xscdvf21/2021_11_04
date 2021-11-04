#ifndef __LEVER_H__
#define __LEVER_H__

#include "GameObject.h"
#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CPhysXMesh;
class CStaticMesh;
class CShader;

END

class CLever : public Engine::CGameObject
{
public:
	enum DIR { LEFT, RIGHT };

private:
	explicit CLever(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLever(void);

public:
	virtual HRESULT			Ready_Object(void* pArg = nullptr) override;
	virtual _int			Update_Object(const _float& TimeDelta) override;
	virtual _int			LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void			Render_Object(const _int& iIndex) override;

private:
	HRESULT					Add_Component(void* pArg = nullptr);
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT& pEffect, const _int& iIndex);

private:
	Engine::CPhysXMesh*		m_pPhysMesh = nullptr;
	Engine::CStaticMesh*	m_pMeshCom = nullptr;
	Engine::CStaticMesh*	m_pBascMeshCom = nullptr;
	Engine::CTransform*		m_pBaseTransformCom = nullptr;
	Engine::CCalculator*	m_pCalculatorCom = nullptr;
	Engine::CCollider*		m_pColliderCom = nullptr;
	Engine::CShader*		m_pShaderCom = nullptr;

public:
	void					Set_Dir(DIR eDir);// { m_eDir = eDir; }
	void					Move(const _float& TimeDelta);

public:
	DIR						Get_Dir() { return m_eDir; }
	_vec3					Get_Point() { return m_vPoint; }
	_vec3					Get_LookingAtDir() { return m_vLookingAtDir; }


public:
	void					Add_FanList(wstring wstrFanName);

public:
	static CLever*			Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg = nullptr);
	OBJINFO					m_tObjInfo;
	CTrigger*				m_pTrigger = nullptr;
	DIR						m_eDir;
	vector<wstring>			m_vecFanList;
	_bool					m_bMove = false;
	_float					m_fAngle = 120.0f;
	_matrix					m_matCombinedMatrix;
	_float3					m_vPoint;
	_vec3					m_vLookingAtDir;
	

private:
	virtual void Free(void) override;
};



#endif // TestWall_h__
