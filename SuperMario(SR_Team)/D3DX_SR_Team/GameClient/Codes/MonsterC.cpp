#include "framework.h"
#include "..\Headers\MonsterC.h"
#include "Player.h"
#include "Coin.h"


CMonsterC::CMonsterC(LPDIRECT3DDEVICE9 pDevice)
	:CSuperMonster(pDevice)
{
	ZeroMemory(&m_tMonsterInfo, sizeof(MONSTERINFO));
}

CMonsterC::CMonsterC(const CMonsterC & other)
	: CSuperMonster(other)
{
}

HRESULT CMonsterC::Ready_GameObject_Prototype()
{
	if (FAILED(CGameObject::Ready_GameObject_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonsterC::Ready_GameObject(void * pArg)
{
	if (FAILED(CGameObject::Ready_GameObject(pArg)))
		return E_FAIL;

	//여기서 처음 추가하는듯.
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pTransformCom->Initialize_vScale(_float3(0.5f, 0.5f, 0.5f));

	//m_tMonsterInfo.iMaxHp = 30;
	//m_tMonsterInfo.iHp = 30;
	//m_tMonsterInfo.iAttack = 3;

	if (nullptr != pArg)
	{
		CTransform::TRANSFORM_DESC* arg = (CTransform::TRANSFORM_DESC*)pArg;

		m_pTransformCom->Initialize_vPos(arg->vPos);
	}

	m_eState = STATE::STATE_IDLE_FRONT;

	return S_OK;;
}

_uint CMonsterC::Update_GameObject(_float fDeltaTime)
{
	auto pManagement = CManagement::Get_Instance();
	CGameObject::Update_GameObject(fDeltaTime);

	//움직임 체크
	if (FAILED(Movement(fDeltaTime)))
		return 0;

	if (bBulletAndMonster)
	{
		m_tPlayerInfo = dynamic_cast<CPlayer*>(pManagement->Get_GameObject((_uint)ESceneType::TestStage, L"Layer_Player"))->Get_Info();
		m_pHealthConditionCom->Hit_HP(m_tPlayerInfo.iAttack);

		if (m_pHealthConditionCom->Get_Status().iHP <= 0)
		{

			for (int i = 0; i < 5; ++i)
			{

				CGameObject* pObj = pManagement->Clone_GameObject_For_Load(
					(_uint)ESceneType::Static,
					L"GameObject_Item_Coin",
					(_uint)ESceneType::TestStage,
					L"Layer_Item_Coin"
				);

				_float3 vMonsterPos = m_pTransformCom->Get_Position();
				dynamic_cast<CCoin*>(pObj)->Set_Pos(_float3(vMonsterPos.x + (i * 0.2), vMonsterPos.y + (i * 0.2f), vMonsterPos.z));
			}


			bIsDead = true;
		}
		bBulletAndMonster = false;
	}

	fAlphaTime += fDeltaTime;


	CMonsterC::Update_Sprite(fDeltaTime);
	CMonsterC::Update_Frame();

	return _uint();
}

_uint CMonsterC::LateUpdate_GameObject(_float fDeltaTime)
{
	//여기서 랜더 리스트에 추가하는구나.
	auto pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return 0;

	if (FAILED(pManagement->Add_RenderList(ERenderPriority::Alpha, this)))
		return 0;

	return m_pTransformCom->Update_Transform();
}

HRESULT CMonsterC::Render_GameObject()
{
	if (FAILED(m_pTransformCom->Set_Transform()))
		return E_FAIL;

	if (FAILED(CGameObject::Render_GameObject()))
		return E_FAIL;

	if (FAILED(m_pTextureCom[m_eState]->Set_Texture(m_tFrame.iSpriteStart)))
		return E_FAIL;

	//알파 블랜딩
	if (FAILED(m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE)))
		return E_FAIL;

	if (FAILED(m_pDevice->SetRenderState(D3DRS_ALPHAREF, 1)))
		return E_FAIL;

	if (FAILED(m_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER)))
		return E_FAIL;


	if (fAlphaTime <= 5.f)
	{
		if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
			return E_FAIL;
	}

	if (fAlphaTime >= 7.f)
	{
		fAlphaTime = 0.f;
	}

	if (FAILED(m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE)))
		return E_FAIL;


	return S_OK;
}

