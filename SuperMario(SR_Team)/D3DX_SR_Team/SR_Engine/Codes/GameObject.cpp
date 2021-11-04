#include "..\Headers\GameObject.h"
#include "Management.h"


USING(Engine)
CGameObject::CGameObject(_Device pDevice)
	:m_pDevice(pDevice)
	, bIsDead(false)
	, bPlayerAndMonster(false)
	, bBulletAndMonster(false)
	, bBulletAndPlayer(false)
	, bBattleMonsterAndPlayer(false)
	, m_iTextureIndex (0)
	, m_bEventTrigger01(false)
	, m_bEventTrigger02(false)
	, m_bEventTrigger03(false)
	, bBillboard(false)
	
{
	Safe_AddReference(m_pDevice);
	Safe_AddReference(m_pTransformCom);
	
}

CGameObject::CGameObject(const CGameObject& other)
	:m_pDevice(other.m_pDevice)
	, bIsDead(false)
	, bPlayerAndMonster(false)
	, bBulletAndMonster(false)
	, bBulletAndPlayer(false)
	, bBattleMonsterAndPlayer(false)
	, bBillboard(false)
	, m_iTextureIndex(other.m_iTextureIndex)
	, m_bEventTrigger01(other.m_bEventTrigger01)
	, m_bEventTrigger02(other.m_bEventTrigger02)
	, m_bEventTrigger03(other.m_bEventTrigger03)
{
	for (auto& Pair : m_Components)
	{
		Safe_AddReference(Pair.second); //돌면서 추가
	}

	Safe_AddReference(m_pDevice);
	Safe_AddReference(m_pTransformCom);
}

CComponent * CGameObject::Get_Component(const wstring& ComponentTag)
{
	auto& iter_find = m_Components.find(ComponentTag); //컴포넌트 태그를 이용해서 찾아냄
	if (m_Components.end() == iter_find) //끝까지 순회했는데. 들어온 태그가 없다
	{
		return nullptr;
	}

	return iter_find->second; //값을 넘겨줌
}
/* 원형 생성시 초기화. Create함수 내부에서 호출 */
HRESULT CGameObject::Ready_GameObject_Prototype()
{
	return S_OK;
}

/* 복제본 생성시 초기화. Clone함수 내부에서 호출 */
HRESULT CGameObject::Ready_GameObject(void * pArg)
{
	CTransform::TRANSFORM_DESC TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORM_DESC));
	TransformDesc.vPos = { 0.f, 0.f, 0.f };
	TransformDesc.vScale = { 1.f, 1.f, 1.f };
	TransformDesc.fSpeed = 5.f;						//속도
	TransformDesc.fRotate = D3DXToRadian(0.f);		//회전

	if (FAILED(CGameObject::Add_Component(
		(_uint)0,									//ESceneType::Static 와 같음.
		L"Component_Transform",
		L"Com_Transform",
		(CComponent**)&m_pTransformCom,
		&TransformDesc)))
		return E_FAIL;

	return S_OK;
}

_uint CGameObject::Update_GameObject(_float fDeltaTime)
{
	return _uint();
}

_uint CGameObject::LateUpdate_GameObject(_float fDeltaTime)
{
	return _uint();
}

HRESULT CGameObject::Render_GameObject()
{
	return S_OK;
}

HRESULT Engine::CGameObject::Quick_Effect(const wstring & LayerTag, NOWINFO _tNowInfo, const int _EffectType, bool _bPosCheck, const wstring & PrototypeTag)
{
	CManagement*	pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
	{
		PRINT_LOG(L"CGameObject", L"Quick_Effect()_pManagement");
		return E_FAIL;
	}

	if (!_bPosCheck)
	{
		_tNowInfo.vPos = m_pTransformCom->Get_Position();
	}


	_tNowInfo.iEffectType = _EffectType;
	if (FAILED(pManagement
		->Clone_GameObject_ToLayer(
			_tNowInfo.iPrototypeType,
			PrototypeTag,
			_tNowInfo.iSceneType,
			LayerTag, &_tNowInfo)))
	{
		PRINT_LOG(L"CGameObject", L"Quick_Effect()_Failed");
		return E_FAIL;
	}

	return S_OK;
}



HRESULT CGameObject::Add_Component(_uint iSceneIndex, const wstring & PrototypeTag, const wstring & ComponentTag, CComponent ** ppOut, void * pArg)
{
	CManagement*	pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
	{
		return E_FAIL;
	}

	CComponent* pComponent = pManagement->Clone_Component(iSceneIndex, PrototypeTag, pArg);
	if (nullptr == pComponent)
	{
		return E_FAIL;
	}

	m_Components.insert(make_pair(ComponentTag, pComponent));

	if (ppOut)
	{
		*ppOut = pComponent;
		Safe_AddReference(pComponent);
	}

	return S_OK;
}

HRESULT Engine::CGameObject::Coin_Create(_uint _CoinCount, _float3 _vPos,  _uint iSceneIndex, _uint CurrentScene)
{
	//auto pManagement = CManagement::Get_Instance();

	//for (int i = 0; i < _CoinCount; ++i)
	//{

	//	CGameObject* pObj = pManagement->Clone_GameObject_For_Load(
	//		iSceneIndex,
	//		L"GameObject_Item_Coin",
	//		CurrentScene,
	//		L"Layer_Item_Coin"
	//	);

	//	_float3 vMonsterPos = m_pTransformCom->Get_Position();
	//	dynamic_cast<CCoin*>(pObj)->Set_Pos(_float3(_vPos.x + (i * 0.2), _vPos.y + (i * 0.2f), _vPos.z));
	//}

	return S_OK;
}


void CGameObject::Free()
{
	Safe_Release(m_pTransformCom);

	for (auto& Pair : m_Components)
	{
		Safe_Release(Pair.second); //돌면서 삭제
	}

	m_Components.clear(); //값 전부 삭제후, map 클리어
	Safe_Release(m_pDevice); 

}

void Engine::CGameObject::Set_X(_float _fX)
{
	m_pTransformCom->AddPostion(_float3(_fX, 0.f, 0.f));
}

void Engine::CGameObject::Set_Z(float _fZ)
{
	m_pTransformCom->AddPostion(_float3(0.f, 0.f, _fZ));
}

void Engine::CGameObject::Set_Y(float _fY)
{
	m_pTransformCom->AddPostion(_float3(0.f, _fY, 0.f));
}


void Engine::CGameObject::Set_ObjPosition(_float3 _vPos)
{
	m_pTransformCom->Set_Position(_vPos); 
}

