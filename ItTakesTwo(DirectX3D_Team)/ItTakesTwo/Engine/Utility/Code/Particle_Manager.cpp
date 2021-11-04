#include "Particle_Manager.h"
#include "particle.h"
#include "..\Resources\Code\ResourcesMgr.h"
#include "ProtoMgr.h"
#include "Renderer.h"

USING(Engine)
IMPLEMENT_SINGLETON(CParticle_Manager)

CParticle_Manager::CParticle_Manager()
	:CGameObject(nullptr)
{
}

CParticle_Manager::~CParticle_Manager()
{
	Free();
}

HRESULT CParticle_Manager::Ready_Particle_Manager(LPDIRECT3DDEVICE9 pDevice, _uint iSize)
{
	//if (!m_listParticle.empty())
	//	return E_FAIL;
	m_pDevice = pDevice;
	m_pDevice->AddRef();

	m_pShaderCom = static_cast<CShader*>(Engine::CProtoMgr::GetInstance()->Clone_Prototype(L"Shader_Effect"));
	m_dwVtxSize = sizeof(VTXPARTICLE);
	m_dwFVF = FVF_PARTICLE;
	_vbSize = 2048;
	_vbOffset = 0;
	_vbBatchSize = 512;
	FAILED_CHECK_RETURN(m_pDevice->CreateVertexBuffer(_vbSize * m_dwVtxSize,
		D3DUSAGE_DYNAMIC | D3DUSAGE_POINTS | D3DUSAGE_WRITEONLY, // 정적 버퍼 사용 시 숫자 0(D3DUSAGE_DYNAMIC : 파티클을 생성할 때)
		m_dwFVF,
		D3DPOOL_DEFAULT, // 일반적으로 정적 버퍼 사용 시 Managed, 동적버퍼 사용 시 Default 사용
		&m_pVB, nullptr), E_FAIL);

	if (!m_listImageParticle.empty())
		return E_FAIL;
	for (_uint i = 0; i < iSize; ++i)
	{
		CParticle* pParticle = new CParticle(pDevice);
		pParticle->Ready_Object();
		m_listImageParticle.emplace_back(pParticle);
	}

	return S_OK;
}

CParticle* CParticle_Manager::Emit_DotParticle(PARTICLE& desc, _uint Count, _uint PassIndex)
{
	for (_uint i = 0; i < Count; ++i)
	{
		Add_Particle(desc);
	}
	return nullptr;
}

CParticle* CParticle_Manager::Emit_ImageParticle(PARTICLE& desc, _uint TextureIndex, _uint PassIndex)
{
	if (m_listImageParticle.begin() == m_listImageParticle.end())
		return nullptr;

	CParticle* pParticle = m_listImageParticle.front();
	m_listImageParticle.pop_front();
	pParticle->Set_SpawnData(desc);
	pParticle->Change_Texture(TextureIndex);
	pParticle->Change_Pass(PassIndex);
	pParticle->Set_Active(true);

	//m_listParticle.emplace_back(pParticle);
	m_listActiveImageParticle.emplace_back(pParticle);

	return pParticle;
}

Engine::CParticle* Engine::CParticle_Manager::Emit_MeshParticle(PARTICLE& desc, const _wstr& tagMesh, _uint PassIndex)
{
	CParticle* pParticle = CParticle::Create(m_pDevice);
	pParticle->Set_SpawnData(desc);
	pParticle->Set_Mesh(tagMesh);
	pParticle->Change_Pass(PassIndex);
	pParticle->Set_Active(true);

	m_listMeshParticle.emplace_back(pParticle);

	return pParticle;
}

void CParticle_Manager::Update_Particle(const _float& fDeltaTime)
{
	m_fDelta = fDeltaTime;
	//Update_DotParticle(fDeltaTime);
	Update_ImageParticle(fDeltaTime);
	Update_MeshParticle(fDeltaTime);
}