HRESULT CMonsterC::Add_Component()
{
	//담겨있는 원형들중 사용할것들을 고르는듯 ?
	//For. Com.VIBuffer
	if (FAILED(CGameObject::Add_Component(
		(_uint)ESceneType::Static,
		L"Component_VIBuffer_Rect",
		L"Com_VIBuffer",
		(CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/*For. Com_Texture*/
	if (FAILED(CGameObject::Add_Component(
		(_uint)ESceneType::Static,
		L"Component_Texture_Monster_ShyGuy",
		L"Com_Texture",
		(CComponent**)&m_pTextureCom[STATE::STATE_MOVE_FRONT]
	)))
		return E_FAIL;

	/*For. Com_Texture*/
	if (FAILED(CGameObject::Add_Component(
		(_uint)ESceneType::Static,
		L"Component_Texture_Monster_ShyGuy_Idle",
		L"Com_Texture",
		(CComponent**)&m_pTextureCom[STATE::STATE_IDLE_FRONT]
	)))
		return E_FAIL;

	//상태창
	CHealthCondition::HEALTH_DESC Healthdesc;
	ZeroMemory(&Healthdesc, sizeof(CHealthCondition::HEALTH_DESC));
	Healthdesc.iHP = 20;
	Healthdesc.iBaseAttack = 3;
	Healthdesc.iBaseDefence = 1;
	Healthdesc.iMoney = 100;

	if (FAILED(CGameObject::Add_Component(
		(_uint)ESceneType::Static,
		L"Component_HealthState",
		L"Com_Health",
		(CComponent**)&m_pHealthConditionCom, &Healthdesc
	)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonsterC::Movement(_float fDeltaTime)
{
	auto pManagement = CManagement::Get_Instance();

	//플레이어 트랜스폼 가져옴.
	CTransform*		pPlayerTransform = (CTransform*)pManagement->Get_Component(
		(_uint)ESceneType::TestStage, L"Layer_Player", L"Com_Transform");

	_float3 vPlayerPos = pPlayerTransform->Get_Position();
	_float3 vPos = m_pTransformCom->Get_Position();

	_float3 vDir = vPos - vPlayerPos;

	_float	fDir = D3DXVec3Length(&vDir);


	if (5.f > fDir && 0.5f < fDir)
	{


		_float3  vDir = vPlayerPos - vPos; //도착지 - 출발지
		D3DXVec3Normalize(&vDir, &vDir);

		m_eState = STATE::STATE_MOVE_FRONT;

		m_pTransformCom->AddPostion(vDir * fDeltaTime);
		if (4.f >= fDir)
		{
			
			m_fAttackTime += fDeltaTime;
			//플레이어와 더가까워지면, 총알쏨.
			if (m_fAttackTime >= 1.f)
			{
				CTransform::TRANSFORM_DESC x;
				x.vPos = m_pTransformCom->Get_Position();
				CGameObject* pObj = pManagement->Clone_GameObject_For_Load(
					(_uint)ESceneType::Static,
					L"GameObject_MonsterBullet",
					(_uint)ESceneType::TestStage,
					L"Layer_MonsterBullet"
					, &x);

				m_fAttackTime = 0.f;
			}
		
		}
	}
	else
	{
		m_eState = STATE::STATE_IDLE_FRONT;
	}




	//m_pTransformCom->Set_Position(vPlayerPos * fDeltaTime);

	return S_OK;
}

HRESULT CMonsterC::IsOnTerrain()
{
	PRINT_LOG(L"Error", L"Not Setted Player's SetTransform");
	return E_FAIL;
}

HRESULT CMonsterC::Set_Transform()
{
	PRINT_LOG(L"Error", L"Not Setted Player's SetTransform");
	return E_FAIL;
}

void CMonsterC::Update_Sprite(_float fDeltaTime)
{
	if (m_ePreState != m_eState)
	{
		switch (m_eState)
		{
		case STATE::STATE_IDLE_FRONT:
			m_tFrame.iSpriteStart = 0;
			m_tFrame.iSpriteEnd = 1;
			m_tFrame.iSpriteNow = 0;//스프라이트의 어떤 종류냐.
			break;
		case STATE::STATE_MOVE_FRONT:
			m_tFrame.iSpriteStart = 0;
			m_tFrame.iSpriteEnd = 5;
			m_tFrame.iSpriteNow = 0;//스프라이트의 어떤 종류냐.

			m_tFrame.dwFrameTime = 200;
			m_tFrame.dwTime = GetTickCount();

			//m_tFrame.fFrameStart = 0.f;
			//m_tFrame.fFrameMax = 2.f;
			//m_tFrame.fFrameSpeed = 5.f;
			break;
		}
		m_ePreState = m_eState;
	}
}

void CMonsterC::Update_Frame()
{
	if (m_tFrame.dwTime + m_tFrame.dwFrameTime < GetTickCount())
	{
		m_tFrame.dwTime = GetTickCount();

		if (m_tFrame.iSpriteStart >= m_tFrame.iSpriteEnd)
		{
			m_tFrame.iSpriteStart = 0;
		}
		++m_tFrame.iSpriteStart;

	}
}

CMonsterC * CMonsterC::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CMonsterC* pInstance = new CMonsterC(pDevice);
	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		PRINT_LOG(L"Error", L"Failed To Create Monster (Cilent - CMonster.cpp)");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CMonsterC::Clone(void * pArg)
{
	CMonsterC* pInstance = new CMonsterC(*this); //복사 생성자 호출.
	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		PRINT_LOG(L"Error", L"Failed To Clone Monster (Cilent - CMonster.cpp)");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMonsterC::Free()
{
	CSuperMonster::Free();
}
