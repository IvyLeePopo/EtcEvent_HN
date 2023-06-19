// ScanMachine.h : ScanMachine DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "ThreadSMManager.h"


// CScanMachineApp
// �йش���ʵ�ֵ���Ϣ������� ScanMachine.cpp
//


class CScanMachineApp : public CWinApp
{
public:
	CScanMachineApp();

// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

public:
	/*****************************ETC����ר�ýӿ�**********************************************************/
	//��ʼ���豸
	static bool WINAPI  IF_InitDev(IN const UINT& dwThreadNotifyCallerID, IN const UINT& dwThreadManagerID, IN const HWND& hWnd, IN const UINT& dwEtcRespMsgID, IN const UINT& dwDevNotifyMsgID, IN OUT void* pIndPtr);
	//������Դ
	static bool WINAPI  IF_DeInitDev(void);
	//��ȡ��һ�δ�����Ϣ
	static const char* WINAPI IF_GetLastErrorDesc(void);
	//��ȡ���״̬
	static bool WINAPI IF_GetDevStatus(IN OUT unsigned int& uStatus);
	//�����豸��ʼ������
	static bool WINAPI IF_SetDevInitData(IN const char* szParamContext, IN const int& iParamSize);
	//���鴦��ʼ
	static bool WINAPI IF_EtcEventDealStart(IN const char* szParamContext, IN const int& iParamSize);
	//��������ȷ��
	static bool WINAPI IF_CheckVehQueue(IN const char* szParamContext, IN const int& iParamSize);
	//������Ϣȷ��
	static bool WINAPI IF_CheckVehInfo(IN const char* szParamContext,IN const int& iParamSize);
	//�����Ϣȷ��
	static bool WINAPI IF_CheckEntryInfo(IN const char* szParamContext,IN const int& iParamSize);
	//�Ѷ���ʾ
	static bool WINAPI IF_ShowFeeInfo(IN const char* szParamContext,IN const int& iParamSize);
	//֧��״̬��ʾ
	static bool WINAPI IF_PayResultDisplay(IN const char* szParamContext,IN const int& iParamSize);
	//���鴦�����
	static bool WINAPI IF_EtcEventDealStop(IN const char* szParamContext,IN const int& iParamSize);
	//����ɨ��
	static bool WINAPI IF_StartScan(IN const int& iFeeValue);
	//ֹͣɨ��
	static bool WINAPI IF_StopScan();
	//ETC��������ʾ
	static bool WINAPI IF_CardOperationNotify(IN const char* szParamContext, IN const int& iParamSize);
	//���������޸�
	static bool WINAPI IF_ModifyVehQueue(IN const char* szParamContext, IN const int& iParamSize);
	//��ȡɨ������
	static bool WINAPI IF_GetData(IN OUT CString& strData, IN OUT int& iDataType, IN OUT int& iSrcType);
	//��С�Ѷ���Ȩ
	static bool WINAPI IF_FeeAuthorize(IN const char* szParamContext, IN const int& iParamSize);
	//������Ȩ�ӿ�
	static bool WINAPI IF_EventAuthorize(IN const char* szParamContext, IN const int& iParamSize);
	//ɾ�������Խӽ��
	static bool WINAPI IF_DelVehQueueResult(IN const char* szParamContext, IN const int& iParamSize);

	//��ά����ʾ
	static bool WINAPI IF_Tr500QRcodeDisplay(IN const char* strJsonParam, IN const int& iJsonParamSize);
	//�Ѷ���ʾ
	static bool WINAPI IF_Tr500FeeDisplay(IN const char* strJsonParam, IN const int& iJsonParamSize);
	//֧�������ʾ
	static bool WINAPI IF_Tr500PayResultDisplay(IN const char* strJsonParam, IN const int& iJsonParamSize);
	//����
	static bool WINAPI IF_Tr500ClearScreen(IN const char* strJsonParam, IN const int& iJsonParamSize);

	
	//websocketͨѶ����
	static bool WINAPI IF_WebsocketControl(IN const char* strJsonParam, IN const int& iJsonParamSize);
	
	
	//ETC���鴦��Ӧ��ָ�룬�ڻص�֪ͨ��ʱ��Ӧ�����ݴ��ݳ�ȥ
	static void* m_pIndPtr;
	static bool m_bIsDevInited;

	UINT m_ThreadNotifyCallerID;
	UINT m_EtcRespMsgID;

	UINT m_ThreadDevManagerID;
	UINT m_DevNotifyMsgID;

	std::string m_pA5InitJsonData;

private:
	//�豸�����߳�ָ��
	CThreadSMManager* m_pThreadSMManager;

public:
	CString GetCurrentDirNew();
	//BOOL	CreateDir(LPCTSTR lpzPath);
	void	RecordMsgTemp(IN LPCTSTR strMsg, IN const BOOL& bForce=TRUE);
	time_t	OleTimeToTime(DATE dTime);
	time_t	CString2time_t(LPCTSTR lpztime);
	CString FmtStr(const TCHAR* lpsFmt,...);
	void	Hex2Bin(IN LPCSTR lpHex,OUT PBYTE pBin,IN DWORD dwHex_len);
	CString Bin2Hex(IN PBYTE pBin,IN DWORD dwBin_Len);
	WORD	CRC16(LPBYTE lpData, WORD wLength);
	void CheckAndDeleteLogFile(void);

	//��¼��������Ŀ¼
	CString	m_strAppDir;	
	//��¼����������־Ŀ¼·��
	CString	m_strLogDirPath;		
	//��¼����������־����·��
	CString	m_strLogFilePath;
	//��¼����������־����·��
	CString	m_strLogFileName;	
	//��ȡ�����ļ�·��ʧ�ܺ����õ�Ĭ�������ļ�·�����붯̬��ͬĿ¼��
	CString	m_strConfigPathFile;			
	//�Ƿ�д��־���
	int		m_iWriteLog;		
	//��־�ļ����ֵ
	int		m_iLogFileMaxSize;	
	//��־�����������
	long long m_iDeleteLogSaveDays;
	//��������
	int m_SerialPort;
	int m_BaudRate;
	int m_InBufferSize;
	int m_OutBufferSize;

	CString m_strLastErrorDesc;

	CString m_QRcode;
	std::string m_DeviceNo;

	//EtcEventӦ��֪ͨ���߳�id����Ϣid

	

public:
	DECLARE_MESSAGE_MAP()
};

void LogCallBack(const std::string& content);
CScanMachineApp* GetGlobalApp();