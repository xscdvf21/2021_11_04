#include "stdafx.h"
#include "WarpGateStar.h"

#include "Export_Function.h"

CWarpGateStar::CWarpGateStar(LPDIRECT3DDEVICE9 pGraphicDev)
	:Engine::CGameObject(pGraphicDev)
{
}

CWarpGateStar::~CWarpGateStar()
{
}

void CWarpGateStar::Light(const _float & fTimeDelta)
{
	if (!m_bBegin)
		return;

	m_fLightTime += fTimeDelta;

	if (!m_bLight[0])
	{
		this->Set_Light(0);
	}
	else if (!m_bLight[1] && m_fLightTime > 1.f)
	{
		this->Set_Light(1);
	}
	else if (!m_bLight[2] && m_fLightTime > 2.f)
	{
		this->Set_Light(2);
	}
	else if (!m_bLight[3] && m_fLightTime > 3.f)
	{
		this->Set_Light(3);
	}
	else if (!m_bLight[4] && m_fLightTime > 4.f)
	{
		this->Set_Light(4);
		m_bBegin = false;
	}
}

HRESULT CWarpGateStar::Ready_Object(void * pArg, _uint Index)
{

	m_iIndex = Index;
	FAILED_CHECK_RETURN(Add_Component(pArg), E_FAIL);

	Set_INFO(pArg);


	return S_OK;
}

_int CWarpGateStar::Update_Object(const _float & fTimeDelta)
{

	Light(fTimeDelta);
	Engine::CGameObject::Update_Object(fTimeDelta);

	Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);

	return OBJ_NOEVENT;
}

_int CWarpGateStar::LateUpdate_Object(const _float & fTimeDelta)
{
	return _int();
}

void CWarpGateStar::Render_Object(const _int & iIndex)
{

	for (_uint i = 0; i < GATESTAR_End; ++i)
	{
		LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
		NULL_CHECK(pEffect);
		pEffect->AddRef();

		FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect, iIndex, i), );

		_uint	iPassMax = 0;

		pEffect->Begin(&iPassMax, 8);		// 1인자 : 현재 쉐이더 파일이 갖고 있는 pass의 최대 개수, 2인자 : 시작하는 방식(default)
											//pEffect->BeginPass(0);
		m_pMeshCom[i]->Render_Meshes(pEffect);

		pEffect->End();

		Safe_Release(pEffect);
	}
	
}


void* CWarpGateStar::On_Active(void* pCaller)
{
	m_bBegin = true;
	return nullptr;
}

HRESULT CWarpGateStar::Add_Component(void * pArg)
{
	Engine::CComponent*		pComponent = nullptr;

	OBJINFO tTemp;
	ZeroMemory(&tTemp, sizeof(OBJINFO));
	if (nullptr != pArg)
	{
		memcpy(&tTemp, pArg, sizeof(OBJINFO));
	}


	for (_uint i = 0; i < GATESTAR_End; ++i)
	{
		_tchar tagTemp[MAX_PATH];
		wsprintf(tagTemp, L"Com_MeshStar %d", i);

		// CStaticMesh
		pComponent = m_pMeshCom[i] = static_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(tTemp.eSceneID, L"WarpGate_Star"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[Engine::ID_STATIC].emplace(tagTemp, pComponent);
	}

	for(_uint i =0; i < GATESTAR_End; ++i)
	{

		_tchar tagTemp[MAX_PATH];
		wsprintf(tagTemp, L"Com_TransformStar %d", i);
		// Transform
		pComponent = m_pTransformCom[i] = static_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[Engine::ID_DYNAMIC].emplace(tagTemp, pComponent);
	}



	// Shader
	pComponent = m_pShaderCom = static_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_All"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);



	return S_OK;
}

HRESULT CWarpGateStar::SetUp_ConstantTable(LPD3DXEFFECT & pEffect, const _int & iIndex, const _int& iMeshNum)
{
	_matrix			matWorld, matView, matProj;

	m_pTransformCom[iMeshNum]->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);


	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	if (m_bLight[iMeshNum])
	{
		pEffect->SetVector("g_vColor", &_vec4(30.f, 255.f, 30.f, 255.f));
	}
	else
		pEffect->SetVector("g_vColor", &_vec4(255.f, 0.f, 0.f, 255.f));

	return S_OK;
}

