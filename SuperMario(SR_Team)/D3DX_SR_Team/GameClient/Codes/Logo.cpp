#include "framework.h"
#include "..\Headers\Logo.h"
#include "OSCamera.h"


CLogo::CLogo(LPDIRECT3DDEVICE9 pDevice)
	:CScene(pDevice)
{
}

HRESULT CLogo::Ready_Scene()
{
	if (FAILED(CScene::Ready_Scene()))
	{
		return E_FAIL;
	}
	
	m_pLoading = CLoading::Create(m_pDevice, ESceneType::Stage);
	if (nullptr == m_pLoading)
		return E_FAIL;

	//Add_OpeningBG_Layer(L"Opening_Scene");

	return S_OK;
}

_uint CLogo::Update_Scene(_float fDeltaTime)
{
	CScene::Update_Scene(fDeltaTime);

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
	{
		return 0;
	}

	if (pManagement->Key_Down(KEY_ENTER) && true == m_pLoading->IsFinished())
	{

		//if (FAILED(pManagement->Setup_CurrentScene((_uint)ESceneType::Stage, CStage01::Create(m_pDevice))))
		//if (FAILED(pManagement->Setup_CurrentScene((_uint)ESceneType::Battle, CBattleStageA::Create(m_pDevice))))
		if (FAILED(pManagement->Setup_CurrentScene((_uint)ESceneType::TestStage, CTestStage::Create(m_pDevice))))

		//if (FAILED(pManagement->Setup_CurrentScene((_uint)ESceneType::Stage, CStage01::Create(m_pDevice))))
	/*	if (FAILED(pManagement->Setup_CurrentScene((_uint)ESceneType::Battle, CBattleStageA::Create(m_pDevice))))*/
		//if (FAILED(pManagement->Setup_CurrentScene((_uint)ESceneType::TestStage, CTestStage::Create(m_pDevice))))

		{
			PRINT_LOG(L"Error", L"Failed To Setup_CurrentScene To Stage (Client - CLogo.cpp)");
			return E_FAIL;
		}

	}

	return NO_EVENT;
}

_uint CLogo::LateUpdate_Scene(_float fDeltaTime)
{
	CScene::LateUpdate_Scene(fDeltaTime);

	return NO_EVENT;
}

//오프닝 이미지 띄우기용
HRESULT CLogo::Add_OpeningBG_Layer(const wstring& LayerTag)
{
	auto pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	COSCamera::CAMERA_DESC CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(COSCamera::CAMERA_DESC));
	CameraDesc.vEye = _float3(0.f, 0.f, -10.f);
	CameraDesc.vUp = _float3(0.f, 1.f, 0.f);
	CameraDesc.fFovY = D3DXToRadian(90.f);
	CameraDesc.fAspect = (_float)WINCX / WINCY;
	CameraDesc.fZNear = 1.f; /* 투영변환 후(z나누기까지)에 0이 되어야하고 */
	CameraDesc.fZFar = 500.f;

	if (FAILED(pManagement->Clone_GameObject_ToLayer(
		(_uint)ESceneType::Static,
		L"GameObject_OpenningScene_Camera",
		(_uint)ESceneType::Logo,
		LayerTag,
		&CameraDesc)))
	{
		return E_FAIL;
	}

	if (FAILED(pManagement->Clone_GameObject_ToLayer(
		(_uint)ESceneType::Static,
		L"GameObject_Opening_UI",
		(_uint)ESceneType::Logo,
		LayerTag)))
	{
		return E_FAIL;
	}

	return S_OK;
}

CLogo * CLogo::Create(LPDIRECT3DDEVICE9 pDevice)
{

	CLogo* pInstance = new CLogo(pDevice);
	if (FAILED(pInstance->Ready_Scene()))
	{
		PRINT_LOG(L"Error", L"Failed To Create Logo (CLogo.cpp)");
		Safe_Release(pInstance);
	}

	SetWindowText(g_hWnd, L"Logo");

	return pInstance;
}

void CLogo::Free()
{
	/* 부모와 자식 간에 소멸자 호출 순서처럼 Free도 동일한 순서로 호출 */
	/* 자식 리소스부터 정리 */
	Safe_Release(m_pLoading);
	/* 부모 리소스 정리 */
	CScene::Free();
}

