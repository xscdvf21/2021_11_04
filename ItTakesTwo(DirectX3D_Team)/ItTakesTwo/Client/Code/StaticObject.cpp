#include "stdafx.h"
#include "StaticObject.h"
#include "Export_Function.h"
#include "MainCamera.h"
#include "..\Engine\Resources\Code\PhysXMesh.h"


CStaticObject::CStaticObject(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{

}

CStaticObject::~CStaticObject(void)
{
}

HRESULT CStaticObject::Ready_Object(void* pArg)
{
	FAILED_CHECK_RETURN(Add_Component(pArg), E_FAIL);


	OBJINFO Temp;
	if (pArg)
	{
		memcpy(&Temp, pArg, sizeof(OBJINFO));
	}
	m_pTransformCom->Set_Pos(Temp.vPos);
	m_pTransformCom->Set_Scale(Temp.vScale);
	m_pTransformCom->Set_Angle(Temp.vAngle);
	m_eRender = Temp.eRenderType;
	lstrcpy(m_tagMesh, Temp.tagMesh);

	m_iPass = 0;

	if (!lstrcmp(m_tagMesh, L"Grass"))
		m_iPass = 4;

	_tchar tag[MAX_PATH];
	for (_int i = 1; i <= 100; ++i)
	{
		lstrcpy(tag, L"Grass");
		lstrcat(tag, std::to_wstring(i).c_str());

		if (!lstrcmp(m_tagMesh, tag))
		{
			m_iPass = 4;
			break;
		}
	}



	return S_OK;
}

Engine::_int CStaticObject::Update_Object(const _float& fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	Engine::CGameObject::Update_Object(fTimeDelta);

	if (!lstrcmp(m_tagMesh, L"Grass"))
	{
		_vec3 vPos = m_pTransformCom->Get_Position();
		Compute_ViewZ(&vPos);
		Engine::Add_RenderGroup(Engine::RENDER_UI, this);
		return 0;
	}

	_tchar tag[MAX_PATH];
	for (_int i = 1; i <= 100; ++i)
	{
		lstrcpy(tag, L"Grass");
		lstrcat(tag, std::to_wstring(i).c_str());

		if (!lstrcmp(m_tagMesh, tag))
		{
			_vec3 vPos = m_pTransformCom->Get_Position();
			Compute_ViewZ(&vPos);
			Engine::Add_RenderGroup(Engine::RENDER_UI, this);
			return 0;
		}
	}

	if (m_eRender == Engine::RENDER_NONALPHA)
		Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);
	else if (m_eRender == Engine::RENDER_ALPHA)
		Engine::Add_RenderGroup(Engine::RENDER_ALPHA, this);



	return OBJ_NOEVENT;
}

_int CStaticObject::LateUpdate_Object(const _float & fTimeDelta)
{
	CMainCamera* pCamera = (CMainCamera*)Engine::Get_GameObject(L"GameObject", L"MainCamera");
	_vec3 vCampos = pCamera->Get_CameraPos();

	m_fFog = fabs(D3DXVec3Length(&(vCampos - m_pTransformCom->Get_Position())));

	return _int();
}

void CStaticObject::Render_Object(const _int& iIndex)
{
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect), );

	_uint	iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);		// 1인자 : 현재 쉐이더 파일이 갖고 있는 pass의 최대 개수, 2인자 : 시작하는 방식(default)
	pEffect->BeginPass(m_iPass);

	if (m_eMesh == Engine::ID_DYNAMIC)
		m_pDynamicMeshCom->Render_Meshes(pEffect);
	else if (m_eMesh == Engine::ID_STATIC)
		m_pStaticMeshCom->Render_Meshes(pEffect);

	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);
}

CStaticObject* CStaticObject::Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg)
{
	CStaticObject*	pInstance = new CStaticObject(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CStaticObject::Free(void)
{
	Engine::CGameObject::Free();
}

HRESULT CStaticObject::Add_Component(void* pArg)
{
	Engine::CComponent*		pComponent = nullptr;

	OBJINFO Temp;
	if (pArg)
	{
		memcpy(&Temp, pArg, sizeof(OBJINFO));
	}
	m_eMesh = Temp.eMeshType;

	if (Temp.eMeshType == Engine::ID_STATIC)
	{
		// CStaticMesh
		pComponent = m_pStaticMeshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(Temp.eSceneID, Temp.tagMesh));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Mesh", pComponent);
	}
	else if (Temp.eMeshType == Engine::ID_DYNAMIC)
	{
		// CDynamicMesh
		pComponent = m_pDynamicMeshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Clone_Resource(Temp.eSceneID, Temp.tagMesh));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Mesh", pComponent);
	}

	// Transform
	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	// Shader
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_StaticObject"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);


	//pComponent = m_pPhysMesh = static_cast<CPhysXMesh*>(Engine::Clone_Resource(0, L"test"));
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Phys", pComponent);


	//auto* pPhysics = Engine::Get_Physics();
	//_vec3 vPos = m_pTransformCom->Get_Position();
	//PxTransform tTransform(Temp.vPos.x, Temp.vPos.y, Temp.vPos.z);
	//PxMaterial* pMaterial = pPhysics->createMaterial(0.5f, 0.5f, 0);
	//PxTriangleMeshGeometry pMeshGeo;
	//pMeshGeo.triangleMesh = m_pPhysMesh->Get_PxMesh();
	//pMeshGeo.scale = PxMeshScale(PxVec3(Temp.vScale.x, Temp.vScale.y, Temp.vScale.z));
	//PxShape* pShape = pPhysics->createShape(pMeshGeo, *pMaterial);

	//if (pShape)
	//	m_pRigidActor = PxCreateStatic(*pPhysics, tTransform, *pShape);
	//m_pRigidActor->setName((char*)this);
	return S_OK;
}

HRESULT CStaticObject::SetUp_Material(void)
{
	D3DMATERIAL9			tMtrlInfo;
	ZeroMemory(&tMtrlInfo, sizeof(tMtrlInfo));

	tMtrlInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrlInfo.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f);
	tMtrlInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrlInfo.Emissive = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);
	tMtrlInfo.Power = 0.f;

	m_pGraphicDev->SetMaterial(&tMtrlInfo);

	return S_OK;
}

HRESULT CStaticObject::SetUp_ConstantTable(LPD3DXEFFECT & pEffect)
{
	_matrix			matWorld, matView, matProj, matInv;

	m_pTransformCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	pEffect->SetFloat("g_fPower", 0.f);
	pEffect->SetFloat("g_Fog", m_fFog);

	pEffect->SetFloat("g_fPower", 0.f);
	D3DXMatrixInverse(&matInv, nullptr, &matView);

	pEffect->SetVector("g_vCamPos", &_vec4(matInv._41, matInv._42, matInv._43, 1.f));

	Engine::SetUp_OnShader(pEffect, L"Target_Depth", "g_DepthTexture");

	//m_pTextureCom->Set_Texture(pEffect, "g_LightMaskTexture", 0);

	pEffect->CommitChanges();


	return S_OK;
}
