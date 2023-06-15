// TestDevIFDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include <string>

#include "ThreadReceiveRespMsg.h"

//#define WM_THREAD_ETCEVENT_SHOWLOG  WM_APP + 8

	
//��ʼ���������
typedef bool (WINAPI *IF_EventInitEnvironment3)(IN const UINT& nThreadID, IN const HWND& hWnd, IN const unsigned int& nNotifyMsgID, IN const char* szAreaInfo, IN const char* szLoaclStation, IN const char* szLoaclLaneID, IN const char* szServerInfo, IN const int& iProvinceID, IN OUT void* pIndPtr, void (*fun)(int option, int result,void* pIndPtr));
//������Դ
typedef bool (WINAPI *IF_EventDestroy)(void);
//��ȡ��һ�δ�����Ϣ
typedef const char* (WINAPI *IF_EventGetLastErrorDesc)(void);
//��ȡ���״̬
typedef bool (WINAPI *IF_EventGetComponentStatus)(IN OUT unsigned int& uStatus);
//���鴦��ʼ
typedef bool (WINAPI *IF_EventDealStart)(IN const char* szParamContext, IN const int& iParamSize, IN const int& iFormatType);
//��������ȷ��
typedef bool (WINAPI *IF_EventCheckVehQueue)(IN const char* szParamContext, IN const int& iParamSize, IN const int& iFormatType);
//������Ϣȷ��
typedef bool (WINAPI *IF_EventCheckVehInfo)(IN const char* szParamContext,IN const int& iParamSize,IN const int& iFormatType);
//�����Ϣȷ��
typedef bool (WINAPI *IF_EventCheckEntryInfo)(IN const char* szParamContext,IN const int& iParamSize,IN const int& iFormatType);
//�Ѷ���ʾ
typedef bool (WINAPI *IF_EventShowFeeInfo)(IN const char* szParamContext,IN const int& iParamSize,IN const int& iFormatType);
//֧��״̬��ʾ
typedef bool (WINAPI *IF_EventPayResultDisplay)(IN const char* szParamContext,IN const int& iParamSize,IN const int& iFormatType);
//���鴦�����
typedef bool (WINAPI *IF_EventDealStop)(IN const char* szParamContext,IN const int& iParamSize,IN const int& iFormatType);
//ETC��������ʾ
typedef bool (WINAPI *IF_EventCardOperationNotify)(IN const char* szParamContext,IN const int& iParamSize,IN const int& iFormatType);
//�޸ĳ�������
typedef bool (WINAPI *IF_EventModifyVehQueue)(IN const char* szParamContext,IN const int& iParamSize,IN const int& iFormatType);
//����ɨ��
typedef bool (WINAPI *IF_EventStartScan)(IN const int& iFeeValue);
//ֹͣɨ��
typedef bool (WINAPI *IF_EventStopScan)();
//��С�Ѷ���Ȩ
typedef bool (WINAPI *IF_EventFeeAuthorize)(IN const char* szParamContext,IN const int& iParamSize,IN const int& iFormatType);
//������Ȩ
typedef bool (WINAPI *IF_EventAuthorize)(IN const char* szParamContext,IN const int& iParamSize,IN const int& iFormatType);
//ɾ�����н��
typedef bool (WINAPI *IF_EventDelVehQueueResult)(IN const char* szParamContext,IN const int& iParamSize,IN const int& iFormatType);


//��ʼ���������			-- �ӿ�1
typedef bool  (WINAPI  *defIF_InitEnvironment)(IN const UINT& nThreadID,IN const HWND& hWnd,IN const unsigned int& nNotifyMsgID,IN const char* szAreaInfo,IN const char* szLoaclStationID,IN const char* szLoaclLaneID,IN const char* szServerInfo,IN const int& iProvinceID);
//ִ�пۿ����			-- �ӿ�2
typedef bool  (WINAPI  *defIF_DebitMoney)(IN const char* szParamContext,IN const int& iParamSize,IN const int& iFormatType);
//ִ�г�������			-- �ӿ�3
typedef bool  (WINAPI  *defIF_DebitCancel)(IN const char* szParamContext,IN const int& iParamSize,IN const int& iFormatType);
//��ȡ�ƶ�֧�����ϴδ�����Ϣ
typedef const char* (WINAPI  *defIF_GetLastErrorDesc)(void);
//ж��֧������
typedef bool (WINAPI *defIF_Destroy)(void);
//��ȡ�ƶ�֧���豸״̬
typedef bool (WINAPI *defIF_GetComponentStatus)(IN OUT unsigned int& uStatus);

typedef bool  (WINAPI  *defIF_TransferCashInvoiceData)(IN const char* szParamContext, IN const int& iParamSize, IN const int& iFormatType);
typedef bool  (WINAPI  *defIF_CancelCashInvoiceData)(IN const char* szParamContext, IN const int& iParamSize, IN const int& iFormatType);
//����
typedef bool (WINAPI *defIF_SetMMI)(IN const char* szParamContext, IN const int& iParamSize, IN const int& iFormatType);


