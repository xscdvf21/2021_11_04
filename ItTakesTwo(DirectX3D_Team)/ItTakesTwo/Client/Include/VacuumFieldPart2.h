
#ifndef __VACUUMFIELDPART2_H__

#include "GameObject.h"
#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CStaticMesh;
class CCalculator;
class CCollider;
class CShader;

END

class CVacuumFieldPart2 : public CGameObject
{
private:
	explicit CVacuumFieldPart2(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CVacuumFieldPart2();

public:
	virtual HRESULT Ready_Object(void* pArg);
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual _int	LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void	Render_Object(const _int& iIndex = 0) override;

private:
	HRESULT			Add_Component(void* pArg);
	HRESULT			SetUp_ConstantTable(LPD3DXEFFECT& pEffect, const _int& iIndex);

	void			MoveUp(const _float& fTimeDelta);
	void			MoveDown(const _float& fTimeDelta);

public:
	_bool			Get_MoveUpCheck() { return m_bMoveUp; }
	_bool			Get_MoveDownCheck() { return m_bMoveDown; }

	void			Set_MoveUpCheck(_bool bMove) { m_bMoveUp = bMove; }
	void			Set_MoveDownCheck(_bool bMove) { m_bMoveDown = bMove; }

private:
	Engine::CStaticMesh*		m_pMeshCom = nullptr;
	Engine::CCalculator*		m_pCalculatorCom = nullptr;
	Engine::CCollider*			m_pColliderCom = nullptr;
	Engine::CShader*			m_pShaderCom = nullptr;

	_bool						m_bMoveUp = false;
	_bool						m_bMoveDown = false;


public:
	static CVacuumFieldPart2*	Create(LPDIRECT3DDEVICE9 pGraphicDev,void* pArg);
	virtual void Free(void)		override;
};
#define __VACUUMFIELDPART2_H__
#endif

