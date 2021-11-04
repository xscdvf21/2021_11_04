#include "stdafx.h"
#include "..\Headers\ManagementTool.h"


IMPLEMENT_SINGLETON(CManagementTool)
//추가되는 프레임 워크에 따라 오류 발생 가능. 프레임웤 작업 후 수정.
CManagementTool::CManagementTool()
	: m_pDeviceManager(CGraphicDevice::Get_Instance())
	//, m_pRenderer(CRenderer::Get_Instance())
{
	Safe_AddReference(m_pDeviceManager);
	//Safe_AddReference(m_pRenderer);
}

HRESULT CManagementTool::Ready_Engine(HWND hWnd, int iWinCX, int iWinCY, EDisplayMode eDisplayMode, _uint iSceneCount)
{
	if (nullptr == hWnd ||
		nullptr == m_pDeviceManager)
	{
		return E_FAIL;
	}

	if (FAILED(m_pDeviceManager->Ready_GraphicDevice(hWnd, iWinCX, iWinCY, eDisplayMode)))
	{
		PRINT_LOG(L"ManagementTool - err", L"그래픽 디바이스 초기 셋팅 실패.");
		return E_FAIL;
	}

	m_pDevice = m_pDeviceManager->Get_Device();

	/*if (FAILED(m_pRenderer->Set_Device(m_pDeviceManager->Get_Device())))
	{
		PRINT_LOG(L"ManagementTool - err", L"그래픽 디바이스를 얻지 못 함.");
		return E_FAIL;
	}*/

	CTimeMgr::Get_Instance()->Ready_Time_Manager();

	CGameObjMgr::Get_Instance()->Ready_GameObject_Manager();

	CComponentMgr::Get_Instance()->Ready_Component_Manager();


	if (FAILED(Ready_Camera()))
	{
		PRINT_LOG(L"CManagementTool", L"카메라 Ready 오류");
		return E_FAIL;
	}

	return S_OK;
}

_uint CManagementTool::Update_Engine()
{
	_float fDeltaTime = CTimeMgr::Get_Instance()->Update_Time_Manager();

	CGameObjMgr::Get_Instance()->Update_GameObject(fDeltaTime);
	

	CGameObjMgr::Get_Instance()->LateUpdate_GameObject(fDeltaTime);

	Update_Camera(fDeltaTime);
	
	return _uint();
}


HRESULT CManagementTool::Render_Engine(HWND hWnd)
{

#pragma region 예비코드 - Renderer
	//if (nullptr == m_pRenderer)
	//	return E_FAIL;

	//if (FAILED(m_pRenderer->Render_RenderList(hWnd)))
	//{
	//	PRINT_LOG(L"ManagementTool - err", L"CRenderer 초기 셋팅 실패 ");
	//	return E_FAIL;
	//}
#pragma endregion

	if (FAILED(m_pDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,
		D3DCOLOR_ARGB(255, 0, 0, 255), 1.f, 0)))
		return E_FAIL;

	m_pDevice->BeginScene();
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	CGameObjMgr::Get_Instance()->Render_GameObject();//렌더

	if (FAILED(m_pDevice->EndScene()))
		return E_FAIL;

	if (FAILED(m_pDevice->Present(nullptr, nullptr, g_hWndTool, nullptr)))
		return E_FAIL;

	return S_OK;
}

_Device CManagementTool::Get_Device()
{
	if (nullptr == m_pDeviceManager)
		return nullptr;
	return m_pDeviceManager->Get_Device();
}

CGameObject * CManagementTool::Get_GameObject(_uint iIndex)
{
	return nullptr;
}

CComponent * CManagementTool::Get_Component(const wstring & ComponentTag, _uint iIndex)
{
	return nullptr;
}

HRESULT CManagementTool::Add_GameObject_Prototype(const wstring & PrototypeTag, CGameObj * pPrototype)
{
	if (nullptr == CGameObjMgr::Get_Instance())
		return E_FAIL;

	return CGameObjMgr::Get_Instance()->Add_GameObject_Prototype(PrototypeTag, pPrototype);
}

