#pragma once

#include "GameObject.h"

enum MIDDLEBOSS_B_STATE
{
	BOSSB_IDLE,
	BOSSB_MOVE,
	BOSSB_END
};

USING(Engine)
class CMiddleBossB : public CGameObject
{
public:
	explicit CMiddleBossB(LPDIRECT3DDEVICE9 pDevice);
	explicit CMiddleBossB(const CMiddleBossB& other);
	virtual ~CMiddleBossB() = default;

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void * pArg = nullptr) override;
	virtual _uint Update_GameObject(_float fDeltaTime) override;
	virtual _uint LateUpdate_GameObject(_float fDeltaTime) override;
	virtual HRESULT Render_GameObject() override;

private:
	HRESULT Add_Component();

public:
	static CMiddleBossB* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject* Clone(void * pArg = nullptr);
	virtual void Free() override;

private:
	void Update_Frame();
	void Update_Sprite(_float fDeltaTime);

	void Stalking(CManagement* _pManagement, float fDeltaTime);
	void Attacked();
	void Attack();
	bool AttackTimer(float fDeltaTime);
	_uint Get_AttackKey();
	//패턴1. 주변에 작은 굼바 뿌리기.	
	void PatternOne();
	//패턴2. 일정 시간마다 먼지몬스터 뿌리기.
	void PatternTwo();

private:
	class CVIBuffer*		m_pVIBufferCom = nullptr;
	class CTexture*			m_pTextureCom[STATE::STATE_END];
	class CTransform*		m_pTargetTransform = nullptr;

	STATE m_eState = STATE::STATE_END;
	STATE m_ePreState = STATE::STATE_END;
	
	bool m_bStalk;

	bool m_bAttack;
	bool m_bAttacked;
	bool m_bAttackStart;

	enum PATTERN_KEY { PATTERN_ONE, PATTERN_TWO, PATTERN_END};
	PATTERN_KEY m_ePatternKey = PATTERN_KEY::PATTERN_END;
	//왜만들었지?
//	PATTERN_KEY m_ePastPatterKey = PATTERN_KEY::PATTERN_END;

	int m_iHp;
	float m_fSpeed;
	int m_iMiniMonNum;
	int m_arrPattern[5] = { 1, 1, 1, 1, 1 };
	int m_iPatternKey = 0;
};

