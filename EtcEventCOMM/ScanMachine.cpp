// ScanMachine.cpp : 定义 DLL 的初始化例程。
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
//TODO: 如果此 DLL 相对于 MFC DLL 是动态链接的，
//		则从此 DLL 导出的任何调入
//		MFC 的函数必须将 AFX_MANAGE_STATE 宏添加到
//		该函数的最前面。
//
//		例如:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 此处为普通函数体
//		}
//
//		此宏先于任何 MFC 调用
//		出现在每个函数中十分重要。这意味着
//		它必须作为函数中的第一个语句
//		出现，甚至先于所有对象变量声明，
//		这是因为它们的构造函数可能生成 MFC
//		DLL 调用。
//
//		有关其他详细信息，
//		请参阅 MFC 技术说明 33 和 58。
//


// CScanMachineApp

void*  CScanMachineApp::m_pIndPtr = NULL;
bool CScanMachineApp::m_bIsDevInited = false;



BEGIN_MESSAGE_MAP(CScanMachineApp, CWinApp)
END_MESSAGE_MAP()


// CScanMachineApp 构造

CScanMachineApp::CScanMachineApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
	m_pThreadSMManager = NULL;
}


// 唯一的一个 CScanMachineApp 对象

CScanMachineApp theApp;

CScanMachineApp* GetGlobalApp() { return &theApp; }

void LogCallBack(const std::string& content)
{
	theApp.RecordMsgTemp(content.c_str(), TRUE);
}

// CScanMachineApp 初始化

BOOL CScanMachineApp::InitInstance()
{
	CWinApp::InitInstance();


	m_DeviceNo = "";
	m_QRcode = "";
	//MiniDump();

	//获取动态库自身路径，只能在动态库已经实例化后进行
	m_strAppDir.Format( _T("%s"), GetCurrentDirNew() );
	//配置日志文件
	m_strLogDirPath.Format(_T("%s\\TWSDNetPayLog"),		m_strAppDir);
	m_strLogFileName.Format(_T("HTS_%s.log"), CTime::GetCurrentTime().Format(_T("%Y%m%d")));
	m_strLogFilePath.Format( _T("%s\\%s"), m_strLogDirPath, m_strLogFileName);
	m_strConfigPathFile.Format( _T("%s\\%s"), m_strAppDir, FILE_NAME_DEVICE_SM_CONFIG);

	char tmp_str[200]={0};
	//是否记录日志标记
	m_iWriteLog	= ::GetPrivateProfileInt(_T("SMControl"),	_T("WriteLog"),	1,	m_strConfigPathFile);

	//串口参数
	theApp.m_SerialPort = ::GetPrivateProfileInt(_T("SerialParam"),	_T("Port"),	1,	m_strConfigPathFile);
	theApp.m_BaudRate   = ::GetPrivateProfileInt(_T("SerialParam"),	_T("BaudRate"),	57600,	m_strConfigPathFile);
	theApp.m_InBufferSize = ::GetPrivateProfileInt(_T("SerialParam"),	_T("InBufferSize"),	2048, m_strConfigPathFile);
	theApp.m_OutBufferSize = ::GetPrivateProfileInt(_T("SerialParam"),	_T("OutBufferSize"), 2048, m_strConfigPathFile);
	//日志文件最大值
	m_iLogFileMaxSize = ::GetPrivateProfileInt(_T("SMControl"),	_T("LogMaxSize"), FILE_SIZE_LOG_MAX, m_strConfigPathFile);

	//theApp.CreateDir(m_strLogDirPath);

	theApp.RecordMsgTemp(_T("/**********************************串口通信动态库 -- 程序启动*******************************************/"));
	CString configinfo;
	configinfo.Format( _T("配置串口号:%d, 波特率:%d"), theApp.m_SerialPort, theApp.m_BaudRate);
	theApp.RecordMsgTemp(configinfo);

	return TRUE;
}

int CScanMachineApp::ExitInstance()
{
	RecordMsgTemp(_T("设备管理线程，线程退出..."));
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

	RecordMsgTemp(_T("设备管理线程，线程退出成功..."));

	return CWinApp::ExitInstance();
}


