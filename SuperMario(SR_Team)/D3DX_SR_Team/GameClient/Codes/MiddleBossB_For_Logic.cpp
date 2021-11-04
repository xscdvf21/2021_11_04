#include "framework.h"
#include "Management.h"

#include "..\Headers\MiddleBossB.h"
#include "MonsterA.h"


void CMiddleBossB::Update_Frame()
{
	if (m_tFrame.dwTime + m_tFrame.dwFrameTime < GetTickCount())
	{
		m_tFrame.dwTime = GetTickCount();

		if (m_tFrame.iSpriteStart >= m_tFrame.iSpriteEnd)
		{

		}

	}

}

void CMiddleBossB::Update_Sprite(_float fDeltaTime)
{
	if (m_ePreState != m_eState)
	{
		switch (m_eState)
		{
		case STATE::STATE_IDLE_FRONT:
			m_tFrame.iSpriteStart = 0;
			m_tFrame.iSpriteEnd = 6;
			m_tFrame.iSpriteNow = 0;//스프라이트의 어떤 종류냐.

			//m_tFrame.dwFrameTime = 1000;
			//m_tFrame.dwTime = GetTickCount();

			//m_tFrame.fFrameStart = 0.f;
			//m_tFrame.fFrameMax = 0.f; // 미정.->
			//m_tFrame.fFrameSpeed = 1.f;

			//m_tFrame.fEventStart = 0.f;
			//m_tFrame.fEventNow = m_tFrame.fEventStart;
			//m_tFrame.fEventMax = 10.f;
			//m_tFrame.fEventSpeed = 1.f;

			break;
		}
		m_ePreState = m_eState;
	}

}

void CMiddleBossB::Stalking(CManagement * _pManagement, float fDeltaTime)
{
	m_pTargetTransform = (CTransform*)_pManagement->Get_Component((_uint)ESceneType::TestStage, L"Layer_Player", L"Com_Transform");

	_float3 vTargetPos = m_pTargetTransform->Get_Position();
	_float3 vPos = m_pTransformCom->Get_Position();

	_float fDis = D3DXVec3Length(&(vPos - vTargetPos));

	if (10.f > fDis)		//스토킹 시작.
	{
		m_bStalk = true;
	}
	else
	{
		m_bStalk = false;
//		m_eState = STATE::STATE_IDLE_FRONT;
	}
//	m_eState = STATE::STATE_IDLE_FRONT;


	if (m_bStalk)
	{
		_float3 vDir = vTargetPos - vPos;
		D3DXVec3Normalize(&vDir, &vDir);

		m_pTransformCom->AddPostion(vDir* fDeltaTime);

//		m_eState = STATE::STATE_MOVE_FRONT;
	}
}


void CMiddleBossB::Attacked()
{
	if (m_iHp < 0)
	{
		bIsDead = true;
		return;
	}

	--m_iHp;
	m_bAttacked = false;
}

void CMiddleBossB::Attack()
{

	switch (m_ePatternKey)
	{
	case PATTERN_KEY::PATTERN_ONE:
		PatternOne();
		break;
	case PATTERN_KEY::PATTERN_TWO:
		PatternTwo();
		break;
	default:
		break;
	}

}

bool CMiddleBossB::AttackTimer(float fDeltaTime)
{
	float Time = fDeltaTime;

	if (!m_bAttack && (Time > 1.f))
	{
		return true;
	}
	return false;
}

_uint CMiddleBossB::Get_AttackKey()
{
	//공격텀 줘야함.
	if (m_bAttack)
	{
		++m_iPatternKey;
		if (m_iPatternKey > 5)
		{
			m_iPatternKey = 0;
		}
		return m_arrPattern[m_iPatternKey];
	}

	//공격초기상태 아니면 원래 공격키를 값으로 반환.
	return (_uint)m_ePatternKey;
}

void CMiddleBossB::PatternOne()
{	//패턴1. 주변에 작은 굼바 뿌리기.
	if (m_bAttackStart)
	{

		m_bAttackStart = false;
		m_bAttack = false;
	}
}

void CMiddleBossB::PatternTwo()
{	//패턴2. 일정 시간마다 먼지몬스터 뿌리기.

	if (m_bAttackStart)
	{
		auto pManagement = CManagement::Get_Instance();
		if (nullptr == pManagement)
			return;

		for (int i = 0; i < m_iMiniMonNum; ++i)
		{
			if (FAILED(pManagement->Clone_GameObject_ToLayer(
				(_uint)ESceneType::Static,
				L"GameObject_Monster_Goomba",
				(_uint)ESceneType::TestStage,
				L"Layer_Monster_Goomba")))
			{
				PRINT_LOG(L"Error", L"Failed To Add GameObject_Monster_Goomba");
				return;
			}

			_float3 vPos = (m_pTransformCom->Get_Position());
//			vPos = vPos + {1.f + (float)i, 0.f, 0.f};

			dynamic_cast<CMonsterA*>(pManagement->Get_GameObject((_uint)ESceneType::TestStage, L"Layer_Monster_Object", 0))
				->Set_Position(vPos);
		}
		m_bAttackStart = false;
		m_bAttack = false;
	}

}
