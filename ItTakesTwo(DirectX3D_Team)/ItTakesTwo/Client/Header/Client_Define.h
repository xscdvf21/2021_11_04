#ifndef Client_Define_h__
#define Client_Define_h__

extern HWND			g_hWnd;
extern HINSTANCE	g_hInst;
extern int			g_Character;
extern int			g_iFlower;
extern bool			g_bStart;
extern bool			g_bDebug;
extern bool			g_bMenu;
extern bool			g_bSlideFinal;
extern D3DXVECTOR3	g_vBackBufferColor;
extern int			g_iDebris;
extern bool			g_bRenderBox;
extern bool			g_bTriggerTest;
extern bool			g_bChinema;
extern int			g_iClearScene;
extern float		g_fEffecctSound;
extern int			g_iChannelID;

#define MOVE_UP			1<<1
#define MOVE_DOWN		1<<2
#define MOVE_RIGHT		1<<3
#define MOVE_LEFT		1<<4
#define JUMP			1<<5
#define SIT				1<<6
#define RUN				1<<7
#define DASH			1<<8
#define ATT				1<<9
#define BASIC			1<<10
#define ORB				1<<11
#define	ULTIMATE		1<<12
#define INTERACTION		1<<13
#define RIDE			1<<14
#define INTERACTION2	1<<15

#define SPONGEBOB	0
#define PATRICK		1
#define SANDY		2

#define GRAVITY		9.8f
#define STATIC_SHADOWLIGHT _vec3(3.f, 70.f, -3.f)
#define SHADOWLIGHT _vec3(3.f, 70.f, -3.f)

#define CHESSTILE_X 8
#define CHESSTILE_Y 8
#define CHESSTILE_CX 2.8f
#define CHESSTILE_CY 2.8f

#define DIRUP		0
#define DIRRIGHT	1
#define DIRDOWN		2
#define DIRLEFT		3

#define GRAVITY_INCORNER 0
#define GRAVITY_OUTCORNER 1

enum LAYERID
{
	LAYER_ENVIRONMENT,
	LAYER_GAMELOGIC,
	LAYER_UI,
	LAYER_END
};
enum LOADINGID
{
	LOADING_STATIC,
	LOADING_MENU,
	LOADING_CHAPTERSELECT,
	LOADING_MENUPLAYERSELECT,
	LOADING_VACUUM_FIELD,
	LOADING_VACUUM_FIELD2,
	LOADING_VACUUM_FIELD3,
	LOADING_VACUUM_BOSS,
	LOADING_CHESS_FIELD,
	LOADING_CHESS_BOSS,
	LOADING_SPACE_FIELD,
	LOADING_SPACE_FIELD2,
	LOADING_SPACE_FIELD3,
	LOADING_SPACE_BOSS,
	LOADING_TEST_FIELD,
	LOADING_ENDING,
	LOADING_END
};
enum SCENEID 
{ 
	SCENE_LOGO, 
	SCENE_MENUSELECT, 
	SCENE_CHAPTERSELECT, 
	SCENE_MENUPLAYERSELECT, 
	SCENE_VF,
	SCENE_VF2,
	SCENE_VF3,
	SCENE_VB, 
	SCENE_CF, 
	SCENE_CB, 
	SCENE_SF,
	SCENE_SF2,
	SCENE_SF3,
	SCENE_SB,
	SCENE_TF, 
	SCENE_ENDING, 
	SCENE_END 
};

enum BUTTONID
{
	BUTTON_DOOR,
	BUTTON_FIRE,
	BUTTON_BRIDGE,
	BUTTON_CRUSHER,
	BUTTON_END
};
enum MOVELOCK
{
	LOCK_LEFT = (1 << 0),
	LOCK_RIGHT = (1 << 1),
	LOCK_UP = (1 << 2),
	LOCK_DOWN = (1 << 2),
};

//// Global Function
//PxVec3 ToPxVec3(const _vec3& pVec3)
//{
//	return PxVec3(pVec3.x, pVec3.y, pVec3.z);
//}
//_vec3 ToVec3(const PxVec3& pVec3)
//{
//	return _vec3(pVec3.x, pVec3.y, pVec3.z);
//}
#define CHANGE_SCENE(_device,_SceneID,_hr)					\
		Engine::CScene*		pScene = nullptr;				\
		Engine::CParticle_Manager::GetInstance()->Reset();  \
		CLightMgr::GetInstance()->Clear_Light();			\
		pScene = CLogo::Create(_device, _SceneID);			\
		NULL_CHECK_RETURN(pScene, _hr);						\
		FAILED_CHECK_RETURN(Engine::Set_Scene(pScene, _device), _hr);\
		g_iChannelID = 0;

#define CHANGE_SCENE_POS(_device,_SceneID,_POS,_hr)			\
		Engine::CScene*		pScene = nullptr;				\
		Engine::CParticle_Manager::GetInstance()->Reset();	\
		CLightMgr::GetInstance()->Clear_Light();			\
		pScene = CLogo::Create(_device, _SceneID);			\
		NULL_CHECK_RETURN(pScene, _hr);						\
		pScene->Set_StartPos(_POS);							\
		FAILED_CHECK_RETURN(Engine::Set_Scene(pScene, _device), _hr);

#endif // Client_Define_h__