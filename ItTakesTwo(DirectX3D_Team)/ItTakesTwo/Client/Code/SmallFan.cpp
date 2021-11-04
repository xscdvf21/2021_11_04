#include "stdafx.h"
#include "SmallFan.h"
#include "Export_Function.h"
#include "MainCamera.h"
#include "PhysXMesh.h"
#include "BoundingBox.h"
#include "FerrisWheelBody.h"

CSmallFan::CSmallFan(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{

}

CSmallFan::~CSmallFan(void)
{
}

HRESULT CSmallFan::Ready_Object(void* pArg)
{
	ZeroMemory(&m_pModuleTransformCom, sizeof(m_pModuleTransformCom));
	ZeroMemory(&m_pModuleMeshCom, sizeof(m_pModuleMeshCom));
	FAILED_CHECK_RETURN(Add_Component(pArg), E_FAIL);

	OBJINFO Temp;
	if (pArg)
	{
		memcpy(&Temp, pArg, sizeof(OBJINFO));
	}
	m_tObjInfo = Temp;

	return S_OK;
}

Engine::_int CSmallFan::Update_Object(const _float& fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	if (m_bStart)
	{
		m_pCodyTransform = Engine::Get_Component(L"GameObject", L"Cody", L"Com_Transform", ID_DYNAMIC)->Is<CTransform>();
		m_pMayTransform = Engine::Get_Component(L"GameObject", L"May", L"Com_Transform", ID_DYNAMIC)->Is<CTransform>();
		m_bStart = false;
	}


	if (m_bSwitchOn)
	{
		m_fRotSpeed += fTimeDelta;
		if (m_fRotSpeed >= 1.0f)
			m_fRotSpeed = 1.0f;
		m_fMoveAngle = fTimeDelta * D3DXToRadian(15.0f) * -m_fRotSpeed;
		m_pTransformCom->Set_AddAngleY(m_fMoveAngle);


		_float3 vDis1 = m_pCodyTransform->Get_Position() - m_pTransformCom->Get_Position();
		_float3 vDis2 = m_pMayTransform->Get_Position() - m_pTransformCom->Get_Position();

		_float fDis1 = D3DXVec3Length(&vDis1);
		_float fDis2 = D3DXVec3Length(&vDis2);

		if (fDis2 <= 70.0f || fDis1 <= 70.0f)
		{
			if (fDis1 < fDis2)
				m_fVolume2 = fDis1;
			else
				m_fVolume2 = fDis2;
			CSoundMgr::GetInstance()->PlaySoundW(L"FerrisWheel.wav", CSoundMgr::CHANNELID::Vaccum_FerrisWheel, g_fEffecctSound * (1 - m_fVolume2 / 70.0f));
		}
		else
			CSoundMgr::GetInstance()->StopSound(CSoundMgr::CHANNELID::Vaccum_FerrisWheel);
		
	}
	else
	{
		if (true == m_bMove)
		{
			m_fRotSpeed += fTimeDelta * m_fDir;
			if (m_fDir >= 0.0f && m_fRotSpeed >= 1.0f)
				m_fRotSpeed = 1.0f;
			else if (m_fDir <= 0.0f && m_fRotSpeed <= -1.0f)
				m_fRotSpeed = -1.0f;

			CSoundMgr::GetInstance()->PlaySoundW(L"FerrisWheel.wav", CSoundMgr::CHANNELID::Vaccum_FerrisWheel, g_fEffecctSound);
			CSoundMgr::GetInstance()->Set_Volum(g_fEffecctSound, CSoundMgr::CHANNELID::Vaccum_FerrisWheel);
			m_fVolume = g_fEffecctSound;
		}
		else
		{
			m_fRotSpeed -= fTimeDelta * m_fDir;

			m_fVolume -= fTimeDelta * 0.2f;
			if (m_fVolume <= 0.0f)
			{
				m_fVolume = 0.0f;	
				CSoundMgr::GetInstance()->StopSound(CSoundMgr::CHANNELID::Vaccum_FerrisWheel);
			}
			CSoundMgr::GetInstance()->Set_Volum(m_fVolume, CSoundMgr::CHANNELID::Vaccum_FerrisWheel);

			if (m_fDir >= 0.0f && m_fRotSpeed <= 0.0f)
			{
				m_fRotSpeed = 0.0f;
			}
			else if (m_fDir <= 0.0f && m_fRotSpeed >= 0.0f)
			{
				m_fRotSpeed = 0.0f;
			}
		}
		m_fMoveAngle = fTimeDelta * D3DXToRadian(15.0f) * m_fRotSpeed;
		m_pTransformCom->Set_AddAngleY(m_fMoveAngle/*fTimeDelta * D3DXToRadian(60.0f) * m_fRotSpeed*/);

		//m_fAddAngle = 0.0f;
	}

	On_Attach(fTimeDelta);

	Engine::CGameObject::Update_Object(fTimeDelta);
	Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);
	return OBJ_NOEVENT;
}

