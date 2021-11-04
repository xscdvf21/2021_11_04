#include "framework.h"
#include "MiddleBossB.h"
#include "Management.h"


CMiddleBossB::CMiddleBossB(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject(pDevice)
{
}


CMiddleBossB::CMiddleBossB(const CMiddleBossB & other)
	: CGameObject(other)
{
}


HRESULT CMiddleBossB::Ready_GameObject_Prototype()
{
	if (FAILED(CGameObject::Ready_GameObject_Prototype()))
		return E_FAIL;

	return S_OK;

}

HRESULT CMiddleBossB::Ready_GameObject(void * pArg)
{
	if (FAILED(CGameObject::Ready_GameObject(pArg)))
		return E_FAIL;

	m_pTransformCom->Initialize_vPos(_float3(1.f, 0.f, 1.f));
	m_pTransformCom->Initialize_vScale(_float3(5.f, 5.f, 5.f));
	m_pTransformCom->Initialize_fRotate(D3DXToRadian(90.f));

	m_eState = STATE::STATE_IDLE_FRONT;

	m_bAttack = false;
	m_bAttacked = false;
	m_bAttackStart = true;

	m_iHp = 10;
	m_fSpeed = 10.f;
	m_iMiniMonNum = 10.f;



	if (FAILED(Add_Component()))
		return E_FAIL;

	return S_OK;
}

_uint CMiddleBossB::Update_GameObject(_float fDeltaTime)
{
	auto pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return 0;

	CGameObject::Update_GameObject(fDeltaTime);
	//CMiddleBossB::Update_Sprite(fDeltaTime);

	if (m_bAttacked)
	{
		m_e
	}
	else if (m_bAttack)
	{

	}

	//if (STATE::STATE_ATTACK_FRONT != m_eState)
	//{
	//	CMiddleBossA::Player_Check(pManagement, fDeltaTime);
	//}
	//else if (STATE::STATE_ATTACK_FRONT == m_eState)
	//{
	//	ImAttacking(pManagement, fDeltaTime);
	//}

	//CMiddleBossA::Attack_TimingChecker(fDeltaTime);


	////스프라이트 올리는 타이밍 째는 코드.
	//m_tFrame.fFrameStart += m_tFrame.fFrameMax*fDeltaTime*m_tFrame.fFrameSpeed;
	//if (m_tFrame.fFrameStart >= m_tFrame.fFrameMax)
	//{
	//	if (m_tFrame.iSpriteNow == m_tFrame.iSpriteEnd)
	//	{
	//		m_tFrame.iSpriteNow = m_tFrame.iSpriteStart;
	//		m_tFrame.fFrameStart = 0.f;
	//	}
	//	else
	//	{
	//		++m_tFrame.iSpriteNow;
	//		m_tFrame.fFrameStart = 0.f;
	//	}
	//}

	//return _uint();

}

_uint CMiddleBossB::LateUpdate_GameObject(_float fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);

	auto pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return 0;

	if (FAILED(pManagement->Add_RenderList(ERenderPriority::Alpha, this)))
		return 0;

	return m_pTransformCom->Update_Transform();
}

HRESULT CMiddleBossB::Render_GameObject()
{
	if (FAILED(m_pTransformCom->Set_Transform()))
		return E_FAIL;

	if (FAILED(CGameObject::Render_GameObject()))
		return E_FAIL;

	if (FAILED(m_pTextureCom[m_eState]->Set_Texture(m_tFrame.iSpriteNow)))
		return E_FAIL;


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

HRESULT CMiddleBossB::Add_Component()
{//필요한 이미지 가져옴
	//if (FAILED(CGameObject::Add_Component(
	//	(_uint)ESceneType::Static,
	//	L"Component_VIBuffer_RectTexture",
	//	L"Com_VIBuffer",
	//	(CComponent**)&m_pVIBufferCom)))
	//	return E_FAIL;

	//if (FAILED(CGameObject::Add_Component(
	//	(_uint)ESceneType::Static,
	//	L"Component_Texture_MiddleA_Move",
	//	L"Com_Texture",
	//	(CComponent**)&m_pTextureCom[STATE::STATE_IDLE_FRONT])))
	//	return E_FAIL;

	return S_OK;
}

CMiddleBossB * CMiddleBossB::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CMiddleBossB* pInstance = new CMiddleBossB(pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		Safe_Release(pInstance);

	}
	return pInstance;
}

CGameObject * CMiddleBossB::Clone(void * pArg)
{
	CMiddleBossB* pInstance = new CMiddleBossB(*this);
	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		PRINT_LOG(L"MiddleBossB", L"Failed Clone");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMiddleBossB::Free()
{
	CGameObject::Free();
}
