#include "framework.h"
#include "..\Headers\BattlePlateEffect.h"

CBattlePlateEffect::CBattlePlateEffect(_Device pDevice)
	: CGameObject(pDevice)
	, m_bVisual(false)
	, avIndex(0)
	, bavIndex(0)
{
}

CBattlePlateEffect::CBattlePlateEffect(const CBattlePlateEffect& other)
	: CGameObject(other)
	, m_bVisual(false)
	, avIndex(0)
	, bavIndex(0)
{

}

void CBattlePlateEffect::Free()
{
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);

	CGameObject::Free();
}
