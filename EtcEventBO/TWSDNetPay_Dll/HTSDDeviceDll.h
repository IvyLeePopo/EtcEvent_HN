/*
 * Copyright(C) 2016,SQUIRREL 个人工作室 保留所有权利。( All rights reserved. )
 * 
 * 文件名称：HTSDDeviceDll.h
 * 摘    要：智能硬件接口动态库加载类，声明文件
 * 当前版本：1.0.0.1
 * 作    者：ROCY
 * 创建日期：2016年7月24日
 */
#pragma once

class CHTSDDeviceDll
{
//函数定义
public:
	//初始化硬件
	//typedef BOOL	(WINAPI  *defIF_InitDev)(IN const UINT& nThreadID,IN const HWND& hWnd,IN const UINT& nNotifyMsgID,IN const int& iProvinceID,IN const int& iOverTime);
	//清理资源
	typedef BOOL	(WINAPI  *defIF_Destroy)(void);
	//获取上一次错误信息
	//typedef CString (WINAPI  *defIF_GetLastErrorDesc)(void);
	//执行启动获取数据操作
	typedef BOOL	(WINAPI  *defIF_StartGetData)(IN const int& iOvertime);
	//执行停止获取数据操作
	typedef BOOL	(WINAPI  *defIF_StopGetData)(IN const int& iChangeMMI);
	//执行检测硬件状态操作
	typedef BOOL	(WINAPI  *defIF_CheckDeviceStatus)(IN OUT DWORD& dwState);
	//获取数据结果
	//typedef BOOL	(WINAPI  *defIF_GetData)(IN OUT CString& strData,IN OUT int& iDataType,IN OUT int& iSrcType);
	//权限认证操作
	typedef	BOOL	(WINAPI  *defIF_Authority)(IN const CString& strData);
	//获取固件版本信息
	typedef	BOOL	(WINAPI  *defIF_GetDeviceFirewareVer)(IN OUT CString& strXML);
	//获取显示内容信息
	typedef	BOOL	(WINAPI  *defIF_GetDisplayContext)(IN OUT int&, CString&);
	//设置显示内容信息
	typedef	BOOL	(WINAPI  *defIF_SetDisplayContext)(IN const int&,IN const CString& strXML);
	//执行数据下发操作
	typedef BOOL	(WINAPI  *defIF_IssuedData)(IN const CString& strXML);
	//执行控制IO操作
	typedef BOOL	(WINAPI  *defIF_SetIOState)(IN const int& iIOState);


	//add 2017-12
	typedef BOOL	(WINAPI  *defIF_UpgradeFirware)(IN const CString& strJson);
	typedef BOOL	(WINAPI  *defIF_IsUpgrading)();
	typedef	BOOL	(WINAPI  *defIF_RealFirmwareVer)(IN OUT CString& strXML);
	typedef	BOOL	(WINAPI  *defIF_RealSlaveFirmwareVer)(IN OUT CString& strXML);

	//add 2018-07
	typedef BOOL (WINAPI* defIF_QueryLEDVersion)(OUT CString& strJson);

	typedef BOOL (WINAPI* defIF_UserDefinedData)(const  CString& strJson);

