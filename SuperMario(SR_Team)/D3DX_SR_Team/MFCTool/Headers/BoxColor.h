#pragma once
#include "GameObj.h"
#include "ManagementTool.h"
#include "TransformTool.h"
#include "VIBufferTool.h"


USING(Engine)
class CBoxColor :
	public CGameObj
{
private:
	explicit CBoxColor(LPDIRECT3DDEVICE9 pDevice);
	explicit CBoxColor(const CBoxColor& other);
	virtual ~CBoxColor() = default;

public:
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void * pArg = nullptr) override;
	virtual _uint Update_GameObject(_float fDeltaTime) override;
	virtual _uint LateUpdate_GameObject(_float fDeltaTime) override;
	virtual HRESULT Render_GameObject() override;

private:
	HRESULT Add_Component();

public:
	static CBoxColor* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObj * Clone(void * pArg = nullptr) override;
	virtual void Free() override;

public:
	class CVIBufferTool*	m_pVIBufferCom = nullptr;
	class CTransformTool*	m_pTransformCom = nullptr;
	

};

