#include "stdafx.h"
#include "Navi.h"
#include "Export_Function.h"


CNavi::CNavi(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev), m_bNaviOn(false), m_bFreeze(false)
{

}

CNavi::~CNavi(void)
{
}

HRESULT CNavi::Ready_Object(void* pArg)
{
	FAILED_CHECK_RETURN(Add_Component(pArg), E_FAIL);

	return S_OK;
}

Engine::_int CNavi::Update_Object(const _float& fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	Engine::CGameObject::Update_Object(fTimeDelta);

	Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);

	return OBJ_NOEVENT;
}

void CNavi::Render_Object(const _int& iIndex)
{
	//LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	//NULL_CHECK(pEffect);
	//pEffect->AddRef();

	//FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect), );

	//_uint	iPassMax = 0;

	//pEffect->Begin(&iPassMax, 0);		// 1인자 : 현재 쉐이더 파일이 갖고 있는 pass의 최대 개수, 2인자 : 시작하는 방식(default)
	//pEffect->BeginPass(0);

	//pEffect->EndPass();
	//pEffect->End();

	//Safe_Release(pEffect);

}

CNavi* CNavi::Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg)
{
	CNavi*	pInstance = new CNavi(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CNavi::Free(void)
{
	Engine::CGameObject::Free();
}

HRESULT CNavi::Add_Component(void* pArg)
{
	Engine::CComponent*		pComponent = nullptr;

	// Transform
	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	// NaviMesh
	pComponent = m_pNaviMeshCom = dynamic_cast<Engine::CNaviMesh*>(Engine::Clone_Resource(0, L"Mesh_Navi"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Navi", pComponent);

	// Shader
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_Mesh"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);

	NAVI Temp;
	if (pArg)
	{
		memcpy(&Temp, pArg, sizeof(NAVI));
	}

	m_pNaviMeshCom->Load_Cell(Temp.tagFilePath);

	return S_OK;
}

HRESULT CNavi::SetUp_Material(void)
{
	D3DMATERIAL9			tMtrlInfo;
	ZeroMemory(&tMtrlInfo, sizeof(tMtrlInfo));

	tMtrlInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrlInfo.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f);
	tMtrlInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrlInfo.Emissive = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);
	tMtrlInfo.Power = 0.f;

	m_pGraphicDev->SetMaterial(&tMtrlInfo);

	return S_OK;
}

HRESULT CNavi::SetUp_ConstantTable(LPD3DXEFFECT & pEffect)
{
	_matrix			matWorld, matView, matProj;

	m_pTransformCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	return S_OK;
}

HRESULT CNavi::Freeze_Cell()
{
	vector<vector<CCell*>> pVecCell = m_pNaviMeshCom->Get_VecCell();

	if (!m_bFreeze)
	{
		for (_uint i = 0; i < pVecCell[25].size(); ++i)
			pVecCell[25][i]->Set_CellState(CCell::STATE_FALL);
		return S_OK;
	}
	for (_uint i = 0; i < pVecCell[25].size(); ++i)
		pVecCell[25][i]->Set_CellState(CCell::STATE_GROUND);

	return S_OK;
}
