#pragma once
#ifndef __SKYBOX_H__

#include "GameObject.h"

USING(Engine)

class CSkybox final : public CGameObject
{
private :
	explicit CSkybox(_Device _pDevice);
	explicit CSkybox(const CSkybox& other);
	virtual ~CSkybox() = default;

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void * pArg = nullptr) override;
	virtual _uint Update_GameObject(_float fDeltaTime) override;
	virtual _uint LateUpdate_GameObject(_float fDeltaTime) override;
	virtual HRESULT Render_GameObject() override;

private:
	HRESULT Add_Component();
	HRESULT Movement(_float fDeltaTime);

public:
	static CSkybox*	Create(_Device _pDevice);
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	virtual void Free() override;

private:
	class CVIBuffer*	m_pVIBufferCom = nullptr;
	class CTexture*		m_pTextureCom = nullptr;

};


#define __SKYBOX_H__
#endif // !__SKYBOX_H__
