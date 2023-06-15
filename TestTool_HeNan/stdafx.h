// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#endif

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER				// Allow use of features specific to Windows XP or later.
#define WINVER 0x0501		// Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif						

#ifndef _WIN32_WINDOWS		// Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE			// Allow use of features specific to IE 6.0 or later.
#define _WIN32_IE 0x0600	// Change this to the appropriate value to target other versions of IE.
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit

// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions


#include <afxdisp.h>        // MFC Automation classes



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

/*
#include "json\json.h"
#ifdef _DEBUG
#pragma comment(lib,".\\lib\\json_vc71_libmtd.lib")
#else
#pragma comment(lib,".\\lib\\json_vc71_libmt.lib")
#endif
*/

#include "json\json.h"
#ifdef _DEBUG
#pragma  comment(lib, "json_vc71_libmtd.lib")  
#else
#pragma  comment(lib, "json_vc71_libmt.lib")  
#endif

#define WM_THREAD_NETPAY_NOTIFY					WM_APP + 1
#define WM_THREAD_NETPAY_START_SCAN_CODE		WM_APP + 2
#define WM_THREAD_NETPAY_STOP_SCAN_CODE			WM_APP + 3
#define WM_THREAD_NETPAY_SHOW_INFO				WM_APP + 4
#define WM_THREAD_NETPAY_DEBIT_MONEY			WM_APP + 5
#define WM_THREAD_NETPAY_DEBIT_CANCEL			WM_APP + 6

#define WM_THREAD_NETPAY_INITENV      			WM_APP + 7
#define WM_THREAD_ETCEVENT_CALLBACK_SHOWLOG		WM_APP + 8  //����Ϣʹ��EtcEvent�Ļص�����
#define WM_HWND_NETPAY_RESULT_SHOWLOG			WM_APP + 9
#define WM_HWND_ETCEVENT_MESSAGE_SHOWLOG		WM_APP + 10  //����Ϣ�Ǿ�����յ���Ϣ֪ͨ

#define WM_THREAD_ETCEVENT_MESSAGE_SHOWLOG		WM_APP + 11  //����Ϣ���߳̽��յ���Ϣ֪ͨ
#define WM_THREAD_NETPAY_MESSAGE_SHOWLOG		WM_APP + 12  //����Ϣ���߳̽��յ���Ϣ֪ͨ





//�����豸����״̬����
enum DEVICE_STATE_DESC
{
	DEVICE_STATE_LOAD			= 0x0001,	//��������ʧ��
	DEVICE_STATE_SERIAL_OPEN	= 0x0002,	//�豸�����޷���
	DEVICE_STATE_SERIAL_COMM	= 0x0004,	//�豸����ͨѶ����
	DEVICE_STATE_PAY_WEB		= 0x0008,	//֧����ƽ̨�������
	DEVICE_STATE_PAY_SERVER		= 0x0010,	//֧����ƽ̨����������
	DEVICE_STATE_NFC            = 0x0020,    //NFC���� add 2017-08-01
	DEVICE_STATE_4G				= 0x0040,	//�ƶ�ͨѶ�豸����

	//DEVICE_STATE_NFC            = 0x0040,    //NFC���� add 2017-08-01
	//DEVICE_NOT_CONNECTED        = 0x0080
};

//JSON���ݸ�ʽ����������
enum JSON_DATA_TYPE
{
	JSON_DT_PARAM_ACCOUNT		 = 0xFF01,	//�û�����JSON��ʽ
	JSON_DT_PARAM_DEBIT_MONEY	 = 0xFF02,	//�ۿ����JSON��ʽ
	JSON_DT_PARAM_DEBIT_CANCLE	 = 0xFF03,	//��������JSON��ʽ
	JSON_DT_PARAM_DEBIT_RESULT	 = 0xFF04,	//�ۿ�������JSON��ʽ
	JSON_DT_PARAM_MMI			 = 0xFF05,	//�����������JSON��ʽ
};

//���׹ؼ���Ϣ
typedef struct _tagPayKeyItems
{
	//��ʱ�ȴ�ʱ��
	int		OverTime;
	//�ۿ�ģʽ
	int		OperationMode;

	/*****************Debit��ϸ��************************/
	//����ʶ����
	CString	PayIdentifier;
	//���׽��(��λ����)
	int		Money;
	//����ʱ��
	CTime	SubTime;
	//����������
	int		DebitMainType;
	//������֧��ƾ֤
	CString PayCertificateCode;
	//֧��ƾ֤����
	int		PayCertificateType;
	//���
	int		Balance;

	//add 2018-03
	CString AutoLicense;
	int AutoColor;

	/*****************Vehicle��ϸ��************************/
	//���� 0x01 -- 0xFF
	int		VehicleType;
	//���� 0�� 1��
	int		VehicleClass;
	//������ɫ
	int		VLColor;
	//����
	CString	VehicleLicense;
	//����
	int		AxisCount;
	//����
	int		TotalWeight;
	//����
	int		LimitWeight;

	/*****************Operation��ϸ��************************/
	//ͨ��ƾ֤����
	int		PassCertificateType;
	//ͨ��ƾ֤����
	CString CardID;
	//ͳ����
	int		TollDate;
	//ͳ�ư��
	int		ShiftID;
	//��ӡƱ�ݺ�
	CString TicketNo;
	//add 2018-03
	int Distance;

	/*****************Entry��ϸ��************************/
	//���վ��
	int		EntryStationID;
	//���վ��
	CString EntryStationName;
	//���ʱ��
	CTime	EntryTime;
	//��ڳ���
	int		EntryLane;
	//��ڳ���
	CString	EntryVehicleLicense;
	//��ڳ�����ɫ
	int		EntryVLColor;
	//��ڳ���
	int		EntryVehicleType;
	//��ڳ���
	int		EntryVehicleClass;
	//����������
	int		EntryAreaID;
	//���·�κ�
	int		EntryRoadID;

	//�շ�Ա����
	CString OperatorName;
	//�շ�ԱId
	CString OperatorID;
}tagPayKeyItems;

//MMI���������ֲ����ṹ��Ϣ
typedef struct _tagMMIItemText
{
	int	Type;
	int Sub;
	int Row;
	BOOL  IsScroll;
	TCHAR Context[48];
}tagMMIItemText;

//MMI���������������ṹ��Ϣ
typedef struct _tagMMIItemVoice
{
	int	Type;
	int No;
}tagMMIItemVoice;

//MMI������ͼ������ṹ��Ϣ
typedef struct _tagMMIItemGraph
{
	int	Type;
	int No;
}tagMMIItemGraph;


typedef struct _tagETCMMIInfo
{
	int	ETCMMIType;
	CString ETCMMIData;
}tagETCMMIInfo;







#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


