// TestDevIFDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TestDevIF.h"
#include "TestDevIFDlg.h"
#include "json/json.h"
#include <sstream>
#include "JsonParamOperate.h"

#include "CodeConv.h"

#include "Markup.h"

#include "mdump.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CTestDevIFDlg *g_DevIFDlg;

#define FILE_NAME_INIT_CONFIG   _T("HTSMConfig.ini")		//初始化配置文件名

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CTestDevIFDlg dialog

int CTestDevIFDlg::No = 1000;
void* CTestDevIFDlg::m_pIndPtr = new char[2048];

int CTestDevIFDlg::VehPlateTail = 100;

CThreadReceiveRespMsg* CTestDevIFDlg::pThreadReceiveMsg = NULL;



CTestDevIFDlg::CTestDevIFDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestDevIFDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	bLoaded = false;

	CString TimeStr = GetUniqueTimeStr();
	std::string timestr(TimeStr.GetBuffer());
	m_EventNo = timestr + "33b2b009e44b151000";

    m_EventTypeNo = 1;
	m_EventHandleMethodNo = 1;

	//load_js();	
	//m_Fun = MyCallBackFun;
	theTestDevIFApp.g_clsMutex = new CMutex(false, _T("abc"));

	m_pGuiPlay1Thread = NULL;
	m_pGuiPlay2Thread = NULL;
	m_pGuiPlay3Thread = NULL;
	m_pGuiPlayTotalThread = NULL;
	m_bThread1Running = false;
	m_bThread2Running = false;
	m_bThread3Running = false;
	m_bThreadTotalRunning = false;
	ReceiveMsgThreadID = 0;
}

void CTestDevIFDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, edt_info);
	DDX_Control(pDX, IDC_EDIT_EtcEventParam, EditParam);
}

