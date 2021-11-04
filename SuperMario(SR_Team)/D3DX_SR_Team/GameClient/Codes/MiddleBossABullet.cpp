#include "framework.h"
#include "..\Headers\MiddleBossABullet.h"


CMiddleBossABullet::CMiddleBossABullet(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject(pDevice)
	, m_fLifeTime(2.f)
{
}

CMiddleBossABullet::CMiddleBossABullet(const CMiddleBossABullet & other)
	: CGameObject(other)
	, m_fLifeTime(2.f)
{
}

HRESULT CMiddleBossABullet::Ready_GameObject_Prototype()
{
	if (FAILED(CGameObject::Ready_GameObject_Prototype()))
		return E_FAIL;

	m_fDeccel = m_fAccel;
	m_fDeccel2 = m_fAccel;
	m_fAccel2 = m_fAccel;
	return S_OK;
}

HRESULT CMiddleBossABullet::Ready_GameObject(void * pArg /*= nullptr*/)
{
	if (FAILED(CGameObject::Ready_GameObject(pArg)))
		return E_FAIL;

	m_fDeccel = 1.f;

	if (FAILED(Add_Component()))
		return E_FAIL;

	return S_OK;
}

_uint CMiddleBossABullet::Update_GameObject(_float fDeltaTime)
{
	auto pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return 0;
	CGameObject::Update_GameObject(fDeltaTime);


	switch (m_eBulletType)
	{
	case TYPE_A:
		D3DXVec3Normalize(&m_vDir, &m_vDir);
		m_fRotY += 500.f * fDeltaTime;
		m_pTransformCom->Set_Rotation(_float3(0.f, m_fRotY, 0.f));
		m_pTransformCom->AddPostion(m_vDir*fDeltaTime*10.f);
		m_fLifeTime -= fDeltaTime;

		break;
	case TYPE_B:

		m_fDeccel += fDeltaTime*m_fAccel*6.f;
		m_fAccel += m_fDeccel*-fDeltaTime;


		m_pTransformCom->AddPostion(_float3(0.f, m_fAccel*fDeltaTime, 0.f));
		m_pTransformCom->AddPostion(m_vDir*fDeltaTime*10.f);

		if (0 > m_pTransformCom->Get_Position().y)
		{
			bIsDead = true;
		}

		break;
	case TYPE_C:
		break;
	case TYPE_D:
		break;
	case TYPE_END:
		break;
	default:
		break;
	}





	

	if (m_fLifeTime <= 0.f) 
	{
		bIsDead = true;
	}


	return _uint();
}

_uint CMiddleBossABullet::LateUpdate_GameObject(_float fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);

	auto pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return 0;

	if (FAILED(pManagement->Add_RenderList(ERenderPriority::Alpha, this)))
		return 0;

	return m_pTransformCom->Update_Transform();
}

HRESULT CMiddleBossABullet::Render_GameObject()
{
	if (FAILED(m_pTransformCom->Set_Transform()))
		return E_FAIL;

	if (FAILED(CGameObject::Render_GameObject()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Set_Texture(0)))
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

HRESULT CMiddleBossABullet::Add_Component()
{
	if (FAILED(CGameObject::Add_Component(
		(_uint)ESceneType::Static,
		L"Component_VIBuffer_RectTexture",
		L"Com_VIBuffer",
		(CComponent**)&m_pVIBufferCom)))
		return E_FAIL;


	if (FAILED(CGameObject::Add_Component(
		(_uint)ESceneType::Static,
		L"Component_Texture_MiddleA_Attack",
		L"Com_Texture",
		(CComponent**)&m_pTextureCom)))
		return E_FAIL;


	return S_OK;
}

CMiddleBossABullet * CMiddleBossABullet::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CMiddleBossABullet* pInstance = new CMiddleBossABullet(pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		Safe_Release(pInstance);

	}
	return pInstance;
}

CGameObject * CMiddleBossABullet::Clone(void * pArg/* = nullptr*/)
{
	CMiddleBossABullet* pInstance = new CMiddleBossABullet(*this);
	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		PRINT_LOG(L"CMiddleBossABullet", L"Failed Clone");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMiddleBossABullet::Free()
{
	CGameObject::Free();
}
