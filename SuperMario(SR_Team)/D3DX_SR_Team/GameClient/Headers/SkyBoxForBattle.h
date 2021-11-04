#pragma once
#ifndef __SKYBOX_BATTLE_H__

#include "GameObject.h"

USING(Engine)

class CSkyBoxForBattle final : public CGameObject
{
private:
	explicit CSkyBoxForBattle(_Device _pDevice);
	explicit CSkyBoxForBattle(const CSkyBoxForBattle& other);
	virtual ~CSkyBoxForBattle() = default;

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void* pArg = nullptr) override;
	virtual _uint Update_GameObject(_float fDeltaTime) override;
	virtual _uint LateUpdate_GameObject(_float fDeltaTime) override;
	virtual HRESULT Render_GameObject() override;

private:
	HRESULT Add_Component();
	HRESULT Movement(_float fDeltaTime);

public:
	static CSkyBoxForBattle* Create(_Device _pDevice);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;

private:
	class CVIBuffer* m_pVIBufferCom = nullptr;
	class CTexture* m_pTextureCom = nullptr;

};


#define __SKYBOX_BATTLE_H__
#endif // !__SKYBOX_H__
