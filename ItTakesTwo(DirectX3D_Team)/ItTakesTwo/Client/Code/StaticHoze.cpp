#include "stdafx.h"
#include "StaticHoze.h"
#include "Export_Function.h"
#include "MainCamera.h"
#include "..\Engine\Resources\Code\PhysXMesh.h"
#include "Logo.h"
#include <fstream>
#include "Blow.h"


CStaticHoze::CStaticHoze(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{

}

CStaticHoze::~CStaticHoze(void)
{
}

bool CStaticHoze::SetUp_ConstantTable(LPD3DXEFFECT pEffect, const _int& iIndex)
{
	_matrix			matWorld, matView, matProj;

	D3DXMatrixScaling(&matWorld, 0.01f, 0.01f, 0.01f);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	pEffect->SetVector("g_vColor", &_vec4(255.f, 255.f, 255.f, 255.f));
	pEffect->CommitChanges();

	return S_OK;
}

HRESULT CStaticHoze::Ready_Object(void* pArg)
{
	return S_OK;
}


HRESULT CStaticHoze::Ready_Object(const _wstr& tagName, const _int& bIstwoWay, void* pArg /*= nullptr*/)
{
	FAILED_CHECK_RETURN(Add_Component(tagName, bIstwoWay, pArg), E_FAIL);

	OBJINFO Temp;
	if (pArg)
	{
		memcpy(&Temp, pArg, sizeof(OBJINFO));
	}
	m_tObjInfo = Temp;
	return S_OK;
}

Engine::_int CStaticHoze::Update_Object(const _float& fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;
	if (m_bStart)
	{

		m_bStart = false;
	}
	m_pTransformCom[0]->Set_Pos(m_vecPos.front());
	if (m_pTransformCom[1])
		m_pTransformCom[1]->Set_Pos(m_vecPos.back());

	m_fEffectTime += fTimeDelta;
	if (m_bActive)
		m_pBlow->Update_Object(fTimeDelta);

	CGameObject::Update_Object(fTimeDelta);
	if (m_pMeshHead[0] && m_bActive)
	{
		m_pMeshHead[0]->Play_Animationset(fTimeDelta);
		m_pBlow->Update_Object(fTimeDelta);
		auto* pFrame = m_pMeshHead[0]->Get_FrameByName("SucktionTubeBase");
		m_pBlow->Set_Parent((pFrame->CombinedTranformationMatrix**m_pTransformCom[0]->Get_WorldMatrix()));

		PARTICLE tParticle;
		_float fRand = 0.f;
		if (CRandoms()(0, 1) == 1)
			fRand = 0.f;
		else
			fRand = 0.5f;
		Engine::Reset_Particle(tParticle);
		Engine::Set_AnimState(tParticle, 2, 2, fRand);

		tParticle.fAcc = 1.f;
		tParticle.fCurrentTime = 0.f;
		tParticle.fLifeTime = 0.15f;
		tParticle.fRotateSpeed = 0.f;
		tParticle.fVelocity = 30.f;

		if (CRandoms()(0, 1) == 1)
			tParticle.fAmount = 0.f;
		else
			tParticle.fAmount = 0.5f;

		_vec3 vLook;
		m_pTransformCom[0]->Get_Info(INFO_LOOK, &vLook);

		tParticle.vPos = m_pTransformCom[0]->Get_Position() + _vec3(CRandoms()(-2.f, 2.f), CRandoms()(-2.f, 2.f), CRandoms()(-2.f, 2.f)) + (vLook * (800.f + CRandoms()(0.f, 200.f)));
		_vec3 vDir = m_pTransformCom[0]->Get_Position() - tParticle.vPos;
		D3DXVec3Normalize(&vDir, &vDir);
		tParticle.vDir = vDir;
		tParticle.vStartSize = _vec3(0.5f, 0.5f, 0.5f);
		tParticle.vEndSize = _vec3(0.2f, 0.2f, 0.2f);
		tParticle.vStartColor = { 255.f * 3.5f, 255.f * 3.5f, 255.f * 3.5f, 250.f };
		tParticle.vEndColor = { 255.f * 3.5f, 255.f * 3.5f, 255.f * 3.5f, 150.f };

		tParticle.bBill = false;
		tParticle.vRot = { CRandoms()(0.f, 360.f), CRandoms()(0.f, 360.f), CRandoms()(0.f, 360.f) };

		Engine::Emit_ImageParticle(tParticle, 43, 23);
	}

	if (m_pMeshHead[1] && m_bActive)
	{
		m_pMeshHead[1]->Play_Animationset(fTimeDelta);

		if (m_fEffectTime > 0.12f)
		{
			PARTICLE tParticle;
			Engine::Reset_Particle(tParticle);
			Engine::Set_AnimState(tParticle, 8, 8, 2.f);

			tParticle.fAcc = 1.f;
			tParticle.fCurrentTime = 0.f;
			tParticle.fLifeTime = 1.3f;
			tParticle.fRotateSpeed = 0.f;
			tParticle.fVelocity = 15.f;

			_vec3 vLook;
			m_pTransformCom[1]->Get_Info(INFO_LOOK, &vLook);
			tParticle.vDir = vLook;
			tParticle.vPos = m_pTransformCom[1]->Get_Position() + (vLook * 200.f);
			tParticle.vStartSize = _vec3(2.f, 2.f, 2.f);
			tParticle.vEndSize = _vec3(5.f, 5.f, 5.f);
			tParticle.vStartColor = { 255.f * 2.5f, 255.f * 2.5f, 255.f * 2.5f, 150.f };
			tParticle.vEndColor = { 150.f, 150.f, 150.f, 0.f };

			Engine::Emit_ImageParticle(tParticle, 1, 4);

			m_fEffectTime = 0.f;
		}
	}

	return OBJ_NOEVENT;
}

_int CStaticHoze::LateUpdate_Object(const _float & fTimeDelta)
{
	Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);
	if (m_iWay >= 1 && m_bActive)
		m_pBlow->LateUpdate_Object(fTimeDelta);
	return _int();
}