HRESULT CManagementTool::Clone_GameObject_ToLayer(const wstring & PrototypeTag, void * pArg)
{
	if (nullptr == CGameObjMgr::Get_Instance())
		return E_FAIL;

	return CGameObjMgr::Get_Instance()->Clone_GameObject_ToLayer(PrototypeTag, pArg);
}

CGameObj* CManagementTool::Get_Clone_GameObject(const wstring & PrototypeTag, void * pArg)
{
	if (nullptr == CGameObjMgr::Get_Instance())
		return nullptr;

	return CGameObjMgr::Get_Instance()->Get_Clone_GameObject(PrototypeTag, pArg);
}


HRESULT CManagementTool::Add_Component_Prototype(const wstring & PrototypeTag, CComponentTool * pPrototype)
{
	if (nullptr == CComponentMgr::Get_Instance())
		return E_FAIL;

	return CComponentMgr::Get_Instance()->Add_Component_Prototype( PrototypeTag, pPrototype);
}

CComponentTool * CManagementTool::Clone_Component(const wstring & PrototypeTag, void * pArg)
{
	if (nullptr == CComponentMgr::Get_Instance())
		return nullptr;

	return CComponentMgr::Get_Instance()->Clone_Component(PrototypeTag, pArg);
}

void CManagementTool::Free()
{
	//Safe_Release(m_pRenderer);
	Safe_Release(m_pDeviceManager);
}

void CManagementTool::Release_Engine()
{
	if (CManagementTool::Destroy_Instance())
		PRINT_LOG(L"Warning", L"Failed To Release CManagementTool");
	//if (CRenderer::Destroy_Instance())
		//PRINT_LOG(L"Warning", L"Failed To Release CRenderer");
	if (CGraphicDevice::Destroy_Instance())
		PRINT_LOG(L"Warning", L"Failed To Release CGraphicDevice");
}


/////////////////////////////////////////////////////////////////////////////////////

#pragma region 카메라를 위한 함수.
HRESULT CManagementTool::Ready_Camera()
{
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixIdentity(&m_matProj);

	D3DXMatrixPerspectiveFovLH(&m_matProj, D3DXToRadian(60.f), (_float) 709 / 709, 1.f, 500.f);

	m_vEye = _float3(0.f, 10.f, -10.f);
	m_vAt =  _float3(0.f, 0.f, 0.f);
	m_vUp = _float3(0.f, 1.f, 0.f);

	D3DXMatrixLookAtLH(&m_matView
		, &m_vEye
		, &m_vAt
		, &m_vUp);

	m_pDevice->SetTransform(D3DTS_VIEW, &m_matView);
	m_pDevice->SetTransform(D3DTS_PROJECTION, &m_matProj);


	

	return S_OK;
}