//�ص���������
typedef void (*fun)(int iOption, int iResult, void* pIndPtr);


// CTestDevIFDlg dialog
class CTestDevIFDlg : public CDialog
{
// Construction
public:
	CTestDevIFDlg(CWnd* pParent = NULL);	// standard constructor


// Dialog Data
	enum { IDD = IDD_TESTDEVIF_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CEdit edt_info;
	void log(std::string str);
private:
	void load_js();
private:
	HMODULE gd_dll;
	bool bLoaded;

	IF_EventInitEnvironment3		fn_EventInitEnvironment3;
	IF_EventDestroy					fn_EventDestroy;
	IF_EventGetLastErrorDesc		fn_EventGetLastErrorDesc;
	IF_EventGetComponentStatus		fn_EventGetComponentStatus;
	IF_EventDealStart				fn_EventDealStart;
	IF_EventCheckVehQueue			fn_EventCheckVehQueue;
	IF_EventCheckVehInfo			fn_EventCheckVehInfo;
	IF_EventCheckEntryInfo			fn_EventCheckEntryInfo;
	IF_EventShowFeeInfo				fn_EventShowFeeInfo;
	IF_EventPayResultDisplay		fn_EventPayResultDisplay;
	IF_EventDealStop				fn_EventDealStop;
	IF_EventCardOperationNotify		fn_EventCardOperationNotify;
	IF_EventModifyVehQueue			fn_EventModifyVehQueue;
	IF_EventStartScan				fn_EventStartScan;
	IF_EventStopScan				fn_EventStopScan;
	IF_EventFeeAuthorize			fn_EventFeeAuthorize;
	IF_EventAuthorize				fn_EventAuthorize;
	IF_EventDelVehQueueResult		fn_EventDelVehQueueResult;

	defIF_InitEnvironment			fn_InitEnvironment;
	defIF_DebitMoney				fn_DebitMoney;
	defIF_DebitCancel				fn_DebitCancel;
	defIF_Destroy					fn_Destroy;
	defIF_GetLastErrorDesc			fn_GetLastErrorDesc;
	defIF_GetComponentStatus        fn_GetComponentStatus;
	defIF_TransferCashInvoiceData	fn_TransferCashInvoiceData;
	defIF_CancelCashInvoiceData		fn_CancelCashInvoiceData;
	defIF_SetMMI					fn_SetMMI;

public:
	afx_msg void OnBnClickedBtnLoadDll();
	afx_msg void OnBnClickedBtnInitEtcEventEnv();
	afx_msg void OnBnClickedBtnEventstart();
	afx_msg void OnBnClickedBtnCheckvehqueue();
	afx_msg void OnBnClickedBtnCheckvehinfo();
	afx_msg void OnBnClickedBtnShowfeeinfo();
	afx_msg void OnBnClickedBtnCheckentryinfo();
	afx_msg void OnBnClickedBtnCheckentryinfoNone();
	afx_msg void OnBnClickedBtnPayresultshow();
	afx_msg void OnBnClickedBtnEventstop();
	afx_msg void OnBnClickedBtnStartscan();
	afx_msg void OnBnClickedBtnStopscan();
	afx_msg void OnBnClickedBtnShowfeeinfosingle();
	afx_msg void OnBnClickedBtnPayresultshowfail();

	static int No;
	std::string m_EventNo;
	static int VehPlateTail;

	static void* m_pIndPtr; 
	afx_msg void OnBnClickedBtnCheckvehqueuedel();

	afx_msg LRESULT OnMsgShowEtcEventLog(WPARAM wParam, LPARAM lParam);  //�ص���������
	afx_msg LRESULT OnHWNDMsgShowEtcEventLog(WPARAM wParam, LPARAM lParam); //����յ���Ϣ�����
	afx_msg LRESULT OnHWNDMsgShowNetPayResultLog(WPARAM wParam,LPARAM lParam); //����յ���Ϣ�����


	CString MyGetCurrenTime();
	CString GetUniqueTimeStr();
	afx_msg void OnBnClickedBtnDestroy();
	afx_msg void OnBnClickedBtnGetcomponentstatus();
	afx_msg void OnBnClickedBtnGetlasterror();
	afx_msg void OnBnClickedBtnGetlasterror2();

	std::string m_CashInvoiceNo;
	CString m_ExitTime;

	/*********��ǰ���鳵����Ϣ********************/
	CString m_CurrentVehPlate;
	int m_CurrentVehPlateColor;
	int m_CurrentVehClass;
	int m_CurrentVehType;
	int m_CurrentAxisCount;
	CString m_CurrentEntryTime;
	/********************************************/
	bool ParseVehInfoCheckResp(char *pBuffer);

	CString GetCurrentDirNew();
	
	//����ETC����������
	int m_EventTypeNo;
	int m_EventHandleMethodNo;
	CString GetEventTypeName(int EventTypeNo);
	CString GetEventHandleMethodName(int EventHandleMethodNo);
	afx_msg void OnBnClickedBtnSwipecard();
	afx_msg void OnBnClickedBtnRwcard();
	afx_msg void OnBnClickedBtnFixvehqueue();

/**************************�ƶ�֧����Ϣ**********************************************/

