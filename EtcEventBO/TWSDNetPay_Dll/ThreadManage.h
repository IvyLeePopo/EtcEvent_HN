/*
 * Copyright(C) 2016,SQUIRREL ���˹����� ��������Ȩ����( All rights reserved. )
 * 
 * �ļ����ƣ�ThreadManage.h
 * ժ    Ҫ��ҵ���̨�����̣߳������ļ�
 * ��ǰ�汾��1.0.0.1
 * ��    �ߣ�ROCY
 * �������ڣ�2016��7��24��
 */
#pragma once
#include "ThreadBase.h"
#include "ThreadHttp.h"
#include "ThreadDevice.h"
#include "ThreadUpdate.h"
#include "ThreadNFCSignIn.h"
#include "DVXMLDebitMoney.h"
#include "NFCProtocol.h"

#include "PayManage.h"
#include "MediaManage.h"
class CThreadManage : public CThreadBase
{
	DECLARE_DYNCREATE(CThreadManage)

private://add 2017-11
	bool goDie;
	int dieNumber;
	CRITICAL_SECTION idleMutex;

protected:
	CThreadManage();           // protected constructor used by dynamic creation
	virtual ~CThreadManage();

	//�Ӻ����ƶ�֧���м���ƹ���---- 20211207
	BOOL RestartWindows();//--------����ϵͳ�����ﲻ��Ҫ
	void MoveConfigFileToExeDir();
	void modifyUpdateflagResult();//------�޸ĸ��º�ķ���״̬�����ﲻ��Ҫ

	bool m_bReBootSystem; //��ʶϵͳ����----����������������ﲻ��Ҫ


public:
	bool IsFuturePay(){return m_PayCenter.IsEanble();}//�Ƿ�ʹ�ø�����֧���߼�
public:
	int StartNewPay(const DebitParameter& dp){return m_PayCenter.StartNewPay(dp);}
    int QueryCurrentPayState(std::string& pay_id,int& state){return m_PayCenter.QueryCurrentPayState(pay_id,state);}//��ѯ���µ�֧��״̬
    int CancelCurrentPay(){return m_PayCenter.CancelCurrentPay();};
    int CancelTargetPay(const DebitCancleParameter& dp){return m_PayCenter.CancelTargetPay(dp);};
	int GetCurrentDebitResult(DebitResultParameter& dp){return m_PayCenter.GetCurrentDebitResult(dp);};

	int InitAgencyPay(const char* szAreaInfo, const char* szLoaclStation,  const char* szLoaclLaneID,  const char* szServerInfo,  const int& iProvinceID);
	bool QueryVirtualCardDebit(const char* szParamContext, const int& iParamSize, const int& iFormatType, int& iSupportAgent, char* szResult);
	bool AgencyDebitQuery(const char* szParamContext, const int& iParamSize, const int& iFormatType, int& iResult);
	bool TransferImage(const char* szImageInfo, const int& iSize, const int& iFormatType);
	int SetAgencyPara(const char* szAreaInfo, const char* szLoaclStation,  const char* szLoaclLaneID,  const char* szServerInfo,  const int& iProvinceID);
	int MediaStart(){return m_MediaCenter.EventDealStart();}
	int MediaStop(){return m_MediaCenter.EventDealStop();}