/*********************************常用函数*************************************************************/

////////////////////////////////////////////////////////////////////////////////////////////////
// 函数名称：CString	FmtStr
// 功能描述：
// 输入参数：const char * lpzFmt
//         ：...
// 编写人员：朱道明
// 编写时间：星期二 2009年9月15日
// 修改人员：
// 修改时间：
// 函数版本：1.0.0.0
// 备注说明：
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
////函数名称:fnHexToBin
////函数功能:将16进制的字符串转换成2进制的数字
////输入参数:IN PBYTE	pHex		-- 需要转换的16进制字符串
////		 OUT PBYTE	pbin		-- 输出的2进制数字数组
////		 IN DWORD	dwHex_len	-- 需要转换的16进制数据长度,必须为2的倍数
////输出参数:无
////编写时间:2020-10-18
////编写人员:朱道明
////修改时间:
////修改人员:
////函数版本:1.0.0.1
////备注说明:所输入的16进制字符串长度必须是2的倍数
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
////函数名称:BinToHex
////函数功能:将2进制的数字转换成16进制的字符串
////输入参数:IN PBYTE	pBin		-- 需要转换的2进制数字数组
////		 IN DWORD	dwHex_len	-- 需要转换的2进制数字数组长度
////输出参数:CString	非空 -- 转换成功后输出的16进制字符串
////		 CString	NULL -- 转换失败
////编写时间:2020-10-18
////编写人员:朱道明
////修改时间:
////修改人员:
////函数版本:1.0.0.0
////备注说明:如果调用成功后,不需要调用者释放内存
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
//	作	  者 : 朱道明
//	时	  间 : 2020-10-18
//  版	  本 : 1.0.0.0
//	函数名称 : CRC16
//	函数功能 : 根据给定的数据，返回其CRC校验码
//	输入参数 : LPBYTE	--	lpData	需要进行校验的字节数据流
//			   WORD		--	wLength	需要进行校验的字节长度
//	输出参数 : WORD		--	返回的CRC校验码，双字节
//  备注说明 : 当给定的校验参数指针为空，则返回 0
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
			CRC16Hi = CRC16Hi / 2; //高位右移一位
			CRC16Lo = CRC16Lo / 2; //低位右移一位
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
// 函数名称：CScanDevApp::GetCurrentDirNew
// 功能描述：获取当前程序运行的所在目录（由调用者决定）
// 输入参数：无
// 输出参数：CString	--	获取到的程序运行目录
// 编写人员：朱道明
// 编写时间：2020年10月18日
// 修改人员：
// 修改时间：
// 函数版本：1.0.0.0
// 备注说明：
////////////////////////////////////////////////////////////////////////////////////////////////
CString CScanMachineApp::GetCurrentDirNew()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	TCHAR szfile[255] = _T("");
	CString csDir = _T("");

	//对于应用程序（调用者），此方式正确。对于动态库（被调用者），此方式获取的是程序路径
	//	DWORD dwLen = GetModuleFileName(NULL, szfile, 255);

	//调整为动态库自身的路径
	HINSTANCE hInsApp;
	hInsApp = AfxGetInstanceHandle();
	DWORD dwLen = GetModuleFileName(hInsApp, szfile, 255);

	csDir.Format( _T("%s"), szfile);
	csDir = csDir.Left( csDir.ReverseFind(_T('\\')) );
	return csDir;
}