BEGIN_MESSAGE_MAP(CTestDevIFDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP

	ON_BN_CLICKED(IDC_BTN_INITENV, &CTestDevIFDlg::OnBnClickedBtnLoadDll)
	ON_BN_CLICKED(IDC_BTN_INITENV2, &CTestDevIFDlg::OnBnClickedBtnInitEtcEventEnv)
	ON_BN_CLICKED(IDC_BTN_EVENTSTART, &CTestDevIFDlg::OnBnClickedBtnEventstart)
	ON_BN_CLICKED(IDC_BTN_CHECKVEHQUEUE, &CTestDevIFDlg::OnBnClickedBtnCheckvehqueue)
	ON_BN_CLICKED(IDC_BTN_CHECKVEHINFO, &CTestDevIFDlg::OnBnClickedBtnCheckvehinfo)
	ON_BN_CLICKED(IDC_BTN_SHOWFEEINFO, &CTestDevIFDlg::OnBnClickedBtnShowfeeinfo)
	ON_BN_CLICKED(IDC_BTN_CHECKENTRYINFO, &CTestDevIFDlg::OnBnClickedBtnCheckentryinfo)
	ON_BN_CLICKED(IDC_BTN_PAYRESULTSHOW,&CTestDevIFDlg::OnBnClickedBtnPayresultshow)

	ON_BN_CLICKED(IDC_BTN_EVENTSTOP, &CTestDevIFDlg::OnBnClickedBtnEventstop)
	ON_BN_CLICKED(IDC_BTN_STARTSCAN, &CTestDevIFDlg::OnBnClickedBtnStartscan)
	ON_BN_CLICKED(IDC_BTN_STOPSCAN, &CTestDevIFDlg::OnBnClickedBtnStopscan)
	ON_BN_CLICKED(IDC_BTN_SHOWFEEINFOSINGLE, &CTestDevIFDlg::OnBnClickedBtnShowfeeinfosingle)
	ON_BN_CLICKED(IDC_BTN_PAYRESULTSHOWFAIL, &CTestDevIFDlg::OnBnClickedBtnPayresultshowfail)
	ON_BN_CLICKED(IDC_BTN_CHECKVEHQUEUEDEL, &CTestDevIFDlg::OnBnClickedBtnCheckvehqueuedel)

	ON_MESSAGE(WM_THREAD_ETCEVENT_CALLBACK_SHOWLOG, OnMsgShowEtcEventLog)
	ON_MESSAGE(WM_HWND_ETCEVENT_MESSAGE_SHOWLOG, OnHWNDMsgShowEtcEventLog)
	ON_MESSAGE(WM_HWND_NETPAY_RESULT_SHOWLOG, OnHWNDMsgShowNetPayResultLog)

	ON_BN_CLICKED(IDC_BTN_DESTROY, &CTestDevIFDlg::OnBnClickedBtnDestroy)
	ON_BN_CLICKED(IDC_BTN_GETCOMPONENTSTATUS, &CTestDevIFDlg::OnBnClickedBtnGetcomponentstatus)
	ON_BN_CLICKED(IDC_BTN_GETLASTERROR, &CTestDevIFDlg::OnBnClickedBtnGetlasterror)
	ON_BN_CLICKED(IDC_BTN_GETLASTERROR2, &CTestDevIFDlg::OnBnClickedBtnGetlasterror2)
	ON_BN_CLICKED(IDC_BTN_CHECKENTRYINFO_NONE, &CTestDevIFDlg::OnBnClickedBtnCheckentryinfoNone)
	ON_BN_CLICKED(IDC_BTN_SWIPECARD, &CTestDevIFDlg::OnBnClickedBtnSwipecard)
	ON_BN_CLICKED(IDC_BTN_RWCARD, &CTestDevIFDlg::OnBnClickedBtnRwcard)
	ON_BN_CLICKED(IDC_BTN_FIXVEHQUEUE, &CTestDevIFDlg::OnBnClickedBtnFixvehqueue)
	ON_BN_CLICKED(IDC_BTN_init, &CTestDevIFDlg::OnBnClickedBtnInitNetPayEnv)
	ON_BN_CLICKED(IDC_BTN_debit, &CTestDevIFDlg::OnBnClickedBtnDebit)
	ON_BN_CLICKED(IDC_BTN_cancel, &CTestDevIFDlg::OnBnClickedBtnCancel)
	ON_BN_CLICKED(IDC_BTN_FeeAuthorize, &CTestDevIFDlg::OnBnClickedBtnFeeAuthorize)
	ON_BN_CLICKED(IDC_BTN_DestroyEtcEvent, &CTestDevIFDlg::OnBnClickedBtnDestroyEtcEvent)
	ON_BN_CLICKED(IDC_BTN_DestroyNetPay, &CTestDevIFDlg::OnBnClickedBtnDestroyNetPay)
	ON_BN_CLICKED(IDC_BTN_GetLastErrorDesc, &CTestDevIFDlg::OnBnClickedBtnGetLastErrorDesc)
	ON_BN_CLICKED(IDC_BTN_FreeDll, &CTestDevIFDlg::OnBnClickedBtnFreeDll)
	ON_BN_CLICKED(IDC_BTN_DebitXML, &CTestDevIFDlg::OnBnClickedBtnDebitXML)
	ON_BN_CLICKED(IDC_BTN_CancelXML, &CTestDevIFDlg::OnBnClickedBtnCancelXML)
	ON_BN_CLICKED(IDC_BTN_EtcDevStatus, &CTestDevIFDlg::OnBnClickedBtnEtcdevstatus)
	ON_BN_CLICKED(IDC_BTN_NetPayDevStatus, &CTestDevIFDlg::OnBnClickedBtnNetpaydevstatus)
	ON_BN_CLICKED(IDC_BTN_GUIPlay1Start, &CTestDevIFDlg::OnBnClickedBtnGuiPlay1Start)
	ON_BN_CLICKED(IDC_BTN_GUIPlay1Stop, &CTestDevIFDlg::OnBnClickedBtnGuiPlay1Stop)
	ON_BN_CLICKED(IDC_BTN_GUIPlay2Start, &CTestDevIFDlg::OnBnClickedBtnGuiPlay2Start)
	ON_BN_CLICKED(IDC_BTN_GUIPlay2Stop, &CTestDevIFDlg::OnBnClickedBtnGuiPlay2Stop)
	ON_BN_CLICKED(IDC_BTN_GUIPlay3Start, &CTestDevIFDlg::OnBnClickedBtnGuiPlay3Start)
	ON_BN_CLICKED(IDC_BTN_GUIPlay3Stop, &CTestDevIFDlg::OnBnClickedBtnGuiPlay3Stop)
	ON_BN_CLICKED(IDC_BTN_GUIPlayTotalStart, &CTestDevIFDlg::OnBnClickedBtnGuiPlayTotalStart)
	ON_BN_CLICKED(IDC_BTN_GUIPlayTotalStop, &CTestDevIFDlg::OnBnClickedBtnGuiPlayTotalStop)
	ON_BN_CLICKED(IDC_BTN_GUIPlayTotalStop2, &CTestDevIFDlg::OnBnClickedBtnGuiPlayTotalStop2)
	ON_BN_CLICKED(IDC_BTN_Authorize, &CTestDevIFDlg::OnBnClickedBtnAuthorize)
	ON_BN_CLICKED(IDC_BTN_InvoiceDataTrans, &CTestDevIFDlg::OnBnClickedBtnInvoiceDataTrans)
	ON_BN_CLICKED(IDC_BTN_InvoiceDataCancel, &CTestDevIFDlg::OnBnClickedBtnInvoiceDataCancel)
	ON_BN_CLICKED(ID_BTN_Change2ETCMode, &CTestDevIFDlg::OnBnClickedBtnChange2EtcMode)
	ON_BN_CLICKED(IDC_BTN_Change2MTCMode, &CTestDevIFDlg::OnBnClickedBtnChange2MtcMode)
	ON_BN_CLICKED(IDC_BTN_DelVehQueueResult, &CTestDevIFDlg::OnBnClickedBtnDelVehQueueResult)
	ON_BN_CLICKED(IDC_BTN_MtcClearScreen, &CTestDevIFDlg::OnBnClickedBtnMtcClearScreen)
	ON_BN_CLICKED(IDC_BTN_MtcClearScreenXML, &CTestDevIFDlg::OnBnClickedBtnMtcClearScreenXML)
	ON_BN_CLICKED(IDC_BTN_EtcGetLastErrorDescrip, &CTestDevIFDlg::OnBnClickedBtnEtcGetLastErrorDescrip)
END_MESSAGE_MAP()


// CTestDevIFDlg message handlers

BOOL CTestDevIFDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//MiniDump();



	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	g_DevIFDlg = this;

	SetDlgItemText(IDC_EDIT_Money, _T("1"));
	SetDlgItemText(IDC_EDIT_Weight, _T("1234"));

	CComboBox *pCombo = (CComboBox *)GetDlgItem(IDC_COMBO_type);
	pCombo->SetCurSel(1);

	SetDlgItemText(IDC_EDIT_axisnum, _T("2"));
	SetDlgItemText(IDC_EDIT_plate, _T("粤A12345"));
	SetDlgItemText(IDC_EDIT_entry, _T("柳湾"));
	SetDlgItemText(IDC_EDIT_timeout, _T("30"));
	SetDlgItemText(IDC_EDIT_operator, _T("收费员"));
	SetDlgItemText(IDC_EDIT_operatorID, _T("13579"));
	SetDlgItemText(IDC_EDIT_balance, _T("9999"));
	SetDlgItemText(IDC_EDIT_cardno, _T("999988887777"));

	((CButton *)GetDlgItem(IDC_RADIO_DefaultParam))->SetCheck(TRUE);	//选上默认的
	((CButton *)GetDlgItem(IDC_RADIO_FreeParam))->SetCheck(FALSE);

	log("开始读取移动支付配置信息");	
	//读取移动支付共享库的配置信息
	char szIP[256] = {0};
	CString strCfgFilePath;
	strCfgFilePath.Format(_T("%s\\TWSDNetPayConfig.ini"), theTestDevIFApp.m_strCommDir);
	::GetPrivateProfileString(_T("ReflectWeb"), _T("IP"), _T("101.200.214.203"), szIP, _countof(szIP), strCfgFilePath);
	m_strServerIP.Format(_T("%s"), szIP);
	m_wServerPort = (WORD)::GetPrivateProfileInt(_T("ReflectWeb"), _T("Port"), 8081, strCfgFilePath);

	//读取测试工具的配置信息
	strCfgFilePath.Format(_T("%s\\TWNetPayTest.ini"), theTestDevIFApp.m_strCommDir);
	m_iProvinceID	= ::GetPrivateProfileInt(_T("Setup"), _T("ProvinceID"), 43, strCfgFilePath);
	m_iAreaID		= ::GetPrivateProfileInt(_T("Setup"), _T("AreaID"), 4412, strCfgFilePath);
	m_iRoadID		= ::GetPrivateProfileInt(_T("Setup"), _T("RoadID"), 30, strCfgFilePath);
	m_iStationID	= ::GetPrivateProfileInt(_T("Setup"), _T("StationID"), 9, strCfgFilePath);
	m_iLaneID       = ::GetPrivateProfileInt(_T("Setup"), _T("LaneID"), 1, strCfgFilePath);
	
	char chs[50] = {0};
	::GetPrivateProfileString(_T("Setup"), _T("StationName"), _T("广东路段"), chs, _countof(chs), strCfgFilePath);
	m_strStationName = chs;	
	::GetPrivateProfileString(_T("Setup"), _T("ProvinceID"), "43", chs, _countof(chs), strCfgFilePath);
	m_strProvinceID = chs;
	::GetPrivateProfileString(_T("Setup"), _T("AreaID"), "4412", chs, _countof(chs), strCfgFilePath);
	m_strAreaID = chs;
	::GetPrivateProfileString(_T("Setup"), _T("RoadID"), "30", chs, _countof(chs), strCfgFilePath);
	m_strRoadID = chs;
	::GetPrivateProfileString(_T("Setup"), _T("StationID"), "9", chs, _countof(chs), strCfgFilePath);
	m_strStationID = chs;
	::GetPrivateProfileString(_T("Setup"), _T("LaneID"), "1", chs, _countof(chs), strCfgFilePath);
	m_strLaneID = chs;	

	theTestDevIFApp.m_strServerIP	= m_strServerIP;
	theTestDevIFApp.m_wServerPort    = m_wServerPort;
    theTestDevIFApp.m_iProvinceID	= m_iProvinceID;
	theTestDevIFApp.m_strAreaID		= m_strAreaID;
	theTestDevIFApp.m_strRoadID		= m_strRoadID;
	theTestDevIFApp.m_strStationID	= m_strStationID;
	theTestDevIFApp.m_strLaneID      = m_strLaneID;
	theTestDevIFApp.m_strStationName = m_strStationName;

	CString strCfgInfo;
	strCfgInfo.Format(_T("ProvinceID: %s, AreaID: %s, RoadID: %s, StationID: %s, LaneID: %s"), m_strProvinceID, m_strAreaID, m_strRoadID, m_strStationID, m_strLaneID);
	//DisplayInfo(strCfgInfo);
	log(strCfgInfo.GetBuffer(0));

	m_bNetPayEnvInitedFlag = false;
	m_bEtcEventEnvInitedFlag = false;

	//启动线程	
	pThreadReceiveMsg = (CThreadReceiveRespMsg *)::AfxBeginThread(RUNTIME_CLASS(CThreadReceiveRespMsg), THREAD_PRIORITY_NORMAL);
	if(pThreadReceiveMsg != NULL)
	{
		log(_T("线程启动成功"));
	}
	else
	{
		log(_T("线程启动失败"));
	}	

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTestDevIFDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTestDevIFDlg::OnPaint()
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
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTestDevIFDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

LRESULT CTestDevIFDlg::OnHWNDMsgShowNetPayResultLog(WPARAM wParam, LPARAM lParam)
{
	if(wParam != 0)
	{
		//获取结果
		BYTE byFunCode		= LOBYTE(LOWORD(wParam));
		BYTE byExecuteState = HIBYTE(LOWORD(wParam));

		CString strTip;
		strTip.Format(_T("功能代码:%d 执行状态:%d"), byFunCode, byExecuteState);
		log(strTip.GetBuffer(0));
		
		switch (byFunCode)
		{
			case 1:		//扣费回馈
			{
				if(byExecuteState == 0)
					strTip.Format(_T(">>>>>>收到移动支付 扣费结果：扣费成功"));
				else if(byExecuteState == 1)
					strTip.Format(_T(">>>>>>收到移动支付 扣费结果：正在输入密码..."));
				else
					strTip.Format(_T(">>>>>>收到移动支付 扣费结果：扣费失败"));
			
				log(strTip.GetBuffer(0));
			}
			break;
			case 2:  //撤单回馈
			{
				if(byExecuteState == 0)
					strTip.Format(_T(">>>>>>收到移动支付 撤单结果：撤单成功"));
				else
					strTip.Format(_T(">>>>>>收到移动支付 撤单结果：撤单失败"));
			
				log(strTip.GetBuffer(0));
			}
			break;
		}
	}
	return 0;
}



void CTestDevIFDlg::load_js()
{
	log("开始加载TWSDNetPay.dll库......");

	fn_EventInitEnvironment3 = NULL;
	fn_EventDestroy = NULL;
	fn_EventGetLastErrorDesc = NULL;
	fn_EventGetComponentStatus = NULL;
	fn_EventDealStart = NULL;
	fn_EventCheckVehQueue = NULL;
	fn_EventCheckVehInfo = NULL;
	fn_EventCheckEntryInfo = NULL;
	fn_EventShowFeeInfo = NULL;
	fn_EventPayResultDisplay = NULL;
	fn_EventDealStop = NULL;
	fn_EventStartScan = NULL;
	fn_EventStopScan = NULL;	
	fn_EventCardOperationNotify = NULL;
	fn_EventModifyVehQueue = NULL;
	fn_EventFeeAuthorize = NULL;
	fn_EventAuthorize = NULL;
	fn_EventDelVehQueueResult = NULL;

	fn_InitEnvironment = NULL;
	fn_DebitMoney  = NULL;
	fn_DebitCancel = NULL;
	fn_GetLastErrorDesc = NULL;
	fn_Destroy = NULL;
	fn_GetComponentStatus = NULL;
	fn_TransferCashInvoiceData = NULL;
	fn_CancelCashInvoiceData = NULL;
	fn_SetMMI = NULL;
	
	gd_dll = ::LoadLibrary("TWSDNetPay.dll");
	//gd_dll = ::LoadLibrary("TWSDNetPayCore.dll");
	if (gd_dll == NULL)
	{
		log("load TWSDNetPay.dll fail!!!");
		return;
	}

	fn_EventInitEnvironment3 = (IF_EventInitEnvironment3)::GetProcAddress(gd_dll,"IF_EventInitEnvironment3");
	fn_EventDestroy = (IF_EventDestroy)::GetProcAddress(gd_dll,"IF_EventDestroy");
	fn_EventGetLastErrorDesc = (IF_EventGetLastErrorDesc)::GetProcAddress(gd_dll,"IF_EventGetLastErrorDesc");
	fn_EventGetComponentStatus = (IF_EventGetComponentStatus)::GetProcAddress(gd_dll,"IF_EventGetComponentStatus");
	fn_EventDealStart = (IF_EventDealStart)::GetProcAddress(gd_dll,"IF_EventDealStart");
	fn_EventCheckVehQueue = (IF_EventCheckVehQueue)::GetProcAddress(gd_dll,"IF_EventCheckVehQueue");
	fn_EventCheckVehInfo = (IF_EventCheckVehInfo)::GetProcAddress(gd_dll,"IF_EventCheckVehInfo");
	fn_EventCheckEntryInfo = (IF_EventCheckEntryInfo)::GetProcAddress(gd_dll,"IF_EventCheckEntryInfo");
	fn_EventShowFeeInfo = (IF_EventShowFeeInfo)::GetProcAddress(gd_dll,"IF_EventShowFeeInfo");
	fn_EventPayResultDisplay = (IF_EventPayResultDisplay)::GetProcAddress(gd_dll,"IF_EventPayResultDisplay");
	fn_EventDealStop = (IF_EventDealStop)::GetProcAddress(gd_dll,"IF_EventDealStop");
	fn_EventStartScan = (IF_EventStartScan)::GetProcAddress(gd_dll,"IF_EventStartScan");
	fn_EventStopScan = (IF_EventStopScan)::GetProcAddress(gd_dll,"IF_EventStopScan");
	fn_EventCardOperationNotify = (IF_EventCardOperationNotify)::GetProcAddress(gd_dll,"IF_EventCardOperationNotify");
	fn_EventModifyVehQueue = (IF_EventModifyVehQueue)::GetProcAddress(gd_dll,"IF_EventModifyVehQueue");
	fn_EventFeeAuthorize = (IF_EventFeeAuthorize)::GetProcAddress(gd_dll,"IF_EventFeeAuthorize");
	fn_EventAuthorize = (IF_EventAuthorize)::GetProcAddress(gd_dll,"IF_EventAuthorize");
	fn_EventDelVehQueueResult = (IF_EventDelVehQueueResult)::GetProcAddress(gd_dll,"IF_EventDelVehQueueResult");	

	fn_InitEnvironment = (defIF_InitEnvironment)::GetProcAddress(gd_dll,"IF_InitEnvironment");
	fn_DebitMoney = (defIF_DebitMoney)::GetProcAddress(gd_dll,"IF_DebitMoney");
	fn_DebitCancel = (defIF_DebitCancel)::GetProcAddress(gd_dll,"IF_DebitCancel");
	fn_Destroy = (defIF_Destroy)::GetProcAddress(gd_dll,"IF_Destroy");
	fn_GetLastErrorDesc = (defIF_GetLastErrorDesc)::GetProcAddress(gd_dll,"IF_GetLastErrorDesc");
	fn_GetComponentStatus = (defIF_GetComponentStatus)::GetProcAddress(gd_dll,"IF_GetComponentStatus");
	fn_TransferCashInvoiceData = (defIF_TransferCashInvoiceData)::GetProcAddress(gd_dll,"IF_TransferCashInvoiceData");
	fn_CancelCashInvoiceData = (defIF_CancelCashInvoiceData)::GetProcAddress(gd_dll,"IF_CancelCashInvoiceData");
	fn_SetMMI = (defIF_SetMMI)::GetProcAddress(gd_dll,"IF_SetMMI");

	if(fn_EventInitEnvironment3 == NULL
	   || fn_EventDestroy == NULL
	   || fn_EventGetLastErrorDesc == NULL
	   || fn_EventGetComponentStatus == NULL
	   || fn_EventDealStart == NULL
	   || fn_EventCheckVehQueue == NULL
	   || fn_EventCheckVehInfo == NULL
	   || fn_EventCheckEntryInfo == NULL
	   || fn_EventShowFeeInfo == NULL
	   || fn_EventPayResultDisplay == NULL
	   || fn_EventDealStop == NULL
	   || fn_EventStartScan == NULL
	   || fn_EventStopScan == NULL
	   || fn_EventCardOperationNotify == NULL
	   || fn_EventModifyVehQueue == NULL
	   || fn_InitEnvironment == NULL
	   || fn_DebitMoney == NULL
	   || fn_DebitCancel == NULL
	   || fn_EventFeeAuthorize == NULL
	   || fn_EventAuthorize == NULL
	   || fn_Destroy == NULL
	   || fn_GetLastErrorDesc == NULL
	   || fn_GetComponentStatus == NULL
	   || fn_EventDelVehQueueResult == NULL
	   || fn_TransferCashInvoiceData == NULL
	   || fn_CancelCashInvoiceData == NULL
	   || fn_SetMMI == NULL)
	{
		log("加载TWSDNetPay.dll失败！！！");
		return;
	}

	log("加载TWSDNetPay.dll成功!");
	bLoaded = true;
}

void CTestDevIFDlg::log(std::string str)
{
	CString tmp;
	edt_info.GetWindowText(tmp);
	CString rt;
	rt.Format(_T("%s\r\n%s"),tmp,str.c_str());
	edt_info.SetWindowText(rt);
	edt_info.LineScroll (edt_info.GetLineCount(), 0);
}


std::string to_string(int i)
{
	std::stringstream ss;
	ss<<i;
	return ss.str();
}
int to_int(std::string str)
{
	if(str.empty())
	{
		return 0;
	}
	int i=0;
	sscanf_s(str.c_str(),"%d",&i);
	return i;
}


void CTestDevIFDlg::OnBnClickedBtnLoadDll()
{
	// TODO: 在此添加控件通知处理程序代码
	load_js();
}

void CTestDevIFDlg::OnBnClickedBtnInitEtcEventEnv()
{
	if(bLoaded == false)
	{
		log("未加载共享库!");
		return;
	}

	if(m_bEtcEventEnvInitedFlag == true)
	{
		log(_T("EtcEvent环境已初始化成功，请不要重复初始化!"));
		return;
	}

	//配置信息从配置文件中读取
	CString	m_strAppDir;
	m_strAppDir.Format(_T("%s"), GetCurrentDirNew());	
	CString	m_strConfigPathFile;
	m_strConfigPathFile.Format(_T("%s\\%s"), m_strAppDir, FILE_NAME_INIT_CONFIG);

	int m_provinceId;
	std::string m_areaId;
	std::string m_areaName = "区域名称";
	std::string m_roadId;
	std::string m_roadName = "路段名称";
	std::string m_stationId;
	std::string m_stationName;
	std::string m_laneId;
	char tmp_str[200]={0};

	::GetPrivateProfileString(_T("Setup"),	_T("AreaID"),"4412",tmp_str,100,m_strConfigPathFile);
	m_areaId = tmp_str;
	::GetPrivateProfileString(_T("Setup"),	_T("RoadID"),"2912",tmp_str,100,m_strConfigPathFile);
	m_roadId = tmp_str;
	::GetPrivateProfileString(_T("Setup"),	_T("StationID"),"29",tmp_str,100,m_strConfigPathFile);
	m_stationId = tmp_str;
	::GetPrivateProfileString(_T("Setup"),	_T("LaneID"),"36",tmp_str,100,m_strConfigPathFile);
	m_laneId = tmp_str;
	::GetPrivateProfileString(_T("Setup"),	_T("StationName"),"八斗",tmp_str,100,m_strConfigPathFile);
	m_stationName = tmp_str;

	m_provinceId = ::GetPrivateProfileInt(_T("Setup"), _T("ProvinceID"), 44, m_strConfigPathFile);

	//新增ETC特情配置项
	m_EventTypeNo = ::GetPrivateProfileInt(_T("EtcEventConfig"), _T("EventTypeNo"), 1, m_strConfigPathFile);
	m_EventHandleMethodNo = ::GetPrivateProfileInt(_T("EtcEventConfig"), _T("EventHandleMethodNo"), 1, m_strConfigPathFile);

	// TODO: 在此添加控件通知处理程序代码
//typedef bool (WINAPI *IF_InitEnvironment3)(IN const UINT& nThreadID,IN const HWND& hWnd,IN const char* szAreaInfo,IN const char* szLoaclStation,IN const char* szLoaclLaneID,IN const char* szServerInfo,IN const int& iProvinceID,IN OUT void* pIndPtr,void (*fun)(int option, int result,void* pIndPtr));
	UINT nThreadID = (UINT)123;
	HWND hWnd = (HWND)444;
	//char* szAreaInfo = "6301#广东#0029#广州北二环";
	char szAreaInfo[128] = {0};
	_snprintf_s(szAreaInfo, 127, "%s#%s#%s#%s", m_areaId.c_str(), m_areaName.c_str(), m_roadId.c_str(), m_roadName.c_str());
	log("区域信息:" + std::string(szAreaInfo));

	//char* szLoaclStation = "6300903#平安西";
	char szLoaclStation[48] = {0};
	_snprintf_s(szLoaclStation, 47, "%s#%s", m_stationId.c_str(), m_stationName.c_str());
	log("站信息:" + std::string(szLoaclStation));

	//char* szLoaclLaneID = "36";
	char* szServerInfo = "127.0.0.1";
	void* pIndPtr = m_pIndPtr;

	log("车道ID:" + m_laneId);
	log("省份ID:" + to_string(m_provinceId));

	bool bRet;
	//窗体句柄接收应答
	//bRet = fn_EventInitEnvironment3(0, this->m_hWnd, WM_HWND_ETCEVENT_MESSAGE_SHOWLOG, szAreaInfo, szLoaclStation, m_laneId.c_str(), szServerInfo, m_provinceId, pIndPtr, MyCallBackFun);
	//线程接收应答
	bRet = fn_EventInitEnvironment3(ReceiveMsgThreadID, 0, WM_THREAD_ETCEVENT_MESSAGE_SHOWLOG, szAreaInfo, szLoaclStation, m_laneId.c_str(), szServerInfo, m_provinceId, pIndPtr, MyCallBackFun);
	if(bRet)
	{
		log("ETC特情环境初始化成功!");
		m_bEtcEventEnvInitedFlag = true;
	}
	else
	{
		log("ETC特情环境初始化失败!");
		m_bEtcEventEnvInitedFlag = false;
	}
}

void CTestDevIFDlg::OnBnClickedBtnEventstart()
{
	if(bLoaded == false)
	{
		log("未加载共享库!");
		return;
	}

	if(m_bNetPayEnvInitedFlag == false || m_bEtcEventEnvInitedFlag == false)
	{
		log("未初始化环境!");
		return;
	}

	// TODO: 在此添加控件通知处理程序代码
	CString strJson;

	if (((CButton *)GetDlgItem(IDC_RADIO_FreeParam))->GetCheck())
	{
		EditParam.GetWindowText(strJson);
		strJson.TrimLeft();
		strJson.TrimRight();
		log("特情事件处理开始(使用自由配置参数):");
		log(strJson.GetBuffer(0));
	}

	if(((CButton *)GetDlgItem(IDC_RADIO_DefaultParam))->GetCheck())
	{
		// 表示整个 json 对象
		Json::Value root;
		// 标识ETC特情交互参数
		Json::Value EventDealStart;
		CString TimeStr = GetUniqueTimeStr();
		std::string timestr(TimeStr.GetBuffer());
		m_EventNo = timestr + "33b2b009e44b15" + to_string(++No);
		EventDealStart["SpecialEventNo"] = Json::Value(m_EventNo);
		EventDealStart["MessageCode"] = Json::Value("001");
		// 标识详细Data数据项
		Json::Value Data;		
		/*Data["ProvinceName"] = Json::Value("广东省");
		Data["ProvinceNo"] = Json::Value("44");
		Data["RoadName"] = Json::Value("广州北二环");
		Data["RoadNo"] = Json::Value("0093");
		Data["StationName"] = Json::Value("石湖收费站");
		Data["StationNo"] = Json::Value("009305");
		Data["LaneNo"] = Json::Value("36");*/
		CString str1 = "粤A123";
		CString temp;
		VehPlateTail++;
		temp.Format(_T("%d"), VehPlateTail);
		Data["VehPlate"] = Json::Value(str1 + temp);
		//Data["VehPlate"] = Json::Value(str1);
		m_CurrentVehPlate = str1 + temp;
		//m_CurrentVehPlate = str1;
		Data["VehPlateColor"] = Json::Value(1);
		m_CurrentVehPlateColor = 1;
		Data["VehClass"] = Json::Value(1);
		m_CurrentVehClass = 1;
		Data["VehType"] = Json::Value(1);
		m_CurrentVehType = 1;
		Data["VehSeatCount"] = Json::Value(7);
		int m_CurrentAxisCount = 2;
		Data["axleCount"] = Json::Value(2);
		Data["CardNum"] = Json::Value("123456");
		Data["VehSeatCount"] = Json::Value(7);
		//Data["EventStartTime"] = Json::Value("1970-01-01 08:00:00");
	
		Data["EventStartTime"] = Json::Value(MyGetCurrenTime());
		Data["EventDescribe"] = Json::Value("无入口信息");
		Data["EventTypeNo"] = Json::Value(m_EventTypeNo);
		Data["EventTypeName"] = Json::Value(GetEventTypeName(m_EventTypeNo));
		
		EventDealStart["Data"] = Data;
		root["EventDealStart"]	= EventDealStart;

		//转化为字符串
		Json::FastWriter fast_writer;
		std::string str = fast_writer.write(root).c_str();
		log("特情事件处理开始:");
		log(str);

		//utf8编码
		//char* strJsonU8 = G2U(str.c_str());
		//int iSize = (int)strlen(strJsonU8);
		
		//GBK编码
		strJson.Format(_T("%s"),str.c_str());
	}
	int iSize = strJson.GetLength();

	bool ret;
	ret = fn_EventDealStart(strJson.GetBuffer(0), iSize, 2);
	//ret = fn_EventDealStart(strJsonU8, iSize, 2);
	if(!ret)
	{
		log("调用特情事件处理开始接口返回失败!");
	}
	//delete[] strJsonU8;

	return;
}

void CTestDevIFDlg::OnBnClickedBtnCheckvehqueue()
{
	if(bLoaded == false)
	{
		log("未加载共享库!");
		return;
	}
	if(m_bNetPayEnvInitedFlag == false || m_bEtcEventEnvInitedFlag == false)
	{
		log("未初始化环境!");
		return;
	}
	// TODO: 在此添加控件通知处理程序代码
	CString strJson;

	if(((CButton *)GetDlgItem(IDC_RADIO_FreeParam))->GetCheck())
	{
		EditParam.GetWindowText(strJson);
		strJson.TrimLeft();
		strJson.TrimRight();
		log("车辆队列确认特情(使用自由配置参数):");
		log(strJson.GetBuffer(0));
	}
	if(((CButton *)GetDlgItem(IDC_RADIO_DefaultParam))->GetCheck())
	{
		// 表示整个 json 对象
		Json::Value root;
		// 标识ETC特情交互参数
		Json::Value EventCheckVehQueue;
		EventCheckVehQueue["SpecialEventNo"] = Json::Value(m_EventNo);
		EventCheckVehQueue["MessageCode"] = Json::Value("002");
		EventCheckVehQueue["CheckFlag"] = Json::Value(0);
		// 标识详细Data数据项
		Json::Value Data;		
		Data["ProvinceName"] = Json::Value("广东省");
		Data["ProvinceNo"] = Json::Value("44");
		Data["RoadName"] = Json::Value("广州北二环");
		Data["RoadNo"] = Json::Value("0094");
		Data["StationName"] = Json::Value("香雪收费站");
		Data["StationNo"] = Json::Value("009304");
		Data["LaneNo"] = Json::Value("1");
		CString str1 = "粤A123";
		CString temp;
		temp.Format(_T("%d"), VehPlateTail);
		Data["VehPlate"] = Json::Value(str1 + temp);
		Data["VehPlateColor"] = Json::Value(1);	
		Data["EventStartTime"] = Json::Value(MyGetCurrenTime());
		Data["EventDescribe"] = Json::Value("确认车辆队列是否正确");
		
		EventCheckVehQueue["Data"] = Data;
		root["EventCheckVehQueue"] = EventCheckVehQueue;

		//转化为字符串
		Json::FastWriter fast_writer;
		std::string str = fast_writer.write(root).c_str();
		log("车辆队列确认特情:");
		log(str);

		//utf8编码
		//char* strJsonU8 =  G2U(str.c_str());
		//int iSize = (int)strlen(strJsonU8);

		strJson.Format(_T("%s"),str.c_str());
	}
	int iSize = strJson.GetLength();

	bool ret;
	ret = fn_EventCheckVehQueue(strJson.GetBuffer(0), iSize, 2);
	//ret = fn_EventCheckVehQueue(strJsonU8, iSize, 2);
	if(!ret)
	{
		log("调用车辆队列确认接口返回失败!");
	}
	//delete[] strJsonU8;
	
	return;
}

void CTestDevIFDlg::OnBnClickedBtnCheckvehqueuedel()
{
	// TODO: 在此添加控件通知处理程序代码
	if(bLoaded == false)
	{
		log("未加载共享库!");
		return;
	}
	if(m_bNetPayEnvInitedFlag == false || m_bEtcEventEnvInitedFlag == false)
	{
		log("未初始化环境!");
		return;
	}
	// TODO: 在此添加控件通知处理程序代码
	CString strJson;
	if (((CButton *)GetDlgItem(IDC_RADIO_FreeParam))->GetCheck())
	{
		EditParam.GetWindowText(strJson);
		strJson.TrimLeft();
		strJson.TrimRight();
		log("取消车辆队列确认特情(使用自由配置参数):");
		log(strJson.GetBuffer(0));
	}

	if(((CButton *)GetDlgItem(IDC_RADIO_DefaultParam))->GetCheck())
	{
		// 表示整个 json 对象
		Json::Value root;
		// 标识ETC特情交互参数
		Json::Value EventCheckVehQueue;
		EventCheckVehQueue["SpecialEventNo"] = Json::Value(m_EventNo);
		EventCheckVehQueue["MessageCode"] = Json::Value("002");
		EventCheckVehQueue["CheckFlag"] = Json::Value(1);
		// 标识详细Data数据项
		Json::Value Data;		

		CString str1 = "粤A123";
		CString temp;
		temp.Format(_T("%d"), VehPlateTail);
		Data["VehPlate"] = Json::Value(str1 + temp);
		Data["VehPlateColor"] = Json::Value(1);	
		Data["EventStartTime"] = Json::Value(MyGetCurrenTime());
		Data["EventDescribe"] = Json::Value("取消确认车辆队列特情");
		
		EventCheckVehQueue["Data"] = Data;
		root["EventCheckVehQueue"] = EventCheckVehQueue;

		//转化为字符串
		Json::FastWriter fast_writer;
		std::string str = fast_writer.write(root).c_str();
		log("取消车辆队列确认特情:");
		log(str);

		//utf8编码
		//char* strJsonU8 =  G2U(str.c_str());
		//int iSize = (int)strlen(strJsonU8);

		strJson.Format(_T("%s"),str.c_str());
	}
	int iSize = strJson.GetLength();

	bool ret;
	ret = fn_EventCheckVehQueue(strJson.GetBuffer(0), iSize, 2);
	//ret = fn_EventCheckVehQueue(strJsonU8, iSize, 2);
	if(!ret)
	{
		log("调用车辆队列确认接口返回失败!");
	}
	//delete[] strJsonU8;

	return;
}


void CTestDevIFDlg::OnBnClickedBtnCheckvehinfo()
{
	if(bLoaded == false)
	{
		log("未加载共享库!");
		return;
	}
	if(m_bNetPayEnvInitedFlag == false || m_bEtcEventEnvInitedFlag == false)
	{
		log("未初始化环境!");
		return;
	}
	// TODO: 在此添加控件通知处理程序代码
	CString strJson;
	if (((CButton *)GetDlgItem(IDC_RADIO_FreeParam))->GetCheck())
	{
		EditParam.GetWindowText(strJson);
		strJson.TrimLeft();
		strJson.TrimRight();
		log("车辆信息确认特情(使用自由配置参数):");
		log(strJson.GetBuffer(0));
	}

	if(((CButton *)GetDlgItem(IDC_RADIO_DefaultParam))->GetCheck())
	{
		// 表示整个 json 对象
		Json::Value root;
		// 标识ETC特情交互参数
		Json::Value EventCheckVehInfo;
		EventCheckVehInfo["SpecialEventNo"] = Json::Value(m_EventNo);
		EventCheckVehInfo["MessageCode"] = Json::Value("003");
		// LaneInfo
		//Json::Value LaneInfo;
		//LaneInfo["ProvinceName"] = Json::Value("广东省");
		//LaneInfo["ProvinceNo"] = Json::Value("44");
		//LaneInfo["RoadName"] = Json::Value("广州北二环");
		//LaneInfo["RoadNo"] = Json::Value("0093");
		//LaneInfo["StationName"] = Json::Value("石湖收费站");
		//LaneInfo["StationNo"] = Json::Value("009305");
		//LaneInfo["LaneNo"] = Json::Value("36");
		//EventCheckVehInfo["LaneInfo"] = LaneInfo;
		
		//EventInfo
		Json::Value EventInfo;	
		EventInfo["VehPlateSnapImage"] = Json::Value("xxx");
		EventInfo["CameraSnapImage"] = Json::Value("xxx");
		EventInfo["RequestTime"] = Json::Value(MyGetCurrenTime());
		EventInfo["RequestType"] = Json::Value("004");
		EventInfo["EventDescribe"] = Json::Value("确认车辆信息是否正确");
		EventInfo["MMIDemand"] = Json::Value("交互诉求");
		EventInfo["TimeLimit"] = Json::Value("360");
		EventCheckVehInfo["EventInfo"] = EventInfo;

		//MMIContent
		Json::Value MMIContent;	
		CString str1 = "粤A123";
		CString temp;
		temp.Format(_T("%d"), VehPlateTail);
		MMIContent["VehPlate"] = Json::Value(str1 + temp);
		MMIContent["VehPlateColor"] = Json::Value(1);
		MMIContent["VehClass"] = Json::Value(1);
		MMIContent["VehType"] = Json::Value(1);
		MMIContent["AxisCount"] = Json::Value(2);
		MMIContent["EntryTime"] = Json::Value(MyGetCurrenTime());
		EventCheckVehInfo["MMIContent"] = MMIContent;

		root["EventCheckVehInfo"] = EventCheckVehInfo;

		//转化为字符串
		Json::FastWriter fast_writer;
		std::string str = fast_writer.write(root).c_str();
		log("车辆信息确认特情:");
		log(str);

		//utf8编码
		//char* strJsonU8 =  G2U(str.c_str());
		//int iSize = (int)strlen(strJsonU8);

		strJson.Format(_T("%s"),str.c_str());
	}
	int iSize = strJson.GetLength();
	
	bool bRet;
	bRet = fn_EventCheckVehInfo(strJson.GetBuffer(0), iSize, 2);
    //bRet = fn_EventCheckVehInfo(strJsonU8, iSize, 2);
	if(!bRet)
	{
		log("调用确认车辆信息接口返回失败!");
	}
	//delete[] strJsonU8;

	return;
}

void CTestDevIFDlg::OnBnClickedBtnShowfeeinfo()
{
	if(bLoaded == false)
	{
		log("未加载共享库!");
		return;
	}
	if(m_bNetPayEnvInitedFlag == false || m_bEtcEventEnvInitedFlag == false)
	{
		log("未初始化环境!");
		return;
	}

	// TODO: 在此添加控件通知处理程序代码
	CString strJson;
	if (((CButton *)GetDlgItem(IDC_RADIO_FreeParam))->GetCheck())
	{
		EditParam.GetWindowText(strJson);
		strJson.TrimLeft();
		strJson.TrimRight();
		log("费额显示(使用自由配置参数):");
		log(strJson.GetBuffer(0));
	}

	if(((CButton *)GetDlgItem(IDC_RADIO_DefaultParam))->GetCheck())
	{
		// 表示整个 json 对象
		Json::Value root;
		// 标识ETC特情交互参数
		Json::Value EventShowFeeInfo;
		EventShowFeeInfo["SpecialEventNo"] = Json::Value(m_EventNo);
		EventShowFeeInfo["MessageCode"] = Json::Value("005");
		//// LaneInfo
		//Json::Value LaneInfo;
		//LaneInfo["ProvinceName"] = Json::Value("广东省");
		//LaneInfo["ProvinceNo"] = Json::Value("44");
		//LaneInfo["RoadName"] = Json::Value("广州北二环");
		//LaneInfo["RoadNo"] = Json::Value("0094");
		//LaneInfo["StationName"] = Json::Value("石湖收费站");
		//LaneInfo["StationNo"] = Json::Value("009304");
		//LaneInfo["LaneNo"] = Json::Value("1");
		//EventShowFeeInfo["LaneInfo"] = LaneInfo;

		EventShowFeeInfo["PayType"] = Json::Value(1);
		EventShowFeeInfo["DefaultFeeItemNo"] = Json::Value(1);
		EventShowFeeInfo["FeeCount"] = Json::Value(2);

		Json::Value Array;
		Json::Value Item;
		Item["Entry"] = Json::Value("广东-莞深高速-石碣站");
		Item["PathInfo"] = Json::Value("0729-0730-0731");
		Item["Fee"] = Json::Value("3000");
		Item["ChargeType"] = Json::Value(2);
		Item["ItemNo"] = Json::Value(1);
		Array.append(Item);

		Item["Entry"] = Json::Value("广东-莞深高速-石碣站");
		Item["PathInfo"] = Json::Value("0729-0730-0731");
		Item["Fee"] = Json::Value("4000");
		Item["ChargeType"] = Json::Value(2);
		Item["ItemNo"] = Json::Value(2);
		Array.append(Item);	
		EventShowFeeInfo["FeeInfo"] = Array;

		EventShowFeeInfo["TotalVehNum"] = Json::Value(4);
		Json::Value VehQueue;
		Json::Value VehItem;
		VehItem["Index"] = Json::Value(0);
		VehItem["VehPlate"] = Json::Value("粤A121121");
		VehItem["VehPlateColor"] = Json::Value(1);
		VehItem["Fee"] = Json::Value("3000");
		VehItem["DealResult"] = Json::Value(0);
		VehItem["FailReason"] = Json::Value("");
		VehQueue.append(VehItem);

		VehItem["Index"] = Json::Value(1);
		VehItem["VehPlate"] = Json::Value("粤A122122");
		VehItem["VehPlateColor"] = Json::Value(1);
		VehItem["Fee"] = Json::Value("3000");
		VehItem["DealResult"] = Json::Value(0);
		VehItem["FailReason"] = Json::Value("");
		VehQueue.append(VehItem);

		VehItem["Index"] = Json::Value(2);
		VehItem["VehPlate"] = Json::Value("粤A123123");
		VehItem["VehPlateColor"] = Json::Value(1);
		VehItem["Fee"] = Json::Value("3000");
		VehItem["DealResult"] = Json::Value(1);
		VehItem["FailReason"] = Json::Value("余额不足");
		VehQueue.append(VehItem);

		VehItem["Index"] = Json::Value(3);
		VehItem["VehPlate"] = Json::Value("粤A124124");
		VehItem["VehPlateColor"] = Json::Value(1);
		VehItem["Fee"] = Json::Value("3000");
		VehItem["DealResult"] = Json::Value(2);
		VehItem["FailReason"] = Json::Value("");
		VehQueue.append(VehItem);
		EventShowFeeInfo["VehQueue"] = VehQueue;

		root["EventShowFeeInfo"] = EventShowFeeInfo;

		//转化为字符串
		Json::FastWriter fast_writer;
		std::string str = fast_writer.write(root).c_str();
		log("费额显示:");
		log(str);

		//utf8编码
		//char* strJsonU8 = G2U(str.c_str());
		//int iSize = (int)strlen(strJsonU8);

		strJson.Format(_T("%s"),str.c_str());
	}
	int iSize = strJson.GetLength();
	
	bool ret;
	ret = fn_EventShowFeeInfo(strJson.GetBuffer(0), iSize, 2);
	//ret = fn_EventShowFeeInfo(strJsonU8, iSize, 2);
	if(!ret)
	{
		log("调用费额显示接口返回失败!");
	}
	//delete[] strJsonU8;

	return;
}

void CTestDevIFDlg::OnBnClickedBtnCheckentryinfo()
{
	if(bLoaded == false)
	{
		log("未加载共享库!");
		return;
	}
	if(m_bNetPayEnvInitedFlag == false || m_bEtcEventEnvInitedFlag == false)
	{
		log("未初始化环境!");
		return;
	}

	// TODO: 在此添加控件通知处理程序代码
	CString strJson;
	if (((CButton *)GetDlgItem(IDC_RADIO_FreeParam))->GetCheck())
	{
		EditParam.GetWindowText(strJson);
		strJson.TrimLeft();
		strJson.TrimRight();
		log("车辆入口信息确认(使用自由配置参数):");
		log(strJson.GetBuffer(0));
	}

	if(((CButton *)GetDlgItem(IDC_RADIO_DefaultParam))->GetCheck())
	{
		// 表示整个 json 对象
		Json::Value root;
		// 标识ETC特情交互参数
		Json::Value EventCheckEntryInfo;
		EventCheckEntryInfo["SpecialEventNo"] = Json::Value(m_EventNo);
		EventCheckEntryInfo["MessageCode"] = Json::Value("005");
		CString str1 = "粤A123";
		CString temp;
		temp.Format(_T("%d"), VehPlateTail);
		EventCheckEntryInfo["VehPlate"] = Json::Value(str1 + temp);
		// LaneInfo
		//Json::Value LaneInfo;
		//LaneInfo["ProvinceName"] = Json::Value("广东省");
		//LaneInfo["ProvinceNo"] = Json::Value("44");
		//LaneInfo["RoadName"] = Json::Value("广州北二环");
		//LaneInfo["RoadNo"] = Json::Value("0094");
		//LaneInfo["StationName"] = Json::Value("石湖收费站");
		//LaneInfo["StationNo"] = Json::Value("009304");
		//LaneInfo["LaneNo"] = Json::Value("1");
		//EventCheckEntryInfo["LaneInfo"] = LaneInfo;
		
		//EventInfo
		Json::Value EventInfo;	
		EventInfo["RequestTime"] = Json::Value(MyGetCurrenTime());
		EventInfo["RequestType"] = Json::Value("004");
		EventInfo["EventDescribe"] = Json::Value("确认车辆信息是否正确");
		EventInfo["MMIDemand"] = Json::Value("交互诉求");
		EventInfo["TimeLimit"] = Json::Value("360");
		EventCheckEntryInfo["EventInfo"] = EventInfo;

		Json::Value Array;
		Json::Value Item;
		Item["StationName"] = Json::Value("香雪收费站");
		Item["StationID"] = Json::Value("G1501440010090");
		Item["StationHex"] = Json::Value("44011D0B");
		Item["laneid"] = Json::Value("0161");
		Item["laneHex"] = Json::Value("A1");
		Item["MediaType"] = Json::Value("1");
		Item["sEnTime"] = Json::Value("1970-01-01 08:00:00");
		Array.append(Item);
		Item["StationName"] = Json::Value("长平收费站");
		Item["StationID"] = Json::Value("G1501440010091");
		Item["StationHex"] = Json::Value("44011D0C");
		Item["laneid"] = Json::Value("0162");
		Item["laneHex"] = Json::Value("A2");
		Item["MediaType"] = Json::Value("1");
		Item["sEnTime"] = Json::Value("1970-01-01 08:00:00");
		Array.append(Item);
		EventCheckEntryInfo["EntryInfo"] = Array;

		root["EventCheckEntryInfo"] = EventCheckEntryInfo;

		//转化为字符串
		Json::FastWriter fast_writer;
		std::string str = fast_writer.write(root).c_str();
		log("车辆入口信息确认:");
		log(str);

		//utf8编码
		//char* strJsonU8 = G2U(str.c_str());
		//int iSize = (int)strlen(strJsonU8);

		strJson.Format(_T("%s"),str.c_str());
	}
	int iSize = strJson.GetLength();

	bool bRet;
	bRet = fn_EventCheckEntryInfo(strJson.GetBuffer(0), iSize, 2);
	//bRet = fn_EventCheckEntryInfo(strJsonU8, iSize, 2);
	if(!bRet)
	{
		log("调用确认车辆入口信息接口返回失败!");
	}
	//delete[] strJsonU8;

	return;
}

void CTestDevIFDlg::OnBnClickedBtnCheckentryinfoNone()
{
	// TODO: 在此添加控件通知处理程序代码
	if(bLoaded == false)
	{
		log("未加载共享库!");
		return;
	}
	if(m_bNetPayEnvInitedFlag == false || m_bEtcEventEnvInitedFlag == false)
	{
		log("未初始化环境!");
		return;
	}

	// TODO: 在此添加控件通知处理程序代码
	CString strJson;
	if (((CButton *)GetDlgItem(IDC_RADIO_FreeParam))->GetCheck())
	{
		EditParam.GetWindowText(strJson);
		strJson.TrimLeft();
		strJson.TrimRight();
		log("车辆入口信息确认(使用自由配置参数):");
		log(strJson.GetBuffer(0));
	}

	if(((CButton *)GetDlgItem(IDC_RADIO_DefaultParam))->GetCheck())
	{
		// 表示整个 json 对象
		Json::Value root;
		// 标识ETC特情交互参数
		Json::Value EventCheckEntryInfo;
		EventCheckEntryInfo["SpecialEventNo"] = Json::Value(m_EventNo);
		EventCheckEntryInfo["MessageCode"] = Json::Value("005");
		CString str1 = "粤A123";
		CString temp;
		temp.Format(_T("%d"), VehPlateTail);
		EventCheckEntryInfo["VehPlate"] = Json::Value(str1 + temp);

		// LaneInfo
		//Json::Value LaneInfo;
		//LaneInfo["ProvinceName"] = Json::Value("广东省");
		//LaneInfo["ProvinceNo"] = Json::Value("44");
		//LaneInfo["RoadName"] = Json::Value("广州北二环");
		//LaneInfo["RoadNo"] = Json::Value("0094");
		//LaneInfo["StationName"] = Json::Value("石湖收费站");
		//LaneInfo["StationNo"] = Json::Value("009304");
		//LaneInfo["LaneNo"] = Json::Value("1");
		//EventCheckEntryInfo["LaneInfo"] = LaneInfo;
		
		//EventInfo
		Json::Value EventInfo;	
		EventInfo["RequestTime"] = Json::Value(MyGetCurrenTime());
		EventInfo["RequestType"] = Json::Value("004");
		EventInfo["EventDescribe"] = Json::Value("确认车辆信息是否正确");
		EventInfo["MMIDemand"] = Json::Value("交互诉求");
		EventInfo["TimeLimit"] = Json::Value("360");

		EventCheckEntryInfo["EventInfo"] = EventInfo;

		Json::Value Array;
	/*
		Json::Value Item;
		Item["StationName"] = Json::Value("香雪收费站");
		Item["StationID"] = Json::Value("G1501440010090");
		Item["StationHex"] = Json::Value("44011D0B");
		Item["laneid"] = Json::Value("0161");
		Item["laneHex"] = Json::Value("A1");
		Item["sEnTime"] = Json::Value("1970-01-01 08:00:00");
		Array.append(Item);
		Item["StationName"] = Json::Value("长平收费站");
		Item["StationID"] = Json::Value("G1501440010091");
		Item["StationHex"] = Json::Value("44011D0C");
		Item["laneid"] = Json::Value("0162");
		Item["laneHex"] = Json::Value("A2");
		Item["sEnTime"] = Json::Value("1970-01-01 08:00:00");
		Array.append(Item);
	*/
		EventCheckEntryInfo["EntryInfo"] = Array;

		root["EventCheckEntryInfo"] = EventCheckEntryInfo;

		//转化为字符串
		Json::FastWriter fast_writer;
		std::string str = fast_writer.write(root).c_str();
		log("车辆入口信息确认:");
		log(str);

		//utf8编码
		//char* strJsonU8 = G2U(str.c_str());
		//int iSize = (int)strlen(strJsonU8);

		strJson.Format(_T("%s"),str.c_str());
	}
	int iSize = strJson.GetLength();

	bool bRet;
	bRet = fn_EventCheckEntryInfo(strJson.GetBuffer(0), iSize, 2);
	//bRet = fn_EventCheckEntryInfo(strJsonU8, iSize, 2);
	if(!bRet)
	{
		log("调用确认车辆入口信息接口返回失败!");
	}
	//delete[] strJsonU8;

	return;
}


void CTestDevIFDlg::OnBnClickedBtnPayresultshow()
{
	if(bLoaded == false)
	{
		log("未加载共享库!");
		return;
	}
	if(m_bNetPayEnvInitedFlag == false || m_bEtcEventEnvInitedFlag == false)
	{
		log("未初始化环境!");
		return;
	}

	//// TODO: 手动添加消息处理函数
	CString strJson;
	if (((CButton *)GetDlgItem(IDC_RADIO_FreeParam))->GetCheck())
	{
		EditParam.GetWindowText(strJson);
		strJson.TrimLeft();
		strJson.TrimRight();
		log("支付状态展示(使用自由配置参数):");
		log(strJson.GetBuffer(0));
	}

	if(((CButton *)GetDlgItem(IDC_RADIO_DefaultParam))->GetCheck())
	{
		// 表示整个 json 对象
		Json::Value root;
		// 标识ETC特情交互参数
		Json::Value EventPayResultShow;
		EventPayResultShow["SpecialEventNo"] = Json::Value(m_EventNo);
		EventPayResultShow["MessageCode"] = Json::Value("006");
		//// LaneInfo
		//Json::Value LaneInfo;
		//LaneInfo["ProvinceName"] = Json::Value("广东省");
		//LaneInfo["ProvinceNo"] = Json::Value("44");
		//LaneInfo["RoadName"] = Json::Value("广州北二环");
		//LaneInfo["RoadNo"] = Json::Value("0093");
		//LaneInfo["StationName"] = Json::Value("石湖收费站");
		//LaneInfo["StationNo"] = Json::Value("009305");
		//LaneInfo["LaneNo"] = Json::Value("36");
		//EventPayResultShow["LaneInfo"] = LaneInfo;
		
		//EventInfo
		Json::Value EventInfo;	
		EventInfo["RequestTime"] = Json::Value(MyGetCurrenTime());
		EventInfo["RequestType"] = Json::Value("004");
		EventInfo["PayResult"] = Json::Value(0);
		EventInfo["Describe"] = Json::Value("支付宝用户支付成功");
		EventInfo["TimeLimit"] = Json::Value("360");
		EventPayResultShow["EventInfo"] = EventInfo;
		
		root["EventPayResultShow"] = EventPayResultShow;

		//转化为字符串
		Json::FastWriter fast_writer;
		std::string str = fast_writer.write(root).c_str();
		log("支付状态展示:");
		log(str);

		//utf8编码
		//char* strJsonU8 = G2U(str.c_str());
		//int iSize = (int)strlen(strJsonU8);

		strJson.Format(_T("%s"),str.c_str());
	}
	int iSize = strJson.GetLength();

	bool ret;
	ret = fn_EventPayResultDisplay(strJson.GetBuffer(0), iSize, 2);
	//ret = fn_EventPayResultDisplay(strJsonU8, iSize, 2);
	if(!ret)
	{
		log("调用支付结果展示接口返回失败!");
	}
	//delete[] strJsonU8;

	return;
}


void CTestDevIFDlg::OnBnClickedBtnEventstop()
{
	if(bLoaded == false)
	{
		log("未加载共享库!");
		return;
	}
	if(m_bNetPayEnvInitedFlag == false || m_bEtcEventEnvInitedFlag == false)
	{
		log("未初始化环境!");
		return;
	}

	// TODO: 在此添加控件通知处理程序代码
	CString strJson;
	if (((CButton *)GetDlgItem(IDC_RADIO_FreeParam))->GetCheck())
	{
		EditParam.GetWindowText(strJson);
		strJson.TrimLeft();
		strJson.TrimRight();
		log("特情处理结束(使用自由配置参数):");
		log(strJson.GetBuffer(0));
	}

	if(((CButton *)GetDlgItem(IDC_RADIO_DefaultParam))->GetCheck())
	{
		// 表示整个 json 对象
		Json::Value root;
		// 标识ETC特情交互参数
		Json::Value EventDealStop;
		EventDealStop["SpecialEventNo"] = Json::Value(m_EventNo);
		EventDealStop["MessageCode"] = Json::Value("006");
		//// LaneInfo
		//Json::Value LaneInfo;
		//LaneInfo["ProvinceName"] = Json::Value("广东省");
		//LaneInfo["ProvinceNo"] = Json::Value("44");
		//LaneInfo["RoadName"] = Json::Value("广州北二环");
		//LaneInfo["RoadNo"] = Json::Value("0093");
		//LaneInfo["StationName"] = Json::Value("石湖收费站");
		//LaneInfo["StationNo"] = Json::Value("009305");
		//LaneInfo["LaneNo"] = Json::Value("36");
		//EventDealStop["LaneInfo"] = LaneInfo;
		
		//EventEndCases
		Json::Value EventEndCases;	

		EventEndCases["EventVehPlate"] = Json::Value(m_CurrentVehPlate);
		EventEndCases["EventVehPlateColor"] = Json::Value(m_CurrentVehPlateColor);
		EventEndCases["EventVehClass"] = Json::Value(m_CurrentVehClass);
		EventEndCases["EventVehType"] = Json::Value(m_CurrentVehType);
		EventEndCases["EventVehSeatCount"] = Json::Value(5);
		EventEndCases["EventVehAxleCount"] = Json::Value(2);
		EventEndCases["MediaType"] = Json::Value(2);
		EventEndCases["CardNum"] = Json::Value("1234567890");
		EventEndCases["enProvinceId"] = Json::Value(m_strProvinceID);
		EventEndCases["enStationId"] = Json::Value(m_strStationID);
		EventEndCases["enTollLaneId"] = Json::Value(m_strRoadID);
		EventEndCases["enTime"] = Json::Value(MyGetCurrenTime());
		EventEndCases["exitFeeType"] = Json::Value(4);

		EventEndCases["EventEndTime"] = Json::Value(MyGetCurrenTime());
		EventEndCases["EventDescribe"] = Json::Value("车辆无入口信息");
		EventEndCases["EventEndReason"] = Json::Value("特情处理完成，支付成功");
		EventEndCases["EventHandleMethodNo"] = Json::Value(m_EventHandleMethodNo);
		EventEndCases["EventHandleMethodName"] = Json::Value(GetEventHandleMethodName(m_EventHandleMethodNo));
		EventDealStop["EventEndCases"] = EventEndCases;
		
		root["EventDealStop"] = EventDealStop;

		//转化为字符串
		Json::FastWriter fast_writer;
		std::string str = fast_writer.write(root).c_str();
		log("特情处理结束:");
		log(str);

		//utf8编码
		//char* strJsonU8 = G2U(str.c_str());
		//int iSize = (int)strlen(strJsonU8);

		strJson.Format(_T("%s"),str.c_str());
	}
	int iSize = strJson.GetLength();
	
	bool ret;
	ret = fn_EventDealStop(strJson.GetBuffer(0), iSize, 2);
	//ret = fn_EventDealStop(strJsonU8, iSize, 2);
	if(!ret)
	{
		log("调用特情处理结束接口返回失败!");
	}
	//delete[] strJsonU8;

	return;
}


void CTestDevIFDlg::OnBnClickedBtnStartscan()
{
	if(bLoaded == false)
	{
		log("未加载共享库!");
		return;
	}
	if(m_bNetPayEnvInitedFlag == false || m_bEtcEventEnvInitedFlag == false)
	{
		log("未初始化环境!");
		return;
	}
	// TODO: 在此添加控件通知处理程序代码
	log("启动扫码");

	fn_EventStartScan(3000);
	return;
}


void CTestDevIFDlg::OnBnClickedBtnStopscan()
{
	if(bLoaded == false)
	{
		log("未加载共享库!");
		return;
	}
	if(m_bNetPayEnvInitedFlag == false || m_bEtcEventEnvInitedFlag == false)
	{
		log("未初始化环境!");
		return;
	}
	// TODO: 在此添加控件通知处理程序代码
	log("停止扫码");

	fn_EventStopScan();

	return;
}

void CTestDevIFDlg::OnBnClickedBtnShowfeeinfosingle()
{
	// TODO: 在此添加控件通知处理程序代码
	if(bLoaded == false)
	{
		log("未加载共享库!");
		return;
	}
	if(m_bNetPayEnvInitedFlag == false || m_bEtcEventEnvInitedFlag == false)
	{
		log("未初始化环境!");
		return;
	}

	// TODO: 在此添加控件通知处理程序代码
	CString strJson;
	if (((CButton *)GetDlgItem(IDC_RADIO_FreeParam))->GetCheck())
	{
		EditParam.GetWindowText(strJson);
		strJson.TrimLeft();
		strJson.TrimRight();
		log("费额显示(使用自由配置参数):");
		log(strJson.GetBuffer(0));
	}

	if(((CButton *)GetDlgItem(IDC_RADIO_DefaultParam))->GetCheck())
	{
		// 表示整个 json 对象
		Json::Value root;
		// 标识ETC特情交互参数
		Json::Value EventShowFeeInfo;
		EventShowFeeInfo["SpecialEventNo"] = Json::Value(m_EventNo);
		EventShowFeeInfo["MessageCode"] = Json::Value("005");
		//// LaneInfo
		//Json::Value LaneInfo;
		//LaneInfo["ProvinceName"] = Json::Value("广东省");
		//LaneInfo["ProvinceNo"] = Json::Value("44");
		//LaneInfo["RoadName"] = Json::Value("广州北二环");
		//LaneInfo["RoadNo"] = Json::Value("0094");
		//LaneInfo["StationName"] = Json::Value("石湖收费站");
		//LaneInfo["StationNo"] = Json::Value("009304");
		//LaneInfo["LaneNo"] = Json::Value("1");
		//EventShowFeeInfo["LaneInfo"] = LaneInfo;

		EventShowFeeInfo["PayType"] = Json::Value(1);
		EventShowFeeInfo["DefaultFeeItemNo"] = Json::Value(1);
		EventShowFeeInfo["FeeCount"] = Json::Value(1);

		Json::Value Array;
		Json::Value Item;
		Item["Entry"] = Json::Value("广东-莞深高速-石碣站");
		Item["PathInfo"] = Json::Value("0729-0730-0731");
		Item["Fee"] = Json::Value("3000");
		Item["ChargeType"] = Json::Value(2);
		Item["ItemNo"] = Json::Value(1);
		Array.append(Item);
		
		EventShowFeeInfo["FeeInfo"] = Array;

		EventShowFeeInfo["TotalVehNum"] = Json::Value(4);
		Json::Value VehQueue;
		Json::Value VehItem;
		VehItem["Index"] = Json::Value(0);
		VehItem["VehPlate"] = Json::Value("粤A121121");
		VehItem["VehPlateColor"] = Json::Value(1);
		VehItem["Fee"] = Json::Value("3000");
		VehItem["DealResult"] = Json::Value(0);
		VehItem["FailReason"] = Json::Value("");
		VehQueue.append(VehItem);

		VehItem["Index"] = Json::Value(1);
		VehItem["VehPlate"] = Json::Value("粤A122122");
		VehItem["VehPlateColor"] = Json::Value(1);
		VehItem["Fee"] = Json::Value("3000");
		VehItem["DealResult"] = Json::Value(0);
		VehItem["FailReason"] = Json::Value("");
		VehQueue.append(VehItem);

		VehItem["Index"] = Json::Value(2);
		VehItem["VehPlate"] = Json::Value("粤A123123");
		VehItem["VehPlateColor"] = Json::Value(1);
		VehItem["Fee"] = Json::Value("3000");
		VehItem["DealResult"] = Json::Value(1);
		VehItem["FailReason"] = Json::Value("余额不足");
		VehQueue.append(VehItem);

		VehItem["Index"] = Json::Value(3);
		VehItem["VehPlate"] = Json::Value("粤A124124");
		VehItem["VehPlateColor"] = Json::Value(1);
		VehItem["Fee"] = Json::Value("3000");
		VehItem["DealResult"] = Json::Value(2);
		VehItem["FailReason"] = Json::Value("");
		VehQueue.append(VehItem);
		EventShowFeeInfo["VehQueue"] = VehQueue;

		root["EventShowFeeInfo"] = EventShowFeeInfo;

		//转化为字符串
		Json::FastWriter fast_writer;
		std::string str = fast_writer.write(root).c_str();
		log("费额显示:");
		log(str);

		//utf8编码
		//char* strJsonU8 = G2U(str.c_str());
		//int iSize = (int)strlen(strJsonU8);

		strJson.Format(_T("%s"),str.c_str());
	}
	int iSize = strJson.GetLength();

	bool ret = fn_EventShowFeeInfo(strJson.GetBuffer(0), iSize, 2);
	//bool ret = fn_EventShowFeeInfo(strJsonU8, iSize, 2);
	if(!ret)
	{
		log("调用费额显示接口返回失败!");
	}
	//delete[] strJsonU8;

	return;
}

void CTestDevIFDlg::OnBnClickedBtnPayresultshowfail()
{
	// TODO: 在此添加控件通知处理程序代码
	if(bLoaded == false)
	{
		log("未加载共享库!");
		return;
	}
	if(m_bNetPayEnvInitedFlag == false || m_bEtcEventEnvInitedFlag == false)
	{
		log("未初始化环境!");
		return;
	}

	//// TODO: 手动添加消息处理函数
	CString strJson;
	if (((CButton *)GetDlgItem(IDC_RADIO_FreeParam))->GetCheck())
	{
		EditParam.GetWindowText(strJson);
		strJson.TrimLeft();
		strJson.TrimRight();
		log("支付状态展示(使用自由配置参数):");
		log(strJson.GetBuffer(0));	
	}

	if(((CButton *)GetDlgItem(IDC_RADIO_DefaultParam))->GetCheck())
	{
		// 表示整个 json 对象
		Json::Value root;
		// 标识ETC特情交互参数
		Json::Value EventPayResultShow;
		EventPayResultShow["SpecialEventNo"] = Json::Value(m_EventNo);
		EventPayResultShow["MessageCode"] = Json::Value("006");
		//// LaneInfo
		//Json::Value LaneInfo;
		//LaneInfo["ProvinceName"] = Json::Value("广东省");
		//LaneInfo["ProvinceNo"] = Json::Value("44");
		//LaneInfo["RoadName"] = Json::Value("广州北二环");
		//LaneInfo["RoadNo"] = Json::Value("0093");
		//LaneInfo["StationName"] = Json::Value("石湖收费站");
		//LaneInfo["StationNo"] = Json::Value("009305");
		//LaneInfo["LaneNo"] = Json::Value("36");
		//EventPayResultShow["LaneInfo"] = LaneInfo;
		
		//EventInfo
		Json::Value EventInfo;	
		EventInfo["RequestTime"] = Json::Value(MyGetCurrenTime());
		EventInfo["RequestType"] = Json::Value("004");
		EventInfo["PayResult"] = Json::Value(2);
		EventInfo["Describe"] = Json::Value("微信支付二维码非法");
		EventInfo["TimeLimit"] = Json::Value("360");
		EventPayResultShow["EventInfo"] = EventInfo;
		
		root["EventPayResultShow"] = EventPayResultShow;

		//转化为字符串
		Json::FastWriter fast_writer;
		std::string str = fast_writer.write(root).c_str();
		log("支付状态展示:");
		log(str);

		//utf8编码
		//char* strJsonU8 = G2U(str.c_str());
		//int iSize = (int)strlen(strJsonU8);

		strJson.Format(_T("%s"),str.c_str());
	}
	int iSize = strJson.GetLength();

	bool ret;
	ret = fn_EventPayResultDisplay(strJson.GetBuffer(0), iSize, 2);
	//ret = fn_EventPayResultDisplay(strJsonU8, iSize, 2);
	if(!ret)
	{
		log("调用支付结果展示接口返回失败!");
	}
	//delete[] strJsonU8;

	return;
}

void MyCallBackFun(int iOption, int iResult, void* pIndPtr)
{
	int Option = iOption;
	int Result = iResult;
	char *ptr = (char *)pIndPtr;

	theTestDevIFApp.m_pMainWnd->PostMessage(WM_THREAD_ETCEVENT_CALLBACK_SHOWLOG,(WPARAM)Result,(LPARAM)0);

	return;
}

LRESULT CTestDevIFDlg::OnMsgShowEtcEventLog(WPARAM wParam, LPARAM lParam)
{
	int result = (int)wParam;
	switch(result)
	{
	case 601:
		log(">>>>>收到特情处理开始应答: " + std::string((char *)m_pIndPtr));
		break;
	case 602:
		log(">>>>>收到车辆队列确认应答: " + std::string((char *)m_pIndPtr));
		break;
	case 603:
		log(">>>>>收到车辆信息确认应答: " + std::string((char *)m_pIndPtr));
		break;
	case 604:
		log(">>>>>收到车辆入口信息确认应答: " + std::string((char *)m_pIndPtr));
		break;
	case 605:
		log(">>>>>收到费额显示应答: " + std::string((char *)m_pIndPtr));
		break;
	case 606:
		log(">>>>>收到支付状态显示应答: " + std::string((char *)m_pIndPtr));
		break;
	case 607:
		log(">>>>>收到特情结束应答: " + std::string((char *)m_pIndPtr));
		break;
	case 608:
		log(">>>>>收到扫描二维码数据应答: " + std::string((char *)m_pIndPtr));
		break;
	case 609:
		log(">>>>>收到停止寻卡应答");
		break;
	case 610:
		log(">>>>>收到主动核单应答: "+ std::string((char *)m_pIndPtr));
		break;
	case 611:
		log(">>>>>收到ETC卡操作提示应答: " + std::string((char *)m_pIndPtr));
		break;
	case 612:
		log(">>>>>收到修改车辆队列应答: " + std::string((char *)m_pIndPtr));
		break;
	case 613:
		log(">>>>>收到停止扫码应答");
		break;
	case 616:
		log(">>>>>收到其他授权应答: " + std::string((char *)m_pIndPtr));
		break;
	}
	if(result == 603)
	{
		char *strGBK = U2G((char *)m_pIndPtr);
		bool bRet;
		bRet = ParseVehInfoCheckResp(strGBK);
		if(bRet)
			log("解析车辆信息确认应答成功");
		else
			log("解析车辆信息确认应答失败!");

		delete strGBK;
	}

	//::PostMessage(WM_THREAD_DEVICE_START_GET_DATA, (WPARAM)0, (LPARAM)0);

	return 0;
}




/*
#define MAKEWORD(a, b)      ((WORD)(((BYTE)((DWORD_PTR)(a) & 0xff)) | ((WORD)((BYTE)((DWORD_PTR)(b) & 0xff))) << 8))
#define MAKELONG(a, b)      ((LONG)(((WORD)((DWORD_PTR)(a) & 0xffff)) | ((DWORD)((WORD)((DWORD_PTR)(b) & 0xffff))) << 16))
#define LOWORD(l)           ((WORD)((DWORD_PTR)(l) & 0xffff))
#define HIWORD(l)           ((WORD)((DWORD_PTR)(l) >> 16))
#define LOBYTE(w)           ((BYTE)((DWORD_PTR)(w) & 0xff))
#define HIBYTE(w)           ((BYTE)((DWORD_PTR)(w) >> 8))
*/
LRESULT CTestDevIFDlg::OnHWNDMsgShowEtcEventLog(WPARAM wParam, LPARAM lParam)
{
	unsigned int result = (int)wParam;
	unsigned short FunCode = LOWORD(result);
	unsigned short SubCode = HIWORD(result);
	//int SubCode = result;
	CString strTip;
	strTip.Format(_T("功能代码(低2位):%d 执行状态(高2位):%d"), FunCode, SubCode);
	log(strTip.GetBuffer(0));

	switch(SubCode)
	{
	case 601:
		log(">>>>>收到特情处理开始应答: " + std::string((char *)m_pIndPtr));
		break;
	case 602:
		log(">>>>>收到车辆队列确认应答: " + std::string((char *)m_pIndPtr));
		break;
	case 603:
		log(">>>>>收到车辆信息确认应答: " + std::string((char *)m_pIndPtr));
		break;
	case 604:
		log(">>>>>收到车辆入口信息确认应答: " + std::string((char *)m_pIndPtr));
		break;
	case 605:
		log(">>>>>收到费额显示应答: " + std::string((char *)m_pIndPtr));
		break;
	case 606:
		log(">>>>>收到支付状态显示应答: " + std::string((char *)m_pIndPtr));
		break;
	case 607:
		log(">>>>>收到特情结束应答: " + std::string((char *)m_pIndPtr));
		break;
	case 608:
		log(">>>>>收到扫描二维码数据应答: " + std::string((char *)m_pIndPtr));
		break;
	case 609:
		log(">>>>>收到停止寻卡应答");
		break;
	case 610:
		log(">>>>>收到主动核单应答: "+ std::string((char *)m_pIndPtr));
		break;
	case 611:
		log(">>>>>收到ETC卡操作提示应答: " + std::string((char *)m_pIndPtr));
		break;
	case 612:
		log(">>>>>收到修改车辆队列应答: " + std::string((char *)m_pIndPtr));
		break;
	case 613:
		log(">>>>>收到最小费额授权应答: " + std::string((char *)m_pIndPtr));
		break;
	case 614:
		log(">>>>>收到设备按键主动应答: " + std::string((char *)m_pIndPtr));
		break;
	case 699:
		log(">>>>>收到停止扫码应答");
		break;
	case 616:
		log(">>>>>收到其他授权应答: " + std::string((char *)m_pIndPtr));
		break;
	case 617:
		log(">>>>>收到设备删除车辆队列主动应答: " + std::string((char *)m_pIndPtr));
		break;
	case 618:
		log(">>>>>收到设备删除车辆队列结果应答: " + std::string((char *)m_pIndPtr));
		break;
	}
	if(result == 603)
	{
		char *strGBK = U2G((char *)m_pIndPtr);
		bool bRet;
		bRet = ParseVehInfoCheckResp(strGBK);
		if(bRet)
			log("解析车辆信息确认应答成功");
		else
			log("解析车辆信息确认应答失败!");

		delete strGBK;
	}

	return 0;
}


bool CTestDevIFDlg::ParseVehInfoCheckResp(char *pBuffer)
{
	Json::Reader reader;
	Json::Value  root;
	Json::Value  EventCheckVehInfoResp;
	Json::Value  Data;	

	if(!reader.parse(pBuffer, root))
	{
		log("车辆信息确认应答json解析错误!");
	}

	if(root["EventCheckVehInfoResp"].type() != Json::nullValue)
	{
		EventCheckVehInfoResp = root["EventCheckVehInfoResp"];
	}
	else
	{
		log("车辆信息确认应答json解析错误,无EventCheckVehInfoResp对象!");
		return false;
	}

	if (EventCheckVehInfoResp["Data"].type() != Json::nullValue)
	{
		Data = EventCheckVehInfoResp["Data"];
	}
	else
	{
		log("车辆信息确认应答json解析错误,无Data对象!");
		return false;
	}

	CString tmpStr = Data["VehPlate"].asCString();
	if(tmpStr.IsEmpty() == false)
		m_CurrentVehPlate = Data["VehPlate"].asCString();
	int tmpN = Data["VehPlateColor"].asInt();
	if(tmpN != -1)
		m_CurrentVehPlateColor = Data["VehPlateColor"].asInt();
	tmpN = Data["VehClass"].asInt();
	if(tmpN != -1)
		m_CurrentVehClass = Data["VehClass"].asInt();
	tmpN = Data["VehType"].asInt();
	if(tmpN != -1)
		m_CurrentVehType = Data["VehType"].asInt();
	tmpN = Data["AxisCount"].asInt();
	if(tmpN != -1)
		m_CurrentAxisCount = Data["AxisCount"].asInt();
	tmpStr = Data["EntryTime"].asCString();
	if(tmpStr.IsEmpty() == false)
		m_CurrentEntryTime = Data["EntryTime"].asCString(); 

	CString LogStr;
	LogStr.Format("收到车辆信息确认应答,更新当前特情车辆信息：VehPlate:%s,VehPlateColor:%d,VehClass:%d,VehType:%d,AxisCount:%d,EntryTime:%s", m_CurrentVehPlate.GetBuffer(0), m_CurrentVehPlateColor,m_CurrentVehClass,m_CurrentVehType,m_CurrentAxisCount,m_CurrentEntryTime.GetBuffer());
	std::string logstr(LogStr.GetBuffer(0));
	log(logstr);


	return true;
}


CString CTestDevIFDlg::MyGetCurrenTime()
{
  CTime time = CTime::GetCurrentTime();
  CString curTime;
  curTime.Format("%04d-%02d-%02d %02d:%02d:%02d", time.GetYear(),time.GetMonth(),time.GetDay(),time.GetHour(),time.GetMinute(),time.GetSecond());
  return curTime;
}


CString CTestDevIFDlg::GetUniqueTimeStr()
{
  CTime time = CTime::GetCurrentTime();
  CString curTime;
  curTime.Format("%04d%02d%02d%02d%02d%02d", time.GetYear(),time.GetMonth(),time.GetDay(),time.GetHour(),time.GetMinute(),time.GetSecond());
  return curTime;
}

void CTestDevIFDlg::OnBnClickedBtnDestroy()
{
	// TODO: 在此添加控件通知处理程序代码
	log("调用接口IF_Destroy()");
	fn_EventDestroy();

	return;
}

void CTestDevIFDlg::OnBnClickedBtnGetcomponentstatus()
{
	// TODO: 在此添加控件通知处理程序代码
	log("调用接口IF_GetComponentStatus()");
	unsigned int status;
	fn_EventGetComponentStatus(status);
	log("status:"+to_string(status));

	return;
}

void CTestDevIFDlg::OnBnClickedBtnGetlasterror()
{
	// TODO: 在此添加控件通知处理程序代码
	log("调用接口IF_GetLastErrorDesc()");
	const char *p = fn_EventGetLastErrorDesc();
	log("LastErrorDesc:" + std::string(p));

	return;
}

void CTestDevIFDlg::OnBnClickedBtnGetlasterror2()
{
	// TODO: 在此添加控件通知处理程序代码
	log("调用接口IF_GetLastErrorDesc()");
	const char *p = fn_EventGetLastErrorDesc();
	log("LastErrorDesc:" + std::string(p));

	return;
}


CString CTestDevIFDlg::GetCurrentDirNew()
{
	//AFX_MANAGE_STATE(AfxGetStaticModuleState());

	TCHAR szfile[255] = _T("");
	CString csDir = _T("");

	//对于应用程序（调用者），此方式正确。对于动态库（被调用者），此方式获取的是程序路径
	DWORD dwLen = GetModuleFileName(NULL, szfile, 255);

	//调整为动态库自身的路径
	//HINSTANCE hInsApp;
	//hInsApp = AfxGetInstanceHandle();
	//DWORD dwLen = GetModuleFileName(hInsApp, szfile, 255);

	csDir.Format( _T("%s"), szfile);
	csDir = csDir.Left( csDir.ReverseFind(_T('\\')) );
	return csDir;
}


CString CTestDevIFDlg::GetEventTypeName(int EventTypeNo)
{
	CString EventTypeName;

	switch(EventTypeNo)
	{
	case 1:
		EventTypeName = "未感应到标签";
		break;
	case 2:
		EventTypeName = "读标签失败";
		break;
	case 3:
		EventTypeName = "标签异常";
		break;
	case 4:
		EventTypeName = "标签黑名单";
		break;
	case 5:
		EventTypeName = "标签超过有效期";
		break;
	case 6:
		EventTypeName = "标签拆卸";
		break;
	case 7:
		EventTypeName = "写卡失败";
		break;
	case 8:
		EventTypeName = "读卡失败";
		break;
	case 9:
		EventTypeName = "卡异常";
		break;
	case 10:
		EventTypeName = "黑名单卡";
		break;
	case 11:
		EventTypeName = "卡超过有效期";
		break;
	case 12:
		EventTypeName = "卡未插好";
		break;
	case 13:
		EventTypeName = "卡签入口不一致";
		break;
	case 14:
		EventTypeName = "卡签车牌不一致";
		break;
	case 15:
		EventTypeName = "无入口信息";
		break;
	case 16:
		EventTypeName = "入口无效";
		break;
	case 17:
		EventTypeName = "余额不足";
		break;
	case 18:
		EventTypeName = "计费异常";
		break;
	case 19:
		EventTypeName = "运营禁行";
		break;
	case 100:
		EventTypeName = "其他特情";
		break;
	default:
		EventTypeName = "其他特情";		
	}

	return EventTypeName;
}


CString CTestDevIFDlg::GetEventHandleMethodName(int EventHandleMethodNo)
{
	CString EventHandleMethodName;
	switch(EventHandleMethodNo)
	{
	case 1:
		EventHandleMethodName = "终端处理";
		break;
	case 2:
		EventHandleMethodName = "特情误报";
		break;
	case 3:
		EventHandleMethodName = "引出车道";
		break;
	case 100:
		EventHandleMethodName = "其他处理方式";
		break;
	default:
		EventHandleMethodName = "终端处理";
	}

	return EventHandleMethodName;
}


void CTestDevIFDlg::OnBnClickedBtnSwipecard()
{
	// TODO: 在此添加控件通知处理程序代码
	if(bLoaded == false)
	{
		log("未加载共享库!");
		return;
	}
	if(m_bNetPayEnvInitedFlag == false || m_bEtcEventEnvInitedFlag == false)
	{
		log("未初始化环境!");
		return;
	}

	CString strJson;
	if (((CButton *)GetDlgItem(IDC_RADIO_FreeParam))->GetCheck())
	{
		EditParam.GetWindowText(strJson);
		strJson.TrimLeft();
		strJson.TrimRight();
		log("刷卡提示(使用自由配置参数):");
		log(strJson.GetBuffer(0));
	}

	if(((CButton *)GetDlgItem(IDC_RADIO_DefaultParam))->GetCheck())
	{
		// 表示整个 json 对象
		Json::Value root;
		// 标识ETC特情交互参数
		Json::Value CardOperationNotify;
		CardOperationNotify["SpecialEventNo"] = Json::Value(m_EventNo);
		CardOperationNotify["MessageCode"] = Json::Value("009");
		
		Json::Value Data;
		Data["NotifyType"] = Json::Value(1);	
		CardOperationNotify["Data"] = Data;

		root["CardOperationNotify"] = CardOperationNotify;

		//转化为字符串
		Json::FastWriter fast_writer;
		std::string str = fast_writer.write(root).c_str();
		log("刷卡提示:");
		log(str);

		//utf8编码
		//char* strJsonU8 = G2U(str.c_str());
		//int iSize = (int)strlen(strJsonU8);

		strJson.Format(_T("%s"), str.c_str());
	}
	int iSize = strJson.GetLength();
	
	bool ret = false;
	//ret = fn_EventCardOperationNotify(strJsonU8, iSize, 2);
	ret = fn_EventCardOperationNotify(strJson.GetBuffer(0), iSize, 2);
	if(!ret)
	{
		log("调用ETC卡操作提示接口返回失败!");
	}

	//delete[] strJsonU8;

	return;
}



void CTestDevIFDlg::OnBnClickedBtnRwcard()
{
	// TODO: 在此添加控件通知处理程序代码
	if(bLoaded == false)
	{
		log("未加载共享库!");
		return;
	}
	if(m_bNetPayEnvInitedFlag == false || m_bEtcEventEnvInitedFlag == false)
	{
		log("未初始化环境!");
		return;
	}

	CString strJson;
	if (((CButton *)GetDlgItem(IDC_RADIO_FreeParam))->GetCheck())
	{
		EditParam.GetWindowText(strJson);
		strJson.TrimLeft();
		strJson.TrimRight();
		log("读写卡提示(使用自由配置参数):");
		log(strJson.GetBuffer(0));
	}

	if(((CButton *)GetDlgItem(IDC_RADIO_DefaultParam))->GetCheck())
	{
		// 表示整个 json 对象
		Json::Value root;
		// 标识ETC特情交互参数
		Json::Value CardOperationNotify;
		CardOperationNotify["SpecialEventNo"] = Json::Value(m_EventNo);
		CardOperationNotify["MessageCode"] = Json::Value("009");
		
		Json::Value Data;
		Data["NotifyType"] = Json::Value(2);	
		CardOperationNotify["Data"] = Data;

		root["CardOperationNotify"] = CardOperationNotify;

		//转化为字符串
		Json::FastWriter fast_writer;
		std::string str = fast_writer.write(root).c_str();
		log("读写卡提示:");
		log(str);

		//utf8编码
		//char* strJsonU8 = G2U(str.c_str());
		//int iSize = (int)strlen(strJsonU8);

		strJson.Format(_T("%s"),str.c_str());
	}
	int iSize = strJson.GetLength();
	
	bool ret = false;
	//ret = fn_EventCardOperationNotify(strJsonU8, iSize, 2);
	ret = fn_EventCardOperationNotify(strJson.GetBuffer(0), iSize, 2);
	if(!ret)
	{
		log("调用ETC卡操作提示接口返回失败!");
	}

	//delete[] strJsonU8;

	return;
}

void CTestDevIFDlg::OnBnClickedBtnFixvehqueue()
{
	// TODO: 在此添加控件通知处理程序代码
	if(bLoaded == false)
	{
		log("未加载共享库!");
		return;
	}
	if(m_bNetPayEnvInitedFlag == false || m_bEtcEventEnvInitedFlag == false)
	{
		log("未初始化环境!");
		return;
	}
	CString strJson;
	if (((CButton *)GetDlgItem(IDC_RADIO_FreeParam))->GetCheck())
	{
		EditParam.GetWindowText(strJson);
		strJson.TrimLeft();
		strJson.TrimRight();
		log("车辆队列修改(使用自由配置参数):");
		log(strJson.GetBuffer(0));
	}

	if(((CButton *)GetDlgItem(IDC_RADIO_DefaultParam))->GetCheck())
	{
		// 表示整个 json 对象
		Json::Value root;
		// 标识ETC特情交互参数
		Json::Value EventModifyVehQueue;
		EventModifyVehQueue["SpecialEventNo"] = Json::Value(m_EventNo);
		EventModifyVehQueue["MessageCode"] = Json::Value("010");
		EventModifyVehQueue["Reasons"] = Json::Value("卡内车牌与特情车牌不一致");
		EventModifyVehQueue["TotalVehNum"] = Json::Value(4);

		Json::Value VehQueue;
		Json::Value VehItem;
		VehItem["Index"] = Json::Value(0);
		VehItem["VehPlate"] = Json::Value("粤A121121");
		VehItem["VehPlateColor"] = Json::Value(1);
		VehItem["Fee"] = Json::Value("3000");
		VehItem["DealResult"] = Json::Value(0);
		VehItem["FailReason"] = Json::Value("");
		VehQueue.append(VehItem);

		VehItem["Index"] = Json::Value(1);
		VehItem["VehPlate"] = Json::Value("粤A122122");
		VehItem["VehPlateColor"] = Json::Value(1);
		VehItem["Fee"] = Json::Value("3000");
		VehItem["DealResult"] = Json::Value(0);
		VehItem["FailReason"] = Json::Value("");
		VehQueue.append(VehItem);

		VehItem["Index"] = Json::Value(2);
		VehItem["VehPlate"] = Json::Value("粤A123123");
		VehItem["VehPlateColor"] = Json::Value(1);
		VehItem["Fee"] = Json::Value("3000");
		VehItem["DealResult"] = Json::Value(1);
		VehItem["FailReason"] = Json::Value("余额不足");
		VehQueue.append(VehItem);

		VehItem["Index"] = Json::Value(3);
		VehItem["VehPlate"] = Json::Value("粤A124124");
		VehItem["VehPlateColor"] = Json::Value(1);
		VehItem["Fee"] = Json::Value("3000");
		VehItem["DealResult"] = Json::Value(2);
		VehItem["FailReason"] = Json::Value("");
		VehQueue.append(VehItem);
		EventModifyVehQueue["VehQueue"] = VehQueue;

		root["EventModifyVehQueue"] = EventModifyVehQueue;

		//转化为字符串
		Json::FastWriter fast_writer;
		std::string str = fast_writer.write(root).c_str();
		log("车辆队列修改:");
		log(str);

		//utf8编码
		//char* strJsonU8 = G2U(str.c_str());
		//int iSize = (int)strlen(strJsonU8);
		
		strJson.Format(_T("%s"),str.c_str());
	}
	int iSize = strJson.GetLength();
	
	bool ret = false;
	//ret = fn_EventModifyVehQueue(strJsonU8, iSize, 2);
	ret = fn_EventModifyVehQueue(strJson.GetBuffer(0), iSize, 2);
	if(!ret)
	{
		log("调用车辆队列修改接口返回失败!");
	}
	//delete[] strJsonU8;

	return;
}

void CTestDevIFDlg::OnBnClickedBtnInitNetPayEnv()
{
	// TODO: 在此添加控件通知处理程序代码
	if(bLoaded == false)
	{
		log("未加载共享库!");
		return;
	}

	CString strTemp;
	if(m_bNetPayEnvInitedFlag == true)
	{
		log(_T("移动支付环境已初始化成功，请不要重复初始化!"));
		return;
	}

	CString strAreaInfo;
	CString strStationInfo;
	CString strLaneInfo;
	CString strServerInfo;

	strAreaInfo.Format(_T("%s#%s"), g_DevIFDlg->m_strAreaID, g_DevIFDlg->m_strRoadID);
	strStationInfo.Format(_T("%s#%s"), g_DevIFDlg->m_strStationID, g_DevIFDlg->m_strStationName);
						
	strLaneInfo.Format(_T("%s"), g_DevIFDlg->m_strLaneID);
	strServerInfo.Format(_T("1#%s#%d"), g_DevIFDlg->m_strServerIP, g_DevIFDlg->m_wServerPort);

	bool bRet = false;
	//窗体句柄接收应答
	//bRet = fn_InitEnvironment(0, this->m_hWnd, WM_HWND_NETPAY_RESULT_SHOWLOG, strAreaInfo, strStationInfo, strLaneInfo, strServerInfo, g_DevIFDlg->m_iProvinceID);
	//线程接收应答
	bRet = fn_InitEnvironment(ReceiveMsgThreadID, 0, WM_THREAD_NETPAY_MESSAGE_SHOWLOG, strAreaInfo, strStationInfo, strLaneInfo, strServerInfo, g_DevIFDlg->m_iProvinceID);

	if(bRet == false)
	{
		log(_T("移动支付环境初始化失败！"));
		m_bNetPayEnvInitedFlag = false;
	}
	else
	{
		log(_T("移动支付环境初始化成功！"));
		m_bNetPayEnvInitedFlag = true;
	}
	return;
}




void CTestDevIFDlg::GetUIDebitInfo()
{
	//int iDebitMoney;
	TCHAR ch[32] = {0};
	GetDlgItem(IDC_EDIT_Money)->GetWindowText(ch, 32);
	m_iDebitMoney = _ttoi(ch);

	//int iAxisNum;
	memset(ch, 0, 32);
	GetDlgItem(IDC_EDIT_axisnum)->GetWindowText(ch, 32);
	m_iAxisNum = _ttoi(ch);

	//int iWeight;
	memset(ch, 0, 32);
	GetDlgItem(IDC_EDIT_Weight)->GetWindowText(ch, 32);
	m_iWeight = _ttoi(ch);

	//int iTimeOut;
	memset(ch, 0, 32);
	GetDlgItem(IDC_EDIT_timeout)->GetWindowText(ch, 32);
	m_iTimeOut = _ttoi(ch);

	//int iBalance;
	memset(ch, 0, 32);
	GetDlgItem(IDC_EDIT_balance)->GetWindowText(ch, 32);
	m_iBalance = _ttoi(ch);
	
	m_iVType = 1;
	m_iVClass = 1;
	m_iVLColor = 0;


	//CString strPlate;
	GetDlgItemText(IDC_EDIT_plate, m_strPlate);
	//CString strEntry;
	GetDlgItemText(IDC_EDIT_entry, m_strEntry);
    //CString strOperator;
	GetDlgItemText(IDC_EDIT_operatotor, m_strOperator);
	//CString strOperatotID;	
	GetDlgItemText(IDC_EDIT_operatorID, m_strOperatotID);
	//CString strCardNo; 
	GetDlgItemText(IDC_EDIT_cardno, m_strCardNo);

	return;
}

const CString CTestDevIFDlg::guid(IN const int iType)
{
	CString strReturn;
	static char buf[64] = { 0 };
	GUID guid;
	if (S_OK == ::CoCreateGuid(&guid))
	{
		strReturn.Format(_T("{%08X-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X}")
		                 , guid.Data1
		                 , guid.Data2
		                 , guid.Data3
		                 , guid.Data4[0], guid.Data4[1]
		                 , guid.Data4[2], guid.Data4[3], guid.Data4[4], guid.Data4[5]
		                 , guid.Data4[6], guid.Data4[7]
		                );
	}
	if (1 == iType)
	{
		strReturn.Replace(_T("-"), _T(""));
		strReturn.Replace(_T("{"), _T(""));
		strReturn.Replace(_T("}"), _T(""));
	}
	return strReturn;
}


////////////////////////////////////////////////////////////////////////////////////////////////
// 函数名称：FillpPayKeyItemsStruct
//
// 功能描述：填充结构体
//
// 输入参数：IN  int _OperationMode //扣款模式
// 输入参数：IN int _PayCertificateType //支付凭证类型
// 输入参数：IN CString _PayCertificateCode  //第三方支付凭证
// 输出参数：OUT tagPayKeyItems& _ppPayKeyItems //传出结构体
// 返回值：成功 返回 TRUE，失败 返回 FALSE
// 编写人员：FORTUNE
// 编写时间：星期日 2016年11月27日
// 修改人员：
// 修改时间：
// 函数版本：1.0.0.1
// 备注说明：
//
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CTestDevIFDlg::FillpPayKeyItemsStruct(IN  int _OperationMode, IN int _PayCertificateType, IN CString _PayCertificateCode, OUT tagPayKeyItems& _ppPayKeyItems)
{
	
	BOOL bRet = FALSE;
	if (NULL != &_ppPayKeyItems)
	{
		_ppPayKeyItems.OverTime = m_iTimeOut * 1000;
		_ppPayKeyItems.OperationMode = _OperationMode;
		_ppPayKeyItems.PayIdentifier = guid() + "1231";
		m_strLastPayIdentifier = _ppPayKeyItems.PayIdentifier;
		_ppPayKeyItems.Money = m_iDebitMoney;
		_ppPayKeyItems.SubTime =CTime::GetCurrentTime();
		_ppPayKeyItems.DebitMainType = 0x01;
		_ppPayKeyItems.PayCertificateCode = _PayCertificateCode;
		_ppPayKeyItems.PayCertificateType = _PayCertificateType;

		_ppPayKeyItems.VehicleType = m_iVType;
		_ppPayKeyItems.VehicleClass = m_iVClass;
		_ppPayKeyItems.VLColor = m_iVLColor;
		_ppPayKeyItems.VehicleLicense = m_strPlate;
		_ppPayKeyItems.TotalWeight = m_iWeight;

		_ppPayKeyItems.AxisCount = m_iAxisNum;

		_ppPayKeyItems.PassCertificateType = 0x01;
		_ppPayKeyItems.CardID = m_strCardNo;//_T("12345678");//90123456789000000000      ");
		CString strTemp;
		strTemp.Format(_T("%04d%02d%02d"), CTime::GetCurrentTime().GetYear(), CTime::GetCurrentTime().GetMonth(), CTime::GetCurrentTime().GetDay());
		_ppPayKeyItems.TollDate = _ttoi(strTemp);
		_ppPayKeyItems.ShiftID = 0x01;

		_ppPayKeyItems.EntryAreaID = 49;
		_ppPayKeyItems.EntryRoadID = 49;
		_ppPayKeyItems.EntryStationID = 43011288;
		_ppPayKeyItems.EntryStationName = m_strEntry;
		_ppPayKeyItems.EntryTime = CTime::GetCurrentTime() - CTimeSpan(0, 0, 10, 0);
		_ppPayKeyItems.EntryLane = 0x001;
		_ppPayKeyItems.EntryVehicleLicense = m_strPlate;
		_ppPayKeyItems.EntryVLColor = 0x00;
		_ppPayKeyItems.EntryVehicleType = 0x01;
		_ppPayKeyItems.EntryVehicleClass = 0x02;
	
		_ppPayKeyItems.OperatorName = m_strOperator;
		_ppPayKeyItems.OperatorID = m_strOperatotID;

		_ppPayKeyItems.AutoLicense = m_strPlate;
		_ppPayKeyItems.AutoColor = 0x00;
		_ppPayKeyItems.Distance = 0x10;
		_ppPayKeyItems.TicketNo = "123";
			
		//把交易识别码显示出来
		SetDlgItemText(IDC_EDIT_PayIdentifier, m_strLastPayIdentifier);
		//m_ED_Identifier.SetWindowText(m_strLastPayIdentifier);

		bRet = TRUE;
	}
	
	return bRet;
}

void CTestDevIFDlg::OnBnClickedBtnDebit()
{
	if(bLoaded == false)
	{
		log("未加载共享库!");
		return;
	}

	if(m_bNetPayEnvInitedFlag == false)
	{
		log("未初始化支付环境！");
		return;
	}

	if(m_bEtcEventEnvInitedFlag == false)
	{
		log("未初始ETC特情环境！");
	}

	// TODO: 在此添加控件通知处理程序代码
	GetUIDebitInfo();
	tagPayKeyItems* pPayKeyItems = new tagPayKeyItems();
	FillpPayKeyItemsStruct(0x01, 0x00, _T(""), *pPayKeyItems);

	CDVJsonParamDebitMoney jsonDebitMoney;
	CJsonParamOperate jsonOperation;

	jsonDebitMoney.OverTime				= pPayKeyItems->OverTime;
	jsonDebitMoney.OperationMode		= pPayKeyItems->OperationMode;
	jsonDebitMoney.PayIdentifier		= pPayKeyItems->PayIdentifier;
	jsonDebitMoney.Money				= pPayKeyItems->Money;
	jsonDebitMoney.SubTime				= pPayKeyItems->SubTime;
	jsonDebitMoney.DebitMainType		= pPayKeyItems->DebitMainType;
	jsonDebitMoney.PayCertificateCode	= pPayKeyItems->PayCertificateCode;
	jsonDebitMoney.PayCertificateType	= pPayKeyItems->PayCertificateType;
	jsonDebitMoney.VehicleType			= pPayKeyItems->VehicleType;
	jsonDebitMoney.VehicleClass			= pPayKeyItems->VehicleClass;
	jsonDebitMoney.VehicleLicense		= pPayKeyItems->VehicleLicense;
	jsonDebitMoney.VLColor				= pPayKeyItems->VLColor;
	jsonDebitMoney.AxisCount			= pPayKeyItems->AxisCount;
	jsonDebitMoney.Weight				= pPayKeyItems->TotalWeight;
	jsonDebitMoney.LimitWeight			= pPayKeyItems->LimitWeight;
	jsonDebitMoney.PassCertificateType	= pPayKeyItems->PassCertificateType;
	//jsonDebitMoney.PassCertificateType	= 3;
	jsonDebitMoney.CardID				= pPayKeyItems->CardID;
	jsonDebitMoney.TollDate				= pPayKeyItems->TollDate;
	jsonDebitMoney.ShiftID				= pPayKeyItems->ShiftID;
	jsonDebitMoney.EntryStationID		= pPayKeyItems->EntryStationID;
	jsonDebitMoney.EntryStationName		= pPayKeyItems->EntryStationName;
	jsonDebitMoney.EntryTime			= pPayKeyItems->EntryTime;
	jsonDebitMoney.EntryLane			= pPayKeyItems->EntryLane;
	jsonDebitMoney.EntryVehicleLicense	= pPayKeyItems->EntryVehicleLicense;
	jsonDebitMoney.EntryVLColor			= pPayKeyItems->EntryVLColor;
	jsonDebitMoney.EntryVehicleClass	= pPayKeyItems->EntryVehicleClass;
	jsonDebitMoney.EntryVehicleType		= pPayKeyItems->EntryVehicleType;
	jsonDebitMoney.EntryRoadID			= pPayKeyItems->EntryRoadID;
	jsonDebitMoney.EntryAreaID			= pPayKeyItems->EntryAreaID;

	jsonDebitMoney.OperatorID = pPayKeyItems->OperatorID;
	jsonDebitMoney.OperatorName = pPayKeyItems->OperatorName;
	jsonDebitMoney.AutoColor = pPayKeyItems->AutoColor;
	jsonDebitMoney.AutoLicense = pPayKeyItems->AutoLicense;
	jsonDebitMoney.Distance = pPayKeyItems->Distance;
	jsonDebitMoney.AutoPrint = 1;			

	CString strDataString;
	strDataString = jsonOperation.FormatParamData2Json(&jsonDebitMoney);

	log("扣费json参数:");
	log(strDataString.GetBuffer(0));
	
	int iSize = strDataString.GetLength();
	int DataParamType = 2;
	fn_DebitMoney(strDataString.GetBuffer(0), iSize, DataParamType);

	strDataString.ReleaseBuffer();
	delete pPayKeyItems;
	pPayKeyItems = NULL;
}

void CTestDevIFDlg::OnBnClickedBtnCancel()
{
	if(bLoaded == false)
	{
		log("未加载共享库!");
		return;
	}

	if(m_bNetPayEnvInitedFlag == false)
	{
		log("未初始化支付环境");
		return;
	}

	// TODO: 在此添加控件通知处理程序代码
	tagPayKeyItems* pPayKeyItems = new tagPayKeyItems();
	if (NULL != pPayKeyItems)
	{
		pPayKeyItems->PayIdentifier.Format(_T("%s"), m_strLastPayIdentifier);
		
		CDVJsonParamDebitCancel jsonDebitCancel;
		CJsonParamOperate jsonOperation;
		jsonDebitCancel.PayIdentifier = pPayKeyItems->PayIdentifier;
		
		CString strDataString;
		strDataString = jsonOperation.FormatParamData2Json(&jsonDebitCancel);

		log("撤单json参数:");
		log(strDataString.GetBuffer(0));

		int iSize = strDataString.GetLength();
		int DataParamType = 2;
		fn_DebitCancel(strDataString.GetBuffer(0), iSize, DataParamType);
		strDataString.ReleaseBuffer();

		delete pPayKeyItems;
		pPayKeyItems = NULL;
	}
}

void CTestDevIFDlg::OnBnClickedBtnFeeAuthorize()
{
	// TODO: 在此添加控件通知处理程序代码
	if(bLoaded == false)
	{
		log("未加载共享库!");
		return;
	}
	if(m_bNetPayEnvInitedFlag == false || m_bEtcEventEnvInitedFlag == false)
	{
		log("未初始化环境!");
		return;
	}

	CString strJson;
	if (((CButton *)GetDlgItem(IDC_RADIO_FreeParam))->GetCheck())
	{
		EditParam.GetWindowText(strJson);
		strJson.TrimLeft();
		strJson.TrimRight();
		log("车辆信息确认特情(使用自由配置参数):");
		log(strJson.GetBuffer(0));
	}

	if(((CButton *)GetDlgItem(IDC_RADIO_DefaultParam))->GetCheck())
	{
	// 表示整个 json 对象
	Json::Value root;
	// 标识ETC特情交互参数
	Json::Value FeeAuthorize;
	FeeAuthorize["SpecialEventNo"] = Json::Value(m_EventNo);
	FeeAuthorize["MessageCode"] = Json::Value("011");
	
	Json::Value Data;
	Data["authorizeType"] = Json::Value(0);	
	FeeAuthorize["Data"] = Data;

	root["Authorize"] = FeeAuthorize;

	//转化为字符串
	Json::FastWriter fast_writer;
	std::string str = fast_writer.write(root).c_str();
	log("最小费额授权:");
	log(str);

	strJson.Format("%s",str.c_str());
	}

	int iSize = strJson.GetLength();
	//utf8编码
    //char* strJsonU8 = G2U(str.c_str());
	//int iSize = (int)strlen(strJsonU8);
	
	bool ret = false;
	ret = fn_EventFeeAuthorize(strJson.GetBuffer(0), iSize, 2);
	if(!ret)
	{
		log("调用最小费额授权接口返回失败!");
	}

	//delete[] strJsonU8;

	return;
}

void CTestDevIFDlg::OnBnClickedBtnDestroyEtcEvent()
{
	// TODO: 在此添加控件通知处理程序代码
	if(bLoaded == false)
	{
		log("未加载共享库!");
		return;
	}
	if(m_bEtcEventEnvInitedFlag == false)
	{
		log("未初始化ETC特情环境!");
		return;
	}

	log("卸载EtcEvent环境接口开始调用");	
	bool bRet = false;
	bRet = fn_EventDestroy();
	if(bRet)
	{
		log("调用卸载EtcEvent环境接口返回成功");
		m_bEtcEventEnvInitedFlag = false;
	}
	else
	{
		log("调用卸载EtcEvent环境接口返回失败");
	}

}

void CTestDevIFDlg::OnBnClickedBtnDestroyNetPay()
{
	// TODO: 在此添加控件通知处理程序代码
	if(bLoaded == false)
	{
		log("未加载共享库!");
		return;
	}
	if(m_bNetPayEnvInitedFlag == false)
	{
		log("未初始化移动支付环境!");
		return;
	}

	log("卸载移动支付环境接口开始调用");	
	bool bRet = false;
	bRet = fn_Destroy();
	if(bRet)
	{
		log("调用卸载移动支付环境接口返回成功!");
		m_bNetPayEnvInitedFlag = false;
	}
	else
	{
		log("调用卸载移动支付环境接口返回失败!");
	}
	return;
}

void CTestDevIFDlg::OnBnClickedBtnGetLastErrorDesc()
{
	// TODO: 在此添加控件通知处理程序代码
	if(bLoaded == false)
	{
		log("未加载共享库!");
		return;
	}
	if(m_bNetPayEnvInitedFlag == false)
	{
		log("未初始化移动支付环境!");
		return;
	}

	log("调用接口IF_GetLastErrorDesc()");
	static std::string ErrorDesc;
	ErrorDesc = fn_GetLastErrorDesc();
	log("返回上次错误信息:" + ErrorDesc);

	return;


}

void CTestDevIFDlg::OnBnClickedBtnFreeDll()
{
	// TODO: 在此添加控件通知处理程序代码
    if(bLoaded == false)
	{
		log("未加载共享库!");
		return;
	}

	//框架库中有一个线程不能通过接口调用退出，此处直接卸载dll会崩溃。
	return;

	
	BOOL bRet = FreeLibrary(gd_dll);
	if(bRet == TRUE)
	{
		log("卸载中间件成功！");
		bLoaded = false;
	}
	else
	{
		log("卸载中间件失败！");
	}
	
	return;
}

void CTestDevIFDlg::OnBnClickedBtnDebitXML()
{
	// TODO: 在此添加控件通知处理程序代码	
	if(bLoaded == false)
	{
		log("未加载共享库!");
		return;
	}

	if(m_bNetPayEnvInitedFlag == false)
	{
		log("未初始化支付环境！");
		return;
	}

	if(m_bEtcEventEnvInitedFlag == false)
	{
		log("未初始ETC特情环境！");
	}

	// TODO: 在此添加控件通知处理程序代码
	GetUIDebitInfo();
	tagPayKeyItems* pPayKeyItems = new tagPayKeyItems();
	FillpPayKeyItemsStruct(0x01, 0x00, _T(""), *pPayKeyItems);

	CMarkup xmlDebit;
	xmlDebit.SetDoc( _T("<?xml version=\"1.0\" encoding=\"GB2312\"?>\r\n") );
	xmlDebit.AddElem( _T("DebitParamContext") );
	xmlDebit.IntoElem();

	xmlDebit.AddElem( _T("Version"),		_T("1.0") );
	xmlDebit.AddElem( _T("Data") );

	xmlDebit.IntoElem();
	xmlDebit.AddElem( _T("OverTime"),			pPayKeyItems->OverTime );
	xmlDebit.AddElem( _T("OperationMode"),		pPayKeyItems->OperationMode );

	xmlDebit.AddElem( _T("Debit") );
	xmlDebit.IntoElem();
	xmlDebit.AddElem( _T("PayIdentifier"),		pPayKeyItems->PayIdentifier );
	xmlDebit.AddElem( _T("Money"),				pPayKeyItems->Money );
	xmlDebit.AddElem( _T("SubTime"),			CTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S") );
	xmlDebit.AddElem( _T("Type"),				0x01 );
	xmlDebit.AddElem( _T("PayCertificateCode"),	pPayKeyItems->PayCertificateCode );
	xmlDebit.AddElem( _T("PayCertificateType"),	pPayKeyItems->PayCertificateType );
	xmlDebit.OutOfElem();

	xmlDebit.AddElem( _T("Vehicle") );
	xmlDebit.IntoElem();
	xmlDebit.AddElem( _T("Type"),				pPayKeyItems->VehicleType );
	xmlDebit.AddElem( _T("Class"),				pPayKeyItems->VehicleClass );
	xmlDebit.AddElem( _T("License"),			pPayKeyItems->VehicleLicense );
	xmlDebit.AddElem( _T("VLColor"),			pPayKeyItems->VLColor );
	xmlDebit.AddElem( _T("AxisCount"),			pPayKeyItems->AxisCount );
	xmlDebit.AddElem( _T("Weight"),				pPayKeyItems->TotalWeight );
	xmlDebit.AddElem( _T("LimitWeight"),		pPayKeyItems->LimitWeight );
	//add 2018-03 
	xmlDebit.AddElem(_T("AutoPlate"));
	xmlDebit.IntoElem();
	xmlDebit.AddElem(_T("AutoLicense"),pPayKeyItems->AutoLicense);
	xmlDebit.AddElem(_T("AutoColor"),pPayKeyItems->AutoColor);	
	xmlDebit.OutOfElem();
	xmlDebit.OutOfElem();

	xmlDebit.AddElem( _T("Operation") );
	xmlDebit.IntoElem();
	xmlDebit.AddElem( _T("PassCertificateType"),pPayKeyItems->PassCertificateType );
	xmlDebit.AddElem( _T("CardID"),				pPayKeyItems->CardID );
	xmlDebit.AddElem( _T("TollDate"),			pPayKeyItems->TollDate );
	xmlDebit.AddElem( _T("ShiftID"),			pPayKeyItems->ShiftID );
	xmlDebit.AddElem( _T("TicketNo"),			pPayKeyItems->TicketNo );
	xmlDebit.AddElem( _T("OperatorID"),			pPayKeyItems->OperatorID );
	xmlDebit.AddElem( _T("OperatorName"),			pPayKeyItems->OperatorName );
	xmlDebit.AddElem( _T("Distance"),			pPayKeyItems->Distance );
	xmlDebit.AddElem( _T("AutoPrint"),			1);
	xmlDebit.OutOfElem();

	xmlDebit.AddElem( _T("Entry") );
	xmlDebit.IntoElem();
	xmlDebit.AddElem( _T("StationID"),			pPayKeyItems->EntryStationID );
	xmlDebit.AddElem( _T("StationName"),		pPayKeyItems->EntryStationName );
	xmlDebit.AddElem( _T("Time"),				pPayKeyItems->EntryTime.Format(_T("%Y-%m-%d %H:%M:%S")));
	xmlDebit.AddElem( _T("LaneID"),				pPayKeyItems->EntryLane );
	xmlDebit.AddElem( _T("License"),			pPayKeyItems->EntryVehicleLicense );
	xmlDebit.AddElem( _T("VColor"),				pPayKeyItems->EntryVLColor );
	xmlDebit.AddElem( _T("VClass"),				pPayKeyItems->EntryVehicleClass );
	xmlDebit.AddElem( _T("VType"),				pPayKeyItems->EntryVehicleType );
	xmlDebit.AddElem( _T("RoadID"),				pPayKeyItems->EntryRoadID );
	xmlDebit.AddElem( _T("AreaID"),				pPayKeyItems->EntryAreaID );
	xmlDebit.OutOfElem();

	xmlDebit.OutOfElem();
	xmlDebit.OutOfElem();
	
	CString strDataString;
	strDataString = xmlDebit.GetDoc();

	log("扣费XML参数:");
	log(strDataString.GetBuffer(0));
	
	int iSize = strDataString.GetLength();
	int DataParamType = 1;
	fn_DebitMoney(strDataString.GetBuffer(0), iSize, DataParamType);

	strDataString.ReleaseBuffer();
	delete pPayKeyItems;
	pPayKeyItems = NULL;
}


void CTestDevIFDlg::OnBnClickedBtnCancelXML()
{
	// TODO: 在此添加控件通知处理程序代码	
	if(bLoaded == false)
	{
		log("未加载共享库!");
		return;
	}

	if(m_bNetPayEnvInitedFlag == false)
	{
		log("未初始化支付环境！");
		return;
	}

	if(m_bEtcEventEnvInitedFlag == false)
	{
		log("未初始ETC特情环境！");
	}

	int iSize = 0x00;
	CString strDataString;

	CMarkup xmlCancel;
	xmlCancel.SetDoc( _T("<?xml version=\"1.0\" encoding=\"GB2312\"?>\r\n") );
	xmlCancel.AddElem( _T("DebitCancelParamContext") );//root
	xmlCancel.IntoElem();

	xmlCancel.AddElem( _T("Version"),		_T("1.0") );
	xmlCancel.AddElem( _T("ProvinceID"),	m_iProvinceID/*63*/ );
	xmlCancel.AddElem( _T("Data") );

	xmlCancel.IntoElem();

	xmlCancel.AddElem( _T("TradeKeyItem"));
	xmlCancel.IntoElem();
	xmlCancel.AddElem( _T("PayIdentifier"),	 m_strLastPayIdentifier);
	xmlCancel.OutOfElem();

	xmlCancel.OutOfElem();

	xmlCancel.OutOfElem();

	strDataString = xmlCancel.GetDoc();
	iSize = strDataString.GetLength();

	log("撤单XML参数:");
	log(strDataString.GetBuffer(0));	

	int DataParamType = 1;
	fn_DebitCancel(strDataString.GetBuffer(0), iSize, DataParamType);
	strDataString.ReleaseBuffer();

	return;
}


void CTestDevIFDlg::OnBnClickedBtnEtcdevstatus()
{
	// TODO: 在此添加控件通知处理程序代码
	bool bRet = false;
	unsigned int uStatus;
	bRet = fn_EventGetComponentStatus(uStatus);
	if(bRet)
	{
		log("调用ETC特情获取设备状态接口返回成功!");
		log("返回状态值:" + to_string(uStatus));
	}
	else
	{
		log("调用ETC特情获取设备状态接口返回失败!");
	}
	return;
}

void CTestDevIFDlg::OnBnClickedBtnNetpaydevstatus()
{
	// TODO: 在此添加控件通知处理程序代码
	bool bRet = false;
	unsigned int uStatus;
	bRet = fn_GetComponentStatus(uStatus);
	if(bRet)
	{
		log("调用移动支付获取设备接口返回成功!");
		log("返回状态值:" + to_string(uStatus));
	}
	else
	{
		log("调用移动支付获取设备接口返回失败!");
	}
	return;
}

/************************************界面循环播放*********************************************/

void CTestDevIFDlg::GuiPlay1Eventstart()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strJson;

		// 表示整个 json 对象
	Json::Value root;
	// 标识ETC特情交互参数
	Json::Value EventDealStart;
	CString TimeStr = GetUniqueTimeStr();
	std::string timestr(TimeStr.GetBuffer());
	m_EventNo = timestr + "33b2b009e44b15" + to_string(++No);
	EventDealStart["SpecialEventNo"] = Json::Value(m_EventNo);
	EventDealStart["MessageCode"] = Json::Value("001");
	// 标识详细Data数据项
	Json::Value Data;		
	/*Data["ProvinceName"] = Json::Value("广东省");
	Data["ProvinceNo"] = Json::Value("44");
	Data["RoadName"] = Json::Value("广州北二环");
	Data["RoadNo"] = Json::Value("0093");
	Data["StationName"] = Json::Value("石湖收费站");
	Data["StationNo"] = Json::Value("009305");
	Data["LaneNo"] = Json::Value("36");*/
	CString str1 = "豫A123";
	CString temp;
	VehPlateTail++;
	temp.Format(_T("%d"), VehPlateTail);
	Data["VehPlate"] = Json::Value(str1 + temp);
	//Data["VehPlate"] = Json::Value(str1);
	m_CurrentVehPlate = str1 + temp;
	//m_CurrentVehPlate = str1;
	Data["VehPlateColor"] = Json::Value(1);
	m_CurrentVehPlateColor = 1;
	Data["VehClass"] = Json::Value(1);
	m_CurrentVehClass = 1;
	Data["VehType"] = Json::Value(1);
	m_CurrentVehType = 1;
	Data["VehSeatCount"] = Json::Value(5);
	int m_CurrentAxisCount = 2;
	Data["axleCount"] = Json::Value(2);
	Data["CardNum"] = Json::Value("123456");
	Data["VehSeatCount"] = Json::Value(5);
	//Data["EventStartTime"] = Json::Value("1970-01-01 08:00:00");
	
	Data["EventStartTime"] = Json::Value(MyGetCurrenTime());
	Data["EventDescribe"] = Json::Value("卡未插好");
	Data["EventTypeNo"] = Json::Value(12);
	Data["EventTypeName"] = Json::Value(GetEventTypeName(12));
	
	EventDealStart["Data"] = Data;
	root["EventDealStart"]	= EventDealStart;

	//转化为字符串
	Json::FastWriter fast_writer;
	std::string str = fast_writer.write(root).c_str();
	log("特情事件处理开始:");
	log(str);

	//utf8编码
	//char* strJsonU8 = G2U(str.c_str());
	//int iSize = (int)strlen(strJsonU8);
	
	//GBK编码
	strJson.Format(_T("%s"),str.c_str());

	int iSize = strJson.GetLength();

	fn_EventDealStart(strJson.GetBuffer(0), iSize, 2);
	//ret = fn_EventDealStart(strJsonU8, iSize, 2);

	//delete[] strJsonU8;

	return;
}



void CTestDevIFDlg::GuiPlay2Eventstart()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strJson;

		// 表示整个 json 对象
	Json::Value root;
	// 标识ETC特情交互参数
	Json::Value EventDealStart;
	CString TimeStr = GetUniqueTimeStr();
	std::string timestr(TimeStr.GetBuffer());
	m_EventNo = timestr + "33b2b009e44b15" + to_string(++No);
	EventDealStart["SpecialEventNo"] = Json::Value(m_EventNo);
	EventDealStart["MessageCode"] = Json::Value("001");
	// 标识详细Data数据项
	Json::Value Data;		
	/*Data["ProvinceName"] = Json::Value("广东省");
	Data["ProvinceNo"] = Json::Value("44");
	Data["RoadName"] = Json::Value("广州北二环");
	Data["RoadNo"] = Json::Value("0093");
	Data["StationName"] = Json::Value("石湖收费站");
	Data["StationNo"] = Json::Value("009305");
	Data["LaneNo"] = Json::Value("36");*/
	CString str1 = "豫A123";
	CString temp;
	VehPlateTail++;
	temp.Format(_T("%d"), VehPlateTail);
	Data["VehPlate"] = Json::Value(str1 + temp);
	//Data["VehPlate"] = Json::Value(str1);
	m_CurrentVehPlate = str1 + temp;
	//m_CurrentVehPlate = str1;
	Data["VehPlateColor"] = Json::Value(1);
	m_CurrentVehPlateColor = 1;
	Data["VehClass"] = Json::Value(1);
	m_CurrentVehClass = 1;
	Data["VehType"] = Json::Value(1);
	m_CurrentVehType = 1;
	Data["VehSeatCount"] = Json::Value(7);
	int m_CurrentAxisCount = 2;
	Data["axleCount"] = Json::Value(2);
	Data["CardNum"] = Json::Value("123456");
	Data["VehSeatCount"] = Json::Value(7);
	//Data["EventStartTime"] = Json::Value("1970-01-01 08:00:00");
	
	Data["EventStartTime"] = Json::Value(MyGetCurrenTime());
	Data["EventDescribe"] = Json::Value("余额不足");
	Data["EventTypeNo"] = Json::Value(17);
	Data["EventTypeName"] = Json::Value(GetEventTypeName(17));
	
	EventDealStart["Data"] = Data;
	root["EventDealStart"]	= EventDealStart;

	//转化为字符串
	Json::FastWriter fast_writer;
	std::string str = fast_writer.write(root).c_str();
	log("特情事件处理开始:");
	log(str);

	//utf8编码
	//char* strJsonU8 = G2U(str.c_str());
	//int iSize = (int)strlen(strJsonU8);
	
	//GBK编码
	strJson.Format(_T("%s"),str.c_str());

	int iSize = strJson.GetLength();

	fn_EventDealStart(strJson.GetBuffer(0), iSize, 2);
	//ret = fn_EventDealStart(strJsonU8, iSize, 2);

	//delete[] strJsonU8;

	return;
}


