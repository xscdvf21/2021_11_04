#pragma once
#include "GameObj.h"
#include "ManagementTool.h"
#include "TransformTool.h"
#include "VIBufferTool.h"
#include "VIBuffer_CubeColorTool.h"


class CCubeLine :
	public CGameObj
{
private:
	explicit CCubeLine(LPDIRECT3DDEVICE9 pDevice);
	explicit CCubeLine(const CCubeLine& other);
	virtual ~CCubeLine() = default;

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
	static CCubeLine* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObj * Clone(void * pArg = nullptr) override;
	virtual void Free() override;

public:
	class CVIBufferTool*	m_pVIBufferCom = nullptr;
	class CTransformTool*	m_pTransformCom = nullptr;
	float m_fScaleAccel = 0.01;
	float m_fScale = 1.f;



public:
	HRESULT Set_ChangeColor(int _iA, int _iR, int _iG, int _iB, _uint _iIndex = 0);


};