void Engine::CParticle_Manager::Update_ImageParticle(const _float& fDeltaTime)
{
	for (auto& pParticle = m_listActiveImageParticle.begin(); pParticle != m_listActiveImageParticle.end();)
	{
		if (!((*pParticle)->Get_Active()))
		{
			Reset_Particle((*pParticle)->Get_Particle());
			m_listImageParticle.emplace_back((*pParticle));
			pParticle = m_listActiveImageParticle.erase(pParticle);
			continue;

		}

		//시간초과나 다른이유로 오브젝트 데드상태를 리턴할경우 비활성화
		if ((*pParticle)->Update_Object(fDeltaTime) == OBJ_DEAD)
		{
			(*pParticle)->Set_Active(false);
			Reset_Particle((*pParticle)->Get_Particle());
			m_listImageParticle.emplace_back((*pParticle));
			pParticle = m_listActiveImageParticle.erase(pParticle);
			continue;
		}
		pParticle++;
	}

}

void Engine::CParticle_Manager::Update_DotParticle(const _float& fDeltaTime)
{
	for (auto& i = m_listActiveParticle.begin(); i != m_listActiveParticle.end(); ++i)
	{
		if (i->bActive)
		{
			Update_Particle(*i, fDeltaTime);
			if (i->fLifeTime < i->fCurrentTime)
				i->bActive = false;
		}
	}
	Remove_DeadParticle();
}

void Engine::CParticle_Manager::Update_Particle(PARTICLE& pParticle, const _float& fDeltaTime)
{
	pParticle.vPos += pParticle.vDir * pParticle.fVelocity*fDeltaTime;
	pParticle.fCurrentTime += fDeltaTime;
	pParticle.fCurSize = (pParticle.fEndSize - pParticle.fStartSize) / pParticle.fLifeTime*pParticle.fCurrentTime;
	pParticle.vCurColor = (pParticle.vEndColor - pParticle.vStartColor) / pParticle.fLifeTime*pParticle.fCurrentTime;
}

void Engine::CParticle_Manager::LateUpdate_Particle()
{
	//Engine::CRenderer::GetInstance()->Add_RenderGroup(RENDERID::RENDER_ALPHA, this);
}

void Engine::CParticle_Manager::Update_MeshParticle(const _float& fDeltaTime)
{
	for (auto& pParticle = m_listMeshParticle.begin(); pParticle != m_listMeshParticle.end();)
	{
		if (!(*pParticle)->Get_Active())
			continue;

		//시간초과나 다른이유로 오브젝트 데드상태를 리턴할경우 비활성화
		if ((*pParticle)->Update_Object(m_fDelta) == OBJ_DEAD)
		{
			Safe_Release(*pParticle);
			pParticle = m_listMeshParticle.erase(pParticle);
			continue;
		}
		pParticle++;
	}
}

void CParticle_Manager::Render_Particle()
{
	if (!m_listActiveParticle.empty())
	{
		m_pDevice->SetFVF(m_dwFVF);
		m_pDevice->SetStreamSource(0, m_pVB, 0, sizeof(VTXPARTICLE));

		if (_vbOffset >= _vbSize)
			_vbOffset = 0;

		VTXPARTICLE* v = 0;
		m_pVB->Lock(
			_vbOffset * sizeof(VTXPARTICLE),
			_vbBatchSize * sizeof(VTXPARTICLE),
			(void**)&v,
			_vbOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);
		DWORD numParticleInBatch = 0;

		list<PARTICLE>::iterator i;
		for (i = m_listActiveParticle.begin(); i != m_listActiveParticle.end(); ++i)
		{
			if (i->bActive)
			{
				if (i->tagTextrue != m_tagTexture)
				{
					if (Set_Texture(i->tagTextrue))
						m_tagTexture = i->tagTextrue;
				}
				v->vPosition = i->vPos;
				v->fSize = i->fCurSize;
				v->vColor = i->vCurColor;
				v++;
				numParticleInBatch++;


				if (numParticleInBatch == _vbBatchSize)
				{
					m_pVB->Unlock();

					m_pDevice->DrawPrimitive(
						D3DPT_POINTLIST,
						_vbOffset,
						_vbBatchSize);

					_vbOffset += _vbBatchSize;

					if (_vbOffset >= _vbSize)
						_vbOffset = 0;
					m_pVB->Lock(
						_vbOffset * sizeof(VTXPARTICLE),
						_vbBatchSize * sizeof(VTXPARTICLE),
						(void**)&v,
						_vbOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD
					);
					numParticleInBatch = 0;
				}
			}
		}
		m_pVB->Unlock();

		if (numParticleInBatch)
		{
			m_pDevice->DrawPrimitive(
				D3DPT_POINTLIST,
				_vbOffset,
				numParticleInBatch);
		}
		_vbOffset += _vbBatchSize;
	}
}