////////////////////////////////////////////////////////////////////////////////////////////////
// 函数名称：CScanDevApp::RecordMsgTemp
// 功能描述：记录信息到临时文件（日志文件）
// 输入参数：IN LPCTSTR strMsg	--	需要记录的信息串
// 输出参数：void
// 编写人员：朱道明
// 编写时间：2020年10月18日
// 修改人员：
// 修改时间：
// 函数版本：1.0.0.0
// 备注说明：
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
	//获得时间
	SYSTEMTIME sysGetTime = {0};
	::GetLocalTime(&sysGetTime);
	CTime m_curtime = CTime::GetCurrentTime();
	CString sTemp = _T("");
	CString strCurrentDateFile;
	strCurrentDateFile.Format(_T("HTS%s.log"), CTime::GetCurrentTime().Format(_T("%Y%m%d")));
	//日志文件名不一致
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
		//大于1K才进行保存
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
//函数名:	OleTimeToTime
//功  能:	将 DATE格式时间转化为 time_t格式时间
//参  数:	DATE dTime		待转化的 DATE型格式时间
//
//返回值:	转化后的 time_t型格式时间
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
//函数名:	CString2time_t
//功  能:	将字符串型格式时间转化为 time_t型格式时间
//参  数:	LPCTSTR lpztime		待转化的字符串型格式时间
//
//返回值:	转化后的 time_t型格式时间
//////////////////////////////////////////////////////////////////////////////////////////////////////
time_t CScanMachineApp::CString2time_t(LPCTSTR lpztime)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString csTemp = _T("");
	COleDateTime oleGetTime;
	csTemp.Format( _T("%s"), lpztime);
	if ( csTemp.GetLength() > 0 )
	{
		if ( csTemp.GetLength() > 19 ) //兼容类似格式 _T("2014-11-23 12:34:56.000");
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
// 函数名称：CThreadManage::CheckAndDeleteLogFile
// 功能描述：检索并删除日志
// 输入参数：void
// 输出参数：void
// 编写人员：朱道明
// 编写时间：2020年10月18日
// 修改人员：
// 修改时间：
// 函数版本：1.0.0.0
// 备注说明：
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
				//获取该文件的时间与现在时间的相差天数
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
		//清除数据
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




/**********************************ETC特情接口************************************************************/
bool WINAPI  CScanMachineApp::IF_InitDev(IN const UINT& dwThreadNotifyCallerID, IN const UINT& dwThreadDevManagerID, IN const HWND& hWnd, IN const UINT& dwEtcRespMsgID, IN const UINT& dwDevNotifyMsgID, IN OUT void* pIndPtr)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	char log[128] = {0};
	_snprintf_s(log, 128, "设备初始化接口(IF_InitDev)调用开始,调用者线程ID:%u,消息通知ID:%u", dwThreadNotifyCallerID, dwEtcRespMsgID);
	theApp.RecordMsgTemp(log);

    CString strLog;
    try
    {
        /*******************检索参数是否正常*******************/
        if ((0 == dwThreadNotifyCallerID) && (NULL == hWnd))
            throw -1;
        if ((NULL == pIndPtr))
		{
			 throw -2;
		}
        //回调函数指针赋值，便于通知调用者
		m_pIndPtr = pIndPtr;
		
		theApp.m_ThreadNotifyCallerID = dwThreadNotifyCallerID;
		theApp.m_EtcRespMsgID = dwEtcRespMsgID;

		theApp.m_ThreadDevManagerID = dwThreadDevManagerID;
		theApp.m_DevNotifyMsgID = dwDevNotifyMsgID;

		//启动串口
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
            strLog.Format(_T("设备初始化接口(IF_InitDev),传入参数检测有误，传入线程ID与窗体句柄均为0"));
            break;
        case -2:
            strLog.Format(_T("设备初始化接口(IF_InitDev),传入参数检测有误，传入回调指针错误"));
            break;
        case -3:
            strLog.Format(_T("设备初始化接口(IF_InitDev),设备初始化失败(重复初始化设备)"));
            break;
        case -4:
            strLog.Format(_T("设备初始化接口(IF_InitDev),创建串口管理线程资源失败"));
            break;
        case -5:
            strLog.Format(_T("设备初始化接口(IF_InitDev),串口打开失败"));
            break;
        }
        //记录日志
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
	theApp.RecordMsgTemp(_T("卸载设备接口(IF_DeInitDev)调用开始"));
	if (NULL != theApp.m_pThreadSMManager )
	{
		theApp.RecordMsgTemp(_T("EtcEvent串口通信库，关闭设备串口..."));
		theApp.m_pThreadSMManager->CloseCom();
	
		theApp.RecordMsgTemp(_T("EtcEvent串口通信库，退出设备管理线程..."));
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

//获取上一次错误信息
const char* WINAPI CScanMachineApp::IF_GetLastErrorDesc(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return false;
}

//获取组件状态
bool WINAPI CScanMachineApp::IF_GetDevStatus(IN OUT unsigned int& uStatus)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	bool bRet = false;
	int StatusTmp = 0;
	if(theApp.m_pThreadSMManager != NULL)
	{
		//串口打开状态，0：正常，2: 串口打开异常
		StatusTmp = (theApp.m_pThreadSMManager->IsConnectionOK() ? 0 : 2);
		//串口通讯状态，0：正常，4: 串口通讯异常
		StatusTmp += theApp.m_pThreadSMManager->m_DevStatus; 
		uStatus = StatusTmp;
		bRet = true;
	}

	return bRet;
}

//设置设备初始化数据
bool WINAPI CScanMachineApp::IF_SetDevInitData(IN const char* szParamContext, IN const int& iParamSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	char log[1024] = {0};
	_snprintf_s(log, 1024, "ETC特情-设置初始化数据接口(IF_SetDevInitData)调用开始,参数:%s", szParamContext);
	theApp.RecordMsgTemp(log);

	if(szParamContext == NULL || iParamSize == 0)
	{
		return false;
	}

	theApp.m_pA5InitJsonData = szParamContext;
	return true;
}


//特情处理开始
bool WINAPI CScanMachineApp::IF_EtcEventDealStart(IN const char* szParamContext, IN const int& iParamSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	bool bRet = false;
	char log[800] = {0};
	_snprintf_s(log, 800, "ETC特情-特情开始接口(IF_EtcEventDealStart)调用开始,参数:%s", szParamContext);
	theApp.RecordMsgTemp(log);

    if(theApp.m_pThreadSMManager != NULL)
	{
		bRet = theApp.m_pThreadSMManager->EventDealStart(szParamContext, iParamSize);
	}

	return bRet;
}

//车辆队列确认
bool WINAPI CScanMachineApp::IF_CheckVehQueue(IN const char* szParamContext, IN const int& iParamSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	bool bRet = false;
	char log[800] = {0};
	_snprintf_s(log, 800, "ETC特情-车辆队列确认事件接口(IF_EventCheckVehQueue)调用开始,参数:%s", szParamContext);
	theApp.RecordMsgTemp(log);

	if(theApp.m_pThreadSMManager != NULL)
	{
		bRet = theApp.m_pThreadSMManager->EventCheckVehQueue(szParamContext, iParamSize);
	}
	return bRet;
}

//车辆信息确认
bool WINAPI CScanMachineApp::IF_CheckVehInfo(IN const char* szParamContext,IN const int& iParamSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	bool bRet = false;
	char log[800] = {0};
	_snprintf_s(log, 800, "ETC特情-车辆信息确认事件接口(IF_CheckVehInfo)调用开始,参数:%s", szParamContext);
	theApp.RecordMsgTemp(log);

	if(theApp.m_pThreadSMManager != NULL)
	{
		bRet = theApp.m_pThreadSMManager->EventCheckVehInfo(szParamContext, iParamSize);
	}

	return bRet;
}

//入口信息确认
bool WINAPI CScanMachineApp::IF_CheckEntryInfo(IN const char* szParamContext,IN const int& iParamSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	bool bRet = false;
	char log[1024] = {0};
	_snprintf_s(log, 1024, "ETC特情-入口信息确认事件接口(IF_CheckEntryInfo)调用开始,参数:%s", szParamContext);
	theApp.RecordMsgTemp(log);

	if(theApp.m_pThreadSMManager != NULL)
	{
		bRet = theApp.m_pThreadSMManager->EventCheckEntryInfo(szParamContext, iParamSize);
	}

	return bRet;
}

//费额显示
bool WINAPI CScanMachineApp::IF_ShowFeeInfo(IN const char* szParamContext,IN const int& iParamSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	bool bRet = false;
	char log[2048] = {0};
	_snprintf_s(log, 2048, "ETC特情-费额信息显示接口(IF_ShowFeeInfo)调用开始,参数:%s", szParamContext);
	theApp.RecordMsgTemp(log);

	if(theApp.m_pThreadSMManager != NULL)
	{
		bRet = theApp.m_pThreadSMManager->EventShowFeeInfo(szParamContext, iParamSize);
	}

	return bRet;
}

//支付状态显示
bool WINAPI CScanMachineApp::IF_PayResultDisplay(IN const char* szParamContext,IN const int& iParamSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	bool bRet = false;
	char log[800] = {0};
	_snprintf_s(log, 800, "ETC特情-支付结果展示接口(IF_PayResultDisplay)调用开始,参数:%s", szParamContext);
	theApp.RecordMsgTemp(log);

	if(theApp.m_pThreadSMManager != NULL)
	{
		bRet = theApp.m_pThreadSMManager->EventPayResultShow(szParamContext, iParamSize);
	}

	return bRet;
}

//特情处理结束
bool WINAPI CScanMachineApp::IF_EtcEventDealStop(IN const char* szParamContext,IN const int& iParamSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	bool bRet = false;
	char log[1024] = {0};
	_snprintf_s(log, 1024, "特情处理结束接口(IF_EtcEventDealStop)调用开始,参数:%s", szParamContext);
	theApp.RecordMsgTemp(log);

	if(theApp.m_pThreadSMManager != NULL)
	{
		bRet = theApp.m_pThreadSMManager->EventDealStop(szParamContext, iParamSize);
	}

	return bRet;
}

//启动扫码
bool WINAPI CScanMachineApp::IF_StartScan(IN const int& iFeeValue)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	bool bRet = false;
	char log[800] = {0};
	_snprintf_s(log, 800, "启动扫码接口(IF_StartScan)调用开始,参数:%d", iFeeValue);
	theApp.RecordMsgTemp(log);

	if(theApp.m_pThreadSMManager != NULL)
	{
		bRet = theApp.m_pThreadSMManager->EventStartScan();
	}

	return bRet;
}