void CStaticHoze::Render_Object(const _int& iIndex)
{
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect, iIndex), );

	_uint	iPassMax = 0;
	_matrix			matWorld, matView, matProj;
	pEffect->Begin(&iPassMax, 0);		// 1인자 : 현재 쉐이더 파일이 갖고 있는 pass의 최대 개수, 2인자 : 시작하는 방식(default)
										//pEffect->BeginPass(0);
	m_pMeshCom->Render_Meshes(pEffect, 4);

	m_pTransformCom[0]->Get_WorldMatrix(&matWorld);
	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->CommitChanges();

	if (m_iWay != 0)
	{

		m_pMeshHead[0]->Render_Meshes_VTF(pEffect, 0);

		if (m_iWay == 2)
		{
			m_pTransformCom[1]->Get_WorldMatrix(&matWorld);

			pEffect->SetMatrix("g_matWorld", &matWorld);

			pEffect->CommitChanges();

			m_pMeshHead[1]->Render_Meshes_VTF(pEffect, 0);
		}
	}
	else if (m_iWay == 0)
	{


		if (m_fEffectTime > 0.12f)
		{
			PARTICLE tParticle;
			Engine::Reset_Particle(tParticle);
			Engine::Set_AnimState(tParticle, 8, 8, 2.f);

			tParticle.fAcc = 1.f;
			tParticle.fCurrentTime = 0.f;
			tParticle.fLifeTime = 1.3f;
			tParticle.fRotateSpeed = 0.f;
			tParticle.fVelocity = 15.f;


			size_t iInterval = m_vecPos.size();
			if (iInterval / 20 != 0)
				iInterval = size_t(iInterval / 20.f);
			else
				iInterval = 1;
			_vec3 vTargetPos1 = m_vecPos[m_vecPos.size() - 1];
			_vec3 vTargetPos2 = m_vecPos[m_vecPos.size() - 1 - iInterval];
			D3DXVec3Normalize(&vTargetPos1, &(vTargetPos1 - vTargetPos2));


			tParticle.vDir = vTargetPos1;
			tParticle.vPos = m_vecPos.back()/* - tParticle.vDir * 100.f*/;
			tParticle.vStartSize = _vec3(2.f, 2.f, 2.f);
			tParticle.vEndSize = _vec3(5.f, 5.f, 5.f);
			tParticle.vStartColor = { 255.f * 2.5f, 255.f * 2.5f, 255.f * 2.5f, 150.f };
			tParticle.vEndColor = { 150.f, 150.f, 150.f, 0.f };

			Engine::Emit_ImageParticle(tParticle, 1, 4);

			m_fEffectTime = 0.f;
		}
	}

	pEffect->End();

	Safe_Release(pEffect);
}

void* CStaticHoze::On_Trigger_Active(void* pTrigger, void* pCaller)
{
	if (!m_bActive)
		return nullptr;
	m_vecOut.clear();
	m_vecOut.emplace_back(&m_vecPos);
	return &m_vecOut;
}

void* CStaticHoze::On_Active(void* pCaller)
{
	Set_Active();
	return nullptr;
}

