#pragma once
#include "Base.h"
#include "GameObj.h"

#pragma region CGameObjMgr 설명서
/*

Layer를 호출하지 않음.

*/
#pragma endregion

USING(Engine)
class CGameObjMgr :
	public CBase
{
	DECLARE_SINGLETON(CGameObjMgr)
public:
	CGameObjMgr();
	~CGameObjMgr() = default;

public:
	_uint Update_GameObject(_float fDeltaTime);
	_uint LateUpdate_GameObject(_float fDeltaTime);
	_uint Render_GameObject();

public:
	HRESULT Ready_GameObject_Manager();
	HRESULT Add_GameObject_Prototype(const wstring& PrototypeTag, class CGameObj* pPrototype);
	HRESULT Clone_GameObject_ToLayer(const wstring& PrototypeTag, void* pArg = nullptr);
	CGameObj* Get_Clone_GameObject(const wstring& PrototypeTag, void* pArg = nullptr);

	HRESULT Set_Clone(CGameObj* pObj);

public:
	virtual void Free() override;

private:
	//기존.
	typedef unordered_map<wstring, class CGameObj*> GAMEOBJECTS; /* 오브젝트들의 원형들을 보관할 컨테이너 */
	GAMEOBJECTS*	m_pGameObjects = nullptr; /* 동적배열 */

	//추가 클론
	typedef list<class CGameObj*>	GAMEOBJECTSCLONE;
	GAMEOBJECTSCLONE m_GameObjClone;	/* 오브젝트들의 복제본 보관 */



	LPDIRECT3DDEVICE9 m_pDevice = nullptr;

	//_uint			m_iSceneCount = 0;
};