//停止扫码
bool WINAPI CScanMachineApp::IF_StopScan()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	bool bRet = false;
	char log[128] = {0};
	_snprintf_s(log, 128, "停止扫码接口(IF_StopScan)调用开始.");
	theApp.RecordMsgTemp(log);

	if(theApp.m_pThreadSMManager != NULL)
	{
		bRet = theApp.m_pThreadSMManager->EventStopScan();
	}
	return false;
}

//ETC卡操作提示
bool WINAPI CScanMachineApp::IF_CardOperationNotify(IN const char* szParamContext, IN const int& iParamSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	bool bRet = false;
	char log[512] = {0};
	_snprintf_s(log, 512, "卡操作提示接口(IF_CardOperationNotify)调用开始,参数:%s", szParamContext);
	theApp.RecordMsgTemp(log);

	if(theApp.m_pThreadSMManager != NULL)
	{
		bRet = theApp.m_pThreadSMManager->EventCardOperationNotify(szParamContext, iParamSize);
	}

	return bRet;
}

//车辆队列修改
bool WINAPI CScanMachineApp::IF_ModifyVehQueue(IN const char* szParamContext, IN const int& iParamSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	bool bRet = false;
	char log[2048] = {0};
	_snprintf_s(log, 2048, "车辆队列修正接口(IF_ModifyVehQueue)调用开始,参数:%s", szParamContext);
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
	_snprintf_s(log, 128, "获取数据接口(IF_GetData)调用开始");
	theApp.RecordMsgTemp(log);
	
	strData = theApp.m_QRcode;
	iDataType = MAKELONG(0x01, 0x01);;
	iSrcType = 1;
	bRet = true;

	return bRet;
}

