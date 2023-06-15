#include "StdAfx.h"
#include "HTSDDeviceDll.h"



CHTSDDeviceDll::CHTSDDeviceDll(void)
	: m_hModule(NULL)
	, m_bInitDll(FALSE)
	, m_bInitDev(FALSE)
	, m_strLastError(_T(""))
	/*
	, fnIF_InitDev(NULL)
	, fnIF_Destroy(NULL)
	, fnIF_GetLastErrorDesc(NULL)
	, fnIF_StartGetData(NULL)
	, fnIF_StopGetData(NULL)
	, fnIF_CheckDeviceStatus(NULL)
	, fnIF_GetData(NULL)
	, fnIF_Authority(NULL)
	, fnIF_GetDeviceFirewareVer(NULL)
	, fnIF_GetDisplayContext(NULL)
	, fnIF_SetDisplayContext(NULL)
	, fnIF_IssuedData(NULL)
	, fnIF_SetIOState(NULL)
	, fnIF_UpgradeFirware(NULL)
	, fnIF_IsUpgrading(NULL)
	, fnIF_RealFirmwareVer(NULL)
	, fnIF_RealSlaveFirmwareVer(NULL)
	, fnIF_QueryLEDVersion(NULL)
	,fnIF_UserDefinedData(NULL)*/
	,fnIF_InitDev(NULL)
	,fnIF_DeInitDev(NULL)
	,fnIF_GetLastErrorDesc(NULL)
	,fnIF_GetDevStatus(NULL)
	,fnIF_EtcEventDealStart(NULL)
	,fnIF_CheckVehQueue(NULL)
	,fnIF_CheckVehInfo(NULL)
	,fnIF_CheckEntryInfo(NULL)
	,fnIF_ShowFeeInfo(NULL)
	,fnIF_PayResultDisplay(NULL)
	,fnIF_EtcEventDealStop(NULL)
	,fnIF_StartScan(NULL)
	,fnIF_StopScan(NULL)
	,fnIF_CardOperationNotify(NULL)
	,fnIF_ModifyVehQueue(NULL)
	,fnIF_GetData(NULL)
	,fnIF_FeeAuthorize(NULL)
	,fnIF_FreeVoicePlay(NULL)
	,fnIF_EventAuthorize(NULL)
	,fnIF_Tr500ClearScreen(NULL)
	,fnIF_Tr500QRcodeDisplay(NULL)
	,fnIF_Tr500FeeDisplay(NULL)
	,fnIF_Tr500PayResultDisplay(NULL)
{
}