void CWarpGateStar::Set_INFO(void * pArg)
{

	OBJINFO tTemp;
	ZeroMemory(&tTemp, sizeof(OBJINFO));

	if (nullptr != pArg)
	{
		memcpy(&tTemp, pArg, sizeof(OBJINFO));
	}

	if (m_iIndex == 0)
	{
		m_pTransformCom[GATESTAR_1]->Set_Scale(_vec3(0.01f, 0.005f, 0.005f));
		m_pTransformCom[GATESTAR_1]->Rotation(ROT_X, D3DXToRadian(tTemp.vAngle.x));
		m_pTransformCom[GATESTAR_1]->Rotation(ROT_Y, D3DXToRadian(tTemp.vAngle.y));
		m_pTransformCom[GATESTAR_1]->Rotation(ROT_Z, D3DXToRadian(tTemp.vAngle.z));
		m_pTransformCom[GATESTAR_1]->Set_Pos(tTemp.vPos);

		m_pTransformCom[GATESTAR_2]->Set_Scale(_vec3(0.01f, 0.01f, 0.01f));
		m_pTransformCom[GATESTAR_2]->Rotation(ROT_X, D3DXToRadian(tTemp.vAngle.x));
		m_pTransformCom[GATESTAR_2]->Rotation(ROT_Y, D3DXToRadian(tTemp.vAngle.y));
		m_pTransformCom[GATESTAR_2]->Rotation(ROT_Z, D3DXToRadian(tTemp.vAngle.z));
		m_pTransformCom[GATESTAR_2]->Set_Pos(tTemp.vPos.x, tTemp.vPos.y + 2.f, tTemp.vPos.z + 0.5f);


		m_pTransformCom[GATESTAR_3]->Set_Scale(_vec3(0.01f, 0.005f, 0.005f));
		m_pTransformCom[GATESTAR_3]->Rotation(ROT_X, D3DXToRadian(tTemp.vAngle.x));
		m_pTransformCom[GATESTAR_3]->Rotation(ROT_Y, D3DXToRadian(tTemp.vAngle.y));
		m_pTransformCom[GATESTAR_3]->Rotation(ROT_Z, D3DXToRadian(tTemp.vAngle.z));
		m_pTransformCom[GATESTAR_3]->Set_Pos(tTemp.vPos.x, tTemp.vPos.y + 4.f, tTemp.vPos.z + 1.f);

		m_pTransformCom[GATESTAR_4]->Set_Scale(_vec3(0.01f, 0.01f, 0.01f));
		m_pTransformCom[GATESTAR_4]->Rotation(ROT_X, D3DXToRadian(tTemp.vAngle.x));
		m_pTransformCom[GATESTAR_4]->Rotation(ROT_Y, D3DXToRadian(tTemp.vAngle.y));
		m_pTransformCom[GATESTAR_4]->Rotation(ROT_Z, D3DXToRadian(tTemp.vAngle.z));
		m_pTransformCom[GATESTAR_4]->Set_Pos(tTemp.vPos.x, tTemp.vPos.y + 4.f, tTemp.vPos.z + 10.f);

		m_pTransformCom[GATESTAR_5]->Set_Scale(_vec3(0.01f, 0.005f, 0.005f));
		m_pTransformCom[GATESTAR_5]->Rotation(ROT_X, D3DXToRadian(tTemp.vAngle.x));
		m_pTransformCom[GATESTAR_5]->Rotation(ROT_Y, D3DXToRadian(tTemp.vAngle.y));
		m_pTransformCom[GATESTAR_5]->Rotation(ROT_Z, D3DXToRadian(tTemp.vAngle.z));
		m_pTransformCom[GATESTAR_5]->Set_Pos(tTemp.vPos.x, tTemp.vPos.y + 2.f, tTemp.vPos.z + 11.f);
	}
	else if (m_iIndex == 1)
	{
		m_pTransformCom[GATESTAR_1]->Set_Scale(_vec3(0.01f, 0.005f, 0.005f));
		m_pTransformCom[GATESTAR_1]->Rotation(ROT_X, D3DXToRadian(tTemp.vAngle.x));
		m_pTransformCom[GATESTAR_1]->Rotation(ROT_Y, D3DXToRadian(tTemp.vAngle.y));
		m_pTransformCom[GATESTAR_1]->Rotation(ROT_Z, D3DXToRadian(tTemp.vAngle.z));
		m_pTransformCom[GATESTAR_1]->Set_Pos(tTemp.vPos);

		m_pTransformCom[GATESTAR_2]->Set_Scale(_vec3(0.01f, 0.01f, 0.01f));
		m_pTransformCom[GATESTAR_2]->Rotation(ROT_X, D3DXToRadian(tTemp.vAngle.x));
		m_pTransformCom[GATESTAR_2]->Rotation(ROT_Y, D3DXToRadian(tTemp.vAngle.y));
		m_pTransformCom[GATESTAR_2]->Rotation(ROT_Z, D3DXToRadian(tTemp.vAngle.z));
		m_pTransformCom[GATESTAR_2]->Set_Pos(tTemp.vPos.x -0.5f, tTemp.vPos.y + 2.f, tTemp.vPos.z);


		m_pTransformCom[GATESTAR_3]->Set_Scale(_vec3(0.01f, 0.005f, 0.005f));
		m_pTransformCom[GATESTAR_3]->Rotation(ROT_X, D3DXToRadian(tTemp.vAngle.x));
		m_pTransformCom[GATESTAR_3]->Rotation(ROT_Y, D3DXToRadian(tTemp.vAngle.y));
		m_pTransformCom[GATESTAR_3]->Rotation(ROT_Z, D3DXToRadian(tTemp.vAngle.z));
		m_pTransformCom[GATESTAR_3]->Set_Pos(tTemp.vPos.x - 1.f, tTemp.vPos.y + 4.f, tTemp.vPos.z);

		m_pTransformCom[GATESTAR_4]->Set_Scale(_vec3(0.01f, 0.01f, 0.01f));
		m_pTransformCom[GATESTAR_4]->Rotation(ROT_X, D3DXToRadian(tTemp.vAngle.x));
		m_pTransformCom[GATESTAR_4]->Rotation(ROT_Y, D3DXToRadian(tTemp.vAngle.y));
		m_pTransformCom[GATESTAR_4]->Rotation(ROT_Z, D3DXToRadian(tTemp.vAngle.z));
		m_pTransformCom[GATESTAR_4]->Set_Pos(tTemp.vPos.x - 10.f, tTemp.vPos.y + 4.f, tTemp.vPos.z);

		m_pTransformCom[GATESTAR_5]->Set_Scale(_vec3(0.01f, 0.005f, 0.005f));
		m_pTransformCom[GATESTAR_5]->Rotation(ROT_X, D3DXToRadian(tTemp.vAngle.x));
		m_pTransformCom[GATESTAR_5]->Rotation(ROT_Y, D3DXToRadian(tTemp.vAngle.y));
		m_pTransformCom[GATESTAR_5]->Rotation(ROT_Z, D3DXToRadian(tTemp.vAngle.z));
		m_pTransformCom[GATESTAR_5]->Set_Pos(tTemp.vPos.x -11.f, tTemp.vPos.y + 2.f, tTemp.vPos.z);
	}
}

CWarpGateStar * CWarpGateStar::Create(LPDIRECT3DDEVICE9 pGraphicDev, void * pArg, _uint Index)
{
	CWarpGateStar*	pInstance = new CWarpGateStar(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pArg, Index)))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWarpGateStar::Free(void)
{
	Engine::CGameObject::Free();
}