	/***********************************对接ETC特情处理硬件库***********************************************************************************/
	//初始化设备
	typedef bool (WINAPI  *defIF_InitDev)(IN const UINT& dwThreadNotifyCallerID, IN const UINT& dwThreadManagerID, IN const HWND& hWnd, IN const UINT& dwEtcRespMsgID, IN const UINT& dwDevNotifyMsgID, IN OUT void* pIndPtr);
	//设置设备初始化数据
	typedef bool (WINAPI  *defIF_SetDevInitData)(IN const char* szParamContext, IN const int& iParamSize);	
	//清理资源
	typedef bool (WINAPI  *defIF_DeInitDev)(void);
	//获取上一次错误信息
	typedef const char* (WINAPI  *defIF_GetLastErrorDesc)(void);
	//获取组件状态
	typedef bool (WINAPI  *defIF_GetDevStatus)(IN OUT unsigned int& uStatus);
	//特情处理开始
	typedef bool (WINAPI  *defIF_EtcEventDealStart)(IN const char* szParamContext, IN const int& iParamSize);
	//车辆队列确认
	typedef bool (WINAPI  *defIF_CheckVehQueue)(IN const char* szParamContext, IN const int& iParamSize);
	//车辆信息确认
	typedef bool (WINAPI  *defIF_CheckVehInfo)(IN const char* szParamContext,IN const int& iParamSize);
	//入口信息确认
	typedef bool (WINAPI  *defIF_CheckEntryInfo)(IN const char* szParamContext,IN const int& iParamSize);
	//费额显示
	typedef bool (WINAPI  *defIF_ShowFeeInfo)(IN const char* szParamContext,IN const int& iParamSize);
	//支付状态显示
	typedef bool (WINAPI  *defIF_PayResultDisplay)(IN const char* szParamContext,IN const int& iParamSize);
	//特情处理结束
	typedef bool (WINAPI  *defIF_EtcEventDealStop)(IN const char* szParamContext,IN const int& iParamSize);
	//启动扫码
	typedef bool (WINAPI  *defIF_StartScan)(IN const int& iFeeValue);
	//停止扫码
	typedef bool (WINAPI  *defIF_StopScan)();
	//ETC卡操作提示
	typedef bool (WINAPI  *defIF_CardOperationNotify)(IN const char* szParamContext, IN const int& iParamSize);
	//车辆队列修改
	typedef bool (WINAPI  *defIF_ModifyVehQueue)(IN const char* szParamContext, IN const int& iParamSize);
	//获取二维码数据
	typedef bool (WINAPI *defIF_GetData)(IN OUT CString& strData, IN OUT int& iDataType, IN OUT int& iSrcType);
	//费额授权
	typedef bool (WINAPI *defIF_FeeAuthorize)(IN const char* szParamContext, IN const int& iParamSize);
	//自由语音播报
	typedef bool (WINAPI *defIF_FreeVoicePlay)(IN const char* szParamContext, IN const int& iParamSize);
	//其他业务授权
	typedef bool (WINAPI * defIF_EventAuthorize)(IN const char* szParamContext, IN const int& iParamSize);
	//删除车辆队列结果
	typedef bool (WINAPI * defIF_DelVehQueueResult)(IN const char* szParamContext, IN const int& iParamSize);
	//二维码显示接口定义
	typedef bool (WINAPI *defIF_Tr500QRcodeDisplay)(IN const char* strJsonParam, IN const int& iJsonParamSize);
	//费额显示接口定义
	typedef bool (WINAPI *defIF_Tr500FeeDisplay)(IN const char* strJsonParam, IN const int& iJsonParamSize);
	//支付结果显示接口定义
	typedef bool (WINAPI *defIF_Tr500PayResultDisplay)(IN const char* strJsonParam, IN const int& iJsonParamSize);
	//清屏接口定义
	typedef bool (WINAPI *defIF_Tr500ClearScreen)(IN const char* strJsonParam, IN const int& iJsonParamSize);
	
	//websocket通讯数据
	typedef bool (WINAPI *defIF_WebsocketControl)(IN const char* strJsonParam, IN const int& iJsonParamSize);

public:
	CHTSDDeviceDll(void);
	~CHTSDDeviceDll(void);
public:
	//初始化动态库
	const BOOL Init(IN const CString& strDllFilePath);
	//获取动态库加载状态
	inline const BOOL GetDllInitState(void){return m_bInitDll;}
	//获取组件初始化状态
	inline const BOOL GetDevInitState(void){return m_bInitDev;}
//Operations
public:
	//组件初始化
	BOOL InitDev(IN const UINT& nThreadID,IN const HWND& hWnd,IN const unsigned int& nNotifyMsgID,IN const int& iProvinceID,IN const int& iOverTime);
	//清理资源
	BOOL Destroy(void);
	//获取上一次错误信息
	CString GetLastErrorDesc(void);
	//执行启动获取数据操作
	BOOL StartGetData(IN const int& iOvertime);
	//执行停止获取数据操作
	BOOL StopGetData(IN const int& iChangeMMI = 0);
	//执行检测硬件状态操作
	BOOL CheckDeviceStatus(IN OUT DWORD& dwState);
	//获取数据结果
	BOOL GetData(IN OUT CString& strData,IN OUT int& iDataType,IN OUT int& iSrcType);
	//权限认证操作
	BOOL Authority(IN const CString& strData);
	//获取固件版本信息
	BOOL GetDeviceFirewareVer(IN OUT CString& strXML);
	//获取显示内容信息
	BOOL GetDisplayContext(IN OUT int& iID, CString& strXML);
	//设置显示内容信息
	BOOL SetDisplayContext(IN const int& iID,IN const CString& strXML);
	//执行数据下发操作
	BOOL IssuedData(IN const CString& strXML);
	//执行控制IO操作
	BOOL SetIOState(IN const int& iIOState);


