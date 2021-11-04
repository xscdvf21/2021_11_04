#include "stdafx.h"
#include "LineObject.h"
#include <fstream>

CLineObject::CLineObject(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject(pDevice)
{

}

CLineObject::CLineObject(CLineObject& other)
	: CGameObject(other)
{

}


HRESULT CLineObject::Ready_Object(void* pArg /*= nullptr*/)
{
	FAILED_CHECK_RETURN(Add_Component(pArg), E_FAIL);
	return S_OK;
}

Engine::_int CLineObject::Update_Object(const _float& TimeDelta)
{
	CGameObject::Update_Object(TimeDelta);
	Slide(TimeDelta);
	return OBJ_NOEVENT;
}

Engine::_int CLineObject::LateUpdate_Object(const _float & TimeDelta)
{
	if (m_bSlide && !m_bSlideEnd)
	{
		PARTICLE tParticle2;
		Engine::Reset_Particle(tParticle2);
		Engine::Set_AnimState(tParticle2, 4, 4, 3.f);
		tParticle2.fAcc = 1.f;
		tParticle2.fCurrentTime = 0.f;

		tParticle2.fRotateSpeed = 0.f;
		tParticle2.fVelocity = 1.f;
		tParticle2.fLifeTime = 0.3f;
		tParticle2.vDir = { 0.f, 0.f, 0.f };
		tParticle2.vStartColor = { 200.f * 2.5f, 200.f * 2.5f, 255.f * 4.5f, 255.f * 3.5f };
		tParticle2.vEndColor = { 200.f * 2.5f, 200.f * 2.5f, 255.f * 4.5f, 255.f * 3.5f };
		tParticle2.bBill = false;

		for (_int i = 0; i < 30; ++i)
		{
			_float fSize = CRandoms()(0.5f, 1.5f);
			tParticle2.vStartSize = _vec3(fSize, fSize, fSize);
			tParticle2.vEndSize = _vec3(0.f, 0.f, 0.f);
			tParticle2.vRot = { D3DXToRadian(_float(CRandoms()(0.f, 360.f))), D3DXToRadian(_float(CRandoms()(0.f, 360.f))), D3DXToRadian(_float(CRandoms()(0.f, 360.f))) };
			tParticle2.vPos = m_pTransformCom->Get_Position() + _vec3(CRandoms()(-0.5f, 0.5f), CRandoms()(-0.3f, 0.7f), CRandoms()(-0.5f, 0.5f));

			Engine::Emit_ImageParticle(tParticle2, 33, 20);
		}

		PARTICLE tParticle;
		Engine::Reset_Particle(tParticle);
		Engine::Set_AnimState(tParticle, 8, 8, 2.f);
		tParticle.fAcc = 1.f;
		tParticle.fCurrentTime = 0.f;

		tParticle.fRotateSpeed = 0.f;
		tParticle.fVelocity = 1.f;
		tParticle.fLifeTime = 0.3f;
		tParticle.vDir = { 0.f, 0.f, 0.f };
		tParticle.vStartColor = { 200.f * 2.5f, 200.f * 2.5f, 255.f * 4.5f, 65.f };
		tParticle.vEndColor = { 200.f * 2.5f, 200.f * 2.5f, 255.f * 4.5f, 0.f };

		tParticle.vStartSize = _vec3(1.f, 1.f, 1.f);
		tParticle.vEndSize = _vec3(6.f, 6.f, 6.f);
		tParticle.vRot = { 0.f, 0.f,D3DXToRadian(CRandoms()(0.f, 360.f)) };
		tParticle.vPos = m_pTransformCom->Get_Position() + _vec3(0.f, 0.3f, 0.f);

		Engine::Emit_ImageParticle(tParticle, 30, 4);

	}

	return OBJ_NOEVENT;
}

void CLineObject::Render_Object(const _int& iIndex /*= 0*/)
{
}

void* CLineObject::On_Active(void* pCaller)
{
	Start();
	return nullptr;
}

void* CLineObject::On_Trigger_Active(void* pTrigger, void* pCaller)
{
	Start();
	return nullptr;
}

void* CLineObject::On_Attach(void* pCaller)
{
	Start();
	return nullptr;
}

void CLineObject::Start()
{
	m_bSlide = true;
	m_bSlideStart = true;
	m_bSlideEnd = false;
}

void CLineObject::Load_Line(const _wstr& tagLine, const _bool& bBack)
{
	wifstream rd;
	rd.open(tagLine.c_str());
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

	m_vecSlidePos.emplace_back(&m_vecPos);
	m_pTransformCom->Set_Pos(m_vecPos.front());
}

HRESULT CLineObject::Add_Component(void* pArg)
{
	m_pTransformCom = Engine::Clone_Prototype(L"Proto_Transform")->Is<CTransform>();
	NULL_CHECK_RETURN(m_pTransformCom, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", m_pTransformCom);

	return S_OK;
}

void CLineObject::Slide(const _float& TimeDelta)
{
	if (!m_bSlide)
		return;

	if (m_vecSlidePos.empty())
		return;

	_vec3 vPos = m_pTransformCom->Get_Position();

	if (m_bSlideStart)
	{
		m_iSlideIndex = 0;
		m_iPathCount = 0;
		m_bSlideStart = false;
		m_iInterval = (m_vecSlidePos[m_iPathCount])->size();
		if (m_iInterval / 20 != 0)
			m_iInterval = size_t(m_iInterval / 20.f);
		else
			m_iInterval = 1;
	}
	if (m_iPathCount >= m_vecSlidePos.size() - 1)
	{
		if (m_iSlideIndex >= m_vecSlidePos[m_iPathCount]->size())
		{

			m_bSlide = false;
			m_bSlideEnd = true;

			for (auto* pObj : m_vecInteractActor)
			{
				((CGameObject*)(pObj))->On_Active(this);
			}
			StopSound(Engine::CSoundMgr::CHANNELID::Vacuum_Electric);

			return;
		}
	}
	else
	{
		if (m_iSlideIndex >= m_vecSlidePos[m_iPathCount]->size())
		{
			++m_iPathCount;
			m_iInterval = m_vecSlidePos[m_iPathCount]->size();
			if (m_iInterval / 20 != 0)
				m_iInterval = size_t(m_iInterval / 20.f);
			else
				m_iInterval = 1;
			m_iSlideIndex = 0;
		}
	}
	if (m_iSlideIndex != 0)
		m_bSlide = true;

	_vec3 vTargetPos = (*m_vecSlidePos[m_iPathCount])[m_iSlideIndex];
	_vec3 vDir = vTargetPos - vPos;
	Engine::PlaySoundW(L"Play_World_Shed_Main_Amb_Spot_ElectricCable_Spark.bnk_1.wav", Engine::CSoundMgr::CHANNELID::Vacuum_Electric, 3.f);

	if (D3DXVec3Length(&(vDir)) > 1.f*m_fSlideSpeed)
	{
		D3DXVec3Normalize(&vDir, &vDir);
		m_pTransformCom->Move_Pos(&vDir, m_fSlideSpeed * 10, TimeDelta);
	}
	else
	{
		m_pTransformCom->Set_Pos(vTargetPos);
		m_iSlideIndex += (_int)m_iInterval;
		return;
	}
}


CLineObject* CLineObject::Create(LPDIRECT3DDEVICE9 pDevice, void* pArg, const _wstr& tagLine, const _bool& bBack /*= false*/)
{
	CLineObject* pInstance = new CLineObject(pDevice);
	if (FAILED(pInstance->Ready_Object(pArg)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	pInstance->Load_Line(tagLine, bBack);

	return pInstance;
}

void CLineObject::Free(void)
{
	CGameObject::Free();
}

Engine::_mat CLineObject::Get_EventPos()
{
	if (m_bSlideEnd)
	{
		_mat temp{};
		ZeroMemory(&temp, sizeof(_mat));
		return temp;
	}
	return *m_pTransformCom->Get_WorldMatrix();
}

