#ifndef __SHEILDER_H__

#include "Enermy.h"

BEGIN(Engine)

class CDynamicMesh;
class CCalculator;
class CCollider;
class CShader;

END


class CSheilder : public CEnermy
{

	enum animID {
		Attack,                                   //0	frame:[31]
		Dead,                                     //1	frame:[21]
		Damage,                                   //2	frame:[21]
		Idle,                                     //3	frame:[143]
		Telegraph_Start,                          //4	frame:[31]
		Telegraph_End,                            //5	frame:[24]
		Walk,                                     //6	frame:[21]
		End
	};

public:
	explicit CSheilder(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSheilder();

public:
	virtual HRESULT Ready_Object(void* pArg = nullptr, void* pOBJINFO = nullptr);
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual _int	LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void	Render_Object(const _int& iIndex = 0) override;

public:
	void			Set_Move(_bool bMove) { m_bMove = bMove; }
private:
	void			Hit_State(const _float& fTimeDelta);
public:
	void			Set_Damage(_int iDamage) override;
private:
	HRESULT			Add_Component(void* pArg = nullptr);
	HRESULT			SetUp_ConstantTable(LPD3DXEFFECT& pEffect, const _int& iIndex);

	void			Move(const _float& fTimeDelta);

private:
	Engine::CDynamicMesh*		m_pMeshCom = nullptr;
	Engine::CCalculator*		m_pCalculatorCom = nullptr;
	Engine::CCollider*			m_pColliderCom = nullptr;
	Engine::CShader*			m_pShaderCom = nullptr;

	class CSheild*				m_pSheild = nullptr;

	_bool						m_bCodySelect = false;
	_bool						m_bMaySelect = false;

	_float						m_fTurnSpeed = D3DXToRadian(6); //턴 스피드
	_float						m_fAngle;	 //플레이어와의 각도

	CGameObject*				m_pTargetPlayer = nullptr;

	_uint						m_iRandom;
	_uint						m_iAniNum = 0; //현재 재생되고있는 애니메이션 번호 저장


private :
	_float						m_fTimeDelta;
	_bool						m_bMove;
public:
	static CSheilder*				Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg = nullptr, void* pOBJINFO = nullptr);
	virtual void Free(void)		override;

};



#define __SHEILDER_H__
#endif

