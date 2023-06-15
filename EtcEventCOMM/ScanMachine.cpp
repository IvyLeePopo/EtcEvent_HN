// ScanMachine.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "ScanMachine.h"
#include "FileFindExt.h"
#include <shlwapi.h>
#pragma comment(lib,"shlwapi.lib")
#include "mdump.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: ����� DLL ����� MFC DLL �Ƕ�̬���ӵģ�
//		��Ӵ� DLL �������κε���
//		MFC �ĺ������뽫 AFX_MANAGE_STATE ����ӵ�
//		�ú�������ǰ�档
//
//		����:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// �˴�Ϊ��ͨ������
//		}
//
//		�˺������κ� MFC ����
//		������ÿ��������ʮ����Ҫ������ζ��
//		��������Ϊ�����еĵ�һ�����
//		���֣������������ж������������
//		������Ϊ���ǵĹ��캯���������� MFC
//		DLL ���á�
//
//		�й�������ϸ��Ϣ��
//		����� MFC ����˵�� 33 �� 58��
//


// CScanMachineApp

void*  CScanMachineApp::m_pIndPtr = NULL;
bool CScanMachineApp::m_bIsDevInited = false;



BEGIN_MESSAGE_MAP(CScanMachineApp, CWinApp)
END_MESSAGE_MAP()


// CScanMachineApp ����

CScanMachineApp::CScanMachineApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
	m_pThreadSMManager = NULL;
}


// Ψһ��һ�� CScanMachineApp ����

CScanMachineApp theApp;

CScanMachineApp* GetGlobalApp() { return &theApp; }

void LogCallBack(const std::string& content)
{
	theApp.RecordMsgTemp(content.c_str(), TRUE);
}

// CScanMachineApp ��ʼ��

BOOL CScanMachineApp::InitInstance()
{
	CWinApp::InitInstance();


	m_DeviceNo = "";
	m_QRcode = "";
	//MiniDump();

	//��ȡ��̬������·����ֻ���ڶ�̬���Ѿ�ʵ���������
	m_strAppDir.Format( _T("%s"), GetCurrentDirNew() );
	//������־�ļ�
	m_strLogDirPath.Format(_T("%s\\TWSDNetPayLog"),		m_strAppDir);
	m_strLogFileName.Format(_T("HTS_%s.log"), CTime::GetCurrentTime().Format(_T("%Y%m%d")));
	m_strLogFilePath.Format( _T("%s\\%s"), m_strLogDirPath, m_strLogFileName);
	m_strConfigPathFile.Format( _T("%s\\%s"), m_strAppDir, FILE_NAME_DEVICE_SM_CONFIG);

	char tmp_str[200]={0};
	//�Ƿ��¼��־���
	m_iWriteLog	= ::GetPrivateProfileInt(_T("SMControl"),	_T("WriteLog"),	1,	m_strConfigPathFile);

	//���ڲ���
	theApp.m_SerialPort = ::GetPrivateProfileInt(_T("SerialParam"),	_T("Port"),	1,	m_strConfigPathFile);
	theApp.m_BaudRate   = ::GetPrivateProfileInt(_T("SerialParam"),	_T("BaudRate"),	57600,	m_strConfigPathFile);
	theApp.m_InBufferSize = ::GetPrivateProfileInt(_T("SerialParam"),	_T("InBufferSize"),	2048, m_strConfigPathFile);
	theApp.m_OutBufferSize = ::GetPrivateProfileInt(_T("SerialParam"),	_T("OutBufferSize"), 2048, m_strConfigPathFile);
	//��־�ļ����ֵ
	m_iLogFileMaxSize = ::GetPrivateProfileInt(_T("SMControl"),	_T("LogMaxSize"), FILE_SIZE_LOG_MAX, m_strConfigPathFile);

	//theApp.CreateDir(m_strLogDirPath);

	theApp.RecordMsgTemp(_T("/**********************************����ͨ�Ŷ�̬�� -- ��������*******************************************/"));
	CString configinfo;
	configinfo.Format( _T("���ô��ں�:%d, ������:%d"), theApp.m_SerialPort, theApp.m_BaudRate);
	theApp.RecordMsgTemp(configinfo);

	return TRUE;
}