void CParticle_Manager::Render_Particle(LPD3DXEFFECT & pEffect)
{
	if (!m_listActiveParticle.empty())
	{

		m_pDevice->SetFVF(m_dwFVF);
		m_pDevice->SetStreamSource(0, m_pVB, 0, sizeof(VTXPARTICLE));

		if (_vbOffset >= _vbSize)
			_vbOffset = 0;

		VTXPARTICLE* v = 0;
		m_pVB->Lock(

			_vbOffset * sizeof(VTXPARTICLE),
			_vbBatchSize * sizeof(VTXPARTICLE),
			(void**)&v,
			_vbOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);

		DWORD numParticleInBatch = 0;
		list<PARTICLE>::iterator i;
		for (i = m_listActiveParticle.begin(); i != m_listActiveParticle.end(); ++i)
		{
			if (i->bActive)
			{
				if (i->tagTextrue != m_tagTexture)
				{
					if (Set_Texture(i->tagTextrue))
						m_tagTexture = i->tagTextrue;
					m_pTexture->Set_Texture(pEffect, "g_BaseTexture");
					pEffect->CommitChanges();
				}
				v->vPosition = i->vPos;
				v->fSize = i->fCurSize;
				v->vColor = i->vCurColor;

				v++;
				numParticleInBatch++;


				if (numParticleInBatch == _vbBatchSize)
				{
					m_pVB->Unlock();
					m_pDevice->DrawPrimitive(
						D3DPT_POINTLIST,
						_vbOffset,
						_vbBatchSize);
					_vbOffset += _vbBatchSize;


					if (_vbOffset >= _vbSize)
						_vbOffset = 0;
					m_pVB->Lock(
						_vbOffset * sizeof(VTXPARTICLE),
						_vbBatchSize * sizeof(VTXPARTICLE),
						(void**)&v,
						_vbOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD
					);
					numParticleInBatch = 0;
				}
			}
		}
		m_pVB->Unlock();

		if (numParticleInBatch)
		{
			m_pDevice->DrawPrimitive(
				D3DPT_POINTLIST,
				_vbOffset,
				numParticleInBatch);
		}
		_vbOffset += _vbBatchSize;
	}
}

void Engine::CParticle_Manager::Set_AniState(PARTICLE& pParticle, _int x, _int y, _float fSpeed /*= 1.f*/)
{
	pParticle.iAniX = x;
	pParticle.iAniY = y;
	pParticle.iAniIndex = 0;
	pParticle.bAni = true;
	pParticle.fAniSpeed = fSpeed;
}

void CParticle_Manager::Stop_All_Particle()
{

}

