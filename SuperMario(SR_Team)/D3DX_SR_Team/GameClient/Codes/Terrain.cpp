#include "framework.h"
#include "Terrain.h"

#include "Management.h"


CTerrain::CTerrain(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject(pDevice)
{
}

CTerrain::CTerrain(const CTerrain & other)
	:CGameObject(other)
{
}

HRESULT CTerrain::Ready_GameObject_Prototype()
{
	if (FAILED(CGameObject::Ready_GameObject_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTerrain::Ready_GameObject(void * pArg /*= nullptr*/)
{
	if (FAILED(CGameObject::Ready_GameObject(pArg)))
		return E_FAIL;

	m_eSceneType = (_uint)ESceneType::Static;

	if (FAILED(Add_Component()))
		return E_FAIL;

	return S_OK;
}

_uint CTerrain::Update_GameObject(_float fDeltaTime)
{
	CGameObject::Update_GameObject(fDeltaTime);

	//CTerrain::Terrain_Colision(fDeltaTime);

	return _uint();
}

_uint CTerrain::LateUpdate_GameObject(_float fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);

	auto pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return 0;

	if (FAILED(pManagement->Add_RenderList(ERenderPriority::NoAlpha, this)))
		return 0;

	return m_pTransformCom->Update_Transform();
}

HRESULT CTerrain::Render_GameObject()
{
	if (FAILED(m_pTransformCom->Set_Transform()))
		return E_FAIL;

	if (FAILED(CGameObject::Render_GameObject()))
		return E_FAIL;

	//if (FAILED(m_pTextureCom->Set_Texture(m_iTextureNumber)))
	if (FAILED(m_pTextureCom->Set_Texture(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
		return E_FAIL;



	return S_OK;
}

HRESULT CTerrain::Add_Component()
{

	if (FAILED(CGameObject::Add_Component(
		(_uint)ESceneType::Static,
		L"Component_VIBuffer_CubeTexture",
		L"Com_VIBuffer",
		(CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(
		(_uint)ESceneType::Static,
		L"Component_Texture_Terrain",
		L"Com_Texture",
		(CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CTerrain::Set_Transform()
{
	return S_OK;
}

CTerrain * CTerrain::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CTerrain* pInstance = new CTerrain(pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		PRINT_LOG(L"Error", L"Failed To Terrain (Client - CTerrain.cpp)");
		Safe_Release(pInstance);

	}
	return pInstance;
}

CGameObject * CTerrain::Clone(void * pArg/* = nullptr*/)
{
	CTerrain* pInstance = new CTerrain(*this);
	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		PRINT_LOG(L"Error", L"Failed To Terrain (Client - CTerrain.cpp)");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTerrain::Free()
{
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);

	CGameObject::Free();
}

void CTerrain::Terrain_Colision(_float _fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();

	//테스트 스테이지에서 테스트할거면 여기 바꿔주세요
	CTransform* pTargetTransform = 
		dynamic_cast<CTransform*>(pManagement->
			Get_Component((_uint)ESceneType::TestStage, L"Layer_Player", L"Com_Transform"));

	
	_float3 vPos = m_pTransformCom->Get_Position();
   	_float3 vTargetPos = pTargetTransform->Get_Position();

	_float fLength = D3DXVec3Length(&_float3(vPos - vTargetPos));

	
	if (1.5f > fLength) //거리가 1보다 작으면.
	{
		//일단 지면 부터.
		if (vTargetPos.y > vPos.y && vTargetPos.y < vPos.y+1.f) // 타겟의 높이가 Target의 높이보다 높고. 지면 보다 낮을 경우
		{
			float fDir = fabs((vPos.y+1.f) - vTargetPos.y);
			pTargetTransform->AddPostion(_float3(0.f, fDir, 0.f));

			return;
		}

		if (vTargetPos.y > vPos.y)
		{
			return;
		}

		_float fX = 0.f, fZ = 0.f;

		fX = fabs(vTargetPos.x - vPos.x);
		fZ = fabs(vTargetPos.z - vPos.z);

		if (fX > fZ)
		{
			if (vTargetPos.z > vPos.z + 0.5f && vTargetPos.z < vPos.z+1.f )
			{
				float fDir = fabs(fabs(vPos.z + 1.f) - fabs(vTargetPos.z));
				pTargetTransform->AddPostion(_float3(0.f, 0.f, fDir));
				return;
			}
			else if(vTargetPos.z < vPos.z -0.5f && vTargetPos.z > vPos.z - 1.f)
			{
				float fDir = fabs(fabs(vPos.z - 1.f) - fabs(vTargetPos.z));
				pTargetTransform->AddPostion(_float3(0.f, 0.f, fDir));
				return;
			}
			return;

		}
		else if (fX < fZ)
		{
			if (vTargetPos.x > vPos.x +0.5f && vTargetPos.x < vPos.x + 1.f)
			{
				float fDir = fabs(fabs(vPos.x + 1.f) - fabs(vTargetPos.x));
				pTargetTransform->AddPostion(_float3(fDir, 0.f, 0.f));
				return;
			}
			else if(vTargetPos.x < vPos.x -0.5f && vTargetPos.x > vPos.x - 1.f)
			{
				float fDir = fabs(fabs(vPos.x - 1.f) - fabs(vTargetPos.x));
				pTargetTransform->AddPostion(_float3(-fDir, 0.f, 0.f));
				return;
			}
			return;
		}

	}

}
