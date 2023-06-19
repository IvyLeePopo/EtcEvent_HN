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

//唯一全局变量
CTWSDNetPay_DllApp theApp;

CTWSDNetPay_DllApp* GetGlobalApp()  { return &theApp; }

BEGIN_MESSAGE_MAP(CTWSDNetPay_DllApp, CWinApp)
END_MESSAGE_MAP()

/***********************变量初始化*********************************/
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

/**********************************EtcEvent相关********************************************/
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

// 数据接口函数
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

	theApp.RecvMsgFuncFromWebsocket(pszData，nSize);
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
// 函数名称：CString   FmtStr
//
// 功能描述：
//
// 输入参数：const char * lpzFmt
//         ：...
// 编写人员：ROCY
// 编写时间：星期二 2009年9月15日
// 修改人员：
// 修改时间：
// 函数版本：1.0.0.1
// 备注说明：
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
// 函数名称：CString2time_t
//
// 功能描述：时间格式字符串转化为 整型数 的时间
//
// 输入参数：LPCTSTR lpstime
// 输出参数：time_t
// 编写人员：ROCY
// 编写时间：星期二 2009年9月15日
// 修改人员：
// 修改时间：
// 函数版本：1.0.0.1
// 备注说明：
//
////////////////////////////////////////////////////////////////////////////////////////////////
time_t CTWSDNetPay_DllApp::CString2time_t(LPCTSTR lpztime)
{
    CString csTemp = _T("");
    COleDateTime oleGetTime;
    csTemp.Format( _T("%s"), lpztime);
    if ( csTemp.GetLength() > 0 )
    {
        if ( csTemp.GetLength() > 19 ) //兼容类似格式 _T("2014-11-23 12:34:56.000");
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
// 函数名称：GetGuidString
//
// 功能描述：获取新的GUID字符窜
//
// 输入参数：无参数
// 输出参数：const
// 编写人员：ROCY
// 编写时间：2012年11月12日
// 修改人员：
// 修改时间：
// 函数版本：1.0.0.1
// 备注说明：
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
// 函数名称：CTWSDNetPay_DllApp::GetCurrentTimeString
//
// 功能描述：获取当前的时间值
//
// 输入参数：IN const int& iTimeType
// 输出参数：const
// 编写人员：ROCY
// 编写时间：2016年8月19日
// 修改人员：
// 修改时间：
// 函数版本：1.0.0.1
// 备注说明：
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
///函数名称: MakeDirectory()
///功能说明: 创建指定的多级目录
///参数说明: CString dd,用来创建的多级目录路径
///数出说明: TRUE  -- 创建成功
///          FALSE -- 创建失败
///修改时间: 2006-04-12
///修改人员: ROCY
///备    注: 需要包含 windows.h
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
//  作   者 : ROCY
//  时   间 : 2008-06-01
//  版   本 : 1.0.0.1
//  函数名称 : GetVersionOfFile
//  函数功能 : 获取指定的文件的版本号
//  输入参数 : LPCTSTR  --  lpsPath 文件的绝对路径
//  输出参数 : DWORD -- 返回的本地计算机IP子网掩码，字节合成格式
//  备注说明 : 无
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
// 函数名称：NotifyCaller
//
// 功能描述：通知外部调用者，业务被异步处理得到的结果
//
// 输入参数：IN const int& iOption             -- 通知类型（1，扣款；2，撤单）
//         ：IN const int& iResult             -- 处理结果，见开发文档
// 输出参数：无
// 编写人员：ROCY
// 编写时间：2016年7月23日
// 修改人员：
// 修改时间：
// 函数版本：1.0.0.1
// 备注说明：接口1
//
////////////////////////////////////////////////////////////////////////////////////////////////
void CTWSDNetPay_DllApp::NotifyCaller(IN const int& iOption, IN const int& iResult)
{
    switch (m_iNotifyMode)
    {
    case 1:
    {

        //通知调用者返回结果，当前操作失败
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
        //使用回调函数通知外部调用者
        if (NULL != m_fnCallbackNotifyFun)
        {
            m_fnCallbackNotifyFun(iOption, iResult);
        }
        else
        {
            RecordLog(FmtStr(_T("[主进程响应],组件通知外部调用者的回调函数(NotifyCaller)指针为空，无法通知调用者")), LOG_LEVEL_ERROR, true);
        }
    }
    break;
    case 3:
    {
        //使用回调函数通知外部调用者(广西特供)
        if (NULL != m_fnCallbackNotifyFun45)
        {
            m_fnCallbackNotifyFun45(iOption, iResult, m_pIndPtrFor45);
        }
        else
        {
            RecordLog(FmtStr(_T("[主进程响应],组件通知外部调用者的回调函数(NotifyCaller45)指针为空，无法通知调用者")), LOG_LEVEL_ERROR, true);
        }
    }
    break;
    default:
        break;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////
// 函数名称：CTWSDNetPay_DllApp::CheckIsAlNum
//
// 功能描述：检测给定的字符串，是否为数字和字母或数字或字母的其中一种
//
// 输入参数：IN const char* szData
// 输出参数：BOOL
// 编写人员：ROCY
// 编写时间：2017年4月27日
// 修改人员：
// 修改时间：
// 函数版本：1.0.0.1
// 备注说明：
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
        case 0://是否数字、字母混合
        {
            if (0 == isalnum(szData[i]))
            {
                bReturn = FALSE;
            }
        }
        break;
        case 1://是否数字
        {
            if ( (szData[i] < '0') || (szData[i] > '9') )
            {
                bReturn = FALSE;
            }
        }
        break;
        case 2://是否字母
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



//用于站号或者车道号转换过程中，检测其内部是否含有字母
BOOL CTWSDNetPay_DllApp::CheckStringLetter(char * strCheck, int nType)
{
    CString csCheck = _T("");
    csCheck.Format( _T("%s"), strCheck );
    csCheck.MakeUpper();

    if ( 1 == nType ) //类型1表示站点编码
    {
        if ( csCheck.GetLength() > 9 ) //对大于9位长度的站点编码，默认按照字符串处理
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









    //add 2018-04 初始化sqlite
    CString sqlite_dll_file;
    sqlite_dll_file.Format(_T("%s\\sqlite3.dll"), m_strWorkDir);
    sqlite3_agent::LoadSqlite(sqlite_dll_file);
    //读取配置文件
    ReadConfigInfo();

    //创建日志目录
    strTemp.Format(_T("%s\\TWSDNetPayLog"), m_strWorkDir);
    _tcscpy_s(m_sTranstParam.PathLogDir, _countof(m_sTranstParam.PathLogDir), strTemp.GetBuffer(0));
    strTemp.ReleaseBuffer();


	printf("\n开始创建日志目录。。。。。。\n");

	MakeDirectory(m_sTranstParam.PathLogDir);
	printf("\n创建日志目录。。。结束。。。\n");


    //获取程序版本
    CString strDllFilePath(szTempDir);

    m_strNetPayDllVersion = GetVersionOfFile(strDllFilePath);

    strDllFilePath.Format(_T("%s\\%s"), strDir, FILE_NAME_DEVICE_DLL);
    m_strDeviceDllVersion = GetVersionOfFile(strDllFilePath);


	//动态库加载句柄   ------ 2020-12-05 --- 更新后让新的设备库马上被进程占用，防止还原为旧版本

	//m_hModule_Device = ::LoadLibrary(strDllFilePath);



    //初始化日志系统
    if (m_bSaveLog)
    {
        INIT_LOG(m_sTranstParam.PathLogDir, _T(""));

        RecordLog(FmtStr(_T("\r\n/*************************************程序启动(业务%s - 设备%s)*************************************/"), m_strNetPayDllVersion, m_strDeviceDllVersion));
    }

    hFileUpdate = CreateEvent(NULL, FALSE, TRUE,    NULL);  //默认属性、自动重置、初始化
    if ( NULL == hFileUpdate )
    {
        RecordLog( _T("创建 播报更新本地存储文件 同步变量失败！") );
    }

    int int_para = GetGlobalApp()->GetLPRQueryOverTimeMilli();
    RecordLog(FmtStr(_T("LPR超时设置:%dms"), int_para));

    //测试NFC
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
        RecordLog("[模块响应]正在等待交易结束...");
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
    //退出日志系统
    if (m_bSaveLog)
    {
        RecordLog(FmtStr(_T("\r\n/*************************************程序停止(业务%s - 设备%s)*************************************/"), m_strNetPayDllVersion, m_strDeviceDllVersion));

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


/****************************************对外接口函数********************************************/

////////////////////////////////////////////////////////////////////////////////////////////////
// 函数名称：IF_InitEnvironment
//
// 功能描述：初始化组件环境
//
// 输入参数：IN const UINT& nThreadID              -- 调用者指定的线程ID
//         ：IN const HWND& hWnd                   -- 调用者指定的窗体句柄
//         ：IN const unsigned int& nNotifyMsgID   -- 调用者指定的通知消息ID
//         ：IN const char* szAreaInfo             -- 区域信息(区域编码#路段号)
//         ：IN const char* szLoaclStation         -- 站点信息(站号#站名)
//         ：IN const char* szLoaclLaneID          -- 车道编码
//         ：IN const char* szServerInfo           -- 服务器信息
//         ：IN const int& iProvinceID             -- 省份编码
// 输出参数：bool
// 编写人员：ROCY
// 编写时间：2016年7月23日
// 修改人员：
// 修改时间：
// 函数版本：1.0.0.1
// 备注说明：接口1
//
////////////////////////////////////////////////////////////////////////////////////////////////
bool WINAPI CTWSDNetPay_DllApp::IF_InitEnvironment(IN const UINT& nThreadID, IN const HWND& hWnd, IN const unsigned int& nNotifyMsgID, IN const char* szAreaInfo, IN const char* szLoaclStation, IN const char* szLoaclLaneID, IN const char* szServerInfo, IN const int& iProvinceID)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    //GetGlobalApp()->m_EtcPayCenter.Init();
    bool bReturn = false;


    CString strLog;
    RecordLog(FmtStr(_T("[主进程响应],组件初始化接口(IF_InitEnvironment)调用开始")));

    try
    {
        /*******************检索参数是否正常*******************/
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
        //填充配置信息
        TCHAR   szTemp[256] = {0};
        CString strTemp;

        m_sTranstParam.ProvinceID            = iProvinceID;
        m_sTranstParam.ActiveNotifyMsgID     = nNotifyMsgID;
        m_sTranstParam.CallerThreadID        = nThreadID;
        m_sTranstParam.CallerWnd             = hWnd;

        //车道编码
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

        //路段、区域编码
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

        //站点编码和站名
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
        //特殊处理代码区域
        switch (m_sTranstParam.ProvinceID)
        {
        case 44:
        {
            CString strSTID;
            switch (m_sTranstParam.LaneConfigInfo.RoadID)
            {
            //广东华快的出口站号特殊处理
            case 30:
            case 31:
            default:/*modify 2017-11 zhuyabing 广东省都使用拼接处理*/
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


		LogToDebugFile("Before Step Into 内部参数配置");
        RecordLog(FmtStr(_T("[主进程响应],组件初始化接口(IF_InitEnvironment)，内部参数配置:%s(省份编码:%d,区域编码:%d,路段编码:%d,站点编码:%s)"), strSTName, m_sTranstParam.ProvinceID, m_sTranstParam.LaneConfigInfo.AreaID, m_sTranstParam.LaneConfigInfo.RoadID, m_sTranstParam.LaneConfigInfo.StationID));

        //GenerateRawInitPara(m_sTranstParam);

        //进行合法性校验
        m_bAuthorizedLegal = TRUE;
        //启动业务管理线程
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
        //将该线程重新唤醒
        m_pThreadManage->ResumeThread();
		LogToDebugFile("Before Step Into AgencyInitDll");

        //版本1060调整 进行代扣动态库加载处理
        AgencyInitDll(szAreaInfo, szLoaclStation, szLoaclLaneID, szServerInfo, iProvinceID);

		m_iCurrentWorkMode = (DWORD)::GetPrivateProfileInt(_T("WorkMode"), _T("Default"), 0, strConfigFilePath);
		if(m_iCurrentWorkMode == 1)
		{
			RecordLog(FmtStr(_T("[主进程响应],当前工作模式为MTC，自动调用IF_EventInitEnvironment3接口初始化ETC特情环境")));
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
				_snprintf_s(ETCAreaInfo, 511, "%s#区域名称#%s#路段名称", m_sTranstParam.LaneConfigInfo.str_AreaID, m_sTranstParam.LaneConfigInfo.str_RoadID);

				void* pIndPtr = (void *)malloc(2048);
				m_bIsInitEnvironment = IF_EventInitEnvironment3(nThreadID, hWnd, nNotifyMsgID, ETCAreaInfo, szLoaclStation, szLoaclLaneID, szServerInfo, iProvinceID, pIndPtr, EtcCallBackFun);
			}
		}
		else
		{
			RecordLog(FmtStr(_T("[主进程响应],当前工作模式为ETC特情，由车道软件调用IF_EventInitEnvironment3接口初始化ETC特情环境")));
		    //标记初始化状态
			m_bIsInitEnvironment = TRUE;
		}

        //标记通知方式
        m_iNotifyMode = 1;

        //返回结果
        bReturn = m_bIsInitEnvironment ? true : false;

        GetGlobalApp()->RecordLog(bReturn ? "init 1" : "init 0");
    }
    catch (int& iErrorCode)
    {
        switch (iErrorCode)
        {
        case -1:
            strLog.Format(_T("[主进程响应],组件初始化接口(IF_InitEnvironment),传入参数检测有误，传入线程ID与窗体句柄均为0"));
            break;
        case -2:
            strLog.Format(_T("[主进程响应],组件初始化接口(IF_InitEnvironment),传入参数检测有误，传入通知调用者消息ID为0"));
            break;
        case -3:
            strLog.Format(_T("[主进程响应],组件初始化接口(IF_InitEnvironment),传入参数检测有误，传入区域信息非法"));
            break;
        case -4:
            strLog.Format(_T("[主进程响应],组件初始化接口(IF_InitEnvironment),传入参数检测有误，传入站点编码信息非法"));
            break;
        case -5:
            strLog.Format(_T("[主进程响应],组件初始化接口(IF_InitEnvironment),传入参数检测有误，传入车道编码信息非法"));
            break;
        case -6:
            strLog.Format(_T("[主进程响应],组件初始化接口(IF_InitEnvironment),传入参数检测有误，传入服务器信息非法"));
            break;
        case -7:
            strLog.Format(_T("[主进程响应],组件初始化接口(IF_InitEnvironment),传入参数检测有误，传入省份编码为0"));
            break;
        case -8:
            strLog.Format(_T("[主进程响应],组件初始化接口(IF_InitEnvironment),传入参数检测有误,传入调用者线程ID和窗体句柄不能同时为空"));
            break;
        case -9:
            strLog.Format(_T("[主进程响应],组件初始化接口(IF_InitEnvironment),传入参数检测有误,传入区域参数格式不正确"));
            break;
        case -10:
            strLog.Format(_T("[主进程响应],组件初始化接口(IF_InitEnvironment),传入参数检测有误,传入站点信息参数格式不正确"));
            break;
        case -11:
            strLog.Format(_T("[主进程响应],组件初始化接口(IF_InitEnvironment),组件不允许多次初始化"));
            break;
        case -12:
            strLog.Format(_T("[主进程响应],组件初始化接口(IF_InitEnvironment),内部初始化错误,无法启动业务管理线程"));
            break;
        }
        //记录日志
        if (m_strLastErrorDesc != strLog)
        {
            m_strLastErrorDesc = strLog;
            RecordLog(m_strLastErrorDesc, LOG_LEVEL_ERROR);
        }
    }

    RecordLog(FmtStr(_T("[主进程响应],组件初始化接口(IF_InitEnvironment)调用结束")));
    return bReturn;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// 函数名称：IF_InitEnvironment2
//
// 功能描述：初始化组件环境
//
// 输入参数：IN const UINT& nThreadID              -- 调用者指定的线程ID
//         ：IN const HWND& hWnd                   -- 调用者指定的窗体句柄
//         ：IN const char* szAreaInfo             -- 区域信息(区域编码#路段号)
//         ：IN const char* szLoaclStation         -- 站点信息(站号#站名)
//         ：IN const char* szLoaclLaneID          -- 车道编码
//         ：IN const char* szServerInfo           -- 服务器信息
//         ：IN const int& iProvinceID             -- 省份编码
//         ：void (*fun)(int option, int result)   -- 扣款/撤单结果通知，回调函数
// 输出参数：bool
// 编写人员：ROCY
// 编写时间：2016年12月14日
// 修改人员：
// 修改时间：
// 函数版本：1.0.0.1
// 备注说明：接口1-1
//
////////////////////////////////////////////////////////////////////////////////////////////////
bool WINAPI CTWSDNetPay_DllApp::IF_InitEnvironment2 (IN const UINT& nThreadID, IN const HWND& hWnd, IN const char* szAreaInfo, IN const char* szLoaclStation, IN const char* szLoaclLaneID, IN const char* szServerInfo, IN const int& iProvinceID, void (WINAPI *fun)(int option, int result))
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    GetGlobalApp()->m_EtcPayCenter.Init();
    bool bReturn = false;
    CString strLog;

    RecordLog(FmtStr(_T("[主进程响应],组件初始化接口(IF_InitEnvironment2)调用开始")));
    try
    {
        /*******************检索参数是否正常*******************/
        if (  (0 == nThreadID) && (NULL == hWnd))
            throw - 1;
        if (NULL == fun)
            throw - 2;
        //回调函数指针赋值，便于通知调用者
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
        //填充配置信息
        TCHAR   szTemp[256] = {0};
        CString strTemp;
        //其它初始化参数赋值
        m_sTranstParam.ProvinceID            = iProvinceID;
        m_sTranstParam.ActiveNotifyMsgID     = 0;
        m_sTranstParam.CallerThreadID        = nThreadID;
        m_sTranstParam.CallerWnd             = hWnd;

        //车道编码
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

        //路段、区域编码
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

        //站点编码和站名
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
        //特殊处理代码区域
        switch (m_sTranstParam.ProvinceID)
        {
        case 44:
        {
            CString strSTID;
            switch (m_sTranstParam.LaneConfigInfo.RoadID)
            {
            //广东华快的出口站号特殊处理
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

        RecordLog(FmtStr(_T("[主进程响应],组件初始化接口(IF_InitEnvironment2)，内部参数配置:%s(省份编码:%d,区域编码:%d,路段编码:%d,站点编码:%s)"), strSTName, m_sTranstParam.ProvinceID, m_sTranstParam.LaneConfigInfo.AreaID, m_sTranstParam.LaneConfigInfo.RoadID, m_sTranstParam.LaneConfigInfo.StationID));



        GenerateRawInitPara(m_sTranstParam);
        //进行合法性校验
        m_bAuthorizedLegal = TRUE;
        //启动业务管理线程
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
        //将该线程重新唤醒
        m_pThreadManage->ResumeThread();

        //版本1060调整 进行代扣动态库加载处理
        AgencyInitDll(szAreaInfo, szLoaclStation, szLoaclLaneID, szServerInfo, iProvinceID);
        //标记初始化状态
        m_bIsInitEnvironment = TRUE;

        //标记通知方式
        m_iNotifyMode = 2;

        //返回结果
        bReturn = m_bIsInitEnvironment ? true : false;
    }
    catch (int& iErrorCode)
    {
        switch (iErrorCode)
        {
        case -1:
            strLog.Format(_T("[主进程响应],组件初始化接口(IF_InitEnvironment2),传入参数检测有误，传入线程ID与窗体句柄均为0"));
            break;
        case -2:
            strLog.Format(_T("[主进程响应],组件初始化接口(IF_InitEnvironment2),传入参数检测有误，传入回调函数指针错误"));
            break;
        case -3:
            strLog.Format(_T("[主进程响应],组件初始化接口(IF_InitEnvironment2),传入参数检测有误，传入区域信息非法"));
            break;
        case -4:
            strLog.Format(_T("[主进程响应],组件初始化接口(IF_InitEnvironment2),传入参数检测有误，传入站点编码信息非法"));
            break;
        case -5:
            strLog.Format(_T("[主进程响应],组件初始化接口(IF_InitEnvironment2),传入参数检测有误，传入车道编码信息非法"));
            break;
        case -6:
            strLog.Format(_T("[主进程响应],组件初始化接口(IF_InitEnvironment2),传入参数检测有误，传入服务器信息非法"));
            break;
        case -7:
            strLog.Format(_T("[主进程响应],组件初始化接口(IF_InitEnvironment2),传入参数检测有误，传入省份编码为0"));
            break;
        case -8:
            strLog.Format(_T("[主进程响应],组件初始化接口(IF_InitEnvironment2),传入参数检测有误,传入调用者线程ID和窗体句柄不能同时为空"));
            break;
        case -9:
            strLog.Format(_T("[主进程响应],组件初始化接口(IF_InitEnvironment2),传入参数检测有误,传入区域参数格式不正确"));
            break;
        case -10:
            strLog.Format(_T("[主进程响应],组件初始化接口(IF_InitEnvironment2),传入参数检测有误,传入站点信息参数格式不正确"));
            break;
        case -11:
            strLog.Format(_T("[主进程响应],组件初始化接口(IF_InitEnvironment2),组件不允许多次初始化"));
            break;
        case -12:
            strLog.Format(_T("[主进程响应],组件初始化接口(IF_InitEnvironment2),内部初始化错误,无法启动业务管理线程"));
            break;
        }
        //记录日志
        if (m_strLastErrorDesc != strLog)
        {
            m_strLastErrorDesc = strLog;
            RecordLog(m_strLastErrorDesc, LOG_LEVEL_ERROR);
        }
    }

    RecordLog(FmtStr(_T("[主进程响应],组件初始化接口(IF_InitEnvironment2)调用结束")));

    return bReturn;
}


////////////////////////////////////////////////////////////////////////////////////////////////
// 函数名称：IF_InitEnvironment3
//
// 功能描述：初始化组件环境
//
// 输入参数：IN const UINT& nThreadID              -- 调用者指定的线程ID
//         ：IN const HWND& hWnd                   -- 调用者指定的窗体句柄
//         ：IN const char* szAreaInfo             -- 区域信息(区域编码#路段号)
//         ：IN const char* szLoaclStation         -- 站点信息(站号#站名)
//         ：IN const char* szLoaclLaneID          -- 车道编码
//         ：IN const char* szServerInfo           -- 服务器信息
//         ：IN const int& iProvinceID             -- 省份编码
//         ：IN OUT void* pIndPtr                  -- 个性化指针
//         ：void (*fun)(int option, int result,void* pIndPtr) -- 扣款/撤单结果通知，回调函数
// 输出参数：bool
// 编写人员：ROCY
// 编写时间：2017年03月16日
// 修改人员：
// 修改时间：
// 函数版本：1.0.0.1
// 备注说明：接口1-2，专门为广西定制的初始化函数，初始化操作的时候，保存一个个性化指针，在回掉的时候传出去
//
////////////////////////////////////////////////////////////////////////////////////////////////
bool WINAPI CTWSDNetPay_DllApp::IF_InitEnvironment3 (IN const UINT& nThreadID, IN const HWND& hWnd, IN const char* szAreaInfo, IN const char* szLoaclStation, IN const char* szLoaclLaneID, IN const char* szServerInfo, IN const int& iProvinceID, IN OUT void* pIndPtr, void (WINAPI *fun)(int option, int result, void* pIndPtr))
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    GetGlobalApp()->m_EtcPayCenter.Init();
    bool bReturn = false;
    CString strLog;

    RecordLog(FmtStr(_T("[主进程响应],组件初始化接口(IF_InitEnvironment3)调用开始,个性化指针参数:%04X，站信息:%s"), pIndPtr, szLoaclStation));
    try
    {
        /*******************检索参数是否正常*******************/
        if (  (0 == nThreadID) && (NULL == hWnd))
            throw - 1;
        if (NULL == fun)
            throw - 2;
        //回调函数指针赋值，便于通知调用者
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
        //填充配置信息
        TCHAR   szTemp[256] = {0};
        CString strTemp;
        //其它初始化参数赋值
        m_sTranstParam.ProvinceID            = iProvinceID;
        m_sTranstParam.ActiveNotifyMsgID     = 0;
        m_sTranstParam.CallerThreadID        = nThreadID;
        m_sTranstParam.CallerWnd             = hWnd;

        //车道编码
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

        //路段、区域编码
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

        //站点编码和站名
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
        //特殊处理代码区域
        switch (m_sTranstParam.ProvinceID)
        {
        case 44:
        {
            CString strSTID;
            switch (m_sTranstParam.LaneConfigInfo.RoadID)
            {
            //广东华快的出口站号特殊处理
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

        //新型操作，保存一个个性化指针
        m_pIndPtrFor45 = pIndPtr;

        //记录日志
        RecordLog(FmtStr(_T("[主进程响应],组件初始化接口(IF_InitEnvironment3)，内部参数配置:%s(省份编码:%d,区域编码:%d,路段编码:%d,站点编码:%s)"), strSTName, m_sTranstParam.ProvinceID, m_sTranstParam.LaneConfigInfo.AreaID, m_sTranstParam.LaneConfigInfo.RoadID, m_sTranstParam.LaneConfigInfo.StationID));



        GenerateRawInitPara(m_sTranstParam);
        //进行合法性校验
        m_bAuthorizedLegal = TRUE;
        //启动业务管理线程
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
        //将该线程重新唤醒
        m_pThreadManage->ResumeThread();

        //版本1060调整 进行代扣动态库加载处理
        AgencyInitDll(szAreaInfo, szLoaclStation, szLoaclLaneID, szServerInfo, iProvinceID);
        //标记初始化状态
        m_bIsInitEnvironment = TRUE;

        //标记通知方式
        m_iNotifyMode = 3;

        //返回结果
        bReturn = m_bIsInitEnvironment ? true : false;
    }
    catch (int& iErrorCode)
    {
        switch (iErrorCode)
        {
        case -1:
            strLog.Format(_T("[主进程响应],组件初始化接口(IF_InitEnvironment3),传入参数检测有误，传入线程ID与窗体句柄均为0"));
            break;
        case -2:
            strLog.Format(_T("[主进程响应],组件初始化接口(IF_InitEnvironment3),传入参数检测有误，传入回调函数指针错误"));
            break;
        case -3:
            strLog.Format(_T("[主进程响应],组件初始化接口(IF_InitEnvironment3),传入参数检测有误，传入区域信息非法"));
            break;
        case -4:
            strLog.Format(_T("[主进程响应],组件初始化接口(IF_InitEnvironment3),传入参数检测有误，传入站点编码信息非法"));
            break;
        case -5:
            strLog.Format(_T("[主进程响应],组件初始化接口(IF_InitEnvironment3),传入参数检测有误，传入车道编码信息非法"));
            break;
        case -6:
            strLog.Format(_T("[主进程响应],组件初始化接口(IF_InitEnvironment3),传入参数检测有误，传入服务器信息非法"));
            break;
        case -7:
            strLog.Format(_T("[主进程响应],组件初始化接口(IF_InitEnvironment3),传入参数检测有误，传入省份编码为0"));
            break;
        case -8:
            strLog.Format(_T("[主进程响应],组件初始化接口(IF_InitEnvironment3),传入参数检测有误,传入调用者线程ID和窗体句柄不能同时为空"));
            break;
        case -9:
            strLog.Format(_T("[主进程响应],组件初始化接口(IF_InitEnvironment3),传入参数检测有误,传入区域参数格式不正确"));
            break;
        case -10:
            strLog.Format(_T("[主进程响应],组件初始化接口(IF_InitEnvironment3),传入参数检测有误,传入站点信息参数格式不正确"));
            break;
        case -11:
            strLog.Format(_T("[主进程响应],组件初始化接口(IF_InitEnvironment3),组件不允许多次初始化"));
            break;
        case -12:
            strLog.Format(_T("[主进程响应],组件初始化接口(IF_InitEnvironment3),内部初始化错误,无法启动业务管理线程"));
            break;
        }
        //记录日志
        if (m_strLastErrorDesc != strLog)
        {
            m_strLastErrorDesc = strLog;
            RecordLog(m_strLastErrorDesc, LOG_LEVEL_ERROR);
        }
    }

	// 开启读卡器动态库的服务器
	bool bStartReaderServer = StartReaderServer();

	if(bStartReaderServer)
		RecordLog(FmtStr(_T("[主进程响应],组件初始化接口(StartReaderServer)开启读卡器动态库的服务器成功")));
	else
		RecordLog(FmtStr(_T("[主进程响应],组件初始化接口(StartReaderServer)开启读卡器动态库的服务器失败")));


    RecordLog(FmtStr(_T("[主进程响应],组件初始化接口(IF_InitEnvironment3)调用结束")));

    return bReturn;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// 函数名称：IF_Destroy
//
// 功能描述：清理资源
//
// 输入参数：void
// 输出参数：bool
// 编写人员：ROCY
// 编写时间：2016年7月23日
// 修改人员：
// 修改时间：
// 函数版本：1.0.0.1
// 备注说明：接口2
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
        // 1052版本调整，为了防止退出过程中其他函数的非法访问，将初始化标记置为假
        m_bIsInitEnvironment = FALSE;

        DWORD dwSleepTime = 0;

        CString strLog;
        /**********************************************清理各种线程资源**********************************************/

        strLog.Format(_T("[主进程响应],组件资源清理接口(IF_Destroy),正在开始退出后台线程..."));
        //记录日志
        RecordLog(strLog);

        //线程退出标记
        DWORD dwThreadExitFlag = 0x00;
        //允许重试的次数
        int iAllowTryCount = 10;
        //已经重试次数
        int iHaveTryCount = 0;
        //每次重试的时候间隔时间（单位：毫秒）
        DWORD   dwSpanTime = 100;

        //停止业务管理线程
        if (NULL != m_pThreadManage)
        {
            //设置该线程推出标记为真
            //m_pThreadManage->m_bExit = TRUE;
            m_pThreadManage->ThreadQuit();
            //沉默200毫秒
            ::Sleep(20);

            //每次等待1000毫秒，重试次数不超过10次
            while (OP_SUCCESS != fnCloseThread(m_pThreadManage->m_hThread, 1000))
            {
                Sleep(dwSpanTime);
                ++iHaveTryCount;
                if (iHaveTryCount >= iAllowTryCount)
                {
                    //标记该线程未能正常退出
                    dwThreadExitFlag |= 0x01;
                    //终止尝试
                    break;
                }
            }
            //MessageBox(0,"m_pThreadManage","",0);
            m_pThreadManage = NULL;


            strLog.Format(_T("[主进程响应],组件资源清理接口(IF_Destroy),%s"), FmtStr(_T("【业务管理线程】已经停止(重试次数 %d)..."), iHaveTryCount));
            //记录日志
            RecordLog(strLog);
        }

        //释放代扣动态库
        m_AgencyPay.Destroy();
        //释放一些指针资源
        m_fnCallbackNotifyFun   = NULL;
        m_fnCallbackNotifyFun45 = NULL;
        m_pIndPtrFor45          = NULL;
        memset(&m_sTranstParam, sizeof(tagTranstParam), 0);
        memset(&m_sCurrentDebitResult, sizeof(tagDebitResultItems), 0);
        //重置标记位
        m_bIsInitEnvironment                        = FALSE;
        m_bAuthorizedLegal                          = FALSE;
        m_bNetErrorHttpServerInTollNetNotExist      = FALSE;
        m_bNetErrorHttpServerTrawePlatformNotExist  = FALSE;
        //沉默500毫秒
        ::Sleep(m_iExitWaitForTime);

		IF_EventDestroy();

        //记录日志
        strLog.Format(_T("[主进程响应],组件即将退出..."));
        RecordLog(strLog, 0, TRUE);
        //标记成功
        bReturn = true;
    }
    catch (int& iErrorCode)
    {
    }

    return bReturn;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// 函数名称：IF_GetLastErrorDesc
//
// 功能描述：获取上一次错误信息
//
// 输入参数：void
// 输出参数：char*
// 编写人员：ROCY
// 编写时间：2016年7月23日
// 修改人员：
// 修改时间：
// 函数版本：1.0.0.1
// 备注说明：接口3
//
////////////////////////////////////////////////////////////////////////////////////////////////
const char* WINAPI CTWSDNetPay_DllApp::IF_GetLastErrorDesc(void)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
	RecordLog(FmtStr(_T("[ETC特情业务],获取上次错误描述接口(IF_GetLastErrorDesc)调用开始")));

    return m_strLastErrorDesc.GetBuffer(0);
}

////////////////////////////////////////////////////////////////////////////////////////////////
// 函数名称：CTWSDNetPay_DllApp::IF_StartGetAccountInfo
//
// 功能描述：启动获取用户信息操作
//
// 输入参数：IN const int& iOvertime
// 输出参数：bool
// 编写人员：ROCY
// 编写时间：2016年8月2日
// 修改人员：
// 修改时间：
// 函数版本：1.0.0.1
// 备注说明：接口4
//
////////////////////////////////////////////////////////////////////////////////////////////////
bool WINAPI CTWSDNetPay_DllApp::IF_StartGetAccountInfo(IN const int& iOvertime)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    bool bReturn = false;
    CString strLog;
    RecordLog(FmtStr(_T("[主进程响应],启动获取用户信息操作接口(IF_StartGetAccountInfo)调用开始")));
    try
    {
        //判断组件是否已经被初始化
        if (!m_bIsInitEnvironment)
            throw - 1;
        //判断组件是否成功授权
        if (!m_bAuthorizedLegal)
            throw - 2;
        //判断业务处理线程是否异常
        if (NULL == m_pThreadManage)
            throw - 3;
        //给业务管理线程发送消息，要求控制硬件进行获取用户信息操作
        m_pThreadManage->PostThreadMessage(WM_THREAD_DEVICE_START_GET_DATA, (WPARAM)iOvertime, (LPARAM)0x01);
        //标记成功
        bReturn = true;
    }
    catch (int& iErrorCode)
    {
        BYTE byFunCode      = 0x03;
        BYTE byExecuteState = RCC_INNER_ERROR;
        switch (iErrorCode)
        {
        case -1:
            strLog.Format(_T("[主进程响应],启动获取用户信息操作(IF_StartGetAccountInfo)出错,组件未成功初始化"));
            byExecuteState  = RCC_COM_NOT_INIT;
            break;
        case -2:
            strLog.Format(_T("[主进程响应],启动获取用户信息操作(IF_StartGetAccountInfo)出错,组件未被授权"));
            byExecuteState  = RCC_COM_ILLEGAL;
            break;
        case -3:
            strLog.Format(_T("[主进程响应],启动获取用户信息操作(IF_StartGetAccountInfo)出错,组件内部资源异常"));
            break;
        }
        //保存错误描述
        if (strLog != m_strLastErrorDesc)
        {
            m_strLastErrorDesc = strLog;
            RecordLog(m_strLastErrorDesc, LOG_LEVEL_ERROR);
        }
        //回馈调用者结果
        if (0 != m_sTranstParam.CallerThreadID)
        {
            ::PostThreadMessage(m_sTranstParam.CallerThreadID, m_sTranstParam.ActiveNotifyMsgID, (WPARAM)MAKELONG(MAKEWORD(byFunCode, byExecuteState), MAKEWORD(0, 0)), (LPARAM)0);
        }

        if (::IsWindow(m_sTranstParam.CallerWnd))
        {
            ::PostMessage(m_sTranstParam.CallerWnd, m_sTranstParam.ActiveNotifyMsgID, (WPARAM)MAKELONG(MAKEWORD(byFunCode, byExecuteState), MAKEWORD(0, 0)), (LPARAM)0);
        }
        //保存日志
        RecordLog(FmtStr(_T("[主进程响应],获取用户信息操作预处理失败，将结果通知调用者(功能代码:%d,执行状况:%d)"), byFunCode, byExecuteState));
    }
    RecordLog(FmtStr(_T("[主进程响应],启动获取用户信息操作接口(IF_StartGetAccountInfo)调用结束")));

    return bReturn;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// 函数名称：CTWSDNetPay_DllApp::IF_StopGetAccountInfo
//
// 功能描述：停止获取用户信息操作
//
// 输入参数：void
// 输出参数：bool
// 编写人员：ROCY
// 编写时间：2016年8月2日
// 修改人员：
// 修改时间：
// 函数版本：1.0.0.1
// 备注说明：接口5
//
////////////////////////////////////////////////////////////////////////////////////////////////
bool WINAPI CTWSDNetPay_DllApp::IF_StopGetAccountInfo(void)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    bool bReturn = false;
    CString strLog;
    RecordLog(FmtStr(_T("[主进程响应],停止获取用户信息操作接口(IF_StopGetAccountInfo)调用开始")));
    try
    {
        //判断组件是否已经被初始化
        if (!m_bIsInitEnvironment)
            throw - 1;
        //判断组件是否成功授权
        if (!m_bAuthorizedLegal)
            throw - 2;
        //判断业务处理线程是否异常
        if (NULL == m_pThreadManage)
            throw - 3;
        //给业务管理线程发送消息，要求控制硬件进行获取用户信息操作
        m_pThreadManage->PostThreadMessage(WM_THREAD_DEVICE_STOP_GET_DATA, (WPARAM)0, (LPARAM)0);
        //标记成功
        bReturn = true;
    }
    catch (int& iErrorCode)
    {
        switch (iErrorCode)
        {
        case -1:
            strLog.Format(_T("[主进程响应],停止获取用户信息操作(IF_StopGetAccountInfo)出错,组件未成功初始化"));
            break;
        case -2:
            strLog.Format(_T("[主进程响应],停止获取用户信息操作(IF_StopGetAccountInfo)出错,组件未被授权"));
            break;
        case -3:
            strLog.Format(_T("[主进程响应],停止获取用户信息操作(IF_StopGetAccountInfo)出错,组件内部资源异常"));
            break;
        case -4:
            strLog.Format(_T("[主进程响应],停止获取用户信息操作(IF_StopGetAccountInfo)出错,硬件未处于工作状态"));
            break;
        }
        //保存错误描述
        if (strLog != m_strLastErrorDesc)
        {
            m_strLastErrorDesc = strLog;
            RecordLog(m_strLastErrorDesc, LOG_LEVEL_ERROR);
        }

    }
    RecordLog(FmtStr(_T("[主进程响应],停止获取用户信息操作接口(IF_StopGetAccountInfo)调用结束")));
    return bReturn;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// 函数名称：CTWSDNetPay_DllApp::IF_GetAccountInfo
//
// 功能描述：获取用户信息
//
// 输入参数：IN OUT char* szParamContext
//         ：IN OUT int& iParamSize
//         ：IN const int& iFormatType
// 输出参数：bool
// 编写人员：ROCY
// 编写时间：2016年8月2日
// 修改人员：
// 修改时间：
// 函数版本：1.0.0.1
// 备注说明：接口6
//
////////////////////////////////////////////////////////////////////////////////////////////////
bool WINAPI CTWSDNetPay_DllApp::IF_GetAccountInfo(IN OUT char* szParamContext, IN OUT int& iParamSize, IN const int& iFormatType)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    bool bReturn = false;
    CString strLog;
    CString strDataString;
    RecordLog(FmtStr(_T("[主进程响应],获取用户信息接口(IF_GetAccountInfo)调用开始,参数1:%04X,参数2:%d,参数3:%d"), szParamContext, iParamSize, iFormatType));
    try
    {
        //判断组件是否已经被初始化
        if (!m_bIsInitEnvironment)
            throw - 1;
        //判断组件是否成功授权
        if (!m_bAuthorizedLegal)
            throw - 2;
        //判断业务处理线程是否异常
        if (NULL == m_pThreadManage)
            throw - 3;
        //判断参数是否异常
        if ((NULL == szParamContext) || (iParamSize < 1024))
            throw - 4;
        if ( (iFormatType > 2) || (iFormatType < 1))
            throw - 5;
        //判断数据是否已经获取成功
        if (!m_pThreadManage->GetAccountDataReadyState())
            throw - 6;
        //获取数据
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
        RecordLog(FmtStr(_T("[主进程响应],获取用户信息接口(IF_GetAccountInfo)输出数据:%s"), strDataString));
        memcpy(szParamContext, strDataString.GetBuffer(0), strDataString.GetLength() >= iParamSize ? (iParamSize - 1) : strDataString.GetLength());
        strDataString.ReleaseBuffer();
        //标记成功
        bReturn = true;
        //设置用户数据准备状态为假
        m_pThreadManage->SetAccountDataReadyState(FALSE);
    }
    catch (int& iErrorCode)
    {
        switch (iErrorCode)
        {
        case -1:
            strLog.Format(_T("[主进程响应],获取用户信息操作(IF_GetAccountInfo)出错,组件未成功初始化"));
            break;
        case -2:
            strLog.Format(_T("[主进程响应],获取用户信息操作(IF_GetAccountInfo)出错,组件未被授权"));
            break;
        case -3:
            strLog.Format(_T("[主进程响应],获取用户信息操作(IF_GetAccountInfo)出错,组件内部资源异常"));
            break;
        case -4:
            strLog.Format(_T("[主进程响应],获取用户信息操作(IF_GetAccountInfo)出错,用户数据未能获取成功"));
            break;
        }
        //保存错误描述
        if (strLog != m_strLastErrorDesc)
        {
            m_strLastErrorDesc = strLog;
            RecordLog(m_strLastErrorDesc, LOG_LEVEL_ERROR);
        }
    }
    RecordLog(FmtStr(_T("[主进程响应],获取用户信息接口(IF_GetAccountInfo)调用结束")));
    return bReturn;
}


bool WINAPI CTWSDNetPay_DllApp::IF_DebitMoney_Future(IN const char* szParamContext, IN const int& iParamSize, IN const int& iFormatType)
{
    CInterfaceTraceInfo trace_interface("IF_DebitMoney", pystring::replace(szParamContext, "\r\n", ""));
    GetGlobalApp()->m_bCurrentDebitMode = TRUE;
    GetGlobalApp()->m_bIsTrading = TRUE;
    if (!m_bIsInitEnvironment)
    {
        TW_TRACE_DIARY_TYPE("未进行初始化", TW_TRACE_TPPE_INTERFACE);
        NotifyCaller(1, RCC_COM_NOT_INIT);
        return false;
    }
    if (0 == m_pThreadManage)
    {
        TW_TRACE_DIARY_TYPE("管理线程无效", TW_TRACE_TPPE_INTERFACE);
        NotifyCaller(1, RCC_INNER_ERROR);
        return false;
    }
    if (szParamContext == 0 || iParamSize == 0)
    {
        TW_TRACE_DIARY_TYPE("参数为空", TW_TRACE_TPPE_INTERFACE);
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

        TW_TRACE_DIARY_TYPE("不支持的参数类型", TW_TRACE_TPPE_INTERFACE);
        NotifyCaller(1, RCC_PARAM_ERROR);
        return false;
    }
    AdjustExpwayInfo(dp);//修正收费站编码等信息
    std::string image_id, task_id;
    GetGlobalApp()->QueryImageIdFromLPRSystem(image_id, task_id);
    strncpy(dp.ImageId, image_id.c_str(), sizeof(dp.ImageId) - 1);
    strncpy(dp.TaskId, task_id.c_str(), sizeof(dp.TaskId) - 1);

    bool start_success = (m_pThreadManage->StartNewPay(dp) == 0);
    return start_success;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// 函数名称：IF_DebitMoney
//
// 功能描述：执行扣款操作
//
// 输入参数：IN const char* szParamContext
//         ：IN const int& iParamSize
//         ：IN const int& iFormatType
// 输出参数：bool
// 编写人员：ROCY
// 编写时间：2016年7月23日
// 修改人员：
// 修改时间：
// 函数版本：1.0.0.1
// 备注说明：接口7
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
//    CString strLog = FmtStr(_T("[主进程响应],执行扣款操作接口(IF_DebitMoney)调用开始,参数1:%s,参数2:%d,参数3:%d"), szParamContext, iParamSize, iFormatType);
//    strLog.Replace(_T("\r"), _T(""));
//    strLog.Replace(_T("\n"), _T(""));
//    RecordLog(strLog);
//    CString strOperationMode(_T("未知"));
//    CString strDataString;
//    CString strAnalyError;
//    try
//    {
//        /*****************************校核过程***********************************/
//        //判断组件是否初始化成功
//        if (!m_bIsInitEnvironment)
//            throw - 1;
//        //判断合法性
//        if (!m_bAuthorizedLegal)
//            throw - 2;
//        //判断参数是否异常
//        if ((NULL == szParamContext) || (0 == iParamSize) || (iFormatType > 2 || iFormatType < 1))
//            throw - 3;
//        //判断上次交易是否完成
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
//            //复制数据
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
//            //新增出口收费员信息
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
////          strDataString.Format(_T("{\"DebitParamContext\":{\"Data\":{\"OperationMode\":2,\"OverTime\":60000,\"Vehicle\":{\"License\":\"0\",\"Weight\":0,\"AxisCount\":0,\"Type\":1,\"VLColor\":0,\"Class\":1},\"Operation\":{\"ShiftID\":1,\"PassCertificateType\":0,\"CardID\":\"0\",\"TollDate\":\"20160823\"},\"Entry\":{\"RoadID\":30,\"License\":\"\",\"AreaID\":4412,\"StationName\":\"华快岑村\",\"VColor\":0,\"VClass\":9,\"VType\":1,\"Time\":\"2016-08-23 14:34:28\",\"LaneID\":0,\"StationID\":8},\"Debit\":{\"PayCertificateType\":1,\"PayCertificateCode\":\"283487891099961852\",\"Money\":200,\"PayIdentifier\":\"1E09381F4F1B90000000000000000004\",\"SubTime\":\"2016-08-23 14:34:28\",\"Type\":1}},\"Version\":\"1.0\"}}"));
//
//            pDebitMoney = (CDVJsonParamDebitMoney*)jsonParamOperation.FormatResponseResult2Class(strDataString, JSON_DT_PARAM_DEBIT_MONEY);
//            if (NULL == pDebitMoney)
//            {
//                strAnalyError.Format(_T(""));
//                throw - 5;
//            }
//            //复制数据
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
//            //新增出口收费员信息
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
//        //add 2018-01 ,增加时间转换，非东八区，转换为东八区
//        m_pCurrentDebitInfo.SubTime = FixUTC8(m_pCurrentDebitInfo.SubTime);
//
//        //特殊处理代码区域
//        switch (m_sTranstParam.ProvinceID)
//        {
//        case 44:
//        {
//            CString strSTID;
//            switch (m_sTranstParam.LaneConfigInfo.RoadID)
//            {
//            //广东华快的入口站号特殊处理
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
//        //检测业务管理线程是否正常
//        if (NULL == m_pThreadManage)
//            throw - 6;
//
//        //标记当前交易正在进行
//        m_bIsTrading = TRUE;
//        /*****************************响应操作***********************************/
//        //add 2018-04
//        GetGlobalApp()->QueryImageIdFromLPRSystem();
//
//        int agency_result = 255;
//        //版本1060调整，首先进行代扣处理
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
//                RecordLog(_T("[主进程响应],执行扣款操作接口(IF_DebitMoney): 不进行扫码处理,接口调用结束"));
//                m_bIsTrading = false;
//                return true;
//            }
//            GetGlobalApp()->NotifyPayStatus(1, agency_result);
//
//
//            //正常的扫码流程
//            switch (m_pCurrentDebitInfo.OperationMode)
//            {
//            case 1://整合操作
//            {
//                strOperationMode.Format(_T("整合模式"));
//                m_bCurrentDebitMode = TRUE;
//                //重构支付参数
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
//                //进行资源申请
//                CString* pstrString = new CString();
//                if (NULL == pstrString)
//                    throw - 8;
//                pstrString->Format(_T("%s"), strXMLString);
//
//                //关闭上次的定时器
//                m_pThreadManage->StopTimer(TIMER_ID_DEBIT_MONEY);
//                m_pThreadManage->StopTimer(TIMER_ID_DEBIT_CHECK);
//                //启动扣款超时定时器
//                m_pThreadManage->StartTimer(TIMER_ID_DEBIT_MONEY, m_pCurrentDebitInfo.OverTime);
//                //开始计时
//                m_dwTradeBeginOvertime = GetTickCount();
//                //启动扣款流程
//                m_pThreadManage->PostThreadMessage(WM_THREAD_MANAGE_START_DEBIT_FLOW, (WPARAM)pstrString, (LPARAM)0);
//                //添加描述
//                m_strLastErrorDesc.Format(_T("[主进程响应],执行扣款操作(IF_DebitMoney),业务正在处理(整合操作)"));
//            }
//            break;
//            case 2://单体操作
//            {
//                strOperationMode.Format(_T("单体模式"));
//                m_bCurrentDebitMode = FALSE;
//                //开始计时
//                m_dwTradeBeginOvertime = GetTickCount();
//                //关闭上次的定时器
//                m_pThreadManage->StopTimer(TIMER_ID_DEBIT_MONEY);
//                m_pThreadManage->StopTimer(TIMER_ID_DEBIT_CHECK);
//                //启动扣款超时定时器
//                m_pThreadManage->StartTimer(TIMER_ID_DEBIT_MONEY, m_pCurrentDebitInfo.OverTime);
//                //执行扣款操作
//                m_pThreadManage->PostThreadMessage(WM_THREAD_MANAGE_ONLY_DEBIT_MONEY, (WPARAM)0, (LPARAM)0);
//                //添加描述
//                m_strLastErrorDesc.Format(_T("[主进程响应],执行扣款操作(IF_DebitMoney),业务正在处理(单体操作)"));
//            }
//            break;
//            }
//            //记录日志
//            RecordLog(m_strLastErrorDesc);
//        }
//
//        //标记成功
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
//            strLog.Format(_T("[主进程响应],执行扣款操作(IF_DebitMoney)出错(%s),组件未成功初始化"), strOperationMode);
//            byExecuteState  = RCC_COM_NOT_INIT;
//            break;
//        case -2:
//            strLog.Format(_T("[主进程响应],执行扣款操作(IF_DebitMoney)出错(%s),合法性校验失败"), strOperationMode);
//            byExecuteState  = RCC_COM_ILLEGAL;
//            break;
//        case -3:
//            strLog.Format(_T("[主进程响应],执行扣款操作(IF_DebitMoney)出错(%s),传入参数异常"), strOperationMode);
//            byExecuteState  = RCC_PARAM_ERROR;
//            break;
//        case -4:
//            strLog.Format(_T("[主进程响应],执行扣款操作(IF_DebitMoney)出错(%s),传入参数(XML格式)解析出错，详细描述:%s"), strOperationMode, strAnalyError);
//            byExecuteState  = RCC_PARAM_ERROR;
//            break;
//        case -5:
//            strLog.Format(_T("[主进程响应],执行扣款操作(IF_DebitMoney)出错(%s),传入参数(JSON格式)解析出错"), strOperationMode);
//            byExecuteState  = RCC_PARAM_ERROR;
//            break;
//        case -6:
//            strLog.Format(_T("[主进程响应],执行扣款操作(IF_DebitMoney)出错(%s),内部处理异常:业务管理模块未能正常工作"), strOperationMode);
//            break;
//        case -7:
//            strLog.Format(_T("[主进程响应],执行扣款操作(IF_DebitMoney)出错(%s),内部处理异常:上次交易未完成，不允许频繁调用"), strOperationMode);
//            byExecuteState  = RCC_LAST_TRADE_NOT_END;
//            break;
//        case -8:
//            strLog.Format(_T("[主进程响应],执行扣款操作(IF_DebitMoney)出错(%s),内部处理异常:资源申请失败"), strOperationMode);
//            break;
//            //case -9:
//            //  RecordLog(_T("[主进程响应],执行扣款操作接口(IF_DebitMoney): 不进行扫码处理"));
//            //  break;
//        }
//        //保存错误描述
//        if (strLog != m_strLastErrorDesc)
//        {
//            m_strLastErrorDesc = strLog;
//            RecordLog(m_strLastErrorDesc, LOG_LEVEL_ERROR);
//        }
//        //回馈调用者结果
//        NotifyCaller(byFunCode, byExecuteState);
//        //保存日志
//        RecordLog(FmtStr(_T("[主进程响应],扣款操作预处理失败，将结果通知调用者(功能代码:%d,执行状况:%d)"), byFunCode, byExecuteState));
//    }
//    RecordLog(FmtStr(_T("[主进程响应],执行扣款操作接口(IF_DebitMoney)调用结束")));
//
//    return bReturn;
}

bool WINAPI CTWSDNetPay_DllApp::IF_DebitCancel_Future(IN const char* szParamContext, IN const int& iParamSize, IN const int& iFormatType)
{
    CInterfaceTraceInfo trace_interface("IF_DebitCancel", pystring::replace(szParamContext, "\r\n", ""));
    if (!m_bIsInitEnvironment)
    {
        TW_TRACE_DIARY_TYPE("未进行初始化", TW_TRACE_TPPE_INTERFACE);
        NotifyCaller(2, RCC_COM_NOT_INIT);
        return false;
    }
    if (0 == m_pThreadManage)
    {
        TW_TRACE_DIARY_TYPE("管理线程无效", TW_TRACE_TPPE_INTERFACE);
        NotifyCaller(2, RCC_INNER_ERROR);
        return false;
    }
    if (szParamContext == 0 || iParamSize == 0)
    {
        TW_TRACE_DIARY_TYPE("参数为空", TW_TRACE_TPPE_INTERFACE);
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
        TW_TRACE_DIARY_TYPE("不支持的参数类型", TW_TRACE_TPPE_INTERFACE);
        NotifyCaller(2, RCC_PARAM_ERROR);
        return false;
    }
    dp.EnableNotification = true;
    bool  cancle_success = (m_pThreadManage->CancelTargetPay(dp) == 0);
    return cancle_success;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// 函数名称：IF_DebitCancel
//
// 功能描述：执行撤单操作
//
// 输入参数：IN const char* szParamContext
//         ：IN const int& iParamSize
//         ：IN const int& iFormatType
// 输出参数：bool
// 编写人员：ROCY
// 编写时间：2016年7月23日
// 修改人员：
// 修改时间：
// 函数版本：1.0.0.1
// 备注说明：接口8
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
    CString strLog = FmtStr(_T("[主进程响应],执行撤单操作接口(IF_DebitCancel)调用开始,参数1:%s,参数2:%d,参数3:%d"), szParamContext, iParamSize, iFormatType);
    strLog.Replace(_T("\r"), _T(""));
    strLog.Replace(_T("\n"), _T(""));
    RecordLog(strLog);

    CString strDataString;
    CString strPayIdentifier;
    CDVXMLDebitCancel xml;
    try
    {
        /*****************************校核过程***********************************/
        //判断组件是否初始化成功
        if (!m_bIsInitEnvironment)
            throw - 1;
        //判断合法性
        if (!m_bAuthorizedLegal)
            throw - 2;
        //判断参数是否异常
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
        //检测业务管理线程是否正常
        if (NULL == m_pThreadManage)
            throw - 6;
        //检测撤单交易标识是否合法
        strPayIdentifier.TrimLeft();
        strPayIdentifier.TrimRight();
        if (strPayIdentifier.IsEmpty())
            throw - 7;
        //进行资源申请
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
            strLog.Format(_T("[主进程响应],执行撤单操作(IF_DebitCancel)出错,组件未成功初始化"));
            byExecuteState  = RCC_COM_NOT_INIT;
            break;
        case -2:
            strLog.Format(_T("[主进程响应],执行撤单操作(IF_DebitCancel)出错,合法性校验失败"));
            byExecuteState  = RCC_COM_ILLEGAL;
            break;
        case -3:
            strLog.Format(_T("[主进程响应],执行撤单操作(IF_DebitCancel)出错,传入参数异常"));
            byExecuteState  = RCC_PARAM_ERROR;
            break;
        case -4:
            strLog.Format(_T("[主进程响应],执行撤单操作(IF_DebitCancel)出错,传入参数解析出错，详细描述:%s"), xml.m_strLastErrorDesc);
            byExecuteState  = RCC_PARAM_ERROR;
            break;
        case -5:
            strLog.Format(_T("[主进程响应],执行撤单操作(IF_DebitCancel)出错,传入参数解析出错(JSON)"));
            byExecuteState  = RCC_PARAM_ERROR;
            break;
        case -6:
            strLog.Format(_T("[主进程响应],执行撤单操作(IF_DebitCancel)出错,内部处理异常:业务管理模块未能正常工作"));
            break;
        case -7:
            strLog.Format(_T("[主进程响应],执行撤单操作(IF_DebitCancel)出错,传入参数有误,需要撤销的订单对应的交易识别码为空"));
            byExecuteState  = RCC_PARAM_ERROR;
            break;
        case -8:
            strLog.Format(_T("[主进程响应],执行撤单操作(IF_DebitCancel)出错,内部处理异常:资源申请失败"));
            break;
        }
        //保存错误描述
        if (strLog != m_strLastErrorDesc)
        {
            m_strLastErrorDesc = strLog;
            RecordLog(m_strLastErrorDesc, LOG_LEVEL_ERROR);
        }
        //回馈调用者结果
        NotifyCaller(byFunCode, byExecuteState);
        //保存日志
        RecordLog(FmtStr(_T("[主进程响应],撤单操作预处理失败，将结果通知调用者(功能代码:%d,执行状况:%d)"), byFunCode, byExecuteState));
    }
    RecordLog(FmtStr(_T("[主进程响应],执行撤单操作接口(IF_DebitCancel)调用结束")));
    return bReturn;
}



bool  WINAPI CTWSDNetPay_DllApp::IF_GetDebitResult_Future(IN OUT char* szParamContext, IN OUT int& iParamSize, IN const int& iFormatType)
{
    CInterfaceTraceInfo trace_interface("IF_GetDebitResult");
    if (!m_bIsInitEnvironment)
    {
        TW_TRACE_DIARY_TYPE("未进行初始化", TW_TRACE_TPPE_INTERFACE);
        return false;
    }
    if (0 == m_pThreadManage)
    {
        TW_TRACE_DIARY_TYPE("管理线程无效", TW_TRACE_TPPE_INTERFACE);
        return false;
    }
    if (iParamSize == 0)
    {
        TW_TRACE_DIARY_TYPE("参数为空", TW_TRACE_TPPE_INTERFACE);
        return false;
    }
    DebitResultParameter dp;
    bool get_result_success = (m_pThreadManage->GetCurrentDebitResult(dp) == 0);
    if (!get_result_success)
    {
        TW_TRACE_DIARY_TYPE("获取结果失败", TW_TRACE_TPPE_INTERFACE);
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
        TW_TRACE_DIARY_TYPE("不支持的参数类型", TW_TRACE_TPPE_INTERFACE);
        return false;
    }
    int result_size = static_cast<int>(result_str.size());
    if ( iParamSize < result_size)
    {
        char err_log[100] = {0};
        sprintf(err_log, "输出缓存过小，目前大小:%d，需要大小:%d", iParamSize, result_size);
        TW_TRACE_DIARY_TYPE(err_log, TW_TRACE_TPPE_INTERFACE);
        return false;
    }

    strncpy(szParamContext, result_str.c_str(), result_size);
    iParamSize = result_size;
    TW_TRACE_DIARY_TYPE("结果内容:" + result_str, TW_TRACE_TPPE_INTERFACE);
    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// 函数名称：IF_GetDebitResult
//
// 功能描述：获取扣款/二次扣款结果
//
// 输入参数：IN OUT char* szParamContext
//         ：IN OUT int& iParamSize
//         ：IN const int& iFormatType
// 输出参数：bool
// 编写人员：ROCY
// 编写时间：2016年7月23日
// 修改人员：
// 修改时间：
// 函数版本：1.0.0.1
// 备注说明：接口9
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
    RecordLog(FmtStr(_T("[主进程响应],获取扣款结果接口(IF_GetDebitResult)调用开始,参数1:%04X,参数2:%d,参数3:%d"), szParamContext, iParamSize, iFormatType));
    try
    {
        //判断组件是否初始化成功
        if (!m_bIsInitEnvironment)
            throw - 1;
        //判断合法性
        if (!m_bAuthorizedLegal)
            throw - 2;
        //判断参数是否异常
        if ((iFormatType > 2) || (iFormatType < 1) || (NULL == szParamContext))
            throw - 3;
        //判断结果是否已经准备好
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
        RecordLog(FmtStr(_T("[主进程响应],获取扣款结果接口(IF_GetDebitResult)输出结果如下:(%s)%s "), (0x01 == iFormatType) ? _T("数据格式:XML") : _T("数据格式:JSON"), strDataString));

        _tcscpy_s(szParamContext, strDataString.GetLength() + 1, strDataString.GetBuffer(0));

        //char chs[3000]={0};
        //sprintf(chs,"参数长度:%d,结果长度:%d,扣款结果：%s",iParamSize,strDataString.GetLength(),szParamContext);
        //GetGlobalApp()->RecordLog(chs);
        strDataString.ReleaseBuffer();
        //重置交易结果
        m_sCurrentDebitResult.Valid = FALSE;

        //标记成功
        bReturn = true;
    }
    catch (int& iErrorCode)
    {
        switch (iErrorCode)
        {
        case -1:
            strLog.Format(_T("[主进程响应],获取扣款结果操作(IF_GetDebitResult)出错,组件未成功初始化"));
            break;
        case -2:
            strLog.Format(_T("[主进程响应],获取扣款结果操作(IF_GetDebitResult)出错,组件未被授权"));
            break;
        case -3:
            strLog.Format(_T("[主进程响应],获取扣款结果操作(IF_GetDebitResult)出错,传入参数异常"));
            break;
        case -4:
            strLog.Format(_T("[主进程响应],获取扣款结果操作(IF_GetDebitResult)出错,扣款结果未准备好"));
            break;
        }
        //保存错误描述
        if (strLog != m_strLastErrorDesc)
        {
            m_strLastErrorDesc = strLog;
            RecordLog(m_strLastErrorDesc, LOG_LEVEL_ERROR);
        }
    }
    RecordLog(FmtStr(_T("[主进程响应],获取扣款结果接口(IF_GetDebitResult)调用结束")));
    return bReturn;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// 函数名称：CTWSDNetPay_DllApp::IF_SetMMI
//
// 功能描述：设置交互界面
//
// 输入参数：IN const char* szParamContext
//         ：IN const int& iParamSize
//         ：IN const int& iFormatType
// 输出参数：bool
// 编写人员：ROCY
// 编写时间：2016年8月2日
// 修改人员：
// 修改时间：
// 函数版本：1.0.0.1
// 备注说明：接口10
//
////////////////////////////////////////////////////////////////////////////////////////////////
bool WINAPI CTWSDNetPay_DllApp::IF_SetMMI(IN const char* szParamContext, IN const int& iParamSize, IN const int& iFormatType)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    bool bReturn = false;
    CString strLog = FmtStr(_T("[主进程响应],设置交互界面接口(IF_SetMMI)调用开始"));
    strLog.Replace(_T("\r"), _T(""));
    strLog.Replace(_T("\n"), _T(""));
    RecordLog(strLog);

    CString strParamString(szParamContext);
    CString strDataString;
    CString strSubError;
    try
    {
        //判断组件是否已经被初始化
        if (!m_bIsInitEnvironment)
            throw - 1;
        //判断组件是否成功授权
        if (!m_bAuthorizedLegal)
            throw - 2;
        //判断业务处理线程是否异常
        if (NULL == m_pThreadManage)
            throw - 3;
        if (NULL == m_pThreadManage->m_pThreadDevice)
            throw - 4;
        //判断参数是否异常
        if ((NULL == szParamContext) || (0 == iParamSize))
            throw - 5;
        //判断参数类型
        if ( (iFormatType > 2) || (iFormatType < 1))
            throw - 6;
        //获取硬件状态
        //  DWORD dwHDState = m_pThreadManage->m_pThreadDevice->GetDeviceState();

        int iMMIType = 0x00;

        if (0x01 == iFormatType)
        {
            //XML格式
            CDVXMLMMI xml;
            if (!xml.Init(strParamString))
            {
                strSubError = xml.m_strLastErrorDesc;
                throw - 7;
            }
            //重构XML数据
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
                    //获取金额、车型车种、重量、限重
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
                        case 1://车型
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
                        case 2://金额
                        {
                            iTemp = _ttoi(xml.Text.GetAt(i).Context);
                            iMoney = iTemp;
                        }
                        break;
                        case 3://载重
                        {
                            iTemp = _ttoi(xml.Text.GetAt(i).Context);
                            iWeight = iTemp;
                        }
                        break;
                        case 4://轴数
                        {
                            iTemp = _ttoi(xml.Text.GetAt(i).Context);
                            iAxis = iTemp;
                        }
                        break;
                        case 5://限重
                        {
                            iTemp = _ttoi(xml.Text.GetAt(i).Context);
                            iLimit = iTemp;
                        }
                        break;
                        case 6://电子支付卡余额
                        {
                            iTemp = _ttoi(xml.Text.GetAt(i).Context);
                            iBalance = iTemp;
                        }
                        break;
                        case 7://入口收费站点名称
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
            //JSON格式
            CDVJsonParamMMI* pJsonMMI = NULL;
            CJsonParamOperate jsonOperation;

//          strParamString.Format(_T("{\"MMIParamContext\":{\"Data\":{\"ClearMMI\":0,\"ItemData\":{\"Graph\":null,\"Voice\":null,\"Text\":[{\"Sub\":1,\"Row\":0,\"Type\":1,\"IsScroll\":0,\"Context\":\"1\"},{\"Sub\":2,\"Row\":0,\"Type\":1,\"IsScroll\":0,\"Context\":\"200\"},{\"Sub\":3,\"Row\":0,\"Type\":1,\"IsScroll\":0,\"Context\":\"0\"},{\"Sub\":4,\"Row\":0,\"Type\":1,\"IsScroll\":0,\"Context\":\"0\"},{\"Sub\":5,\"Row\":0,\"Type\":1,\"IsScroll\":0,\"Context\":\"0\"},{\"Sub\":6,\"Row\":0,\"Type\":1,\"IsScroll\":0,\"Context\":\"0\"},{\"Sub\":7,\"Row\":0,\"Type\":1,\"IsScroll\":0,\"Context\":\"华快岑村\"}]},\"ItemCount\":7},\"Version\":\"1.0\"}}"));

            pJsonMMI = (CDVJsonParamMMI*)jsonOperation.FormatResponseResult2Class(strParamString, JSON_DT_PARAM_MMI);
            if (NULL == pJsonMMI)
            {
                strSubError.Format(_T("%s"), jsonOperation.GetLastErrorDesc());
                throw - 8;
            }

            //重构XML数据
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
                //获取金额、车型车种、重量、限重
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
                    case 1://车型
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
                    case 2://金额
                    {
                        iTemp = _ttoi(pJsonMMI->Text.GetAt(i).Context);

                        iMoney = iTemp;
                    }
                    break;
                    case 3://载重
                    {
                        iTemp = _ttoi(pJsonMMI->Text.GetAt(i).Context);

                        iWeight = iTemp;
                    }
                    break;
                    case 4://轴数
                    {
                        iTemp = _ttoi(pJsonMMI->Text.GetAt(i).Context);

                        iAxis = iTemp;
                    }
                    break;
                    case 5://限重
                    {
                        iTemp = _ttoi(pJsonMMI->Text.GetAt(i).Context);

                        iLimit = iTemp;
                    }
                    break;
                    case 6://电子支付卡余额
                    {
                        iTemp = _ttoi(pJsonMMI->Text.GetAt(i).Context);

                        iBalance = iTemp;
                    }
                    break;
                    case 7://入口收费站点名称
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

        //获取数据
        CString* pstrString = new CString();
        if (NULL == pstrString)
            throw - 10;
        pstrString->Format(_T("%s"), strDataString);

        m_pThreadManage->m_pThreadDevice->PostThreadMessage(WM_THREAD_DEVICE_SHOW_DEAL_INFO, (WPARAM)pstrString, (LPARAM)iMMIType);

        //标记成功
        bReturn = true;
    }
    catch (int& iErrorCode)
    {
        switch (iErrorCode)
        {
        case -1:
            strLog.Format(_T("[主进程响应],设置交互界面操作(IF_SetMMI)出错,组件未成功初始化"));
            break;
        case -2:
            strLog.Format(_T("[主进程响应],设置交互界面操作(IF_SetMMI)出错,组件未被授权"));
            break;
        case -3:
        case -4:
        case -5:
        case -6:
        case -10:
            strLog.Format(_T("[主进程响应],设置交互界面操作(IF_SetMMI)出错,组件内部资源异常"));
            break;
        case -7:
            strLog.Format(_T("[主进程响应],设置交互界面操作(IF_SetMMI)出错,解析参数(XML格式)有误，详细描述:%s"), strSubError);
            break;
        case -8:
            strLog.Format(_T("[主进程响应],设置交互界面操作(IF_SetMMI)出错,解析参数(JSON格式)有误，详细描述:%s"), strSubError);
            break;
        }
        //保存错误描述
        if (strLog != m_strLastErrorDesc)
        {
            m_strLastErrorDesc = strLog;
            RecordLog(m_strLastErrorDesc, LOG_LEVEL_ERROR);
        }
    }
    RecordLog(FmtStr(_T("[主进程响应],设置交互界面接口(IF_SetMMI)调用结束")));
    return bReturn;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// 函数名称：WINAPI    CTWSDNetPay_DllApp::IF_GetParam
//
// 功能描述：获取硬件参数
//
// 输入参数：IN const char* szParamContext
//         ：IN const int& iParamSize
//         ：IN const int& iFormatType
//         ：IN OUT char* szResultContext
//         ：IN OUT int& iResultSize
// 输出参数：bool
// 编写人员：ROCY
// 编写时间：2016年8月4日
// 修改人员：
// 修改时间：
// 函数版本：1.0.0.1
// 备注说明：接口11
//
////////////////////////////////////////////////////////////////////////////////////////////////
bool WINAPI CTWSDNetPay_DllApp::IF_GetParam(IN const char* szParamContext, IN const int& iParamSize, IN const int& iFormatType, IN OUT char* szResultContext, IN OUT int& iResultSize)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    bool bReturn = false;
    CString strLog = FmtStr(_T("[主进程响应],获取硬件参数接口(IF_GetParam)暂不提供此项功能"));

    if (strLog != m_strLastErrorDesc)
    {
        m_strLastErrorDesc = strLog;
        RecordLog(m_strLastErrorDesc, LOG_LEVEL_ERROR);
    }
    return bReturn;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// 函数名称：IF_GetComponentStatus
//
// 功能描述：检测组件状态
//
// 输入参数：IN OUT unsigned int& uStatus
// 输出参数：bool
// 编写人员：ROCY
// 编写时间：2016年7月23日
// 修改人员：
// 修改时间：
// 函数版本：1.0.0.1
// 备注说明：接口11
//
////////////////////////////////////////////////////////////////////////////////////////////////
bool WINAPI CTWSDNetPay_DllApp::IF_GetComponentStatus(IN OUT unsigned int& uStatus)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

	uStatus = 0;
    bool bReturn = false;
    DWORD dwState = 0x00;
    CString strLog;
	RecordLog(FmtStr(_T("[主进程响应],检测组件状态接口(IF_GetComponentStatus)调用开始")));

    try
    {
        //判断组件是否已经被初始化
        if (!m_bIsInitEnvironment)
            throw - 1;
        //判断组件是否成功授权
        if (!m_bAuthorizedLegal)
            throw - 2;
        //判断业务处理线程是否异常
        if (NULL == m_pThreadManage)
            throw - 3;
        //判断硬件控制线程是否正常
        if (NULL == m_pThreadManage->m_pThreadDevice)
            throw - 4;

        //获取硬件状态
		bReturn = m_pThreadManage->m_pThreadDevice->EtcEventGetDevStatus(uStatus);
		if(bReturn == false)
			m_strEtcEventLastErrorDesc.Format(_T("发送串口数据(IF_GetComponentStatus)失败！"));
    }
    catch (int& iErrorCode)
    {
        switch (iErrorCode)
        {
        case -1:
            strLog.Format(_T("[主进程响应],检测组件状态(IF_GetComponentStatus)出错,组件未成功初始化"));
            break;
        case -2:
            strLog.Format(_T("[主进程响应],检测组件状态(IF_GetComponentStatus)出错,组件未被授权"));
            break;
        case -3:
        case -4:
            strLog.Format(_T("[主进程响应],检测组件状态(IF_GetComponentStatus)出错,组件内部资源异常"));
            break;
        }
        //保存错误描述
        if (strLog != m_strLastErrorDesc)
        {
            m_strLastErrorDesc = strLog;
        }
    }
  RecordLog(FmtStr(_T("[主进程响应],检测组件状态接口(IF_GetComponentStatus)调用结束,返回状态：%ld"),uStatus));

    return bReturn;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// 函数名称：IF_TranslateData
//
// 功能描述：执行传输关键信息数据操作
//
// 输入参数：IN const char* szParamContext
//         ：IN const int& iParamSize
//         ：IN const int& iFormatType
// 输出参数：bool
// 编写人员：ROCY
// 编写时间：2016年7月23日
// 修改人员：
// 修改时间：
// 函数版本：1.0.0.1
// 备注说明：接口12
//
////////////////////////////////////////////////////////////////////////////////////////////////
bool WINAPI CTWSDNetPay_DllApp::IF_TranslateData(IN const char* szParamContext, IN const int& iParamSize, IN const int& iFormatType)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    bool bReturn = false;
    CString strLog = FmtStr(_T("[主进程响应],执行传输关键信息数据接口(IF_TranslateData)暂不提供此项功能"));
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
    //判断是否启用代扣动态库
    if (!m_bUseAgencyDll)
    {
        return false;
    }
    RecordLog(FmtStr(_T("[主进程响应],虚拟卡接口查询(IF_QueryVirtualCardDebit)调用开始")));
    //执行代扣查询
    if (m_pThreadManage != 0
            && m_pThreadManage->IsFuturePay())
    {   //add 2019-01 使用新支付逻辑
        return m_pThreadManage->QueryVirtualCardDebit(szParamContext, iParamSize, iFormatType, iSupportAgent, szResult);
    }
    iSupportAgent = m_AgencyPay.AgencyQueryVirtualCardDebit(szParamContext, iParamSize, iFormatType, szResult);
    RecordLog(FmtStr(_T("[主进程响应],虚拟卡接口查询(IF_QueryVirtualCardDebit)调用结束")));
    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// 函数名称：IF_Authority
//
// 功能描述：权限验证
//
// 输入参数：IN const char* szAuthKey
//         ：IN const int& iAuthKeySize
// 输出参数：bool
// 编写人员：ROCY
// 编写时间：2016年7月23日
// 修改人员：
// 修改时间：
// 函数版本：1.0.0.1
// 备注说明：内部接口
//
////////////////////////////////////////////////////////////////////////////////////////////////
bool WINAPI CTWSDNetPay_DllApp::IF_Authority(IN const char* szAuthKey, IN const int& iAuthKeySize)
{
    bool bReturn = false;

    return bReturn;
}


////////////////////////////////////////////////////////////////////////////////////////////////
// 函数名称：IF_TransferImage
//
// 功能描述：传输图像
//
// 输入参数：  IN const char* szImageInfo  图像内容串（XML，JSON）
//              IN const int& iSize         图像内容串大小
//              IN const int& iFormatType   图像内容串格式（XML，JSON）
// 输出参数：bool
// 编写人员：WingF
// 编写时间：2017年8月13日
// 修改人员：
// 修改时间：
// 函数版本：1.0.0.1
// 备注说明：内部接口
//
////////////////////////////////////////////////////////////////////////////////////////////////
bool WINAPI CTWSDNetPay_DllApp::IF_TransferImage(IN const char* szImageInfo, IN const int& iSize, IN const int& iFormatType)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strLog = FmtStr(_T("[主进程响应],图像传输操作接口(IF_TransferImage)调用开始(返回TRUE)"));
    RecordLog(strLog);

	return true;


    if (m_pThreadManage != 0
            && m_pThreadManage->IsFuturePay())
    {   //add 2019-01 使用新支付
        int execute_ret = m_pThreadManage->TransferImage(szImageInfo, iSize, iFormatType);
        return (execute_ret == 0);
    }

    //GetGlobalApp()->RecordLog(szImageInfo);
    bool bReturn = false;
    strLog = _T("");
    try
    {
        //判断组件是否已经被初始化
        if (!m_bIsInitEnvironment)
            throw - 1;
        //判断业务处理线程是否异常
        if (NULL == m_pThreadManage)
            throw - 2;

        int nImageType = 0;
        int ImageFormat;
        CString csPayID = _T("");
        CString csImageData = _T("");

        CString strImage = _T("");
        strImage.Format( _T("%s"), szImageInfo );

        m_strLastErrorDesc.Format(_T("[主进程响应],执行图像传输操作(IF_TransferImage), 正在解析图像传入参数"));
        //解析并获取关键数据
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

        //构造传入XML参数
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
        m_strLastErrorDesc.Format(_T("[主进程响应],执行图像传输操作(IF_TransferImage), 正在进行图像上传处理"));
        if ( 0 == (iResultCode = m_AgencyPay.AgencyDebitImage( strImageParam.GetBuffer(0), nImageLength, 1)) )
        {
            RecordLog(FmtStr(_T("[主进程响应],执行图像传输操作(IF_TransferImage)，调用成功")));
            bReturn = true;
        }
        else
        {
            RecordLog(FmtStr(_T("[主进程响应],执行图像传输操作(IF_TransferImage)异常, 函数返回失败(%d)"), iResultCode) );
        }
    }
    catch (int& iErrorCode)
    {
        switch (iErrorCode)
        {
        case -1:
            strLog.Format(_T("[主进程响应],图像传输(IF_TransferImage)出错,组件未成功初始化"));
            break;
        case -2:
            strLog.Format(_T("[主进程响应],图像传输(IF_TransferImage)出错,业务线程异常"));
            break;
        case -11:
            strLog.Format(_T("[主进程响应],图像传输(IF_TransferImage)出错,传入XML参数加载异常"));
            break;
        case -12:
            strLog.Format(_T("[主进程响应],图像传输(IF_TransferImage)出错,传入XML参数根节点异常"));
            break;
        case -13:
            strLog.Format(_T("[主进程响应],图像传输(IF_TransferImage)出错,传入XML参数 数据区 Data 异常"));
            break;
        case -14:
            strLog.Format(_T("[主进程响应],图像传输(IF_TransferImage)出错,传入XML参数 数据 Type 异常"));
            break;
        case -15:
            strLog.Format(_T("[主进程响应],图像传输(IF_TransferImage)出错,传入XML参数 数据 PayIdentifier 异常"));
            break;
        case -16:
            strLog.Format(_T("[主进程响应],图像传输(IF_TransferImage)出错,传入XML参数 数据 Image 异常"));
            break;
        case -21:
            strLog.Format(_T("[主进程响应],图像传输(IF_TransferImage)出错,传入JSON参数加载异常"));
            break;
        case -22:
            strLog.Format(_T("[主进程响应],图像传输(IF_TransferImage)出错,传入JSON参数 数据 Type 异常"));
            break;
        case -23:
            strLog.Format(_T("[主进程响应],图像传输(IF_TransferImage)出错,传入JSON参数 数据 PayIdentifier 异常"));
            break;
        case -24:
            strLog.Format(_T("[主进程响应],图像传输(IF_TransferImage)出错,传入JSON参数 数据 Image 异常"));
            break;
        }
        //保存错误描述
        if (strLog != m_strLastErrorDesc)
        {
            m_strLastErrorDesc = strLog;
            RecordLog(m_strLastErrorDesc);
        }
    }
    return bReturn;
}


//传输现金发票数据
bool WINAPI CTWSDNetPay_DllApp::IF_TransferCashInvoiceData(IN const char* szInvoiceData, IN const int& iSize, IN const int& iFormatType)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

    CString strLog = FmtStr(_T("[主进程响应],传输现金发票数据接口(IF_TransferCashInvoiceData)调用开始,参数1:%s,参数2:%d,参数3:%d"), szInvoiceData, iSize, iFormatType);
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
			strLog.Format(_T("IF_TransferCashInvoiceData接口参数类型错误！"));
			m_strLastErrorDesc = strLog;
		}
    }

	return bRet;
}

//撤销现金发票数据
bool WINAPI CTWSDNetPay_DllApp::IF_CancelCashInvoiceData(IN const char* szInvoiceData, IN const int& iSize, IN const int& iFormatType)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

    CString strLog = FmtStr(_T("[主进程响应],撤销现金发票数据接口(IF_CancelCashInvoiceData)调用开始,参数1:%s,参数2:%d,参数3:%d"), szInvoiceData, iSize, iFormatType);
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
			strLog.Format(_T("IF_CancelCashInvoiceData接口参数类型错误！"));
			m_strLastErrorDesc = strLog;
		}
    }

	return bRet;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// 函数名称：WINAPI    CTWSDNetPay_DllApp::IF_AgencyDebitQuery
//
// 功能描述：代扣查询（查询车辆是否支持代扣）
//
// 输入参数：IN const char* szParamContext
//         ：IN const int& iSize
//         ：IN const int& iFormatType
// 输出参数：bool
// 编写人员：ROCY
// 编写时间：2017年10月16日
// 修改人员：
// 修改时间：
// 函数版本：1.0.0.1
// 备注说明：
//
////////////////////////////////////////////////////////////////////////////////////////////////
bool CTWSDNetPay_DllApp::IF_AgencyDebitQuery(IN const char* szParamContext, IN const int& iParamSize, IN const int& iFormatType, IN OUT int& iResult)
{
    //判断是否启用代扣动态库
    if (!m_bUseAgencyDll)
    {
        return false;
    }
    if (m_pThreadManage != 0
            && m_pThreadManage->IsFuturePay())
    {   //add 2019-01 使用新支付逻辑
        return m_pThreadManage->AgencyDebitQuery(szParamContext, iParamSize, iFormatType, iResult);
    }
    //执行代扣查询
    iResult = m_AgencyPay.AgencyDebitQuery(szParamContext, iParamSize, iFormatType);

    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// 函数名称：WINAPI    CTWSDNetPay_DllApp::IF_InnerGetDeviceNo
//
// 功能描述：获取硬件的终端序号
//
// 输入参数：IN char* szParamContext
//         ：IN int& iSize
// 输出参数：bool
// 编写人员：ROCY
// 编写时间：2017年10月16日
// 修改人员：
// 修改时间：
// 函数版本：1.0.0.1
// 备注说明：内部接口
//
////////////////////////////////////////////////////////////////////////////////////////////////
bool CTWSDNetPay_DllApp::IF_InnerGetDeviceNo(IN char* szParamContext, IN int& iSize)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    bool bReturn = false;
    DWORD dwState = 0x00;
    CString strLog;
//  RecordLog(FmtStr(_T("[主进程响应],内部接口(IF_InnerGetDeviceNo)调用开始,参数1:%d"),szParamContext,iSize));
    try
    {
        //判断组件是否已经被初始化
        if (!m_bIsInitEnvironment)
            throw - 1;
        //判断组件是否成功授权
        if (!m_bAuthorizedLegal)
            throw - 2;
        //判断参数输入是否有效
        if ((NULL == szParamContext) || (iSize < 24))
            throw - 3;

        if (!m_strTWProductSerialNo.IsEmpty())
        {
            memset(szParamContext, 0, iSize);
            strcpy_s(szParamContext, iSize, m_strTWProductSerialNo.GetBuffer(0));
            m_strTWProductSerialNo.ReleaseBuffer();
        }

        //标记成功
        bReturn = true;
    }
    catch (int& iErrorCode)
    {
        switch (iErrorCode)
        {
        case -1:
            strLog.Format(_T("[主进程响应],检测组件状态(IF_GetComponentStatus)出错,组件未成功初始化"));
            break;
        case -2:
            strLog.Format(_T("[主进程响应],检测组件状态(IF_GetComponentStatus)出错,组件未被授权"));
            break;
        case -3:
        case -4:
            strLog.Format(_T("[主进程响应],检测组件状态(IF_GetComponentStatus)出错,组件内部资源异常"));
            break;
        }
        //保存错误描述
        if (strLog != m_strLastErrorDesc)
        {
            m_strLastErrorDesc = strLog;
        }
    }
//  RecordLog(FmtStr(_T("[主进程响应],内部接口(IF_InnerGetDeviceNo)调用结束")));

    return bReturn;
}


////////////////////////////////////////////////////////////////////////////////////////////////
// 函数名称：CTWSDNetPay_DllApp::IF_InnerGetSpecialData
//
// 功能描述：获取指定数据
//
// 输入参数：IN char* szParamContext
//         ：IN int& iParamSize
// 输出参数：bool
// 编写人员：ROCY
// 编写时间：2017年10月17日
// 修改人员：
// 修改时间：
// 函数版本：1.0.0.1
// 备注说明：内部接口
//
////////////////////////////////////////////////////////////////////////////////////////////////
bool CTWSDNetPay_DllApp::IF_InnerGetSpecialData(IN char* szParamContext, IN int& iParamSize)
{
    return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// 函数名称：CTWSDNetPay_DllApp::IF_InnerSetSpecialData
//
// 功能描述：传输指定参数
//
// 输入参数：IN const char* szParamContext
//         ：IN const int& iParamSize
// 输出参数：bool
// 编写人员：ROCY
// 编写时间：2017年10月17日
// 修改人员：
// 修改时间：
// 函数版本：1.0.0.1
// 备注说明：内部接口
//
////////////////////////////////////////////////////////////////////////////////////////////////
bool CTWSDNetPay_DllApp::IF_InnerSetSpecialData(IN const char* szParamContext, IN const int& iParamSize)
{
    return false;
}



//初始化组件环境
bool WINAPI CTWSDNetPay_DllApp::IF_EventInitEnvironment3 (IN const UINT& nThreadID, IN const HWND& hWnd, IN const unsigned int& nNotifyMsgID, IN const char* szAreaInfo, IN const char* szLocalStation, IN const char* szLocalLaneID, IN const char* szServerInfo, IN const int& iProvinceID, IN OUT void* pIndPtr, void (*fun)(int option, int result, void* pIndPtr))
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	bool bRet = false;

	RecordLog(FmtStr(_T("[ETC特情业务],ETC特情初始化接口(IF_EventInitEnvironment3)调用开始")));
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
		
		EtcEventRespMessgeID   = nNotifyMsgID;		//EtcEvent应答通知消息ID（河南要求通过消息通知的方式进行EtcEvent应答）
		EtcEventCallerWnd	   = hWnd;				//EtcEvent调用者窗体句柄
		EtcEventCallerThreadID = nThreadID;			//EtcEvent调用者线程ID

        if (NULL == pIndPtr && m_iCurrentWorkMode == 0)
            throw - 2;

        //回调函数指针赋值
		if(fun != NULL)
			m_CallbackNotifyEtcEvent = fun;
		//ETC特情应答指针，在回掉通知的时候将应答内容传递出去
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
        //填充配置信息
        TCHAR   szTemp[256] = {0};
        CString strTemp;
        CString strSTID;
		CString csErrorMsg;

        //保存车道软件的初始化参数
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
		
		//启动串口
		if (m_bIsEtcEventEnvironmeInited)
		{
			throw -9;
		}

		//设备管理线程由移动支付接口来创建并启动
	    //if(theApp.m_pThreadSMManager==NULL)
		//	theApp.m_pThreadSMManager = (CThreadSMManager*)::AfxBeginThread(RUNTIME_CLASS(CThreadSMManager), THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
		//避免外部连续调用移动支付初始化和该接口，而设备线程还没有创建，在这里等待200毫秒，等创建完成------2021-12-14
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
            strLog.Format(_T("EtcEvent环境初始化接口(IF_EventInitEnvironment3),传入参数检测有误，传入线程ID与窗体句柄均为0"));
            break;
        case -2:
            strLog.Format(_T("EtcEvent环境初始化接口(IF_EventInitEnvironment3),传入参数检测有误，传入应答指针错误"));
            break;
        case -3:
            strLog.Format(_T("EtcEvent环境初始化接口(IF_EventInitEnvironment3),传入参数检测有误，传入区域信息非法"));
            break;
        case -4:
            strLog.Format(_T("EtcEvent环境初始化接口(IF_EventInitEnvironment3),传入参数检测有误，传入站点编码信息非法"));
            break;
        case -5:
            strLog.Format(_T("EtcEvent环境初始化接口(IF_EventInitEnvironment3),传入参数检测有误，传入车道编码信息非法"));
            break;
        case -6:
            strLog.Format(_T("EtcEvent环境初始化接口(IF_EventInitEnvironment3),传入参数检测有误，传入服务器信息非法"));
            break;
        case -7:
            strLog.Format(_T("EtcEvent环境初始化接口(IF_EventInitEnvironment3),传入参数检测有误，传入省份编码为0"));
            break;
        case -8:
            strLog.Format(_T("EtcEvent环境初始化接口(IF_EventInitEnvironment3),传入参数检测有误,传入调用者线程ID和窗体句柄不能同时为空"));
            break;
        case -9:
            strLog.Format(_T("EtcEvent环境初始化接口(IF_EventInitEnvironment3),初始化失败(重复初始化EtcEvent环境)"));
            break;
        case -10:
            strLog.Format(_T("EtcEvent环境初始化接口(IF_EventInitEnvironment3),业务管理线程资源为空！"));
            break;
        case -11:
            strLog.Format(_T("EtcEvent环境初始化接口(IF_EventInitEnvironment3),设备控制线程资源为空！"));
            break;
		case -12:
            strLog.Format(_T("EtcEvent环境初始化接口(IF_EventInitEnvironment3),创建组件回调通知线程资源失败"));
            break;
		case -13:
            strLog.Format(_T("EtcEvent环境初始化接口(IF_EventInitEnvironment3),请先初始化移动支付环境！"));
            break;
		case -14:
            strLog.Format(_T("EtcEvent环境初始化接口(IF_EventInitEnvironment3),设置设备初始化数据失败！"));
            break;
        }
        //记录日志
        if (theApp.m_strEtcEventLastErrorDesc != strLog)
        {
            theApp.m_strEtcEventLastErrorDesc = strLog;
        }
		RecordLog(strLog);
    }

	return bRet;
}


//清理资源
bool WINAPI CTWSDNetPay_DllApp::IF_EventDestroy(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	RecordLog(FmtStr(_T("[ETC特情业务],特情组件卸载接口(IF_EventDestroy)调用开始")));
	CString strLog;

	if(m_pThreadManage != NULL)
	{
		if(m_pThreadManage->m_pThreadDevice != NULL)
		{
			m_pThreadManage->m_pThreadDevice->EtcEventDeInitDev();
		}
	}

	RecordLog(_T("NotifyCaller线程退出..."));
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


//获取上一次错误信息
const char* WINAPI CTWSDNetPay_DllApp::IF_EventGetLastErrorDesc(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	RecordLog(FmtStr(_T("[ETC特情业务],获取上次错误描述接口(IF_EventGetLastErrorDesc)调用开始")));

	return m_strLastErrorDesc.GetBuffer(0);
}

//获取组件状态
bool WINAPI CTWSDNetPay_DllApp::IF_EventGetComponentStatus(IN OUT unsigned int& uStatus)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	bool bRet = false;
	uStatus = 0;
	RecordLog(FmtStr(_T("[ETC特情业务],获取设备状态接口(IF_EventGetComponentStatus)调用开始")));

	CString strLog;
	try
    {
        //判断组件是否已经被初始化
        if (!m_bIsInitEnvironment)
            throw - 1;
        //判断组件是否成功授权
        if (!m_bAuthorizedLegal)
            throw - 2;
        //判断业务处理线程是否异常
        if (NULL == m_pThreadManage)
            throw - 3;
        //判断硬件控制线程是否正常
        if (NULL == m_pThreadManage->m_pThreadDevice)
            throw - 4;

        bRet = m_pThreadManage->m_pThreadDevice->EtcEventGetDevStatus(uStatus);
		if(bRet == false)
			m_strEtcEventLastErrorDesc.Format(_T("发送串口数据(IF_EventGetComponentStatus)失败！"));
    }
    catch (int& iErrorCode)
    {
        switch (iErrorCode)
        {
        case -1:
            strLog.Format(_T("[ETC特情业务],获取组件状态接口(IF_EventGetComponentStatus)出错,组件未成功初始化"));
            break;
        case -2:
            strLog.Format(_T("[ETC特情业务],获取组件状态接口(IF_EventGetComponentStatus)出错,组件未被授权"));
            break;
        case -3:
        case -4:
            strLog.Format(_T("[ETC特情业务],获取组件状态接口(IF_EventGetComponentStatus)出错,组件内部资源异常"));
            break;
        }
        //保存错误描述
        if (strLog != m_strEtcEventLastErrorDesc)
        {
            m_strEtcEventLastErrorDesc = strLog;
        }
    }

	RecordLog(FmtStr(_T("[ETC特情业务],获取设备状态接口(IF_EventGetComponentStatus)调用结束，返回状态值：%ld"),uStatus));


	return bRet;
}

//特情处理开始
bool WINAPI CTWSDNetPay_DllApp::IF_EventDealStart(IN const char* szParamContext, IN const int& iParamSize, IN const int& iFormatType)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	bool bRet = false;

    CString strLog;
    RecordLog(FmtStr(_T("[ETC特情业务],特情处理开始接口(IF_EventDealStart)调用开始,参数:%s"),szParamContext));
	CString ErrorDescribe;
	if(!CheckEventDealStartJsonValidity(szParamContext, ErrorDescribe))
	{
		m_strEtcEventLastErrorDesc = ErrorDescribe;
		return bRet;
	}

    try
    {
        //判断组件是否已经被初始化
        if (!m_bIsInitEnvironment)
            throw - 1;
        //判断组件是否成功授权
        if (!m_bAuthorizedLegal)
            throw - 2;
        //判断业务处理线程是否异常
        if (NULL == m_pThreadManage)
            throw - 3;
        //判断硬件控制线程是否正常
        if (NULL == m_pThreadManage->m_pThreadDevice)
            throw - 4;

        bRet = m_pThreadManage->m_pThreadDevice->EtcEventDealStart(szParamContext, iParamSize);
		if(bRet == false)
			m_strEtcEventLastErrorDesc.Format(_T("发送串口数据失败！"));

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
            strLog.Format(_T("[ETC特情业务],特情处理开始接口(IF_EventDealStart)出错,组件未成功初始化"));
            break;
        case -2:
            strLog.Format(_T("[ETC特情业务],特情处理开始接口(IF_EventDealStart)出错,组件未被授权"));
            break;
        case -3:
        case -4:
            strLog.Format(_T("[ETC特情业务],特情处理开始接口(IF_EventDealStart)出错,组件内部资源异常"));
            break;
        }
        //保存错误描述
        if (strLog != m_strEtcEventLastErrorDesc)
        {
            m_strEtcEventLastErrorDesc = strLog;
        }
    }

	return bRet;
}

//车辆队列确认
bool WINAPI CTWSDNetPay_DllApp::IF_EventCheckVehQueue(IN const char* szParamContext, IN const int& iParamSize, IN const int& iFormatType)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	bool bRet = false;

	CString strLog;
    RecordLog(FmtStr(_T("[ETC特情业务],车辆队列确认接口(IF_EventCheckVehQueue)调用开始,参数:%s"),szParamContext));
	CString ErrorDescribe;
	if(!CheckEventCheckVehQueueJsonValidity(szParamContext, ErrorDescribe))
	{
		m_strEtcEventLastErrorDesc = ErrorDescribe;
		return bRet;
	}

    try
    {
        //判断组件是否已经被初始化
        if (!m_bIsInitEnvironment)
            throw - 1;
        //判断组件是否成功授权
        if (!m_bAuthorizedLegal)
            throw - 2;
        //判断业务处理线程是否异常
        if (NULL == m_pThreadManage)
            throw - 3;
        //判断硬件控制线程是否正常
        if (NULL == m_pThreadManage->m_pThreadDevice)
            throw - 4;

        bRet = m_pThreadManage->m_pThreadDevice->EtcEventCheckVehQueue(szParamContext, iParamSize);
		if(bRet == false)
			m_strEtcEventLastErrorDesc.Format(_T("发送串口数据失败！"));
    }
    catch (int& iErrorCode)
    {
        switch (iErrorCode)
        {
        case -1:
            strLog.Format(_T("[ETC特情业务],车辆队列确认接口(IF_EventCheckVehQueue)出错,组件未成功初始化"));
            break;
        case -2:
            strLog.Format(_T("[ETC特情业务],车辆队列确认接口(IF_EventCheckVehQueue)出错,组件未被授权"));
            break;
        case -3:
        case -4:
            strLog.Format(_T("[ETC特情业务],车辆队列确认接口(IF_EventCheckVehQueue)出错,组件内部资源异常"));
            break;
        }
        //保存错误描述
        if (strLog != m_strEtcEventLastErrorDesc)
        {
            m_strEtcEventLastErrorDesc = strLog;
        }
    }

	return bRet;
}

//车辆信息确认
bool WINAPI CTWSDNetPay_DllApp::IF_EventCheckVehInfo(IN const char* szParamContext,IN const int& iParamSize,IN const int& iFormatType)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	bool bRet = false;

	CString strLog;
    RecordLog(FmtStr(_T("[ETC特情业务],车辆信息确认接口(IF_EventCheckVehInfo)调用开始,参数:%s"),szParamContext));
	CString ErrorDescribe;
	if(!CheckEventCheckVehInfoJsonValidity(szParamContext, ErrorDescribe))
	{
		m_strEtcEventLastErrorDesc = ErrorDescribe;
		return bRet;
	}

    try
    {
        //判断组件是否已经被初始化
        if (!m_bIsInitEnvironment)
            throw - 1;
        //判断组件是否成功授权
        if (!m_bAuthorizedLegal)
            throw - 2;
        //判断业务处理线程是否异常
        if (NULL == m_pThreadManage)
            throw - 3;
        //判断硬件控制线程是否正常
        if (NULL == m_pThreadManage->m_pThreadDevice)
            throw - 4;

        bRet = m_pThreadManage->m_pThreadDevice->EtcEventCheckVehInfo(szParamContext, iParamSize);
		if(bRet == false)
			m_strEtcEventLastErrorDesc.Format(_T("发送串口数据失败！"));
    }
    catch (int& iErrorCode)
    {
        switch (iErrorCode)
        {
        case -1:
            strLog.Format(_T("[ETC特情业务],车辆信息确认接口(IF_EventCheckVehInfo)出错,组件未成功初始化"));
            break;
        case -2:
            strLog.Format(_T("[ETC特情业务],车辆信息确认接口(IF_EventCheckVehInfo)出错,组件未被授权"));
            break;
        case -3:
        case -4:
            strLog.Format(_T("[ETC特情业务],车辆信息确认接口(IF_EventCheckVehInfo)出错,组件内部资源异常"));
            break;
        }
        //保存错误描述
        if (strLog != m_strEtcEventLastErrorDesc)
        {
            m_strEtcEventLastErrorDesc = strLog;
        }
    }

	return bRet;
}

