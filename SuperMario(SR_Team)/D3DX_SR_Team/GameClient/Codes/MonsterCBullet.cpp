#include "framework.h"
#include "..\Headers\MonsterCBullet.h"
#include "MonsterC.h"


CMonsterCBullet::CMonsterCBullet(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject(pDevice)
{
}


CMonsterCBullet::CMonsterCBullet(const CMonsterCBullet& other)
	: CGameObject(other)
{
}

HRESULT CMonsterCBullet::Ready_GameObject_Prototype()
{
	if (FAILED(CGameObject::Ready_GameObject_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonsterCBullet::Ready_GameObject(void * pArg)
{
	if (FAILED(CGameObject::Ready_GameObject(pArg)))
		return E_FAIL;

	if (FAILED(Add_Component()))
		return E_FAIL;

	if (nullptr != pArg)
	{
		CTransform::TRANSFORM_DESC* arg = (CTransform::TRANSFORM_DESC*)pArg;

		m_pTransformCom->Initialize_vPos(arg->vPos);
		m_pTransformCom->Initialize_vScale(_float3(0.2f, 0.2f, 0.2f));
	}

	auto pManagement = CManagement::Get_Instance();

	CTransform*		pPlayerTransform = (CTransform*)pManagement->Get_Component(
		(_uint)ESceneType::TestStage, L"Layer_Player", L"Com_Transform");

	_float3 vPlayerPos = pPlayerTransform->Get_Position();
	_float3 vPos = m_pTransformCom->Get_Position();

	vDir = vPlayerPos - vPos; //도착지 - 출발지
	//D3DXVec3Normalize(&vDir, &vDir);

	return S_OK;
}

_uint CMonsterCBullet::Update_GameObject(_float fDeltaTime)
{
	auto pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return 0;
	CGameObject::Update_GameObject(fDeltaTime);

	//일자 발사. 이후 지울것.
	m_pTransformCom->AddPostion(vDir * fDeltaTime);

	CMonsterCBullet::Movement(fDeltaTime);

	m_pLifeTime += fDeltaTime;

	if (m_pLifeTime > 3.0f) //총알 라이프타임.
	{
		bIsDead = true;
	}
	return _uint();
}

_uint CMonsterCBullet::LateUpdate_GameObject(_float fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);

	auto pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return 0;

	if (FAILED(pManagement->Add_RenderList(ERenderPriority::Alpha, this)))
		return 0;

	//m_pLifeTime += fDeltaTime;
	//if (m_pLifeTime >= 0.3f)
	//{
	//	bIsDead = true;
	//}

	return m_pTransformCom->Update_Transform();
}

HRESULT CMonsterCBullet::Render_GameObject()
{
	if (FAILED(m_pTransformCom->Set_Transform()))
		return E_FAIL;

	if (FAILED(CGameObject::Render_GameObject()))
		return E_FAIL;

	//if (FAILED(m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE)))
	//	return E_FAIL;
	//if (FAILED(m_pDevice->SetRenderState(D3DRS_ALPHAREF, 1)))
	//	return E_FAIL;
	//if (FAILED(m_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER)))
	//	return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
		return E_FAIL;

	//if (FAILED(m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE)))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CMonsterCBullet::Add_Component()
{
	if (FAILED(CGameObject::Add_Component(
		(_uint)ESceneType::Static,
		L"Component_VIBuffer_RectTexture",
		L"Com_VIBuffer",
		(CComponent**)&m_pVIBufferCom)))
		return E_FAIL;



	return S_OK;
}

HRESULT CMonsterCBullet::Movement(_float fDeltaTime)
{
	//auto pManagement = CManagement::Get_Instance();

	//CTransform*		pPlayerTransform = (CTransform*)pManagement->Get_Component(
	//	(_uint)ESceneType::TestStage, L"Layer_Player", L"Com_Transform");

	//_float3 vPlayerPos = pPlayerTransform->Get_Position();
	//_float3 vPos = m_pTransformCom->Get_Position();
	//
	//m_fJumpTime += fDeltaTime;


	//_float3 vDir = vPos - vPlayerPos;


	//_float fLen = D3DXVec3Length(&vDir);

	//fJumpPower = fLen * 9.8f * 0.5f / 5.f;

	//m_fJumpTime += fDeltaTime;

	//float fY = fStartPosY + (fJumpPower * m_fJumpTime - 0.45f * 9.8f * m_fJumpTime * m_fJumpTime);

	//m_pTransformCom->Set_Position(_float3(m_pTransformCom->Get_Position().x, fY , m_pTransformCom->Get_Position().z));

	return S_OK;
}



CMonsterCBullet * CMonsterCBullet::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CMonsterCBullet*	pInstance = new CMonsterCBullet(pDevice);
	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		PRINT_LOG(L"Error", L"플레이어 프로토타입 생성하다가 실패, CMonsterCBullet.cpp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CMonsterCBullet::Clone(void * pArg)
{
	CMonsterCBullet*	pInstance = new CMonsterCBullet(*this);



	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		PRINT_LOG(L"Error", L"플레이어 클론 생성하다가 실패, CPlayerBullet.cpp");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMonsterCBullet::Free()
{
	CGameObject::Free();
}