void CTestDevIFDlg::GuiPlay3Eventstart()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strJson;

		// 表示整个 json 对象
	Json::Value root;
	// 标识ETC特情交互参数
	Json::Value EventDealStart;
	CString TimeStr = GetUniqueTimeStr();
	std::string timestr(TimeStr.GetBuffer());
	m_EventNo = timestr + "33b2b009e44b15" + to_string(++No);
	EventDealStart["SpecialEventNo"] = Json::Value(m_EventNo);
	EventDealStart["MessageCode"] = Json::Value("001");
	// 标识详细Data数据项
	Json::Value Data;		
	/*Data["ProvinceName"] = Json::Value("广东省");
	Data["ProvinceNo"] = Json::Value("44");
	Data["RoadName"] = Json::Value("广州北二环");
	Data["RoadNo"] = Json::Value("0093");
	Data["StationName"] = Json::Value("石湖收费站");
	Data["StationNo"] = Json::Value("009305");
	Data["LaneNo"] = Json::Value("36");*/
	CString str1 = "豫A123";
	CString temp;
	VehPlateTail++;
	temp.Format(_T("%d"), VehPlateTail);
	Data["VehPlate"] = Json::Value(str1 + temp);
	//Data["VehPlate"] = Json::Value(str1);
	m_CurrentVehPlate = str1 + temp;
	//m_CurrentVehPlate = str1;
	Data["VehPlateColor"] = Json::Value(1);
	m_CurrentVehPlateColor = 1;
	Data["VehClass"] = Json::Value(1);
	m_CurrentVehClass = 1;
	Data["VehType"] = Json::Value(1);
	m_CurrentVehType = 1;
	Data["VehSeatCount"] = Json::Value(7);
	int m_CurrentAxisCount = 2;
	Data["axleCount"] = Json::Value(2);
	Data["CardNum"] = Json::Value("123456");
	Data["VehSeatCount"] = Json::Value(7);
	//Data["EventStartTime"] = Json::Value("1970-01-01 08:00:00");
	
	Data["EventStartTime"] = Json::Value(MyGetCurrenTime());
	Data["EventDescribe"] = Json::Value("无入口信息");
	Data["EventTypeNo"] = Json::Value(15);
	Data["EventTypeName"] = Json::Value(GetEventTypeName(15));
	
	EventDealStart["Data"] = Data;
	root["EventDealStart"]	= EventDealStart;

	//转化为字符串
	Json::FastWriter fast_writer;
	std::string str = fast_writer.write(root).c_str();
	log("特情事件处理开始:");
	log(str);

	//utf8编码
	//char* strJsonU8 = G2U(str.c_str());
	//int iSize = (int)strlen(strJsonU8);
	
	//GBK编码
	strJson.Format(_T("%s"),str.c_str());

	int iSize = strJson.GetLength();

	fn_EventDealStart(strJson.GetBuffer(0), iSize, 2);
	//ret = fn_EventDealStart(strJsonU8, iSize, 2);

	//delete[] strJsonU8;

	return;
}

