#pragma once
#include"GameObject.h"

USING(Engine)
class CFlipPuzzle :
	public CGameObject
{
private:
	explicit CFlipPuzzle(LPDIRECT3DDEVICE9 pDevice);
	explicit CFlipPuzzle(const CFlipPuzzle& other);
	virtual ~CFlipPuzzle() = default;

public:
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void* pArg = nullptr) override;
	virtual _uint Update_GameObject(_float fDeltaTime) override;
	virtual _uint LateUpdate_GameObject(_float fDeltaTime) override;
	virtual HRESULT Render_GameObject() override;

private:
	HRESULT Add_Component();

public:
	static CFlipPuzzle* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;

private:
	class CVIBuffer* m_pVIBufferCom = nullptr;
	class CTexture* m_pTextureCom = nullptr;
};