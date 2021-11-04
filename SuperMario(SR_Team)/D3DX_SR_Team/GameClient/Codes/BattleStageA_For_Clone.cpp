#include "framework.h"
#include "BattleStageA.h"


HRESULT CBattleStageA::Add_Player_Layer(const wstring& LayerTag)
{
	auto pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	if (FAILED(pManagement->Clone_GameObject_ToLayer(
		(_uint)ESceneType::Static,
		L"GameObject_Player",
		(_uint)ESceneType::Battle,
		LayerTag)))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CBattleStageA::Add_Camera_Layer(const wstring& LayerTag)
{
	auto pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CBattleCamera::CAMERA_DESC CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CBattleCamera::CAMERA_DESC));
	CameraDesc.vEye = _float3(0.f, 10.f, -10.f);
	CameraDesc.vUp = _float3(0.f, 1.f, 0.f);
	CameraDesc.fFovY = D3DXToRadian(90.f);
	CameraDesc.fAspect = (_float)WINCX / WINCY;
	CameraDesc.fZNear = 1.f; /* ������ȯ ��(z���������)�� 0�� �Ǿ���ϰ� */
	CameraDesc.fZFar = 500.f; /* ������ȯ ��(z���������)�� 1�� �Ǿ���� */

	if (FAILED(pManagement->Clone_GameObject_ToLayer(
		(_uint)ESceneType::Static,
		L"GameObject_BattleCamera",
		(_uint)ESceneType::Battle,
		LayerTag,
		&CameraDesc)))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CBattleStageA::Add_SkyBox_Layer(const wstring& LayerTag)
{
	auto pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
	{
		return E_FAIL;
	}

	if (FAILED(pManagement->Clone_GameObject_ToLayer(
		(_uint)ESceneType::Static,
		L"GameObject_Skybox_Battle",
		(_uint)ESceneType::Battle,
		LayerTag)))
	{
		return E_FAIL;
	}


	return S_OK;
}

HRESULT CBattleStageA::Add_Monster_Layer(const wstring& LayerTag)
{
	auto pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;


	return S_OK;
}
HRESULT CBattleStageA::Add_Monster_Object_Layer(const wstring& LayerTag)
{
	auto pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;


	if (FAILED(pManagement->Clone_GameObject_ToLayer(
		(_uint)ESceneType::Static,
		L"GameObject_BattlePlate_DropAttack_Goomba",
		(_uint)ESceneType::Battle,
		LayerTag)))
	{
		PRINT_LOG(L"BattleStageA", L"���������� ���ϱ��� ���� ����");
		return E_FAIL;
	}


	return S_OK;
}

HRESULT CBattleStageA::Add_BattlePlate_Layer(const wstring& LayerTag)
{
	auto pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	if (FAILED(pManagement->Clone_GameObject_ToLayer(
		(_uint)ESceneType::Static,
		L"GameObject_BattlePlate",
		(_uint)ESceneType::Battle,
		LayerTag)))
	{
		PRINT_LOG(L"BattleStageA", L"��Ʋ �÷���Ʈ ���� ����");
		return E_FAIL;
	}

	//�ð����� �غ�����
	if (FAILED(pManagement->Clone_GameObject_ToLayer(
		(_uint)ESceneType::Static,
		L"GameObject_BattlePlate_Circle",
		(_uint)ESceneType::Battle,
		LayerTag)))
	{
		PRINT_LOG(L"BattleStageA", L"��Ʋ �÷���Ʈ ���� ����Ʈ ����");
		return E_FAIL;
	}

	if (FAILED(pManagement->Clone_GameObject_ToLayer(
		(_uint)ESceneType::Static,
		L"GameObject_BattlePlate_Slide",
		(_uint)ESceneType::Battle,
		LayerTag)))
	{
		PRINT_LOG(L"BattleStageA", L"��Ʋ �÷���Ʈ �����̵� ����Ʈ ����");
		return E_FAIL;
	}

	if (FAILED(pManagement->Clone_GameObject_ToLayer(
		(_uint)ESceneType::Static,
		L"GameObject_BattlePlate_Select_Slide",
		(_uint)ESceneType::Battle,
		LayerTag)))
	{
		PRINT_LOG(L"BattleStageA", L"��Ʋ �÷���Ʈ ���� �����̵� ����Ʈ ����");
		return E_FAIL;
	}
	if (FAILED(pManagement->Clone_GameObject_ToLayer(
		(_uint)ESceneType::Static,
		L"GameObject_BattlePlate_Select_Cube",
		(_uint)ESceneType::Battle,
		LayerTag)))
	{
		PRINT_LOG(L"BattleStageA", L"��Ʋ �÷���Ʈ ���� �׸� ����Ʈ ����");
		return E_FAIL;
	}


	_uint test = sizeof(CGameObject);

	//////���� �߰���Ʈ. ���߿� ���̾� �߰��ؼ� �ٲٱ�

	//static_cast<CBattlePlate*>(pManagement->Get_GameObject((_uint)ESceneType::Battle, LayerTag, 0))->Add_Monster(L"GameObject_TestMonster", 2, 11);
	//static_cast<CBattlePlate*>(pManagement->Get_GameObject((_uint)ESceneType::Battle, LayerTag, 0))->Add_Monster(L"GameObject_TestMonster", 3, 4);
	//static_cast<CBattlePlate*>(pManagement->Get_GameObject((_uint)ESceneType::Battle, LayerTag, 0))->Add_Monster(L"GameObject_TestMonster", 1, 6);
	//static_cast<CBattlePlate*>(pManagement->Get_GameObject((_uint)ESceneType::Battle, LayerTag, 0))->Add_Monster(L"GameObject_TestMonster", 0, 9);

#pragma region �ּ�

	return S_OK;
}

HRESULT CBattleStageA::Add_Battle_UI(const wstring& LayerTag)
{
	auto pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	if (FAILED(pManagement->Clone_GameObject_ToLayer(
		(_uint)ESceneType::Static,
		L"GameObject_BattleUI",
		(_uint)ESceneType::Battle,
		LayerTag)))
	{
		return E_FAIL;
	}

	return S_OK;
}