void CTestDevIFDlg::GuiPlayXSwipeCardNotify()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strJson;
	// 表示整个 json 对象
	Json::Value root;
	// 标识ETC特情交互参数
	Json::Value CardOperationNotify;
	CardOperationNotify["SpecialEventNo"] = Json::Value(m_EventNo);
	CardOperationNotify["MessageCode"] = Json::Value("009");
	
	Json::Value Data;
	Data["NotifyType"] = Json::Value(1);	
	CardOperationNotify["Data"] = Data;

	root["CardOperationNotify"] = CardOperationNotify;

	//转化为字符串
	Json::FastWriter fast_writer;
	std::string str = fast_writer.write(root).c_str();

	//utf8编码
	//char* strJsonU8 = G2U(str.c_str());
	//int iSize = (int)strlen(strJsonU8);

	strJson.Format(_T("%s"), str.c_str());

	int iSize = strJson.GetLength();
	
	//ret = fn_EventCardOperationNotify(strJsonU8, iSize, 2);
	fn_EventCardOperationNotify(strJson.GetBuffer(0), iSize, 2);

	//delete[] strJsonU8;

	return;
}

void CTestDevIFDlg::GuiPlayXShowFeeInfo()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strJson;

	// 表示整个 json 对象
	Json::Value root;
	// 标识ETC特情交互参数
	Json::Value EventShowFeeInfo;
	EventShowFeeInfo["SpecialEventNo"] = Json::Value(m_EventNo);
	EventShowFeeInfo["MessageCode"] = Json::Value("005");

	EventShowFeeInfo["PayType"] = Json::Value(1);
	EventShowFeeInfo["DefaultFeeItemNo"] = Json::Value(1);
	EventShowFeeInfo["FeeCount"] = Json::Value(2);

	Json::Value Array;
	Json::Value Item;
	Item["Entry"] = Json::Value("广东-莞深高速-石碣站");
	Item["PathInfo"] = Json::Value("0729-0730-0731");
	Item["Fee"] = Json::Value("3000");
	Item["ChargeType"] = Json::Value(2);
	Item["ItemNo"] = Json::Value(1);
	Array.append(Item);

	Item["Entry"] = Json::Value("广东-莞深高速-石碣站");
	Item["PathInfo"] = Json::Value("0729-0730-0731");
	Item["Fee"] = Json::Value("4000");
	Item["ChargeType"] = Json::Value(2);
	Item["ItemNo"] = Json::Value(2);
	Array.append(Item);	
	EventShowFeeInfo["FeeInfo"] = Array;

	EventShowFeeInfo["TotalVehNum"] = Json::Value(4);
	Json::Value VehQueue;
	Json::Value VehItem;
	VehItem["Index"] = Json::Value(0);
	VehItem["VehPlate"] = Json::Value("豫A121121");
	VehItem["VehPlateColor"] = Json::Value(1);
	VehItem["Fee"] = Json::Value("3000");
	VehItem["DealResult"] = Json::Value(0);
	VehItem["FailReason"] = Json::Value("");
	VehQueue.append(VehItem);

	VehItem["Index"] = Json::Value(1);
	VehItem["VehPlate"] = Json::Value("豫A122122");
	VehItem["VehPlateColor"] = Json::Value(1);
	VehItem["Fee"] = Json::Value("3000");
	VehItem["DealResult"] = Json::Value(0);
	VehItem["FailReason"] = Json::Value("");
	VehQueue.append(VehItem);

	VehItem["Index"] = Json::Value(2);
	VehItem["VehPlate"] = Json::Value("豫A123123");
	VehItem["VehPlateColor"] = Json::Value(1);
	VehItem["Fee"] = Json::Value("3000");
	VehItem["DealResult"] = Json::Value(1);
	VehItem["FailReason"] = Json::Value("余额不足");
	VehQueue.append(VehItem);

	VehItem["Index"] = Json::Value(3);
	VehItem["VehPlate"] = Json::Value("豫A124124");
	VehItem["VehPlateColor"] = Json::Value(1);
	VehItem["Fee"] = Json::Value("3000");
	VehItem["DealResult"] = Json::Value(2);
	VehItem["FailReason"] = Json::Value("");
	VehQueue.append(VehItem);
	EventShowFeeInfo["VehQueue"] = VehQueue;

	root["EventShowFeeInfo"] = EventShowFeeInfo;

	//转化为字符串
	Json::FastWriter fast_writer;
	std::string str = fast_writer.write(root).c_str();

	strJson.Format(_T("%s"),str.c_str());

	int iSize = strJson.GetLength();
	
	fn_EventShowFeeInfo(strJson.GetBuffer(0), iSize, 2);

	return;
}


