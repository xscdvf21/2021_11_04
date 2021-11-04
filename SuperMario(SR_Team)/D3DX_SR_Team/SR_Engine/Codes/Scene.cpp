#include "Scene.h"

USING(Engine)

CScene::CScene(_Device pDevice)
	:m_pDevice(pDevice)
{
	Safe_AddReference(m_pDevice);
}

HRESULT CScene::Ready_Scene()
{
	return S_OK;
}

_uint CScene::Update_Scene(_float fDeltaTime)
{
	return 0;
}

_uint CScene::LateUpdate_Scene(_float fDeltaTime)
{
	return 0;
}

void CScene::Free()
{
	Safe_Release(m_pDevice);
}