CHTSDDeviceDll::~CHTSDDeviceDll(void)
{
	this->Destroy();
}
////////////////////////////////////////////////////////////////////////////////////////////////
// 函数名称：CAgencyPay::Init
//
// 功能描述：初始化动态库
//
// 输入参数：IN const CString& strDllFilePath
// 输出参数：const
// 编写人员：ROCY
// 编写时间：2016年7月23日
// 修改人员：
// 修改时间：
// 函数版本：1.0.0.1
// 备注说明：
//
////////////////////////////////////////////////////////////////////////////////////////////////
const BOOL CHTSDDeviceDll::Init(IN const CString& strDllFilePath)
{
	//获取动态库文件所在路径目录
	CString strDir = strDllFilePath.Left(strDllFilePath.ReverseFind('\\'));
	//获取当前工作目录
	TCHAR szOldDir[260] = {0};
	::GetCurrentDirectory(260, szOldDir);
	//设置工作目录
	//remove 2018-04 ::SetCurrentDirectory(strDir);
	//加载动态库
	if (NULL != (m_hModule = ::LoadLibrary(strDllFilePath)))
	{
		/*
		fnIF_InitDev				= (defIF_InitDev)::GetProcAddress(m_hModule, "IF_InitDev");
		fnIF_Destroy				= (defIF_Destroy)::GetProcAddress(m_hModule, "IF_Destroy");
		fnIF_GetLastErrorDesc		= (defIF_GetLastErrorDesc)::GetProcAddress(m_hModule, "IF_GetLastErrorDesc");
		fnIF_StartGetData			= (defIF_StartGetData)::GetProcAddress(m_hModule, "IF_StartGetData");
		fnIF_StopGetData			= (defIF_StopGetData)::GetProcAddress(m_hModule, "IF_StopGetData");
		fnIF_CheckDeviceStatus		= (defIF_CheckDeviceStatus)::GetProcAddress(m_hModule, "IF_CheckDeviceStatus");
		fnIF_GetData				= (defIF_GetData)::GetProcAddress(m_hModule, "IF_GetData");
		fnIF_Authority				= (defIF_Authority)::GetProcAddress(m_hModule, "IF_Authority");
		fnIF_GetDeviceFirewareVer	= (defIF_GetDeviceFirewareVer)::GetProcAddress(m_hModule, "IF_GetDeviceFirewareVer");
		fnIF_GetDisplayContext		= (defIF_GetDisplayContext)::GetProcAddress(m_hModule, "IF_GetDisplayContext");
		fnIF_SetDisplayContext		= (defIF_SetDisplayContext)::GetProcAddress(m_hModule, "IF_SetDisplayContext");
		fnIF_IssuedData				= (defIF_IssuedData)::GetProcAddress(m_hModule, "IF_IssuedData");
		fnIF_SetIOState				= (defIF_SetIOState)::GetProcAddress(m_hModule, "IF_SetIOState");

		//add 2017-12
		fnIF_UpgradeFirware         = (defIF_UpgradeFirware)::GetProcAddress(m_hModule, "IF_UpgradeFirware");
		fnIF_IsUpgrading            = (defIF_IsUpgrading)::GetProcAddress(m_hModule, "IF_IsUpgrading");
		fnIF_RealFirmwareVer            = (defIF_RealFirmwareVer)::GetProcAddress(m_hModule, "IF_RealDeviceFirmwareVer");
		fnIF_RealSlaveFirmwareVer            = (defIF_RealSlaveFirmwareVer)::GetProcAddress(m_hModule, "IF_RealSlaveDeviceFirmwareVer");

		fnIF_QueryLEDVersion            = (defIF_QueryLEDVersion)::GetProcAddress(m_hModule, "IF_QueryLEDVersion");
		fnIF_UserDefinedData         = (defIF_UserDefinedData)::GetProcAddress(m_hModule, "IF_UserDefinedData");;
		if (		(NULL != fnIF_InitDev)
		            && (NULL != fnIF_Destroy)
		            && (NULL != fnIF_GetLastErrorDesc)
		            && (NULL != fnIF_StartGetData)
		            && (NULL != fnIF_StopGetData)
		            && (NULL != fnIF_CheckDeviceStatus)
		            && (NULL != fnIF_GetData)
		            && (NULL != fnIF_Authority)
		            && (NULL != fnIF_GetDeviceFirewareVer)
		            && (NULL != fnIF_GetDisplayContext)
		            && (NULL != fnIF_SetDisplayContext)
		            && (NULL != fnIF_IssuedData)
		            && (NULL != fnIF_SetIOState)
		   )*/
		fnIF_InitDev             = (defIF_InitDev)::GetProcAddress(m_hModule, "IF_InitDev");
		fnIF_SetDevInitData		 = (defIF_SetDevInitData)::GetProcAddress(m_hModule, "IF_SetDevInitData");
		fnIF_DeInitDev			 = (defIF_DeInitDev)::GetProcAddress(m_hModule, "IF_DeInitDev");
		fnIF_GetLastErrorDesc    = (defIF_GetLastErrorDesc)::GetProcAddress(m_hModule, "IF_GetLastErrorDesc");
		fnIF_GetDevStatus        = (defIF_GetDevStatus)::GetProcAddress(m_hModule, "IF_GetDevStatus");
		fnIF_EtcEventDealStart   = (defIF_EtcEventDealStart)::GetProcAddress(m_hModule, "IF_EtcEventDealStart");
		fnIF_CheckVehQueue       = (defIF_CheckVehQueue)::GetProcAddress(m_hModule, "IF_CheckVehQueue");
		fnIF_CheckVehInfo        = (defIF_CheckVehInfo)::GetProcAddress(m_hModule, "IF_CheckVehInfo");
		fnIF_CheckEntryInfo      = (defIF_CheckEntryInfo)::GetProcAddress(m_hModule, "IF_CheckEntryInfo");
		fnIF_ShowFeeInfo         = (defIF_ShowFeeInfo)::GetProcAddress(m_hModule, "IF_ShowFeeInfo");
		fnIF_PayResultDisplay    = (defIF_PayResultDisplay)::GetProcAddress(m_hModule, "IF_PayResultDisplay");
		fnIF_EtcEventDealStop    = (defIF_EtcEventDealStop)::GetProcAddress(m_hModule, "IF_EtcEventDealStop");
		fnIF_StartScan           = (defIF_StartScan)::GetProcAddress(m_hModule, "IF_StartScan");
		fnIF_StopScan            = (defIF_StopScan)::GetProcAddress(m_hModule, "IF_StopScan");
		fnIF_CardOperationNotify = (defIF_CardOperationNotify)::GetProcAddress(m_hModule, "IF_CardOperationNotify");
		fnIF_ModifyVehQueue      = (defIF_ModifyVehQueue)::GetProcAddress(m_hModule, "IF_ModifyVehQueue");
		fnIF_GetData			 = (defIF_GetData)::GetProcAddress(m_hModule, "IF_GetData");
		fnIF_FeeAuthorize		 = (defIF_FeeAuthorize)::GetProcAddress(m_hModule, "IF_FeeAuthorize");
		fnIF_FreeVoicePlay		 = (defIF_FreeVoicePlay)::GetProcAddress(m_hModule, "IF_FreeVoicePlay");
		fnIF_EventAuthorize		 = (defIF_EventAuthorize)::GetProcAddress(m_hModule, "IF_EventAuthorize");
		fnIF_DelVehQueueResult   = (defIF_DelVehQueueResult)::GetProcAddress(m_hModule, "IF_DelVehQueueResult");
		fnIF_Tr500QRcodeDisplay	 = (defIF_Tr500QRcodeDisplay)::GetProcAddress(m_hModule, "IF_Tr500QRcodeDisplay");
		fnIF_Tr500FeeDisplay	 = (defIF_Tr500FeeDisplay)::GetProcAddress(m_hModule, "IF_Tr500FeeDisplay");
		fnIF_Tr500PayResultDisplay   = (defIF_Tr500PayResultDisplay)::GetProcAddress(m_hModule, "IF_Tr500PayResultDisplay");
		fnIF_Tr500ClearScreen		 = (defIF_Tr500ClearScreen)::GetProcAddress(m_hModule, "IF_Tr500ClearScreen");
		if (		(NULL != fnIF_InitDev)
					&& (NULL != fnIF_SetDevInitData)
		            && (NULL != fnIF_DeInitDev)
		            && (NULL != fnIF_GetLastErrorDesc)
		            && (NULL != fnIF_GetDevStatus)
		            && (NULL != fnIF_EtcEventDealStart)
		            && (NULL != fnIF_CheckVehQueue)
		            && (NULL != fnIF_CheckVehInfo)
		            && (NULL != fnIF_CheckEntryInfo)
		            && (NULL != fnIF_ShowFeeInfo)
		            && (NULL != fnIF_PayResultDisplay)
		            && (NULL != fnIF_EtcEventDealStop)
		            && (NULL != fnIF_StartScan)
		            && (NULL != fnIF_StopScan) 
					&& (NULL != fnIF_CardOperationNotify)
		            && (NULL != fnIF_ModifyVehQueue)
					&& (NULL != fnIF_GetData)
					&& (NULL != fnIF_FeeAuthorize)
					&& (NULL != fnIF_EventAuthorize)
					&& (NULL != fnIF_DelVehQueueResult)
					&& (NULL != fnIF_Tr500QRcodeDisplay)
					&& (NULL != fnIF_Tr500FeeDisplay)
					&& (NULL != fnIF_Tr500PayResultDisplay)
					&& (NULL != fnIF_Tr500ClearScreen)
					)

		{
			m_bInitDll = TRUE;
		}
	
	}
	//回复工作目录
	//remove 2018-04 ::SetCurrentDirectory(szOldDir);
	//返回执行状态



	return m_bInitDll;
}



