#include "stdafx.h"
#include "Map.h"
#include "Export_Function.h"
#include "MainCamera.h"
#include "PhysXMesh.h"
#include "..\Engine\Utility\Code\Frustum.h"
#include "Cody.h"
#include "May.h"


CMap::CMap(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{

}

CMap::~CMap(void)
{
}

HRESULT CMap::Ready_Object(void* pArg)
{
	D3DXMatrixIdentity(&m_matProj);
	FAILED_CHECK_RETURN(Add_Component(pArg), E_FAIL);

	OBJINFO Temp;
	if (pArg)
	{
		memcpy(&Temp, pArg, sizeof(OBJINFO));
	}
	//m_pTransformCom->Set_Pos(Temp.vPos);
	//m_pTransformCom->Set_Scale(Temp.vScale);
	//m_pTransformCom->Set_Angle(Temp.vAngle);
	m_eRender = Temp.eRenderType;
	lstrcpy(m_tagMesh, Temp.tagMesh);

	m_iPass = 8;


	return S_OK;
}

Engine::_int CMap::Update_Object(const _float& fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;



	if (m_bShadow)
	{
		_matrix matWorld, matView, matProj;

		m_pTransformCom->Get_WorldMatrix(&matWorld);
		m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
		m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

		matView = matWorld * matView;
		m_matProj = matView * matProj;
		m_bShadow = false;
	}

	//Engine::CGameObject::Update_Object(fTimeDelta);


	Engine::Add_RenderGroup(Engine::RENDERID(m_eRender), this);
	//Engine::Add_RenderGroup(Engine::RENDER_SHADOW, this);

	return OBJ_NOEVENT;
}

_int CMap::LateUpdate_Object(const _float & fTimeDelta)
{
	//CMainCamera* pCamera = (CMainCamera*)Engine::Get_GameObject(L"GameObject", L"MainCamera");
	//_vec3 vCampos = pCamera->Get_CameraPos();

	//m_fFog = fabs(D3DXVec3Length(&(vCampos - m_pTransformCom->Get_Position())));

	return _int();
}

void CMap::Render_Object(const _int& iIndex)
{
	//m_pStaticMeshCom->Culling(m_pFrustum,m_pTransformCom->Get_Scale().x);
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	_matrix		LightViewMatrix, LightProjMatrix;
	if (m_bStart)
	{
		D3DXMatrixPerspectiveFovLH(&LightProjMatrix, D3DXToRadian(60.f), _float(WINCX) / WINCY, 0.1f, 500.f);
		pEffect->SetMatrix("g_matLightProj", &LightProjMatrix);
		m_bStart = false;
	}

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect), );

	CMay* pMay = (CMay*)Engine::Get_GameObject(L"GameObject", L"May");
	CTransform* pMayTrans = (CTransform*)pMay->Get_Component(L"Com_Transform", ID_DYNAMIC);

	CCody* pCody = (CCody*)Engine::Get_GameObject(L"GameObject", L"Cody");
	CTransform* pCodyTrans = (CTransform*)pCody->Get_Component(L"Com_Transform", ID_DYNAMIC);

	if (iIndex == 0)
	{
		_vec3 vAt = (pCodyTrans->Get_Position() + pMayTrans->Get_Position()) * 0.5f;
		_vec3 vEye = vAt + SHADOWLIGHT;

		D3DXMatrixLookAtLH(&LightViewMatrix, &vEye, &vAt, &vUP);
		pEffect->SetMatrix("g_matLightView", &LightViewMatrix);

		//D3DXMatrixPerspectiveFovLH(&LightProjMatrix, D3DXToRadian(60.f), _float(WINCX) / WINCY, 0.1f, 500.f);
		//pEffect->SetMatrix("g_matLightProj", &LightProjMatrix);
		Engine::SetUp_OnShader(pEffect, L"Target_ShadowDepth_May", "g_Add1Texture");
	}
	else if (iIndex == 1)
	{
		_vec3 vAt = (pCodyTrans->Get_Position() + pMayTrans->Get_Position()) * 0.5f;
		_vec3 vEye = vAt + SHADOWLIGHT;

		D3DXMatrixLookAtLH(&LightViewMatrix, &vEye, &vAt, &vUP);
		pEffect->SetMatrix("g_matLightView", &LightViewMatrix);

		//D3DXMatrixPerspectiveFovLH(&LightProjMatrix, D3DXToRadian(60.f), _float(WINCX) / WINCY, 0.1f, 500.f);
		//pEffect->SetMatrix("g_matLightProj", &LightProjMatrix);
		Engine::SetUp_OnShader(pEffect, L"Target_ShadowDepth_Cody", "g_Add1Texture");
	}
	else if (iIndex == 2)
	{
		CMainCamera* pCamera = (CMainCamera*)Engine::Get_GameObject(L"GameObject", L"MainCamera");
		if (!pCamera)
		{
			_uint	iPassMax = 0;
			pEffect->Begin(&iPassMax, 0);
			m_pStaticMeshCom->Render_Meshes(pEffect, 8);
			pEffect->End();
			Safe_Release(pEffect);
			return;
		}

		_vec3 vEye = pCamera->Get_CameraPos();
		_vec3 vAt = pCamera->Get_CameraAt();

		_vec3		vLightEye = _vec3(4.f, 0.f, -4.f);

		D3DXMatrixLookAtLH(&LightViewMatrix, &(vEye + vLightEye), &vAt, &vUP);
		pEffect->SetMatrix("g_matLightView", &LightViewMatrix);

		//D3DXMatrixPerspectiveFovLH(&LightProjMatrix, D3DXToRadian(60.f), _float(WINCX) / WINCY, 0.1f, 1000.f);
		//pEffect->SetMatrix("g_matLightProj", &LightProjMatrix);
		Engine::SetUp_OnShader(pEffect, L"Target_ShadowDepth_May", "g_Add1Texture");
	}

	_uint	iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);		// 1인자 : 현재 쉐이더 파일이 갖고 있는 pass의 최대 개수, 2인자 : 시작하는 방식(default)

	m_pStaticMeshCom->Render_Meshes(pEffect, 8);

	pEffect->End();

	Safe_Release(pEffect);
}

