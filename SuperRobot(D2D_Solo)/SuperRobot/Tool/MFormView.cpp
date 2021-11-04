// MFormView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "MFormView.h"


// CMFormView

IMPLEMENT_DYNCREATE(CMFormView, CFormView)

CMFormView::CMFormView()
	: CFormView(IDD_MFORMVIEW)
{

}

CMFormView::~CMFormView()
{
}

void CMFormView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFormView, CFormView)
	ON_BN_CLICKED(IDC_BUTTON1, &CMFormView::OnBnClickedUnit)
	ON_BN_CLICKED(IDC_BUTTON2, &CMFormView::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON7, &CMFormView::OnBnClickedPathExtraction)
END_MESSAGE_MAP()


// CMFormView 진단입니다.

#ifdef _DEBUG
void CMFormView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMFormView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMFormView 메시지 처리기입니다.


void CMFormView::OnBnClickedUnit()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (nullptr == m_tUnitTool.GetSafeHwnd())
		m_tUnitTool.Create(IDD_UNITTOOL);
	m_tUnitTool.ShowWindow(SW_SHOW);
	//ERR_MSG(L"dd"); 
}


void CMFormView::OnBnClickedButton2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (nullptr == m_tMapTool.GetSafeHwnd())
		m_tMapTool.Create(IDD_MAPTOOL); 

	m_tMapTool.ShowWindow(SW_SHOW); 
}


void CMFormView::OnBnClickedPathExtraction()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (nullptr == m_tExtraction.GetSafeHwnd())
		m_tExtraction.Create(IDD_FILEPATHEXTRACTION);

	m_tExtraction.ShowWindow(SW_SHOW);

}
