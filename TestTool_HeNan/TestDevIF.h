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

	//����Ŀ¼
	CString m_strCommDir;

	//������IP
	CString m_strServerIP;
	//�������˿�
	WORD	m_wServerPort;

	//ʡ�ݱ���
	int		m_iProvinceID;
	//�������
	CString		m_strAreaID;
	//·�α���
	CString		m_strRoadID;
	//վ�����
	CString		m_strStationID;
	//վ����
	CString     m_strStationName;
	//����
	CString     m_strLaneID;	
	//�������ͣ� 1:xml�� 2:json
	int m_iDataParamType;


	DECLARE_MESSAGE_MAP()
};

extern CTestDevIFApp theTestDevIFApp;