//最小费额授权
bool WINAPI CScanMachineApp::IF_FeeAuthorize(IN const char* szParamContext, IN const int& iParamSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	bool bRet = false;
	char log[512] = {0};
	_snprintf_s(log, 512, "最小费额授权接口(IF_FeeAuthorize)调用开始,参数:%s", szParamContext);
	theApp.RecordMsgTemp(log);

	if(theApp.m_pThreadSMManager != NULL)
	{
		bRet = theApp.m_pThreadSMManager->EventFeeAuthorize(szParamContext, iParamSize);
	}

	return bRet;
}

//其他授权
bool WINAPI CScanMachineApp::IF_EventAuthorize(IN const char* szParamContext, IN const int& iParamSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	bool bRet = false;
	char log[512] = {0};
	_snprintf_s(log, 512, "其他授权接口(IF_EventAuthorize)调用开始,参数:%s", szParamContext);
	theApp.RecordMsgTemp(log);

	if(theApp.m_pThreadSMManager != NULL)
	{
		bRet = theApp.m_pThreadSMManager->EventAuthorize(szParamContext, iParamSize);
	}

	return bRet;
}

//删除车辆队列结果
bool WINAPI CScanMachineApp::IF_DelVehQueueResult(IN const char* szParamContext, IN const int& iParamSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	bool bRet = false;
	char log[512] = {0};
	_snprintf_s(log, 512, "删除车辆队列结果接口(IF_DelVehQueueResult)调用开始,参数:%s", szParamContext);
	theApp.RecordMsgTemp(log);

	if(theApp.m_pThreadSMManager != NULL)
	{
		bRet = theApp.m_pThreadSMManager->EventDelVehQueueResult(szParamContext, iParamSize);
	}

	return bRet;
}



