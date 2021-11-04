#ifndef __CHESSDUODOOR_H__

#include "GameObject.h"
#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CStaticMesh;
class CCalculator;
class CCollider;
class CShader;

END

class CChessDuoDoor : public CGameObject
{
private:
	explicit CChessDuoDoor(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CChessDuoDoor();

public:
	virtual HRESULT		Ready_Object(_bool mDir, void* pArg = nullptr);
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual _int		LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void		Render_Object(const _int& iIndex = 0) override;

public:
	void				Set_Open(_bool _bOpen) { m_bOpen = _bOpen; }

private:
	HRESULT				Add_Component(void* pArg = nullptr);
	HRESULT				SetUp_ConstantTable(LPD3DXEFFECT& pEffect, const _int& iIndex);

private:
	void				Move(const _float& fTimeDelta);

private:
	Engine::CStaticMesh*			m_pMeshCom = nullptr;
	Engine::CCalculator*			m_pCalculatorCom = nullptr;
	Engine::CCollider*				m_pColliderCom = nullptr;
	Engine::CShader*				m_pShaderCom = nullptr;

private:
	_bool							m_bOpen;
	_bool							m_bDir;
	_float							m_fOpenSpeed;
	_float							m_fAngle;
	_float							m_fCurrentAngle;

	_uint							m_iArbalistCount;
public:
	static CChessDuoDoor*			Create(LPDIRECT3DDEVICE9 pGraphicDev, _bool mDir, void* pArg = nullptr);
	virtual void					Free(void)		override;


};

#define __CHESSDUODOOR_H__
#endif

