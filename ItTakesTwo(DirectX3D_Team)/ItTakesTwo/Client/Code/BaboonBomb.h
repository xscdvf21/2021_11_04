#ifndef __BABOONBOMB_H__

#include "Enermy.h"


BEGIN(Engine)

class CStaticMesh;
class CCalculator;
class CCollider;
class CShader;
class CTexture;

END

class CBaboonBomb : public CEnermy
{
	enum SoundID
	{
		Sound_Effect,
		Sound_End,
	};

private:
	explicit CBaboonBomb(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBaboonBomb();

public:
	virtual HRESULT Ready_Object(void* pArg = nullptr, CGameObject* pPlayer = nullptr, _int iIndex = 0);
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual _int	LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void	Render_Object(const _int& iIndex = 0) override;

private:
	HRESULT			Add_Component(void* pArg);
	HRESULT			SetUp_ConstantTable(LPD3DXEFFECT& pEffect, const _int& iIndex);

private:
	void			Move(const _float& fTimeDelta);
	void			PhyscisCreate();
	void			Effect();
	void			Effect_Thrust(const _float & fTimeDelta);
private:
	Engine::CStaticMesh*		m_pMeshCom = nullptr;
	Engine::CCalculator*		m_pCalculatorCom = nullptr;
	Engine::CCollider*			m_pColliderCom = nullptr;
	Engine::CShader*			m_pShaderCom = nullptr;
	Engine::CTexture*			m_pTextureCom = nullptr;


	CGameObject*				m_pTarget = nullptr;

private:
	_vec3			m_vTramDis;
	_vec3			m_vTramDst;
	_vec3			m_vTramDir;
	_bool			m_bTram;
	_bool			m_bTramStart;
	_float			m_fTramPower;
	_float			m_fTramTime;
	_float			m_fTramHeight;
	_float			m_fDuration;
	_float			m_fJumpTestTime = 0.f;
	_vec3			m_vSetPos; //스타트할때의 목표점.
	_bool			m_bAnglecheck;
	
	_int			m_iCount = 0;

public:
	static CBaboonBomb*				Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg = nullptr, CGameObject* pPlayer = nullptr, _int iIndex = 0);
	virtual void					Free(void)			override;
};



#define __BABOONBOMB_H__
#endif

