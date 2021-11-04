#include "framework.h"
#include "..\Headers\MonsterD.h"




CMonsterD::CMonsterD(LPDIRECT3DDEVICE9 pDevice)
	:CSuperMonster(pDevice)
{
	ZeroMemory(&m_tMonsterInfo, sizeof(MONSTERINFO));
}

CMonsterD::CMonsterD(const CMonsterD & other)
	: CSuperMonster(other)
{
}

HRESULT CMonsterD::Ready_GameObject_Prototype()
{
	if (FAILED(CGameObject::Ready_GameObject_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonsterD::Ready_GameObject(void * pArg)
{
	if (FAILED(CGameObject::Ready_GameObject(pArg)))
		return E_FAIL;

	//여기서 처음 추가하는듯.
	if (FAILED(Add_Component()))
		return E_FAIL;


	m_tMonsterInfo.iMaxHp = 10;
	m_tMonsterInfo.iHp = 10;
	m_tMonsterInfo.iAttack = 10;

	return S_OK;
}

_uint CMonsterD::Update_GameObject(_float fDeltaTime)
{
	CGameObject::Update_GameObject(fDeltaTime);

	//움직임 체크
	if (FAILED(Movement(fDeltaTime)))
		return 0;


	return _uint();
}

_uint CMonsterD::LateUpdate_GameObject(_float fDeltaTime)
{
	auto pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return 0;

	if (FAILED(pManagement->Add_RenderList(ERenderPriority::NoAlpha, this)))
		return 0;

	return m_pTransformCom->Update_Transform();
}

HRESULT CMonsterD::Render_GameObject()
{
	if (FAILED(m_pTransformCom->Set_Transform()))
		return E_FAIL;

	if (FAILED(CGameObject::Ready_GameObject()))
		return E_FAIL;

	if (FAILED(m_pTextureCom[m_eState]->Set_Texture(m_tFrame.iSpriteStart)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonsterD::Add_Component()
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
		L"Component_Texture_MonsterD",
		L"Com_Texture",
		(CComponent**)&m_pTextureCom
	)))
		return E_FAIL;

	//상태창
	CHealthCondition::HEALTH_DESC Healthdesc;
	ZeroMemory(&Healthdesc, sizeof(CHealthCondition::HEALTH_DESC));
	Healthdesc.iHP = 10;
	Healthdesc.iBaseAttack = 8;
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

HRESULT CMonsterD::Movement(_float fDeltaTime)
{
	return S_OK;
}

HRESULT CMonsterD::IsOnTerrain()
{
	PRINT_LOG(L"Error", L"Not Setted Player's SetTransform");
	return E_FAIL;
}

HRESULT CMonsterD::Set_Transform()
{
	PRINT_LOG(L"Error", L"Not Setted Player's SetTransform");
	return E_FAIL;
}

CMonsterD * CMonsterD::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CMonsterD* pInstance = new CMonsterD(pDevice);
	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		PRINT_LOG(L"Error", L"Failed To Create Monster (Cilent - CMonster.cpp)");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CMonsterD::Clone(void * pArg)
{
	CMonsterD* pInstance = new CMonsterD(*this); //복사 생성자 호출.
	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		PRINT_LOG(L"Error", L"Failed To Clone Monster (Cilent - CMonster.cpp)");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMonsterD::Free()
{
	CSuperMonster::Free();
}
