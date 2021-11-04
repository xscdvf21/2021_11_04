#include "framework.h"
#include "Player.h"



CPlayer::CPlayer(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice)
{
	ZeroMemory(&m_tInfo, sizeof(PLAYERINFO));
}

CPlayer::CPlayer(const CPlayer& other)
	: CGameObject(other)
{
}

HRESULT CPlayer::Ready_GameObject_Prototype()
{
	if (FAILED(CGameObject::Ready_GameObject_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayer::Ready_GameObject(void* pArg/* = nullptr*/)
{
	if (FAILED(CGameObject::Ready_GameObject(pArg)))
		return E_FAIL;

	NOWINFO tNowInfo;

	m_tNowInfo.iPrototypeType = (_uint)ESceneType::Static;
	m_tNowInfo.iSceneType = (_uint)ESceneType::Stage;

	if (nullptr != pArg)
	{
		tNowInfo = *(NOWINFO*)pArg;
		m_tNowInfo.iSceneType = tNowInfo.iSceneType;
	}

	m_tPlayerInfo.iMaxHp = 60;
	m_tPlayerInfo.iHp = 40;
	m_tPlayerInfo.iAttack = 10;
	m_tPlayerInfo.iCoin = 10;
	m_tPlayerInfo.iPaper = 10;

	m_fAngel = 0.f;


#pragma region 한승현 테스트 코드.
	m_pTransformCom->Set_Position(_float3(0.f, 3.f, 0.f));
#pragma endregion

	if (FAILED(Add_Component()))
		return E_FAIL;

	m_eState = STATE::STATE_IDLE_FRONT;
	m_eState = STATE::STATE_IDLE_SPECIAL;
	
	return S_OK;
}

_uint CPlayer::Update_GameObject(_float fDeltaTime)
{
	if (bBulletAndPlayer) //몬스터가 플레이어와 충돌했을떄, 플레이어 피닳게함. 
	{
		m_tPlayerInfo.iHp--;
		bBulletAndPlayer = false;
	}
	
	
	if (false == m_bSwitch)
	{
		CPlayer::KeyCheck(fDeltaTime);
		CPlayer::Bullet_Product();
	}
	else if (true == m_bSwitch)
	{
		CPlayer::KeyCheck_2D(fDeltaTime);
	}

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	if (pManagement->Key_Down(KEY_C))
	{
		if (true == m_bSwitch)
		{
			m_bSwitch = false;
		}
		else if (false == m_bSwitch)
		{
			m_bSwitch = true;
		}

	}



	if (pManagement->Key_Down(KEY_D))
	{
		m_pTransformCom->Set_Rotation(EAxis::Y, D3DXToRadian(m_fAngel+90.f));
	}
	if (pManagement->Key_Down(KEY_A))
	{
		m_pTransformCom->Set_Rotation(EAxis::Y, D3DXToRadian(m_fAngel-90.f));
	}



	//CGameObject::Update_GameObject(fDeltaTime);
	//CPlayer::Jump(fDeltaTime); //플레이어 점프
	//CPlayer::Movement(fDeltaTime); //플레이어 공격 및 좌우
	//CPlayer::Update_Sprite(); //스프라이트 씬 전환
	//CPlayer::TestCode(fDeltaTime);

	CPlayer::Update_Frame(fDeltaTime); //스프라이트 프레임 속도
	return _uint();
}

_uint CPlayer::LateUpdate_GameObject(_float fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);

	auto pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return 0;

	if (FAILED(pManagement->Add_RenderList(ERenderPriority::Alpha, this)))
		return 0;


#pragma region 한승현 테스트 코드.
	if (fStartPosY > m_pTransformCom->Get_Position().y)
	{
		m_fAccel = 0.f;
		m_bSinkholeCheck = false;
		m_pTransformCom->Set_PosY(fStartPosY);
		m_bJumpEvent = false;

		if (false == m_bSwitch)
		{
			m_eState = STATE::STATE_IDLE_FRONT;
		}
		else if (true == m_bSwitch 
			&& STATE::STATE_MOVE_SPECIAL != m_eState 
			&& STATE::STATE_TURN_SPECIAL != m_eState 
			&& STATE::STATE_SIT_SPECIAL != m_eState)
		{
			m_eState = STATE::STATE_IDLE_SPECIAL;
		}
		
		CPlayer::Update_Sprite();
	
	}
#pragma endregion

	return m_pTransformCom->Update_Transform();
}

HRESULT CPlayer::Render_GameObject()
{
	float wtf = m_pTransformCom->Get_Position().y;
	if (FAILED(m_pTransformCom->Set_Transform()))
		return E_FAIL;

	if (FAILED(CGameObject::Render_GameObject()))
		return E_FAIL;

	if (FAILED(m_pTextureCom[m_eState]->Set_Texture(m_tFrame.iSpriteNow)))
		return E_FAIL;

	//알파 블랜딩
	if (FAILED(m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE)))
		return E_FAIL;

	if (FAILED(m_pDevice->SetRenderState(D3DRS_ALPHAREF, 1)))
		return E_FAIL;

	if (FAILED(m_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
		return E_FAIL;

	if (FAILED(m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE)))
		return E_FAIL;

	return S_OK;
}
HRESULT CPlayer::Add_Component()
{
	CPlayer::Add_Component_Lower();
	CPlayer::Add_Component_Lower_For_2D();
	return S_OK;
}

CPlayer* CPlayer::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CPlayer* pInstance = new CPlayer(pDevice);
	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		PRINT_LOG(L"Error", L"Failed To Create Player");
		Safe_Release(pInstance);
	}

	return pInstance; 
}
CGameObject* CPlayer::Clone(void* pArg/* = nullptr*/)
{
	CPlayer* pInstance = new CPlayer(*this); /*복사생성호출*/
	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		PRINT_LOG(L"Error", L"Failed To Clone Player");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayer::Free()
{
	CGameObject::Free();
}