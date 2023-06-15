// TestDevIF.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols
#include "afxmt.h"

//#include "ThreadNetPay.h"


// CTestDevIFApp:
// See TestDevIF.cpp for the implementation of this class
//

class CTestDevIFDlg;

class CTestDevIFApp : public CWinApp
{
public:
	CTestDevIFApp();

// Overrides
	public:
	virtual BOOL InitInstance();
	//CTestDevIFDlg *m_Dlg;
// Implementation
	CMutex *g_clsMutex;

	//程序目录
	CString m_strCommDir;

	//服务器IP
	CString m_strServerIP;
	//服务器端口
	WORD	m_wServerPort;

	//省份编码
	int		m_iProvinceID;
	//区域编码
	CString		m_strAreaID;
	//路段编码
	CString		m_strRoadID;
	//站点编码
	CString		m_strStationID;
	//站名字
	CString     m_strStationName;
	//车道
	CString     m_strLaneID;	
	//参数类型， 1:xml， 2:json
	int m_iDataParamType;


	DECLARE_MESSAGE_MAP()
};

extern CTestDevIFApp theTestDevIFApp;