//二维码显示
bool WINAPI CScanMachineApp::IF_Tr500QRcodeDisplay(IN const char* strJsonParam, IN const int& iJsonParamSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	bool bRet = false;
	char log[1024] = {0};
	_snprintf_s(log, 1024, "二维码显示接口(IF_QRcodeDisplay)调用开始,参数:%s", strJsonParam);
	theApp.RecordMsgTemp(log);

	if(theApp.m_pThreadSMManager != NULL)
	{
		bRet = theApp.m_pThreadSMManager->QRcodeDisplay(strJsonParam, iJsonParamSize);
	}

	return bRet;
}

//费额显示
bool WINAPI CScanMachineApp::IF_Tr500FeeDisplay(IN const char* strJsonParam, IN const int& iJsonParamSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	bool bRet = false;
	char log[512] = {0};
	_snprintf_s(log, 512, "费额显示接口(IF_FeeDisplay)调用开始,参数:%s", strJsonParam);
	theApp.RecordMsgTemp(log);

	if(theApp.m_pThreadSMManager != NULL)
	{
		bRet = theApp.m_pThreadSMManager->FeeDisplay(strJsonParam, iJsonParamSize);
	}

	return bRet;
}

//支付结果显示
bool WINAPI CScanMachineApp::IF_Tr500PayResultDisplay(IN const char* strJsonParam, IN const int& iJsonParamSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	bool bRet = false;
	char log[512] = {0};
	_snprintf_s(log, 512, "支付结果显示接口(IF_PayResultDisplay)调用开始,参数:%s", strJsonParam);
	theApp.RecordMsgTemp(log);

	if(theApp.m_pThreadSMManager != NULL)
	{
		bRet = theApp.m_pThreadSMManager->PayResultDisplay(strJsonParam, iJsonParamSize);
	}

	return bRet;
}

//清屏
bool WINAPI CScanMachineApp::IF_Tr500ClearScreen(IN const char* strJsonParam, IN const int& iJsonParamSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	bool bRet = false;
	char log[256] = {0};
	_snprintf_s(log, 256, "清屏接口(IF_ClearScreen)调用开始,参数:%s", strJsonParam);
	theApp.RecordMsgTemp(log);

	if(theApp.m_pThreadSMManager != NULL)
	{
		bRet = theApp.m_pThreadSMManager->ClearScreen(strJsonParam, iJsonParamSize);
	}

	return bRet;
}
