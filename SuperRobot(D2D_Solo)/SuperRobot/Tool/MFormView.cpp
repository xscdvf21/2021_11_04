// MFormView.cpp : ���� �����Դϴ�.
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


// CMFormView �����Դϴ�.

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


// CMFormView �޽��� ó�����Դϴ�.


void CMFormView::OnBnClickedUnit()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (nullptr == m_tUnitTool.GetSafeHwnd())
		m_tUnitTool.Create(IDD_UNITTOOL);
	m_tUnitTool.ShowWindow(SW_SHOW);
	//ERR_MSG(L"dd"); 
}


void CMFormView::OnBnClickedButton2()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (nullptr == m_tMapTool.GetSafeHwnd())
		m_tMapTool.Create(IDD_MAPTOOL); 

	m_tMapTool.ShowWindow(SW_SHOW); 
}


void CMFormView::OnBnClickedPathExtraction()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (nullptr == m_tExtraction.GetSafeHwnd())
		m_tExtraction.Create(IDD_FILEPATHEXTRACTION);

	m_tExtraction.ShowWindow(SW_SHOW);

}