	bool TransferCashInvoiceDataXML(const char* szInvoiceData, const int& iSize);
	bool TransferCashInvoiceDataJSON(const char* szInvoiceData, const int& iSize);
	bool CancelCashInvoiceDataXML(const char* szInvoiceData, const int& iSize);
	bool CancelCashInvoiceDataJSON(const char* szInvoiceData, const int& iSize);

private:
	std::string m_PayAreaInfo;
	std::string m_PayStationInfo;
	std::string m_PayLaneInfo;
	std::string m_PayServerInfo;
	int         m_PayProvince;
	CPayManage  m_PayCenter;
	CMediaManage m_MediaCenter;

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual BOOL OnIdle(LONG lCount);
//Operations
public:
	//���������߳�
	const BOOL StartWorkThreads(IN const tagTranstParam& sTranstParam);
	//�ж��û������Ƿ��Ѿ�������Ӳ���ṩ��
	const BOOL GetAccountDataReadyState(void) {return m_bAccountInfoIsReady;}
	//�����û������Ƿ�׼������
	void SetAccountDataReadyState(IN const BOOL& bState);
	//��ȡ�ϴμ��������û�����
	const tagAccountInfo GetLastAccountInfo(void){return m_sAccountInfo;}
	//���ö�ʱ��
	void StartTimer(IN const UINT& nEventID,IN const DWORD& dwDelayTime);
	//ֹͣ��ʱ��
	void StopTimer(IN const UINT& nEventID);
	//��ȡӲ����������״��
	const BOOL GetDeviceLoadStae(void) const {return m_bDeviceLoadState;}
	//�߳��˳�
	void	ThreadQuit();

//Override
protected:
	//��������������Դ
	void fnReleaseResource();
//Inner Functions
private:
	//���췢�͵�֧����ƽ̨������
	BOOL ContructDebitMoneyInfo(IN const tagAccountInfo* pAccountInfo);
	//���췢�͵�֧����ƽ̨������
	BOOL ContructDebitMoneyInfo(void);
	//���췢�͵�֧����ƽ̨������
	BOOL ContructDebitCancelInfo(IN const CString& strPayIdentifier);
	//��ȡ����ƥ����
	const CString GetNetPayRecordMatchCode(IN const CString& strEncKey=_T(""));
	//��ȡ���װ�ȫ��
	const CString GetNetPaySecurityCode(void);
	//������ɾ����־
	void CheckAndDeleteLogFile(void);
	//���׽����Ĵ���
	CString FinishTrade(IN const BOOL& bNormal);
	//������ȡ���Ľ��ײ����г�������
	void FinishHadCancelTrade(IN const CDVJsonBase* pDVJsonBase);
	//��ʱ����Ӧ����
	static void CALLBACK OneMilliSecondProc(UINT nTimerID, UINT msg,DWORD dwUser,DWORD dwl,DWORD dw2);


	//NFC�������ӻ���������
	void NFCLinkTestDataProcessFromPOS(IN CNFCProtocol& cNFCProtocol);
	//NFC�������ݴ�����
	void NFCConsumeDataProcessFromPOS(IN CNFCProtocol& cNFCProtocol);
	//NFC�������ݴ�����
	void NFCCorrectDataProcessFromPOS(IN CNFCProtocol& cNFCProtocol);
	//NFCģ���������֡�쳣����
	void NFCProcessErrorFrame(IN CNFCProtocol& cNFCProtocol);
	//NFCǩ�����ݴ����������ݴ�POS�ǽ�ģ��ش���
	void NFCSignInDataProcessFromPOS(IN CNFCProtocol& cNFCProtocol);
	//NFCǩ�����ݴ����������ݴ�HTTP�ش���
	void NFCSignInDataProcessFromHTTP(IN const CString& strData);
	//NFC����ʧ�ܴ�������δ�ݽ��ƶˣ�
	void NFCLocalTradeFailProcess(IN const int& iExecuteCode = 0xFF);
	//NFC�������̣�����Ĭ�ϵ�ָ�1��������·/��λ��2������ǩ����3��ִֹͣ����һ�����̣�
	void NFCSendDefaultCmdToPos(IN const int& iCmdType);

