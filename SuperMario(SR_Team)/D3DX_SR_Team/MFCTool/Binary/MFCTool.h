
// MFCTool.h : MFCTool ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.


// CMFCToolApp:
// �� Ŭ������ ������ ���ؼ��� MFCTool.cpp�� �����Ͻʽÿ�.
//

class CMFCToolApp : public CWinApp
{
public:
	CMFCToolApp();


// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnIdle(LONG lCount);
};

extern CMFCToolApp theApp;
