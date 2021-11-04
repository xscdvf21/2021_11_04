#include "framework.h"
#include "..\Headers\Monster.h"
#include "TestCamera.h"
#include "BattleCamera.h"

CMonster::CMonster(LPDIRECT3DDEVICE9 pDevice)
	:CSuperMonster(pDevice)
{
}

CMonster::CMonster(const CMonster & other)
	: CSuperMonster(other)
{
}

HRESULT CMonster::Ready_GameObject_Prototype()
{
	if (FAILED(CGameObject::Ready_GameObject_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonster::Ready_GameObject(void * pArg /*= nullptr*/)
{
	if (FAILED(CGameObject::Ready_GameObject(pArg)))
		return E_FAIL;

	m_pTransformCom->Initialize_vPos(_float3(1.f, 1.f, 1.f));
	m_pTransformCom->Initialize_vScale(_float3(100.f, 100.f, 1.f));

	//여기서 처음 추가하는듯.
	if (FAILED(Add_Component()))
		return E_FAIL;


	D3DXCreateFont(m_pDevice, 25, 25, 1000, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, L"맑은고딕", &m_pFont);

	return S_OK;
}

_uint CMonster::Update_GameObject(_float fDeltaTime)
{
	CGameObject::Update_GameObject(fDeltaTime);

	//움직임 체크
	if (FAILED(Movement(fDeltaTime)))
		return 0;

	return _uint();
}

_uint CMonster::LateUpdate_GameObject(_float fDeltaTime)
{
	//여기서 랜더 리스트에 추가하는구나.
	auto pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return 0;

	if (FAILED(pManagement->Add_RenderList(ERenderPriority::UI, this)))
		return 0;

	return m_pTransformCom->Update_Transform();
}

HRESULT CMonster::Render_GameObject()
{
	//if (FAILED(IsBillboarding()))
	//	return E_FAIL;

	auto pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return 0;

	CTestCamera*	pCamera = dynamic_cast<CTestCamera*>(pManagement->Get_GameObject((_uint)ESceneType::TestStage, L"Layer_Camera"));
	
	TCHAR szBuf[128] = L"";			//테스트 문구 수정해주세요
	swprintf_s(szBuf, L"HP사진");
	i++;


	//사각형 영역 설정 , -1은 옵션에 NOCLIP을 넣었기 때문에 , 값을 주지 않은것이다

	RECT rc = { 100, 100, -1, -1 };

	m_pFont->DrawText(NULL, szBuf, -1, &rc, DT_LEFT | DT_NOCLIP, 0xffffff00);

	pCamera->Set_Ortho_For_UI(_float3(100.f, 100.f, 1.f), _float3(-350.f, 200.f, 1.f));

	//_mat	matWorld = pCamera->Get_CameraDesc().matView;

	///*m_pDevice->GetTransform(D3DTS_VIEW, &matWorld);*/
	//D3DXMatrixInverse(&matWorld, NULL, &matWorld);

	//_vec3 temp = { matWorld._31, matWorld._32, matWorld._33 };

	////CStageUI*	pStageUI = dynamic_cast<CStageUI*>(pManagement->Get_GameObject((_uint)ESceneType::Stage, L"Layer_UI"));
	////m_pTransformCom->Set_Position(temp);
	//matWorld._41 = (float)(int)(matWorld._41 + temp.x);
	//matWorld._42 = (float)(int)(matWorld._42 + temp.y);
	//matWorld._43 = (float)(int)(matWorld._43 + temp.z);

	////최종 행렬 세팅
	//m_pTransformCom->Set_WorldMatrix(matWorld);



	if (FAILED(m_pTransformCom->Set_Transform()))
		return E_FAIL;

	if (FAILED(CGameObject::Render_GameObject()))
		return E_FAIL;
	

	if (FAILED(m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE)))
		return E_FAIL;

	if (FAILED(m_pTextureCom[m_eState]->Set_Texture(m_tFrame.iSpriteStart)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
		return E_FAIL;

	if (FAILED(m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE)))
		return E_FAIL;



	


	return S_OK;
}

HRESULT CMonster::Add_Component()
{

	//Engine에 만들어서 수정해야함. //플레이어랑 같은거 써서터지는듯.

	//For. Com.VIBuffer
	if (FAILED(CGameObject::Add_Component(
		(_uint)ESceneType::Static,
		L"Component_VIBuffer_Rect",
		L"Com_VIBuffer",
		(CComponent**)&m_pVIBufferCom)))
			return E_FAIL;

	/*For. Com_Texture*/
	//if (FAILED(CGameObject::Add_Component(
	//	(_uint)ESceneType::Static,
	//	L"Component_Texture_Monster",
	//	L"Com_Texture",
	//	(CComponent**)&m_pTextureCom
	//)))
	//	return E_FAIL;





	return S_OK;
}

HRESULT CMonster::Movement(_float fDeltaTime)
{
	return S_OK;
}

HRESULT CMonster::IsOnTerrain()
{
	PRINT_LOG(L"Error", L"Not Setted Player's SetTransform");
	return E_FAIL;
}

HRESULT CMonster::Set_Transform()
{
	PRINT_LOG(L"Error", L"Not Setted Player's SetTransform");
	return E_FAIL;
}

HRESULT CMonster::IsBillboarding()
{

	CManagement*	pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
	{
		return E_FAIL;
	}

	CTestCamera* pTestCamera = (CTestCamera*)pManagement->Get_GameObject((_uint)ESceneType::Stage, L"Layer_Camera");
	if (nullptr == pTestCamera)
	{
		return E_FAIL;
	}

	_float4x4 matView = pTestCamera->Get_CameraDesc().matView;
	
	/*Y축 빌보드*/
	_float4x4 matBillY;
	D3DXMatrixIdentity(&matBillY);

	/*뷰행렬에서 Y축 회전 정보를 얻어온다.*/
	matBillY._11 = matView._11;
	matBillY._13 = matView._13;
	matBillY._31 = matView._31;
	matBillY._33 = matView._33;

	/* 공전의 반대로 자전해야하기 때문에 회전행렬을 역행렬로 만든다. */
	D3DXMatrixInverse(&matBillY, 0, &matBillY);

	_float4x4 matWorld = m_pTransformCom->Get_WorldMatrix();

	/* 위에서 구한 공전행렬의 반대행렬을 자전 자리에 곱해준다. */

	matWorld = matBillY * matWorld;

	m_pTransformCom->Set_WorldMatrix(matWorld); /* 스 * 자 * 이 * 공 * 부 */

	return S_OK;
}

//void CMonster::Set_Position(_float3 _vPos)
//{
//	m_pTransformCom->Set_Position(_vPos);
//	m_pTransformCom->Update_Transform();
//}

CMonster * CMonster::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CMonster* pInstance = new CMonster(pDevice);
	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		PRINT_LOG(L"Error", L"Failed To Create Monster (Cilent - CMonster.cpp)");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CMonster::Clone(void * pArg /*= nullptr*/)
{
	CMonster* pInstance = new CMonster(*this); //복사 생성자 호출.
	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		PRINT_LOG(L"Error", L"Failed To Clone Monster (Cilent - CMonster.cpp)");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMonster::Free()
{
	CSuperMonster::Free();
}
