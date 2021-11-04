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
	/* unordered_map: �ؽ� ����� ��, Ž���ӵ��� ���ϰ� ������. ��� Ű�� ������ ����. */
	typedef unordered_map<wstring, CComponentTool*> COMPONENTS; /* ������Ʈ���� �������� ������ �����̳� */
	COMPONENTS*	m_pComponents = nullptr; /* �����迭 */

	_uint		m_iSceneCount = 0;
};

