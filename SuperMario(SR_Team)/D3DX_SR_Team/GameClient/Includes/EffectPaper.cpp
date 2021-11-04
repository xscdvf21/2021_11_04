#include "framework.h"
#include "Effecter.h"

#include "EffectPaper.h"
#include "EffectParticle.h"



CEffectPaper::CEffectPaper(LPDIRECT3DDEVICE9 pDevice)
	: CEffecter(pDevice)
{
}

CEffectPaper::CEffectPaper(const CEffectPaper & other)
	: CEffecter(other)
{
}

HRESULT CEffectPaper::Ready_GameObject_Prototype()
{
	if (FAILED(CGameObject::Ready_GameObject_Prototype()))
		return E_FAIL;

	auto pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	UpLoad(pManagement);

	return S_OK;
}

HRESULT CEffectPaper::Ready_GameObject(void * pArg)
{
	if (FAILED(CGameObject::Ready_GameObject(pArg)))
		return E_FAIL;

	for (auto& iter : m_listParticle)
	{
		dynamic_cast<CEffectParticle*>(iter)->Set_Start();
	}

	if (FAILED(Add_Component()))
		return E_FAIL;


	auto pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

//	UpLoad(pManagement);

	return S_OK;
}

_uint CEffectPaper::Update_GameObject(_float fDeltaTime)
{
	CGameObject::Update_GameObject(fDeltaTime);

	for (auto& iter : m_listParticle)
	{
		iter->Update_GameObject(fDeltaTime);

	}

	return _uint();
}

_uint CEffectPaper::LateUpdate_GameObject(_float fDeltaTime)
{
	auto pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return 0;


	for (auto& iter : m_listParticle)
	{
		iter->LateUpdate_GameObject(fDeltaTime);
	}


	if (FAILED(pManagement->Add_RenderList(ERenderPriority::NoAlpha, this)))
		return 0;

	return m_pTransformCom->Update_Transform();

	return _uint();
}

HRESULT CEffectPaper::Render_GameObject()
{
	auto pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return 0;


	if (FAILED(m_pTransformCom->Set_Transform()))
		return E_FAIL;

	if (FAILED(CGameObject::Render_GameObject()))
		return E_FAIL;

	for (auto& iter : m_listParticle)
	{
		iter->Render_GameObject();
	}


	return S_OK;
}

HRESULT CEffectPaper::Add_Component()
{
	return S_OK;
}

HRESULT CEffectPaper::UpLoad(CManagement * pManagment)
{
	HANDLE hFile = CreateFile(L"../Resources/Texture/Test/EffectPaper.dat", GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		PRINT_LOG(L"EffectPaper파일 오픈 실패", L"EffectPaper파일 오픈 실패");
		return E_FAIL;
	}

	DWORD dwByte = 0;
	DWORD dwStrByte = 0;

	CGameObject* pObj = nullptr;

	while (true)
	{
		D3DXVECTOR3 vScaleStart, vScaleEnd;
		D3DXVECTOR3 vColorStart, vColorEnd;
		D3DXVECTOR3 vShotting;

		int eAxis;
		bool bJump, bMulti;
		float fRotation, fAccel;
		float fJumpPowerY, fLifeTime;

		ReadFile(hFile, &vScaleStart, sizeof(D3DXVECTOR3), &dwByte, nullptr);
		ReadFile(hFile, &vScaleEnd, sizeof(D3DXVECTOR3), &dwByte, nullptr);
		ReadFile(hFile, &vColorStart, sizeof(D3DXVECTOR3), &dwByte, nullptr);
		ReadFile(hFile, &vColorEnd, sizeof(D3DXVECTOR3), &dwByte, nullptr);
		ReadFile(hFile, &vShotting, sizeof(D3DXVECTOR3), &dwByte, nullptr);
		ReadFile(hFile, &eAxis, sizeof(int), &dwByte, nullptr);
		ReadFile(hFile, &bJump, sizeof(bool), &dwByte, nullptr);
		ReadFile(hFile, &bMulti, sizeof(bool), &dwByte, nullptr);
		ReadFile(hFile, &fRotation, sizeof(float), &dwByte, nullptr);
		ReadFile(hFile, &fJumpPowerY, sizeof(float), &dwByte, nullptr);
		ReadFile(hFile, &fLifeTime, sizeof(float), &dwByte, nullptr);
		ReadFile(hFile, &fAccel, sizeof(float), &dwByte, nullptr);

		if (0 == dwByte)
		{
			break;
		}

		pObj = pManagment->Clone_GameObject_For_Load(
			(_uint)ESceneType::Static,
			L"GameObject_EffectParticle",
			(_uint)ESceneType::ParticleSpace,
			L"ParticleFire");

		dynamic_cast<CEffectParticle*>(pObj)->Set_LifeTime(fLifeTime);
		dynamic_cast<CEffectParticle*>(pObj)->Set_Scale(vScaleStart, vScaleEnd);
		dynamic_cast<CEffectParticle*>(pObj)->Set_Color(vColorStart, vColorEnd, bMulti);
		dynamic_cast<CEffectParticle*>(pObj)->Set_Rotation(eAxis, fRotation);
		dynamic_cast<CEffectParticle*>(pObj)->Set_Jump(bJump, fJumpPowerY, vShotting);
		dynamic_cast<CEffectParticle*>(pObj)->Set_Accel(fAccel);

		m_listParticle.push_back(pObj);
	}
	CloseHandle(hFile);

	return S_OK;
}

CEffectPaper * CEffectPaper::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CEffectPaper* pInstance = new CEffectPaper(pDevice);
	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		PRINT_LOG(L"Error", L"Failed To Create CEffecter");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CEffectPaper::Clone(void * pArg)
{
	CEffectPaper* pInstance = new CEffectPaper(*this); //복사 생성자 호출.
	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		PRINT_LOG(L"Error", L"Failed To Clone CEffectPaper");
		Safe_Release(pInstance);
	}

	for (auto& iter : m_listParticle)
	{
		dynamic_cast<CEffectParticle*>(iter)->Set_Start();
		dynamic_cast<CEffectParticle*>(iter)->Set_Position(m_vStartPoint);
	}

	return pInstance;
}

void CEffectPaper::Free()
{
	for (auto& iter : m_listParticle)
	{
		iter->Free();
	}

	CGameObject::Free();
}
