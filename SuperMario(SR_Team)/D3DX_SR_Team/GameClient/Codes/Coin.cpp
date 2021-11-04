#include "framework.h"
#include "..\Headers\Coin.h"
#include "Player.h"
CCoin::CCoin(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject(pDevice)
{
}

CCoin::CCoin(const CCoin & other)
	:CGameObject(other)
{
}

HRESULT CCoin::Ready_GameObject_Prototype()
{
	if (FAILED(CGameObject::Ready_GameObject_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCoin::Ready_GameObject(void * pArg/* = nullptr*/)
{
	CGameObject::Ready_GameObject(pArg);

	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pTransformCom->Initialize_vScale(_float3(0.3f, 0.3f, 0.3f));

	return S_OK;
}

_uint CCoin::Update_GameObject(_float fDeltaTime)
{
	CGameObject::Update_GameObject(fDeltaTime);
	if (bIsDead)
	{
		CManagement* pManagement = CManagement::Get_Instance();
		m_tPlayerInfo = dynamic_cast<CPlayer*>(pManagement->Get_GameObject((_uint)ESceneType::TestStage, L"Layer_Player"))->Get_Info();
		m_tPlayerInfo.iCoin++;
		dynamic_cast<CPlayer*>(pManagement->Get_GameObject((_uint)ESceneType::TestStage, L"Layer_Player"))->Set_Info(m_tPlayerInfo);
		
	}
	CCoin::Movement(fDeltaTime);

	return _uint();
}

_uint CCoin::LateUpdate_GameObject(_float fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);

	auto pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return 0;

	m_pTransformCom->Set_Rotation(_float3(0.f, m_fAngle, 0.f));
	m_fAngle += 2.f;

	if (FAILED(pManagement->Add_RenderList(ERenderPriority::Alpha, this)))
		return 0;

	return m_pTransformCom->Update_Transform();
}

HRESULT CCoin::Render_GameObject()
{

	if (FAILED(m_pTransformCom->Set_Transform()))
		return E_FAIL;

	if (FAILED(CGameObject::Render_GameObject()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Set_Texture(0)))
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

HRESULT CCoin::Add_Component()
{
	if (FAILED(CGameObject::Add_Component(
		(_uint)ESceneType::Static,
		L"Component_VIBuffer_RectTexture",
		L"Com_VIBuffer",
		(CComponent**)&m_pVIBufferCom
	)))
	{
		return E_FAIL;
	}

	if (FAILED(CGameObject::Add_Component(
		(_uint)ESceneType::Static,
		L"Component_Texture_Item_Coin",
		L"Com_Texture",
		(CComponent**)&m_pTextureCom
	)))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CCoin::Movement(_float fDeltaTime)
{

	m_pTransformCom->Set_PosY(m_pTransformCom->Get_Position().y - (fDeltaTime));

	if (0.f >= m_pTransformCom->Get_Position().y)
	{
		m_pTransformCom->Set_PosY(0.f);
	}
	return S_OK;
}

CCoin * CCoin::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CCoin*		pInstance = new CCoin(pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		PRINT_LOG(L"Error", L"CCoin 프로토타입 생성 실패.");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CCoin::Clone(void * pArg)
{
	CCoin*		pInstance = new CCoin(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		PRINT_LOG(L"Error", L"CCoin 클론 생성 실패.");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCoin::Free()
{
	CGameObject::Free();
}