void CTestDevIFDlg::GuiPlay13PayResultShow()
{
	//// TODO: 手动添加消息处理函数
	CString strJson;

	// 表示整个 json 对象
	Json::Value root;
	// 标识ETC特情交互参数
	Json::Value EventPayResultShow;
	EventPayResultShow["SpecialEventNo"] = Json::Value(m_EventNo);
	EventPayResultShow["MessageCode"] = Json::Value("006");
	
	//EventInfo
	Json::Value EventInfo;	
	EventInfo["RequestTime"] = Json::Value(MyGetCurrenTime());
	EventInfo["RequestType"] = Json::Value("004");
	EventInfo["PayResult"] = Json::Value(0);
	EventInfo["Describe"] = Json::Value("刷卡支付成功");
	EventInfo["TimeLimit"] = Json::Value("360");
	EventPayResultShow["EventInfo"] = EventInfo;
	
	root["EventPayResultShow"] = EventPayResultShow;

	//转化为字符串
	Json::FastWriter fast_writer;
	std::string str = fast_writer.write(root).c_str();

	strJson.Format(_T("%s"),str.c_str());

	int iSize = strJson.GetLength();
	fn_EventPayResultDisplay(strJson.GetBuffer(0), iSize, 2);

	return;
}


void CTestDevIFDlg::GuiPlay2PayResultShow()
{
	//// TODO: 手动添加消息处理函数
	CString strJson;

	// 表示整个 json 对象
	Json::Value root;
	// 标识ETC特情交互参数
	Json::Value EventPayResultShow;
	EventPayResultShow["SpecialEventNo"] = Json::Value(m_EventNo);
	EventPayResultShow["MessageCode"] = Json::Value("006");
	
	//EventInfo
	Json::Value EventInfo;	
	EventInfo["RequestTime"] = Json::Value(MyGetCurrenTime());
	EventInfo["RequestType"] = Json::Value("004");
	EventInfo["PayResult"] = Json::Value(0);
	EventInfo["Describe"] = Json::Value("移动支付成功");
	EventInfo["TimeLimit"] = Json::Value("360");
	EventPayResultShow["EventInfo"] = EventInfo;
	
	root["EventPayResultShow"] = EventPayResultShow;

	//转化为字符串
	Json::FastWriter fast_writer;
	std::string str = fast_writer.write(root).c_str();

	strJson.Format(_T("%s"),str.c_str());

	int iSize = strJson.GetLength();
	fn_EventPayResultDisplay(strJson.GetBuffer(0), iSize, 2);

	return;
}


