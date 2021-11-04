#pragma once
#include "GameObject.h"
USING(Engine)
class CTeleporter :
	public CGameObject
{
private:
	explicit CTeleporter(LPDIRECT3DDEVICE9 pDevice);
	explicit CTeleporter(const CTeleporter& other);
	virtual ~CTeleporter() = default;


public:
	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void * pArg = nullptr) override;
	virtual _uint Update_GameObject(_float fDeltaTime) override;
	virtual _uint LateUpdate_GameObject(_float fDeltaTime) override;
	virtual HRESULT Render_GameObject() override;

private:
	HRESULT Add_Component();

public:
	static CTeleporter* Create(LPDIRECT3DDEVICE9 pDevice);
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

	_float3					m_vPosition;
};

