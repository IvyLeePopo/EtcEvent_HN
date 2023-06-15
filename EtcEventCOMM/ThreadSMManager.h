/*
 * Copyright(C) ������΢���� ��������Ȩ����( All rights reserved. )
 *
 * �ļ����ƣ�ThreadSMManager.h
 * ժ    Ҫ��ɨ���������߳��࣬�����ļ�
 * ��ǰ�汾��1.0.0.0
 * ��    �ߣ������
 * �������ڣ�2020��10��18��
 */
#pragma once
#include "ThreadBase.h"
#include "ThreadHelper.h"
#include "SerialCom.h"
#include "UniQrProtocol.h"
#include <bitset>

// CThreadSMManager
class CThreadSMManager : public CThreadBase
{
	DECLARE_DYNCREATE(CThreadSMManager)

public:
	CThreadSMManager();           // protected constructor used by dynamic creation
	virtual ~CThreadSMManager();

	bool IsConnectionOK();
	void SetComPara(int port,int baud);

/********************ETC����ר�ýӿ�***********************************************************/
	bool EventDealStart(const char* ParamContext, int iParamSize);
	bool EventCheckVehQueue(const char* ParamContext, int iParamSize);
	bool EventCheckVehInfo(const char* ParamContext, int iParamSize);
	bool EventCheckEntryInfo(const char* ParamContext, int iParamSize);
    bool EventShowFeeInfo(const char* ParamContext, int iParamSize);
    bool EventPayResultShow(const char* ParamContext, int iParamSize);
    bool EventDealStop(const char* ParamContext, int iParamSize);
	bool EventStartScan();
    bool EventStopScan();
	bool EventCardOperationNotify(const char* ParamContext, int iParamSize);
	bool EventModifyVehQueue(const char* ParamContext, int iParamSize);
	bool EventFeeAuthorize(const char* ParamContext, int iParamSize);
	bool EventAuthorize(const char* ParamContext, int iParamSize);
	bool EventDelVehQueueResult(const char* szParamContext, int iParamSize);

/********************************************************************************************/
	bool QRcodeDisplay(const char* ParamContext, int iParamSize);
	bool FeeDisplay(const char* ParamContext, int iParamSize);
	bool PayResultDisplay(const char* ParamContext, int iParamSize);
	bool ClearScreen(const char* ParamContext, int iParamSize);

private:
	UniQrDriver m_driver;
	CRITICAL_SECTION	m_driverMutex;
	CSerialCom*			m_pCOM;
	tagCOMParam			m_sLocalCOM;
	time_t				m_tLastComReadDataTime;
	time_t				m_tLastComWriteDataTime;
private:
	unsigned short NextCMDId();
	bool WriteDataToComm(const std::string& hex_data);
	bool IsSendSuccess(unsigned short id);
	void CheckTerminalAlive();
	bool CheckDeviceNo();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual BOOL OnIdle(LONG lCount);
//���غ���
protected:
	//��������������Դ
	void fnReleaseResource();
	
protected:
	DECLARE_MESSAGE_MAP()

	/*******************************�ڲ�������Ϣ����������*******************************/
	//�ڲ�������Ϣ���������� -- ��ȡ����������
	afx_msg void OnMsgReceiveDataFromCOM(WPARAM wParam, LPARAM lParam);
	//֪ͨ�������������Ѿ��������
	afx_msg void OnMsgDataSendEnd(WPARAM wParam, LPARAM lParam);
	//֪ͨ�����ߴ��ڷ�������
	afx_msg void OnMsgComError(WPARAM wParam, LPARAM lParam);

private:
	//����˿ڲ������� -- �򿪴�������
	BOOL	OpenCom(void);
	//����˿ڲ������� -- ������д���ݣ��ֽ�����ʽ
	DWORD	WriteDataToComm(IN const LPBYTE byData, IN const DWORD& dwSize);

public:
	//����˿ڲ������� -- �رմ�������
	BOOL	CloseCom(void);
	//���ڴ򿪱��
	BOOL	m_bOpen;
	int     m_DevStatus; //0��������4��Ӳ��ͨѶ����
};