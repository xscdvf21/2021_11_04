#pragma once
#ifndef __TERRAIN_H__

#include "GameObject.h"


USING(Engine)
class CTerrain final : public CGameObject
{
private:
	explicit CTerrain(LPDIRECT3DDEVICE9 pDevice);
	explicit CTerrain(const CTerrain& other);
	virtual ~CTerrain() = default;


public :
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void * pArg = nullptr) override;
	virtual _uint Update_GameObject(_float fDeltaTime) override;
	virtual _uint LateUpdate_GameObject(_float fDeltaTime) override;
	virtual HRESULT Render_GameObject() override;
	
private :
	HRESULT Add_Component();
	HRESULT Set_Transform();

public :
	static CTerrain* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	virtual void Free() override;

public://Set
	void Set_TextureNumber( int _iTextuer) { m_iTextureNumber = _iTextuer; }
	void Set_SceneNumber(_uint _eSceneType) { m_eSceneType = _eSceneType; }

public://Get

private:
	void Terrain_Colision(_float _fDeltaTime);
private :
	class CVIBuffer*		m_pVIBufferCom = nullptr;
	class CTexture*			m_pTextureCom = nullptr;
	int						m_iTextureNumber = 0;
	_uint					m_eSceneType = 0;

};

#define __TERRAIN_H__
#endif // !__TERRAIN_H__