void CTestDevIFDlg::GuiPlay3CheckEntryInfoNone()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strJson;

	// 表示整个 json 对象
	Json::Value root;
	// 标识ETC特情交互参数
	Json::Value EventCheckEntryInfo;
	EventCheckEntryInfo["SpecialEventNo"] = Json::Value(m_EventNo);
	EventCheckEntryInfo["MessageCode"] = Json::Value("005");
	CString str1 = "豫A123";
	CString temp;
	temp.Format(_T("%d"), VehPlateTail);
	EventCheckEntryInfo["VehPlate"] = Json::Value(str1 + temp);
	
	//EventInfo
	Json::Value EventInfo;	
	EventInfo["RequestTime"] = Json::Value(MyGetCurrenTime());
	EventInfo["RequestType"] = Json::Value("004");
	EventInfo["EventDescribe"] = Json::Value("确认车辆信息是否正确");
	EventInfo["MMIDemand"] = Json::Value("交互诉求");
	EventInfo["TimeLimit"] = Json::Value("360");

	EventCheckEntryInfo["EventInfo"] = EventInfo;

	Json::Value Array;
	/*
		Json::Value Item;
		Item["StationName"] = Json::Value("香雪收费站");
		Item["StationID"] = Json::Value("G1501440010090");
		Item["StationHex"] = Json::Value("44011D0B");
		Item["laneid"] = Json::Value("0161");
		Item["laneHex"] = Json::Value("A1");
		Item["sEnTime"] = Json::Value("1970-01-01 08:00:00");
		Array.append(Item);
		Item["StationName"] = Json::Value("长平收费站");
		Item["StationID"] = Json::Value("G1501440010091");
		Item["StationHex"] = Json::Value("44011D0C");
		Item["laneid"] = Json::Value("0162");
		Item["laneHex"] = Json::Value("A2");
		Item["sEnTime"] = Json::Value("1970-01-01 08:00:00");
		Array.append(Item);
	*/
	EventCheckEntryInfo["EntryInfo"] = Array;

	root["EventCheckEntryInfo"] = EventCheckEntryInfo;

	//转化为字符串
	Json::FastWriter fast_writer;
	std::string str = fast_writer.write(root).c_str();

	strJson.Format(_T("%s"),str.c_str());
	int iSize = strJson.GetLength();

	fn_EventCheckEntryInfo(strJson.GetBuffer(0), iSize, 2);

	return;
}