	//add 2018-05
	bool CheckPayResultValidity(const CDVJsonDebit&  payResult);
public:
	void NFCParaQuery(const CString& strData);
	//void SetDebitSuccessHint(int debitType);
protected:
	//����������ȡ�û���Ϣ��������Ϣ
	afx_msg void OnMsgStartGetAccountInfo(WPARAM wParam,LPARAM lParam);
	//����ֹͣ��ȡ�û���Ϣ��������Ϣ
	afx_msg void OnMsgStopGetAccountInfo(WPARAM wParam,LPARAM lParam);
	//�����ȡ�û���Ϣ����Ϣ
	afx_msg void OnMsgReceiveAccountInfo(WPARAM wParam,LPARAM lParam);
	//����֧����ƽ̨��������Ϣ
	afx_msg void OnMsgDealSubPayPlatformResponse(WPARAM wParam,LPARAM lParam);
	//��ѯ�û���������Ľ��״̬
	afx_msg void OnMsgCheckDebitPasswordState(WPARAM wParam,LPARAM lParam);
	//ִ�пۿ����(����ģʽ)
	afx_msg void OnMsgManageStartDebitFlow(WPARAM wParam,LPARAM lParam);
	//ִ�пۿ����(����ģʽ)
	afx_msg void OnMsgOnlyDebitMoney(WPARAM wParam,LPARAM lParam);
	//ִ�г�������
	afx_msg void OnMsgDebitCancel(WPARAM wParam,LPARAM lParam);
	//�����HTTP�˷��ص�NFCǩ������
	afx_msg void OnMsgRevNFCSignDataFromHTTP(WPARAM wParam,LPARAM lParam);
	//��ȡ֪ͨ������ǩ������
	afx_msg void OnMsgNFCStartSignInFlow(WPARAM wParam,LPARAM lParam);

	//add 2017-12
	afx_msg void OnMsgRemoteControlFirmware(WPARAM wParam,LPARAM lParam);
	afx_msg void OnMsgRemoteUpgradeResult(WPARAM wParam,LPARAM lParam);

	afx_msg void OnMsgQueryLPRImageId(WPARAM wParam,LPARAM lParam);

	DECLARE_MESSAGE_MAP()
//Attribute
public:
	//Ӳ��������ָ��
	CThreadDevice*		m_pThreadDevice;

	//�����߳���ָ��
	CThreadUpdate*		m_pThreadUpdate;

	//����ģ��ǩ���߳���ָ��
	CThreadNFCSignIn*	m_pThreadNFCSignIn;

	//�ۿ�״̬��ѯ
	CDVJsonDebitCheck*  m_pDVJsonDebitCheck;
	//�ۿ����ݽṹ��
	CDVJsonDebit		m_sDVJsonDebit;
	//Ĭ����������ĳ�ʱʱ��
	DWORD				m_dwInputPasswordDelayTime;
private:
	//֧����ƽ̨ͨѶ�߳���ָ��
	CThreadHttp*		m_pThreadHttp;
	//�û���Ϣ�Ƿ�׼������(���ݱ�ȡ�ߺ���Ҫ����״̬)
	BOOL				m_bAccountInfoIsReady;
	//Ӳ�������������
	BOOL				m_bDeviceLoadState;
	//��ʱ���ֱ���
	UINT				m_nAccuracy;

	//��ʱ��ID(ɨ�볬ʱ)
	UINT				m_nTimerIDForScanCode;
	//��ʱ��ID(�ۿʱ)
	UINT				m_nTimerIDForDebitMoney;
	//��ʱ��ID(�ۿ�״̬��ѯ)
	UINT				m_nTimerIDForCheckDebitState;
	//��ʱ��ID(NFC������ˮ��ȡ��ʱ)
	UINT				m_nTimerIDForNFCGetCorrentData;
	//��ʱ��ID(NFC��ͨ���ݻ�ȡ��ʱ)
	UINT				m_nTimerIDForNFCGetNormalData;
	//�ۿ�״̬��ѯʱ����
	DWORD				m_dwCheckDebitSpanTime;
	//�ۿ�״̬��ѯʱ���ܼ�
	DWORD				m_dwCheckDebitAccumulateTime;
	//�ϴλ�ȡ���û�����
	tagAccountInfo		m_sAccountInfo;
};