int CScanMachineApp::ExitInstance()
{
	RecordMsgTemp(_T("�豸�����̣߳��߳��˳�..."));
	if ( NULL != theApp.m_pThreadSMManager )
	{
		theApp.m_pThreadSMManager->m_bExit = TRUE;
		::Sleep(100);

		int itry = 0;
		while (::WaitForSingleObject(theApp.m_pThreadSMManager->m_hThread, 200) != WAIT_OBJECT_0)
		{
			int nErrCode = ::GetLastError();
			if (6 == nErrCode) break;

			::Sleep(100);
			itry++;
			if (itry > 9) break;
		}
		theApp.m_pThreadSMManager = NULL;
	}

	RecordMsgTemp(_T("�豸�����̣߳��߳��˳��ɹ�..."));

	return CWinApp::ExitInstance();
}


/*********************************���ú���*************************************************************/

////////////////////////////////////////////////////////////////////////////////////////////////
// �������ƣ�CString	FmtStr
// ����������
// ���������const char * lpzFmt
//         ��...
// ��д��Ա�������
// ��дʱ�䣺���ڶ� 2009��9��15��
// �޸���Ա��
// �޸�ʱ�䣺
// �����汾��1.0.0.0
// ��ע˵����
////////////////////////////////////////////////////////////////////////////////////////////////
CString	CScanMachineApp::FmtStr(const TCHAR* lpsFmt, ...)
{
	CString	str;
	va_list	lpzArg = NULL;

	va_start(lpzArg, lpsFmt);
	str.FormatV(lpsFmt, lpzArg);

	va_end(lpzArg);

	return str;
}

