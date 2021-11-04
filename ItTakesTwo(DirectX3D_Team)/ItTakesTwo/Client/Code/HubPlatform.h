#ifndef __HUBPLATFORM_H__

#include "GameObject.h"
#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CPhysXMesh;
class CStaticMesh;
class CDynamicMesh;
class CShader;

END

class CHubPlatform : public Engine::CGameObject
{
private:
	explicit CHubPlatform(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CHubPlatform(void);

public:
	virtual HRESULT			Ready_Object(_vec3 vStartPos, _vec3 vEndPos, void* pArg = nullptr);
	virtual _int			Update_Object(const _float& TimeDelta) override;
	virtual _int			LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void			Render_Object(const _int& iIndex) override;

private:
	HRESULT					Add_Component(void* pArg = nullptr);
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT& pEffect, const _int& iIndex);

private :
	void					Move(const _float& fTimeDelta);
	void					ChangePos(const _float& fTimeDelta, const _float& fMoveTime, _vec3 vStartPos, _vec3 vEndPos);
private :
	Engine::CStaticMesh*	m_pMeshPlatformCom = nullptr;
	Engine::CDynamicMesh*   m_pMeshHubCom = nullptr;

	Engine::CTransform*		m_pPlatformTransformCom = nullptr;
	Engine::CTransform*		m_pHubTransformCom = nullptr;

	Engine::CPhysXMesh*		m_pPhysMesh = nullptr;
	Engine::CCalculator*	m_pCalculatorCom = nullptr;
	Engine::CCollider*		m_pColliderCom = nullptr;
	Engine::CShader*		m_pShaderCom = nullptr;


private :
	_float							m_fDir;
	_float							m_fAngle;
	_float							m_fCurrentAngle;
	_float							m_fTurnSpeed;
	_float							m_fPlatformHeight;
private:
	_float							m_fCurTime;
	_float							m_fMoveTime;

	_vec3							m_vStartPos;
	_vec3							m_vEndPos;

	_vec3							m_vMoveStartPos;
	_vec3							m_vMoveEndPos;

	_bool							m_bChangeStart_Pos;


public:
	static CHubPlatform*			Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vStartPos, _vec3 vEndPos, void* pArg = nullptr);

private:
	virtual void Free(void) override;
};

#define __HUBPLATFORM_H__
#endif

