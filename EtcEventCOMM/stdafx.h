// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// �� Windows ͷ���ų�����ʹ�õ�����
#endif

// ������뽫λ������ָ��ƽ̨֮ǰ��ƽ̨��ΪĿ�꣬���޸����ж��塣
// �йز�ͬƽ̨��Ӧֵ��������Ϣ����ο� MSDN��
#ifndef WINVER				// ����ʹ���ض��� Windows XP ����߰汾�Ĺ��ܡ�
#define WINVER 0x0501		// ����ֵ����Ϊ��Ӧ��ֵ���������� Windows �������汾��
#endif

#ifndef _WIN32_WINNT		// ����ʹ���ض��� Windows XP ����߰汾�Ĺ��ܡ�
#define _WIN32_WINNT 0x0501	// ����ֵ����Ϊ��Ӧ��ֵ���������� Windows �������汾��
#endif						

#ifndef _WIN32_WINDOWS		// ����ʹ���ض��� Windows 98 ����߰汾�Ĺ��ܡ�
#define _WIN32_WINDOWS 0x0410 // ����ֵ����Ϊ�ʵ���ֵ����ָ���� Windows Me ����߰汾��ΪĿ�ꡣ
#endif

#ifndef _WIN32_IE			// ����ʹ���ض��� IE 6.0 ����߰汾�Ĺ��ܡ�
#define _WIN32_IE 0x0600	// ����ֵ����Ϊ��Ӧ��ֵ���������� IE �������汾��
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// ĳЩ CString ���캯��������ʽ��

#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE ��
#include <afxodlgs.h>       // MFC OLE �Ի�����
#include <afxdisp.h>        // MFC �Զ�����
#endif // _AFX_NO_OLE_SUPPORT

#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>			// MFC ODBC ���ݿ���
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>			// MFC DAO ���ݿ���
#endif // _AFX_NO_DAO_SUPPORT

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>		// MFC �� Internet Explorer 4 �����ؼ���֧��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT


/*******************************************************��Ϣ����********************************************************************/

//��������߳���Ϣ����ʼ����
#define	MSG_START_ID_SQL		0		//SQL�̴߳�����Ϣ��ʼ������
#define	MSG_START_ID_SM			100		//ɨ���̴߳�����Ϣ��ʼ������
#define	MSG_START_ID_IC			200		//��д���̴߳�����Ϣ��ʼ������
#define	MSG_START_ID_EQUIPEMENT	300		//IO�����̴߳�����Ϣ��ʼ������
#define MSG_START_ID_HTTP		400		//HttpͨѶ�̴߳�����Ϣ��ʼ������
#define MSG_START_ID_PLATE		500		//����ʶ�����߳���Ϣ��ʼ������
#define MSG_START_ID_VDM		600		//VDM�����߳���Ϣ��ʼ������
#define MSG_START_ID_TFI		700		//���Դ����߳���Ϣ��ʼ������
#define MSG_START_ID_REMOTE		800		//Զ�̵�ͼ����Ϣ��ʼ������
#define MSG_START_ID_AXIS		900		//�����߳���Ϣ��ʼ������
#define MSG_START_ID_SOCKET		1000	//SocketͨѶ�����Ϣ��ʼ����
#define MSG_START_ID_DM			1100	//�������ݹ����߳������Ϣ��ʼ����
#define MSG_START_ID_MAIN_UI	1200	//�����������Ϣ��ʼ������


//Http�߳��ദ�����Ϣ
#define WM_THREAD_HTTP_SEND_DATA_REQUEST			WM_APP + MSG_START_ID_HTTP + 1	//���Ͳ�ѯ������Ϣ����
#define WM_WND_DEAL_PLATFORM_RESPONSE_RESULT		WM_APP + MSG_START_ID_HTTP + 2	//��������Httpƽ̨������������
#define WM_THREAD_HTTP_RESEND_DATA_REQUEST			WM_APP + MSG_START_ID_HTTP + 3	//�ط�����


