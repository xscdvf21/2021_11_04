#pragma once
#ifndef __GAMEOBJECT_MANAGER_H__

#include "Base.h"

BEGIN(Engine)
class CGameObject_Manager final : public CBase
{
	DECLARE_SINGLETON(CGameObject_Manager)

private :
	CGameObject_Manager();
	virtual ~CGameObject_Manager() = default;

public :
	class CGameObject*	Get_GameObject(_uint iSceneIndex, const wstring& LayerTag, _uint iIndex = 0);
	class CComponent*	Get_Component(_uint iSceneIndex, const wstring& LayerTag, const wstring& ComponentTag, _uint iIndex = 0);

public :
	HRESULT Ready_GameObject_Manager(_uint iSceneCount);
	HRESULT Add_GameObject_Prototype(_uint iSceneIndex, const wstring& PrototypeTag, class CGameObject* pPrototype);
	HRESULT Clone_GameObject_ToLayer(_uint iFromSceneIndex, const wstring& PrototypeTag, _uint iToSceneIndex, const wstring& LayerTag, void* pArg = nullptr);
	CGameObject * Clone_GameObject_For_Load(_uint iFromSceneIndex, const wstring & PrototypeTag, _uint iToSceneIndex, const wstring & LayerTag, void * pArg);
	void Clear_ForScene(_uint iSceneIndex);

public:
	_uint Update_GameObject(_float fDeltaTime);
	_uint LateUpdate_GameObject(_float fDeltaTime);
	

public://get
	void Set_GameObjectLayer(_uint iLayerSelect) { m_iLayerSelect = iLayerSelect; }

public :
	virtual void Free() override;

private :
	typedef unordered_map<wstring, class CGameObject*> GAMEOBJECTS; //������Ʈ ������ ������ �����̳�.
	GAMEOBJECTS*	m_pGameObjects = nullptr; //�����迭

	//���̾���� ������ �ʿ��� ������Ʈ�� �������� ������
	typedef unordered_map<wstring, class CLayer*> LAYERS; //���̾ ������ �����̳�
	LAYERS*			m_pLayers = nullptr; //�����迭

	_uint			m_iSceneCount = 0; //���� ������ ī����
	_uint			m_iLayerSelect = 0;
};
END

#define	__GAMEOBJECT_MANAGER_H__
#endif // ! __GAMEOBJECT_MANAGER_H__