void Engine::CParticle_Manager::Reset_Particle(PARTICLE& pParticle)
{
	//ZeroMemory(&pParticle, sizeof(PARTICLE));
	pParticle.fVelocity = 1.f;
	pParticle.vPos = { 0.f, -100.f, 0.f };
	pParticle.vDir = { 0.f, 0.f, 0.f };
	pParticle.tagTextrue = L"";
	pParticle.bActive = false;
	pParticle.fLifeTime = 0.f;
	pParticle.fCurrentTime = 0;
	pParticle.fCurSize = 0.f;
	pParticle.fAcc = 0.f;
	pParticle.fEndSize = 0.f;
	pParticle.fStartSize = 0.f;
	pParticle.vStartSize = { 0.f, 0.f, 0.f };
	pParticle.vEndSize = { 0.f, 0.f, 0.f };
	pParticle.fRotateSpeed = 0.f;
	pParticle.vRot = { 0.f, 0.f, 0.f };
	pParticle.bBill = true;
	pParticle.fAmount = 0.f;
	pParticle.bGravity = false;
	pParticle.fGravityForce = 9.8f;
	pParticle.bRotAdd = false;

	pParticle.vStartColor = { 0.f,0.f,0.f,1.f };
	pParticle.vEndColor = { 1.f,1.f,1.f,1.f };
	pParticle.vCurColor = { 0.f,0.f,0.f,0.f };
}

void Engine::CParticle_Manager::Add_Particle()
{
	PARTICLE tParticle;
	Reset_Particle(tParticle);

	m_listActiveParticle.emplace_back(tParticle);
}
void Engine::CParticle_Manager::Add_Particle(PARTICLE pParticle)
{
	m_listActiveParticle.emplace_back(pParticle);
}

void Engine::CParticle_Manager::Remove_DeadParticle()
{
	for (auto& pParticle = m_listActiveParticle.begin(); pParticle != m_listActiveParticle.end();)
	{
		if (pParticle->bActive == false)
		{
			pParticle = m_listActiveParticle.erase(pParticle);
		}
		else
		{
			++pParticle;
		}
	}
}

Engine::CTexture* Engine::CParticle_Manager::Find_Texture(const _wstr& tagTexture)
{
	auto iter = m_mapTexture.find(tagTexture);
	if (iter == m_mapTexture.end())
		return nullptr;
	return iter->second;
}

_bool Engine::CParticle_Manager::Set_Texture(const _wstr& tagTexture)
{
	CTexture* pTexture = nullptr;
	if (pTexture = Find_Texture(tagTexture))
	{
		m_pTexture = pTexture;
		return true;
	}
	if (pTexture = static_cast<CTexture*>(Engine::CResourcesMgr::GetInstance()->Clone_Resource(0, tagTexture.c_str())))
	{
		m_mapTexture.emplace(tagTexture, pTexture);
		m_pTexture = pTexture;
		return true;
	}
	m_pTexture = nullptr;
	return false;
}

void Engine::CParticle_Manager::Reset()
{
	for (auto i = m_listActiveParticle.begin(); i != m_listActiveParticle.end(); ++i)
		Reset_Particle(*i);
	for (auto pParticle : m_listActiveImageParticle)
	{
		pParticle->Set_Active(false);
	}
}

void CParticle_Manager::Free()
{
	this->Set_Finish();

	for (auto& pParticle : m_listImageParticle)
	{
		Safe_Release(pParticle);
	}
	for (auto& pParticle : m_listActiveImageParticle)
	{
		Safe_Release(pParticle);
	}
	m_listImageParticle.clear();
	m_listActiveImageParticle.clear();

	for (auto& pParticle : m_listMeshParticle)
	{
		Safe_Release(pParticle);
	}

	for_each(m_mapTexture.begin(), m_mapTexture.end(), CDeleteMap());
	Safe_Release(m_pVB);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pDevice);
}

void Engine::CParticle_Manager::Render_Object(const _int& iIndex /*= 0*/)
{
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	_mat	matView, matProj;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);


	UINT iMaxPass = 0;
	pEffect->Begin(&iMaxPass, 0);
	pEffect->BeginPass(0);

	Render_Particle(pEffect);

	pEffect->EndPass();
	pEffect->End();
	Safe_Release(pEffect);
}