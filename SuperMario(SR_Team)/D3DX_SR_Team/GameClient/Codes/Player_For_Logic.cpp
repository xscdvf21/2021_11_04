#include "framework.h"
#include "Player.h"



HRESULT CPlayer::KeyCheck(_float fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	//이동 관련.
	CPlayer::MoveKey_Manager(fDeltaTime);
	CPlayer::JumpKey_Manager(fDeltaTime);
	CPlayer::AttackKey_Manager(fDeltaTime);
	CPlayer::PaperKey_Manager(fDeltaTime);

}


HRESULT CPlayer::MoveKey_Manager(_float fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	//점프, 공격, 페이퍼에 리턴.
#pragma region 예외 처리.
	if (STATE::STATE_JUMP_FORNT == m_eState
		|| STATE::STATE_JUMP_BACK == m_eState
		|| STATE::STATE_ATTACK_SIDE == m_eState
		|| STATE::STATE_ATTACK_BACK == m_eState
		|| STATE::STATE_ATTACK_FRONT == m_eState
		|| STATE::STATE_PAPER_SIDE == m_eState
		|| STATE::STATE_PAPER_BACK == m_eState
		|| STATE::STATE_PAPER_FRONT == m_eState
		|| true == m_bJumpEvent
		|| true == m_bAttackCheck
		|| true == m_bPaperCheck)
	{
		return S_OK;
	}
#pragma endregion
	_float3 vPos = { 0.f,0.f,0.f };

	if (pManagement->Key_Pressing(KEY_UP))
	{
		if (pManagement->Key_Pressing(KEY_RIGHT)) // 대각선 방향 처리
		{
			m_eState = STATE::STATE_MOVE_BACK;
			vPos = { m_fSpeed*0.5f, 0.f, m_fSpeed*0.5f };
			CPlayer::Update_Sprite();
			m_pTransformCom->Set_Rotation(_float3(0.f, 0.f, 0.f));
		}
		else if (pManagement->Key_Pressing(KEY_LEFT))// 대각선 방향 처리
		{
			m_eState = STATE::STATE_MOVE_BACK;
			vPos = { -m_fSpeed*0.5f, 0.f, m_fSpeed *0.5f };
			CPlayer::Update_Sprite();
			m_pTransformCom->Set_Rotation(_float3(0.f, 180.f, 0.f));
		}
		else
		{
			m_eState = STATE::STATE_MOVE_BACK;
			vPos = { 0.f, 0.f, m_fSpeed };
			CPlayer::Update_Sprite(); // rot 넣지 않는 것이 맍음.
		}
	}
	else if (pManagement->Key_Pressing(KEY_DOWN)) // LLMan
	{
		if (pManagement->Key_Pressing(KEY_RIGHT))// 대각선 방향 처리
		{
			m_eState = STATE::STATE_MOVE_FRONT;
			vPos = { m_fSpeed*0.5f, 0.f, -m_fSpeed*0.5f };
			CPlayer::Update_Sprite();
			m_pTransformCom->Set_Rotation(_float3(0.f, 0.f, 0.f));
		}
		else if(pManagement->Key_Pressing(KEY_LEFT))// 대각선 방향 처리
		{
			m_eState = STATE::STATE_MOVE_FRONT;
			vPos = { -m_fSpeed*0.5f, 0.f, -m_fSpeed *0.5f};
			CPlayer::Update_Sprite();
			m_pTransformCom->Set_Rotation(_float3(0.f, 180.f, 0.f));
		}
		else
		{
			m_eState = STATE::STATE_MOVE_FRONT;
			vPos = { 0.f, 0.f, -m_fSpeed };
			CPlayer::Update_Sprite();
		}
		
	}
	else if (pManagement->Key_Pressing(KEY_RIGHT))
	{
		m_eState = STATE::STATE_MOVE_FRONT;
		vPos = { m_fSpeed, 0.f, 0.f };
		CPlayer::Update_Sprite();
		m_pTransformCom->Set_Rotation(_float3(0.f, 0.f, 0.f));
	}
	else if (pManagement->Key_Pressing(KEY_LEFT))
	{
		m_eState = STATE::STATE_MOVE_FRONT;
		vPos = { -m_fSpeed, 0.f, 0.f };
		CPlayer::Update_Sprite();
		m_pTransformCom->Set_Rotation(_float3(0.f, 180.f, 0.f));
	}
	else//LLMan
	{
		CPlayer::State_VariableCorrection();
		CPlayer::Update_Sprite();
	}

	m_pTransformCom->AddPostion(vPos*fDeltaTime);

	return S_OK;
}