//入口信息确认
bool WINAPI CTWSDNetPay_DllApp::IF_EventCheckEntryInfo(IN const char* szParamContext,IN const int& iParamSize,IN const int& iFormatType)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	bool bRet = false;

	CString strLog;
    RecordLog(FmtStr(_T("[ETC特情业务],入口信息确认接口(IF_EventCheckEntryInfo)调用开始,参数:%s"),szParamContext));
	CString ErrorDescribe;
	if(!CheckEventCheckEntryInfoJsonValidity(szParamContext, ErrorDescribe))
	{
		m_strEtcEventLastErrorDesc = ErrorDescribe;
		return bRet;
	}

    try
    {
        //判断组件是否已经被初始化
        if (!m_bIsInitEnvironment)
            throw - 1;
        //判断组件是否成功授权
        if (!m_bAuthorizedLegal)
            throw - 2;
        //判断业务处理线程是否异常
        if (NULL == m_pThreadManage)
            throw - 3;
        //判断硬件控制线程是否正常
        if (NULL == m_pThreadManage->m_pThreadDevice)
            throw - 4;

        bRet = m_pThreadManage->m_pThreadDevice->EtcEventCheckEntryInfo(szParamContext, iParamSize);
		if(bRet == false)
			m_strEtcEventLastErrorDesc.Format(_T("发送串口数据失败！"));
    }
    catch (int& iErrorCode)
    {
        switch (iErrorCode)
        {
        case -1:
            strLog.Format(_T("[ETC特情业务],入口信息确认接口(IF_EventCheckEntryInfo)出错,组件未成功初始化"));
            break;
        case -2:
            strLog.Format(_T("[ETC特情业务],入口信息确认接口(IF_EventCheckEntryInfo)出错,组件未被授权"));
            break;
        case -3:
        case -4:
            strLog.Format(_T("[ETC特情业务],入口信息确认接口(IF_EventCheckEntryInfo)出错,组件内部资源异常"));
            break;
        }
        //保存错误描述
        if (strLog != m_strEtcEventLastErrorDesc)
        {
            m_strEtcEventLastErrorDesc = strLog;
        }
    }

	return bRet;
}

