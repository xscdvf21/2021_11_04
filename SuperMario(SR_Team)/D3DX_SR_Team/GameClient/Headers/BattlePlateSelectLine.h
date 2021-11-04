#pragma once


#ifndef __BATTLEPALTE_SELECT_LINE_H__
#define __BATTLEPALTE_SELECT_LINE_H__


#include "BattlePlateEffect.h"

USING(Engine);

//라인 어택 이펙트 패널
class CBattlePlateSelectLine : public CBattlePlateEffect
{
private:
	explicit CBattlePlateSelectLine(_Device pDevice);
	explicit CBattlePlateSelectLine(const CBattlePlateSelectLine& other);
	virtual ~CBattlePlateSelectLine() = default;

public:
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void* pArg = nullptr) override;
	virtual _uint Update_GameObject(_float fDeltaTime) override;
	virtual _uint LateUpdate_GameObject(_float fDeltaTime) override;
	virtual HRESULT Render_GameObject() override;
	void Set_avIndex(_uint iIndex) { avIndex = iIndex; }

public:
	static CBattlePlateSelectLine* Create(_Device pDevice);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;

private:
	HRESULT Add_Component();

	class CVIBuffer* m_pVIBufferCom = nullptr;
	class CTexture* m_pTextureCom = nullptr;



};


#endif // !__BATTLEPALTE_SELECT_LINE_H__