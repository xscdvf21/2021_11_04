#pragma once
#include "Base.h"
#include "ComponentTool.h"

USING(Engine)
class CComponentMgr :
	public CBase
{
	DECLARE_SINGLETON(CComponentMgr)
private:
	CComponentMgr();
	virtual ~CComponentMgr() = default;

public:
	HRESULT Ready_Component_Manager();
	HRESULT Add_Component_Prototype(const wstring& PrototypeTag, CComponentTool* pPrototype);
	CComponentTool* Clone_Component(const wstring& PrototypeTag, void* pArg = nullptr);
	void Clear_ForScene(_uint iSceneIndex);

public:
	virtual void Free() override;

private:
	/* unordered_map: 해시 기반의 맵, 탐색속도가 어마어마하게 빠르다. 대신 키값 정렬이 없다. */
	typedef unordered_map<wstring, CComponentTool*> COMPONENTS; /* 컴포넌트들의 원형들을 보관할 컨테이너 */
	COMPONENTS*	m_pComponents = nullptr; /* 동적배열 */

	_uint		m_iSceneCount = 0;
};

