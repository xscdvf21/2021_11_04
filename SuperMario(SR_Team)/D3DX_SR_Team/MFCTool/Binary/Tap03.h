#pragma once
#include "afxwin.h"
#include "afxcmn.h"

#include "CubeColor.h"
#include "RandomMgr.h"

// CTap03 대화 상자입니다.

typedef struct SimObject
{
	SimObject()
	{
		D3DXMatrixIdentity(&m_matWorld);
	}

	//오브젝트의 월드 행렬.
	D3DXMATRIX m_matWorld;

	//회전.
	float m_fRotX = 0.f;
	float m_fRotY = 0.f;
	float m_fRotZ = 0.f;
	float m_fRotation = 0.f;

	//크기
	float m_fScaleXStart = 0.f;
	float m_fScaleYStart = 0.f;
	float m_fScaleZStart = 0.f;

	float m_fScaleXEnd = 0.f;
	float m_fScaleYEnd = 0.f;
	float m_fScaleZEnd = 0.f;
	
	//라이프 타임.
	float m_fLifeTime = 0.f;
	float m_fDelay = 0.f;

	//중력
	float m_fGrvy = 0.f;

	//가속
	float m_fAccelX = 0;
	float m_fAccelY = 0;
	float m_fAccelZ = 0;

	//색깔.
	D3DXVECTOR3 m_vColorStart = {};
	D3DXVECTOR3 m_vColorEnd = {};


	//생성 큐브의 갯수
	int		m_iTotalNum = 0;

	//버텍스에 접근하여 컬러 값을 수정하는 함수 필요.
	//오브젝트의 버텍스의 개수를 반환하는 함수 필요.

}SGameObject;


class CTap03 : public CDialog
{
	DECLARE_DYNAMIC(CTap03)

public:
	CTap03(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CTap03();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TAP03 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	//아 왜 만들었지?
	CButton m_btctlAdd_Obj;// 오브젝트 추가 버튼
	CButton m_btctl_Del_Obj;// 오브젝트 삭제 버튼
	//아 왜 만들었지?

	CString m_strObjNameEC;// 오브젝트 이름을 담을 Edit Control
	CListBox m_lbcObjPlate;

	int m_iObjNumED; // 추가할 오브젝트 총 수
	
	afx_msg void OnBnClicked_AddObject();
	afx_msg void OnBnClicked_DeleteObject();
	afx_msg void OnBnClicked_Clear();
						
	CSliderCtrl m_ctlRedSLIDER;
	CSliderCtrl m_ctlGreenSLIDER;
	CSliderCtrl m_ctlBlueSLIDER;
	virtual BOOL OnInitDialog();
	
	//컬러
	afx_msg void CheckBox_BeginCol();
	afx_msg void CheckBox_EndCol();
	afx_msg void Slider_Red(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void Slider_Green(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void Slider_Blue(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClicked_Apply();
	CButton m_cbBeginCol;// 체크박스 컬러 begin
	CButton m_cbEndCol; // 체크박스 컬러 end
	int m_iRedED;
	int m_iGreenED;
	int m_iBlueED;
	int m_iRed_Begin = 0;
	int m_iGreen_Begin = 0;
	int m_iBlue_Begin = 0;
	int m_iRed_End = 0;
	int m_iGreen_End = 0;
	int m_iBlue_End = 0;


	//행렬을 위한 멤버 변수.
	D3DXMATRIX m_matWorld;//행렬은 입력받기 위한 것이 아님. 디버그용.
	float m_fm11ED;
	float m_fm12ED;
	float m_fm13ED;
	float m_fm21ED;
	float m_fm22ED;
	float m_fm23ED;
	float m_fm31ED;
	float m_fm32ED;
	float m_fm33ED;
	float m_fm41ED;
	float m_fm42ED;
	float m_fm43ED;


	CButton m_cbRotX;
	CButton m_cbRotY;
	CButton m_cbRotZ;
	float m_fRotMinED;
	float m_fRotMaxED;
	float m_fRotation;

	float m_fScaleXStartED;
	float m_fScaleYStartED;
	float m_fScaleZStartED;
	float m_fScaleXEndED;
	float m_fScaleYEndED;
	float m_fScaleZEndED;

	CButton m_cbGravityCheck;
	float m_fGravityMinED;
	float m_fGravityMaxED;

	float m_fLifeTimeMinED;
	float m_fLifeTimeMaxED;
	float m_fLifeTimeDelayED;

	//가속도 // 방향성도 가진다. // 최소, 최대.
	float m_fAccelXMinED;
	float m_fAccelYMinED;
	float m_fAccelZMinED;
	float m_fAccelXMaxED;
	float m_fAccelYMaxED;
	float m_fAccelZMaxED;
	float m_fDecel = 0.f;


	//SGameObject* m_pObj = nullptr;
	
	list<SGameObject*> m_plistObj;

	void			Showing_Cube(D3DXVECTOR3 _vShotting, float _RandomKey);

	CGameObj*		m_pCube;
	CGameObj*		m_pSampleCube;
	CCubeColor*		m_pColorCube;

	float fLifeTime;

	afx_msg void OnBnClickedBtnPlay();
	afx_msg void OnLbnSelchangeListObjectplate();
	CButton m_cbMultiColorCheck;
	float			m_fGravPower;
	D3DXVECTOR3		m_vShotting;

	CButton		m_iTotalNumRD[3];
	CButton		m_iConstOrRand[2];
	CButton		m_iType[3];

	float Random(float _MinVal, float _MaxVal, float _Df, float _Per);
	float Random(float _Val, float _Df);
	float m_fRandom;

	int m_iNumber = 0;
	list<CGameObj*> m_listObj;

	afx_msg void OnBnClicked_Save();
};
