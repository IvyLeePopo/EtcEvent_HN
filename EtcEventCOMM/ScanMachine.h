// ScanMachine.h : ScanMachine DLL 的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号
#include "ThreadSMManager.h"


// CScanMachineApp
// 有关此类实现的信息，请参阅 ScanMachine.cpp
//


class CScanMachineApp : public CWinApp
{
public:
	CScanMachineApp();

// 重写
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

public:
	/*****************************ETC特情专用接口**********************************************************/
	//初始化设备
	static bool WINAPI  IF_InitDev(IN const UINT& dwThreadNotifyCallerID, IN const UINT& dwThreadManagerID, IN const HWND& hWnd, IN const UINT& dwEtcRespMsgID, IN const UINT& dwDevNotifyMsgID, IN OUT void* pIndPtr);
	//清理资源
	static bool WINAPI  IF_DeInitDev(void);
	//获取上一次错误信息
	static const char* WINAPI IF_GetLastErrorDesc(void);
	//获取组件状态
	static bool WINAPI IF_GetDevStatus(IN OUT unsigned int& uStatus);
	//设置设备初始化数据
	static bool WINAPI IF_SetDevInitData(IN const char* szParamContext, IN const int& iParamSize);
	//特情处理开始
	static bool WINAPI IF_EtcEventDealStart(IN const char* szParamContext, IN const int& iParamSize);
	//车辆队列确认
	static bool WINAPI IF_CheckVehQueue(IN const char* szParamContext, IN const int& iParamSize);
	//车辆信息确认
	static bool WINAPI IF_CheckVehInfo(IN const char* szParamContext,IN const int& iParamSize);
	//入口信息确认
	static bool WINAPI IF_CheckEntryInfo(IN const char* szParamContext,IN const int& iParamSize);
	//费额显示
	static bool WINAPI IF_ShowFeeInfo(IN const char* szParamContext,IN const int& iParamSize);
	//支付状态显示
	static bool WINAPI IF_PayResultDisplay(IN const char* szParamContext,IN const int& iParamSize);
	//特情处理结束
	static bool WINAPI IF_EtcEventDealStop(IN const char* szParamContext,IN const int& iParamSize);
	//启动扫码
	static bool WINAPI IF_StartScan(IN const int& iFeeValue);
	//停止扫码
	static bool WINAPI IF_StopScan();
	//ETC卡操作提示
	static bool WINAPI IF_CardOperationNotify(IN const char* szParamContext, IN const int& iParamSize);
	//车辆队列修改
	static bool WINAPI IF_ModifyVehQueue(IN const char* szParamContext, IN const int& iParamSize);
	//获取扫码数据
	static bool WINAPI IF_GetData(IN OUT CString& strData, IN OUT int& iDataType, IN OUT int& iSrcType);
	//最小费额授权
	static bool WINAPI IF_FeeAuthorize(IN const char* szParamContext, IN const int& iParamSize);
	//其他授权接口
	static bool WINAPI IF_EventAuthorize(IN const char* szParamContext, IN const int& iParamSize);
	//删除车辆对接结果
	static bool WINAPI IF_DelVehQueueResult(IN const char* szParamContext, IN const int& iParamSize);

	//二维码显示
	static bool WINAPI IF_Tr500QRcodeDisplay(IN const char* strJsonParam, IN const int& iJsonParamSize);
	//费额显示
	static bool WINAPI IF_Tr500FeeDisplay(IN const char* strJsonParam, IN const int& iJsonParamSize);
	//支付结果显示
	static bool WINAPI IF_Tr500PayResultDisplay(IN const char* strJsonParam, IN const int& iJsonParamSize);
	//清屏
	static bool WINAPI IF_Tr500ClearScreen(IN const char* strJsonParam, IN const int& iJsonParamSize);

	
	//websocket通讯数据
	static bool WINAPI IF_WebsocketControl(IN const char* strJsonParam, IN const int& iJsonParamSize);
	
	
	//ETC特情处理应答指针，在回掉通知的时候将应答内容传递出去
	static void* m_pIndPtr;
	static bool m_bIsDevInited;

	UINT m_ThreadNotifyCallerID;
	UINT m_EtcRespMsgID;

	UINT m_ThreadDevManagerID;
	UINT m_DevNotifyMsgID;

	std::string m_pA5InitJsonData;

private:
	//设备管理线程指针
	CThreadSMManager* m_pThreadSMManager;

public:
	CString GetCurrentDirNew();
	//BOOL	CreateDir(LPCTSTR lpzPath);
	void	RecordMsgTemp(IN LPCTSTR strMsg, IN const BOOL& bForce=TRUE);
	time_t	OleTimeToTime(DATE dTime);
	time_t	CString2time_t(LPCTSTR lpztime);
	CString FmtStr(const TCHAR* lpsFmt,...);
	void	Hex2Bin(IN LPCSTR lpHex,OUT PBYTE pBin,IN DWORD dwHex_len);
	CString Bin2Hex(IN PBYTE pBin,IN DWORD dwBin_Len);
	WORD	CRC16(LPBYTE lpData, WORD wLength);
	void CheckAndDeleteLogFile(void);

	//记录程序运行目录
	CString	m_strAppDir;	
	//记录程序运行日志目录路径
	CString	m_strLogDirPath;		
	//记录程序运行日志完整路径
	CString	m_strLogFilePath;
	//记录程序运行日志完整路径
	CString	m_strLogFileName;	
	//读取配置文件路径失败后，启用的默认配置文件路径（与动态库同目录）
	CString	m_strConfigPathFile;			
	//是否写日志标记
	int		m_iWriteLog;		
	//日志文件最大值
	int		m_iLogFileMaxSize;	
	//日志保存最大天数
	long long m_iDeleteLogSaveDays;
	//串口配置
	int m_SerialPort;
	int m_BaudRate;
	int m_InBufferSize;
	int m_OutBufferSize;

	CString m_strLastErrorDesc;

	CString m_QRcode;
	std::string m_DeviceNo;

	//EtcEvent应答通知的线程id和消息id

	

public:
	DECLARE_MESSAGE_MAP()
};

void LogCallBack(const std::string& content);
CScanMachineApp* GetGlobalApp();