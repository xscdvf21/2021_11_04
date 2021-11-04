#pragma once
#include "Base.h"
#include "ComponentTool.h"
#include "ManagementTool.h"
USING(Engine)
class CGameObj :
	public CBase
{
protected:
	explicit CGameObj(LPDIRECT3DDEVICE9 pDevice);
	explicit CGameObj(const CGameObj& other);
	virtual ~CGameObj() = default;

public:
	class CComponentTool* Get_Component(const wstring& ComponentTag);

public:
	virtual HRESULT Ready_GameObject_Prototype() = 0;
	virtual HRESULT Ready_GameObject(void* pArg = nullptr) = 0;
	virtual _uint Update_GameObject(_float fDeltaTime) = 0;
	virtual _uint LateUpdate_GameObject(_float fDeltaTime) = 0;
	virtual HRESULT Render_GameObject() = 0;

protected:
	HRESULT Add_Component(
		const wstring& PrototypeTag,
		const wstring& ComponentTag,
		class CComponentTool ** ppOut = nullptr,
		void* pArg = nullptr);

public:
	virtual CGameObj* Clone(void* pArg = nullptr) = 0;
	virtual void Free() override;

protected:
	typedef unordered_map<wstring, class CComponentTool*>	COMPONENTS;
	COMPONENTS	m_Components;	/* 컴포넌트의 복제본들 보관 */

	LPDIRECT3DDEVICE9	m_pDevice;

public:
	bool m_bDead = false;
	_int m_iTextuer = 0;
};

