#include "framework.h"
#include "..\Headers\StageHP.h"
#include "TestCamera.h"
#include "Player.h"




CStageHP::CStageHP(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject(pDevice)
{
}

CStageHP::CStageHP(const CStageHP & other)
	: CGameObject(other)
{
}


HRESULT CStageHP::Ready_GameObject_Prototype()
{
	if (FAILED(CGameObject::Ready_GameObject_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CStageHP::Ready_GameObject(void * pArg)
{
	if (FAILED(CGameObject::Ready_GameObject(pArg)))
		return E_FAIL;

	if (FAILED(Add_Component()))
		return E_FAIL;

	D3DXCreateFont(m_pDevice, 25, 25, 1000, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, L"맑은고딕", &m_pFont);

	return S_OK;
}

_uint CStageHP::Update_GameObject(_float fDeltaTime)
{
	CGameObject::Update_GameObject(fDeltaTime);

	return _uint();
}

_uint CStageHP::LateUpdate_GameObject(_float fDeltaTime)
{
	auto pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return 0;

	if (FAILED(pManagement->Add_RenderList(ERenderPriority::UI, this)))
		return 0;

	return m_pTransformCom->Update_Transform();
}

HRESULT CStageHP::Render_GameObject()
{
	auto pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return 0;

	CTestCamera*	pCamera = dynamic_cast<CTestCamera*>(pManagement->Get_GameObject((_uint)ESceneType::TestStage, L"Layer_Camera"));


	m_tPlayerInfo = dynamic_cast<CPlayer*>(pManagement->Get_GameObject((_uint)ESceneType::TestStage, L"Layer_Player"))->Get_Info();
	TCHAR szBuf[128] = L"";			//테스트 문구 수정해주세요
	swprintf_s(szBuf, L"%d / %d", m_tPlayerInfo.iHp, m_tPlayerInfo.iMaxHp );


	//사각형 영역 설정 , -1은 옵션에 NOCLIP을 넣었기 때문에 , 값을 주지 않은것이다

	RECT rc = { 100, 100, -1, -1 };

	m_pFont->DrawText(NULL, szBuf, -1, &rc, DT_LEFT | DT_NOCLIP, 0xffffff00);

	pCamera->Set_Ortho_For_UI(_float3(50.f, 50.f, 1.f), _float3(-350.f, 200.f, 1.f));

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

HRESULT CStageHP::Add_Component()
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
		L"Component_Texture_Stage_UI_Hp",
		L"Com_Texture",
		(CComponent**)&m_pTextureCom
	)))
		return E_FAIL;

	return S_OK;
}

CStageHP * CStageHP::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CStageHP* pInstance = new CStageHP(pDevice);
	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		PRINT_LOG(L"Error", L"Failed To Create CStageHP (Cilent - CStageHP.cpp)");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CStageHP::Clone(void * pArg)
{
	CStageHP* pInstance = new CStageHP(*this); //복사 생성자 호출.
	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		PRINT_LOG(L"Error", L"Failed To Clone CStageHP (Cilent - CMonster.cpp)");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStageHP::Free()
{
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	CGameObject::Free();
}