void CMap::Render_Shadow(_vec3* vEye, _vec3* vAt, const _int& iIndex)
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

	_vec3 vInv = { matInv._41,matInv._42,matInv._43 };

	//CCody* pCody = (CCody*)Engine::Get_GameObject(L"GameObject", L"Cody");
	//CTransform* pCodyTrans = (CTransform*)pCody->Get_Component(L"Com_Transform", ID_DYNAMIC);

	//CMay* pMay = (CMay*)Engine::Get_GameObject(L"GameObject", L"May");
	//CTransform* pMayTrans = (CTransform*)pCody->Get_Component(L"Com_Transform", ID_DYNAMIC);

	//*vAt = (pMayTrans->Get_Position() + pCodyTrans->Get_Position()) * 0.5f;
	//*vEye = *vAt + _vec3(4.f, 50.f, 4.f);
	*vAt = _vec3(143.f, -155.f, -80.f);
	//*vEye = *vAt + STATIC_SHADOWLIGHT;
	*vEye = _vec3(-181.f, 200.f, 193.f);

	_matrix		LightViewMatrix, LightProjMatrix;

	D3DXMatrixLookAtLH(&LightViewMatrix, &*vEye, &*vAt, &vUP);
	pEffect->SetMatrix("g_matView", &LightViewMatrix);

	D3DXMatrixPerspectiveFovLH(&LightProjMatrix, D3DXToRadian(60.f), _float(WINCX) / WINCY, 0.1f, 500.f);
	pEffect->SetMatrix("g_matProj", &LightProjMatrix);

	_uint	iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);		// 1인자 : 현재 쉐이더 파일이 갖고 있는 pass의 최대 개수, 2인자 : 시작하는 방식(default)
										//pEffect->BeginPass(0);
	m_pStaticMeshCom->Render_Meshes(pEffect, 12);

	pEffect->End();

	Safe_Release(pEffect);
}

