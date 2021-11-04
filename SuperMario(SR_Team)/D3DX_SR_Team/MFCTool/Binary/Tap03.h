#pragma once
#include "afxwin.h"
#include "afxcmn.h"

#include "CubeColor.h"
#include "RandomMgr.h"

// CTap03 ��ȭ �����Դϴ�.

typedef struct SimObject
{
	SimObject()
	{
		D3DXMatrixIdentity(&m_matWorld);
	}

	//������Ʈ�� ���� ���.
	D3DXMATRIX m_matWorld;

	//ȸ��.
	float m_fRotX = 0.f;
	float m_fRotY = 0.f;
	float m_fRotZ = 0.f;
	float m_fRotation = 0.f;

	//ũ��
	float m_fScaleXStart = 0.f;
	float m_fScaleYStart = 0.f;
	float m_fScaleZStart = 0.f;

	float m_fScaleXEnd = 0.f;
	float m_fScaleYEnd = 0.f;
	float m_fScaleZEnd = 0.f;
	
	//������ Ÿ��.
	float m_fLifeTime = 0.f;
	float m_fDelay = 0.f;

	//�߷�
	float m_fGrvy = 0.f;

	//����
	float m_fAccelX = 0;
	float m_fAccelY = 0;
	float m_fAccelZ = 0;

	//����.
	D3DXVECTOR3 m_vColorStart = {};
	D3DXVECTOR3 m_vColorEnd = {};


	//���� ť���� ����
	int		m_iTotalNum = 0;

	//���ؽ��� �����Ͽ� �÷� ���� �����ϴ� �Լ� �ʿ�.
	//������Ʈ�� ���ؽ��� ������ ��ȯ�ϴ� �Լ� �ʿ�.

}SGameObject;


class CTap03 : public CDialog
{
	DECLARE_DYNAMIC(CTap03)

public:
	CTap03(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CTap03();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TAP03 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	//�� �� �������?
	CButton m_btctlAdd_Obj;// ������Ʈ �߰� ��ư
	CButton m_btctl_Del_Obj;// ������Ʈ ���� ��ư
	//�� �� �������?

	CString m_strObjNameEC;// ������Ʈ �̸��� ���� Edit Control
	CListBox m_lbcObjPlate;

	int m_iObjNumED; // �߰��� ������Ʈ �� ��
	
	afx_msg void OnBnClicked_AddObject();
	afx_msg void OnBnClicked_DeleteObject();
	afx_msg void OnBnClicked_Clear();
						
	CSliderCtrl m_ctlRedSLIDER;
	CSliderCtrl m_ctlGreenSLIDER;
	CSliderCtrl m_ctlBlueSLIDER;
	virtual BOOL OnInitDialog();
	
	//�÷�
	afx_msg void CheckBox_BeginCol();
	afx_msg void CheckBox_EndCol();
	afx_msg void Slider_Red(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void Slider_Green(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void Slider_Blue(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClicked_Apply();
	CButton m_cbBeginCol;// üũ�ڽ� �÷� begin
	CButton m_cbEndCol; // üũ�ڽ� �÷� end
	int m_iRedED;
	int m_iGreenED;
	int m_iBlueED;
	int m_iRed_Begin = 0;
	int m_iGreen_Begin = 0;
	int m_iBlue_Begin = 0;
	int m_iRed_End = 0;
	int m_iGreen_End = 0;
	int m_iBlue_End = 0;


	//����� ���� ��� ����.
	D3DXMATRIX m_matWorld;//����� �Է¹ޱ� ���� ���� �ƴ�. ����׿�.
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

	//���ӵ� // ���⼺�� ������. // �ּ�, �ִ�.
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
