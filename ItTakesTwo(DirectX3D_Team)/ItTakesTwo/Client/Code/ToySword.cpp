#include "stdafx.h"
#include "ToySword.h"
#include "May_Castle.h"
#include "MainCamera.h"
#include "CollisionMgr.h"
#include "Export_Function.h"


CToySword::CToySword(LPDIRECT3DDEVICE9 pGraphicDev)
	: CPlayer(pGraphicDev), m_fDeltaTime(0.f)
{

}

CToySword::~CToySword(void)
{

}

HRESULT CToySword::Ready_Object(void* pArg)
{
	m_fColDis = 1.0f;

	// 콜라이더
	ZeroMemory(&m_tColInfo, sizeof(SPHERE_INFO));
	m_tColInfo.fRadius = m_fColDis;

	FAILED_CHECK_RETURN(Add_Component(pArg), E_FAIL);

	m_tShader.m_fRimDist = 1.f;
	m_tShader.m_iRimPower = 100;
	m_tShader.m_vRimColor = _vec4(0.8f, 0.7f, 0.3f, 1.f);

	m_vColor = { 255.f, 255.f, 255.f, 255.f };

	return S_OK;
}

Engine::_int CToySword::Update_Object(const _float& fTimeDelta)
{
	m_fDeltaTime += fTimeDelta;

	_vec3 vPos;
	vPos = m_pTransformCom->Get_Position();



	if (m_bStart)
	{
		m_bStart = false;
		return 0;
	}

	if (m_vColor.x <= 0.f)
		m_bColor = true;
	else if (m_vColor.x >= 255.f)
		m_bColor = false;

	if (m_bColor)
	{
		++m_vColor.x;
		++m_vColor.y;
		++m_vColor.z;
	}
	else
	{
		--m_vColor.x;
		--m_vColor.y;
		--m_vColor.z;
	}



	if (nullptr == m_pParentBoneMatrix)
	{
		CMayCastle* pPlayer = (CMayCastle*)Engine::Get_GameObject(L"GameObject", L"May");
		Engine::CDynamicMesh*	pPlayerMeshCom = (CDynamicMesh*)pPlayer->Get_Component(L"Com_Mesh", ID_STATIC);
		NULL_CHECK_RETURN(pPlayerMeshCom, 0);

		const Engine::D3DXFRAME_DERIVED* pFrame = pPlayerMeshCom->Get_FrameByName("RightAttach");

		m_pParentBoneMatrix = &pFrame->CombinedTranformationMatrix;

		Engine::CTransform*		pSpongeBobTransformCom = dynamic_cast<CTransform*>(pPlayer->Get_Component(L"Com_Transform", ID_DYNAMIC));

		m_pParentWorldMatrix = pSpongeBobTransformCom->Get_WorldMatrix();
	}

	Engine::CGameObject::Update_Object(fTimeDelta);

	m_pTransformCom->Set_ParentMatrix((*m_pParentBoneMatrix * *m_pParentWorldMatrix));


	//m_bColl = Collision_ToObject(L"GameLogic", L"Player");

	CCollisionMgr::GetInstance()->Add_PlayerWeaponList(this);
	if (m_bRenderState)
	{
		Engine::Add_RenderGroup(Engine::RENDER_ALPHA, this);
		Engine::Add_RenderGroup(Engine::RENDER_SHADOW, this);
	}

	return 0;
}

_int CToySword::LateUpdate_Object(const _float & fTimeDelta)
{
	return _int();
}

void CToySword::Render_Object(const _int& iIndex)
{
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect), );

	_uint	iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);		// 1인자 : 현재 쉐이더 파일이 갖고 있는 pass의 최대 개수, 2인자 : 시작하는 방식(default)

	m_pMeshCom->Render_Meshes(pEffect, 2);

	pEffect->End();

	Safe_Release(pEffect);

	CMainCamera* pCamera = (CMainCamera*)Engine::Get_GameObject(L"GameObject", L"MainCamera");
	pCamera->Update_View_Proj();

	//Spine2
	_float4x4 matWorld;
	D3DXMatrixTranslation(&matWorld, 0.0f, 80.0f, 0.0f);

	m_tColInfo.matWorld = matWorld * *m_pTransformCom->Get_WorldMatrix();
	m_pColliderCom->Render_Collider(m_tColInfo.matWorld, false);
}