////////////////////////////////////////////////////////////////////////////////////////////////
// 函数名称：CHTSDDeviceDll::InitDev
//
// 功能描述：组件初始化
//
// 输入参数：IN const UINT& nThreadID
//         ：IN const HWND& hWnd
//         ：IN const unsigned int& nNotifyMsgID
//         ：IN const int& iProvinceID
//         ：IN const int& iOverTime
// 输出参数：BOOL
// 编写人员：ROCY
// 编写时间：2016年7月24日
// 修改人员：
// 修改时间：
// 函数版本：1.0.0.1
// 备注说明：
//
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CHTSDDeviceDll::InitDev(IN const UINT& nThreadID, IN const HWND& hWnd, IN const unsigned int& nNotifyMsgID, IN const int& iProvinceID, IN const int& iOverTime)
{
	if (m_bInitDll)
	{
		//m_bInitDev = fnIF_InitDev(nThreadID, hWnd, nNotifyMsgID, iProvinceID, iOverTime);
		return m_bInitDev;
	}
	return FALSE;
}


////////////////////////////////////////////////////////////////////////////////////////////////
// 函数名称：CHTSDDeviceDll::Destroy
//
// 功能描述：清理资源
//
// 输入参数：void
// 输出参数：BOOL
// 编写人员：ROCY
// 编写时间：2016年7月23日
// 修改人员：
// 修改时间：
// 函数版本：1.0.0.1
// 备注说明：
//
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CHTSDDeviceDll::Destroy(void)
{
	if (m_bInitDll)
	{
		BOOL bRet = false;
		//bRet = fnIF_Destroy();

		if ( m_hModule )
		{
			::FreeLibrary( m_hModule );
		}
		m_hModule = NULL;
		m_bInitDll = false;
		return bRet;
	}
	return FALSE;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// 函数名称：CHTSDDeviceDll::GetLastErrorDesc
//
// 功能描述：获取上一次错误信息
//
// 输入参数：void
// 输出参数：CString
// 编写人员：ROCY
// 编写时间：2016年7月23日
// 修改人员：
// 修改时间：
// 函数版本：1.0.0.1
// 备注说明：
//
////////////////////////////////////////////////////////////////////////////////////////////////
CString CHTSDDeviceDll::GetLastErrorDesc(void)
{
	if (m_bInitDll)
		//return fnIF_GetLastErrorDesc();
	return _T("");
}

////////////////////////////////////////////////////////////////////////////////////////////////
// 函数名称：CHTSDDeviceDll::StartGetData
//
// 功能描述：执行启动获取数据操作
//
// 输入参数：void
// 输出参数：BOOL
// 编写人员：ROCY
// 编写时间：2016年7月23日
// 修改人员：
// 修改时间：
// 函数版本：1.0.0.1
// 备注说明：
//
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CHTSDDeviceDll::StartGetData(IN const int& iOvertime)
{
	//if (m_bInitDll && m_bInitDev)
		//return fnIF_StartGetData(iOvertime);
	return FALSE;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// 函数名称：CHTSDDeviceDll::StopGetData
//
// 功能描述：执行停止获取数据操作
//
// 输入参数：void
// 输出参数：BOOL
// 编写人员：ROCY
// 编写时间：2016年7月23日
// 修改人员：
// 修改时间：
// 函数版本：1.0.0.1
// 备注说明：
//
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CHTSDDeviceDll::StopGetData(IN const int& iChangeMMI)
{
	//if (m_bInitDll && m_bInitDev)
		//return fnIF_StopGetData(iChangeMMI);
	return FALSE;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// 函数名称：CHTSDDeviceDll::CheckDeviceStatus
//
// 功能描述：执行检测硬件状态操作
//
// 输入参数：IN OUT DWORD& dwState
// 输出参数：BOOL
// 编写人员：ROCY
// 编写时间：2016年7月23日
// 修改人员：
// 修改时间：
// 函数版本：1.0.0.1
// 备注说明：
//
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CHTSDDeviceDll::CheckDeviceStatus(IN OUT DWORD& dwState)
{
	bool bRet = false;
	if (m_bInitDll && m_bInitDev)
	{
		unsigned int Status = 0;
		bRet = fnIF_GetDevStatus(Status);
		dwState = Status;
	}
	return bRet;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// 函数名称：CHTSDDeviceDll::GetData
//
// 功能描述：获取数据结果
//
// 输入参数：IN OUT CString& strData
//         ：IN OUT int& iDataType
//         ：IN OUT int& iSrcType
// 输出参数：BOOL
// 编写人员：ROCY
// 编写时间：2016年7月23日
// 修改人员：
// 修改时间：
// 函数版本：1.0.0.1
// 备注说明：
//
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CHTSDDeviceDll::GetData(IN OUT CString& strData, IN OUT int& iDataType, IN OUT int& iSrcType)
{
	if (m_bInitDll && m_bInitDev)
		return fnIF_GetData(strData, iDataType, iSrcType);
	return FALSE;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// 函数名称：CHTSDDeviceDll::Authority
//
// 功能描述：权限认证操作
//
// 输入参数：IN const CString& strData
// 输出参数：BOOL
// 编写人员：ROCY
// 编写时间：2016年8月4日
// 修改人员：
// 修改时间：
// 函数版本：1.0.0.1
// 备注说明：
//
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CHTSDDeviceDll::Authority(IN const CString& strData)
{
	if (m_bInitDll && m_bInitDev)
		//return fnIF_Authority(strData);
	return FALSE;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// 函数名称：CHTSDDeviceDll::GetDeviceFirewareVer
//
// 功能描述：获取固件版本信息
//
// 输入参数：IN OUT CString& strXML
// 输出参数：BOOL
// 编写人员：ROCY
// 编写时间：2016年8月4日
// 修改人员：
// 修改时间：
// 函数版本：1.0.0.1
// 备注说明：
//
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CHTSDDeviceDll::GetDeviceFirewareVer(IN OUT CString& strXML)
{
	//if (m_bInitDll && m_bInitDev)
		//return fnIF_GetDeviceFirewareVer(strXML);
	return FALSE;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// 函数名称：CHTSDDeviceDll::GetDisplayContext
//
// 功能描述：获取显示内容信息
//
// 输入参数：IN OUT int& iID
//         ：CString& strXML
// 输出参数：BOOL
// 编写人员：ROCY
// 编写时间：2016年8月4日
// 修改人员：
// 修改时间：
// 函数版本：1.0.0.1
// 备注说明：
//
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CHTSDDeviceDll::GetDisplayContext(IN OUT int& iID, CString& strXML)
{
	if (m_bInitDll && m_bInitDev)
		//return fnIF_GetDisplayContext(iID, strXML);
	return FALSE;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// 函数名称：CHTSDDeviceDll::SetDisplayContext
//
// 功能描述：设置显示内容信息
//
// 输入参数：IN const int& iID
//         ：IN const CString& strXML
// 输出参数：BOOL
// 编写人员：ROCY
// 编写时间：2016年8月4日
// 修改人员：
// 修改时间：
// 函数版本：1.0.0.1
// 备注说明：
//
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CHTSDDeviceDll::SetDisplayContext(IN const int& iID, IN const CString& strXML)
{
	if (m_bInitDll && m_bInitDev)
	{
		//return fnIF_SetDisplayContext(iID, strXML); //此处改为调用EtcEvent串口库中的打开扫码头
		int iFeeValue = 0;
		return fnIF_StartScan(iFeeValue);
	}

	return FALSE;
}


BOOL CHTSDDeviceDll::QRcodeDisplay(const CString& strJson)
{
	if (m_bInitDll && fnIF_Tr500QRcodeDisplay != NULL)
	{
		CString param = strJson;
		fnIF_Tr500QRcodeDisplay(param.GetBuffer(0), param.GetLength());
		param.ReleaseBuffer();
		return true;
	}
	return false;
}

BOOL CHTSDDeviceDll::FeeDisplay(const CString& strJson)
{
	if (m_bInitDll && fnIF_Tr500FeeDisplay != NULL)
	{
		CString param = strJson;
		fnIF_Tr500FeeDisplay(param.GetBuffer(0), param.GetLength());
		param.ReleaseBuffer();
		return true;
	}
	return false;
}

BOOL CHTSDDeviceDll::PayResultDisplay(const CString& strJson)
{
	if (m_bInitDll && fnIF_Tr500PayResultDisplay != NULL)
	{
		CString param = strJson;
		fnIF_Tr500PayResultDisplay(param.GetBuffer(0), param.GetLength());
		param.ReleaseBuffer();
		return true;
	}
	return false;
}

BOOL CHTSDDeviceDll::ClearScreen(const CString& strJson)
{
	if (m_bInitDll && fnIF_Tr500ClearScreen != NULL)
	{
		CString param = strJson;
		fnIF_Tr500ClearScreen(param.GetBuffer(0), param.GetLength());
		param.ReleaseBuffer();
		return true;
	}
	return false;
}


////////////////////////////////////////////////////////////////////////////////////////////////
// 函数名称：CHTSDDeviceDll::IssuedData
//
// 功能描述：执行数据下发操作
//
// 输入参数：IN const CString& strXML
// 输出参数：BOOL
// 编写人员：ROCY
// 编写时间：2016年7月23日
// 修改人员：
// 修改时间：
// 函数版本：1.0.0.1
// 备注说明：
//
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CHTSDDeviceDll::IssuedData(IN const CString& strXML)
{
	//if (m_bInitDll && m_bInitDev)
		//return fnIF_IssuedData(strXML);
	return FALSE;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// 函数名称：SetIOState
//
// 功能描述：执行控制IO操作
//
// 输入参数：IN const int& iIOState
// 输出参数：BOOL
// 编写人员：ROCY
// 编写时间：2016年7月23日
// 修改人员：
// 修改时间：
// 函数版本：1.0.0.1
// 备注说明：
//
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CHTSDDeviceDll::SetIOState(IN const int& iIOState)
{
	//if (m_bInitDll && m_bInitDev)
		//return fnIF_SetIOState(iIOState);
	return FALSE;
}

BOOL CHTSDDeviceDll::UpgradeFirware(const CString& strJson)
{
	//if (m_bInitDll && fnIF_UpgradeFirware != NULL)
		//return fnIF_UpgradeFirware(strJson);
	return FALSE;
}
BOOL CHTSDDeviceDll::IsUpgrading()
{
	//if (m_bInitDll && fnIF_IsUpgrading != NULL)
		//return fnIF_IsUpgrading();
	return FALSE;
}

BOOL CHTSDDeviceDll::RealFirmwareVer(IN OUT CString& strXML)
{
	//if (m_bInitDll && fnIF_RealFirmwareVer != NULL)
		//return fnIF_RealFirmwareVer(strXML);
	return false;
}

BOOL CHTSDDeviceDll::RealSlaveFirmwareVer(IN OUT CString& strXML)
{
	//if (m_bInitDll && fnIF_RealSlaveFirmwareVer != NULL)
		//return fnIF_RealSlaveFirmwareVer(strXML);
	return false;
}

BOOL CHTSDDeviceDll::QueryLEDVersion(OUT CString& strJson)
{
	//if (m_bInitDll && fnIF_QueryLEDVersion != NULL)
		//return fnIF_QueryLEDVersion(strJson);
	return false;
}

BOOL CHTSDDeviceDll::TransferUserDefinedData(const CString& strJson)
{
	//if (m_bInitDll && fnIF_UserDefinedData != NULL)
		//return fnIF_UserDefinedData(strJson);
	return false;
}