void CTestDevIFDlg::GuiPlayXEventstop()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strJson;

	// 表示整个 json 对象
	Json::Value root;
	// 标识ETC特情交互参数
	Json::Value EventDealStop;
	EventDealStop["SpecialEventNo"] = Json::Value(m_EventNo);
	EventDealStop["MessageCode"] = Json::Value("006");
	
	//EventEndCases
	Json::Value EventEndCases;	
	EventEndCases["EventVehPlate"] = Json::Value(m_CurrentVehPlate);
	EventEndCases["EventEndTime"] = Json::Value(MyGetCurrenTime());
	EventEndCases["EventDescribe"] = Json::Value("车辆无入口信息");
	EventEndCases["EventEndReason"] = Json::Value("特情处理完成，支付成功");
	EventEndCases["EventHandleMethodNo"] = Json::Value(m_EventHandleMethodNo);
	EventEndCases["EventHandleMethodName"] = Json::Value(GetEventHandleMethodName(m_EventHandleMethodNo));
	EventDealStop["EventEndCases"] = EventEndCases;
	
	root["EventDealStop"] = EventDealStop;

	//转化为字符串
	Json::FastWriter fast_writer;
	std::string str = fast_writer.write(root).c_str();

	strJson.Format(_T("%s"),str.c_str());

	int iSize = strJson.GetLength();
	
	fn_EventDealStop(strJson.GetBuffer(0), iSize, 2);

	return;
}

UINT CTestDevIFDlg::GuiPlay1ThreadFun(LPVOID pParam)
{
	CTestDevIFDlg *dlg = (CTestDevIFDlg*)pParam;
	dlg->log("GUI循环播放1开始启动...");	

	while(true)
	{
		dlg->log("GUI循环播放1: 特情开始(卡未插好)");	
		dlg->GuiPlay1Eventstart();
		Sleep(10*1000);

		dlg->log("GUI循环播放1: 请刷卡");	
		dlg->GuiPlayXSwipeCardNotify();
		Sleep(10*1000);

		dlg->log("GUI循环播放1: 费额展示");	
		dlg->GuiPlayXShowFeeInfo();
		Sleep(10*1000);

		dlg->log("GUI循环播放1: 刷卡支付成功");	
		dlg->GuiPlay13PayResultShow();
		Sleep(10*1000);
	
		dlg->log("GUI循环播放1: 特情结束");	
		dlg->GuiPlayXEventstop();
		Sleep(10*1000);
	}

	return 0;
}

UINT CTestDevIFDlg::GuiPlay2ThreadFun(LPVOID pParam)
{
	CTestDevIFDlg *dlg = (CTestDevIFDlg*)pParam;
	dlg->log("GUI循环播放2开始启动...");
	while(true)
	{
		dlg->log("GUI循环播放2: 特情开始(余额不足)");	
		dlg->GuiPlay2Eventstart();
		Sleep(10*1000);

		dlg->log("GUI循环播放2: 请刷卡");	
		dlg->GuiPlayXSwipeCardNotify();
		Sleep(10*1000);

		dlg->log("GUI循环播放2: 费额展示");	
		dlg->GuiPlayXShowFeeInfo();
		Sleep(10*1000);

		dlg->log("GUI循环播放2: 移动支付成功");	
		dlg->GuiPlay2PayResultShow();
		Sleep(10*1000);
	
		dlg->log("GUI循环播放2: 特情结束");	
		dlg->GuiPlayXEventstop();
		Sleep(10*1000);
	}

	return 0;
}

UINT CTestDevIFDlg::GuiPlay3ThreadFun(LPVOID pParam)
{
	CTestDevIFDlg *dlg = (CTestDevIFDlg*)pParam;
	dlg->log("GUI循环播放3开始启动...");
	while(true)
	{
		dlg->log("GUI循环播放3: 特情开始(无入口信息)");	
		dlg->GuiPlay3Eventstart();
		Sleep(10*1000);

		dlg->log("GUI循环播放3: 请刷卡");	
		dlg->GuiPlayXSwipeCardNotify();
		Sleep(10*1000);

		dlg->log("GUI循环播放3: 后台正在确认入口信息");	
		dlg->GuiPlay3CheckEntryInfoNone();
		Sleep(10*1000);

		dlg->log("GUI循环播放3: 费额展示");	
		dlg->GuiPlayXShowFeeInfo();
		Sleep(10*1000);

		dlg->log("GUI循环播放3: 刷卡支付成功");	
		dlg->GuiPlay13PayResultShow();
		Sleep(10*1000);
	
		dlg->log("GUI循环播放3: 特情结束");	
		dlg->GuiPlayXEventstop();
		Sleep(10*1000);
	}

	return 0;
}

UINT CTestDevIFDlg::GuiPlayTotalThreadFun(LPVOID pParam)
{
	CTestDevIFDlg *dlg = (CTestDevIFDlg*)pParam;
	dlg->log("GUI总循环播放开始启动...");

	while(true)
	{
		dlg->log("GUI总循环播放: 特情开始(卡未插好)");	
		dlg->GuiPlay1Eventstart();
		Sleep(10*1000);

		dlg->log("GUI总循环播放: 请刷卡");	
		dlg->GuiPlayXSwipeCardNotify();
		Sleep(10*1000);

		dlg->log("GUI总循环播放: 费额展示");	
		dlg->GuiPlayXShowFeeInfo();
		Sleep(10*1000);

		dlg->log("GUI总循环播放: 刷卡支付成功");	
		dlg->GuiPlay13PayResultShow();
		Sleep(10*1000);
	
		dlg->log("GUI总循环播放: 特情结束");	
		dlg->GuiPlayXEventstop();
		Sleep(10*1000);


		dlg->log("GUI总循环播放: 特情开始(余额不足)");	
		dlg->GuiPlay2Eventstart();
		Sleep(10*1000);

		dlg->log("GUI总循环播放: 请刷卡");	
		dlg->GuiPlayXSwipeCardNotify();
		Sleep(10*1000);

		dlg->log("GUI总循环播放: 费额展示");	
		dlg->GuiPlayXShowFeeInfo();
		Sleep(10*1000);

		dlg->log("GUI总循环播放: 移动支付成功");	
		dlg->GuiPlay2PayResultShow();
		Sleep(10*1000);
	
		dlg->log("GUI总循环播放: 特情结束");	
		dlg->GuiPlayXEventstop();
		Sleep(10*1000);


		dlg->log("GUI总循环播放: 特情开始(无入口信息)");	
		dlg->GuiPlay3Eventstart();
		Sleep(10*1000);

		dlg->log("GUI总循环播放: 请刷卡");	
		dlg->GuiPlayXSwipeCardNotify();
		Sleep(10*1000);

		dlg->log("GUI总循环播放: 后台正在确认入口信息");	
		dlg->GuiPlay3CheckEntryInfoNone();
		Sleep(10*1000);

		dlg->log("GUI总循环播放: 费额展示");	
		dlg->GuiPlayXShowFeeInfo();
		Sleep(10*1000);

		dlg->log("GUI总循环播放: 刷卡支付成功");	
		dlg->GuiPlay13PayResultShow();
		Sleep(10*1000);
	
		dlg->log("GUI总循环播放: 特情结束");	
		dlg->GuiPlayXEventstop();
		Sleep(10*1000);
	}
}


void CTestDevIFDlg::OnBnClickedBtnGuiPlay1Start()
{
	// TODO: 在此添加控件通知处理程序代码
	if(bLoaded == false)
	{
		log("未加载共享库！");
		return;
	}

	if(m_bEtcEventEnvInitedFlag == false)
	{
		log("未初始化ETC特情环境！");
		return;
	}

	//首先暂停其它两个线程的运行
	if(m_bThread2Running)
	{
		log("GUI循环播放2正在运行中，请先停止GUI循环播放2 ！！！");
		return;
	}

	if(m_bThread3Running)
	{
		log("GUI循环播放3正在运行中，请先停止GUI循环播放3 ！！！");
		return;
	}

	if(m_bThreadTotalRunning)
	{
		log("GUI总循环播放正在运行中，请先停止GUI总循环播放 ！！！");
		return;
	}

	if(m_bThread1Running)
	{
		log("GUI循环播放1正在运行中，请不要重复启动 ！！！");
		return;
	}
	
	log("启动GUI循环播放1.");
	if(m_pGuiPlay1Thread == NULL)
		m_pGuiPlay1Thread = AfxBeginThread(GuiPlay1ThreadFun,(LPVOID)this);
	else
		ResumeThread(m_pGuiPlay1Thread->m_hThread);
	
	m_bThread1Running = true;

	return;
}

void CTestDevIFDlg::OnBnClickedBtnGuiPlay1Stop()
{
	// TODO: 在此添加控件通知处理程序代码
	if(bLoaded == false)
	{
		log("未加载共享库！");
		return;
	}

	if(m_bEtcEventEnvInitedFlag == false)
	{
		log("未初始化ETC特情环境！");
		return;
	}

	if(m_bThread1Running == false)
	{
		log("停止GUI循环播放1，当前GUI循环播放1未运行！！！");
		return;
	}

	if(m_pGuiPlay1Thread != NULL)
	{
		log("停止GUI循环播放1.");
		 //方法1
		 DWORD exitCode=12345;
		 TerminateThread(m_pGuiPlay1Thread->m_hThread, exitCode);
		 m_pGuiPlay1Thread = NULL;
	}
	m_bThread1Running = false;

	return;
}

void CTestDevIFDlg::OnBnClickedBtnGuiPlay2Start()
{
	// TODO: 在此添加控件通知处理程序代码
	if(bLoaded == false)
	{
		log("未加载共享库！");
		return;
	}

	if(m_bEtcEventEnvInitedFlag == false)
	{
		log("未初始化ETC特情环境！");
		return;
	}

	//首先暂停其它两个线程的运行
	if(m_bThread1Running)
	{
		log("GUI循环播放1正在运行中，请先停止GUI循环播放1 ！！！");
		return;
	}
	if(m_bThread3Running)
	{
		log("GUI循环播放3正在运行中，请先停止GUI循环播放3 ！！！");
		return;
	}
	if(m_bThreadTotalRunning)
	{
		log("GUI总循环播放正在运行中，请先停止GUI总循环播放 ！！！");
		return;
	}

	if(m_bThread2Running)
	{
		log("GUI循环播放2正在运行中，请不要重复启动 ！！！");
		return;
	}


	log("启动GUI循环播放2.");
	if(m_pGuiPlay2Thread == NULL)
		m_pGuiPlay2Thread = AfxBeginThread(GuiPlay2ThreadFun,(LPVOID)this);
	else
		ResumeThread(m_pGuiPlay2Thread->m_hThread);

	m_bThread2Running = true;
	return;
}

void CTestDevIFDlg::OnBnClickedBtnGuiPlay2Stop()
{
	// TODO: 在此添加控件通知处理程序代码
	if(bLoaded == false)
	{
		log("未加载共享库！");
		return;
	}

	if(m_bEtcEventEnvInitedFlag == false)
	{
		log("未初始化ETC特情环境！");
		return;
	}

	if(m_bThread2Running == false)
	{
		log("停止GUI循环播放2，当前GUI循环播放2未运行！！！");
		return;
	}

	if(m_pGuiPlay2Thread != NULL)
	{
		 //方法1
		 log("停止GUI循环播放2.");
		 DWORD exitCode=12345;
		 TerminateThread(m_pGuiPlay2Thread->m_hThread, exitCode);
		 m_pGuiPlay2Thread = NULL;
	}
	m_bThread2Running = false;

	return;
}

void CTestDevIFDlg::OnBnClickedBtnGuiPlay3Start()
{
	// TODO: 在此添加控件通知处理程序代码
	if(bLoaded == false)
	{
		log("未加载共享库！");
		return;
	}

	if(m_bEtcEventEnvInitedFlag == false)
	{
		log("未初始化ETC特情环境！");
		return;
	}

	//首先暂停其它两个线程的运行
	if(m_bThread1Running)
	{
		log("GUI循环播放1正在运行中，请先停止GUI循环播放1 ！！！");
		return;
	}
	if(m_bThread2Running)
	{
		log("GUI循环播放2正在运行中，请先停止GUI循环播放2 ！！！");
		return;
	}
	if(m_bThreadTotalRunning)
	{
		log("GUI总循环播放正在运行中，请先停止GUI总循环播放 ！！！");
		return;
	}

	if(m_bThread3Running)
	{
		log("GUI循环播放3正在运行中，请不要重复启动 ！！！");
		return;
	}

	log("启动GUI循环播放3.");
	if(m_pGuiPlay3Thread == NULL)
		m_pGuiPlay3Thread = AfxBeginThread(GuiPlay3ThreadFun,(LPVOID)this);
	else
		ResumeThread(m_pGuiPlay3Thread->m_hThread);

	m_bThread3Running = true;

	return;
}

