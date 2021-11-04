#include "framework.h"
#include "..\Headers\MonsterB.h"



CMonsterB::CMonsterB(LPDIRECT3DDEVICE9 pDevice)
	:CSuperMonster(pDevice)
{
	ZeroMemory(&m_tMonsterInfo, sizeof(MONSTERINFO));
}

CMonsterB::CMonsterB(const CMonsterB & other)
	: CSuperMonster(other)
{
}

HRESULT CMonsterB::Ready_GameObject_Prototype()
{
	if (FAILED(CGameObject::Ready_GameObject_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonsterB::Ready_GameObject(void * pArg)
{
	if (FAILED(CGameObject::Ready_GameObject(pArg)))
		return E_FAIL;

	//���⼭ ó�� �߰��ϴµ�.
	if (FAILED(Add_Component()))
		return E_FAIL;

	//�̰� �ʿ���ڴµ� ?
	//m_tMonsterInfo.iMaxHp = 10; 
	//m_tMonsterInfo.iHp = 10;
	//m_tMonsterInfo.iAttack = 10;

	return S_OK;
}

_uint CMonsterB::Update_GameObject(_float fDeltaTime)
{
	CGameObject::Update_GameObject(fDeltaTime);

	//������ üũ
	if (FAILED(Movement(fDeltaTime)))
		return 0;

	CMonsterB::Update_Sprite(fDeltaTime);
	CMonsterB::Update_Frame();


	return _uint();
}

_uint CMonsterB::LateUpdate_GameObject(_float fDeltaTime)
{
	//���⼭ ���� ����Ʈ�� �߰��ϴ±���.
	auto pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return 0;

	if (FAILED(pManagement->Add_RenderList(ERenderPriority::NoAlpha, this)))
		return 0;

	return m_pTransformCom->Update_Transform();
}

HRESULT CMonsterB::Render_GameObject()
{
	if (FAILED(m_pTransformCom->Set_Transform()))
		return E_FAIL;

	if (FAILED(CGameObject::Render_GameObject()))
		return E_FAIL;

	if (FAILED(m_pTextureCom[m_eState]->Set_Texture(m_tFrame.iSpriteStart)))
		return E_FAIL;

	//���� ������
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

HRESULT CMonsterB::Add_Component()
{
	//����ִ� �������� ����Ұ͵��� �����µ� ?
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
		L"Component_Texture_MonsterB",
		L"Com_Texture",
		(CComponent**)&m_pTextureCom
	)))
		return E_FAIL;


	//����â
	CHealthCondition::HEALTH_DESC Healthdesc;
	ZeroMemory(&Healthdesc, sizeof(CHealthCondition::HEALTH_DESC));
	Healthdesc.iHP = 10;
	Healthdesc.iBaseAttack = 8;
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

HRESULT CMonsterB::Movement(_float fDeltaTime)
{
	auto pManagement = CManagement::Get_Instance();

	//�÷��̾� Ʈ������ ������.
	CTransform*		pPlayerTransform = (CTransform*)pManagement->Get_Component(
		(_uint)ESceneType::TestStage, L"Layer_Player", L"Com_Transform");

	_float3 vPlayerPos = pPlayerTransform->Get_Position();
	_float3 vPos = m_pTransformCom->Get_Position();

	_float3 vDir = vPos - vPlayerPos;

	_float	fDir = D3DXVec3Length(&vDir);


	if (5.f > fDir && 0.5f < fDir)
	{


		_float3  vDir = vPlayerPos - vPos; //������ - �����
		D3DXVec3Normalize(&vDir, &vDir);

		m_eState = STATE::STATE_MOVE_FRONT;

		m_pTransformCom->AddPostion(vDir * fDeltaTime);
	}
	else
	{
		m_eState = STATE::STATE_IDLE_FRONT;
	}

	//m_pTransformCom->Set_Position(vPlayerPos * fDeltaTime);

	return S_OK;
}

HRESULT CMonsterB::IsOnTerrain()
{

	PRINT_LOG(L"Error", L"Not Setted CMonsterB's OnTerrain");

	return E_FAIL;
}

HRESULT CMonsterB::Set_Transform()
{

	PRINT_LOG(L"Error", L"Not Setted CMonsterB's SetTransform");

	return E_FAIL;
}

void CMonsterB::Update_Sprite(_float fDeltaTime)
{
	if (m_ePreState != m_eState)
	{
		switch (m_eState)
		{
		case STATE::STATE_IDLE_FRONT:
			m_tFrame.iSpriteStart = 0;
			m_tFrame.iSpriteEnd = 1;
			m_tFrame.iSpriteNow = 0;//��������Ʈ�� � ������.
			break;
		case STATE::STATE_MOVE_FRONT:
			m_tFrame.iSpriteStart = 0;
			m_tFrame.iSpriteEnd = 5;
			m_tFrame.iSpriteNow = 0;//��������Ʈ�� � ������.

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

void CMonsterB::Update_Frame()
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

CMonsterB * CMonsterB::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CMonsterB* pInstance = new CMonsterB(pDevice);
	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		PRINT_LOG(L"Error", L"Failed To Create Monster (Cilent - CMonster.cpp)");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CMonsterB::Clone(void * pArg)
{
	CMonsterB* pInstance = new CMonsterB(*this); //���� ������ ȣ��.
	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		PRINT_LOG(L"Error", L"Failed To Clone Monster (Cilent - CMonster.cpp)");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMonsterB::Free()
{
	CSuperMonster::Free();
}
