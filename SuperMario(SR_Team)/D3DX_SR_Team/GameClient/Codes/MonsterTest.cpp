#include "framework.h"
#include "MonsterTest.h"



CMonsterTest::CMonsterTest(LPDIRECT3DDEVICE9 pDevice)
	:CSuperMonster(pDevice)
	, m_pTextureComp(nullptr)
{
}

CMonsterTest::CMonsterTest(const CMonsterTest& other)
	: CSuperMonster(other)
	, m_pTextureComp(other.m_pTextureComp)
{
	Safe_AddReference(m_pTextureComp);
	for (auto& pObj : m_pTextureCom)
	{
		Safe_AddReference(pObj);
	}

}

HRESULT CMonsterTest::Ready_GameObject_Prototype()
{
	if (FAILED(CGameObject::Ready_GameObject_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonsterTest::Ready_GameObject(void* pArg)
{
	if (FAILED(CGameObject::Ready_GameObject(pArg)))
		return E_FAIL;

	//여기서 처음 추가하는듯.
	if (FAILED(Add_Component()))
		return E_FAIL;

	return S_OK;
}

_uint CMonsterTest::Update_GameObject(_float fDeltaTime)
{
	CGameObject::Update_GameObject(fDeltaTime);

	//움직임 체크
	if (FAILED(Movement(fDeltaTime)))
		return 0;


	return _uint();
}

_uint CMonsterTest::LateUpdate_GameObject(_float fDeltaTime)
{
	//여기서 랜더 리스트에 추가하는구나.
	auto pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return 0;

	if (FAILED(pManagement->Add_RenderList(ERenderPriority::Alpha, this)))
		return 0;

	return m_pTransformCom->Update_Transform();
}

HRESULT CMonsterTest::Render_GameObject()
{
	if (FAILED(m_pTransformCom->Set_Transform()))
		return E_FAIL;

	if (FAILED(CGameObject::Ready_GameObject()))
		return E_FAIL;

	if (FAILED(m_pTextureComp->Set_Texture(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonsterTest::Add_Component()
{

	//담겨있는 원형들중 사용할것들을 고르는듯 ?
	//For. Com.VIBuffer
	if (FAILED(CGameObject::Add_Component(
		(_uint)ESceneType::Static,
		L"Component_VIBuffer_Rect",
		L"Com_VIBuffer",
		(CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/*For. Com_Texture*/
	if (FAILED(CGameObject::Add_Component(
		(_uint)ESceneType::Static,
		L"Component_Texture_Monster_Goomba",
		L"Com_Texture",
		(CComponent**)&m_pTextureComp
	)))
		return E_FAIL;

	//상태창
	CHealthCondition::HEALTH_DESC Healthdesc;
	ZeroMemory(&Healthdesc, sizeof(CHealthCondition::HEALTH_DESC));
	Healthdesc.iHP = 10;
	Healthdesc.iBaseAttack = 5;
	Healthdesc.iBaseDefence = 1;
	Healthdesc.iMoney = 100;

	if (FAILED(CGameObject::Add_Component(
		(_uint)ESceneType::Static,
		L"Component_HealthState",
		L"Com_Health",
		(CComponent**)&m_pHealthConditionCom, &Healthdesc
	)))
		return E_FAIL;


	return S_OK;
}

HRESULT CMonsterTest::Movement(_float fDeltaTime)
{
	return S_OK;
}

HRESULT CMonsterTest::IsOnTerrain()
{

	PRINT_LOG(L"Error", L"Not Setted CMonsterTest's OnTerrain");

	return E_FAIL;
}

HRESULT CMonsterTest::Set_Transform()
{

	PRINT_LOG(L"Error", L"Not Setted CMonsterTest's SetTransform");

	return E_FAIL;
}

CMonsterTest* CMonsterTest::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CMonsterTest* pInstance = new CMonsterTest(pDevice);
	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		PRINT_LOG(L"Error", L"Failed To Create MonsterA (Cilent - CMonster.cpp)");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CMonsterTest::Clone(void* pArg)
{
	CMonsterTest* pInstance = new CMonsterTest(*this); //복사 생성자 호출.
	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		PRINT_LOG(L"Error", L"Failed To Clone MonsterTest (Cilent - CMonster.cpp)");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMonsterTest::Free()
{
	for (auto& pObj : m_pTextureCom)
	{
		Safe_Release(pObj);
	}

	Safe_Release(m_pTextureComp);
	CSuperMonster::Free();
}