//费额显示
bool WINAPI CTWSDNetPay_DllApp::IF_EventShowFeeInfo(IN const char* szParamContext,IN const int& iParamSize,IN const int& iFormatType)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	bool bRet = false;

	CString strLog;
    RecordLog(FmtStr(_T("[ETC特情业务],费额显示接口(IF_EventShowFeeInfo)调用开始,参数:%s"),szParamContext));
	CString ErrorDescribe;
	if(!CheckEventShowFeeInfoJsonValidity(szParamContext, ErrorDescribe))
	{
		m_strEtcEventLastErrorDesc = ErrorDescribe;
		return bRet;
	}

    try
    {
        //判断组件是否已经被初始化
        if (!m_bIsInitEnvironment)
            throw - 1;
        //判断组件是否成功授权
        if (!m_bAuthorizedLegal)
            throw - 2;
        //判断业务处理线程是否异常
        if (NULL == m_pThreadManage)
            throw - 3;
        //判断硬件控制线程是否正常
        if (NULL == m_pThreadManage->m_pThreadDevice)
            throw - 4;

        bRet = m_pThreadManage->m_pThreadDevice->EtcEventShowFeeInfo(szParamContext, iParamSize);
		if(bRet == false)
			m_strEtcEventLastErrorDesc.Format(_T("发送串口数据失败！"));
    }
    catch (int& iErrorCode)
    {
        switch (iErrorCode)
        {
        case -1:
            strLog.Format(_T("[ETC特情业务],费额显示接口(IF_EventShowFeeInfo)出错,组件未成功初始化"));
            break;
        case -2:
            strLog.Format(_T("[ETC特情业务],费额显示接口(IF_EventShowFeeInfo)出错,组件未被授权"));
            break;
        case -3:
        case -4:
            strLog.Format(_T("[ETC特情业务],费额显示接口(IF_EventShowFeeInfo)出错,组件内部资源异常"));
            break;
        }
        //保存错误描述
        if (strLog != m_strEtcEventLastErrorDesc)
        {
            m_strEtcEventLastErrorDesc = strLog;
        }
    }

	return bRet;
}