void CTestDevIFDlg::OnBnClickedBtnGuiPlay3Stop()
{
	// TODO: 在此添加控件通知处理程序代码
	if(bLoaded == false)
	{
		log("未加载共享库！");
		return;
	}

	if(m_bEtcEventEnvInitedFlag == false)
	{
		log("未初始化ETC特情环境！");
		return;
	}

	if(m_bThread3Running == false)
	{
		log("停止GUI循环播放3，当前GUI循环播放3未运行！！！");
		return;
	}

	if(m_pGuiPlay3Thread != NULL)
	{
		 log("停止GUI循环播放3.");
		 DWORD exitCode=12345;
		 TerminateThread(m_pGuiPlay3Thread->m_hThread, exitCode);
		 m_pGuiPlay3Thread = NULL;
	}

	m_bThread3Running = false;
	return;
}

void CTestDevIFDlg::OnBnClickedBtnGuiPlayTotalStart()
{
	// TODO: 在此添加控件通知处理程序代码
	if(bLoaded == false)
	{
		log("未加载共享库！");
		return;
	}

	if(m_bEtcEventEnvInitedFlag == false)
	{
		log("未初始化ETC特情环境！");
		return;
	}

	//首先暂停其它两个线程的运行
	if(m_bThread1Running)
	{
		log("GUI循环播放1正在运行中，请先停止GUI循环播放1 ！！！");
		return;
	}
	if(m_bThread2Running)
	{
		log("GUI循环播放2正在运行中，请先停止GUI循环播放2 ！！！");
		return;
	}
	if(m_bThread3Running)
	{
		log("GUI循环播放3正在运行中，请先停止GUI循环播放3 ！！！");
		return;
	}
	if(m_bThreadTotalRunning)
	{
		log("GUI总循环播放正在运行中，请不要重复启动 ！！！");
		return;
	}

	log("启动GUI总循环播放.");
	if(m_pGuiPlayTotalThread == NULL)
		m_pGuiPlayTotalThread = AfxBeginThread(GuiPlayTotalThreadFun,(LPVOID)this);
	else
		ResumeThread(m_pGuiPlayTotalThread->m_hThread);

	m_bThreadTotalRunning = true;

	return;
}

void CTestDevIFDlg::OnBnClickedBtnGuiPlayTotalStop()
{
	// TODO: 在此添加控件通知处理程序代码
	if(bLoaded == false)
	{
		log("未加载共享库！");
		return;
	}

	if(m_bEtcEventEnvInitedFlag == false)
	{
		log("未初始化ETC特情环境！");
		return;
	}

	if(m_bThreadTotalRunning == false)
	{
		log("停止GUI总循环播放，当前GUI总循环播放未运行！！！");
		return;
	}

	if(m_pGuiPlayTotalThread != NULL)
	{
		 log("停止GUI总循环播放.");
		 DWORD exitCode=12345;
		 TerminateThread(m_pGuiPlayTotalThread->m_hThread, exitCode);
		 m_pGuiPlayTotalThread = NULL;
	}

	m_bThreadTotalRunning = false;
	return;
}


void CTestDevIFDlg::OnBnClickedBtnGuiPlayTotalStop2()
{
	// TODO: 在此添加控件通知处理程序代码
	if(bLoaded == false)
	{
		log("未加载共享库！");
		return;
	}

	if(m_bEtcEventEnvInitedFlag == false)
	{
		log("未初始化ETC特情环境！");
		return;
	}

	if(m_bThreadTotalRunning == false)
	{
		log("停止GUI总循环播放，当前GUI总循环播放未运行！！！");
		return;
	}

	if(m_pGuiPlayTotalThread != NULL)
	{
		 log("停止GUI总循环播放.");
		 DWORD exitCode=12345;
		 TerminateThread(m_pGuiPlayTotalThread->m_hThread, exitCode);
		 m_pGuiPlayTotalThread = NULL;
	}

	m_bThreadTotalRunning = false;
	return;
}

void CTestDevIFDlg::OnBnClickedBtnAuthorize()
{
	if(bLoaded == false)
	{
		log("未加载共享库!");
		return;
	}
	if(m_bNetPayEnvInitedFlag == false || m_bEtcEventEnvInitedFlag == false)
	{
		log("未初始化环境!");
		return;
	}

	CString strJson;
	if (((CButton *)GetDlgItem(IDC_RADIO_FreeParam))->GetCheck())
	{
		EditParam.GetWindowText(strJson);
		strJson.TrimLeft();
		strJson.TrimRight();
		strJson.Trim();
		log("车辆信息确认特情(使用自由配置参数):");
		log(strJson.GetBuffer(0));
	}

	if(((CButton *)GetDlgItem(IDC_RADIO_DefaultParam))->GetCheck())
	{
	// 表示整个 json 对象
	Json::Value root;
	// 标识ETC特情交互参数
	Json::Value Authorize;
	Authorize["SpecialEventNo"] = Json::Value(m_EventNo);
	Authorize["MessageCode"] = Json::Value("011");
	Authorize["VehPlate"] = Json::Value(m_CurrentVehPlate);
	Authorize["VehPlateColor"] = Json::Value("1");

	Json::Value Data;
	Data["authorizeType"] = Json::Value(1);	
	Data["authorizeReason"] = Json::Value("在线计费异常");	
	Data["authorizeOption"] = Json::Value("1:补缴放行#2:异议放行#3:拦截车辆");	
	Authorize["Data"] = Data;

	root["Authorize"] = Authorize;

	//转化为字符串
	Json::FastWriter fast_writer;
	std::string str = fast_writer.write(root).c_str();
	log("其他授权:");
	log(str);

	//utf8编码
	//char* strJsonU8 = G2U(str.c_str());
	//int iSize = (int)strlen(strJsonU8);

	strJson.Format(_T("%s"),str.c_str());
	}

	int iSize = strJson.GetLength();

	bool ret = false;
	ret = fn_EventAuthorize(strJson.GetBuffer(0), iSize, 2);
	if(!ret)
	{
		log("其他授权接口返回失败!");
	}

	//delete[] strJsonU8;

	return;
}

void CTestDevIFDlg::OnBnClickedBtnInvoiceDataTrans()
{
	// TODO: 在此添加控件通知处理程序代码
	if(bLoaded == false)
	{
		log("未加载共享库!");
		return;
	}
	if(m_bNetPayEnvInitedFlag == false)
	{
		log("未初始化支付环境！");
		return;
	}

	if(m_bEtcEventEnvInitedFlag == false)
	{
		log("未初始ETC特情环境！");
	}

	CString strJson;
	if (((CButton *)GetDlgItem(IDC_RADIO_FreeParam))->GetCheck())
	{
		EditParam.GetWindowText(strJson);
		strJson.TrimLeft();
		strJson.TrimRight();
		strJson.Trim();
		log("现金发票流水上传(使用自由配置参数):");
		log(strJson.GetBuffer(0));
	}

	if(((CButton *)GetDlgItem(IDC_RADIO_DefaultParam))->GetCheck())
	{
		// 表示整个json对象
		Json::Value root;
		// 标识交互参数
		Json::Value CashInvoiceData;
		// 标识详细数据区
		Json::Value Data;
		// 车辆数据区
		Json::Value Vehicle;
		// 出口数据区
		Json::Value Exit;
		//扣费数据区
		Json::Value Debit;
		//操作数据区
		Json::Value Operation;
		//入口数据区
		Json::Value Entry;

		CString TimeStr = GetUniqueTimeStr();
		std::string timestr(TimeStr.GetBuffer());
		m_CashInvoiceNo = "33b2b009e44b15ffff" + timestr;
		CashInvoiceData["CashInvoiceNo"] = Json::Value(m_CashInvoiceNo);

		Vehicle["VehicleLicense"] = Json::Value("豫A12345");
		//Vehicle["VehicleLicense"] = Json::Value("");
		Vehicle["VLColor"] = Json::Value(2);
		Vehicle["VehicleType"] = Json::Value(1);
		Vehicle["VehicleClass"] = Json::Value(1);
		Vehicle["AxisCount"] = Json::Value(2);
		Vehicle["Weight"] = Json::Value(1234);
		Vehicle["LimitWeight"] = Json::Value(2345);

		Exit["PayAreaID"] = Json::Value("4101");
		Exit["PayProvinceId"] = Json::Value("41");
		Exit["PayRoadID"] = Json::Value("21");
		Exit["PayStationID"] = Json::Value("65001");
		Exit["PayStationName"] = Json::Value("郑州西南站");
		//Exit["PayStationName"] = Json::Value("鹤");
		Exit["PayLaneID"] = Json::Value("133");
		m_ExitTime = CTime::GetCurrentTime().Format("%Y%m%d%H%M%S");
		Exit["ExitTime"] = Json::Value(m_ExitTime);

		Debit["Money"] = Json::Value(3000);
		Debit["PayMode"] = Json::Value(26);
		Debit["SubTime"] = Json::Value(CTime::GetCurrentTime().Format("%Y%m%d%H%M%S"));

		Operation["ShiftID"] = Json::Value(1);
		Operation["OperatorID"] = Json::Value("1357");
		Operation["OperatorName"] = Json::Value("收费员002");
		Operation["PassCertificateType"] = Json::Value(1);
		Operation["PassCertificateCode"] = Json::Value("1234567890");

		Entry["EntryAreaID"] = Json::Value("49");
		Entry["EntryRoadID"] = Json::Value("49");
		Entry["EntryStationID"] = Json::Value("43011288");
		Entry["EntryStationName"] = Json::Value("新郑新区站");
		//Entry["EntryStationName"] = Json::Value("新");
		Entry["EntryLaneID"] = Json::Value("1");
		Entry["Distance"] = Json::Value(0);
		CTime time = CTime::GetCurrentTime() - CTimeSpan(0, 0, 20, 0);
		Entry["EntryTime"] = Json::Value(time.Format("%Y%m%d%H%M%S"));
		Entry["EntryVehicleLicense"] = Json::Value("豫A12345");
		//Entry["EntryVehicleLicense"] = Json::Value("");
		Entry["EntryVLColor"] = Json::Value(2);
		Entry["EntryVehicleClass"] = Json::Value(1);
		Entry["EntryVehicleType"] = Json::Value(1);

		Data["Vehicle"] = Vehicle;
		Data["Exit"] = Exit;
		Data["Debit"] = Debit;
		Data["Operation"] = Operation;
		Data["Entry"] = Entry;
		CashInvoiceData["Data"] = Data;
		root["CashInvoiceData"] = CashInvoiceData;

		//转化为字符串
		Json::FastWriter fast_writer;
		std::string str = fast_writer.write(root).c_str();

		log("现金发票流水上传:");
		log(str);

		strJson.Format(_T("%s"),str.c_str());
	}

	int iSize = strJson.GetLength();
	bool bRet = false;
	bRet = fn_TransferCashInvoiceData(strJson.GetBuffer(0), iSize, 2);
	if(bRet)
	{
		log("调用流水上传JSON接口返回成功!");
	}
	else
	{
		log("调用流水上传JSON接口返回失败!");
	}
	strJson.ReleaseBuffer();
	return;
}

void CTestDevIFDlg::OnBnClickedBtnInvoiceDataCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	if(bLoaded == false)
	{
		log("未加载共享库!");
		return;
	}
	if(m_bNetPayEnvInitedFlag == false)
	{
		log("未初始化支付环境！");
		return;
	}

	if(m_bEtcEventEnvInitedFlag == false)
	{
		log("未初始ETC特情环境！");
	}

	if(m_CashInvoiceNo.length() == 0)
	{
		log("未现金支付，请先上传现金支付流水！");
		return;
	}

	CString strJson;
	if (((CButton *)GetDlgItem(IDC_RADIO_FreeParam))->GetCheck())
	{
		EditParam.GetWindowText(strJson);
		strJson.TrimLeft();
		strJson.TrimRight();
		strJson.Trim();
		log("现金发票流水撤销(使用自由配置参数):");
		log(strJson.GetBuffer(0));
	}
	
	if(((CButton *)GetDlgItem(IDC_RADIO_DefaultParam))->GetCheck())
	{
		// 表示整个json对象
		Json::Value root;
		// 标识交互参数
		Json::Value CancelCashInvoiceData;

		CancelCashInvoiceData["CashInvoiceNo"] = Json::Value(m_CashInvoiceNo);
		CancelCashInvoiceData["PayProvinceId"] = Json::Value("41");
		CancelCashInvoiceData["SubTime"] = Json::Value(m_ExitTime);

		root["CancelCashInvoiceData"] = CancelCashInvoiceData;

		//转化为字符串
		Json::FastWriter fast_writer;
		strJson.Format(_T("%s"),fast_writer.write(root).c_str());
		
		log("现金流水撤销JSON参数:");
		log(strJson.GetBuffer(0));
	}

	bool bRet = false;
	bRet = fn_CancelCashInvoiceData(strJson.GetBuffer(0), strJson.GetLength(), 2);
	if(bRet)
	{
		log("调用流水撤销JSON接口返回成功!");
	}
	else
	{
		log("调用流水撤销JSON接口返回失败!");
	}

	strJson.ReleaseBuffer();
	return;
}

void CTestDevIFDlg::OnBnClickedBtnChange2EtcMode()
{
	// TODO: 在此添加控件通知处理程序代码
	if(bLoaded == false)
	{
		log("未加载共享库!");
		return;
	}
	if(m_bNetPayEnvInitedFlag == false || m_bEtcEventEnvInitedFlag == false)
	{
		log("未初始化环境!");
		return;
	}

	CString strJson;

	// 表示整个 json 对象
	Json::Value root;
	// 标识ETC特情交互参数
	Json::Value CardOperationNotify;
	CardOperationNotify["SpecialEventNo"] = Json::Value(m_EventNo);
	CardOperationNotify["MessageCode"] = Json::Value("009");
	
	Json::Value Data;
	Data["NotifyType"] = Json::Value(99);	
	CardOperationNotify["Data"] = Data;

	root["CardOperationNotify"] = CardOperationNotify;

	//转化为字符串
	Json::FastWriter fast_writer;
	std::string str = fast_writer.write(root).c_str();
	log("切换到ETC自助工作模式:");
	log(str);

	strJson.Format(_T("%s"), str.c_str());
	int iSize = strJson.GetLength();
	
	bool ret = false;
	ret = fn_EventCardOperationNotify(strJson.GetBuffer(0), iSize, 2);
	if(!ret)
	{
		log("调用ETC卡操作提示接口返回失败!");
	}

	return;
}


void CTestDevIFDlg::OnBnClickedBtnChange2MtcMode()
{
	// TODO: 在此添加控件通知处理程序代码
	if(bLoaded == false)
	{
		log("未加载共享库!");
		return;
	}
	if(m_bNetPayEnvInitedFlag == false || m_bEtcEventEnvInitedFlag == false)
	{
		log("未初始化环境!");
		return;
	}

	CString strJson;

	// 表示整个 json 对象
	Json::Value root;
	// 标识ETC特情交互参数
	Json::Value CardOperationNotify;
	CardOperationNotify["SpecialEventNo"] = Json::Value(m_EventNo);
	CardOperationNotify["MessageCode"] = Json::Value("009");
	
	Json::Value Data;
	Data["NotifyType"] = Json::Value(98);
	CardOperationNotify["Data"] = Data;

	root["CardOperationNotify"] = CardOperationNotify;

	//转化为字符串
	Json::FastWriter fast_writer;
	std::string str = fast_writer.write(root).c_str();
	log("切换到MTC人工模式:");
	log(str);

	strJson.Format(_T("%s"), str.c_str());
	int iSize = strJson.GetLength();
	
	bool ret = false;
	ret = fn_EventCardOperationNotify(strJson.GetBuffer(0), iSize, 2);
	if(!ret)
	{
		log("调用ETC卡操作提示接口返回失败!");
	}

	return;
}

void CTestDevIFDlg::OnBnClickedBtnDelVehQueueResult()
{
	// TODO: 在此添加控件通知处理程序代码
	if(bLoaded == false)
	{
		log("未加载共享库!");
		return;
	}
	if(m_bNetPayEnvInitedFlag == false || m_bEtcEventEnvInitedFlag == false)
	{
		log("未初始化环境!");
		return;
	}

	CString strJson;
	if (((CButton *)GetDlgItem(IDC_RADIO_FreeParam))->GetCheck())
	{
		EditParam.GetWindowText(strJson);
		strJson.TrimLeft();
		strJson.TrimRight();
		log("删除车辆队列结果(使用自由配置参数):");
		log(strJson.GetBuffer(0));
	}

	if(((CButton *)GetDlgItem(IDC_RADIO_DefaultParam))->GetCheck())
	{
	// 表示整个 json 对象
	Json::Value root;
	// 标识ETC特情交互参数
	Json::Value DelVehQueueResult;
	DelVehQueueResult["SpecialEventNo"] = Json::Value(m_EventNo);
	DelVehQueueResult["MessageCode"] = Json::Value("011");
	
	Json::Value Data;
	Data["ExecuteType"] = Json::Value(0);	
	Data["DelVehNum"] = Json::Value(1);		
	Data["ExecuteResult"] = Json::Value(0);	

	DelVehQueueResult["Data"] = Data;

	root["DelVehQueueResult"] = DelVehQueueResult;

	//转化为字符串
	Json::FastWriter fast_writer;
	std::string str = fast_writer.write(root).c_str();
	log("删除车辆队列结果:");
	log(str);

	strJson.Format("%s",str.c_str());
	}

	int iSize = strJson.GetLength();	
	bool ret = false;
	ret = fn_EventDelVehQueueResult(strJson.GetBuffer(0), iSize, 2);
	if(!ret)
	{
		log("调用删除车辆队列结果接口返回失败!");
	}

	return;
}

void CTestDevIFDlg::OnBnClickedBtnMtcClearScreen()
{
	// TODO: 在此添加控件通知处理程序代码
	if(bLoaded == false)
	{
		log("未加载共享库!");
		return;
	}
	if(m_bNetPayEnvInitedFlag == false)
	{
		log("未初始化支付环境！");
		return;
	}

	if(m_bEtcEventEnvInitedFlag == false)
	{
		log("未初始ETC特情环境！");
	}

	// 表示整个 json 对象
	Json::Value root;
	// 标识交互参数
	Json::Value MMIParamContext;
	// 标识详细数据项
	Json::Value Data;
	// 标识设置数据项区域
	Json::Value ItemData;

	//标识版本
	Data["ClearMMI"]			= Json::Value(1);
	MMIParamContext["Data"]		= Data;
	MMIParamContext["Version"]	= Json::Value("1.0");
	root["MMIParamContext"]		= MMIParamContext;

	//转化为字符串
	CString strReturn;
	Json::FastWriter fast_writer;
	strReturn.Format(_T("%s"),fast_writer.write(root).c_str());
	bool bRet = false;
	bRet = fn_SetMMI(strReturn.GetBuffer(0), strReturn.GetLength(), 2);
	if(bRet)
	{
		log("调用清屏JSON接口返回成功!");
	}
	else
	{
		log("调用清屏JSON接口返回失败!");
	}

	return;
}

void CTestDevIFDlg::OnBnClickedBtnMtcClearScreenXML()
{
	// TODO: 在此添加控件通知处理程序代码
	if(bLoaded == false)
	{
		log("未加载共享库!");
		return;
	}
	if(m_bNetPayEnvInitedFlag == false)
	{
		log("未初始化支付环境！");
		return;
	}

	if(m_bEtcEventEnvInitedFlag == false)
	{
		log("未初始ETC特情环境！");
	}

	CMarkup xmlMMI;
	xmlMMI.SetDoc( _T("<?xml version=\"1.0\" encoding=\"GB2312\"?>\r\n") );
	xmlMMI.AddElem( _T("MMIParamContext") );//root
	xmlMMI.IntoElem();

	xmlMMI.AddElem( _T("Version"),	_T("1.0"));
	xmlMMI.AddElem( _T("Data") );
	xmlMMI.IntoElem();
	xmlMMI.AddElem( _T("ClearMMI"),	1);
	xmlMMI.OutOfElem();//Data
	xmlMMI.OutOfElem();//MMIParamContext
	CString strDataString = xmlMMI.GetDoc();

	bool bRet = false;
	bRet = fn_SetMMI(strDataString.GetBuffer(0), strDataString.GetLength(), 1);
	if(bRet)
	{
		log("调用清屏XML接口返回成功!");
	}
	else
	{
		log("调用清屏XML接口返回失败!");
	}

	return;
}

void CTestDevIFDlg::OnBnClickedBtnEtcGetLastErrorDescrip()
{
	// TODO: 在此添加控件通知处理程序代码
	if(bLoaded == false)
	{
		log("未加载共享库!");
		return;
	}
	if(m_bNetPayEnvInitedFlag == false)
	{
		log("未初始化移动支付环境!");
		return;
	}

	log("调用接口IF_EventGetLastErrorDesc()");
	static std::string ErrorDesc;
	ErrorDesc = fn_EventGetLastErrorDesc();
	log("返回上次错误信息:" + ErrorDesc);

	return;
}
