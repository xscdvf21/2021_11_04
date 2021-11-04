#pragma once
#include "GameObject.h"

USING(Engine)
class CFakeTerrain :
	public CGameObject
{
private:
	explicit CFakeTerrain(LPDIRECT3DDEVICE9 pDevice);
	explicit CFakeTerrain(const CFakeTerrain& other);
	virtual ~CFakeTerrain() = default;


public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void * pArg = nullptr) override;
	virtual _uint Update_GameObject(_float fDeltaTime) override;
	virtual _uint LateUpdate_GameObject(_float fDeltaTime) override;
	virtual HRESULT Render_GameObject() override;

private:
	HRESULT Add_Component();

public:
	static CFakeTerrain* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	virtual void Free() override;

public://Set
	void Set_TextureNumber(int _iTextuer) { m_iTextureNumber = _iTextuer; }
	void Set_SceneNumber(_uint _eSceneType) { m_eSceneType = _eSceneType; }

public://Get

private:
	class CVIBuffer*		m_pVIBufferCom = nullptr;
	class CTexture*			m_pTextureCom = nullptr;
	int						m_iTextureNumber = 0;
	_uint					m_eSceneType = 0;
};

