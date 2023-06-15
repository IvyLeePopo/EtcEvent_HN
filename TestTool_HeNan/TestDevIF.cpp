// TestDevIF.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "TestDevIF.h"
#include "TestDevIFDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTestDevIFApp

BEGIN_MESSAGE_MAP(CTestDevIFApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CTestDevIFApp construction

CTestDevIFApp::CTestDevIFApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance

	m_iDataParamType = 2;
}


// The one and only CTestDevIFApp object

CTestDevIFApp theTestDevIFApp;


// CTestDevIFApp initialization

BOOL CTestDevIFApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	TCHAR szDir[256]={0};
	::GetModuleFileName(NULL, szDir, 256);
	CString strTemp(szDir);
	m_strCommDir = strTemp.Left(strTemp.ReverseFind(_T('\\')));

	CTestDevIFDlg dlg;
	//m_Dlg = new CTestDevIFDlg;
	m_pMainWnd = &dlg;

	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	if (NULL != CTestDevIFDlg::pThreadReceiveMsg)
	{
		CTestDevIFDlg::pThreadReceiveMsg->m_bExit = TRUE;
		::Sleep(100);

		int itry = 0;
		while (::WaitForSingleObject(CTestDevIFDlg::pThreadReceiveMsg->m_hThread, 200) != WAIT_OBJECT_0)
		{
			int nErrCode = ::GetLastError();
			if (6 == nErrCode) break;
			::Sleep(100);
			if (itry++ > 9) break;
		}
		CTestDevIFDlg::pThreadReceiveMsg = NULL;
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