HRESULT CManagementTool::Update_Camera(_float fDeltaTime)
{
	_float4x4 matCam;
	D3DXMatrixInverse(&matCam, NULL, &m_matView);
	float fAngleSpeed = 90.f;

	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{

		if (GetAsyncKeyState('W') & 0x8000)                       
		{
			_float3	vLook;

			memcpy(&vLook, &matCam.m[2][0], sizeof(_vec3));
			D3DXVec3Normalize(&vLook, &vLook);

			_float3 vPos = vLook*10.f * fDeltaTime;

			m_vEye += vPos;
			m_vAt += vPos;
		}

		if (GetAsyncKeyState('S') & 0x8000)
		{
			_float3	vLook;

			memcpy(&vLook, &matCam.m[2][0], sizeof(_vec3));
			D3DXVec3Normalize(&vLook, &vLook);

			_float3 vPos = vLook*10.f * fDeltaTime ;

			m_vEye -= vPos;
			m_vAt -= vPos;
		}

		if (GetAsyncKeyState('D') & 0x8000)
		{
			_float3	vRight;

			memcpy(&vRight, &matCam.m[0][0], sizeof(_vec3));
			D3DXVec3Normalize(&vRight, &vRight);

			_float3 vPos = vRight*10.f * fDeltaTime;

			m_vEye += vPos;
			m_vAt += vPos;
		}

		if (GetAsyncKeyState('A') & 0x8000)
		{
			_float3	vRight;

			memcpy(&vRight, &matCam.m[0][0], sizeof(_vec3));
			D3DXVec3Normalize(&vRight, &vRight);

			_float3 vPos = vRight*10.f * fDeltaTime;

			m_vEye -= vPos;
			m_vAt -= vPos;
		}

		if (GetAsyncKeyState(VK_UP) & 0x8000)
		{
			_float3	vLook = m_vAt - m_vEye;
			_float3	vUp = _float3(0.f, 1.f, 0.f);

			D3DXVec3Normalize(&vLook, &vLook);
			_float3 vRight;
			D3DXVec3Cross(&vRight,&vUp, &vLook);
			

			_float4x4 matRotAxis;
			D3DXMatrixRotationAxis(&matRotAxis, &vRight, D3DXToRadian(-fAngleSpeed*fDeltaTime));
			D3DXVec3TransformNormal(&vLook, &vLook, &matRotAxis);

			m_vAt = vLook + m_vEye;
		}

		if (GetAsyncKeyState(VK_DOWN) & 0x8000)
		{
			_float3	vLook = m_vAt - m_vEye;
			_float3	vUp = _float3(0.f, 1.f, 0.f);

			D3DXVec3Normalize(&vLook, &vLook);
			_float3 vRight;
			D3DXVec3Cross(&vRight, &vUp, &vLook);


			_float4x4 matRotAxis;
			D3DXMatrixRotationAxis(&matRotAxis, &vRight, D3DXToRadian(fAngleSpeed*fDeltaTime));
			D3DXVec3TransformNormal(&vLook, &vLook, &matRotAxis);

			m_vAt = vLook + m_vEye;
		}


		if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		{
			_float3	vLook = m_vAt - m_vEye;
			_float3	vUp = _float3(0.f, 1.f, 0.f);

			_float4x4 matRotAxis;
			D3DXMatrixRotationAxis(&matRotAxis, &vUp, D3DXToRadian(fAngleSpeed*fDeltaTime));
			D3DXVec3TransformNormal(&vLook, &vLook, &matRotAxis);

			m_vAt = vLook + m_vEye;
		}

		if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		{
			_float3	vLook = m_vAt - m_vEye;
			_float3	vUp = _float3(0.f, 1.f, 0.f);

			_float4x4 matRotAxis;
			D3DXMatrixRotationAxis(&matRotAxis, &vUp, D3DXToRadian(-fAngleSpeed*fDeltaTime));
			D3DXVec3TransformNormal(&vLook, &vLook, &matRotAxis);

			m_vAt = vLook + m_vEye;
		}

		if (GetAsyncKeyState('E'))
		{
			_float3	vUp;

			memcpy(&vUp, &matCam.m[1][0], sizeof(_vec3));
			D3DXVec3Normalize(&vUp, &vUp);

			_float3 vPos = vUp*10.f * fDeltaTime;

			m_vEye += vPos;
			m_vAt += vPos;
		}
		if (GetAsyncKeyState('Q'))
		{
			_float3	vUp;

			memcpy(&vUp, &matCam.m[1][0], sizeof(_vec3));
			D3DXVec3Normalize(&vUp, &vUp);

			_float3 vPos = vUp*10.f * fDeltaTime;

			m_vEye -= vPos;
			m_vAt -= vPos;
		}



	}

	//카메라 eye 구하기
	
	D3DXMatrixLookAtLH(&m_matView
		, &m_vEye
		, &m_vAt
		, &m_vUp);

	//m_pDevice->SetTransform(D3DTS_WORLD, &m_matCamera);
	m_pDevice->SetTransform(D3DTS_VIEW, &m_matView);

	return S_OK;
}
#pragma endregion