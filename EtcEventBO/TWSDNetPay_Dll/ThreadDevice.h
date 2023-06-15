/*
 * Copyright(C) 2016,SQUIRREL ���˹����� ��������Ȩ����( All rights reserved. )
 * 
 * �ļ����ƣ�ThreadDevice.h
 * ժ    Ҫ��Ӳ�������̣߳������ļ�
 * ��ǰ�汾��1.0.0.1
 * ��    �ߣ�ROCY
 * �������ڣ�2016��7��23��
 */
#pragma once
#include "ThreadBase.h"
#include "HTSDDeviceDll.h"
#include "NFCProtocol.h"
#include "DVXMLUnipay.h"
  
// CThreadDevice

class CThreadDevice : public CThreadBase
{
	DECLARE_DYNCREATE(CThreadDevice)

public://protected --> public ����δ֪��ԭ��ExitInstance���߳��˳���ʱ��û�б�ִ�У������Ҫ�ֶ�������Դ��
	CThreadDevice();           // protected constructor used by dynamic creation
	virtual ~CThreadDevice();
public:
	CUpdateData		c_update_device;//������ײ���Ʒ��͵ĸ��¶���
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual BOOL OnIdle(LONG lCount);
//Operations
	inline const BOOL GetDeviceInitState(void){if(NULL != m_pHTSDDeviceDll) return m_pHTSDDeviceDll->GetDevInitState(); return FALSE;}
	inline const BOOL GetDllInitState(void){if(NULL != m_pHTSDDeviceDll) return m_pHTSDDeviceDll->GetDllInitState(); return FALSE;}
	//��ȡӲ���豸״̬
	DWORD	GetDeviceState(void);

/**************ETC����ʹ�ýӿ�****************************************************/
	bool EtcEventInitDev(UINT& dwThreadNotifyCallerID, UINT& dwThreadManagerID, HWND& hWnd, int dwEtcRespMsgID, int dwDevNotifyMsgID, void* pIndPtr);
	bool EtcEventSetA5InitData(const char* szParamContext, int iParamSize);
	bool EtcEventDeInitDev();
	CString EtcEventGetLastErrorDesc();
	bool EtcEventGetDevStatus(unsigned int& uStatus);
	bool EtcEventDealStart(const char* szParamContext, int iParamSize);
	bool EtcEventCheckVehQueue(const char* szParamContext, int iParamSize);
	bool EtcEventCheckVehInfo(const char* szParamContext, int iParamSize);
	bool EtcEventCheckEntryInfo(const char* szParamContext, int iParamSize);
	bool EtcEventShowFeeInfo(const char* szParamContext, int iParamSize);
	bool EtcEventPayResultDisplay(const char* szParamContext, int iParamSize);
	bool EtcEventDealStop(const char* szParamContext, int iParamSize);
	bool EtcEventStartScan();
	bool EtcEventStopScan();
	bool EtcEventCardOperationNotify(const char* szParamContext, int iParamSize);
	bool EtcEventModifyVehQueue(const char* szParamContext, int iParamSize);
	bool EtcEventFeeAuthorize(const char* szParamContext, int iParamSize);
	bool EtcEventFreeVoicePlay(const char* szParamContext, int iParamSize);
	bool EtcEventAuthorize(const char* szParamContext, int iParamSize);
	bool EtcEventDelVehQueueResult(const char* szParamContext, int iParamSize);
/*****************************************************************************/
	CString ConstructClearScreenJson();

	void	ReadWebConfig();
	void	ITMSMain();
	void	LocalDBCheck();
	void	ArrayCheck();
	void	GGPublish(int nPos);

	int		GetLocalDB2Array(LPCTSTR strDBFile, LPCTSTR strTBName, CUpdateData& cUpdate, int nType, CString& csError);
	int		SetLocalDBTransMark(LPCTSTR strDBFile, LPCTSTR strTBName, CUpdateData& cUpdate, CString& csError );

public://add zhuyabing
	void RestartNFCPay();
	void RemoteControlFirmware(const std::string&para);
	BOOL QueryLEDVersion(OUT CString& strJson);//add 2018-07
	void TransferUserDefineData(const std::string& type,const std::string& content);
private:
	void SetDebitSuccessHint(int debitType);
private://add zhuyabing
	CString m_LastNFCPayData;


protected:
	//ִ���·����ݹ��ܲ���
	afx_msg void OnMsgIssueData(WPARAM wParam,LPARAM lParam);
	//ִ��������ȡ���ݵĲ���
	afx_msg void OnMsgStartGetData(WPARAM wParam,LPARAM lParam);
	//ִ��ֹͣ��ȡ���ݵĲ���
	afx_msg void OnMsgStopGetData(WPARAM wParam,LPARAM lParam);
	//���յ�Ӳ�����͵���Ϣ���������ݻ�ȡ
	afx_msg void OnMsgGetData(WPARAM wParam,LPARAM lParam);
	//���յ���Ϣ��Ҫ��ִ�з��Թ���
	afx_msg void OnMsgShowDealInfo(WPARAM wParam,LPARAM lParam);
	//���յ���Ϣ�������ۿ�����
	afx_msg void OnMsgStartDebitFlow(WPARAM wParam,LPARAM lParam);
	//���յ���Ϣ��Ҫ��չʾ�ɷѽ����������
	afx_msg void OnMsgControlResultMMI(WPARAM wParam,LPARAM lParam);

	afx_msg void OnMsgRemoteUpgradeResult(WPARAM wParam,LPARAM lParam);

	afx_msg void OnMsgPlaySoundOfPaying(WPARAM wParam,LPARAM lParam);

	afx_msg void OnMsgFeeDisplay(WPARAM wParam, LPARAM lParam);
	afx_msg void OnMsgPayResultDisplay(WPARAM wParam, LPARAM lParam);
	afx_msg void OnMsgQrcodeDisplay(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
private:
	//������Ӳ������ʱ��
	BOOL UpdateDeviceTime(void);
	//������ʾ֧���쳣����Ϣ��ռ�ù��λ�ã�
	void SetPayErrorMsgInfo(IN const int& iErrorCode = 0,IN const bool& bForceDefault = false);
//Attribute:
private:
	//����Ӳ���汾(1,��ʼ�汾;2,1.5�汾)
	int				m_iDeviceVersion;
	//Ӳ�����ƽӿ���
	CHTSDDeviceDll*	m_pHTSDDeviceDll;
	//Ӳ���ϴμ���״̬
	DWORD			m_dwLastCheckState;

	CString			m_csUpdateLDB;	//���ظ��´洢�ļ�
	CString			m_csLDBTable;	//���ظ��´洢�ļ�����
	CString			m_csDevLog;		//Ӳ����־��¼

	int				nLocalDBSpan;	//�������ݿ��⴦����
	int				nArraySpan;		//���¶��м�⴦����
	DWORD			dwLocalDBCheck;	//�������ݿ���ʱ���
	DWORD			dwArrayCheck;	//���¶��м��ʱ���
	CString			m_strDefaultGGText;	//Ĭ�ϴ��ƶ������������ֹ������

	bool m_firmwareUpdateFlag ;//add 2018-01 ��ʾ�Ƿ��й̼����£�
	time_t m_firmwareUpdateTime; //add 2018-01 �̼�������ʱ��

private:
	void Write_dev_id();
	std::string mShareMemNameOfDevId;
	HANDLE mhShareMemOfDevId;
	char* mpShareMemOfDevId;

};


