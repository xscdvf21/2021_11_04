#pragma once
#include "GameObj.h"
#include "ManagementTool.h"
#include "TransformTool.h"
#include "VIBufferTool.h"
#include "TextureTool.h"


USING(Engine)
class CPlate :
	public CGameObj
{
private:
	explicit CPlate(LPDIRECT3DDEVICE9 pDevice);
	explicit CPlate(const CPlate& other);
	virtual ~CPlate() = default;

public:
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void * pArg = nullptr) override;
	virtual _uint Update_GameObject(_float fDeltaTime) override;
	virtual _uint LateUpdate_GameObject(_float fDeltaTime) override;
	virtual HRESULT Render_GameObject() override;

private:
	HRESULT Add_Component();

public:
	static CPlate* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObj * Clone(void * pArg = nullptr) override;
	virtual void Free() override;

public:
	class CVIBufferTool*	m_pVIBufferCom = nullptr;
	class CTransformTool*	m_pTransformCom = nullptr;
	class CTextureTool*		m_pTextureCom = nullptr;
};

