// TWSDNetPay_Dll.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "TWSDNetPay_Dll.h"
#include "fs_funcs.h"
#include "pystring.h"
#include "TypeHelper.h"
#include "XMLHelper.h"
#include "sqlite3_agent.h"
#include "ProtocolHelper.h"
#include "RunParameter.h"
#include "ETCPayHelper.h"

#include "unexcept/mdump.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//Ψһȫ�ֱ���
CTWSDNetPay_DllApp theApp;

CTWSDNetPay_DllApp* GetGlobalApp()  { return &theApp; }

BEGIN_MESSAGE_MAP(CTWSDNetPay_DllApp, CWinApp)
END_MESSAGE_MAP()

/***********************������ʼ��*********************************/
CThreadManage*  CTWSDNetPay_DllApp::m_pThreadManage                             = NULL;
BOOL            CTWSDNetPay_DllApp::m_bIsInitEnvironment                        = FALSE;
BOOL            CTWSDNetPay_DllApp:: m_bAuthorizedLegal                         = FALSE;
BOOL            CTWSDNetPay_DllApp::m_bNetErrorHttpServerInTollNetNotExist      = FALSE;
BOOL            CTWSDNetPay_DllApp::m_bNetErrorHttpServerTrawePlatformNotExist  = FALSE;
BOOL            CTWSDNetPay_DllApp::m_bSaveLog                                  = FALSE;
DWORD           CTWSDNetPay_DllApp::m_dwThreadCheckSpanTime                     = 300;
CString         CTWSDNetPay_DllApp::m_strWorkDir                                = _T("");
CString         CTWSDNetPay_DllApp::m_strLastErrorDesc                          = _T("");
CString         CTWSDNetPay_DllApp::m_strVirtualCardPrefix                      = _T("");
tagTranstParam  CTWSDNetPay_DllApp::m_sTranstParam                              = {0};
BOOL            CTWSDNetPay_DllApp::m_bCurrentDebitMode                         = FALSE;
BOOL            CTWSDNetPay_DllApp::m_bIsTrading                                = FALSE;
//CPayKeyItems* CTWSDNetPay_DllApp::m_pCurrentDebitInfo                         = NULL;
CPayKeyItems    CTWSDNetPay_DllApp::m_pCurrentDebitInfo;
tagDebitResultItems CTWSDNetPay_DllApp::m_sCurrentDebitResult                   = {0};
DWORD               CTWSDNetPay_DllApp::m_dwTradeBeginOvertime                  = 0;
DWORD               CTWSDNetPay_DllApp::m_dwUpdateDevieTimeSpanTime             = 0;
BOOL                CTWSDNetPay_DllApp::m_bIsTestData                           = FALSE;
CString         CTWSDNetPay_DllApp::m_strNetPayDllVersion                       = _T("");
CString         CTWSDNetPay_DllApp::m_strDeviceDllVersion                       = _T("");
int             CTWSDNetPay_DllApp::m_iDeleteLogStartHour                       = 0;
int             CTWSDNetPay_DllApp::m_iDeleteLogEndHour                         = 0;
int             CTWSDNetPay_DllApp::m_iDeleteLogSpanTime                        = 0;
int             CTWSDNetPay_DllApp::m_iDeleteLogSaveDays                        = 0;
int             CTWSDNetPay_DllApp::m_iDebitTaskNo                              = 0;
int             CTWSDNetPay_DllApp::m_iNotifyMode                               = 1;
int             CTWSDNetPay_DllApp::m_iLastDelayTime                            = 20;
time_t          CTWSDNetPay_DllApp::m_tResendDataSpanTime                       = 30;
NotifyCallerFun CTWSDNetPay_DllApp::m_fnCallbackNotifyFun                       = NULL;
NotifyCallerFun45 CTWSDNetPay_DllApp::m_fnCallbackNotifyFun45                   = NULL;
void*           CTWSDNetPay_DllApp::m_pIndPtrFor45                              = NULL;
int             CTWSDNetPay_DllApp::m_iExitWaitForTime                          = 500;
int             CTWSDNetPay_DllApp::m_iHTTPVersion                              = 0;
int             CTWSDNetPay_DllApp::m_iDeviceType                               = 0;
CAgencyPay      CTWSDNetPay_DllApp::m_AgencyPay;
bool            CTWSDNetPay_DllApp::m_bReturnPayChannelCodeUnify                = false;
int             CTWSDNetPay_DllApp::m_iAgencyDebitTimeout                       = 0;
int             CTWSDNetPay_DllApp::m_iClearMMIDelayTime                        = 0;
bool            CTWSDNetPay_DllApp::m_bUseAgencyDll                             = true;
BOOL            CTWSDNetPay_DllApp::m_bIsExternMMIDetail                        = FALSE;
CString         CTWSDNetPay_DllApp::m_strTWProductSerialNo                      = _T("");
CString         CTWSDNetPay_DllApp::m_strTWProductFirmwareVer = _T("");
CString         CTWSDNetPay_DllApp::m_strTWProductSlaveFirmwareVer = _T("");
BOOL            CTWSDNetPay_DllApp::m_bIsAllowUseNewMMIDetailMode               = FALSE;

int             CTWSDNetPay_DllApp::m_iNFCModuleState                           = 0;
bool            CTWSDNetPay_DllApp::m_bNFCSignInSuccess                         = false;
bool            CTWSDNetPay_DllApp::m_bNFCIsUse                                 = false;
int             CTWSDNetPay_DllApp::m_iNFCCheckSignSpanTime                     = 0;
BOOL            CTWSDNetPay_DllApp::m_bNFCPayFail                               = FALSE;
CString         CTWSDNetPay_DllApp::m_strNFCTerminal                            = _T("");
CString         CTWSDNetPay_DllApp::m_strNFCSerialNo                            = _T("");
int             CTWSDNetPay_DllApp::m_iNFCSignFlowDataCount                     = 0;
int             CTWSDNetPay_DllApp::m_iNFCDefaultStartReSignHour                = 0;
int             CTWSDNetPay_DllApp::m_iNFCDefaultEndReSignHour                  = 0;
BOOL            CTWSDNetPay_DllApp::m_bNFCAutoSignIsExecute                     = FALSE;
int             CTWSDNetPay_DllApp::m_iNFCSignDataLimitSpanTime                 = 0;
time_t          CTWSDNetPay_DllApp::m_tNFCLastSignDataTime                      = 0;
BOOL            CTWSDNetPay_DllApp::m_bNFCIsAllowDebitWithoutCancelData         = FALSE;
int             CTWSDNetPay_DllApp::m_iNFCGetCancelDataLimitTime                = 0;
time_t          CTWSDNetPay_DllApp::m_tNFCStartSignInTime                       = 0;
int             CTWSDNetPay_DllApp::m_iNFCPayFailCount                          = 0;

/**********************************EtcEvent���********************************************/
CString         CTWSDNetPay_DllApp::m_strEtcEventLastErrorDesc                  = _T("");
NotifyCallerEtcEvent  CTWSDNetPay_DllApp::m_CallbackNotifyEtcEvent	     		= NULL;
void*			CTWSDNetPay_DllApp::m_pIndPtrEtcEvent							= NULL;
bool			CTWSDNetPay_DllApp::m_bIsEtcEventEnvironmeInited				= false;
CThreadNotifyCaller* CTWSDNetPay_DllApp::m_pThreadNotifyCaller					= NULL;
UINT			CTWSDNetPay_DllApp::NotifyCallerThreadID						= 0;
UINT			CTWSDNetPay_DllApp::DevMangerThreadID							= 0;
UINT			CTWSDNetPay_DllApp::EtcEventRespMessgeID						= 0;
HWND			CTWSDNetPay_DllApp::EtcEventCallerWnd							= 0;
UINT			CTWSDNetPay_DllApp::EtcEventCallerThreadID						= 0;
BOOL			CTWSDNetPay_DllApp::m_bEnableMediaGateWay						= false;
UINT			CTWSDNetPay_DllApp::m_iCurrentWorkMode							= 0;
WebSocketServer* CTWSDNetPay_DllApp::m_pWebSocketServerReader					= NULL;



time_t CTWSDNetPay_DllApp::m_lastPayTime = 0;

std::string CTWSDNetPay_DllApp::m_CommonInitPara = std::string();



void CopyExpwayInfo(const tagTranstParam& lane_config, DebitParameter&dp);
void CopyExpwayInfo(const tagTranstParam& lane_config, DebitCancleParameter&dp);
void AdjustExpwayInfo(DebitParameter&dp);

// ���ݽӿں���
void __cdecl DataForwardRecv(int destKey, char *message, int len, int srcKey)
{
    theApp.DataForwardRead(destKey, message, len, srcKey);
    return;
}


std::string TmpGetDumpDir()
{
    char modePath[300] = {0};
    memset(modePath, 0, 260);

    GetModuleFileName(AfxGetInstanceHandle(), modePath, 260);

    std::string pathStr = modePath;
    size_t pos = pathStr.find_last_of('\\');
    if (pos == std::string::npos)
    {
        return "";
    }
    pathStr = pathStr.substr(0, pos) + "\\TWSDNetPayLog";
    return pathStr;
}

void TranstParam2LPRInitPara(const tagTranstParam&src, LPRInitData& dest);

void _recvMsgFuncFromWebsocket(void* pArgs, const char* pszData, int nSize)
{
	if (nullptr == pArgs || nullptr == pszData)
		return;

	theApp.RecvMsgFuncFromWebsocket(pszData��nSize);
    return;
}

CTWSDNetPay_DllApp::CTWSDNetPay_DllApp():
    m_SendId(20)
{

    m_connect_bus_core = NULL;
    m_commonbus_module = NULL;
    m_disconnect_bus_core = NULL;
    m_send_core = NULL;
    m_register_core = NULL;
    m_unRegister_core = NULL;
    m_bushandle_core = 0;
    hFileUpdate = NULL;

    m_NfcLinkSuccess = false;

    SetLPRQueryOverTimeMilli();


    m_Forward_Dll = NULL;
    m_Forward_RegisterFun = NULL;
    m_Forward_UnregisterFun = NULL;
    m_Forward_WriteFun = NULL;
    m_Forward_ReadFun = NULL;


	m_hModule_Device = NULL;


	m_MessageCenter = 0;

    ::InitializeCriticalSection(&m_DllAccessMutex);

    //ConnectBus();
    //this->register2Bus(KEY_BUSINESS);


}

CTWSDNetPay_DllApp::~CTWSDNetPay_DllApp()
{
    this->unRegister2Bus(KEY_BUSINESS);
    DisConnectBus();
}

////////////////////////////////////////////////////////////////////////////////////////////////
// �������ƣ�CString   FmtStr
//
// ����������
//
// ���������const char * lpzFmt
//         ��...
// ��д��Ա��ROCY
// ��дʱ�䣺���ڶ� 2009��9��15��
// �޸���Ա��
// �޸�ʱ�䣺
// �����汾��1.0.0.1
// ��ע˵����
//
////////////////////////////////////////////////////////////////////////////////////////////////
CString CTWSDNetPay_DllApp::FmtStr(const TCHAR* lpsFmt, ...)
{
    CString str;
    va_list lpzArg = NULL;

    va_start(lpzArg, lpsFmt);
    str.FormatV(lpsFmt, lpzArg);

    va_end(lpzArg);

    return str;
}

time_t OleTimeToTime(DATE dTime)
{
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

////////////////////////////////////////////////////////////////////////////////////////////////
// �������ƣ�CString2time_t
//
// ����������ʱ���ʽ�ַ���ת��Ϊ ������ ��ʱ��
//
// ���������LPCTSTR lpstime
// ���������time_t
// ��д��Ա��ROCY
// ��дʱ�䣺���ڶ� 2009��9��15��
// �޸���Ա��
// �޸�ʱ�䣺
// �����汾��1.0.0.1
// ��ע˵����
//
////////////////////////////////////////////////////////////////////////////////////////////////
time_t CTWSDNetPay_DllApp::CString2time_t(LPCTSTR lpztime)
{
    CString csTemp = _T("");
    COleDateTime oleGetTime;
    csTemp.Format( _T("%s"), lpztime);
    if ( csTemp.GetLength() > 0 )
    {
        if ( csTemp.GetLength() > 19 ) //�������Ƹ�ʽ _T("2014-11-23 12:34:56.000");
        {
            csTemp = csTemp.Left(19);
        }

        if ( oleGetTime.ParseDateTime(csTemp) )
        {
            return OleTimeToTime(oleGetTime.m_dt);
        }
        else
        {
            return time_t( DEFAULT_TIME );
        }
    }
    else
    {
        return 0;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////
// �������ƣ�GetGuidString
//
// ������������ȡ�µ�GUID�ַ���
//
// ����������޲���
// ���������const
// ��д��Ա��ROCY
// ��дʱ�䣺2012��11��12��
// �޸���Ա��
// �޸�ʱ�䣺
// �����汾��1.0.0.1
// ��ע˵����
//
////////////////////////////////////////////////////////////////////////////////////////////////
const CString CTWSDNetPay_DllApp::GetGuidString(IN const int& iType)
{
    CString strReturn;
    static char buf[64] = {0};
    GUID guid;
    if (S_OK == ::CoCreateGuid(&guid))
    {
        strReturn.Format(_T("{%08X-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X}")
                         , guid.Data1
                         , guid.Data2
                         , guid.Data3
                         , guid.Data4[0], guid.Data4[1]
                         , guid.Data4[2], guid.Data4[3], guid.Data4[4], guid.Data4[5]
                         , guid.Data4[6], guid.Data4[7]
                        );
    }
    if (1 == iType)
    {
        strReturn.Replace(_T("-"), _T(""));
        strReturn.Replace(_T("{"), _T(""));
        strReturn.Replace(_T("}"), _T(""));
    }
    return strReturn;
}


////////////////////////////////////////////////////////////////////////////////////////////////
// �������ƣ�CTWSDNetPay_DllApp::GetCurrentTimeString
//
// ������������ȡ��ǰ��ʱ��ֵ
//
// ���������IN const int& iTimeType
// ���������const
// ��д��Ա��ROCY
// ��дʱ�䣺2016��8��19��
// �޸���Ա��
// �޸�ʱ�䣺
// �����汾��1.0.0.1
// ��ע˵����
//
////////////////////////////////////////////////////////////////////////////////////////////////
const CString CTWSDNetPay_DllApp::GetCurrentTimeString(IN const int& iTimeType)
{
    SYSTEMTIME sysTime = {0};
    ::GetLocalTime(&sysTime);
    CString strTime;
    if (0 == iTimeType)
    {
        strTime.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"), sysTime.wYear, sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
    }
    else
    {
        strTime.Format(_T("%04d-%02d-%02d %02d:%02d:%02d.%03d"), sysTime.wYear, sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond, sysTime.wMilliseconds);
    }
    return strTime;
}

//////////////////////////////////////////////////////////////////////////////////
///��������: MakeDirectory()
///����˵��: ����ָ���Ķ༶Ŀ¼
///����˵��: CString dd,���������Ķ༶Ŀ¼·��
///����˵��: TRUE  -- �����ɹ�
///          FALSE -- ����ʧ��
///�޸�ʱ��: 2006-04-12
///�޸���Ա: ROCY
///��    ע: ��Ҫ���� windows.h
//////////////////////////////////////////////////////////////////////////////////
BOOL CTWSDNetPay_DllApp::MakeDirectory(CString strCreateDir)
{
    HANDLE  fFile;                      // File Handle
    WIN32_FIND_DATA fileinfo = {0};     // File Information Structure
    CStringArray arr;               // CString Array to hold Directory Structures
    BOOL bSuccess = false;              // BOOL used to test if Create Directory was successful
    int nCount = 0;                     // Counter
    CString strTemp;                    // Temporary CString Object

    fFile = FindFirstFile(strCreateDir, &fileinfo);

    // if the file exists and it is a directory
    if (fileinfo.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
    {
        //  Directory Exists close file and return
        FindClose(fFile);
        return TRUE;
    }
    arr.RemoveAll();

    for (nCount = 0; nCount < strCreateDir.GetLength(); nCount++ ) // Parse the supplied CString Directory String
    {
        if (strCreateDir.GetAt(nCount) != '\\') // if the Charachter is not a \ 
            strTemp += strCreateDir.GetAt(nCount);  // add the character to the Temp String
        else
        {
            arr.Add(strTemp);   // if the Character is a \ 
            strTemp += _T("\\");   // Now add the \ to the temp string
        }
        if (nCount == strCreateDir.GetLength() - 1) // If we reached the end of the String
            arr.Add(strTemp);
    }

    // Close the file
    FindClose(fFile);

    // Now lets cycle through the String Array and create each directory in turn
    for (nCount = 1; nCount < arr.GetSize(); nCount++)
    {
        strTemp = arr.GetAt(nCount);
        bSuccess = CreateDirectory(strTemp, NULL);

        // If the Directory exists it will return a false
        if (bSuccess)
            SetFileAttributes(strTemp, FILE_ATTRIBUTE_NORMAL);
        // If we were successful we set the attributes to normal
    }
    //  Now lets see if the directory was successfully created
    fFile = FindFirstFile(strCreateDir, &fileinfo);

    arr.RemoveAll();
    if (fileinfo.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
    {
        //  Directory Exists close file and return
        FindClose(fFile);
        return TRUE;
    }
    else
    {
        // For Some reason the Function Failed  Return FALSE
        FindClose(fFile);
        return FALSE;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
//  ��   �� : ROCY
//  ʱ   �� : 2008-06-01
//  ��   �� : 1.0.0.1
//  �������� : GetVersionOfFile
//  �������� : ��ȡָ�����ļ��İ汾��
//  ������� : LPCTSTR  --  lpsPath �ļ��ľ���·��
//  ������� : DWORD -- ���صı��ؼ����IP�������룬�ֽںϳɸ�ʽ
//  ��ע˵�� : ��
///////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CTWSDNetPay_DllApp::GetVersionOfFile(LPCTSTR lpsPath)
{
    DWORD   dwLength = 0;
    DWORD   dwNullHandle = 0;
    UINT    uiVerLength;
    LPVOID  lpVersionPtr;
    CString strVer = _T("0.0.0.0");

    dwLength = ::GetFileVersionInfoSize(lpsPath, &dwNullHandle);
    if (dwLength > 0)
    {
        BYTE* pVersionInfo = new BYTE [dwLength];
        if (::GetFileVersionInfo (lpsPath, NULL, dwLength, pVersionInfo))
        {
            if (::VerQueryValue (pVersionInfo, _T ("\\"), &lpVersionPtr, &uiVerLength))
            {
                VS_FIXEDFILEINFO * p = (VS_FIXEDFILEINFO *)lpVersionPtr;
                strVer.Format(_T("%d.%d.%d.%d"),
                              HIWORD(p->dwFileVersionMS),
                              LOWORD(p->dwFileVersionMS),
                              HIWORD(p->dwFileVersionLS),
                              LOWORD(p->dwFileVersionLS));
            }
        }
        delete []pVersionInfo;
    }
    return strVer;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// �������ƣ�NotifyCaller
//
// ����������֪ͨ�ⲿ�����ߣ�ҵ���첽����õ��Ľ��
//
// ���������IN const int& iOption             -- ֪ͨ���ͣ�1���ۿ2��������
//         ��IN const int& iResult             -- ���������������ĵ�
// �����������
// ��д��Ա��ROCY
// ��дʱ�䣺2016��7��23��
// �޸���Ա��
// �޸�ʱ�䣺
// �����汾��1.0.0.1
// ��ע˵�����ӿ�1
//
////////////////////////////////////////////////////////////////////////////////////////////////
void CTWSDNetPay_DllApp::NotifyCaller(IN const int& iOption, IN const int& iResult)
{
    switch (m_iNotifyMode)
    {
    case 1:
    {

        //֪ͨ�����߷��ؽ������ǰ����ʧ��
        if (0 != m_sTranstParam.CallerThreadID)
        {
            ::PostThreadMessage(m_sTranstParam.CallerThreadID, m_sTranstParam.ActiveNotifyMsgID, (WPARAM)MAKELONG(MAKEWORD(iOption, iResult), MAKEWORD(0, 0)), (LPARAM)0);
        }

        if (::IsWindow(m_sTranstParam.CallerWnd))
        {
            ::PostMessage(m_sTranstParam.CallerWnd, m_sTranstParam.ActiveNotifyMsgID, (WPARAM)MAKELONG(MAKEWORD(iOption, iResult), MAKEWORD(0, 0)), (LPARAM)0);
        }
    }
    break;
    case 2:
    {
        //ʹ�ûص�����֪ͨ�ⲿ������
        if (NULL != m_fnCallbackNotifyFun)
        {
            m_fnCallbackNotifyFun(iOption, iResult);
        }
        else
        {
            RecordLog(FmtStr(_T("[��������Ӧ],���֪ͨ�ⲿ�����ߵĻص�����(NotifyCaller)ָ��Ϊ�գ��޷�֪ͨ������")), LOG_LEVEL_ERROR, true);
        }
    }
    break;
    case 3:
    {
        //ʹ�ûص�����֪ͨ�ⲿ������(�����ع�)
        if (NULL != m_fnCallbackNotifyFun45)
        {
            m_fnCallbackNotifyFun45(iOption, iResult, m_pIndPtrFor45);
        }
        else
        {
            RecordLog(FmtStr(_T("[��������Ӧ],���֪ͨ�ⲿ�����ߵĻص�����(NotifyCaller45)ָ��Ϊ�գ��޷�֪ͨ������")), LOG_LEVEL_ERROR, true);
        }
    }
    break;
    default:
        break;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////
// �������ƣ�CTWSDNetPay_DllApp::CheckIsAlNum
//
// �������������������ַ������Ƿ�Ϊ���ֺ���ĸ�����ֻ���ĸ������һ��
//
// ���������IN const char* szData
// ���������BOOL
// ��д��Ա��ROCY
// ��дʱ�䣺2017��4��27��
// �޸���Ա��
// �޸�ʱ�䣺
// �����汾��1.0.0.1
// ��ע˵����
//
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CTWSDNetPay_DllApp::CheckIsAlNum(IN const char* szData, IN const int& iType)
{
    BOOL bReturn = TRUE;

    if (NULL == szData)
        return bReturn;

    int iLen = _tcslen(szData);
    for (int i = 0; i < iLen; ++i)
    {
        switch (iType)
        {
        case 0://�Ƿ����֡���ĸ���
        {
            if (0 == isalnum(szData[i]))
            {
                bReturn = FALSE;
            }
        }
        break;
        case 1://�Ƿ�����
        {
            if ( (szData[i] < '0') || (szData[i] > '9') )
            {
                bReturn = FALSE;
            }
        }
        break;
        case 2://�Ƿ���ĸ
        {
            if (0 == isalpha(szData[i]))
            {
                bReturn = FALSE;
            }
        }
        break;
        }
        if (!bReturn)
        {
            break;
        }
    }

    return bReturn;
}



//����վ�Ż��߳�����ת�������У�������ڲ��Ƿ�����ĸ
BOOL CTWSDNetPay_DllApp::CheckStringLetter(char * strCheck, int nType)
{
    CString csCheck = _T("");
    csCheck.Format( _T("%s"), strCheck );
    csCheck.MakeUpper();

    if ( 1 == nType ) //����1��ʾվ�����
    {
        if ( csCheck.GetLength() > 9 ) //�Դ���9λ���ȵ�վ����룬Ĭ�ϰ����ַ�������
        {
            return TRUE;
        }
    }

    BOOL bCheck = FALSE;
    for (int i = 0; i < csCheck.GetLength(); ++i)
    {
        char    tc = csCheck.GetAt( i );
        if ( tc >= _T('A') && tc <= _T('Z') )
        {
            bCheck = TRUE;
            break;
        }
    }
    return bCheck;
}

//#include "unexcept\CatchDump.h"

BOOL CTWSDNetPay_DllApp::InitInstance()
{


	printf("\nCTWSDNetPay_DllApp::InitInstance()  start-----------------------\n");

    CWinApp::InitInstance();

	//MiniDump();

    TCHAR szTempDir[260] = {0};
    ::GetModuleFileName(AfxGetInstanceHandle(), szTempDir, sizeof(TCHAR) * 260);
    CString strTemp(szTempDir);
    CString strDir;
    strDir = strTemp.Left(strTemp.ReverseFind('\\'));
    m_strWorkDir = strDir;


	//string cmd = "del   ";
	//cmd += strDir.GetBuffer();
	//cmd += "\\*.dllbak";

	//printf("[#CTWSDNetPay_DllApp::InitInstance() cmd:---->%s\r\n",cmd.c_str());
	//system(cmd.c_str());









    //add 2018-04 ��ʼ��sqlite
    CString sqlite_dll_file;
    sqlite_dll_file.Format(_T("%s\\sqlite3.dll"), m_strWorkDir);
    sqlite3_agent::LoadSqlite(sqlite_dll_file);
    //��ȡ�����ļ�
    ReadConfigInfo();

    //������־Ŀ¼
    strTemp.Format(_T("%s\\TWSDNetPayLog"), m_strWorkDir);
    _tcscpy_s(m_sTranstParam.PathLogDir, _countof(m_sTranstParam.PathLogDir), strTemp.GetBuffer(0));
    strTemp.ReleaseBuffer();


	printf("\n��ʼ������־Ŀ¼������������\n");

	MakeDirectory(m_sTranstParam.PathLogDir);
	printf("\n������־Ŀ¼����������������\n");


    //��ȡ����汾
    CString strDllFilePath(szTempDir);

    m_strNetPayDllVersion = GetVersionOfFile(strDllFilePath);

    strDllFilePath.Format(_T("%s\\%s"), strDir, FILE_NAME_DEVICE_DLL);
    m_strDeviceDllVersion = GetVersionOfFile(strDllFilePath);


	//��̬����ؾ��   ------ 2020-12-05 --- ���º����µ��豸�����ϱ�����ռ�ã���ֹ��ԭΪ�ɰ汾

	//m_hModule_Device = ::LoadLibrary(strDllFilePath);



    //��ʼ����־ϵͳ
    if (m_bSaveLog)
    {
        INIT_LOG(m_sTranstParam.PathLogDir, _T(""));

        RecordLog(FmtStr(_T("\r\n/*************************************��������(ҵ��%s - �豸%s)*************************************/"), m_strNetPayDllVersion, m_strDeviceDllVersion));
    }

    hFileUpdate = CreateEvent(NULL, FALSE, TRUE,    NULL);  //Ĭ�����ԡ��Զ����á���ʼ��
    if ( NULL == hFileUpdate )
    {
        RecordLog( _T("���� �������±��ش洢�ļ� ͬ������ʧ�ܣ�") );
    }

    int int_para = GetGlobalApp()->GetLPRQueryOverTimeMilli();
    RecordLog(FmtStr(_T("LPR��ʱ����:%dms"), int_para));

    //����NFC
    m_strNFCSerialNo.Format(_T("53 4E 32 30 31 35 30 31 30 38 30 31 00"));
    m_strNFCTerminal.Format(_T("4C 41 4E 44 49 2D 45 35 58 30 00"));

    m_strNFCSerialNo.Replace(_T(" "), _T(""));
    m_strNFCTerminal.Replace(_T(" "), _T(""));
    ConnectBus();
    this->register2Bus(KEY_BUSINESS);



    //catch
	/*
    std::string tmpStr = TmpGetDumpDir();
    CString strDumpFilePath;
    strDumpFilePath.Format(_T("%s\\DumpFile.dmp"), tmpStr.c_str());
    CCatchDump *m_pCatchDump = new CCatchDump(1);

    if(NULL!=m_pCatchDump)
    {
        m_pCatchDump->InitCatchExecption(strDumpFilePath,FALSE,_T(""),2);
    }

    */


    return TRUE;
}

int CTWSDNetPay_DllApp::ExitInstance()
{
	RecordLog("AppExitInstance",LOG_LEVEL_RUN,TRUE);
    while (m_bIsTrading)
    {
        RecordLog("[ģ����Ӧ]���ڵȴ����׽���...");
        Sleep(1000);
    }
    ::DeleteCriticalSection(&m_DllAccessMutex);

	if(m_hModule_Device)
	{
		FreeLibrary(m_hModule_Device);
	}
	m_hModule_Device = NULL;



    this->unRegister2Bus(KEY_BUSINESS);
    //IF_Destroy();
    //�˳���־ϵͳ
    if (m_bSaveLog)
    {
        RecordLog(FmtStr(_T("\r\n/*************************************����ֹͣ(ҵ��%s - �豸%s)*************************************/"), m_strNetPayDllVersion, m_strDeviceDllVersion));

        QUIT_LOG();
    }
    //add 2018-04
    sqlite3_agent::UnloadSqlite();

    if ( NULL != hFileUpdate )
    {
        ::CloseHandle(hFileUpdate);
    }
    return CWinApp::ExitInstance();
}
#include <fstream>
void LogToDebugFile(std::string log)
{
	return ;
	std::ofstream out_file("debut_info.txt",ios::app);
	out_file<<log<<endl;
	out_file.close();
}

void EtcCallBackFun(int iOption, int iResult, void* pIndPtr)
{
	return;
}


/****************************************����ӿں���********************************************/

////////////////////////////////////////////////////////////////////////////////////////////////
// �������ƣ�IF_InitEnvironment
//
// ������������ʼ���������
//
// ���������IN const UINT& nThreadID              -- ������ָ�����߳�ID
//         ��IN const HWND& hWnd                   -- ������ָ���Ĵ�����
//         ��IN const unsigned int& nNotifyMsgID   -- ������ָ����֪ͨ��ϢID
//         ��IN const char* szAreaInfo             -- ������Ϣ(�������#·�κ�)
//         ��IN const char* szLoaclStation         -- վ����Ϣ(վ��#վ��)
//         ��IN const char* szLoaclLaneID          -- ��������
//         ��IN const char* szServerInfo           -- ��������Ϣ
//         ��IN const int& iProvinceID             -- ʡ�ݱ���
// ���������bool
// ��д��Ա��ROCY
// ��дʱ�䣺2016��7��23��
// �޸���Ա��
// �޸�ʱ�䣺
// �����汾��1.0.0.1
// ��ע˵�����ӿ�1
//
////////////////////////////////////////////////////////////////////////////////////////////////
bool WINAPI CTWSDNetPay_DllApp::IF_InitEnvironment(IN const UINT& nThreadID, IN const HWND& hWnd, IN const unsigned int& nNotifyMsgID, IN const char* szAreaInfo, IN const char* szLoaclStation, IN const char* szLoaclLaneID, IN const char* szServerInfo, IN const int& iProvinceID)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    //GetGlobalApp()->m_EtcPayCenter.Init();
    bool bReturn = false;


    CString strLog;
    RecordLog(FmtStr(_T("[��������Ӧ],�����ʼ���ӿ�(IF_InitEnvironment)���ÿ�ʼ")));

    try
    {
        /*******************���������Ƿ�����*******************/
        if (  (0 == nThreadID) && (NULL == hWnd))
            throw - 1;
        if ( 0 == nNotifyMsgID)
            throw - 2;
        if ( (NULL == szAreaInfo) || (0 == _tcslen(szAreaInfo)))
            throw - 3;
        if ( (NULL == szLoaclStation) || (0 == _tcslen(szLoaclStation)))
            throw - 4;
        if ( (NULL == szLoaclLaneID) || (0 == _tcslen(szLoaclLaneID)))
            throw - 5;
        if ( (NULL == szServerInfo) || (0 == _tcslen(szServerInfo)))
            throw - 6;
        if ( 0 == iProvinceID)
            throw - 7;
        if ( (0 == nThreadID) && (!::IsWindow(hWnd)))
            throw - 8;
        //���������Ϣ
        TCHAR   szTemp[256] = {0};
        CString strTemp;

        m_sTranstParam.ProvinceID            = iProvinceID;
        m_sTranstParam.ActiveNotifyMsgID     = nNotifyMsgID;
        m_sTranstParam.CallerThreadID        = nThreadID;
        m_sTranstParam.CallerWnd             = hWnd;

        //��������
        if ( CheckStringLetter( (char *)szLoaclLaneID ) )
        {
            _tcsncpy_s(m_sTranstParam.LaneConfigInfo.LaneID, _countof(m_sTranstParam.LaneConfigInfo.LaneID), szLoaclLaneID, _countof(m_sTranstParam.LaneConfigInfo.LaneID) - 1);
        }
        else
        {
            UINT nLaneID = _ttoi( szLoaclLaneID ) % 100000000;
            ::_stprintf(m_sTranstParam.LaneConfigInfo.LaneID, "%u", nLaneID );
        }

        if (!CheckIsAlNum(m_sTranstParam.LaneConfigInfo.LaneID))
            ;//throw - 5;

        //·�Ρ��������
        strTemp.Format(_T("%s"), szAreaInfo);
        if (-1 == strTemp.Find(_T("#")))
            throw - 9;
        char szTempArea[200] = {0};
        _tcsncpy_s(szTempArea, _countof(szTempArea), strTemp.Left(strTemp.Find(_T("#"))).GetBuffer(0), _countof(szTempArea) - 1);
        strTemp.Left(strTemp.Find(_T("#"))).ReleaseBuffer();
        if (!CheckIsAlNum(szTempArea, 0x01))
            ;//throw - 3;
        m_sTranstParam.LaneConfigInfo.AreaID = _ttoi(szTempArea);

        char szTempRaod[200] = {0};
        CString strRoadID = strTemp.Right(strTemp.GetLength() - strTemp.Find(_T("#")) - 1);
        _tcsncpy_s(szTempRaod, _countof(szTempRaod), strRoadID.GetBuffer(0), _countof(szTempRaod) - 1);
        strRoadID.ReleaseBuffer();
        if (!CheckIsAlNum(szTempRaod, 0x01))
            ;//throw - 3;
        m_sTranstParam.LaneConfigInfo.RoadID = _ttoi(strRoadID);

        //վ������վ��
        strTemp.Format(_T("%s"), szLoaclStation);
        if (-1 == strTemp.Find(_T("#")))
            throw - 10;
        CString strSTTemp = strTemp.Left(strTemp.Find(_T("#")));

        BOOL bCheck = CheckStringLetter( strSTTemp.GetBuffer(0), 1 );
        strSTTemp.ReleaseBuffer();
        if ( bCheck )
        {
            _tcsncpy_s(m_sTranstParam.LaneConfigInfo.StationID, _countof(m_sTranstParam.LaneConfigInfo.StationID), strSTTemp.GetBuffer(0), _countof(m_sTranstParam.LaneConfigInfo.StationID) - 1); strSTTemp.ReleaseBuffer();
        }
        else
        {
            UINT nStationID = _ttoi( strSTTemp );
            ::_stprintf(m_sTranstParam.LaneConfigInfo.StationID, "%u", nStationID );
        }

        strncpy(m_sTranstParam.LaneConfigInfo.str_LaneID , szLoaclLaneID, 128);
        strncpy(m_sTranstParam.LaneConfigInfo.str_StationID , strSTTemp, 128);
        strncpy(m_sTranstParam.LaneConfigInfo.str_RoadID , szTempRaod, 128);
        strncpy(m_sTranstParam.LaneConfigInfo.str_AreaID , szTempArea, 128);

		LogToDebugFile("Before Step Into Area Code");
        //���⴦���������
        switch (m_sTranstParam.ProvinceID)
        {
        case 44:
        {
            CString strSTID;
            switch (m_sTranstParam.LaneConfigInfo.RoadID)
            {
            //�㶫����ĳ���վ�����⴦��
            case 30:
            case 31:
            default:/*modify 2017-11 zhuyabing �㶫ʡ��ʹ��ƴ�Ӵ���*/
            {
                strSTID.Format(_T("%04d%03d%02d"), m_sTranstParam.LaneConfigInfo.AreaID, m_sTranstParam.LaneConfigInfo.RoadID, _ttoi(m_sTranstParam.LaneConfigInfo.StationID));
                _tcscpy_s(m_sTranstParam.LaneConfigInfo.StationID, _countof(m_sTranstParam.LaneConfigInfo.StationID), strSTID.GetBuffer(0)); strSTID.ReleaseBuffer();
                //m_sTranstParam.LaneConfigInfo.StationID = _ttoi(strSTID);

                CString Area_zero_str = m_sTranstParam.LaneConfigInfo.str_AreaID;
                if (Area_zero_str.GetLength() < 4)
                {
                    Area_zero_str = CString('0', 4 - Area_zero_str.GetLength()) + Area_zero_str;
                }
                CString Road_zero_str = m_sTranstParam.LaneConfigInfo.str_RoadID;
                if (Road_zero_str.GetLength() < 3)
                {
                    Road_zero_str = CString('0', 3 - Road_zero_str.GetLength()) + Road_zero_str;
                }
                CString Station_zero_str = m_sTranstParam.LaneConfigInfo.str_StationID;
                if (Station_zero_str.GetLength() < 2)
                {
                    Station_zero_str = CString('0', 2 - Station_zero_str.GetLength()) + Station_zero_str;
                }
                CString long_str;
                long_str.Format(_T("%s%s%s"), Area_zero_str, Road_zero_str, Station_zero_str);
                strncpy(m_sTranstParam.LaneConfigInfo.str_StationID, long_str.GetBuffer(), 128);
            }
            break;
            }
        }
        break;
        }
        if (!CheckIsAlNum(m_sTranstParam.LaneConfigInfo.StationID))
            ;// throw - 4;

        CString strSTName = strTemp.Right(strTemp.GetLength() - strTemp.Find(_T("#")) - 1);
        _tcsncpy_s(m_sTranstParam.LaneConfigInfo.StationTab, _countof(m_sTranstParam.LaneConfigInfo.StationTab), strSTName.GetBuffer(0), _countof(m_sTranstParam.LaneConfigInfo.StationTab) - 1); strSTName.ReleaseBuffer();

        _tcsncpy_s(m_sTranstParam.PathCommDir, _countof(m_sTranstParam.PathCommDir), m_strWorkDir.GetBuffer(0), _countof(m_sTranstParam.PathCommDir) - 1); m_strWorkDir.ReleaseBuffer();

        if ( m_bIsInitEnvironment )
            throw - 11;


		LogToDebugFile("Before Step Into �ڲ���������");
        RecordLog(FmtStr(_T("[��������Ӧ],�����ʼ���ӿ�(IF_InitEnvironment)���ڲ���������:%s(ʡ�ݱ���:%d,�������:%d,·�α���:%d,վ�����:%s)"), strSTName, m_sTranstParam.ProvinceID, m_sTranstParam.LaneConfigInfo.AreaID, m_sTranstParam.LaneConfigInfo.RoadID, m_sTranstParam.LaneConfigInfo.StationID));

        //GenerateRawInitPara(m_sTranstParam);

        //���кϷ���У��
        m_bAuthorizedLegal = TRUE;
        //����ҵ������߳�
        CString strConfigFilePath;

        m_pThreadManage = (CThreadManage *)::AfxBeginThread(RUNTIME_CLASS(CThreadManage), THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
        if (NULL == m_pThreadManage)
            throw - 12;

	
		LogToDebugFile("Before Step Into CreateTasks");
        GetGlobalApp()->CreateTasks();
		std::string topic_info = std::string(szLoaclStation) + std::string(szLoaclLaneID);
        GetGlobalApp()->DataForwardRegister(topic_info);
        strConfigFilePath.Format(_T("%s\\%s"), m_strWorkDir, FILE_NAME_APP_CONFIG);
        m_pThreadManage->SetLocalConfigFilePath(strConfigFilePath);
        m_pThreadManage->SetParam(&m_sTranstParam, NULL);
		m_bEnableMediaGateWay = (DWORD)::GetPrivateProfileInt(_T("MediaGateWay"), _T("Enable"), 0, strConfigFilePath);
        //�����߳����»���
        m_pThreadManage->ResumeThread();
		LogToDebugFile("Before Step Into AgencyInitDll");

        //�汾1060���� ���д��۶�̬����ش���
        AgencyInitDll(szAreaInfo, szLoaclStation, szLoaclLaneID, szServerInfo, iProvinceID);

		m_iCurrentWorkMode = (DWORD)::GetPrivateProfileInt(_T("WorkMode"), _T("Default"), 0, strConfigFilePath);
		if(m_iCurrentWorkMode == 1)
		{
			RecordLog(FmtStr(_T("[��������Ӧ],��ǰ����ģʽΪMTC���Զ�����IF_EventInitEnvironment3�ӿڳ�ʼ��ETC���黷��")));
			int times = 6;
			while(times > 0)
			{
				Sleep(500);
				if(m_pThreadManage->m_pThreadDevice != NULL && m_pThreadManage->m_pThreadDevice->GetDllInitState() == TRUE)
					break;
				times--;
			}
			if(m_pThreadManage->m_pThreadDevice->GetDllInitState() == TRUE)
			{
				char ETCAreaInfo[512] = {0};
				_snprintf_s(ETCAreaInfo, 511, "%s#��������#%s#·������", m_sTranstParam.LaneConfigInfo.str_AreaID, m_sTranstParam.LaneConfigInfo.str_RoadID);

				void* pIndPtr = (void *)malloc(2048);
				m_bIsInitEnvironment = IF_EventInitEnvironment3(nThreadID, hWnd, nNotifyMsgID, ETCAreaInfo, szLoaclStation, szLoaclLaneID, szServerInfo, iProvinceID, pIndPtr, EtcCallBackFun);
			}
		}
		else
		{
			RecordLog(FmtStr(_T("[��������Ӧ],��ǰ����ģʽΪETC���飬�ɳ����������IF_EventInitEnvironment3�ӿڳ�ʼ��ETC���黷��")));
		    //��ǳ�ʼ��״̬
			m_bIsInitEnvironment = TRUE;
		}

        //���֪ͨ��ʽ
        m_iNotifyMode = 1;

        //���ؽ��
        bReturn = m_bIsInitEnvironment ? true : false;

        GetGlobalApp()->RecordLog(bReturn ? "init 1" : "init 0");
    }
    catch (int& iErrorCode)
    {
        switch (iErrorCode)
        {
        case -1:
            strLog.Format(_T("[��������Ӧ],�����ʼ���ӿ�(IF_InitEnvironment),�������������󣬴����߳�ID�봰������Ϊ0"));
            break;
        case -2:
            strLog.Format(_T("[��������Ӧ],�����ʼ���ӿ�(IF_InitEnvironment),�������������󣬴���֪ͨ��������ϢIDΪ0"));
            break;
        case -3:
            strLog.Format(_T("[��������Ӧ],�����ʼ���ӿ�(IF_InitEnvironment),�������������󣬴���������Ϣ�Ƿ�"));
            break;
        case -4:
            strLog.Format(_T("[��������Ӧ],�����ʼ���ӿ�(IF_InitEnvironment),�������������󣬴���վ�������Ϣ�Ƿ�"));
            break;
        case -5:
            strLog.Format(_T("[��������Ӧ],�����ʼ���ӿ�(IF_InitEnvironment),�������������󣬴��복��������Ϣ�Ƿ�"));
            break;
        case -6:
            strLog.Format(_T("[��������Ӧ],�����ʼ���ӿ�(IF_InitEnvironment),�������������󣬴����������Ϣ�Ƿ�"));
            break;
        case -7:
            strLog.Format(_T("[��������Ӧ],�����ʼ���ӿ�(IF_InitEnvironment),�������������󣬴���ʡ�ݱ���Ϊ0"));
            break;
        case -8:
            strLog.Format(_T("[��������Ӧ],�����ʼ���ӿ�(IF_InitEnvironment),��������������,����������߳�ID�ʹ���������ͬʱΪ��"));
            break;
        case -9:
            strLog.Format(_T("[��������Ӧ],�����ʼ���ӿ�(IF_InitEnvironment),��������������,�������������ʽ����ȷ"));
            break;
        case -10:
            strLog.Format(_T("[��������Ӧ],�����ʼ���ӿ�(IF_InitEnvironment),��������������,����վ����Ϣ������ʽ����ȷ"));
            break;
        case -11:
            strLog.Format(_T("[��������Ӧ],�����ʼ���ӿ�(IF_InitEnvironment),����������γ�ʼ��"));
            break;
        case -12:
            strLog.Format(_T("[��������Ӧ],�����ʼ���ӿ�(IF_InitEnvironment),�ڲ���ʼ������,�޷�����ҵ������߳�"));
            break;
        }
        //��¼��־
        if (m_strLastErrorDesc != strLog)
        {
            m_strLastErrorDesc = strLog;
            RecordLog(m_strLastErrorDesc, LOG_LEVEL_ERROR);
        }
    }

    RecordLog(FmtStr(_T("[��������Ӧ],�����ʼ���ӿ�(IF_InitEnvironment)���ý���")));
    return bReturn;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// �������ƣ�IF_InitEnvironment2
//
// ������������ʼ���������
//
// ���������IN const UINT& nThreadID              -- ������ָ�����߳�ID
//         ��IN const HWND& hWnd                   -- ������ָ���Ĵ�����
//         ��IN const char* szAreaInfo             -- ������Ϣ(�������#·�κ�)
//         ��IN const char* szLoaclStation         -- վ����Ϣ(վ��#վ��)
//         ��IN const char* szLoaclLaneID          -- ��������
//         ��IN const char* szServerInfo           -- ��������Ϣ
//         ��IN const int& iProvinceID             -- ʡ�ݱ���
//         ��void (*fun)(int option, int result)   -- �ۿ�/�������֪ͨ���ص�����
// ���������bool
// ��д��Ա��ROCY
// ��дʱ�䣺2016��12��14��
// �޸���Ա��
// �޸�ʱ�䣺
// �����汾��1.0.0.1
// ��ע˵�����ӿ�1-1
//
////////////////////////////////////////////////////////////////////////////////////////////////
bool WINAPI CTWSDNetPay_DllApp::IF_InitEnvironment2 (IN const UINT& nThreadID, IN const HWND& hWnd, IN const char* szAreaInfo, IN const char* szLoaclStation, IN const char* szLoaclLaneID, IN const char* szServerInfo, IN const int& iProvinceID, void (WINAPI *fun)(int option, int result))
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    GetGlobalApp()->m_EtcPayCenter.Init();
    bool bReturn = false;
    CString strLog;

    RecordLog(FmtStr(_T("[��������Ӧ],�����ʼ���ӿ�(IF_InitEnvironment2)���ÿ�ʼ")));
    try
    {
        /*******************���������Ƿ�����*******************/
        if (  (0 == nThreadID) && (NULL == hWnd))
            throw - 1;
        if (NULL == fun)
            throw - 2;
        //�ص�����ָ�븳ֵ������֪ͨ������
        m_fnCallbackNotifyFun = fun;

        if ( (NULL == szAreaInfo) || (0 == _tcslen(szAreaInfo)))
            throw - 3;
        if ( (NULL == szLoaclStation) || (0 == _tcslen(szLoaclStation)))
            throw - 4;
        if ( (NULL == szLoaclLaneID) || (0 == _tcslen(szLoaclLaneID)))
            throw - 5;
        if ( (NULL == szServerInfo) || (0 == _tcslen(szServerInfo)))
            throw - 6;
        if ( 0 == iProvinceID)
            throw - 7;
        if ( (0 == nThreadID) && (!::IsWindow(hWnd)))
            throw - 8;
        //���������Ϣ
        TCHAR   szTemp[256] = {0};
        CString strTemp;
        //������ʼ��������ֵ
        m_sTranstParam.ProvinceID            = iProvinceID;
        m_sTranstParam.ActiveNotifyMsgID     = 0;
        m_sTranstParam.CallerThreadID        = nThreadID;
        m_sTranstParam.CallerWnd             = hWnd;

        //��������
        if ( CheckStringLetter( (char *)szLoaclLaneID ) )
        {
            _tcsncpy_s(m_sTranstParam.LaneConfigInfo.LaneID, _countof(m_sTranstParam.LaneConfigInfo.LaneID), szLoaclLaneID, _countof(m_sTranstParam.LaneConfigInfo.LaneID) - 1);
        }
        else
        {
            UINT nLaneID = _ttoi( szLoaclLaneID ) % 100000000;
            ::_stprintf(m_sTranstParam.LaneConfigInfo.LaneID, "%u", nLaneID );
        }
        if (!CheckIsAlNum(m_sTranstParam.LaneConfigInfo.LaneID))
            ;//throw - 5;

        //·�Ρ��������
        strTemp.Format(_T("%s"), szAreaInfo);
        if (-1 == strTemp.Find(_T("#")))
            throw - 9;
        char szTempArea[20] = {0};
        _tcsncpy_s(szTempArea, _countof(szTempArea), strTemp.Left(strTemp.Find(_T("#"))).GetBuffer(0), _countof(szTempArea) - 1);
        strTemp.Left(strTemp.Find(_T("#"))).ReleaseBuffer();
        if (!CheckIsAlNum(szTempArea, 0x01))
            ;//throw - 3;
        m_sTranstParam.LaneConfigInfo.AreaID = _ttoi(szTempArea);

        char szTempRaod[20] = {0};
        CString strRoadID = strTemp.Right(strTemp.GetLength() - strTemp.Find(_T("#")) - 1);
        _tcsncpy_s(szTempRaod, _countof(szTempRaod), strRoadID.GetBuffer(0), _countof(szTempRaod) - 1);
        strRoadID.ReleaseBuffer();
        if (!CheckIsAlNum(szTempRaod, 0x01))
            ;//throw - 3;
        m_sTranstParam.LaneConfigInfo.RoadID = _ttoi(strRoadID);

        //վ������վ��
        strTemp.Format(_T("%s"), szLoaclStation);
        if (-1 == strTemp.Find(_T("#")))
            throw - 10;
        CString strSTTemp = strTemp.Left(strTemp.Find(_T("#")));

        BOOL bCheck = CheckStringLetter( strSTTemp.GetBuffer(0), 1 );
        strSTTemp.ReleaseBuffer();
        if ( bCheck )
        {
            _tcsncpy_s(m_sTranstParam.LaneConfigInfo.StationID, _countof(m_sTranstParam.LaneConfigInfo.StationID), strSTTemp.GetBuffer(0), _countof(m_sTranstParam.LaneConfigInfo.StationID) - 1); strSTTemp.ReleaseBuffer();
        }
        else
        {
            UINT nStationID = _ttoi( strSTTemp );
            ::_stprintf(m_sTranstParam.LaneConfigInfo.StationID, "%u", nStationID );
        }
        strncpy(m_sTranstParam.LaneConfigInfo.str_LaneID , szLoaclLaneID, 128);
        strncpy(m_sTranstParam.LaneConfigInfo.str_StationID , strSTTemp, 128);
        strncpy(m_sTranstParam.LaneConfigInfo.str_RoadID , szTempRaod, 128);
        strncpy(m_sTranstParam.LaneConfigInfo.str_AreaID , szTempArea, 128);
        //���⴦���������
        switch (m_sTranstParam.ProvinceID)
        {
        case 44:
        {
            CString strSTID;
            switch (m_sTranstParam.LaneConfigInfo.RoadID)
            {
            //�㶫����ĳ���վ�����⴦��
            case 30:
            case 31:
            default:
            {
                strSTID.Format(_T("%04d%03d%02d"), m_sTranstParam.LaneConfigInfo.AreaID, m_sTranstParam.LaneConfigInfo.RoadID, _ttoi(m_sTranstParam.LaneConfigInfo.StationID));
                _tcsncpy_s(m_sTranstParam.LaneConfigInfo.StationID, _countof(m_sTranstParam.LaneConfigInfo.StationID), strSTID.GetBuffer(0), _countof(m_sTranstParam.LaneConfigInfo.StationID) - 1); strSTID.ReleaseBuffer();

                CString Area_zero_str = m_sTranstParam.LaneConfigInfo.str_AreaID;
                if (Area_zero_str.GetLength() < 4)
                {
                    Area_zero_str = CString('0', 4 - Area_zero_str.GetLength()) + Area_zero_str;
                }
                CString Road_zero_str = m_sTranstParam.LaneConfigInfo.str_RoadID;
                if (Road_zero_str.GetLength() < 3)
                {
                    Road_zero_str = CString('0', 3 - Road_zero_str.GetLength()) + Road_zero_str;
                }
                CString Station_zero_str = m_sTranstParam.LaneConfigInfo.str_StationID;
                if (Station_zero_str.GetLength() < 2)
                {
                    Station_zero_str = CString('0', 2 - Station_zero_str.GetLength()) + Station_zero_str;
                }
                CString long_str;
                long_str.Format(_T("%s%s%s"), Area_zero_str, Road_zero_str, Station_zero_str);
                strncpy(m_sTranstParam.LaneConfigInfo.str_StationID, long_str.GetBuffer(), 128);

            }
            break;
            }
        }
        break;
        }
        if (!CheckIsAlNum(m_sTranstParam.LaneConfigInfo.StationID))
            throw - 4;

        CString strSTName = strTemp.Right(strTemp.GetLength() - strTemp.Find(_T("#")) - 1);
        _tcsncpy_s(m_sTranstParam.LaneConfigInfo.StationTab, _countof(m_sTranstParam.LaneConfigInfo.StationTab), strSTName.GetBuffer(0), _countof(m_sTranstParam.LaneConfigInfo.StationTab) - 1); strSTName.ReleaseBuffer();

        _tcsncpy_s(m_sTranstParam.PathCommDir, _countof(m_sTranstParam.PathCommDir), m_strWorkDir.GetBuffer(0), _countof(m_sTranstParam.PathCommDir) - 1); m_strWorkDir.ReleaseBuffer();

        if ( m_bIsInitEnvironment )
            throw - 11;

        RecordLog(FmtStr(_T("[��������Ӧ],�����ʼ���ӿ�(IF_InitEnvironment2)���ڲ���������:%s(ʡ�ݱ���:%d,�������:%d,·�α���:%d,վ�����:%s)"), strSTName, m_sTranstParam.ProvinceID, m_sTranstParam.LaneConfigInfo.AreaID, m_sTranstParam.LaneConfigInfo.RoadID, m_sTranstParam.LaneConfigInfo.StationID));



        GenerateRawInitPara(m_sTranstParam);
        //���кϷ���У��
        m_bAuthorizedLegal = TRUE;
        //����ҵ������߳�
        CString strConfigFilePath;
        m_pThreadManage = (CThreadManage *)::AfxBeginThread(RUNTIME_CLASS(CThreadManage), THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
        if (NULL == m_pThreadManage)
            throw - 12;

        GetGlobalApp()->CreateTasks();
        std::string topic_info = std::string(szLoaclStation) + std::string(szLoaclLaneID);
        GetGlobalApp()->DataForwardRegister(topic_info);
        strConfigFilePath.Format(_T("%s\\%s"), m_strWorkDir, FILE_NAME_APP_CONFIG);
        m_pThreadManage->SetLocalConfigFilePath(strConfigFilePath);
        m_pThreadManage->SetParam(&m_sTranstParam, NULL);
        //�����߳����»���
        m_pThreadManage->ResumeThread();

        //�汾1060���� ���д��۶�̬����ش���
        AgencyInitDll(szAreaInfo, szLoaclStation, szLoaclLaneID, szServerInfo, iProvinceID);
        //��ǳ�ʼ��״̬
        m_bIsInitEnvironment = TRUE;

        //���֪ͨ��ʽ
        m_iNotifyMode = 2;

        //���ؽ��
        bReturn = m_bIsInitEnvironment ? true : false;
    }
    catch (int& iErrorCode)
    {
        switch (iErrorCode)
        {
        case -1:
            strLog.Format(_T("[��������Ӧ],�����ʼ���ӿ�(IF_InitEnvironment2),�������������󣬴����߳�ID�봰������Ϊ0"));
            break;
        case -2:
            strLog.Format(_T("[��������Ӧ],�����ʼ���ӿ�(IF_InitEnvironment2),�������������󣬴���ص�����ָ�����"));
            break;
        case -3:
            strLog.Format(_T("[��������Ӧ],�����ʼ���ӿ�(IF_InitEnvironment2),�������������󣬴���������Ϣ�Ƿ�"));
            break;
        case -4:
            strLog.Format(_T("[��������Ӧ],�����ʼ���ӿ�(IF_InitEnvironment2),�������������󣬴���վ�������Ϣ�Ƿ�"));
            break;
        case -5:
            strLog.Format(_T("[��������Ӧ],�����ʼ���ӿ�(IF_InitEnvironment2),�������������󣬴��복��������Ϣ�Ƿ�"));
            break;
        case -6:
            strLog.Format(_T("[��������Ӧ],�����ʼ���ӿ�(IF_InitEnvironment2),�������������󣬴����������Ϣ�Ƿ�"));
            break;
        case -7:
            strLog.Format(_T("[��������Ӧ],�����ʼ���ӿ�(IF_InitEnvironment2),�������������󣬴���ʡ�ݱ���Ϊ0"));
            break;
        case -8:
            strLog.Format(_T("[��������Ӧ],�����ʼ���ӿ�(IF_InitEnvironment2),��������������,����������߳�ID�ʹ���������ͬʱΪ��"));
            break;
        case -9:
            strLog.Format(_T("[��������Ӧ],�����ʼ���ӿ�(IF_InitEnvironment2),��������������,�������������ʽ����ȷ"));
            break;
        case -10:
            strLog.Format(_T("[��������Ӧ],�����ʼ���ӿ�(IF_InitEnvironment2),��������������,����վ����Ϣ������ʽ����ȷ"));
            break;
        case -11:
            strLog.Format(_T("[��������Ӧ],�����ʼ���ӿ�(IF_InitEnvironment2),����������γ�ʼ��"));
            break;
        case -12:
            strLog.Format(_T("[��������Ӧ],�����ʼ���ӿ�(IF_InitEnvironment2),�ڲ���ʼ������,�޷�����ҵ������߳�"));
            break;
        }
        //��¼��־
        if (m_strLastErrorDesc != strLog)
        {
            m_strLastErrorDesc = strLog;
            RecordLog(m_strLastErrorDesc, LOG_LEVEL_ERROR);
        }
    }

    RecordLog(FmtStr(_T("[��������Ӧ],�����ʼ���ӿ�(IF_InitEnvironment2)���ý���")));

    return bReturn;
}


////////////////////////////////////////////////////////////////////////////////////////////////
// �������ƣ�IF_InitEnvironment3
//
// ������������ʼ���������
//
// ���������IN const UINT& nThreadID              -- ������ָ�����߳�ID
//         ��IN const HWND& hWnd                   -- ������ָ���Ĵ�����
//         ��IN const char* szAreaInfo             -- ������Ϣ(�������#·�κ�)
//         ��IN const char* szLoaclStation         -- վ����Ϣ(վ��#վ��)
//         ��IN const char* szLoaclLaneID          -- ��������
//         ��IN const char* szServerInfo           -- ��������Ϣ
//         ��IN const int& iProvinceID             -- ʡ�ݱ���
//         ��IN OUT void* pIndPtr                  -- ���Ի�ָ��
//         ��void (*fun)(int option, int result,void* pIndPtr) -- �ۿ�/�������֪ͨ���ص�����
// ���������bool
// ��д��Ա��ROCY
// ��дʱ�䣺2017��03��16��
// �޸���Ա��
// �޸�ʱ�䣺
// �����汾��1.0.0.1
// ��ע˵�����ӿ�1-2��ר��Ϊ�������Ƶĳ�ʼ����������ʼ��������ʱ�򣬱���һ�����Ի�ָ�룬�ڻص���ʱ�򴫳�ȥ
//
////////////////////////////////////////////////////////////////////////////////////////////////
bool WINAPI CTWSDNetPay_DllApp::IF_InitEnvironment3 (IN const UINT& nThreadID, IN const HWND& hWnd, IN const char* szAreaInfo, IN const char* szLoaclStation, IN const char* szLoaclLaneID, IN const char* szServerInfo, IN const int& iProvinceID, IN OUT void* pIndPtr, void (WINAPI *fun)(int option, int result, void* pIndPtr))
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    GetGlobalApp()->m_EtcPayCenter.Init();
    bool bReturn = false;
    CString strLog;

    RecordLog(FmtStr(_T("[��������Ӧ],�����ʼ���ӿ�(IF_InitEnvironment3)���ÿ�ʼ,���Ի�ָ�����:%04X��վ��Ϣ:%s"), pIndPtr, szLoaclStation));
    try
    {
        /*******************���������Ƿ�����*******************/
        if (  (0 == nThreadID) && (NULL == hWnd))
            throw - 1;
        if (NULL == fun)
            throw - 2;
        //�ص�����ָ�븳ֵ������֪ͨ������
        m_fnCallbackNotifyFun45 = fun;

        if ( (NULL == szAreaInfo) || (0 == _tcslen(szAreaInfo)))
            throw - 3;
        if ( (NULL == szLoaclStation) || (0 == _tcslen(szLoaclStation)))
            throw - 4;
        if ( (NULL == szLoaclLaneID) || (0 == _tcslen(szLoaclLaneID)))
            throw - 5;
        if ( (NULL == szServerInfo) || (0 == _tcslen(szServerInfo)))
            throw - 6;
        if ( 0 == iProvinceID)
            throw - 7;
        if ( (0 == nThreadID) && (!::IsWindow(hWnd)))
            throw - 8;
        //���������Ϣ
        TCHAR   szTemp[256] = {0};
        CString strTemp;
        //������ʼ��������ֵ
        m_sTranstParam.ProvinceID            = iProvinceID;
        m_sTranstParam.ActiveNotifyMsgID     = 0;
        m_sTranstParam.CallerThreadID        = nThreadID;
        m_sTranstParam.CallerWnd             = hWnd;

        //��������
        if ( CheckStringLetter( (char *)szLoaclLaneID ) )
        {
            _tcsncpy_s(m_sTranstParam.LaneConfigInfo.LaneID, _countof(m_sTranstParam.LaneConfigInfo.LaneID), szLoaclLaneID, _countof(m_sTranstParam.LaneConfigInfo.LaneID) - 1);
        }
        else
        {
            UINT nLaneID = _ttoi( szLoaclLaneID ) % 100000000;
            ::_stprintf(m_sTranstParam.LaneConfigInfo.LaneID, "%u", nLaneID );
        }
        if (!CheckIsAlNum(m_sTranstParam.LaneConfigInfo.LaneID))
            ;// throw - 5;

        //·�Ρ��������
        strTemp.Format(_T("%s"), szAreaInfo);
        if (-1 == strTemp.Find(_T("#")))
            throw - 9;
        char szTempArea[20] = {0};
        _tcsncpy_s(szTempArea, _countof(szTempArea), strTemp.Left(strTemp.Find(_T("#"))).GetBuffer(0), _countof(szTempArea) - 1);
        strTemp.Left(strTemp.Find(_T("#"))).ReleaseBuffer();
        if (!CheckIsAlNum(szTempArea, 0x01))
            ;// throw - 3;
        m_sTranstParam.LaneConfigInfo.AreaID = _ttoi(szTempArea);

        char szTempRaod[20] = {0};
        CString strRoadID = strTemp.Right(strTemp.GetLength() - strTemp.Find(_T("#")) - 1);
        _tcsncpy_s(szTempRaod, _countof(szTempRaod), strRoadID.GetBuffer(0), _countof(szTempRaod) - 1);
        strRoadID.ReleaseBuffer();
        if (!CheckIsAlNum(szTempRaod, 0x01))
            ;// throw - 3;
        m_sTranstParam.LaneConfigInfo.RoadID = _ttoi(strRoadID);

        //վ������վ��
        strTemp.Format(_T("%s"), szLoaclStation);
        if (-1 == strTemp.Find(_T("#")))
            throw - 10;
        CString strSTTemp = strTemp.Left(strTemp.Find(_T("#")));

        BOOL bCheck = CheckStringLetter( strSTTemp.GetBuffer(0), 1 );
        strSTTemp.ReleaseBuffer();
        if ( bCheck )
        {
            _tcsncpy_s(m_sTranstParam.LaneConfigInfo.StationID, _countof(m_sTranstParam.LaneConfigInfo.StationID), strSTTemp.GetBuffer(0), _countof(m_sTranstParam.LaneConfigInfo.StationID) - 1); strSTTemp.ReleaseBuffer();
        }
        else
        {
            UINT nStationID = _ttoi( strSTTemp );
            ::_stprintf(m_sTranstParam.LaneConfigInfo.StationID, "%u", nStationID );
        }
        strncpy(m_sTranstParam.LaneConfigInfo.str_LaneID , szLoaclLaneID, 128);
        strncpy(m_sTranstParam.LaneConfigInfo.str_StationID , strSTTemp, 128);
        strncpy(m_sTranstParam.LaneConfigInfo.str_RoadID , szTempRaod, 128);
        strncpy(m_sTranstParam.LaneConfigInfo.str_AreaID , szTempArea, 128);
        //���⴦���������
        switch (m_sTranstParam.ProvinceID)
        {
        case 44:
        {
            CString strSTID;
            switch (m_sTranstParam.LaneConfigInfo.RoadID)
            {
            //�㶫����ĳ���վ�����⴦��
            case 30:
            case 31:
            default:
            {
                strSTID.Format(_T("%04d%03d%02d"), m_sTranstParam.LaneConfigInfo.AreaID, m_sTranstParam.LaneConfigInfo.RoadID, _ttoi(m_sTranstParam.LaneConfigInfo.StationID));
                _tcsncpy_s(m_sTranstParam.LaneConfigInfo.StationID, _countof(m_sTranstParam.LaneConfigInfo.StationID), strSTID.GetBuffer(0), _countof(m_sTranstParam.LaneConfigInfo.StationID) - 1); strSTID.ReleaseBuffer();

                CString Area_zero_str = m_sTranstParam.LaneConfigInfo.str_AreaID;
                if (Area_zero_str.GetLength() < 4)
                {
                    Area_zero_str = CString('0', 4 - Area_zero_str.GetLength()) + Area_zero_str;
                }
                CString Road_zero_str = m_sTranstParam.LaneConfigInfo.str_RoadID;
                if (Road_zero_str.GetLength() < 3)
                {
                    Road_zero_str = CString('0', 3 - Road_zero_str.GetLength()) + Road_zero_str;
                }
                CString Station_zero_str = m_sTranstParam.LaneConfigInfo.str_StationID;
                if (Station_zero_str.GetLength() < 2)
                {
                    Station_zero_str = CString('0', 2 - Station_zero_str.GetLength()) + Station_zero_str;
                }
                CString long_str;
                long_str.Format(_T("%s%s%s"), Area_zero_str, Road_zero_str, Station_zero_str);
                strncpy(m_sTranstParam.LaneConfigInfo.str_StationID, long_str.GetBuffer(), 128);
            }
            break;
            }
        }
        break;
        }

        CString strSTName = strTemp.Right(strTemp.GetLength() - strTemp.Find(_T("#")) - 1);
        _tcsncpy_s(m_sTranstParam.LaneConfigInfo.StationTab, _countof(m_sTranstParam.LaneConfigInfo.StationTab), strSTName.GetBuffer(0), _countof(m_sTranstParam.LaneConfigInfo.StationTab) - 1); strSTName.ReleaseBuffer();

        _tcsncpy_s(m_sTranstParam.PathCommDir, _countof(m_sTranstParam.PathCommDir), m_strWorkDir.GetBuffer(0), _countof(m_sTranstParam.PathCommDir) - 1); m_strWorkDir.ReleaseBuffer();

        if ( m_bIsInitEnvironment )
            throw - 11;

        //���Ͳ���������һ�����Ի�ָ��
        m_pIndPtrFor45 = pIndPtr;

        //��¼��־
        RecordLog(FmtStr(_T("[��������Ӧ],�����ʼ���ӿ�(IF_InitEnvironment3)���ڲ���������:%s(ʡ�ݱ���:%d,�������:%d,·�α���:%d,վ�����:%s)"), strSTName, m_sTranstParam.ProvinceID, m_sTranstParam.LaneConfigInfo.AreaID, m_sTranstParam.LaneConfigInfo.RoadID, m_sTranstParam.LaneConfigInfo.StationID));



        GenerateRawInitPara(m_sTranstParam);
        //���кϷ���У��
        m_bAuthorizedLegal = TRUE;
        //����ҵ������߳�
        CString strConfigFilePath;
        m_pThreadManage = (CThreadManage *)::AfxBeginThread(RUNTIME_CLASS(CThreadManage), THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
        if (NULL == m_pThreadManage)
            throw - 12;

        GetGlobalApp()->CreateTasks();
        std::string topic_info = std::string(szLoaclStation) + std::string(szLoaclLaneID);
        GetGlobalApp()->DataForwardRegister(topic_info);
        strConfigFilePath.Format(_T("%s\\%s"), m_strWorkDir, FILE_NAME_APP_CONFIG);
        m_pThreadManage->SetLocalConfigFilePath(strConfigFilePath);
        m_pThreadManage->SetParam(&m_sTranstParam, NULL);
        //�����߳����»���
        m_pThreadManage->ResumeThread();

        //�汾1060���� ���д��۶�̬����ش���
        AgencyInitDll(szAreaInfo, szLoaclStation, szLoaclLaneID, szServerInfo, iProvinceID);
        //��ǳ�ʼ��״̬
        m_bIsInitEnvironment = TRUE;

        //���֪ͨ��ʽ
        m_iNotifyMode = 3;

        //���ؽ��
        bReturn = m_bIsInitEnvironment ? true : false;
    }
    catch (int& iErrorCode)
    {
        switch (iErrorCode)
        {
        case -1:
            strLog.Format(_T("[��������Ӧ],�����ʼ���ӿ�(IF_InitEnvironment3),�������������󣬴����߳�ID�봰������Ϊ0"));
            break;
        case -2:
            strLog.Format(_T("[��������Ӧ],�����ʼ���ӿ�(IF_InitEnvironment3),�������������󣬴���ص�����ָ�����"));
            break;
        case -3:
            strLog.Format(_T("[��������Ӧ],�����ʼ���ӿ�(IF_InitEnvironment3),�������������󣬴���������Ϣ�Ƿ�"));
            break;
        case -4:
            strLog.Format(_T("[��������Ӧ],�����ʼ���ӿ�(IF_InitEnvironment3),�������������󣬴���վ�������Ϣ�Ƿ�"));
            break;
        case -5:
            strLog.Format(_T("[��������Ӧ],�����ʼ���ӿ�(IF_InitEnvironment3),�������������󣬴��복��������Ϣ�Ƿ�"));
            break;
        case -6:
            strLog.Format(_T("[��������Ӧ],�����ʼ���ӿ�(IF_InitEnvironment3),�������������󣬴����������Ϣ�Ƿ�"));
            break;
        case -7:
            strLog.Format(_T("[��������Ӧ],�����ʼ���ӿ�(IF_InitEnvironment3),�������������󣬴���ʡ�ݱ���Ϊ0"));
            break;
        case -8:
            strLog.Format(_T("[��������Ӧ],�����ʼ���ӿ�(IF_InitEnvironment3),��������������,����������߳�ID�ʹ���������ͬʱΪ��"));
            break;
        case -9:
            strLog.Format(_T("[��������Ӧ],�����ʼ���ӿ�(IF_InitEnvironment3),��������������,�������������ʽ����ȷ"));
            break;
        case -10:
            strLog.Format(_T("[��������Ӧ],�����ʼ���ӿ�(IF_InitEnvironment3),��������������,����վ����Ϣ������ʽ����ȷ"));
            break;
        case -11:
            strLog.Format(_T("[��������Ӧ],�����ʼ���ӿ�(IF_InitEnvironment3),����������γ�ʼ��"));
            break;
        case -12:
            strLog.Format(_T("[��������Ӧ],�����ʼ���ӿ�(IF_InitEnvironment3),�ڲ���ʼ������,�޷�����ҵ������߳�"));
            break;
        }
        //��¼��־
        if (m_strLastErrorDesc != strLog)
        {
            m_strLastErrorDesc = strLog;
            RecordLog(m_strLastErrorDesc, LOG_LEVEL_ERROR);
        }
    }

	// ������������̬��ķ�����
	bool bStartReaderServer = StartReaderServer();

	if(bStartReaderServer)
		RecordLog(FmtStr(_T("[��������Ӧ],�����ʼ���ӿ�(StartReaderServer)������������̬��ķ������ɹ�")));
	else
		RecordLog(FmtStr(_T("[��������Ӧ],�����ʼ���ӿ�(StartReaderServer)������������̬��ķ�����ʧ��")));


    RecordLog(FmtStr(_T("[��������Ӧ],�����ʼ���ӿ�(IF_InitEnvironment3)���ý���")));

    return bReturn;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// �������ƣ�IF_Destroy
//
// ����������������Դ
//
// ���������void
// ���������bool
// ��д��Ա��ROCY
// ��дʱ�䣺2016��7��23��
// �޸���Ա��
// �޸�ʱ�䣺
// �����汾��1.0.0.1
// ��ע˵�����ӿ�2
//
////////////////////////////////////////////////////////////////////////////////////////////////
bool WINAPI CTWSDNetPay_DllApp::IF_Destroy(void)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
	GetGlobalApp()->m_EtcPayCenter.Destroy();
    GetGlobalApp()->StopListenMessage();
    bool bReturn = false;
    CString strLog;
    try
    {
        // 1052�汾������Ϊ�˷�ֹ�˳����������������ķǷ����ʣ�����ʼ�������Ϊ��
        m_bIsInitEnvironment = FALSE;

        DWORD dwSleepTime = 0;

        CString strLog;
        /**********************************************��������߳���Դ**********************************************/

        strLog.Format(_T("[��������Ӧ],�����Դ����ӿ�(IF_Destroy),���ڿ�ʼ�˳���̨�߳�..."));
        //��¼��־
        RecordLog(strLog);

        //�߳��˳����
        DWORD dwThreadExitFlag = 0x00;
        //�������ԵĴ���
        int iAllowTryCount = 10;
        //�Ѿ����Դ���
        int iHaveTryCount = 0;
        //ÿ�����Ե�ʱ����ʱ�䣨��λ�����룩
        DWORD   dwSpanTime = 100;

        //ֹͣҵ������߳�
        if (NULL != m_pThreadManage)
        {
            //���ø��߳��Ƴ����Ϊ��
            //m_pThreadManage->m_bExit = TRUE;
            m_pThreadManage->ThreadQuit();
            //��Ĭ200����
            ::Sleep(20);

            //ÿ�εȴ�1000���룬���Դ���������10��
            while (OP_SUCCESS != fnCloseThread(m_pThreadManage->m_hThread, 1000))
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
            //MessageBox(0,"m_pThreadManage","",0);
            m_pThreadManage = NULL;


            strLog.Format(_T("[��������Ӧ],�����Դ����ӿ�(IF_Destroy),%s"), FmtStr(_T("��ҵ������̡߳��Ѿ�ֹͣ(���Դ��� %d)..."), iHaveTryCount));
            //��¼��־
            RecordLog(strLog);
        }

        //�ͷŴ��۶�̬��
        m_AgencyPay.Destroy();
        //�ͷ�һЩָ����Դ
        m_fnCallbackNotifyFun   = NULL;
        m_fnCallbackNotifyFun45 = NULL;
        m_pIndPtrFor45          = NULL;
        memset(&m_sTranstParam, sizeof(tagTranstParam), 0);
        memset(&m_sCurrentDebitResult, sizeof(tagDebitResultItems), 0);
        //���ñ��λ
        m_bIsInitEnvironment                        = FALSE;
        m_bAuthorizedLegal                          = FALSE;
        m_bNetErrorHttpServerInTollNetNotExist      = FALSE;
        m_bNetErrorHttpServerTrawePlatformNotExist  = FALSE;
        //��Ĭ500����
        ::Sleep(m_iExitWaitForTime);

		IF_EventDestroy();

        //��¼��־
        strLog.Format(_T("[��������Ӧ],��������˳�..."));
        RecordLog(strLog, 0, TRUE);
        //��ǳɹ�
        bReturn = true;
    }
    catch (int& iErrorCode)
    {
    }

    return bReturn;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// �������ƣ�IF_GetLastErrorDesc
//
// ������������ȡ��һ�δ�����Ϣ
//
// ���������void
// ���������char*
// ��д��Ա��ROCY
// ��дʱ�䣺2016��7��23��
// �޸���Ա��
// �޸�ʱ�䣺
// �����汾��1.0.0.1
// ��ע˵�����ӿ�3
//
////////////////////////////////////////////////////////////////////////////////////////////////
const char* WINAPI CTWSDNetPay_DllApp::IF_GetLastErrorDesc(void)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
	RecordLog(FmtStr(_T("[ETC����ҵ��],��ȡ�ϴδ��������ӿ�(IF_GetLastErrorDesc)���ÿ�ʼ")));

    return m_strLastErrorDesc.GetBuffer(0);
}

////////////////////////////////////////////////////////////////////////////////////////////////
// �������ƣ�CTWSDNetPay_DllApp::IF_StartGetAccountInfo
//
// ����������������ȡ�û���Ϣ����
//
// ���������IN const int& iOvertime
// ���������bool
// ��д��Ա��ROCY
// ��дʱ�䣺2016��8��2��
// �޸���Ա��
// �޸�ʱ�䣺
// �����汾��1.0.0.1
// ��ע˵�����ӿ�4
//
////////////////////////////////////////////////////////////////////////////////////////////////
bool WINAPI CTWSDNetPay_DllApp::IF_StartGetAccountInfo(IN const int& iOvertime)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    bool bReturn = false;
    CString strLog;
    RecordLog(FmtStr(_T("[��������Ӧ],������ȡ�û���Ϣ�����ӿ�(IF_StartGetAccountInfo)���ÿ�ʼ")));
    try
    {
        //�ж�����Ƿ��Ѿ�����ʼ��
        if (!m_bIsInitEnvironment)
            throw - 1;
        //�ж�����Ƿ�ɹ���Ȩ
        if (!m_bAuthorizedLegal)
            throw - 2;
        //�ж�ҵ�����߳��Ƿ��쳣
        if (NULL == m_pThreadManage)
            throw - 3;
        //��ҵ������̷߳�����Ϣ��Ҫ�����Ӳ�����л�ȡ�û���Ϣ����
        m_pThreadManage->PostThreadMessage(WM_THREAD_DEVICE_START_GET_DATA, (WPARAM)iOvertime, (LPARAM)0x01);
        //��ǳɹ�
        bReturn = true;
    }
    catch (int& iErrorCode)
    {
        BYTE byFunCode      = 0x03;
        BYTE byExecuteState = RCC_INNER_ERROR;
        switch (iErrorCode)
        {
        case -1:
            strLog.Format(_T("[��������Ӧ],������ȡ�û���Ϣ����(IF_StartGetAccountInfo)����,���δ�ɹ���ʼ��"));
            byExecuteState  = RCC_COM_NOT_INIT;
            break;
        case -2:
            strLog.Format(_T("[��������Ӧ],������ȡ�û���Ϣ����(IF_StartGetAccountInfo)����,���δ����Ȩ"));
            byExecuteState  = RCC_COM_ILLEGAL;
            break;
        case -3:
            strLog.Format(_T("[��������Ӧ],������ȡ�û���Ϣ����(IF_StartGetAccountInfo)����,����ڲ���Դ�쳣"));
            break;
        }
        //�����������
        if (strLog != m_strLastErrorDesc)
        {
            m_strLastErrorDesc = strLog;
            RecordLog(m_strLastErrorDesc, LOG_LEVEL_ERROR);
        }
        //���������߽��
        if (0 != m_sTranstParam.CallerThreadID)
        {
            ::PostThreadMessage(m_sTranstParam.CallerThreadID, m_sTranstParam.ActiveNotifyMsgID, (WPARAM)MAKELONG(MAKEWORD(byFunCode, byExecuteState), MAKEWORD(0, 0)), (LPARAM)0);
        }

        if (::IsWindow(m_sTranstParam.CallerWnd))
        {
            ::PostMessage(m_sTranstParam.CallerWnd, m_sTranstParam.ActiveNotifyMsgID, (WPARAM)MAKELONG(MAKEWORD(byFunCode, byExecuteState), MAKEWORD(0, 0)), (LPARAM)0);
        }
        //������־
        RecordLog(FmtStr(_T("[��������Ӧ],��ȡ�û���Ϣ����Ԥ����ʧ�ܣ������֪ͨ������(���ܴ���:%d,ִ��״��:%d)"), byFunCode, byExecuteState));
    }
    RecordLog(FmtStr(_T("[��������Ӧ],������ȡ�û���Ϣ�����ӿ�(IF_StartGetAccountInfo)���ý���")));

    return bReturn;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// �������ƣ�CTWSDNetPay_DllApp::IF_StopGetAccountInfo
//
// ����������ֹͣ��ȡ�û���Ϣ����
//
// ���������void
// ���������bool
// ��д��Ա��ROCY
// ��дʱ�䣺2016��8��2��
// �޸���Ա��
// �޸�ʱ�䣺
// �����汾��1.0.0.1
// ��ע˵�����ӿ�5
//
////////////////////////////////////////////////////////////////////////////////////////////////
bool WINAPI CTWSDNetPay_DllApp::IF_StopGetAccountInfo(void)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    bool bReturn = false;
    CString strLog;
    RecordLog(FmtStr(_T("[��������Ӧ],ֹͣ��ȡ�û���Ϣ�����ӿ�(IF_StopGetAccountInfo)���ÿ�ʼ")));
    try
    {
        //�ж�����Ƿ��Ѿ�����ʼ��
        if (!m_bIsInitEnvironment)
            throw - 1;
        //�ж�����Ƿ�ɹ���Ȩ
        if (!m_bAuthorizedLegal)
            throw - 2;
        //�ж�ҵ�����߳��Ƿ��쳣
        if (NULL == m_pThreadManage)
            throw - 3;
        //��ҵ������̷߳�����Ϣ��Ҫ�����Ӳ�����л�ȡ�û���Ϣ����
        m_pThreadManage->PostThreadMessage(WM_THREAD_DEVICE_STOP_GET_DATA, (WPARAM)0, (LPARAM)0);
        //��ǳɹ�
        bReturn = true;
    }
    catch (int& iErrorCode)
    {
        switch (iErrorCode)
        {
        case -1:
            strLog.Format(_T("[��������Ӧ],ֹͣ��ȡ�û���Ϣ����(IF_StopGetAccountInfo)����,���δ�ɹ���ʼ��"));
            break;
        case -2:
            strLog.Format(_T("[��������Ӧ],ֹͣ��ȡ�û���Ϣ����(IF_StopGetAccountInfo)����,���δ����Ȩ"));
            break;
        case -3:
            strLog.Format(_T("[��������Ӧ],ֹͣ��ȡ�û���Ϣ����(IF_StopGetAccountInfo)����,����ڲ���Դ�쳣"));
            break;
        case -4:
            strLog.Format(_T("[��������Ӧ],ֹͣ��ȡ�û���Ϣ����(IF_StopGetAccountInfo)����,Ӳ��δ���ڹ���״̬"));
            break;
        }
        //�����������
        if (strLog != m_strLastErrorDesc)
        {
            m_strLastErrorDesc = strLog;
            RecordLog(m_strLastErrorDesc, LOG_LEVEL_ERROR);
        }

    }
    RecordLog(FmtStr(_T("[��������Ӧ],ֹͣ��ȡ�û���Ϣ�����ӿ�(IF_StopGetAccountInfo)���ý���")));
    return bReturn;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// �������ƣ�CTWSDNetPay_DllApp::IF_GetAccountInfo
//
// ������������ȡ�û���Ϣ
//
// ���������IN OUT char* szParamContext
//         ��IN OUT int& iParamSize
//         ��IN const int& iFormatType
// ���������bool
// ��д��Ա��ROCY
// ��дʱ�䣺2016��8��2��
// �޸���Ա��
// �޸�ʱ�䣺
// �����汾��1.0.0.1
// ��ע˵�����ӿ�6
//
////////////////////////////////////////////////////////////////////////////////////////////////
bool WINAPI CTWSDNetPay_DllApp::IF_GetAccountInfo(IN OUT char* szParamContext, IN OUT int& iParamSize, IN const int& iFormatType)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    bool bReturn = false;
    CString strLog;
    CString strDataString;
    RecordLog(FmtStr(_T("[��������Ӧ],��ȡ�û���Ϣ�ӿ�(IF_GetAccountInfo)���ÿ�ʼ,����1:%04X,����2:%d,����3:%d"), szParamContext, iParamSize, iFormatType));
    try
    {
        //�ж�����Ƿ��Ѿ�����ʼ��
        if (!m_bIsInitEnvironment)
            throw - 1;
        //�ж�����Ƿ�ɹ���Ȩ
        if (!m_bAuthorizedLegal)
            throw - 2;
        //�ж�ҵ�����߳��Ƿ��쳣
        if (NULL == m_pThreadManage)
            throw - 3;
        //�жϲ����Ƿ��쳣
        if ((NULL == szParamContext) || (iParamSize < 1024))
            throw - 4;
        if ( (iFormatType > 2) || (iFormatType < 1))
            throw - 5;
        //�ж������Ƿ��Ѿ���ȡ�ɹ�
        if (!m_pThreadManage->GetAccountDataReadyState())
            throw - 6;
        //��ȡ����
        tagAccountInfo sAccountInfo = m_pThreadManage->GetLastAccountInfo();
        if (1 == iFormatType)
        {
            CDVXMLAccountContext xmlAccount;
            strDataString = xmlAccount.GetXMLString(sAccountInfo.DataContext, sAccountInfo.DataType, sAccountInfo.SrcType);
            strDataString.Replace(_T("\r"), _T(""));
            strDataString.Replace(_T("\n"), _T(""));
        }
        else
        {
            CDVJsonParamAccount jsonAccount;
            jsonAccount.DataType    = sAccountInfo.DataType;
            jsonAccount.DataSrc     = sAccountInfo.SrcType;
            jsonAccount.DataContext.Format(_T("%s"), sAccountInfo.DataContext);

            CJsonParamOperate jsonParamOperation;
            strDataString = jsonParamOperation.FormatParamData2Json(&jsonAccount);
            strDataString.TrimRight();
        }
        if (strDataString.IsEmpty())
            throw - 7;
        RecordLog(FmtStr(_T("[��������Ӧ],��ȡ�û���Ϣ�ӿ�(IF_GetAccountInfo)�������:%s"), strDataString));
        memcpy(szParamContext, strDataString.GetBuffer(0), strDataString.GetLength() >= iParamSize ? (iParamSize - 1) : strDataString.GetLength());
        strDataString.ReleaseBuffer();
        //��ǳɹ�
        bReturn = true;
        //�����û�����׼��״̬Ϊ��
        m_pThreadManage->SetAccountDataReadyState(FALSE);
    }
    catch (int& iErrorCode)
    {
        switch (iErrorCode)
        {
        case -1:
            strLog.Format(_T("[��������Ӧ],��ȡ�û���Ϣ����(IF_GetAccountInfo)����,���δ�ɹ���ʼ��"));
            break;
        case -2:
            strLog.Format(_T("[��������Ӧ],��ȡ�û���Ϣ����(IF_GetAccountInfo)����,���δ����Ȩ"));
            break;
        case -3:
            strLog.Format(_T("[��������Ӧ],��ȡ�û���Ϣ����(IF_GetAccountInfo)����,����ڲ���Դ�쳣"));
            break;
        case -4:
            strLog.Format(_T("[��������Ӧ],��ȡ�û���Ϣ����(IF_GetAccountInfo)����,�û�����δ�ܻ�ȡ�ɹ�"));
            break;
        }
        //�����������
        if (strLog != m_strLastErrorDesc)
        {
            m_strLastErrorDesc = strLog;
            RecordLog(m_strLastErrorDesc, LOG_LEVEL_ERROR);
        }
    }
    RecordLog(FmtStr(_T("[��������Ӧ],��ȡ�û���Ϣ�ӿ�(IF_GetAccountInfo)���ý���")));
    return bReturn;
}


bool WINAPI CTWSDNetPay_DllApp::IF_DebitMoney_Future(IN const char* szParamContext, IN const int& iParamSize, IN const int& iFormatType)
{
    CInterfaceTraceInfo trace_interface("IF_DebitMoney", pystring::replace(szParamContext, "\r\n", ""));
    GetGlobalApp()->m_bCurrentDebitMode = TRUE;
    GetGlobalApp()->m_bIsTrading = TRUE;
    if (!m_bIsInitEnvironment)
    {
        TW_TRACE_DIARY_TYPE("δ���г�ʼ��", TW_TRACE_TPPE_INTERFACE);
        NotifyCaller(1, RCC_COM_NOT_INIT);
        return false;
    }
    if (0 == m_pThreadManage)
    {
        TW_TRACE_DIARY_TYPE("�����߳���Ч", TW_TRACE_TPPE_INTERFACE);
        NotifyCaller(1, RCC_INNER_ERROR);
        return false;
    }
    if (szParamContext == 0 || iParamSize == 0)
    {
        TW_TRACE_DIARY_TYPE("����Ϊ��", TW_TRACE_TPPE_INTERFACE);
        NotifyCaller(1, RCC_PARAM_ERROR);
        return false;
    }
    DebitParameter dp;
    CopyExpwayInfo(GetGlobalApp()->m_sTranstParam, dp);
    if (iFormatType == 1 )
    {
        if (!ParseDebitParaFromXML( szParamContext, dp))
        {
            NotifyCaller(1, RCC_PARAM_ERROR);
            return false;
        }
    }
    else if (iFormatType == 2 )
    {
        if (!ParseDebitParaFromJson(szParamContext, dp))
        {
            NotifyCaller(1, RCC_PARAM_ERROR);
            return false;
        }

    }
    else
    {

        TW_TRACE_DIARY_TYPE("��֧�ֵĲ�������", TW_TRACE_TPPE_INTERFACE);
        NotifyCaller(1, RCC_PARAM_ERROR);
        return false;
    }
    AdjustExpwayInfo(dp);//�����շ�վ�������Ϣ
    std::string image_id, task_id;
    GetGlobalApp()->QueryImageIdFromLPRSystem(image_id, task_id);
    strncpy(dp.ImageId, image_id.c_str(), sizeof(dp.ImageId) - 1);
    strncpy(dp.TaskId, task_id.c_str(), sizeof(dp.TaskId) - 1);

    bool start_success = (m_pThreadManage->StartNewPay(dp) == 0);
    return start_success;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// �������ƣ�IF_DebitMoney
//
// ����������ִ�пۿ����
//
// ���������IN const char* szParamContext
//         ��IN const int& iParamSize
//         ��IN const int& iFormatType
// ���������bool
// ��д��Ա��ROCY
// ��дʱ�䣺2016��7��23��
// �޸���Ա��
// �޸�ʱ�䣺
// �����汾��1.0.0.1
// ��ע˵�����ӿ�7
//
////////////////////////////////////////////////////////////////////////////////////////////////
bool WINAPI CTWSDNetPay_DllApp::IF_DebitMoney(IN const char* szParamContext, IN const int& iParamSize, IN const int& iFormatType)
{
    m_lastPayTime = time(NULL);
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    if (m_pThreadManage != 0
            && m_pThreadManage->IsFuturePay())
    {
        return IF_DebitMoney_Future(szParamContext, iParamSize,  iFormatType);
    }

//    bool bReturn = false;
//    CString strLog = FmtStr(_T("[��������Ӧ],ִ�пۿ�����ӿ�(IF_DebitMoney)���ÿ�ʼ,����1:%s,����2:%d,����3:%d"), szParamContext, iParamSize, iFormatType);
//    strLog.Replace(_T("\r"), _T(""));
//    strLog.Replace(_T("\n"), _T(""));
//    RecordLog(strLog);
//    CString strOperationMode(_T("δ֪"));
//    CString strDataString;
//    CString strAnalyError;
//    try
//    {
//        /*****************************У�˹���***********************************/
//        //�ж�����Ƿ��ʼ���ɹ�
//        if (!m_bIsInitEnvironment)
//            throw - 1;
//        //�жϺϷ���
//        if (!m_bAuthorizedLegal)
//            throw - 2;
//        //�жϲ����Ƿ��쳣
//        if ((NULL == szParamContext) || (0 == iParamSize) || (iFormatType > 2 || iFormatType < 1))
//            throw - 3;
//        //�ж��ϴν����Ƿ����
//        if (m_bIsTrading)
//            throw - 7;
//        GetGlobalApp()->ResetDebitResult();
//
//        strDataString.Format(_T("%s"), szParamContext);
//
//        if (1 == iFormatType)
//        {
//            CDVXMLDebitParamContext xml;
//            if (!xml.Init(strDataString))
//            {
//                strAnalyError = xml.m_strLastErrorDesc;
//                throw - 4;
//            }
//
//            //��������
//            m_pCurrentDebitInfo.OverTime            = xml.OverTime;
//            m_pCurrentDebitInfo.OperationMode       = xml.OperationMode;
//            m_pCurrentDebitInfo.PayModel            = xml.PayModel;
//
//            m_pCurrentDebitInfo.PayCertificateCode  = xml.PayCertificateCode;
//            m_pCurrentDebitInfo.PayCertificateType  = xml.PayCertificateType;
//            m_pCurrentDebitInfo.PayIdentifier       = xml.PayIdentifier;
//            m_pCurrentDebitInfo.Money               = xml.Money;
//            m_pCurrentDebitInfo.SubTime             = xml.SubTime;
//            m_pCurrentDebitInfo.DebitMainType       = xml.DebitMainType;
//            m_pCurrentDebitInfo.VehicleType         = xml.VehicleType;
//            m_pCurrentDebitInfo.VehicleClass        = xml.VehicleClass;
//            m_pCurrentDebitInfo.VLColor             = xml.VLColor;
//            m_pCurrentDebitInfo.VehicleLicense      = xml.VehicleLicense;
//            m_pCurrentDebitInfo.AxisCount           = xml.AxisCount;
//            m_pCurrentDebitInfo.TotalWeight         = xml.Weight;
//            m_pCurrentDebitInfo.LimitWeight         = xml.LimitWeight;
//            m_pCurrentDebitInfo.PassCertificateType = xml.PassCertificateType;
//            m_pCurrentDebitInfo.CardID              = xml.CardID;
//            m_pCurrentDebitInfo.TollDate            = xml.TollDate;
//            m_pCurrentDebitInfo.ShiftID             = xml.ShiftID;
//            m_pCurrentDebitInfo.EntryStationID      = xml.EntryStationID;
//            m_pCurrentDebitInfo.EntryStationName    = xml.EntryStationName;
//            m_pCurrentDebitInfo.EntryTime           = xml.EntryTime;
//            m_pCurrentDebitInfo.EntryLane           = xml.EntryLane;
//            m_pCurrentDebitInfo.EntryVehicleLicense = xml.EntryVehicleLicense;
//            m_pCurrentDebitInfo.EntryVLColor        = xml.EntryVLColor;
//            m_pCurrentDebitInfo.EntryVehicleType    = xml.EntryVehicleType;
//            m_pCurrentDebitInfo.EntryVehicleClass   = xml.EntryVehicleClass;
//            m_pCurrentDebitInfo.EntryRoadID         = xml.EntryRoadID;
//            m_pCurrentDebitInfo.EntryAreaID         = xml.EntryAreaID;
//            m_pCurrentDebitInfo.TicketNo            = xml.TicketNo;
//            //���������շ�Ա��Ϣ
//            m_pCurrentDebitInfo.OperatorID          = xml.OperatorID;
//            m_pCurrentDebitInfo.OperatorName        = xml.OperatorName;
//
//            //add 2018-03
//            m_pCurrentDebitInfo.AutoLicense        = xml.AutoLicense;
//            m_pCurrentDebitInfo.AutoColor        = xml.AutoColor;
//            m_pCurrentDebitInfo.Distance        = xml.Distance;
//
//            m_pCurrentDebitInfo.Valid               = TRUE;
//        }
//        else
//        {
//            CDVJsonParamDebitMoney* pDebitMoney = NULL;
//            CJsonParamOperate jsonParamOperation;
////          strDataString.Format(_T("{\"DebitParamContext\":{\"Data\":{\"OperationMode\":2,\"OverTime\":60000,\"Vehicle\":{\"License\":\"0\",\"Weight\":0,\"AxisCount\":0,\"Type\":1,\"VLColor\":0,\"Class\":1},\"Operation\":{\"ShiftID\":1,\"PassCertificateType\":0,\"CardID\":\"0\",\"TollDate\":\"20160823\"},\"Entry\":{\"RoadID\":30,\"License\":\"\",\"AreaID\":4412,\"StationName\":\"����᯴�\",\"VColor\":0,\"VClass\":9,\"VType\":1,\"Time\":\"2016-08-23 14:34:28\",\"LaneID\":0,\"StationID\":8},\"Debit\":{\"PayCertificateType\":1,\"PayCertificateCode\":\"283487891099961852\",\"Money\":200,\"PayIdentifier\":\"1E09381F4F1B90000000000000000004\",\"SubTime\":\"2016-08-23 14:34:28\",\"Type\":1}},\"Version\":\"1.0\"}}"));
//
//            pDebitMoney = (CDVJsonParamDebitMoney*)jsonParamOperation.FormatResponseResult2Class(strDataString, JSON_DT_PARAM_DEBIT_MONEY);
//            if (NULL == pDebitMoney)
//            {
//                strAnalyError.Format(_T(""));
//                throw - 5;
//            }
//            //��������
//            m_pCurrentDebitInfo.OverTime            = pDebitMoney->OverTime;
//            m_pCurrentDebitInfo.OperationMode       = pDebitMoney->OperationMode;
//            m_pCurrentDebitInfo.PayModel            = pDebitMoney->PayModel;
//
//
//            m_pCurrentDebitInfo.PayCertificateCode  = pDebitMoney->PayCertificateCode;
//            m_pCurrentDebitInfo.PayCertificateType  = pDebitMoney->PayCertificateType;
//            m_pCurrentDebitInfo.PayIdentifier       = pDebitMoney->PayIdentifier;
//            m_pCurrentDebitInfo.Money               = pDebitMoney->Money;
//            m_pCurrentDebitInfo.SubTime             = pDebitMoney->SubTime;
//            m_pCurrentDebitInfo.DebitMainType       = pDebitMoney->DebitMainType;
//            m_pCurrentDebitInfo.VehicleType         = pDebitMoney->VehicleType;
//            m_pCurrentDebitInfo.VehicleClass        = pDebitMoney->VehicleClass;
//            m_pCurrentDebitInfo.VLColor             = pDebitMoney->VLColor;
//            m_pCurrentDebitInfo.VehicleLicense      = pDebitMoney->VehicleLicense;
//            m_pCurrentDebitInfo.AxisCount           = pDebitMoney->AxisCount;
//            m_pCurrentDebitInfo.TotalWeight         = pDebitMoney->Weight;
//            m_pCurrentDebitInfo.LimitWeight         = pDebitMoney->LimitWeight;
//            m_pCurrentDebitInfo.PassCertificateType = pDebitMoney->PassCertificateType;
//            m_pCurrentDebitInfo.CardID              = pDebitMoney->CardID;
//            m_pCurrentDebitInfo.TollDate            = pDebitMoney->TollDate;
//            m_pCurrentDebitInfo.ShiftID             = pDebitMoney->ShiftID;
//            m_pCurrentDebitInfo.EntryStationID      = pDebitMoney->EntryStationID;
//            m_pCurrentDebitInfo.EntryStationName    = pDebitMoney->EntryStationName;
//            m_pCurrentDebitInfo.EntryTime           = pDebitMoney->EntryTime;
//            m_pCurrentDebitInfo.EntryLane           = pDebitMoney->EntryLane;
//            m_pCurrentDebitInfo.EntryVehicleLicense = pDebitMoney->EntryVehicleLicense;
//            m_pCurrentDebitInfo.EntryVLColor        = pDebitMoney->EntryVLColor;
//            m_pCurrentDebitInfo.EntryVehicleType    = pDebitMoney->EntryVehicleType;
//            m_pCurrentDebitInfo.EntryVehicleClass   = pDebitMoney->EntryVehicleClass;
//            m_pCurrentDebitInfo.EntryRoadID         = pDebitMoney->EntryRoadID;
//            m_pCurrentDebitInfo.EntryAreaID         = pDebitMoney->EntryAreaID;
//            m_pCurrentDebitInfo.TicketNo            = pDebitMoney->TicketNo;
//            //���������շ�Ա��Ϣ
//            m_pCurrentDebitInfo.OperatorID          = pDebitMoney->OperatorID;
//            m_pCurrentDebitInfo.OperatorName        = pDebitMoney->OperatorName;
//
//            //add 2018-03
//            m_pCurrentDebitInfo.AutoLicense        = pDebitMoney->AutoLicense;
//            m_pCurrentDebitInfo.AutoColor        = pDebitMoney->AutoColor;
//            m_pCurrentDebitInfo.Distance        = pDebitMoney->Distance;
//
//            m_pCurrentDebitInfo.Valid               = TRUE;
//        }
//        //add 2018-01 ,����ʱ��ת�����Ƕ�������ת��Ϊ������
//        m_pCurrentDebitInfo.SubTime = FixUTC8(m_pCurrentDebitInfo.SubTime);
//
//        //���⴦���������
//        switch (m_sTranstParam.ProvinceID)
//        {
//        case 44:
//        {
//            CString strSTID;
//            switch (m_sTranstParam.LaneConfigInfo.RoadID)
//            {
//            //�㶫��������վ�����⴦��
//            case 30:
//            case 31:
//            default:
//            {
//                strSTID.Format(_T("%04d%03d%02d"), m_pCurrentDebitInfo.EntryAreaID, m_pCurrentDebitInfo.EntryRoadID, _ttoi(m_pCurrentDebitInfo.EntryStationID.GetBuffer(0))); m_pCurrentDebitInfo.EntryStationID.ReleaseBuffer();
//                m_pCurrentDebitInfo.EntryStationID = strSTID;
//                //m_pCurrentDebitInfo.EntryStationID = _ttoi(strSTID);
//
//
//            }
//            break;
//            }
//        }
//        break;
//        }
//
//        //���ҵ������߳��Ƿ�����
//        if (NULL == m_pThreadManage)
//            throw - 6;
//
//        //��ǵ�ǰ�������ڽ���
//        m_bIsTrading = TRUE;
//        /*****************************��Ӧ����***********************************/
//        //add 2018-04
//        GetGlobalApp()->QueryImageIdFromLPRSystem();
//
//        int agency_result = 255;
//        //�汾1060���������Ƚ��д��۴���
//        if (!AgencyConstructDebitData(agency_result))
//        {
//            bool is_disable_codepay = (m_pCurrentDebitInfo.PayModel == 2);
//            if (is_disable_codepay)
//            {
//                m_pThreadManage->m_pThreadDevice->PostThreadMessage(WM_THREAD_DEVICE_CONTROL_DEAL_RESULT_MMI, (WPARAM)0x02, (LPARAM)0);//modify 2018-10
//                //throw -9;
//                int err_code = RCC_AGENT_DEBIT_ERROR;
//                if (agency_result == 26)
//                {
//                    err_code = RCC_AGENT_DEBIT_EXCESS_ERROR;
//                }
//                else if (agency_result == 27)
//                {
//                    err_code = RCC_AGENT_DEBIT_RISK_ERROR;
//                }
//                NotifyCaller(0x01, err_code);
//                RecordLog(_T("[��������Ӧ],ִ�пۿ�����ӿ�(IF_DebitMoney): ������ɨ�봦��,�ӿڵ��ý���"));
//                m_bIsTrading = false;
//                return true;
//            }
//            GetGlobalApp()->NotifyPayStatus(1, agency_result);
//
//
//            //������ɨ������
//            switch (m_pCurrentDebitInfo.OperationMode)
//            {
//            case 1://���ϲ���
//            {
//                strOperationMode.Format(_T("����ģʽ"));
//                m_bCurrentDebitMode = TRUE;
//                //�ع�֧������
//                CDVXMLDebitFlow xmlDebit;
//                xmlDebit.ProvinceID     = m_sTranstParam.ProvinceID;
//                xmlDebit.Type           = 0x03;
//                xmlDebit.Money          = m_pCurrentDebitInfo.Money;
//                xmlDebit.Weight         = m_pCurrentDebitInfo.TotalWeight;
//                xmlDebit.VehicleType    = m_pCurrentDebitInfo.VehicleType;
//                xmlDebit.VehicleClass   = m_pCurrentDebitInfo.VehicleClass;
//                xmlDebit.CurrentTime    = time(NULL);
//                CString strXMLString    = xmlDebit.ConstructFormatData();
//
//                //������Դ����
//                CString* pstrString = new CString();
//                if (NULL == pstrString)
//                    throw - 8;
//                pstrString->Format(_T("%s"), strXMLString);
//
//                //�ر��ϴεĶ�ʱ��
//                m_pThreadManage->StopTimer(TIMER_ID_DEBIT_MONEY);
//                m_pThreadManage->StopTimer(TIMER_ID_DEBIT_CHECK);
//                //�����ۿʱ��ʱ��
//                m_pThreadManage->StartTimer(TIMER_ID_DEBIT_MONEY, m_pCurrentDebitInfo.OverTime);
//                //��ʼ��ʱ
//                m_dwTradeBeginOvertime = GetTickCount();
//                //�����ۿ�����
//                m_pThreadManage->PostThreadMessage(WM_THREAD_MANAGE_START_DEBIT_FLOW, (WPARAM)pstrString, (LPARAM)0);
//                //�������
//                m_strLastErrorDesc.Format(_T("[��������Ӧ],ִ�пۿ����(IF_DebitMoney),ҵ�����ڴ���(���ϲ���)"));
//            }
//            break;
//            case 2://�������
//            {
//                strOperationMode.Format(_T("����ģʽ"));
//                m_bCurrentDebitMode = FALSE;
//                //��ʼ��ʱ
//                m_dwTradeBeginOvertime = GetTickCount();
//                //�ر��ϴεĶ�ʱ��
//                m_pThreadManage->StopTimer(TIMER_ID_DEBIT_MONEY);
//                m_pThreadManage->StopTimer(TIMER_ID_DEBIT_CHECK);
//                //�����ۿʱ��ʱ��
//                m_pThreadManage->StartTimer(TIMER_ID_DEBIT_MONEY, m_pCurrentDebitInfo.OverTime);
//                //ִ�пۿ����
//                m_pThreadManage->PostThreadMessage(WM_THREAD_MANAGE_ONLY_DEBIT_MONEY, (WPARAM)0, (LPARAM)0);
//                //�������
//                m_strLastErrorDesc.Format(_T("[��������Ӧ],ִ�пۿ����(IF_DebitMoney),ҵ�����ڴ���(�������)"));
//            }
//            break;
//            }
//            //��¼��־
//            RecordLog(m_strLastErrorDesc);
//        }
//
//        //��ǳɹ�
//        bReturn = true;
//    }
//    catch (int& iErrorCode)
//    {
//        BYTE byFunCode      = 0x01;
//        BYTE byExecuteState = RCC_INNER_ERROR;
//        m_bIsTrading = FALSE;//add 2018-10
//
//        switch (iErrorCode)
//        {
//        case -1:
//            strLog.Format(_T("[��������Ӧ],ִ�пۿ����(IF_DebitMoney)����(%s),���δ�ɹ���ʼ��"), strOperationMode);
//            byExecuteState  = RCC_COM_NOT_INIT;
//            break;
//        case -2:
//            strLog.Format(_T("[��������Ӧ],ִ�пۿ����(IF_DebitMoney)����(%s),�Ϸ���У��ʧ��"), strOperationMode);
//            byExecuteState  = RCC_COM_ILLEGAL;
//            break;
//        case -3:
//            strLog.Format(_T("[��������Ӧ],ִ�пۿ����(IF_DebitMoney)����(%s),��������쳣"), strOperationMode);
//            byExecuteState  = RCC_PARAM_ERROR;
//            break;
//        case -4:
//            strLog.Format(_T("[��������Ӧ],ִ�пۿ����(IF_DebitMoney)����(%s),�������(XML��ʽ)����������ϸ����:%s"), strOperationMode, strAnalyError);
//            byExecuteState  = RCC_PARAM_ERROR;
//            break;
//        case -5:
//            strLog.Format(_T("[��������Ӧ],ִ�пۿ����(IF_DebitMoney)����(%s),�������(JSON��ʽ)��������"), strOperationMode);
//            byExecuteState  = RCC_PARAM_ERROR;
//            break;
//        case -6:
//            strLog.Format(_T("[��������Ӧ],ִ�пۿ����(IF_DebitMoney)����(%s),�ڲ������쳣:ҵ�����ģ��δ����������"), strOperationMode);
//            break;
//        case -7:
//            strLog.Format(_T("[��������Ӧ],ִ�пۿ����(IF_DebitMoney)����(%s),�ڲ������쳣:�ϴν���δ��ɣ�������Ƶ������"), strOperationMode);
//            byExecuteState  = RCC_LAST_TRADE_NOT_END;
//            break;
//        case -8:
//            strLog.Format(_T("[��������Ӧ],ִ�пۿ����(IF_DebitMoney)����(%s),�ڲ������쳣:��Դ����ʧ��"), strOperationMode);
//            break;
//            //case -9:
//            //  RecordLog(_T("[��������Ӧ],ִ�пۿ�����ӿ�(IF_DebitMoney): ������ɨ�봦��"));
//            //  break;
//        }
//        //�����������
//        if (strLog != m_strLastErrorDesc)
//        {
//            m_strLastErrorDesc = strLog;
//            RecordLog(m_strLastErrorDesc, LOG_LEVEL_ERROR);
//        }
//        //���������߽��
//        NotifyCaller(byFunCode, byExecuteState);
//        //������־
//        RecordLog(FmtStr(_T("[��������Ӧ],�ۿ����Ԥ����ʧ�ܣ������֪ͨ������(���ܴ���:%d,ִ��״��:%d)"), byFunCode, byExecuteState));
//    }
//    RecordLog(FmtStr(_T("[��������Ӧ],ִ�пۿ�����ӿ�(IF_DebitMoney)���ý���")));
//
//    return bReturn;
}

bool WINAPI CTWSDNetPay_DllApp::IF_DebitCancel_Future(IN const char* szParamContext, IN const int& iParamSize, IN const int& iFormatType)
{
    CInterfaceTraceInfo trace_interface("IF_DebitCancel", pystring::replace(szParamContext, "\r\n", ""));
    if (!m_bIsInitEnvironment)
    {
        TW_TRACE_DIARY_TYPE("δ���г�ʼ��", TW_TRACE_TPPE_INTERFACE);
        NotifyCaller(2, RCC_COM_NOT_INIT);
        return false;
    }
    if (0 == m_pThreadManage)
    {
        TW_TRACE_DIARY_TYPE("�����߳���Ч", TW_TRACE_TPPE_INTERFACE);
        NotifyCaller(2, RCC_INNER_ERROR);
        return false;
    }
    if (szParamContext == 0 || iParamSize == 0)
    {
        TW_TRACE_DIARY_TYPE("����Ϊ��", TW_TRACE_TPPE_INTERFACE);
        NotifyCaller(2, RCC_PARAM_ERROR);
        return false;
    }
    DebitCancleParameter dp;
    CopyExpwayInfo(GetGlobalApp()->m_sTranstParam, dp);
    if (iFormatType == 1 )
    {
        if (!ParseDebitCancleParaFromXML( szParamContext, dp))
        {
            NotifyCaller(2, RCC_PARAM_ERROR);
            return false;
        }
    }
    else if (iFormatType == 2 )
    {
        if (!ParseDebitCancleParaFromJson(szParamContext, dp))
        {
            NotifyCaller(2, RCC_PARAM_ERROR);
            return false;
        }

    }
    else
    {
        TW_TRACE_DIARY_TYPE("��֧�ֵĲ�������", TW_TRACE_TPPE_INTERFACE);
        NotifyCaller(2, RCC_PARAM_ERROR);
        return false;
    }
    dp.EnableNotification = true;
    bool  cancle_success = (m_pThreadManage->CancelTargetPay(dp) == 0);
    return cancle_success;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// �������ƣ�IF_DebitCancel
//
// ����������ִ�г�������
//
// ���������IN const char* szParamContext
//         ��IN const int& iParamSize
//         ��IN const int& iFormatType
// ���������bool
// ��д��Ա��ROCY
// ��дʱ�䣺2016��7��23��
// �޸���Ա��
// �޸�ʱ�䣺
// �����汾��1.0.0.1
// ��ע˵�����ӿ�8
//
////////////////////////////////////////////////////////////////////////////////////////////////
bool WINAPI CTWSDNetPay_DllApp::IF_DebitCancel(IN const char* szParamContext, IN const int& iParamSize, IN const int& iFormatType)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    if (m_pThreadManage != 0
            && m_pThreadManage->IsFuturePay())
    {
        return IF_DebitCancel_Future(szParamContext, iParamSize, iFormatType);
    }

    bool bReturn = false;
    CString strLog = FmtStr(_T("[��������Ӧ],ִ�г��������ӿ�(IF_DebitCancel)���ÿ�ʼ,����1:%s,����2:%d,����3:%d"), szParamContext, iParamSize, iFormatType);
    strLog.Replace(_T("\r"), _T(""));
    strLog.Replace(_T("\n"), _T(""));
    RecordLog(strLog);

    CString strDataString;
    CString strPayIdentifier;
    CDVXMLDebitCancel xml;
    try
    {
        /*****************************У�˹���***********************************/
        //�ж�����Ƿ��ʼ���ɹ�
        if (!m_bIsInitEnvironment)
            throw - 1;
        //�жϺϷ���
        if (!m_bAuthorizedLegal)
            throw - 2;
        //�жϲ����Ƿ��쳣
        if ((NULL == szParamContext) || (0 == iParamSize) || (iFormatType > 2 || iFormatType < 1))
            throw - 3;

        strDataString.Format(_T("%s"), szParamContext);
        if (1 == iFormatType)
        {
            if (!xml.Init(strDataString))
                throw - 4;
            strPayIdentifier.Format(_T("%s"), xml.PayIdentifier);
        }
        else
        {
            CDVJsonParamDebitCancel* pDebitCancel = NULL;
            CJsonParamOperate jsonParamOperation;
            pDebitCancel = (CDVJsonParamDebitCancel*)jsonParamOperation.FormatResponseResult2Class(strDataString, JSON_DT_PARAM_DEBIT_CANCLE);
            if (NULL == pDebitCancel)
            {
                throw - 5;
            }
            strPayIdentifier = pDebitCancel->PayIdentifier;
        }
        //���ҵ������߳��Ƿ�����
        if (NULL == m_pThreadManage)
            throw - 6;
        //��⳷�����ױ�ʶ�Ƿ�Ϸ�
        strPayIdentifier.TrimLeft();
        strPayIdentifier.TrimRight();
        if (strPayIdentifier.IsEmpty())
            throw - 7;
        //������Դ����
        CString* pstrString = new CString();
        if (NULL == pstrString)
            throw - 8;
        pstrString->Format(_T("%s"), strPayIdentifier);

        m_pThreadManage->PostThreadMessage(WM_THREAD_MANAGE_DEBIT_CANCEL, (WPARAM)pstrString, (LPARAM)0);

        bReturn = true;
    }
    catch (int& iErrorCode)
    {
        BYTE byFunCode      = 0x02;
        BYTE byExecuteState = RCC_INNER_ERROR;

        switch (iErrorCode)
        {
        case -1:
            strLog.Format(_T("[��������Ӧ],ִ�г�������(IF_DebitCancel)����,���δ�ɹ���ʼ��"));
            byExecuteState  = RCC_COM_NOT_INIT;
            break;
        case -2:
            strLog.Format(_T("[��������Ӧ],ִ�г�������(IF_DebitCancel)����,�Ϸ���У��ʧ��"));
            byExecuteState  = RCC_COM_ILLEGAL;
            break;
        case -3:
            strLog.Format(_T("[��������Ӧ],ִ�г�������(IF_DebitCancel)����,��������쳣"));
            byExecuteState  = RCC_PARAM_ERROR;
            break;
        case -4:
            strLog.Format(_T("[��������Ӧ],ִ�г�������(IF_DebitCancel)����,�����������������ϸ����:%s"), xml.m_strLastErrorDesc);
            byExecuteState  = RCC_PARAM_ERROR;
            break;
        case -5:
            strLog.Format(_T("[��������Ӧ],ִ�г�������(IF_DebitCancel)����,���������������(JSON)"));
            byExecuteState  = RCC_PARAM_ERROR;
            break;
        case -6:
            strLog.Format(_T("[��������Ӧ],ִ�г�������(IF_DebitCancel)����,�ڲ������쳣:ҵ�����ģ��δ����������"));
            break;
        case -7:
            strLog.Format(_T("[��������Ӧ],ִ�г�������(IF_DebitCancel)����,�����������,��Ҫ�����Ķ�����Ӧ�Ľ���ʶ����Ϊ��"));
            byExecuteState  = RCC_PARAM_ERROR;
            break;
        case -8:
            strLog.Format(_T("[��������Ӧ],ִ�г�������(IF_DebitCancel)����,�ڲ������쳣:��Դ����ʧ��"));
            break;
        }
        //�����������
        if (strLog != m_strLastErrorDesc)
        {
            m_strLastErrorDesc = strLog;
            RecordLog(m_strLastErrorDesc, LOG_LEVEL_ERROR);
        }
        //���������߽��
        NotifyCaller(byFunCode, byExecuteState);
        //������־
        RecordLog(FmtStr(_T("[��������Ӧ],��������Ԥ����ʧ�ܣ������֪ͨ������(���ܴ���:%d,ִ��״��:%d)"), byFunCode, byExecuteState));
    }
    RecordLog(FmtStr(_T("[��������Ӧ],ִ�г��������ӿ�(IF_DebitCancel)���ý���")));
    return bReturn;
}



bool  WINAPI CTWSDNetPay_DllApp::IF_GetDebitResult_Future(IN OUT char* szParamContext, IN OUT int& iParamSize, IN const int& iFormatType)
{
    CInterfaceTraceInfo trace_interface("IF_GetDebitResult");
    if (!m_bIsInitEnvironment)
    {
        TW_TRACE_DIARY_TYPE("δ���г�ʼ��", TW_TRACE_TPPE_INTERFACE);
        return false;
    }
    if (0 == m_pThreadManage)
    {
        TW_TRACE_DIARY_TYPE("�����߳���Ч", TW_TRACE_TPPE_INTERFACE);
        return false;
    }
    if (iParamSize == 0)
    {
        TW_TRACE_DIARY_TYPE("����Ϊ��", TW_TRACE_TPPE_INTERFACE);
        return false;
    }
    DebitResultParameter dp;
    bool get_result_success = (m_pThreadManage->GetCurrentDebitResult(dp) == 0);
    if (!get_result_success)
    {
        TW_TRACE_DIARY_TYPE("��ȡ���ʧ��", TW_TRACE_TPPE_INTERFACE);
        return false;
    }
    std::string result_str;


    if (iFormatType == 1 )
    {
        PackDebitResult2XML(dp, result_str);
    }
    else if (iFormatType == 2 )
    {
        PackDebitResult2Json(dp, result_str);
    }
    else
    {
        TW_TRACE_DIARY_TYPE("��֧�ֵĲ�������", TW_TRACE_TPPE_INTERFACE);
        return false;
    }
    int result_size = static_cast<int>(result_str.size());
    if ( iParamSize < result_size)
    {
        char err_log[100] = {0};
        sprintf(err_log, "��������С��Ŀǰ��С:%d����Ҫ��С:%d", iParamSize, result_size);
        TW_TRACE_DIARY_TYPE(err_log, TW_TRACE_TPPE_INTERFACE);
        return false;
    }

    strncpy(szParamContext, result_str.c_str(), result_size);
    iParamSize = result_size;
    TW_TRACE_DIARY_TYPE("�������:" + result_str, TW_TRACE_TPPE_INTERFACE);
    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// �������ƣ�IF_GetDebitResult
//
// ������������ȡ�ۿ�/���οۿ���
//
// ���������IN OUT char* szParamContext
//         ��IN OUT int& iParamSize
//         ��IN const int& iFormatType
// ���������bool
// ��д��Ա��ROCY
// ��дʱ�䣺2016��7��23��
// �޸���Ա��
// �޸�ʱ�䣺
// �����汾��1.0.0.1
// ��ע˵�����ӿ�9
//
////////////////////////////////////////////////////////////////////////////////////////////////
bool WINAPI CTWSDNetPay_DllApp::IF_GetDebitResult(IN OUT char* szParamContext, IN OUT int& iParamSize, IN const int& iFormatType)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    if (m_pThreadManage != 0
            && m_pThreadManage->IsFuturePay())
    {
        return IF_GetDebitResult_Future(szParamContext, iParamSize, iFormatType);
    }

    bool bReturn = false;
    CString strLog;
    CString strDataString;
    RecordLog(FmtStr(_T("[��������Ӧ],��ȡ�ۿ����ӿ�(IF_GetDebitResult)���ÿ�ʼ,����1:%04X,����2:%d,����3:%d"), szParamContext, iParamSize, iFormatType));
    try
    {
        //�ж�����Ƿ��ʼ���ɹ�
        if (!m_bIsInitEnvironment)
            throw - 1;
        //�жϺϷ���
        if (!m_bAuthorizedLegal)
            throw - 2;
        //�жϲ����Ƿ��쳣
        if ((iFormatType > 2) || (iFormatType < 1) || (NULL == szParamContext))
            throw - 3;
        //�жϽ���Ƿ��Ѿ�׼����
        if (!m_sCurrentDebitResult.Valid)
            throw - 4;


        switch (iFormatType)
        {
        case 1:
        {
            CDVXMLDebitResult xml;
            xml.ResultType      = m_sCurrentDebitResult.ResultType;
            xml.ExecuteCode     = m_sCurrentDebitResult.ExecuteCode;
            xml.PayPlatformType = m_sCurrentDebitResult.PayPlatformType;

            if (xml.PayPlatformType == 24)
            {
                xml.PayPlatformType = 8;
            }
            xml.DebitTime       = m_sCurrentDebitResult.DebitTime;
            xml.ConsumeTime     = m_sCurrentDebitResult.ConsumeTime;
            xml.ExecuteDesc.Format(_T("%s"), m_sCurrentDebitResult.ExecuteDesc);
            xml.TradeSecurityCode.Format(_T("%s"), m_sCurrentDebitResult.TradeSecurityCode);
            xml.PayIdentifier.Format(_T("%s"), m_sCurrentDebitResult.PayIdentifier);
            xml.DebitOrder.Format(_T("%s"), m_sCurrentDebitResult.DebitOrder);
            memcpy(&xml.ExpandETCInfo, &m_sCurrentDebitResult.ExpandETCData, sizeof(xml.ExpandETCInfo));


            strDataString = xml.GetXMLString();

            strDataString.Replace(_T("\r"), _T(""));
            strDataString.Replace(_T("\n"), _T(""));
        }
        break;
        case 2:
        {
            CDVJsonParamDebitResult jsonDebitResult;
            jsonDebitResult.ResultType      = m_sCurrentDebitResult.ResultType;
            jsonDebitResult.ExecuteCode     = m_sCurrentDebitResult.ExecuteCode;
            jsonDebitResult.PayPlatformType = m_sCurrentDebitResult.PayPlatformType;
            jsonDebitResult.DebitTime       = m_sCurrentDebitResult.DebitTime;
            jsonDebitResult.ConsumeTime     = m_sCurrentDebitResult.ConsumeTime;
            jsonDebitResult.ExecuteDesc.Format(_T("%s"), m_sCurrentDebitResult.ExecuteDesc);
            jsonDebitResult.TradeSecurityCode.Format(_T("%s"), m_sCurrentDebitResult.TradeSecurityCode);
            jsonDebitResult.PayIdentifier.Format(_T("%s"), m_sCurrentDebitResult.PayIdentifier);
            jsonDebitResult.DebitOrder.Format(_T("%s"), m_sCurrentDebitResult.DebitOrder);
            memcpy(&jsonDebitResult.ExpandETCInfo, &m_sCurrentDebitResult.ExpandETCData, sizeof(jsonDebitResult.ExpandETCInfo));

            if (jsonDebitResult.PayPlatformType == 14)
            {
                jsonDebitResult.PayPlatformType = 8;
            }
            CJsonParamOperate jsonParamOperation;
            strDataString = jsonParamOperation.FormatParamData2Json(&jsonDebitResult);
            if (strDataString.IsEmpty())
            {
                throw - 5;
            }
        }
        break;
        default:
            break;
        }
        GetGlobalApp()->ResetDebitResult();
        if (strDataString.GetLength() > (iParamSize + 1))
            throw - 6;

        strDataString.TrimRight();
        RecordLog(FmtStr(_T("[��������Ӧ],��ȡ�ۿ����ӿ�(IF_GetDebitResult)����������:(%s)%s "), (0x01 == iFormatType) ? _T("���ݸ�ʽ:XML") : _T("���ݸ�ʽ:JSON"), strDataString));

        _tcscpy_s(szParamContext, strDataString.GetLength() + 1, strDataString.GetBuffer(0));

        //char chs[3000]={0};
        //sprintf(chs,"��������:%d,�������:%d,�ۿ�����%s",iParamSize,strDataString.GetLength(),szParamContext);
        //GetGlobalApp()->RecordLog(chs);
        strDataString.ReleaseBuffer();
        //���ý��׽��
        m_sCurrentDebitResult.Valid = FALSE;

        //��ǳɹ�
        bReturn = true;
    }
    catch (int& iErrorCode)
    {
        switch (iErrorCode)
        {
        case -1:
            strLog.Format(_T("[��������Ӧ],��ȡ�ۿ�������(IF_GetDebitResult)����,���δ�ɹ���ʼ��"));
            break;
        case -2:
            strLog.Format(_T("[��������Ӧ],��ȡ�ۿ�������(IF_GetDebitResult)����,���δ����Ȩ"));
            break;
        case -3:
            strLog.Format(_T("[��������Ӧ],��ȡ�ۿ�������(IF_GetDebitResult)����,��������쳣"));
            break;
        case -4:
            strLog.Format(_T("[��������Ӧ],��ȡ�ۿ�������(IF_GetDebitResult)����,�ۿ���δ׼����"));
            break;
        }
        //�����������
        if (strLog != m_strLastErrorDesc)
        {
            m_strLastErrorDesc = strLog;
            RecordLog(m_strLastErrorDesc, LOG_LEVEL_ERROR);
        }
    }
    RecordLog(FmtStr(_T("[��������Ӧ],��ȡ�ۿ����ӿ�(IF_GetDebitResult)���ý���")));
    return bReturn;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// �������ƣ�CTWSDNetPay_DllApp::IF_SetMMI
//
// �������������ý�������
//
// ���������IN const char* szParamContext
//         ��IN const int& iParamSize
//         ��IN const int& iFormatType
// ���������bool
// ��д��Ա��ROCY
// ��дʱ�䣺2016��8��2��
// �޸���Ա��
// �޸�ʱ�䣺
// �����汾��1.0.0.1
// ��ע˵�����ӿ�10
//
////////////////////////////////////////////////////////////////////////////////////////////////
bool WINAPI CTWSDNetPay_DllApp::IF_SetMMI(IN const char* szParamContext, IN const int& iParamSize, IN const int& iFormatType)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    bool bReturn = false;
    CString strLog = FmtStr(_T("[��������Ӧ],���ý�������ӿ�(IF_SetMMI)���ÿ�ʼ"));
    strLog.Replace(_T("\r"), _T(""));
    strLog.Replace(_T("\n"), _T(""));
    RecordLog(strLog);

    CString strParamString(szParamContext);
    CString strDataString;
    CString strSubError;
    try
    {
        //�ж�����Ƿ��Ѿ�����ʼ��
        if (!m_bIsInitEnvironment)
            throw - 1;
        //�ж�����Ƿ�ɹ���Ȩ
        if (!m_bAuthorizedLegal)
            throw - 2;
        //�ж�ҵ�����߳��Ƿ��쳣
        if (NULL == m_pThreadManage)
            throw - 3;
        if (NULL == m_pThreadManage->m_pThreadDevice)
            throw - 4;
        //�жϲ����Ƿ��쳣
        if ((NULL == szParamContext) || (0 == iParamSize))
            throw - 5;
        //�жϲ�������
        if ( (iFormatType > 2) || (iFormatType < 1))
            throw - 6;
        //��ȡӲ��״̬
        //  DWORD dwHDState = m_pThreadManage->m_pThreadDevice->GetDeviceState();

        int iMMIType = 0x00;

        if (0x01 == iFormatType)
        {
            //XML��ʽ
            CDVXMLMMI xml;
            if (!xml.Init(strParamString))
            {
                strSubError = xml.m_strLastErrorDesc;
                throw - 7;
            }
            //�ع�XML����
            CMarkup xmlShow;
            xmlShow.SetDoc( _T("<?xml version=\"1.0\" encoding=\"GB2312\"?>\r\n") );
            xmlShow.AddElem( _T("DisplayContext") );//root
            xmlShow.IntoElem();

            xmlShow.AddElem( _T("Version"),     _T("1.0") );
            xmlShow.AddElem( _T("ProvinceID"),  m_sTranstParam.ProvinceID );
            xmlShow.AddElem( _T("VerifyCode"),  0 );

            if (xml.ClearMMI > 0)
            {
                iMMIType = 0x04;
                switch (xml.ClearMMI)
                {
                case 1:
                    iMMIType = 0x04;
                    break;
                case 2:
                    iMMIType = 0x05;
                    break;
                case 3:
                    iMMIType = 0x06;
                    break;
                }

                xmlShow.AddElem( _T("Data") );
                xmlShow.IntoElem();
                xmlShow.AddElem( _T("Type"),        iMMIType );
                xmlShow.OutOfElem();
            }
            else
            {
                if ( xml.Text.GetSize() > 0 )
                {
                    xmlShow.AddElem( _T("Data") );
                    xmlShow.IntoElem();
                    xmlShow.AddElem( _T("Type"),        0x01 );
                    //��ȡ�����ͳ��֡�����������
                    int iMoney  = 0x00;
                    int iWeight = 0x00;
                    int iVType  = 0x00;
                    int iVClass = 0x00;
                    int iLimit  = 0x00;
                    int iAxis   = 0x00;
                    int iBalance = 0x00;
                    CString strEntrySTName;
                    for (int i = 0; i < xml.Text.GetSize(); ++i)
                    {
                        int iTemp = 0x00;
                        switch (xml.Text.GetAt(i).Sub)
                        {
                        case 1://����
                        {
                            iTemp = _ttoi(xml.Text.GetAt(i).Context);
                            if (iTemp > 10)
                            {
                                iVType = 0x02;
                            }
                            else
                            {
                                iVType = 0x01;
                            }
                            iVClass = iTemp % 10;
                        }
                        break;
                        case 2://���
                        {
                            iTemp = _ttoi(xml.Text.GetAt(i).Context);
                            iMoney = iTemp;
                        }
                        break;
                        case 3://����
                        {
                            iTemp = _ttoi(xml.Text.GetAt(i).Context);
                            iWeight = iTemp;
                        }
                        break;
                        case 4://����
                        {
                            iTemp = _ttoi(xml.Text.GetAt(i).Context);
                            iAxis = iTemp;
                        }
                        break;
                        case 5://����
                        {
                            iTemp = _ttoi(xml.Text.GetAt(i).Context);
                            iLimit = iTemp;
                        }
                        break;
                        case 6://����֧�������
                        {
                            iTemp = _ttoi(xml.Text.GetAt(i).Context);
                            iBalance = iTemp;
                        }
                        break;
                        case 7://����շ�վ������
                        {
                            strEntrySTName = xml.Text.GetAt(i).Context;
                        }
                        break;
                        }
                    }

                    xmlShow.AddElem( _T("Money"),           iMoney );
                    xmlShow.AddElem( _T("Weight"),          iWeight );
                    xmlShow.AddElem( _T("VehicleType"),     iVType );
                    xmlShow.AddElem( _T("VehicleClass"),    iVClass );
                    xmlShow.AddElem( _T("LimitWeight"),     iLimit );
                    xmlShow.AddElem( _T("AixsCount"),       iAxis );
                    xmlShow.AddElem( _T("Balance"),         iBalance );
                    xmlShow.AddElem( _T("EntrySTName"),     strEntrySTName );
                    xmlShow.AddElem( _T("CurrentTime"),     CTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S") );
                    xmlShow.OutOfElem();
                }

                if ( xml.Media.GetSize() > 0 )
                {
                    CString strTemp = _T("");
                    for (int j = 0; j < xml.Media.GetSize(); ++j)
                    {
                        xmlShow.AddElem( _T("Data") );
                        xmlShow.IntoElem();
                        xmlShow.AddElem( _T("Type"),        xml.Media.GetAt(j).Type );

                        strTemp.Format( _T("%s"),           xml.Media.GetAt(j).Content );
                        xmlShow.AddElem( _T("Context"),     strTemp );
                        xmlShow.OutOfElem();
                    }
                }
            }

            xmlShow.OutOfElem();
            strDataString = xmlShow.GetDoc();
        }
        else
        {
            //JSON��ʽ
            CDVJsonParamMMI* pJsonMMI = NULL;
            CJsonParamOperate jsonOperation;

//          strParamString.Format(_T("{\"MMIParamContext\":{\"Data\":{\"ClearMMI\":0,\"ItemData\":{\"Graph\":null,\"Voice\":null,\"Text\":[{\"Sub\":1,\"Row\":0,\"Type\":1,\"IsScroll\":0,\"Context\":\"1\"},{\"Sub\":2,\"Row\":0,\"Type\":1,\"IsScroll\":0,\"Context\":\"200\"},{\"Sub\":3,\"Row\":0,\"Type\":1,\"IsScroll\":0,\"Context\":\"0\"},{\"Sub\":4,\"Row\":0,\"Type\":1,\"IsScroll\":0,\"Context\":\"0\"},{\"Sub\":5,\"Row\":0,\"Type\":1,\"IsScroll\":0,\"Context\":\"0\"},{\"Sub\":6,\"Row\":0,\"Type\":1,\"IsScroll\":0,\"Context\":\"0\"},{\"Sub\":7,\"Row\":0,\"Type\":1,\"IsScroll\":0,\"Context\":\"����᯴�\"}]},\"ItemCount\":7},\"Version\":\"1.0\"}}"));

            pJsonMMI = (CDVJsonParamMMI*)jsonOperation.FormatResponseResult2Class(strParamString, JSON_DT_PARAM_MMI);
            if (NULL == pJsonMMI)
            {
                strSubError.Format(_T("%s"), jsonOperation.GetLastErrorDesc());
                throw - 8;
            }

            //�ع�XML����
            CMarkup xmlShow;
            xmlShow.SetDoc( _T("<?xml version=\"1.0\" encoding=\"GB2312\"?>\r\n") );
            xmlShow.AddElem( _T("DisplayContext") );//root
            xmlShow.IntoElem();

            xmlShow.AddElem( _T("Version"),     _T("1.0") );
            xmlShow.AddElem( _T("ProvinceID"),  m_sTranstParam.ProvinceID );
            xmlShow.AddElem( _T("VerifyCode"),  0 );
            xmlShow.AddElem( _T("Data") );

            xmlShow.IntoElem();

            if (pJsonMMI->ClearMMI > 0)
            {
                iMMIType = 0x04;
                switch (pJsonMMI->ClearMMI)
                {
                case 1:
                    iMMIType = 0x04;
                    break;
                case 2:
                    iMMIType = 0x05;
                    break;
                case 3:
                    iMMIType = 0x06;
                    break;
                }
                xmlShow.AddElem( _T("Type"),        iMMIType );
            }
            else
            {
                xmlShow.AddElem( _T("Type"),        0x01 );
                //��ȡ�����ͳ��֡�����������
                int iMoney  = 0x00;
                int iWeight = 0x00;
                int iVType  = 0x00;
                int iVClass = 0x00;
                int iLimit  = 0x00;
                int iAxis   = 0x00;
                int iBalance = 0x00;
                CString strEntrySTName;
                for (int i = 0; i < pJsonMMI->Text.GetSize(); ++i)
                {
                    int iTemp = 0x00;
                    switch (pJsonMMI->Text.GetAt(i).Sub)
                    {
                    case 1://����
                    {
                        iTemp = _ttoi(pJsonMMI->Text.GetAt(i).Context);
                        if (iTemp > 10)
                        {
                            iVType = 0x02;
                        }
                        else
                        {
                            iVType = 0x01;
                        }
                        iVClass = iTemp % 10;
                    }
                    break;
                    case 2://���
                    {
                        iTemp = _ttoi(pJsonMMI->Text.GetAt(i).Context);

                        iMoney = iTemp;
                    }
                    break;
                    case 3://����
                    {
                        iTemp = _ttoi(pJsonMMI->Text.GetAt(i).Context);

                        iWeight = iTemp;
                    }
                    break;
                    case 4://����
                    {
                        iTemp = _ttoi(pJsonMMI->Text.GetAt(i).Context);

                        iAxis = iTemp;
                    }
                    break;
                    case 5://����
                    {
                        iTemp = _ttoi(pJsonMMI->Text.GetAt(i).Context);

                        iLimit = iTemp;
                    }
                    break;
                    case 6://����֧�������
                    {
                        iTemp = _ttoi(pJsonMMI->Text.GetAt(i).Context);

                        iBalance = iTemp;
                    }
                    break;
                    case 7://����շ�վ������
                    {
                        strEntrySTName = pJsonMMI->Text.GetAt(i).Context;
                    }
                    break;
                    }
                }

                xmlShow.AddElem( _T("Money"),           iMoney );
                xmlShow.AddElem( _T("Weight"),          iWeight );
                xmlShow.AddElem( _T("VehicleType"),     iVType );
                xmlShow.AddElem( _T("VehicleClass"),    iVClass );
                xmlShow.AddElem( _T("LimitWeihgt"),     iLimit );
                xmlShow.AddElem( _T("AixsCount"),       iAxis );
                xmlShow.AddElem( _T("Balance"),         iBalance );
                xmlShow.AddElem( _T("EntrySTName"),     strEntrySTName );
                xmlShow.AddElem( _T("CurrentTime"),     CTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S") );
            }

            xmlShow.OutOfElem();
            xmlShow.OutOfElem();

            strDataString = xmlShow.GetDoc();
        }

        if (strDataString.IsEmpty())
            throw - 9;

        //��ȡ����
        CString* pstrString = new CString();
        if (NULL == pstrString)
            throw - 10;
        pstrString->Format(_T("%s"), strDataString);

        m_pThreadManage->m_pThreadDevice->PostThreadMessage(WM_THREAD_DEVICE_SHOW_DEAL_INFO, (WPARAM)pstrString, (LPARAM)iMMIType);

        //��ǳɹ�
        bReturn = true;
    }
    catch (int& iErrorCode)
    {
        switch (iErrorCode)
        {
        case -1:
            strLog.Format(_T("[��������Ӧ],���ý����������(IF_SetMMI)����,���δ�ɹ���ʼ��"));
            break;
        case -2:
            strLog.Format(_T("[��������Ӧ],���ý����������(IF_SetMMI)����,���δ����Ȩ"));
            break;
        case -3:
        case -4:
        case -5:
        case -6:
        case -10:
            strLog.Format(_T("[��������Ӧ],���ý����������(IF_SetMMI)����,����ڲ���Դ�쳣"));
            break;
        case -7:
            strLog.Format(_T("[��������Ӧ],���ý����������(IF_SetMMI)����,��������(XML��ʽ)������ϸ����:%s"), strSubError);
            break;
        case -8:
            strLog.Format(_T("[��������Ӧ],���ý����������(IF_SetMMI)����,��������(JSON��ʽ)������ϸ����:%s"), strSubError);
            break;
        }
        //�����������
        if (strLog != m_strLastErrorDesc)
        {
            m_strLastErrorDesc = strLog;
            RecordLog(m_strLastErrorDesc, LOG_LEVEL_ERROR);
        }
    }
    RecordLog(FmtStr(_T("[��������Ӧ],���ý�������ӿ�(IF_SetMMI)���ý���")));
    return bReturn;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// �������ƣ�WINAPI    CTWSDNetPay_DllApp::IF_GetParam
//
// ������������ȡӲ������
//
// ���������IN const char* szParamContext
//         ��IN const int& iParamSize
//         ��IN const int& iFormatType
//         ��IN OUT char* szResultContext
//         ��IN OUT int& iResultSize
// ���������bool
// ��д��Ա��ROCY
// ��дʱ�䣺2016��8��4��
// �޸���Ա��
// �޸�ʱ�䣺
// �����汾��1.0.0.1
// ��ע˵�����ӿ�11
//
////////////////////////////////////////////////////////////////////////////////////////////////
bool WINAPI CTWSDNetPay_DllApp::IF_GetParam(IN const char* szParamContext, IN const int& iParamSize, IN const int& iFormatType, IN OUT char* szResultContext, IN OUT int& iResultSize)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    bool bReturn = false;
    CString strLog = FmtStr(_T("[��������Ӧ],��ȡӲ�������ӿ�(IF_GetParam)�ݲ��ṩ�����"));

    if (strLog != m_strLastErrorDesc)
    {
        m_strLastErrorDesc = strLog;
        RecordLog(m_strLastErrorDesc, LOG_LEVEL_ERROR);
    }
    return bReturn;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// �������ƣ�IF_GetComponentStatus
//
// ����������������״̬
//
// ���������IN OUT unsigned int& uStatus
// ���������bool
// ��д��Ա��ROCY
// ��дʱ�䣺2016��7��23��
// �޸���Ա��
// �޸�ʱ�䣺
// �����汾��1.0.0.1
// ��ע˵�����ӿ�11
//
////////////////////////////////////////////////////////////////////////////////////////////////
bool WINAPI CTWSDNetPay_DllApp::IF_GetComponentStatus(IN OUT unsigned int& uStatus)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

	uStatus = 0;
    bool bReturn = false;
    DWORD dwState = 0x00;
    CString strLog;
	RecordLog(FmtStr(_T("[��������Ӧ],������״̬�ӿ�(IF_GetComponentStatus)���ÿ�ʼ")));

    try
    {
        //�ж�����Ƿ��Ѿ�����ʼ��
        if (!m_bIsInitEnvironment)
            throw - 1;
        //�ж�����Ƿ�ɹ���Ȩ
        if (!m_bAuthorizedLegal)
            throw - 2;
        //�ж�ҵ�����߳��Ƿ��쳣
        if (NULL == m_pThreadManage)
            throw - 3;
        //�ж�Ӳ�������߳��Ƿ�����
        if (NULL == m_pThreadManage->m_pThreadDevice)
            throw - 4;

        //��ȡӲ��״̬
		bReturn = m_pThreadManage->m_pThreadDevice->EtcEventGetDevStatus(uStatus);
		if(bReturn == false)
			m_strEtcEventLastErrorDesc.Format(_T("���ʹ�������(IF_GetComponentStatus)ʧ�ܣ�"));
    }
    catch (int& iErrorCode)
    {
        switch (iErrorCode)
        {
        case -1:
            strLog.Format(_T("[��������Ӧ],������״̬(IF_GetComponentStatus)����,���δ�ɹ���ʼ��"));
            break;
        case -2:
            strLog.Format(_T("[��������Ӧ],������״̬(IF_GetComponentStatus)����,���δ����Ȩ"));
            break;
        case -3:
        case -4:
            strLog.Format(_T("[��������Ӧ],������״̬(IF_GetComponentStatus)����,����ڲ���Դ�쳣"));
            break;
        }
        //�����������
        if (strLog != m_strLastErrorDesc)
        {
            m_strLastErrorDesc = strLog;
        }
    }
  RecordLog(FmtStr(_T("[��������Ӧ],������״̬�ӿ�(IF_GetComponentStatus)���ý���,����״̬��%ld"),uStatus));

    return bReturn;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// �������ƣ�IF_TranslateData
//
// ����������ִ�д���ؼ���Ϣ���ݲ���
//
// ���������IN const char* szParamContext
//         ��IN const int& iParamSize
//         ��IN const int& iFormatType
// ���������bool
// ��д��Ա��ROCY
// ��дʱ�䣺2016��7��23��
// �޸���Ա��
// �޸�ʱ�䣺
// �����汾��1.0.0.1
// ��ע˵�����ӿ�12
//
////////////////////////////////////////////////////////////////////////////////////////////////
bool WINAPI CTWSDNetPay_DllApp::IF_TranslateData(IN const char* szParamContext, IN const int& iParamSize, IN const int& iFormatType)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    bool bReturn = false;
    CString strLog = FmtStr(_T("[��������Ӧ],ִ�д���ؼ���Ϣ���ݽӿ�(IF_TranslateData)�ݲ��ṩ�����"));
    if (strLog != m_strLastErrorDesc)
    {
        m_strLastErrorDesc = strLog;
        RecordLog(m_strLastErrorDesc, LOG_LEVEL_ERROR);
    }
    return bReturn;
}


bool WINAPI  CTWSDNetPay_DllApp::IF_QueryVirtualCardDebit(IN const char* szParamContext, IN const int& iParamSize, IN const int& iFormatType, IN int& iSupportAgent, OUT char* szResult)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    //�ж��Ƿ����ô��۶�̬��
    if (!m_bUseAgencyDll)
    {
        return false;
    }
    RecordLog(FmtStr(_T("[��������Ӧ],���⿨�ӿڲ�ѯ(IF_QueryVirtualCardDebit)���ÿ�ʼ")));
    //ִ�д��۲�ѯ
    if (m_pThreadManage != 0
            && m_pThreadManage->IsFuturePay())
    {   //add 2019-01 ʹ����֧���߼�
        return m_pThreadManage->QueryVirtualCardDebit(szParamContext, iParamSize, iFormatType, iSupportAgent, szResult);
    }
    iSupportAgent = m_AgencyPay.AgencyQueryVirtualCardDebit(szParamContext, iParamSize, iFormatType, szResult);
    RecordLog(FmtStr(_T("[��������Ӧ],���⿨�ӿڲ�ѯ(IF_QueryVirtualCardDebit)���ý���")));
    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// �������ƣ�IF_Authority
//
// ����������Ȩ����֤
//
// ���������IN const char* szAuthKey
//         ��IN const int& iAuthKeySize
// ���������bool
// ��д��Ա��ROCY
// ��дʱ�䣺2016��7��23��
// �޸���Ա��
// �޸�ʱ�䣺
// �����汾��1.0.0.1
// ��ע˵�����ڲ��ӿ�
//
////////////////////////////////////////////////////////////////////////////////////////////////
bool WINAPI CTWSDNetPay_DllApp::IF_Authority(IN const char* szAuthKey, IN const int& iAuthKeySize)
{
    bool bReturn = false;

    return bReturn;
}


////////////////////////////////////////////////////////////////////////////////////////////////
// �������ƣ�IF_TransferImage
//
// ��������������ͼ��
//
// ���������  IN const char* szImageInfo  ͼ�����ݴ���XML��JSON��
//              IN const int& iSize         ͼ�����ݴ���С
//              IN const int& iFormatType   ͼ�����ݴ���ʽ��XML��JSON��
// ���������bool
// ��д��Ա��WingF
// ��дʱ�䣺2017��8��13��
// �޸���Ա��
// �޸�ʱ�䣺
// �����汾��1.0.0.1
// ��ע˵�����ڲ��ӿ�
//
////////////////////////////////////////////////////////////////////////////////////////////////
bool WINAPI CTWSDNetPay_DllApp::IF_TransferImage(IN const char* szImageInfo, IN const int& iSize, IN const int& iFormatType)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strLog = FmtStr(_T("[��������Ӧ],ͼ��������ӿ�(IF_TransferImage)���ÿ�ʼ(����TRUE)"));
    RecordLog(strLog);

	return true;


    if (m_pThreadManage != 0
            && m_pThreadManage->IsFuturePay())
    {   //add 2019-01 ʹ����֧��
        int execute_ret = m_pThreadManage->TransferImage(szImageInfo, iSize, iFormatType);
        return (execute_ret == 0);
    }

    //GetGlobalApp()->RecordLog(szImageInfo);
    bool bReturn = false;
    strLog = _T("");
    try
    {
        //�ж�����Ƿ��Ѿ�����ʼ��
        if (!m_bIsInitEnvironment)
            throw - 1;
        //�ж�ҵ�����߳��Ƿ��쳣
        if (NULL == m_pThreadManage)
            throw - 2;

        int nImageType = 0;
        int ImageFormat;
        CString csPayID = _T("");
        CString csImageData = _T("");

        CString strImage = _T("");
        strImage.Format( _T("%s"), szImageInfo );

        m_strLastErrorDesc.Format(_T("[��������Ӧ],ִ��ͼ�������(IF_TransferImage), ���ڽ���ͼ�������"));
        //��������ȡ�ؼ�����
        if ( 1 == iFormatType ) // XML
        {
            CMarkup xml;
            if ( xml.SetDoc( strImage ) )
            {
                if ( !xml.FindElem( ) )
                {
                    throw - 12;
                }
                xml.IntoElem();
                xml.ResetMainPos();

                if ( !xml.FindElem(_T("Data")) )
                {
                    throw - 13;
                }
                xml.IntoElem();

                xml.ResetMainPos();
                if (!xml.FindElem(_T("Type")))
                    throw - 14;
                nImageType = _ttoi(xml.GetData());


                xml.ResetMainPos();
                if (!xml.FindElem(_T("DataFormat")))//modify 2018-09
                    throw - 14;
                ImageFormat = _ttoi(xml.GetData());


                xml.ResetMainPos();
                if (!xml.FindElem(_T("PayIdentifier")))
                    throw - 15;
                csPayID.Format( _T("%s"), xml.GetData() );

                xml.ResetMainPos();
                if (!xml.FindElem(_T("Image")))
                    throw - 16;
                csImageData.Format( _T("%s"), xml.GetData() );
            }
            else
            {
                throw - 11;
            }
        }
        else// JSON
        {
            Json::Reader reader;
            Json::Value  root;
            if ( reader.parse( strImage.GetBuffer(0), root) )
            {
                strImage.ReleaseBuffer();

                Json::Value  infos;
                infos = root["DebitImage"];
                Json::Value  datas;
                datas = infos["Data"];

                //Type
                if (Json::intValue == datas["Type"].type())
                {
                    nImageType = datas["Type"].asInt();
                }
                else throw - 22;

                if (Json::intValue == datas["DataFormat"].type())
                {
                    ImageFormat = datas["DataFormat"].asInt();
                }
                else throw - 22;

                //PayIdentifier
                if (Json::stringValue == datas["PayIdentifier"].type())
                {
                    csPayID.Format( _T("%s"), datas["PayIdentifier"].asCString() );
                }
                else throw - 23;

                //Image
                if (Json::stringValue == datas["Image"].type())
                {
                    csImageData.Format( _T("%s"), datas["Image"].asCString() );
                }
                else throw - 24;
            }
            else
            {
                throw - 21;
            }
        }

        //���촫��XML����
        CMarkup xmlImage;
        xmlImage.SetDoc( _T("<?xml version=\"1.0\" encoding=\"GB2312\"?>\r\n") );
        xmlImage.AddElem( _T("AgencyImageContext") );//root
        xmlImage.IntoElem();

        xmlImage.AddElem( _T("Version"),        _T("1.0") );
        xmlImage.AddElem( _T("TimeOut"),        5000 );

        xmlImage.AddElem( _T("Data") );
        xmlImage.IntoElem();
        xmlImage.AddElem( _T("PayIdentifier"),  csPayID );
        xmlImage.AddElem( _T("ImageType"),      nImageType );

        xmlImage.AddElem( _T("ItemData") );
        xmlImage.IntoElem();
        xmlImage.AddElem( _T("Item") );
        xmlImage.IntoElem();

        xmlImage.AddElem( _T("Ordinal"),        1 );

        CString strImgType(_T("Hex"));
        switch (ImageFormat)
        {
        case 1:
            strImgType.Format(_T("Hex"));
            break;
        case 2:
            strImgType.Format(_T("Base64"));
            break;
        }
        xmlImage.AddElem( _T("Type"),           strImgType );
        xmlImage.AddElem( _T("Image"),          csImageData );
        xmlImage.AddElem( _T("MD5"),            _T("No MD5") );

        xmlImage.OutOfElem();
        xmlImage.OutOfElem();
        xmlImage.OutOfElem();

        CString strImageParam = xmlImage.GetDoc();
        int     nImageLength = strImageParam.GetLength();
        int iResultCode = 0x00;
        m_strLastErrorDesc.Format(_T("[��������Ӧ],ִ��ͼ�������(IF_TransferImage), ���ڽ���ͼ���ϴ�����"));
        if ( 0 == (iResultCode = m_AgencyPay.AgencyDebitImage( strImageParam.GetBuffer(0), nImageLength, 1)) )
        {
            RecordLog(FmtStr(_T("[��������Ӧ],ִ��ͼ�������(IF_TransferImage)�����óɹ�")));
            bReturn = true;
        }
        else
        {
            RecordLog(FmtStr(_T("[��������Ӧ],ִ��ͼ�������(IF_TransferImage)�쳣, ��������ʧ��(%d)"), iResultCode) );
        }
    }
    catch (int& iErrorCode)
    {
        switch (iErrorCode)
        {
        case -1:
            strLog.Format(_T("[��������Ӧ],ͼ����(IF_TransferImage)����,���δ�ɹ���ʼ��"));
            break;
        case -2:
            strLog.Format(_T("[��������Ӧ],ͼ����(IF_TransferImage)����,ҵ���߳��쳣"));
            break;
        case -11:
            strLog.Format(_T("[��������Ӧ],ͼ����(IF_TransferImage)����,����XML���������쳣"));
            break;
        case -12:
            strLog.Format(_T("[��������Ӧ],ͼ����(IF_TransferImage)����,����XML�������ڵ��쳣"));
            break;
        case -13:
            strLog.Format(_T("[��������Ӧ],ͼ����(IF_TransferImage)����,����XML���� ������ Data �쳣"));
            break;
        case -14:
            strLog.Format(_T("[��������Ӧ],ͼ����(IF_TransferImage)����,����XML���� ���� Type �쳣"));
            break;
        case -15:
            strLog.Format(_T("[��������Ӧ],ͼ����(IF_TransferImage)����,����XML���� ���� PayIdentifier �쳣"));
            break;
        case -16:
            strLog.Format(_T("[��������Ӧ],ͼ����(IF_TransferImage)����,����XML���� ���� Image �쳣"));
            break;
        case -21:
            strLog.Format(_T("[��������Ӧ],ͼ����(IF_TransferImage)����,����JSON���������쳣"));
            break;
        case -22:
            strLog.Format(_T("[��������Ӧ],ͼ����(IF_TransferImage)����,����JSON���� ���� Type �쳣"));
            break;
        case -23:
            strLog.Format(_T("[��������Ӧ],ͼ����(IF_TransferImage)����,����JSON���� ���� PayIdentifier �쳣"));
            break;
        case -24:
            strLog.Format(_T("[��������Ӧ],ͼ����(IF_TransferImage)����,����JSON���� ���� Image �쳣"));
            break;
        }
        //�����������
        if (strLog != m_strLastErrorDesc)
        {
            m_strLastErrorDesc = strLog;
            RecordLog(m_strLastErrorDesc);
        }
    }
    return bReturn;
}


//�����ֽ�Ʊ����
bool WINAPI CTWSDNetPay_DllApp::IF_TransferCashInvoiceData(IN const char* szInvoiceData, IN const int& iSize, IN const int& iFormatType)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

    CString strLog = FmtStr(_T("[��������Ӧ],�����ֽ�Ʊ���ݽӿ�(IF_TransferCashInvoiceData)���ÿ�ʼ,����1:%s,����2:%d,����3:%d"), szInvoiceData, iSize, iFormatType);
    strLog.Replace(_T("\r"), _T(""));
    strLog.Replace(_T("\n"), _T(""));
    RecordLog(strLog);

	bool bRet = false;
    if (m_pThreadManage != 0 && m_pThreadManage->IsFuturePay())
    {   
		if (iFormatType == 1)
		{
			bRet = m_pThreadManage->TransferCashInvoiceDataXML(szInvoiceData, iSize);
		}
		else if(iFormatType == 2)
		{
			bRet = m_pThreadManage->TransferCashInvoiceDataJSON(szInvoiceData, iSize);
		}
		else
		{			
			strLog.Format(_T("IF_TransferCashInvoiceData�ӿڲ������ʹ���"));
			m_strLastErrorDesc = strLog;
		}
    }

	return bRet;
}

//�����ֽ�Ʊ����
bool WINAPI CTWSDNetPay_DllApp::IF_CancelCashInvoiceData(IN const char* szInvoiceData, IN const int& iSize, IN const int& iFormatType)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

    CString strLog = FmtStr(_T("[��������Ӧ],�����ֽ�Ʊ���ݽӿ�(IF_CancelCashInvoiceData)���ÿ�ʼ,����1:%s,����2:%d,����3:%d"), szInvoiceData, iSize, iFormatType);
    strLog.Replace(_T("\r"), _T(""));
    strLog.Replace(_T("\n"), _T(""));
    RecordLog(strLog);

	bool bRet = false;
    if (m_pThreadManage != 0 && m_pThreadManage->IsFuturePay())
    {   
		if (iFormatType == 1)
		{
			bRet = m_pThreadManage->CancelCashInvoiceDataXML(szInvoiceData, iSize);
		}
		else if(iFormatType == 2)
		{
			bRet = m_pThreadManage->CancelCashInvoiceDataJSON(szInvoiceData, iSize);
		}
		else
		{			
			strLog.Format(_T("IF_CancelCashInvoiceData�ӿڲ������ʹ���"));
			m_strLastErrorDesc = strLog;
		}
    }

	return bRet;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// �������ƣ�WINAPI    CTWSDNetPay_DllApp::IF_AgencyDebitQuery
//
// �������������۲�ѯ����ѯ�����Ƿ�֧�ִ��ۣ�
//
// ���������IN const char* szParamContext
//         ��IN const int& iSize
//         ��IN const int& iFormatType
// ���������bool
// ��д��Ա��ROCY
// ��дʱ�䣺2017��10��16��
// �޸���Ա��
// �޸�ʱ�䣺
// �����汾��1.0.0.1
// ��ע˵����
//
////////////////////////////////////////////////////////////////////////////////////////////////
bool CTWSDNetPay_DllApp::IF_AgencyDebitQuery(IN const char* szParamContext, IN const int& iParamSize, IN const int& iFormatType, IN OUT int& iResult)
{
    //�ж��Ƿ����ô��۶�̬��
    if (!m_bUseAgencyDll)
    {
        return false;
    }
    if (m_pThreadManage != 0
            && m_pThreadManage->IsFuturePay())
    {   //add 2019-01 ʹ����֧���߼�
        return m_pThreadManage->AgencyDebitQuery(szParamContext, iParamSize, iFormatType, iResult);
    }
    //ִ�д��۲�ѯ
    iResult = m_AgencyPay.AgencyDebitQuery(szParamContext, iParamSize, iFormatType);

    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// �������ƣ�WINAPI    CTWSDNetPay_DllApp::IF_InnerGetDeviceNo
//
// ������������ȡӲ�����ն����
//
// ���������IN char* szParamContext
//         ��IN int& iSize
// ���������bool
// ��д��Ա��ROCY
// ��дʱ�䣺2017��10��16��
// �޸���Ա��
// �޸�ʱ�䣺
// �����汾��1.0.0.1
// ��ע˵�����ڲ��ӿ�
//
////////////////////////////////////////////////////////////////////////////////////////////////
bool CTWSDNetPay_DllApp::IF_InnerGetDeviceNo(IN char* szParamContext, IN int& iSize)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    bool bReturn = false;
    DWORD dwState = 0x00;
    CString strLog;
//  RecordLog(FmtStr(_T("[��������Ӧ],�ڲ��ӿ�(IF_InnerGetDeviceNo)���ÿ�ʼ,����1:%d"),szParamContext,iSize));
    try
    {
        //�ж�����Ƿ��Ѿ�����ʼ��
        if (!m_bIsInitEnvironment)
            throw - 1;
        //�ж�����Ƿ�ɹ���Ȩ
        if (!m_bAuthorizedLegal)
            throw - 2;
        //�жϲ��������Ƿ���Ч
        if ((NULL == szParamContext) || (iSize < 24))
            throw - 3;

        if (!m_strTWProductSerialNo.IsEmpty())
        {
            memset(szParamContext, 0, iSize);
            strcpy_s(szParamContext, iSize, m_strTWProductSerialNo.GetBuffer(0));
            m_strTWProductSerialNo.ReleaseBuffer();
        }

        //��ǳɹ�
        bReturn = true;
    }
    catch (int& iErrorCode)
    {
        switch (iErrorCode)
        {
        case -1:
            strLog.Format(_T("[��������Ӧ],������״̬(IF_GetComponentStatus)����,���δ�ɹ���ʼ��"));
            break;
        case -2:
            strLog.Format(_T("[��������Ӧ],������״̬(IF_GetComponentStatus)����,���δ����Ȩ"));
            break;
        case -3:
        case -4:
            strLog.Format(_T("[��������Ӧ],������״̬(IF_GetComponentStatus)����,����ڲ���Դ�쳣"));
            break;
        }
        //�����������
        if (strLog != m_strLastErrorDesc)
        {
            m_strLastErrorDesc = strLog;
        }
    }
//  RecordLog(FmtStr(_T("[��������Ӧ],�ڲ��ӿ�(IF_InnerGetDeviceNo)���ý���")));

    return bReturn;
}


////////////////////////////////////////////////////////////////////////////////////////////////
// �������ƣ�CTWSDNetPay_DllApp::IF_InnerGetSpecialData
//
// ������������ȡָ������
//
// ���������IN char* szParamContext
//         ��IN int& iParamSize
// ���������bool
// ��д��Ա��ROCY
// ��дʱ�䣺2017��10��17��
// �޸���Ա��
// �޸�ʱ�䣺
// �����汾��1.0.0.1
// ��ע˵�����ڲ��ӿ�
//
////////////////////////////////////////////////////////////////////////////////////////////////
bool CTWSDNetPay_DllApp::IF_InnerGetSpecialData(IN char* szParamContext, IN int& iParamSize)
{
    return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// �������ƣ�CTWSDNetPay_DllApp::IF_InnerSetSpecialData
//
// ��������������ָ������
//
// ���������IN const char* szParamContext
//         ��IN const int& iParamSize
// ���������bool
// ��д��Ա��ROCY
// ��дʱ�䣺2017��10��17��
// �޸���Ա��
// �޸�ʱ�䣺
// �����汾��1.0.0.1
// ��ע˵�����ڲ��ӿ�
//
////////////////////////////////////////////////////////////////////////////////////////////////
bool CTWSDNetPay_DllApp::IF_InnerSetSpecialData(IN const char* szParamContext, IN const int& iParamSize)
{
    return false;
}



//��ʼ���������
bool WINAPI CTWSDNetPay_DllApp::IF_EventInitEnvironment3 (IN const UINT& nThreadID, IN const HWND& hWnd, IN const unsigned int& nNotifyMsgID, IN const char* szAreaInfo, IN const char* szLocalStation, IN const char* szLocalLaneID, IN const char* szServerInfo, IN const int& iProvinceID, IN OUT void* pIndPtr, void (*fun)(int option, int result, void* pIndPtr))
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	bool bRet = false;

	RecordLog(FmtStr(_T("[ETC����ҵ��],ETC�����ʼ���ӿ�(IF_EventInitEnvironment3)���ÿ�ʼ")));
	RecordLog(FmtStr(_T("AreaInfo:%s, LocalStation:%s, ServerInfo:%s, pIndPtr:%04X, Fun:%04X"), szAreaInfo, szLocalStation, szServerInfo, m_pIndPtrEtcEvent, m_CallbackNotifyEtcEvent));

    CString strLog;
    try
    {
		if(m_bIsInitEnvironment == false && m_iCurrentWorkMode == 0)
		{
			throw - 13;
		}

        if ((0 == nThreadID) && (NULL == hWnd))
		{
            throw - 1;
		}
		
		EtcEventRespMessgeID   = nNotifyMsgID;		//EtcEventӦ��֪ͨ��ϢID������Ҫ��ͨ����Ϣ֪ͨ�ķ�ʽ����EtcEventӦ��
		EtcEventCallerWnd	   = hWnd;				//EtcEvent�����ߴ�����
		EtcEventCallerThreadID = nThreadID;			//EtcEvent�������߳�ID

        if (NULL == pIndPtr && m_iCurrentWorkMode == 0)
            throw - 2;

        //�ص�����ָ�븳ֵ
		if(fun != NULL)
			m_CallbackNotifyEtcEvent = fun;
		//ETC����Ӧ��ָ�룬�ڻص�֪ͨ��ʱ��Ӧ�����ݴ��ݳ�ȥ
		m_pIndPtrEtcEvent = pIndPtr;

        if ((NULL == szAreaInfo) || (0 == _tcslen(szAreaInfo)))
            throw - 3;
        if ((NULL == szLocalStation) || (0 == _tcslen(szLocalStation)))
            throw - 4;
        if ((NULL == szLocalLaneID) || (0 == _tcslen(szLocalLaneID)))
            throw - 5;
        if ((NULL == szServerInfo) || (0 == _tcslen(szServerInfo)))
            throw - 6;
        if (0 == iProvinceID)
            throw - 7;
        if ((0 == nThreadID) && (!::IsWindow(hWnd)))
            throw - 8;
        //���������Ϣ
        TCHAR   szTemp[256] = {0};
        CString strTemp;
        CString strSTID;
		CString csErrorMsg;

        //���泵������ĳ�ʼ������
        int nProvinceID = iProvinceID;
        UINT CallerThreadID = nThreadID;
        HWND CallerWnd = hWnd;
        int iLaneID = _ttoi(szLocalLaneID);

		theApp.m_EtcProvinceId = iProvinceID;

		std::string TmpAreaInfo = szAreaInfo;
		std::size_t pos = TmpAreaInfo.find("#");
		if (pos == std::string::npos)
			throw -3;
		theApp.m_EtcAreaId = TmpAreaInfo.substr(0, pos);

		std::string strleft = TmpAreaInfo.substr(pos + 1);
		pos = strleft.find("#");
		if (pos == std::string::npos) 
			throw -3;
		theApp.m_EtcAreaName = strleft.substr(0, pos);

		strleft = strleft.substr(pos + 1);
		pos = strleft.find("#");
		if (pos == std::string::npos)
			throw -3;
		theApp.m_EtcRoadId = strleft.substr(0, pos);
		theApp.m_EtcRoadName = strleft.substr(pos + 1);		

		char AreaInfo[128]={0};
		_snprintf(AreaInfo,128,"m_EtcAreaId:%s,m_EtcAreaName:%s,m_EtcRoadId:%s,m_EtcRoadName:%s",theApp.m_EtcAreaId.c_str(),theApp.m_EtcAreaName.c_str(),theApp.m_EtcRoadId.c_str(),theApp.m_EtcRoadName.c_str());
		RecordLog(AreaInfo);

		std::string TmpStation = szLocalStation;
		pos = TmpStation.find("#");
		if (pos == std::string::npos)
			pos = 0;
		theApp.m_EtcStationId = TmpStation.substr(0, pos);
		theApp.m_EtcStationName = TmpStation.substr(pos+1);

		theApp.m_EtcLaneId = szLocalLaneID;
		theApp.m_EtcServerIP = "127.0.0.1";
		theApp.m_EtcServerPort = 8080;
		theApp.m_EtcServerUpdatePort = 3457;		
		
		//��������
		if (m_bIsEtcEventEnvironmeInited)
		{
			throw -9;
		}

		//�豸�����߳����ƶ�֧���ӿ�������������
	    //if(theApp.m_pThreadSMManager==NULL)
		//	theApp.m_pThreadSMManager = (CThreadSMManager*)::AfxBeginThread(RUNTIME_CLASS(CThreadSMManager), THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
		//�����ⲿ���������ƶ�֧����ʼ���͸ýӿڣ����豸�̻߳�û�д�����������ȴ�200���룬�ȴ������------2021-12-14
		Sleep(200);
		if (theApp.m_pThreadManage == NULL)
		{
			throw -10;
		}
		if (theApp.m_pThreadManage->m_pThreadDevice == NULL)
		{
			throw -11;
		}

		if(theApp.m_pThreadNotifyCaller == NULL)
			theApp.m_pThreadNotifyCaller = (CThreadNotifyCaller*)::AfxBeginThread(RUNTIME_CLASS(CThreadNotifyCaller), THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
		if (theApp.m_pThreadNotifyCaller == NULL)
		{
			throw -12;
		}
		theApp.m_pThreadNotifyCaller->ResumeThread(); 
		
		std::string InitJsonData = GenerateEtcEventInitData();
		if(theApp.m_pThreadManage->m_pThreadDevice->EtcEventSetA5InitData(InitJsonData.c_str(), InitJsonData.size()) == false)
		{
			throw -14;
		}
		HWND hWnd = 0;
		NotifyCallerThreadID = m_pThreadNotifyCaller->m_nThreadID;
		bRet = theApp.m_pThreadManage->m_pThreadDevice->EtcEventInitDev(NotifyCallerThreadID, DevMangerThreadID, hWnd, WM_THREAD_DEVICE_ETC_EVENT_RESP, WM_THREAD_DEVICE_NOTIFY, m_pIndPtrEtcEvent);
		
		if(bRet)
			m_bIsEtcEventEnvironmeInited = true;        
    }
    catch (int& iErrorCode)
    {
        switch (iErrorCode)
        {
        case -1:
            strLog.Format(_T("EtcEvent������ʼ���ӿ�(IF_EventInitEnvironment3),�������������󣬴����߳�ID�봰������Ϊ0"));
            break;
        case -2:
            strLog.Format(_T("EtcEvent������ʼ���ӿ�(IF_EventInitEnvironment3),�������������󣬴���Ӧ��ָ�����"));
            break;
        case -3:
            strLog.Format(_T("EtcEvent������ʼ���ӿ�(IF_EventInitEnvironment3),�������������󣬴���������Ϣ�Ƿ�"));
            break;
        case -4:
            strLog.Format(_T("EtcEvent������ʼ���ӿ�(IF_EventInitEnvironment3),�������������󣬴���վ�������Ϣ�Ƿ�"));
            break;
        case -5:
            strLog.Format(_T("EtcEvent������ʼ���ӿ�(IF_EventInitEnvironment3),�������������󣬴��복��������Ϣ�Ƿ�"));
            break;
        case -6:
            strLog.Format(_T("EtcEvent������ʼ���ӿ�(IF_EventInitEnvironment3),�������������󣬴����������Ϣ�Ƿ�"));
            break;
        case -7:
            strLog.Format(_T("EtcEvent������ʼ���ӿ�(IF_EventInitEnvironment3),�������������󣬴���ʡ�ݱ���Ϊ0"));
            break;
        case -8:
            strLog.Format(_T("EtcEvent������ʼ���ӿ�(IF_EventInitEnvironment3),��������������,����������߳�ID�ʹ���������ͬʱΪ��"));
            break;
        case -9:
            strLog.Format(_T("EtcEvent������ʼ���ӿ�(IF_EventInitEnvironment3),��ʼ��ʧ��(�ظ���ʼ��EtcEvent����)"));
            break;
        case -10:
            strLog.Format(_T("EtcEvent������ʼ���ӿ�(IF_EventInitEnvironment3),ҵ������߳���ԴΪ�գ�"));
            break;
        case -11:
            strLog.Format(_T("EtcEvent������ʼ���ӿ�(IF_EventInitEnvironment3),�豸�����߳���ԴΪ�գ�"));
            break;
		case -12:
            strLog.Format(_T("EtcEvent������ʼ���ӿ�(IF_EventInitEnvironment3),��������ص�֪ͨ�߳���Դʧ��"));
            break;
		case -13:
            strLog.Format(_T("EtcEvent������ʼ���ӿ�(IF_EventInitEnvironment3),���ȳ�ʼ���ƶ�֧��������"));
            break;
		case -14:
            strLog.Format(_T("EtcEvent������ʼ���ӿ�(IF_EventInitEnvironment3),�����豸��ʼ������ʧ�ܣ�"));
            break;
        }
        //��¼��־
        if (theApp.m_strEtcEventLastErrorDesc != strLog)
        {
            theApp.m_strEtcEventLastErrorDesc = strLog;
        }
		RecordLog(strLog);
    }

	return bRet;
}


//������Դ
bool WINAPI CTWSDNetPay_DllApp::IF_EventDestroy(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	RecordLog(FmtStr(_T("[ETC����ҵ��],�������ж�ؽӿ�(IF_EventDestroy)���ÿ�ʼ")));
	CString strLog;

	if(m_pThreadManage != NULL)
	{
		if(m_pThreadManage->m_pThreadDevice != NULL)
		{
			m_pThreadManage->m_pThreadDevice->EtcEventDeInitDev();
		}
	}

	RecordLog(_T("NotifyCaller�߳��˳�..."));
	if (NULL != theApp.m_pThreadNotifyCaller)
	{
		theApp.m_pThreadNotifyCaller->m_bExit = TRUE;
		::Sleep(100);

		int itry = 0;
		while (::WaitForSingleObject(theApp.m_pThreadNotifyCaller->m_hThread, 200) != WAIT_OBJECT_0)
		{
			int nErrCode = ::GetLastError();
			if (6 == nErrCode) break;
			::Sleep(100);
			if (itry++ > 9) break;
		}
		theApp.m_pThreadNotifyCaller = NULL;
	}

	m_bIsEtcEventEnvironmeInited = false;
	return true;
}


//��ȡ��һ�δ�����Ϣ
const char* WINAPI CTWSDNetPay_DllApp::IF_EventGetLastErrorDesc(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	RecordLog(FmtStr(_T("[ETC����ҵ��],��ȡ�ϴδ��������ӿ�(IF_EventGetLastErrorDesc)���ÿ�ʼ")));

	return m_strLastErrorDesc.GetBuffer(0);
}

//��ȡ���״̬
bool WINAPI CTWSDNetPay_DllApp::IF_EventGetComponentStatus(IN OUT unsigned int& uStatus)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	bool bRet = false;
	uStatus = 0;
	RecordLog(FmtStr(_T("[ETC����ҵ��],��ȡ�豸״̬�ӿ�(IF_EventGetComponentStatus)���ÿ�ʼ")));

	CString strLog;
	try
    {
        //�ж�����Ƿ��Ѿ�����ʼ��
        if (!m_bIsInitEnvironment)
            throw - 1;
        //�ж�����Ƿ�ɹ���Ȩ
        if (!m_bAuthorizedLegal)
            throw - 2;
        //�ж�ҵ�����߳��Ƿ��쳣
        if (NULL == m_pThreadManage)
            throw - 3;
        //�ж�Ӳ�������߳��Ƿ�����
        if (NULL == m_pThreadManage->m_pThreadDevice)
            throw - 4;

        bRet = m_pThreadManage->m_pThreadDevice->EtcEventGetDevStatus(uStatus);
		if(bRet == false)
			m_strEtcEventLastErrorDesc.Format(_T("���ʹ�������(IF_EventGetComponentStatus)ʧ�ܣ�"));
    }
    catch (int& iErrorCode)
    {
        switch (iErrorCode)
        {
        case -1:
            strLog.Format(_T("[ETC����ҵ��],��ȡ���״̬�ӿ�(IF_EventGetComponentStatus)����,���δ�ɹ���ʼ��"));
            break;
        case -2:
            strLog.Format(_T("[ETC����ҵ��],��ȡ���״̬�ӿ�(IF_EventGetComponentStatus)����,���δ����Ȩ"));
            break;
        case -3:
        case -4:
            strLog.Format(_T("[ETC����ҵ��],��ȡ���״̬�ӿ�(IF_EventGetComponentStatus)����,����ڲ���Դ�쳣"));
            break;
        }
        //�����������
        if (strLog != m_strEtcEventLastErrorDesc)
        {
            m_strEtcEventLastErrorDesc = strLog;
        }
    }

	RecordLog(FmtStr(_T("[ETC����ҵ��],��ȡ�豸״̬�ӿ�(IF_EventGetComponentStatus)���ý���������״ֵ̬��%ld"),uStatus));


	return bRet;
}

//���鴦��ʼ
bool WINAPI CTWSDNetPay_DllApp::IF_EventDealStart(IN const char* szParamContext, IN const int& iParamSize, IN const int& iFormatType)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	bool bRet = false;

    CString strLog;
    RecordLog(FmtStr(_T("[ETC����ҵ��],���鴦��ʼ�ӿ�(IF_EventDealStart)���ÿ�ʼ,����:%s"),szParamContext));
	CString ErrorDescribe;
	if(!CheckEventDealStartJsonValidity(szParamContext, ErrorDescribe))
	{
		m_strEtcEventLastErrorDesc = ErrorDescribe;
		return bRet;
	}

    try
    {
        //�ж�����Ƿ��Ѿ�����ʼ��
        if (!m_bIsInitEnvironment)
            throw - 1;
        //�ж�����Ƿ�ɹ���Ȩ
        if (!m_bAuthorizedLegal)
            throw - 2;
        //�ж�ҵ�����߳��Ƿ��쳣
        if (NULL == m_pThreadManage)
            throw - 3;
        //�ж�Ӳ�������߳��Ƿ�����
        if (NULL == m_pThreadManage->m_pThreadDevice)
            throw - 4;

        bRet = m_pThreadManage->m_pThreadDevice->EtcEventDealStart(szParamContext, iParamSize);
		if(bRet == false)
			m_strEtcEventLastErrorDesc.Format(_T("���ʹ�������ʧ�ܣ�"));

		if(m_bEnableMediaGateWay)
		{
			m_pThreadManage->MediaStart();
		}
    }
    catch (int& iErrorCode)
    {
        switch (iErrorCode)
        {
        case -1:
            strLog.Format(_T("[ETC����ҵ��],���鴦��ʼ�ӿ�(IF_EventDealStart)����,���δ�ɹ���ʼ��"));
            break;
        case -2:
            strLog.Format(_T("[ETC����ҵ��],���鴦��ʼ�ӿ�(IF_EventDealStart)����,���δ����Ȩ"));
            break;
        case -3:
        case -4:
            strLog.Format(_T("[ETC����ҵ��],���鴦��ʼ�ӿ�(IF_EventDealStart)����,����ڲ���Դ�쳣"));
            break;
        }
        //�����������
        if (strLog != m_strEtcEventLastErrorDesc)
        {
            m_strEtcEventLastErrorDesc = strLog;
        }
    }

	return bRet;
}

//��������ȷ��
bool WINAPI CTWSDNetPay_DllApp::IF_EventCheckVehQueue(IN const char* szParamContext, IN const int& iParamSize, IN const int& iFormatType)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	bool bRet = false;

	CString strLog;
    RecordLog(FmtStr(_T("[ETC����ҵ��],��������ȷ�Ͻӿ�(IF_EventCheckVehQueue)���ÿ�ʼ,����:%s"),szParamContext));
	CString ErrorDescribe;
	if(!CheckEventCheckVehQueueJsonValidity(szParamContext, ErrorDescribe))
	{
		m_strEtcEventLastErrorDesc = ErrorDescribe;
		return bRet;
	}

    try
    {
        //�ж�����Ƿ��Ѿ�����ʼ��
        if (!m_bIsInitEnvironment)
            throw - 1;
        //�ж�����Ƿ�ɹ���Ȩ
        if (!m_bAuthorizedLegal)
            throw - 2;
        //�ж�ҵ�����߳��Ƿ��쳣
        if (NULL == m_pThreadManage)
            throw - 3;
        //�ж�Ӳ�������߳��Ƿ�����
        if (NULL == m_pThreadManage->m_pThreadDevice)
            throw - 4;

        bRet = m_pThreadManage->m_pThreadDevice->EtcEventCheckVehQueue(szParamContext, iParamSize);
		if(bRet == false)
			m_strEtcEventLastErrorDesc.Format(_T("���ʹ�������ʧ�ܣ�"));
    }
    catch (int& iErrorCode)
    {
        switch (iErrorCode)
        {
        case -1:
            strLog.Format(_T("[ETC����ҵ��],��������ȷ�Ͻӿ�(IF_EventCheckVehQueue)����,���δ�ɹ���ʼ��"));
            break;
        case -2:
            strLog.Format(_T("[ETC����ҵ��],��������ȷ�Ͻӿ�(IF_EventCheckVehQueue)����,���δ����Ȩ"));
            break;
        case -3:
        case -4:
            strLog.Format(_T("[ETC����ҵ��],��������ȷ�Ͻӿ�(IF_EventCheckVehQueue)����,����ڲ���Դ�쳣"));
            break;
        }
        //�����������
        if (strLog != m_strEtcEventLastErrorDesc)
        {
            m_strEtcEventLastErrorDesc = strLog;
        }
    }

	return bRet;
}

//������Ϣȷ��
bool WINAPI CTWSDNetPay_DllApp::IF_EventCheckVehInfo(IN const char* szParamContext,IN const int& iParamSize,IN const int& iFormatType)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	bool bRet = false;

	CString strLog;
    RecordLog(FmtStr(_T("[ETC����ҵ��],������Ϣȷ�Ͻӿ�(IF_EventCheckVehInfo)���ÿ�ʼ,����:%s"),szParamContext));
	CString ErrorDescribe;
	if(!CheckEventCheckVehInfoJsonValidity(szParamContext, ErrorDescribe))
	{
		m_strEtcEventLastErrorDesc = ErrorDescribe;
		return bRet;
	}

    try
    {
        //�ж�����Ƿ��Ѿ�����ʼ��
        if (!m_bIsInitEnvironment)
            throw - 1;
        //�ж�����Ƿ�ɹ���Ȩ
        if (!m_bAuthorizedLegal)
            throw - 2;
        //�ж�ҵ�����߳��Ƿ��쳣
        if (NULL == m_pThreadManage)
            throw - 3;
        //�ж�Ӳ�������߳��Ƿ�����
        if (NULL == m_pThreadManage->m_pThreadDevice)
            throw - 4;

        bRet = m_pThreadManage->m_pThreadDevice->EtcEventCheckVehInfo(szParamContext, iParamSize);
		if(bRet == false)
			m_strEtcEventLastErrorDesc.Format(_T("���ʹ�������ʧ�ܣ�"));
    }
    catch (int& iErrorCode)
    {
        switch (iErrorCode)
        {
        case -1:
            strLog.Format(_T("[ETC����ҵ��],������Ϣȷ�Ͻӿ�(IF_EventCheckVehInfo)����,���δ�ɹ���ʼ��"));
            break;
        case -2:
            strLog.Format(_T("[ETC����ҵ��],������Ϣȷ�Ͻӿ�(IF_EventCheckVehInfo)����,���δ����Ȩ"));
            break;
        case -3:
        case -4:
            strLog.Format(_T("[ETC����ҵ��],������Ϣȷ�Ͻӿ�(IF_EventCheckVehInfo)����,����ڲ���Դ�쳣"));
            break;
        }
        //�����������
        if (strLog != m_strEtcEventLastErrorDesc)
        {
            m_strEtcEventLastErrorDesc = strLog;
        }
    }

	return bRet;
}

//�����Ϣȷ��
bool WINAPI CTWSDNetPay_DllApp::IF_EventCheckEntryInfo(IN const char* szParamContext,IN const int& iParamSize,IN const int& iFormatType)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	bool bRet = false;

	CString strLog;
    RecordLog(FmtStr(_T("[ETC����ҵ��],�����Ϣȷ�Ͻӿ�(IF_EventCheckEntryInfo)���ÿ�ʼ,����:%s"),szParamContext));
	CString ErrorDescribe;
	if(!CheckEventCheckEntryInfoJsonValidity(szParamContext, ErrorDescribe))
	{
		m_strEtcEventLastErrorDesc = ErrorDescribe;
		return bRet;
	}

    try
    {
        //�ж�����Ƿ��Ѿ�����ʼ��
        if (!m_bIsInitEnvironment)
            throw - 1;
        //�ж�����Ƿ�ɹ���Ȩ
        if (!m_bAuthorizedLegal)
            throw - 2;
        //�ж�ҵ�����߳��Ƿ��쳣
        if (NULL == m_pThreadManage)
            throw - 3;
        //�ж�Ӳ�������߳��Ƿ�����
        if (NULL == m_pThreadManage->m_pThreadDevice)
            throw - 4;

        bRet = m_pThreadManage->m_pThreadDevice->EtcEventCheckEntryInfo(szParamContext, iParamSize);
		if(bRet == false)
			m_strEtcEventLastErrorDesc.Format(_T("���ʹ�������ʧ�ܣ�"));
    }
    catch (int& iErrorCode)
    {
        switch (iErrorCode)
        {
        case -1:
            strLog.Format(_T("[ETC����ҵ��],�����Ϣȷ�Ͻӿ�(IF_EventCheckEntryInfo)����,���δ�ɹ���ʼ��"));
            break;
        case -2:
            strLog.Format(_T("[ETC����ҵ��],�����Ϣȷ�Ͻӿ�(IF_EventCheckEntryInfo)����,���δ����Ȩ"));
            break;
        case -3:
        case -4:
            strLog.Format(_T("[ETC����ҵ��],�����Ϣȷ�Ͻӿ�(IF_EventCheckEntryInfo)����,����ڲ���Դ�쳣"));
            break;
        }
        //�����������
        if (strLog != m_strEtcEventLastErrorDesc)
        {
            m_strEtcEventLastErrorDesc = strLog;
        }
    }

	return bRet;
}

//�Ѷ���ʾ
bool WINAPI CTWSDNetPay_DllApp::IF_EventShowFeeInfo(IN const char* szParamContext,IN const int& iParamSize,IN const int& iFormatType)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	bool bRet = false;

	CString strLog;
    RecordLog(FmtStr(_T("[ETC����ҵ��],�Ѷ���ʾ�ӿ�(IF_EventShowFeeInfo)���ÿ�ʼ,����:%s"),szParamContext));
	CString ErrorDescribe;
	if(!CheckEventShowFeeInfoJsonValidity(szParamContext, ErrorDescribe))
	{
		m_strEtcEventLastErrorDesc = ErrorDescribe;
		return bRet;
	}

    try
    {
        //�ж�����Ƿ��Ѿ�����ʼ��
        if (!m_bIsInitEnvironment)
            throw - 1;
        //�ж�����Ƿ�ɹ���Ȩ
        if (!m_bAuthorizedLegal)
            throw - 2;
        //�ж�ҵ�����߳��Ƿ��쳣
        if (NULL == m_pThreadManage)
            throw - 3;
        //�ж�Ӳ�������߳��Ƿ�����
        if (NULL == m_pThreadManage->m_pThreadDevice)
            throw - 4;

        bRet = m_pThreadManage->m_pThreadDevice->EtcEventShowFeeInfo(szParamContext, iParamSize);
		if(bRet == false)
			m_strEtcEventLastErrorDesc.Format(_T("���ʹ�������ʧ�ܣ�"));
    }
    catch (int& iErrorCode)
    {
        switch (iErrorCode)
        {
        case -1:
            strLog.Format(_T("[ETC����ҵ��],�Ѷ���ʾ�ӿ�(IF_EventShowFeeInfo)����,���δ�ɹ���ʼ��"));
            break;
        case -2:
            strLog.Format(_T("[ETC����ҵ��],�Ѷ���ʾ�ӿ�(IF_EventShowFeeInfo)����,���δ����Ȩ"));
            break;
        case -3:
        case -4:
            strLog.Format(_T("[ETC����ҵ��],�Ѷ���ʾ�ӿ�(IF_EventShowFeeInfo)����,����ڲ���Դ�쳣"));
            break;
        }
        //�����������
        if (strLog != m_strEtcEventLastErrorDesc)
        {
            m_strEtcEventLastErrorDesc = strLog;
        }
    }

	return bRet;
}

//֧��״̬��ʾ
bool WINAPI CTWSDNetPay_DllApp::IF_EventPayResultDisplay(IN const char* szParamContext,IN const int& iParamSize,IN const int& iFormatType)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	bool bRet = false;

	CString strLog;
    RecordLog(FmtStr(_T("[ETC����ҵ��],֧�����չʾ�ӿ�(IF_EventPayResultDisplay)���ÿ�ʼ,����:%s"),szParamContext));
	CString ErrorDescribe;
	if(!CheckEventPayResultShowJsonValidity(szParamContext, ErrorDescribe))
	{
		m_strEtcEventLastErrorDesc = ErrorDescribe;
		return bRet;
	}

    try
    {
        //�ж�����Ƿ��Ѿ�����ʼ��
        if (!m_bIsInitEnvironment)
            throw - 1;
        //�ж�����Ƿ�ɹ���Ȩ
        if (!m_bAuthorizedLegal)
            throw - 2;
        //�ж�ҵ�����߳��Ƿ��쳣
        if (NULL == m_pThreadManage)
            throw - 3;
        //�ж�Ӳ�������߳��Ƿ�����
        if (NULL == m_pThreadManage->m_pThreadDevice)
            throw - 4;

        bRet = m_pThreadManage->m_pThreadDevice->EtcEventPayResultDisplay(szParamContext, iParamSize);
		if(bRet == false)
			m_strEtcEventLastErrorDesc.Format(_T("���ʹ�������ʧ�ܣ�"));
    }
    catch (int& iErrorCode)
    {
        switch (iErrorCode)
        {
        case -1:
            strLog.Format(_T("[ETC����ҵ��],֧�����չʾ�ӿ�(IF_EventPayResultDisplay)����,���δ�ɹ���ʼ��"));
            break;
        case -2:
            strLog.Format(_T("[ETC����ҵ��],֧�����չʾ�ӿ�(IF_EventPayResultDisplay)����,���δ����Ȩ"));
            break;
        case -3:
        case -4:
            strLog.Format(_T("[ETC����ҵ��],֧�����չʾ�ӿ�(IF_EventPayResultDisplay)����,����ڲ���Դ�쳣"));
            break;
        }
        //�����������
        if (strLog != m_strEtcEventLastErrorDesc)
        {
            m_strEtcEventLastErrorDesc = strLog;
        }
    }

	return bRet;
}

//���鴦�����
bool WINAPI CTWSDNetPay_DllApp::IF_EventDealStop(IN const char* szParamContext,IN const int& iParamSize,IN const int& iFormatType)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	bool bRet = false;

	CString strLog;
    RecordLog(FmtStr(_T("[ETC����ҵ��],���鴦������ӿ�(IF_EventDealStop)���ÿ�ʼ,����:%s"),szParamContext));
	CString ErrorDescribe;
	if(!CheckEventDealStopJsonValidity(szParamContext, ErrorDescribe))
	{
		m_strEtcEventLastErrorDesc = ErrorDescribe;
		return bRet;
	}

    try
    {
        //�ж�����Ƿ��Ѿ�����ʼ��
        if (!m_bIsInitEnvironment)
            throw - 1;
        //�ж�����Ƿ�ɹ���Ȩ
        if (!m_bAuthorizedLegal)
            throw - 2;
        //�ж�ҵ�����߳��Ƿ��쳣
        if (NULL == m_pThreadManage)
            throw - 3;
        //�ж�Ӳ�������߳��Ƿ�����
        if (NULL == m_pThreadManage->m_pThreadDevice)
            throw - 4;

        bRet = m_pThreadManage->m_pThreadDevice->EtcEventDealStop(szParamContext, iParamSize);
		if(bRet == false)
			m_strEtcEventLastErrorDesc.Format(_T("���ʹ�������ʧ�ܣ�"));

		if(m_bEnableMediaGateWay)
		{
			m_pThreadManage->MediaStop();
		}
    }
    catch (int& iErrorCode)
    {
        switch (iErrorCode)
        {
        case -1:
            strLog.Format(_T("[ETC����ҵ��],���鴦������ӿ�(IF_EventDealStop)����,���δ�ɹ���ʼ��"));
            break;
        case -2:
            strLog.Format(_T("[ETC����ҵ��],���鴦������ӿ�(IF_EventDealStop)����,���δ����Ȩ"));
            break;
        case -3:
        case -4:
            strLog.Format(_T("[ETC����ҵ��],���鴦������ӿ�(IF_EventDealStop)����,����ڲ���Դ�쳣"));
            break;
        }
        //�����������
        if (strLog != m_strEtcEventLastErrorDesc)
        {
            m_strEtcEventLastErrorDesc = strLog;
        }
    }

	return bRet;
}

//����ɨ��
bool WINAPI CTWSDNetPay_DllApp::IF_EventStartScan(IN const int& iFeeValue)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	bool bRet = false;

	CString strLog;
    RecordLog(FmtStr(_T("[ETC����ҵ��],����ɨ��ӿ�(IF_EventStartScan)���ÿ�ʼ,����:%d"),iFeeValue));
    try
    {
        //�ж�����Ƿ��Ѿ�����ʼ��
        if (!m_bIsInitEnvironment)
            throw - 1;
        //�ж�����Ƿ�ɹ���Ȩ
        if (!m_bAuthorizedLegal)
            throw - 2;
        //�ж�ҵ�����߳��Ƿ��쳣
        if (NULL == m_pThreadManage)
            throw - 3;
        //�ж�Ӳ�������߳��Ƿ�����
        if (NULL == m_pThreadManage->m_pThreadDevice)
            throw - 4;

        bRet = m_pThreadManage->m_pThreadDevice->EtcEventStartScan();
		if(bRet == false)
			m_strEtcEventLastErrorDesc.Format(_T("���ʹ�������ʧ�ܣ�"));
    }
    catch (int& iErrorCode)
    {
        switch (iErrorCode)
        {
        case -1:
            strLog.Format(_T("[ETC����ҵ��],����ɨ��ӿ�(IF_EventStartScan)����,���δ�ɹ���ʼ��"));
            break;
        case -2:
            strLog.Format(_T("[ETC����ҵ��],����ɨ��ӿ�(IF_EventStartScan)����,���δ����Ȩ"));
            break;
        case -3:
        case -4:
            strLog.Format(_T("[ETC����ҵ��],����ɨ��ӿ�(IF_EventStartScan)����,����ڲ���Դ�쳣"));
            break;
        }
        //�����������
        if (strLog != m_strEtcEventLastErrorDesc)
        {
            m_strEtcEventLastErrorDesc = strLog;
        }
    }

	return bRet;
}

//ֹͣɨ��
bool WINAPI CTWSDNetPay_DllApp::IF_EventStopScan()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	bool bRet = false;

	CString strLog;
    RecordLog(FmtStr(_T("[ETC����ҵ��],ֹͣɨ��ӿ�(IF_EventStopScan)���ÿ�ʼ")));
    try
    {
        //�ж�����Ƿ��Ѿ�����ʼ��
        if (!m_bIsInitEnvironment)
            throw - 1;
        //�ж�����Ƿ�ɹ���Ȩ
        if (!m_bAuthorizedLegal)
            throw - 2;
        //�ж�ҵ�����߳��Ƿ��쳣
        if (NULL == m_pThreadManage)
            throw - 3;
        //�ж�Ӳ�������߳��Ƿ�����
        if (NULL == m_pThreadManage->m_pThreadDevice)
            throw - 4;

        bRet = m_pThreadManage->m_pThreadDevice->EtcEventStopScan();
		if(bRet == false)
			m_strEtcEventLastErrorDesc.Format(_T("���ʹ�������ʧ�ܣ�"));
    }
    catch (int& iErrorCode)
    {
        switch (iErrorCode)
        {
        case -1:
            strLog.Format(_T("[ETC����ҵ��],ֹͣɨ��ӿ�(IF_EventStopScan)����,���δ�ɹ���ʼ��"));
            break;
        case -2:
            strLog.Format(_T("[ETC����ҵ��],ֹͣɨ��ӿ�(IF_EventStopScan)����,���δ����Ȩ"));
            break;
        case -3:
        case -4:
            strLog.Format(_T("[ETC����ҵ��],ֹͣɨ��ӿ�(IF_EventStopScan)����,����ڲ���Դ�쳣"));
            break;
        }
        //�����������
        if (strLog != m_strEtcEventLastErrorDesc)
        {
            m_strEtcEventLastErrorDesc = strLog;
        }
    }

	return bRet;
}

//ETC��������ʾ
bool WINAPI CTWSDNetPay_DllApp::IF_EventCardOperationNotify(IN const char* szParamContext, IN const int& iParamSize, IN const int& iFormatType)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	bool bRet = false;
	
	CString strLog;
    RecordLog(FmtStr(_T("[ETC����ҵ��],��������ʾ�ӿ�(IF_EventCardOperationNotify)���ÿ�ʼ,����:%s"),szParamContext));
	CString ErrorDescribe;
	int NotifyType;
	if(!CheckCardOperationNotifyJsonValidity(szParamContext, ErrorDescribe, NotifyType))
	{
		m_strEtcEventLastErrorDesc = ErrorDescribe;
		return bRet;
	}

    try
    {
        //�ж�����Ƿ��Ѿ�����ʼ��
        if (!m_bIsInitEnvironment)
            throw - 1;
        //�ж�����Ƿ�ɹ���Ȩ
        if (!m_bAuthorizedLegal)
            throw - 2;
        //�ж�ҵ�����߳��Ƿ��쳣
        if (NULL == m_pThreadManage)
            throw - 3;
        //�ж�Ӳ�������߳��Ƿ�����
        if (NULL == m_pThreadManage->m_pThreadDevice)
            throw - 4;

		if(NotifyType == 98)
		{
			RecordLog("[ETC����ҵ��],��������ʾ�ӿ�IF_EventCardOperationNotify, ����ģʽ�л�����MTC�˹�ģʽ��������");
			m_iCurrentWorkMode = 1;
			return true;
		}
		if(NotifyType == 99)
		{
			RecordLog("[ETC����ҵ��],��������ʾ�ӿ�IF_EventCardOperationNotify, ����ģʽ�л�����ETC��������ģʽ��������");
			m_iCurrentWorkMode = 0;
			return true;
		}		

        bRet = m_pThreadManage->m_pThreadDevice->EtcEventCardOperationNotify(szParamContext, iParamSize);
		if(bRet == false)
			m_strEtcEventLastErrorDesc.Format(_T("���ʹ�������ʧ�ܣ�"));
    }
    catch (int& iErrorCode)
    {
        switch (iErrorCode)
        {
        case -1:
            strLog.Format(_T("[ETC����ҵ��],��������ʾ�ӿ�(IF_EventCardOperationNotify)����,���δ�ɹ���ʼ��"));
            break;
        case -2:
            strLog.Format(_T("[ETC����ҵ��],��������ʾ�ӿ�(IF_EventCardOperationNotify)����,���δ����Ȩ"));
            break;
        case -3:
        case -4:
            strLog.Format(_T("[ETC����ҵ��],��������ʾ�ӿ�(IF_EventCardOperationNotify)����,����ڲ���Դ�쳣"));
            break;
        }
        //�����������
        if (strLog != m_strEtcEventLastErrorDesc)
        {
            m_strEtcEventLastErrorDesc = strLog;
        }
    }

	return bRet;
}

//���������޸�
bool WINAPI CTWSDNetPay_DllApp::IF_EventModifyVehQueue(IN const char* szParamContext, IN const int& iParamSize, IN const int& iFormatType)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	bool bRet = false;

	CString strLog;
    RecordLog(FmtStr(_T("[ETC����ҵ��],�����������нӿ�(IF_EventModifyVehQueue)���ÿ�ʼ,����:%s"),szParamContext));
	CString ErrorDescribe;
	if(!CheckEventModifyVehQueueJsonValidity(szParamContext, ErrorDescribe))
	{
		m_strEtcEventLastErrorDesc = ErrorDescribe;
		return bRet;
	}

    try
    {
        //�ж�����Ƿ��Ѿ�����ʼ��
        if (!m_bIsInitEnvironment)
            throw - 1;
        //�ж�����Ƿ�ɹ���Ȩ
        if (!m_bAuthorizedLegal)
            throw - 2;
        //�ж�ҵ�����߳��Ƿ��쳣
        if (NULL == m_pThreadManage)
            throw - 3;
        //�ж�Ӳ�������߳��Ƿ�����
        if (NULL == m_pThreadManage->m_pThreadDevice)
            throw - 4;

        bRet = m_pThreadManage->m_pThreadDevice->EtcEventModifyVehQueue(szParamContext, iParamSize);
		if(bRet == false)
			m_strEtcEventLastErrorDesc.Format(_T("���ʹ�������ʧ�ܣ�"));
    }
    catch (int& iErrorCode)
    {
        switch (iErrorCode)
        {
        case -1:
            strLog.Format(_T("[ETC����ҵ��],�����������нӿ�(IF_EventModifyVehQueue)����,���δ�ɹ���ʼ��"));
            break;
        case -2:
            strLog.Format(_T("[ETC����ҵ��],�����������нӿ�(IF_EventModifyVehQueue)����,���δ����Ȩ"));
            break;
        case -3:
        case -4:
            strLog.Format(_T("[ETC����ҵ��],�����������нӿ�(IF_EventModifyVehQueue)����,����ڲ���Դ�쳣"));
            break;
        }
        //�����������
        if (strLog != m_strEtcEventLastErrorDesc)
        {
            m_strEtcEventLastErrorDesc = strLog;
        }
    }

	return bRet;
}

//��С�Ѷ���Ȩ
bool WINAPI CTWSDNetPay_DllApp::IF_EventFeeAuthorize(IN const char* szParamContext, IN const int& iParamSize, IN const int& iFormatType)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	bool bRet = false;
	
	CString strLog;
    RecordLog(FmtStr(_T("[ETC����ҵ��],��С�Ѷ���Ȩ�ӿ�(IF_EventFeeAuthorize)���ÿ�ʼ,����:%s"),szParamContext));

    try
    {
        //�ж�����Ƿ��Ѿ�����ʼ��
        if (!m_bIsInitEnvironment)
            throw - 1;
        //�ж�����Ƿ�ɹ���Ȩ
        if (!m_bAuthorizedLegal)
            throw - 2;
        //�ж�ҵ�����߳��Ƿ��쳣
        if (NULL == m_pThreadManage)
            throw - 3;
        //�ж�Ӳ�������߳��Ƿ�����
        if (NULL == m_pThreadManage->m_pThreadDevice)
            throw - 4;

        bRet = m_pThreadManage->m_pThreadDevice->EtcEventFeeAuthorize(szParamContext, iParamSize);
		if(bRet == false)
			m_strEtcEventLastErrorDesc.Format(_T("���ʹ�������ʧ�ܣ�"));
    }
    catch (int& iErrorCode)
    {
        switch (iErrorCode)
        {
        case -1:
            strLog.Format(_T("[ETC����ҵ��],��С�Ѷ���Ȩ�ӿ�(IF_EventFeeAuthorize)����,���δ�ɹ���ʼ��"));
            break;
        case -2:
            strLog.Format(_T("[ETC����ҵ��],��С�Ѷ���Ȩ�ӿ�(IF_EventFeeAuthorize)����,���δ����Ȩ"));
            break;
        case -3:
        case -4:
            strLog.Format(_T("[ETC����ҵ��],��С�Ѷ���Ȩ�ӿ�(IF_EventFeeAuthorize)����,����ڲ���Դ�쳣"));
            break;
        }
        //�����������
        if (strLog != m_strEtcEventLastErrorDesc)
        {
            m_strEtcEventLastErrorDesc = strLog;
        }
    }

	return bRet;
}


//������������
bool WINAPI CTWSDNetPay_DllApp::IF_EventFreeVoicePlay(IN const char* szParamContext,IN const int& iParamSize,IN const int& iFormatType)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	bool bRet = false;

	CString strLog;
	RecordLog(FmtStr(_T("[ETC����ҵ��],�������������ӿ�(IF_EventFreeVoicePlay)���ÿ�ʼ,����:%s"),szParamContext));

	try
	{
		//�ж�����Ƿ��Ѿ�����ʼ��
		if (!m_bIsInitEnvironment)
			throw - 1;
		//�ж�����Ƿ�ɹ���Ȩ
		if (!m_bAuthorizedLegal)
			throw - 2;
		//�ж�ҵ�����߳��Ƿ��쳣
		if (NULL == m_pThreadManage)
			throw - 3;
		//�ж�Ӳ�������߳��Ƿ�����
		if (NULL == m_pThreadManage->m_pThreadDevice)
			throw - 4;

		bRet = m_pThreadManage->m_pThreadDevice->EtcEventFreeVoicePlay(szParamContext, iParamSize);
		if(bRet == false)
			m_strEtcEventLastErrorDesc.Format(_T("���ʹ�������ʧ�ܣ�"));
	}
	catch (int& iErrorCode)
	{
		switch (iErrorCode)
		{
		case -1:
			strLog.Format(_T("[ETC����ҵ��],��������ʾ�ӿ�(IF_EventFreeVoicePlay)����,���δ�ɹ���ʼ��"));
			break;
		case -2:
			strLog.Format(_T("[ETC����ҵ��],��������ʾ�ӿ�(IF_EventFreeVoicePlay)����,���δ����Ȩ"));
			break;
		case -3:
		case -4:
			strLog.Format(_T("[ETC����ҵ��],��������ʾ�ӿ�(IF_EventFreeVoicePlay)����,����ڲ���Դ�쳣"));
			break;
		}
		//�����������
		if (strLog != m_strEtcEventLastErrorDesc)
		{
			m_strEtcEventLastErrorDesc = strLog;
		}
	}

	return bRet;
}


// ������Ȩ�ӿڣ���С�Ѷ���⣩
bool WINAPI CTWSDNetPay_DllApp::IF_EventAuthorize(IN const char* szParamContext,
					   IN const int& iParamSize,IN const int& iFormatType)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	bool bRet = false;

	CString strLog;
	RecordLog(FmtStr(_T("[ETC����ҵ��], ������Ȩ�ӿ�(IF_EventAuthorize)���ÿ�ʼ,���ȣ�%d,����:%s"),iParamSize,szParamContext));

	CString ErrorDescribe;
	if(!CheckAuthorize(szParamContext, ErrorDescribe))
	{
		m_strEtcEventLastErrorDesc = ErrorDescribe;
		return bRet;
	}

	try
	{
		//�ж�����Ƿ��Ѿ�����ʼ��
		if (!m_bIsInitEnvironment)
			throw - 1;
		//�ж�����Ƿ�ɹ���Ȩ
		if (!m_bAuthorizedLegal)
			throw - 2;
		//�ж�ҵ�����߳��Ƿ��쳣
		if (NULL == m_pThreadManage)
			throw - 3;
		//�ж�Ӳ�������߳��Ƿ�����
		if (NULL == m_pThreadManage->m_pThreadDevice)
			throw - 4;

		bRet = m_pThreadManage->m_pThreadDevice->EtcEventAuthorize(szParamContext, iParamSize);
		if(bRet == false)
			m_strEtcEventLastErrorDesc.Format(_T("���ʹ�������ʧ�ܣ�"));
	}
	catch (int& iErrorCode)
	{
		switch (iErrorCode)
		{
		case -1:
			strLog.Format(_T("[ETC����ҵ��],������Ȩ�ӿ�(IF_EventAuthorize)����,���δ�ɹ���ʼ��"));
			break;
		case -2:
			strLog.Format(_T("[ETC����ҵ��],������Ȩ�ӿ�(IF_EventAuthorize)����,���δ����Ȩ"));
			break;
		case -3:
		case -4:
			strLog.Format(_T("[ETC����ҵ��],������Ȩ�ӿ�(IF_EventAuthorize)����,����ڲ���Դ�쳣"));
			break;
		}
		//�����������
		if (strLog != m_strEtcEventLastErrorDesc)
		{
			m_strEtcEventLastErrorDesc = strLog;
		}
	}

	return bRet;
}

//��������ɾ�����
bool WINAPI CTWSDNetPay_DllApp::IF_EventDelVehQueueResult(IN const char* szParamContext, IN const int& iParamSize, IN const int& iFormatType)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	bool bRet = false;

	CString strLog;
	RecordLog(FmtStr(_T("[ETC����ҵ��],ɾ�����н���ӿ�(IF_EventDelVehQueueResult)���ÿ�ʼ,���ȣ�%d,����:%s"), iParamSize, szParamContext));

	CString ErrorDescribe;
	if(!CheckEventDelVehQueueResult(szParamContext, ErrorDescribe))
	{
		m_strEtcEventLastErrorDesc = ErrorDescribe;
		return bRet;
	}

	try
	{
		//�ж�����Ƿ��Ѿ�����ʼ��
		if (!m_bIsInitEnvironment)
			throw - 1;
		//�ж�����Ƿ�ɹ���Ȩ
		if (!m_bAuthorizedLegal)
			throw - 2;
		//�ж�ҵ�����߳��Ƿ��쳣
		if (NULL == m_pThreadManage)
			throw - 3;
		//�ж�Ӳ�������߳��Ƿ�����
		if (NULL == m_pThreadManage->m_pThreadDevice)
			throw - 4;

		bRet = m_pThreadManage->m_pThreadDevice->EtcEventDelVehQueueResult(szParamContext, iParamSize);
		if(bRet == false)
			m_strEtcEventLastErrorDesc.Format(_T("���ʹ�������ʧ�ܣ�"));
	}
	catch (int& iErrorCode)
	{
		switch (iErrorCode)
		{
		case -1:
			strLog.Format(_T("[ETC����ҵ��],ɾ�����н���ӿ�(IF_EventDelVehQueueResult)����,���δ�ɹ���ʼ��"));
			break;
		case -2:
			strLog.Format(_T("[ETC����ҵ��],ɾ�����н���ӿ�(IF_EventDelVehQueueResult)����,���δ����Ȩ"));
			break;
		case -3:
		case -4:
			strLog.Format(_T("[ETC����ҵ��],ɾ�����н���ӿ�(IF_EventDelVehQueueResult)����,����ڲ���Դ�쳣"));
			break;
		}
		//�����������
		if (strLog != m_strEtcEventLastErrorDesc)
		{
			m_strEtcEventLastErrorDesc = strLog;
		}
	}

	return bRet;
}



std::string GetProviceNameByID(unsigned short id)
{
	std::string NameStr = std::string("�㶫");
	switch(id)
	{
	case 11: NameStr = std::string("����");break;	
	case 12: NameStr = std::string("���");break;
	case 13: NameStr = std::string("�ӱ�");break;
	case 14: NameStr = std::string("ɽ��");break;	
	case 15: NameStr = std::string("���ɹ�");break;
	case 21: NameStr = std::string("����");break;
	case 22: NameStr = std::string("����");break;	
	case 23: NameStr = std::string("������");break;
	case 31: NameStr = std::string("�Ϻ�");break;
	case 32: NameStr = std::string("����");break;	
	case 33: NameStr = std::string("�㽭");break;
	case 34: NameStr = std::string("����");break;
	case 35: NameStr = std::string("����");break;	
	case 36: NameStr = std::string("����");break;
	case 37: NameStr = std::string("ɽ��");break;
	case 41: NameStr = std::string("����");break;	
	case 42: NameStr = std::string("����");break;
	case 43: NameStr = std::string("����");break;
	case 44: NameStr = std::string("�㶫");break;	
	case 45: NameStr = std::string("����");break;
	case 46: NameStr = std::string("����");break;
	case 50: NameStr = std::string("����");break;
	case 51: NameStr = std::string("�Ĵ�");break;
	case 52: NameStr = std::string("����");break;
	case 53: NameStr = std::string("����");break;
	case 54: NameStr = std::string("����");break;
	case 61: NameStr = std::string("����");break;
	case 62: NameStr = std::string("����");break;
	case 63: NameStr = std::string("�ຣ");break;
	case 64: NameStr = std::string("����");break;
	case 65: NameStr = std::string("�½�");break;
	case 71: NameStr = std::string("̨��");break;
	case 81: NameStr = std::string("���");break;
	case 82: NameStr = std::string("����");break;
	}

	return NameStr;
}


std::string CTWSDNetPay_DllApp::GenerateEtcEventInitData()
{
	Json::Value root;
    Json::Value DeviceParamContext;
    Json::Value Data;

	Data["ProvinceID"]          = Json::Value(theApp.m_EtcProvinceId);
	Data["AreaID"]              = Json::Value(theApp.m_EtcAreaId);
	Data["RoadID"]              = Json::Value(theApp.m_EtcRoadId);
	Data["StationID"]           = Json::Value(theApp.m_EtcStationId);
	Data["LaneID"]              = Json::Value(theApp.m_EtcLaneId);
	
    Data["ServerType"]          = Json::Value(0);
    Data["ServerIP"]            = Json::Value(theApp.m_EtcServerIP);
    Data["ServerPort"]          = Json::Value(theApp.m_EtcServerPort);
    Data["DevicePayMode"]       = Json::Value(3);
    Data["CurrentTime"]         = Json::Value(CTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S"));
    Data["UpdatePort"]          = Json::Value(theApp.m_EtcServerUpdatePort);
    Data["ReturnPayChannelCodeUnify"] = Json::Value(1);
	
	std::string ProvinceName = GetProviceNameByID(theApp.m_EtcProvinceId);
	Data["ProvinceName"]    = Json::Value(ProvinceName);
	CString strProvinceNo;
	strProvinceNo.Format(_T("%d"), theApp.m_EtcProvinceId);
	Data["ProvinceNo"]      = Json::Value(strProvinceNo);
	Data["RoadName"]        = Json::Value(theApp.m_EtcRoadName);
	Data["RoadNo"]          = Json::Value(theApp.m_EtcRoadId);
	Data["StationNo"]       = Json::Value(theApp.m_EtcStationId);
	Data["StationName"]     = Json::Value(theApp.m_EtcStationName);
	Data["LaneNo"]          = Json::Value(theApp.m_EtcLaneId);

    // DeviceParamContext["Data"]      = Data;
    // DeviceParamContext["Version"]   = Json::Value(_T("1.0"));
    // root["DeviceParamContext"]      = DeviceParamContext;

    Json::FastWriter fast_writer;
    std::string json_data = fast_writer.write(Data);

    return json_data;
}


bool CTWSDNetPay_DllApp::CheckEventDealStartJsonValidity(const char* szParamContext, CString& ErrorDescribe)
{
	//������������У��
	Json::Reader reader;
	Json::Value  root;
	Json::Value  EventDealStart;
	Json::Value  Data;	

	if(!reader.parse(szParamContext, root))
	{
		ErrorDescribe = "�����¼���ʼ����: Json��ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(root["EventDealStart"].type() != Json::objectValue)
	{
		ErrorDescribe = "�����¼���ʼ����: EventDealStart�ֶθ�ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}

	EventDealStart = root["EventDealStart"];	
	if(EventDealStart["SpecialEventNo"].type() == Json::nullValue || EventDealStart["SpecialEventNo"].type() != Json::stringValue)
	{
		ErrorDescribe = "�����¼���ʼ����: SpecialEventNo�ֶθ�ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}
	theApp.m_EtcEventNo = EventDealStart["SpecialEventNo"].asString();
	if(EventDealStart["MessageCode"].type() == Json::nullValue || EventDealStart["MessageCode"].type() != Json::stringValue)
	{
		ErrorDescribe = "�����¼���ʼ����: MessageCode�ֶθ�ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(EventDealStart["Data"].type() != Json::objectValue)
	{
		ErrorDescribe = "�����¼���ʼ����: Data�ֶθ�ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}

	Data = EventDealStart["Data"];	
	if(Data["VehPlate"].type() == Json::nullValue || Data["VehPlate"].type() != Json::stringValue)
	{
		ErrorDescribe = "�����¼���ʼ����: VehPlate�ֶθ�ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(Data["VehPlateColor"].type() == Json::nullValue || Data["VehPlateColor"].type() != Json::intValue)
	{
		ErrorDescribe = "�����¼���ʼ����: VehPlateColor�ֶθ�ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(Data["VehClass"].type() == Json::nullValue || Data["VehClass"].type() != Json::intValue)
	{
		ErrorDescribe = "�����¼���ʼ����: VehClass�ֶθ�ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(Data["VehType"].type() == Json::nullValue || Data["VehType"].type() != Json::intValue)
	{
		ErrorDescribe = "�����¼���ʼ����: VehType�ֶθ�ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(Data["VehSeatCount"].type() == Json::nullValue || Data["VehSeatCount"].type() != Json::intValue)
	{
		ErrorDescribe = "�����¼���ʼ����: VehSeatCount�ֶθ�ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(Data["axleCount"].type() == Json::nullValue || Data["axleCount"].type() != Json::intValue)
	{
		ErrorDescribe = "�����¼���ʼ����: axleCount�ֶθ�ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(Data["CardNum"].type() == Json::nullValue || Data["CardNum"].type() != Json::stringValue)
	{
		ErrorDescribe = "�����¼���ʼ����: CardNum�ֶθ�ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(Data["EventStartTime"].type() == Json::nullValue || Data["EventStartTime"].type() != Json::stringValue)
	{
		ErrorDescribe = "�����¼���ʼ����: EventStartTime�ֶθ�ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(Data["EventDescribe"].type() == Json::nullValue || Data["EventDescribe"].type() != Json::stringValue)
	{
		ErrorDescribe = "�����¼���ʼ����: EventDescribe�ֶθ�ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}	

	return true;
}

bool CTWSDNetPay_DllApp::CheckEventCheckVehQueueJsonValidity(const char* szParamContext, CString& ErrorDescribe)
{
	//������������У��
	Json::Reader reader;
	Json::Value  root;
	Json::Value  EventCheckVehQueue;
	Json::Value  Data;	

	if(!reader.parse(szParamContext, root))
	{
		ErrorDescribe = "ȷ�ϳ������в���: Json��ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(root["EventCheckVehQueue"].type() != Json::objectValue)
	{
		ErrorDescribe = "ȷ�ϳ������в���: EventDealStart�ֶθ�ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}

	EventCheckVehQueue = root["EventCheckVehQueue"];	
	if(EventCheckVehQueue["SpecialEventNo"].type() == Json::nullValue || EventCheckVehQueue["SpecialEventNo"].type() != Json::stringValue)
	{
		ErrorDescribe = "ȷ�ϳ������в���: SpecialEventNo�ֶθ�ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(EventCheckVehQueue["MessageCode"].type() == Json::nullValue || EventCheckVehQueue["MessageCode"].type() != Json::stringValue)
	{
		ErrorDescribe = "ȷ�ϳ������в���: MessageCode�ֶθ�ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(EventCheckVehQueue["Data"].type() != Json::objectValue)
	{
		ErrorDescribe = "ȷ�ϳ������в���: Data�ֶθ�ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}

	Data = EventCheckVehQueue["Data"];
	
	if(Data["VehPlate"].type() == Json::nullValue || Data["VehPlate"].type() != Json::stringValue)
	{
		ErrorDescribe = "ȷ�ϳ������в���: VehPlate�ֶθ�ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(Data["VehPlateColor"].type() == Json::nullValue || Data["VehPlateColor"].type() != Json::intValue)
	{
		ErrorDescribe = "ȷ�ϳ������в���: VehPlateColor�ֶθ�ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(Data["EventStartTime"].type() == Json::nullValue || Data["EventStartTime"].type() != Json::stringValue)
	{
		ErrorDescribe = "ȷ�ϳ������в���: EventStartTime�ֶθ�ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(Data["EventDescribe"].type() == Json::nullValue || Data["EventDescribe"].type() != Json::stringValue)
	{
		ErrorDescribe = "ȷ�ϳ������в���: EventDescribe�ֶθ�ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}	

	return true;
}

bool CTWSDNetPay_DllApp::CheckEventShowFeeInfoJsonValidity(const char* szParamContext, CString& ErrorDescribe)
{
	//������������У��
	Json::Reader reader;
	Json::Value  root;
	Json::Value  EventShowFeeInfo;
	Json::Value  FeeInfo;

	if(!reader.parse(szParamContext, root))
	{
		ErrorDescribe = "�Ѷ���ʾ����: Json��ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(root["EventShowFeeInfo"].type() != Json::objectValue)
	{
		ErrorDescribe = "�Ѷ���ʾ����: EventShowFeeInfo�ֶθ�ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}

	EventShowFeeInfo = root["EventShowFeeInfo"];	
	if(EventShowFeeInfo["SpecialEventNo"].type() == Json::nullValue || EventShowFeeInfo["SpecialEventNo"].type() != Json::stringValue)
	{
		ErrorDescribe = "�Ѷ���ʾ����: SpecialEventNo�ֶθ�ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(EventShowFeeInfo["MessageCode"].type() == Json::nullValue || EventShowFeeInfo["MessageCode"].type() != Json::stringValue)
	{
		ErrorDescribe = "�Ѷ���ʾ����: MessageCode�ֶθ�ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}

	if(EventShowFeeInfo["PayType"].type() == Json::nullValue || EventShowFeeInfo["PayType"].type() != Json::intValue)
	{
		ErrorDescribe = "�Ѷ���ʾ����: PayType�ֶθ�ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(EventShowFeeInfo["DefaultFeeItemNo"].type() == Json::nullValue || EventShowFeeInfo["DefaultFeeItemNo"].type() != Json::intValue)
	{
		ErrorDescribe = "�Ѷ���ʾ����: DefaultFeeItemNo�ֶθ�ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(EventShowFeeInfo["FeeCount"].type() == Json::nullValue || EventShowFeeInfo["FeeCount"].type() != Json::intValue)
	{
		ErrorDescribe = "�Ѷ���ʾ����: FeeCount�ֶθ�ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(EventShowFeeInfo["FeeInfo"].type() == Json::nullValue || EventShowFeeInfo["FeeInfo"].type() != Json::arrayValue)
	{
		ErrorDescribe = "�Ѷ���ʾ����: FeeInfo�ֶθ�ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}

	return true;
}

bool CTWSDNetPay_DllApp::CheckEventPayResultShowJsonValidity(const char* szParamContext, CString& ErrorDescribe)
{
	//������������У��
	Json::Reader reader;
	Json::Value  root;
	Json::Value  EventPayResultShow;
	Json::Value  EventInfo;

	if(!reader.parse(szParamContext, root))
	{
		ErrorDescribe = "չʾ֧��״̬: Json��ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(root["EventPayResultShow"].type() != Json::objectValue)
	{
		ErrorDescribe = "չʾ֧��״̬: EventPayResultShow�ֶθ�ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}

	EventPayResultShow = root["EventPayResultShow"];	
	if(EventPayResultShow["SpecialEventNo"].type() == Json::nullValue || EventPayResultShow["SpecialEventNo"].type() != Json::stringValue)
	{
		ErrorDescribe = "չʾ֧��״̬: SpecialEventNo�ֶθ�ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(EventPayResultShow["MessageCode"].type() == Json::nullValue || EventPayResultShow["MessageCode"].type() != Json::stringValue)
	{
		ErrorDescribe = "չʾ֧��״̬: MessageCode�ֶθ�ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}

	EventInfo = EventPayResultShow["EventInfo"];
	if(EventInfo["RequestTime"].type() == Json::nullValue || EventInfo["RequestTime"].type() != Json::stringValue)
	{
		ErrorDescribe = "չʾ֧��״̬: RequestTime�ֶθ�ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(EventInfo["RequestType"].type() == Json::nullValue || EventInfo["RequestType"].type() != Json::stringValue)
	{
		ErrorDescribe = "չʾ֧��״̬: RequestType�ֶθ�ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(EventInfo["PayResult"].type() == Json::nullValue || EventInfo["PayResult"].type() != Json::intValue)
	{
		ErrorDescribe = "չʾ֧��״̬: PayResult�ֶθ�ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(EventInfo["Describe"].type() == Json::nullValue || EventInfo["Describe"].type() != Json::stringValue)
	{
		ErrorDescribe = "չʾ֧��״̬: Describe�ֶθ�ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(EventInfo["TimeLimit"].type() == Json::nullValue || EventInfo["TimeLimit"].type() != Json::stringValue)
	{
		ErrorDescribe = "չʾ֧��״̬: TimeLimit�ֶθ�ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}

	return true;
}

bool CTWSDNetPay_DllApp::CheckEventDealStopJsonValidity(const char* szParamContext, CString& ErrorDescribe)
{
	//������������У��
	Json::Reader reader;
	Json::Value  root;
	Json::Value  EventDealStop;
	Json::Value  EventEndCases;

	if(!reader.parse(szParamContext, root))
	{
		ErrorDescribe = "�������鴦��: Json��ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(root["EventDealStop"].type() != Json::objectValue)
	{
		ErrorDescribe = "�������鴦��: EventDealStop�ֶθ�ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}

	EventDealStop = root["EventDealStop"];	
	if(EventDealStop["SpecialEventNo"].type() == Json::nullValue || EventDealStop["SpecialEventNo"].type() != Json::stringValue)
	{
		ErrorDescribe = "�������鴦��: SpecialEventNo�ֶθ�ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}
	theApp.m_EtcEventNo = EventDealStop["SpecialEventNo"].asString();

	if(EventDealStop["MessageCode"].type() == Json::nullValue || EventDealStop["MessageCode"].type() != Json::stringValue)
	{
		ErrorDescribe = "�������鴦��: MessageCode�ֶθ�ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}

	if(EventDealStop["EventEndCases"].type() != Json::objectValue)
	{
		ErrorDescribe = "�������鴦��: EventEndCases�ֶθ�ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}
	
	EventEndCases = EventDealStop["EventEndCases"];
	if(EventEndCases["EventVehPlate"].type() == Json::nullValue || EventEndCases["EventVehPlate"].type() != Json::stringValue)
	{
		ErrorDescribe = "�������鴦��: EventVehPlate�ֶθ�ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(EventEndCases["EventEndTime"].type() == Json::nullValue || EventEndCases["EventEndTime"].type() != Json::stringValue)
	{
		ErrorDescribe = "�������鴦��: EventEndTime�ֶθ�ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(EventEndCases["EventDescribe"].type() == Json::nullValue || EventEndCases["EventDescribe"].type() != Json::stringValue)
	{
		ErrorDescribe = "�������鴦��: EventDescribe�ֶθ�ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(EventEndCases["EventEndReason"].type() == Json::nullValue || EventEndCases["EventEndReason"].type() != Json::stringValue)
	{
		ErrorDescribe = "�������鴦��: EventEndReason�ֶθ�ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}

	return true;
}


bool CTWSDNetPay_DllApp::CheckEventCheckVehInfoJsonValidity(const char* szParamContext, CString& ErrorDescribe)
{
	//������������У��
	Json::Reader reader;
	Json::Value  root;
	Json::Value  EventCheckVehInfo;
	Json::Value  EventInfo;
	Json::Value  MMIContent;

	if(!reader.parse(szParamContext, root))
	{
		ErrorDescribe = "ȷ�ϳ�����Ϣ: Json��ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(root["EventCheckVehInfo"].type() != Json::objectValue)
	{
		ErrorDescribe = "ȷ�ϳ�����Ϣ: EventCheckVehInfo�ֶθ�ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}

	EventCheckVehInfo = root["EventCheckVehInfo"];	
	if(EventCheckVehInfo["SpecialEventNo"].type() == Json::nullValue || EventCheckVehInfo["SpecialEventNo"].type() != Json::stringValue)
	{
		ErrorDescribe = "ȷ�ϳ�����Ϣ: SpecialEventNo�ֶθ�ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(EventCheckVehInfo["MessageCode"].type() == Json::nullValue || EventCheckVehInfo["MessageCode"].type() != Json::stringValue)
	{
		ErrorDescribe = "ȷ�ϳ�����Ϣ: MessageCode�ֶθ�ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}

	EventInfo = EventCheckVehInfo["EventInfo"];
	if(EventInfo["VehPlateSnapImage"].type() == Json::nullValue || EventInfo["VehPlateSnapImage"].type() != Json::stringValue)
	{
		ErrorDescribe = "ȷ�ϳ�����Ϣ: VehPlateSnapImage�ֶθ�ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(EventInfo["CameraSnapImage"].type() == Json::nullValue || EventInfo["CameraSnapImage"].type() != Json::stringValue)
	{
		ErrorDescribe = "ȷ�ϳ�����Ϣ: CameraSnapImage�ֶθ�ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(EventInfo["RequestTime"].type() == Json::nullValue || EventInfo["RequestTime"].type() != Json::stringValue)
	{
		ErrorDescribe = "ȷ�ϳ�����Ϣ: RequestTime�ֶθ�ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(EventInfo["RequestType"].type() == Json::nullValue || EventInfo["RequestType"].type() != Json::stringValue)
	{
		ErrorDescribe = "ȷ�ϳ�����Ϣ: RequestType�ֶθ�ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(EventInfo["EventDescribe"].type() == Json::nullValue || EventInfo["EventDescribe"].type() != Json::stringValue)
	{
		ErrorDescribe = "ȷ�ϳ�����Ϣ: EventDescribe�ֶθ�ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(EventInfo["MMIDemand"].type() == Json::nullValue || EventInfo["MMIDemand"].type() != Json::stringValue)
	{
		ErrorDescribe = "ȷ�ϳ�����Ϣ: MMIDemand�ֶθ�ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(EventInfo["TimeLimit"].type() == Json::nullValue || EventInfo["TimeLimit"].type() != Json::stringValue)
	{
		ErrorDescribe = "ȷ�ϳ�����Ϣ: TimeLimit�ֶθ�ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}

	MMIContent = EventCheckVehInfo["MMIContent"];
	if(MMIContent["VehPlate"].type() == Json::nullValue || MMIContent["VehPlate"].type() != Json::stringValue)
	{
		ErrorDescribe = "ȷ�ϳ�����Ϣ: VehPlate�ֶθ�ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(MMIContent["VehPlateColor"].type() == Json::nullValue || MMIContent["VehPlateColor"].type() != Json::intValue)
	{
		ErrorDescribe = "ȷ�ϳ�����Ϣ: VehPlateColor�ֶθ�ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(MMIContent["VehClass"].type() == Json::nullValue || MMIContent["VehClass"].type() != Json::intValue)
	{
		ErrorDescribe = "ȷ�ϳ�����Ϣ: VehClass�ֶθ�ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(MMIContent["VehType"].type() == Json::nullValue || MMIContent["VehType"].type() != Json::intValue)
	{
		ErrorDescribe = "ȷ�ϳ�����Ϣ: VehType�ֶθ�ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(MMIContent["AxisCount"].type() == Json::nullValue || MMIContent["AxisCount"].type() != Json::intValue)
	{
		ErrorDescribe = "ȷ�ϳ�����Ϣ: AxisCount�ֶθ�ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(MMIContent["EntryTime"].type() == Json::nullValue || MMIContent["EntryTime"].type() != Json::stringValue)
	{
		ErrorDescribe = "ȷ�ϳ�����Ϣ: EntryTime�ֶθ�ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}

	return true;
}


bool CTWSDNetPay_DllApp::CheckEventCheckEntryInfoJsonValidity(const char* szParamContext, CString& ErrorDescribe)
{
	//������������У��
	Json::Reader reader;
	Json::Value  root;
	Json::Value  EventCheckEntryInfo;
	Json::Value  EventInfo;
	Json::Value  EntryInfo;

	if(!reader.parse(szParamContext, root))
	{
		ErrorDescribe = "ȷ�ϳ��������Ϣ: Json��ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(root["EventCheckEntryInfo"].type() != Json::objectValue)
	{
		ErrorDescribe = "ȷ�ϳ��������Ϣ: EventCheckEntryInfo�ֶθ�ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}

	EventCheckEntryInfo = root["EventCheckEntryInfo"];	
	if(EventCheckEntryInfo["SpecialEventNo"].type() == Json::nullValue || EventCheckEntryInfo["SpecialEventNo"].type() != Json::stringValue)
	{
		ErrorDescribe = "ȷ�ϳ��������Ϣ: SpecialEventNo�ֶθ�ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(EventCheckEntryInfo["MessageCode"].type() == Json::nullValue || EventCheckEntryInfo["MessageCode"].type() != Json::stringValue)
	{
		ErrorDescribe = "ȷ�ϳ��������Ϣ: MessageCode�ֶθ�ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}	

	EventInfo = EventCheckEntryInfo["EventInfo"];
	if(EventInfo["RequestTime"].type() == Json::nullValue || EventInfo["RequestTime"].type() != Json::stringValue)
	{
		ErrorDescribe = "ȷ�ϳ�����Ϣ: RequestTime�ֶθ�ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(EventInfo["RequestType"].type() == Json::nullValue || EventInfo["RequestType"].type() != Json::stringValue)
	{
		ErrorDescribe = "ȷ�ϳ�����Ϣ: RequestType�ֶθ�ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(EventInfo["EventDescribe"].type() == Json::nullValue || EventInfo["EventDescribe"].type() != Json::stringValue)
	{
		ErrorDescribe = "ȷ�ϳ�����Ϣ: EventDescribe�ֶθ�ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(EventInfo["MMIDemand"].type() == Json::nullValue || EventInfo["MMIDemand"].type() != Json::stringValue)
	{
		ErrorDescribe = "ȷ�ϳ�����Ϣ: MMIDemand�ֶθ�ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(EventInfo["TimeLimit"].type() == Json::nullValue || EventInfo["TimeLimit"].type() != Json::stringValue)
	{
		ErrorDescribe = "ȷ�ϳ�����Ϣ: TimeLimit�ֶθ�ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}

	return true;
}

bool CTWSDNetPay_DllApp::CheckCardOperationNotifyJsonValidity(const char* szParamContext, CString& ErrorDescribe, int& NotifyType)
{
	//������������У��
	Json::Reader reader;
	Json::Value  root;
	Json::Value  CardOperationNotify;
	Json::Value  Data;

	if(!reader.parse(szParamContext, root))
	{
		ErrorDescribe = "ETC��������ʾ�ӿ�: Json��ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}

	if(root["CardOperationNotify"].type() != Json::objectValue)
	{
		ErrorDescribe = "ETC��������ʾ�ӿ�: CardOperationNotify�ֶθ�ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}

	CardOperationNotify = root["CardOperationNotify"];	
	if(CardOperationNotify["SpecialEventNo"].type() == Json::nullValue || CardOperationNotify["SpecialEventNo"].type() != Json::stringValue)
	{
		ErrorDescribe = "ETC��������ʾ�ӿ�: SpecialEventNo�ֶθ�ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(CardOperationNotify["MessageCode"].type() == Json::nullValue || CardOperationNotify["MessageCode"].type() != Json::stringValue)
	{
		ErrorDescribe = "ETC��������ʾ�ӿ�: MessageCode�ֶθ�ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}

	Data = CardOperationNotify["Data"];
	if(Data["NotifyType"].type() == Json::nullValue || Data["NotifyType"].type() != Json::intValue)
	{
		ErrorDescribe = "ETC��������ʾ�ӿ�: NotifyType�ֶθ�ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}
	NotifyType = Data["NotifyType"].asUInt();

	return true;
}

bool CTWSDNetPay_DllApp::CheckEventModifyVehQueueJsonValidity(const char* szParamContext, CString& ErrorDescribe)
{
	//������������У��
	Json::Reader reader;
	Json::Value  root;
	Json::Value  EventModifyVehQueue;

	if(!reader.parse(szParamContext, root))
	{
		ErrorDescribe = "�޸ĳ������нӿ�: Json��ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}

	if(root["EventModifyVehQueue"].type() != Json::objectValue)
	{
		ErrorDescribe = "�޸ĳ������нӿ�: EventModifyVehQueue�ֶθ�ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}

	EventModifyVehQueue = root["EventModifyVehQueue"];	
	if(EventModifyVehQueue["SpecialEventNo"].type() == Json::nullValue || EventModifyVehQueue["SpecialEventNo"].type() != Json::stringValue)
	{
		ErrorDescribe = "�޸ĳ������нӿ�: SpecialEventNo�ֶθ�ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(EventModifyVehQueue["MessageCode"].type() == Json::nullValue || EventModifyVehQueue["MessageCode"].type() != Json::stringValue)
	{
		ErrorDescribe = "�޸ĳ������нӿ�: MessageCode�ֶθ�ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(EventModifyVehQueue["TotalVehNum"].type() == Json::nullValue || EventModifyVehQueue["TotalVehNum"].type() != Json::intValue)
	{
		ErrorDescribe = "�޸ĳ������нӿ�: TotalVehNum�ֶθ�ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}

	return true;
}

bool CTWSDNetPay_DllApp::CheckAuthorize(const char* szParamContext, CString& ErrorDescribe)
{
	//������������У��
	Json::Reader reader;
	Json::Value  root;
	Json::Value  Authorize, Data;

	if(!reader.parse(szParamContext, root))
	{
		ErrorDescribe = "������Ȩ�ӿ�: Json��ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}

	if(root["Authorize"].type() != Json::objectValue)
	{
		ErrorDescribe = "������Ȩ�ӿ�: EventModifyVehQueue�ֶθ�ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}

	Authorize = root["Authorize"];	
	if(Authorize["SpecialEventNo"].type() == Json::nullValue || Authorize["SpecialEventNo"].type() != Json::stringValue)
	{
		ErrorDescribe = "������Ȩ�ӿ�: SpecialEventNo�ֶθ�ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(Authorize["MessageCode"].type() == Json::nullValue || Authorize["MessageCode"].type() != Json::stringValue)
	{
		ErrorDescribe = "������Ȩ�ӿ�: MessageCode�ֶθ�ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(Authorize["VehPlate"].type() == Json::nullValue || Authorize["VehPlate"].type() != Json::stringValue)
	{
		ErrorDescribe = "������Ȩ�ӿ�: VehPlate�ֶθ�ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}

	if(Authorize["VehPlateColor"].type() == Json::nullValue || Authorize["VehPlateColor"].type() != Json::stringValue)
	{
		ErrorDescribe = "������Ȩ�ӿ�: VehPlateColor�ֶθ�ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}

	if(Authorize["Data"].type() != Json::objectValue)
	{
		ErrorDescribe = "������Ȩ�ӿ�: Data�ֶθ�ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}
	Data = Authorize["Data"];

	if(Data["authorizeType"].type() == Json::nullValue || Data["authorizeType"].type() != Json::intValue)
	{
		ErrorDescribe = "������Ȩ�ӿ�: authorizeType�ֶθ�ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}

	if(Data["authorizeReason"].type() == Json::nullValue || Data["authorizeReason"].type() != Json::stringValue)
	{
		ErrorDescribe = "������Ȩ�ӿ�: authorizeReason�ֶθ�ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}

	if(Data["authorizeOption"].type() == Json::nullValue || Data["authorizeOption"].type() != Json::stringValue)
	{
		ErrorDescribe = "������Ȩ�ӿ�: authorizeOption�ֶθ�ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}

	return true;
}


bool CTWSDNetPay_DllApp::CheckEventFeeAuthorize(const char* szParamContext, CString& ErrorDescribe)
{
	//������������У��
	Json::Reader reader;
	Json::Value  root;
	Json::Value  Authorize, Data;

	if(!reader.parse(szParamContext, root))
	{
		ErrorDescribe = "��С�Ѷ���Ȩ�ӿ�: Json��ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}

	if(root["Authorize"].type() != Json::objectValue)
	{
		ErrorDescribe = "��С�Ѷ���Ȩ�ӿ�: EventModifyVehQueue�ֶθ�ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}

	Authorize = root["Authorize"];	
	if(Authorize["SpecialEventNo"].type() == Json::nullValue || Authorize["SpecialEventNo"].type() != Json::stringValue)
	{
		ErrorDescribe = "��С�Ѷ���Ȩ�ӿ�: SpecialEventNo�ֶθ�ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(Authorize["MessageCode"].type() == Json::nullValue || Authorize["MessageCode"].type() != Json::stringValue)
	{
		ErrorDescribe = "��С�Ѷ���Ȩ�ӿ�: MessageCode�ֶθ�ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}
	//if(Authorize["VehPlate"].type() == Json::nullValue || Authorize["VehPlate"].type() != Json::stringValue)
	//{
	//	ErrorDescribe = "��С�Ѷ���Ȩ�ӿ�: VehPlate�ֶθ�ʽ����";
	//	RecordLog(ErrorDescribe);
	//	return false;
	//}

	//if(Authorize["VehPlateColor"].type() == Json::nullValue || Authorize["VehPlateColor"].type() != Json::stringValue)
	//{
	//	ErrorDescribe = "��С�Ѷ���Ȩ�ӿ�: VehPlateColor�ֶθ�ʽ����";
	//	RecordLog(ErrorDescribe);
	//	return false;
	//}

	if(Authorize["Data"].type() != Json::objectValue)
	{
		ErrorDescribe = "��С�Ѷ���Ȩ�ӿ�: Data�ֶθ�ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}
	Data = Authorize["Data"];

	if(Data["authorizeType"].type() == Json::nullValue || Data["authorizeType"].type() != Json::intValue)
	{
		ErrorDescribe = "��С�Ѷ���Ȩ�ӿ�: authorizeType�ֶθ�ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}

	//if(Data["authorizeReason"].type() == Json::nullValue || Data["authorizeReason"].type() != Json::stringValue)
	//{
	//	ErrorDescribe = "��С�Ѷ���Ȩ�ӿ�: authorizeReason�ֶθ�ʽ����";
	//	RecordLog(ErrorDescribe);
	//	return false;
	//}

	//if(Data["authorizeOption"].type() == Json::nullValue || Data["authorizeOption"].type() != Json::stringValue)
	//{
	//	ErrorDescribe = "��С�Ѷ���Ȩ�ӿ�: authorizeOption�ֶθ�ʽ����";
	//	RecordLog(ErrorDescribe);
	//	return false;
	//}

	return true;
}


bool CTWSDNetPay_DllApp::CheckEventDelVehQueueResult(const char* szParamContext, CString& ErrorDescribe)
{
	//������������У��
	Json::Reader reader;
	Json::Value  root;
	Json::Value  DelVehQueueResult, Data;

	if(!reader.parse(szParamContext, root))
	{
		ErrorDescribe = "ɾ���������н���ӿ�: Json��ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}

	if(root["DelVehQueueResult"].type() != Json::objectValue)
	{
		ErrorDescribe = "ɾ���������н���ӿ�: DelVehQueueResult�ֶθ�ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}

	DelVehQueueResult = root["DelVehQueueResult"];	
	if(DelVehQueueResult["SpecialEventNo"].type() == Json::nullValue || DelVehQueueResult["SpecialEventNo"].type() != Json::stringValue)
	{
		ErrorDescribe = "ɾ���������н���ӿ�: SpecialEventNo�ֶθ�ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(DelVehQueueResult["MessageCode"].type() == Json::nullValue || DelVehQueueResult["MessageCode"].type() != Json::stringValue)
	{
		ErrorDescribe = "ɾ���������н���ӿ�: MessageCode�ֶθ�ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}

	if(DelVehQueueResult["Data"].type() != Json::objectValue)
	{
		ErrorDescribe = "ɾ���������н���ӿ�: Data�ֶθ�ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}
	Data = DelVehQueueResult["Data"];

	if(Data["ExecuteType"].type() == Json::nullValue || Data["ExecuteType"].type() != Json::intValue)
	{
		ErrorDescribe = "ɾ���������н���ӿ�: ExecuteType�ֶθ�ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}

	if(Data["DelVehNum"].type() == Json::nullValue || Data["DelVehNum"].type() != Json::intValue)
	{
		ErrorDescribe = "ɾ���������н���ӿ�: DelVehNum�ֶθ�ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}

	if(Data["ExecuteResult"].type() == Json::nullValue || Data["ExecuteResult"].type() != Json::intValue)
	{
		ErrorDescribe = "ɾ���������н���ӿ�: ExecuteResult�ֶθ�ʽ����";
		RecordLog(ErrorDescribe);
		return false;
	}

	return true;
}


bool CTWSDNetPay_DllApp::StartReaderServer()
{
	if(!m_pWebSocketServerReader)
	{
		m_pWebSocketServerReader = new WebSocketServer();
		m_pWebSocketServerReader->startServer();
	}

	return true;
}




bool CTWSDNetPay_DllApp::RecvMsgFuncFromWebsocket(const char* pszData, int nSize)
{
	// ���տͻ��˵���Ϣ��ת����������
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	RecordLog(FmtStr(_T("[ETC����ҵ��],ת�����ݸ����ڽӿ�(EtcEventWebsocketControl)���ÿ�ʼ")));
	CString strLog;
	bool bRet = false;

	try
	{
		if(m_pThreadManage == NULL)
			throw -1;

		if(m_pThreadManage->m_pThreadDevice == NULL)
			throw -2;

		bRet = theApp.m_pThreadManage->m_pThreadDevice->EtcEventWebsocketControl(pszData, nSize);
	}
    catch (int& iErrorCode)
    {
        switch (iErrorCode)
        {
        case -1:
            strLog.Format(_T("EtcEvent������ʼ���ӿ�(IF_EventInitEnvironment3),ҵ������߳���ԴΪ�գ�"));
            break;
        case -2:
            strLog.Format(_T("EtcEvent������ʼ���ӿ�(IF_EventInitEnvironment3),�豸�����߳���ԴΪ�գ�"));
            break;
        }
        //��¼��־
		RecordLog(strLog);
	}

	return bRet;

}

/****************************************�ڲ���������********************************************/

////////////////////////////////////////////////////////////////////////////////////////////////
// �������ƣ�CTWSDNetPay_DllApp::RecordLog
//
// ������������¼��־
//
// ���������IN const CString& strLog -- ��־����
//         ��IN const int& iLevel    -- ��־�ȼ�
//         ��IN const BOOL& bSave    -- ��־�Ƿ�������
// ���������void
// ��д��Ա��ROCY
// ��дʱ�䣺2016��7��24��
// �޸���Ա��
// �޸�ʱ�䣺
// �����汾��1.0.0.1
// ��ע˵����
//
////////////////////////////////////////////////////////////////////////////////////////////////
void CTWSDNetPay_DllApp::RecordLog(IN const CString& strLog, IN const int& iLevel, IN const BOOL& bSave)
{
    if (m_bSaveLog)
    {
        if (strLog.GetLength() < 4)
        {
            return;
        }
        WRITE_LOG(strLog, iLevel, bSave);
        //::OutputDebugString(strLog);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////
// �������ƣ�CTWSDNetPay_DllApp::ReadConfigInfo
//
// ������������ȡ������Ϣ
//
// ���������void
// ���������void
// ��д��Ա��ROCY
// ��дʱ�䣺2016��7��24��
// �޸���Ա��
// �޸�ʱ�䣺
// �����汾��1.0.0.1
// ��ע˵����
//
////////////////////////////////////////////////////////////////////////////////////////////////
void CTWSDNetPay_DllApp::ReadConfigInfo(void)
{
    TCHAR szTemp[256] = {0};
    CString strConfigFilePath;
    strConfigFilePath.Format(_T("%s\\%s"), m_strWorkDir, FILE_NAME_APP_CONFIG);
    //�Ƿ񱣴���־
    m_bSaveLog              = (BOOL)::GetPrivateProfileIntA(_T("Log"), _T("Save"), 0, strConfigFilePath);
    //��ȡ��־ɾ���޶�ʱ������
    m_iDeleteLogStartHour = ::GetPrivateProfileIntA(_T("Log"), _T("DeleteStartHour"), 2, strConfigFilePath);
    //��ȡ��־ɾ���޶�ʱ����յ�
    m_iDeleteLogEndHour = ::GetPrivateProfileIntA(_T("Log"), _T("DeleteEndHour"), 2, strConfigFilePath);
    //��־ɾ��������ѯʱ��
    m_iDeleteLogSpanTime = ::GetPrivateProfileIntA(_T("Log"), _T("DeleteSpanTime"), 30 * 60, strConfigFilePath);
    //��־���ر�������ʱ��
    m_iDeleteLogSaveDays = ::GetPrivateProfileIntA(_T("Log"), _T("DeleteSaveDays"), 60, strConfigFilePath);
    //����Ƿ��������
    m_bIsTestData           = (BOOL)::GetPrivateProfileIntA(_T("Setup"), _T("TestData"), 0, strConfigFilePath);
    //�̴߳����־���ʱ����
    m_dwThreadCheckSpanTime = ::GetPrivateProfileIntA(_T("Setup"), _T("ThreadCheckSpanTime"), 0, strConfigFilePath);
    //��ȡ���⿨ǰ׺
    ::GetPrivateProfileString(_T("Setup"), _T("VirtualCardPrefix"), _T("4412"), szTemp, 256, strConfigFilePath);
    m_strVirtualCardPrefix.Format(_T("%s"), szTemp);
    //��ȡ������Ӳ���·�ʱ��ļ��
    m_dwUpdateDevieTimeSpanTime = (DWORD)::GetPrivateProfileIntA(_T("Setup"), _T("UpdateDeviceTimeSpanTime"), 30 * 60, strConfigFilePath);
    m_dwUpdateDevieTimeSpanTime *= 1000;
    //���������ط����ʱ����
    m_tResendDataSpanTime       = (DWORD)::GetPrivateProfileInt(_T("CancelRecord"), _T("CheckTime"), 30, strConfigFilePath);
    //�����������汾��ʱ�ޣ���λ�����ӣ�
    m_iLastDelayTime            = (DWORD)::GetPrivateProfileInt(_T("CancelRecord"), _T("LastDelay"), 30, strConfigFilePath);
    //�����˳�ǰ�ĵȴ�ʱ�䣨�ȴ���Դ�����
    m_iExitWaitForTime          = (DWORD)::GetPrivateProfileInt(_T("Setup"), _T("ExitWaitForTime"), 500, strConfigFilePath);
    //HTTP�ڲ����
    m_iHTTPVersion              = ::GetPrivateProfileInt(_T("ReflectWeb"),  _T("Version"),  1,  strConfigFilePath);

    //add 2018-04
    char szTempIP[256] = {0};
    ::GetPrivateProfileString(_T("ReflectWeb"), _T("IP"), _T("101.200.214.203"), szTempIP, _countof(szTempIP), strConfigFilePath);
    _tcscpy_s(m_sTranstParam.LaneConfigInfo.WebServerIP, _countof(m_sTranstParam.LaneConfigInfo.WebServerIP), szTempIP);
    m_sTranstParam.LaneConfigInfo.WebServerPort =  ::GetPrivateProfileInt(_T("ReflectWeb"), _T("PORT"), 80, strConfigFilePath);

    int int_para = ::GetPrivateProfileInt(_T("TimeArgu"), _T("LPRQueryLimit"), 100, strConfigFilePath);
    GetGlobalApp()->SetLPRQueryOverTimeMilli(int_para);



    //ȷ��ͨѶ��̬����ص��豸��������
    m_iDeviceType               = ::GetPrivateProfileInt(_T("Setup"),       _T("Device"),   0,  strConfigFilePath);
    //�Ƿ��ƶ�֧���ʹ���֧����֧�����������뱣��һ��
    m_bReturnPayChannelCodeUnify    = (bool)::GetPrivateProfileInt(_T("Setup"),     _T("ReturnPayChannelCodeUnify"), 0, strConfigFilePath);
    //��ȡ���ۿ۷Ѳ�����ʱʱ��ֵ
    m_iAgencyDebitTimeout       = ::GetPrivateProfileInt(_T("Setup"), _T("AgencyDebitTimeout"), 2000, strConfigFilePath);
    //��ȡ���������ڲ���ʱʱ��ֵ
    m_iClearMMIDelayTime        = ::GetPrivateProfileInt(_T("Setup"), _T("ClearMMIDelayTime"), 100, strConfigFilePath);
    //�Ƿ����ô��۶�̬��
    m_bUseAgencyDll             = (bool)::GetPrivateProfileInt(_T("Setup"), _T("UseAgencyPayDll"), 0, strConfigFilePath);
    //�Ƿ�ʹ����չ�Ľ���ϸ��ģʽ
    m_bIsExternMMIDetail        = (BOOL)::GetPrivateProfileInt(_T("Setup"), _T("UseExternMMI"), 0, strConfigFilePath);
    //�Ƿ�����ʹ���°汾�Ľ���ϸ��ģʽ
    m_bIsAllowUseNewMMIDetailMode = (BOOL)::GetPrivateProfileInt(_T("Setup"), _T("AllowUseNewMMIMode"), 0, strConfigFilePath);

    //�Ƿ����������ǽ�ģ��
    m_bNFCIsUse                         = (bool)::GetPrivateProfileInt(_T("Unipay"), _T("UseUnipay"), 0, strConfigFilePath);
    //����ģ��ǩ��״̬���ʱ����
    m_iNFCCheckSignSpanTime             = ::GetPrivateProfileInt(_T("Unipay"), _T("CheckSignSpanTime"), 10, strConfigFilePath);
    if (m_iNFCCheckSignSpanTime > 1800 || m_iNFCCheckSignSpanTime < 10)
    {
        m_iNFCCheckSignSpanTime = 30;
    }
    //����ģ�鶨ʱ��ǩ���ʱ����ʼ��
    m_iNFCDefaultStartReSignHour        = ::GetPrivateProfileInt(_T("Unipay"), _T("AutoStartReSignHour"), 0, strConfigFilePath);
    //����ģ�鶨ʱ��ǩ���ʱ���ֹ��
    m_iNFCDefaultEndReSignHour          = ::GetPrivateProfileInt(_T("Unipay"), _T("AutoEndReSignHour"), 3, strConfigFilePath);
    //����ģ��ǩ�����ݽ���ʱ��������
    m_iNFCSignDataLimitSpanTime         = ::GetPrivateProfileInt(_T("Unipay"), _T("SignDataLimitSpanTime"), 60, strConfigFilePath);
    if (m_iNFCSignDataLimitSpanTime > 70 || m_iNFCSignDataLimitSpanTime < 30)
    {
        m_iNFCSignDataLimitSpanTime = 60;
    }
    //����֧�����̣��Ƿ�����û�л�ȡ��������Ҳ�����Ϳ۷�����
    m_bNFCIsAllowDebitWithoutCancelData = (BOOL)::GetPrivateProfileInt(_T("Unipay"), _T("AllowDebitWithoutCancelData"), 0, strConfigFilePath);
    //����֧�����̣���ȡ��ˢ�����ݺ����Ƶȴ��������ݵ�ʱ�䣨��λ���룩
    m_iNFCGetCancelDataLimitTime        = ::GetPrivateProfileInt(_T("Unipay"), _T("GetCancelDataLimitTime"), 10, strConfigFilePath);
    if (m_iNFCGetCancelDataLimitTime > 15 || m_iNFCGetCancelDataLimitTime < 5)
    {
        m_iNFCGetCancelDataLimitTime = 10;
    }
    m_sTranstParam.LaneConfigInfo.ForceUTC8 = ::GetPrivateProfileInt(_T("SetUp"), _T("ForceUTC8"), 0, strConfigFilePath);



}

////////////////////////////////////////////////////////////////////////////////////////////////
// �������ƣ�CTWSDNetPay_DllApp::fnCloseThread
//
// �����������ȴ�һ���������ر��߳������ر�
//
// ���������IN const HANDLE& hThread          --  ��Ҫ���رյ��߳̾��
//         ��IN const DWORD& dwMilliseconds        --  �ȴ�ʱ�䣨��λ�����룩
// ���������int
//         ��0  -- �߳��Ѿ��������ر�
//         ��-1    -- �߳̾����Ч
//         ��-2    -- �ں˶���δ�������ȴ�ʱ���ѵ�
//         ��-3    -- �ȴ��Ļ�����δ���ͷ�
//         ��-4    -- �ȴ�����ִ��ʧ��
// ��д��Ա��ROCY
// ��дʱ�䣺2015��11��15��
// �޸���Ա��
// �޸�ʱ�䣺
// �����汾��1.0.0.1
// ��ע˵����
//
////////////////////////////////////////////////////////////////////////////////////////////////
int CTWSDNetPay_DllApp::fnCloseThread(IN const HANDLE& hThread, IN const DWORD& dwMilliseconds)
{
    int iReturn = 0;
    //���Թر��߳�
    DWORD dwWaitResult = ::WaitForSingleObject(hThread, dwMilliseconds);
    //��ô�����
    DWORD dwErrorCode = ::GetLastError();
    //��Ϣ�ṹ����
    MSG  mMsg;

    if (6 == dwErrorCode)//�����Ч
    {
        iReturn = 0;
    }
    else
    {
        //�жϵȴ��������ؽ��
        switch (dwWaitResult)
        {
        case WAIT_OBJECT_0: //�ں˶����Ϊ����״̬
            iReturn = 0;
            break;
        case WAIT_TIMEOUT:  //�ں˶���δ�������ȴ�ʱ���ѵ�
        {
            if (::PeekMessage(&mMsg, NULL, 0, 0, PM_REMOVE))
            {
                ::TranslateMessage(&mMsg); Sleep(10);
                ::DispatchMessage(&mMsg); Sleep(10);
            }
        }
        iReturn = -2;
        break;
        case WAIT_ABANDONED://�ȴ��Ļ�����δ���ͷ�
        {
            if (::PeekMessage(&mMsg, NULL, 0, 0, PM_REMOVE))
            {
                ::TranslateMessage(&mMsg); Sleep(10);
                ::DispatchMessage(&mMsg); Sleep(10);
            }
        }
        iReturn = -3;
        break;
        default:
            iReturn = -4;
            break;
        }

    }
    return iReturn;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
//  ��   �� : ROCY
//  ʱ   �� : 2008-05-31
//  ��   �� : 1.0.0.1
//  �������� : DisplayWinError
//  �������� : ��ʾϵͳ���µĴ�����Ϣ
//  ������� : DWORD    --  dwErrorCode ������ϵͳ�������,һ���� GetLastError() ��������
//             int      --  iType       Ĭ��Ϊ0
//             BOOL     --  bPrompt     �Ƿ���Ҫ�öԻ�����ʾ�ô�����Ϣ��TRUEΪ��Ҫ��FALSEΪ����Ҫ��
//  ������� : LPCTSTR -- ��ϸ�Ĵ���������Ϣ
//  ��ע˵�� : ÿ�ε���API�󶼿��Ե��ô˺����鿴��ϸ�Ĵ�����Ϣ
///////////////////////////////////////////////////////////////////////////////////////////////////////////
LPCTSTR CTWSDNetPay_DllApp::DisplayWinError(DWORD dwErrorCode, int nType, BOOL bPrompt)
{
    static CString  strTemp = _T("");
    LPSTR lpMessageBuffer = NULL;
    USES_CONVERSION;
    ::FormatMessageA( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,// | FORMAT_MESSAGE_IGNORE_INSERTS,
                      NULL,
                      (nType == 0) ? dwErrorCode : LsaNtStatusToWinError(dwErrorCode),
                      0,
                      (LPSTR) &lpMessageBuffer,
                      0,
                      NULL);
    strTemp.Format(_T("���������:%d�� -- ����ϸ����:%s��"), dwErrorCode, A2T(lpMessageBuffer));
    strTemp.Replace(_T("\r\n"), _T(" "));

    if (bPrompt)
        ::MessageBox(NULL, strTemp, _T("ϵͳ��ʾ"), MB_ICONWARNING);

    LocalFree(lpMessageBuffer);
    return strTemp;
}

//��ȡ��ǰ������ʹ�õ��ڴ�
DWORD CTWSDNetPay_DllApp::GetProcessMemory(IN const int& iShowType)
{
    DWORD dDevValue = (1 == iShowType) ? (1024 * 1024) : 1024;
    //��ȡ��ǰ����ʹ���ڴ���Ϣ
    DWORD dwProcessMemory = 0x00;
    PROCESS_MEMORY_COUNTERS pLocalProcessMemory = {0};
    if (GetProcessMemoryInfo(GetCurrentProcess(), &pLocalProcessMemory, sizeof(pLocalProcessMemory)))
    {
        dwProcessMemory = pLocalProcessMemory.PagefileUsage / dDevValue;
    }

    return dwProcessMemory;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// �������ƣ�CTWSDNetPay_DllApp::AgencyInitDll
//
// ������������ʼ�����۶�̬��
//
// ���������void
// ���������bool
// ��д��Ա��ROCY
// ��дʱ�䣺2017��8��24��
// �޸���Ա��
// �޸�ʱ�䣺
// �����汾��1.0.0.1
// ��ע˵����
//
////////////////////////////////////////////////////////////////////////////////////////////////
bool CTWSDNetPay_DllApp::AgencyInitDll(IN const char* szAreaInfo, IN const char* szLoaclStation, IN const char* szLoaclLaneID, IN const char* szServerInfo, IN const int& iProvinceID)
{
    bool bInitAgencyDll = false;

    //�ж��Ƿ����ô��۶�̬��
	m_bUseAgencyDll = false;
    if (!m_bUseAgencyDll)
    {
        RecordLog( FmtStr(_T("[��������Ӧ],�����ʼ���ӿ�(IF_InitEnvironment)�����۶�̬��δ����!)")) );
        return bInitAgencyDll;
    }
    if (m_pThreadManage == 0)
    {
        RecordLog( FmtStr(_T("[��������Ӧ],�޷���ʼ�����ۿ⣬�����߳�δ����!)")) );
        return  false;
    }
    {//add 2019-01 
        Sleep(150);//��֤m_pThreadManage�Ѿ�����
        if (m_pThreadManage->IsFuturePay())
        {
            int init_result = m_pThreadManage->InitAgencyPay(szAreaInfo, szLoaclStation, szLoaclLaneID, szServerInfo, iProvinceID);
            return (init_result == 0); //modify 2019-01 ʹ���µ�֧���߼�
        }
    }

    int iResultCode = 0x00;
    CString strAgencyPayDll;
    strAgencyPayDll.Format( _T("%s\\TWITC_AGENCY.dll"), m_strWorkDir );
    if ( m_AgencyPay.Init( strAgencyPayDll ))
    {
        if (0 == (iResultCode = m_AgencyPay.InitEnvironment(szAreaInfo, szLoaclStation, szLoaclLaneID, szServerInfo, iProvinceID)) )
        {
            bInitAgencyDll = true;
            RecordLog( FmtStr(_T("[��������Ӧ],�����ʼ���ӿ�(IF_InitEnvironment)�����۶�̬��(%s)��ʼ���ɹ�!)"), strAgencyPayDll) );
        }
        else
        {
            RecordLog( FmtStr(_T("[��������Ӧ],�����ʼ���ӿ�(IF_InitEnvironment),�ڲ���ʼ������,�޷���ʼ�����۶�̬��(�������:%d)"), iResultCode) );
        }
    }
    else
    {
        RecordLog( FmtStr(_T("[��������Ӧ],�����ʼ���ӿ�(IF_InitEnvironment),�ڲ���ʼ������,���۶�̬��(%s)����ʧ��"), strAgencyPayDll) );
    }
    return bInitAgencyDll;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// �������ƣ�CTWSDNetPay_DllApp::AgencyConstructDebitData
//
// ����������
//
// ���������void
// ���������int
// ��д��Ա��ROCY
// ��дʱ�䣺2017��8��17��
// �޸���Ա��
// �޸�ʱ�䣺
// �����汾��1.0.0.1
// ��ע˵���������ؽ��Ϊtrue�����ʾ���ٽ��к�����ɨ��ҵ�����̣������ؽ��Ϊfalse�����ʾ������Ҫִ��ɨ��ҵ������
//
////////////////////////////////////////////////////////////////////////////////////////////////
bool CTWSDNetPay_DllApp::AgencyConstructDebitData(int & agency_result)
{
    //�ж��Ƿ����ô��۶�̬��
    if (!m_bUseAgencyDll)
    {
        return false;
    }

    bool is_disable_agency = (m_pCurrentDebitInfo.PayModel == 1);
    if (is_disable_agency)
    {
        RecordLog(_T("[��������Ӧ],ִ�пۿ�����ӿ�(IF_DebitMoney): �����д��۴���"));
        return false;
    }

    //�����жϳ�����Ч����������7λ��
    if ( m_pCurrentDebitInfo.VehicleLicense.GetLength() < 7 )
        return false;
    BYTE byFunCode      = 0x01;
    BYTE byExecuteState = RCC_INNER_ERROR;

    m_dwTradeBeginOvertime = ::GetTickCount();  //��ʱ
    m_strLastErrorDesc.Format(_T("[��������Ӧ],ִ�пۿ����(IF_DebitMoney), ���ڽ��г������۴���"));

    CMarkup xmlAgencyDebit;
    xmlAgencyDebit.SetDoc( _T("<?xml version=\"1.0\" encoding=\"GB2312\"?>\r\n") );
    xmlAgencyDebit.AddElem( _T("DebitParamContext") );//root
    xmlAgencyDebit.IntoElem();

    xmlAgencyDebit.AddElem( _T("Version"),      _T("1.0") );
    xmlAgencyDebit.AddElem( _T("TimeOut"),      m_iAgencyDebitTimeout );

    xmlAgencyDebit.AddElem( _T("Data") );
    xmlAgencyDebit.IntoElem();
    xmlAgencyDebit.AddElem( _T("OverTime"),     m_pCurrentDebitInfo.OverTime );

    xmlAgencyDebit.AddElem( _T("Debit") );
    xmlAgencyDebit.IntoElem();
    xmlAgencyDebit.AddElem( _T("PayIdentifier"),    m_pCurrentDebitInfo.PayIdentifier );
    xmlAgencyDebit.AddElem( _T("imageId"),    m_pCurrentDebitInfo.imageId );
    //xmlAgencyDebit.AddElem( _T("Distance"),    m_pCurrentDebitInfo.Distance );

    xmlAgencyDebit.AddElem( _T("Money"),            m_pCurrentDebitInfo.Money );
    xmlAgencyDebit.AddElem( _T("SubTime"),          m_pCurrentDebitInfo.SubTime.Format(_T("%Y-%m-%d %H:%M:%S")) );
    xmlAgencyDebit.AddElem( _T("Type"),             1 );
    //xmlAgencyDebit.AddElem( _T("PayOperatorId"),    m_pCurrentDebitInfo.OperatorID );
    //xmlAgencyDebit.AddElem( _T("PayOperatorName"),  m_pCurrentDebitInfo.OperatorName );
    xmlAgencyDebit.OutOfElem();

    xmlAgencyDebit.AddElem( _T("Vehicle") );
    xmlAgencyDebit.IntoElem();
    xmlAgencyDebit.AddElem( _T("Type"),         m_pCurrentDebitInfo.VehicleType );
    xmlAgencyDebit.AddElem( _T("Class"),        m_pCurrentDebitInfo.VehicleClass );
    xmlAgencyDebit.AddElem( _T("License"),      m_pCurrentDebitInfo.VehicleLicense );
    xmlAgencyDebit.AddElem( _T("VLColor"),      m_pCurrentDebitInfo.VLColor );
    xmlAgencyDebit.AddElem( _T("AxisCount"),    m_pCurrentDebitInfo.AxisCount );
    xmlAgencyDebit.AddElem( _T("Weight"),       m_pCurrentDebitInfo.TotalWeight );
    xmlAgencyDebit.AddElem( _T("LimitWeight"),  m_pCurrentDebitInfo.LimitWeight );

    xmlAgencyDebit.AddElem( _T("AutoPlate"));
    xmlAgencyDebit.IntoElem();
    xmlAgencyDebit.AddElem( _T("AutoLicense"),  m_pCurrentDebitInfo.AutoLicense );
    xmlAgencyDebit.AddElem( _T("AutoColor"),  m_pCurrentDebitInfo.AutoColor );
    xmlAgencyDebit.OutOfElem();

    xmlAgencyDebit.OutOfElem();

    xmlAgencyDebit.AddElem( _T("Operation") );
    xmlAgencyDebit.IntoElem();
    xmlAgencyDebit.AddElem( _T("PassCertificateType"),  m_pCurrentDebitInfo.PassCertificateType );
    xmlAgencyDebit.AddElem( _T("CardID"),       m_pCurrentDebitInfo.CardID );
    xmlAgencyDebit.AddElem( _T("TollDate"),     m_pCurrentDebitInfo.TollDate );
    xmlAgencyDebit.AddElem( _T("ShiftID"),      m_pCurrentDebitInfo.ShiftID );
    xmlAgencyDebit.AddElem( _T("TicketNo"),     m_pCurrentDebitInfo.TicketNo );
    xmlAgencyDebit.AddElem( _T("Distance"),    m_pCurrentDebitInfo.Distance );
    xmlAgencyDebit.AddElem( _T("PayOperatorId"),    m_pCurrentDebitInfo.OperatorID );
    xmlAgencyDebit.AddElem( _T("PayOperatorName"),  m_pCurrentDebitInfo.OperatorName );
    xmlAgencyDebit.OutOfElem();

    xmlAgencyDebit.AddElem( _T("Entry") );
    xmlAgencyDebit.IntoElem();
    xmlAgencyDebit.AddElem( _T("ProvinceId"),   m_sTranstParam.ProvinceID );
    xmlAgencyDebit.AddElem( _T("AreaID"),       m_pCurrentDebitInfo.EntryAreaID );
    xmlAgencyDebit.AddElem( _T("RoadID"),       m_pCurrentDebitInfo.EntryRoadID );
    xmlAgencyDebit.AddElem( _T("StationID"),    m_pCurrentDebitInfo.EntryStationID );
    xmlAgencyDebit.AddElem( _T("StationName"),  m_pCurrentDebitInfo.EntryStationName );
    xmlAgencyDebit.AddElem( _T("Time"),         m_pCurrentDebitInfo.EntryTime.Format(_T("%Y-%m-%d %H:%M:%S")) );

    xmlAgencyDebit.AddElem( _T("LaneID"),       m_pCurrentDebitInfo.EntryLane );
    xmlAgencyDebit.AddElem( _T("License"),      m_pCurrentDebitInfo.EntryVehicleLicense );
    xmlAgencyDebit.AddElem( _T("VColor"),       m_pCurrentDebitInfo.EntryVLColor );
    xmlAgencyDebit.AddElem( _T("VClass"),       m_pCurrentDebitInfo.EntryVehicleClass );
    xmlAgencyDebit.AddElem( _T("VType"),        m_pCurrentDebitInfo.EntryVehicleType );
    xmlAgencyDebit.OutOfElem();

    xmlAgencyDebit.OutOfElem(); //xmlAgencyDebit.AddElem( _T("Data") );
    CString strAgencyDebit = xmlAgencyDebit.GetDoc();
    int     nAgencyDebitLength = strAgencyDebit.GetLength();
    char    szAgencyResult[2048] = {0};


    int iIsCancelAnency = 0x00;
    int iAnencyResult = 0x00;
    DWORD dwStartDebitTime = ::GetTickCount();
    iAnencyResult = m_AgencyPay.AgencyDebitMoney( strAgencyDebit.GetBuffer(0), szAgencyResult, nAgencyDebitLength, 1);
    DWORD dwConsumeTimeCount = ::GetTickCount() - dwStartDebitTime;
    GetGlobalApp()->GetETCDataFromAgencyResult(szAgencyResult);
    agency_result = iAnencyResult;
    if ( 0 == iAnencyResult )
    {

        CString strAgencyResult = _T("");
        strAgencyResult.Format( _T("%s"), szAgencyResult );
        int serverDebitType = 0;
        if ( 0 == GetAgencyResult( strAgencyResult, dwConsumeTimeCount, serverDebitType ) )
        {
            //add 2018-04 �����۽������������ʶ��
            GetGlobalApp()->LPR_PackPayResult(GetGlobalApp()->m_sCurrentDebitResult.PayPlatformType);
            //֪ͨ�ɹ�
            byFunCode = 0x01;
            byExecuteState = RCC_OK;
            //�����ն���ʾ�ɹ�
            m_pThreadManage->m_pThreadDevice->PostThreadMessage(WM_THREAD_DEVICE_CONTROL_DEAL_RESULT_MMI, (WPARAM)0x01, (LPARAM)serverDebitType);//modify 2018-09
            //��ȡ�����Ϣ�ɹ���֪ͨ������
            NotifyCaller(byFunCode, byExecuteState);
            //��¼��־
            RecordLog(FmtStr(_T("[��������Ӧ],ִ�пۿ�����ӿ�(IF_DebitMoney)���ý���(%s,����֧��,�ɹ���"), m_pCurrentDebitInfo.VehicleLicense));
            return true;
        }
        else
        {
            iIsCancelAnency = 0x01;
        }
    }
    else if (iAnencyResult == 25) //����ǳ��Ͳ���
    {
        GetGlobalApp()->m_sCurrentDebitResult.ExecuteCode = 10;//
    }
    else
    {
        //�жϷ��ش���ȷ���Ƿ���Ҫ������۳���
        switch (iAnencyResult)
        {
        case 6:
        case 12:
            iIsCancelAnency = 0x02;
            break;
        }
    }

    //�ж��Ƿ���Ҫִ�д��۳���
    if (iIsCancelAnency > 0)
    {
        //��ʶ�۷��������
        GetGlobalApp()->m_bIsTrading        = FALSE;
        GetGlobalApp()->m_bCurrentDebitMode = FALSE;

        AgencyConstructCancelData(m_pCurrentDebitInfo.PayIdentifier);
        //֪ͨʧ��
        byFunCode = 0x01;
        byExecuteState = RCC_PAY_PLATFORM_ERROR;
        //֪ͨ������
        NotifyCaller(byFunCode, byExecuteState);
        //��¼��־
        RecordLog(FmtStr(_T("[��������Ӧ],����(%s)���ۿ۷ѽ������ʧ��(%s)���Ѿ����𳷵���ת��ɨ�봦��"), m_pCurrentDebitInfo.VehicleLicense, (0x01 == iIsCancelAnency) ? _T("��ȡ����쳣") : _T("���糬ʱ")) );
        //���ٽ���ɨ������
        return true;
    }
    else
    {
        RecordLog(FmtStr(_T("[��������Ӧ],����(%s)���ۿ۷ѽ������ʧ�ܣ�����Ҫ������ת��ɨ�봦��"), m_pCurrentDebitInfo.VehicleLicense) );
    }
    //��������ɨ������
    return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// �������ƣ�CTWSDNetPay_DllApp::AgencyConstructCancelData
//
// ����������������۳������ݽṹ��
//
// ����������޲���
// ��д��Ա��ROCY
// ��дʱ�䣺2017��8��17��
// �޸���Ա��
// �޸�ʱ�䣺
// �����汾��1.0.0.1
// ��ע˵����
//
////////////////////////////////////////////////////////////////////////////////////////////////
int CTWSDNetPay_DllApp::AgencyConstructCancelData(IN const CString& strPayIdentifier)
{
    //�ж��Ƿ����ô��۶�̬��
    if (!m_bUseAgencyDll)
    {
        return false;
    }

    //��ȡ�����Ϣʧ�ܣ����г�������ת��ɨ��
    CMarkup xmlCancel;
    xmlCancel.SetDoc( _T("<?xml version=\"1.0\" encoding=\"GB2312\"?>\r\n") );
    xmlCancel.AddElem( _T("DebitCancelParamContext") );//root
    xmlCancel.IntoElem();

    xmlCancel.AddElem( _T("Version"),       _T("1.0") );
    xmlCancel.AddElem( _T("TimeOut"),       300 );

    xmlCancel.AddElem( _T("Data") );
    xmlCancel.IntoElem();
    xmlCancel.AddElem( _T("TradeKeyItem") );
    xmlCancel.IntoElem();

    xmlCancel.AddElem( _T("Type"),          1 );
    xmlCancel.AddElem( _T("PayIdentifier"), strPayIdentifier);

    xmlCancel.OutOfElem();
    xmlCancel.OutOfElem();

    CString strCancel = xmlCancel.GetDoc();
    int     nCancelLength = strCancel.GetLength();
    return m_AgencyPay.AgencyDebitCancel( strCancel.GetBuffer(0), nCancelLength, 1);
}


////////////////////////////////////////////////////////////////////////////////////////////////
// �������ƣ�CTWSDNetPay_DllApp::GetAgencyResult
//
// �����������������۵ķ��ؽ���ַ������γɸ�ʽ�ַ���
//
// ���������CString strXML
// ���������int
// ��д��Ա��ROCY
// ��дʱ�䣺2017��8��17��
// �޸���Ա��
// �޸�ʱ�䣺
// �����汾��1.0.0.1
// ��ע˵����
//
////////////////////////////////////////////////////////////////////////////////////////////////
int CTWSDNetPay_DllApp::GetAgencyResult(CString strXML, DWORD dwConsumeTime, int&debitType)
{
    int iReturn = -99;
    CString strTemp = _T("");
    CMarkup xml;
    try
    {
        if ( xml.SetDoc( strXML ) )
        {
            if ( !xml.FindElem(_T("DebitResultContext")) )
            {
                throw - 2;
            }
            xml.IntoElem();
            xml.ResetMainPos();

            if ( !xml.FindElem(_T("Data")) )
            {
                throw - 3;
            }
            xml.IntoElem();

            xml.ResetMainPos();
            if (!xml.FindElem(_T("ResultType")))
                throw - 11;
            m_sCurrentDebitResult.ResultType = 0;

            xml.ResetMainPos();
            if (!xml.FindElem(_T("ExecuteCode")))
                throw - 12;
            m_sCurrentDebitResult.ExecuteCode = 0;

            xml.ResetMainPos();
            if (!xml.FindElem(_T("ExecuteDesc")))
                throw - 13;
            ::_stprintf(m_sCurrentDebitResult.ExecuteDesc, _T("%s"), xml.GetData() );

            if ( !xml.FindElem(_T("TradeKeyItem")) )
            {
                throw - 14;
            }
            xml.IntoElem();

            xml.ResetMainPos();
            if (!xml.FindElem(_T("TradeSecurityCode")))
                throw - 15;
            ::_stprintf(m_sCurrentDebitResult.TradeSecurityCode, _T("%s"), xml.GetData() );

            xml.ResetMainPos();
            if (!xml.FindElem(_T("PayPlatformType")))
                throw - 16;
            int iPayChennelType = _ttoi(xml.GetData());
            debitType = iPayChennelType;
            //�ж��Ƿ����֧��
            switch (iPayChennelType)
            {
            case 0x07:
                m_sCurrentDebitResult.PayPlatformType = PCTC_ALIIPAY_AGENCY;
                break;
            case 0x08:
                m_sCurrentDebitResult.PayPlatformType = PCTC_WEIXIN_AGENCY;
                break;
            case 0x09:
                m_sCurrentDebitResult.PayPlatformType = PCTC_BAIDU_AGENCY;
                break;
            case 0x0A:
                m_sCurrentDebitResult.PayPlatformType = PCTC_JD_AGENCY;
                break;
            case 0x0B:
                m_sCurrentDebitResult.PayPlatformType = PCTC_UNIPAY_AGENCY;
                break;
            case 0x11:
                m_sCurrentDebitResult.PayPlatformType = PCTC_ICBC_AGENCY;
                break;
            case 0x13:
                m_sCurrentDebitResult.PayPlatformType = PCTC_VIRTUALCARD;
                break;
            }

            //�ж��Ƿ񽫴���֧���ϲ����ƶ�֧��ԭ�ж��嵱��
            if (m_bReturnPayChannelCodeUnify)
            {
                switch (m_sCurrentDebitResult.PayPlatformType)
                {
                case PCTC_WEIXIN_AGENCY:
                    m_sCurrentDebitResult.PayPlatformType = PCTC_WENXIN;
                    break;
                case PCTC_ALIIPAY_AGENCY:
                    m_sCurrentDebitResult.PayPlatformType = PCTC_ALIIPAY;
                    break;
                case PCTC_BAIDU_AGENCY:
                    m_sCurrentDebitResult.PayPlatformType = PCTC_BAIDU;
                    break;
                case PCTC_JD_AGENCY:
                    m_sCurrentDebitResult.PayPlatformType = PCTC_JD;
                    break;
                case PCTC_UNIPAY_AGENCY:
                    m_sCurrentDebitResult.PayPlatformType = PCTC_UNIPAY_QR;
                    break;
                }
            }
            else
            {
                if(m_sTranstParam.ProvinceID == 41)
                {
                    if(m_sCurrentDebitResult.PayPlatformType==PCTC_ALIIPAY_AGENCY)
                    {
                        m_sCurrentDebitResult.PayPlatformType=PCTC_WEIXIN_AGENCY;
                    }
                    else if(m_sCurrentDebitResult.PayPlatformType==PCTC_WEIXIN_AGENCY)
                    {
                        m_sCurrentDebitResult.PayPlatformType=PCTC_ALIIPAY_AGENCY;   
                    }
                }
            }

            //add 2018-10
            bool is_jilin = (m_sTranstParam.ProvinceID == 22);//����ʡ

            if ( is_jilin &&
                    (m_sCurrentDebitResult.PayPlatformType == PCTC_ICBC_AGENCY)
               )
            {
                m_sCurrentDebitResult.PayPlatformType = PCTC_ALIIPAY;
                //GetGlobalApp()->RecordLog("����ʡ�ݣ�����֧�����ݹ��ൽ֧����");
            }

            xml.ResetMainPos();
            if (!xml.FindElem(_T("PayIdentifier")))
                throw - 17;
            ::_stprintf(m_sCurrentDebitResult.PayIdentifier, _T("%s"), xml.GetData() );

            xml.ResetMainPos();
            if (!xml.FindElem(_T("DebitOrder")))
                throw - 18;
            ::_stprintf(m_sCurrentDebitResult.DebitOrder, _T("%s"), xml.GetData() );

            xml.ResetMainPos();
            if (!xml.FindElem(_T("DebitTime")))
                throw - 19;
            m_sCurrentDebitResult.DebitTime = CString2time_t( xml.GetData() );

            //��ȡ��������ʱ��
            m_sCurrentDebitResult.ConsumeTime = dwConsumeTime;

            //��������
            m_sCurrentDebitResult.ConsumeTime   = (::GetTickCount() - m_dwTradeBeginOvertime);
            m_sCurrentDebitResult.Valid         = TRUE;

            //��ʶ�۷��������
            GetGlobalApp()->m_bIsTrading        = FALSE;
            GetGlobalApp()->m_bCurrentDebitMode = FALSE;
            //���ش���
            iReturn = 0;
        }
        else
        {
            throw - 1;
        }
    }
    catch (int& iErrorCode)
    {
        iReturn = iErrorCode;
        switch (iErrorCode)
        {
        case -1:
            m_strLastErrorDesc.Format(_T("���۽������ʧ�ܣ�����XML���쳣"));
            break;
        case -2:
            m_strLastErrorDesc.Format(_T("���۽������ʧ�ܣ�δ�ҵ�XMLͷ DebitResultContext"));
            break;
        case -3:
            m_strLastErrorDesc.Format(_T("���۽������ʧ��, δ�ҵ�ָ�������� Data"));
            break;
        case -11:
            m_strLastErrorDesc.Format(_T("���۽������ʧ��, δ�ҵ�ָ�������� ResultType"));
            break;
        case -12:
            m_strLastErrorDesc.Format(_T("���۽������ʧ��, δ�ҵ�ָ�������� ExecuteCode"));
            break;
        case -13:
            m_strLastErrorDesc.Format(_T("���۽������ʧ��, δ�ҵ�ָ�������� ExecuteDesc"));
            break;
        case -14:
            m_strLastErrorDesc.Format(_T("���۽������ʧ��, δ�ҵ�ָ�������� TradeKeyItem"));
            break;
        case -15:
            m_strLastErrorDesc.Format(_T("���۽������ʧ��, δ�ҵ�ָ�������� TradeSecurityCode"));
            break;
        case -16:
            m_strLastErrorDesc.Format(_T("���۽������ʧ��, δ�ҵ�ָ�������� PayPlatformType"));
            break;
        case -17:
            m_strLastErrorDesc.Format(_T("���۽������ʧ��, δ�ҵ�ָ�������� PayIdentifier"));
            break;
        case -18:
            m_strLastErrorDesc.Format(_T("���۽������ʧ��, δ�ҵ�ָ�������� DebitOrder"));
            break;
        case -19:
            m_strLastErrorDesc.Format(_T("���۽������ʧ��, δ�ҵ�ָ�������� DebitTime"));
            break;

        default:
            m_strLastErrorDesc.Format(_T("���۽������ʧ��, δ֪�쳣"));
            break;
        }
        RecordLog( m_strLastErrorDesc );
    }

    return iReturn;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// �������ƣ�CTWSDNetPay_DllApp::Hex2Bin
//
// ������������16���Ƶ��ַ���ת����2���Ƶ�����
//
// ���������IN LPTSTR lpHex   -- ��Ҫת����16�����ַ���
//         ��OUT PBYTE pBin        -- �����2������������
//         ��IN DWORD dwHex_len    -- ��Ҫת����16�������ݳ���,����Ϊ2�ı���
// ���������void
// ��д��Ա��ROCY
// ��дʱ�䣺2017��9��21��
// �޸���Ա��
// �޸�ʱ�䣺
// �����汾��1.0.0.1
// ��ע˵�����������16�����ַ������ȱ�����2�ı���
//
////////////////////////////////////////////////////////////////////////////////////////////////
void CTWSDNetPay_DllApp::Hex2Bin(IN LPTSTR lpHex, OUT PBYTE pBin, IN DWORD dwHex_len)
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

////////////////////////////////////////////////////////////////////////////////////////////////
// �������ƣ�CTWSDNetPay_DllApp::Bin2Hex
//
// ������������2���Ƶ�����ת����16���Ƶ��ַ���
//
// ���������IN PBYTE pBin         -- ��Ҫת����2������������
//         ��IN DWORD dwBin_Len        -- ��Ҫת����2�����������鳤��
// ���������CString  �ǿ� -- ת���ɹ��������16�����ַ���;NULL -- ת��ʧ��
// ��д��Ա��ROCY
// ��дʱ�䣺2017��9��21��
// �޸���Ա��
// �޸�ʱ�䣺
// �����汾��1.0.0.1
// ��ע˵����
//
////////////////////////////////////////////////////////////////////////////////////////////////
CString CTWSDNetPay_DllApp::Bin2Hex(IN PBYTE pBin, IN DWORD dwBin_Len)
{
    CString strHex;
    CString strTemp;
    for ( DWORD i = 0; i < dwBin_Len; ++i )
    {
        strTemp.Format(_T("%02X"), pBin[i] );
        strHex += strTemp;
    }
    return strHex;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// �������ƣ�CTWSDNetPay_DllApp::HexToBase64
//
// ����������ת��16�����ַ���Ϊbase64����
//
// ���������IN const CString& strSrcHex
//         ��OUT CString& strDesBase64
// ���������BOOL
// ��д��Ա��ROCY
// ��дʱ�䣺2017��9��21��
// �޸���Ա��
// �޸�ʱ�䣺
// �����汾��1.0.0.1
// ��ע˵�����ڲ��ӿ�
//
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CTWSDNetPay_DllApp::HexToBase64(IN const CString& strSrcHex, OUT CString& strDesBase64)
{
    BOOL bReturn = FALSE;
    try
    {
        if (0 == strSrcHex.GetLength())
            throw - 1;

        if (0  != (strSrcHex.GetLength() % 2))
            throw - 1;

        unsigned int uSize = strSrcHex.GetLength();
        BYTE *pTemp = new BYTE[uSize / 2 + 1];
        memset( pTemp, 0, (uSize / 2 + 1)*sizeof(BYTE) );
        for (unsigned int i = 0; i < uSize; i += 2)
        {
            unsigned char tempC = 0, bcd = 0;
            tempC = strSrcHex.GetAt(i);
            if (tempC >= 'a' && tempC <= 'f') tempC = tempC - 'a' + 'A';
            bcd = ( tempC >= 'A' ? tempC - 'A' + 0x0a : tempC - '0') << 4;
            tempC = strSrcHex.GetAt(i + 1);
            if (tempC >= 'a' && tempC <= 'f') tempC = tempC - 'a' + 'A';
            bcd += tempC >= 'A' ? tempC - 'A' + 0x0a : tempC - '0';
            pTemp[i / 2] = bcd;
        }
        char *pOutBuffer = new char[uSize];
        int  iOutDataLen = 0x00;
        memset(pOutBuffer, 0, uSize);

        iOutDataLen = BASE64_Encode(pTemp, uSize / 2, pOutBuffer);

        pOutBuffer[iOutDataLen] = 0;

        strDesBase64.Format("%s", pOutBuffer);

        //������Դ
        if (NULL != pOutBuffer)
            delete []pOutBuffer;
        pOutBuffer = NULL;

        if (NULL != pTemp)
            delete []pTemp;
        pTemp = NULL;

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
// �������ƣ�CTWSDNetPay_DllApp::Base64ToHex
//
// ����������ת��base64����Ϊ16�����ַ�
//
// ���������IN const CString& strSrcBase64
//         ��OUT CString& strDesHex
// ���������BOOL
// ��д��Ա��ROCY
// ��дʱ�䣺2017��9��22��
// �޸���Ա��
// �޸�ʱ�䣺
// �����汾��1.0.0.1
// ��ע˵����
//
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CTWSDNetPay_DllApp::Base64ToHex(IN const CString& strSrcBase64, OUT CString& strDesHex)
{
    BOOL bReturn = FALSE;
    try
    {
        //�жϳ���
        if ( 0 != (strSrcBase64.GetLength() % 2))
            throw - 1;
        if (0 == strSrcBase64.GetLength())
            throw - 2;

        char szSrc[5120] = {0};
        char szDes[5120] = {0};
        CString strTemp(strSrcBase64);
        strcpy_s(szSrc, _countof(szSrc), strTemp.GetBuffer(0)); strTemp.ReleaseBuffer();

        BYTE input[4] = {0};
        int iSrcLen = strSrcBase64.GetLength();
        int i = 0;
        int index = 0;
        for (i = 0; i < iSrcLen; i += 4)
        {
            //byte[0]
            input[0] = Decode_GetByte(szSrc[i]);
            input[1] = Decode_GetByte(szSrc[i + 1]);
            szDes[index++] = (input[0] << 2) + (input[1] >> 4);

            //byte[1]
            if (szSrc[i + 2] != '=')
            {
                input[2] = Decode_GetByte(szSrc[i + 2]);
                szDes[index++] = ((input[1] & 0x0f) << 4) + (input[2] >> 2);
            }

            //byte[2]
            if (szSrc[i + 3] != '=')
            {
                input[3] = Decode_GetByte(szSrc[i + 3]);
                szDes[index++] = ((input[2] & 0x03) << 6) + (input[3]);
            }
        }

        //null-terminator
        szDes[index] = 0;

        strDesHex = Bin2Hex((BYTE*)szDes, index);

        bReturn = TRUE;
    }
    catch (int& iErrorCode)
    {
    }
    return bReturn;
}


////////////////////////////////////////////////////////////////////////////////////////////////
// �������ƣ�CTWSDNetPay_DllApp::BASE64_Encode
//
// ��������������ת����������
//
// ���������const BYTE* inputBuffer
//         ��INT inputCount
//         ��TCHAR* outputBuffer
// ���������INT
// ��д��Ա��ROCY
// ��дʱ�䣺2017��9��21��
// �޸���Ա��
// �޸�ʱ�䣺
// �����汾��1.0.0.1
// ��ע˵����
//
////////////////////////////////////////////////////////////////////////////////////////////////
INT CTWSDNetPay_DllApp::BASE64_Encode(const BYTE* inputBuffer, INT inputCount, TCHAR* outputBuffer)
{
    INT  i  = 0x00;
    BYTE b0 = 0x00;
    BYTE b1 = 0x00;
    BYTE b2 = 0x00;

    CHAR* DATA_BIN2ASCII = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    if ((inputBuffer == NULL) || (inputCount < 0))
    {
        return -1;  // ��������
    }

    if (outputBuffer != NULL)
    {
        for (i = inputCount; i > 0; i -= 3)
        {
            if (i >= 3)
            {   // ��3�ֽ�����ת����4��ASCII�ַ�
                b0 = *inputBuffer++;
                b1 = *inputBuffer++;
                b2 = *inputBuffer++;

                *outputBuffer++ = DATA_BIN2ASCII[b0 >> 2];
                *outputBuffer++ = DATA_BIN2ASCII[((b0 << 4) | (b1 >> 4)) & 0x3F];
                *outputBuffer++ = DATA_BIN2ASCII[((b1 << 2) | (b2 >> 6)) & 0x3F];
                *outputBuffer++ = DATA_BIN2ASCII[b2 & 0x3F];
            }
            else
            {
                b0 = *inputBuffer++;
                if (i == 2)b1 = *inputBuffer++; else b1 = 0;

                *outputBuffer++ = DATA_BIN2ASCII[b0 >> 2];
                *outputBuffer++ = DATA_BIN2ASCII[((b0 << 4) | (b1 >> 4)) & 0x3F];
                *outputBuffer++ = (i == 1) ? TEXT('=') : DATA_BIN2ASCII[(b1 << 2) & 0x3F];
                *outputBuffer++ = TEXT('=');
            }
        } // End for i

        *outputBuffer++ = TEXT('/0');  // ����ַ����������
    }

    return ((inputCount + 2) / 3) * 4;  // ������Ч�ַ�����
}

BYTE CTWSDNetPay_DllApp::Decode_GetByte(char c)
{
    if (c == '+')
        return 62;
    else if (c == '/')
        return 63;
    else if (c <= '9')
        return (BYTE)(c - '0' + 52);
    else if (c == '=')
        return 64;
    else if (c <= 'Z')
        return (BYTE)(c - 'A');
    else if (c <= 'z')
        return (BYTE)(c - 'a' + 26);
    return 64;
}

//delete 2018-05 ��ʱ�������룬�Է�ֹ�ⲿ���
// int CTWSDNetPay_DllApp::SelfServiceTerminalInterrupt(int destKey,const char* para,int size,int srcKey)
// {
//  Json::Value rootN;
//  Json::Reader jsonReader;
//  if(!jsonReader.parse(para,rootN))
//  {
//      return -1;
//  }
//  bool readSuccess =false;
//  int type;
//  readSuccess = ReadJsonInt(rootN,"type",type);
//  if(!readSuccess)
//  {
//      return -1;
//  }
//  if(type==1)
//  {
//      Json::Value rspRoot;
//      Json::FastWriter jsonWriter;
//      rspRoot["type"]=Json::Value(1);

//      Json::Value paraData;
//      if(!jsonReader.parse(m_CommonInitPara,paraData))
//      {
//          return -1;
//      }
//      rspRoot["data"]= paraData;
//      std::string rspStr = jsonWriter.write(rspRoot);
//      //MessageBox(0,rspStr.c_str(),"",0);
//      char * para_msg=  new char[1000];
//         memset(para_msg,0,1000);
//         strncpy(para_msg,rspStr.c_str(),500);
//         send2Bus(srcKey,para_msg,strlen(para_msg),destKey) ;

//         delete para_msg;
//         para_msg=NULL;
//  }
//  else if(type==2)
//  {
//      Json::Value imageIdN = rootN["data"];
//      std::string imageId;
//      readSuccess = ReadJsonStr(imageIdN,"imageId",imageId);
//      if(!readSuccess)
//      {
//          return -1;
//      }
//      m_LPR.SaveImageId(imageId);

//  }
//     return 0;
// }
/**
 * ����Ӳ�����ݲ�ѯ
 * @param  para
 * @return
 */
int CTWSDNetPay_DllApp::DevcieQueryInterrupt(const char* para)
{
    //��ѯNFC����״̬
    bool nfc_link_success = GetGlobalApp()->m_NfcLinkSuccess;

    //��ѯNFCǩ��״̬
    bool nfc_signin_success = GetGlobalApp()->m_bNFCSignInSuccess;

    //��ȡ�ǽ�ģ���豸��Ϣ
    if (nfc_link_success && nfc_signin_success && (GetGlobalApp()->m_NfcPara.SerialNumber[0] == '\0' || GetGlobalApp()->m_NfcPara.SoftVersion[0] == '\0' ) )
    {
        CNFCProtocol tmp_pro;
        //GetGlobalApp()->m_NfcPara.reset();


        m_pThreadManage->NFCParaQuery(tmp_pro.ConstructSoftVerQueryCmd());
        Sleep(100);
        m_pThreadManage->NFCParaQuery(tmp_pro.ConstructParaQueryCmd());
        int sleepUnit = 100;
        int sleepCount = 20;
        for (int i = 0; i < sleepCount; ++i)
        {
            if (GetGlobalApp()->m_NfcPara.valid)
            {
                break;
            }
            Sleep(sleepUnit);
        }
    }

    Json::Value rootN;
    rootN["LinkSuccess"] = Json::Value(nfc_link_success);
    rootN["SignSuccess"] = Json::Value(nfc_signin_success);
    rootN["NFCCompany"] = Json::Value(GetGlobalApp()->m_NfcPara.Company);
    rootN["NFCType"] = Json::Value(GetGlobalApp()->m_NfcPara.Type);
    rootN["NFCSerialNo"] = Json::Value(GetGlobalApp()->m_NfcPara.SerialNumber);
    rootN["SoftVersion"] = Json::Value(GetGlobalApp()->m_NfcPara.SoftVersion);
    rootN["TerminalNo"] = Json::Value(GetGlobalApp()->m_NfcPara.TerminalNo);
    rootN["MerchantCode"] = Json::Value(GetGlobalApp()->m_NfcPara.MerchantCode);

    Json::FastWriter JFW;
    std::string msg = JFW.write(rootN);
    char msg_chs[1000] = {0};
    strcpy(msg_chs, msg.c_str());

    theApp.RecordLog(msg_chs);

    send2Bus(KEY_DEVICE_TEST, msg_chs, msg.size(), KEY_BUSINESS);
    return 0;
}
int CTWSDNetPay_DllApp::recvFromBus(int destKey, char *msg, int len, int srcKey )
{
    {
        CScopeLock dll_lock(&theApp.m_DllAccessMutex);
        if (m_MessageCenter)
        {
            m_MessageCenter->NewMessage(destKey, msg, len, srcKey);
        }

    }
    if (srcKey == KEY_COMMON_FRAME)
    {
        int pay_diff = time(NULL) - m_lastPayTime;
        char chs_debug[10] = {0};
        sprintf(chs_debug, "%d", pay_diff);

        if (  pay_diff > 60) //
        {

            return 1;
        }
    }
    else if (srcKey == KEY_DEVICE_TEST)
    {
        //Ӳ�����Բ�ѯ
        DevcieQueryInterrupt(msg);
    }
    else if (srcKey == KEY_COMMONBUS_ACCESS)
    {
        /*
        clock_t before_send = clock();
         SelfServiceTerminalInterrupt(destKey,msg,len,srcKey);
        clock_t after_send = clock();
        CString debug_info;
        debug_info.Format("about recv %d  -->  %d",before_send,after_send);
        */
        //RecordLog(msg);

    }
    return 0;
}

//
//CommonBus*  WINAPI CTWSDNetPay_DllApp::getInstance()
//{
//    return static_cast<CommonBus*>(GetGlobalApp());
//}
//CommonBus*  WINAPI CTWSDNetPay_DllApp::deleteInstance()
//{
//    return NULL;/*do nothing*/
//    // return static_cast<CommonBus*>GetGlobalApp();
//}



int RealCalTimeZone()
{
    struct tm time_local, time_utc;
    time_t t;
    t = time(NULL);
    time_local = *localtime(&t);
    time_utc = *gmtime(&t);
    return  ((time_local.tm_mday - time_utc.tm_mday) * 24 +  time_local.tm_hour - time_utc.tm_hour);
}

//����������8��
int GetCurrentTimeZone()
{
    static int time_zone = RealCalTimeZone();
    return time_zone;
}



CTime CTWSDNetPay_DllApp::FixUTC8(const CTime& old_time)
{
    if (m_sTranstParam.LaneConfigInfo.ForceUTC8 == 0)
    {
        return old_time;
    }
    int time_diff_hour = 8 - GetCurrentTimeZone();
    CTime new_time ;
    if (time_diff_hour >= 0)
    {
        new_time = old_time - CTimeSpan(0, time_diff_hour, 0, 0);
    }
    else
    {
        new_time = old_time + CTimeSpan(0, -time_diff_hour, 0, 0);
    }


    return new_time;
}

time_t CTWSDNetPay_DllApp::FixUTC(const time_t& old_time)
{
    if (m_sTranstParam.LaneConfigInfo.ForceUTC8 == 0)
    {
        return old_time;
    }
    time_t new_time = old_time + (8 - GetCurrentTimeZone()) * 60 * 60;
    return new_time;
}

CTime CTWSDNetPay_DllApp::FixUTC(const CTime& old_time)
{
    if (m_sTranstParam.LaneConfigInfo.ForceUTC8 == 0)
    {
        return old_time;
    }
    CTime new_time ;
    int time_diff_hour = 8 - GetCurrentTimeZone();

    if (time_diff_hour >= 0)
    {
        new_time = old_time + CTimeSpan(0, time_diff_hour, 0, 0);
    }
    else
    {
        new_time = old_time - CTimeSpan(0, -time_diff_hour, 0, 0);
    }
    return new_time;
}

int CTWSDNetPay_DllApp::QueryImageIdFromLPRSystem(std::string& image_id, std::string& task_id)
{
    if (!m_LPR.QueryImageIDAndTaskID(image_id, task_id))
    {
        return -1;
    }
    return 0;
}

//////////////////////////////////////////////////////////////
// ��������:CTWSDNetPay_DllApp::QueryImageIdFromLPRSystem
//
// ������������ȡ����ʶ��ϵͳ�õ���ͼ��ID
//
// �������:
// �������:int
// ��д��Ա:���Ǳ�
// ��дʱ��:2018-04-23
// �޸���Ա:
// �޸�ʱ��:
// �����汾:1.0.0.0
// ��ע˵��:
// 1���˹��̻��Զ���ʱ��ĿǰΪ100ms
//
//////////////////////////////////////////////////////////////
int CTWSDNetPay_DllApp::QueryImageIdFromLPRSystem()
{
    m_pCurrentDebitInfo.imageId = _T("");
    m_pCurrentDebitInfo.taskId = _T("");
    std::string imageId, taskId;
    if (!m_LPR.QueryImageIDAndTaskID(imageId, taskId))
    {
        return -1;
    }
    m_pCurrentDebitInfo.imageId.Format(_T("%s"), imageId.c_str());
    m_pCurrentDebitInfo.taskId.Format(_T("%s"), taskId.c_str());
    return 0;
    //delete 2018-05 ����Ĵ��룬�Ѿ���Ч����ʱ����������Ϊ�ⲿ��������ʱ�ᷢ���仯���Է�����
    // m_LPR.Reset();
    // //������Ϣ
    // static char *chs= "{\"type\":2,\"data\":\"\"}";
    // clock_t before_send = clock();
    // //GetGlobalApp()->send2Bus(KEY_COMMONBUS_ACCESS,chs,strlen(chs),KEY_BUSINESS);
    // clock_t after_send = clock();
    // CString debug_info;
    // debug_info.Format("about send %d  -->  %d",before_send,after_send);
    // //RecordLog(debug_info);
    // m_pThreadManage->PostThreadMessage(WM_THREAD_LPR_IMAGEID, (WPARAM)0, (LPARAM)0);//ʹ���첽���ͣ����첽����������manage�̣߳����ܴ�������
    // static int queryOverCount = GetLPRQueryOverTimeMilli()/10;
//    for(int i=0;i< queryOverCount ;++i)//ѭ���ȴ�
//    {
//        if(m_LPR.CaptureSuccess())
//        {
//            break;
//        }
//        Sleep(10);
//    }
//    if(m_LPR.CaptureSuccess())
//    {
    //  m_pCurrentDebitInfo.imageId.Format(_T("%s"),m_LPR.Value().c_str());
//    }
    // else
    // {
    //  RecordLog("��ȡ����ͼ���ʶ��ʱ");
    // }
    return 0;
}

void CTWSDNetPay_DllApp::SetLPRQueryOverTimeMilli(int LPRQueryOverTimeMilli)
{
    if (LPRQueryOverTimeMilli < 100)
    {
        m_LPRQueryOverTimeMilli = 100;
    }
    else if (LPRQueryOverTimeMilli > 1000)
    {
        m_LPRQueryOverTimeMilli = 1000;
    }
    else
    {
        m_LPRQueryOverTimeMilli = LPRQueryOverTimeMilli;
    }
    CString debug_info;
    debug_info.Format("����ʶ��ʱʱ������:%dms", m_LPRQueryOverTimeMilli);
    RecordLog(debug_info);
}

int CTWSDNetPay_DllApp::GetLPRQueryOverTimeMilli()
{
    return m_LPRQueryOverTimeMilli;
}




void CTWSDNetPay_DllApp::GenerateRawInitPara(const tagTranstParam& para)
{
//    Json::Value rootN;
    // rootN["AreaID"] = Json::Value(para.LaneConfigInfo.str_AreaID);
    // rootN["RoadID"] = Json::Value(para.LaneConfigInfo.str_RoadID);
    // rootN["StationID"] = Json::Value(para.LaneConfigInfo.str_StationID);
    // rootN["StationName"] = Json::Value(para.LaneConfigInfo.StationTab);
    // rootN["szLocalLaneID"] = Json::Value(para.LaneConfigInfo.str_LaneID);
    // rootN["ServerIP"] = Json::Value(para.LaneConfigInfo.WebServerIP);
    // rootN["ServerPort"] = Json::Value(para.LaneConfigInfo.WebServerPort);
    // rootN["ProvinceID"] = Json::Value(Int2Str(para.ProvinceID));

//    Json::FastWriter jsonWriter;

    // m_CommonInitPara = jsonWriter.write(rootN);
    LPRInitData lprPara;
    TranstParam2LPRInitPara(para, lprPara);
    std::string str1, str2;
    GetGlobalApp()->m_LPR.WriteInitPara(lprPara);
    //m_LPR.WriteInitPara(lprPara);

}

void CTWSDNetPay_DllApp::LPR_PackPayResult(int LPR)
{
    std::string imageId, taskId, payId;
    int channelType = int(LPR);
    imageId = std::string(LPCTSTR(m_pCurrentDebitInfo.imageId));
    taskId = std::string(LPCTSTR(m_pCurrentDebitInfo.taskId));
    payId = std::string(LPCTSTR(m_pCurrentDebitInfo.PayIdentifier));

    m_LPR.WritePayAbstract(imageId, taskId, payId, channelType);

    //delete 2018-05 ��ʱ����֮ǰ�Ĵ��룬�Է��ⲿҪ��䶯
    // Json::Value rootN;
    // rootN["type"] = Json::Value(3);

    // Json::Value dataN;
    // dataN["imageId"] = Json::Value(m_pCurrentDebitInfo.imageId);
    // dataN["payIdentifier"] = Json::Value(m_pCurrentDebitInfo.PayIdentifier);
    // dataN["payChannel"]= Json::Value(int(LPR));
    // rootN["data"]=dataN;

//    Json::FastWriter jsonWriter;

    // std::string payResult = jsonWriter.write(rootN);
    // char chs[1000]={0};
    // strncpy(chs,payResult.c_str(),800);
    // //MessageBox(0,chs,"",0);
    // send2Bus(KEY_COMMONBUS_ACCESS,chs,strlen(chs),KEY_BUSINESS);
}

void CTWSDNetPay_DllApp::ResetDebitResult()
{
    GetGlobalApp()->m_sCurrentDebitResult.Valid = FALSE;
    GetGlobalApp()->m_sCurrentDebitResult.ExecuteCode = 255;
    memset(GetGlobalApp()->m_sCurrentDebitResult.PayIdentifier, 0, sizeof(GetGlobalApp()->m_sCurrentDebitResult.PayIdentifier));
    memset(&GetGlobalApp()->m_sCurrentDebitResult.ExpandETCData, 0, sizeof(ETCCardData));
    GetGlobalApp()->m_sCurrentDebitResult.ExpandETCData.ETCExcuteCode = 255;

}



bool CTWSDNetPay_DllApp::IsProvinceTianJin()
{
    bool is_tianjin = (m_sTranstParam.ProvinceID == 12);
    return is_tianjin;
}

void CTWSDNetPay_DllApp::GetETCDataFromAgencyResult(const char* result)
{
    //GetGlobalApp()->RecordLog(result);
    if (result == NULL)
    {
        return ;
    }

    bool is_tianjin = (m_sTranstParam.ProvinceID == 12);//�Ƿ������
    if (!is_tianjin)
    {
        return ;
    }

    CMarkup xml;
    xml.SetDoc(result);
    bool read_success = true;
    read_success = read_success ? IntoXMLSubNode(xml, "DebitResultContext") : false;
    read_success = read_success ? IntoXMLSubNode(xml, "Data") : false;


    tagDebitResultItems& currDebitResultRef = GetGlobalApp()->m_sCurrentDebitResult;

    int etc_excute_code = 0;
    read_success = read_success ? ReadXMLInt(xml, "ExecuteCode", etc_excute_code) : false;

    read_success = read_success ? IntoXMLSubNode(xml, "TradeKeyItem") : false;

    ReadXMLCStr(xml, "PayIdentifier", currDebitResultRef.PayIdentifier, 50);
    ReadXMLCStr(xml, "DebitOrder", currDebitResultRef.DebitOrder, 50);

    read_success = read_success ? IntoXMLSubNode(xml, "ETCTradeInfo") : false;

    read_success = read_success ? ReadXMLInt(xml, "IsUseVirtualETCCard", currDebitResultRef.ExpandETCData.IsUseVirtualETCCard) : false;
    read_success = read_success ? ReadXMLCStr(xml, "VirtualETCCardID", currDebitResultRef.ExpandETCData.VirtualETCCardID, 20) : false;
    read_success = read_success ? ReadXMLCStr(xml, "IssueFlag", currDebitResultRef.ExpandETCData.IssueFlag, 10) : false;
    read_success = read_success ? ReadXMLCStr(xml, "IssueIndex", currDebitResultRef.ExpandETCData.IssueIndex, 10) : false;
    read_success = read_success ? ReadXMLCStr(xml, "ETCVehicleLicense", currDebitResultRef.ExpandETCData.ETCVehicleLicense, 16) : false;
    read_success = read_success ? ReadXMLInt(xml, "ETCCardType", currDebitResultRef.ExpandETCData.ETCCardType) : false;
    read_success = read_success ? ReadXMLInt(xml, "ETCBeforeBalance", currDebitResultRef.ExpandETCData.ETCBeforeBalance) : false;
    read_success = read_success ? ReadXMLInt(xml, "ETCAfterBalance", currDebitResultRef.ExpandETCData.ETCAfterBalance) : false;
    read_success = read_success ? ReadXMLInt(xml, "ETCProTradeNo", currDebitResultRef.ExpandETCData.ETCProTradeNo) : false;
    read_success = read_success ? ReadXMLCStr(xml, "ETCTerminalTradeNo", currDebitResultRef.ExpandETCData.ETCTerminalTradeNo, 8) : false;
    read_success = read_success ? ReadXMLInt(xml, "ETCTradeDate", currDebitResultRef.ExpandETCData.ETCTradeDate) : false;
    read_success = read_success ? ReadXMLInt(xml, "ETCTradeTime", currDebitResultRef.ExpandETCData.ETCTradeTime) : false;
    read_success = read_success ? ReadXMLCStr(xml, "ETCTac", currDebitResultRef.ExpandETCData.ETCTac, 8) : false;


    strncpy(currDebitResultRef.PayIdentifier, m_pCurrentDebitInfo.PayIdentifier, 50);
    if (read_success)
    {
        currDebitResultRef.Valid = TRUE;
    }
    currDebitResultRef.ExpandETCData.ETCExcuteCode = 0;
    switch (etc_excute_code)
    {
    case 0:
        currDebitResultRef.ExpandETCData.ETCExcuteCode = 0;
        break;
    case 22:
        currDebitResultRef.ExpandETCData.ETCExcuteCode = 1;
        break;
    case 25:
        currDebitResultRef.ExpandETCData.ETCExcuteCode = 3;
        break;
    case 26:
        currDebitResultRef.ExpandETCData.ETCExcuteCode = 5;
        break;
    case 27:
        currDebitResultRef.ExpandETCData.ETCExcuteCode = 4;
        break;
    //case 31:
    //  currDebitResultRef.ExpandETCData.ETCExcuteCode = 2;
    //  break;
    default:
        currDebitResultRef.ExpandETCData.IsUseVirtualETCCard = 0;
        char err_log[100] = {0};
        sprintf(err_log, "���⿨ִ������������%d", etc_excute_code);
        GetGlobalApp()->RecordLog(err_log);
        break;
    }

}

int CTWSDNetPay_DllApp::NotifyPayStatus(int type, int value)
{
    //Ŀǰtype���Ǵ��ۣ���������
    if (!IsProvinceTianJin())
    {
        return -1;
    }

    int result_val = 255;
    if (type == 1)
    {
        switch (value)
        {
        case 12:
            result_val = 1;
            break;
        case 21:
            result_val = 2;
            break;
        case 22:
            result_val = 3;
            break;
        case 25:
            result_val = 4;
            break;
        case 26:
            result_val = 5;
            break;
        case 27:
            result_val = 6;
            break;
        }
    }
    NotifyCaller(5, result_val);
    return 0;
}

//add 2018-03���˴�����commonbus�ļ��ݴ���

int recvFromBus_X(int destKey, char *msg, int len, int srcKey)
{
    return GetGlobalApp()->recvFromBus(destKey, msg, len, srcKey);
}




void CTWSDNetPay_DllApp::ConnectBus()
{
    std::string base_dir = fs_thisModulePath();//pystring::os::path::dirname(fs_GetModulePath());
    std::string commonBusPath = pystring::os::path::join( base_dir, "CommonBus.dll");
    HMODULE tmp_module = ::LoadLibrary(commonBusPath.c_str());
    if (tmp_module == NULL)
    {
        RecordLog("Connect CommonBus Fail");
        return ; //no such dll
    }
    RecordLog("Connect CommonBus Success");

    m_connect_bus_core = (NewBusConnector)::GetProcAddress(tmp_module, "NewBusConnector");
    m_disconnect_bus_core = (RemoveBusConnector)::GetProcAddress(tmp_module, "RemoveBusConnector");
    m_send_core = (send2Bus_Target)::GetProcAddress(tmp_module, "send2Bus_Target");
    m_register_core = (register2Bus_Target)::GetProcAddress(tmp_module, "register2Bus_Target");
    m_unRegister_core = (unRegister2Bus_Target)::GetProcAddress(tmp_module, "unRegister2Bus_Target");
    if (m_connect_bus_core != NULL)
    {
        m_bushandle_core = m_connect_bus_core(recvFromBus_X);
    }
    if (m_send_core == NULL
            || m_register_core == NULL
            || m_unRegister_core == NULL)
    {
        ;
    }
    m_commonbus_module = tmp_module;


}

void CTWSDNetPay_DllApp::DisConnectBus()
{
    if (m_disconnect_bus_core != NULL)
    {
        m_disconnect_bus_core(m_bushandle_core);
    }
}
void CTWSDNetPay_DllApp::register2Bus(int key)
{
    if (m_register_core != NULL)
    {
        m_register_core(m_bushandle_core, key);
    }
}
void CTWSDNetPay_DllApp::unRegister2Bus(int key)
{
    if (m_unRegister_core != NULL)
    {
        m_unRegister_core(m_bushandle_core, key);
    }

}
int CTWSDNetPay_DllApp::send2Bus(int destKey, char *msg, int len, int srcKey)
{
    DataForwardWrite(destKey, msg, len);
    if (m_send_core != NULL)
    {

        return m_send_core(m_bushandle_core, destKey, msg, len, srcKey);
    }
    return -1;
}




void TranstParam2LPRInitPara(const tagTranstParam&src, LPRInitData& dest)
{
    dest.ProvinceID = src.ProvinceID;
    dest.AreaID = std::string(src.LaneConfigInfo.str_AreaID);
    dest.RoadID = std::string(src.LaneConfigInfo.str_RoadID);
    dest.StationID = std::string(src.LaneConfigInfo.str_StationID);
    dest.StationName = std::string(src.LaneConfigInfo.StationTab);
    dest.LaneID = std::string(src.LaneConfigInfo.str_LaneID);
    dest.WebServerIP = std::string(src.LaneConfigInfo.WebServerIP);
    dest.WebServerPort = Int2Str(src.LaneConfigInfo.WebServerPort);
}


#define TW_TRACE_DIARY_TEST(x) sys_trace_info_test(x)
void sys_trace_info_test(const std::string& x)
{
    GetGlobalApp()->RecordLog(x.c_str());
}


int CTWSDNetPay_DllApp::CreateTasks()
{
    CScopeLock dll_lock(&m_DllAccessMutex);
    static bool create_success = false;
    if (create_success)
    {
        return -1;
    }
    create_success = true;
    ConnectDataForward();
    m_MessageCenter = new trawe::CComponentMessage(this);
    if (m_MessageCenter)
    {
        m_MessageCenter->Resume();
    }
    else
    {
        TW_TRACE_DIARY("ERROR:Message Center Error");
    }
    return 0;
}

int CTWSDNetPay_DllApp::ExecuteMessage(int src_key, char* msg, int msg_len)
{
    RecordLog("$$$$recv msg\n");
    return 0;
}




void CTWSDNetPay_DllApp::DataForwardRegister( const std::string& topic)
{
    CScopeLock dll_lock(&theApp.m_DllAccessMutex);
    if (m_Forward_RegisterFun == NULL)
    {
        TW_TRACE_DIARY("Error:Can't connect dds");
        return ;
    }
    char topic_chs[300] = {0};
    strncpy(topic_chs, topic.c_str(), 260);

    char info_chs[100] = {0};
    _snprintf(info_chs, 80, "Connect DDS,Key:%d,topic:%s", m_SendId, topic_chs);
    TW_TRACE_DIARY(info_chs);
    m_Forward_RegisterFun(m_SendId, DataForwardRecv , topic_chs);
}
void CTWSDNetPay_DllApp::DataForwardUnregister()
{
    CScopeLock dll_lock(&theApp.m_DllAccessMutex);
    if (m_Forward_UnregisterFun == NULL)
    {
        return ;
    }
    m_Forward_UnregisterFun(m_SendId);
}

void CTWSDNetPay_DllApp::DataForwardWrite(int destKey, char *message, int len, bool pointerFlag)
{
    CScopeLock dll_lock(&theApp.m_DllAccessMutex);
    if (m_Forward_WriteFun == NULL)
    {
        return ;
    }
    m_Forward_WriteFun(destKey, message, len, m_SendId, pointerFlag);
}

void CTWSDNetPay_DllApp::DataForwardRead(int destKey, char *message, int len, int srcKey)
{
    CScopeLock dll_lock(&theApp.m_DllAccessMutex);
    if (m_MessageCenter)
    {
        m_MessageCenter->NewMessage(destKey, message, len, srcKey);
    }
}

void CTWSDNetPay_DllApp::ConnectDataForward()
{
    CScopeLock dll_lock(&theApp.m_DllAccessMutex);
    if (m_Forward_Dll != NULL)
    {
        return ;
    }
    std::string dll_path = pystring::os::path::join(GetDllDir(), "DDsCommonBus.dll");
    m_Forward_Dll = ::LoadLibrary(dll_path.c_str());
    if (m_Forward_Dll == NULL)
    {
        char err_log[1000];
        _snprintf(err_log, 900, "Error:Load DDsCommonBus fail ,path:%s,code:%d", dll_path.c_str(), GetLastError());
        TW_TRACE_DIARY(err_log);
        return ;
    }

    m_Forward_RegisterFun = (DataForward_Register)::GetProcAddress(m_Forward_Dll, "if_register2Bus");
    m_Forward_UnregisterFun = (DataForward_Unregister)::GetProcAddress(m_Forward_Dll, "if_unRegister2Bus");
    m_Forward_WriteFun = (DataForward_Write)::GetProcAddress(m_Forward_Dll, "if_sendToBus");

    return ;
}
void CTWSDNetPay_DllApp::DisonnectDataForward()
{
    DataForwardUnregister();
    CScopeLock dll_lock(&theApp.m_DllAccessMutex);
    //FreeLibrary(m_Forward_Dll);//2019-04 zhuyabing �˴�ж�ش������⣬��ȡ��

    m_Forward_Dll = 0;
    m_Forward_RegisterFun = 0;
    m_Forward_UnregisterFun = 0;
    m_Forward_WriteFun = 0;
    return ;
}



int CTWSDNetPay_DllApp::ETCInterfaceBasicCheck(const char *  szParamContext, const int& iParamSize )
{
    if (!m_bIsInitEnvironment)
    {
        TW_TRACE_DIARY_TYPE("δ���г�ʼ��", TW_TRACE_TYPE_INTERFACE);
        return 255;
    }
    if (!m_bAuthorizedLegal)
    {
        TW_TRACE_DIARY_TYPE("δ��Ȩ", TW_TRACE_TYPE_INTERFACE);
        return 255;
    }
    if ((NULL == szParamContext) || (0 == iParamSize) )
    {
        TW_TRACE_DIARY_TYPE("�����쳣", TW_TRACE_TYPE_INTERFACE);
        return 255;
    }
    if (NULL == m_pThreadManage)
    {
        TW_TRACE_DIARY_TYPE("�����߳���Ч", TW_TRACE_TYPE_INTERFACE);
        return 255;
    }
    if (GetGlobalApp()->m_EtcPayCenter.HasUnrecoverableError())
    {
        TW_TRACE_DIARY_TYPE("�����޷��ָ��ĵĴ��󣬽�ֹ����", TW_TRACE_TYPE_INTERFACE);
        return 255;
    }
    return 0;
}
#include <fstream>
void UnitTestLog(std::string info)
{
    std::ofstream out_file("unit_test.txt", std::ios::app);
    out_file << info << std::endl;
    out_file.close();
}

void UnitTestLanePara()
{
    //=======��������ѯ��������==========
    std::string correct_para = "<?xml version=\"1.0\" encoding=\"GB2312\"?><ETCDebitListQuery><Version>1.0</Version><Data><TradeKeyItem><PayIdentifier/><Money>1</Money><OBUMAC/><OBUID/><CardID>0</CardID><Vehicle><Class>1</Class><Type>1</Type><License>��A12345</License><VLColor>0</VLColor><AutoPlate><AutoLicense>AutoLicense</AutoLicense><AutoColor>1</AutoColor></AutoPlate></Vehicle><Entry><RoadID>49</RoadID><StationID>49</StationID><LaneID>12</LaneID><Time>2019-01-04 11:40:21</Time></Entry></TradeKeyItem></Data></ETCDebitListQuery>";
    std::string lack_filed_para = "<?xml version=\"1.0\" encoding=\"GB2312\"?><ETCDebitListQuery><Version>1.0</Version><Data><TradeKeyItem><PayIdentifier/><Money>1</Money><OBUMAC/><OBUID/><CardID>0</CardID><Vehicle><Class>1</Class><Type>1</Type><License>��A12345</License><VLColor>0</VLColor><AutoPlate><AutoLicense>AutoLicense</AutoLicense><AutoColor>1</AutoColor></AutoPlate></Vehicle><Entry><RoadID>49</RoadID><StationID>49</StationID><LaneID>12</LaneID><Time_lack>2019-01-04 11:40:21</Time_lack></Entry></TradeKeyItem></Data></ETCDebitListQuery>";
    ObuWhiteListQueryParameter query_para;
    ParseWhiteListQueryParaFromXML(lack_filed_para, query_para);

    //==========�ۿ��������=============
    correct_para = "<?xml version=\"1.0\" encoding=\"GB2312\"?><DebitParamContext><Version>1.0</Version><Data><OverTime>30000</OverTime><OperationMode>0</OperationMode><PayModel>0</PayModel><Debit><PayIdentifier/><Money>1</Money><SubTime>2019-01-04 14:07:59</SubTime><Type>1</Type><PayCertificateCode>0</PayCertificateCode><PayCertificateType>0</PayCertificateType></Debit><Vehicle><Type>1</Type><Class>1</Class><License>��A12345</License><VLColor>0</VLColor><AxisCount>0</AxisCount><Weight>0</Weight><LimitWeight>0</LimitWeight><AutoPlate><AutoLicense>AutoLicense</AutoLicense><AutoColor>1</AutoColor></AutoPlate></Vehicle><Operation><PassCertificateType>0</PassCertificateType><CardID>0</CardID><TollDate>20190104</TollDate><ShiftID>1</ShiftID><TicketNo>12</TicketNo><OperatorID>12</OperatorID><OperatorName>����Ա</OperatorName><Distance>0</Distance></Operation><Entry><StationID>49</StationID><StationName>����</StationName><Time>2019-01-04 13:57:59</Time><LaneID>1</LaneID><License>��A12345</License><VColor>0</VColor><VClass>0</VClass><VType>0</VType><RoadID>49</RoadID><AreaID>49</AreaID></Entry><OBU><License>��A12345</License><VColor>0</VColor><Class>0</Class><Type>0</Type><ID/><MAC/></OBU></Data></DebitParamContext>";
    lack_filed_para = "<?xml version=\"1.0\" encoding=\"GB2312\"?><DebitParamContext><Version>1.0</Version><Data><OverTime>30000</OverTime><OperationMode>0</OperationMode><PayModel>0</PayModel><Debit><PayIdentifier/><Money>1</Money><SubTime>2019-01-04 14:07:59</SubTime><Type>1</Type><PayCertificateCode>0</PayCertificateCode><PayCertificateType>0</PayCertificateType></Debit><Vehicle><Type>1</Type><Class>1</Class><License>��A12345</License><VLColor>0</VLColor><AxisCount>0</AxisCount><Weight>0</Weight><LimitWeight>0</LimitWeight><AutoPlate><AutoLicense>AutoLicense</AutoLicense><AutoColor>1</AutoColor></AutoPlate></Vehicle><Operation><PassCertificateType>0</PassCertificateType><CardID>0</CardID><TollDate>20190104</TollDate><ShiftID>1</ShiftID><TicketNo>12</TicketNo><OperatorID>12</OperatorID><OperatorName>����Ա</OperatorName><Distance>0</Distance></Operation><Entry><StationID>49</StationID><StationName>����</StationName><Time>2019-01-04 13:57:59</Time><LaneID>1</LaneID><License>��A12345</License><VColor>0</VColor><VClass>0</VClass><VType>0</VType><RoadID>49</RoadID><AreaID>49</AreaID></Entry><OBU><License_Lack>��A12345</License_Lack><VColor>0</VColor><Class>0</Class><Type>0</Type><ID/><MAC/></OBU></Data></DebitParamContext>";
    ETCDebitRecord debit_para;
    ParseETCDebitParaFromXML(lack_filed_para, debit_para);

    //===========�ۿ�����ѯ==============
    correct_para = "<?xml version=\"1.0\" encoding=\"GB2312\"?><ETCDebitResultQuery><Version>1.0</Version><Data><TradeKeyItem><PayIdentifier>1231</PayIdentifier><Vehicle><Class>1</Class><Type>1</Type><License>��A12345</License><VLColor>0</VLColor><AutoPlate><AutoLicense>AutoLicense</AutoLicense><AutoColor>1</AutoColor></AutoPlate></Vehicle></TradeKeyItem></Data></ETCDebitResultQuery>";
    lack_filed_para = "<?xml version=\"1.0\" encoding=\"GB2312\"?><ETCDebitResultQuery><Version>1.0</Version><Data><TradeKeyItem><PayIdentifier_Lack>1231</PayIdentifier_Lack><Vehicle><Class>1</Class><Type>1</Type><License>��A12345</License><VLColor>0</VLColor><AutoPlate><AutoLicense>AutoLicense</AutoLicense><AutoColor>1</AutoColor></AutoPlate></Vehicle></TradeKeyItem></Data></ETCDebitResultQuery>";
    DebitStateQueryParameter debit_state_query_para;
    ParseDebitStateQueryParaFromXML(lack_filed_para, debit_state_query_para);

}

#include "DataBaseHelper.h"
using namespace trawe;
#include <sstream>
std::string RandStr(int rand_len = 10)
{
    static std::string stand_str = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int stand_len = static_cast<int>(stand_str.size());
    std::stringstream ss;
    for (int i = 0; i < rand_len; ++i)
    {
        ss << stand_str[rand() % stand_len];
    }
    return ss.str();
}

std::string DebitRecordArchiveAbstract(ETCDebitRecord& record)
{
    std::stringstream ss;
    ss.imbue(std::locale(std::locale::classic(), "", std::locale::ctype));
    ss << "PayIdentifier:" << record.PayIdentifier;
    ss << ",LocalResponse.ExecuteCode:" << record.LocalResponse.ExecuteCode;
    ss << ",ServerResponse.ServerStatus:" << record.ServerResponse.ServerStatus;
    ss << ",LocalResponse.TradeSecurityCode:" << record.LocalResponse.TradeSecurityCode;
    ss << ",LocalResponse.PayPlatformType:" << record.LocalResponse.PayPlatformType;
    ss << ",ServerResponse.DebitOrder:" << record.ServerResponse.DebitOrder;
    ss << ",DebitTime:" << Datetime2Str(record.LocalResponse.DebitTime);
    ss << ",LocalResponse.PaySerialNo:" << record.LocalResponse.PaySerialNo;
    ss << ",Money:" << record.Money;
    ss << ",OBUID:" << record.OBUID;
    ss << ",OBULicense:" << record.OBULicense;
    ss << ",OBULicenseColor:" << record.OBULicenseColor;
    ss << ",CardID:" << record.CardID;

    return ss.str();
}


void RandFillRecordArchive(ETCDebitRecord& record)
{
    int one_month_secs = 30 * 24 * 60 * 60;
    strcpy(record.PayIdentifier, GetGlobalApp()->GetGuidString(1));
    record.LocalResponse.ExecuteCode = rand();
    record.ServerResponse.ServerStatus = rand();
    strcpy(record.LocalResponse.TradeSecurityCode, RandStr(60).c_str());
    record.LocalResponse.PayPlatformType = rand();
    strcpy(record.ServerResponse.DebitOrder, RandStr(60).c_str());
    record.LocalResponse.DebitTime = ChinaTime() - one_month_secs + rand() % (2 * one_month_secs);
    strcpy(record.LocalResponse.PaySerialNo, RandStr(60).c_str());
    record.Money = rand();
    strcpy(record.OBUID, RandStr().c_str());
    strcpy(record.OBULicense, RandStr().c_str());
    record.OBULicenseColor = rand();
    strcpy(record.CardID, RandStr().c_str());
}
void UnitTestDebitCacheOperation()
{
    sqlite3* sql_handle = DataBaseOpen("TestDebitCache.db");
    DataBaseCreateTable(sql_handle, DataBaseGetDebitRequestTableName(), DataBaseGetDebitQuestTableDefine());
    char hint_info[200] = {0};
    //����
    for (int i = 0; i < 10; ++i)
    {
        ETCDebitRecord debit_record;
        strcpy(debit_record.PayIdentifier, GetGlobalApp()->GetGuidString(1));
        debit_record.Money = rand();
        sprintf(hint_info, "�������ݣ�֧����ʶ:%s,���:%d", debit_record.PayIdentifier, debit_record.Money);
        GetGlobalApp()->RecordLog(hint_info);
        if (DataBaseAddDebitQuest(sql_handle, debit_record) != 0)
        {
            GetGlobalApp()->RecordLog("��������ʧ��");
            GetGlobalApp()->RecordLog(DataBaseGetLastError(sql_handle).c_str());
        }
    }

    int loop_max = 100;
    //��ѯ��ɾ��
    while (true && (loop_max-- > 0) )
    {
        ETCDebitRecord debit_record;
        if (DataBaseQueryDebitQuest(sql_handle, debit_record) != 0)
        {
            break;
        }
        sprintf(hint_info, "��ѯ�����ݣ�֧����ʶ:%s,���:%d", debit_record.PayIdentifier, debit_record.Money);
        GetGlobalApp()->RecordLog(hint_info);
        DataBaseDeleteDebitQuest(sql_handle, debit_record);

    }

    //========
    DataBaseClose(sql_handle);
}
void UnitTestDebitRecordArchive()
{
    sqlite3* sql_handle = DataBaseOpen("TestDebitRecord.db");
    DataBaseCreateTable(sql_handle, DataBaseGetDebitRecordTableName(), DataBaseGetDebitRecordTableDefine());
    char hint_info[200] = {0};

    //����
    std::list<std::string> record_payid_group;
    for (int i = 0; i < 10; ++i)
    {
        ETCDebitRecord debit_record;
        RandFillRecordArchive(debit_record);
        std::string hint_info = "��������->" + DebitRecordArchiveAbstract(debit_record);
        GetGlobalApp()->RecordLog(hint_info.c_str());
        record_payid_group.push_back(debit_record.PayIdentifier);
        if (DataBaseAddRecord(sql_handle, debit_record) != 0)
        {
            GetGlobalApp()->RecordLog("��������ʧ��");
            GetGlobalApp()->RecordLog(DataBaseGetLastError(sql_handle).c_str());
        }
    }

    int loop_max = 100;
    //��ѯ��ɾ��
    while (true && (loop_max-- > 0) )
    {
        if (record_payid_group.empty())
        {
            break;
        }
        ETCDebitRecord debit_record;
        strcpy(debit_record.PayIdentifier, record_payid_group.front().c_str());
        record_payid_group.pop_front();
        if (DataBaseQueryRecord(sql_handle, debit_record) != 0)
        {
            break;
        }
        std::string hint_info = "��ѯ������->" + DebitRecordArchiveAbstract(debit_record);
        GetGlobalApp()->RecordLog(hint_info.c_str());

    }

    DataBaseClose(sql_handle);
}
void UnitTestRequestPara()
{
    //��������ѯ����
    std::string stand_para = "<?xml version=\"1.0\" encoding=\"GB2312\"?><ETCDebitListQuery><Version>1.0</Version><Data><TradeKeyItem><PayIdentifier/><Money>1</Money><OBUMAC>OBUMAC</OBUMAC><OBUID>OBUID</OBUID><CardID>2134566</CardID><Vehicle><Class>1</Class><Type>1</Type><License>��A12345</License><VLColor>0</VLColor><AutoPlate><AutoLicense>AutoLicense</AutoLicense><AutoColor>1</AutoColor></AutoPlate></Vehicle><Entry><RoadID>49</RoadID><StationID>49</StationID><LaneID>12</LaneID><Time>2019-01-05 10:34:59</Time></Entry></TradeKeyItem></Data></ETCDebitListQuery>";
    ObuWhiteListQueryParameter query_para;
    std::string request_json;
    ParseWhiteListQueryParaFromXML(stand_para, query_para);
    PackOBUWhiteListQueryJson(query_para, request_json);
    GetGlobalApp()->RecordLog("�������������");
    GetGlobalApp()->RecordLog(stand_para.c_str());
    GetGlobalApp()->RecordLog("���������Ͳ���");
    GetGlobalApp()->RecordLog(request_json.c_str());

    //�۷ѱ���
    stand_para = "<?xml version=\"1.0\" encoding=\"GB2312\"?><DebitParamContext><Version>1.0</Version><Data><OverTime>30000</OverTime><OperationMode>0</OperationMode><PayModel>0</PayModel><Debit><PayIdentifier>12313123123123213</PayIdentifier><Money>1</Money><SubTime>2019-01-05 11:05:24</SubTime><Type>1</Type><PayCertificateCode>0</PayCertificateCode><PayCertificateType>0</PayCertificateType></Debit><Vehicle><Type>1</Type><Class>1</Class><License>��A12345</License><VLColor>0</VLColor><AxisCount>0</AxisCount><Weight>0</Weight><LimitWeight>0</LimitWeight><AutoPlate><AutoLicense>AutoLicense</AutoLicense><AutoColor>1</AutoColor></AutoPlate></Vehicle><Operation><PassCertificateType>0</PassCertificateType><CardID>abcdabcd</CardID><TollDate>20190105</TollDate><ShiftID>1</ShiftID><TicketNo>12</TicketNo><OperatorID>12</OperatorID><OperatorName>����Ա</OperatorName><Distance>0</Distance></Operation><Entry><StationID>49</StationID><StationName>����</StationName><Time>2019-01-05 10:55:24</Time><LaneID>1</LaneID><License>��A12345</License><VColor>0</VColor><VClass>0</VClass><VType>0</VType><RoadID>49</RoadID><AreaID>49</AreaID></Entry><OBU><License>��A12345</License><VColor>2</VColor><Class>3</Class><Type>4</Type><ID>ab</ID><MAC>cd</MAC></OBU></Data></DebitParamContext>";
    ETCDebitRecord debit_para;
    ParseETCDebitParaFromXML(stand_para, debit_para);
    strcpy(debit_para.LocalResponse.PaySerialNo, RandStr().c_str());
    strcpy(debit_para.LocalResponse.TradeSecurityCode, RandStr().c_str());
    PackETCDebitRequestJson(debit_para, request_json);
    GetGlobalApp()->RecordLog("�۷Ѵ������");
    GetGlobalApp()->RecordLog(stand_para.c_str());
    GetGlobalApp()->RecordLog("�۷ѷ��Ͳ���");
    GetGlobalApp()->RecordLog(request_json.c_str());

}
void UnitTest()
{
    GetGlobalApp()->RecordLog("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$");
    UnitTestLanePara();
    UnitTestDebitCacheOperation();
    UnitTestDebitRecordArchive();
    UnitTestRequestPara();
    GetGlobalApp()->RecordLog("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$");
}


int QueryAgentDebitEtcUp_Stake(IN const char* szParamContext, IN const int& iParamSize, IN const int& iFormatType, IN int& iSupportAgent, IN const int& iTimeout, const ObuWhiteListQueryParameter& parseResult)
{
    int real_timeout = iTimeout;
    real_timeout -= 5;
    if (real_timeout < 0)
    {
        real_timeout = 0;
    }
    Sleep(real_timeout);
    TW_TRACE_DIARY_TYPE("���Գ���:" + std::string(parseResult.VehicleLicense), TW_TRACE_TYPE_INTERFACE);
    bool isin_whitelist = (stricmp(parseResult.VehicleLicense, "��A12345") == 0);
    if ( isin_whitelist)
    {
        TW_TRACE_DIARY_TYPE("�������ڰ�����:" + std::string(parseResult.VehicleLicense), TW_TRACE_TYPE_INTERFACE);
        iSupportAgent = 0;
    }
    else
    {
        TW_TRACE_DIARY_TYPE("���Ʋ����ڰ�����:" + std::string(parseResult.VehicleLicense), TW_TRACE_TYPE_INTERFACE);
        iSupportAgent = 1;
    }

    return 0;
}

//������ѯ����ѯ�û��Ƿ����ڴ����û�
int WINAPI CTWSDNetPay_DllApp::IF_QueryAgentDebitEtcUp(IN const char* szParamContext, IN const int& iParamSize, IN const int& iFormatType, IN int& iSupportAgent, IN const int& iTimeout)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    CInterfaceTraceInfo trace_interface("IF_QueryAgentDebitEtcUp", pystring::replace(szParamContext, "\r\n", ""));
    int checkRet = ETCInterfaceBasicCheck(szParamContext, iParamSize);
    if (checkRet != 0)
    {
        return checkRet;
    }
    SYSTEMTIME tmpTimeExecuteBegin;
    GetSystemTime(&tmpTimeExecuteBegin);
    ObuWhiteListQueryParameter query_para;
    if (iFormatType == 1)
    {
        if (!ParseWhiteListQueryParaFromXML(szParamContext, query_para))
        {
            TW_TRACE_DIARY_TYPE("XML��������ʧ��", TW_TRACE_TYPE_INTERFACE);
            return 255;
        }
    }
    else if (iFormatType == 2)
    {
        if (!ParseWhiteListQueryParaFromJson(szParamContext, query_para))
        {
            TW_TRACE_DIARY_TYPE("Json��������ʧ��", TW_TRACE_TYPE_INTERFACE);
            return 255;
        }
    }
    else
    {
        char err_log[100] = {0};
        sprintf(err_log, "��Ч�Ĳ�������:%d", iFormatType);
        TW_TRACE_DIARY_TYPE(err_log, TW_TRACE_TYPE_INTERFACE);
        return 255;
    }
    //return QueryAgentDebitEtcUp_Stake(szParamContext,iParamSize,iFormatType,iSupportAgent,iTimeout,query_para);
    TW_TRACE_DIARY_TYPE("��ʼ���ð�������ѯ", TW_TRACE_TYPE_INTERFACE);
    int query_ret = GetGlobalApp()->m_EtcPayCenter.StartWhiteListQuery(query_para);
    if (query_ret == 0)
    {
        TW_TRACE_DIARY_TYPE("�����������ѯ����ɹ����ȴ��������", TW_TRACE_TYPE_INTERFACE);
    }
    else if (query_ret == -1)
    {
        TW_TRACE_DIARY_TYPE("�����������ѯ����ʧ�ܣ�����ԭ��", TW_TRACE_TYPE_INTERFACE);
        return 255;
    }
    else
    {
        TW_TRACE_DIARY_TYPE("�����������ѯ����ʧ�ܣ������������", TW_TRACE_TYPE_INTERFACE);
        return 1;
    }
    SYSTEMTIME tmpTimeCurrent;
    while (true)
    {
        Sleep(10);
        GetSystemTime(&tmpTimeCurrent);
        int diffMs = GetDiffMilliSeconds(tmpTimeExecuteBegin, tmpTimeCurrent)    ;
        if (diffMs > iTimeout)
        {
            char err_log[100] = {0};
            sprintf(err_log, "�ȴ���ѯ�����ʱ,Ԥ��ʱ��:%d����,��ʹ��ʱ��:%d����,�����������:1����������ѯ�����ʧ��", iTimeout, diffMs);
            TW_TRACE_DIARY_TYPE(err_log, TW_TRACE_TYPE_INTERFACE);
            return 1;
        }
        int state_ret = GetGlobalApp()->m_EtcPayCenter.IsInWhiteList(query_para);
        if (state_ret == -1)
        {
            continue;
        }
        char info_log[100] = {0};
        sprintf(info_log, "�����������:0,��������ѯ���:%d", state_ret);
        TW_TRACE_DIARY_TYPE(info_log, TW_TRACE_TYPE_INTERFACE);

        iSupportAgent = state_ret;
        return 0;
    }

    return 255;
}

int QueryAgentDebitResultEtcUp_Stake(IN const char* szParamContext, IN const int& iParamSize, IN const int& iFormatType, IN OUT char* szResultContext, IN OUT int& iResultSize, IN int iTimeout, const DebitStateQueryParameter& parseResult)
{
    int real_timeout = iTimeout;
    real_timeout -= 5;
    if (real_timeout < 0)
    {
        real_timeout = 0;
    }
    Sleep(real_timeout);
    TW_TRACE_DIARY_TYPE("���Գ��ƺ�:" + std::string(parseResult.VehicleLicense), TW_TRACE_TYPE_INTERFACE);
    std::string stake_result ;
    if (stricmp(parseResult.VehicleLicense, "��A12345") == 0)
    {
        TW_TRACE_DIARY_TYPE("���ڸó��ƵĽ��׽��:" + std::string(parseResult.VehicleLicense), TW_TRACE_TYPE_INTERFACE);
        stake_result = "<?xml version=\"1.0\" encoding=\"GB2312\"?><DebitResultContext><Version>1.0</Version><Data><ResultType>0</ResultType><ExecuteCode>0</ExecuteCode><ExecuteDesc/><TradeKeyItem><TradeSecurityCode>6f59dc0795cc69c176fb1577a36878ca</TradeSecurityCode><PayPlatformType>1</PayPlatformType><PayIdentifier>A1A8A10E9AE947b29FEF2D97B6ABF2F61231</PayIdentifier><DebitOrder>8683782D385044b994F478EFE20E30E1</DebitOrder><DebitTime>2020-02-01 15:14:25</DebitTime><ConsumeTime>0</ConsumeTime></TradeKeyItem></Data></DebitResultContext>";
    }
    else
    {
        TW_TRACE_DIARY_TYPE("�����ڸó��ƵĽ��׽��:" + std::string(parseResult.VehicleLicense), TW_TRACE_TYPE_INTERFACE);
        return 255;
    }

    int stake_result_size = static_cast<int>(stake_result.size());
    if (iResultSize < stake_result_size)
    {
        char err_log[100] = {0};
        sprintf(err_log, "������ѯ���ʧ��,�޷���������浽���棬�����С:%d�������С:%d", iResultSize, stake_result_size);
        TW_TRACE_DIARY_TYPE(err_log, TW_TRACE_TYPE_INTERFACE);
        return 255;
    }
    TW_TRACE_DIARY_TYPE("���ز�ѯ���:" + stake_result, TW_TRACE_TYPE_INTERFACE);
    strcpy(szResultContext, stake_result.c_str());
    iResultSize = stake_result_size;
    return 0;
}

//���н��ײ�ѯ
int WINAPI CTWSDNetPay_DllApp::IF_QueryAgentDebitResultEtcUp(IN const char* szParamContext, IN const int& iParamSize, IN const int& iFormatType, IN OUT char* szResultContext, IN OUT int& iResultSize, IN int iTimeout)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    CInterfaceTraceInfo trace_interface("IF_QueryAgentDebitResultEtcUp", pystring::replace(szParamContext, "\r\n", ""));
    int checkRet = ETCInterfaceBasicCheck(szParamContext, iParamSize);
    if (checkRet != 0)
    {
        return checkRet;
    }
    SYSTEMTIME tmpTimeExecuteBegin;
    GetSystemTime(&tmpTimeExecuteBegin);
    DebitStateQueryParameter query_para;
    if (iFormatType == 1)
    {
        if (!ParseDebitStateQueryParaFromXML(szParamContext, query_para))
        {
            TW_TRACE_DIARY_TYPE("XML��������ʧ��", TW_TRACE_TYPE_INTERFACE);
            return 255;
        }
    }
    else if (iFormatType == 2)
    {
        if (!ParseDebitStateQueryParaFromJson(szParamContext, query_para))
        {
            TW_TRACE_DIARY_TYPE("Json��������ʧ��", TW_TRACE_TYPE_INTERFACE);
            return 255;
        }
    }
    else
    {
        char err_log[100] = {0};
        sprintf(err_log, "��Ч�Ĳ�������:%d", iFormatType);
        TW_TRACE_DIARY_TYPE(err_log, TW_TRACE_TYPE_INTERFACE);
        return 255;
    }
    //return QueryAgentDebitResultEtcUp_Stake(szParamContext,iParamSize,iFormatType,szResultContext,iResultSize,iTimeout,query_para);

    int query_ret = GetGlobalApp()->m_EtcPayCenter.StartDebitStateQuery(query_para);
    if (query_ret == 0)
    {
        TW_TRACE_DIARY_TYPE("������۽����ѯ����ɹ����ȴ��������", TW_TRACE_TYPE_INTERFACE);
    }
    else if (query_ret == -1)
    {
        TW_TRACE_DIARY_TYPE("������۽����ѯ����ʧ�ܣ�����ԭ��", TW_TRACE_TYPE_INTERFACE);
        return 255;
    }
    else
    {
        TW_TRACE_DIARY_TYPE("������۽����ѯ����ʧ�ܣ������������", TW_TRACE_TYPE_INTERFACE);
        return 1;
    }
    SYSTEMTIME tmpTimeCurrent;
    ETCDebitRecord debitState;
    while (true)
    {
        Sleep(10);
        GetSystemTime(&tmpTimeCurrent);
        int diffMs = GetDiffMilliSeconds(tmpTimeExecuteBegin, tmpTimeCurrent)    ;
        if (diffMs > iTimeout)
        {
            char err_log[100] = {0};
            sprintf(err_log, "�ȴ������ʱ,Ԥ��ʱ��:%d����,��ʹ��ʱ��:%d����", iTimeout, diffMs);
            TW_TRACE_DIARY_TYPE(err_log, TW_TRACE_TYPE_INTERFACE);
            return 1;
        }
        int state_ret = GetGlobalApp()->m_EtcPayCenter.GetDebitState(query_para, debitState);
        if (state_ret == -1)
        {
            continue;
        }
        else if (state_ret != 0)
        {
            return state_ret;
        }
        std::string debit_state_str;
        if (iFormatType == 1)
        {   //xml
            if (PackETCDebitResultXML(debitState, debit_state_str) != 0)
            {
                TW_TRACE_DIARY_TYPE("������ѯ���ʧ��", TW_TRACE_TYPE_INTERFACE);
                return 255;
            }
        }
        else
        {   //json
            if (PackETCDebitResultJson(debitState, debit_state_str) != 0)
            {
                TW_TRACE_DIARY_TYPE("������ѯ���ʧ��", TW_TRACE_TYPE_INTERFACE);
                return 255;
            }
        }
        int result_size = static_cast<int>(debit_state_str.size());
        if ( result_size > iResultSize)
        {
            char err_log[200] = {0};
            sprintf(err_log, "������ѯ���ʧ��,�޷���������浽���棬�����С:%d�������С:%d", iResultSize, result_size);
            TW_TRACE_DIARY_TYPE(err_log, TW_TRACE_TYPE_INTERFACE);
            return 255;
        }
        strcpy(szResultContext, debit_state_str.c_str());
        iResultSize = result_size;
        return 0;
    }
    return 1;
}

int DebitMoneyEtcUp_Stake(IN const char* szParamContext, IN const int& iParamSize, IN const int& iFormatType, IN OUT char* szResultContext, IN OUT int& iResultSize, IN int iTimeout, const ETCDebitRecord& parseResult)
{
    int real_timeout = iTimeout;
    real_timeout -= 5;
    if (real_timeout < 0)
    {
        real_timeout = 0;
    }
    Sleep(real_timeout);
    std::string stake_result;
    TW_TRACE_DIARY_TYPE("���Գ��ƺ�:" + std::string(parseResult.VehicleLicense), TW_TRACE_TYPE_INTERFACE);
    if (stricmp(parseResult.VehicleLicense, "��A12345") == 0)
    {
        stake_result = "<?xml version=\"1.0\" encoding=\"GB2312\"?><DebitResultContext><Version>1.0</Version><Data><ResultType>0</ResultType><ExecuteCode>0</ExecuteCode><ExecuteDesc/><TradeKeyItem><TradeSecurityCode>6f59dc0795cc69c176fb1577a36878ca</TradeSecurityCode><PayPlatformType>1</PayPlatformType><PayIdentifier>A1A8A10E9AE947b29FEF2D97B6ABF2F61231</PayIdentifier><DebitOrder>8683782D385044b994F478EFE20E30E1</DebitOrder><DebitTime>2020-02-01 15:14:25</DebitTime><ConsumeTime>0</ConsumeTime></TradeKeyItem></Data></DebitResultContext>";
        TW_TRACE_DIARY_TYPE("�������ڰ�����:" + std::string(parseResult.VehicleLicense), TW_TRACE_TYPE_INTERFACE);
    }
    else
    {
        TW_TRACE_DIARY_TYPE("���Ʋ����ڰ�����:" + std::string(parseResult.VehicleLicense), TW_TRACE_TYPE_INTERFACE);
        return 255;
    }

    int stake_result_size = static_cast<int>(stake_result.size());
    if (iResultSize < stake_result_size)
    {
        char err_log[100] = {0};
        sprintf(err_log, "������ѯ���ʧ��,�޷���������浽���棬�����С:%d�������С:%d", iResultSize, stake_result_size);
        TW_TRACE_DIARY_TYPE(err_log, TW_TRACE_TYPE_INTERFACE);
        return 255;
    }
    TW_TRACE_DIARY_TYPE("���ؿۿ���:" + stake_result, TW_TRACE_TYPE_INTERFACE);
    strcpy(szResultContext, stake_result.c_str());
    iResultSize = stake_result_size;
    return 0;
}

int WINAPI CTWSDNetPay_DllApp::IF_DebitMoneyEtcUp(IN const char* szParamContext, IN const int& iParamSize, IN const int& iFormatType, IN OUT char* szResultContext, IN OUT int& iResultSize, IN int iTimeout)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    CInterfaceTraceInfo trace_interface("IF_DebitMoneyEtcUp", pystring::replace(szParamContext, "\r\n", ""));
    int checkRet = ETCInterfaceBasicCheck(szParamContext, iParamSize);

    if (checkRet != 0)
    {
        return checkRet;
    }
    ETCDebitRecord debit_para;
    if (iFormatType == 1)
    {
        if (!ParseETCDebitParaFromXML(szParamContext, debit_para))
        {
            TW_TRACE_DIARY_TYPE("XML��������ʧ��", TW_TRACE_TYPE_INTERFACE);
            return 255;
        }
    }
    else if (iFormatType == 2)
    {
        if (!ParseETCDebitParaFromJson(szParamContext, debit_para))
        {
            TW_TRACE_DIARY_TYPE("Json��������ʧ��", TW_TRACE_TYPE_INTERFACE);
            return 255;
        }
    }
    else
    {
        char err_log[100] = {0};
        sprintf(err_log, "��Ч�Ĳ�������:%d", iFormatType);
        TW_TRACE_DIARY_TYPE(err_log, TW_TRACE_TYPE_INTERFACE);
        return 255;
    }
    // return DebitMoneyEtcUp_Stake(szParamContext,iParamSize,iFormatType,szResultContext,iResultSize,iTimeout,debit_para);

    std::string result_str;
    int debit_ret = GetGlobalApp()->m_EtcPayCenter.DebitMoney(debit_para, result_str, iFormatType);
    if (debit_ret != 0)
    {
		if( debit_ret <0)
		{
			debit_ret = 255;
		}
        char err_log[100] = {0};
        sprintf(err_log, "�ۿ�ʧ�ܣ�������:%d", debit_ret);
        TW_TRACE_DIARY_TYPE(err_log, TW_TRACE_TYPE_INTERFACE);
        return debit_ret;
    }
    int result_size = static_cast<int>(result_str.size());
    if (result_size > iResultSize)
    {
        char err_log[200] = {0};
        sprintf(err_log, "������ѯ���ʧ��,�޷���������浽���棬�����С:%d�������С:%d", iResultSize, result_size);
        TW_TRACE_DIARY_TYPE(err_log, TW_TRACE_TYPE_INTERFACE);
        return 255;
    }
    TW_TRACE_DIARY_TYPE("���ؿۿ���:" + result_str, TW_TRACE_TYPE_INTERFACE);
    strcpy(szResultContext, result_str.c_str());
    iResultSize = result_size;
    return 0;
}


void CTWSDNetPay_DllApp::StopListenMessage()
{
    DisonnectDataForward();
    if (m_MessageCenter)
    {
        m_MessageCenter->Stop();
        delete m_MessageCenter;
        m_MessageCenter = 0;
    }
}
void CopyExpwayInfo(const tagTranstParam& lane_config, DebitParameter&dp)
{
    dp.LaneConfig.ProvinceID = lane_config.ProvinceID;
    strncpy(dp.LaneConfig.AreaID, lane_config.LaneConfigInfo.str_AreaID, sizeof(dp.LaneConfig.AreaID) - 1);
    strncpy(dp.LaneConfig.RoadID, lane_config.LaneConfigInfo.str_RoadID, sizeof(dp.LaneConfig.RoadID) - 1);
    strncpy(dp.LaneConfig.StationID, lane_config.LaneConfigInfo.str_StationID, sizeof(dp.LaneConfig.StationID) - 1);
    strncpy(dp.LaneConfig.LaneID, lane_config.LaneConfigInfo.str_LaneID, sizeof(dp.LaneConfig.LaneID) - 1);

    strncpy(dp.LaneConfig.StationName, lane_config.LaneConfigInfo.StationTab, sizeof(dp.LaneConfig.StationName) - 1);

}

void CopyExpwayInfo(const tagTranstParam& lane_config, DebitCancleParameter&dp)
{
    dp.ProvinceID = lane_config.ProvinceID;
	if((strlen(lane_config.LaneConfigInfo.str_RoadID) 
	   + strlen(lane_config.LaneConfigInfo.str_StationID) 
	   + strlen(lane_config.LaneConfigInfo.StationTab) 
	   + strlen(lane_config.LaneConfigInfo.str_LaneID)) < 96)
	{
		_snprintf(dp.LaneInfo, 100, "%s-%s-%s-%s", lane_config.LaneConfigInfo.str_RoadID, lane_config.LaneConfigInfo.str_StationID, lane_config.LaneConfigInfo.StationTab, lane_config.LaneConfigInfo.str_LaneID);
	}
}

void AdjustExpwayInfo(DebitParameter&dp)
{
    bool is_guang_dong = (dp.LaneConfig.ProvinceID == 44);
    if (!is_guang_dong)
    {
        return ;
    }
    char full_station[300] = {0};
    _snprintf(full_station, 200, "%04d%03d%02d", dp.EntryAreaID, dp.EntryRoadID, dp.EntryStationID);
    strncpy(dp.EntryStationID, full_station, sizeof(dp.EntryStationID) - 1);
}



//
////�Ӹ��¿�autoUpdate.cpp ��Ǩ�ƹ���-------20201130
//
//// ��������ݣ���Ϊ�˼���֮ǰ�İ汾
//
//class    CommonBus
//{
//
//public:
//	CommonBus(){}
//	virtual ~CommonBus(){}
//    //ע��  key:���������ȫ�ֶ����Ψһ��ֵ
//	virtual void register2Bus(int key){}
//    //ע��  key:���������ȫ�ֶ����Ψһ��ֵ
//	virtual void unRegister2Bus(int key){}
//    /*
//    ����
//    destKey:����Ŀ�������ȫ�ֶ����Ψһ��ֵ
//    msg:��Ҫ���͵���Ϣ����
//    len:��Ҫ���͵���Ϣ���ݵĳ���
//    srcKey���������ȫ�ֶ����Ψһ��ֵ��Ĭ��Ϊ0��
//*/
//	virtual int send2Bus(int destKey,char *msg,int len,int srcKey=0){return 1;}
//
//    /*
//        ����
//        destKey:���������ȫ�ֶ����Ψһ��ֵ
//        msg:��Ҫ���͵���Ϣ����
//        len:��Ҫ���͵���Ϣ���ݵĳ���
//        srcKey������Դ�����ȫ�ֶ����Ψһ��ֵ
//    */
//	virtual int recvFromBus(int destKey,char *msg,int len, int srcKey=0){return 1;}
//
//public:
//
//    int commpentsKey;
//};
//
//
//CommonBus* comp_data1= new CommonBus();
//
//
//CommonBus*  WINAPI getInstance()
//{
//	//comp_data1->commpentsKey = -12;
//	//return NULL;
//	return comp_data1;
//}
//CommonBus*  WINAPI deleteInstance()
//{
//	delete comp_data1;
//	return NULL;/*do nothing*/
//}