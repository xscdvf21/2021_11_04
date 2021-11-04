#pragma once
class CScene; 
class CScene_Manager
{
	DECLEARE_SINGLETON(CScene_Manager)
public:
	enum SCENEID {SCENE_START, SCENE_STAGEBRFORE,SCENE_STAGESTART, SCENE_STAGE,SCENE_STAGEPLAYING,SCENE_STAGEEND,SCENE_STAGE2, SCENE_WEAPONSTORE, SCENE_BOSS, SCENE_END};
private:
	CScene_Manager();
	~CScene_Manager();
public:
	HRESULT SceneChange_SceneManager(SCENEID eSceneID); 
	void Update_SceneManager();
	void Late_Update_SceneManager(); 
	void Render_SceneManager(); 
	void Release_SceneManager(); 
private:
	SCENEID m_eCurScene;
	SCENEID m_eNextScene; 
	CScene* m_pScene; 

};