void CToySword::Render_Shadow(_vec3 * vEye, _vec3 * vAt, const _int & iIndex)
{
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	_matrix matWorld;
	m_pTransformCom->Get_WorldMatrix(&matWorld);
	pEffect->SetMatrix("g_matWorld", &matWorld);

	_matrix matView, matInv;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matInv, nullptr, &matView);

	CMainCamera* pCamera = (CMainCamera*)Engine::Get_GameObject(L"GameObject", L"MainCamera");
	*vEye = pCamera->Get_CameraPos();
	*vAt = pCamera->Get_CameraAt();

	_vec3		vLightEye = _vec3(4.f, 0.f, -4.f);

	_matrix		LightViewMatrix, LightProjMatrix;

	D3DXMatrixLookAtLH(&LightViewMatrix, &(*vEye + vLightEye), &*vAt, &vUP);
	//D3DXMatrixLookAtLH(&LightViewMatrix, &vLightEye, &vLightAt, &vUP);
	pEffect->SetMatrix("g_matView", &LightViewMatrix);

	D3DXMatrixPerspectiveFovLH(&LightProjMatrix, D3DXToRadian(60.f), _float(WINCX) / WINCY, 0.1f, 1000.f);
	pEffect->SetMatrix("g_matProj", &LightProjMatrix);

	_uint	iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);		// 1인자 : 현재 쉐이더 파일이 갖고 있는 pass의 최대 개수, 2인자 : 시작하는 방식(default)
										//pEffect->BeginPass(0);
	m_pMeshCom->Render_Meshes(pEffect, 7);

	pEffect->End();

	Safe_Release(pEffect);
}

CToySword* CToySword::Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg)
{
	CToySword*	pInstance = new CToySword(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CToySword::Free(void)
{
	CPlayer::Free();
}

HRESULT CToySword::Add_Component(void* pArg)
{

	Engine::CComponent*		pComponent = nullptr;
	OBJINFO tTemp;
	if (pArg)
	{
		memcpy(&tTemp, pArg, sizeof(OBJINFO));
	}
	// StaticMesh
	pComponent = m_pMeshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(RESOURCE_PLAYER, L"ToySword"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Mesh", pComponent);

	// Texture
	pComponent = m_pEffectCom = dynamic_cast<Engine::CTexture*>(Engine::Clone_Resource(tTemp.eSceneID, L"SwordEffect"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Effect", pComponent);

	// Transform
	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	// Calculator
	pComponent = m_pCalculatorCom = dynamic_cast<Engine::CCalculator*>(Engine::Clone_Prototype(L"Proto_Calculator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Calculator", pComponent);

	// Collider
	pComponent = m_pColliderCom = Engine::CCollider::Create(m_pGraphicDev, 1.0f, _vec4(100.f, 255.f, 0.0f, 0.f));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Collider", pComponent);

	// Shader
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_All"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);

	//m_pMeshCom
	return S_OK;
}

HRESULT CToySword::SetUp_ConstantTable(LPD3DXEFFECT & pEffect)
{
	_matrix			matWorld, matView, matProj, matInv;

	m_pTransformCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matInv, nullptr, &matView);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	pEffect->SetFloat("g_fRimDist", m_tShader.m_fRimDist);
	pEffect->SetInt("g_iRimPower", m_tShader.m_iRimPower);
	pEffect->SetVector("g_vRimColor", &m_tShader.m_vRimColor);

	pEffect->SetVector("g_vCamPos", &_vec4(matInv._41, matInv._42, matInv._43, 1.f));
	pEffect->SetVector("g_vColor", &m_vColor);
	m_pEffectCom->Set_Texture(pEffect, "g_Add1Texture", 0);
	m_pEffectCom->Set_Texture(pEffect, "g_Add2Texture", 4);
	pEffect->SetFloat("g_DeltaTime", m_fDeltaTime);

	pEffect->SetFloat("g_fAmount", 0.f);
	pEffect->SetFloat("g_fMaterialIndex", 0.f);
	pEffect->CommitChanges();

	return S_OK;
}
