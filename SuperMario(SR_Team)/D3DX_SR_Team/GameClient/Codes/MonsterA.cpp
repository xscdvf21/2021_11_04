#include "framework.h"
#include "..\Headers\MonsterA.h"



CMonsterA::CMonsterA(LPDIRECT3DDEVICE9 pDevice)
	:CSuperMonster(pDevice)
{
}

CMonsterA::CMonsterA(const CMonsterA & other)
	: CSuperMonster(other)
{
}

HRESULT CMonsterA::Ready_GameObject_Prototype()
{
	if (FAILED(CGameObject::Ready_GameObject_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonsterA::Ready_GameObject(void * pArg)
{
	if (FAILED(CGameObject::Ready_GameObject(pArg)))
		return E_FAIL;
	//여기서 처음 추가하는듯.
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pTransformCom->Initialize_vScale(_float3(0.5f, 0.5f, 0.5f));

	if (nullptr != pArg)
	{
		CTransform::TRANSFORM_DESC* arg = (CTransform::TRANSFORM_DESC*)pArg;

		m_pTransformCom->Initialize_vPos(arg->vPos);
	}

	m_eState = STATE::STATE_IDLE_FRONT;


	return S_OK;
}

_uint CMonsterA::Update_GameObject(_float fDeltaTime)
{


	CGameObject::Update_GameObject(fDeltaTime);

	//움직임 체크
	if (FAILED(Movement(fDeltaTime)))
		return 0;
	

	
	CMonsterA::Update_Sprite(fDeltaTime);
	CMonsterA::Update_Frame();
	


	return _uint();
}

_uint CMonsterA::LateUpdate_GameObject(_float fDeltaTime)
{
	//여기서 랜더 리스트에 추가하는구나.
	auto pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return 0;

	if (FAILED(pManagement->Add_RenderList(ERenderPriority::Alpha, this)))
		return 0;

	return m_pTransformCom->Update_Transform();
}

HRESULT CMonsterA::Render_GameObject()
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

	if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
		return E_FAIL;

	if (FAILED(m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE)))
		return E_FAIL;


	return S_OK;
}

HRESULT CMonsterA::Add_Component()
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
		L"Component_Texture_Monster_Goomba",
		L"Com_Texture",
		(CComponent**)&m_pTextureCom[STATE::STATE_MOVE_FRONT]
	)))
		return E_FAIL;

	/*For. Com_Texture*/
	if (FAILED(CGameObject::Add_Component(
		(_uint)ESceneType::Static,
		L"Component_Texture_Monster_Goomba_Idle",
		L"Com_Texture",
		(CComponent**)&m_pTextureCom[STATE::STATE_IDLE_FRONT]
	)))
		return E_FAIL;

	//상태창
	CHealthCondition::HEALTH_DESC Healthdesc;
	ZeroMemory(&Healthdesc, sizeof(CHealthCondition::HEALTH_DESC));
	Healthdesc.iHP = 10;
	Healthdesc.iBaseAttack = 5;
	Healthdesc.iBaseDefence = 1;
	Healthdesc.iMoney = 100;

	if (FAILED(CGameObject::Add_Component(
		(_uint)ESceneType::Static,
		L"Component_HealthState",
		L"Com_Health",
		(CComponent**)&m_pHealthConditionCom,&Healthdesc
	)))
		return E_FAIL;


	return S_OK;
}

HRESULT CMonsterA::Movement(_float fDeltaTime)
{


	auto pManagement = CManagement::Get_Instance();

	//플레이어 트랜스폼 가져옴.
	CTransform*		pPlayerTransform = (CTransform*)pManagement->Get_Component(
		(_uint)ESceneType::TestStage, L"Layer_Player", L"Com_Transform");
	if (pPlayerTransform != nullptr)
	{


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
		}
		else
		{
			m_eState = STATE::STATE_IDLE_FRONT;
		}

	}


	//m_pTransformCom->Set_Position(vPlayerPos * fDeltaTime);

	return S_OK;
}

HRESULT CMonsterA::IsOnTerrain()
{

	PRINT_LOG(L"Error", L"Not Setted CMonsterA's OnTerrain");

	return E_FAIL;
}

HRESULT CMonsterA::Set_Transform()
{

	PRINT_LOG(L"Error", L"Not Setted CMonsterA's SetTransform");

	return E_FAIL;
}

void CMonsterA::Update_Sprite(_float fDeltaTime)
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

void CMonsterA::Update_Frame()
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



CMonsterA * CMonsterA::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CMonsterA* pInstance = new CMonsterA(pDevice);
	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		PRINT_LOG(L"Error", L"Failed To Create MonsterA (");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CMonsterA::Clone(void * pArg)
{
	CMonsterA* pInstance = new CMonsterA(*this); //복사 생성자 호출.
	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		PRINT_LOG(L"Error", L"Failed To Clone MonsterA ");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMonsterA::Free()
{
	CSuperMonster::Free();	
}