_int CSmallFan::LateUpdate_Object(const _float & fTimeDelta)
{
	
	return _int();
}

void CSmallFan::Render_Object(const _int& iIndex)
{
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect, iIndex), );

	_uint	iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);		// 1인자 : 현재 쉐이더 파일이 갖고 있는 pass의 최대 개수, 2인자 : 시작하는 방식(default)
										//pEffect->BeginPass(0);
	m_pMeshCom->Render_Meshes(pEffect, 8);

	for (_uint i = 0; i < 3; ++i)
	{
		pEffect->SetMatrix("g_matWorld", m_pModuleTransformCom[i]->Get_WorldMatrix());
		m_pModuleMeshCom[i]->Render_Meshes(pEffect, 8);
	}

	_matrix matWorld;
	for (_uint i = 0; i < 4; ++i)
	{
		m_pTransformCom->Get_WorldMatrix(&matWorld);
		matWorld._41 += i * 10.0f + 10.0f;
		pEffect->SetMatrix("g_matWorld", &matWorld);
		m_pSylinderMeshCom[i]->Render_Meshes(pEffect, 8);

		if (i == 3)
		{
			
			m_pTransformCom->Get_WorldMatrix(&matWorld);
			matWorld._41 += i * 10.0f;
			pEffect->SetMatrix("g_matWorld", &matWorld);
			m_pMeshCom->Render_Meshes(pEffect, 8);
		}
	}

	pEffect->End();

	Safe_Release(pEffect);
}

void* CSmallFan::On_Active(void* pCaller)
{
	if (false == m_bSwitchOn)
	{
		m_fRotSpeed = 0.0f;
	}
	m_bSwitchOn = true;
	return nullptr;
}