//��Ӫƽ̨ͨѶ�ദ�����Ϣ
#define	WM_WND_DEAL_RESPONSE_FOR_ENTRY_QUERY		WM_APP + MSG_START_ID_HTTP + 3	//��HTTP�߳̽��յ�����ڲ�ѯ���Ļ������
#define	WM_WND_DEAL_RESPONSE_FOR_EXIT_QUERY			WM_APP + MSG_START_ID_HTTP + 4	//��HTTP�߳̽��յ������ڲ�ѯ���Ļ������
#define WM_WND_DEAL_RESPONSE_FOR_OPEN_LOOP			WM_APP + MSG_START_ID_HTTP + 5	//��HTTP�߳̽��յ�����ڿ����������Ļ������
#define WM_WND_DEAL_RESPONSE_FOR_CLOSE_LOOP			WM_APP + MSG_START_ID_HTTP + 6	//��HTTP�߳̽��յ������ڱջ��������Ļ������
#define WM_WND_DEAL_RESPONSE_FOR_DEBIT				WM_APP + MSG_START_ID_HTTP + 7	//��HTTP�߳̽��յ����۷Ѳ������Ļ������
#define WM_WND_DEAL_RESPONSE_FOR_DEBIT_CHECK		WM_APP + MSG_START_ID_HTTP + 8	//��HTTP�߳̽��յ����۷�״̬��ѯ���Ļ������
#define WM_WND_DEAL_RESPONSE_FOR_PASS_OPEN_STATION	WM_APP + MSG_START_ID_HTTP + 9	//��HTTP�߳̽��յ���������ʽ�շ�վ���Ļ������


//ɨ�������������Ϣ
#define WM_THREAD_SM_INIT_WORK							WM_APP + MSG_START_ID_SM + 25	//��ʼ�����еĹ����߳�

#define WM_THREAD_SM_GET_SCAN_CODE_DATA					WM_APP + MSG_START_ID_SM + 1	//��ȡ��ɨ����
#define WM_THREAD_SM_GET_SYSTEM_INFO					WM_APP + MSG_START_ID_SM + 2	//��ȡɨ����ϵͳ��Ϣ
#define WM_THREAD_SM_START_SCAN							WM_APP + MSG_START_ID_SM + 3	//ǿ�ƿ�ʼɨ��
#define WM_THREAD_SM_STOP_SCAN							WM_APP + MSG_START_ID_SM + 4	//��ʼ�����еĹ����߳�
#define WM_THREAD_SM_RESET_MACHINE						WM_APP + MSG_START_ID_SM + 5	//��ʼ��ָ��ɨ����
#define WM_THREAD_SM_DEAL_INNER_DEVICE_RESPONSE_INFO	WM_APP + MSG_START_ID_SM + 6	//Э������෢�͸������̹߳���ɨ��״̬��ʾ��Ϣ
#define WM_THREAD_SM_CONTROL_BOARD						WM_APP + MSG_START_ID_SM + 7	//���ƴ��ڰ�
#define WM_THREAD_SM_STATE								WM_APP + MSG_START_ID_SM + 8	//�����̷߳��͸�UI����ɨ������״̬��ʾ��Ϣ
#define WM_THREAD_SM_SET_LIGHT_MODE						WM_APP + MSG_START_ID_SM + 9	//���Ƶƹ����

#define WM_WND_NET_SM_DEBIT_TOLL						WM_APP + MSG_START_ID_SM + 10	//�����ۿ�
#define WM_WND_CHECK_DEBIT_PASSWORD_STATE				WM_APP + MSG_START_ID_SM + 11	//�ۿ�����ѯ

//������������ʾ��Ϣ
#define WM_THREAD_SM_SET_DISPLAY						WM_APP + MSG_START_ID_SM + 12	//������ʾ����
#define WM_THREAD_SM_SET_TRANSMISSION					WM_APP + MSG_START_ID_SM + 13	//͸��
#define WM_THREAD_SM_TRANSMISSION_RESPONSE				WM_APP + MSG_START_ID_SM + 14	//͸������

#define WM_THREAD_SM_SET_ZHEJIANG_TEXT					WM_APP + MSG_START_ID_SM + 20	//�����㽭�ı�
#define WM_THREAD_SM_SET_ZHEJIANG_SOUND					WM_APP + MSG_START_ID_SM + 21	//�����㽭����

