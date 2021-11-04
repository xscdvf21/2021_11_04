#include "framework.h"
#include "..\Headers\BattleMonster.h"

CBattleMonster::CBattleMonster(LPDIRECT3DDEVICE9 pDevice)
	:CSuperMonster(pDevice)
{
}

CBattleMonster::CBattleMonster(const CBattleMonster & other)
	:CSuperMonster(other)
{
}

HRESULT CBattleMonster::Ready_GameObject_Prototype()
{
	if (FAILED(CGameObject::Ready_GameObject_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBattleMonster::Ready_GameObject(void * pArg)
{
	if (FAILED(CGameObject::Ready_GameObject(pArg)))
		return E_FAIL;

	//여기서 처음 추가하는듯.
	if (FAILED(Add_Component()))
		return E_FAIL;

	return S_OK;
}

_uint CBattleMonster::Update_GameObject(_float fDeltaTime)
{
	CGameObject::Update_GameObject(fDeltaTime);

	//움직임 체크
	if (FAILED(Movement(fDeltaTime)))
		return 0;

	CBattleMonster::Update_Sprite(fDeltaTime);
	CBattleMonster::Update_Frame();
	


	return _uint();
}

_uint CBattleMonster::LateUpdate_GameObject(_float fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);

	//여기서 랜더 리스트에 추가하는구나.
	auto pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return 0;

	if (FAILED(pManagement->Add_RenderList(ERenderPriority::NoAlpha, this)))
		return 0;

	return m_pTransformCom->Update_Transform();
}

HRESULT CBattleMonster::Render_GameObject()
{
	if (FAILED(m_pTransformCom->Set_Transform()))
		return E_FAIL;

	if (FAILED(CGameObject::Ready_GameObject()))
		return E_FAIL;

	if (FAILED(m_pTextureCom[m_eState]->Set_Texture(m_tFrame.iSpriteStart)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBattleMonster::Add_Component()
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
		L"Component_Texture_Monster_Lakitu",
		L"Com_Texture",
		(CComponent**)&m_pTextureCom[STATE::STATE_MOVE_FRONT]
	)))
		return E_FAIL;

	/*For. Com_Texture*/
	if (FAILED(CGameObject::Add_Component(
		(_uint)ESceneType::Static,
		L"Component_Texture_Monster_Lakitu_Idle",
		L"Com_Texture",
		(CComponent**)&m_pTextureCom[STATE::STATE_IDLE_FRONT]
	)))
		return E_FAIL;


	return S_OK;
}

HRESULT CBattleMonster::Movement(_float fDeltaTime)
{
	auto pManagement = CManagement::Get_Instance();

	//플레이어 트랜스폼 가져옴.
	CTransform*		pPlayerTransform = (CTransform*)pManagement->Get_Component(
		(_uint)ESceneType::TestStage, L"Layer_Player", L"Com_Transform");

	_float3 vPlayerPos = pPlayerTransform->Get_Position();
	_float3 vPos = m_pTransformCom->Get_Position();

	_float3 vDir = vPos - vPlayerPos;

	_float	fDir = D3DXVec3Length(&vDir);


	if (5.f > fDir)
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

	return S_OK;
}

void CBattleMonster::Update_Sprite(_float fDeltaTime)
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
			m_tFrame.iSpriteEnd = 1;
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

void CBattleMonster::Update_Frame()
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

CBattleMonster * CBattleMonster::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CBattleMonster* pInstance = new CBattleMonster(pDevice);
	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		PRINT_LOG(L"Error", L"Failed To Create Monster (Cilent - CMonster.cpp)");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CBattleMonster::Clone(void * pArg)
{
	CBattleMonster* pInstance = new CBattleMonster(*this); //복사 생성자 호출.
	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		PRINT_LOG(L"Error", L"Failed To Clone Monster (Cilent - CMonster.cpp)");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBattleMonster::Free()
{
	CSuperMonster::Free();
}