CSmallFan* CSmallFan::Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg)
{
	CSmallFan*	pInstance = new CSmallFan(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CSmallFan::Free(void)
{
	Engine::CGameObject::Free();
}

HRESULT CSmallFan::Add_Component(void* pArg)
{
	Engine::CComponent*		pComponent = nullptr;

	OBJINFO tTemp;
	ZeroMemory(&tTemp, sizeof(OBJINFO));
	if (nullptr != pArg)
	{
		memcpy(&tTemp, pArg, sizeof(OBJINFO));
	}

	// CStaticMesh
	pComponent = m_pMeshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(tTemp.eSceneID, L"BigFanBlade"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Mesh", pComponent);

	// Transform
	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);
	m_pTransformCom->Set_Scale(_float3(0.01f, 0.01f, 0.01f));
	m_pTransformCom->Set_Pos(tTemp.vPos);
	m_pTransformCom->Set_AddAngleZ(D3DXToRadian(90.0f));
	m_pTransformCom->Set_Mode(CTransform::MODE_AXIS);
	//m_pTransformCom->Set_AddAngleY(D3DXToRadian(90.0f));

	for (_uint i = 0; i < 4; ++i)
	{
		pComponent = m_pSylinderMeshCom[i] = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(tTemp.eSceneID, L"BigFanSylinder"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		TCHAR szbuf[MAX_PATH] = L"";
		wsprintf(szbuf, L"Com_BigFanSylinder%d", i);
		m_mapComponent[Engine::ID_STATIC].emplace(szbuf, pComponent);
	}

	for (_uint i = 0; i < 3; ++i)
	{
		// CStaticMesh
		pComponent = m_pModuleMeshCom[i] = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(tTemp.eSceneID, L"FerrisModule"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		TCHAR szbuf[MAX_PATH] = L"";
		wsprintf(szbuf, L"Com_ModuleMeshCom%d", i);
		m_mapComponent[Engine::ID_STATIC].emplace(szbuf, pComponent);
	}

	for (_uint i = 0; i < 3; ++i)
	{
		_matrix matRotX;
		_vec3 vAddDir = _vec3(0.0f, -1.0f, 0.0f) * 18.0f;
		D3DXMatrixRotationX(&matRotX, D3DXToRadian(120.0f * i));
		D3DXVec3TransformNormal(&vAddDir, &vAddDir, &matRotX);

		// Transform
		pComponent = m_pModuleTransformCom[i] = dynamic_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		TCHAR szbuf[MAX_PATH] = L"";
		wsprintf(szbuf, L"Com_ModuleTransformCom%d", i);
		m_mapComponent[Engine::ID_DYNAMIC].emplace(szbuf, pComponent);
		m_pModuleTransformCom[i]->Set_Scale(_float3(0.01f, 0.01f, 0.01f));
		m_pModuleTransformCom[i]->Set_Pos(tTemp.vPos + vAddDir);
		//m_pTransformCom->Set_AddAngleZ(D3DXToRadian(90.0f));
		m_pModuleTransformCom[i]->Set_AddAngleY(D3DXToRadian(180.0f));
	}

	// Shader
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_All"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);

	return S_OK;
}

HRESULT CSmallFan::SetUp_ConstantTable(LPD3DXEFFECT & pEffect, const _int & iIndex)
{
	_matrix			matWorld, matView, matProj;

	m_pTransformCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	pEffect->SetVector("g_vColor", &_vec4(255.f, 255.f, 255.f, 255.f));
	pEffect->SetFloat("g_fAmount", 0.f);
	pEffect->SetFloat("g_fMaterialIndex", 1.f);

	pEffect->CommitChanges();

	return S_OK;
}


void * CSmallFan::On_Attach(void * pCaller)
{
	CTransform* pTransform = (CTransform*)((CGameObject*)pCaller)->Get_Component(L"Com_Transform", ID_DYNAMIC);
	_vec3 vPlayerPos = pTransform->Get_Position();
	_vec3 vPos = m_pTransformCom->Get_Position();

	vPlayerPos -= vPos;
	vPlayerPos.x = 0.0f;

	_matrix matRotZ;
	D3DXMatrixRotationX(&matRotZ, -m_fMoveAngle);

	_vec3 vResultPos;
	D3DXVec3TransformNormal(&vResultPos, &vPlayerPos, &matRotZ);

	_vec3 vResultDir = vResultPos - vPlayerPos;
	m_vResult = vResultDir;
	return &m_vResult;
}

void CSmallFan::On_Attach(const _float& fTimeDelta)
{
	for (_uint i = 0; i < 3; ++i)
	{
		_vec3 vPos = m_pModuleTransformCom[i]->Get_Position();
		_vec3 vTargetPos = m_pTransformCom->Get_Position();

		vPos -= vTargetPos;
		vPos.x = 0.0f;

		_matrix matRotZ;
		D3DXMatrixRotationX(&matRotZ, -m_fMoveAngle);

		_vec3 vResultPos;
		D3DXVec3TransformNormal(&vResultPos, &vPos, &matRotZ);

		_vec3 vResultDir = vResultPos - vPos;

		m_pModuleTransformCom[i]->Set_Pos(m_pModuleTransformCom[i]->Get_Position() + vResultDir);

		Engine::Get_GameObject(L"GameObject",
			m_vecBodyList[i].c_str())->Is<CFerriswheelBody>()->Update_Position(m_pModuleTransformCom[i]->Get_Position(),
				m_pTransformCom->Get_Position(), m_fMoveAngle, vResultDir, fTimeDelta);
	}

	//CTransform* pTransform = (CTransform*)((CGameObject*)pCaller)->Get_Component(L"Com_Transform", ID_DYNAMIC);
	//_vec3 vPlayerPos = pTransform->Get_Position();
	//_vec3 vPos = m_pTransformCom->Get_Position();

	//vPlayerPos -= vPos;
	//vPlayerPos.x = 0.0f;

	//_matrix matRotZ;
	//D3DXMatrixRotationX(&matRotZ, -m_fMoveAngle);

	//_vec3 vResultPos;
	//D3DXVec3TransformNormal(&vResultPos, &vPlayerPos, &matRotZ);

	//_vec3 vResultDir = vResultPos - vPlayerPos;
	//m_vResult = vResultDir;
}