void CStaticHoze::Load_Path(const _wstr& tagPath, const _bool& bBack)
{
	wifstream rd;
	rd.open(tagPath.c_str());
	_wstr Path;
	TCHAR* szBuf;
	_wstr wPos;
	TCHAR* buffer;
	TCHAR* Pos;
	double dwPos;

	while (!rd.eof())
	{
		getline(rd, Path);

		szBuf = new TCHAR[Path.length() + 1];
		lstrcpy(szBuf, Path.c_str());
		Pos = wcstok_s(szBuf, L",", &buffer);
		while (Pos)
		{
			wPos = Pos;
			wPos.erase(remove(wPos.begin(),
				wPos.end(), '['), wPos.end());
			wPos.erase(remove(wPos.begin(),
				wPos.end(), ']'), wPos.end());

			dwPos = _ttof(wPos.c_str());
			m_vecPoint.emplace_back((_float)dwPos*0.01f);

			Pos = wcstok_s(nullptr, L",", &buffer);
		}

		Safe_Delete_Array(szBuf);
	}
	rd.close();
	if (bBack)
	{
		for (size_t i = m_vecPoint.size(); i > 0.f; i -= 3)
		{
			m_vecPos.emplace_back(_vec3{ (_float)m_vecPoint[i - 3],(_float)m_vecPoint[i - 1],(_float)m_vecPoint[i - 2] });
		}
	}
	else
	{

		for (size_t i = 0; i < m_vecPoint.size(); i += 3)
		{
			m_vecPos.emplace_back(_vec3{ (_float)m_vecPoint[i],(_float)m_vecPoint[i + 2],(_float)m_vecPoint[i + 1] });
		}
	}
	m_vecPoint.clear();






}

void CStaticHoze::Set_Active()
{
	m_bActive = true;
	if (m_pMeshHead[0])
		m_pMeshHead[0]->Set_Animationset(0);
	if (!m_bStart)
		Engine::StopNPlaySound(L"Play_Gameplay_Vehicle_VacuumHubSmall_ChangeDirection.bnk_1.wav", CSoundMgr::CHANNELID::Vacuum_Start, 0.5f);

}

HRESULT CStaticHoze::Add_Component(const _wstr& tagName, const _int& bIsTwoWay /*= nullptr*/, void* pArg)
{
	Engine::CComponent*		pComponent = nullptr;

	OBJINFO tTemp;

	if (pArg)
	{
		memcpy(&tTemp, pArg, sizeof(OBJINFO));
	}
	m_iWay = bIsTwoWay;

	pComponent = m_pMeshCom = static_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(tTemp.eSceneID, tagName.c_str()));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Mesh", pComponent);

	// Shader
	pComponent = m_pShaderCom = static_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_VTF"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);


	// Transform
	pComponent = m_pTransformCom[0] = static_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform1", pComponent);
	m_pTransformCom[0]->Set_Scale(tTemp.vScale);
	m_pTransformCom[0]->Set_Angle({ D3DXToRadian(tTemp.vAngle.x),D3DXToRadian(tTemp.vAngle.y),D3DXToRadian(tTemp.vAngle.z) });
	if (bIsTwoWay != 0)
	{
		// CDynamicMesh
		pComponent = m_pMeshHead[0] = static_cast<Engine::CDynamicMesh*>(Engine::Clone_Resource(tTemp.eSceneID, L"MiniVacuumHead"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[Engine::ID_STATIC].emplace(L"Com_MeshHead1", pComponent);
		m_pMeshHead[0]->Set_Animationset(6);
		m_pMeshHead[0]->Play_Animationset(3.3f);


		if (bIsTwoWay >= 2)
		{
			// CDynamicMesh
			pComponent = m_pMeshHead[1] = static_cast<Engine::CDynamicMesh*>(Engine::Clone_Resource(tTemp.eSceneID, L"MiniVacuumHead"));
			NULL_CHECK_RETURN(pComponent, E_FAIL);
			m_mapComponent[Engine::ID_STATIC].emplace(L"Com_MeshHead2", pComponent);

			// Transform
			pComponent = m_pTransformCom[1] = static_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
			NULL_CHECK_RETURN(pComponent, E_FAIL);
			m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform2", pComponent);
			m_pTransformCom[1]->Set_Scale(tTemp.vScale);
			m_pTransformCom[1]->Set_Angle({ D3DXToRadian(-10.f),D3DXToRadian(-90.f),0.f });

		}
	}
	m_pBlow = CBlow::Create(m_pGraphicDev, pArg);

	return S_OK;
}

CStaticHoze* CStaticHoze::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _wstr& tagName, const vector<_wstr>& tagPath, const _int& Way/*=false*/, const _bool& bBack /*= false*/, void* pArg /*= nullptr*/)
{
	CStaticHoze*	pInstance = new CStaticHoze(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(tagName, Way, pArg)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	for (size_t i = 0; i < tagPath.size(); ++i)
	{
		pInstance->Load_Path(tagPath[i], bBack);
	}

	return pInstance;
}

void CStaticHoze::Free(void)
{
	Safe_Release(m_pBlow);
	Engine::CGameObject::Free();
}