	//TWSDNetPayConfig.ini �����ļ�����
	//�ڲ�����IP
	CString m_strServerIP;
	//�ڲ����Ӷ˿�
	WORD	m_wServerPort;

	//TWNetPayTest.ini �����ļ�����
	//ʡ�ݱ���
	int		m_iProvinceID;
	CString m_strProvinceID;
    //ʡ������
	CString m_strStationName;
	//�������
	int		m_iAreaID;
	CString m_strAreaID;
	//·�α���
	int		m_iRoadID;
	CString m_strRoadID;
	//վ�����
	int		m_iStationID;
	CString m_strStationID;
	//��������
	int		m_iLaneID;
	CString m_strLaneID;

	//UI�ۿ���Ϣ
	int m_iDebitMoney;
	int m_iAxisNum;
	int m_iWeight;
	int m_iTimeOut;
	int m_iBalance;
	int m_iVType;
	int m_iVClass;
	int m_iVLColor;

	CString m_strPlate;
	CString m_strEntry;
    CString m_strOperator;
	CString m_strOperatotID;	
	CString m_strCardNo; 

	CString m_strLastPayIdentifier;

	void GetUIDebitInfo();
	const CString guid(IN const int iType=1);
	BOOL FillpPayKeyItemsStruct(IN  int _OperationMode, IN int _PayCertificateType, IN CString _PayCertificateCode, OUT tagPayKeyItems& _ppPayKeyItems);

	bool m_bNetPayEnvInitedFlag;
	bool m_bEtcEventEnvInitedFlag;
	afx_msg void OnBnClickedBtnInitNetPayEnv();
	afx_msg void OnBnClickedBtnDebit();
	afx_msg void OnBnClickedBtnCancel();

	UINT ReceiveMsgThreadID;
	static CThreadReceiveRespMsg *pThreadReceiveMsg;
	afx_msg void OnBnClickedBtnFeeAuthorize();
	CEdit EditParam;
	afx_msg void OnBnClickedBtnDestroyEtcEvent();
	afx_msg void OnBnClickedBtnDestroyNetPay();
	afx_msg void OnBnClickedBtnGetLastErrorDesc();
	afx_msg void OnBnClickedBtnFreeDll();
	afx_msg void OnBnClickedBtnDebitXML();
	afx_msg void OnBnClickedBtnCancelXML();
	afx_msg void OnBnClickedBtnEtcdevstatus();
	afx_msg void OnBnClickedBtnNetpaydevstatus();
	afx_msg void OnBnClickedBtnGuiPlay1Start();
	afx_msg void OnBnClickedBtnGuiPlay1Stop();
	afx_msg void OnBnClickedBtnGuiPlay2Start();
	afx_msg void OnBnClickedBtnGuiPlay2Stop();
	afx_msg void OnBnClickedBtnGuiPlay3Start();
	afx_msg void OnBnClickedBtnGuiPlay3Stop();

	CWinThread* m_pGuiPlay1Thread;
	CWinThread* m_pGuiPlay2Thread;
	CWinThread* m_pGuiPlay3Thread;
	CWinThread* m_pGuiPlayTotalThread;
	bool m_bThread1Running;
	bool m_bThread2Running;
	bool m_bThread3Running;
	bool m_bThreadTotalRunning;
	static UINT GuiPlay1ThreadFun(LPVOID pParam);
	static UINT GuiPlay2ThreadFun(LPVOID pParam);
	static UINT GuiPlay3ThreadFun(LPVOID pParam);
	static UINT GuiPlayTotalThreadFun(LPVOID pParam);
	void GuiPlay1Eventstart();
	void GuiPlay2Eventstart();
	void GuiPlay3Eventstart();
	void GuiPlayXSwipeCardNotify();
	void GuiPlayXShowFeeInfo();
	void GuiPlay13PayResultShow();
	void GuiPlay2PayResultShow();
	void GuiPlay3CheckEntryInfoNone();
	void GuiPlayXEventstop();
	afx_msg void OnBnClickedBtnGuiPlayTotalStart();
	afx_msg void OnBnClickedBtnGuiPlayTotalStop();
	afx_msg void OnBnClickedBtnGuiPlayTotalStop2();
	afx_msg void OnBnClickedBtnAuthorize();
	afx_msg void OnBnClickedBtnInvoiceDataTrans();
	afx_msg void OnBnClickedBtnInvoiceDataCancel();
	afx_msg void OnBnClickedBtnChange2EtcMode();
	afx_msg void OnBnClickedBtnChange2MtcMode();
	afx_msg void OnBnClickedBtnDelVehQueueResult();
	afx_msg void OnBnClickedBtnMtcClearScreen();
	afx_msg void OnBnClickedBtnMtcClearScreenXML();
	afx_msg void OnBnClickedBtnEtcGetLastErrorDescrip();
};

void MyCallBackFun(int iOption, int iResult, void* pIndPtr);