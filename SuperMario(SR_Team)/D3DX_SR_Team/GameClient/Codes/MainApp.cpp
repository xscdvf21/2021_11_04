#include "framework.h"
#include "..\Headers\MainApp.h"
#include "Logo.h"

CMainApp::CMainApp() 
	: m_pManagement(CManagement::Get_Instance())
{
	Safe_AddReference(m_pManagement);
}

HRESULT CMainApp::Ready_MainApp()
{
	if (nullptr == m_pManagement)
		return E_FAIL;

	if (FAILED(m_pManagement->Ready_Engine(g_hWnd, WINCX, WINCY, EDisplayMode::Win, (_uint)ESceneType::MaxCount)))
	{
		PRINT_LOG(L"Error", L"Failed To Ready_Engine");
		return E_FAIL;
	}

	m_pDevice = m_pManagement->Get_Device();
	m_pFont = m_pManagement->Get_Font();
	m_pSprite = m_pManagement->Get_Sprite();

	if (nullptr == m_pDevice)
	{
		PRINT_LOG(L"Error", L"Failed To Get_Device");
		return E_FAIL;
	}

	Safe_AddReference(m_pDevice);
	Safe_AddReference(m_pFont);
	Safe_AddReference(m_pSprite);



	if (FAILED(HighLevel_Prototype_Caller()))
	{
		PRINT_LOG(L"Error", L"HighLevel_Prototype_Caller Prototype Failed");
		return E_FAIL;
	}

	if (FAILED(m_pManagement->Setup_CurrentScene((_uint)ESceneType::Logo, CLogo::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed To Setup_CurrentScene To Logo (MainApp.cpp)");
		return E_FAIL;
	}

	if (FAILED(Setup_DefaultSetting()))
	{
		PRINT_LOG(L"Error", L"Failed To Setup_DefaultSetting");
		return E_FAIL;
	}

	return S_OK;
}

_uint CMainApp::Update_MainApp()
{
	_uint iEvent = m_pManagement->Update_Engine();
	if (CHANGE_SCENE == iEvent)
		return NO_EVENT;

	if (FAILED(m_pManagement->Render_Engine(g_hWnd)))
		return NO_EVENT;	

	//pCube->Render_VIBuffer();

	//�����
	_CrtSetBreakAlloc(606);

	return NO_EVENT;
}

HRESULT CMainApp::Setup_DefaultSetting()
{
	/* ���� off */
	if (FAILED(m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE)))
		return E_FAIL;

	/*
	D3DRS_CULLMODE: �ĸ� �߷����� �ɼ�
	D3DCULL_CCW(Counter Clock Wise): �ݽð������ ���� �߷�����. ����Ʈ��.
	D3DCULL_CW(Clock Wise): �ð������ ���� �߷�����.
	D3DCULL_NONE: �ƹ��͵� �߷����� ����.
	*/
	if (FAILED(m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE)))
		return E_FAIL;
	//---�ӽ�
		/* ����� ����� ��ġ���� ���� */
	//_float4x4 matView;
	//D3DXMatrixLookAtLH(
	//	&matView, /* ����� ��ȯ */
	//	&_float3(0.f, 5.f, -5.f), /* Eye: ī�޶�(������)�� ����� ��ġ */
	//	&_float3(0.f, 0.f, 0.f), /* At: ī�޶�(������)�� �ٶ� ����� ��ġ */
	//	&_float3(0.f, 1.f, 0.f) /* Up: ������� ������. ī�޶��� ������� ����� ���� �ʿ��� ��� */);

	//if (FAILED(m_pDevice->SetTransform(D3DTS_VIEW, &matView)))
	//	return E_FAIL;

	///* ������� ����� ��ġ���� ���� */
	//_float4x4 matProj;
	//D3DXMatrixPerspectiveFovLH(
	//	&matProj, /* ����������� ��ȯ */
	//	D3DXToRadian(90.f), /* FovY: Y���� Field Of View (Y�� �þ߰�) */
	//	(_float)WINCX / WINCY, /* Aspect: ��Ⱦ�� */
	//	1.f, /* Near Z: �����ڿ� ���� ����� ����� Z�Ÿ� */
	//	1000.f /* Far Z: �����ڿ� ���� �� ����� Z�Ÿ� */);

	//if (FAILED(m_pDevice->SetTransform(D3DTS_PROJECTION, &matProj)))
	//	return E_FAIL;

	//---


	return S_OK;
}

#pragma region prototype 

