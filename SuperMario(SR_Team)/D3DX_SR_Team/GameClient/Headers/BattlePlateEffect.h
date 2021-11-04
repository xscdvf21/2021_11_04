#pragma once

#ifndef __BATTLEPLATE_EFFECT_ABSTRACT_H__
#define __BATTLEPLATE_EFFECT_ABSTRACT_H__

#include "GameObject.h"

USING(Engine);

class CBattlePlateEffect abstract : public CGameObject
{
protected:
	explicit CBattlePlateEffect(_Device pDevice);
	explicit CBattlePlateEffect(const CBattlePlateEffect& other);
	virtual ~CBattlePlateEffect() = default;

public:
	virtual HRESULT Ready_GameObject_Prototype() PURE;
	virtual HRESULT Ready_GameObject(void* pArg = nullptr) PURE;
	virtual _uint Update_GameObject(_float fDeltaTime) PURE;
	virtual _uint LateUpdate_GameObject(_float fDeltaTime) PURE;
	virtual HRESULT Render_GameObject() PURE;
	void Set_avIndex(_uint iIndex) { avIndex = iIndex; }
	void Set_Visual() { m_bVisual = true;}

public:
	virtual CGameObject* Clone(void* pArg = nullptr) PURE;
	virtual void Free();

protected:
	virtual HRESULT Add_Component() PURE;

	class CVIBuffer* m_pVIBufferCom = nullptr;
	class CTexture* m_pTextureCom = nullptr;

	//_float3 avPos[7];
	//_float3 avScale[7];

	int avIndex;
	int bavIndex;
	_bool m_bVisual;

};


#endif // !__BATTLEPLATE_EFFECT_ABSTRACT_H__