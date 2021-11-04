#include "framework.h"
#include "Player.h"



HRESULT CPlayer::KeyCheck_2D(_float fDeltaTime)
{
	CPlayer::MoveKey_Manager_2D(fDeltaTime);
	CPlayer::JumpKey_Manager_2D(fDeltaTime);
	return S_OK;
}

HRESULT CPlayer::MoveKey_Manager_2D(_float fDeltaTime)
{

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	//점프, 공격, 페이퍼에 리턴.
#pragma region 예외 처리.
	if (STATE::STATE_JUMP_SPECIAL == m_eState
		|| true == m_bJumpEvent)
	{
		return S_OK;
	}
#pragma endregion
	_float3 vPos = { 0.f,0.f,0.f };
	
	if (pManagement->Key_Pressing(KEY_DOWN)) // 앉기
	{

		if (-0.1f > m_f2dAccel)
		{
			m_f2dAccel += m_f2dAccelSlide*fDeltaTime*1.5f;
		}
		else if (0.1f < m_f2dAccel)
		{
			m_f2dAccel += m_f2dAccelSlide*-fDeltaTime*1.5f;
		}
		else
		{
			m_f2dAccel = 0.f;
		}
		m_eState = STATE::STATE_SIT_SPECIAL;
		CPlayer::Update_Sprite();
	}
	else if (pManagement->Key_Pressing(KEY_RIGHT)) // 걷기
	{
		if (m_f2dAccelLimit > m_f2dAccel)
		{
			m_f2dAccel += m_f2dAccelSlide*fDeltaTime;
		}
		//밀림 구현.
		if (-0.1f > m_f2dAccel)
		{
			m_eState = STATE::STATE_TURN_SPECIAL;
		}
		else
		{
			m_eState = STATE::STATE_MOVE_SPECIAL;
		}
		
		
		vPos = { m_f2dAccel, 0.f, 0.f };
		//m_pTransformCom->Go_AxisStraight(EAxis::X, m_f2dAccel*fDeltaTime);
		CPlayer::Update_Sprite();
		m_pTransformCom->Set_Rotation(_float3(0.f, 0.f, 0.f));                                              
	}
	else if (pManagement->Key_Pressing(KEY_LEFT))// 걷기.
	{
		if (m_f2dAccelLimit*-1.f < m_f2dAccel)
		{
			m_f2dAccel += m_f2dAccelSlide*-fDeltaTime;
		}

		//밀림 구현
		if (0.1f < m_f2dAccel)
		{
			m_eState = STATE::STATE_TURN_SPECIAL;
		}
		else
		{
			m_eState = STATE::STATE_MOVE_SPECIAL;
		}

		vPos = { m_f2dAccel, 0.f, 0.f };
		//m_pTransformCom->Go_AxisStraight(EAxis::X, m_f2dAccel*fDeltaTime);
		CPlayer::Update_Sprite();
		m_pTransformCom->Set_Rotation(_float3(0.f, 180.f, 0.f));
	}
	else
	{
		if (-0.1f > m_f2dAccel)
		{
			m_f2dAccel += m_f2dAccelSlide*fDeltaTime*1.5f;
		}
		else if (0.1f < m_f2dAccel)
		{
			m_f2dAccel += m_f2dAccelSlide*-fDeltaTime*1.5f;
		}
		else
		{
			m_f2dAccel = 0.f;
		}
		
		m_eState = STATE::STATE_IDLE_SPECIAL;	// idle
		CPlayer::Update_Sprite();
	}
	m_pTransformCom->AddPostion(vPos*fDeltaTime);
	return S_OK;
}
HRESULT CPlayer::JumpKey_Manager_2D(_float fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	//공격, 페이퍼시 리턴.
	if (STATE::STATE_SIT_SPECIAL == m_eState)
	{
		return S_OK;
	}

	_float3 vPos = { 0.f,0.f,0.f };

	if (pManagement->Key_Down(KEY_SPACE) || m_bJumpEvent == true) // 점프시 방향전환을 위해.
	{
		if (true != m_bJumpEvent)//안전장치 // 점프시 가속과 감속을 한 번에 초기화 함.
		{
			m_bJumpEvent = true;
			m_fAccel = 6.f;
			m_fDeccl = 10.f;
		}

		if (pManagement->Key_Pressing(KEY_RIGHT))
		{

			if (m_f2dAccelLimit > m_f2dAccel)
			{
				m_f2dAccel += m_f2dAccelSlide*fDeltaTime;
			}


			m_eState = STATE::STATE_JUMP_SPECIAL;
			//vPos = { m_fSpeed*m_fSpeedDamage  , 0.f, 0.f };
			vPos = { m_f2dAccel, 0.f, 0.f };
			CPlayer::Update_Sprite();
			m_pTransformCom->Set_Rotation(_float3(0.f, 0.f, 0.f));
		}
		else if (pManagement->Key_Pressing(KEY_LEFT))
		{
			if (m_f2dAccelLimit*-1.f < m_f2dAccel)
			{
				m_f2dAccel += m_f2dAccelSlide*-fDeltaTime;
			}

			m_eState = STATE::STATE_JUMP_SPECIAL;
			//vPos = { -m_fSpeed*m_fSpeedDamage  ,0.f, 0.f };
			vPos = { m_f2dAccel, 0.f, 0.f };
			CPlayer::Update_Sprite();
			m_pTransformCom->Set_Rotation(_float3(0.f, 180.f, 0.f));
		}
		else
		{
			m_eState = STATE::STATE_JUMP_SPECIAL;
			CPlayer::Update_Sprite();
		}
	}

	//가속 감속.
	if (true == m_bJumpEvent || true == m_bSinkholeCheck)
	{
		m_fAccel -= m_fDeccl *fDeltaTime;
		m_pTransformCom->AddPostion(_float3(0.f, m_fAccel*fDeltaTime, 0.f));
	}
	m_pTransformCom->AddPostion(vPos*fDeltaTime);

	return S_OK;
}