//支付状态显示
bool WINAPI CTWSDNetPay_DllApp::IF_EventPayResultDisplay(IN const char* szParamContext,IN const int& iParamSize,IN const int& iFormatType)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	bool bRet = false;

	CString strLog;
    RecordLog(FmtStr(_T("[ETC特情业务],支付结果展示接口(IF_EventPayResultDisplay)调用开始,参数:%s"),szParamContext));
	CString ErrorDescribe;
	if(!CheckEventPayResultShowJsonValidity(szParamContext, ErrorDescribe))
	{
		m_strEtcEventLastErrorDesc = ErrorDescribe;
		return bRet;
	}

    try
    {
        //判断组件是否已经被初始化
        if (!m_bIsInitEnvironment)
            throw - 1;
        //判断组件是否成功授权
        if (!m_bAuthorizedLegal)
            throw - 2;
        //判断业务处理线程是否异常
        if (NULL == m_pThreadManage)
            throw - 3;
        //判断硬件控制线程是否正常
        if (NULL == m_pThreadManage->m_pThreadDevice)
            throw - 4;

        bRet = m_pThreadManage->m_pThreadDevice->EtcEventPayResultDisplay(szParamContext, iParamSize);
		if(bRet == false)
			m_strEtcEventLastErrorDesc.Format(_T("发送串口数据失败！"));
    }
    catch (int& iErrorCode)
    {
        switch (iErrorCode)
        {
        case -1:
            strLog.Format(_T("[ETC特情业务],支付结果展示接口(IF_EventPayResultDisplay)出错,组件未成功初始化"));
            break;
        case -2:
            strLog.Format(_T("[ETC特情业务],支付结果展示接口(IF_EventPayResultDisplay)出错,组件未被授权"));
            break;
        case -3:
        case -4:
            strLog.Format(_T("[ETC特情业务],支付结果展示接口(IF_EventPayResultDisplay)出错,组件内部资源异常"));
            break;
        }
        //保存错误描述
        if (strLog != m_strEtcEventLastErrorDesc)
        {
            m_strEtcEventLastErrorDesc = strLog;
        }
    }

	return bRet;
}

