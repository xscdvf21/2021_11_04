#pragma once

#ifndef __MANAGEMENT_H__
#define __MANAGEMENT_H__

#include "Base.h"
#include "GraphicDevice.h"
#include "Renderer.h"
#include "Component_Manager.h"
#include "Key_Manager.h"
#include "GameObject_Manager.h"
#include "Time_Manager.h"
#include "Scene_Manager.h"
#include "SoundManager.h"

BEGIN(Engine)

class ENGINE_DLL CManagement final : public CBase
{
	DECLARE_SINGLETON(CManagement)


private:
	CManagement();
	virtual ~CManagement() = default;

public: /*For.General*/
	HRESULT Ready_Engine(HWND hWnd, int iWinCX, int iWinCY, EDisplayMode eDisplayMode, _uint iSceneCount);
	_uint Update_Engine();
	HRESULT Render_Engine(HWND hWnd = nullptr);

public: /*For.Graphic_Device*/
	_Device	Get_Device();
	LPD3DXFONT Get_Font();
	LPD3DXSPRITE Get_Sprite();

public : /*For. Time_Manager*/

public : /*For. Key_Manager*/
	_bool Key_Pressing(_uint iKey);
	_bool Key_Down(_uint iKey);
	_bool Key_Up(_uint iKey);

public : /*For. GameObject_Manager*/
	class CGameObject* Get_GameObject(_uint iSceneIndex, const wstring & LayerTag, _uint iIndex = 0);
	class CComponent* Get_Component(_uint iSceneIndex, const wstring & LayerTag, const wstring & ComponentTag, _uint iIndex = 0);
	HRESULT Add_GameObject_Prototype(_uint iSceneIndex, const wstring & PrototypeTag, class CGameObject* pPrototype);
	HRESULT Clone_GameObject_ToLayer(_uint iFromSceneIndex, const wstring & PrototypeTag, _uint iToSceneIndex, const wstring & LayerTag, void* pArg = nullptr);
	CGameObject * Clone_GameObject_For_Load(_uint iFromSceneIndex, const wstring & PrototypeTag, _uint iToSceneIndex, const wstring & LayerTag, void * pArg = nullptr);

	//게임 메니저에서 오브젝트를 삭제하는 함수.
	void Clear_ForScene(_uint iSceneIndex);

public: /*For.Renderer*/
	HRESULT Add_RenderList(ERenderPriority ePriority, class CGameObject* pGameObject);

public : /*For. Scene_Manager*/
	HRESULT Setup_CurrentScene(_int iSceneIndex, class CScene* pCurrentScene);
	HRESULT SetUp_ScnenSwap(_int iSceneIndex, class CScene* pCurrentScene);

public : /*For.Component_Manager*/
	HRESULT Add_Component_Prototype(_uint iSceneIndex, const wstring& PrototypeTag, CComponent* pPrototype);
	CComponent* Clone_Component(_uint iSceneIndex, const wstring& PrototypeTag, void* pArg = nullptr);

public: //SoundManager
	HRESULT Load_Sound();
	HRESULT Play_Sound(TCHAR* FileName, SOUND_CHANNELID ChannelID);
	HRESULT Stop_Sound(SOUND_CHANNELID ChannelID);
	HRESULT Play_BGM(TCHAR * FileName);
	HRESULT Stop_AllSound();


public:
	virtual void Free() override;
	static void Release_Engine();

public:
	const _int Get_DuplicateChecker() const;

private:	
	CGraphicDevice*			m_pDeviceManager = nullptr;
	CRenderer*				m_pRenderer = nullptr;
	CComponent_Manager*		m_pComponent_Manager = nullptr;
	CKey_Manager*			m_pKey_Manager = nullptr;
	CTime_Manager*			m_pTime_Manager = nullptr;
	CGameObject_Manager*	m_pGameObject_Manager = nullptr;
	CScene_Manager*			m_pScene_Manager = nullptr;
	CSoundManager*			m_pSound_Manager = nullptr;

};
END

#endif // !__MANAGEMENT_H__