/////////////////////////////////////////////////////////////////////////////////////////
////��������:fnHexToBin
////��������:��16���Ƶ��ַ���ת����2���Ƶ�����
////�������:IN PBYTE	pHex		-- ��Ҫת����16�����ַ���
////		 OUT PBYTE	pbin		-- �����2������������
////		 IN DWORD	dwHex_len	-- ��Ҫת����16�������ݳ���,����Ϊ2�ı���
////�������:��
////��дʱ��:2020-10-18
////��д��Ա:�����
////�޸�ʱ��:
////�޸���Ա:
////�����汾:1.0.0.1
////��ע˵��:�������16�����ַ������ȱ�����2�ı���
//////////////////////////////////////////////////////////////////////////////////////////
void CScanMachineApp::Hex2Bin(IN LPCSTR lpHex, OUT PBYTE pBin, IN DWORD dwHex_len)
{
	if (dwHex_len % 2)
		return;

	CString strTemp(lpHex);
	strTemp.MakeUpper();

	USES_CONVERSION;
	for (int i = 0; i < strTemp.GetLength() / 2; i++)
	{
		BYTE a = (BYTE)(strTemp.GetAt(i * 2) >= 'A') ? (BYTE)(strTemp.GetAt(i * 2) - 'A' + 10) : (BYTE)(strTemp.GetAt(i * 2) - '0');
		BYTE b = (BYTE)(strTemp.GetAt(i * 2 + 1) >= 'A') ? (BYTE)(strTemp.GetAt(i * 2 + 1) - 'A' + 10) : (BYTE)(strTemp.GetAt(i * 2 + 1) - '0');
		pBin[i] = (BYTE)(a * 0x10 + b);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////
////��������:BinToHex
////��������:��2���Ƶ�����ת����16���Ƶ��ַ���
////�������:IN PBYTE	pBin		-- ��Ҫת����2������������
////		 IN DWORD	dwHex_len	-- ��Ҫת����2�����������鳤��
////�������:CString	�ǿ� -- ת���ɹ��������16�����ַ���
////		 CString	NULL -- ת��ʧ��
////��дʱ��:2020-10-18
////��д��Ա:�����
////�޸�ʱ��:
////�޸���Ա:
////�����汾:1.0.0.0
////��ע˵��:������óɹ���,����Ҫ�������ͷ��ڴ�
//////////////////////////////////////////////////////////////////////////////////////////
CString CScanMachineApp::Bin2Hex(IN PBYTE pBin, IN DWORD dwBin_Len)
{
	CString strHex;
	CString strTemp;
	if (NULL != pBin)
	{
		for ( DWORD i = 0; i < dwBin_Len; ++i )
		{
			strTemp.Format(_T("%02X"), pBin[i] );
			strHex += strTemp;
		}
	}
	return strHex;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//	��	  �� : �����
//	ʱ	  �� : 2020-10-18
//  ��	  �� : 1.0.0.0
//	�������� : CRC16
//	�������� : ���ݸ��������ݣ�������CRCУ����
//	������� : LPBYTE	--	lpData	��Ҫ����У����ֽ�������
//			   WORD		--	wLength	��Ҫ����У����ֽڳ���
//	������� : WORD		--	���ص�CRCУ���룬˫�ֽ�
//  ��ע˵�� : ��������У�����ָ��Ϊ�գ��򷵻� 0
///////////////////////////////////////////////////////////////////////////////////////////////////////////
WORD CScanMachineApp::CRC16(LPBYTE lpData, WORD wLength)
{
	BYTE	CRC16Hi = 0xFF;
	BYTE	CRC16Lo = 0xFF;
	BYTE	CH = 0xA0;
	BYTE	CL = 0x01;
	BYTE	SaveHi = 0;
	BYTE	SaveLo = 0;

	if (NULL == lpData)
		return 0;

	for (int i = 0; i < wLength; i++)
	{
		CRC16Lo = CRC16Lo ^ lpData[i];

		for (int Flag = 0; Flag < 8; Flag++)
		{
			SaveHi = CRC16Hi;
			SaveLo = CRC16Lo;
			CRC16Hi = CRC16Hi / 2; //��λ����һλ
			CRC16Lo = CRC16Lo / 2; //��λ����һλ
			if ((SaveHi & 1) == 1)
				CRC16Lo |= 0x80;
			if ((SaveLo & 1) == 1)
			{
				CRC16Hi = CRC16Hi ^ CH;
				CRC16Lo = CRC16Lo ^ CL;
			}
		}//for(int Flag = 0;Flag < 8;Flag++)
	}//for(int i = 0;i < nLenght;i++)
	return (WORD)(CRC16Hi * 0x100 + CRC16Lo);
}

////////////////////////////////////////////////////////////////////////////////////////////////
// �������ƣ�CScanDevApp::GetCurrentDirNew
// ������������ȡ��ǰ�������е�����Ŀ¼���ɵ����߾�����
// �����������
// ���������CString	--	��ȡ���ĳ�������Ŀ¼
// ��д��Ա�������
// ��дʱ�䣺2020��10��18��
// �޸���Ա��
// �޸�ʱ�䣺
// �����汾��1.0.0.0
// ��ע˵����
////////////////////////////////////////////////////////////////////////////////////////////////
CString CScanMachineApp::GetCurrentDirNew()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	TCHAR szfile[255] = _T("");
	CString csDir = _T("");

	//����Ӧ�ó��򣨵����ߣ����˷�ʽ��ȷ�����ڶ�̬�⣨�������ߣ����˷�ʽ��ȡ���ǳ���·��
	//	DWORD dwLen = GetModuleFileName(NULL, szfile, 255);

	//����Ϊ��̬�������·��
	HINSTANCE hInsApp;
	hInsApp = AfxGetInstanceHandle();
	DWORD dwLen = GetModuleFileName(hInsApp, szfile, 255);

	csDir.Format( _T("%s"), szfile);
	csDir = csDir.Left( csDir.ReverseFind(_T('\\')) );
	return csDir;
}



////////////////////////////////////////////////////////////////////////////////////////////////
// �������ƣ�CScanDevApp::RecordMsgTemp
// ������������¼��Ϣ����ʱ�ļ�����־�ļ���
// ���������IN LPCTSTR strMsg	--	��Ҫ��¼����Ϣ��
// ���������void
// ��д��Ա�������
// ��дʱ�䣺2020��10��18��
// �޸���Ա��
// �޸�ʱ�䣺
// �����汾��1.0.0.0
// ��ע˵����
////////////////////////////////////////////////////////////////////////////////////////////////
void CScanMachineApp::RecordMsgTemp(IN LPCTSTR strMsg, IN const BOOL& bForce)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (  ( 0 == m_iWriteLog ) || (0 == _tcslen(strMsg) ) )
	{
		return;
	}

	CFile m_file;

	static CString strBufferLog = _T("");
	//���ʱ��
	SYSTEMTIME sysGetTime = {0};
	::GetLocalTime(&sysGetTime);
	CTime m_curtime = CTime::GetCurrentTime();
	CString sTemp = _T("");
	CString strCurrentDateFile;
	strCurrentDateFile.Format(_T("HTS%s.log"), CTime::GetCurrentTime().Format(_T("%Y%m%d")));
	//��־�ļ�����һ��
	if (strCurrentDateFile != m_strLogFileName)
	{
		if (CFile::hFileNull != m_file.m_hFile)
		{
			m_file.Close();
			Sleep(10);
		}

		m_strLogFileName = strCurrentDateFile;
		m_strLogFilePath.Format(_T("%s\\%s"), m_strLogDirPath, m_strLogFileName);
	}

	if (m_file.Open(m_strLogFilePath, CFile::modeWrite | CFile::modeCreate | CFile::modeNoTruncate))
	{
		sTemp.Format(_T("%04d-%02d-%02d %02d:%02d:%02d.%03d -- "), sysGetTime.wYear, sysGetTime.wMonth, sysGetTime.wDay, sysGetTime.wHour, sysGetTime.wMinute, sysGetTime.wSecond, sysGetTime.wMilliseconds); // = m_curtime.Format( _T("%Y-%m-%d %H:%M:%S -- "));
		sTemp += strMsg;
		sTemp += _T("\r\n\r\n");

		strBufferLog += sTemp;
		//����1K�Ž��б���
		if ( (strBufferLog.GetLength() > 1024) || bForce )
		{
			m_file.SeekToEnd();
			m_file.Write((LPBYTE)strBufferLog.GetBuffer(0), strBufferLog.GetLength());
			strBufferLog.ReleaseBuffer();
			m_file.Close();
			strBufferLog.Empty();
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//������:	OleTimeToTime
//��  ��:	�� DATE��ʽʱ��ת��Ϊ time_t��ʽʱ��
//��  ��:	DATE dTime		��ת���� DATE�͸�ʽʱ��
//
//����ֵ:	ת����� time_t�͸�ʽʱ��
//////////////////////////////////////////////////////////////////////////////////////////////////////
time_t CScanMachineApp::OleTimeToTime(DATE dTime)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	COleDateTimeSpan tSpan;
	COleDateTime t1 = (time_t)0;
	COleDateTime t2 = dTime;
	COleDateTime t3(2999, 12, 31, 23, 59, 59);
	if ( t2 > t3 )
	{
		t2 = t3;
	}
	tSpan = t2 - t1;
	return time_t( tSpan.m_span * 86400 + 0.5 );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//������:	CString2time_t
//��  ��:	���ַ����͸�ʽʱ��ת��Ϊ time_t�͸�ʽʱ��
//��  ��:	LPCTSTR lpztime		��ת�����ַ����͸�ʽʱ��
//
//����ֵ:	ת����� time_t�͸�ʽʱ��
//////////////////////////////////////////////////////////////////////////////////////////////////////
time_t CScanMachineApp::CString2time_t(LPCTSTR lpztime)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString csTemp = _T("");
	COleDateTime oleGetTime;
	csTemp.Format( _T("%s"), lpztime);
	if ( csTemp.GetLength() > 0 )
	{
		if ( csTemp.GetLength() > 19 ) //�������Ƹ�ʽ _T("2014-11-23 12:34:56.000");
		{
			csTemp = csTemp.Left(19);
		}
		oleGetTime.ParseDateTime(csTemp);
		return OleTimeToTime(oleGetTime.m_dt);
	}
	else
		return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// �������ƣ�CThreadManage::CheckAndDeleteLogFile
// ����������������ɾ����־
// ���������void
// ���������void
// ��д��Ա�������
// ��дʱ�䣺2020��10��18��
// �޸���Ա��
// �޸�ʱ�䣺
// �����汾��1.0.0.0
// ��ע˵����
////////////////////////////////////////////////////////////////////////////////////////////////
void CScanMachineApp::CheckAndDeleteLogFile(void)
{
	static time_t lastCheckTime = 0;
	if ( (time(NULL) - lastCheckTime) < 4 * 60 * 60 )
	{
		return;
	}
	lastCheckTime = time(NULL);


	CFileFindExt cFindFile;
	CString strDir;
	CString strExtName(_T("log"));
	strDir.Format(_T("%s"), m_strLogDirPath);
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
			iYear = _ttoi(strFileName.Mid(3, 4));
			iMon  = _ttoi(strFileName.Mid(7, 2));
			iDay  = _ttoi(strFileName.Mid(9, 2));

			if ( (0 != iYear) && (0 != iMon) && (0 != iDay) )
			{
				CTime cFileTime(iYear, iMon, iDay, 0, 0, 0);
				//��ȡ���ļ���ʱ��������ʱ����������
				CTimeSpan span = cCurrentTime - cFileTime ;
				long long iSpanDay = span.GetDays();
				if (iSpanDay < m_iDeleteLogSaveDays)
					continue;
				if (!::PathFileExists(strFilePath))
					throw - 3;
				::DeleteFile(strFilePath);
				if (++iDeleteCount >= 10)
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




/**********************************ETC����ӿ�************************************************************/
bool WINAPI  CScanMachineApp::IF_InitDev(IN const UINT& dwThreadNotifyCallerID, IN const UINT& dwThreadDevManagerID, IN const HWND& hWnd, IN const UINT& dwEtcRespMsgID, IN const UINT& dwDevNotifyMsgID, IN OUT void* pIndPtr)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	char log[128] = {0};
	_snprintf_s(log, 128, "�豸��ʼ���ӿ�(IF_InitDev)���ÿ�ʼ,�������߳�ID:%u,��Ϣ֪ͨID:%u", dwThreadNotifyCallerID, dwEtcRespMsgID);
	theApp.RecordMsgTemp(log);

    CString strLog;
    try
    {
        /*******************���������Ƿ�����*******************/
        if ((0 == dwThreadNotifyCallerID) && (NULL == hWnd))
            throw -1;
        if ((NULL == pIndPtr))
		{
			 throw -2;
		}
        //�ص�����ָ�븳ֵ������֪ͨ������
		m_pIndPtr = pIndPtr;
		
		theApp.m_ThreadNotifyCallerID = dwThreadNotifyCallerID;
		theApp.m_EtcRespMsgID = dwEtcRespMsgID;

		theApp.m_ThreadDevManagerID = dwThreadDevManagerID;
		theApp.m_DevNotifyMsgID = dwDevNotifyMsgID;

		//��������
		m_bIsDevInited = false;
		if (m_bIsDevInited)
		{
			throw -3;
		}

		int baudrate = theApp.m_BaudRate;
		int portNo = theApp.m_SerialPort;
	
	    if(theApp.m_pThreadSMManager == NULL)
			theApp.m_pThreadSMManager = (CThreadSMManager*)::AfxBeginThread(RUNTIME_CLASS(CThreadSMManager), THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
		if (theApp.m_pThreadSMManager == NULL)
		{
			throw -4;
		}
		
		theApp.m_pThreadSMManager->SetComPara(portNo, baudrate);
		theApp.m_pThreadSMManager->ResumeThread();
		Sleep(1000);
		
		if (!theApp.m_pThreadSMManager->IsConnectionOK())
		{
			throw -5;
		}
		else
		{
			m_bIsDevInited = true;
		}
    }
    catch (int& iErrorCode)
    {
        switch (iErrorCode)
        {
        case -1:
            strLog.Format(_T("�豸��ʼ���ӿ�(IF_InitDev),�������������󣬴����߳�ID�봰������Ϊ0"));
            break;
        case -2:
            strLog.Format(_T("�豸��ʼ���ӿ�(IF_InitDev),�������������󣬴���ص�ָ�����"));
            break;
        case -3:
            strLog.Format(_T("�豸��ʼ���ӿ�(IF_InitDev),�豸��ʼ��ʧ��(�ظ���ʼ���豸)"));
            break;
        case -4:
            strLog.Format(_T("�豸��ʼ���ӿ�(IF_InitDev),�������ڹ����߳���Դʧ��"));
            break;
        case -5:
            strLog.Format(_T("�豸��ʼ���ӿ�(IF_InitDev),���ڴ�ʧ��"));
            break;
        }
        //��¼��־
        if (theApp.m_strLastErrorDesc != strLog)
        {
            theApp.m_strLastErrorDesc = strLog;
        }
		theApp.RecordMsgTemp(strLog);
		return false;
    }

	return true;
}


bool WINAPI  CScanMachineApp::IF_DeInitDev(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	theApp.RecordMsgTemp(_T("ж���豸�ӿ�(IF_DeInitDev)���ÿ�ʼ"));
	if (NULL != theApp.m_pThreadSMManager )
	{
		theApp.RecordMsgTemp(_T("EtcEvent����ͨ�ſ⣬�ر��豸����..."));
		theApp.m_pThreadSMManager->CloseCom();
	
		theApp.RecordMsgTemp(_T("EtcEvent����ͨ�ſ⣬�˳��豸�����߳�..."));
		theApp.m_pThreadSMManager->m_bExit = TRUE;
		::Sleep(100);

		int itry = 0;
		while (::WaitForSingleObject(theApp.m_pThreadSMManager->m_hThread, 200) != WAIT_OBJECT_0)
		{
			int nErrCode = ::GetLastError();
			if (6 == nErrCode) break;
			::Sleep(100);
			if (itry++ > 9) break;
		}
		theApp.m_pThreadSMManager = NULL;
	}

	return false;
}

//��ȡ��һ�δ�����Ϣ
const char* WINAPI CScanMachineApp::IF_GetLastErrorDesc(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return false;
}

//��ȡ���״̬
bool WINAPI CScanMachineApp::IF_GetDevStatus(IN OUT unsigned int& uStatus)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	bool bRet = false;
	int StatusTmp = 0;
	if(theApp.m_pThreadSMManager != NULL)
	{
		//���ڴ�״̬��0��������2: ���ڴ��쳣
		StatusTmp = (theApp.m_pThreadSMManager->IsConnectionOK() ? 0 : 2);
		//����ͨѶ״̬��0��������4: ����ͨѶ�쳣
		StatusTmp += theApp.m_pThreadSMManager->m_DevStatus; 
		uStatus = StatusTmp;
		bRet = true;
	}

	return bRet;
}

//�����豸��ʼ������
bool WINAPI CScanMachineApp::IF_SetDevInitData(IN const char* szParamContext, IN const int& iParamSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	char log[1024] = {0};
	_snprintf_s(log, 1024, "ETC����-���ó�ʼ�����ݽӿ�(IF_SetDevInitData)���ÿ�ʼ,����:%s", szParamContext);
	theApp.RecordMsgTemp(log);

	if(szParamContext == NULL || iParamSize == 0)
	{
		return false;
	}

	theApp.m_pA5InitJsonData = szParamContext;
	return true;
}


//���鴦��ʼ
bool WINAPI CScanMachineApp::IF_EtcEventDealStart(IN const char* szParamContext, IN const int& iParamSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	bool bRet = false;
	char log[800] = {0};
	_snprintf_s(log, 800, "ETC����-���鿪ʼ�ӿ�(IF_EtcEventDealStart)���ÿ�ʼ,����:%s", szParamContext);
	theApp.RecordMsgTemp(log);

    if(theApp.m_pThreadSMManager != NULL)
	{
		bRet = theApp.m_pThreadSMManager->EventDealStart(szParamContext, iParamSize);
	}

	return bRet;
}

//��������ȷ��
bool WINAPI CScanMachineApp::IF_CheckVehQueue(IN const char* szParamContext, IN const int& iParamSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	bool bRet = false;
	char log[800] = {0};
	_snprintf_s(log, 800, "ETC����-��������ȷ���¼��ӿ�(IF_EventCheckVehQueue)���ÿ�ʼ,����:%s", szParamContext);
	theApp.RecordMsgTemp(log);

	if(theApp.m_pThreadSMManager != NULL)
	{
		bRet = theApp.m_pThreadSMManager->EventCheckVehQueue(szParamContext, iParamSize);
	}
	return bRet;
}

//������Ϣȷ��
bool WINAPI CScanMachineApp::IF_CheckVehInfo(IN const char* szParamContext,IN const int& iParamSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	bool bRet = false;
	char log[800] = {0};
	_snprintf_s(log, 800, "ETC����-������Ϣȷ���¼��ӿ�(IF_CheckVehInfo)���ÿ�ʼ,����:%s", szParamContext);
	theApp.RecordMsgTemp(log);

	if(theApp.m_pThreadSMManager != NULL)
	{
		bRet = theApp.m_pThreadSMManager->EventCheckVehInfo(szParamContext, iParamSize);
	}

	return bRet;
}

//�����Ϣȷ��
bool WINAPI CScanMachineApp::IF_CheckEntryInfo(IN const char* szParamContext,IN const int& iParamSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	bool bRet = false;
	char log[1024] = {0};
	_snprintf_s(log, 1024, "ETC����-�����Ϣȷ���¼��ӿ�(IF_CheckEntryInfo)���ÿ�ʼ,����:%s", szParamContext);
	theApp.RecordMsgTemp(log);

	if(theApp.m_pThreadSMManager != NULL)
	{
		bRet = theApp.m_pThreadSMManager->EventCheckEntryInfo(szParamContext, iParamSize);
	}

	return bRet;
}

//�Ѷ���ʾ
bool WINAPI CScanMachineApp::IF_ShowFeeInfo(IN const char* szParamContext,IN const int& iParamSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	bool bRet = false;
	char log[2048] = {0};
	_snprintf_s(log, 2048, "ETC����-�Ѷ���Ϣ��ʾ�ӿ�(IF_ShowFeeInfo)���ÿ�ʼ,����:%s", szParamContext);
	theApp.RecordMsgTemp(log);

	if(theApp.m_pThreadSMManager != NULL)
	{
		bRet = theApp.m_pThreadSMManager->EventShowFeeInfo(szParamContext, iParamSize);
	}

	return bRet;
}

//֧��״̬��ʾ
bool WINAPI CScanMachineApp::IF_PayResultDisplay(IN const char* szParamContext,IN const int& iParamSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	bool bRet = false;
	char log[800] = {0};
	_snprintf_s(log, 800, "ETC����-֧�����չʾ�ӿ�(IF_PayResultDisplay)���ÿ�ʼ,����:%s", szParamContext);
	theApp.RecordMsgTemp(log);

	if(theApp.m_pThreadSMManager != NULL)
	{
		bRet = theApp.m_pThreadSMManager->EventPayResultShow(szParamContext, iParamSize);
	}

	return bRet;
}

//���鴦�����
bool WINAPI CScanMachineApp::IF_EtcEventDealStop(IN const char* szParamContext,IN const int& iParamSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	bool bRet = false;
	char log[1024] = {0};
	_snprintf_s(log, 1024, "���鴦������ӿ�(IF_EtcEventDealStop)���ÿ�ʼ,����:%s", szParamContext);
	theApp.RecordMsgTemp(log);

	if(theApp.m_pThreadSMManager != NULL)
	{
		bRet = theApp.m_pThreadSMManager->EventDealStop(szParamContext, iParamSize);
	}

	return bRet;
}

//����ɨ��
bool WINAPI CScanMachineApp::IF_StartScan(IN const int& iFeeValue)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	bool bRet = false;
	char log[800] = {0};
	_snprintf_s(log, 800, "����ɨ��ӿ�(IF_StartScan)���ÿ�ʼ,����:%d", iFeeValue);
	theApp.RecordMsgTemp(log);

	if(theApp.m_pThreadSMManager != NULL)
	{
		bRet = theApp.m_pThreadSMManager->EventStartScan();
	}

	return bRet;
}

//ֹͣɨ��
bool WINAPI CScanMachineApp::IF_StopScan()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	bool bRet = false;
	char log[128] = {0};
	_snprintf_s(log, 128, "ֹͣɨ��ӿ�(IF_StopScan)���ÿ�ʼ.");
	theApp.RecordMsgTemp(log);

	if(theApp.m_pThreadSMManager != NULL)
	{
		bRet = theApp.m_pThreadSMManager->EventStopScan();
	}
	return false;
}

//ETC��������ʾ
bool WINAPI CScanMachineApp::IF_CardOperationNotify(IN const char* szParamContext, IN const int& iParamSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	bool bRet = false;
	char log[512] = {0};
	_snprintf_s(log, 512, "��������ʾ�ӿ�(IF_CardOperationNotify)���ÿ�ʼ,����:%s", szParamContext);
	theApp.RecordMsgTemp(log);

	if(theApp.m_pThreadSMManager != NULL)
	{
		bRet = theApp.m_pThreadSMManager->EventCardOperationNotify(szParamContext, iParamSize);
	}

	return bRet;
}

//���������޸�
bool WINAPI CScanMachineApp::IF_ModifyVehQueue(IN const char* szParamContext, IN const int& iParamSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	bool bRet = false;
	char log[2048] = {0};
	_snprintf_s(log, 2048, "�������������ӿ�(IF_ModifyVehQueue)���ÿ�ʼ,����:%s", szParamContext);
	theApp.RecordMsgTemp(log);

	if(theApp.m_pThreadSMManager != NULL)
	{
		bRet = theApp.m_pThreadSMManager->EventModifyVehQueue(szParamContext, iParamSize);
	}

	return bRet;
}

bool WINAPI CScanMachineApp::IF_GetData(IN OUT CString& strData, IN OUT int& iDataType, IN OUT int& iSrcType)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	bool bRet = false;
	char log[128] = {0};
	_snprintf_s(log, 128, "��ȡ���ݽӿ�(IF_GetData)���ÿ�ʼ");
	theApp.RecordMsgTemp(log);
	
	strData = theApp.m_QRcode;
	iDataType = MAKELONG(0x01, 0x01);;
	iSrcType = 1;
	bRet = true;

	return bRet;
}

//��С�Ѷ���Ȩ
bool WINAPI CScanMachineApp::IF_FeeAuthorize(IN const char* szParamContext, IN const int& iParamSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	bool bRet = false;
	char log[512] = {0};
	_snprintf_s(log, 512, "��С�Ѷ���Ȩ�ӿ�(IF_FeeAuthorize)���ÿ�ʼ,����:%s", szParamContext);
	theApp.RecordMsgTemp(log);

	if(theApp.m_pThreadSMManager != NULL)
	{
		bRet = theApp.m_pThreadSMManager->EventFeeAuthorize(szParamContext, iParamSize);
	}

	return bRet;
}

//������Ȩ
bool WINAPI CScanMachineApp::IF_EventAuthorize(IN const char* szParamContext, IN const int& iParamSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	bool bRet = false;
	char log[512] = {0};
	_snprintf_s(log, 512, "������Ȩ�ӿ�(IF_EventAuthorize)���ÿ�ʼ,����:%s", szParamContext);
	theApp.RecordMsgTemp(log);

	if(theApp.m_pThreadSMManager != NULL)
	{
		bRet = theApp.m_pThreadSMManager->EventAuthorize(szParamContext, iParamSize);
	}

	return bRet;
}

//ɾ���������н��
bool WINAPI CScanMachineApp::IF_DelVehQueueResult(IN const char* szParamContext, IN const int& iParamSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	bool bRet = false;
	char log[512] = {0};
	_snprintf_s(log, 512, "ɾ���������н���ӿ�(IF_DelVehQueueResult)���ÿ�ʼ,����:%s", szParamContext);
	theApp.RecordMsgTemp(log);

	if(theApp.m_pThreadSMManager != NULL)
	{
		bRet = theApp.m_pThreadSMManager->EventDelVehQueueResult(szParamContext, iParamSize);
	}

	return bRet;
}



//��ά����ʾ
bool WINAPI CScanMachineApp::IF_Tr500QRcodeDisplay(IN const char* strJsonParam, IN const int& iJsonParamSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	bool bRet = false;
	char log[1024] = {0};
	_snprintf_s(log, 1024, "��ά����ʾ�ӿ�(IF_QRcodeDisplay)���ÿ�ʼ,����:%s", strJsonParam);
	theApp.RecordMsgTemp(log);

	if(theApp.m_pThreadSMManager != NULL)
	{
		bRet = theApp.m_pThreadSMManager->QRcodeDisplay(strJsonParam, iJsonParamSize);
	}

	return bRet;
}

//�Ѷ���ʾ
bool WINAPI CScanMachineApp::IF_Tr500FeeDisplay(IN const char* strJsonParam, IN const int& iJsonParamSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	bool bRet = false;
	char log[512] = {0};
	_snprintf_s(log, 512, "�Ѷ���ʾ�ӿ�(IF_FeeDisplay)���ÿ�ʼ,����:%s", strJsonParam);
	theApp.RecordMsgTemp(log);

	if(theApp.m_pThreadSMManager != NULL)
	{
		bRet = theApp.m_pThreadSMManager->FeeDisplay(strJsonParam, iJsonParamSize);
	}

	return bRet;
}

//֧�������ʾ
bool WINAPI CScanMachineApp::IF_Tr500PayResultDisplay(IN const char* strJsonParam, IN const int& iJsonParamSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	bool bRet = false;
	char log[512] = {0};
	_snprintf_s(log, 512, "֧�������ʾ�ӿ�(IF_PayResultDisplay)���ÿ�ʼ,����:%s", strJsonParam);
	theApp.RecordMsgTemp(log);

	if(theApp.m_pThreadSMManager != NULL)
	{
		bRet = theApp.m_pThreadSMManager->PayResultDisplay(strJsonParam, iJsonParamSize);
	}

	return bRet;
}

//����
bool WINAPI CScanMachineApp::IF_Tr500ClearScreen(IN const char* strJsonParam, IN const int& iJsonParamSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	bool bRet = false;
	char log[256] = {0};
	_snprintf_s(log, 256, "�����ӿ�(IF_ClearScreen)���ÿ�ʼ,����:%s", strJsonParam);
	theApp.RecordMsgTemp(log);

	if(theApp.m_pThreadSMManager != NULL)
	{
		bRet = theApp.m_pThreadSMManager->ClearScreen(strJsonParam, iJsonParamSize);
	}

	return bRet;
}