//特情处理结束
bool WINAPI CTWSDNetPay_DllApp::IF_EventDealStop(IN const char* szParamContext,IN const int& iParamSize,IN const int& iFormatType)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	bool bRet = false;

	CString strLog;
    RecordLog(FmtStr(_T("[ETC特情业务],特情处理结束接口(IF_EventDealStop)调用开始,参数:%s"),szParamContext));
	CString ErrorDescribe;
	if(!CheckEventDealStopJsonValidity(szParamContext, ErrorDescribe))
	{
		m_strEtcEventLastErrorDesc = ErrorDescribe;
		return bRet;
	}

    try
    {
        //判断组件是否已经被初始化
        if (!m_bIsInitEnvironment)
            throw - 1;
        //判断组件是否成功授权
        if (!m_bAuthorizedLegal)
            throw - 2;
        //判断业务处理线程是否异常
        if (NULL == m_pThreadManage)
            throw - 3;
        //判断硬件控制线程是否正常
        if (NULL == m_pThreadManage->m_pThreadDevice)
            throw - 4;

        bRet = m_pThreadManage->m_pThreadDevice->EtcEventDealStop(szParamContext, iParamSize);
		if(bRet == false)
			m_strEtcEventLastErrorDesc.Format(_T("发送串口数据失败！"));

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
            strLog.Format(_T("[ETC特情业务],特情处理结束接口(IF_EventDealStop)出错,组件未成功初始化"));
            break;
        case -2:
            strLog.Format(_T("[ETC特情业务],特情处理结束接口(IF_EventDealStop)出错,组件未被授权"));
            break;
        case -3:
        case -4:
            strLog.Format(_T("[ETC特情业务],特情处理结束接口(IF_EventDealStop)出错,组件内部资源异常"));
            break;
        }
        //保存错误描述
        if (strLog != m_strEtcEventLastErrorDesc)
        {
            m_strEtcEventLastErrorDesc = strLog;
        }
    }

	return bRet;
}

//启动扫码
bool WINAPI CTWSDNetPay_DllApp::IF_EventStartScan(IN const int& iFeeValue)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	bool bRet = false;

	CString strLog;
    RecordLog(FmtStr(_T("[ETC特情业务],启动扫码接口(IF_EventStartScan)调用开始,参数:%d"),iFeeValue));
    try
    {
        //判断组件是否已经被初始化
        if (!m_bIsInitEnvironment)
            throw - 1;
        //判断组件是否成功授权
        if (!m_bAuthorizedLegal)
            throw - 2;
        //判断业务处理线程是否异常
        if (NULL == m_pThreadManage)
            throw - 3;
        //判断硬件控制线程是否正常
        if (NULL == m_pThreadManage->m_pThreadDevice)
            throw - 4;

        bRet = m_pThreadManage->m_pThreadDevice->EtcEventStartScan();
		if(bRet == false)
			m_strEtcEventLastErrorDesc.Format(_T("发送串口数据失败！"));
    }
    catch (int& iErrorCode)
    {
        switch (iErrorCode)
        {
        case -1:
            strLog.Format(_T("[ETC特情业务],启动扫码接口(IF_EventStartScan)出错,组件未成功初始化"));
            break;
        case -2:
            strLog.Format(_T("[ETC特情业务],启动扫码接口(IF_EventStartScan)出错,组件未被授权"));
            break;
        case -3:
        case -4:
            strLog.Format(_T("[ETC特情业务],启动扫码接口(IF_EventStartScan)出错,组件内部资源异常"));
            break;
        }
        //保存错误描述
        if (strLog != m_strEtcEventLastErrorDesc)
        {
            m_strEtcEventLastErrorDesc = strLog;
        }
    }

	return bRet;
}

//停止扫码
bool WINAPI CTWSDNetPay_DllApp::IF_EventStopScan()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	bool bRet = false;

	CString strLog;
    RecordLog(FmtStr(_T("[ETC特情业务],停止扫码接口(IF_EventStopScan)调用开始")));
    try
    {
        //判断组件是否已经被初始化
        if (!m_bIsInitEnvironment)
            throw - 1;
        //判断组件是否成功授权
        if (!m_bAuthorizedLegal)
            throw - 2;
        //判断业务处理线程是否异常
        if (NULL == m_pThreadManage)
            throw - 3;
        //判断硬件控制线程是否正常
        if (NULL == m_pThreadManage->m_pThreadDevice)
            throw - 4;

        bRet = m_pThreadManage->m_pThreadDevice->EtcEventStopScan();
		if(bRet == false)
			m_strEtcEventLastErrorDesc.Format(_T("发送串口数据失败！"));
    }
    catch (int& iErrorCode)
    {
        switch (iErrorCode)
        {
        case -1:
            strLog.Format(_T("[ETC特情业务],停止扫码接口(IF_EventStopScan)出错,组件未成功初始化"));
            break;
        case -2:
            strLog.Format(_T("[ETC特情业务],停止扫码接口(IF_EventStopScan)出错,组件未被授权"));
            break;
        case -3:
        case -4:
            strLog.Format(_T("[ETC特情业务],停止扫码接口(IF_EventStopScan)出错,组件内部资源异常"));
            break;
        }
        //保存错误描述
        if (strLog != m_strEtcEventLastErrorDesc)
        {
            m_strEtcEventLastErrorDesc = strLog;
        }
    }

	return bRet;
}

//ETC卡操作提示
bool WINAPI CTWSDNetPay_DllApp::IF_EventCardOperationNotify(IN const char* szParamContext, IN const int& iParamSize, IN const int& iFormatType)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	bool bRet = false;
	
	CString strLog;
    RecordLog(FmtStr(_T("[ETC特情业务],卡操作提示接口(IF_EventCardOperationNotify)调用开始,参数:%s"),szParamContext));
	CString ErrorDescribe;
	int NotifyType;
	if(!CheckCardOperationNotifyJsonValidity(szParamContext, ErrorDescribe, NotifyType))
	{
		m_strEtcEventLastErrorDesc = ErrorDescribe;
		return bRet;
	}

    try
    {
        //判断组件是否已经被初始化
        if (!m_bIsInitEnvironment)
            throw - 1;
        //判断组件是否成功授权
        if (!m_bAuthorizedLegal)
            throw - 2;
        //判断业务处理线程是否异常
        if (NULL == m_pThreadManage)
            throw - 3;
        //判断硬件控制线程是否正常
        if (NULL == m_pThreadManage->m_pThreadDevice)
            throw - 4;

		if(NotifyType == 98)
		{
			RecordLog("[ETC特情业务],卡操作提示接口IF_EventCardOperationNotify, 工作模式切换到【MTC人工模式】！！！");
			m_iCurrentWorkMode = 1;
			return true;
		}
		if(NotifyType == 99)
		{
			RecordLog("[ETC特情业务],卡操作提示接口IF_EventCardOperationNotify, 工作模式切换到【ETC特情自助模式】！！！");
			m_iCurrentWorkMode = 0;
			return true;
		}		

        bRet = m_pThreadManage->m_pThreadDevice->EtcEventCardOperationNotify(szParamContext, iParamSize);
		if(bRet == false)
			m_strEtcEventLastErrorDesc.Format(_T("发送串口数据失败！"));
    }
    catch (int& iErrorCode)
    {
        switch (iErrorCode)
        {
        case -1:
            strLog.Format(_T("[ETC特情业务],卡操作提示接口(IF_EventCardOperationNotify)出错,组件未成功初始化"));
            break;
        case -2:
            strLog.Format(_T("[ETC特情业务],卡操作提示接口(IF_EventCardOperationNotify)出错,组件未被授权"));
            break;
        case -3:
        case -4:
            strLog.Format(_T("[ETC特情业务],卡操作提示接口(IF_EventCardOperationNotify)出错,组件内部资源异常"));
            break;
        }
        //保存错误描述
        if (strLog != m_strEtcEventLastErrorDesc)
        {
            m_strEtcEventLastErrorDesc = strLog;
        }
    }

	return bRet;
}

//车辆队列修改
bool WINAPI CTWSDNetPay_DllApp::IF_EventModifyVehQueue(IN const char* szParamContext, IN const int& iParamSize, IN const int& iFormatType)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	bool bRet = false;

	CString strLog;
    RecordLog(FmtStr(_T("[ETC特情业务],修正车辆队列接口(IF_EventModifyVehQueue)调用开始,参数:%s"),szParamContext));
	CString ErrorDescribe;
	if(!CheckEventModifyVehQueueJsonValidity(szParamContext, ErrorDescribe))
	{
		m_strEtcEventLastErrorDesc = ErrorDescribe;
		return bRet;
	}

    try
    {
        //判断组件是否已经被初始化
        if (!m_bIsInitEnvironment)
            throw - 1;
        //判断组件是否成功授权
        if (!m_bAuthorizedLegal)
            throw - 2;
        //判断业务处理线程是否异常
        if (NULL == m_pThreadManage)
            throw - 3;
        //判断硬件控制线程是否正常
        if (NULL == m_pThreadManage->m_pThreadDevice)
            throw - 4;

        bRet = m_pThreadManage->m_pThreadDevice->EtcEventModifyVehQueue(szParamContext, iParamSize);
		if(bRet == false)
			m_strEtcEventLastErrorDesc.Format(_T("发送串口数据失败！"));
    }
    catch (int& iErrorCode)
    {
        switch (iErrorCode)
        {
        case -1:
            strLog.Format(_T("[ETC特情业务],修正车辆队列接口(IF_EventModifyVehQueue)出错,组件未成功初始化"));
            break;
        case -2:
            strLog.Format(_T("[ETC特情业务],修正车辆队列接口(IF_EventModifyVehQueue)出错,组件未被授权"));
            break;
        case -3:
        case -4:
            strLog.Format(_T("[ETC特情业务],修正车辆队列接口(IF_EventModifyVehQueue)出错,组件内部资源异常"));
            break;
        }
        //保存错误描述
        if (strLog != m_strEtcEventLastErrorDesc)
        {
            m_strEtcEventLastErrorDesc = strLog;
        }
    }

	return bRet;
}

//最小费额授权
bool WINAPI CTWSDNetPay_DllApp::IF_EventFeeAuthorize(IN const char* szParamContext, IN const int& iParamSize, IN const int& iFormatType)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	bool bRet = false;
	
	CString strLog;
    RecordLog(FmtStr(_T("[ETC特情业务],最小费额授权接口(IF_EventFeeAuthorize)调用开始,参数:%s"),szParamContext));

    try
    {
        //判断组件是否已经被初始化
        if (!m_bIsInitEnvironment)
            throw - 1;
        //判断组件是否成功授权
        if (!m_bAuthorizedLegal)
            throw - 2;
        //判断业务处理线程是否异常
        if (NULL == m_pThreadManage)
            throw - 3;
        //判断硬件控制线程是否正常
        if (NULL == m_pThreadManage->m_pThreadDevice)
            throw - 4;

        bRet = m_pThreadManage->m_pThreadDevice->EtcEventFeeAuthorize(szParamContext, iParamSize);
		if(bRet == false)
			m_strEtcEventLastErrorDesc.Format(_T("发送串口数据失败！"));
    }
    catch (int& iErrorCode)
    {
        switch (iErrorCode)
        {
        case -1:
            strLog.Format(_T("[ETC特情业务],最小费额授权接口(IF_EventFeeAuthorize)出错,组件未成功初始化"));
            break;
        case -2:
            strLog.Format(_T("[ETC特情业务],最小费额授权接口(IF_EventFeeAuthorize)出错,组件未被授权"));
            break;
        case -3:
        case -4:
            strLog.Format(_T("[ETC特情业务],最小费额授权接口(IF_EventFeeAuthorize)出错,组件内部资源异常"));
            break;
        }
        //保存错误描述
        if (strLog != m_strEtcEventLastErrorDesc)
        {
            m_strEtcEventLastErrorDesc = strLog;
        }
    }

	return bRet;
}


//自由语音播报
bool WINAPI CTWSDNetPay_DllApp::IF_EventFreeVoicePlay(IN const char* szParamContext,IN const int& iParamSize,IN const int& iFormatType)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	bool bRet = false;

	CString strLog;
	RecordLog(FmtStr(_T("[ETC特情业务],自由语音播报接口(IF_EventFreeVoicePlay)调用开始,参数:%s"),szParamContext));

	try
	{
		//判断组件是否已经被初始化
		if (!m_bIsInitEnvironment)
			throw - 1;
		//判断组件是否成功授权
		if (!m_bAuthorizedLegal)
			throw - 2;
		//判断业务处理线程是否异常
		if (NULL == m_pThreadManage)
			throw - 3;
		//判断硬件控制线程是否正常
		if (NULL == m_pThreadManage->m_pThreadDevice)
			throw - 4;

		bRet = m_pThreadManage->m_pThreadDevice->EtcEventFreeVoicePlay(szParamContext, iParamSize);
		if(bRet == false)
			m_strEtcEventLastErrorDesc.Format(_T("发送串口数据失败！"));
	}
	catch (int& iErrorCode)
	{
		switch (iErrorCode)
		{
		case -1:
			strLog.Format(_T("[ETC特情业务],卡操作提示接口(IF_EventFreeVoicePlay)出错,组件未成功初始化"));
			break;
		case -2:
			strLog.Format(_T("[ETC特情业务],卡操作提示接口(IF_EventFreeVoicePlay)出错,组件未被授权"));
			break;
		case -3:
		case -4:
			strLog.Format(_T("[ETC特情业务],卡操作提示接口(IF_EventFreeVoicePlay)出错,组件内部资源异常"));
			break;
		}
		//保存错误描述
		if (strLog != m_strEtcEventLastErrorDesc)
		{
			m_strEtcEventLastErrorDesc = strLog;
		}
	}

	return bRet;
}


// 其他授权接口（最小费额除外）
bool WINAPI CTWSDNetPay_DllApp::IF_EventAuthorize(IN const char* szParamContext,
					   IN const int& iParamSize,IN const int& iFormatType)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	bool bRet = false;

	CString strLog;
	RecordLog(FmtStr(_T("[ETC特情业务], 其他授权接口(IF_EventAuthorize)调用开始,长度：%d,参数:%s"),iParamSize,szParamContext));

	CString ErrorDescribe;
	if(!CheckAuthorize(szParamContext, ErrorDescribe))
	{
		m_strEtcEventLastErrorDesc = ErrorDescribe;
		return bRet;
	}

	try
	{
		//判断组件是否已经被初始化
		if (!m_bIsInitEnvironment)
			throw - 1;
		//判断组件是否成功授权
		if (!m_bAuthorizedLegal)
			throw - 2;
		//判断业务处理线程是否异常
		if (NULL == m_pThreadManage)
			throw - 3;
		//判断硬件控制线程是否正常
		if (NULL == m_pThreadManage->m_pThreadDevice)
			throw - 4;

		bRet = m_pThreadManage->m_pThreadDevice->EtcEventAuthorize(szParamContext, iParamSize);
		if(bRet == false)
			m_strEtcEventLastErrorDesc.Format(_T("发送串口数据失败！"));
	}
	catch (int& iErrorCode)
	{
		switch (iErrorCode)
		{
		case -1:
			strLog.Format(_T("[ETC特情业务],其他授权接口(IF_EventAuthorize)出错,组件未成功初始化"));
			break;
		case -2:
			strLog.Format(_T("[ETC特情业务],其他授权接口(IF_EventAuthorize)出错,组件未被授权"));
			break;
		case -3:
		case -4:
			strLog.Format(_T("[ETC特情业务],其他授权接口(IF_EventAuthorize)出错,组件内部资源异常"));
			break;
		}
		//保存错误描述
		if (strLog != m_strEtcEventLastErrorDesc)
		{
			m_strEtcEventLastErrorDesc = strLog;
		}
	}

	return bRet;
}

//车辆队列删除结果
bool WINAPI CTWSDNetPay_DllApp::IF_EventDelVehQueueResult(IN const char* szParamContext, IN const int& iParamSize, IN const int& iFormatType)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	bool bRet = false;

	CString strLog;
	RecordLog(FmtStr(_T("[ETC特情业务],删除队列结果接口(IF_EventDelVehQueueResult)调用开始,长度：%d,参数:%s"), iParamSize, szParamContext));

	CString ErrorDescribe;
	if(!CheckEventDelVehQueueResult(szParamContext, ErrorDescribe))
	{
		m_strEtcEventLastErrorDesc = ErrorDescribe;
		return bRet;
	}

	try
	{
		//判断组件是否已经被初始化
		if (!m_bIsInitEnvironment)
			throw - 1;
		//判断组件是否成功授权
		if (!m_bAuthorizedLegal)
			throw - 2;
		//判断业务处理线程是否异常
		if (NULL == m_pThreadManage)
			throw - 3;
		//判断硬件控制线程是否正常
		if (NULL == m_pThreadManage->m_pThreadDevice)
			throw - 4;

		bRet = m_pThreadManage->m_pThreadDevice->EtcEventDelVehQueueResult(szParamContext, iParamSize);
		if(bRet == false)
			m_strEtcEventLastErrorDesc.Format(_T("发送串口数据失败！"));
	}
	catch (int& iErrorCode)
	{
		switch (iErrorCode)
		{
		case -1:
			strLog.Format(_T("[ETC特情业务],删除队列结果接口(IF_EventDelVehQueueResult)出错,组件未成功初始化"));
			break;
		case -2:
			strLog.Format(_T("[ETC特情业务],删除队列结果接口(IF_EventDelVehQueueResult)出错,组件未被授权"));
			break;
		case -3:
		case -4:
			strLog.Format(_T("[ETC特情业务],删除队列结果接口(IF_EventDelVehQueueResult)出错,组件内部资源异常"));
			break;
		}
		//保存错误描述
		if (strLog != m_strEtcEventLastErrorDesc)
		{
			m_strEtcEventLastErrorDesc = strLog;
		}
	}

	return bRet;
}



