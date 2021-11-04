#ifndef __CENTERHATCH_H__

#include "GameObject.h"
#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CStaticMesh;
class CCalculator;
class CCollider;
class CShader;

END

class CCenterHatch : public CGameObject
{
private:
	explicit CCenterHatch(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCenterHatch();

public:
	virtual HRESULT Ready_Object(_bool _bDir,void* pArg = nullptr);
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual _int	LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void	Render_Object(const _int& iIndex = 0) override;

public :
	_bool			Get_HatchOpen() { return m_bHatchOpen; }

private:
	HRESULT			Add_Component(void* pArg = nullptr);
	HRESULT			SetUp_ConstantTable(LPD3DXEFFECT& pEffect, const _int& iIndex);
private:
	void			Move(const _float& fTimeDelta);
private:
	Engine::CStaticMesh*			m_pMeshCom = nullptr;
	Engine::CCalculator*			m_pCalculatorCom = nullptr;
	Engine::CCollider*				m_pColliderCom = nullptr;
	Engine::CShader*				m_pShaderCom = nullptr;

private :
	_float							m_fAngle;
	_float							m_fOpenSpeed;

	_bool							m_bDir; //왼쪽으로 돌릴지 오른쪽으로 돌릴지.
	_bool							m_bMove;
	_bool							m_bSoundStart = false;
	_bool							m_bHatchOpen;

	_uint							m_iHatchOpen;
public:
	static CCenterHatch*			Create(LPDIRECT3DDEVICE9 pGraphicDev, _bool _bDir, void* pArg = nullptr);
	virtual void					Free(void)		override;
};

#define __CENTERHATCH_H__
#endif

