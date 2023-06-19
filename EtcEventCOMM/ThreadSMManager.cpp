/*
 * Copyright(C) ������΢���� ��������Ȩ����( All rights reserved. )
 *
 * �ļ����ƣ�ThreadSMManager.cpp
 * ժ    Ҫ��ɨ���������߳��࣬ʵ���ļ�
 * ��ǰ�汾��1.0.0.0
 * ��    �ߣ������
 * �������ڣ�2020��10��18��
 */
// ThreadSMManager.cpp : implementation file
//

#include "stdafx.h"
#include "StateDefine.h"
#include "ThreadSMManager.h"
#include "ScanMachine.h"

extern CScanMachineApp theApp;


// CThreadSMManager

IMPLEMENT_DYNCREATE(CThreadSMManager, CThreadBase)

CThreadSMManager::CThreadSMManager()
	: m_bOpen(FALSE)
	, m_pCOM(NULL)
	, m_tLastComReadDataTime(0)
	, m_tLastComWriteDataTime(0)
{
	m_strPathLocalConfigFile.Format(_T("%s\\%s"), theApp.m_strAppDir, FILE_NAME_DEVICE_SM_CONFIG);

	m_driver.next_send_id = 0;
	m_driver.initdev_flag = false;

	::InitializeCriticalSection(&m_driverMutex);

	m_DevStatus = 0;
}

CThreadSMManager::~CThreadSMManager()
{
	::DeleteCriticalSection(&m_driverMutex);
}

BOOL CThreadSMManager::InitInstance()
{
	m_pCOM = new CSerialCom();
	if (NULL != m_pCOM)
	{
		m_pCOM->SetMain(m_nThreadID);
		m_pCOM->SetNotifyNum(1);

		::Sleep(50);
		m_bOpen = OpenCom();
		if (!m_bOpen)
		{
			theApp.RecordMsgTemp(theApp.FmtStr(_T("[�豸�����߳�(ID:%ld)],����[�˿�:%d]��ʧ��"), m_nThreadID, m_sLocalCOM.Port));
		}
		else
		{
			theApp.RecordMsgTemp(theApp.FmtStr(_T("[�豸�����߳�(ID:%ld)],����[�˿�:%d]�򿪳ɹ�"), m_nThreadID, m_sLocalCOM.Port));
		}		
	}

	m_tLastComReadDataTime = time(NULL);
	return TRUE;
}

int CThreadSMManager::ExitInstance()
{
	fnReleaseResource();
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CThreadSMManager, CThreadBase)
	ON_THREAD_MESSAGE(ON_COM_RECEIVE, OnMsgReceiveDataFromCOM)
	ON_THREAD_MESSAGE(ON_COM_TXEMPTY, OnMsgDataSendEnd)
	ON_THREAD_MESSAGE(ON_COM_ERROR,	  OnMsgComError)
END_MESSAGE_MAP()



////////////////////////////////////////////////////////////////////////////////////////////////
// �������ƣ�CThreadSMManager::fnReleaseResource
// ������������������������Դ
// ����������޲���
// ���������void
// ��д��Ա�������
// ��дʱ�䣺2020��10��18��
// �޸���Ա��
// �޸�ʱ�䣺
// �����汾��1.0.0.0
// ��ע˵����
////////////////////////////////////////////////////////////////////////////////////////////////
void CThreadSMManager::fnReleaseResource()
{
	::Sleep(10);
	//�رմ���
	CloseCom();

	if (NULL != m_pCOM)
	{
		delete m_pCOM;
		m_pCOM = NULL;
	}
}

