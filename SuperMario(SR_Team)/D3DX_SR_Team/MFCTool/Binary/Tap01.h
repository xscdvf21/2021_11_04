#pragma once
#include "afxcmn.h"
#include "afxwin.h"

#include "ManagementTool.h"		///////////

#include "CubeColor.h"

// CTap01 ��ȭ �����Դϴ�.

typedef struct CubeObject
{
	CubeObject()
	{
		D3DXMatrixIdentity(&m_matWorld);
	}

	//������Ʈ�� ���� ���.
	D3DXMATRIX m_matWorld;
	CString m_stCubeName;

	//Coloring Part
	int m_iAlpha;
	int m_iRed;
	int m_iGreen;
	int m_iBlue;

	//Texture Part
	CString m_stTextureName;

	//Etc
	D3DXVECTOR3 m_vBoxTotal;
	BOOL	m_bCollision;
	

}SCubeObject;

typedef struct tagTexInfo
{
	LPDIRECT3DTEXTURE9 pTexture; // �ؽ��� ������ ����-> �İ�ü. 
	D3DXIMAGE_INFO tImageInfo; // �̹����� ������ ������ ���� ����ü. 
}TEXINFO;



class CTap01 : public CDialog
{
	DECLARE_DYNAMIC(CTap01)

public:
	CTap01(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CTap01();

public:
	int Get_DrawID() { return m_iDrawID; }

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TAP01 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

private:
	int m_iDrawID;

public:
	//Checking Option of Coloring Or Texture
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();

	// Coloring Part
	afx_msg void Control_AlphaValue(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void Control_RedValue(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void Control_GreenValue(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void Control_BlueValue(NMHDR *pNMHDR, LRESULT *pResult);
	
	//Texture Part
	void HorizontalScroll();
	afx_msg void OnLbnSelchangeListTexturefile();

	//Etc Part
	afx_msg void OnDeltaposSpinBoxx(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinBoxy(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinBoxz(NMHDR *pNMHDR, LRESULT *pResult);

	//Save and Load
	afx_msg void OnBnClickedButtonLoadTexture();
	afx_msg void OnEnChangeEditWritename();
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedButtonLoad();
	afx_msg void OnBnClickedButtonRemove();



	void Set_CubeSize();
	void Set_CubeVertex();

private:
	int iColoringStart;
	int iColoringEnd;
	int iColoringMoveNum;

	int iBoxMin;
	int iBoxMax;
	int iBoxX;
	int iBoxY;
	int iBoxZ;

	int iBoxSizeX;
	int iBoxSizeY;
	int iBoxSizeZ;

public:
	virtual BOOL OnInitDialog();

private:
	CButton m_iColoringTexture[2];

	//for Coloring
	CSliderCtrl m_sldAlpha;
	CSliderCtrl m_sldRed;
	CSliderCtrl m_sldGreen;
	CSliderCtrl m_sldBlue;

	CEdit m_eAlpha;
	CEdit m_eRed;
	CEdit m_eGreen;
	CEdit m_eBlue;

	//for Tecture
	CString m_stCubeName;

	//for Etc
	CSpinButtonCtrl m_spBoxX;
	CSpinButtonCtrl m_spBoxY;
	CSpinButtonCtrl m_spBoxZ;
	CEdit m_eBoxTotal;

	BOOL m_bCollision;

public:
	CStatic m_Picture;

private:
	CListBox m_ListBox;

	//ť���� ������
	D3DXVECTOR3 m_vStartPoint;

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


	map<CString, SCubeObject*> m_mapCubeInfo;
public:
	afx_msg void OnDropFiles(HDROP hDropInfo);
	CManagementTool* m_pManagementTool = nullptr;		///////////
	_Device	m_pDevice = nullptr;

	CEdit m_eCubeName;

	void Set_Cube();
	CGameObj*	m_pCube;
	CCubeColor*		m_pColorCube;
	bool		m_bCreateCube;
	afx_msg void OnBnClickedButtonRemove2();
};
