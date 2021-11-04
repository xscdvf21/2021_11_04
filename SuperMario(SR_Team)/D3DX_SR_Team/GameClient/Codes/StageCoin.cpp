#include "framework.h"
#include "..\Headers\StageCoin.h"
#include "TestCamera.h"
#include "Player.h"

CStageCoin::CStageCoin(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject(pDevice)
{
}

CStageCoin::CStageCoin(const CStageCoin & other)
	: CGameObject(other)
{
}

HRESULT CStageCoin::Ready_GameObject_Prototype()
{
	if (FAILED(CGameObject::Ready_GameObject_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CStageCoin::Ready_GameObject(void * pArg)
{
	if (FAILED(CGameObject::Ready_GameObject(pArg)))
		return E_FAIL;

	//m_pTransformCom->Initialize_vPos(_float3(1.f, 1.f, 1.f));
	//m_pTransformCom->Initialize_vScale(_float3(24.f, 10.f, 1.f));
	//m_pTransformCom->Initialize_fSpeed(0.f);
	//m_pTransformCom->Initialize_fRotate(D3DXToRadian(0.f));

	//���⼭ ó�� �߰��ϴµ�.
	if (FAILED(Add_Component()))
		return E_FAIL;

	D3DXCreateFont(m_pDevice, 25, 25, 1000, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, L"�������", &m_pFont);

	return S_OK;
}


_uint CStageCoin::Update_GameObject(_float fDeltaTime)
{
	CGameObject::Update_GameObject(fDeltaTime);

	return _uint();
}

_uint CStageCoin::LateUpdate_GameObject(_float fDeltaTime)
{
	auto pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return 0;

	m_pTransformCom->Set_Rotation(_float3(0.f, m_fAngle,0.f));
	m_fAngle += 2.f;

	if (FAILED(pManagement->Add_RenderList(ERenderPriority::UI, this)))
		return 0;

	return m_pTransformCom->Update_Transform();
}

HRESULT CStageCoin::Render_GameObject()
{
	auto pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return 0;

	CTestCamera*	pCamera = dynamic_cast<CTestCamera*>(pManagement->Get_GameObject((_uint)ESceneType::TestStage, L"Layer_Camera"));

	m_tPlayerInfo = dynamic_cast<CPlayer*>(pManagement->Get_GameObject((_uint)ESceneType::TestStage, L"Layer_Player"))->Get_Info();



	TCHAR szBuf[128] = L"";			//�׽�Ʈ ���� �������ּ���
	swprintf_s(szBuf, L"%d", m_tPlayerInfo.iCoin);
	


	//�簢�� ���� ���� , -1�� �ɼǿ� NOCLIP�� �־��� ������ , ���� ���� �������̴�

	RECT rc = { 680, 500, -1, -1 };

	m_pFont->DrawText(NULL, szBuf, -1, &rc, DT_LEFT | DT_NOCLIP, 0xffffff00);

	pCamera->Set_Ortho_For_UI(_float3(50.f, 50.f, 1.f), _float3(250.f, -200.f, 5.f));

	if (FAILED(m_pTransformCom->Set_Transform()))
		return E_FAIL;

	if (FAILED(CGameObject::Ready_GameObject()))
		return E_FAIL;

	if (FAILED(m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Set_Texture(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
		return E_FAIL;

	if (FAILED(m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE)))
		return E_FAIL;


	return S_OK;
}

HRESULT CStageCoin::Add_Component()
{
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
		L"Component_Texture_Stage_UI_Coin",
		L"Com_Texture",
		(CComponent**)&m_pTextureCom
	)))
		return E_FAIL;

	return S_OK;
}

CStageCoin * CStageCoin::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CStageCoin* pInstance = new CStageCoin(pDevice);
	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		PRINT_LOG(L"Error", L"Failed To Create Monster (Cilent - CMonster.cpp)");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CStageCoin::Clone(void * pArg)
{
	CStageCoin* pInstance = new CStageCoin(*this); //���� ������ ȣ��.
	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		PRINT_LOG(L"Error", L"Failed To Clone Monster (Cilent - CMonster.cpp)");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStageCoin::Free()
{
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	CGameObject::Free();
}