#define WM_THREAD_SM_NOTIFY_CALLER						WM_APP + MSG_START_ID_SM + 22	//�ص�����֪ͨ���������



#define FILE_SIZE_LOG_MAX				50*1024*1024			//��־�ļ����ֵ
#define DIR_NAME_DEVICE_SCAN			_T("ScanMachine")		//ɨ����

#define FILE_NAME_DLL_SCAN				_T("HTSM.dll")			//ɨ�������ƶ�̬��
#define FILE_NAME_HTTP_CONFIG			_T("ReflectWeb.ini")	//���������������Ϣ�ļ���

#define FILE_NAME_DEVICE_SM_CONFIG		_T("HTSMConfig.ini")		//Ӳ��������Ϣ�ļ���--ɨ����
#define FILE_NAME_DEVICE_SM_CONFIG1		_T("HTSMConfig1.ini")		//Ӳ��������Ϣ�ļ���--ɨ����


#pragma pack(1)



//����������Ϣ�ṹ��
typedef struct _tagLaneConfigInfo
{
	int		MainType;		//���ʽ����ڡ�����ʽ
	int		SubType;		//ETC,MTC,�Զ�����,��Я�շѻ�
	int		StationID;		//վ�����
	int		LaneID;			//��������
	int		LaneType;		//��������(1,���;2,����)
	TCHAR	LaneTab[16];	//����Ψһ����
	TCHAR	StationTab[32]; //վ��Ψһ����
	int		STServerTpye;	//վ������������
	TCHAR	STServerIP[24];	//վ��������IP��ַ
	TCHAR	STServerID[24];	//վ��������ID����
	TCHAR	WebServerIP[24];//�������������IP��ַ
}tagLaneConfigInfo;

//����APP��������߳��ഫ�ݻ���������Ϣ�Ľṹ��
typedef struct _tagTranstParam
{
	//ʡ�ݱ���
	int					ProvinceID;
	//�������û�����Ϣ
	tagLaneConfigInfo	LaneConfigInfo;
	//��������������ϢID
	DWORD				ActiveNotifyMsgID;
	/******************����·������******************/

	//������ʼĿ¼����·��,Ĭ��Ϊ���г���ĵ�ǰĿ¼
	TCHAR	PathCommDir[MAX_PATH];
	//�������̲�������ʱ�ؼ��ļ��洢Ŀ¼����·��
	TCHAR	PathDataDir[MAX_PATH];
	//����Ӳ������Ŀ¼����·��
	TCHAR	PathDeviceDir[MAX_PATH];
	//������־Ŀ¼����·��
	TCHAR	PathLogDir[MAX_PATH];
	//�������ݱ���Ŀ¼����·��
	TCHAR	PathDataBackupDir[MAX_PATH];
	//��ʱ�ļ��洢Ŀ¼����·��
	TCHAR	PathTempDir[MAX_PATH];
	//�������ز���Ŀ¼����·��
	TCHAR	PathDownloadDir[MAX_PATH];

	//�������ݶ���Ŀ¼�������ţ�
	TCHAR	PathDataSecondDirForLaneID[MAX_PATH];
	//�������ݶ���Ŀ¼�����ݣ�
	TCHAR	PathDataSecondDirForCore[MAX_PATH];
	//�������ݶ���Ŀ¼��������
	TCHAR	PathDataSecondDirForParam[MAX_PATH];
	//�������ݶ���Ŀ¼��ͼ��
	TCHAR   PathDataSecondDirForImage[MAX_PATH];
}tagTranstParam;


//���ڲ�����Ϣ�ṹ��
typedef struct _tagCOMParam
{
	int		Port;					//����
	int		BaudRate;				//������
	int		ParityBit;				//У��λ
	int		StopBit;				//ֹͣλ
	int		DataBit;				//����λ

	DWORD	InBufferSize;			//���뻺������С
	DWORD	OutBufferSize;			//�����������С
}tagCOMParam;


#ifdef _DEBUG
#pragma comment(lib,".\\lib\\json_vc71_libmtd.lib")
#else
#pragma comment(lib,".\\lib\\json_vc71_libmt.lib")
#endif

#pragma pack()