std::string GetProviceNameByID(unsigned short id)
{
	std::string NameStr = std::string("广东");
	switch(id)
	{
	case 11: NameStr = std::string("北京");break;	
	case 12: NameStr = std::string("天津");break;
	case 13: NameStr = std::string("河北");break;
	case 14: NameStr = std::string("山西");break;	
	case 15: NameStr = std::string("内蒙古");break;
	case 21: NameStr = std::string("辽宁");break;
	case 22: NameStr = std::string("吉林");break;	
	case 23: NameStr = std::string("黑龙江");break;
	case 31: NameStr = std::string("上海");break;
	case 32: NameStr = std::string("江苏");break;	
	case 33: NameStr = std::string("浙江");break;
	case 34: NameStr = std::string("安徽");break;
	case 35: NameStr = std::string("福建");break;	
	case 36: NameStr = std::string("江西");break;
	case 37: NameStr = std::string("山东");break;
	case 41: NameStr = std::string("河南");break;	
	case 42: NameStr = std::string("湖北");break;
	case 43: NameStr = std::string("湖南");break;
	case 44: NameStr = std::string("广东");break;	
	case 45: NameStr = std::string("广西");break;
	case 46: NameStr = std::string("海南");break;
	case 50: NameStr = std::string("重庆");break;
	case 51: NameStr = std::string("四川");break;
	case 52: NameStr = std::string("贵州");break;
	case 53: NameStr = std::string("云南");break;
	case 54: NameStr = std::string("西藏");break;
	case 61: NameStr = std::string("陕西");break;
	case 62: NameStr = std::string("甘肃");break;
	case 63: NameStr = std::string("青海");break;
	case 64: NameStr = std::string("宁夏");break;
	case 65: NameStr = std::string("新疆");break;
	case 71: NameStr = std::string("台湾");break;
	case 81: NameStr = std::string("香港");break;
	case 82: NameStr = std::string("澳门");break;
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
	//进行数据类型校验
	Json::Reader reader;
	Json::Value  root;
	Json::Value  EventDealStart;
	Json::Value  Data;	

	if(!reader.parse(szParamContext, root))
	{
		ErrorDescribe = "特情事件开始参数: Json格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(root["EventDealStart"].type() != Json::objectValue)
	{
		ErrorDescribe = "特情事件开始参数: EventDealStart字段格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}

	EventDealStart = root["EventDealStart"];	
	if(EventDealStart["SpecialEventNo"].type() == Json::nullValue || EventDealStart["SpecialEventNo"].type() != Json::stringValue)
	{
		ErrorDescribe = "特情事件开始参数: SpecialEventNo字段格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}
	theApp.m_EtcEventNo = EventDealStart["SpecialEventNo"].asString();
	if(EventDealStart["MessageCode"].type() == Json::nullValue || EventDealStart["MessageCode"].type() != Json::stringValue)
	{
		ErrorDescribe = "特情事件开始参数: MessageCode字段格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(EventDealStart["Data"].type() != Json::objectValue)
	{
		ErrorDescribe = "特情事件开始参数: Data字段格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}

	Data = EventDealStart["Data"];	
	if(Data["VehPlate"].type() == Json::nullValue || Data["VehPlate"].type() != Json::stringValue)
	{
		ErrorDescribe = "特情事件开始参数: VehPlate字段格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(Data["VehPlateColor"].type() == Json::nullValue || Data["VehPlateColor"].type() != Json::intValue)
	{
		ErrorDescribe = "特情事件开始参数: VehPlateColor字段格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(Data["VehClass"].type() == Json::nullValue || Data["VehClass"].type() != Json::intValue)
	{
		ErrorDescribe = "特情事件开始参数: VehClass字段格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(Data["VehType"].type() == Json::nullValue || Data["VehType"].type() != Json::intValue)
	{
		ErrorDescribe = "特情事件开始参数: VehType字段格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(Data["VehSeatCount"].type() == Json::nullValue || Data["VehSeatCount"].type() != Json::intValue)
	{
		ErrorDescribe = "特情事件开始参数: VehSeatCount字段格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(Data["axleCount"].type() == Json::nullValue || Data["axleCount"].type() != Json::intValue)
	{
		ErrorDescribe = "特情事件开始参数: axleCount字段格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(Data["CardNum"].type() == Json::nullValue || Data["CardNum"].type() != Json::stringValue)
	{
		ErrorDescribe = "特情事件开始参数: CardNum字段格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(Data["EventStartTime"].type() == Json::nullValue || Data["EventStartTime"].type() != Json::stringValue)
	{
		ErrorDescribe = "特情事件开始参数: EventStartTime字段格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(Data["EventDescribe"].type() == Json::nullValue || Data["EventDescribe"].type() != Json::stringValue)
	{
		ErrorDescribe = "特情事件开始参数: EventDescribe字段格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}	

	return true;
}

bool CTWSDNetPay_DllApp::CheckEventCheckVehQueueJsonValidity(const char* szParamContext, CString& ErrorDescribe)
{
	//进行数据类型校验
	Json::Reader reader;
	Json::Value  root;
	Json::Value  EventCheckVehQueue;
	Json::Value  Data;	

	if(!reader.parse(szParamContext, root))
	{
		ErrorDescribe = "确认车辆队列参数: Json格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(root["EventCheckVehQueue"].type() != Json::objectValue)
	{
		ErrorDescribe = "确认车辆队列参数: EventDealStart字段格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}

	EventCheckVehQueue = root["EventCheckVehQueue"];	
	if(EventCheckVehQueue["SpecialEventNo"].type() == Json::nullValue || EventCheckVehQueue["SpecialEventNo"].type() != Json::stringValue)
	{
		ErrorDescribe = "确认车辆队列参数: SpecialEventNo字段格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(EventCheckVehQueue["MessageCode"].type() == Json::nullValue || EventCheckVehQueue["MessageCode"].type() != Json::stringValue)
	{
		ErrorDescribe = "确认车辆队列参数: MessageCode字段格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(EventCheckVehQueue["Data"].type() != Json::objectValue)
	{
		ErrorDescribe = "确认车辆队列参数: Data字段格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}

	Data = EventCheckVehQueue["Data"];
	
	if(Data["VehPlate"].type() == Json::nullValue || Data["VehPlate"].type() != Json::stringValue)
	{
		ErrorDescribe = "确认车辆队列参数: VehPlate字段格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(Data["VehPlateColor"].type() == Json::nullValue || Data["VehPlateColor"].type() != Json::intValue)
	{
		ErrorDescribe = "确认车辆队列参数: VehPlateColor字段格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(Data["EventStartTime"].type() == Json::nullValue || Data["EventStartTime"].type() != Json::stringValue)
	{
		ErrorDescribe = "确认车辆队列参数: EventStartTime字段格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(Data["EventDescribe"].type() == Json::nullValue || Data["EventDescribe"].type() != Json::stringValue)
	{
		ErrorDescribe = "确认车辆队列参数: EventDescribe字段格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}	

	return true;
}

bool CTWSDNetPay_DllApp::CheckEventShowFeeInfoJsonValidity(const char* szParamContext, CString& ErrorDescribe)
{
	//进行数据类型校验
	Json::Reader reader;
	Json::Value  root;
	Json::Value  EventShowFeeInfo;
	Json::Value  FeeInfo;

	if(!reader.parse(szParamContext, root))
	{
		ErrorDescribe = "费额显示参数: Json格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(root["EventShowFeeInfo"].type() != Json::objectValue)
	{
		ErrorDescribe = "费额显示参数: EventShowFeeInfo字段格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}

	EventShowFeeInfo = root["EventShowFeeInfo"];	
	if(EventShowFeeInfo["SpecialEventNo"].type() == Json::nullValue || EventShowFeeInfo["SpecialEventNo"].type() != Json::stringValue)
	{
		ErrorDescribe = "费额显示参数: SpecialEventNo字段格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(EventShowFeeInfo["MessageCode"].type() == Json::nullValue || EventShowFeeInfo["MessageCode"].type() != Json::stringValue)
	{
		ErrorDescribe = "费额显示参数: MessageCode字段格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}

	if(EventShowFeeInfo["PayType"].type() == Json::nullValue || EventShowFeeInfo["PayType"].type() != Json::intValue)
	{
		ErrorDescribe = "费额显示参数: PayType字段格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(EventShowFeeInfo["DefaultFeeItemNo"].type() == Json::nullValue || EventShowFeeInfo["DefaultFeeItemNo"].type() != Json::intValue)
	{
		ErrorDescribe = "费额显示参数: DefaultFeeItemNo字段格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(EventShowFeeInfo["FeeCount"].type() == Json::nullValue || EventShowFeeInfo["FeeCount"].type() != Json::intValue)
	{
		ErrorDescribe = "费额显示参数: FeeCount字段格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(EventShowFeeInfo["FeeInfo"].type() == Json::nullValue || EventShowFeeInfo["FeeInfo"].type() != Json::arrayValue)
	{
		ErrorDescribe = "费额显示参数: FeeInfo字段格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}

	return true;
}

bool CTWSDNetPay_DllApp::CheckEventPayResultShowJsonValidity(const char* szParamContext, CString& ErrorDescribe)
{
	//进行数据类型校验
	Json::Reader reader;
	Json::Value  root;
	Json::Value  EventPayResultShow;
	Json::Value  EventInfo;

	if(!reader.parse(szParamContext, root))
	{
		ErrorDescribe = "展示支付状态: Json格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(root["EventPayResultShow"].type() != Json::objectValue)
	{
		ErrorDescribe = "展示支付状态: EventPayResultShow字段格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}

	EventPayResultShow = root["EventPayResultShow"];	
	if(EventPayResultShow["SpecialEventNo"].type() == Json::nullValue || EventPayResultShow["SpecialEventNo"].type() != Json::stringValue)
	{
		ErrorDescribe = "展示支付状态: SpecialEventNo字段格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(EventPayResultShow["MessageCode"].type() == Json::nullValue || EventPayResultShow["MessageCode"].type() != Json::stringValue)
	{
		ErrorDescribe = "展示支付状态: MessageCode字段格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}

	EventInfo = EventPayResultShow["EventInfo"];
	if(EventInfo["RequestTime"].type() == Json::nullValue || EventInfo["RequestTime"].type() != Json::stringValue)
	{
		ErrorDescribe = "展示支付状态: RequestTime字段格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(EventInfo["RequestType"].type() == Json::nullValue || EventInfo["RequestType"].type() != Json::stringValue)
	{
		ErrorDescribe = "展示支付状态: RequestType字段格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(EventInfo["PayResult"].type() == Json::nullValue || EventInfo["PayResult"].type() != Json::intValue)
	{
		ErrorDescribe = "展示支付状态: PayResult字段格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(EventInfo["Describe"].type() == Json::nullValue || EventInfo["Describe"].type() != Json::stringValue)
	{
		ErrorDescribe = "展示支付状态: Describe字段格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(EventInfo["TimeLimit"].type() == Json::nullValue || EventInfo["TimeLimit"].type() != Json::stringValue)
	{
		ErrorDescribe = "展示支付状态: TimeLimit字段格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}

	return true;
}

bool CTWSDNetPay_DllApp::CheckEventDealStopJsonValidity(const char* szParamContext, CString& ErrorDescribe)
{
	//进行数据类型校验
	Json::Reader reader;
	Json::Value  root;
	Json::Value  EventDealStop;
	Json::Value  EventEndCases;

	if(!reader.parse(szParamContext, root))
	{
		ErrorDescribe = "结束特情处理: Json格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(root["EventDealStop"].type() != Json::objectValue)
	{
		ErrorDescribe = "结束特情处理: EventDealStop字段格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}

	EventDealStop = root["EventDealStop"];	
	if(EventDealStop["SpecialEventNo"].type() == Json::nullValue || EventDealStop["SpecialEventNo"].type() != Json::stringValue)
	{
		ErrorDescribe = "结束特情处理: SpecialEventNo字段格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}
	theApp.m_EtcEventNo = EventDealStop["SpecialEventNo"].asString();

	if(EventDealStop["MessageCode"].type() == Json::nullValue || EventDealStop["MessageCode"].type() != Json::stringValue)
	{
		ErrorDescribe = "结束特情处理: MessageCode字段格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}

	if(EventDealStop["EventEndCases"].type() != Json::objectValue)
	{
		ErrorDescribe = "结束特情处理: EventEndCases字段格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}
	
	EventEndCases = EventDealStop["EventEndCases"];
	if(EventEndCases["EventVehPlate"].type() == Json::nullValue || EventEndCases["EventVehPlate"].type() != Json::stringValue)
	{
		ErrorDescribe = "结束特情处理: EventVehPlate字段格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(EventEndCases["EventEndTime"].type() == Json::nullValue || EventEndCases["EventEndTime"].type() != Json::stringValue)
	{
		ErrorDescribe = "结束特情处理: EventEndTime字段格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(EventEndCases["EventDescribe"].type() == Json::nullValue || EventEndCases["EventDescribe"].type() != Json::stringValue)
	{
		ErrorDescribe = "结束特情处理: EventDescribe字段格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(EventEndCases["EventEndReason"].type() == Json::nullValue || EventEndCases["EventEndReason"].type() != Json::stringValue)
	{
		ErrorDescribe = "结束特情处理: EventEndReason字段格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}

	return true;
}


bool CTWSDNetPay_DllApp::CheckEventCheckVehInfoJsonValidity(const char* szParamContext, CString& ErrorDescribe)
{
	//进行数据类型校验
	Json::Reader reader;
	Json::Value  root;
	Json::Value  EventCheckVehInfo;
	Json::Value  EventInfo;
	Json::Value  MMIContent;

	if(!reader.parse(szParamContext, root))
	{
		ErrorDescribe = "确认车辆信息: Json格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(root["EventCheckVehInfo"].type() != Json::objectValue)
	{
		ErrorDescribe = "确认车辆信息: EventCheckVehInfo字段格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}

	EventCheckVehInfo = root["EventCheckVehInfo"];	
	if(EventCheckVehInfo["SpecialEventNo"].type() == Json::nullValue || EventCheckVehInfo["SpecialEventNo"].type() != Json::stringValue)
	{
		ErrorDescribe = "确认车辆信息: SpecialEventNo字段格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(EventCheckVehInfo["MessageCode"].type() == Json::nullValue || EventCheckVehInfo["MessageCode"].type() != Json::stringValue)
	{
		ErrorDescribe = "确认车辆信息: MessageCode字段格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}

	EventInfo = EventCheckVehInfo["EventInfo"];
	if(EventInfo["VehPlateSnapImage"].type() == Json::nullValue || EventInfo["VehPlateSnapImage"].type() != Json::stringValue)
	{
		ErrorDescribe = "确认车辆信息: VehPlateSnapImage字段格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(EventInfo["CameraSnapImage"].type() == Json::nullValue || EventInfo["CameraSnapImage"].type() != Json::stringValue)
	{
		ErrorDescribe = "确认车辆信息: CameraSnapImage字段格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(EventInfo["RequestTime"].type() == Json::nullValue || EventInfo["RequestTime"].type() != Json::stringValue)
	{
		ErrorDescribe = "确认车辆信息: RequestTime字段格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(EventInfo["RequestType"].type() == Json::nullValue || EventInfo["RequestType"].type() != Json::stringValue)
	{
		ErrorDescribe = "确认车辆信息: RequestType字段格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(EventInfo["EventDescribe"].type() == Json::nullValue || EventInfo["EventDescribe"].type() != Json::stringValue)
	{
		ErrorDescribe = "确认车辆信息: EventDescribe字段格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(EventInfo["MMIDemand"].type() == Json::nullValue || EventInfo["MMIDemand"].type() != Json::stringValue)
	{
		ErrorDescribe = "确认车辆信息: MMIDemand字段格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(EventInfo["TimeLimit"].type() == Json::nullValue || EventInfo["TimeLimit"].type() != Json::stringValue)
	{
		ErrorDescribe = "确认车辆信息: TimeLimit字段格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}

	MMIContent = EventCheckVehInfo["MMIContent"];
	if(MMIContent["VehPlate"].type() == Json::nullValue || MMIContent["VehPlate"].type() != Json::stringValue)
	{
		ErrorDescribe = "确认车辆信息: VehPlate字段格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(MMIContent["VehPlateColor"].type() == Json::nullValue || MMIContent["VehPlateColor"].type() != Json::intValue)
	{
		ErrorDescribe = "确认车辆信息: VehPlateColor字段格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(MMIContent["VehClass"].type() == Json::nullValue || MMIContent["VehClass"].type() != Json::intValue)
	{
		ErrorDescribe = "确认车辆信息: VehClass字段格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(MMIContent["VehType"].type() == Json::nullValue || MMIContent["VehType"].type() != Json::intValue)
	{
		ErrorDescribe = "确认车辆信息: VehType字段格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(MMIContent["AxisCount"].type() == Json::nullValue || MMIContent["AxisCount"].type() != Json::intValue)
	{
		ErrorDescribe = "确认车辆信息: AxisCount字段格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(MMIContent["EntryTime"].type() == Json::nullValue || MMIContent["EntryTime"].type() != Json::stringValue)
	{
		ErrorDescribe = "确认车辆信息: EntryTime字段格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}

	return true;
}


bool CTWSDNetPay_DllApp::CheckEventCheckEntryInfoJsonValidity(const char* szParamContext, CString& ErrorDescribe)
{
	//进行数据类型校验
	Json::Reader reader;
	Json::Value  root;
	Json::Value  EventCheckEntryInfo;
	Json::Value  EventInfo;
	Json::Value  EntryInfo;

	if(!reader.parse(szParamContext, root))
	{
		ErrorDescribe = "确认车辆入口信息: Json格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(root["EventCheckEntryInfo"].type() != Json::objectValue)
	{
		ErrorDescribe = "确认车辆入口信息: EventCheckEntryInfo字段格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}

	EventCheckEntryInfo = root["EventCheckEntryInfo"];	
	if(EventCheckEntryInfo["SpecialEventNo"].type() == Json::nullValue || EventCheckEntryInfo["SpecialEventNo"].type() != Json::stringValue)
	{
		ErrorDescribe = "确认车辆入口信息: SpecialEventNo字段格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(EventCheckEntryInfo["MessageCode"].type() == Json::nullValue || EventCheckEntryInfo["MessageCode"].type() != Json::stringValue)
	{
		ErrorDescribe = "确认车辆入口信息: MessageCode字段格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}	

	EventInfo = EventCheckEntryInfo["EventInfo"];
	if(EventInfo["RequestTime"].type() == Json::nullValue || EventInfo["RequestTime"].type() != Json::stringValue)
	{
		ErrorDescribe = "确认车辆信息: RequestTime字段格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(EventInfo["RequestType"].type() == Json::nullValue || EventInfo["RequestType"].type() != Json::stringValue)
	{
		ErrorDescribe = "确认车辆信息: RequestType字段格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(EventInfo["EventDescribe"].type() == Json::nullValue || EventInfo["EventDescribe"].type() != Json::stringValue)
	{
		ErrorDescribe = "确认车辆信息: EventDescribe字段格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(EventInfo["MMIDemand"].type() == Json::nullValue || EventInfo["MMIDemand"].type() != Json::stringValue)
	{
		ErrorDescribe = "确认车辆信息: MMIDemand字段格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(EventInfo["TimeLimit"].type() == Json::nullValue || EventInfo["TimeLimit"].type() != Json::stringValue)
	{
		ErrorDescribe = "确认车辆信息: TimeLimit字段格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}

	return true;
}

bool CTWSDNetPay_DllApp::CheckCardOperationNotifyJsonValidity(const char* szParamContext, CString& ErrorDescribe, int& NotifyType)
{
	//进行数据类型校验
	Json::Reader reader;
	Json::Value  root;
	Json::Value  CardOperationNotify;
	Json::Value  Data;

	if(!reader.parse(szParamContext, root))
	{
		ErrorDescribe = "ETC卡操作提示接口: Json格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}

	if(root["CardOperationNotify"].type() != Json::objectValue)
	{
		ErrorDescribe = "ETC卡操作提示接口: CardOperationNotify字段格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}

	CardOperationNotify = root["CardOperationNotify"];	
	if(CardOperationNotify["SpecialEventNo"].type() == Json::nullValue || CardOperationNotify["SpecialEventNo"].type() != Json::stringValue)
	{
		ErrorDescribe = "ETC卡操作提示接口: SpecialEventNo字段格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(CardOperationNotify["MessageCode"].type() == Json::nullValue || CardOperationNotify["MessageCode"].type() != Json::stringValue)
	{
		ErrorDescribe = "ETC卡操作提示接口: MessageCode字段格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}

	Data = CardOperationNotify["Data"];
	if(Data["NotifyType"].type() == Json::nullValue || Data["NotifyType"].type() != Json::intValue)
	{
		ErrorDescribe = "ETC卡操作提示接口: NotifyType字段格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}
	NotifyType = Data["NotifyType"].asUInt();

	return true;
}

bool CTWSDNetPay_DllApp::CheckEventModifyVehQueueJsonValidity(const char* szParamContext, CString& ErrorDescribe)
{
	//进行数据类型校验
	Json::Reader reader;
	Json::Value  root;
	Json::Value  EventModifyVehQueue;

	if(!reader.parse(szParamContext, root))
	{
		ErrorDescribe = "修改车辆队列接口: Json格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}

	if(root["EventModifyVehQueue"].type() != Json::objectValue)
	{
		ErrorDescribe = "修改车辆队列接口: EventModifyVehQueue字段格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}

	EventModifyVehQueue = root["EventModifyVehQueue"];	
	if(EventModifyVehQueue["SpecialEventNo"].type() == Json::nullValue || EventModifyVehQueue["SpecialEventNo"].type() != Json::stringValue)
	{
		ErrorDescribe = "修改车辆队列接口: SpecialEventNo字段格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(EventModifyVehQueue["MessageCode"].type() == Json::nullValue || EventModifyVehQueue["MessageCode"].type() != Json::stringValue)
	{
		ErrorDescribe = "修改车辆队列接口: MessageCode字段格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(EventModifyVehQueue["TotalVehNum"].type() == Json::nullValue || EventModifyVehQueue["TotalVehNum"].type() != Json::intValue)
	{
		ErrorDescribe = "修改车辆队列接口: TotalVehNum字段格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}

	return true;
}

bool CTWSDNetPay_DllApp::CheckAuthorize(const char* szParamContext, CString& ErrorDescribe)
{
	//进行数据类型校验
	Json::Reader reader;
	Json::Value  root;
	Json::Value  Authorize, Data;

	if(!reader.parse(szParamContext, root))
	{
		ErrorDescribe = "其他授权接口: Json格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}

	if(root["Authorize"].type() != Json::objectValue)
	{
		ErrorDescribe = "其他授权接口: EventModifyVehQueue字段格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}

	Authorize = root["Authorize"];	
	if(Authorize["SpecialEventNo"].type() == Json::nullValue || Authorize["SpecialEventNo"].type() != Json::stringValue)
	{
		ErrorDescribe = "其他授权接口: SpecialEventNo字段格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(Authorize["MessageCode"].type() == Json::nullValue || Authorize["MessageCode"].type() != Json::stringValue)
	{
		ErrorDescribe = "其他授权接口: MessageCode字段格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(Authorize["VehPlate"].type() == Json::nullValue || Authorize["VehPlate"].type() != Json::stringValue)
	{
		ErrorDescribe = "其他授权接口: VehPlate字段格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}

	if(Authorize["VehPlateColor"].type() == Json::nullValue || Authorize["VehPlateColor"].type() != Json::stringValue)
	{
		ErrorDescribe = "其他授权接口: VehPlateColor字段格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}

	if(Authorize["Data"].type() != Json::objectValue)
	{
		ErrorDescribe = "其他授权接口: Data字段格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}
	Data = Authorize["Data"];

	if(Data["authorizeType"].type() == Json::nullValue || Data["authorizeType"].type() != Json::intValue)
	{
		ErrorDescribe = "其他授权接口: authorizeType字段格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}

	if(Data["authorizeReason"].type() == Json::nullValue || Data["authorizeReason"].type() != Json::stringValue)
	{
		ErrorDescribe = "其他授权接口: authorizeReason字段格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}

	if(Data["authorizeOption"].type() == Json::nullValue || Data["authorizeOption"].type() != Json::stringValue)
	{
		ErrorDescribe = "其他授权接口: authorizeOption字段格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}

	return true;
}


bool CTWSDNetPay_DllApp::CheckEventFeeAuthorize(const char* szParamContext, CString& ErrorDescribe)
{
	//进行数据类型校验
	Json::Reader reader;
	Json::Value  root;
	Json::Value  Authorize, Data;

	if(!reader.parse(szParamContext, root))
	{
		ErrorDescribe = "最小费额授权接口: Json格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}

	if(root["Authorize"].type() != Json::objectValue)
	{
		ErrorDescribe = "最小费额授权接口: EventModifyVehQueue字段格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}

	Authorize = root["Authorize"];	
	if(Authorize["SpecialEventNo"].type() == Json::nullValue || Authorize["SpecialEventNo"].type() != Json::stringValue)
	{
		ErrorDescribe = "最小费额授权接口: SpecialEventNo字段格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(Authorize["MessageCode"].type() == Json::nullValue || Authorize["MessageCode"].type() != Json::stringValue)
	{
		ErrorDescribe = "最小费额授权接口: MessageCode字段格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}
	//if(Authorize["VehPlate"].type() == Json::nullValue || Authorize["VehPlate"].type() != Json::stringValue)
	//{
	//	ErrorDescribe = "最小费额授权接口: VehPlate字段格式错误";
	//	RecordLog(ErrorDescribe);
	//	return false;
	//}

	//if(Authorize["VehPlateColor"].type() == Json::nullValue || Authorize["VehPlateColor"].type() != Json::stringValue)
	//{
	//	ErrorDescribe = "最小费额授权接口: VehPlateColor字段格式错误";
	//	RecordLog(ErrorDescribe);
	//	return false;
	//}

	if(Authorize["Data"].type() != Json::objectValue)
	{
		ErrorDescribe = "最小费额授权接口: Data字段格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}
	Data = Authorize["Data"];

	if(Data["authorizeType"].type() == Json::nullValue || Data["authorizeType"].type() != Json::intValue)
	{
		ErrorDescribe = "最小费额授权接口: authorizeType字段格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}

	//if(Data["authorizeReason"].type() == Json::nullValue || Data["authorizeReason"].type() != Json::stringValue)
	//{
	//	ErrorDescribe = "最小费额授权接口: authorizeReason字段格式错误";
	//	RecordLog(ErrorDescribe);
	//	return false;
	//}

	//if(Data["authorizeOption"].type() == Json::nullValue || Data["authorizeOption"].type() != Json::stringValue)
	//{
	//	ErrorDescribe = "最小费额授权接口: authorizeOption字段格式错误";
	//	RecordLog(ErrorDescribe);
	//	return false;
	//}

	return true;
}


bool CTWSDNetPay_DllApp::CheckEventDelVehQueueResult(const char* szParamContext, CString& ErrorDescribe)
{
	//进行数据类型校验
	Json::Reader reader;
	Json::Value  root;
	Json::Value  DelVehQueueResult, Data;

	if(!reader.parse(szParamContext, root))
	{
		ErrorDescribe = "删除车辆队列结果接口: Json格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}

	if(root["DelVehQueueResult"].type() != Json::objectValue)
	{
		ErrorDescribe = "删除车辆队列结果接口: DelVehQueueResult字段格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}

	DelVehQueueResult = root["DelVehQueueResult"];	
	if(DelVehQueueResult["SpecialEventNo"].type() == Json::nullValue || DelVehQueueResult["SpecialEventNo"].type() != Json::stringValue)
	{
		ErrorDescribe = "删除车辆队列结果接口: SpecialEventNo字段格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}
	if(DelVehQueueResult["MessageCode"].type() == Json::nullValue || DelVehQueueResult["MessageCode"].type() != Json::stringValue)
	{
		ErrorDescribe = "删除车辆队列结果接口: MessageCode字段格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}

	if(DelVehQueueResult["Data"].type() != Json::objectValue)
	{
		ErrorDescribe = "删除车辆队列结果接口: Data字段格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}
	Data = DelVehQueueResult["Data"];

	if(Data["ExecuteType"].type() == Json::nullValue || Data["ExecuteType"].type() != Json::intValue)
	{
		ErrorDescribe = "删除车辆队列结果接口: ExecuteType字段格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}

	if(Data["DelVehNum"].type() == Json::nullValue || Data["DelVehNum"].type() != Json::intValue)
	{
		ErrorDescribe = "删除车辆队列结果接口: DelVehNum字段格式错误";
		RecordLog(ErrorDescribe);
		return false;
	}

	if(Data["ExecuteResult"].type() == Json::nullValue || Data["ExecuteResult"].type() != Json::intValue)
	{
		ErrorDescribe = "删除车辆队列结果接口: ExecuteResult字段格式错误";
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
	// 接收客户端的信息，转发给读卡器
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	RecordLog(FmtStr(_T("[ETC特情业务],转发数据给串口接口(EtcEventWebsocketControl)调用开始")));
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
            strLog.Format(_T("EtcEvent环境初始化接口(IF_EventInitEnvironment3),业务管理线程资源为空！"));
            break;
        case -2:
            strLog.Format(_T("EtcEvent环境初始化接口(IF_EventInitEnvironment3),设备控制线程资源为空！"));
            break;
        }
        //记录日志
		RecordLog(strLog);
	}

	return bRet;

}

/****************************************内部辅助函数********************************************/

////////////////////////////////////////////////////////////////////////////////////////////////
// 函数名称：CTWSDNetPay_DllApp::RecordLog
//
// 功能描述：记录日志
//
// 输入参数：IN const CString& strLog -- 日志内容
//         ：IN const int& iLevel    -- 日志等级
//         ：IN const BOOL& bSave    -- 日志是否立马保存
// 输出参数：void
// 编写人员：ROCY
// 编写时间：2016年7月24日
// 修改人员：
// 修改时间：
// 函数版本：1.0.0.1
// 备注说明：
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
// 函数名称：CTWSDNetPay_DllApp::ReadConfigInfo
//
// 功能描述：读取配置信息
//
// 输入参数：void
// 输出参数：void
// 编写人员：ROCY
// 编写时间：2016年7月24日
// 修改人员：
// 修改时间：
// 函数版本：1.0.0.1
// 备注说明：
//
////////////////////////////////////////////////////////////////////////////////////////////////
void CTWSDNetPay_DllApp::ReadConfigInfo(void)
{
    TCHAR szTemp[256] = {0};
    CString strConfigFilePath;
    strConfigFilePath.Format(_T("%s\\%s"), m_strWorkDir, FILE_NAME_APP_CONFIG);
    //是否保存日志
    m_bSaveLog              = (BOOL)::GetPrivateProfileIntA(_T("Log"), _T("Save"), 0, strConfigFilePath);
    //获取日志删除限定时间段起点
    m_iDeleteLogStartHour = ::GetPrivateProfileIntA(_T("Log"), _T("DeleteStartHour"), 2, strConfigFilePath);
    //获取日志删除限定时间段终点
    m_iDeleteLogEndHour = ::GetPrivateProfileIntA(_T("Log"), _T("DeleteEndHour"), 2, strConfigFilePath);
    //日志删除操作轮询时间
    m_iDeleteLogSpanTime = ::GetPrivateProfileIntA(_T("Log"), _T("DeleteSpanTime"), 30 * 60, strConfigFilePath);
    //日志本地保存天数时间
    m_iDeleteLogSaveDays = ::GetPrivateProfileIntA(_T("Log"), _T("DeleteSaveDays"), 60, strConfigFilePath);
    //标记是否测试数据
    m_bIsTestData           = (BOOL)::GetPrivateProfileIntA(_T("Setup"), _T("TestData"), 0, strConfigFilePath);
    //线程存活日志检测时间间隔
    m_dwThreadCheckSpanTime = ::GetPrivateProfileIntA(_T("Setup"), _T("ThreadCheckSpanTime"), 0, strConfigFilePath);
    //获取虚拟卡前缀
    ::GetPrivateProfileString(_T("Setup"), _T("VirtualCardPrefix"), _T("4412"), szTemp, 256, strConfigFilePath);
    m_strVirtualCardPrefix.Format(_T("%s"), szTemp);
    //获取往智能硬件下发时间的间隔
    m_dwUpdateDevieTimeSpanTime = (DWORD)::GetPrivateProfileIntA(_T("Setup"), _T("UpdateDeviceTimeSpanTime"), 30 * 60, strConfigFilePath);
    m_dwUpdateDevieTimeSpanTime *= 1000;
    //撤单数据重发检测时间间隔
    m_tResendDataSpanTime       = (DWORD)::GetPrivateProfileInt(_T("CancelRecord"), _T("CheckTime"), 30, strConfigFilePath);
    //撤单数据留存本地时限（单位：分钟）
    m_iLastDelayTime            = (DWORD)::GetPrivateProfileInt(_T("CancelRecord"), _T("LastDelay"), 30, strConfigFilePath);
    //程序退出前的等待时间（等待资源清除）
    m_iExitWaitForTime          = (DWORD)::GetPrivateProfileInt(_T("Setup"), _T("ExitWaitForTime"), 500, strConfigFilePath);
    //HTTP内部版号
    m_iHTTPVersion              = ::GetPrivateProfileInt(_T("ReflectWeb"),  _T("Version"),  1,  strConfigFilePath);

    //add 2018-04
    char szTempIP[256] = {0};
    ::GetPrivateProfileString(_T("ReflectWeb"), _T("IP"), _T("101.200.214.203"), szTempIP, _countof(szTempIP), strConfigFilePath);
    _tcscpy_s(m_sTranstParam.LaneConfigInfo.WebServerIP, _countof(m_sTranstParam.LaneConfigInfo.WebServerIP), szTempIP);
    m_sTranstParam.LaneConfigInfo.WebServerPort =  ::GetPrivateProfileInt(_T("ReflectWeb"), _T("PORT"), 80, strConfigFilePath);

    int int_para = ::GetPrivateProfileInt(_T("TimeArgu"), _T("LPRQueryLimit"), 100, strConfigFilePath);
    GetGlobalApp()->SetLPRQueryOverTimeMilli(int_para);



    //确定通讯动态库加载的设备厂家类型
    m_iDeviceType               = ::GetPrivateProfileInt(_T("Setup"),       _T("Device"),   0,  strConfigFilePath);
    //是否将移动支付和代扣支付的支付渠道返回码保持一致
    m_bReturnPayChannelCodeUnify    = (bool)::GetPrivateProfileInt(_T("Setup"),     _T("ReturnPayChannelCodeUnify"), 0, strConfigFilePath);
    //获取代扣扣费操作超时时间值
    m_iAgencyDebitTimeout       = ::GetPrivateProfileInt(_T("Setup"), _T("AgencyDebitTimeout"), 2000, strConfigFilePath);
    //获取清屏操作内部延时时间值
    m_iClearMMIDelayTime        = ::GetPrivateProfileInt(_T("Setup"), _T("ClearMMIDelayTime"), 100, strConfigFilePath);
    //是否启用代扣动态库
    m_bUseAgencyDll             = (bool)::GetPrivateProfileInt(_T("Setup"), _T("UseAgencyPayDll"), 0, strConfigFilePath);
    //是否使用扩展的交互细节模式
    m_bIsExternMMIDetail        = (BOOL)::GetPrivateProfileInt(_T("Setup"), _T("UseExternMMI"), 0, strConfigFilePath);
    //是否允许使用新版本的交互细节模式
    m_bIsAllowUseNewMMIDetailMode = (BOOL)::GetPrivateProfileInt(_T("Setup"), _T("AllowUseNewMMIMode"), 0, strConfigFilePath);

    //是否启用银联非接模块
    m_bNFCIsUse                         = (bool)::GetPrivateProfileInt(_T("Unipay"), _T("UseUnipay"), 0, strConfigFilePath);
    //银联模块签到状态监测时间间隔
    m_iNFCCheckSignSpanTime             = ::GetPrivateProfileInt(_T("Unipay"), _T("CheckSignSpanTime"), 10, strConfigFilePath);
    if (m_iNFCCheckSignSpanTime > 1800 || m_iNFCCheckSignSpanTime < 10)
    {
        m_iNFCCheckSignSpanTime = 30;
    }
    //银联模块定时重签检测时间起始点
    m_iNFCDefaultStartReSignHour        = ::GetPrivateProfileInt(_T("Unipay"), _T("AutoStartReSignHour"), 0, strConfigFilePath);
    //银联模块定时重签检测时间截止点
    m_iNFCDefaultEndReSignHour          = ::GetPrivateProfileInt(_T("Unipay"), _T("AutoEndReSignHour"), 3, strConfigFilePath);
    //银联模块签到数据交互时间间隔限制
    m_iNFCSignDataLimitSpanTime         = ::GetPrivateProfileInt(_T("Unipay"), _T("SignDataLimitSpanTime"), 60, strConfigFilePath);
    if (m_iNFCSignDataLimitSpanTime > 70 || m_iNFCSignDataLimitSpanTime < 30)
    {
        m_iNFCSignDataLimitSpanTime = 60;
    }
    //银联支付流程，是否允许没有获取冲正报文也允许发送扣费请求
    m_bNFCIsAllowDebitWithoutCancelData = (BOOL)::GetPrivateProfileInt(_T("Unipay"), _T("AllowDebitWithoutCancelData"), 0, strConfigFilePath);
    //银联支付流程，获取到刷卡数据后，限制等待冲正数据的时间（单位：秒）
    m_iNFCGetCancelDataLimitTime        = ::GetPrivateProfileInt(_T("Unipay"), _T("GetCancelDataLimitTime"), 10, strConfigFilePath);
    if (m_iNFCGetCancelDataLimitTime > 15 || m_iNFCGetCancelDataLimitTime < 5)
    {
        m_iNFCGetCancelDataLimitTime = 10;
    }
    m_sTranstParam.LaneConfigInfo.ForceUTC8 = ::GetPrivateProfileInt(_T("SetUp"), _T("ForceUTC8"), 0, strConfigFilePath);



}

////////////////////////////////////////////////////////////////////////////////////////////////
// 函数名称：CTWSDNetPay_DllApp::fnCloseThread
//
// 功能描述：等待一个即将被关闭线程正常关闭
//
// 输入参数：IN const HANDLE& hThread          --  需要被关闭的线程句柄
//         ：IN const DWORD& dwMilliseconds        --  等待时间（单位：毫秒）
// 输出参数：int
//         ：0  -- 线程已经被正常关闭
//         ：-1    -- 线程句柄无效
//         ：-2    -- 内核对象未激发而等待时间已到
//         ：-3    -- 等待的互斥器未被释放
//         ：-4    -- 等待函数执行失败
// 编写人员：ROCY
// 编写时间：2015年11月15日
// 修改人员：
// 修改时间：
// 函数版本：1.0.0.1
// 备注说明：
//
////////////////////////////////////////////////////////////////////////////////////////////////
int CTWSDNetPay_DllApp::fnCloseThread(IN const HANDLE& hThread, IN const DWORD& dwMilliseconds)
{
    int iReturn = 0;
    //尝试关闭线程
    DWORD dwWaitResult = ::WaitForSingleObject(hThread, dwMilliseconds);
    //获得错误码
    DWORD dwErrorCode = ::GetLastError();
    //消息结构体提
    MSG  mMsg;

    if (6 == dwErrorCode)//句柄无效
    {
        iReturn = 0;
    }
    else
    {
        //判断等待函数返回结果
        switch (dwWaitResult)
        {
        case WAIT_OBJECT_0: //内核对象变为激发状态
            iReturn = 0;
            break;
        case WAIT_TIMEOUT:  //内核对象未激发而等待时间已到
        {
            if (::PeekMessage(&mMsg, NULL, 0, 0, PM_REMOVE))
            {
                ::TranslateMessage(&mMsg); Sleep(10);
                ::DispatchMessage(&mMsg); Sleep(10);
            }
        }
        iReturn = -2;
        break;
        case WAIT_ABANDONED://等待的互斥器未被释放
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
//  作   者 : ROCY
//  时   间 : 2008-05-31
//  版   本 : 1.0.0.1
//  函数名称 : DisplayWinError
//  函数功能 : 显示系统最新的错误信息
//  输入参数 : DWORD    --  dwErrorCode 给定的系统错误代码,一般由 GetLastError() 函数捕获
//             int      --  iType       默认为0
//             BOOL     --  bPrompt     是否需要用对话框显示该错误信息（TRUE为需要，FALSE为不需要）
//  输出参数 : LPCTSTR -- 详细的错误描述信息
//  备注说明 : 每次调用API后都可以调用此函数查看详细的错误信息
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
    strTemp.Format(_T("【错误代码:%d】 -- 【详细描述:%s】"), dwErrorCode, A2T(lpMessageBuffer));
    strTemp.Replace(_T("\r\n"), _T(" "));

    if (bPrompt)
        ::MessageBox(NULL, strTemp, _T("系统提示"), MB_ICONWARNING);

    LocalFree(lpMessageBuffer);
    return strTemp;
}

//获取当前进程所使用的内存
DWORD CTWSDNetPay_DllApp::GetProcessMemory(IN const int& iShowType)
{
    DWORD dDevValue = (1 == iShowType) ? (1024 * 1024) : 1024;
    //获取当前进程使用内存信息
    DWORD dwProcessMemory = 0x00;
    PROCESS_MEMORY_COUNTERS pLocalProcessMemory = {0};
    if (GetProcessMemoryInfo(GetCurrentProcess(), &pLocalProcessMemory, sizeof(pLocalProcessMemory)))
    {
        dwProcessMemory = pLocalProcessMemory.PagefileUsage / dDevValue;
    }

    return dwProcessMemory;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// 函数名称：CTWSDNetPay_DllApp::AgencyInitDll
//
// 功能描述：初始化代扣动态库
//
// 输入参数：void
// 输出参数：bool
// 编写人员：ROCY
// 编写时间：2017年8月24日
// 修改人员：
// 修改时间：
// 函数版本：1.0.0.1
// 备注说明：
//
////////////////////////////////////////////////////////////////////////////////////////////////
bool CTWSDNetPay_DllApp::AgencyInitDll(IN const char* szAreaInfo, IN const char* szLoaclStation, IN const char* szLoaclLaneID, IN const char* szServerInfo, IN const int& iProvinceID)
{
    bool bInitAgencyDll = false;

    //判断是否启用代扣动态库
	m_bUseAgencyDll = false;
    if (!m_bUseAgencyDll)
    {
        RecordLog( FmtStr(_T("[主进程响应],组件初始化接口(IF_InitEnvironment)，代扣动态库未启用!)")) );
        return bInitAgencyDll;
    }
    if (m_pThreadManage == 0)
    {
        RecordLog( FmtStr(_T("[主进程响应],无法初始化代扣库，管理线程未创建!)")) );
        return  false;
    }
    {//add 2019-01 
        Sleep(150);//保证m_pThreadManage已经启动
        if (m_pThreadManage->IsFuturePay())
        {
            int init_result = m_pThreadManage->InitAgencyPay(szAreaInfo, szLoaclStation, szLoaclLaneID, szServerInfo, iProvinceID);
            return (init_result == 0); //modify 2019-01 使用新的支付逻辑
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
            RecordLog( FmtStr(_T("[主进程响应],组件初始化接口(IF_InitEnvironment)，代扣动态库(%s)初始化成功!)"), strAgencyPayDll) );
        }
        else
        {
            RecordLog( FmtStr(_T("[主进程响应],组件初始化接口(IF_InitEnvironment),内部初始化错误,无法初始化代扣动态库(错误代码:%d)"), iResultCode) );
        }
    }
    else
    {
        RecordLog( FmtStr(_T("[主进程响应],组件初始化接口(IF_InitEnvironment),内部初始化错误,代扣动态库(%s)加载失败"), strAgencyPayDll) );
    }
    return bInitAgencyDll;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// 函数名称：CTWSDNetPay_DllApp::AgencyConstructDebitData
//
// 功能描述：
//
// 输入参数：void
// 输出参数：int
// 编写人员：ROCY
// 编写时间：2017年8月17日
// 修改人员：
// 修改时间：
// 函数版本：1.0.0.1
// 备注说明：当返回结果为true，则表示不再进行后续的扫码业务流程；当返回结果为false，则表示继续需要执行扫码业务流程
//
////////////////////////////////////////////////////////////////////////////////////////////////
bool CTWSDNetPay_DllApp::AgencyConstructDebitData(int & agency_result)
{
    //判断是否启用代扣动态库
    if (!m_bUseAgencyDll)
    {
        return false;
    }

    bool is_disable_agency = (m_pCurrentDebitInfo.PayModel == 1);
    if (is_disable_agency)
    {
        RecordLog(_T("[主进程响应],执行扣款操作接口(IF_DebitMoney): 不进行代扣处理"));
        return false;
    }

    //首先判断车牌有效（长度最少7位）
    if ( m_pCurrentDebitInfo.VehicleLicense.GetLength() < 7 )
        return false;
    BYTE byFunCode      = 0x01;
    BYTE byExecuteState = RCC_INNER_ERROR;

    m_dwTradeBeginOvertime = ::GetTickCount();  //计时
    m_strLastErrorDesc.Format(_T("[主进程响应],执行扣款操作(IF_DebitMoney), 正在进行车辆代扣处理"));

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
            //add 2018-04 将代扣结果回馈给车牌识别
            GetGlobalApp()->LPR_PackPayResult(GetGlobalApp()->m_sCurrentDebitResult.PayPlatformType);
            //通知成功
            byFunCode = 0x01;
            byExecuteState = RCC_OK;
            //控制终端提示成功
            m_pThreadManage->m_pThreadDevice->PostThreadMessage(WM_THREAD_DEVICE_CONTROL_DEAL_RESULT_MMI, (WPARAM)0x01, (LPARAM)serverDebitType);//modify 2018-09
            //获取结果信息成功，通知调用者
            NotifyCaller(byFunCode, byExecuteState);
            //记录日志
            RecordLog(FmtStr(_T("[主进程响应],执行扣款操作接口(IF_DebitMoney)调用结束(%s,代扣支付,成功）"), m_pCurrentDebitInfo.VehicleLicense));
            return true;
        }
        else
        {
            iIsCancelAnency = 0x01;
        }
    }
    else if (iAnencyResult == 25) //如果是车型不符
    {
        GetGlobalApp()->m_sCurrentDebitResult.ExecuteCode = 10;//
    }
    else
    {
        //判断返回错误，确定是否需要发起代扣撤单
        switch (iAnencyResult)
        {
        case 6:
        case 12:
            iIsCancelAnency = 0x02;
            break;
        }
    }

    //判断是否需要执行代扣撤单
    if (iIsCancelAnency > 0)
    {
        //标识扣费流程完成
        GetGlobalApp()->m_bIsTrading        = FALSE;
        GetGlobalApp()->m_bCurrentDebitMode = FALSE;

        AgencyConstructCancelData(m_pCurrentDebitInfo.PayIdentifier);
        //通知失败
        byFunCode = 0x01;
        byExecuteState = RCC_PAY_PLATFORM_ERROR;
        //通知调用者
        NotifyCaller(byFunCode, byExecuteState);
        //记录日志
        RecordLog(FmtStr(_T("[主进程响应],车辆(%s)代扣扣费结果处理失败(%s)，已经发起撤单，转入扫码处理"), m_pCurrentDebitInfo.VehicleLicense, (0x01 == iIsCancelAnency) ? _T("获取结果异常") : _T("网络超时")) );
        //不再进行扫码流程
        return true;
    }
    else
    {
        RecordLog(FmtStr(_T("[主进程响应],车辆(%s)代扣扣费结果处理失败，不需要撤单，转入扫码处理"), m_pCurrentDebitInfo.VehicleLicense) );
    }
    //继续进行扫码流程
    return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// 函数名称：CTWSDNetPay_DllApp::AgencyConstructCancelData
//
// 功能描述：构造代扣撤单数据结构体
//
// 输入参数：无参数
// 编写人员：ROCY
// 编写时间：2017年8月17日
// 修改人员：
// 修改时间：
// 函数版本：1.0.0.1
// 备注说明：
//
////////////////////////////////////////////////////////////////////////////////////////////////
int CTWSDNetPay_DllApp::AgencyConstructCancelData(IN const CString& strPayIdentifier)
{
    //判断是否启用代扣动态库
    if (!m_bUseAgencyDll)
    {
        return false;
    }

    //获取结果信息失败，进行撤单处理，转入扫码
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
// 函数名称：CTWSDNetPay_DllApp::GetAgencyResult
//
// 功能描述：解析代扣的返回结果字符串，形成格式字符串
//
// 输入参数：CString strXML
// 输出参数：int
// 编写人员：ROCY
// 编写时间：2017年8月17日
// 修改人员：
// 修改时间：
// 函数版本：1.0.0.1
// 备注说明：
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
            //判断是否代扣支付
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

            //判断是否将代扣支付合并到移动支付原有定义当中
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
            bool is_jilin = (m_sTranstParam.ProvinceID == 22);//吉林省

            if ( is_jilin &&
                    (m_sCurrentDebitResult.PayPlatformType == PCTC_ICBC_AGENCY)
               )
            {
                m_sCurrentDebitResult.PayPlatformType = PCTC_ALIIPAY;
                //GetGlobalApp()->RecordLog("吉林省份，工行支付数据归类到支付宝");
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

            //获取代扣消耗时间
            m_sCurrentDebitResult.ConsumeTime = dwConsumeTime;

            //其它数据
            m_sCurrentDebitResult.ConsumeTime   = (::GetTickCount() - m_dwTradeBeginOvertime);
            m_sCurrentDebitResult.Valid         = TRUE;

            //标识扣费流程完成
            GetGlobalApp()->m_bIsTrading        = FALSE;
            GetGlobalApp()->m_bCurrentDebitMode = FALSE;
            //返回代码
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
            m_strLastErrorDesc.Format(_T("代扣结果解析失败，加载XML串异常"));
            break;
        case -2:
            m_strLastErrorDesc.Format(_T("代扣结果解析失败，未找到XML头 DebitResultContext"));
            break;
        case -3:
            m_strLastErrorDesc.Format(_T("代扣结果解析失败, 未找到指定数据区 Data"));
            break;
        case -11:
            m_strLastErrorDesc.Format(_T("代扣结果解析失败, 未找到指定数据项 ResultType"));
            break;
        case -12:
            m_strLastErrorDesc.Format(_T("代扣结果解析失败, 未找到指定数据项 ExecuteCode"));
            break;
        case -13:
            m_strLastErrorDesc.Format(_T("代扣结果解析失败, 未找到指定数据项 ExecuteDesc"));
            break;
        case -14:
            m_strLastErrorDesc.Format(_T("代扣结果解析失败, 未找到指定数据区 TradeKeyItem"));
            break;
        case -15:
            m_strLastErrorDesc.Format(_T("代扣结果解析失败, 未找到指定数据项 TradeSecurityCode"));
            break;
        case -16:
            m_strLastErrorDesc.Format(_T("代扣结果解析失败, 未找到指定数据项 PayPlatformType"));
            break;
        case -17:
            m_strLastErrorDesc.Format(_T("代扣结果解析失败, 未找到指定数据项 PayIdentifier"));
            break;
        case -18:
            m_strLastErrorDesc.Format(_T("代扣结果解析失败, 未找到指定数据项 DebitOrder"));
            break;
        case -19:
            m_strLastErrorDesc.Format(_T("代扣结果解析失败, 未找到指定数据项 DebitTime"));
            break;

        default:
            m_strLastErrorDesc.Format(_T("代扣结果解析失败, 未知异常"));
            break;
        }
        RecordLog( m_strLastErrorDesc );
    }

    return iReturn;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// 函数名称：CTWSDNetPay_DllApp::Hex2Bin
//
// 功能描述：将16进制的字符串转换成2进制的数字
//
// 输入参数：IN LPTSTR lpHex   -- 需要转换的16进制字符串
//         ：OUT PBYTE pBin        -- 输出的2进制数字数组
//         ：IN DWORD dwHex_len    -- 需要转换的16进制数据长度,必须为2的倍数
// 输出参数：void
// 编写人员：ROCY
// 编写时间：2017年9月21日
// 修改人员：
// 修改时间：
// 函数版本：1.0.0.1
// 备注说明：所输入的16进制字符串长度必须是2的倍数
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
// 函数名称：CTWSDNetPay_DllApp::Bin2Hex
//
// 功能描述：将2进制的数字转换成16进制的字符串
//
// 输入参数：IN PBYTE pBin         -- 需要转换的2进制数字数组
//         ：IN DWORD dwBin_Len        -- 需要转换的2进制数字数组长度
// 输出参数：CString  非空 -- 转换成功后输出的16进制字符串;NULL -- 转换失败
// 编写人员：ROCY
// 编写时间：2017年9月21日
// 修改人员：
// 修改时间：
// 函数版本：1.0.0.1
// 备注说明：
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
// 函数名称：CTWSDNetPay_DllApp::HexToBase64
//
// 功能描述：转换16进制字符串为base64编码
//
// 输入参数：IN const CString& strSrcHex
//         ：OUT CString& strDesBase64
// 输出参数：BOOL
// 编写人员：ROCY
// 编写时间：2017年9月21日
// 修改人员：
// 修改时间：
// 函数版本：1.0.0.1
// 备注说明：内部接口
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

        //清理资源
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
// 函数名称：CTWSDNetPay_DllApp::Base64ToHex
//
// 功能描述：转换base64编码为16进制字符
//
// 输入参数：IN const CString& strSrcBase64
//         ：OUT CString& strDesHex
// 输出参数：BOOL
// 编写人员：ROCY
// 编写时间：2017年9月22日
// 修改人员：
// 修改时间：
// 函数版本：1.0.0.1
// 备注说明：
//
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CTWSDNetPay_DllApp::Base64ToHex(IN const CString& strSrcBase64, OUT CString& strDesHex)
{
    BOOL bReturn = FALSE;
    try
    {
        //判断长度
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
// 函数名称：CTWSDNetPay_DllApp::BASE64_Encode
//
// 功能描述：数据转换辅助函数
//
// 输入参数：const BYTE* inputBuffer
//         ：INT inputCount
//         ：TCHAR* outputBuffer
// 输出参数：INT
// 编写人员：ROCY
// 编写时间：2017年9月21日
// 修改人员：
// 修改时间：
// 函数版本：1.0.0.1
// 备注说明：
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
        return -1;  // 参数错误
    }

    if (outputBuffer != NULL)
    {
        for (i = inputCount; i > 0; i -= 3)
        {
            if (i >= 3)
            {   // 将3字节数据转换成4个ASCII字符
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

        *outputBuffer++ = TEXT('/0');  // 添加字符串结束标记
    }

    return ((inputCount + 2) / 3) * 4;  // 返回有效字符个数
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

//delete 2018-05 暂时保留代码，以防止外部变更
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
 * 处理硬件数据查询
 * @param  para
 * @return
 */
int CTWSDNetPay_DllApp::DevcieQueryInterrupt(const char* para)
{
    //查询NFC连接状态
    bool nfc_link_success = GetGlobalApp()->m_NfcLinkSuccess;

    //查询NFC签到状态
    bool nfc_signin_success = GetGlobalApp()->m_bNFCSignInSuccess;

    //获取非接模块设备信息
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
        //硬件测试查询
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

//东八区返回8，
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
// 函数名称:CTWSDNetPay_DllApp::QueryImageIdFromLPRSystem
//
// 功能描述：获取车牌识别系统拿到的图像ID
//
// 输入参数:
// 输出参数:int
// 编写人员:朱亚冰
// 编写时间:2018-04-23
// 修改人员:
// 修改时间:
// 函数版本:1.0.0.0
// 备注说明:
// 1、此过程会自动超时，目前为100ms
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
    //delete 2018-05 下面的代码，已经无效，暂时保留，是因为外部的需求，随时会发生变化，以防不测
    // m_LPR.Reset();
    // //发送消息
    // static char *chs= "{\"type\":2,\"data\":\"\"}";
    // clock_t before_send = clock();
    // //GetGlobalApp()->send2Bus(KEY_COMMONBUS_ACCESS,chs,strlen(chs),KEY_BUSINESS);
    // clock_t after_send = clock();
    // CString debug_info;
    // debug_info.Format("about send %d  -->  %d",before_send,after_send);
    // //RecordLog(debug_info);
    // m_pThreadManage->PostThreadMessage(WM_THREAD_LPR_IMAGEID, (WPARAM)0, (LPARAM)0);//使用异步发送，此异步操作以来与manage线程，可能存在阻塞
    // static int queryOverCount = GetLPRQueryOverTimeMilli()/10;
//    for(int i=0;i< queryOverCount ;++i)//循环等待
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
    //  RecordLog("获取车牌图像标识超时");
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
    debug_info.Format("车牌识别超时时间设置:%dms", m_LPRQueryOverTimeMilli);
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

    //delete 2018-05 暂时保留之前的代码，以防外部要求变动
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

    bool is_tianjin = (m_sTranstParam.ProvinceID == 12);//是否是天津
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
        sprintf(err_log, "虚拟卡执行码解析结果：%d", etc_excute_code);
        GetGlobalApp()->RecordLog(err_log);
        break;
    }

}

int CTWSDNetPay_DllApp::NotifyPayStatus(int type, int value)
{
    //目前type都是代扣，不用区分
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

//add 2018-03，此处进行commonbus的兼容处理

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
    //FreeLibrary(m_Forward_Dll);//2019-04 zhuyabing 此处卸载存在问题，故取消

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
        TW_TRACE_DIARY_TYPE("未进行初始化", TW_TRACE_TYPE_INTERFACE);
        return 255;
    }
    if (!m_bAuthorizedLegal)
    {
        TW_TRACE_DIARY_TYPE("未授权", TW_TRACE_TYPE_INTERFACE);
        return 255;
    }
    if ((NULL == szParamContext) || (0 == iParamSize) )
    {
        TW_TRACE_DIARY_TYPE("参数异常", TW_TRACE_TYPE_INTERFACE);
        return 255;
    }
    if (NULL == m_pThreadManage)
    {
        TW_TRACE_DIARY_TYPE("管理线程无效", TW_TRACE_TYPE_INTERFACE);
        return 255;
    }
    if (GetGlobalApp()->m_EtcPayCenter.HasUnrecoverableError())
    {
        TW_TRACE_DIARY_TYPE("存在无法恢复的的错误，禁止调用", TW_TRACE_TYPE_INTERFACE);
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
    //=======白名单查询参数测试==========
    std::string correct_para = "<?xml version=\"1.0\" encoding=\"GB2312\"?><ETCDebitListQuery><Version>1.0</Version><Data><TradeKeyItem><PayIdentifier/><Money>1</Money><OBUMAC/><OBUID/><CardID>0</CardID><Vehicle><Class>1</Class><Type>1</Type><License>京A12345</License><VLColor>0</VLColor><AutoPlate><AutoLicense>AutoLicense</AutoLicense><AutoColor>1</AutoColor></AutoPlate></Vehicle><Entry><RoadID>49</RoadID><StationID>49</StationID><LaneID>12</LaneID><Time>2019-01-04 11:40:21</Time></Entry></TradeKeyItem></Data></ETCDebitListQuery>";
    std::string lack_filed_para = "<?xml version=\"1.0\" encoding=\"GB2312\"?><ETCDebitListQuery><Version>1.0</Version><Data><TradeKeyItem><PayIdentifier/><Money>1</Money><OBUMAC/><OBUID/><CardID>0</CardID><Vehicle><Class>1</Class><Type>1</Type><License>京A12345</License><VLColor>0</VLColor><AutoPlate><AutoLicense>AutoLicense</AutoLicense><AutoColor>1</AutoColor></AutoPlate></Vehicle><Entry><RoadID>49</RoadID><StationID>49</StationID><LaneID>12</LaneID><Time_lack>2019-01-04 11:40:21</Time_lack></Entry></TradeKeyItem></Data></ETCDebitListQuery>";
    ObuWhiteListQueryParameter query_para;
    ParseWhiteListQueryParaFromXML(lack_filed_para, query_para);

    //==========扣款参数测试=============
    correct_para = "<?xml version=\"1.0\" encoding=\"GB2312\"?><DebitParamContext><Version>1.0</Version><Data><OverTime>30000</OverTime><OperationMode>0</OperationMode><PayModel>0</PayModel><Debit><PayIdentifier/><Money>1</Money><SubTime>2019-01-04 14:07:59</SubTime><Type>1</Type><PayCertificateCode>0</PayCertificateCode><PayCertificateType>0</PayCertificateType></Debit><Vehicle><Type>1</Type><Class>1</Class><License>京A12345</License><VLColor>0</VLColor><AxisCount>0</AxisCount><Weight>0</Weight><LimitWeight>0</LimitWeight><AutoPlate><AutoLicense>AutoLicense</AutoLicense><AutoColor>1</AutoColor></AutoPlate></Vehicle><Operation><PassCertificateType>0</PassCertificateType><CardID>0</CardID><TollDate>20190104</TollDate><ShiftID>1</ShiftID><TicketNo>12</TicketNo><OperatorID>12</OperatorID><OperatorName>操作员</OperatorName><Distance>0</Distance></Operation><Entry><StationID>49</StationID><StationName>柳湾</StationName><Time>2019-01-04 13:57:59</Time><LaneID>1</LaneID><License>京A12345</License><VColor>0</VColor><VClass>0</VClass><VType>0</VType><RoadID>49</RoadID><AreaID>49</AreaID></Entry><OBU><License>京A12345</License><VColor>0</VColor><Class>0</Class><Type>0</Type><ID/><MAC/></OBU></Data></DebitParamContext>";
    lack_filed_para = "<?xml version=\"1.0\" encoding=\"GB2312\"?><DebitParamContext><Version>1.0</Version><Data><OverTime>30000</OverTime><OperationMode>0</OperationMode><PayModel>0</PayModel><Debit><PayIdentifier/><Money>1</Money><SubTime>2019-01-04 14:07:59</SubTime><Type>1</Type><PayCertificateCode>0</PayCertificateCode><PayCertificateType>0</PayCertificateType></Debit><Vehicle><Type>1</Type><Class>1</Class><License>京A12345</License><VLColor>0</VLColor><AxisCount>0</AxisCount><Weight>0</Weight><LimitWeight>0</LimitWeight><AutoPlate><AutoLicense>AutoLicense</AutoLicense><AutoColor>1</AutoColor></AutoPlate></Vehicle><Operation><PassCertificateType>0</PassCertificateType><CardID>0</CardID><TollDate>20190104</TollDate><ShiftID>1</ShiftID><TicketNo>12</TicketNo><OperatorID>12</OperatorID><OperatorName>操作员</OperatorName><Distance>0</Distance></Operation><Entry><StationID>49</StationID><StationName>柳湾</StationName><Time>2019-01-04 13:57:59</Time><LaneID>1</LaneID><License>京A12345</License><VColor>0</VColor><VClass>0</VClass><VType>0</VType><RoadID>49</RoadID><AreaID>49</AreaID></Entry><OBU><License_Lack>京A12345</License_Lack><VColor>0</VColor><Class>0</Class><Type>0</Type><ID/><MAC/></OBU></Data></DebitParamContext>";
    ETCDebitRecord debit_para;
    ParseETCDebitParaFromXML(lack_filed_para, debit_para);

    //===========扣款结果查询==============
    correct_para = "<?xml version=\"1.0\" encoding=\"GB2312\"?><ETCDebitResultQuery><Version>1.0</Version><Data><TradeKeyItem><PayIdentifier>1231</PayIdentifier><Vehicle><Class>1</Class><Type>1</Type><License>京A12345</License><VLColor>0</VLColor><AutoPlate><AutoLicense>AutoLicense</AutoLicense><AutoColor>1</AutoColor></AutoPlate></Vehicle></TradeKeyItem></Data></ETCDebitResultQuery>";
    lack_filed_para = "<?xml version=\"1.0\" encoding=\"GB2312\"?><ETCDebitResultQuery><Version>1.0</Version><Data><TradeKeyItem><PayIdentifier_Lack>1231</PayIdentifier_Lack><Vehicle><Class>1</Class><Type>1</Type><License>京A12345</License><VLColor>0</VLColor><AutoPlate><AutoLicense>AutoLicense</AutoLicense><AutoColor>1</AutoColor></AutoPlate></Vehicle></TradeKeyItem></Data></ETCDebitResultQuery>";
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
    //插入
    for (int i = 0; i < 10; ++i)
    {
        ETCDebitRecord debit_record;
        strcpy(debit_record.PayIdentifier, GetGlobalApp()->GetGuidString(1));
        debit_record.Money = rand();
        sprintf(hint_info, "插入数据，支付标识:%s,金额:%d", debit_record.PayIdentifier, debit_record.Money);
        GetGlobalApp()->RecordLog(hint_info);
        if (DataBaseAddDebitQuest(sql_handle, debit_record) != 0)
        {
            GetGlobalApp()->RecordLog("增加数据失败");
            GetGlobalApp()->RecordLog(DataBaseGetLastError(sql_handle).c_str());
        }
    }

    int loop_max = 100;
    //查询和删除
    while (true && (loop_max-- > 0) )
    {
        ETCDebitRecord debit_record;
        if (DataBaseQueryDebitQuest(sql_handle, debit_record) != 0)
        {
            break;
        }
        sprintf(hint_info, "查询到数据，支付标识:%s,金额:%d", debit_record.PayIdentifier, debit_record.Money);
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

    //插入
    std::list<std::string> record_payid_group;
    for (int i = 0; i < 10; ++i)
    {
        ETCDebitRecord debit_record;
        RandFillRecordArchive(debit_record);
        std::string hint_info = "插入数据->" + DebitRecordArchiveAbstract(debit_record);
        GetGlobalApp()->RecordLog(hint_info.c_str());
        record_payid_group.push_back(debit_record.PayIdentifier);
        if (DataBaseAddRecord(sql_handle, debit_record) != 0)
        {
            GetGlobalApp()->RecordLog("增加数据失败");
            GetGlobalApp()->RecordLog(DataBaseGetLastError(sql_handle).c_str());
        }
    }

    int loop_max = 100;
    //查询和删除
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
        std::string hint_info = "查询到数据->" + DebitRecordArchiveAbstract(debit_record);
        GetGlobalApp()->RecordLog(hint_info.c_str());

    }

    DataBaseClose(sql_handle);
}
void UnitTestRequestPara()
{
    //白名单查询报文
    std::string stand_para = "<?xml version=\"1.0\" encoding=\"GB2312\"?><ETCDebitListQuery><Version>1.0</Version><Data><TradeKeyItem><PayIdentifier/><Money>1</Money><OBUMAC>OBUMAC</OBUMAC><OBUID>OBUID</OBUID><CardID>2134566</CardID><Vehicle><Class>1</Class><Type>1</Type><License>京A12345</License><VLColor>0</VLColor><AutoPlate><AutoLicense>AutoLicense</AutoLicense><AutoColor>1</AutoColor></AutoPlate></Vehicle><Entry><RoadID>49</RoadID><StationID>49</StationID><LaneID>12</LaneID><Time>2019-01-05 10:34:59</Time></Entry></TradeKeyItem></Data></ETCDebitListQuery>";
    ObuWhiteListQueryParameter query_para;
    std::string request_json;
    ParseWhiteListQueryParaFromXML(stand_para, query_para);
    PackOBUWhiteListQueryJson(query_para, request_json);
    GetGlobalApp()->RecordLog("白名单传入参数");
    GetGlobalApp()->RecordLog(stand_para.c_str());
    GetGlobalApp()->RecordLog("白名单发送参数");
    GetGlobalApp()->RecordLog(request_json.c_str());

    //扣费报文
    stand_para = "<?xml version=\"1.0\" encoding=\"GB2312\"?><DebitParamContext><Version>1.0</Version><Data><OverTime>30000</OverTime><OperationMode>0</OperationMode><PayModel>0</PayModel><Debit><PayIdentifier>12313123123123213</PayIdentifier><Money>1</Money><SubTime>2019-01-05 11:05:24</SubTime><Type>1</Type><PayCertificateCode>0</PayCertificateCode><PayCertificateType>0</PayCertificateType></Debit><Vehicle><Type>1</Type><Class>1</Class><License>京A12345</License><VLColor>0</VLColor><AxisCount>0</AxisCount><Weight>0</Weight><LimitWeight>0</LimitWeight><AutoPlate><AutoLicense>AutoLicense</AutoLicense><AutoColor>1</AutoColor></AutoPlate></Vehicle><Operation><PassCertificateType>0</PassCertificateType><CardID>abcdabcd</CardID><TollDate>20190105</TollDate><ShiftID>1</ShiftID><TicketNo>12</TicketNo><OperatorID>12</OperatorID><OperatorName>操作员</OperatorName><Distance>0</Distance></Operation><Entry><StationID>49</StationID><StationName>柳湾</StationName><Time>2019-01-05 10:55:24</Time><LaneID>1</LaneID><License>京A12345</License><VColor>0</VColor><VClass>0</VClass><VType>0</VType><RoadID>49</RoadID><AreaID>49</AreaID></Entry><OBU><License>京A12345</License><VColor>2</VColor><Class>3</Class><Type>4</Type><ID>ab</ID><MAC>cd</MAC></OBU></Data></DebitParamContext>";
    ETCDebitRecord debit_para;
    ParseETCDebitParaFromXML(stand_para, debit_para);
    strcpy(debit_para.LocalResponse.PaySerialNo, RandStr().c_str());
    strcpy(debit_para.LocalResponse.TradeSecurityCode, RandStr().c_str());
    PackETCDebitRequestJson(debit_para, request_json);
    GetGlobalApp()->RecordLog("扣费传入参数");
    GetGlobalApp()->RecordLog(stand_para.c_str());
    GetGlobalApp()->RecordLog("扣费发送参数");
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
    TW_TRACE_DIARY_TYPE("测试车牌:" + std::string(parseResult.VehicleLicense), TW_TRACE_TYPE_INTERFACE);
    bool isin_whitelist = (stricmp(parseResult.VehicleLicense, "甘A12345") == 0);
    if ( isin_whitelist)
    {
        TW_TRACE_DIARY_TYPE("车牌属于白名单:" + std::string(parseResult.VehicleLicense), TW_TRACE_TYPE_INTERFACE);
        iSupportAgent = 0;
    }
    else
    {
        TW_TRACE_DIARY_TYPE("车牌不属于白名单:" + std::string(parseResult.VehicleLicense), TW_TRACE_TYPE_INTERFACE);
        iSupportAgent = 1;
    }

    return 0;
}

//名单查询，查询用户是否属于代扣用户
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
            TW_TRACE_DIARY_TYPE("XML参数解析失败", TW_TRACE_TYPE_INTERFACE);
            return 255;
        }
    }
    else if (iFormatType == 2)
    {
        if (!ParseWhiteListQueryParaFromJson(szParamContext, query_para))
        {
            TW_TRACE_DIARY_TYPE("Json参数解析失败", TW_TRACE_TYPE_INTERFACE);
            return 255;
        }
    }
    else
    {
        char err_log[100] = {0};
        sprintf(err_log, "无效的参数类型:%d", iFormatType);
        TW_TRACE_DIARY_TYPE(err_log, TW_TRACE_TYPE_INTERFACE);
        return 255;
    }
    //return QueryAgentDebitEtcUp_Stake(szParamContext,iParamSize,iFormatType,iSupportAgent,iTimeout,query_para);
    TW_TRACE_DIARY_TYPE("开始调用白名单查询", TW_TRACE_TYPE_INTERFACE);
    int query_ret = GetGlobalApp()->m_EtcPayCenter.StartWhiteListQuery(query_para);
    if (query_ret == 0)
    {
        TW_TRACE_DIARY_TYPE("发起白名单查询请求成功、等待结果回馈", TW_TRACE_TYPE_INTERFACE);
    }
    else if (query_ret == -1)
    {
        TW_TRACE_DIARY_TYPE("发起白名单查询请求失败，其他原因", TW_TRACE_TYPE_INTERFACE);
        return 255;
    }
    else
    {
        TW_TRACE_DIARY_TYPE("发起白名单查询请求失败，网络出现问题", TW_TRACE_TYPE_INTERFACE);
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
            sprintf(err_log, "等待查询结果超时,预定时间:%d毫秒,已使用时间:%d毫秒,回馈车道结果:1，白名单查询结果：失败", iTimeout, diffMs);
            TW_TRACE_DIARY_TYPE(err_log, TW_TRACE_TYPE_INTERFACE);
            return 1;
        }
        int state_ret = GetGlobalApp()->m_EtcPayCenter.IsInWhiteList(query_para);
        if (state_ret == -1)
        {
            continue;
        }
        char info_log[100] = {0};
        sprintf(info_log, "回馈车道结果:0,白名单查询结果:%d", state_ret);
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
    TW_TRACE_DIARY_TYPE("测试车牌号:" + std::string(parseResult.VehicleLicense), TW_TRACE_TYPE_INTERFACE);
    std::string stake_result ;
    if (stricmp(parseResult.VehicleLicense, "甘A12345") == 0)
    {
        TW_TRACE_DIARY_TYPE("存在该车牌的交易结果:" + std::string(parseResult.VehicleLicense), TW_TRACE_TYPE_INTERFACE);
        stake_result = "<?xml version=\"1.0\" encoding=\"GB2312\"?><DebitResultContext><Version>1.0</Version><Data><ResultType>0</ResultType><ExecuteCode>0</ExecuteCode><ExecuteDesc/><TradeKeyItem><TradeSecurityCode>6f59dc0795cc69c176fb1577a36878ca</TradeSecurityCode><PayPlatformType>1</PayPlatformType><PayIdentifier>A1A8A10E9AE947b29FEF2D97B6ABF2F61231</PayIdentifier><DebitOrder>8683782D385044b994F478EFE20E30E1</DebitOrder><DebitTime>2020-02-01 15:14:25</DebitTime><ConsumeTime>0</ConsumeTime></TradeKeyItem></Data></DebitResultContext>";
    }
    else
    {
        TW_TRACE_DIARY_TYPE("不存在该车牌的交易结果:" + std::string(parseResult.VehicleLicense), TW_TRACE_TYPE_INTERFACE);
        return 255;
    }

    int stake_result_size = static_cast<int>(stake_result.size());
    if (iResultSize < stake_result_size)
    {
        char err_log[100] = {0};
        sprintf(err_log, "解析查询结果失败,无法将结果保存到缓存，缓存大小:%d，结果大小:%d", iResultSize, stake_result_size);
        TW_TRACE_DIARY_TYPE(err_log, TW_TRACE_TYPE_INTERFACE);
        return 255;
    }
    TW_TRACE_DIARY_TYPE("返回查询结果:" + stake_result, TW_TRACE_TYPE_INTERFACE);
    strcpy(szResultContext, stake_result.c_str());
    iResultSize = stake_result_size;
    return 0;
}

//进行交易查询
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
            TW_TRACE_DIARY_TYPE("XML参数解析失败", TW_TRACE_TYPE_INTERFACE);
            return 255;
        }
    }
    else if (iFormatType == 2)
    {
        if (!ParseDebitStateQueryParaFromJson(szParamContext, query_para))
        {
            TW_TRACE_DIARY_TYPE("Json参数解析失败", TW_TRACE_TYPE_INTERFACE);
            return 255;
        }
    }
    else
    {
        char err_log[100] = {0};
        sprintf(err_log, "无效的参数类型:%d", iFormatType);
        TW_TRACE_DIARY_TYPE(err_log, TW_TRACE_TYPE_INTERFACE);
        return 255;
    }
    //return QueryAgentDebitResultEtcUp_Stake(szParamContext,iParamSize,iFormatType,szResultContext,iResultSize,iTimeout,query_para);

    int query_ret = GetGlobalApp()->m_EtcPayCenter.StartDebitStateQuery(query_para);
    if (query_ret == 0)
    {
        TW_TRACE_DIARY_TYPE("发起代扣结果查询请求成功、等待结果回馈", TW_TRACE_TYPE_INTERFACE);
    }
    else if (query_ret == -1)
    {
        TW_TRACE_DIARY_TYPE("发起代扣结果查询请求失败，其他原因", TW_TRACE_TYPE_INTERFACE);
        return 255;
    }
    else
    {
        TW_TRACE_DIARY_TYPE("发起代扣结果查询请求失败，网络出现问题", TW_TRACE_TYPE_INTERFACE);
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
            sprintf(err_log, "等待结果超时,预定时间:%d毫秒,已使用时间:%d毫秒", iTimeout, diffMs);
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
                TW_TRACE_DIARY_TYPE("解析查询结果失败", TW_TRACE_TYPE_INTERFACE);
                return 255;
            }
        }
        else
        {   //json
            if (PackETCDebitResultJson(debitState, debit_state_str) != 0)
            {
                TW_TRACE_DIARY_TYPE("解析查询结果失败", TW_TRACE_TYPE_INTERFACE);
                return 255;
            }
        }
        int result_size = static_cast<int>(debit_state_str.size());
        if ( result_size > iResultSize)
        {
            char err_log[200] = {0};
            sprintf(err_log, "解析查询结果失败,无法将结果保存到缓存，缓存大小:%d，结果大小:%d", iResultSize, result_size);
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
    TW_TRACE_DIARY_TYPE("测试车牌号:" + std::string(parseResult.VehicleLicense), TW_TRACE_TYPE_INTERFACE);
    if (stricmp(parseResult.VehicleLicense, "甘A12345") == 0)
    {
        stake_result = "<?xml version=\"1.0\" encoding=\"GB2312\"?><DebitResultContext><Version>1.0</Version><Data><ResultType>0</ResultType><ExecuteCode>0</ExecuteCode><ExecuteDesc/><TradeKeyItem><TradeSecurityCode>6f59dc0795cc69c176fb1577a36878ca</TradeSecurityCode><PayPlatformType>1</PayPlatformType><PayIdentifier>A1A8A10E9AE947b29FEF2D97B6ABF2F61231</PayIdentifier><DebitOrder>8683782D385044b994F478EFE20E30E1</DebitOrder><DebitTime>2020-02-01 15:14:25</DebitTime><ConsumeTime>0</ConsumeTime></TradeKeyItem></Data></DebitResultContext>";
        TW_TRACE_DIARY_TYPE("车牌属于白名单:" + std::string(parseResult.VehicleLicense), TW_TRACE_TYPE_INTERFACE);
    }
    else
    {
        TW_TRACE_DIARY_TYPE("车牌不属于白名单:" + std::string(parseResult.VehicleLicense), TW_TRACE_TYPE_INTERFACE);
        return 255;
    }

    int stake_result_size = static_cast<int>(stake_result.size());
    if (iResultSize < stake_result_size)
    {
        char err_log[100] = {0};
        sprintf(err_log, "解析查询结果失败,无法将结果保存到缓存，缓存大小:%d，结果大小:%d", iResultSize, stake_result_size);
        TW_TRACE_DIARY_TYPE(err_log, TW_TRACE_TYPE_INTERFACE);
        return 255;
    }
    TW_TRACE_DIARY_TYPE("返回扣款结果:" + stake_result, TW_TRACE_TYPE_INTERFACE);
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
            TW_TRACE_DIARY_TYPE("XML参数解析失败", TW_TRACE_TYPE_INTERFACE);
            return 255;
        }
    }
    else if (iFormatType == 2)
    {
        if (!ParseETCDebitParaFromJson(szParamContext, debit_para))
        {
            TW_TRACE_DIARY_TYPE("Json参数解析失败", TW_TRACE_TYPE_INTERFACE);
            return 255;
        }
    }
    else
    {
        char err_log[100] = {0};
        sprintf(err_log, "无效的参数类型:%d", iFormatType);
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
        sprintf(err_log, "扣款失败，错误码:%d", debit_ret);
        TW_TRACE_DIARY_TYPE(err_log, TW_TRACE_TYPE_INTERFACE);
        return debit_ret;
    }
    int result_size = static_cast<int>(result_str.size());
    if (result_size > iResultSize)
    {
        char err_log[200] = {0};
        sprintf(err_log, "解析查询结果失败,无法将结果保存到缓存，缓存大小:%d，结果大小:%d", iResultSize, result_size);
        TW_TRACE_DIARY_TYPE(err_log, TW_TRACE_TYPE_INTERFACE);
        return 255;
    }
    TW_TRACE_DIARY_TYPE("返回扣款结果:" + result_str, TW_TRACE_TYPE_INTERFACE);
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
////从更新库autoUpdate.cpp 中迁移过来-------20201130
//
//// 下面的内容，是为了兼容之前的版本
//
//class    CommonBus
//{
//
//public:
//	CommonBus(){}
//	virtual ~CommonBus(){}
//    //注册  key:代表组件在全局定义的唯一键值
//	virtual void register2Bus(int key){}
//    //注销  key:代表组件在全局定义的唯一键值
//	virtual void unRegister2Bus(int key){}
//    /*
//    发布
//    destKey:代表目标组件在全局定义的唯一键值
//    msg:需要发送的消息内容
//    len:需要发送的消息内容的长度
//    srcKey：本组件的全局定义的唯一键值。默认为0。
//*/
//	virtual int send2Bus(int destKey,char *msg,int len,int srcKey=0){return 1;}
//
//    /*
//        订阅
//        destKey:代表本组件的全局定义的唯一键值
//        msg:需要发送的消息内容
//        len:需要发送的消息内容的长度
//        srcKey：代表源组件在全局定义的唯一键值
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