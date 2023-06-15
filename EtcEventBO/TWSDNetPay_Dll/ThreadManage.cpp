/*
 * Copyright(C) 2016,SQUIRREL ���˹����� ��������Ȩ����( All rights reserved. )
 *
 * �ļ����ƣ�ThreadManage.cpp
 * ժ    Ҫ��ҵ���̨�����̣߳�ʵ���ļ�
 * ��ǰ�汾��1.0.0.1
 * ��    �ߣ�ROCY
 * �������ڣ�2016��7��24��
 */
// ThreadManage.cpp : implementation file
//

#include "stdafx.h"
#include "TWSDNetPay_Dll.h"
#include "ThreadManage.h"
#include "FileFindExt.h"
#include "XMLHelper.h"
#include "JsonHelper.h"
#include <sstream>
#include <fstream>
#include <iostream>

// CThreadManage

IMPLEMENT_DYNCREATE(CThreadManage, CThreadBase)

CThreadManage::CThreadManage()
	: m_pThreadHttp(NULL)
	, m_pThreadDevice(NULL)
	, m_pThreadUpdate(NULL)
	, m_pThreadNFCSignIn(NULL)
	, m_pDVJsonDebitCheck(NULL)
	, m_bAccountInfoIsReady(FALSE)
	, m_bDeviceLoadState(FALSE)
	, m_nAccuracy(0)
	, m_nTimerIDForScanCode(0)
	, m_nTimerIDForDebitMoney(0)
	, m_nTimerIDForCheckDebitState(0)
	, m_nTimerIDForNFCGetCorrentData(0)
	, m_nTimerIDForNFCGetNormalData(0)
	, m_dwCheckDebitSpanTime(0)
	, m_dwCheckDebitAccumulateTime(0)
	, m_dwInputPasswordDelayTime(30 * 1000)
{
	::InitializeCriticalSection(&idleMutex);
}

CThreadManage::~CThreadManage()
{
}


BOOL CThreadManage::RestartWindows()
{
	HANDLE   hToken;
	TOKEN_PRIVILEGES   tkp;
	//��ȡϵͳ��Ϣ
	OSVERSIONINFO   osvi;
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	if( GetVersionEx( &osvi ) == 0 ) return FALSE;

	if( !OpenProcessToken( GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY, &hToken ) ) return FALSE;
	LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);
	tkp.PrivilegeCount = 1;     //   one   privilege   to   set
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	AdjustTokenPrivileges( hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL,0);
	ExitWindowsEx(EWX_REBOOT|EWX_FORCEIFHUNG,   0);
	return TRUE;
}

void CThreadManage::MoveConfigFileToExeDir()
{

	bool bhadIni= false, bHadtxt= false;
	std::string updateDir = fs_PathJoin(fs_thisModulePath(),"TWUpdate"); //��ֹɾ�����ļ��У����¿⻹������

	Search_file_info sfi;
	strncpy(sfi.root_catalog, updateDir.c_str(), 200);


	{
		strcpy(sfi.pattern, "*.ini");
		sfi.enable_wildcard = true;
		std::list<std::string> extract_files = Search_files(sfi);
		for(std::list<std::string>::const_iterator it = extract_files.begin();it!=extract_files.end();++it)
		{
			Move_file_info mfi;
			strncpy(mfi.src,it->c_str(),200);
			if(!Move_file_to_target_catalog(mfi,fs_thisModulePath()))
			{
				TW_TRACE_DIARY_TYPE("Move extract file error !", 12);
				break;
			}
			bhadIni = true;
		}
	}

	{
		strcpy(sfi.pattern, "*.txt");
		sfi.enable_wildcard = true;

		std::list<std::string> extract_files = Search_files(sfi);
		for(std::list<std::string>::const_iterator it = extract_files.begin();it!=extract_files.end();++it)
		{
			Move_file_info mfi;
			strncpy(mfi.src,it->c_str(),200);
			if(!Move_file_to_target_catalog(mfi,fs_thisModulePath()))
			{
				TW_TRACE_DIARY_TYPE("Move extract file error !", 12);
				break;
			}
			bHadtxt = true;
		}

	}

	if (bhadIni || bHadtxt)
	{
		m_bReBootSystem = true;
	}

	return;
}

//ҵ������֮�����ڿ�ܿ�͸��¿ⶼ�޷����и��£����صĸ��¿�Ϳ�ܻ���ԭ���ģ�ֻ����ҵ����д���һ��----20211126 lhq
void CThreadManage::modifyUpdateflagResult()
{
	std::string flagFilePath = fs_PathJoin(fs_thisModulePath(),"updateflag.txt"); //��ֹɾ�����ļ��У����¿⻹������
	std::ifstream flagFile(flagFilePath.c_str());
	if (!flagFile.is_open())
	{
		return ;
	}
	char chs[2000] = {0};
	flagFile.getline(chs, 1000);

	if (chs[0] == '\0')
	{
		TW_TRACE_DIARY_TYPE("update result file read fail:" + flagFilePath, 12);
		flagFile.close();
		return ;
	}
	std::string update_str =  std::string(chs);
	flagFile.close();

	Json::Value rootN;
	Json::Reader  reader;
	if (!reader.parse(update_str.c_str(), rootN))
	{
		TW_TRACE_DIARY_TYPE("update result json read fail:" + update_str, 12);
		return ;
	}

	rootN["result"] =3; //��ʾȷ�ϳɹ�

	Json::FastWriter jfw;
	std::string code_str = jfw.write(rootN);

	std::ofstream file_tmp;
	file_tmp.open(flagFilePath.c_str());
	file_tmp.write(code_str.c_str(),code_str.size());
	file_tmp.close();

	TW_TRACE_DIARY_TYPE("CThreadManage::modifyUpdateflagResult()! ����updateflag.txt�ļ���resultΪ��3", 12);


	//if ( rootN["code"].type() != Json::stringValue)
	//{
	//	TW_TRACE_DIARY_TYPE("update result json[code] read fail:" + update_str, 12);
	//	return 0;
	//}
	//std::string code_str = rootN["code"].asString();

	//if ( rootN["result"].type() != Json::intValue)
	//{
	//	TW_TRACE_DIARY_TYPE("update result json[result] read fail:" + update_str, TW_DIARY_MSG_THREAD_API);
	//	return 0;
	//}

}


BOOL CThreadManage::InitInstance()
{

	MoveConfigFileToExeDir();

	modifyUpdateflagResult();
	
	////�����Ҫ�豸��һ����£���Ҫ����ϵͳ�����ֳ��Ŀ�ܿⲻ�ܻ�ԭ�豸�⣬������Ҫ���µ�ҵ�������ʱ�����ϼ����豸�⣬ ----- 2021-12-05 
	////if (m_bReBootSystem)
	////{
	////	RestartWindows();
	////	m_bReBootSystem= false;
	////}

	StartWorkThreads(m_sTranstParam);
	return TRUE;
}

int CThreadManage::ExitInstance()
{

	//�رռ�ʱ��
	if (0 != m_nTimerIDForScanCode)
	{
		timeKillEvent(m_nTimerIDForScanCode);
		m_nTimerIDForScanCode = 0;
	}
	if (0 != m_nTimerIDForDebitMoney)
	{
		timeKillEvent(m_nTimerIDForDebitMoney);
		m_nTimerIDForDebitMoney = 0;
	}
	if (0 != m_nTimerIDForCheckDebitState)
	{
		timeKillEvent(m_nTimerIDForCheckDebitState);
		m_nTimerIDForCheckDebitState = 0;
	}
	if (0 != m_nTimerIDForNFCGetCorrentData)
	{
		timeKillEvent(m_nTimerIDForNFCGetCorrentData);
		m_nTimerIDForNFCGetCorrentData = 0;
	}
	if (0 != m_nTimerIDForNFCGetNormalData)
	{
		timeKillEvent(m_nTimerIDForNFCGetNormalData);
		m_nTimerIDForNFCGetNormalData = 0;
	}
	//�����С��ʱ������
	timeEndPeriod(m_nAccuracy);
	/*
	goDie = true;
	while (true)
	{
		if (goDie && dieNumber == 2017)
		{
			break;
		}
		Sleep(50);
	}
	//MessageBox(0,"main over ExitInstance","",0);

	fnReleaseResource();
	*/
	::DeleteCriticalSection(&idleMutex);
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CThreadManage, CThreadBase)
	ON_THREAD_MESSAGE(WM_THREAD_DEVICE_START_GET_DATA, OnMsgStartGetAccountInfo)
	ON_THREAD_MESSAGE(WM_THREAD_DEVICE_STOP_GET_DATA, OnMsgStopGetAccountInfo)
	ON_THREAD_MESSAGE(WM_THREAD_DEVICE_NOTIFY, OnMsgReceiveAccountInfo)
	ON_THREAD_MESSAGE(WM_THREAD_MANAGE_DEAL_PLATFORM_RESPONSE_RESULT, OnMsgDealSubPayPlatformResponse)
	ON_THREAD_MESSAGE(WM_THREAD_MANAGE_CHECK_DEBIT_PASSWORD_STATE, OnMsgCheckDebitPasswordState)
	ON_THREAD_MESSAGE(WM_THREAD_MANAGE_ONLY_DEBIT_MONEY, OnMsgOnlyDebitMoney)
	ON_THREAD_MESSAGE(WM_THREAD_MANAGE_START_DEBIT_FLOW, OnMsgManageStartDebitFlow)
	ON_THREAD_MESSAGE(WM_THREAD_MANAGE_DEBIT_CANCEL, OnMsgDebitCancel)
	ON_THREAD_MESSAGE(WM_THREAD_NFC_SIGN_DATA_RETURN_FROM_HTTP, OnMsgRevNFCSignDataFromHTTP)
	ON_THREAD_MESSAGE(WM_THREAD_NFC_START_SIGN, OnMsgNFCStartSignInFlow)

	ON_THREAD_MESSAGE(WM_THREAD_REMOTE_CONTROL_FIRMWARE, OnMsgRemoteControlFirmware)

	ON_THREAD_MESSAGE(WM_THREAD_REMOTE_UPRGADE_RESULT,OnMsgRemoteUpgradeResult)

	ON_THREAD_MESSAGE(WM_THREAD_LPR_IMAGEID,OnMsgQueryLPRImageId)

	
END_MESSAGE_MAP()

////////////////////////////////////////////////////////////////////////////////////////////////
// �������ƣ�CThreadManage::OnIdle
//
// �����������߳̿��д�������
//
// ���������LONG lCount
// ���������BOOL
// ��д��Ա��ROCY
// ��дʱ�䣺2016��7��24��
// �޸���Ա��
// �޸�ʱ�䣺
// �����汾��1.0.0.1
// ��ע˵����
//
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CThreadManage::OnIdle(LONG lCount)
{
    CScopeLock tmpIdle(&idleMutex);
	if (m_bExit)
	{
		//������Դ
		//	fnReleaseResource();

		//�Ա��߳��׳��˳���Ϣ
		::PostQuitMessage(0);

		return FALSE;
	}


	//��������������(10����һ��)
	static time_t tLastCheckDeviceLoadState = time(NULL);
	if ( (time(NULL) - tLastCheckDeviceLoadState) > 10)
	{
		//GetGlobalApp()->RecordLog("manage check state",LOG_LEVEL_ERROR,TRUE);

		if (NULL != m_pThreadDevice)
		{
			m_bDeviceLoadState = m_pThreadDevice->GetDeviceInitState();
		}

		tLastCheckDeviceLoadState = time(NULL);
	}


	CString strLog;
	//�̴߳����־��¼���
	static time_t tLastManageLogTime = 0;
	if ( (time(NULL) - tLastManageLogTime) > GetGlobalApp()->m_dwThreadCheckSpanTime)
	{
		strLog.Format(_T("[ҵ������߳�],�̴߳����־���"));
		GetGlobalApp()->RecordLog(strLog);
		//������ʱ��
		tLastManageLogTime = time(NULL);
	}

	//����Ƿ���Ҫɾ����־(�޶�ʱ���ִ�иò���)
	CTime cCurrentTime = CTime::GetCurrentTime();
	if ( (cCurrentTime.GetHour() >= ::GetGlobalApp()->m_iDeleteLogStartHour ) && (cCurrentTime.GetHour() <= ::GetGlobalApp()->m_iDeleteLogEndHour) )
	{
		//ÿ��ָ��ʱ�����һ���Ƿ���Ҫɾ����־
		static time_t tLastDeleteLogTime = 0;
		if ( (time(NULL) - tLastDeleteLogTime) > GetGlobalApp()->m_iDeleteLogSpanTime)
		{
			//������ɾ����־
			CheckAndDeleteLogFile();
			//������ʱ��
			tLastDeleteLogTime = time(NULL);
		}
	}

	::Sleep(10);


	return CThreadBase::OnIdle(lCount);
}