void CThreadSMManager::SetComPara(int port, int baud)
{
	m_sLocalCOM.Port = port;
	m_sLocalCOM.BaudRate = baud;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// �������ƣ�CThreadSMManager::OpenCom
//
// ������������ʼ����������
//
// �����������
// ���������BOOL
// ��д��Ա��ROCY
// ��дʱ�䣺2015��12��1��
// �޸���Ա��
// �޸�ʱ�䣺
// �����汾��1.0.0.1
// ��ע˵����
//
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CThreadSMManager::OpenCom()
{
	if (NULL != m_pCOM)
	{
		m_pCOM->SetNotifyNum(1);
		m_bOpen = m_pCOM->Open(m_sLocalCOM.Port, m_sLocalCOM.BaudRate);

		CString strDesc = theApp.FmtStr(_T("���ش���[�˿ں�:%d,������:%d,ֹͣλ:%d,У��λ:%d,����λ:%d]��%s"), m_sLocalCOM.Port, m_sLocalCOM.BaudRate, m_sLocalCOM.StopBit, m_sLocalCOM.ParityBit, m_sLocalCOM.DataBit, m_bOpen ? _T("�ɹ�") : _T("ʧ��"));
		theApp.RecordMsgTemp(strDesc);
		strDesc.ReleaseBuffer();
	}
	return m_bOpen;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// �������ƣ�CThreadSMManager::CloseCom
//
// �����������رմ�������
//
// ����������޲���
// ���������BOOL
// ��д��Ա��ROCY
// ��дʱ�䣺2015��12��1��
// �޸���Ա��
// �޸�ʱ�䣺
// �����汾��1.0.0.1
// ��ע˵����
//
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CThreadSMManager::CloseCom()
{
	BOOL bReturn = FALSE;
	if (NULL != m_pCOM)
	{
		m_pCOM->Close();
		m_bOpen = FALSE;
		bReturn = TRUE;

	}
	return bReturn;
}


////////////////////////////////////////////////////////////////////////////////////////////////
// �������ƣ�CThreadSMManager::WriteDataToComm
//
// ����������������д���ݣ��ֽ�����ʽ
//
// ���������IN const LPBYTE byData
//         ��IN const DWORD& dwCommandDataSize
// ���������DWORD
// ��д��Ա��ROCY
// ��дʱ�䣺2015��12��1��
// �޸���Ա��
// �޸�ʱ�䣺
// �����汾��1.0.0.1
// ��ע˵����
//
////////////////////////////////////////////////////////////////////////////////////////////////
DWORD CThreadSMManager::WriteDataToComm(IN const LPBYTE byData, IN const DWORD& dwCommandDataSize)
{
	DWORD dwWriteDataLen = 0;
	if (NULL != m_pCOM)
	{
		dwWriteDataLen = m_pCOM->Write(byData, dwCommandDataSize);
		if (1)
		{
			CString strTemp;
			CString strCmd;
			for (DWORD i = 0; i < dwCommandDataSize; ++i)
			{
				strTemp.Format(_T("%02X "), byData[i]);
				strCmd += strTemp;
			}
			theApp.RecordMsgTemp(theApp.FmtStr(_T("[ɨ�빤���߳�],�������ݵ��豸:%d -- %s"), dwCommandDataSize, strCmd));
		}
	}
	return dwCommandDataSize;
}


////////////////////////////////////////////////////////////////////////////////////////////////
// �������ƣ�CThreadSMManager::OnIdle
//
// ����������ɨ�����̹߳����࣬���д�����
//
// ���������LONG lCount
// ���������BOOL
// ��д��Ա��ROCY
// ��дʱ�䣺2015��11��27��
// �޸���Ա��
// �޸�ʱ�䣺
// �����汾��1.0.0.1
// ��ע˵����
//
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CThreadSMManager::OnIdle(LONG lCount)
{
	if (m_bExit)
	{
		//�رմ�������
		CloseCom();
		//�����˳���Ϣ
		::PostQuitMessage(0);

		return FALSE;
	}

	CheckTerminalAlive();

	CheckDeviceNo();

	::Sleep(100);
	return TRUE;
}


////////////////////////////////////////////////////////////////////////////////////////////////
// �������ƣ�CThreadSMManager::OnMsgDataSendEnd
// ����������֪ͨ�������������Ѿ��������
// ���������WPARAM wParam
//         ��LPARAM lParam
// ���������void
// ��д��Ա�������
// ��дʱ�䣺2020��10��18��
// �޸���Ա��
// �޸�ʱ�䣺
// �����汾��1.0.0.0
// ��ע˵����
////////////////////////////////////////////////////////////////////////////////////////////////
void CThreadSMManager::OnMsgDataSendEnd(WPARAM wParam, LPARAM lParam)
{
	m_tLastComWriteDataTime = time(NULL);
}

////////////////////////////////////////////////////////////////////////////////////////////////
// �������ƣ�CThreadSMManager::OnMsgComError
// ����������֪ͨ�����ߴ��ڷ�������
// ���������WPARAM wParam
//         ��LPARAM lParam
// ���������void
// ��д��Ա�������
// ��дʱ�䣺2020��10��18��
// �޸���Ա��
// �޸�ʱ�䣺
// �����汾��1.0.0.0
// ��ע˵������Ӧ��Ϣ ON_COM_ERROR
////////////////////////////////////////////////////////////////////////////////////////////////
void CThreadSMManager::OnMsgComError(WPARAM wParam, LPARAM lParam)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////
// �������ƣ�CThreadSMManager::OnMsgReceiveDataFromCOM
// �����������ڲ�������Ϣ���������� -- ��ȡ����������
// ���������WPARAM wParam
//         ��LPARAM lParam
// ���������void
// ��д��Ա�������
// ��дʱ�䣺2020��10��18��
// �޸���Ա��
// �޸�ʱ�䣺
// �����汾��1.0.0.0
// ��ע˵����
////////////////////////////////////////////////////////////////////////////////////////////////
void CThreadSMManager::OnMsgReceiveDataFromCOM(WPARAM wParam, LPARAM lParam)
{
	char buff[1000];
	DWORD buf_size = 800;
	DWORD read_size = m_pCOM->Read(buff, buf_size);
	while (read_size > 0)
	{
		{
			CScopeLock lock(&m_driverMutex);
			std::copy(buff, buff + read_size, std::back_inserter(m_driver.uncomplete_datas));
		}
		buf_size = 800;
		read_size = m_pCOM->Read(buff, buf_size);
	}
	m_tLastComReadDataTime = time(NULL);
	m_DevStatus = 0; //Ӳ��ͨѶ����
	CScopeLock lock(&m_driverMutex);
	std::vector<std::string> c0_queue;
	ParseProtocolData(m_driver,c0_queue);

	for(size_t i=0;i<c0_queue.size();++i)
	{
		WriteDataToComm(c0_queue[i]);
	}

	if(m_driver.EtcEventId == 608)
	{
		//theApp.RecordMsgTemp(_T("�յ�֧����ά������"));
		
		theApp.m_QRcode.Empty();
		theApp.m_QRcode = ParseQRcode(m_driver.EtcEventRespContent);
		theApp.RecordMsgTemp(theApp.FmtStr(_T("֧����ά������: %s"), theApp.m_QRcode.GetBuffer(0)));

		::PostThreadMessage(theApp.m_ThreadDevManagerID, theApp.m_DevNotifyMsgID, WPARAM(1), LPARAM(0));
		return;
	}
	if (m_driver.EtcEventId == 630)
	{
		//m_driver.isEtcEventResp = false;
		theApp.m_DeviceNo = m_driver.EtcEventRespContent;
		theApp.RecordMsgTemp(theApp.FmtStr(_T("�豸���к�: %s"), theApp.m_DeviceNo.c_str()));
	}

	if(m_driver.isEtcEventResp == true)
	{
		//theApp.NotifyCaller(m_driver.EtcEventId, m_driver.EtcEventRespContent);
		//theApp.RecordMsgTemp(_T("�ص������������"));
		memcpy(CScanMachineApp::m_pIndPtr, m_driver.EtcEventRespContent, sizeof(m_driver.EtcEventRespContent));
		::PostThreadMessage(theApp.m_ThreadNotifyCallerID, theApp.m_EtcRespMsgID, WPARAM(m_driver.EtcEventId), LPARAM(0));
	}
	return;
}

bool CThreadSMManager::IsConnectionOK()
{
	return m_bOpen;
}

/**********************ETC����ר��*********************************************/

bool CThreadSMManager::EventDealStart(const char* ParamContext, int iParamSize)
{
	unsigned short id = NextCMDId();
	std::string cmd_data = PackDataEventDealStart(ParamContext, iParamSize, id);
	
	bool SendResult = false;
	int times = 3;
	while(times)
	{		
		WriteDataToComm(cmd_data);
		if(IsSendSuccess(id))
		{
			SendResult = true;
			break;
		}
		times--;
	}

	return SendResult;
}

bool CThreadSMManager::EventCheckVehQueue(const char* ParamContext, int iParamSize)
{
	unsigned short id = NextCMDId();
	std::string cmd_data = PackDataEventCheckVehQueue(ParamContext, iParamSize, id);

	bool SendResult = false;
	int times = 3;
	while(times)
	{		
		WriteDataToComm(cmd_data);
		if(IsSendSuccess(id))
		{
			SendResult = true;
			break;
		}
		times--;
	}
	return SendResult;
}

bool CThreadSMManager::EventCheckVehInfo(const char* ParamContext, int iParamSize)
{
	unsigned short id = NextCMDId();
	std::string cmd_data = PackDataEventCheckVehInfo(ParamContext, iParamSize, id);
	
	bool SendResult = false;
	int times = 3;
	while(times)
	{		
		WriteDataToComm(cmd_data);
		if(IsSendSuccess(id))
		{
			SendResult = true;
			break;
		}
		times--;
	}
	return SendResult;
}

bool CThreadSMManager::EventCheckEntryInfo(const char* ParamContext, int iParamSize)
{
	unsigned short id = NextCMDId();
	std::string cmd_data = PackDataEventCheckEntryInfo(ParamContext, iParamSize, id);
	
	bool SendResult = false;
	int times = 3;
	while(times)
	{		
		WriteDataToComm(cmd_data);
		if(IsSendSuccess(id))
		{
			SendResult = true;
			break;
		}
		times--;
	}
	return SendResult;
}

bool CThreadSMManager::EventShowFeeInfo(const char* ParamContext, int iParamSize)
{
	unsigned short id = NextCMDId();
	std::string cmd_data = PackDataEventShowFeeInfo(ParamContext, iParamSize, id);
	
	bool SendResult = false;
	int times = 3;
	while(times)
	{		
		WriteDataToComm(cmd_data);
		if(IsSendSuccess(id))
		{
			SendResult = true;
			break;
		}
		times--;
	}
	return SendResult;
}

bool CThreadSMManager::EventPayResultShow(const char* ParamContext, int iParamSize)
{
	unsigned short id = NextCMDId();
	std::string cmd_data = PackDataEventPayResultShow(ParamContext, iParamSize, id);
	
	bool SendResult = false;
	int times = 3;
	while(times)
	{		
		WriteDataToComm(cmd_data);
		if(IsSendSuccess(id))
		{
			SendResult = true;
			break;
		}
		times--;
	}
	return SendResult;
}

bool CThreadSMManager::EventDealStop(const char* ParamContext, int iParamSize)
{
	unsigned short id = NextCMDId();
	std::string cmd_data = PackDataEventDealStop(ParamContext, iParamSize, id);
	
	bool SendResult = false;
	int times = 3;
	while(times)
	{		
		WriteDataToComm(cmd_data);
		if(IsSendSuccess(id))
		{
			SendResult = true;
			break;
		}
		times--;
	}
	return SendResult;
}

bool CThreadSMManager::EventCardOperationNotify(const char* ParamContext, int iParamSize)
{
	unsigned short id = NextCMDId();
	std::string cmd_data = PackDataEventCardOperationNotify(ParamContext, iParamSize, id);

	bool SendResult = false;
	int times = 3;
	while(times)
	{		
		WriteDataToComm(cmd_data);
		if(IsSendSuccess(id))
		{
			SendResult = true;
			break;
		}
		times--;
	}
	return SendResult;
}

bool CThreadSMManager::EventModifyVehQueue(const char* ParamContext, int iParamSize)
{
	unsigned short id = NextCMDId();
	std::string cmd_data = PackDataEventModifyVehQueue(ParamContext, iParamSize, id);

	bool SendResult = false;
	int times = 3;
	while(times)
	{		
		WriteDataToComm(cmd_data);
		if(IsSendSuccess(id))
		{
			SendResult = true;
			break;
		}
		times--;
	}
	return SendResult;
}

bool CThreadSMManager::EventFeeAuthorize(const char* ParamContext, int iParamSize)
{
	unsigned short id = NextCMDId();
	std::string cmd_data = PackDataEventFeeAuthorize(ParamContext, iParamSize, id);

	bool SendResult = false;
	int times = 3;
	while(times)
	{		
		WriteDataToComm(cmd_data);
		if(IsSendSuccess(id))
		{
			SendResult = true;
			break;
		}
		times--;
	}
	return SendResult;
}

bool CThreadSMManager::EventAuthorize(const char* ParamContext, int iParamSize)
{
	unsigned short id = NextCMDId();
	std::string cmd_data = PackDataEventAuthorize(ParamContext, iParamSize, id);

	bool SendResult = false;
	int times = 3;
	while(times)
	{		
		WriteDataToComm(cmd_data);
		if(IsSendSuccess(id))
		{
			SendResult = true;
			break;
		}
		times--;
	}
	return SendResult;
}

bool CThreadSMManager::EventDelVehQueueResult(const char* ParamContext, int iParamSize)
{
	unsigned short id = NextCMDId();
	std::string cmd_data = PackDataEventDelVehQueueResult(ParamContext, iParamSize, id);

	bool SendResult = false;
	int times = 3;
	while(times)
	{		
		WriteDataToComm(cmd_data);
		if(IsSendSuccess(id))
		{
			SendResult = true;
			break;
		}
		times--;
	}
	return SendResult;
}

bool CThreadSMManager::EventWebsocketControl(const char* ParamContext, int iParamSize)
{
	unsigned short id = NextCMDId();
	std::string cmd_data = PackDataEventDelVehQueueResult(ParamContext, iParamSize, id);
	
	bool SendResult = false;
	int times = 3;
	while(times)
	{		
		WriteDataToComm(cmd_data);
		if(IsSendSuccess(id))
		{
			SendResult = true;
			break;
		}
		times--;
	}
	return SendResult;
}

bool CThreadSMManager::EventStartScan()
{
	unsigned short id = NextCMDId();
	int iFeeValue = 0;
	std::string cmd_data = PackDataEventStartScan(iFeeValue, id);
	
	bool SendResult = false;
	int times = 3;
	while(times)
	{		
		WriteDataToComm(cmd_data);
		if(IsSendSuccess(id))
		{
			SendResult = true;
			break;
		}
		times--;
	}
	return SendResult;
}

bool CThreadSMManager::EventStopScan()
{
	unsigned short id = NextCMDId();
	std::string cmd_data = PackDataEventStopScan(id);
	
	bool SendResult = false;
	int times = 3;
	while(times)
	{		
		WriteDataToComm(cmd_data);
		if(IsSendSuccess(id))
		{
			SendResult = true;
			break;
		}
		times--;
	}
	return SendResult;
}


bool CThreadSMManager::QRcodeDisplay(const char* ParamContext, int iParamSize)
{
	unsigned short id = NextCMDId();
	std::string cmd_data = PackDataQRcodeDisplay(ParamContext, iParamSize, id);

	bool SendResult = false;
	int times = 3;
	while(times)
	{		
		WriteDataToComm(cmd_data);
		if(IsSendSuccess(id))
		{
			SendResult = true;
			break;
		}
		times--;
	}
	return SendResult;
}

bool CThreadSMManager::FeeDisplay(const char* ParamContext, int iParamSize)
{
	unsigned short id = NextCMDId();
	std::string cmd_data = PackDataFeeDisplay(ParamContext, iParamSize, id);

	bool SendResult = false;
	int times = 3;
	while(times)
	{		
		WriteDataToComm(cmd_data);
		if(IsSendSuccess(id))
		{
			SendResult = true;
			break;
		}
		times--;
	}
	return SendResult;
}

bool CThreadSMManager::PayResultDisplay(const char* ParamContext, int iParamSize)
{
	unsigned short id = NextCMDId();
	std::string cmd_data = PackDataPayResultDisplay(ParamContext, iParamSize, id);

	bool SendResult = false;
	int times = 3;
	while(times)
	{		
		WriteDataToComm(cmd_data);
		if(IsSendSuccess(id))
		{
			SendResult = true;
			break;
		}
		times--;
	}
	return SendResult;
}

bool CThreadSMManager::ClearScreen(const char* ParamContext, int iParamSize)
{
	unsigned short id = NextCMDId();
	std::string cmd_data = PackDataClearScreen(ParamContext, iParamSize, id);

	bool SendResult = false;
	int times = 3;
	while(times)
	{		
		WriteDataToComm(cmd_data);
		if(IsSendSuccess(id))
		{
			SendResult = true;
			break;
		}
		times--;
	}
	return SendResult;
}


/******************************************************************************/
unsigned short CThreadSMManager::NextCMDId()
{
	CScopeLock lock(&m_driverMutex);
	m_driver.next_send_id = m_driver.next_send_id++ % (1 << 15);
	return m_driver.next_send_id;
}

bool CThreadSMManager::WriteDataToComm( const std::string& hex_data)
{
	if (hex_data.size() % 2 != 0)
	{
		LogCallBack("д����ʧ�ܣ���ʽ����," + hex_data);
		return false;
	}
	DWORD byte_len = static_cast<DWORD>(hex_data.size() / 2);
	if (byte_len > COMDATA_MAXLEN)
	{
		theApp.RecordMsgTemp("д����ʧ�ܣ����ݹ���");
		return false;
	}

	BYTE bytes[COMDATA_MAXLEN] = {0};
	theApp.Hex2Bin(hex_data.c_str(), bytes, byte_len << 1);
	WriteDataToComm(bytes, byte_len);

	return true;
}


bool CThreadSMManager::IsSendSuccess(unsigned short id)
{
	for (int i = 0; i < 100; ++i)
	{
		{  
			CScopeLock lock(&m_driverMutex);
			if (CheckTargetACK(m_driver, id))
			{
				return true;
			}
		}
		Sleep(10);
	}

	theApp.RecordMsgTemp(theApp.FmtStr(_T("��������[%X]����ʧ��, δ�յ����ڵ�C0Ӧ��!")), id);
	theApp.m_strLastErrorDesc = "�������ݷ���ʧ��, δ�յ����ڵ�C0Ӧ��!";
	return false;
}

void CThreadSMManager::CheckTerminalAlive()
{
	{
		CScopeLock lock(&m_driverMutex);
		if (!m_driver.initdev_flag)
		{
			theApp.RecordMsgTemp("����A5��ʼ����Ϣ...");
			unsigned short id = NextCMDId();
			std::string cmd_data = PackDataInitTerminal(id);
			WriteDataToComm(cmd_data);
			m_driver.initdev_flag = true;
		}
	}

	time_t diff = time(NULL) - m_tLastComReadDataTime;
	if (diff < 120)
	{
		return ;
	}
	m_tLastComReadDataTime = time(NULL);

	theApp.RecordMsgTemp(theApp.FmtStr(_T("[ɨ�빤���߳�],����[�˿�:%d]״̬�����쳣���޶�ʱ����δ�ɹ�[�յ�/����]�κ����ݣ�����Ҫ��������"), m_sLocalCOM.Port));
	CloseCom();
	m_DevStatus = 4; //Ӳ��ͨѶ���ϣ����ظ��������
	Sleep(500);
	OpenCom();

	if (!m_bOpen)
	{
		theApp.RecordMsgTemp(theApp.FmtStr(_T("[ɨ�빤���߳�],����[�˿�:%d]��ʧ��"), m_sLocalCOM.Port));
	}
}

bool CThreadSMManager::CheckDeviceNo()
{
	static time_t tLastTime = time(NULL);
	time_t diff = time(NULL) - tLastTime;
	if ((diff < 5))
	{
		return false;
	}
	tLastTime = time(NULL);

	if (!m_bOpen)
	{
		return false;
	}

	if (!theApp.m_DeviceNo.empty())
	{
		return true;
	}


	unsigned short id = NextCMDId();
	std::string cmd_data = PackDataDeviceNo("", 0, id);

	bool SendResult = false;
	int times = 1;
	while(times)
	{		
		WriteDataToComm(cmd_data);
		//if(IsSendSuccess(id))
		{
			SendResult = true;
			break;
		}
		times--;
	}
	return SendResult;
}