CMap* CMap::Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg)
{
	CMap*	pInstance = new CMap(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CMap::Free(void)
{
	Safe_Release(m_pFrustum);
	Engine::CGameObject::Free();
}

HRESULT CMap::Add_Component(void* pArg)
{
	Engine::CComponent*		pComponent = nullptr;

	OBJINFO Temp;
	if (pArg)
	{
		memcpy(&Temp, pArg, sizeof(OBJINFO));
	}
	m_eMesh = Temp.eMeshType;



	// CStaticMesh
	pComponent = m_pStaticMeshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(Temp.eSceneID, Temp.tagMesh));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Mesh", pComponent);

	// Transform
	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	// Shader
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_All"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);


	// Shader
	m_pFrustum = Engine::CFrustum::Create(m_pGraphicDev);


	PxShape* pShape = nullptr;
	auto* pPhysics = Engine::Get_Physics();
	_vec3 vPos = Temp.vPos;
	PxQuat qQuat(0.f, { 0.f,1.f,0.f });
	PxTransform tTransform;

	if (Temp.pWorld)
	{
		m_matWorld = *Temp.pWorld;

		m_pTransformCom->Set_WorldMatrix(*Temp.pWorld);
		vPos = *((_vec3*)&(Temp.pWorld->_41));
		PxVec3 vRight, vLook, vUp;
		memcpy(&vRight, Temp.pWorld, sizeof(_vec3));
		memcpy(&vUp, &Temp.pWorld->_21, sizeof(_vec3));
		memcpy(&vLook, &Temp.pWorld->_31, sizeof(_vec3));
		PxMat33 matRot(vRight, vUp, vLook);
		qQuat = PxQuat(Temp.qQuat.x, Temp.qQuat.y, Temp.qQuat.z, Temp.qQuat.w);
		PxMaterial* pMaterial = pPhysics->createMaterial(0.5f, 0.5f, 0);
		PxTriangleMeshGeometry pMeshGeo;
		pMeshGeo.triangleMesh = m_pStaticMeshCom->Get_PxMesh();//m_pPhysMesh->Get_PxMesh();
		pMeshGeo.scale = PxMeshScale(PxVec3(Temp.vScale.x, Temp.vScale.y, Temp.vScale.z));
		pShape = pPhysics->createShape(pMeshGeo, *pMaterial, true);
		pShape->setSimulationFilterData(PxFilterData(FilterGroup::eGround, FilterGroup::ePlayer | FilterGroup::eCody | FilterGroup::eMay, 0, 0));
		pShape->setQueryFilterData(PxFilterData(FilterGroup::eGround, 0, 0, 0));
		tTransform = PxTransform(vPos.x, vPos.y, vPos.z, qQuat);

		pShape->setQueryFilterData(PxFilterData(FilterGroup::eGround, FilterGroup::ePlayer, 0, 0));
	}
	else
	{

		m_pTransformCom->Set_Pos(Temp.vPos.x, Temp.vPos.y, Temp.vPos.z);
		m_pTransformCom->Set_Scale(Temp.vScale.x, Temp.vScale.y, Temp.vScale.z);
		m_pTransformCom->Set_Angle({ Temp.vAngle.x, Temp.vAngle.y, Temp.vAngle.z });
		m_pTransformCom->Update_Component(0.f);
		qQuat = PxQuat(Temp.vAngle.x, { 1.f,0.f,0.f }) *PxQuat(Temp.vAngle.y, { 0.f,1.f,0.f })*PxQuat(Temp.vAngle.z, { 0.f,0.f,1.f });
		tTransform = PxTransform(vPos.x, vPos.y, vPos.z, qQuat);
		
		PxMaterial* pMaterial = pPhysics->createMaterial(0.5f, 0.5f, 0);
		PxTriangleMeshGeometry pMeshGeo;
		pMeshGeo.triangleMesh = m_pStaticMeshCom->Get_PxMesh();//m_pPhysMesh->Get_PxMesh();
		pMeshGeo.scale = PxMeshScale(PxVec3(Temp.vScale.x, Temp.vScale.y, Temp.vScale.z));
		pShape = pPhysics->createShape(pMeshGeo, *pMaterial, true);
		pShape->setSimulationFilterData(PxFilterData(FilterGroup::eGround, FilterGroup::ePlayer | FilterGroup::eCody | FilterGroup::eMay, 0, 0));

		pShape->setQueryFilterData(PxFilterData(FilterGroup::eGround, FilterGroup::ePlayer, 0, 0));
	}


	if (pShape)
	{


		m_pRigidActor = PxCreateStatic(*pPhysics, tTransform, *pShape);
		//if (!m_pRigidActor)
		//	return E_FAIL;
		m_pRigidActor->setName((char*)this);
	}

	return S_OK;
}

HRESULT CMap::SetUp_Material(void)
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

HRESULT CMap::SetUp_ConstantTable(LPD3DXEFFECT & pEffect)
{
	_matrix			matWorld, matView, matProj, matInv;

	m_pTransformCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);
	pEffect->SetMatrix("g_matCustom", &m_matProj);

	pEffect->SetFloat("g_fPower", 0.f);
	pEffect->SetFloat("g_Fog", m_fFog);

	pEffect->SetFloat("g_fPower", 0.f);
	D3DXMatrixInverse(&matInv, nullptr, &matView);

	pEffect->SetVector("g_vCamPos", &_vec4(matInv._41, matInv._42, matInv._43, 1.f));
	pEffect->SetFloat("g_fMaterialIndex", m_fMaterialIndex);


	pEffect->SetVector("g_vColor", &_vec4(255.f, 255.f, 255.f, 255.f));
	pEffect->SetFloat("g_fAmount", 0.f);

	Engine::SetUp_OnShader(pEffect, L"Target_", "g_DepthTexture");

	pEffect->CommitChanges();


	return S_OK;
}
