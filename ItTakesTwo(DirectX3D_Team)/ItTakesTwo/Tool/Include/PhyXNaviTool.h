#pragma once


// CPhyXNaviTool 대화 상자입니다.

class CPhyXNaviTool : public CDialogEx
{
	DECLARE_DYNAMIC(CPhyXNaviTool)

public:
	CPhyXNaviTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CPhyXNaviTool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TOOLPHYXNAVI };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
