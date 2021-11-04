#pragma once
#include "afxwin.h"
#include "MFCToolView.h"
#include "MainTool.h"
#include "GameObj.h"
#include "TransformTool.h"
#include "VIBuffer.h"
#include "GameObjMgr.h"
#include "ComponentTool.h"
#include "CubeTexture.h"


extern CMFCToolView*	g_pView;

// CTap04 대화 상자입니다.

class CTap04 : public CDialog
{
	DECLARE_DYNAMIC(CTap04)

public:
	CTap04(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CTap04();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_Tap04 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public: // 다이얼로그 고유 함수.
	virtual BOOL OnInitDialog();

#pragma region 다이얼로그 관련.
public:
	afx_msg void OnBnClicked_Set();
	afx_msg void OnBnClicked_Delete();
	afx_msg void OnBnClicked_AllClear();
	afx_msg void OnBnClicked_Save();
	afx_msg void OnBnClicked_Load();

public:
	//오브젝트 위치 관련

	float m_fPosXXED;
	float m_fPosYYED;
	float m_fPosZZED;

	//오브젝트 크기 관련
	float m_fScaleXED;
	float m_fScaleYED;
	float m_fScaleZED;


	//스넵 활성화 체크 관련
	CButton m_ctlSnapCHECK;

	//간격 관련


	//방향에 따라 연속 생성.
	CButton m_ctlChainCHECK;
	int m_ivXED;
	int m_ivYED;
	int m_ivZED;
	int m_iCountED;

	//오브젝트 리스트 관련
	afx_msg void OnLIST_Object();
	CListBox m_ctlObjectLIST;
	
	//텍스쳐 리스트 관련
	afx_msg void OnLIST_Texture();
	CListBox m_ctlTextureLIST;

	//회전 관련
	afx_msg void OnBnClicked_RotX();
	afx_msg void OnBnClicked_RotY();
	afx_msg void OnBnClicked_RotZ();
	afx_msg void OnBnClicked_MatrixInverse();
	
	//세이브 관련.
	CButton m_ctlAllSaveCB;
	//CButton m_ctlSelectSaveCB;
	afx_msg void CheckBox_AllSave();
	//afx_msg void CheckBox_SelectSave();

	//이름 관련.
	CString m_strNameED;

	//색상 관련.
	CButton m_ctlRedCB;
	CButton m_ctlGreenCB;
	CButton m_ctlBlueCB;
	
	//오브젝트 관련.
	CButton m_ctlCubeColorCB;
	CButton m_ctlCubeTextureCB;
	CButton m_ctlSliderColorCB;
	CButton m_ctlSliderTextureCB;
	CButton m_ctlEtcCB;

#pragma endregion
	
public:// 한승현
	CMainTool* m_pMainTool = nullptr;
	list<CGameObj*> m_listObj;
	CGameObj* m_pLineObj = nullptr;


	afx_msg void CheckBox_CubeColor();
	afx_msg void CheckBox_CubeTexture();
	afx_msg void CheckBox_SliderColor();
	afx_msg void CheckBox_SliderTexture();
	afx_msg void CheckBox_Etc();
	afx_msg void OnBnClicked_UpXSet();
	afx_msg void OnBnClicked_DownXSet();
	afx_msg void OnBnClicked_UpZSet();
	afx_msg void OnBnClicked_DownZSet();
	afx_msg void OnBnClicked_UpYSet();
	afx_msg void OnBnClicked_DownYSet();
	afx_msg void OnBnClicked_Fix();



	CButton m_ctlSetCheckerCB;

	bool TerrainChecker(CGameObj* Dst);
	bool TerrainChecker_Select(CGameObj * Dst, CGameObj * Src);

	int m_iCount = 0;
	CButton m_ctlStaticLoadCB;
	CButton m_ctlEraseCB;
};