	BOOL UpgradeFirware(const CString& strJson);
	BOOL IsUpgrading();

	BOOL RealFirmwareVer(IN OUT CString& strXML);

	BOOL RealSlaveFirmwareVer(IN OUT CString& strXML);
	BOOL QueryLEDVersion(OUT CString& strJson);
	BOOL TransferUserDefinedData(const CString& strJson);

	BOOL QRcodeDisplay(const CString& strJson);
	BOOL FeeDisplay(const CString& strJson);
	BOOL PayResultDisplay(const CString& strJson);
	BOOL ClearScreen(const CString& strJson);
	BOOL WebsocketControl(const CString& strJson);

//Attributes
private:
	//动态库加载句柄
	HMODULE	m_hModule;
	//动态库初始化标记
	BOOL	m_bInitDll;

	//错误说明
	CString m_strLastError;
	//函数定义变量
	/*
	defIF_InitDev				fnIF_InitDev;
	defIF_Destroy				fnIF_Destroy;
	defIF_GetLastErrorDesc		fnIF_GetLastErrorDesc;
	defIF_StartGetData			fnIF_StartGetData;
	defIF_StopGetData			fnIF_StopGetData;
	defIF_CheckDeviceStatus		fnIF_CheckDeviceStatus;
	defIF_GetData				fnIF_GetData;
	defIF_Authority				fnIF_Authority;
	defIF_GetDeviceFirewareVer	fnIF_GetDeviceFirewareVer;
	defIF_GetDisplayContext		fnIF_GetDisplayContext;
	defIF_SetDisplayContext		fnIF_SetDisplayContext;
	defIF_IssuedData			fnIF_IssuedData;
	defIF_SetIOState			fnIF_SetIOState;

	defIF_UpgradeFirware        fnIF_UpgradeFirware;
	defIF_IsUpgrading           fnIF_IsUpgrading;

	defIF_RealFirmwareVer       fnIF_RealFirmwareVer;
	defIF_RealSlaveFirmwareVer  fnIF_RealSlaveFirmwareVer;
	defIF_QueryLEDVersion       fnIF_QueryLEDVersion;
	defIF_UserDefinedData       fnIF_UserDefinedData;
*/
public:
	defIF_InitDev				fnIF_InitDev;
	defIF_SetDevInitData		fnIF_SetDevInitData;
	defIF_DeInitDev				fnIF_DeInitDev;
	defIF_GetLastErrorDesc      fnIF_GetLastErrorDesc;
	defIF_GetDevStatus			fnIF_GetDevStatus;
	defIF_EtcEventDealStart		fnIF_EtcEventDealStart;
	defIF_CheckVehQueue			fnIF_CheckVehQueue;
	defIF_CheckVehInfo			fnIF_CheckVehInfo;
	defIF_CheckEntryInfo		fnIF_CheckEntryInfo;
	defIF_ShowFeeInfo			fnIF_ShowFeeInfo;
	defIF_PayResultDisplay		fnIF_PayResultDisplay;
	defIF_EtcEventDealStop		fnIF_EtcEventDealStop;
	defIF_StartScan				fnIF_StartScan;
	defIF_StopScan				fnIF_StopScan;
	defIF_CardOperationNotify   fnIF_CardOperationNotify;
	defIF_ModifyVehQueue		fnIF_ModifyVehQueue;
	defIF_GetData				fnIF_GetData;
	defIF_FeeAuthorize			fnIF_FeeAuthorize;
	defIF_FreeVoicePlay			fnIF_FreeVoicePlay;
	defIF_EventAuthorize		fnIF_EventAuthorize;
	defIF_DelVehQueueResult		fnIF_DelVehQueueResult;

	defIF_Tr500QRcodeDisplay	fnIF_Tr500QRcodeDisplay;
	defIF_Tr500FeeDisplay		fnIF_Tr500FeeDisplay;
	defIF_Tr500PayResultDisplay	fnIF_Tr500PayResultDisplay;
	defIF_Tr500ClearScreen		fnIF_Tr500ClearScreen;

	defIF_WebsocketControl		fnIF_WebsocketControl;

	//组件初始化标记
	BOOL	m_bInitDev;

};