HRESULT CPlayer::JumpKey_Manager(_float fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	//공격, 페이퍼시 리턴.
	if (STATE::STATE_ATTACK_SIDE == m_eState
		|| STATE::STATE_ATTACK_BACK == m_eState
		|| STATE::STATE_ATTACK_FRONT == m_eState
		|| STATE::STATE_PAPER_SIDE == m_eState
		|| STATE::STATE_PAPER_BACK == m_eState
		|| STATE::STATE_PAPER_FRONT == m_eState
		|| true == m_bAttackCheck
		|| true == m_bPaperCheck)
	{
		return S_OK;
	}

	_float3 vPos = { 0.f,0.f,0.f };

	if (pManagement->Key_Down(KEY_SPACE)||m_bJumpEvent == true) // 점프시 방향전환을 위해.
	{
		if (true != m_bJumpEvent)//안전장치 // 점프시 가속과 감속을 한 번에 초기화 함.
		{
			m_bJumpEvent = true;
			m_fAccel = 6.f;
			m_fDeccl = 10.f;
		}

		if (pManagement->Key_Pressing(KEY_UP))
		{
			if (pManagement->Key_Pressing(KEY_RIGHT))
			{
				m_eState = STATE::STATE_JUMP_BACK;
				vPos = { m_fSpeed* 0.5f*m_fSpeedDamage , 0.f, m_fSpeed* 0.5f*m_fSpeedDamage };
				CPlayer::Update_Sprite();
				m_pTransformCom->Set_Rotation(_float3(0.f, 0.f, 0.f));
			}
			else if (pManagement->Key_Pressing(KEY_LEFT))
			{
				m_eState = STATE::STATE_JUMP_BACK;
				vPos = { -m_fSpeed* 0.5f*m_fSpeedDamage ,0.f, m_fSpeed* 0.5f*m_fSpeedDamage };
				CPlayer::Update_Sprite();
				m_pTransformCom->Set_Rotation(_float3(0.f, 180.f, 0.f));
			}
			else
			{
				m_eState = STATE::STATE_JUMP_BACK;
				vPos = { 0.f, 0.f, m_fSpeed*m_fSpeedDamage };
				CPlayer::Update_Sprite();
			}
		}
		else if (pManagement->Key_Pressing(KEY_DOWN))
		{
			if (pManagement->Key_Pressing(KEY_RIGHT))
			{
				m_eState = STATE::STATE_JUMP_FORNT;
				vPos = { m_fSpeed*0.5f*m_fSpeedDamage , 0.f, -m_fSpeed*0.5f*m_fSpeedDamage };
				CPlayer::Update_Sprite();
				m_pTransformCom->Set_Rotation(_float3(0.f, 0.f, 0.f));
			}
			else if (pManagement->Key_Pressing(KEY_LEFT))
			{
				m_eState = STATE::STATE_JUMP_FORNT;
				vPos = { -m_fSpeed*0.5f*m_fSpeedDamage ,0.f, -m_fSpeed*0.5f*m_fSpeedDamage };
				CPlayer::Update_Sprite();
				m_pTransformCom->Set_Rotation(_float3(0.f, 180.f, 0.f));
			}
			else
			{
				m_eState = STATE::STATE_JUMP_FORNT;
				vPos = { 0.f, 0.f, -m_fSpeed*m_fSpeedDamage };
				CPlayer::Update_Sprite();
			}
			
		}
		else if (pManagement->Key_Pressing(KEY_RIGHT))
		{
			m_eState = STATE::STATE_JUMP_FORNT;
			vPos = { m_fSpeed*m_fSpeedDamage  , 0.f, 0.f};
			CPlayer::Update_Sprite();
			m_pTransformCom->Set_Rotation(_float3(0.f, 0.f, 0.f));
		}
		else if (pManagement->Key_Pressing(KEY_LEFT))
		{
			m_eState = STATE::STATE_JUMP_FORNT;
			vPos = { -m_fSpeed*m_fSpeedDamage  ,0.f, 0.f};
			CPlayer::Update_Sprite();
			m_pTransformCom->Set_Rotation(_float3(0.f, 180.f, 0.f));
		}
		else//LLMan
		{
			m_eState = STATE::STATE_JUMP_FORNT;
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

HRESULT CPlayer::AttackKey_Manager(_float fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;
	
	if (STATE::STATE_JUMP_FORNT == m_eState
		|| STATE::STATE_JUMP_BACK == m_eState
		|| STATE::STATE_PAPER_SIDE == m_eState
		|| STATE::STATE_PAPER_SIDE == m_eState
		|| STATE::STATE_PAPER_BACK == m_eState
		|| true == m_bJumpEvent
		|| true == m_bPaperCheck)
	{
		return S_OK;
	}


	_float3 vDir = { 0.f, 0.f, 0.f };

	if (pManagement->Key_Down(KEY_Q) && true != m_bAttackCheck)
	{
		m_bAttackCheck = true;


		if (pManagement->Key_Pressing(KEY_UP))
		{
			m_eState = STATE::STATE_ATTACK_BACK;
			CPlayer::Update_Sprite();
		}
		else if (pManagement->Key_Pressing(KEY_DOWN))
		{
			m_eState = STATE::STATE_ATTACK_FRONT;
			CPlayer::Update_Sprite();
		}
		else if (pManagement->Key_Pressing(KEY_RIGHT))
		{
			m_eState = STATE::STATE_ATTACK_SIDE;
			CPlayer::Update_Sprite();
			m_pTransformCom->Set_Rotation(_float3(0.f, 0.f, 0.f));
		}
		else if (pManagement->Key_Pressing(KEY_LEFT))
		{
			m_eState = STATE::STATE_ATTACK_SIDE;
			CPlayer::Update_Sprite();
			m_pTransformCom->Set_Rotation(_float3(0.f, 180.f, 0.f));
		}
		else
		{
			m_eState = STATE::STATE_ATTACK_SIDE;
			CPlayer::Update_Sprite();
		}
	}

	return S_OK;
}

HRESULT CPlayer::PaperKey_Manager(_float fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	if (STATE::STATE_JUMP_FORNT == m_eState
		|| STATE::STATE_JUMP_BACK == m_eState
		|| STATE::STATE_ATTACK_SIDE == m_eState
		|| STATE::STATE_ATTACK_BACK == m_eState
		|| STATE::STATE_ATTACK_FRONT == m_eState
		|| true == m_bJumpEvent
		|| true == m_bAttackCheck)
	{
		return S_OK;
	}

	

	if (pManagement->Key_Down(KEY_E) && true != m_bPaperCheck)
	{
		m_bPaperCheck = true;
	
		//수정필요 // 씬에 맞는 
		m_tNowInfo.iSceneType = (_uint)ESceneType::TestStage;
		CGameObject::Quick_Effect(L"PlayerEffectLayer", m_tNowInfo, EFFECTTYPE::EFFECTTYPE_PAPER);

		if (pManagement->Key_Pressing(KEY_UP))
		{
			m_eState = STATE::STATE_PAPER_BACK;
			CPlayer::Update_Sprite();
		}
		else if (pManagement->Key_Pressing(KEY_DOWN))
		{
			m_eState = STATE::STATE_PAPER_FRONT;
			CPlayer::Update_Sprite();
		}
		else if (pManagement->Key_Pressing(KEY_RIGHT))
		{
			m_eState = STATE::STATE_PAPER_SIDE;
			CPlayer::Update_Sprite();
			m_pTransformCom->Set_Rotation(_float3(0.f, 0.f, 0.f));
		}
		else if (pManagement->Key_Pressing(KEY_LEFT))
		{
			m_eState = STATE::STATE_PAPER_SIDE;
			CPlayer::Update_Sprite();
			m_pTransformCom->Set_Rotation(_float3(0.f, 180.f, 0.f));
			State_VariableCorrection();
		}
		else
		{
			m_eState = STATE::STATE_PAPER_SIDE;
			CPlayer::Update_Sprite();
		}
	}

	return S_OK;
}


//=================================================================
//=================================================================
//=================================================================



HRESULT CPlayer::State_VariableCorrection() // 값으로 if문을 하기 때문에 안정성이 좋지 않음.
{
	_float fYAngle = D3DXToDegree(m_pTransformCom->Get_vScale(EAxis::Y));

	//MoveKey를 위한 조건.
	if (STATE::STATE_MOVE_BACK == m_eState
		|| STATE::STATE_JUMP_BACK == m_eState
		|| STATE::STATE_ATTACK_BACK == m_eState
		|| STATE::STATE_PAPER_BACK == m_eState)
	{
		if (false == m_bJumpEvent
			&& false == m_bAttackCheck
			&& false == m_bPaperCheck)
		{
			m_eState = STATE::STATE_IDLE_BACK; // idle 백이 없네.
			return S_OK;
		}
	}
	else
	{
		if (false == m_bJumpEvent
			&& false == m_bAttackCheck
			&& false == m_bPaperCheck)
		{
			m_eState = STATE::STATE_IDLE_FRONT;
			return S_OK;
		}
	}

	//점프 상황에 // 일단 만들고 아직은 쓰지 않을 거임.
	if (true == m_bJumpEvent)
	{
		if (STATE::STATE_IDLE_BACK == m_eState
			|| STATE::STATE_MOVE_BACK == m_eState
			|| STATE::STATE_JUMP_BACK == m_eState)
		{
			m_eState = STATE::STATE_JUMP_BACK;
			return S_OK;
		}
		else if (STATE::STATE_IDLE_FRONT == m_eState
			|| STATE::STATE_MOVE_FRONT == m_eState
			|| STATE::STATE_JUMP_FORNT == m_eState)
		{
			m_eState = STATE::STATE_JUMP_FORNT;
			return S_OK;
		}
	}

	if (90.f <= fYAngle)
	{
	}
	else if (90.f >= fYAngle)
	{
	}

	return S_OK;	
}





//=================================================================
//=================================================================
//=================================================================




HRESULT CPlayer::Movement(_float fDeltaTime)
{

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	m_fBulletTime += fDeltaTime;

	Bullet_Product();

	//움직이면서 공격, 페이퍼 할수있으나. 공격모션이 들어갔을땐 플레이어가 움직이면 안돼
	if (!bAttackCheck && !bPaperCheck)
	{
		//점프가 아니고, 키가 안눌릴때에는 아이들 상태에 두기위해.
		if (!m_bJumpEvent)
		{
			switch (iKeyCheck)
			{
			case KEY_RIGHT:
				m_eState = STATE::STATE_IDLE_FRONT;
				break;
			case KEY_LEFT:
				m_eState = STATE::STATE_IDLE_BACK;
				break;
			case KEY_UP:
				m_eState = STATE::STATE_IDLE_UP;
				break;
			case KEY_DOWN:
				m_eState = STATE::STATE_IDLE_FRONT;
				break;
			}
		}

		//점프 작동.
		if (pManagement->Key_Down(KEY_SPACE))
		{
			m_bJumpEvent = true;

			m_fDDeccel = 0.1f;
			m_fAccel = 3.f;
			m_fDeccl = 3.f;
			m_bJumpEvent = true;

			if (m_eState ==STATE::STATE_IDLE_UP)
				m_eState = STATE::STATE_JUMP_FORNT;
			//bJump = true;
			Quick_Effect(L"Player_Effect", m_tNowInfo, EFFECTTYPE::EFFECTTYPE_FIRE);
		}

		if (pManagement->Key_Pressing(KEY_DOWN))
		{
			if (!m_bJumpEvent)
				m_eState = STATE::STATE_MOVE_DOWN;

			iKeyCheck = KEY_DOWN;

			m_pTransformCom->Set_Scale(_float3(1.f, 1.f, 1.f));
			m_pTransformCom->AddPostion(_float3(0.f, 0.f, -1.f) * (2.5f * fDeltaTime));

		}
		else if (pManagement->Key_Pressing(KEY_UP))
		{

			if (!m_bJumpEvent)
				m_eState = STATE::STATE_MOVE_UP;

			iKeyCheck = KEY_UP;

			m_pTransformCom->AddPostion(_float3(0.f, 0.f, 1.f) * (2.5f * fDeltaTime));

		}


		if (pManagement->Key_Pressing(KEY_LEFT))
		{
			//점프중에 키 눌렀을때 방향 선정.
			if (!m_bJumpEvent)
				m_eState = STATE::STATE_MOVE_BACK;
			else if (m_bJumpEvent)
				m_eState = STATE::STATE_JUMP_BACK;

			iKeyCheck = KEY_LEFT;

			m_pTransformCom->AddPostion(_float3(-1.f, 0.f, 0.f) * (2.5f * fDeltaTime));

		}
		else if (pManagement->Key_Pressing(KEY_RIGHT))
		{
			//점프중에 키 눌렀을때 방향 선정.
			if (!m_bJumpEvent)
				m_eState = STATE::STATE_MOVE_FRONT;
			else if (m_bJumpEvent)
				m_eState = STATE::STATE_JUMP_FORNT;

			iKeyCheck = KEY_RIGHT;

			m_pTransformCom->AddPostion(_float3(1.f, 0.f, 0.f) * (2.5f * fDeltaTime));
		}

		//페이퍼키, 점프 아닐떄에만 할 수 있도록
		if (pManagement->Key_Down(KEY_E) && !m_bJumpEvent)
		{
			//테스트 
			m_pTransformCom->Set_Position(_float3(0.f, 3.f, 0.f));
			Quick_Effect(L"Player_Effect", m_tNowInfo, EFFECTTYPE::EFFECTTYPE_PAPER);
			switch (iKeyCheck)
			{
			case KEY_RIGHT:
				m_eState = STATE::STATE_PAPER_FRONT;
				break;
			case KEY_LEFT:
				m_eState = STATE::STATE_PAPER_BACK;
				break;
			case KEY_UP:
				m_eState = STATE::STATE_PAPER_UP;
				break;
			case KEY_DOWN:
				m_eState = STATE::STATE_PAPER_DOWN;
				break;
			}

			bPaperCheck = true;
		}

		//공격키인데. 점프가 아닐때에만 공격할수있도록.

		if (pManagement->Key_Down(KEY_Q) && !m_bJumpEvent)
		{
			m_fBulletTime = 0.f;
			
#pragma region 주석
			//여기는 맨처음에 총알 생성만을 목적으로 두었으나. 공격 애니메이션 타이밍과 총알생성을 맞추기위해,
			//위에m_fBulletTime를 이용하여, 조정


			//CGameObject* pObj = pManagement->Clone_GameObject_For_Load(
			//	(_uint)ESceneType::Static,
			//	L"GameObject_PlayerBullet",
			//	(_uint)ESceneType::TestStage,
			//	L"Layer_PlayerBullet"
			//);
			//_float3 vPos = m_pTransformCom->Get_Position();
#pragma endregion
			switch (iKeyCheck)
			{
			case KEY_RIGHT:
				m_eState = STATE::STATE_ATTACK_FRONT;
				break;
			case KEY_LEFT:
				m_eState = STATE::STATE_ATTACK_BACK;
				break;
			case KEY_UP:
				m_eState = STATE::STATE_ATTACK_UP;
				break;
			case KEY_DOWN:
				m_eState = STATE::STATE_ATTACK_DOWN;
				break;
			}

			bAttackCheck = true;
			return S_OK;
		}

	}
	return S_OK;
}

HRESULT CPlayer::Jump(_float fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return S_OK;




	//if (bJump)
	if(m_bJumpEvent)
	{
		bCheck_Collection();
		
		if (bJumpStart)
		{
			fStartPosY; //m_pTransformCom->Get_Position().y; // 중력 관련.
			bJumpStart = false;
		}

		
		//float fY = fStartPosY + (fJumpPower * m_fJumpTime - 0.45f * m_fJumpTime * 9.8f * m_fJumpTime);
		//m_pTransformCom->Set_Position(_float3(m_pTransformCom->Get_Position().x, fY, m_pTransformCom->Get_Position().z));


		//if (fStartPosY > m_pTransformCom->Get_Position().y) // y 축 0.f 이하로 떨어지지 않게 잡는 코드.
		//{
		//	m_pTransformCom->Set_Position({ m_pTransformCom->Get_Position().x, fStartPosY, m_pTransformCom->Get_Position().z });

		//	//bJump = false;
		//	m_bJumpEvent = false;
		//	m_fJumpTime = 0.f;
		//	bJumpStart = true;

		//	/*m_eAniIndex = PlayerAni::ANI_MOVE_FRONT;*/
		//	m_eState = STATE::STATE_IDLE_FRONT;
		//}
	}

	return S_OK;
}

void CPlayer::Update_Frame(_float fDeltaTime)
{
	m_tFrame.fFrameStart += m_tFrame.fFrameMax*fDeltaTime*m_tFrame.fFrameSpeed;

	if (m_tFrame.fFrameStart >= m_tFrame.fFrameMax)
	{
		if (m_tFrame.iSpriteNow >= m_tFrame.iSpriteEnd)//스프라이트가 끝일때.
		{
			m_tFrame.iSpriteNow = m_tFrame.iSpriteStart;
			m_tFrame.fFrameStart = 0.f;

			if (true == m_bAttackCheck || true == m_bPaperCheck)
			{
				m_bPaperCheck = false;
				m_bAttackCheck = false;

				if (false == m_bSwitch)
				{
					m_eState = STATE::STATE_IDLE_FRONT;//LLMan
				}
				else
				{
					m_eState = STATE::STATE_IDLE_SPECIAL;//LLMan
				}
				CPlayer::Update_Sprite();
			}
		}
		else
		{
			++m_tFrame.iSpriteNow;
			m_tFrame.fFrameStart = 0.f;
		}

	}
#pragma region 주석 

	//m_tFrame.fFrameStart += m_tFrame.fFrameMax*fDeltaTime*m_tFrame.fFrameSpeed;

	//if (m_tFrame.dwTime + m_tFrame.dwFrameTime < GetTickCount())
	//{
	//	m_tFrame.dwTime = GetTickCount();


	//	if (m_tFrame.iSpriteStart >= m_tFrame.iSpriteEnd)
	//	{
	//		//공격이 끝나고 난 후 다시 되돌려주기위해.
	//		if (bAttackCheck || bPaperCheck/*m_eState == STATE::STATE_ATTACK_FRONT ||
	//			m_eState != STATE::STATE_ATTACK_UP ||
	//			m_eState != STATE::STATE_ATTACK_BACK ||
	//			m_eState != STATE::STATE_ATTACK_DOWN*/)
	//		{
	//			bPaperCheck = false;
	//			bAttackCheck = false;
	//			m_eState = STATE::STATE_IDLE_FRONT;

	//			i = 0; //공격이 끝나면 다시 0으로바꿔서 총알 생성할수있게해줘야함.
	//		}

	//		m_tFrame.iSpriteStart = 0;

	//	}
	//	++m_tFrame.iSpriteStart;
	//}

#pragma endregion
}

void CPlayer::bCheck_Collection()
{
	//점프 모션 앞 뒤
	if (true == bJump)
	{
		if (m_eState == STATE::STATE_IDLE_FRONT)
		{
			m_eState = STATE::STATE_JUMP_FORNT;
		}
		else if (m_eState == STATE::STATE_IDLE_BACK)
		{
			m_eState = STATE::STATE_JUMP_BACK;
		}
	}

	//공격 모션 회수.
	if (m_eState == STATE::STATE_ATTACK_UP)
	{
		m_eState = STATE::STATE_IDLE_UP;
	}
	else if(m_eState == STATE::STATE_ATTACK_BACK) 
	{
		m_eState = STATE::STATE_IDLE_BACK;
	}
	else if (m_eState == STATE::STATE_ATTACK_DOWN)
	{
		m_eState = STATE::STATE_IDLE_DOWN;
	}
	else if (m_eState == STATE::STATE_ATTACK_FRONT)
	{
		m_eState = STATE::STATE_ATTACK_FRONT;
	}


	//아래 bCheck 회수.
	bPaperCheck = false;
	bAttackCheck = false;
	m_iBulletCounter = 0;
}


void CPlayer::Bullet_Product()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return;

	if (m_bAttackCheck)
	{
		if(m_tFrame.iSpriteNow == m_tFrame.iSpriteEnd)//애니메이션 끝이면 총알 생성.
		{
			CGameObject* pObj = pManagement->Clone_GameObject_For_Load(
				(_uint)ESceneType::Static,
				L"GameObject_PlayerBullet",
				(_uint)ESceneType::TestStage,
				L"Layer_PlayerBullet"
			);

			_float3 vPos = m_pTransformCom->Get_Position();

			_float fYAngle = D3DXToDegree(m_pTransformCom->Get_vScale(EAxis::Y));

			if (m_eState == STATE::STATE_ATTACK_FRONT)
				dynamic_cast<CPlayerBullet*>(pObj)->Set_Pos(_float3(vPos.x , vPos.y, vPos.z - 0.5f));

			else if (m_eState == STATE::STATE_ATTACK_BACK)
				dynamic_cast<CPlayerBullet*>(pObj)->Set_Pos(_float3(vPos.x , vPos.y, vPos.z + 0.5f));

			else if (m_eState == STATE::STATE_ATTACK_SIDE &&  90.f > fYAngle)
				dynamic_cast<CPlayerBullet*>(pObj)->Set_Pos(_float3(vPos.x + 0.5f, vPos.y, vPos.z ));

			else if (m_eState == STATE::STATE_ATTACK_SIDE && 90.f < fYAngle)
				dynamic_cast<CPlayerBullet*>(pObj)->Set_Pos(_float3(vPos.x - 0.5f, vPos.y, vPos.z ));
		}
	}
}

//한승현 테스트 코드.
#pragma region 한승현 테스트
void CPlayer::TestCode(_float fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return;


	if (pManagement->Key_Down(KEY_SPACE))
	{
		m_fDDeccel = 0.1f;
		m_fAccel = 3.f;
		m_fDeccl = 3.f;
		m_bJumpEvent = true;

		auto pManagement = CManagement::Get_Instance();
	}

	if (true == m_bJumpEvent)
	{
		m_fAccel -= m_fDeccl *fDeltaTime;
		m_pTransformCom->AddPostion(_float3(0.f, m_fAccel*fDeltaTime, 0.f));
	}

	m_pTransformCom->AddPostion(_float3(0.f, 1.f*-fDeltaTime, 0.f));

	if (pManagement->Key_Pressing(KEY_UP))
	{
		m_pTransformCom->AddPostion(_float3(0.f, 0.f, 20.f*fDeltaTime));
	}
	if (pManagement->Key_Pressing(KEY_DOWN))
	{
		m_pTransformCom->AddPostion(_float3(0.f, 0.f, 20.f*-fDeltaTime));
	}
	if (pManagement->Key_Pressing(KEY_LEFT))
	{
		m_pTransformCom->AddPostion(_float3(20.f*-fDeltaTime, 0.f, 0.f));
	}
	if (pManagement->Key_Pressing(KEY_RIGHT))
	{
		m_pTransformCom->AddPostion(_float3(20.f*fDeltaTime, 0.f, 0.f));
	}

}
#pragma endregion


HRESULT CPlayer::Jump_Collection(int _AniState)
{
	



	return S_OK;
}