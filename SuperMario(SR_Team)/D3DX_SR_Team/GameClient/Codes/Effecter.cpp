#include "framework.h"
#include "Effecter.h"

#include "EffectParticle.h"

//GameObject_EffectParticle
CEffecter::CEffecter(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject(pDevice)
{

}

CEffecter::CEffecter(const CEffecter & other)
	: CGameObject(other)
{
	for (int i = 0; i < EFFECTTYPE::EFFECTTYPE_END; ++i)
	{
		m_listParticle[i] = other.m_listParticle[i];
	}
}

HRESULT CEffecter::Ready_GameObject_Prototype()
{
	if (FAILED(CGameObject::Ready_GameObject_Prototype()))
		return E_FAIL;

	auto pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;
	
	UpLoad_EffectFire(pManagement, EFFECTTYPE::EFFECTTYPE_FIRE);
	UpLoad_EffectPaper(pManagement, EFFECTTYPE::EFFECTTYPE_PAPER);
	UpLoad_EffectGreenBall(pManagement, EFFECTTYPE::EFFECTTYPE_GREENDIAMOND);
	UpLoad_EffectHammer(pManagement, EFFECTTYPE::EFFECTTYPE_HAMMER);

	return S_OK;
}

HRESULT CEffecter::Ready_GameObject(void * pArg /*= nullptr*/)
{
	if (FAILED(CGameObject::Ready_GameObject(pArg)))
		return E_FAIL;

	if (FAILED(Add_Component()))
		return E_FAIL;

	if (nullptr == pArg)
	{
		PRINT_LOG(L"이펙터", L"이펙트에 보이드 포인터를 넣으세요.");
		return E_FAIL;
	}

	m_tNowInfo = *(NOWINFO*)pArg;

	m_listParticleClone.clear();
	
	CGameObject* pObj;

	for (auto& iter : m_listParticle[m_tNowInfo.iEffectType])
	{
		pObj = CEffectParticle::CloneToClone(dynamic_cast<CEffectParticle*>(iter));
		dynamic_cast<CEffectParticle*>(pObj)->Set_vStartPoint(m_tNowInfo.vPos);
		m_listParticleClone.push_back(pObj);
	}

	return S_OK;
}

_uint CEffecter::Update_GameObject(_float fDeltaTime)
{
	CGameObject::Update_GameObject(fDeltaTime);

	for (auto& iter = m_listParticleClone.begin(); iter != m_listParticleClone.end(); ++iter)
	{
		(*iter)->Update_GameObject(fDeltaTime);
		
		if ((*iter)->Get_Dead() == true) {
			Safe_Release(*iter);
			iter = m_listParticleClone.erase(iter);
		}
	}


	if (0 == m_listParticleClone.size())
	{
		bIsDead = true;
	}

	return _uint();
}

_uint CEffecter::LateUpdate_GameObject(_float fDeltaTime)
{
	//여기서 랜더 리스트에 추가하는구나.
	auto pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return 0;


	for (auto& iter : m_listParticleClone)
	{
		iter->LateUpdate_GameObject(fDeltaTime);
	}

	if (FAILED(pManagement->Add_RenderList(ERenderPriority::NoAlpha, this)))
		return 0;

	return m_pTransformCom->Update_Transform();
}

HRESULT CEffecter::Render_GameObject()
{
	auto pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return 0;


	if (FAILED(m_pTransformCom->Set_Transform()))
		return E_FAIL;

	if (FAILED(CGameObject::Render_GameObject()))
		return E_FAIL;
	
	for (auto& iter : m_listParticleClone)
	{
		iter->Render_GameObject();
	}

	return S_OK;
}


HRESULT CEffecter::UpLoad_EffectFire(CManagement* pMan, _uint iEffectType)
{
	HANDLE hFile = CreateFile(L"../Resources/Texture/Test/EffectFire.dat", GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		PRINT_LOG(L"Effect파일 오픈 실패", L"Effect파일 오픈 실패");
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

		pObj = pMan->Clone_GameObject_For_Load(
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

		m_listParticle[iEffectType].push_back(pObj);
	}
	CloseHandle(hFile);


	return S_OK;
}

HRESULT CEffecter::UpLoad_EffectGreenBall(CManagement* pMan, _uint iEffectType)
{
	HANDLE hFile = CreateFile(L"../Resources/Texture/Test/EffectGreenBall.dat", GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		PRINT_LOG(L"Effect파일 오픈 실패", L"Effect파일 오픈 실패");
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

		pObj = pMan->Clone_GameObject_For_Load(
			(_uint)ESceneType::Static,
			L"GameObject_EffectParticle",
			(_uint)ESceneType::ParticleSpace,
			L"ParticlePaper");

		dynamic_cast<CEffectParticle*>(pObj)->Set_LifeTime(fLifeTime);
		dynamic_cast<CEffectParticle*>(pObj)->Set_Scale(vScaleStart, vScaleEnd);
		dynamic_cast<CEffectParticle*>(pObj)->Set_Color(vColorStart, vColorEnd, bMulti);
		dynamic_cast<CEffectParticle*>(pObj)->Set_Rotation(eAxis, fRotation);
		dynamic_cast<CEffectParticle*>(pObj)->Set_Jump(bJump, fJumpPowerY, vShotting);
		dynamic_cast<CEffectParticle*>(pObj)->Set_Accel(fAccel);

		m_listParticle[iEffectType].push_back(pObj);
	}
	CloseHandle(hFile);

	return S_OK;
}

HRESULT CEffecter::UpLoad_EffectPaper(CManagement* pMan, _uint iEffectType)
{
	HANDLE hFile = CreateFile(L"../Resources/Texture/Test/EffectPaper.dat", GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		PRINT_LOG(L"Effect파일 오픈 실패", L"Effect파일 오픈 실패");
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

		pObj = pMan->Clone_GameObject_For_Load(
			(_uint)ESceneType::Static,
			L"GameObject_EffectParticle",
			(_uint)ESceneType::ParticleSpace,
			L"ParticleGreenBall");

		dynamic_cast<CEffectParticle*>(pObj)->Set_LifeTime(fLifeTime);
		dynamic_cast<CEffectParticle*>(pObj)->Set_Scale(vScaleStart, vScaleEnd);
		dynamic_cast<CEffectParticle*>(pObj)->Set_Color(vColorStart, vColorEnd, bMulti);
		dynamic_cast<CEffectParticle*>(pObj)->Set_Rotation(eAxis, fRotation);
		dynamic_cast<CEffectParticle*>(pObj)->Set_Jump(bJump, fJumpPowerY, vShotting);
		dynamic_cast<CEffectParticle*>(pObj)->Set_Accel(fAccel);

		m_listParticle[iEffectType].push_back(pObj);
	}
	CloseHandle(hFile);

	return S_OK;
}

HRESULT CEffecter::UpLoad_EffectHammer(CManagement * pMan, _uint iEffectType)
{
	HANDLE hFile = CreateFile(L"../Resources/Texture/Test/EffectHammer.dat", GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		PRINT_LOG(L"Effect파일 오픈 실패", L"Effect파일 오픈 실패");
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

		pObj = pMan->Clone_GameObject_For_Load(
			(_uint)ESceneType::Static,
			L"GameObject_EffectParticle",
			(_uint)ESceneType::ParticleSpace,
			L"EffectHammer");

		dynamic_cast<CEffectParticle*>(pObj)->Set_LifeTime(fLifeTime);
		dynamic_cast<CEffectParticle*>(pObj)->Set_Scale(vScaleStart, vScaleEnd);
		dynamic_cast<CEffectParticle*>(pObj)->Set_Color(vColorStart, vColorEnd, bMulti);
		dynamic_cast<CEffectParticle*>(pObj)->Set_Rotation(eAxis, fRotation);
		dynamic_cast<CEffectParticle*>(pObj)->Set_Jump(bJump, fJumpPowerY, vShotting);
		dynamic_cast<CEffectParticle*>(pObj)->Set_Accel(fAccel);

		m_listParticle[iEffectType].push_back(pObj);
	}
	CloseHandle(hFile);

	return S_OK;
}

HRESULT CEffecter::Add_Component()
{
	return S_OK;
}

CEffecter * CEffecter::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CEffecter* pInstance = new CEffecter(pDevice);
	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		PRINT_LOG(L"Error", L"Failed To Create CEffecter");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CEffecter::Clone(void * pArg /*= nullptr*/)
{
	CEffecter* pInstance = new CEffecter(*this); //복사 생성자 호출.
	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		PRINT_LOG(L"Error", L"Failed To Clone CEffecter");
		Safe_Release(pInstance);
	}

	for (auto& iter : m_listParticleClone)
	{
		dynamic_cast<CEffectParticle*>(iter)->Set_Start();
	}

	return pInstance;
}

void CEffecter::Free()
{
	for (auto&iter : m_listParticleClone)
	{
		Safe_Release(iter);
	}

	for (int i = 0; i < EFFECTTYPE::EFFECTTYPE_END; ++i)
	{
		for (auto& iter : m_listParticle[i])
		{
			Safe_Release(iter);
		}
		m_listParticle[i].clear();
	}

	m_listParticleClone.clear();

	CGameObject::Free();
}

//if (FAILED(pManagement->Clone_GameObject_ToLayer(
//(_uint)ESceneType::Static,
//L"GameObject_EffectParticle",
//(_uint)ESceneType::Stage,
//L"ParticleFire")))
//{ 
//	return E_FAIL;
//}
