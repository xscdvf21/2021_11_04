#include "framework.h"
#include "..\Headers\MonsterE.h"


CMonsterE::CMonsterE(LPDIRECT3DDEVICE9 pDevice)
	:CSuperMonster(pDevice)
{
}

CMonsterE::CMonsterE(const CMonsterE & other)
	:CSuperMonster(other)
{
}



HRESULT CMonsterE::Ready_GameObject_Prototype()
{
	if (FAILED(CGameObject::Ready_GameObject_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonsterE::Ready_GameObject(void * pArg)
{
	if (FAILED(CGameObject::Ready_GameObject(pArg)))
		return E_FAIL;

	//여기서 처음 추가하는듯.
	if (FAILED(Add_Component()))
		return E_FAIL;

	//m_pTransformCom->Initialize_vPos(_float3(10.f, 0.f, 10.f));

	m_eState = STATE::STATE_MOVE_FRONT;

	return S_OK;
}

_uint CMonsterE::Update_GameObject(_float fDeltaTime)
{
	CGameObject::Update_GameObject(fDeltaTime);

	//움직임 체크
	if (FAILED(Movement(fDeltaTime)))
		return 0;


	CMonsterE::Update_Sprite(fDeltaTime);
	CMonsterE::Update_Frame();

	return _uint();
}

_uint CMonsterE::LateUpdate_GameObject(_float fDeltaTime)
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

HRESULT CMonsterE::Render_GameObject()
{
	if (FAILED(m_pTransformCom->Set_Transform()))
		return E_FAIL;

	if (FAILED(CGameObject::Render_GameObject()))
		return E_FAIL;

	if (FAILED(m_pTextureCom[m_eState]->Set_Texture(m_tFrame.iSpriteStart)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonsterE::Add_Component()
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
		L"Component_Texture_Monster_MoveGoomba",
		L"Com_Texture",
		(CComponent**)&m_pTextureCom[STATE::STATE_MOVE_FRONT]
	)))
		return E_FAIL;


	return S_OK;
}

HRESULT CMonsterE::Movement(_float fDeltaTime)
{
	auto pManagement = CManagement::Get_Instance();

	_float3 vPos = m_pTransformCom->Get_Position();

	fMoveTime += fDeltaTime;


	if (fMoveTime <= 2.f)
	{
		//vPos.x += 1.f;
		m_pTransformCom->AddPostion(_float3(fMoveSpeed * 0.5 * fDeltaTime, 0.f, 0.f) * 1.f);
	}
	else if (fMoveTime >= 2.f)
	{
		m_pTransformCom->AddPostion(_float3(fMoveSpeed * 0.5 * fDeltaTime, 0.f, 0.f) * -1.f);
		if (fMoveTime >= 4.f)
		{
			fMoveTime = 0.f;
		}
	}



	if (pManagement->Key_Down(KEY_LMBUTTON))
	{
		fMoveSpeed *= -1.f;
	}



	return S_OK;
}

void CMonsterE::Update_Sprite(_float fDeltaTime)
{
	if (m_ePreState != m_eState)
	{
		switch (m_eState)
		{
		case STATE::STATE_MOVE_FRONT:
			m_tFrame.iSpriteStart = 0;
			m_tFrame.iSpriteEnd = 5;
			m_tFrame.iSpriteNow = 0;//스프라이트의 어떤 종류냐.

			m_tFrame.dwFrameTime = 200;
			m_tFrame.dwTime = GetTickCount();
			break;
		}
		m_ePreState = m_eState;
	}
}

void CMonsterE::Update_Frame()
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


CMonsterE * CMonsterE::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CMonsterE* pInstance = new CMonsterE(pDevice);
	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		PRINT_LOG(L"Error", L"Failed To Create Monster (Cilent - CMonster.cpp)");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CMonsterE::Clone(void * pArg)
{
	CMonsterE* pInstance = new CMonsterE(*this); //복사 생성자 호출.
	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		PRINT_LOG(L"Error", L"Failed To Clone Monster (Cilent - CMonster.cpp)");
		Safe_Release(pInstance);
	}

	return pInstance; 
}

void CMonsterE::Free()
{
	CSuperMonster::Free();
}
