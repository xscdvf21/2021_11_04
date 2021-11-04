#pragma once
#include "GameObj.h"
#include "ManagementTool.h"
#include "TransformTool.h"
#include "VIBufferTool.h"
#include "VIBuffer_SliderColorTool.h"

class CSliderColor : public CGameObj
{
private:
	explicit CSliderColor(LPDIRECT3DDEVICE9 pDevice);
	explicit CSliderColor(const CSliderColor& other);
	virtual ~CSliderColor() = default;

public:
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void * pArg = nullptr) override;
	virtual _uint Update_GameObject(_float fDeltaTime) override;
	virtual _uint LateUpdate_GameObject(_float fDeltaTime) override;
	virtual HRESULT Render_GameObject() override;

private:
	HRESULT Add_Component();

public:
	static CSliderColor* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObj * Clone(void * pArg = nullptr) override;
	virtual void Free() override;

public:
	class CVIBufferTool*	m_pVIBufferCom = nullptr;
	class CTransformTool*	m_pTransformCom = nullptr;

public:
	//HRESULT Set_ChangeColor(int _iA, int _iR, int _iG, int _iB, _uint _iIndex = 0);


};

