/*
 * Copyright(C) 北京特微智能 保留所有权利。( All rights reserved. )
 *
 * 文件名称：ThreadSMManager.h
 * 摘    要：扫码器管理线程类，声明文件
 * 当前版本：1.0.0.0
 * 作    者：朱道明
 * 创建日期：2020年10月18日
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

/********************ETC特情专用接口***********************************************************/
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
//重载函数
protected:
	//清除本类申请的资源
	void fnReleaseResource();
	
protected:
	DECLARE_MESSAGE_MAP()

	/*******************************内部被动消息触发处理函数*******************************/
	//内部被动消息触发处理函数 -- 获取到串口数据
	afx_msg void OnMsgReceiveDataFromCOM(WPARAM wParam, LPARAM lParam);
	//通知缓冲区的数据已经发送完毕
	afx_msg void OnMsgDataSendEnd(WPARAM wParam, LPARAM lParam);
	//通知调用者串口发生错误
	afx_msg void OnMsgComError(WPARAM wParam, LPARAM lParam);

private:
	//物理端口操作函数 -- 打开串口连接
	BOOL	OpenCom(void);
	//物理端口操作函数 -- 往串口写数据，字节流形式
	DWORD	WriteDataToComm(IN const LPBYTE byData, IN const DWORD& dwSize);

public:
	//物理端口操作函数 -- 关闭串口连接
	BOOL	CloseCom(void);
	//串口打开标记
	BOOL	m_bOpen;
	int     m_DevStatus; //0：正常，4：硬件通讯故障
};