void CThreadManage::ThreadQuit()
{	// MessageBox(0,"wait ...","",0);
	//�رռ�ʱ��
	if (0 != m_nTimerIDForScanCode)
	{
		//MessageBox(0,"debug","",0);
		timeKillEvent(m_nTimerIDForScanCode);
		m_nTimerIDForScanCode = 0;
	}

	if (0 != m_nTimerIDForDebitMoney)
	{
		timeKillEvent(m_nTimerIDForDebitMoney);
		m_nTimerIDForDebitMoney = 0;
	}
	if (0 != m_nTimerIDForCheckDebitState)
	{
		timeKillEvent(m_nTimerIDForCheckDebitState);
		m_nTimerIDForCheckDebitState = 0;
	}
	if (0 != m_nTimerIDForNFCGetCorrentData)
	{
		timeKillEvent(m_nTimerIDForNFCGetCorrentData);
		m_nTimerIDForNFCGetCorrentData = 0;
	}
	if (0 != m_nTimerIDForNFCGetNormalData)
	{
		timeKillEvent(m_nTimerIDForNFCGetNormalData);
		m_nTimerIDForNFCGetNormalData = 0;
	}

	//�����С��ʱ������
	timeEndPeriod(m_nAccuracy);

	{
        if (this->m_bExit)                                              
        {
            return ;
        }
		//GetGlobalApp()->RecordLog("before exit manage",LOG_LEVEL_ERROR,TRUE);
        CScopeLock tmpIdle(&idleMutex);
		fnReleaseResource();
        this->m_bExit = true;
        
    }

	//MessageBox(0, "main over ThreadQuit", "", 0);
	//::PostThreadMessage(this->m_nThreadID, WM_QUIT, 0, 0);

	//������Դ
	


	m_bExit = TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// �������ƣ�CThreadManage::OnMsgStartGetAccountInfo
//
// ��������������Ҫ��ִ��������Ӳ����ȡ�û���Ϣ�Ĳ���
//
// ���������WPARAM wParam
//         ��LPARAM lParam
// ���������void
// ��д��Ա��ROCY
// ��дʱ�䣺2016��7��25��
// �޸���Ա��
// �޸�ʱ�䣺
// �����汾��1.0.0.1
// ��ע˵������Ӧ��Ϣ WM_THREAD_DEVICE_START_GET_DATA
//
////////////////////////////////////////////////////////////////////////////////////////////////
void CThreadManage::OnMsgStartGetAccountInfo(WPARAM wParam, LPARAM lParam)
{
	int iOverTime = (int)wParam;
	if (NULL != m_pThreadDevice)
	{
		m_pThreadDevice->PostThreadMessage(WM_THREAD_DEVICE_START_GET_DATA, (WPARAM)0, (LPARAM)0);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////
// �������ƣ�CThreadManage::OnMsgStartGetAccountInfo
//
// ��������������Ҫ��ִ��ֹͣ��Ӳ����ȡ�û���Ϣ�Ĳ���
//
// ���������WPARAM wParam
//         ��LPARAM lParam
// ���������void
// ��д��Ա��ROCY
// ��дʱ�䣺2016��7��25��
// �޸���Ա��
// �޸�ʱ�䣺
// �����汾��1.0.0.1
// ��ע˵������Ӧ��Ϣ WM_THREAD_DEVICE_STOP_GET_DATA
//
////////////////////////////////////////////////////////////////////////////////////////////////
void CThreadManage::OnMsgStopGetAccountInfo(WPARAM wParam, LPARAM lParam)
{
	if (NULL != m_pThreadDevice)
	{
		m_pThreadDevice->PostThreadMessage(WM_THREAD_DEVICE_STOP_GET_DATA, (WPARAM)0, (LPARAM)0);
		//�����ʱ��
		StopTimer(TIMER_ID_DEBIT_CHECK);
		StopTimer(TIMER_ID_DEBIT_MONEY);
		//ֹͣ���մ���֧����ƽ̨����������
		GetGlobalApp()->m_bIsTrading = FALSE;
		//�����ۿ������Դ
		GetGlobalApp()->m_pCurrentDebitInfo.Valid	= FALSE;
		GetGlobalApp()->m_sCurrentDebitResult.Valid = FALSE;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////
// �������ƣ�CThreadManage::OnMsgGetAccountInfo
//
// ������������ȡ���û���Ϣ��Ĵ�������
//
// ���������WPARAM wParam
//         ��LPARAM lParam
// ���������void
// ��д��Ա��ROCY
// ��дʱ�䣺2016��7��24��
// �޸���Ա��
// �޸�ʱ�䣺
// �����汾��1.0.0.1
// ��ע˵������Ӧ��Ϣ WM_THREAD_DEVICE_NOTIFY
//
////////////////////////////////////////////////////////////////////////////////////////////////
void CThreadManage::OnMsgReceiveAccountInfo(WPARAM wParam, LPARAM lParam)
{
	BOOL bGetData	= (BOOL)wParam;
	DWORD dwParam	= 0x00;
	BYTE byFunCode = 0x03;
	BYTE byExecuteState = 0x00;
	BYTE bySrcType = 0x00;
	tagAccountInfo* pAccountInfo	= (tagAccountInfo*)lParam;
	try
	{
		//��ȡ�û���Ϣ
		if (NULL == pAccountInfo)
			throw - 1;
		if ( 0 == pAccountInfo->DataSize )
			throw - 2;

		//NFC�ǽ�ģ��
		bySrcType = pAccountInfo->SrcType;
		if (0x02 == bySrcType)
		{
			if (::GetGlobalApp()->m_bNFCIsUse)
			{
				//ֹͣ��ʱ��
				StopTimer(TIMER_ID_NFC_WAIT_NORMAL_DATA);
				//��������֡
				int iFrameType = UFT_UNKNOW;
				CNFCProtocol cNFCProtocol;
				iFrameType = cNFCProtocol.Analyze(pAccountInfo->DataContext);

				
				/*
				char debug_chs[1000]={0};
				sprintf(debug_chs,"frameType:%d��%s",iFrameType,pAccountInfo->DataContext);
				theApp.RecordLog(debug_chs);
				*/

				switch (iFrameType)
				{
				//ǩ�����
				case UFT_N_SIGN_IN_RES_DATA:
				case UFT_N_SIGN_VERIFY_ERR:
				case UFT_N_SIGN_CONFIRM:
					NFCSignInDataProcessFromPOS(cNFCProtocol);
					break;
				//�������
				case UFT_N_CONSUME_RES_W2:
				case UFT_N_CONSUME_RES_W3:
				case UFT_N_CONSUME_TIP:
				case UFT_N_CONSUME_RES_DATA:
				case UFT_N_CONSUME_ERR:
				case UFT_N_CONSUME_NO_SIGN:
					NFCConsumeDataProcessFromPOS(cNFCProtocol);
					break;
				//�������
				case UFT_N_CORRECT_RES_DATA:
					NFCCorrectDataProcessFromPOS(cNFCProtocol);
					break;
				case UFT_N_TEST_LINK_SUCCESS:
					GetGlobalApp()->m_NfcLinkSuccess = true;//add 2018-04
				case UFT_N_TEST_LINK_ERROR:
					NFCLinkTestDataProcessFromPOS(cNFCProtocol);
					break;
				case UFT_ACK:
					break;
				
				//����
				default:
					NFCProcessErrorFrame(cNFCProtocol);
					break;
				}
			}
		}
		else//Ĭ�ϵ�ɨ������
		{
			//���ϲ�����ֱ����֧����ƽ̨������Ϣ
			if (GetGlobalApp()->m_bCurrentDebitMode)
			{
				//�жϵ�ǰ�����Ƿ��������пۿ�ҵ����
				if (!GetGlobalApp()->m_bIsTrading)
					throw - 3;
				//��ǿۿ����̽��յ�֧��ƾ֤
				GetGlobalApp()->m_iDebitTaskNo = 0x02;
				//�����������ݲ�֪֧ͨ����ƽ̨ͨѶ�߳̿�ʼ����
				ContructDebitMoneyInfo(pAccountInfo);

				m_PayCenter.SetCertificate(LPCSTR(pAccountInfo->DataContext),"","");

				//�������������֧�������ڴ���Ҫֹͣ��������
				if (GetGlobalApp()->m_bNFCIsUse)
				{
					//��Ĭ100����
					::Sleep(100);
					//����ָ��
					GetGlobalApp()->SetUnipayWorkFlowState(UMS_STOP);
					NFCSendDefaultCmdToPos(3);
				}
			}
			else//��������������û�����
			{
				//�����û���Ϣ
				memcpy(&m_sAccountInfo, pAccountInfo, sizeof(tagAccountInfo));
				SetAccountDataReadyState(TRUE);
				//֪ͨ�������û������Ѿ�׼������
				byExecuteState = 0x00;
				dwParam = MAKELONG(MAKEWORD(byFunCode, byExecuteState), MAKEWORD(0x00, 0x00));
				//��¼��־
				GetGlobalApp()->RecordLog(GetGlobalApp()->FmtStr(_T("[ҵ������߳�],��Ӧ����Ӳ�������̻߳����������(��ȡ�û���Ϣ)����ʽ�����(���ܴ���:%d,ִ��״��:%d)"), 0x03, 0x00));
			}
		}
	}
	catch (int& iErrorCode)
	{
		switch (iErrorCode)
		{
		case -1:
		case -2:
			break;
		}
		//֪ͨ�������û����ݻ�ȡʧ��
		byExecuteState = 0x05;
		dwParam = MAKELONG(MAKEWORD(byFunCode, byExecuteState), MAKEWORD(0x00, 0x00));
		//��¼��־
		GetGlobalApp()->RecordLog(GetGlobalApp()->FmtStr(_T("[ҵ������߳�],��Ӧ����Ӳ�������̻߳����������(��ȡ�û���Ϣ)����ʽ�����(���ܴ���:%d,ִ��״��:%d)"), 0x03, 0x05));
	}

	//������Դ
	if (NULL != pAccountInfo)
	{
		delete pAccountInfo;
		pAccountInfo = NULL;
	}
	//֪ͨ������
	if (0x02 != bySrcType)
	{
		//GetGlobalApp()->RecordLog("befoe src notify");
		::GetGlobalApp()->NotifyCaller(byFunCode, byExecuteState);
		//GetGlobalApp()->RecordLog("after src notify");
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////
// �������ƣ�CThreadManage::OnMsgStartDebitFlow
//
// �������������յ���Ϣ�������ۿ����̣����ϲ���ģʽ��
//
// ���������WPARAM wParam
//         ��LPARAM lParam
// ���������void
// ��д��Ա��ROCY
// ��дʱ�䣺2016��8��5��
// �޸���Ա��
// �޸�ʱ�䣺
// �����汾��1.0.0.1
// ��ע˵����WM_THREAD_MANAGE_START_DEBIT_FLOW
//
////////////////////////////////////////////////////////////////////////////////////////////////
void CThreadManage::OnMsgManageStartDebitFlow(WPARAM wParam, LPARAM lParam)
{
	CString strLog;
	try
	{
		//��ǿۿ����̿�ʼ
		GetGlobalApp()->m_iDebitTaskNo = 0x01;

		CString* pstrXML = (CString*)wParam;
		if (NULL == pstrXML)
			throw - 1;
		if (NULL == m_pThreadDevice)
			throw - 2;
		//�����һ�ε��û���Ϣ
		memset(&m_sAccountInfo, 0, sizeof(tagAccountInfo));
		//֪ͨӲ�������߳�
		m_pThreadDevice->PostThreadMessage(WM_THREAD_DEVICE_START_DEBIT_FLOW, (WPARAM)wParam, (LPARAM)0);
		//��¼��־
		if (GetGlobalApp()->m_bNFCIsUse)
		{
			strLog.Format(_T("[ҵ������߳�],�ⲿ�����������Ͽۿ����(֧��ɨ��֧���ͷǽ�֧��)�����ױ�ʶ:%s"), GetGlobalApp()->m_pCurrentDebitInfo.PayIdentifier);
		}
		else
		{
			strLog.Format(_T("[ҵ������߳�],�ⲿ�����������Ͽۿ����(��֧��ɨ��֧��)�����ױ�ʶ:%s"), GetGlobalApp()->m_pCurrentDebitInfo.PayIdentifier);
		}
		GetGlobalApp()->RecordLog(strLog);
	}
	catch (int& iErrorCode)
	{
		switch (iErrorCode)
		{
		case -1:
			break;
		case -2:
			break;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////
// �������ƣ�CThreadManage::OnMsgOnlyDebitMoney
//
// ����������ִ�пۿ����
//
// ���������WPARAM wParam
//         ��LPARAM lParam
// ���������void
// ��д��Ա��ROCY
// ��дʱ�䣺2016��8��3��
// �޸���Ա��
// �޸�ʱ�䣺
// �����汾��1.0.0.1
// ��ע˵������Ӧ��Ϣ WM_THREAD_MANAGE_ONLY_DEBIT_MONEY
//
////////////////////////////////////////////////////////////////////////////////////////////////
void CThreadManage::OnMsgOnlyDebitMoney(WPARAM wParam, LPARAM lParam)
{
	CString strLog;
	try
	{
		//�����������ݲ�֪֧ͨ����ƽ̨ͨѶ�߳̿�ʼ����
		if (!ContructDebitMoneyInfo())
			throw - 1;
		//��¼��־
		GetGlobalApp()->RecordLog(GetGlobalApp()->FmtStr(_T("[ҵ������߳�]:�ⲿ�������ÿۿ����(�������)�����ױ�ʶ:%s"), GetGlobalApp()->m_pCurrentDebitInfo.PayIdentifier));
	}
	catch (int& iErrorCode)
	{
		switch (iErrorCode)
		{
		case -1:
			strLog.Format(_T("[ҵ������߳�]:�ⲿ�������ÿۿ����(�������),���첢���Ϳۿ����ݲ���ʧ��"));
			break;
		}
		//������־
		if (strLog != m_strLastErrorDesc)
		{
			m_strLastErrorDesc = strLog;
			GetGlobalApp()->RecordLog(m_strLastErrorDesc, LOG_LEVEL_ERROR);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////
// �������ƣ�CThreadManage::OnMsgDebitCancel
//
// ����������ִ�г�������
//
// ���������WPARAM wParam
//         ��LPARAM lParam
// ���������void
// ��д��Ա��ROCY
// ��дʱ�䣺2016��8��3��
// �޸���Ա��
// �޸�ʱ�䣺
// �����汾��1.0.0.1
// ��ע˵������Ӧ��Ϣ WM_THREAD_MANAGE_DEBIT_CANCEL
//
////////////////////////////////////////////////////////////////////////////////////////////////
void CThreadManage::OnMsgDebitCancel(WPARAM wParam, LPARAM lParam)
{
	CString strLog;
	CString* pstrPayIdentifier = (CString*)wParam;
	try
	{
		if (NULL == pstrPayIdentifier)
			throw - 1;
		//�رռ��ۿ�״̬��ʱ��
		StopTimer(TIMER_ID_DEBIT_MONEY);
		//�رտۿʱ��ⶨʱ��
		StopTimer(TIMER_ID_DEBIT_CHECK);
		//���ÿۿ�ģʽ
		//GetGlobalApp()->m_bCurrentDebitMode = FALSE;//remove 2018-10 zhuyabing 
		//��ʶ���״������
		GetGlobalApp()->m_bIsTrading = FALSE;
		//����ۿ�״̬��ѯ�ṹ��
		if (NULL == m_pDVJsonDebitCheck)
		{
			delete m_pDVJsonDebitCheck;
			m_pDVJsonDebitCheck = NULL;
		}
		m_sDVJsonDebit.Reset();
		//��ʶ�ۿ����̽���
		if (GetGlobalApp()->m_bCurrentDebitMode)
		{
			//�ر�ɨ����
			if (0x01 == GetGlobalApp()->m_iDebitTaskNo)
			{
				if (NULL != m_pThreadDevice)
				{
					m_pThreadDevice->PostThreadMessage(WM_THREAD_DEVICE_STOP_GET_DATA, (WPARAM)0, (LPARAM)0);
				}
			}
			GetGlobalApp()->m_iDebitTaskNo = 0x00;
		}
		GetGlobalApp()->m_bCurrentDebitMode = FALSE;//add 2018-10 zhuyabing

		m_pThreadHttp->m_bLoopBreak = TRUE;	//��Ҫ�����۷�ִ��ѭ��

		//���쳷�����ݲ�֪֧ͨ����ƽ̨ͨѶ�߳̿�ʼ����
		CString strPayIdentifier;
		strPayIdentifier.Format(_T("%s"), *pstrPayIdentifier);
		if (!ContructDebitCancelInfo(strPayIdentifier))
			throw - 2;
		//��¼��־
		GetGlobalApp()->RecordLog(GetGlobalApp()->FmtStr(_T("[ҵ������߳�]:�ⲿ�������ó������������ױ�ʶ:%s"), strPayIdentifier));
	}
	catch (int& iErrorCode)
	{
		switch (iErrorCode)
		{
		case -1:
			strLog.Format(_T("[ҵ������߳�]:�ⲿ�������ó����ӿ�,�������(����ʶ����)ָ��Ϊ��"));
			break;
		case -2:
			strLog.Format(_T("[ҵ������߳�]:�ⲿ�������ó����ӿ�,���첢���ͳ������ݲ���ʧ��"));
			break;
		}
		//������־
		if (strLog != m_strLastErrorDesc)
		{
			m_strLastErrorDesc = strLog;
			GetGlobalApp()->RecordLog(m_strLastErrorDesc, LOG_LEVEL_ERROR);
		}
	}
	//������Դ
	if (NULL != pstrPayIdentifier)
	{
		delete pstrPayIdentifier;
		pstrPayIdentifier = NULL;
	}
}

bool CThreadManage::CheckPayResultValidity(const CDVJsonDebit&  payResult)
{
	if(payResult.PayIdentifier!= GetGlobalApp()->m_pCurrentDebitInfo.PayIdentifier)
	{
		return false;
	}
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// �������ƣ�CThreadManage::OnMsgDealSubPayPlatformResponse
//
// ��������������֧����ƽ̨�Ļ�����Ϣ
//
// ���������WPARAM wParam
//         ��LPARAM lParam
// ���������void
// ��д��Ա��ROCY
// ��дʱ�䣺2016��7��24��
// �޸���Ա��
// �޸�ʱ�䣺
// �����汾��1.0.0.1
// ��ע˵������Ӧ��Ϣ WM_THREAD_MANAGE_DEAL_PLATFORM_RESPONSE_RESULT
//
////////////////////////////////////////////////////////////////////////////////////////////////
void CThreadManage::OnMsgDealSubPayPlatformResponse(WPARAM wParam, LPARAM lParam)
{
	CString strLog;
	CString strResponseType;
	int		iMMIDetailType = SM_PRPTOCOL_LIGHT_FAIL;
	//����������ݵ�����
	WORD wOpType  = LOWORD(lParam);
	//��÷����������Ľ��
	WORD wOpState = HIWORD(lParam);

	bool isValidResponse = true;//add 2018-05

	//δ���ڽ��׵��У������ջ���
	if (   (JSON_DT_SEND_DIBIT	    == wOpType)
	        || (JSON_DT_SEND_DIBIT_CHECK == wOpType)
	   )
	{
		/**************************Ϊ�˱�֤����/�ƶ�״̬һ�£������ʱ���ױ�ʾ�Ѿ������ã�����Ҫ����һ�γ�������֤��ǰ���ڱ������Ľ��ױ���ֹ**************************/
		if (!GetGlobalApp()->m_bIsTrading)
		{
			//�����ڷ����������֧���������и����
			if (JSON_DT_SEND_DIBIT == wOpType)
			{
				CDVJsonBase* pDVJsonBaseTemp = (CDVJsonBase*)wParam;
				if (NULL != pDVJsonBaseTemp)
				{
					//����ȡ���Ľ��׽��г�������
					//FinishHadCancelTrade(pDVJsonBaseTemp);
					////��¼��־
					//GetGlobalApp()->RecordLog(GetGlobalApp()->FmtStr(_T("[ҵ������߳�]:�ۿ�/��ѯ���������������ǰ�����ѱ����ã��Զ����ó������������ױ�ʶ:%s"),pDVJsonBaseTemp->PayIdentifier));
					//������Դ
					delete pDVJsonBaseTemp;
					pDVJsonBaseTemp = NULL;
				}
			}
			//ֱ�ӷ��أ����ٴ���
			return;
		}
	}

	//��ʱ����״̬
	BOOL bIsInputPassword	= FALSE;
	BOOL bDebitSuccess		= FALSE;

	CDVJsonBase* pDVJsonBase = (CDVJsonBase*)wParam;

	CString strTip;
	CDVJsonDebit* pDVLocalDebit = new CDVJsonDebit();


	BYTE byFunCode		= 0x01;
	BYTE byExecuteState = RCC_INNER_ERROR;

	try
	{
		//��ȡҵ��������
		switch (wOpType)
		{
		case JSON_DT_SEND_DIBIT:
			strResponseType.Format(_T("�۷�����"));
			break;
		case JSON_DT_SEND_DIBIT_CHECK:
			strResponseType.Format(_T("�۷Ѳ�ѯ"));
			break;
		case JSON_DT_SEND_DIBIT_CANCEL:
			strResponseType.Format(_T("��������"));
			break;
		}

		//��úϷ��Ļ�����Ϣ
		if (HTTP_RC_DATA_LEGAL != wOpState)
			throw - 1;
		//�ж���Ϣ��������
		if (    (JSON_DT_SEND_DIBIT			!= wOpType)
		        && (JSON_DT_SEND_DIBIT_CHECK	!= wOpType)
		        && (JSON_DT_SEND_DIBIT_CANCEL	!= wOpType)
		   )
		{
			throw - 2;
		}

		//У��ָ���Ƿ�Ƿ�
		if ( (NULL == pDVJsonBase) || (NULL == pDVLocalDebit) )
			throw - 3;

		if (JSON_DT_SEND_DIBIT == wOpType)
		{
			*pDVLocalDebit = *((CDVJsonDebit*)pDVJsonBase);
			m_sDVJsonDebit = *pDVLocalDebit;
		}
		else
		{
			*((CDVJsonBase*)pDVLocalDebit) = *pDVJsonBase;
		}

		//ֻ�����ۿ�/��ѯ������������������������
		if (JSON_DT_SEND_DIBIT_CANCEL != wOpType)
		{
			switch (pDVJsonBase->ServerResponseCode)
			{
			case HTTP_RC_DEBIT_NEED_PASSWORD:			//��Ҫ�û���������
				//���������������
				bIsInputPassword	= TRUE;
				byExecuteState		= RCC_INPUT_PASSWORD;
				//��չ����ϸ�ڣ����ֶ����ͬ��ʧ����ʾ��
				iMMIDetailType		= SM_PRPTOCOL_LIGHT_FALI_PSW_INPUT;
				break;
			case HTTP_RC_DEBIT_INSUFFICIENT_BALANCE:	//����
				byExecuteState		= RCC_NO_MONEY;
				//��չ����ϸ�ڣ����ֶ����ͬ��ʧ����ʾ��
				iMMIDetailType		= SM_PRPTOCOL_LIGHT_FALI_BL_ERR;
				break;
			case HTTP_RC_DEBIT_SRC_ILLGAL:				//��ά��Ƿ�
				byExecuteState		= RCC_QRCODE_ILLEGAL;
				//��չ����ϸ�ڣ����ֶ����ͬ��ʧ����ʾ��
				iMMIDetailType		= SM_PRPTOCOL_LIGHT_FAIL_QR_ERR1;
				break;
			case HTTP_RC_DEBIT_CODE_TIMEOUT:			//��ά����Ч�ڳ�ʱ
				byExecuteState		= RCC_QRCODE_OT;
				//��չ����ϸ�ڣ����ֶ����ͬ��ʧ����ʾ��
				iMMIDetailType		= SM_PRPTOCOL_LIGHT_FAIL_QR_ERR2;
				break;
			case HTTP_RC_INPUT_PASSWORD_ERROR:			//�û������������
				byExecuteState		= RCC_PSW_ERROR;
				//��չ����ϸ�ڣ����ֶ����ͬ��ʧ����ʾ��
				iMMIDetailType		= SM_PRPTOCOL_LIGHT_FAIL_PSW_ERR;
				break;
			case HTTP_RC_NO_SERVICES:
				byExecuteState		= RCC_INNER_ERROR;	//ƽ̨��ͣ����
				//��չ����ϸ�ڣ����ֶ����ͬ��ʧ����ʾ��
				iMMIDetailType		= SM_PRPTOCOL_LIGHT_FALI_PL_STOP;
				break;

			case HTTP_RC_NFC_NO_SUPPORT:				//NFC����Ƭ��֧������
				byExecuteState		= RCC_INNER_ERROR;
				//��չ����ϸ�ڣ����ֶ����ͬ��ʧ����ʾ��
				iMMIDetailType		= SM_PRPTOCOL_LIGHT_FALI_NFC_01;
				break;
			case HTTP_RC_NFC_NO_INIT:					//NFC����Ƭδ��ʼ��
				byExecuteState		= RCC_INNER_ERROR;
				//��չ����ϸ�ڣ����ֶ����ͬ��ʧ����ʾ��
				iMMIDetailType		= SM_PRPTOCOL_LIGHT_FALI_NFC_02;
				break;
			case HTTP_RC_NFC_NO_SERVICES:				//NFC������������ͣ����
				byExecuteState		= RCC_INNER_ERROR;
				//��չ����ϸ�ڣ����ֶ����ͬ��ʧ����ʾ��
				iMMIDetailType		= SM_PRPTOCOL_LIGHT_FALI_NFC_03;
				break;


			case HTTP_RC_DEBIT_QUERY_ERROR:				//�۷Ѳ�ѯʧ�ܣ�ѭ��ִ�л�ȡ״̬����ʾ�۷ѽ����У�
				byExecuteState		= RCC_DEBIT_QUERY_ERROR;
				break;
			case HTTP_RC_DEBIT_FAIL:					//֧��ʧ��
				byExecuteState		= RCC_INNER_ERROR;
				break;

			case HTTP_RC_DEBIT_SUCCESS:					//�۷ѳɹ�
				//��ǿۿ�ɹ�
				bDebitSuccess		= TRUE;
				byExecuteState		= RCC_OK;
				break;
			default:									//δ֪�쳣
			{
				//�����쳣
				byExecuteState = RCC_INNER_ERROR;
			}
			break;
			}
		}
	}
	catch (int& iErrorCode)
	{
		switch (iErrorCode)
		{
		case -1:
			if (RCC_PAY_PLATFORM_ERROR == wOpState)
			{
				byExecuteState		= wOpState;
				strLog.Format(_T("[ҵ������߳�],��Ӧ֧����ƽ̨������Ϣ(%s),����״̬:��������������"), strResponseType);
			}
			else if (0xF0 == wOpState)
			{
				byExecuteState		= RCC_DEBIT_OT;
				strLog.Format(_T("[ҵ������߳�],��Ӧ֧����ƽ̨������Ϣ(%s),����״̬:ͨѶ��ʱ"), strResponseType);
			}

			else
			{
				strLog.Format(_T("[ҵ������߳�],��Ӧ֧����ƽ̨������Ϣ(%s),����״̬:��������У��Ƿ�"), strResponseType);
				byExecuteState		= RCC_PARAM_ERROR;
			}
			break;
		case -2:
			strLog.Format(_T("[ҵ������߳�],��Ӧ֧����ƽ̨������Ϣ(%s),����״̬:��Ч����Ϣ����"), strResponseType);
			if (RCC_PAY_PLATFORM_ERROR == wOpState)
			{
				byExecuteState     = wOpState;
			}
			else
			{
				byExecuteState		= RCC_PARAM_ERROR;
			}
			break;
		case -3:
			strLog.Format(_T("[ҵ������߳�],��Ӧ֧����ƽ̨������Ϣ(%s),����״̬:�ڲ���Դ�������쳣"), strResponseType);
			byExecuteState     = RCC_INNER_ERROR;
			break;
		}
		GetGlobalApp()->RecordLog(strLog, LOG_LEVEL_ERROR);
		strLog.Empty();
	}

	//�������������߼�����(���������ڴ˽���)
	if (JSON_DT_SEND_DIBIT_CANCEL == wOpType)
	{
		byFunCode		= 0x02;
		//�����ɹ�
		if ((NULL != pDVJsonBase) && (HTTP_RC_DATA_VALID == pDVJsonBase->ServerResponseCode))
		{
			byExecuteState	= RCC_OK;
		}
		//������־
		GetGlobalApp()->RecordLog(GetGlobalApp()->FmtStr(_T("[ҵ������߳�],��Ӧ֧����ƽ̨�����������(��������)�������֪ͨ������(���ܴ���:%d,ִ��״��:%d)"), byFunCode, byExecuteState));
	}
	else
	{
		/*******************************�ۿ���������߼�����*******************************/

		if (bIsInputPassword) //�ۿ�����δ��ȷ
		{
			if ( 0 == theApp.m_iHTTPVersion ) //ԭ�а汾����
			{
				//�û�������������Ĵ���
				if (NULL != pDVLocalDebit)
				{
					if (NULL == m_pDVJsonDebitCheck)
					{
						m_pDVJsonDebitCheck			= new CDVJsonDebitCheck(pDVLocalDebit);
						m_dwCheckDebitSpanTime		= 0;
						m_dwInputPasswordDelayTime	= GetGlobalApp()->m_pCurrentDebitInfo.OverTime * 1000 - (::GetTickCount() - ::GetGlobalApp()->m_dwTradeBeginOvertime);
						if (m_dwInputPasswordDelayTime < 10000)
						{
							m_dwInputPasswordDelayTime					 = 30000;
						}
					}

					//��¼��־
					strLog.Format(_T("[ҵ������߳�],��Ӧ֧����ƽ̨������Ϣ(%s),�û������������룬���пۿ�����ѯ"), strResponseType);
					//����ۿ������ٲ�ѯ
					PostThreadMessage(WM_THREAD_MANAGE_CHECK_DEBIT_PASSWORD_STATE, (WPARAM)0, (LPARAM)lParam);

					//Ҫ���豸����֪ͨ(���ټ��10��)
					static time_t tLastSendInputPSWInfo = 0;
					if ( (time(NULL) - tLastSendInputPSWInfo) > 10)
					{
						m_pThreadDevice->PostThreadMessage(WM_THREAD_DEVICE_CONTROL_DEAL_RESULT_MMI, (WPARAM)iMMIDetailType, (LPARAM)0x00);
						tLastSendInputPSWInfo = time(NULL);
					}
				}
			}
			else//�°汾����������HTTP�߳���ѭ�������ڴ˴���������
			{
			}
		}
		else				//�ۿ����Ѿ�����(ʧ�ܻ�ɹ�)
		{
			//����ۿ�ɹ�������б���ۿ�������
			if (bDebitSuccess)
			{
				GetGlobalApp()->m_sCurrentDebitResult.Valid				= TRUE;

				GetGlobalApp()->m_sCurrentDebitResult.ResultType		= 0x00;
				GetGlobalApp()->m_sCurrentDebitResult.ExecuteCode		= 0x00;
				GetGlobalApp()->m_sCurrentDebitResult.PayPlatformType	= m_sDVJsonDebit.QRCodeType;
				GetGlobalApp()->m_sCurrentDebitResult.DebitTime			= pDVLocalDebit->ServerDebitTime.GetTime();
				GetGlobalApp()->m_sCurrentDebitResult.ConsumeTime		= ::GetTickCount() - ::GetGlobalApp()->m_dwTradeBeginOvertime;
				_tcscpy_s(GetGlobalApp()->m_sCurrentDebitResult.ExecuteDesc, _countof(GetGlobalApp()->m_sCurrentDebitResult.ExecuteDesc), m_sDVJsonDebit.DebitStatusDesc);

				CString strTempCode = GetNetPayRecordMatchCode();
				_tcscpy_s(GetGlobalApp()->m_sCurrentDebitResult.TradeSecurityCode, _countof(GetGlobalApp()->m_sCurrentDebitResult.TradeSecurityCode), strTempCode.GetBuffer(0)); strTempCode.ReleaseBuffer();
				_tcscpy_s(GetGlobalApp()->m_sCurrentDebitResult.PayIdentifier, _countof(GetGlobalApp()->m_sCurrentDebitResult.PayIdentifier), m_sDVJsonDebit.PayIdentifier);
				_tcscpy_s(GetGlobalApp()->m_sCurrentDebitResult.DebitOrder, _countof(GetGlobalApp()->m_sCurrentDebitResult.DebitOrder), m_sDVJsonDebit.DebitOrderNumber);
				//������ʾ�ɷѳɹ�����
//#ifdef _DEBUG
//				iMMIDetailType = 0x05;
//				m_pThreadDevice->PostThreadMessage(WM_THREAD_DEVICE_CONTROL_DEAL_RESULT_MMI,(WPARAM)iMMIDetailType,(LPARAM)0x00);
//#else
				m_pThreadDevice->PostThreadMessage(WM_THREAD_DEVICE_CONTROL_DEAL_RESULT_MMI, (WPARAM)0x01, (LPARAM)GetGlobalApp()->m_sCurrentDebitResult.PayPlatformType);//modify 2018-09
//#endif
				//��¼��־
				//add 2018-04,�۷ѳɹ���֪ͨ����ʶ��ϵͳ
				GetGlobalApp()->LPR_PackPayResult(GetGlobalApp()->m_sCurrentDebitResult.PayPlatformType);
				strLog.Format(_T("[ҵ������߳�],��Ӧ֧����ƽ̨������Ϣ(%s),����״̬:�۷ѳɹ�,��������߷���֪ͨ"), strResponseType);

				//SetDebitSuccessHint(GetGlobalApp()->m_sCurrentDebitResult.PayPlatformType);
				//add 2018-09 �ۿ�ɹ������ý������� 
				
				if(isValidResponse)
				{
					isValidResponse =  CheckPayResultValidity(m_sDVJsonDebit);
				}
				  
			}
			else
			{
				//�۷Ѳ�ѯʧ�ܣ���ʱ���أ������Լ����ȴ�����ʱ������
				if ( RCC_DEBIT_QUERY_ERROR == byExecuteState )
				{
				}
				else
				{
					//������ʾ�ɷ�ʧ�ܽ���
					if (GetGlobalApp()->m_bIsExternMMIDetail)
					{
						m_pThreadDevice->PostThreadMessage(WM_THREAD_DEVICE_CONTROL_DEAL_RESULT_MMI, (WPARAM)iMMIDetailType, (LPARAM)0x00);
					}
					else
					{
						m_pThreadDevice->PostThreadMessage(WM_THREAD_DEVICE_CONTROL_DEAL_RESULT_MMI, (WPARAM)0x02, (LPARAM)0x00);
					}
					//��ʶ������ʱ
					m_dwCheckDebitAccumulateTime = 0x0FFF;
					//��¼��־
					strLog.Format(_T("[ҵ������߳�],��Ӧ֧����ƽ̨������Ϣ(%s),����״̬:�۷�ʧ��,��������߷���֪ͨ"), strResponseType);
				}
			}

			//��������������ܣ����ڴ�����ģ��״̬(1.0.6.8����)


			//modify 2018-06
			bool is_nfc_signing = (::GetGlobalApp()->GetUnipayWorkFlowState()==UMS_SIGNING);
			if (::GetGlobalApp()->m_bNFCIsUse &&  !is_nfc_signing)
			{
				//����ģ�鹤��״̬
				GetGlobalApp()->SetUnipayWorkFlowState(UMS_UNKNOW);
			}

			//�۷Ѳ�ѯʧ�ܣ���ʱ���أ������Լ����ȴ�����ʱ������
			if ( RCC_DEBIT_QUERY_ERROR == byExecuteState )
			{
			}
			else
			{
				FinishTrade(TRUE);	//�����������ף��ǳ�ʱ��
				GetGlobalApp()->RecordLog(GetGlobalApp()->FmtStr(_T("[ҵ������߳�],��Ӧ֧����ƽ̨�����������(�ۿ�����/��ѯ)����ʽ�����(���ܴ���:%d,ִ��״��:%d)"), byFunCode, byExecuteState));
			}
		}
		//������־
		GetGlobalApp()->RecordLog(strLog);
	}
	//������Դ
	if (NULL != pDVJsonBase)
	{
		delete pDVJsonBase;
		pDVJsonBase = NULL;
	}

	if (NULL != pDVLocalDebit)
	{
		delete pDVLocalDebit;
		pDVLocalDebit = NULL;
	}

	//�۷Ѳ�ѯʧ�ܣ���ʱ���أ������Լ����ȴ�����ʱ������
	if ( RCC_DEBIT_QUERY_ERROR == byExecuteState )
	{
	}
	else if(isValidResponse)
	{
		//GetGlobalApp()->RecordLog("before valid notify");
		//֪ͨ�����߷��ؽ��
		::GetGlobalApp()->NotifyCaller(byFunCode, byExecuteState);
		//GetGlobalApp()->RecordLog("after valid notify");
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////
// �������ƣ�CThreadManage::OnMsgCheckDebitPasswordState
//
// ������������ѯ�û���������Ŀۿ���״̬
//
// ���������WPARAM wParam
//         ��LPARAM lParam
// ���������void
// ��д��Ա��ROCY
// ��дʱ�䣺2016��7��24��
// �޸���Ա��
// �޸�ʱ�䣺
// �����汾��1.0.0.1
// ��ע˵������Ӧ��Ϣ WM_THREAD_MANAGE_CHECK_DEBIT_PASSWORD_STATE
//
////////////////////////////////////////////////////////////////////////////////////////////////
void CThreadManage::OnMsgCheckDebitPasswordState(WPARAM wParam, LPARAM lParam)
{
	if(!GetGlobalApp()->m_bIsTrading)
	{
		GetGlobalApp()->RecordLog(_T("δ���ڽ����У�ֹͣ��ѯ"));
		return ;
	}
	//���п۷Ѳ�ѯ׼��(6��,5��,4��,3��,2�룩��ѯ
	switch (m_dwCheckDebitSpanTime)
	{
	case 0:
		m_dwCheckDebitSpanTime = 6000;
		break;
	case 6000:
		m_dwCheckDebitSpanTime = 5000;
		break;
	case 5000:
		m_dwCheckDebitSpanTime = 4000;
		break;
	case 4000:
		m_dwCheckDebitSpanTime = 3000;
		break;
	case 3000:
		m_dwCheckDebitSpanTime = 2000;
		break;
	case 2000:
		m_dwCheckDebitSpanTime = 2000;
		break;
	}
	//���ö�ʱ��
	StartTimer(TIMER_ID_DEBIT_CHECK, m_dwCheckDebitSpanTime);
	//��¼��־
	GetGlobalApp()->RecordLog(GetGlobalApp()->FmtStr(_T("[ҵ������߳�],��Ӧ֧�������̵߳�֧��״̬��ѯ��Ϣ,���ж�ʱ�����ã���ʱʱ�� %d ����"), m_dwCheckDebitSpanTime));
}

////////////////////////////////////////////////////////////////////////////////////////////////
// �������ƣ�CThreadManage::OnMsgRevNFCSignDataFromHTTP
//
// ������������ȡ��HTTP������ǩ������
//
// ���������WPARAM wParam
//         ��LPARAM lParam
// ���������void
// ��д��Ա��ROCY
// ��дʱ�䣺2017��9��20��
// �޸���Ա��
// �޸�ʱ�䣺
// �����汾��1.0.0.1
// ��ע˵������Ӧ��Ϣ WM_THREAD_NFC_SIGN_DATA_RETURN_FROM_HTTP
//
////////////////////////////////////////////////////////////////////////////////////////////////
void CThreadManage::OnMsgRevNFCSignDataFromHTTP(WPARAM wParam, LPARAM lParam)
{
	CString strLog;
	CString strRevSignData;
	CString strResponseType;
	//����������ݵ�����
	WORD wOpType  = LOWORD(lParam);
	//��÷����������Ľ��
	WORD wOpState = HIWORD(lParam);

	if ( HTTP_RC_DATA_LEGAL == wOpState) //ǩ�����ݻ����ɹ�
	{
		//������ָ���Ƿ�����
		CDVJsonBase* pDVJsonBaseTemp = (CDVJsonBase*)wParam;
		if (NULL != pDVJsonBaseTemp)
		{
			//��������ȡNFCǩ�������ѱ���
			strRevSignData = ((CDVJsonNFCSignIn*)pDVJsonBaseTemp)->NFCSignData;
			//������Դ
			delete pDVJsonBaseTemp;
			pDVJsonBaseTemp = NULL;
			//����NFCǩ����������
			NFCSignInDataProcessFromHTTP(strRevSignData);
		}
	}
	else
	{
		//ǩ������ʧ��
		strLog.Format(_T("[ҵ������߳�]:NFC�ǽ�֧�����̣�ϵͳ��⵽ǩ�����̵���,�ƶ˽�������ʧ�ܣ���%d�ν��������ϴ����ݽ����������%d�룩���ж�ǩ��ʧ�ܣ�������ʼ����ǩ������"), GetGlobalApp()->m_iNFCSignFlowDataCount, GetGlobalApp()->m_iNFCSignDataLimitSpanTime);
		//����ǩ��ʧ��״̬�������´���ǩ׼��
		GetGlobalApp()->m_bNFCSignInSuccess = false;
		GetGlobalApp()->SetUnipayWorkFlowState(UMS_UNKNOW);
		//�����ϴ�ǩ������
		GetGlobalApp()->m_tNFCLastSignDataTime = time(NULL);
		//��¼��־
		GetGlobalApp()->RecordLog(strLog);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////
// �������ƣ�CThreadManage::OnMsgNFCStartSignInFlow
//
// ������������ʼ����ǩ������
//
// ���������WPARAM wParam
//         ��LPARAM lParam
// ���������void
// ��д��Ա��ROCY
// ��дʱ�䣺2017��9��21��
// �޸���Ա��
// �޸�ʱ�䣺
// �����汾��1.0.0.1
// ��ע˵����
//
////////////////////////////////////////////////////////////////////////////////////////////////
void CThreadManage::OnMsgNFCStartSignInFlow(WPARAM wParam, LPARAM lParam)
{
	try
	{
		if (NULL == m_pThreadDevice)
			throw - 1;

		CString* pstrString = new CString();
		if (NULL == pstrString)
			throw - 2;
		//����ǩ��״̬Ϊ��
		GetGlobalApp()->m_bNFCSignInSuccess = false;
		//����ǩ����������Ϊ0
		GetGlobalApp()->m_iNFCSignFlowDataCount = 0;
		//������·���ԣ���λ��ָ��
		NFCSendDefaultCmdToPos(0x01);
		//���õȴ�POS���س�ʱ
//		StartTimer(TIMER_ID_NFC_WAIT_NORMAL_DATA,10000);
	}
	catch (int& iErrorCode)
	{
		switch (iErrorCode)
		{
		case -1:
			break;
		}
	}
}

/****************************************�ڲ���������********************************************/

////////////////////////////////////////////////////////////////////////////////////////////////
// �������ƣ�CThreadManage::fnReleaseResource
//
// ������������������������Դ
//
// ����������޲���
// ���������voi
// ��д��Ա��ROCY
// ��дʱ�䣺2016��7��24��
// �޸���Ա��
// �޸�ʱ�䣺
// �����汾��1.0.0.1
// ��ע˵����
//
////////////////////////////////////////////////////////////////////////////////////////////////
void CThreadManage::fnReleaseResource()
{
	m_PayCenter.Destroy();
	m_MediaCenter.Destroy();
	// DWORD dwThreadExitFlag	= 0x00;
	// 	//�������ԵĴ���
	// 	int iAllowTryCount		= 10;
	// 	//�Ѿ����Դ���
	// 	int iHaveTryCount		= 0;
	// 	//ÿ�����Ե�ʱ����ʱ�䣨��λ�����룩
	// 	DWORD	dwSpanTime		= 100;

	// if (NULL != bing_Debug)
	// {

	// 	char chs_debug[100]={0};
	// 	sprintf(chs_debug,"%d",bing_Debug->m_nThreadID);
	// 	MessageBox(0,chs_debug,"bing_Debug",0);
	// 	//���ø��߳��Ƴ����Ϊ��
	// 	bing_Debug->m_bExit = TRUE;

	// 	//��Ĭ50����
	// 	::Sleep(50);
		
		
	// 	//ÿ�εȴ�1000���룬���Դ���������10��
	// 	while (OP_SUCCESS != GetGlobalApp()->fnCloseThread(bing_Debug->m_hThread, 1000))
	// 	{
	// 		Sleep(dwSpanTime);
	// 		++iHaveTryCount;
	// 		if (iHaveTryCount >= iAllowTryCount)
	// 		{
	// 			//��Ǹ��߳�δ�������˳�
	// 			dwThreadExitFlag |= 0x01;
	// 			//MessageBox(0,"not exit","",0);
	// 			//��ֹ����
	// 			break;
	// 		}
	// 	}
		
	// 	//add zhuyabing 2017-11
	// 	//delete m_pThreadDevice;
	// 	//m_pThreadDevice=NULL;

	// 	bing_Debug= NULL;
	
	// 	//��¼��־
		
	// }
	// return ;


	try
	{
		DWORD dwSleepTime = 0;

		CString strLog;
		/**********************************************���������߳���Դ**********************************************/

		//����ۿ�״̬��ѯ�ṹ��
		if (NULL == m_pDVJsonDebitCheck)
		{
			delete m_pDVJsonDebitCheck;
			m_pDVJsonDebitCheck = NULL;
		}
		m_sDVJsonDebit.Reset();

		strLog.Format(_T("[ҵ������߳�],���ڿ�ʼ�˳������߳�..."));
		//��¼��־
		GetGlobalApp()->RecordLog(strLog, LOG_LEVEL_RUN, TRUE);

		//�߳��˳����
		DWORD dwThreadExitFlag	= 0x00;
		//�������ԵĴ���
		int iAllowTryCount		= 10;
		//�Ѿ����Դ���
		int iHaveTryCount		= 0;
		//ÿ�����Ե�ʱ����ʱ�䣨��λ�����룩
		DWORD	dwSpanTime		= 100;


		//CThreadUpdate*		m_pThreadUpdate;
		if (NULL != m_pThreadUpdate)
		{
			//���ø��߳��Ƴ����Ϊ��
			//GetGlobalApp()->RecordLog("exit threadupdate",LOG_LEVEL_ERROR,TRUE);
			m_pThreadUpdate->ThrQuit();

			//��Ĭ50����
			::Sleep(50);

			//ÿ�εȴ�1000���룬���Դ���������10��
			while (OP_SUCCESS != GetGlobalApp()->fnCloseThread(m_pThreadUpdate->m_hThread, 1000))
			{
				Sleep(dwSpanTime);
				++iHaveTryCount;
				if (iHaveTryCount >= iAllowTryCount)
				{
					//��Ǹ��߳�δ�������˳�
					dwThreadExitFlag |= 0x01;
					//��ֹ����
					break;
				}
			}
			m_pThreadUpdate = NULL;
			strLog.Format(_T("[ҵ������߳�],%s"), GetGlobalApp()->FmtStr(_T("�����������̡߳��Ѿ�ֹͣ(���Դ��� %d)..."), iHaveTryCount));
			//��¼��־
			GetGlobalApp()->RecordLog(strLog, LOG_LEVEL_RUN, TRUE);
		}
		::Sleep(100);
		GetGlobalApp()->RecordLog("���������߳��Ѿ��˳�", LOG_LEVEL_RUN, TRUE);

		iHaveTryCount = 0;
		//ֹͣ����Ӳ�������߳�
		if (NULL != m_pThreadDevice)
		{
			//���ø��߳��Ƴ����Ϊ��
			m_pThreadDevice->m_bExit = TRUE;

			//��Ĭ50����
			::Sleep(50);

			//ÿ�εȴ�1000���룬���Դ���������10��
			while (OP_SUCCESS != GetGlobalApp()->fnCloseThread(m_pThreadDevice->m_hThread, 1000))
			{
				Sleep(dwSpanTime);
				++iHaveTryCount;
				if (iHaveTryCount >= iAllowTryCount)
				{
					//��Ǹ��߳�δ�������˳�
					dwThreadExitFlag |= 0x01;
					//MessageBox(0,"not exit","",0);
					//��ֹ����
					break;
				}
			}
			//add zhuyabing 2017-11
			//delete m_pThreadDevice;
			m_pThreadDevice=NULL;


			strLog.Format(_T("[ҵ������߳�],%s"), GetGlobalApp()->FmtStr(_T("���豸�����̡߳��Ѿ�ֹͣ(���Դ��� %d)..."), iHaveTryCount));
			//��¼��־
			GetGlobalApp()->RecordLog(strLog, LOG_LEVEL_RUN, TRUE);
		}
		::Sleep(dwSleepTime);
		GetGlobalApp()->RecordLog("�豸�����߳��Ѿ��˳�", LOG_LEVEL_RUN, TRUE);

		iHaveTryCount = 0;
		//ֹ֧ͣ����ƽ̨ͨѶ�߳�
		if (NULL != m_pThreadHttp)
		{
			//���ø��߳��Ƴ����Ϊ��
			
			//MessageBox(0, "before-----------", "", 0);
			HANDLE httpHandle = m_pThreadHttp->m_hThread;
			m_pThreadHttp->m_bLoopBreak = TRUE;	//��Ҫ�����۷�ִ��ѭ��
			m_pThreadHttp->waitTillIdle();
			///MessageBox(0, "after --------", "", 0);
			

			
			//m_pThreadHttp->m_bLoopBreak = TRUE;	//��Ҫ�����۷�ִ��ѭ��
			//m_pThreadHttp->m_bExit = TRUE;

			//��Ĭ50����
			::Sleep(50);

			//ÿ�εȴ�1000���룬���Դ���������10��
			while(OP_SUCCESS != GetGlobalApp()->fnCloseThread(httpHandle,1000))
			{
				Sleep(dwSpanTime);
				++iHaveTryCount;
				if (iHaveTryCount >= iAllowTryCount)
				{
					//��Ǹ��߳�δ�������˳�
					dwThreadExitFlag |= 0x01;
					//��ֹ����
					break;
				}
			}
			
			m_pThreadHttp = NULL;



			strLog.Format(_T("[ҵ������߳�],%s"), GetGlobalApp()->FmtStr(_T("��֧��ͨѶ�̡߳��Ѿ�ֹͣ(���Դ��� %d)..."), iHaveTryCount));
			//��¼��־
			GetGlobalApp()->RecordLog(strLog, LOG_LEVEL_RUN, TRUE);
		}
		GetGlobalApp()->RecordLog("֧��ͨѶ�߳��Ѿ��˳�", LOG_LEVEL_RUN, TRUE);

		iHaveTryCount = 0;
		//ֹͣNFCǩ��ͨѶ�߳�
		if (NULL != m_pThreadNFCSignIn)
		{
			//���ø��߳��Ƴ����Ϊ��
			m_pThreadNFCSignIn->m_bExit = TRUE;
			//��Ĭ50����
			::Sleep(50);

			//ÿ�εȴ�1000���룬���Դ���������10��
			while (OP_SUCCESS != GetGlobalApp()->fnCloseThread(m_pThreadNFCSignIn->m_hThread, 1000))
			{
				Sleep(dwSpanTime);
				++iHaveTryCount;
				if (iHaveTryCount >= iAllowTryCount)
				{
					//��Ǹ��߳�δ�������˳�
					dwThreadExitFlag |= 0x01;
					//��ֹ����
					break;
				}
			}

			strLog.Format(_T("[ҵ������߳�],%s"), GetGlobalApp()->FmtStr(_T("��NFCǩ���̡߳��Ѿ�ֹͣ(���Դ��� %d)..."), iHaveTryCount));
			//��¼��־
			GetGlobalApp()->RecordLog(strLog, LOG_LEVEL_RUN, TRUE);
		}
		GetGlobalApp()->RecordLog("NFCǩ���߳��Ѿ��˳�", LOG_LEVEL_RUN, TRUE);
	}
	catch (int& iErrorCode)
	{
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////
// �������ƣ�CThreadManage::ContructDebitMoneyInfo
//
// �������������췢�͵�֧����ƽ̨������
//
// ���������IN const tagAccountInfo* pAccountInfo -- ����Ӳ����ȡ�����û���Ϣ
// ���������BOOL
// ��д��Ա��ROCY
// ��дʱ�䣺2016��7��24��
// �޸���Ա��
// �޸�ʱ�䣺
// �����汾��1.0.0.1
// ��ע˵����
//
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CThreadManage::ContructDebitMoneyInfo(IN const tagAccountInfo* pAccountInfo)
{
	BOOL bReturn = FALSE;
	if(IsFuturePay())
	{
		return FALSE;//remove 2019-01 ʹ����֧��
	}
	
	try
	{
		if (NULL == pAccountInfo)
			throw - 1;
		//���ۿ���Ϣ�ṹ��
		CDVJsonDebit* pDVJsonDebit = new CDVJsonDebit();
		if (NULL == pDVJsonDebit)
			throw - 2;
		CString strPayId = GetGlobalApp()->GetGuidString(1);
		CString strTemp;

		//��ʶ�Ƿ��������
		pDVJsonDebit->TestingData			= ::GetGlobalApp()->m_bIsTestData;
		//��ȡ�����汾��
		pDVJsonDebit->NetPayDllVersion		= ::GetGlobalApp()->m_strNetPayDllVersion;
		pDVJsonDebit->DeviceDllVersion		= ::GetGlobalApp()->m_strDeviceDllVersion;
		pDVJsonDebit->TWProductSerialNo		= ::GetGlobalApp()->m_strTWProductSerialNo;

		//��䱨��ͷ��Ϣ
		pDVJsonDebit->Version				= _T("1.5");
		pDVJsonDebit->PackageType			= JSON_DT_SEND_DIBIT;
		pDVJsonDebit->Token					= _T("");
		pDVJsonDebit->ProvinceID			= m_sTranstParam.ProvinceID;
		pDVJsonDebit->SubmitCount			= 0x01;
		pDVJsonDebit->SubmitTime			= GetGlobalApp()->GetCurrentTimeString(1);
		pDVJsonDebit->m_hCallerWnd			= NULL;
		pDVJsonDebit->RecordMatchCode		= GetNetPayRecordMatchCode();
		pDVJsonDebit->TradeSecurityCode		= _T("");
		pDVJsonDebit->PayIdentifier			= GetGlobalApp()->m_pCurrentDebitInfo.PayIdentifier;

		//��������Ϣ
		pDVJsonDebit->QRCodeData				= GetGlobalApp()->FmtStr(_T("%s"), pAccountInfo->DataContext);
		pDVJsonDebit->QRCodeType				= 0x00;
		pDVJsonDebit->IDBindInPayPlatform		= _T("");
		pDVJsonDebit->IDBindInPayPlatformType	= 0x00;
		pDVJsonDebit->DebitType					= JSON_CMD_TYPE_DEBIT_ONLY_PAY;

#ifdef _DEBUG
		if (0 != GetGlobalApp()->m_pCurrentDebitInfo.Money)
		{
			pDVJsonDebit->DebitMoney				= 1;
		}
#else
		pDVJsonDebit->DebitMoney				= GetGlobalApp()->m_pCurrentDebitInfo.Money;
#endif
		pDVJsonDebit->DebitTime					= GetGlobalApp()->m_pCurrentDebitInfo.SubTime;
		pDVJsonDebit->DebitTollDate				= GetGlobalApp()->m_pCurrentDebitInfo.TollDate;
		pDVJsonDebit->DebitTollShift			= GetGlobalApp()->m_pCurrentDebitInfo.ShiftID;
		pDVJsonDebit->AccountInfoSrcType		= LOWORD(pAccountInfo->DataType);
		pDVJsonDebit->AccountInfoDeviceID		= HIWORD(pAccountInfo->DataType);
		pDVJsonDebit->AccountInfoDataType		= 0x01;


		//��丽����Ϣ
		pDVJsonDebit->EntryProvinceID		= m_sTranstParam.ProvinceID;
		pDVJsonDebit->EntryAreaID			= GetGlobalApp()->m_pCurrentDebitInfo.EntryAreaID;
		pDVJsonDebit->EntryRoadID			= GetGlobalApp()->m_pCurrentDebitInfo.EntryRoadID;
		pDVJsonDebit->EntryStationID		= GetGlobalApp()->m_pCurrentDebitInfo.EntryStationID;
		pDVJsonDebit->EntryStationName		= GetGlobalApp()->m_pCurrentDebitInfo.EntryStationName;
		pDVJsonDebit->EntryLane				= GetGlobalApp()->m_pCurrentDebitInfo.EntryLane;
		pDVJsonDebit->EntryTime				= GetGlobalApp()->m_pCurrentDebitInfo.EntryTime;
		if (pDVJsonDebit->EntryStationName.IsEmpty())
		{
			pDVJsonDebit->EntryStationName.Format(_T("%s"), pDVJsonDebit->EntryStationID);
		}

		pDVJsonDebit->PayProvinceID			= m_sTranstParam.ProvinceID;
		pDVJsonDebit->PayStationID			= m_sTranstParam.LaneConfigInfo.str_StationID;
		pDVJsonDebit->PayStationName		= m_sTranstParam.LaneConfigInfo.StationTab;
		pDVJsonDebit->PayLane				= m_sTranstParam.LaneConfigInfo.str_LaneID;
		pDVJsonDebit->PayRoadID				= m_sTranstParam.LaneConfigInfo.str_RoadID;


		
		//�����շ�Ա��Ϣ(�汾 1.0.3.4)
		pDVJsonDebit->PayOperatorID			= GetGlobalApp()->m_pCurrentDebitInfo.OperatorID;
		pDVJsonDebit->PayOperatorName		= GetGlobalApp()->m_pCurrentDebitInfo.OperatorName;

		pDVJsonDebit->VehicleClass			= GetGlobalApp()->m_pCurrentDebitInfo.VehicleClass;
		pDVJsonDebit->VehicleType			= GetGlobalApp()->m_pCurrentDebitInfo.VehicleType;
		pDVJsonDebit->VehicleLicense		= GetGlobalApp()->m_pCurrentDebitInfo.VehicleLicense;
		pDVJsonDebit->VehicleColor			= GetGlobalApp()->m_pCurrentDebitInfo.VLColor;
		pDVJsonDebit->AxisCount				= GetGlobalApp()->m_pCurrentDebitInfo.AxisCount;
		pDVJsonDebit->TotalWeight			= GetGlobalApp()->m_pCurrentDebitInfo.TotalWeight;
		pDVJsonDebit->LimitWeight			= GetGlobalApp()->m_pCurrentDebitInfo.LimitWeight;

			//add 2018-03
        pDVJsonDebit->AutoLicense           = GetGlobalApp()->m_pCurrentDebitInfo.AutoLicense;
        pDVJsonDebit->AutoColor           = GetGlobalApp()->m_pCurrentDebitInfo.AutoColor;
        pDVJsonDebit->PassCertificateType           = GetGlobalApp()->m_pCurrentDebitInfo.PassCertificateType;
        pDVJsonDebit->TicketNo           = GetGlobalApp()->m_pCurrentDebitInfo.TicketNo;
        pDVJsonDebit->Distance          = GetGlobalApp()->m_pCurrentDebitInfo.Distance;
        pDVJsonDebit->EntryLicense          = GetGlobalApp()->m_pCurrentDebitInfo.EntryVehicleLicense;
        pDVJsonDebit->EntryVColor          = GetGlobalApp()->m_pCurrentDebitInfo.EntryVLColor;
        pDVJsonDebit->EntryVClass          = GetGlobalApp()->m_pCurrentDebitInfo.EntryVehicleClass;
		pDVJsonDebit->EntryVType			= GetGlobalApp()->m_pCurrentDebitInfo.EntryVehicleType;
		//����NFC�۷�������Ϣ
		pDVJsonDebit->NFCTerminal			= GetGlobalApp()->m_strNFCTerminal;
		pDVJsonDebit->NFCSerialNo			= GetGlobalApp()->m_strNFCSerialNo;
		pDVJsonDebit->NFCPayData			= GetGlobalApp()->m_pCurrentDebitInfo.NFCPayData;
		pDVJsonDebit->NFCRevData			= GetGlobalApp()->m_pCurrentDebitInfo.NFCRevData;

		//add 2018-04
		pDVJsonDebit->imageId               = GetGlobalApp()->m_pCurrentDebitInfo.imageId;

		bool is_xuanyuantong_code = (pDVJsonDebit->QRCodeData.GetLength() == 731);
		if(is_xuanyuantong_code)
		{
			pDVJsonDebit->AccountInfoSrcType		= 24;
			pDVJsonDebit->AccountInfoDataType		= 24;
		}
		

		//���Ϳۿ�����
		m_pThreadHttp->PostThreadMessage(WM_THREAD_HTTP_SEND_DATA_REQUEST, (WPARAM)pDVJsonDebit, (LPARAM)MAKELONG(JSON_DT_SEND_DIBIT, MAKEWORD(0x01, 0x00)));
		//��ʶ������ʱ
		m_dwCheckDebitAccumulateTime = 0x00;
		//��ǳɹ�
		bReturn = TRUE;

		//�������
		GetGlobalApp()->m_pCurrentDebitInfo.NFCPayData = _T("");
		GetGlobalApp()->m_pCurrentDebitInfo.NFCRevData = _T("");
	}
	catch (int& iErrorCode)
	{

	}

	return bReturn;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// �������ƣ�CThreadManage::ContructDebitMoneyInfo
//
// �������������췢�͵�֧����ƽ̨������
//
// ���������void
// ���������BOOL
// ��д��Ա��ROCY
// ��дʱ�䣺2016��8��3��
// �޸���Ա��
// �޸�ʱ�䣺
// �����汾��1.0.0.1
// ��ע˵����
//
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CThreadManage::ContructDebitMoneyInfo(void)
{
	BOOL bReturn = FALSE;
	if(IsFuturePay())
	{
		return FALSE;//remove 2019-01 ʹ����֧��
	}
	try
	{
		//���ۿ���Ϣ�ṹ��
		CDVJsonDebit* pDVJsonDebit = new CDVJsonDebit();
		if (NULL == pDVJsonDebit)
			throw - 1;
		CString strPayId = GetGlobalApp()->GetGuidString(1);
		CString strTemp;

		//��ʶ�Ƿ��������
		pDVJsonDebit->TestingData			= ::GetGlobalApp()->m_bIsTestData;
		//��ȡ�����汾��
		pDVJsonDebit->NetPayDllVersion		= ::GetGlobalApp()->m_strNetPayDllVersion;
		pDVJsonDebit->DeviceDllVersion		= ::GetGlobalApp()->m_strDeviceDllVersion;
		pDVJsonDebit->TWProductSerialNo		= ::GetGlobalApp()->m_strTWProductSerialNo;

		//��䱨��ͷ��Ϣ
		pDVJsonDebit->Version				= _T("1.5");
		pDVJsonDebit->PackageType			= JSON_DT_SEND_DIBIT;
		pDVJsonDebit->Token					= _T("");
		pDVJsonDebit->ProvinceID			= m_sTranstParam.ProvinceID;
		pDVJsonDebit->SubmitCount			= 0x01;
		pDVJsonDebit->SubmitTime			= GetGlobalApp()->GetCurrentTimeString(1);
		pDVJsonDebit->m_hCallerWnd			= NULL;
		pDVJsonDebit->RecordMatchCode		= GetNetPayRecordMatchCode();
		pDVJsonDebit->TradeSecurityCode		= _T("");
		pDVJsonDebit->PayIdentifier			= GetGlobalApp()->m_pCurrentDebitInfo.PayIdentifier;

		//��������Ϣ
		pDVJsonDebit->QRCodeData				= GetGlobalApp()->m_pCurrentDebitInfo.PayCertificateCode;
		pDVJsonDebit->QRCodeType				= 0x00;
		pDVJsonDebit->IDBindInPayPlatform		= _T("");
		pDVJsonDebit->IDBindInPayPlatformType	= 0x00;
		pDVJsonDebit->DebitType					= JSON_CMD_TYPE_DEBIT_ONLY_PAY;

#ifdef _DEBUG
		if (0 != GetGlobalApp()->m_pCurrentDebitInfo.Money)
		{
			pDVJsonDebit->DebitMoney				= 1;
		}
#else
		pDVJsonDebit->DebitMoney				= GetGlobalApp()->m_pCurrentDebitInfo.Money;
#endif
		pDVJsonDebit->DebitTime					= GetGlobalApp()->m_pCurrentDebitInfo.SubTime;
		pDVJsonDebit->DebitTollDate				= GetGlobalApp()->m_pCurrentDebitInfo.TollDate;
		pDVJsonDebit->DebitTollShift			= GetGlobalApp()->m_pCurrentDebitInfo.ShiftID;
		pDVJsonDebit->AccountInfoSrcType		= LOWORD(GetGlobalApp()->m_pCurrentDebitInfo.PayCertificateType);
		pDVJsonDebit->AccountInfoDeviceID		= HIWORD(GetGlobalApp()->m_pCurrentDebitInfo.PayCertificateType);
		pDVJsonDebit->AccountInfoDataType		= 0x01;

		//��丽����Ϣ
		pDVJsonDebit->EntryProvinceID		= m_sTranstParam.ProvinceID;
		pDVJsonDebit->EntryAreaID			= GetGlobalApp()->m_pCurrentDebitInfo.EntryAreaID;
		pDVJsonDebit->EntryRoadID			= GetGlobalApp()->m_pCurrentDebitInfo.EntryRoadID;
		pDVJsonDebit->EntryStationID		= GetGlobalApp()->m_pCurrentDebitInfo.EntryStationID;
		pDVJsonDebit->EntryStationName		= GetGlobalApp()->m_pCurrentDebitInfo.EntryStationName;
		pDVJsonDebit->EntryLane				= GetGlobalApp()->m_pCurrentDebitInfo.EntryLane;
		pDVJsonDebit->EntryTime				= GetGlobalApp()->m_pCurrentDebitInfo.EntryTime;
		if (pDVJsonDebit->EntryStationName.IsEmpty())
		{
			pDVJsonDebit->EntryStationName.Format(_T("%s"), pDVJsonDebit->EntryStationID);
		}
		pDVJsonDebit->EntryRoadID			= GetGlobalApp()->m_pCurrentDebitInfo.EntryRoadID;


		pDVJsonDebit->PayProvinceID			= m_sTranstParam.ProvinceID;
		pDVJsonDebit->PayStationID			= m_sTranstParam.LaneConfigInfo.str_StationID;
		pDVJsonDebit->PayStationName		= m_sTranstParam.LaneConfigInfo.StationTab;
		pDVJsonDebit->PayLane				= m_sTranstParam.LaneConfigInfo.str_LaneID;
		pDVJsonDebit->PayRoadID				= m_sTranstParam.LaneConfigInfo.str_RoadID;

		//GetGlobalApp()->RecordLog(pDVJsonDebit->PayRoadID+"==============");
		//�����շ�Ա��Ϣ(�汾 1.0.3.4)
		pDVJsonDebit->PayOperatorID			= GetGlobalApp()->m_pCurrentDebitInfo.OperatorID;
		pDVJsonDebit->PayOperatorName		= GetGlobalApp()->m_pCurrentDebitInfo.OperatorName;

		pDVJsonDebit->VehicleClass			= GetGlobalApp()->m_pCurrentDebitInfo.VehicleClass;
		pDVJsonDebit->VehicleType			= GetGlobalApp()->m_pCurrentDebitInfo.VehicleType;
		pDVJsonDebit->VehicleLicense		= GetGlobalApp()->m_pCurrentDebitInfo.VehicleLicense;
		pDVJsonDebit->VehicleColor			= GetGlobalApp()->m_pCurrentDebitInfo.VLColor;
		pDVJsonDebit->AxisCount				= GetGlobalApp()->m_pCurrentDebitInfo.AxisCount;
		pDVJsonDebit->TotalWeight			= GetGlobalApp()->m_pCurrentDebitInfo.TotalWeight;
        pDVJsonDebit->LimitWeight           = GetGlobalApp()->m_pCurrentDebitInfo.LimitWeight;

		
		//add 2018-03
        pDVJsonDebit->AutoLicense           = GetGlobalApp()->m_pCurrentDebitInfo.AutoLicense;
        pDVJsonDebit->AutoColor           = GetGlobalApp()->m_pCurrentDebitInfo.AutoColor;
        pDVJsonDebit->PassCertificateType           = GetGlobalApp()->m_pCurrentDebitInfo.PassCertificateType;
        pDVJsonDebit->TicketNo           = GetGlobalApp()->m_pCurrentDebitInfo.TicketNo;
        pDVJsonDebit->Distance          = GetGlobalApp()->m_pCurrentDebitInfo.Distance;
        pDVJsonDebit->EntryLicense          = GetGlobalApp()->m_pCurrentDebitInfo.EntryVehicleLicense;
        pDVJsonDebit->EntryVColor          = GetGlobalApp()->m_pCurrentDebitInfo.EntryVLColor;
        pDVJsonDebit->EntryVClass          = GetGlobalApp()->m_pCurrentDebitInfo.EntryVehicleClass;
		pDVJsonDebit->EntryVType			= GetGlobalApp()->m_pCurrentDebitInfo.EntryVehicleType;

		//add 2018-04
		pDVJsonDebit->imageId              = GetGlobalApp()->m_pCurrentDebitInfo.imageId;

	
	

		//����NFC�۷�������Ϣ
		pDVJsonDebit->NFCTerminal			= GetGlobalApp()->m_strNFCTerminal;
		pDVJsonDebit->NFCSerialNo			= GetGlobalApp()->m_strNFCSerialNo;
		pDVJsonDebit->NFCPayData			= GetGlobalApp()->m_pCurrentDebitInfo.NFCPayData;
		pDVJsonDebit->NFCRevData			= GetGlobalApp()->m_pCurrentDebitInfo.NFCRevData;

		//���Ϳۿ�����
		m_pThreadHttp->PostThreadMessage(WM_THREAD_HTTP_SEND_DATA_REQUEST, (WPARAM)pDVJsonDebit, (LPARAM)MAKELONG(JSON_DT_SEND_DIBIT, MAKEWORD(0x01, 0x00)));
		//��ʶ������ʱ
		m_dwCheckDebitAccumulateTime = 0x00;
		//��ǳɹ�
		bReturn = TRUE;

		//�������
		GetGlobalApp()->m_pCurrentDebitInfo.NFCPayData = _T("");
		GetGlobalApp()->m_pCurrentDebitInfo.NFCRevData = _T("");
	}
	catch (int& iErrorCode)
	{
		switch (iErrorCode)
		{
		case -1:
			break;
		}
	}

	return bReturn;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// �������ƣ�CThreadManage::ContructDebitCancelInfo
//
// �������������쳷�����ݷ��͵�֧����ƽ̨
//
// ���������IN const CString& strPayIdentifier
// ���������BOOL
// ��д��Ա��ROCY
// ��дʱ�䣺2016��8��6��
// �޸���Ա��
// �޸�ʱ�䣺
// �����汾��1.0.0.1
// ��ע˵����
//
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CThreadManage::ContructDebitCancelInfo(IN const CString& strPayIdentifier)
{
	BOOL bReturn = FALSE;
	try
	{
		//���ۿ���Ϣ�ṹ��
		CDVJsonDebitCancel* pDVJsonDebitCancel = new CDVJsonDebitCancel();
		if (NULL == pDVJsonDebitCancel)
			throw - 1;
		CString strPayId = GetGlobalApp()->GetGuidString(1);
		CString strTemp;
		//��䱨��ͷ��Ϣ
		pDVJsonDebitCancel->Version				= _T("1.5");
		pDVJsonDebitCancel->PackageType			= JSON_DT_SEND_DIBIT_CANCEL;
		pDVJsonDebitCancel->Token				= _T("");
		pDVJsonDebitCancel->ProvinceID			= m_sTranstParam.ProvinceID;
		pDVJsonDebitCancel->SubmitCount			= 0x01;
		pDVJsonDebitCancel->SubmitTime			= GetGlobalApp()->GetCurrentTimeString(1);
		pDVJsonDebitCancel->m_hCallerWnd		= NULL;
		pDVJsonDebitCancel->PayIdentifier		= strPayIdentifier;
		((CDVJsonBase*)pDVJsonDebitCancel)->PayIdentifier = strPayIdentifier;
		//���Ϳۿ�����
		m_pThreadHttp->PostThreadMessage(WM_THREAD_HTTP_SEND_DATA_REQUEST, (WPARAM)pDVJsonDebitCancel, (LPARAM)MAKELONG(JSON_DT_SEND_DIBIT_CANCEL, MAKEWORD(0x01, 0x00)));
		//��ǳɹ�
		bReturn = TRUE;
	}
	catch (int& iErrorCode)
	{
		switch (iErrorCode)
		{
		case -1:
			break;
		}
	}

	return bReturn;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// �������ƣ�CThreadManage::GetNetPayRecordMatchCode
//
// ������������ȡ������֧���Ľ���ƥ����
//
// ���������IN const CString& strEncKey -- AES�㷨��Կ
// ���������const
// ��д��Ա��ROCY
// ��дʱ�䣺2016��4��12��
// �޸���Ա��
// �޸�ʱ�䣺
// �����汾��1.0.0.1
// ��ע˵����
// ƥ��ʹ�õ�������
// 1.���վ��	��7/10/20λ�ַ�)
// 2.���ʱ��	��14λ�ַ�)
// 3.����վ��	��7/10/20λ�ַ�)
// 4.����ʱ��	��14λ�ַ�)
// 5.���ڳ�����	��3/8λ�ַ���
// 6.���⿨��	��20λ�ַ���
// 7.���׽��	��10λ�ַ���
// ʹ��ԭ��
// �������ƴ�ӳ�һ���ַ���S����S����AES���ܣ�ʹ����ԿK���õ�����D��16�����ַ�������
// ������D����MD5����õ�ƥ����M
// У��ԭ�򣺸���������ʽ����µ�ƥ����M1��M��M1���ƥ������
////////////////////////////////////////////////////////////////////////////////////////////////
const CString CThreadManage::GetNetPayRecordMatchCode(IN const CString& strEncKey)
{
	CString strMatchCode;
	//У����Կ
	CString strRealKey(strEncKey);
	if (strRealKey.IsEmpty())
	{
		switch (this->m_sTranstParam.ProvinceID)
		{
		case 11:
			strRealKey.Format(_T("%s"), AES_ENC_KEY_DATA_11);
			break;
		case 12:
			strRealKey.Format(_T("%s"), AES_ENC_KEY_DATA_12);
			break;
		case 13:
			strRealKey.Format(_T("%s"), AES_ENC_KEY_DATA_13);
			break;
		case 14:
			strRealKey.Format(_T("%s"), AES_ENC_KEY_DATA_14);
			break;
		case 15:
			strRealKey.Format(_T("%s"), AES_ENC_KEY_DATA_15);
			break;

		case 31:
			strRealKey.Format(_T("%s"), AES_ENC_KEY_DATA_31);
			break;
		case 32:
			strRealKey.Format(_T("%s"), AES_ENC_KEY_DATA_32);
			break;
		case 33:
			strRealKey.Format(_T("%s"), AES_ENC_KEY_DATA_33);
			break;
		case 34:
			strRealKey.Format(_T("%s"), AES_ENC_KEY_DATA_34);
			break;
		case 35:
			strRealKey.Format(_T("%s"), AES_ENC_KEY_DATA_35);
			break;
		case 36:
			strRealKey.Format(_T("%s"), AES_ENC_KEY_DATA_36);
			break;
		case 37:
			strRealKey.Format(_T("%s"), AES_ENC_KEY_DATA_37);
			break;

		case 41:
			strRealKey.Format(_T("%s"), AES_ENC_KEY_DATA_41);
			break;
		case 42:
			strRealKey.Format(_T("%s"), AES_ENC_KEY_DATA_42);
			break;
		case 43:
			strRealKey.Format(_T("%s"), AES_ENC_KEY_DATA_43);
			break;
		case 44:
			strRealKey.Format(_T("%s"), AES_ENC_KEY_DATA_44);
			break;
		case 45:
			strRealKey.Format(_T("%s"), AES_ENC_KEY_DATA_45);
			break;

		case 50:
			strRealKey.Format(_T("%s"), AES_ENC_KEY_DATA_50);
			break;
		case 51:
			strRealKey.Format(_T("%s"), AES_ENC_KEY_DATA_51);
			break;
		case 52:
			strRealKey.Format(_T("%s"), AES_ENC_KEY_DATA_52);
			break;
		case 53:
			strRealKey.Format(_T("%s"), AES_ENC_KEY_DATA_53);
			break;
		case 54:
			strRealKey.Format(_T("%s"), AES_ENC_KEY_DATA_54);
			break;

		case 61:
			strRealKey.Format(_T("%s"), AES_ENC_KEY_DATA_61);
			break;
		case 62:
			strRealKey.Format(_T("%s"), AES_ENC_KEY_DATA_62);
			break;
		case 63:
			strRealKey.Format(_T("%s"), AES_ENC_KEY_DATA_63);
			break;
		case 64:
			strRealKey.Format(_T("%s"), AES_ENC_KEY_DATA_64);
			break;
		case 65:
			strRealKey.Format(_T("%s"), AES_ENC_KEY_DATA_65);
			break;
		default:
			//Ĭ����Կ
			strRealKey.Format(_T("%s"), AES_ENC_KEY_DATA);
			break;
		}
	}
	/**************************�����������**************************/
	CString strData;
	//���վ��
	CString strEntryST;
	CString strTempEnST(_T("00000000000000000000"));
	int iSTLen = _tcslen(GetGlobalApp()->m_pCurrentDebitInfo.EntryStationID);
	if (63 == GetGlobalApp()->m_sTranstParam.ProvinceID)
	{
		if (iSTLen < 7)
		{
			strEntryST.Format(_T("%s%s"), strTempEnST.Left(7 - iSTLen), GetGlobalApp()->m_pCurrentDebitInfo.EntryStationID);
		}
		else
		{
			strEntryST.Format(_T("%s"), GetGlobalApp()->m_pCurrentDebitInfo.EntryStationID);
		}
	}
	else
	{
		if (iSTLen < 10)
		{
			strEntryST.Format(_T("%s%s"), strTempEnST.Left(10 - iSTLen), GetGlobalApp()->m_pCurrentDebitInfo.EntryStationID);
		}
		else
		{
			strEntryST.Format(_T("%s%s"), strTempEnST.Left(20 - iSTLen), GetGlobalApp()->m_pCurrentDebitInfo.EntryStationID);
		}
	}

	//���ʱ��
	CString strEntryTime;
	CTime cEntryTime(GetGlobalApp()->m_pCurrentDebitInfo.EntryTime);
	strEntryTime.Format(_T("%04d%02d%02d%02d%02d%02d"), cEntryTime.GetYear(), cEntryTime.GetMonth(), cEntryTime.GetDay(), cEntryTime.GetHour(), cEntryTime.GetMinute(), cEntryTime.GetSecond());

	//����վ��
	CString strExitST;
	iSTLen = _tcslen(GetGlobalApp()->m_sTranstParam.LaneConfigInfo.StationID);
	if (63 == GetGlobalApp()->m_sTranstParam.ProvinceID)
	{
		if (iSTLen < 7)
		{
			strExitST.Format(_T("%s%s"), strTempEnST.Left(7 - iSTLen), GetGlobalApp()->m_sTranstParam.LaneConfigInfo.StationID);
		}
		else
		{
			strExitST.Format(_T("%s"), GetGlobalApp()->m_sTranstParam.LaneConfigInfo.StationID);
		}
	}
	else
	{
		if (iSTLen < 10)
		{
			strExitST.Format(_T("%s%s"), strTempEnST.Left(10 - iSTLen), GetGlobalApp()->m_sTranstParam.LaneConfigInfo.StationID);
		}
		else
		{
			strExitST.Format(_T("%s%s"), strTempEnST.Left(20 - iSTLen), GetGlobalApp()->m_sTranstParam.LaneConfigInfo.StationID);
		}
	}

	//����ʱ��
	CString strExitTime;
	CTime cExitTime(GetGlobalApp()->m_pCurrentDebitInfo.SubTime);
	strExitTime.Format(_T("%04d%02d%02d%02d%02d%02d"), cExitTime.GetYear(), cExitTime.GetMonth(), cExitTime.GetDay(), cExitTime.GetHour(), cExitTime.GetMinute(), cExitTime.GetSecond());

	//���ڳ���
	CString strExitLane;
	int iLaneLen = _tcslen(GetGlobalApp()->m_sTranstParam.LaneConfigInfo.LaneID);
	if (   (44 == GetGlobalApp()->m_sTranstParam.ProvinceID)
	        || (61 == GetGlobalApp()->m_sTranstParam.ProvinceID)
	        || (62 == GetGlobalApp()->m_sTranstParam.ProvinceID)
	        || (63 == GetGlobalApp()->m_sTranstParam.ProvinceID)
	   )
	{
		if (iLaneLen < 3)
		{
			strExitLane.Format(_T("%s%s"), strTempEnST.Left(3 - iLaneLen), GetGlobalApp()->m_sTranstParam.LaneConfigInfo.LaneID);
		}
		else
		{
			strExitLane.Format(_T("%s"), GetGlobalApp()->m_sTranstParam.LaneConfigInfo.LaneID);
		}
	}
	else
	{
		if (iLaneLen < 8)
		{
			strExitLane.Format(_T("%s%s"), strTempEnST.Left(8 - iLaneLen), GetGlobalApp()->m_sTranstParam.LaneConfigInfo.LaneID);
		}
		else
		{
			strExitLane.Format(_T("%s"), GetGlobalApp()->m_sTranstParam.LaneConfigInfo.LaneID);
		}
	}

	//���⿨��
	CString strVirtualCardID;

	if (   (44 == GetGlobalApp()->m_sTranstParam.ProvinceID)
	        || (62 == GetGlobalApp()->m_sTranstParam.ProvinceID)
	   )
	{
		strVirtualCardID.Format(_T("%s"), _T("00000000000000000000"));
	}
	else
	{
		if (GetGlobalApp()->m_pCurrentDebitInfo.CardID.IsEmpty())
		{
			strVirtualCardID.Format(_T("%s"), _T("00000000000000000000"));
		}
		else
		{
			CString strCardTemp;
			strCardTemp.Format(_T("%s"), _T("00000000000000000000"));
			if (GetGlobalApp()->m_pCurrentDebitInfo.CardID.GetLength() < 20)
			{
				int iRemainCount = 20 - GetGlobalApp()->m_pCurrentDebitInfo.CardID.GetLength();
				strVirtualCardID.Format(_T("%s%s"), strCardTemp.Left(iRemainCount), GetGlobalApp()->m_pCurrentDebitInfo.CardID);
			}
			else
			{
				CString strVT(GetGlobalApp()->m_pCurrentDebitInfo.CardID);
				strVT.TrimLeft();
				strVT.TrimRight();
				strVirtualCardID = strVT;
			}
		}
	}

	//���׽��
	CString strTradeMoney;
	strTradeMoney.Format(_T("%010d"), GetGlobalApp()->m_pCurrentDebitInfo.Money);

	strData.Format(_T("%s%s%s%s%s%s%s"), strEntryST, strEntryTime, strExitST, strExitTime, strExitLane, strVirtualCardID, strTradeMoney);

	CString debug_data;
	debug_data.Format(_T("%s@%s@%s@%s@%s@%s@%s"), strEntryST, strEntryTime, strExitST, strExitTime, strExitLane, strVirtualCardID, strTradeMoney);
	//theApp.RecordLog(debug_data);
	/**************************���ܴ���**************************/

	//AES����
	CAesECB cAesECB(strRealKey);
	CString strEncData = cAesECB.EncryptData(strData);

	

	//MD5У��
	CMD5 md5((LPCSTR)strEncData);
	strMatchCode = md5.GetMD5();
	//theApp.RecordLog("Enc:"+strEncData);
	//theApp.RecordLog("md5:"+strMatchCode);
	//����ƥ����
	return strMatchCode;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// �������ƣ�CThreadManage::GetNetPaySecurityCode
//
// ������������ȡ���װ�ȫ��
//
// ���������void
// ���������const
// ��д��Ա��ROCY
// ��дʱ�䣺2016��8��18��
// �޸���Ա��
// �޸�ʱ�䣺
// �����汾��1.0.0.1
// ��ע˵����
//
////////////////////////////////////////////////////////////////////////////////////////////////
const CString CThreadManage::GetNetPaySecurityCode(void)
{
	return GetNetPayRecordMatchCode();
}

////////////////////////////////////////////////////////////////////////////////////////////////
// �������ƣ�CThreadManage::CheckAndDeleteLogFile
//
// ����������������ɾ����־
//
// ���������void
// ���������void
// ��д��Ա��ROCY
// ��дʱ�䣺2016��8��27��
// �޸���Ա��
// �޸�ʱ�䣺
// �����汾��1.0.0.1
// ��ע˵����
//
////////////////////////////////////////////////////////////////////////////////////////////////
void CThreadManage::CheckAndDeleteLogFile(void)
{
	CFileFindExt cFindFile;
	CString strDir;
	CString strExtName(_T("log"));
	strDir.Format(_T("%s"), ::GetGlobalApp()->m_sTranstParam.PathLogDir);
	try
	{
		if (!cFindFile.SearchFiles(strDir, strExtName))
			throw - 1;

		int iDeleteCount = 0;
		CTime cCurrentTime = CTime::GetCurrentTime();
		for (int i = 0; i < cFindFile.GetSize(); ++i)
		{
			CString strFilePath;
			CString strFileName = cFindFile.GetAt(i);
			strFilePath.Format(_T("%s\\%s"), strDir, strFileName);

			int iYear = 0;
			int iMon  = 0;
			int iDay  = 0;
			if ( 15 == strFileName.GetLength() )
			{
				iYear = _ttoi(strFileName.Mid(3, 4));
				iMon  = _ttoi(strFileName.Mid(7, 2));
				iDay  = _ttoi(strFileName.Mid(9, 2));
			}
			else
			{
				BOOL bNeedProcess = FALSE;
				CString strDate;
				if (strFileName.GetLength() > 15)
				{
					if (0 == strFileName.Left(4).CompareNoCase(_T("BASE")))
					{
						bNeedProcess = TRUE;
					}
					else if (0 == strFileName.Left(6).CompareNoCase(_T("UPDATE")))
					{
						bNeedProcess = TRUE;
					}
				}

				if (bNeedProcess)
				{
					strDate = strFileName.Right(8);

					iYear = _ttoi(strDate.Mid(0, 4));
					iMon  = _ttoi(strDate.Mid(4, 2));
					iDay  = _ttoi(strDate.Mid(6, 2));
				}
			}
			
			//add 2018-04 zhuyabing
			struct stat statbuf; 
			if(stat(strFilePath,&statbuf)==0)
			{
				struct tm time_tm =  *localtime(&statbuf.st_mtime);
				//CString info_str;
				//info_str.Format("%d-%d-%d",time_tm.tm_year,time_tm.tm_mon,time_tm.tm_mday);
				iYear = 1900+time_tm.tm_year;
				iMon = 1+time_tm.tm_mon;
				iDay = time_tm.tm_mday;
				CString file_str;
				file_str.Format("%s[]%d-%d-%d",strFilePath,iYear,iMon,iDay);
				//MessageBox(0,file_str,"",0);
				
			}
			
			
			
			if ( (0 != iYear) && (0 != iMon) && (0 != iDay) )
			{
				CTime cFileTime(iYear, iMon, iDay, 0, 0, 0);
				//��ȡ���ļ���ʱ��������ʱ����������
				CTimeSpan span = cCurrentTime - cFileTime ;
				
				int iSpanDay = span.GetDays();
				//iSpanDay = (cCurrentTime.GetTime() - cFileTime.GetTime())/(24*60*60);

				
				if (iSpanDay < GetGlobalApp()->m_iDeleteLogSaveDays)
					continue;
				
				if (!::PathFileExists(strFilePath))
					throw - 3;
			
				::DeleteFile(strFilePath);
				if (++iDeleteCount >= 20)
				{
					break;
				}
			}
		}
		//�������
		cFindFile.Close();
	}
	catch (int& iErrorCode)
	{
		switch (iErrorCode)
		{
		case -1:
			break;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////
// �������ƣ�CThreadDevice::StartWorkThreads
//
// �������������������߳�
//
// ���������IN const tagTranstParam& sTranstParam
// ���������const
// ��д��Ա��ROCY
// ��дʱ�䣺2016��7��24��
// �޸���Ա��
// �޸�ʱ�䣺
// �����汾��1.0.0.1
// ��ע˵����
//
////////////////////////////////////////////////////////////////////////////////////////////////
const BOOL CThreadManage::StartWorkThreads(IN const tagTranstParam& sTranstParam)
{
	theApp.RecordLog(">>>>>StartWorkThreads...");
	BOOL bReturn = FALSE;

	m_PayCenter.Init();
	m_MediaCenter.Init();
	
// //for debug
// 	bing_Debug = NULL; 
// 	//����֧����ƽ̨ͨѶ�߳�
// 	bing_Debug = (CThreadDevice_ForDebug *)::AfxBeginThread(RUNTIME_CLASS(CThreadDevice_ForDebug), THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	
// 	if (NULL == bing_Debug)
// 		return false ;
// 	bing_Debug->m_bAutoDelete = true;
// 	//�����߳����»���
// 	bing_Debug->ResumeThread();

// 	//��Ĭ200����
// 	Sleep(200); 
// 	return true;
	try
	{
		/*******************������Ӧ�߳�*******************/
		CString strConfigFilePath;
		//����Ӳ�������߳�
		m_pThreadDevice = (CThreadDevice *)::AfxBeginThread(RUNTIME_CLASS(CThreadDevice), THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
		if (NULL == m_pThreadDevice)
			throw - 2;
		tagTranstParam sTranstParam = {0};
		memcpy(&sTranstParam, &m_sTranstParam, sizeof(tagTranstParam));

		sTranstParam.CallerThreadID    = m_nThreadID;
		sTranstParam.ActiveNotifyMsgID = WM_THREAD_DEVICE_NOTIFY;
		strConfigFilePath.Format(_T("%s\\%s"), GetGlobalApp()->m_strWorkDir, FILE_NAME_DEVICE_CONFIG);
		m_pThreadDevice->SetLocalConfigFilePath(strConfigFilePath);
		m_pThreadDevice->SetParam(&sTranstParam, NULL);
		//�����߳����»���
		m_pThreadDevice->ResumeThread();
		theApp.RecordLog(">>>>ThreadDevice has been statrted...");
		//��Ĭ200����
		Sleep(200);

		//����֧����ƽ̨ͨѶ�߳�
		m_pThreadHttp = (CThreadHttp *)::AfxBeginThread(RUNTIME_CLASS(CThreadHttp), THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
		if (NULL == m_pThreadHttp)
			throw - 1;

		sTranstParam.CallerThreadID    = m_nThreadID;
		sTranstParam.ActiveNotifyMsgID = WM_THREAD_MANAGE_DEAL_PLATFORM_RESPONSE_RESULT;
		strConfigFilePath.Format(_T("%s\\%s"), GetGlobalApp()->m_strWorkDir, FILE_NAME_HTTP_CONFIG);
		m_pThreadHttp->SetLocalConfigFilePath(strConfigFilePath);
		m_pThreadHttp->SetParam(&sTranstParam, NULL);
		//�����߳����»���
		m_pThreadHttp->ResumeThread();
		//��Ĭ200����
		Sleep(200);

		m_pThreadUpdate = (CThreadUpdate *)::AfxBeginThread(RUNTIME_CLASS(CThreadUpdate), THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
		if (NULL == m_pThreadUpdate)
			throw - 3;
		sTranstParam.CallerThreadID    = m_nThreadID;
		sTranstParam.ActiveNotifyMsgID = WM_THREAD_UPDATE_RESPONSE;
		strConfigFilePath.Format(_T("%s\\%s"), GetGlobalApp()->m_strWorkDir, FILE_NAME_UPDATE_CONFIG);
		m_pThreadUpdate->SetLocalConfigFilePath(strConfigFilePath);
		m_pThreadUpdate->SetParam(&sTranstParam, NULL);
		m_pThreadUpdate->ResumeThread();
		//��Ĭ200����
		::Sleep(200);

		//��������ǩ���߳�
		m_pThreadNFCSignIn = (CThreadNFCSignIn *)::AfxBeginThread(RUNTIME_CLASS(CThreadNFCSignIn), THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
		if (NULL == m_pThreadNFCSignIn)
			throw - 2;
		sTranstParam.CallerThreadID    = m_nThreadID;
		sTranstParam.ActiveNotifyMsgID = 0x00;
		strConfigFilePath.Format(_T("%s\\%s"), GetGlobalApp()->m_strWorkDir, FILE_NAME_APP_CONFIG);
		m_pThreadNFCSignIn->SetLocalConfigFilePath(strConfigFilePath);
		m_pThreadNFCSignIn->SetParam(&sTranstParam, NULL);
		//�����߳����»���
		m_pThreadNFCSignIn->ResumeThread();
		//��Ĭ200����
		Sleep(200);
	}
	catch (int& iErrorCode)
	{
		switch (iErrorCode)
		{
		case -1:
			break;
		case -2:
			theApp.RecordLog("ThreadDevice�߳�����ʧ�ܣ�");
			break;
		case -3:
			break;
		}
	}
	return bReturn;
}


////////////////////////////////////////////////////////////////////////////////////////////////
// �������ƣ�CThreadManage::StartTimer
//
// �������������ö�ʱ��
//
// ���������IN const UINT& nEventID
//         ��IN const DWORD& dwDelayTime
// ���������void
// ��д��Ա��ROCY
// ��дʱ�䣺2016��8��3��
// �޸���Ա��
// �޸�ʱ�䣺
// �����汾��1.0.0.1
// ��ע˵����
//
////////////////////////////////////////////////////////////////////////////////////////////////
void CThreadManage::StartTimer(IN const UINT& nEventID, IN const DWORD& dwDelayTime)
{
	switch (nEventID)
	{
	case TIMER_ID_DEBIT_MONEY://�ۿ����̶�ʱ��
	{
		if (0 == m_nTimerIDForDebitMoney)
		{
			TIMECAPS   tc;
			//���ú���timeGetDevCapsȡ��ϵͳ�ֱ��ʵ�ȡֵ��Χ������޴������
			if (timeGetDevCaps(&tc, sizeof(TIMECAPS)) == TIMERR_NOERROR)
			{
				//�ֱ��ʵ�ֵ���ܳ���ϵͳ��ȡֵ��Χ
				m_nAccuracy = min(max(tc.wPeriodMin, 1), tc.wPeriodMax);
				//����timeBeginPeriod�������ö�ʱ���ķֱ���
				timeBeginPeriod(m_nAccuracy);
				//���ÿۿ����̶�ʱ��
				m_nTimerIDForDebitMoney = timeSetEvent(
				                              dwDelayTime,						//��ʱ(MS)
				                              m_nAccuracy,						//�ӳٷֱ���
				                              (LPTIMECALLBACK)OneMilliSecondProc,	//�ص�����
				                              (DWORD)this,						//ʹ�ûص������л��ᴦ���������������
				                              TIME_ONESHOT);						//ֻ����һ��(TIME_PERIODIC:���ڵ���)
			}
		}
	}
	break;
	case TIMER_ID_SCAN_CODE://��ȡ�û���Ϣ��ʱ��
	{
		if (0 == m_nTimerIDForScanCode)
		{
			TIMECAPS   tc;
			//���ú���timeGetDevCapsȡ��ϵͳ�ֱ��ʵ�ȡֵ��Χ������޴������
			if (timeGetDevCaps(&tc, sizeof(TIMECAPS)) == TIMERR_NOERROR)
			{
				//�ֱ��ʵ�ֵ���ܳ���ϵͳ��ȡֵ��Χ
				m_nAccuracy = min(max(tc.wPeriodMin, 1), tc.wPeriodMax);
				//����timeBeginPeriod�������ö�ʱ���ķֱ���
				timeBeginPeriod(m_nAccuracy);
				//���û�ȡ�û���Ϣ��ʱ��
				m_nTimerIDForScanCode = timeSetEvent(
				                            dwDelayTime,						//��ʱ(MS)
				                            m_nAccuracy,						//�ӳٷֱ���
				                            (LPTIMECALLBACK)OneMilliSecondProc,	//�ص�����
				                            (DWORD)this,						//ʹ�ûص������л��ᴦ���������������
				                            TIME_ONESHOT);						//ֻ����һ��(TIME_PERIODIC:���ڵ���)
			}
		}
	}
	break;
	case TIMER_ID_DEBIT_CHECK://�ۿ�״̬��ѯ��ʱ��
	{
		if (0 == m_nTimerIDForCheckDebitState)
		{
			TIMECAPS   tc;
			//���ú���timeGetDevCapsȡ��ϵͳ�ֱ��ʵ�ȡֵ��Χ������޴������
			if (timeGetDevCaps(&tc, sizeof(TIMECAPS)) == TIMERR_NOERROR)
			{
				//�ֱ��ʵ�ֵ���ܳ���ϵͳ��ȡֵ��Χ
				m_nAccuracy = min(max(tc.wPeriodMin, 1), tc.wPeriodMax);
				//����timeBeginPeriod�������ö�ʱ���ķֱ���
				timeBeginPeriod(m_nAccuracy);
				//���ÿۿ�״̬��ѯ��ʱ��
				m_nTimerIDForCheckDebitState = timeSetEvent(
				                                   dwDelayTime,						//��ʱ(MS)
				                                   m_nAccuracy,						//�ӳٷֱ���
				                                   (LPTIMECALLBACK)OneMilliSecondProc,	//�ص�����
				                                   (DWORD)this,						//ʹ�ûص������л��ᴦ���������������
				                                   TIME_ONESHOT);						//ֻ����һ��(TIME_PERIODIC:���ڵ���)
			}
		}
	}
	break;
	case TIMER_ID_NFC_WAIT_CORRECT_DATA://NFC�������ݻ�ȡ��ʱ��
	{
		if (0 == m_nTimerIDForNFCGetCorrentData)
		{
			TIMECAPS   tc;
			//���ú���timeGetDevCapsȡ��ϵͳ�ֱ��ʵ�ȡֵ��Χ������޴������
			if (timeGetDevCaps(&tc, sizeof(TIMECAPS)) == TIMERR_NOERROR)
			{
				//�ֱ��ʵ�ֵ���ܳ���ϵͳ��ȡֵ��Χ
				m_nAccuracy = min(max(tc.wPeriodMin, 1), tc.wPeriodMax);
				//����timeBeginPeriod�������ö�ʱ���ķֱ���
				timeBeginPeriod(m_nAccuracy);
				//����NFC�������ݻ�ȡ��ʱ��
				m_nTimerIDForNFCGetCorrentData = timeSetEvent(
				                                     dwDelayTime,						//��ʱ(MS)
				                                     m_nAccuracy,						//�ӳٷֱ���
				                                     (LPTIMECALLBACK)OneMilliSecondProc,	//�ص�����
				                                     (DWORD)this,						//ʹ�ûص������л��ᴦ���������������
				                                     TIME_ONESHOT);						//ֻ����һ��(TIME_PERIODIC:���ڵ���)
			}
		}
	}
	break;
	case TIMER_ID_NFC_WAIT_NORMAL_DATA://NFC���ݣ��������������ȴ���ʱ��ȡ��ʱ��
	{
		if (0 == m_nTimerIDForNFCGetNormalData)
		{
			TIMECAPS   tc;
			//���ú���timeGetDevCapsȡ��ϵͳ�ֱ��ʵ�ȡֵ��Χ������޴������
			if (timeGetDevCaps(&tc, sizeof(TIMECAPS)) == TIMERR_NOERROR)
			{
				//�ֱ��ʵ�ֵ���ܳ���ϵͳ��ȡֵ��Χ
				m_nAccuracy = min(max(tc.wPeriodMin, 1), tc.wPeriodMax);
				//����timeBeginPeriod�������ö�ʱ���ķֱ���
				timeBeginPeriod(m_nAccuracy);
				//����NFC�������ݻ�ȡ��ʱ��
				m_nTimerIDForNFCGetNormalData = timeSetEvent(
				                                    dwDelayTime,						//��ʱ(MS)
				                                    m_nAccuracy,						//�ӳٷֱ���
				                                    (LPTIMECALLBACK)OneMilliSecondProc,	//�ص�����
				                                    (DWORD)this,						//ʹ�ûص������л��ᴦ���������������
				                                    TIME_ONESHOT);						//ֻ����һ��(TIME_PERIODIC:���ڵ���)
			}
		}
	}
	break;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////
// �������ƣ�CThreadManage::StopTimer
//
// ����������ֹͣ��ʱ��
//
// ���������IN const UINT& nEventID
// ���������void
// ��д��Ա��ROCY
// ��дʱ�䣺2016��9��4��
// �޸���Ա��
// �޸�ʱ�䣺
// �����汾��1.0.0.1
// ��ע˵����
//
////////////////////////////////////////////////////////////////////////////////////////////////
void CThreadManage::StopTimer(IN const UINT& nEventID)
{
	switch (nEventID)
	{
	case TIMER_ID_DEBIT_MONEY://�ۿ����̶�ʱ��
	{
		if (0 != m_nTimerIDForDebitMoney)
		{
			timeKillEvent(m_nTimerIDForDebitMoney);
			m_nTimerIDForDebitMoney = 0;
		}
	}
	break;
	case TIMER_ID_SCAN_CODE://��ȡ�û���Ϣ��ʱ��
	{
		if (0 == m_nTimerIDForScanCode)
		{
			timeKillEvent(m_nTimerIDForScanCode);
			m_nTimerIDForScanCode = 0;
		}
	}
	break;
	case TIMER_ID_DEBIT_CHECK://�ۿ�״̬��ѯ��ʱ��
	{
		if (0 != m_nTimerIDForCheckDebitState)
		{
			timeKillEvent(m_nTimerIDForCheckDebitState);
			m_nTimerIDForCheckDebitState = 0;
		}
	}
	break;
	case TIMER_ID_NFC_WAIT_NORMAL_DATA://NFC��ͨ���ݻ�ȡ��ʱ��ֹ��ʱ��
	{
		if (0 != m_nTimerIDForNFCGetNormalData)
		{
			timeKillEvent(m_nTimerIDForNFCGetNormalData);
			m_nTimerIDForNFCGetNormalData = 0;
		}
	}
	break;
	case TIMER_ID_NFC_WAIT_CORRECT_DATA://NFC�������ݻ�ȡ��ʱ��ֹ��ʱ��
	{
		if (0 != m_nTimerIDForNFCGetCorrentData)
		{
			timeKillEvent(m_nTimerIDForNFCGetCorrentData);
			m_nTimerIDForNFCGetCorrentData = 0;
		}
	}
	break;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////
// �������ƣ�CThreadManage::FinishTrade
//
// �������������׽����Ĵ���
//
// ���������IN const BOOL& bNormal -- �������쳣��������
// ���������CString �ôν��׵�ʶ����
// ��д��Ա��ROCY
// ��дʱ�䣺2016��9��11��
// �޸���Ա��
// �޸�ʱ�䣺
// �����汾��1.0.0.1
// ��ע˵����
//
////////////////////////////////////////////////////////////////////////////////////////////////
CString CThreadManage::FinishTrade(IN const BOOL& bNormal)
{
	CString strReturn = GetGlobalApp()->m_pCurrentDebitInfo.PayIdentifier;
	//ֹͣ���մ���֧����ƽ̨����������
	GetGlobalApp()->m_bIsTrading		= FALSE;
	GetGlobalApp()->m_bCurrentDebitMode = FALSE;

	//�رտۿʱ��ʱ��
	StopTimer(TIMER_ID_DEBIT_MONEY);
	//�رռ��ۿ�״̬��ʱ��
	StopTimer(TIMER_ID_DEBIT_CHECK);
	//����ۿ�״̬��ѯ�ṹ��
	if (NULL != m_pDVJsonDebitCheck)
	{
		delete m_pDVJsonDebitCheck;
		m_pDVJsonDebitCheck = NULL;
	}
	m_sDVJsonDebit.Reset();

	m_pThreadHttp->m_bLoopBreak = TRUE;	//��Ҫ�����۷�ִ��ѭ��

	//��ʱ�������µĽ��׽�������Ҫ�Զ����𳷵�
	if (!bNormal)
	{
		//�����ۿ������Դ
		GetGlobalApp()->m_pCurrentDebitInfo.Valid	= FALSE;
		GetGlobalApp()->m_sCurrentDebitResult.Valid = FALSE;
		//���쳷������
		CDVJsonDebitCancel* pDVJsonDebitCancle = new CDVJsonDebitCancel(&m_sDVJsonDebit);
		if (NULL != pDVJsonDebitCancle)
		{
			pDVJsonDebitCancle->ProvinceID	  = ::GetGlobalApp()->m_sTranstParam.ProvinceID;
			pDVJsonDebitCancle->Version		  = _T("1.0");
			pDVJsonDebitCancle->SubmitTime	  = GetGlobalApp()->GetCurrentTimeString(1);
			//���ó�������
			pDVJsonDebitCancle->PackageType   = JSON_DT_SEND_DIBIT_CANCEL;
			//��ȡ����ʶ����
			pDVJsonDebitCancle->PayIdentifier = GetGlobalApp()->m_pCurrentDebitInfo.PayIdentifier;
			((CDVJsonBase*)pDVJsonDebitCancle)->PayIdentifier = GetGlobalApp()->m_pCurrentDebitInfo.PayIdentifier;
			//���ͳ�������
			m_pThreadHttp->PostThreadMessage(WM_THREAD_HTTP_SEND_DATA_REQUEST, (WPARAM)pDVJsonDebitCancle, (LPARAM)MAKELONG(JSON_DT_SEND_DIBIT_CANCEL, MAKEWORD(0x00, 0x00)));
		}
	}
	//��ʶ�ۿ����̽���
	if (GetGlobalApp()->m_bCurrentDebitMode)
	{
		GetGlobalApp()->m_iDebitTaskNo = 0x00;
	}

	return strReturn;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// �������ƣ�CThreadManage::FinishTrade
//
// �������������׽����Ĵ���
//
// ���������IN const CDVJsonBase* pDVJsonBase -- ���ؽ��׵Ļ�����Ϣ�ṹָ��
// �����������
// ��д��Ա��ROCY
// ��дʱ�䣺2017��2��16��
// �޸���Ա��
// �޸�ʱ�䣺
// �����汾��1.0.0.1
// ��ע˵����
//
////////////////////////////////////////////////////////////////////////////////////////////////
void CThreadManage::FinishHadCancelTrade(IN const CDVJsonBase* pDVJsonBase)
{
	//�رտۿʱ��ʱ��
	StopTimer(TIMER_ID_DEBIT_MONEY);
	//�رռ��ۿ�״̬��ʱ��
	StopTimer(TIMER_ID_DEBIT_CHECK);
	//����ۿ�״̬��ѯ�ṹ��
	if (NULL != m_pDVJsonDebitCheck)
	{
		delete m_pDVJsonDebitCheck;
		m_pDVJsonDebitCheck = NULL;
	}
	m_sDVJsonDebit.Reset();

	if (NULL != pDVJsonBase)
	{
		m_pThreadHttp->m_bLoopBreak = TRUE;	//��Ҫ�����۷�ִ��ѭ��

		//���쳷������
		CDVJsonDebitCancel* pDVJsonDebitCancle = new CDVJsonDebitCancel(&m_sDVJsonDebit);
		if (NULL != pDVJsonDebitCancle)
		{
			pDVJsonDebitCancle->ProvinceID	  = pDVJsonBase->ProvinceID;
			pDVJsonDebitCancle->Version		  = _T("1.0");
			pDVJsonDebitCancle->SubmitTime	  = GetGlobalApp()->GetCurrentTimeString(1);
			//���ó�������
			pDVJsonDebitCancle->PackageType   = JSON_DT_SEND_DIBIT_CANCEL;
			//��ȡ����ʶ����
			pDVJsonDebitCancle->PayIdentifier = pDVJsonBase->PayIdentifier;
			((CDVJsonBase*)pDVJsonDebitCancle)->PayIdentifier = pDVJsonBase->PayIdentifier;
			//���ͳ�������
			m_pThreadHttp->PostThreadMessage(WM_THREAD_HTTP_SEND_DATA_REQUEST, (WPARAM)pDVJsonDebitCancle, (LPARAM)MAKELONG(JSON_DT_SEND_DIBIT_CANCEL, MAKEWORD(0x00, 0x00)));
		}

		//��ʶ�ۿ����̽���
		if (GetGlobalApp()->m_bCurrentDebitMode)
		{
			GetGlobalApp()->m_iDebitTaskNo = 0x00;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////
// �������ƣ�CThreadManage::OneMilliSecondProc
//
// ������������ʱ����Ӧ����
//
// ���������UINT wTimerID	-- ��ʱ��ID
//         ��UINT msg		-- ��ʱ����Ϣ
//         ��DWORD dwUser	-- ������ָ��
//         ��DWORD dwl		-- ��ʱ������1
//         ��DWORD dw2		-- ��ʱ������2
// ���������void
// ��д��Ա��ROCY
// ��дʱ�䣺2016��8��3��
// �޸���Ա��
// �޸�ʱ�䣺
// �����汾��1.0.0.1
// ��ע˵����
//
////////////////////////////////////////////////////////////////////////////////////////////////
void CALLBACK CThreadManage::OneMilliSecondProc(UINT nTimerID, UINT msg, DWORD dwUser, DWORD dwl, DWORD dw2)
{
	if (((CThreadManage*)dwUser)->m_nTimerIDForScanCode == nTimerID)
	{
		//�ر�ɨ�볬ʱ��ʱ��
		((CThreadManage*)dwUser)->StopTimer(TIMER_ID_SCAN_CODE);
	}
	else if (((CThreadManage*)dwUser)->m_nTimerIDForDebitMoney == nTimerID)	//������ָ����ʱʱ���ѵ�
	{
		//�رռ��ۿ�״̬��ʱ��
		((CThreadManage*)dwUser)->StopTimer(TIMER_ID_DEBIT_MONEY);

		//������Ϊ��ʱ���������
		CString strTradeID = ((CThreadManage*)dwUser)->FinishTrade(FALSE);

		//���ý�������
		((CThreadManage*)dwUser)->m_pThreadDevice->PostThreadMessage(WM_THREAD_DEVICE_CONTROL_DEAL_RESULT_MMI, (WPARAM)0x02, (LPARAM)0);

		//��¼��־
		GetGlobalApp()->RecordLog(GetGlobalApp()->FmtStr(_T("[ҵ������߳�]:�ۿ������ʱ������Ԥ����ʱ,%d �룩���Զ����ó������������ױ�ʶ:%s"), ::GetGlobalApp()->m_pCurrentDebitInfo.OverTime / 1000, strTradeID));

		//֪ͨ�����߷��ؽ������ǰ����ʧ��
		::GetGlobalApp()->NotifyCaller(0x01, RCC_DEBIT_OT);
	}
	else if (((CThreadManage*)dwUser)->m_nTimerIDForCheckDebitState == nTimerID) //�û��������룬��ⳬʱ
	{
		//�رռ��ۿ�״̬��ʱ��
		((CThreadManage*)dwUser)->StopTimer(TIMER_ID_DEBIT_CHECK);

		//add 2017-12 �ж��Ƿ����ڽ�����
		if(!GetGlobalApp()->m_bIsTrading)
		{
			GetGlobalApp()->RecordLog(_T("δ���ڽ����У�ֹͣ��ѯ"));
			return ;
		}

		//��ȡ��ѯ�Ѿ�ʹ�õ�ʱ��
		((CThreadManage*)dwUser)->m_dwCheckDebitAccumulateTime += ((CThreadManage*)dwUser)->m_dwCheckDebitSpanTime;

		//�ж��Ƿ�ʱ
		if (((CThreadManage*)dwUser)->m_dwCheckDebitAccumulateTime >= ((CThreadManage*)dwUser)->m_dwInputPasswordDelayTime)
		{
			//������Ϊ��ʱ���������
			CString strTradeID = ((CThreadManage*)dwUser)->FinishTrade(FALSE);

			//���ý�������
			((CThreadManage*)dwUser)->m_pThreadDevice->PostThreadMessage(WM_THREAD_DEVICE_CONTROL_DEAL_RESULT_MMI, (WPARAM)0x02, (LPARAM)0);

			//��¼��־
			GetGlobalApp()->RecordLog(GetGlobalApp()->FmtStr(_T("[ҵ������߳�]:�ۿ������ʱ����������ۼ�,%d �����Զ����ó������������ױ�ʶ:%s"), ((CThreadManage*)dwUser)->m_dwCheckDebitAccumulateTime, strTradeID));

			//֪ͨ�����߷��ؽ������ǰ����ʧ��
			::GetGlobalApp()->NotifyCaller(0x01, RCC_DEBIT_OT);
		}
		else
		{
			//��ֹ����֧��״̬���ṹ��ָ��Ƿ����ã���Ҫ���Ӽ��
			if (NULL != ((CThreadManage*)dwUser)->m_pDVJsonDebitCheck )
			{
				CDVJsonDebitCheck* pDVJsonDebitCheck = new CDVJsonDebitCheck(((CThreadManage*)dwUser)->m_pDVJsonDebitCheck);
				if (NULL != pDVJsonDebitCheck)
				{
					pDVJsonDebitCheck->PayIdentifier = GetGlobalApp()->m_pCurrentDebitInfo.PayIdentifier;
					((CDVJsonBase*)pDVJsonDebitCheck)->PayIdentifier = GetGlobalApp()->m_pCurrentDebitInfo.PayIdentifier;
					pDVJsonDebitCheck->PackageType  = JSON_DT_SEND_DIBIT_CHECK;
					pDVJsonDebitCheck->SubmitTime   = GetGlobalApp()->GetCurrentTimeString(1);
					//��¼��־
					//add 2018-01 ,�����˶��ܳ�ʱʱ�����־�����
					GetGlobalApp()->RecordLog(GetGlobalApp()->FmtStr(_T("[ҵ������߳�]:�ۿ������ʱδ���������ۼ� %d ����/%d�������ݹ����Զ�������һ�εĿۿ�״̬��⣬���ױ�ʶ:%s"), ((CThreadManage*)dwUser)->m_dwCheckDebitAccumulateTime,((CThreadManage*)dwUser)->m_dwInputPasswordDelayTime, pDVJsonDebitCheck->PayIdentifier));
					((CThreadManage*)dwUser)->m_pThreadHttp->PostThreadMessage(WM_THREAD_HTTP_SEND_DATA_REQUEST, (WPARAM)pDVJsonDebitCheck, (LPARAM)MAKELONG(JSON_DT_SEND_DIBIT_CHECK, 0x01));
				}
				//֪ͨ��һ�β�ѯ
				((CThreadManage*)dwUser)->PostThreadMessage(WM_THREAD_MANAGE_CHECK_DEBIT_PASSWORD_STATE, (WPARAM)0x00, (LPARAM)0x00);
			}
		}
	}
	else if (((CThreadManage*)dwUser)->m_nTimerIDForNFCGetCorrentData == nTimerID) //NFC֧������ȡ�������ݳ�ʱ��ֹ
	{
		//�رռ��ۿ�״̬��ʱ��
		((CThreadManage*)dwUser)->StopTimer(TIMER_ID_NFC_WAIT_CORRECT_DATA);

		//�ж��Ƿ�������û�г�������Ҳ�ܹ�����NFC�۷ѣ����ƶ˷�������
		if (GetGlobalApp()->m_bNFCIsAllowDebitWithoutCancelData)
		{
			//����۷�����
			((CThreadManage*)dwUser)->ContructDebitMoneyInfo();
		}
		else
		{
			GetGlobalApp()->RecordLog(GetGlobalApp()->FmtStr(_T("[ҵ������߳�]:NFC֧�����̣���ȡ�������ݳ�ʱ���ж�����ʧ�ܣ����ױ�ʶ:%s"), GetGlobalApp()->m_pCurrentDebitInfo.PayIdentifier));
			//��ȡ��ʱ������ʧ��(���ڴ�ʱû�еݽ������ƶˣ�����Ҫ�Զ�������
			((CThreadManage*)dwUser)->FinishTrade(TRUE);
			//֪ͨ�����߷��ؽ������ǰ����ʧ��
			::GetGlobalApp()->NotifyCaller(0x01, RCC_DEBIT_OT);
		}
	}
	else if (((CThreadManage*)dwUser)->m_nTimerIDForNFCGetNormalData == nTimerID) //NFC֧������ȡ��ͨ���ݣ�������������ʱ��ֹ
	{
		//�رռ��ۿ�״̬��ʱ��
		((CThreadManage*)dwUser)->StopTimer(TIMER_ID_NFC_WAIT_NORMAL_DATA);

		switch (::GetGlobalApp()->GetUnipayWorkFlowState())
		{
		case UMS_DEBITING://�����ǰ����������
			//ֱ���ж�����ʧ�ܣ�֪ͨ������
			((CThreadManage*)dwUser)->NFCLocalTradeFailProcess();
			break;
		case UMS_SIGNING://�����ǰ��ǩ������
			//�ж�ǩ��ʧ�ܣ��ȴ��´�����ǩ��
		{
			//��¼��־
			CString strLog;
			strLog.Format(_T("[ҵ������߳�]:NFC�ǽ�֧�����̣��ȴ�ǩ�����ݳ�ʱ��ֹ��ǩ������ʧ��(ģ�����ݽ�������:%d)"), GetGlobalApp()->m_iNFCSignFlowDataCount);
			GetGlobalApp()->RecordLog(strLog);
			//����ǩ��״̬Ϊ��
			GetGlobalApp()->m_bNFCSignInSuccess = false;
			//����NFC֧��״̬
			GetGlobalApp()->m_bNFCPayFail = FALSE;
			GetGlobalApp()->m_iNFCPayFailCount = 0;
			//����NFCǩ����������
			GetGlobalApp()->m_iNFCSignFlowDataCount = 0;
			//����NFCģ�鵱ǰ��������
			GetGlobalApp()->SetUnipayWorkFlowState(UMS_UNKNOW);
		}
		break;
		default:		 //����
			break;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////
// �������ƣ�CThreadManage::SetAccountDataReadyState
//
// ���������������û������Ƿ�׼������
//
// ���������IN const BOOL& bState
// ���������void
// ��д��Ա��ROCY
// ��дʱ�䣺2016��9��14��
// �޸���Ա��
// �޸�ʱ�䣺
// �����汾��1.0.0.1
// ��ע˵����
//
////////////////////////////////////////////////////////////////////////////////////////////////
void CThreadManage::SetAccountDataReadyState(IN const BOOL& bState)
{
	m_bAccountInfoIsReady = bState;
	if (!bState)
	{
		memset(&m_sAccountInfo, 0, sizeof(tagAccountInfo));
	}
}



////////////////////////////////////////////////////////////////////////////////////////////////
// �������ƣ�CThreadManage::NFCSignInDataProcessFromHTTP
//
// ����������NFCǩ�����ݴ������������ݴ�HTTP�ش���,ת����POS����
//
// ���������IN const CString& strData
// ���������void
// ��д��Ա��ROCY
// ��дʱ�䣺2017��9��20��
// �޸���Ա��
// �޸�ʱ�䣺
// �����汾��1.0.0.1
// ��ע˵�����μ��ӿ�Э��ĺ����Ĳ���������IssueData
//
////////////////////////////////////////////////////////////////////////////////////////////////
void CThreadManage::NFCSignInDataProcessFromHTTP(IN const CString& strData)
{
	try
	{
		//����ǩ�����ݴ���
		CString* pstrString = new CString();
		if (NULL == pstrString)
			throw - 1;
		if (NULL == m_pThreadDevice)
			throw - 2;
		CNFCProtocol cNFCProtocol;
		pstrString->Format(_T("%s"), cNFCProtocol.ConstructSignDataFromHttpCmd(strData));

		m_pThreadDevice->PostThreadMessage(WM_THREAD_NFC_ISSUE_DATA, (WPARAM)pstrString, (LPARAM)UMS_SIGNING);
	}
	catch (int& iErrorCode)
	{
		switch (iErrorCode)
		{
		case -1:
			break;
		case -2:
			break;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////
// �������ƣ�CThreadManage::NFCSignInDataProcessFromPOS
//
// ����������NFCǩ�����ݴ������������ݴ�POS�ǽ�ģ��ش���
//
// ���������IN CString& strData
// ���������void
// ��д��Ա��ROCY
// ��дʱ�䣺2017��9��20��
// �޸���Ա��
// �޸�ʱ�䣺
// �����汾��1.0.0.1
// ��ע˵����
//
////////////////////////////////////////////////////////////////////////////////////////////////
void CThreadManage::NFCSignInDataProcessFromPOS(IN CNFCProtocol& cNFCProtocol)
{
	CString strLog;
	//ǩ�����̳ɹ����
	BOOL bSuccess = FALSE;
	CString* pstrSendData = NULL;
	try
	{
		GetGlobalApp()->m_iNFCSignFlowDataCount = GetGlobalApp()->m_iNFCSignFlowDataCount + 1;
		//ǩ��ҵ����
		switch (cNFCProtocol.FramrType)
		{
		case UFT_N_SIGN_IN_RES_DATA:
		{
			//�����жϾ����ϴν���ǩ������ʱ�����Ƿ񳬹����ƣ�����������ƣ�ֱ���ж���ǰǩ��ʧ��
			if ( (time(NULL) - GetGlobalApp()->m_tNFCLastSignDataTime) > GetGlobalApp()->m_iNFCSignDataLimitSpanTime)
			{
				strLog.Format(_T("[ҵ������߳�]:NFC�ǽ�֧�����̣�ϵͳ��⵽ǩ�����̵������ݽ���ʱ�����ϳ�����%d�ν��������ϴ����ݽ����������%d�룩���ж�ǩ��ʧ�ܣ�������ʼ����ǩ������"), GetGlobalApp()->m_iNFCSignFlowDataCount, GetGlobalApp()->m_iNFCSignDataLimitSpanTime);
				//����ǩ��ʧ��״̬�������´���ǩ׼��
				GetGlobalApp()->m_bNFCSignInSuccess = false;
				GetGlobalApp()->SetUnipayWorkFlowState(UMS_UNKNOW);
				//�����ϴ�ǩ������
				GetGlobalApp()->m_tNFCLastSignDataTime = time(NULL);
				//ǩ��ʧ�ܣ�Ҫ��POSǿ�ƽ�������
				NFCSendDefaultCmdToPos(0x03);
			}
			else
			{
				if (NULL == m_pThreadNFCSignIn)
					throw - 1;
				pstrSendData = new CString();
				if (NULL == pstrSendData)
					throw - 2;
				//��������ת��
				pstrSendData->Format(_T("%04X%s"), cNFCProtocol.Context.GetLength() / 2, cNFCProtocol.Context);
				//ת��ǩ�����ݵ��ƶ�
				m_pThreadNFCSignIn->PostThreadMessage(WM_THREAD_NFC_SIGN_DATA_RETURN_FROM_POS, (WPARAM)pstrSendData, (LPARAM)0);
				//��¼��־
				strLog.Format(_T("[ҵ������߳�]:NFC�ǽ�֧�����̣�ϵͳ���ڽ���ǩ��������ģ�����ݽ�������:%d"), GetGlobalApp()->m_iNFCSignFlowDataCount);
				//ÿ���յ�POS��ǩ�����ݣ�����Ҫ�����ϴ�ǩ������ʱ�䣬��֤��ʱ������Ч
				GetGlobalApp()->m_tNFCLastSignDataTime = time(NULL);
				//���õȴ���ʱ
//					StartTimer(TIMER_ID_NFC_WAIT_NORMAL_DATA,25000);
			}
		}
		break;
		case UFT_N_SIGN_CONFIRM:
		{
			//��¼��־
			strLog.Format(_T("[ҵ������߳�]:NFC�ǽ�֧�����̣�ϵͳǩ�����ɹ���,��ʱ %d ��(ģ�����ݽ�������:%d)"), (time(NULL) - GetGlobalApp()->m_tNFCStartSignInTime), GetGlobalApp()->m_iNFCSignFlowDataCount);
			GetGlobalApp()->RecordLog(strLog);//modify 2018-06, 0, TRUE);
			strLog.Empty();
			//�����ǩ����������Ӧ�Ŀ���λ
			GetGlobalApp()->m_bNFCSignInSuccess = true;
			//����NFC֧��״̬
			GetGlobalApp()->m_bNFCPayFail = FALSE;
			GetGlobalApp()->m_iNFCPayFailCount = 0;
			//����NFCǩ����������
			GetGlobalApp()->m_iNFCSignFlowDataCount = 0;
			//add zhuyabing 2017-10
			GetGlobalApp()->SetUnipayWorkFlowState(UMS_UNKNOW);

			//�ж��Ƿ�ÿ����ǩ�Ѿ���ɣ���ָ��ʱ����ڣ�����й�ǩ���ɹ���¼������Ϊ��ÿ���Զ���ǩ��ɣ�
			CTime cCurrentTime = CTime::GetCurrentTime();
			if (    (cCurrentTime.GetHour() >= ::GetGlobalApp()->m_iNFCDefaultStartReSignHour)
			        && (cCurrentTime.GetHour() <= ::GetGlobalApp()->m_iNFCDefaultEndReSignHour) )
			{
				GetGlobalApp()->m_bNFCAutoSignIsExecute = TRUE;
			}
			else
			{
				GetGlobalApp()->m_bNFCAutoSignIsExecute = FALSE;
			}
		}
		break;
		case UFT_N_SIGN_VERIFY_ERR:
		{
			//��¼��־
			strLog.Format(_T("[ҵ������߳�]:NFC�ǽ�֧�����̣��ǽ�ģ���������У�鲻�ɹ���ǩ������ʧ��(ģ�����ݽ�������:%d)"), GetGlobalApp()->m_iNFCSignFlowDataCount);
			//����ǩ��״̬Ϊ��
			GetGlobalApp()->m_bNFCSignInSuccess = false;
			//����NFC֧��״̬
			GetGlobalApp()->m_bNFCPayFail = FALSE;
			GetGlobalApp()->m_iNFCPayFailCount = 0;
			//����NFCǩ����������
			GetGlobalApp()->m_iNFCSignFlowDataCount = 0;
			//ǩ��ʧ�ܣ�Ҫ��POSǿ�ƽ�������
			NFCSendDefaultCmdToPos(0x03);
		}
		break;
		}
	}
	catch (int& iErrorCode)
	{
		switch (iErrorCode)
		{
		case -1:
			break;
		}
	}
	//��¼��־
	if (!strLog.IsEmpty())
	{
		GetGlobalApp()->RecordLog(strLog);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////
// �������ƣ�CThreadManage::NFCConsumeDataProcessFromPOS
//
// ����������NFC�������ݴ�������
//
// ���������IN CNFCProtocol& cNFCProtocol
// ���������void
// ��д��Ա��ROCY
// ��дʱ�䣺2017��9��20��
// �޸���Ա��
// �޸�ʱ�䣺
// �����汾��1.0.0.1
// ��ע˵����
//
////////////////////////////////////////////////////////////////////////////////////////////////
void CThreadManage::NFCConsumeDataProcessFromPOS(IN CNFCProtocol& cNFCProtocol)
{
	CString strLog;
	try
	{
		//��Դ�Ϸ����ж�
		CString* pstrString = new CString();
		if (NULL == pstrString)
			throw - 1;
		if (NULL == m_pThreadDevice)
			throw - 2;
		if (!::GetGlobalApp()->m_bIsTrading)
			throw - 3;

		//�ж�����
		switch (cNFCProtocol.FramrType)
		{
		case UFT_N_CONSUME_RES_W2:	//���п���������
		{
			strLog.Format(_T("[ҵ������߳�]:NFC�ǽ�֧�����̣���ѯ�������Ͳ����ɹ�(���п���������)"));
			//�������п�����ָ��
			pstrString->Format(_T("%s"), cNFCProtocol.ConstructExecuteConsumeCmd(0x01));
			//�������ݵ�POSģ��
			m_pThreadDevice->PostThreadMessage(WM_THREAD_NFC_ISSUE_DATA, (WPARAM)pstrString, (LPARAM)UMS_DEBITING);
			//����NFC����������ˮ��ȡ�ȴ���ʱ��
//				StartTimer(TIMER_ID_NFC_WAIT_NORMAL_DATA,3000);
		}
		break;
		case UFT_N_CONSUME_RES_W3:	//POSͨ��������
		{
			strLog.Format(_T("[ҵ������߳�]:NFC�ǽ�֧�����̣���ѯ�������Ͳ����ɹ�(POSͨ��������)"));
			//����POSͨ����ָ��
			pstrString->Format(_T("%s"), cNFCProtocol.ConstructExecuteConsumeCmd(0x02));
			//�������ݵ�POSģ��
			m_pThreadDevice->PostThreadMessage(WM_THREAD_NFC_ISSUE_DATA, (WPARAM)pstrString, (LPARAM)UMS_DEBITING);
			//����NFC����������ˮ��ȡ�ȴ���ʱ��
//				StartTimer(TIMER_ID_NFC_WAIT_NORMAL_DATA,3000);
		}
		break;
		case UFT_N_CONSUME_TIP:		//������ʾ
		{
			//������¼��־����������
			strLog.Format(_T("[ҵ������߳�]:NFC�ǽ�֧�����̣���ȡˢ����ʾ����������Ҫ����"));
		}
		break;
		case UFT_N_CONSUME_RES_DATA://��������
		{
			//��ȡ�����ѱ��ģ����浱ǰ���ѱ��ģ���Ҫ���ӳ��ȣ�
			WORD wDataLen = cNFCProtocol.Context.GetLength() / 2;
			CString strDataLen;
			strDataLen.Format(_T("%04X"), wDataLen);
			GetGlobalApp()->m_pCurrentDebitInfo.NFCPayData.Format(_T("%s%s"), strDataLen, cNFCProtocol.Context);
			//��¼��־
			strLog.Format(_T("[ҵ������߳�]:NFC�ǽ�֧�����̣��ɹ���ȡˢ������"));
			GetGlobalApp()->RecordLog(strLog);
			//��Ĭ150����
			//Sleep(150);
			//���г������ݻ�ȡ
			strLog.Format(_T("[ҵ������߳�]:NFC�ǽ�֧�����̣����г������ݻ�ȡ"));
			GetGlobalApp()->RecordLog(strLog);
			strLog.Empty();
			pstrString->Format(_T("%s"), cNFCProtocol.ConstructCorrectCmd());
			//�������ݵ�POSģ��
			m_pThreadDevice->PostThreadMessage(WM_THREAD_NFC_ISSUE_DATA, (WPARAM)pstrString, (LPARAM)UMS_CORRECT);
			//����NFC������ˮ���ݻ�ȡ�ȴ���ʱ��
//				StartTimer(TIMER_ID_NFC_WAIT_CORRECT_DATA,GetGlobalApp()->m_iNFCGetCancelDataLimitTime * 1000);
		}
		break;
		case UFT_N_CONSUME_ERR:		//����������װ����ˢ��������
		{
			//��¼��־
			strLog.Format(_T("[ҵ������߳�]:NFC�ǽ�֧�����̣�ϵͳ�޷���ȡ�ǽ�ģ��ˢ����������(�ǽ�ģ��������:%02X)�����ν���ʧ�ܣ����ױ�ʶ:%s,��֪ͨ������"), cNFCProtocol.CurrentConsumeErrorCode, GetGlobalApp()->m_pCurrentDebitInfo.PayIdentifier);
			//��������
			NFCLocalTradeFailProcess(RCC_INNER_ERROR);
		}
		break;
		case UFT_N_CONSUME_NO_SIGN:	//ģ��δ��ǩ��
		{
			//δǩ��������ʧ��
			strLog.Format(_T("[ҵ������߳�]:NFC�ǽ�֧�����̣�ϵͳ���ģ��δǩ���ɹ�����Ҫ�Ƚ���ǩ�����������ν���ʧ�ܣ����ױ�ʶ:%s,��֪ͨ������"), GetGlobalApp()->m_pCurrentDebitInfo.PayIdentifier);
			//��������
			NFCLocalTradeFailProcess(RCC_INNER_ERROR);
		}
		break;
		}
	}
	catch (int& iErrorCode)
	{
		switch (iErrorCode)
		{
		case -1:
			break;
		case -2:
			break;
		case -3:
			strLog.Format(_T("[ҵ������߳�]:NFC�ǽ����ݴ��������ڵ�ǰ���ڽ������̵��У������ݲ����Դ���"));
			break;
		}
	}
	//��¼��־
	if (!strLog.IsEmpty())
	{
		GetGlobalApp()->RecordLog(strLog);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////
// �������ƣ�CThreadManage::NFCCorrectDataProcessFromPOS
//
// ����������NFC�������ݴ�������
//
// ���������IN CNFCProtocol& cNFCProtocol
// ���������void
// ��д��Ա��ROCY
// ��дʱ�䣺2017��9��20��
// �޸���Ա��
// �޸�ʱ�䣺
// �����汾��1.0.0.1
// ��ע˵����
//
////////////////////////////////////////////////////////////////////////////////////////////////
void CThreadManage::NFCCorrectDataProcessFromPOS(IN CNFCProtocol& cNFCProtocol)
{
	//ֹͣ��ʱ��
	StopTimer(TIMER_ID_NFC_WAIT_CORRECT_DATA);
	//�����жϵ�ǰ�Ƿ��ڽ�������
	if (GetGlobalApp()->m_bIsTrading)
	{
		//��¼��־
		CString strLog;
		strLog.Format(_T("[ҵ������߳�]:NFC�ǽ�֧�����̣��ɹ���ȡ��������"));
		GetGlobalApp()->RecordLog(strLog);
		strLog.Format(_T("[ҵ������߳�]:NFC�ǽ�֧�����̣�ϵͳ��ȡNFC����������ݳɹ�������Ԥ������Ϣ�����ݽ��۷����󣬽��ױ�ʶ:%s"), GetGlobalApp()->m_pCurrentDebitInfo.PayIdentifier);
		GetGlobalApp()->RecordLog(strLog);
		//����NFC�������ݣ���Ҫ��֮ǰ���ӳ��ȣ�
		WORD wDataLen = cNFCProtocol.Context.GetLength() / 2;
		CString strDataLen;
		strDataLen.Format(_T("%04X"), wDataLen);
		GetGlobalApp()->m_pCurrentDebitInfo.NFCRevData.Format(_T("%s%s"), strDataLen, cNFCProtocol.Context);
		//����۷�����
		CString strTempQRCode = GetGlobalApp()->m_pCurrentDebitInfo.PayCertificateCode;
		GetGlobalApp()->m_pCurrentDebitInfo.PayCertificateCode = _T("111111");

		m_PayCenter.SetCertificate("",LPCSTR(GetGlobalApp()->m_pCurrentDebitInfo.NFCPayData),LPCSTR(GetGlobalApp()->m_pCurrentDebitInfo.NFCRevData));
		//ֹͣ��ȡɨ�����ݣ����Ҳ��ٱ任�ն���Ļ
		m_pThreadDevice->PostThreadMessage(WM_THREAD_DEVICE_STOP_GET_DATA, (WPARAM)0, (LPARAM)0x01);
		m_pThreadDevice->PostThreadMessage(WM_THREAD_DEVICE_CUSTOMDATA_A5FA, (WPARAM)0, (LPARAM)0);

		ContructDebitMoneyInfo();
		GetGlobalApp()->m_pCurrentDebitInfo.PayCertificateCode = strTempQRCode;
		//����ģ�鹤��״̬
		GetGlobalApp()->SetUnipayWorkFlowState(UMS_UNKNOW);
		//����ҵ����(ֹͣPOS���ѣ���ȡ������ˮ)
		strLog.Format(_T("[ҵ������߳�]:NFC�ǽ�֧������,֧��ҵ�����ݻ�ȡ��ϣ�ֹͣPOS�����Ѳ���"));
		GetGlobalApp()->RecordLog(strLog);
		//���·�NAK��ֹͣPOS������������
		NFCSendDefaultCmdToPos(0x03);
	}
	else
	{
		CString strLog;
		strLog.Format(_T("[ҵ������߳�]:NFC�ǽ����ݴ��������ڵ�ǰ���ڽ������̵��У������ݲ����Դ���"));
		GetGlobalApp()->RecordLog(strLog);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////
// �������ƣ�CThreadManage::NFCLinkTestDataProcessFromPOS
//
// ����������NFC�������ӻ�����������
//
// ���������IN CNFCProtocol& cNFCProtocol
// ���������void
// ��д��Ա��ROCY
// ��дʱ�䣺2017��9��22��
// �޸���Ա��
// �޸�ʱ�䣺
// �����汾��1.0.0.1
// ��ע˵����
//
////////////////////////////////////////////////////////////////////////////////////////////////
void CThreadManage::NFCLinkTestDataProcessFromPOS(IN CNFCProtocol& cNFCProtocol)
{
	//����Ǵ���ǩ�����̣�����з���ǩ������
	try
	{
		if (NULL == m_pThreadDevice)
			throw - 1;

		CString* pstrString = new CString();
		if (NULL == pstrString)
			throw - 2;
		//����ǩ��״̬Ϊ��
		GetGlobalApp()->m_bNFCSignInSuccess = false;
		//����ǩ����������Ϊ0
		GetGlobalApp()->m_iNFCSignFlowDataCount = 0;
		//��ȡǩ������
		CNFCProtocol cNFCProtocol;
		pstrString->Format(_T("%s"), cNFCProtocol.ConstructStartSignInCmd());
		//����ǩ�����ݵ��豸�����߳�
		m_pThreadDevice->PostThreadMessage(WM_THREAD_NFC_ISSUE_DATA, (WPARAM)pstrString, (LPARAM)UMS_SIGNING);
		//����ǩ�����ݵ�һ�ν���ʱ��
		GetGlobalApp()->m_tNFCLastSignDataTime = time(NULL);
		//����ģ�鵱ǰ��������Ϊǩ��
		GetGlobalApp()->SetUnipayWorkFlowState(UMS_SIGNING);
		//���õȴ�POS���س�ʱ
//		StartTimer(TIMER_ID_NFC_WAIT_NORMAL_DATA,10000);
	}
	catch (int& iErrorCode)
	{
		switch (iErrorCode)
		{
		case -1:
			break;
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////
// �������ƣ�CThreadManage::NFCLocalTradeFailProcess
//
// ����������NFC����ʧ�ܴ���������δ�ݽ��ƶˣ�
//
// ���������IN const int& iExecuteCode
// ���������void
// ��д��Ա��ROCY
// ��дʱ�䣺2017��9��21��
// �޸���Ա��
// �޸�ʱ�䣺
// �����汾��1.0.0.1
// ��ע˵����
//
////////////////////////////////////////////////////////////////////////////////////////////////
void CThreadManage::NFCLocalTradeFailProcess(IN const int& iExecuteCode)
{
	//add 2017-10
	if (!GetGlobalApp()->m_bIsTrading)
	{
		return ;
	}
	GetGlobalApp()->m_bNFCPayFail		= TRUE;
	if (GetGlobalApp()->m_iNFCPayFailCount > 3)
	{
		GetGlobalApp()->m_iNFCPayFailCount = 1;
	}
	else
	{
		GetGlobalApp()->m_iNFCPayFailCount += 1;
	}

	GetGlobalApp()->SetUnipayWorkFlowState(UMS_UNKNOW);
	
	if(IsFuturePay())
	{
		m_PayCenter.SetCertificateError();
		return ;
	}


	//����ʧ�ܣ�����������
	FinishTrade(TRUE);	//�����������ף��ǳ�ʱ��
	//�����ն���ʾʧ��
	m_pThreadDevice->PostThreadMessage(WM_THREAD_DEVICE_CONTROL_DEAL_RESULT_MMI, (WPARAM)0x02, (LPARAM)0x00);
	//֪ͨ�����߷��ؽ��
	BYTE byFunCode		= 0x01;
	GetGlobalApp()->NotifyCaller(byFunCode, iExecuteCode);
}

////////////////////////////////////////////////////////////////////////////////////////////////
// �������ƣ�CThreadManage::NFCProcessErrorFrame
//
// ����������NFCģ���������֡�쳣����
//
// ���������IN CNFCProtocol& cNFCProtocol
// ���������void
// ��д��Ա��ROCY
// ��дʱ�䣺2017��9��23��
// �޸���Ա��
// �޸�ʱ�䣺
// �����汾��1.0.0.1
// ��ע˵����
//
////////////////////////////////////////////////////////////////////////////////////////////////
void CThreadManage::NFCProcessErrorFrame(IN CNFCProtocol& cNFCProtocol)
{
	//add zhuyabing 2017-11 �޸�NFC��ʱ�߼�
	if (::GetGlobalApp()->m_bIsTrading)
	{
		/*
		m_pThreadDevice->RestartNFCPay();
		return ;
		*/
	}


	CString strLog;
	switch (::GetGlobalApp()->GetUnipayWorkFlowState())
	{
	case UMS_UNKNOW://������
		break;
	case UMS_SIGNING:
		strLog.Format(_T("[ҵ������߳�]:NFCҵ�����̣�ϵͳ��ȡPOSģ���������,�յ��޷�������֡���ж�����ǩ������ʧ�ܡ�"));
		//����ǩ��ʧ��״̬�������´���ǩ׼��
		GetGlobalApp()->m_bNFCSignInSuccess = false;
		//�����ϴ�ǩ������
		GetGlobalApp()->m_tNFCLastSignDataTime = time(NULL);
		break;
	case UMS_DEBITING:
		strLog.Format(_T("[ҵ������߳�]:NFCҵ�����̣�ϵͳ��ȡPOSģ���������,�յ��޷�������֡���ж����ν�������ʧ�ܡ�"));
		NFCLocalTradeFailProcess();
		break;
	case UMS_CORRECT:
		strLog.Format(_T("[ҵ������߳�]:NFCҵ�����̣�ϵͳ��ȡPOSģ���������,�յ��޷�������֡���ж����γ�������ʧ�ܡ�"));
		break;
	}
	//����POSģ�鵱ǰ��������״̬
	GetGlobalApp()->SetUnipayWorkFlowState(UMS_UNKNOW);
	//��¼��־
	if (!strLog.IsEmpty())
	{
		GetGlobalApp()->RecordLog(strLog);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////
// �������ƣ�CThreadManage::NFCSendDefaultCmdToPos
//
// ����������NFC�������̣�����NAKָ��
//
// ���������void
// ���������void
// ��д��Ա��ROCY
// ��дʱ�䣺2017��9��25��
// �޸���Ա��
// �޸�ʱ�䣺
// �����汾��1.0.0.1
// ��ע˵����
//
////////////////////////////////////////////////////////////////////////////////////////////////
void CThreadManage::NFCSendDefaultCmdToPos(IN const int& iCmdType)
{
	CString* pstrString = NULL;
	pstrString = new CString();
	WORD wCMDType = UMS_UNKNOW;
	if (NULL != pstrString)
	{
		CNFCProtocol cNFCProtocol;

		switch (iCmdType)
		{
		case 0x01:
			wCMDType = UMS_TEST_LINK;
			pstrString->Format(_T("%s"), cNFCProtocol.ConstructTestLinkCmd());
			break;
		case 0x02:
			wCMDType = UMS_SIGNING;
			pstrString->Format(_T("%s"), cNFCProtocol.ConstructStartSignInCmd());
			break;
		case 0x03:
			wCMDType = UMS_STOP;
			pstrString->Format(_T("%s"), cNFCProtocol.ConstructStopExecuteCmd());
			break;
		default:
			wCMDType = UMS_UNKNOW;
			break;
		}
		//�������ݵ�POSģ��
		m_pThreadDevice->PostThreadMessage(WM_THREAD_NFC_ISSUE_DATA, (WPARAM)pstrString, (LPARAM)wCMDType/*UMS_STOP*/);
	}
	//����ģ�鵱ǰ����״̬Ϊδ֪
	GetGlobalApp()->SetUnipayWorkFlowState(wCMDType);
}

void CThreadManage::OnMsgRemoteControlFirmware(WPARAM wParam,LPARAM lParam)
{
	char* tmp_str = (char*)wParam;
	if(tmp_str==NULL)
	{
		return ;
	}
	std::string logInfo = "firmware control:" +std::string(tmp_str);
	GetGlobalApp()->RecordLog(logInfo.c_str());
	m_pThreadDevice->RemoteControlFirmware(tmp_str);
	delete tmp_str;
}



afx_msg void CThreadManage::OnMsgRemoteUpgradeResult(WPARAM wParam,LPARAM lParam)
{	//	�̼��������
	if(m_pThreadUpdate!=NULL)
	{
		m_pThreadUpdate->RemoteUpgradeResult(wParam);
	}
}


void CThreadManage::NFCParaQuery(const CString& strData)
{
		CString* pstrString = new CString();
		if (NULL == pstrString)
			return;
		CNFCProtocol cNFCProtocol;
		pstrString->Format(_T("%s"), strData);
		m_pThreadDevice->PostThreadMessage(WM_THREAD_NFC_ISSUE_DATA, (WPARAM)pstrString, (LPARAM)UMS_PARA_QUERY);
}


afx_msg void CThreadManage::OnMsgQueryLPRImageId(WPARAM wParam,LPARAM lParam)
{
	static char *chs= "{\"type\":2,\"data\":\"\"}";
	//MessageBox(0,chs,"",0);
	GetGlobalApp()->send2Bus(KEY_COMMONBUS_ACCESS,chs,strlen(chs),KEY_BUSINESS);
}

int CThreadManage::InitAgencyPay(const char* szAreaInfo, const char* szLoaclStation,  const char* szLoaclLaneID,  const char* szServerInfo,  const int& iProvinceID)
{
	return m_PayCenter.InitAgencyPay(szAreaInfo,szLoaclStation,szLoaclLaneID,szServerInfo,iProvinceID);
}

bool CThreadManage::QueryVirtualCardDebit(IN const char* szParamContext, IN const int& iParamSize, IN const int& iFormatType, IN int& iSupportAgent, OUT char* szResult)
{
	return m_PayCenter.QueryVirtualCardDebit(szParamContext,iParamSize,iFormatType,iSupportAgent,szResult);
}

bool CThreadManage::AgencyDebitQuery(IN const char* szParamContext, IN const int& iParamSize, IN const int& iFormatType, IN OUT int& iResult)
{
	return m_PayCenter.AgencyDebitQuery(szParamContext,iParamSize,iFormatType,iResult);
}
bool CThreadManage::TransferImage(IN const char* szImageInfo, IN const int& iSize, IN const int& iFormatType)
{
	return m_PayCenter.TransferImage(szImageInfo,iSize,iFormatType);
}

int CThreadManage::SetAgencyPara(const char* szAreaInfo, const char* szLoaclStation,  const char* szLoaclLaneID,  const char* szServerInfo,  const int& iProvinceID)
{
	m_PayAreaInfo = szAreaInfo;
	m_PayStationInfo = szLoaclStation;
	m_PayLaneInfo = szLoaclLaneID;
	m_PayServerInfo =szServerInfo ;
	m_PayProvince = iProvinceID;
	return 0;
}

bool CThreadManage::TransferCashInvoiceDataXML(const char* szInvoiceData, const int& iSize)
{
	CString strLog;
	strLog.Format(_T("[ҵ������߳�]:�����ֽ�Ʊ����XML�������ϴ�����..."));
	GetGlobalApp()->RecordLog(strLog);

	InvoiceParameter param;
	param.NeedNotifyResult = 1;
	
	CMarkup xml;
    xml.SetDoc(szInvoiceData);
    xml.ResetMainPos();

	bool read_success = true;
    read_success = read_success ? IntoXMLSubNode(xml, "CashInvoiceData") : false;
	read_success = read_success ? ReadXMLCStr(xml, "CashInvoiceNo", param.ElectronicInvoiceNo, sizeof(param.ElectronicInvoiceNo)) : false;

	read_success = read_success ? IntoXMLSubNode(xml, "Data") : false;
	//Debit������
	read_success = read_success ? IntoXMLSubNode(xml, "Debit") : false;
	read_success = read_success ? ReadXMLInt(xml, "Money", param.Money) : false;
	read_success = read_success ? ReadXMLInt(xml, "PayMode", param.PayMode) : false;
	read_success = read_success ? ReadXMLCStr(xml, "SubTime", param.TollDate, sizeof(param.TollDate)) : false;
	read_success = read_success ? OutXML2Parent(xml) : false;

	//Vehicle������
	read_success = read_success ? IntoXMLSubNode(xml, "Vehicle") : false;
	read_success = read_success ? ReadXMLInt(xml, "VehicleType", param.VehicleType) : false;
	read_success = read_success ? ReadXMLInt(xml, "VehicleClass", param.VehicleClass) : false;
	read_success = read_success ? ReadXMLCStr(xml, "VehicleLicense", param.VehicleLicense, sizeof(param.VehicleLicense)) : false;
	read_success = read_success ? ReadXMLInt(xml, "VLColor", param.VehicleColor) : false;
	read_success = read_success ? ReadXMLInt(xml, "AxisCount", param.AxisCount) : false;
	read_success = read_success ? ReadXMLInt(xml, "Weight", param.TotalWeight) : false;
	read_success = read_success ? ReadXMLInt(xml, "LimitWeight", param.LimitWeight) : false;
	read_success = read_success ? OutXML2Parent(xml) : false;

	//Operation������
	read_success = read_success ? IntoXMLSubNode(xml, "Operation") : false;
	read_success = read_success ? ReadXMLInt(xml, "PassCertificateType", param.PassCertificateType) : false;
	read_success = read_success ? ReadXMLCStr(xml, "PassCertificateCode", param.PassCertificateCode, sizeof(param.PassCertificateCode)) : false;
	read_success = read_success ? ReadXMLInt(xml, "ShiftID", param.ShiftID) : false;
	read_success = read_success ? ReadXMLCStr(xml, "OperatorID", param.OperatorID, sizeof(param.OperatorID)) : false;
	read_success = read_success ? ReadXMLCStr(xml, "OperatorName", param.OperatorName, sizeof(param.OperatorName)) : false;
	read_success = read_success ? OutXML2Parent(xml) : false;

	//Entry������
	read_success = read_success ? IntoXMLSubNode(xml, "Entry") : false;
	read_success = read_success ? ReadXMLCStr(xml, "EntryAreaID", param.EntryAreaID, sizeof(param.EntryAreaID)) : false;
	read_success = read_success ? ReadXMLCStr(xml, "EntryRoadID", param.EntryRoadID, sizeof(param.EntryRoadID)) : false;
	read_success = read_success ? ReadXMLCStr(xml, "EntryStationID", param.EntryStationID, sizeof(param.EntryStationID)) : false;
	read_success = read_success ? ReadXMLCStr(xml, "EntryStationName", param.EntryStationName, sizeof(param.EntryStationName)) : false;
	read_success = read_success ? ReadXMLCStr(xml, "EntryLaneID", param.EntryLaneID, sizeof(param.EntryLaneID)) : false;
	read_success = read_success ? ReadXMLInt(xml, "Distance", param.Distance) : false;
	read_success = read_success ? ReadXMLCStr(xml, "EntryTime", param.EntryTime, sizeof(param.EntryTime)) : false;
	read_success = read_success ? ReadXMLCStr(xml, "EntryVehicleLicense", param.EntryVehicleLicense, sizeof(param.EntryVehicleLicense)) : false;
	read_success = read_success ? ReadXMLInt(xml, "EntryVLColor", param.EntryVLColor) : false;
	read_success = read_success ? ReadXMLInt(xml, "EntryVehicleClass", param.EntryVehicleClass) : false;
	read_success = read_success ? ReadXMLInt(xml, "EntryVehicleType", param.EntryVehicleType) : false;
	read_success = read_success ? OutXML2Parent(xml) : false;

	//Exit������
	read_success = read_success ? IntoXMLSubNode(xml, "Exit") : false;
	read_success = read_success ? ReadXMLCStr(xml, "PayAreaID", param.PayAreaID, sizeof(param.PayAreaID)) : false;
	read_success = read_success ? ReadXMLCStr(xml, "PayProvinceId", param.PayProvinceId, sizeof(param.PayProvinceId)) : false;
	read_success = read_success ? ReadXMLCStr(xml, "PayRoadID", param.PayRoadID, sizeof(param.PayRoadID)) : false;
	read_success = read_success ? ReadXMLCStr(xml, "PayStationID", param.PayStationID, sizeof(param.PayStationID)) : false;
	read_success = read_success ? ReadXMLCStr(xml, "PayStationName", param.PayStationName, sizeof(param.PayStationName)) : false;
	read_success = read_success ? ReadXMLCStr(xml, "PayLaneID", param.PayLane, sizeof(param.PayLane)) : false;
	read_success = read_success ? ReadXMLCStr(xml, "ExitTime", param.ExitTime, sizeof(param.ExitTime)) : false;
	read_success = read_success ? OutXML2Parent(xml) : false;

    if (!read_success)
    {
        TW_TRACE_DIARY("�ֽ�ƱXML������������:" + GetXMLParseError());
        return false;
    }

	//��װ��ά������
	/*char strMoney[16] = {0};
	sprintf(strMoney, "%d", param.Money);
	std::stringstream ss;
	ss << param.ElectronicInvoiceNo << "&";
	ss << "a=" << param.VehicleLicense << "&";
	ss << "b=" << param.VehicleColor << "&";
	ss << "c=" << strMoney << "&";
	ss << "d=" << param.PayMode << "&";
	ss << "e=" << param.EntryStationName << "&";
	ss << "f=" << param.EntryTime << "&";
	ss << "g=" << param.PayStationName << "&";
	ss << "h=" << param.ExitTime;

	strncpy(param.QRcode, "https://ytcard.trawe.cn/opr/bknd/fat/tw/invoice/ElectronicInvoice/entrance?parameter=", sizeof(param.QRcode));
	strcat(param.QRcode, ss.str().c_str());*/

	int ret = m_PayCenter.StartNewCashInvoiceTransmit(param);
	
	return (ret == 0);
}

//�������if(read_success)TW_TRACE_DIARY("1");
bool CThreadManage::TransferCashInvoiceDataJSON(const char* szInvoiceData, const int& iSize)
{
	CString strLog;
	strLog.Format(_T("[ҵ������߳�]:�����ֽ�Ʊ����JSON�������ϴ�����..."));
	GetGlobalApp()->RecordLog(strLog);

	Json::Reader json_reader;
    Json::Value root_node, CashInvoiceData_node, Data_node, Vehicle_node;
    Json::Value Exit_node, Debit_node, Operation_node, Entry_node;

	InvoiceParameter param;
	param.NeedNotifyResult = 1;

	if (!json_reader.parse(szInvoiceData, root_node))
    {
        return false;
    }
	bool read_success = true;
	read_success = read_success ? ReadJsonSubNode(root_node, "CashInvoiceData", CashInvoiceData_node) : false;

    read_success = read_success ? ReadJsonSubNode(CashInvoiceData_node, "Data", Data_node) : false;
	read_success = read_success ? ReadJsonSubNode(Data_node, "Vehicle", Vehicle_node) : false;
    read_success = read_success ? ReadJsonSubNode(Data_node, "Exit", Exit_node) : false;
    read_success = read_success ? ReadJsonSubNode(Data_node, "Debit", Debit_node) : false;
    read_success = read_success ? ReadJsonSubNode(Data_node, "Operation", Operation_node) : false;
    read_success = read_success ? ReadJsonSubNode(Data_node, "Entry", Entry_node) : false;
	//��Ʊ���к�
	read_success = read_success ? ReadJsonChars(CashInvoiceData_node, "CashInvoiceNo", param.ElectronicInvoiceNo, sizeof(param.ElectronicInvoiceNo)) : false;
	//Vehicle������
	read_success = read_success ? ReadJsonChars(Vehicle_node, "VehicleLicense", param.VehicleLicense, sizeof(param.VehicleLicense)) : false;
    read_success = read_success ? ReadJsonInt(Vehicle_node, "VLColor", param.VehicleColor) : false;
    read_success = read_success ? ReadJsonInt(Vehicle_node, "VehicleType", param.VehicleType) : false;
    read_success = read_success ? ReadJsonInt(Vehicle_node, "VehicleClass", param.VehicleClass) : false;
    read_success = read_success ? ReadJsonInt(Vehicle_node, "AxisCount", param.AxisCount) : false;
    read_success = read_success ? ReadJsonInt(Vehicle_node, "Weight", param.TotalWeight) : false;
    read_success = read_success ? ReadJsonInt(Vehicle_node, "LimitWeight", param.LimitWeight) : false;
	//Exit������
	read_success = read_success ? ReadJsonChars(Exit_node, "PayAreaID", param.PayAreaID, sizeof(param.PayAreaID)) : false;
	read_success = read_success ? ReadJsonChars(Exit_node, "PayProvinceId", param.PayProvinceId, sizeof(param.PayProvinceId)) : false;
	read_success = read_success ? ReadJsonChars(Exit_node, "PayRoadID", param.PayRoadID, sizeof(param.PayRoadID)) : false;
	read_success = read_success ? ReadJsonChars(Exit_node, "PayStationID", param.PayStationID, sizeof(param.PayStationID)) : false;
	read_success = read_success ? ReadJsonChars(Exit_node, "PayStationName", param.PayStationName, sizeof(param.PayStationName)) : false;
	read_success = read_success ? ReadJsonChars(Exit_node, "PayLaneID", param.PayLane, sizeof(param.PayLane)) : false;
	read_success = read_success ? ReadJsonChars(Exit_node, "ExitTime", param.ExitTime, sizeof(param.ExitTime)) : false;
	//Debit������
    read_success = read_success ? ReadJsonInt(Debit_node, "Money", param.Money) : false;
    read_success = read_success ? ReadJsonInt(Debit_node, "PayMode", param.PayMode) : false;
	read_success = read_success ? ReadJsonChars(Debit_node, "SubTime", param.TollDate, sizeof(param.TollDate)) : false;
	//Operation������
    read_success = read_success ? ReadJsonInt(Operation_node, "ShiftID", param.ShiftID) : false;
   	read_success = read_success ? ReadJsonChars(Operation_node, "OperatorID", param.OperatorID, sizeof(param.OperatorID)) : false;
	read_success = read_success ? ReadJsonChars(Operation_node, "OperatorName", param.OperatorName, sizeof(param.OperatorName)) : false;
	read_success = read_success ? ReadJsonInt(Operation_node, "PassCertificateType", param.PassCertificateType) : false;
	read_success = read_success ? ReadJsonChars(Operation_node, "PassCertificateCode", param.PassCertificateCode, sizeof(param.PassCertificateCode)) : false;
	//Entry������
	read_success = read_success ? ReadJsonChars(Entry_node, "EntryAreaID", param.EntryAreaID, sizeof(param.EntryAreaID)) : false;
	read_success = read_success ? ReadJsonChars(Entry_node, "EntryRoadID", param.EntryRoadID, sizeof(param.EntryRoadID)) : false;
	read_success = read_success ? ReadJsonChars(Entry_node, "EntryStationID", param.EntryStationID, sizeof(param.EntryStationID)) : false;
	read_success = read_success ? ReadJsonChars(Entry_node, "EntryStationName", param.EntryStationName, sizeof(param.EntryStationName)) : false;
	read_success = read_success ? ReadJsonChars(Entry_node, "EntryLaneID", param.EntryLaneID, sizeof(param.EntryLaneID)) : false;
	read_success = read_success ? ReadJsonInt(Entry_node, "Distance", param.Distance) : false;
	read_success = read_success ? ReadJsonChars(Entry_node, "EntryTime", param.EntryTime, sizeof(param.EntryTime)) : false;
	read_success = read_success ? ReadJsonChars(Entry_node, "EntryVehicleLicense", param.EntryVehicleLicense, sizeof(param.EntryVehicleLicense)) : false;
    read_success = read_success ? ReadJsonInt(Entry_node, "EntryVLColor", param.EntryVLColor) : false;
    read_success = read_success ? ReadJsonInt(Entry_node, "EntryVehicleClass", param.EntryVehicleClass) : false;
    read_success = read_success ? ReadJsonInt(Entry_node, "EntryVehicleType", param.EntryVehicleType) : false;
	if (!read_success)
    {
        TW_TRACE_DIARY("�ֽ�ƱJSON������������:" + GetJsonParseError());
        return false;
    }
	//��װ��ά������
	/*std::stringstream ss;
	ss << param.ElectronicInvoiceNo << "&";
	ss << "a=" << param.VehicleLicense << "&";
	ss << "b=" << param.VehicleColor << "&";
	ss << "c=" << param.Money << "&";
	ss << "d=" << param.PayMode << "&";
	ss << "e=" << param.EntryStationName << "&";
	ss << "f=" << param.EntryTime << "&";
	ss << "g=" << param.PayStationName << "&";
	ss << "h=" << param.ExitTime;

	strncpy(param.QRcode, "https://ytcard.trawe.cn/opr/bknd/fat/tw/invoice/ElectronicInvoice/entrance?parameter=", sizeof(param.QRcode));
	strcat(param.QRcode, ss.str().c_str());*/

	int ret = m_PayCenter.StartNewCashInvoiceTransmit(param);
	
	return (ret == 0);
}


bool CThreadManage::CancelCashInvoiceDataXML(const char* szInvoiceData, const int& iSize)
{
	CString strLog;
	strLog.Format(_T("[ҵ������߳�]:�������������ֽ�Ʊ����XML��������������..."));
	GetGlobalApp()->RecordLog(strLog);

	InvoiceCancelParameter param;
	param.NeedNotifyResult = 1;
	
	CMarkup xml;
    xml.SetDoc(szInvoiceData);
    xml.ResetMainPos();

	bool read_success = true;
    read_success = read_success ? IntoXMLSubNode(xml, "CancelCashInvoiceData") : false;
	read_success = read_success ? ReadXMLCStr(xml, "CashInvoiceNo", param.ElectronicInvoiceNo, sizeof(param.ElectronicInvoiceNo)) : false;
	read_success = read_success ? ReadXMLCStr(xml, "PayProvinceId", param.PayProvinceId, sizeof(param.PayProvinceId)) : false;
	read_success = read_success ? ReadXMLCStr(xml, "ExitTime", param.ExitTime, sizeof(param.ExitTime)) : false;
	read_success = read_success ? OutXML2Parent(xml) : false;

	if (!read_success)
    {
        TW_TRACE_DIARY("�����ֽ�ƱXML������������:" + GetXMLParseError());
        return false;
    }
	int ret = m_PayCenter.StartCashInvoiceCancel(param);
	
	return (ret == 0);
}


bool CThreadManage::CancelCashInvoiceDataJSON(const char* szInvoiceData, const int& iSize)
{
	CString strLog;
	strLog.Format(_T("[ҵ������߳�]:�������������ֽ�Ʊ����JSON��������������..."));
	GetGlobalApp()->RecordLog(strLog);

	Json::Reader json_reader;
    Json::Value root_node, CancelCashInvoiceData_node;

	InvoiceCancelParameter param;
	param.NeedNotifyResult = 1;

	if (!json_reader.parse(szInvoiceData, root_node))
    {
        return false;
    }
	bool read_success = true;
	read_success = read_success ? ReadJsonSubNode(root_node, "CancelCashInvoiceData", CancelCashInvoiceData_node) : false;
	read_success = read_success ? ReadJsonChars(CancelCashInvoiceData_node, "CashInvoiceNo", param.ElectronicInvoiceNo, sizeof(param.ElectronicInvoiceNo)) : false;
   	read_success = read_success ? ReadJsonChars(CancelCashInvoiceData_node, "PayProvinceId", param.PayProvinceId, sizeof(param.PayProvinceId)) : false;
   	read_success = read_success ? ReadJsonChars(CancelCashInvoiceData_node, "SubTime", param.ExitTime, sizeof(param.ExitTime)) : false;

	if (!read_success)
    {
        TW_TRACE_DIARY("�����ֽ�ƱJSON������������:" + GetJsonParseError());
        return false;
    }
	int ret = m_PayCenter.StartCashInvoiceCancel(param);
	
	return (ret == 0);
}


