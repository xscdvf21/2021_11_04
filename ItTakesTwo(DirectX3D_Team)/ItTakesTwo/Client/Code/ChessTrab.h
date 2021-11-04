#ifndef __CHESSTRAB_H__

#include "Enermy.h"

BEGIN(Engine)

class CStaticMesh;
class CCalculator;
class CCollider;
class CShader;

END

class CChessTrab : public CEnermy
{
private:
	explicit CChessTrab(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CChessTrab();

public:
	virtual HRESULT Ready_Object(_vec3 vStartPos, _vec3 vEndPos,void* pArg = nullptr);
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual _int	LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void	Render_Object(const _int& iIndex = 0) override;

private:
	HRESULT				Add_Component(void* pArg = nullptr);
	HRESULT				SetUp_ConstantTable(LPD3DXEFFECT& pEffect, const _int& iIndex);

private:
	void				Move(const _float& fTimeDelta);
	void				ChangePos(const _float& fTimeDelta, const _float& fMoveTime, _vec3 vStartPos, _vec3 vEndPos);
private:
	Engine::CStaticMesh*			m_pMeshCom = nullptr;
	Engine::CCalculator*			m_pCalculatorCom = nullptr;
	Engine::CCollider*				m_pColliderCom = nullptr;
	Engine::CShader*				m_pShaderCom = nullptr;

private:
	_float							m_fMoveSpeed;
	_float							m_fAngle;
	_float							m_fTurnSpeed;

private :
	_vec3							m_vMovePos[4];
	_float							m_fDeltaTime;

private :
	_float							m_fCurTime;
	_float							m_fMoveTime;
	
	_vec3							m_vStartPos;
	_vec3							m_vEndPos;

	_vec3							m_vMoveStartPos;
	_vec3							m_vMoveEndPos;

	_bool							m_bChangeStart_Pos;

	_float							m_fCollisionTime;
public:
	static CChessTrab*				Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vStartPos, _vec3 vEndPos, void* pArg = nullptr);
	virtual void					Free(void)		override;

};

#define __CHESSTRAB_H__
#endif

