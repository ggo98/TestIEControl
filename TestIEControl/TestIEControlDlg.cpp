
// TestIEControlDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TestIEControl.h"
#include "TestIEControlDlg.h"
#include "afxdialogex.h"

#include <pathcch.h>
#pragma comment(lib, "pathcch.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTestIEControlDlg dialog



CTestIEControlDlg::CTestIEControlDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTestIEControlDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestIEControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EXPLORER1, m_Explorer1);
}

BEGIN_MESSAGE_MAP(CTestIEControlDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()

static void GetModuleFileName(BOOL exeIfHinstEqNULL, CString& dest, HINSTANCE Inst = NULL)
{
	LPTSTR p = NULL;
	int ret;
	dest.Empty();

	if (NULL == Inst)
		if (!exeIfHinstEqNULL)
			Inst = AfxGetInstanceHandle();

	p = dest.GetBuffer(_MAX_PATH + 16);
	ret = ::GetModuleFileName(Inst, p, _MAX_PATH);
	dest.ReleaseBuffer();
}

static void GetModulePathOnly(BOOL exeIfHinstEqNULL, CString& dest, HINSTANCE Inst = NULL)
{
	CString temp;
	CString Letter;
	CString Dir;
	CString File;
	CString Ext;

	dest.Empty();

	if (NULL == Inst)
		if (!exeIfHinstEqNULL)
			Inst = AfxGetInstanceHandle();

	GetModuleFileName(exeIfHinstEqNULL, temp, Inst);
	_tsplitpath(temp, Letter.GetBuffer(_MAX_PATH), Dir.GetBuffer(_MAX_PATH), File.GetBuffer(_MAX_PATH), Ext.GetBuffer(_MAX_PATH));
	Letter.ReleaseBuffer();
	Dir.ReleaseBuffer();
	dest = Letter + Dir;
}

static CString GetModulePathOnly(BOOL exeIfHinstEqNULL, HINSTANCE Inst = NULL)
{
	CString temp;
	GetModulePathOnly(exeIfHinstEqNULL, temp, Inst);
	return temp;
}  

// CTestIEControlDlg message handlers

BOOL CTestIEControlDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	WCHAR result[MAX_PATH];    
    HRESULT hr = PathCchCombine(result, MAX_PATH, GetModulePathOnly(true), _T("Page.htm"));
	m_Explorer1.Navigate(CString(_T("file:///")) + result, NULL, NULL, NULL, NULL);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTestIEControlDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTestIEControlDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BEGIN_EVENTSINK_MAP(CTestIEControlDlg, CDialogEx)
	ON_EVENT(CTestIEControlDlg, IDC_EXPLORER1, 250, CTestIEControlDlg::OnBeforenavigate2Explorer1, VTS_DISPATCH VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PBOOL)
END_EVENTSINK_MAP()


void CTestIEControlDlg::OnBeforenavigate2Explorer1(LPDISPATCH pDisp, VARIANT* URL, VARIANT* Flags, VARIANT* TargetFrameName, VARIANT* PostData, VARIANT* Headers, BOOL* Cancel)
{
    CString strURL = URL->bstrVal;

    if (strURL.Find(_T("http://")) == 0 || strURL.Find(_T("https://")) == 0)
    {
        *Cancel = VARIANT_TRUE;

        ShellExecute(NULL, _T("open"), strURL, NULL, NULL, SW_SHOWNORMAL);
	}
}
