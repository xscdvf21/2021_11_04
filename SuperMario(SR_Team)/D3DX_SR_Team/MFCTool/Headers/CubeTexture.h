#pragma once
#include "GameObj.h"
#include "ManagementTool.h"
#include "TransformTool.h"
#include "VIBufferTool.h"
#include "TextureTool.h"
#include "VIBuffer_CubeColorTool.h"

class CCubeTexture :
	public CGameObj
{
private:
	explicit CCubeTexture(LPDIRECT3DDEVICE9 pDevice);
	explicit CCubeTexture(const CCubeTexture& other);
	virtual ~CCubeTexture() = default;

public:
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void * pArg = nullptr) override;
	virtual _uint Update_GameObject(_float fDeltaTime) override;
	virtual _uint LateUpdate_GameObject(_float fDeltaTime) override;
	virtual HRESULT Render_GameObject() override;

private:
	HRESULT Add_Component();

public:
	CTransformTool* Get_Transform() { return m_pTransformCom; }

public:
	void Set_Dead() { m_bDead = true; }

public:
	static CCubeTexture* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObj * Clone(void * pArg = nullptr) override;
	virtual void Free() override;

public:
	HRESULT ImageClone(wstring _strProtoTypeTag, int _imageCount);

public:
	class CVIBufferTool*	m_pVIBufferCom = nullptr;
	class CTransformTool*	m_pTransformCom = nullptr;
	class CTextureTool*		m_pTextureCom = nullptr;
	CString m_strName = L"";
};