HRESULT CMainApp::HighLevel_Prototype_Caller()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	if (FAILED(GameObject_Prototype_Caller(pManagement)))
	{
		PRINT_LOG(L"CMainApp - Han", L"���� ������Ʈ ������Ÿ�� ����");
		return E_FAIL;
	}

	if (FAILED(Component_Prototype_Caller(pManagement)))
	{
		PRINT_LOG(L"CMainApp - Han", L"������Ʈ ������Ÿ�� ����");
		return E_FAIL;
	}

	if (FAILED(Texture_Prototype_Caller(pManagement)))
	{
		PRINT_LOG(L"CMainApp - Han", L"�ؽ��� ������Ÿ�� ����");
		return E_FAIL;
	}

	if (FAILED(Etc_Prototype_Caller(pManagement)))
	{
		PRINT_LOG(L"CMainApp - Han", L"Etc ������Ÿ�� ����");
		return E_FAIL;
	}


	return S_OK;
}

#pragma endregion
HRESULT CMainApp::GameObject_Prototype_Caller(CManagement* _pManagement)
{
	if (FAILED(Monster_Texture_Prototype_JDY(_pManagement)))
	{
		return E_FAIL;
	}

	if (FAILED(Player_Prototype(_pManagement)))
	{
		return E_FAIL;
	}
	if (FAILED(Monster_Prototype(_pManagement)))
	{
		return E_FAIL;
	}
	if (FAILED(BattlePlate_Prototype(_pManagement)))
	{
		return E_FAIL;
	}

	if (FAILED(Camera_Prototype(_pManagement)))
	{
		return E_FAIL;
	}
	if (FAILED(UI_Prototype(_pManagement)))
	{
		return E_FAIL;
	}

	if (FAILED(BATTLE_UI_Prototype(_pManagement)))
	{
		return E_FAIL;
	}

	if (FAILED(Terrain_Prototype(_pManagement)))
	{
		return E_FAIL;
	}

	if (FAILED(SkyBox_Prototype(_pManagement)))
	{
		return E_FAIL;
	}
	
	if (FAILED(Item_Prototype(_pManagement)))
	{
		return E_FAIL;
	}

	//if (FAILED(Effect_Prototype(_pManagement)))
	//{
	//	return E_FAIL;
	//}

	if (FAILED(MiddleBossABullet_Prototype(_pManagement))) 
	{
		return E_FAIL;
	}


	return S_OK;
}
HRESULT CMainApp::Component_Prototype_Caller(CManagement* _pManagement)
{
	if (FAILED(Transform_Prototype(_pManagement)))
	{
		return E_FAIL;
	}

	if (FAILED(VIBuffer_Prototype(_pManagement)))
	{
		return E_FAIL;
	}

	if (FAILED(Font_Prototype(_pManagement)))
	{
		return E_FAIL;
	} 

	if (FAILED(Status_Comp_Prototype(_pManagement)))
	{
		return E_FAIL;
	}



	return S_OK;
}

HRESULT CMainApp::Texture_Prototype_Caller(CManagement* _pManagement)
{
	if (FAILED(Player_Texture_Prototype(_pManagement)))
	{
		return E_FAIL;
	}
	if (FAILED(Player_2DTexture_Prototype(_pManagement)))
	{
		return E_FAIL;
	}

	if (FAILED(Monster_Texture_Prototype(_pManagement)))
	{
		return E_FAIL;
	}

	if (FAILED(BattlePlate_Texture_Prototype(_pManagement)))
	{
		return E_FAIL;
	}

	if (FAILED(UI_Texture_Prototype(_pManagement)))
	{
		return E_FAIL;
	}

	if (FAILED(Terrain_Texture_Prototype(_pManagement)))
	{
		return E_FAIL;
	}

	if (FAILED(SkyBox_Texture_Prototype(_pManagement)))
	{
		return E_FAIL;
	}

	if (FAILED(Item_Texture_Prototype(_pManagement)))
	{
		return E_FAIL;
	}

	if (FAILED(BackGround_Texture_Prototype(_pManagement)))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CMainApp::Etc_Prototype_Caller(CManagement * _pManagement)
{
	if (FAILED(Particle_Prototype(_pManagement)))
	{
		return E_FAIL;
	}

	if (FAILED(Effecter_Prototype(_pManagement)))
	{
		return E_FAIL;
	}

	return S_OK;
}

CMainApp* CMainApp::Create()
{
	CMainApp* pInstance = new CMainApp;
	if (FAILED(pInstance->Ready_MainApp()))
	{
		PRINT_LOG(L"Error", L"Failed To Create MainApp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainApp::Free()
{
	Safe_Release(m_pSprite);
	Safe_Release(m_pFont);
	Safe_Release(m_pDevice);
	Safe_Release(m_pManagement);



}
