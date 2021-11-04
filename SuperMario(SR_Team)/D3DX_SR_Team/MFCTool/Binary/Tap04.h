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

// CTap04 ��ȭ �����Դϴ�.

class CTap04 : public CDialog
{
	DECLARE_DYNAMIC(CTap04)

public:
	CTap04(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CTap04();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_Tap04 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public: // ���̾�α� ���� �Լ�.
	virtual BOOL OnInitDialog();

#pragma region ���̾�α� ����.
public:
	afx_msg void OnBnClicked_Set();
	afx_msg void OnBnClicked_Delete();
	afx_msg void OnBnClicked_AllClear();
	afx_msg void OnBnClicked_Save();
	afx_msg void OnBnClicked_Load();

public:
	//������Ʈ ��ġ ����

	float m_fPosXXED;
	float m_fPosYYED;
	float m_fPosZZED;

	//������Ʈ ũ�� ����
	float m_fScaleXED;
	float m_fScaleYED;
	float m_fScaleZED;


	//���� Ȱ��ȭ üũ ����
	CButton m_ctlSnapCHECK;

	//���� ����


	//���⿡ ���� ���� ����.
	CButton m_ctlChainCHECK;
	int m_ivXED;
	int m_ivYED;
	int m_ivZED;
	int m_iCountED;

	//������Ʈ ����Ʈ ����
	afx_msg void OnLIST_Object();
	CListBox m_ctlObjectLIST;
	
	//�ؽ��� ����Ʈ ����
	afx_msg void OnLIST_Texture();
	CListBox m_ctlTextureLIST;

	//ȸ�� ����
	afx_msg void OnBnClicked_RotX();
	afx_msg void OnBnClicked_RotY();
	afx_msg void OnBnClicked_RotZ();
	afx_msg void OnBnClicked_MatrixInverse();
	
	//���̺� ����.
	CButton m_ctlAllSaveCB;
	//CButton m_ctlSelectSaveCB;
	afx_msg void CheckBox_AllSave();
	//afx_msg void CheckBox_SelectSave();

	//�̸� ����.
	CString m_strNameED;

	//���� ����.
	CButton m_ctlRedCB;
	CButton m_ctlGreenCB;
	CButton m_ctlBlueCB;
	
	//������Ʈ ����.
	CButton m_ctlCubeColorCB;
	CButton m_ctlCubeTextureCB;
	CButton m_ctlSliderColorCB;
	CButton m_ctlSliderTextureCB;
	CButton m_ctlEtcCB;

#pragma endregion
	
public:// �ѽ���
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
